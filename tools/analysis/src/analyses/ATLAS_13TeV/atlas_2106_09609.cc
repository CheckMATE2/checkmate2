#include "atlas_2106_09609.h"
#include <limits.h>
#include <float.h>
#include </home/krolb/tools/CheckMATE/ONNX/tools/onnxruntime-linux-x64-1.12.1/include/onnxruntime_cxx_api.h>
#include <TLegend.h>
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
  bookSignalRegions("SR0;SR1;SR2;SR3;SR4;SR5;SR6;SR7;SR8;SR9;SR10;SR11;SR12;SR13;SR14;SR15;SR16;SR17;SR18;SR19;SR20;SR21");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here

  Ort::AllocatorWithDefaultOptions allocator;
  std::vector<const char*> input_node_names;
  std::vector<const char*> output_node_names;
  std::vector<int64_t> input_node_dims;

  Ort::Env env[5];
  for (int i = 0; i < 5; i++)
    env[i] = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "EWK_SR");

  Ort::SessionOptions session_options[5];
  session[0] = new Ort::Session(env[0], "/home/krolb/tools/CheckMATE/ONNX/data/SUSY-2019-04_4jets.onnx", session_options[0]);
  session[1] = new Ort::Session(env[1], "/home/krolb/tools/CheckMATE/ONNX/data/SUSY-2019-04_5jets.onnx", session_options[1]);
  session[2] = new Ort::Session(env[2], "/home/krolb/tools/CheckMATE/ONNX/data/SUSY-2019-04_6jets.onnx", session_options[2]);
  session[3] = new Ort::Session(env[3], "/home/krolb/tools/CheckMATE/ONNX/data/SUSY-2019-04_7jets.onnx", session_options[3]);
  session[4] = new Ort::Session(env[4], "/home/krolb/tools/CheckMATE/ONNX/data/SUSY-2019-04_8jets.onnx", session_options[4]);
  
  // this shows how to get information about input nodes and tensors
  // here assume for simplicity all nets have the same input/output; so just do that for session[0]
  // 1 input node named "input.1" rank 2 with dimensions 1*65
  // cf. https://github.com/microsoft/onnxruntime-inference-examples/tree/main/c_cxx
  
  const size_t num_input_nodes = session[0]->GetInputCount();
  cout << "Number input nodes: " << num_input_nodes << endl;
  input_node_names.reserve(num_input_nodes);
  
  for (size_t i = 0; i < num_input_nodes; i++) {
    auto type_info = session[0]->GetInputTypeInfo(i);
    auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
    auto input_name = session[0]->GetInputNameAllocated(i, allocator);
    std::cout << "Input " << i << " : name = " << input_name.get() << std::endl;
    input_node_names.push_back(input_name.get());
    input_names.push_back(input_name.get());
    ONNXTensorElementDataType type = tensor_info.GetElementType(); 
    std::cout << "Input " << i << " : type = " << type << std::endl;
    input_node_dims = tensor_info.GetShape();
    std::cout << "Input " << i << " : num_dims = " << input_node_dims.size() << '\n';
    for (size_t j = 0; j < input_node_dims.size(); j++) 
        std::cout << "Input " << i << " : dim[" << j << "] = " << input_node_dims[j] << '\n';
  }   // input_tensor_size = input_node_dims[0] * input_node_dims[1] * .... 
  
  //asume just 1 output node 
  
  const size_t num_output_nodes = session[0]->GetOutputCount();
  cout << "Number output nodes: " << num_output_nodes << endl;
  output_node_names.reserve(num_output_nodes);
  auto output_name = session[0]->GetOutputNameAllocated(0, allocator);
  std::cout << "Output " << "0" << " : name = " << output_name.get() << std::endl;
  output_node_names.push_back(output_name.get());
  auto type_info = session[0]->GetOutputTypeInfo(0);
  auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
  std::vector<int64_t> output_node_dims = tensor_info.GetShape();
  std::cout << "Output " << 0 << " : num_dims = " << output_node_dims.size() << '\n';
  for (size_t j = 0; j < input_node_dims.size(); j++) 
    std::cout << "Output " << 0 << " : dim[" << j << "] = " << output_node_dims[j] << '\n';

  int ifile = bookFile("atlas_2106_09609.root", true);
  const char *rootFileName = fNames[ifile].c_str() ;
  hfile = new TFile(rootFileName, "RECREATE", "Saving Histograms");
  nn4j = new TH1F("nn4j", "nn", 4, 0., 1.);
  nn5j = new TH1F("nn5j", "nn", 4, 0., 1.);
  nn6j = new TH1F("nn6j", "nn", 4, 0., 1.);
  nn7j = new TH1F("nn7j", "nn", 4, 0., 1.);
  nn8j = new TH1F("nn8j", "nn", 4, 0., 1.);  
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
  
  muonsCombined = filterIsolation(muonsCombined);
  //muonsCombined = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined, tracks, towers, 0.3, 10., 0.2, 0.06, 0.2, false); //old
  //muonsCombined = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined, tracks, towers, 0.3, 10., 0.2, 0.15, 0.3, true); //Loose
  muonsCombined = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined, tracks, towers, 0.3, 10., 0.2, 0.04, 0.15, true); //Tight
  electronsTight = filterIsolation(electronsTight);

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
  if(leptons.size() == 2 && leptons[0]->Charge * leptons[1]->Charge > 0) {
      if(electronsTight.size() == 2) {
        TLorentzVector pair = ( electronsTight[0]->P4() + electronsTight[1]->P4() );
        mee = pair.M();
        if ( fabs( mee-91.2) > 10.) twoLeptonSC = true;
      }
      else twoLeptonSC = true;
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
      countCutflowEvent("07_1l_pT>20_06j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>20_>2b_06j");
      if ( !bjets.size() ) countCutflowEvent("07_1l_pT>20_0b_06j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_1l_pT>20_08j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>20_>2b_08j");
      if ( !bjets.size() ) countCutflowEvent("07_1l_pT>20_0b_08j");
    }
    if ( sigjets.size() >=10 ) {
      countCutflowEvent("07_1l_pT>20_10j"); 
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>20_>2b_10j");
      if ( !bjets.size() ) countCutflowEvent("07_1l_pT>20_0b_10j");
    }
    if ( sigjets.size() >=11 ) {
      countCutflowEvent("07_1l_pT>20_11j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>20_>2b_11j");
      if ( !bjets.size() ) countCutflowEvent("07_1l_pT>20_0b_11j");
    }  
    if ( sigjets.size() >=12 ) {
      countCutflowEvent("07_1l_pT>20_12j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>20_>2b_12j");
      if ( !bjets.size() ) countCutflowEvent("07_1l_pT>20_0b_12j");
    } 
    if ( sigjets.size() >=15 ) {
      countCutflowEvent("07_1l_pT>20_15j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>20_>2b_15j");
      if ( !bjets.size() ) countCutflowEvent("07_1l_pT>20_0b_15j");
    } 
    
  
    if(sigjets.size() == 4 ) {
      countCutflowEvent("06_EW_1l_4j");
      if ( bjets.size() >= 4) {
        countCutflowEvent("06_EW_1l_4b_4j");
        float outputnn = Passes_Cuts_NNSR(sigjets, bjets, leptons);
        nn4j->Fill(outputnn, weight);
        if (outputnn > 0.73 ) countCutflowEvent("06_EW_1l_4b_4j_NN");
      }
    }
    if(sigjets.size() == 5) {
      countCutflowEvent("06_EW_1l_5j");
      if ( bjets.size() >= 4) {
        countCutflowEvent("06_EW_1l_4b_5j");
        float outputnn = Passes_Cuts_NNSR(sigjets, bjets, leptons);
        nn5j->Fill(outputnn, weight);
        if (outputnn > 0.76 ) countCutflowEvent("06_EW_1l_4b_5j_NN");
      }
    }
    if(sigjets.size() == 6) {
      countCutflowEvent("06_EW_1l_6j");
      if ( bjets.size() >= 4) {
        countCutflowEvent("06_EW_1l_4b_6j");
        float outputnn = Passes_Cuts_NNSR(sigjets, bjets, leptons);
        nn6j->Fill(outputnn, weight);
        if (outputnn > 0.77 ) {
          countCutflowEvent("06_EW_1l_4b_6j_NN");
          countSignalEvent("SR0");
        }
      }
    }
    if(sigjets.size() == 7) {
      countCutflowEvent("06_EW_1l_7j");
      if ( bjets.size() >= 4) {
        countCutflowEvent("06_EW_1l_4b_7j");
        float outputnn = Passes_Cuts_NNSR(sigjets, bjets, leptons);
        nn7j->Fill(outputnn, weight);
        if (outputnn > 0.72 ) countCutflowEvent("06_EW_1l_4b_7j_NN");
      }
    }
    if(sigjets.size() == 8) {
      countCutflowEvent("06_EW_1l_8j");
      if ( bjets.size() >= 4) {
        countCutflowEvent("06_EW_1l_4b_8j");
        float outputnn = Passes_Cuts_NNSR(sigjets, bjets, leptons);
        nn8j->Fill(outputnn, weight);
        if (outputnn > 0.73 ) countCutflowEvent("06_EW_1l_4b_8j_NN");
      }
    }
                                       
  
    if(sigjets.size() >= 15 && bjets.size() == 0)
      countSignalEvent("SR1");

    if(sigjets.size() >= 15 && bjets.size() >= 3)
      countSignalEvent("SR2");

    sigjets = filterPhaseSpace(sigjets, 40., -2.5, 2.5);
    bjets = filterPhaseSpace(bjets, 40., -2.5, 2.5);
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_1l_pT>40_06j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>40_>2b_06j");
      if (!bjets.size()  ) countCutflowEvent("07_1l_pT>40_0b_06j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_1l_pT>40_08j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>40_>2b_08j");
      if (!bjets.size() ) countCutflowEvent("07_1l_pT>40_0b_08j");
    }
    if ( sigjets.size() >=10 ) {
      countCutflowEvent("07_1l_pT>40_10j"); 
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>40_>2b_10j");
      if (!bjets.size()  ) countCutflowEvent("07_1l_pT>40_0b_10j");
    }
    if ( sigjets.size() >=11 ) {
      countCutflowEvent("07_1l_pT>40_11j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>40_>2b_11j");
      if (!bjets.size()  ) countCutflowEvent("07_1l_pT>40_0b_11j");
    }  
    if ( sigjets.size() >=12 ) {
      countCutflowEvent("07_1l_pT>40_12j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>40_>2b_12j");
      if (!bjets.size()  ) countCutflowEvent("07_1l_pT>40_0b_12j");
    } 

    if(sigjets.size() >= 12 && bjets.size() == 0)
      countSignalEvent("SR3");

    if(sigjets.size() >= 12 && bjets.size() >= 3)
      countSignalEvent("SR4");
  
    sigjets = filterPhaseSpace(sigjets, 60., -2.5, 2.5);
    bjets = filterPhaseSpace(bjets, 60., -2.5, 2.5);
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_1l_pT>60_06j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>60_>2b_06j");
      if (!bjets.size() ) countCutflowEvent("07_1l_pT>60_0b_06j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_1l_pT>60_08j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>60_>2b_08j");
      if (!bjets.size() ) countCutflowEvent("07_1l_pT>60_0b_08j");
    }
    if ( sigjets.size() >=10 ) {
      countCutflowEvent("07_1l_pT>60_10j"); 
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>60_>2b_10j");
      if (!bjets.size() ) countCutflowEvent("07_1l_pT>60_0b_10j");
    }
    if ( sigjets.size() >=11 ) {
      countCutflowEvent("07_1l_pT>60_11j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>60_>2b_11j");
      if (!bjets.size() ) countCutflowEvent("07_1l_pT>60_0b_11j");
    }      

    if(sigjets.size() >= 11 && bjets.size() == 0)
      countSignalEvent("SR5");

    if(sigjets.size() >= 11 && bjets.size() >= 3)
      countSignalEvent("SR6");

    sigjets = filterPhaseSpace(sigjets, 80., -2.5, 2.5);
    bjets = filterPhaseSpace(bjets, 80., -2.5, 2.5);
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_1l_pT>80_06j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>80_>2b_06j");
      if (!bjets.size() ) countCutflowEvent("07_1l_pT>80_0b_06j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_1l_pT>80_08j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>80_>2b_08j");
      if (!bjets.size() ) countCutflowEvent("07_1l_pT>80_0b_08j");
    }
    if ( sigjets.size() >=10 ) {
      countCutflowEvent("07_1l_pT>80_10j"); 
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>80_>2b_10j");
      if (!bjets.size() ) countCutflowEvent("07_1l_pT>80_0b_10j");
    }

    if(sigjets.size() >= 10 && bjets.size() == 0)
      countSignalEvent("SR7");

    if(sigjets.size() >= 10 && bjets.size() >= 3)
      countSignalEvent("SR8");

    sigjets = filterPhaseSpace(sigjets, 100., -2.5, 2.5);
    bjets = filterPhaseSpace(bjets, 100., -2.5, 2.5);
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_1l_pT>100_06j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>100_>2b_06j");
      if (!bjets.size() ) countCutflowEvent("07_1l_pT>100_0b_06j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_1l_pT>100_08j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_1l_pT>100_>2b_08j");
      if (!bjets.size() ) countCutflowEvent("07_1l_pT>100_0b_08j");
    }

    if(sigjets.size() >= 8 && bjets.size() == 0)
      countSignalEvent("SR9");

    if(sigjets.size() >= 8 && bjets.size() >= 3)
      countSignalEvent("SR10");
  }

  sigjets = storeOriginalJets;
  bjets = storeOriginalBJets;

  if ( twoLeptonSC ) {
    
    if ( sigjets.size() >=6 ) {
      countCutflowEvent("07_2lSC_pT>20_6j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>20_>2b_6j");
      if (!bjets.size() ) countCutflowEvent("07_2lSC_pT>20_0b_6j");
    }
    if ( sigjets.size() >=7 ) {
      countCutflowEvent("07_2lSC_pT>20_7j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>20_>2b_7j");
      if (!bjets.size() ) countCutflowEvent("07_2lSC_pT>20_0b_7j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_2lSC_pT>20_8j"); 
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>20_>2b_8j");
      if (!bjets.size() ) countCutflowEvent("07_2lSC_pT>20_0b_8j");
    }
    if ( sigjets.size() >=10 ) {
      countCutflowEvent("07_2lSC_pT>20_10j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>20_>2b_10j");
      if (!bjets.size() ) countCutflowEvent("07_2lSC_pT>20_0b_10j");
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
      if (!bjets.size() ) countCutflowEvent("07_2lSC_pT>40_0b_6j");
    }
    if ( sigjets.size() >=7 ) {
      countCutflowEvent("07_2lSC_pT>40_7j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>40_>2b_7j");
      if (!bjets.size() ) countCutflowEvent("07_2lSC_pT>40_0b_7j");
    }
    if ( sigjets.size() >=8 ) {
      countCutflowEvent("07_2lSC_pT>40_8j"); 
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>40_>2b_8j");
      if (!bjets.size() ) countCutflowEvent("07_2lSC_pT>40_0b_8j");
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
      if (!bjets.size() ) countCutflowEvent("07_2lSC_pT>60_0b_6j");
    }
    if ( sigjets.size() >=7 ) {
      countCutflowEvent("07_2lSC_pT>60_7j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>60_>2b_7j");
      if (!bjets.size() ) countCutflowEvent("07_2lSC_pT>60_0b_7j");
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
      if (!bjets.size() ) countCutflowEvent("07_2lSC_pT>80_0b_6j");
    }
    if ( sigjets.size() >=7 ) {
      countCutflowEvent("07_2lSC_pT>80_7j");
      if (bjets.size() >= 3 ) countCutflowEvent("07_2lSC_pT>80_>2b_7j");
      if (!bjets.size() ) countCutflowEvent("07_2lSC_pT>80_0b_7j");
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
      if (!bjets.size() ) countCutflowEvent("07_2lSC_pT>100_0b_6j");
    }    

    if(sigjets.size() >= 6  && bjets.size() == 0 )
      countSignalEvent("SR20");

    if(sigjets.size() >= 6  && bjets.size() >= 3 )
      countSignalEvent("SR21");
  }
    
}

void Atlas_2106_09609::finalize() {
  // Whatever should be done after the run goes here
  
  TCanvas can1;
  nn4j->DrawNormalized("hist");
  nn4j->SetLineColor(kRed);
  //nn4j->SetMaximum(1.);
  //nn4j->SetMinimum(0.);
  //nn4j->SetTitle("NN output 4 jets");
  //can1.Close();
  
  //TCanvas can2;
  nn5j->DrawNormalized("hist same");
  nn5j->SetLineColor(kBlue);
  //nn5j->SetTitle("NN output 5 jets");
  //can2.Close();
  
  //TCanvas can3;
  nn6j->DrawNormalized("hist same");
  nn6j->SetLineColor(kBlack);
  //nn6j->SetTitle("NN output 6 jets");
  //can3.Close(); 
  
  //TCanvas can4;
  nn7j->DrawNormalized("hist same");
  nn7j->SetLineColor(kOrange);
  //nn7j->SetTitle("NN output 7 jets");
  //can4.Close();  
  
  //TCanvas can5;
  nn8j->DrawNormalized("hist same");
  nn8j->SetLineColor(kGreen);
  //nn8j->SetTitle("NN output 8 jets");
  //can5.Close();    

  auto L_cth = new TLegend(0.65, 0.87, 0.92, 0.38);
  L_cth->AddEntry(nn4j,"4j","lp");
  L_cth->AddEntry(nn5j,"5j","lp");
  L_cth->AddEntry(nn6j,"6j","lp");  
  L_cth->AddEntry(nn7j,"7j","lp");
  L_cth->AddEntry(nn8j,"8j","lp");
  L_cth->Draw("same");
  //can1.Print("histo1.pdf");
  can1.Close();
  hfile->Write();

  hfile->Close();
  
  for (int i = 0; i < 5; i++)
    delete session[i];
  
  
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

float Atlas_2106_09609::Passes_Cuts_NNSR(const std::vector<Jet*> sigjets, const std::vector<Jet*> bjets, const std::vector<FinalStateObject*> leptons) {
  
  const float Escale = 100; 
  std::vector<float> input_tensor_values;  //input_tensor_size = 65
  
  assert(leptons.size());
  assert(sigjets.size() >= 4); 
  assert(sigjets.size() < 9); 
  assert(bjets.size() >= 4);
  std::vector b_cat = jet_btag_category(sigjets, bjets);
  
  input_tensor_values.push_back(sigjets.size());
  input_tensor_values.push_back(3); // n_bjets >= 4;
  input_tensor_values.push_back(bjets.size()); // n_bjets
  input_tensor_values.push_back(calc_three_jet_max_pt_mass(sigjets) / Escale);
  input_tensor_values.push_back(HT(sigjets) / Escale);
  input_tensor_values.push_back(HT(bjets) / Escale);
  input_tensor_values.push_back(calc_three_jet_max_pt_mass(sigjets, leptons[0]->P4(), missingET->P4()) / Escale);
  input_tensor_values.push_back(min_dr(sigjets, leptons[0]->P4())); // snippet and text are inconsistent on that (all leptons vs leading)
  input_tensor_values.push_back(calc_minmax_mass(sigjets) / Escale);
  input_tensor_values.push_back(missingET->P4().Pt() / Escale);
  input_tensor_values.push_back(missingET->P4().Phi());
  for (int i = 0; i < 10; i++)
    input_tensor_values.push_back(sigjets.size() > i ? sigjets[i]->PT / Escale : 1e-7);
  for (int i = 0; i < 10; i++)
    input_tensor_values.push_back(sigjets.size() > i ? sigjets[i]->Eta : -5.0);
  for (int i = 0; i < 10; i++)
    input_tensor_values.push_back(sigjets.size() > i ? sigjets[i]->Phi : -5.0);
  for (int i = 0; i < 10; i++)
    input_tensor_values.push_back(sigjets.size() > i ? sigjets[i]->P4().E() / Escale : 1e-7);
  for (int i = 0; i < 10; i++)
    input_tensor_values.push_back(sigjets.size() > i ? b_cat[i] : 0);
  input_tensor_values.push_back(leptons[0]->PT / Escale);
  input_tensor_values.push_back(leptons[0]->Eta );
  input_tensor_values.push_back(leptons[0]->Phi );
  input_tensor_values.push_back(leptons[0]->P4().E() / Escale);

  for(int i = 0; i < 65; i++)
    cout << "Input " << i << ": " << input_tensor_values[i] << std::endl;
  
  assert(input_tensor_values.size() == 65);
  
  //for (int i = 0; i < input_tensor_size; i++) input_tensor_values[i] = (float)i / (input_tensor_size + 1);
  auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
  auto input_tensor = Ort::Value::CreateTensor(memory_info, input_tensor_values.data(), input_tensor_size, input_dims.data(), 2); //rank = 2
  
  auto output_tensors = session[sigjets.size()-4]->Run(Ort::RunOptions{nullptr}, input_names.data(), &input_tensor, 1, output_names.data(), 1);
    
  float* output = output_tensors.front().GetTensorMutableData<float>();
  cout << "Score: " << output[0] << std::endl;
  
  return output[0];
  
  
  return 0;
  
}


float Atlas_2106_09609::calc_three_jet_max_pt_mass(const std::vector<Jet*> sigjets, const TLorentzVector lepton, const TLorentzVector met)  // from Atlas snippet
{
    
  TLorentzVector max_pt_system(0,0,0,0);

  for (size_t i=0; i < sigjets.size(); ++i) 
    for (size_t j=i+1; j < sigjets.size(); ++j) 
      for (size_t k=j+1; k < sigjets.size(); ++k) {
        TLorentzVector system = sigjets[i]->P4() + sigjets[j]->P4() + sigjets[k]->P4() + lepton + met;
        if (system.Pt() > max_pt_system.Pt()) max_pt_system = system;
      }

  return max_pt_system.M();
}

double Atlas_2106_09609::HT(const std::vector<Jet*> sigjets) {
  
  double  PTSum = 0.;
  for (int i = 0; i < sigjets.size(); i++) PTSum += sigjets[i]->PT;
  return PTSum;
}

double Atlas_2106_09609::min_dr(const std::vector<Jet*> sigjets, TLorentzVector lepton) {
  
 double min = 100.; 
 for (int i = 0; i < sigjets.size(); i++) {
   double dr = sigjets[i]->P4().DeltaR(lepton);
   if ( dr < min) min = dr;
 }
 
 return min;
   
}

std::vector<double> Atlas_2106_09609::jet_btag_category(const std::vector<Jet*> sigjets, const std::vector<Jet*> bjets) {
  
  std::vector<double> cat;
  int k = 0; //reduce number of loops
  for (int i = 0; i < sigjets.size(); i++) {
    bool btag = false;
    for (int j = k; j < bjets.size(); j++)
      if (sigjets[i] == bjets[j]) {
        k++;
        btag = true;
        break;
      }
    //if (btag) cat.push_back(5.); else cat.push_back(1.);
    if (btag) {
      double tag = 1.45 + rand()/(RAND_MAX +1.) * 8.;// https://arxiv.org/pdf/1907.05120.pdf Fig. 1
      //cat.push_back( tag);
      cat.push_back(5.); // ATLAS simiplified_analysis snippet
    }
    else {
      double  tag = -4. + rand()/(RAND_MAX +1.) * 3.;
      //cat.push_back( tag);
      //cat.push_back(-3.);
      cat.push_back(1.);
    }
  }
  
  return cat;
  
}

double Atlas_2106_09609::calc_minmax_mass(const std::vector<Jet*> sigjets) { //ATLAS routine
  
    const int nJets = sigjets.size();
    const int jetdiff = 10;

    //bitwise representation of all jets and for which half they are selected
    // One bit for every jet, marking into which set they are grouped
    const unsigned int bitmax = 1 << nJets;
    double minmass = 999999999;

    for(unsigned int bit=0; bit < bitmax; bit++){
        const int bitcount = countSetBits(bit);
        if (abs(nJets - 2*bitcount) > jetdiff) {
          continue;
        }

        TLorentzVector sum1, sum2;
        // loop through jets and assign to either sum, depending on bit
        for(int i=0; i<nJets; i++) {
            if (bit & (1<<i)) 
              sum1 += jets[i]->P4();
            else
              sum2 += jets[i]->P4();
        }
        if (sum1.M() > sum2.M() && sum1.M() < minmass) 
          minmass = sum1.M();
        else if (sum2.M() > sum1.M() && sum2.M() < minmass)
          minmass = sum2.M();
    }

    return minmass;  
  
}


unsigned int Atlas_2106_09609::countSetBits(unsigned int n)
{
  unsigned int count = 0;
  while (n) {
    count += n & 1;
    n >>= 1;
  }
  return count;
}
