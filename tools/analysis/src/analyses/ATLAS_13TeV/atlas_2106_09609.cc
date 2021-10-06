#include "atlas_2106_09609.h"
#include <limits.h>
#include <float.h>
// AUTHOR: JSK
//  EMAIL: jsk@th.physik.uni-bonn.de
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
}

void Atlas_2106_09609::analyze() {
  // Your eventwise analysis code goes here
  // The following objects are always defined unless they are 'ignored' above. They form std::vector objects of the respective Delphes class type (except for Etmiss which is a single object)
  // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
  // Within a std::vector, all members are ordered with highest pt coming first.

  // electronsLoose, electronsMedium, electronsTight   are list of electrons that passed respective efficiency and reconstruction cuts
  // muonsCombinedPlus, muonsCombined                  as above for muons
  // photonsMedium                                     as above for photons
  // jets are all reconstructed jets                   as above for jets. Note that electrons are most certainly also reconstructed as a jet -> overlap removal do avoid double counting necessary!
  // tracks, towers                                    calorimeter and tracker information. Usually not needed.
  // missingET                                         rec missing ET EXCLUDING muons.

  
  // Here is a couple of useful functions and lines:  
  //------------Phase Space Cuts (defined for jets, electronsXYZ, muonsXYZ, photonsXYZ)
  // jets = filterPhaseSpace(jets, 20., -2.8, 2.8)  // The vector 'jets' only contains jets with pt >= 20 GeV and -2.8 < eta < 2.8. This function is applicable to other particles too (electronsMedium, ... ).
  // jets = overlapRemoval(jets, electronsLoose, 0.2) Removes all jets for which there exists any electron in 'electronsLoose' with deltaR < 0.2.
  // jets = overlapRemovel(jets, 0.2) If two jets overlap within deltaR < 0.2, only the harder jet is stored.
  
  //------------Isolation Checks (defined for electronsXYZ, muonsXYZ, photonsXYZ
  //------------        For each object, if the user entered N isolation conditions, they can be
  //------------        checked individually be the second argument (running from 0 to N-1).
  // electronsMedium = filterIsolation(electronsMedium, 0)            Removes electrons that do not pass the first isolation condition entered into the AnalysisManager by the user
  // std::vector<int> flags; flags.push_back(0); flags.push_back(2);
  // electronsMedium = filterIsolation(electronsMedium, flags)        Same as above, but both the first and the third condition have to be fulfilled
  // electronsMedium = filterIsolation(electronsMedium)               Same as above, but all conditions have to be fulfilled.
  
  //-----------Flavour Tag Checks (defined for jets only)
  //----------          Tau tags "loose", "medium" or "tight" can be individually checked if the user enabled tau tagging in the AM.
  //----------          For b-tags, if N working points have been defined, the ith condition can be tested by putting i-1 into the second argument (if there is only one, the argument can be omitted)
  // if checkTauTag(jets[0], "tight") leadingJetIsTagged = True;
  // if checkBTag(jets[0], 0) leadingJetIsBTagged = True;


  //-----------Auxiliary Information
  // - Always ensure that you don't access vectors out of bounds. E.g. 'if(jets[1]->PT > 150)' should rather be if (jets.size() > 1 && jets[1]->PT > 150). 
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in CheckMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  electronsTight = filterPhaseSpace(electronsTight, 15., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 15., -2.7, 2.7);
  auto storeOriginalJets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);

  
  electronsTight = overlapRemoval(electronsTight, muonsCombined, 0.01);
  jets = overlapRemoval(jets, electronsTight, 0.2);
  electronsTight = overlapRemoval(electronsTight, jets, 0.04);
  jets = overlapRemoval(jets, muonsCombined, 0.2);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.04);

  std::vector<Jet*> bjets;
  std::vector<Jet*> nonbjets;
  for (int i = 0; i < jets.size(); i++)
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i]) ) 
      bjets.push_back(jets[i]);
    else nonbjets.push_back(jets[i]);  

  std::vector<FinalStateObject*> leptons;
  for(int e = 0; e < electronsTight.size(); e++) {
    FinalStateObject* lep = newFinalStateObject(electronsTight[e]);
    leptons.push_back(lep);
  }
  for(int m = 0; m < muonsCombined.size(); m++) {
    FinalStateObject* lep = newFinalStateObject(muonsCombined[m]);
    leptons.push_back(lep);
  }
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

  countCutflowEvent("02_lepton[0]->PT>27");
  
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
      for(int i=0; i < jets.size(); ++i)
	{
	  for(int j=i+1; j < jets.size(); ++j)
	    {
	      TLorentzVector pair = leptons[0]->P4() + jets[i]->P4();
	      mlj1=pair.M();
	      pair=leptons[0]->P4() + jets[j]->P4();
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

  countCutflowEvent("06_pT>20: " + std::to_string(jets.size()));
  
  if(jets.size() >= 15 && oneLepton && bjets.size() == 0)
    countSignalEvent("SR1");

  if(jets.size() >= 15 && oneLepton && bjets.size() >= 3)
    countSignalEvent("SR2");

  jets = filterPhaseSpace(jets, 40., -4.5, 4.5);

  if(jets.size() >= 12 && oneLepton && bjets.size() == 0)
    countSignalEvent("SR3");

  if(jets.size() >= 12 && oneLepton && bjets.size() >= 3)
    countSignalEvent("SR4");
  
  jets = filterPhaseSpace(jets, 60., -4.5, 4.5);

  if(jets.size() >= 11 && oneLepton && bjets.size() == 0)
    countSignalEvent("SR5");

  if(jets.size() >= 11 && oneLepton && bjets.size() >= 3)
    countSignalEvent("SR6");

  jets = filterPhaseSpace(jets, 80., -4.5, 4.5);

  if(jets.size() >= 10 && oneLepton && bjets.size() == 0)
    countSignalEvent("SR7");

  if(jets.size() >= 10 && oneLepton && bjets.size() >= 3)
    countSignalEvent("SR8");

  jets = filterPhaseSpace(jets, 100., -4.5, 4.5);

  if(jets.size() >= 8 && oneLepton && bjets.size() == 0)
    countSignalEvent("SR9");

  if(jets.size() >= 8 && oneLepton && bjets.size() >= 3)
    countSignalEvent("SR10");

  jets = storeOriginalJets;

  countCutflowEvent("07_pT>20_2lSC: " + std::to_string(jets.size()));
  
  if(jets.size() == 6 && twoLeptonSC && bjets.size() >= 3)// && mlj < 155.)
    countSignalEvent("SR11");
  
  if(jets.size() >= 10 && twoLeptonSC && bjets.size() == 0)
    countSignalEvent("SR12");

  if(jets.size() >= 10 && twoLeptonSC && bjets.size() >= 3)
    countSignalEvent("SR13");

  jets = filterPhaseSpace(jets, 40., -4.5, 4.5);

  if(jets.size() >= 8 && twoLeptonSC && bjets.size() == 0)
    countSignalEvent("SR14");

  if(jets.size() >= 8 && twoLeptonSC && bjets.size() >= 3)
    countSignalEvent("SR15");
  
  jets = filterPhaseSpace(jets, 60., -4.5, 4.5);

  if(jets.size() >= 7 && twoLeptonSC && bjets.size() == 0)
    countSignalEvent("SR16");

  if(jets.size() >= 7 && twoLeptonSC && bjets.size() >= 3)
    countSignalEvent("SR17");

  jets = filterPhaseSpace(jets, 80., -4.5, 4.5);

  if(jets.size() >= 7 && twoLeptonSC && bjets.size() == 0)
    countSignalEvent("SR18");

  if(jets.size() >= 7 && twoLeptonSC && bjets.size() >= 3)
    countSignalEvent("SR19");

  jets = filterPhaseSpace(jets, 100., -4.5, 4.5);

  if(jets.size() >= 6 && twoLeptonSC && bjets.size() == 0)
    countSignalEvent("SR20");

  if(jets.size() >= 6 && twoLeptonSC && bjets.size() >= 3)
    countSignalEvent("SR21");
    
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
