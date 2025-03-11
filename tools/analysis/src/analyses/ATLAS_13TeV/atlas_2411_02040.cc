#include "atlas_2411_02040.h"
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
#ifdef HAVE_ONNX
#include "onnxruntime_cxx_api.h"
#endif

std::vector<std::vector<std::vector<size_t>>> pairings = {{{1, 2},{3, 4},{5, 6}},{{1, 2},{3, 5},{4, 6}},{{1, 2},{3, 6},{5, 4}},{{1, 3},{2, 4},{5, 6}},{{1, 3},{2, 5},{4, 6}},{{1, 3},{2, 6},{5, 4}},{{1, 4},{3, 2},{5, 6}},{{1, 4},{3, 5},{2, 6}},{{1, 4},{3, 6},{5, 2}},{{1, 5},{3, 4},{2, 6}},{{1, 5},{3, 2},{4, 6}},{{1, 5},{3, 6},{2, 4}},{{1, 6},{3, 4},{5, 2}},{{1, 6},{3, 5},{4, 2}},{{1, 6},{3, 2},{5, 4}}}; 

void Atlas_2411_02040::initialize() {
  setAnalysisName("atlas_2411_02040");          
  setInformation(""
    "# triple Higgs search in the 6b final state with NN\n"
  "");
  setLuminosity(126.0*units::INVFB);      
  bookSignalRegions("Inc_6b;Nonres;Res;Heavy;CR_5b");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
#ifdef HAVE_ONNX
  char *a = Global::get_maindir();
  std::string maindir(a, strlen(a));

  Ort::AllocatorWithDefaultOptions allocator;
  std::vector<const char*> input_node_names;
  std::vector<const char*> output_node_names;
  std::vector<int64_t> input_node_dims;

  Ort::Env env[6];
  for (int i = 0; i < 6; i++)
    env[i] = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "TRIH");

  std::vector<std::string> onnx_files = {
    maindir  + "/data/atlas_2411_02040/ANA-HIGP-2024-32_nonresDNN_model_even.onnx",
    maindir  + "/data/atlas_2411_02040/ANA-HIGP-2024-32_nonresDNN_model_odd.onnx",
    maindir  + "/data/atlas_2411_02040/ANA-HIGP-2024-32_resDNN_model_even.onnx",
    maindir  + "/data/atlas_2411_02040/ANA-HIGP-2024-32_resDNN_model_odd.onnx",
    maindir  + "/data/atlas_2411_02040/ANA-HIGP-2024-32_heavyresDNN_model_even.onnx",
    maindir  + "/data/atlas_2411_02040/ANA-HIGP-2024-32_heavyresDNN_model_odd.onnx"};
  Ort::SessionOptions session_options[6];
  for (int i = 0; i < 6; i++)
    session[i] = new Ort::Session(env[i], onnx_files[i].c_str(), session_options[i]);

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
#endif

  int ifile = bookFile("atlas_2411_02040.root", true);
  const char *rootFileName = fNames[ifile].c_str() ;
  hfile = new TFile(rootFileName, "RECREATE", "Saving Histograms");

  hist_mhradius = new TH1F("mHradius", "mHradius", 25, 0., 250.);
  hist_rmsdeltarjj = new TH1F("RMSdeltaRjj", "RMSdeltaRjj", 20, 0., 2.);

}

void Atlas_2411_02040::analyze() {


  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out. Probably not since 3.4.2
  
  electronsLoose = filterPhaseSpace(electronsLoose, 20, -2.47, 2.47);
  electronsLoose = filterIsolation(electronsLoose);
  muonsCombined = filterPhaseSpace(muonsCombined, 20, -2.5, 2.5);
  muonsCombined = filterIsolation(muonsCombined);
  jets = filterPhaseSpace(jets, 17., -2.5, 2.5);
  
  std::vector<Jet*> nonbjets={};
  std::vector<Jet*> bjets={};
  std::vector<Jet*> bjets_truth={};
  for (int i = 0; i < jets.size(); i++) {
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i], 0) )
      bjets_truth.push_back(jets[i]);
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i], 1) )
      bjets.push_back(jets[i]);
    else nonbjets.push_back(jets[i]);
  }
  
  countCutflowEvent("00_all");
  
  if (bjets.size() > 3) countCutflowEvent("00_control4j");

  nonbjets = overlapRemoval(nonbjets, electronsLoose, 0.2, "y");      
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.4, "y");      
  electronsLoose = overlapRemoval(electronsLoose, nonbjets, 0.4, "y");      
  nonbjets = overlapRemoval_muon_jet_tracks(nonbjets, muonsCombined, 0.2, 2);   
  bjets = overlapRemoval_muon_jet_tracks(bjets, muonsCombined, 0.2, 2);   
  muonsCombined = overlapRemoval(muonsCombined, nonbjets, 0.4, "y");
  muonsCombined = overlapRemoval(muonsCombined, bjets, 0.4, "y");
  
  if (bjets.size() > 3) countCutflowEvent("00_overlap4j");
  
  if (bjets_truth.size() < 4) return;
  countCutflowEvent("00_filter4b");

  if (bjets.size() + nonbjets.size() < 4) return;
  std::vector<Jet*> bjets35 = filterPhaseSpace(bjets, 35., -2.5, 2.5);
  std::vector<Jet*> nonbjets35 = filterPhaseSpace(nonbjets, 35., -2.5, 2.5);
  std::vector<Jet*> bjets40 = filterPhaseSpace(bjets, 40., -2.5, 2.5);
  std::vector<Jet*> nonbjets40 = filterPhaseSpace(nonbjets, 40., -2.5, 2.5);
  bool trigger = false;
  if (rand()/(RAND_MAX +1.) < 0.7) 
    if (bjets35.size() > 1 and bjets35.size() + nonbjets35.size() > 3) trigger = true; 
  else
    if (bjets35.size() > 2 and bjets35.size() + nonbjets35.size() > 3) trigger = true; 
    
  if (!trigger) return;
  countCutflowEvent("01_trigger");
  
  if (bjets.size() + nonbjets.size() < 6) return;
  countCutflowEvent("02_6jets");
  
  if (bjets40.size() + nonbjets40.size() < 4) return;
  countCutflowEvent("03_4*pt>40");
  
  if (bjets40.size() < 4) return;
  countCutflowEvent("04_4b*pt>40");
  
  bool bjets4 = false;
  bool bjets5 = false;
  bool bjets6 = false;
  std::vector<Jet*> sigjets = {};
  if (bjets.size() == 4) {bjets4 = true; countCutflowEvent("05_4b"); return;} //dropping these events for now
  else if (bjets.size() == 5) {
    bjets5 = true; countCutflowEvent("05_5b");
    sigjets = bjets;
    sigjets.push_back(nonbjets[0]);
  }
  else {
    bjets6 = true; 
    countCutflowEvent("05_6b");
    for (int i = 0; i < 6; i++) sigjets.push_back(bjets[i]);
  }
  
  double pair_score = -1.;
  std::vector<int> pairs = {-1, -1, -1, -1, -1, -1,};
  double pTH1, pTH2, pTH3, mH1, mH2, mH3;
  for (int i = 0; i < 15; i++) {
    std::vector<int> tmp_p = {-1, -1, -1, -1, -1, -1,};
    double pTHa = (sigjets[pairings[i][0][0]-1]->PT + sigjets[pairings[i][0][1]-1]->PT);
    double mHa = (sigjets[pairings[i][0][0]-1]->P4() + sigjets[pairings[i][0][1]-1]->P4()).M();
    double pTHb = (sigjets[pairings[i][1][0]-1]->PT + sigjets[pairings[i][1][1]-1]->PT);
    double mHb = (sigjets[pairings[i][1][0]-1]->P4() + sigjets[pairings[i][1][1]-1]->P4()).M();
    double pTHc = (sigjets[pairings[i][2][0]-1]->PT + sigjets[pairings[i][2][1]-1]->PT);
    double mHc = (sigjets[pairings[i][2][0]-1]->P4() + sigjets[pairings[i][2][1]-1]->P4()).M();
    if ((pTHa >= pTHb) and (pTHb >= pTHc)) {
      pTH1 = pTHa; pTH2 = pTHb; pTH3 = pTHc; mH1 = mHa; mH2 = mHb; mH3 = mHc;
      tmp_p[0] = pairings[i][0][0]; tmp_p[1] = pairings[i][0][1]; 
      tmp_p[2] = pairings[i][1][0]; tmp_p[3] = pairings[i][1][1]; 
      tmp_p[4] = pairings[i][2][0]; tmp_p[5] = pairings[i][2][1];
    } 
    else if ((pTHa >= pTHc) && (pTHc >= pTHb)) {
      pTH1 = pTHa; pTH2 = pTHc; pTH3 = pTHb; mH1 = mHa; mH2 = mHc; mH3 = mHb;
      tmp_p[0] = pairings[i][0][0]; tmp_p[1] = pairings[i][0][1]; 
      tmp_p[2] = pairings[i][2][0]; tmp_p[3] = pairings[i][2][1]; 
      tmp_p[4] = pairings[i][1][0]; tmp_p[5] = pairings[i][1][1];      
    } 
    else if ((pTHb >= pTHa) && (pTHa >= pTHc)) {
      pTH1 = pTHb; pTH2 = pTHa; pTH3 = pTHc; mH1 = mHb; mH2 = mHa; mH3 = mHc;
      tmp_p[0] = pairings[i][1][0]; tmp_p[1] = pairings[i][1][1]; 
      tmp_p[2] = pairings[i][0][0]; tmp_p[3] = pairings[i][0][1]; 
      tmp_p[4] = pairings[i][2][0]; tmp_p[5] = pairings[i][2][1];      
    } 
    else if ((pTHb >= pTHc) && (pTHc >= pTHa)) {
      pTH1 = pTHb; pTH2 = pTHc; pTH3 = pTHa; mH1 = mHb; mH2 = mHc; mH3 = mHa;
      tmp_p[0] = pairings[i][1][0]; tmp_p[1] = pairings[i][1][1]; 
      tmp_p[2] = pairings[i][2][0]; tmp_p[3] = pairings[i][2][1]; 
      tmp_p[4] = pairings[i][0][0]; tmp_p[5] = pairings[i][0][1];
    } 
    else if ((pTHc >= pTHa) && (pTHa >= pTHb)) {
      pTH1 = pTHc; pTH2 = pTHa; pTH3 = pTHb; mH1 = mHc; mH2 = mHa; mH3 = mHb;
      tmp_p[0] = pairings[i][2][0]; tmp_p[1] = pairings[i][2][1]; 
      tmp_p[2] = pairings[i][0][0]; tmp_p[3] = pairings[i][0][1]; 
      tmp_p[4] = pairings[i][1][0]; tmp_p[5] = pairings[i][1][1];      
    } 
    else {
      pTH1 = pTHc; pTH2 = pTHb; pTH3 = pTHa; mH1 = mHc; mH2 = mHb; mH3 = mHa;
      tmp_p[0] = pairings[i][2][0]; tmp_p[1] = pairings[i][2][1]; 
      tmp_p[2] = pairings[i][1][0]; tmp_p[3] = pairings[i][1][1]; 
      tmp_p[4] = pairings[i][0][0]; tmp_p[5] = pairings[i][0][1];
    } 
    double tmp_score = 1./(std::abs(mH1 - 120.) + std::abs(mH2 - 115.) + std::abs(mH3 - 110.));
    if (tmp_score > pair_score) {
        pair_score = tmp_score;
        pairs = tmp_p;
    }
  }


}

void Atlas_2411_02040::finalize() {
  // Whatever should be done after the run goes here
}       


bool Atlas_2411_02040::sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }

std::vector<Jet*> Atlas_2411_02040::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
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


bool Atlas_2411_02040::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}
