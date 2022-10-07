#include "atlas_2106_09609.h"
#include <limits.h>
#include <float.h>
#include </home/krolb/tools/CheckMATE/ONNX/tools/onnxruntime-linux-x64-1.12.1/include/onnxruntime_cxx_api.h>
// AUTHOR: JSK
//  EMAIL: jsk@th.physik.uni-bonn.de
// NN AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl

void Atlas_2106_09609::initialize() {
  setAnalysisName("atlas_2106_09609");          
  setInformation(""
    "# ATLAS search for RPV susy in a final state containing leptons and many jets\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("SR1;SR2;SR3;SR4;SR5;SR6;SR7;SR8;SR9;SR10;SR11;SR12;SR13;SR14;SR15;SR16;SR17;SR18;SR19;SR20;SR21");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  
  Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "EWK_SR");
  Ort::SessionOptions session_options;
  session_4j = new Ort::Session(env, "/home/krolb/tools/CheckMATE/ONNX/data/SUSY-2019-04_4jets.onnx", session_options);
  
  OrtAllocator* allocator;
//  char* name = session_4j->GetInputName(0, allocator);
//  cout << name;
  size_t ninp = session_4j->GetInputCount();
  cout << ninp;
  /*std::vector<std::string> input_names = session_4j->GetInputNames();
  std::vector<std::vector<int64_t> > input_shapes = session_4j->GetInputShapes();
  cout << "Input Node Name/Shape (" << input_names.size() << "):" << endl;
  for (size_t i = 0; i < input_names.size(); i++) {
    cout << "\t" << input_names[i] << " : " << print_shape(input_shapes[i]) << endl;
  }
  
  std::vector<std::string> output_names = session_4j->GetOutputNames();
  std::vector<std::vector<int64_t> > output_shapes = session_4j->GetOutputShapes();
  cout << "Output Node Name/Shape (" << output_names.size() << "):" << endl;
  for (size_t i = 0; i < output_names.size(); i++) {
    cout << "\t" << output_names[i] << " : " << print_shape(output_shapes[i]) << endl;
  } */ 
  
}

void Atlas_2106_09609::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 15., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5);
  //auto storeOriginalJets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  jets = filterPhaseSpace(jets, 20., -2.5, 2.5);

  std::vector<Jet*> bjets;
  std::vector<Jet*> nonbjets;
  for (int i = 0; i < jets.size(); i++)
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i]) ) 
      bjets.push_back(jets[i]);
    else nonbjets.push_back(jets[i]);    
  
  electronsTight = overlapRemoval(electronsTight, muonsCombined, 0.01);
  nonbjets = overlapRemoval(nonbjets, electronsMedium, 0.2);
  nonbjets = overlapRemoval_muon_jet_tracks(nonbjets, muonsCombined, 0.4, 2); 

  std::vector<Jet*> sigjets = nonbjets;
  sigjets.insert(sigjets.end(), bjets.begin(), bjets.end());
  std::sort(sigjets.begin(), sigjets.end(), Atlas_2106_09609::jetsortByPT );     
  
  electronsMedium = specialoverlap(electronsMedium, sigjets);
  electronsTight = specialoverlap(electronsTight, sigjets);
  muonsCombined = specialoverlap(muonsCombined, sigjets); 
  
  muonsCombined = filterPhaseSpace(muonsCombined, 15., -2.7, 2.7);
  auto storeOriginalJets = sigjets;
  auto storeOriginalBJets = bjets;  

  std::vector<FinalStateObject*> leptons;
  for(int e = 0; e < electronsTight.size(); e++) {
    FinalStateObject* lep = newFinalStateObject(electronsTight[e]);
    leptons.push_back(lep);
  }
  for(int m = 0; m < muonsCombined.size(); m++) {
    FinalStateObject* lep = newFinalStateObject(muonsCombined[m]);
    leptons.push_back(lep);
  }
  
  std::sort( leptons.begin(), leptons.end(), sortByPT );
  /*
  electronsTight=removeLowMassResonances(electronsTight, 81.2, 101.2);
  muonsCombined=removeLowMassResonances(muonsCombined, 81.2, 101.2);
  */

  double mlj=std::numeric_limits<double>::max(), tempMlj, mlj1, mlj2;
  double mee,mmumu;
  
  if(leptons.size() == 0)
    return;

  countCutflowEvent("01_nLeptons>0");
  
  if(leptons[0]->PT < 27)
    return;

  countCutflowEvent("02_leadLep_PT>27");
  
  bool twoLeptonSC = false;
  bool oneLepton = false;
  bool oneLeptonSF = false;
  
  //2 lepton same charge SR
  if(leptons.size() == 2 && leptons[0]->Charge * leptons[1]->Charge > 0)
    {
      //JJJ: the following conditon cannot be applied since we do not have misidentified charges for leptons
      /*
      if(electronsTight.size() == 2)
	{
	  TLorentzVector pair = ( electronsTight[0]->P4() + electronsTight[1]->P4() );
	  mee = pair.M();
	  if ( fabs( mee-91.2) > 10.)
	    {
	      twoLeptonSC = true;
	    }
	}
      */
      twoLeptonSC = true;
    }

  //JJJ: this is just used for BKG estimation
  if(false){
    if(leptons.size() >= 3 && bjets.size() == 0 && ( leptons[0]->Charge * leptons[1]->Charge > 0 ||
						     leptons[0]->Charge * leptons[2]->Charge > 0 ||
						     leptons[1]->Charge * leptons[2]->Charge > 0) )
      {
	if(electronsTight.size() == 2)
	  {
	    TLorentzVector pair = ( electronsTight[0]->P4() + electronsTight[1]->P4() );
	    mee = pair.M();
	    if ( fabs( mee-91.2) < 10.)
	      {
		twoLeptonSC = true;
	      }
	  }
	if(muonsCombined.size() == 2)
	  {
	    TLorentzVector pair = ( muonsCombined[0]->P4() + muonsCombined[1]->P4() );
	    mmumu = pair.M();
	    if ( fabs(mmumu-91.2) < 10.)
	      {
		twoLeptonSC = true;
	      }
	  }    
      }
  }
  
  if( twoLeptonSC )
    {
      for(int i=0; i < sigjets.size(); ++i)
	{
	  for(int j=i+1; j < sigjets.size(); ++j)
	    {
	      TLorentzVector pair = leptons[0]->P4() + sigjets[i]->P4();
	      mlj1=pair.M();
	      pair=leptons[0]->P4() + sigjets[j]->P4();
	      mlj2=pair.M();
	      tempMlj=std::max(mlj1,mlj2);
	      if(tempMlj < mlj)
		mlj = tempMlj;
	    }
	}
	
    }

  if( !twoLeptonSC )
    {
      oneLepton = true;
      if(electronsTight.size() == 2)
	{
	  TLorentzVector pair = ( electronsTight[0]->P4() + electronsTight[1]->P4() );
	  mee = pair.M();
	  if ( fabs( mee-91.2) < 10.)
	    {
	      oneLeptonSF = true;
	    }
	}
      if(muonsCombined.size() == 2)
	{
	  TLorentzVector pair = ( muonsCombined[0]->P4() + muonsCombined[1]->P4() );
	  mmumu = pair.M();
	  if ( fabs(mmumu-91.2) < 10.)
	    {
	      oneLeptonSF = true;
	    }
	}    
      
    }
  
  if( !oneLeptonSF && !oneLepton && !twoLeptonSC )
    return;

  if(oneLeptonSF || oneLepton)
      countCutflowEvent("03_1l event");

  if(twoLeptonSC)
      countCutflowEvent("04_2lSC event");

  
  countCutflowEvent("05_no 1l or 2lSC candidates");

  if ( oneLepton ) {
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_1l_pT>20l_06j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>20_>2b_06j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_1l_pT>20_08j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>20_>2b_08j");
    }
    if ( sigjets.size() >=10 ) {
      countCutflowEvent("07_1l_pT>20_10j"); 
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>20_>2b_10j");
    }
    if ( sigjets.size() >=11 ) {
      countCutflowEvent("07_1l_pT>20_11j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>20_>2b_11j");
    }  
    if ( sigjets.size() >=12 ) {
      countCutflowEvent("07_1l_pT>20_12j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>20_>2b_12j");
    } 
    if ( sigjets.size() >=15 ) {
      countCutflowEvent("07_1l_pT>20_15j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>20_>2b_15j");
    } 
    
  
    if(sigjets.size() == 4 ) {
      countCutflowEvent("06_EW_1l_4j");
      if ( bjets.size() >= 4) countCutflowEvent("06_EW_1l_4b_4j");
    }
    if(sigjets.size() == 5) {
      countCutflowEvent("06_EW_1l_5j");
      if ( bjets.size() >= 4) countCutflowEvent("06_EW_1l_4b_5j");
    }
    if(sigjets.size() == 6) {
      countCutflowEvent("06_EW_1l_6j");
      if ( bjets.size() >= 4) countCutflowEvent("06_EW_1l_4b_6j");
    }
    if(sigjets.size() == 7) {
      countCutflowEvent("06_EW_1l_7j");
      if ( bjets.size() >= 4) countCutflowEvent("06_EW_1l_4b_7j");
    }
    if(sigjets.size() == 8) {
      countCutflowEvent("06_EW_1l_8j");
      if ( bjets.size() >= 4) countCutflowEvent("06_EW_1l_4b_8j"); 
    }
                                       
  
    if(sigjets.size() >= 15 && oneLepton && bjets.size() == 0)
      countSignalEvent("SR1");

    if(sigjets.size() >= 15 && oneLepton && bjets.size() >= 3)
      countSignalEvent("SR2");

    sigjets = filterPhaseSpace(sigjets, 40., -2.5, 2.5);
    bjets = filterPhaseSpace(bjets, 40., -2.5, 2.5);
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_1l_pT>40_06j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>40_>2b_06j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_1l_pT>40_08j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>40_>2b_08j");
    }
    if ( sigjets.size() >=10 ) {
      countCutflowEvent("07_1l_pT>40_10j"); 
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>40_>2b_10j");
    }
    if ( sigjets.size() >=11 ) {
      countCutflowEvent("07_1l_pT>40_11j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>40_>2b_11j");
    }  
    if ( sigjets.size() >=12 ) {
      countCutflowEvent("07_1l_pT>40_12j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>40_>2b_12j");
    } 

    if(sigjets.size() >= 12 && oneLepton && bjets.size() == 0)
      countSignalEvent("SR3");

    if(sigjets.size() >= 12 && oneLepton && bjets.size() >= 3)
      countSignalEvent("SR4");
  
    sigjets = filterPhaseSpace(sigjets, 60., -2.5, 2.5);
    bjets = filterPhaseSpace(bjets, 60., -2.5, 2.5);
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_1l_pT>60_06j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>60_>2b_06j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_1l_pT>60_08j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>60_>2b_08j");
    }
    if ( sigjets.size() >=10 ) {
      countCutflowEvent("07_1l_pT>60_10j"); 
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>60_>2b_10j");
    }
    if ( sigjets.size() >=11 ) {
      countCutflowEvent("07_1l_pT>60_11j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>60_>2b_11j");
    }      

    if(sigjets.size() >= 11 && oneLepton && bjets.size() == 0)
      countSignalEvent("SR5");

    if(sigjets.size() >= 11 && oneLepton && bjets.size() >= 3)
      countSignalEvent("SR6");

    sigjets = filterPhaseSpace(sigjets, 80., -2.5, 2.5);
    bjets = filterPhaseSpace(bjets, 80., -2.5, 2.5);
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_1l_pT>80_06j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>80_>2b_06j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_1l_pT>80_08j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>80_>2b_08j");
    }
    if ( sigjets.size() >=10 ) {
      countCutflowEvent("07_1l_pT>80_10j"); 
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>80_>2b_10j");
    }

    if(sigjets.size() >= 10 && oneLepton && bjets.size() == 0)
      countSignalEvent("SR7");

    if(sigjets.size() >= 10 && oneLepton && bjets.size() >= 3)
      countSignalEvent("SR8");

    sigjets = filterPhaseSpace(sigjets, 100., -2.5, 2.5);
    bjets = filterPhaseSpace(bjets, 100., -2.5, 2.5);
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_1l_pT>100_06j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>100_>2b_06j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_1l_pT>100_08j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>100_>2b_08j");
    }

    if(sigjets.size() >= 8 && oneLepton && bjets.size() == 0)
      countSignalEvent("SR9");

    if(sigjets.size() >= 8 && oneLepton && bjets.size() >= 3)
      countSignalEvent("SR10");
  }

  sigjets = storeOriginalJets;
  bjets = storeOriginalBJets;

  if ( twoLeptonSC ) {
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_2lSC_pT>20_6j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>20_>2b_6j");
    }
    if ( sigjets.size() >=7 ) {
      countCutflowEvent("07_2lSC_pT>20_7j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>20_>2b_7j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_2lSC_pT>20_8j"); 
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>20_>2b_8j");
    }
    if ( sigjets.size() >=10 ) {
      countCutflowEvent("07_2lSC_pT>20_10j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>20_>2b_10j");
    }
    
    if (mlj < 155. and sigjets.size() >= 4 and sigjets.size() < 9 ) {
      countCutflowEvent("07_EW_2lSC_" + std::to_string(sigjets.size()) + "j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_EW_2lSC_>2b_" + std::to_string(sigjets.size()) + "j");
    }
  
    if(sigjets.size() == 6 && bjets.size() >= 3 && mlj < 155.)
      countSignalEvent("SR11");
  
    if(sigjets.size() >= 10  && bjets.size() == 0 )
      countSignalEvent("SR12");

    if(sigjets.size() >= 10  && bjets.size() >= 3 )
      countSignalEvent("SR13");

    sigjets = filterPhaseSpace(sigjets, 40., -2.5, 2.5);
    bjets = filterPhaseSpace(bjets, 40., -2.5, 2.5);
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_2lSC_pT>40_6j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>40_>2b_6j");
    }
    if ( sigjets.size() >=7 ) {
      countCutflowEvent("07_2lSC_pT>40_7j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>40_>2b_7j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_2lSC_pT>40_8j"); 
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>40_>2b_8j");
    }    

    if(sigjets.size() >= 8  && bjets.size() == 0 )
      countSignalEvent("SR14");

    if(sigjets.size() >= 8  && bjets.size() >= 3 )
      countSignalEvent("SR15");
  
    sigjets = filterPhaseSpace(sigjets, 60., -2.5, 2.5);
    bjets = filterPhaseSpace(bjets, 60., -2.5, 2.5);
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_2lSC_pT>60_6j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>60_>2b_6j");
    }
    if ( sigjets.size() >=7 ) {
      countCutflowEvent("07_2lSC_pT>60_7j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>60_>2b_7j");
    }    

    if(sigjets.size() >= 7  && bjets.size() == 0 )
      countSignalEvent("SR16");

    if(sigjets.size() >= 7  && bjets.size() >= 3 )
      countSignalEvent("SR17");

    sigjets = filterPhaseSpace(sigjets, 80., -2.5, 2.5);
    bjets = filterPhaseSpace(bjets, 80., -2.5, 2.5);
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_2lSC_pT>80_6j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>80_>2b_6j");
    }
    if ( sigjets.size() >=7 ) {
      countCutflowEvent("07_2lSC_pT>80_7j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>80_>2b_7j");
    }        

    if(sigjets.size() >= 7  && bjets.size() == 0 )
      countSignalEvent("SR18");

    if(sigjets.size() >= 7 && bjets.size() >= 3 )
      countSignalEvent("SR19");

    sigjets = filterPhaseSpace(sigjets, 100., -2.5, 2.5);
    bjets = filterPhaseSpace(bjets, 100., -2.5, 2.5);
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_2lSC_pT>100_6j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>100_>2b_6j");
    }    

    if(sigjets.size() >= 6  && bjets.size() == 0 )
      countSignalEvent("SR20");

    if(sigjets.size() >= 6  && bjets.size() >= 3 )
      countSignalEvent("SR21");
  }
    
}

void Atlas_2106_09609::finalize() {
  // Whatever should be done after the run goes here
}       


template <class X>
std::vector <X*> Atlas_2106_09609::removeLowMassResonances(std::vector <X*> candidates, double minminv, double maxminv)
{
  int z_candidate=0;
  std::set <std::pair<double, double>> zmassdiff;
  double mll;
  
  //  for (int e = 0; e < candidates.size(); e++) {
  //  for (int e2 = e+1; e2 < candidates.size(); e2++) {

  auto it = candidates.begin();
  std::vector <int> removeCandidates;
  while(it != candidates.end())
    {
       removeCandidates.push_back(1);
      ++it;
    }
  
  
  for(auto cand1 = candidates.begin(); cand1 != candidates.end(); ++cand1){
    //    for(auto cand2 = std::next(cand1,1); cand2 != candidates.end();){
    for(auto cand2 = candidates.begin(); cand2 != candidates.end(); ++cand2){

      if(cand1 == cand2)
	continue;
      
      if ((*cand1)->Charge*(*cand2)->Charge > 0)
	continue;

      TLorentzVector pair = ( (*cand1)->P4() + (*cand2)->P4() );
      mll = pair.M();
      if (mll > minminv && mll < maxminv) 
	{
	  //	  std::distance(candidates.begin(), cand1);
	   removeCandidates[std::distance(candidates.begin(), cand1)]=0;
	   removeCandidates[std::distance(candidates.begin(), cand2)]=0;
	  //	  cand1 = candidates.erase(cand1);
	  //cand2 = candidates.erase(cand2);
	}
      //      else
      //	{
      //	  ++cand1;
      //	  ++cand2;
      //	}	
    }//for
  }//for


  it = candidates.begin();
  size_t index = 0;
  
  while( it != candidates.end())
    {
      if( removeCandidates[index] == 0){
	it = candidates.erase(it);
	++index;
      }
      else
	{
	  ++it;
	  ++index;
	}
    }
  

  return candidates;
}

    
template <class X>
void Atlas_2106_09609::findZ(std::vector<X*> candidates, std::vector <std::pair<double, double>> & zmassdiff)
{

  double mll=0.;
  
  for (int e = 0; e < candidates.size(); e++) {
    for (int e2 = e+1; e2 < candidates.size(); e2++) {      
      if (candidates[e]->Charge*candidates[e2]->Charge > 0) continue;

      TLorentzVector pair = ( candidates[e]->P4() + candidates[e2]->P4() );
      mll = pair.M();
      if ( fabs( mll-91.2) < 10.){
	zmassdiff.push_back(std::make_pair(fabs(mll-91.2),mll));
      }//if z boson
    }//for
  }//for
  return;
}

std::vector<Jet*> Atlas_2106_09609::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
  std::vector<Jet*> passed;
  for (std::vector<Jet*>::iterator jet = cand_jets.begin(); jet != cand_jets.end(); jet++) {
  
    if (check_nTrack_jet(*jet, tracks, nTracks)) {
      passed.push_back(*jet);
      continue;
    }
    double dR = deltaR;
    bool iso = true;
        
    for (std::vector<Muon*>::iterator mu=cand_muons.begin(); mu!=cand_muons.end(); mu++) 
      if ((*jet)->P4().DeltaR((*mu)->P4()) < dR) {
	iso = false;
	break;
      }
        		    
    if (iso) passed.push_back(*jet);
  }
  
  return passed;
}

template <class X, class Y>
std::vector<X*> Atlas_2106_09609::specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours) {
      // If neighbours are empty, return candidates
      if(neighbours.size() == 0)
        return candidates;
      std::vector<X*> passed_candidates;
      // Loop over candidates
      for(int i = 0; i < candidates.size(); i++) {
        bool overlap = false;
        // If a neighbour is too close, declare overlap, break and don't save candidate
        for(int j = 0; j < neighbours.size(); j++) {
          if (candidates[i]->P4().DeltaR(neighbours[j]->P4()) > 0.2 and candidates[i]->P4().DeltaR(neighbours[j]->P4()) < std::min(0.4, 0.04 + 10./candidates[i]->PT) ) {
            overlap = true;
            break;
          }
        }
        if (!overlap)
          passed_candidates.push_back(candidates[i]);
      }
      return passed_candidates;
    }
    
bool Atlas_2106_09609::jetsortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }    

bool Atlas_2106_09609::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}
