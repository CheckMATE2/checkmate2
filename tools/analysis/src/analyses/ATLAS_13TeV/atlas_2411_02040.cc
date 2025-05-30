#include "atlas_2411_02040.h"
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
#ifdef HAVE_ONNX
#include "onnxruntime_cxx_api.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
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
    maindir  + "/data/atlas_2411_02040/ANA-HIGP-2024-32_nonresDNN_model_even",
    maindir  + "/data/atlas_2411_02040/ANA-HIGP-2024-32_nonresDNN_model_odd",
    maindir  + "/data/atlas_2411_02040/ANA-HIGP-2024-32_resDNN_model_even",
    maindir  + "/data/atlas_2411_02040/ANA-HIGP-2024-32_resDNN_model_odd",
    maindir  + "/data/atlas_2411_02040/ANA-HIGP-2024-32_heavyresDNN_model_even",
    maindir  + "/data/atlas_2411_02040/ANA-HIGP-2024-32_heavyresDNN_model_odd"};
  Ort::SessionOptions session_options[6];
  for (int i = 0; i < 6; i++) {
    std::ifstream input_file((onnx_files[i] + "_variables.json").c_str());
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(input_file, pt);
    std::vector<double> offset = read_tree<double>(pt, "offset");
    std::vector<double> scale = read_tree<double>(pt, "scale");
    scales.push_back(scale);
    offsets.push_back(offset);
    session[i] = new Ort::Session(env[i], (onnx_files[i] + ".onnx").c_str(), session_options[i]);
  }

  const size_t num_input_nodes = session[0]->GetInputCount();
  cout << "Number input nodes: " << num_input_nodes << endl;
  input_node_names.reserve(num_input_nodes);

  for (size_t k = 0; k < 6; k++) {
  for (size_t i = 0; i < num_input_nodes; i++) {
    auto type_info = session[k]->GetInputTypeInfo(i);
    auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
    auto input_name = session[k]->GetInputNameAllocated(i, allocator);
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
  }
  //asume just 1 output node

  for (size_t i = 0; i < 6; i++) {
    const size_t num_output_nodes = session[i]->GetOutputCount();
    cout << "Number output nodes: " << num_output_nodes << endl;
    output_node_names.reserve(num_output_nodes);
    auto output_name = session[i]->GetOutputNameAllocated(0, allocator);
    std::cout << "Output " << "0" << " : name = " << output_name.get() << std::endl;
    output_node_names.push_back(output_name.get());
    auto type_info = session[i]->GetOutputTypeInfo(0);
    auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
    std::vector<int64_t> output_node_dims = tensor_info.GetShape();
    std::cout << "Output " << 0 << " : num_dims = " << output_node_dims.size() << '\n';
    for (size_t j = 0; j < input_node_dims.size(); j++)
      std::cout << "Output " << 0 << " : dim[" << j << "] = " << output_node_dims[j] << '\n';
  }
#endif

  int ifile = bookFile("atlas_2411_02040.root", true);
  const char *rootFileName = fNames[ifile].c_str() ;
  hfile = new TFile(rootFileName, "RECREATE", "Saving Histograms");

  hist_mhradius = new TH1F("mHradius", "mHradius", 25, 0., 250.);
  hist_rmsdeltarjj = new TH1F("RMSdeltaRjj", "RMSdeltaRjj", 20, 0., 2.);
  hist_rmsdeltarjjrivet = new TH1F("RMSdeltaRjj_rivet", "RMSdeltaRjj", 20, 0., 2.);
  hist_rmsmjj = new TH1F("RMSmjj", "RMSmjj", 20, 0., 400.);
  hist_rmsetah = new TH1F("RMSetaH", "RMSetaH", 20, 0., 2.);
  hist_rmsdeltaajj = new TH1F("RMSdeltaAjj", "RMSdeltaAjj", 20, -2., 4.);
  hist_rmsdeltaajjrivet = new TH1F("RMSdeltaAjj_rivetBin", "RMSdeltaAjj_rivetBin", 22, -1., 3.5);
  hist_deltarh1 = new TH1F("deltaRH1", "deltaRH1", 25, 0., 5.);
  hist_deltarh2 = new TH1F("deltaRH2", "deltaRH2", 25, 0., 5.);
  hist_deltarh3 = new TH1F("deltaRH3", "deltaRH3", 25, 0., 5.);
  hist_massh1 = new TH1F("massH1", "massH1", 17, 0., 350.);
  hist_massh1rivet = new TH1F("massH1_rivetBin", "massH1_rivetBin", 20, 0., 400.);
  hist_ht6j = new TH1F("HT6j", "HT6j", 20, 0., 1500.);
  hist_ht6jrivet = new TH1F("HT6j_rivetBin", "HT6j_rivetBin", 28, 0., 1600.);
  hist_etamhhhfrac = new TH1F("eta_mHHH_frac", "eta_mHHH_frac", 20, 0., 1.);
  hist_costheta = new TH1F("cosTheta", "cosTheta", 20, -1., 1.);
  hist_aplanarity6j = new TH1F("Aplanarity_6j", "Aplanarity_6j", 20, 0., 0.4);
  hist_sphericityallj = new TH1F("Sphericity_allj", "Sphericity_allj", 20, 0., 1.);
  hist_sphericity6j = new TH1F("Sphericity_6j", "Sphericity_6j", 20, 0., 1.);
  hist_transvsphericty6j = new TH1F("Transverse_Sphericity_6j", "Transverse_Sphericity_6j", 20, 0., 1.);

  hist_resdnnscore = new TH1F("resDNN_Score", "resDNN_Score", 20., 0., 1.);
  hist_nonresdnnscore = new TH1F("nonresDNN_Score", "nonresDNN_Score", 20., 0., 1.);
  hist_heavyresdnnscore = new TH1F("heavyresDNN_Score", "heavyresDNN_Score", 20., 0., 1.);

  float binning[7] = {0.5, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5};
  hist_weight = new TH1F("TotalWeight", "TotalWeight", 6, binning);

  eventNumber = -1;
}

void Atlas_2411_02040::analyze() {

  eventNumber++;
  int cutflow = 0;
  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out. Probably not since 3.4.2
  
  electronsLoose = filterPhaseSpace(electronsLoose, 20, -2.47, 2.47);
  electronsLoose = filterIsolation(electronsLoose);
  muonsCombined = filterPhaseSpace(muonsCombined, 20, -2.5, 2.5);
  muonsCombined = filterIsolation(muonsCombined);
  jets = filterPhaseSpace(jets, 20., -2.5, 2.5);
  
  std::vector<Jet*> nonbjets={};
  std::vector<Jet*> bjets={};
  std::vector<Jet*> bjets_truth={};
  for (int i = 0; i < jets.size(); i++) {
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i], 0) and jets[i]->PT > 25. )
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
  cutflow++;
  hist_weight->Fill(cutflow, weight);

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
  cutflow++;
  hist_weight->Fill(cutflow, weight);
  
  if (bjets.size() + nonbjets.size() < 6) return;
  countCutflowEvent("02_6jets");
  cutflow++;
  hist_weight->Fill(cutflow, weight);
  
  if (bjets40.size() + nonbjets40.size() < 4) return;
  countCutflowEvent("03_4*pt>40");
  cutflow++;
  hist_weight->Fill(cutflow, weight);
  
  if (bjets40.size() < 4) return;
  countCutflowEvent("04_4b*pt>40");
  cutflow++;
  hist_weight->Fill(cutflow, weight);
  
  bool bjets4 = false;
  bool bjets5 = false;
  bool bjets6 = false;
  std::vector<Jet*> sigjets = {};
  if (bjets.size() == 4) {bjets4 = true; countCutflowEvent("05_4b"); return;} //dropping these events for now
  else if (bjets.size() == 5) {
    bjets5 = true; countCutflowEvent("05_5b");
    sigjets = bjets;
    countSignalEvent("CR_5b");
    //sigjets.push_back(nonbjets[0]);
    return;
  }
  else {
    bjets6 = true; 
    countCutflowEvent("05_6b");
    for (int i = 0; i < 6; i++) sigjets.push_back(bjets[i]);
    cutflow++;
    hist_weight->Fill(cutflow, weight);
  }
  
  double pair_score = -1.;
  std::vector<size_t> pairs = {10, 10, 10, 10, 10, 10};
  double pTH1, pTH2, pTH3, mH1, mH2, mH3;
  for (int i = 0; i < 15; i++) {
    std::vector<size_t> tmp_p (6, 0);
    double pTHa = (sigjets[pairings[i][0][0]-1]->P4() + sigjets[pairings[i][0][1]-1]->P4()).Pt();
    double mHa = (sigjets[pairings[i][0][0]-1]->P4() + sigjets[pairings[i][0][1]-1]->P4()).M();
    double pTHb = (sigjets[pairings[i][1][0]-1]->P4() + sigjets[pairings[i][1][1]-1]->P4()).Pt();
    double mHb = (sigjets[pairings[i][1][0]-1]->P4() + sigjets[pairings[i][1][1]-1]->P4()).M();
    double pTHc = (sigjets[pairings[i][2][0]-1]->P4() + sigjets[pairings[i][2][1]-1]->P4()).Pt();
    double mHc = (sigjets[pairings[i][2][0]-1]->P4() + sigjets[pairings[i][2][1]-1]->P4()).M();
    if ((pTHa >= pTHb) and (pTHb >= pTHc)) {
      pTH1 = pTHa; pTH2 = pTHb; pTH3 = pTHc; mH1 = mHa; mH2 = mHb; mH3 = mHc;
      tmp_p[0] = pairings[i][0][0]-1; tmp_p[1] = pairings[i][0][1]-1;
      tmp_p[2] = pairings[i][1][0]-1; tmp_p[3] = pairings[i][1][1]-1;
      tmp_p[4] = pairings[i][2][0]-1; tmp_p[5] = pairings[i][2][1]-1;
    } 
    else if ((pTHa >= pTHc) and (pTHc >= pTHb)) {
      pTH1 = pTHa; pTH2 = pTHc; pTH3 = pTHb; mH1 = mHa; mH2 = mHc; mH3 = mHb;
      tmp_p[0] = pairings[i][0][0]-1; tmp_p[1] = pairings[i][0][1]-1;
      tmp_p[2] = pairings[i][2][0]-1; tmp_p[3] = pairings[i][2][1]-1;
      tmp_p[4] = pairings[i][1][0]-1; tmp_p[5] = pairings[i][1][1]-1;
    } 
    else if ((pTHb >= pTHa) && (pTHa >= pTHc)) {
      pTH1 = pTHb; pTH2 = pTHa; pTH3 = pTHc; mH1 = mHb; mH2 = mHa; mH3 = mHc;
      tmp_p[0] = pairings[i][1][0]-1; tmp_p[1] = pairings[i][1][1]-1;
      tmp_p[2] = pairings[i][0][0]-1; tmp_p[3] = pairings[i][0][1]-1;
      tmp_p[4] = pairings[i][2][0]-1; tmp_p[5] = pairings[i][2][1]-1;
    } 
    else if ((pTHb >= pTHc) && (pTHc >= pTHa)) {
      pTH1 = pTHb; pTH2 = pTHc; pTH3 = pTHa; mH1 = mHb; mH2 = mHc; mH3 = mHa;
      tmp_p[0] = pairings[i][1][0]-1; tmp_p[1] = pairings[i][1][1]-1;
      tmp_p[2] = pairings[i][2][0]-1; tmp_p[3] = pairings[i][2][1]-1;
      tmp_p[4] = pairings[i][0][0]-1; tmp_p[5] = pairings[i][0][1]-1;
    } 
    else if ((pTHc >= pTHa) && (pTHa >= pTHb)) {
      pTH1 = pTHc; pTH2 = pTHa; pTH3 = pTHb; mH1 = mHc; mH2 = mHa; mH3 = mHb;
      tmp_p[0] = pairings[i][2][0]-1; tmp_p[1] = pairings[i][2][1]-1;
      tmp_p[2] = pairings[i][0][0]-1; tmp_p[3] = pairings[i][0][1]-1;
      tmp_p[4] = pairings[i][1][0]-1; tmp_p[5] = pairings[i][1][1]-1;
    } 
    else {
      pTH1 = pTHc; pTH2 = pTHb; pTH3 = pTHa; mH1 = mHc; mH2 = mHb; mH3 = mHa;
      tmp_p[0] = pairings[i][2][0]-1; tmp_p[1] = pairings[i][2][1]-1;
      tmp_p[2] = pairings[i][1][0]-1; tmp_p[3] = pairings[i][1][1]-1;
      tmp_p[4] = pairings[i][0][0]-1; tmp_p[5] = pairings[i][0][1]-1;
    } 
    double tmp_score = 1./(std::abs(mH1 - 120.) + std::abs(mH2 - 115.) + std::abs(mH3 - 110.));
    if (tmp_score > pair_score) {
        pair_score = tmp_score;
        pairs = tmp_p;
    }
  }

  //cout << eventNumber << ": " << sigjets.size() << "  ";
  //for (auto i: pairs)
  //  std::cout << i << "  " << sigjets[i]->PT << "  " ;
  //cout << endl;
  float HT6j = 0.;
  TLorentzVector Hmom = {0.,0.,0.,0.};
  for (int i = 0; i < 6; i++ ) {
    HT6j += sigjets[i]->PT;
    Hmom += sigjets[i]->P4();
    //cout << sigjets[i]->PT << "  ";
  }
  //cout << endl;
  float massHHH = Hmom.M();
  float deltaRH1 = sigjets[pairs[0]]->P4().DeltaR(sigjets[pairs[1]]->P4());
  float deltaRH2 = sigjets[pairs[2]]->P4().DeltaR(sigjets[pairs[3]]->P4());
  float deltaRH3 = sigjets[pairs[4]]->P4().DeltaR(sigjets[pairs[5]]->P4());

  float ptH1 = (sigjets[pairs[0]]->P4() + sigjets[pairs[1]]->P4()).Pt();
  float ptH2 = (sigjets[pairs[2]]->P4() + sigjets[pairs[3]]->P4()).Pt();
  float ptH3 = (sigjets[pairs[4]]->P4() + sigjets[pairs[5]]->P4()).Pt();

  float etaH1 = (sigjets[pairs[0]]->P4() + sigjets[pairs[1]]->P4()).Eta();
  float etaH2 = (sigjets[pairs[2]]->P4() + sigjets[pairs[3]]->P4()).Eta();
  float etaH3 = (sigjets[pairs[4]]->P4() + sigjets[pairs[5]]->P4()).Eta();
  //float RMSetaH = getRMS({etaH1, etaH2, etaH3});
  float RMSetaH = getRMS({sigjets[pairs[0]]->Eta, sigjets[pairs[1]]->Eta, sigjets[pairs[2]]->Eta, sigjets[pairs[3]]->Eta, sigjets[pairs[4]]->Eta, sigjets[pairs[5]]->Eta});

  float massH1 = (sigjets[pairs[0]]->P4() + sigjets[pairs[1]]->P4()).M();
  float massH2 = (sigjets[pairs[2]]->P4() + sigjets[pairs[3]]->P4()).M();
  float massH3 = (sigjets[pairs[4]]->P4() + sigjets[pairs[5]]->P4()).M();
  float mHradius = sqrt(pow(massH1 - 120., 2) + pow(massH2 - 115., 2) + pow(massH3 - 110., 2) );

  //std::vector<float> distance_pair_center = {massH1 - 120., massH2 - 115., massH3 - 110.};
  //std::vector<float> unit = {1./sqrt(3), 1./sqrt(3), 1./sqrt(3)};
  //float cosTheta = std::inner_product(std::begin(distance_pair_center), std::end(distance_pair_center), std::begin(unit), 0.)/mHradius;
  float cosTheta = (massH1 - 120. + massH2 - 115. + massH3 - 110.)/mHradius/sqrt(3);

  std::vector<float> deltaAjjs, mjjs, deltaRjjs;
  float sumpTcosh = 0.;
  for (int i = 0; i < 6; i++)
    for (int j = i + 1; j < 6; j++){
      mjjs.push_back((sigjets[i]->P4() + sigjets[j]->P4()).M());
      deltaRjjs.push_back(sigjets[i]->P4().DeltaR(sigjets[j]->P4()));
      deltaAjjs.push_back(cosh(sigjets[i]->Eta - sigjets[j]->Eta) - cos(sigjets[i]->P4().DeltaPhi(sigjets[j]->P4())));
      sumpTcosh += 2 * sigjets[i]->PT * sigjets[j]->PT * (cosh(sigjets[i]->Eta - sigjets[j]->Eta) - 1.);
  }

  float RMSmjj = getRMS(mjjs);
  float RMSdeltaRjj = getRMS(deltaRjjs);
  float RMSdeltaAjj = getSkewness(deltaAjjs);
  float eta_mHHH_frac = sumpTcosh/std::pow(massHHH, 2);

  TLorentzVector hhh = {0., 0., 0., 0.};
  for (int i = 0; i < 6; i++) 
    hhh += sigjets[i]->P4();

  std::vector<TLorentzVector> signal_boosted;
  for (int i = 0; i < 6; i++) {
    TLorentzVector temp = sigjets[i]->P4();
    temp.Boost(-hhh.BoostVector());
    signal_boosted.push_back(temp);
  }

  //std::vector<float> spheraplan_6j = Aplan_spher(sigjets, 1);
  std::vector<float> spheraplan_6j = Aplan_spher_P4(signal_boosted, 1);
  std::vector<float> spheraplan_allj = Aplan_spher(jets, 1);
  //std::vector<float> transpher_6j = Aplan_spher(sigjets, 2);
  std::vector<float> transpher_6j = Aplan_spher_P4(signal_boosted, 2);

  float Aplanarity_6j = spheraplan_6j[0];
  float Sphericity_6j = spheraplan_6j[1];
  float Sphericity_allj = spheraplan_allj[1];
  float Transverse_Sphericity_6j = transpher_6j[2];

  float resDNN_Score = getNN({HT6j, RMSdeltaRjj, deltaRH2, eta_mHHH_frac, Sphericity_6j, Aplanarity_6j, deltaRH1, deltaRH3, RMSdeltaAjj, cosTheta},"res");
  float nonresDNN_Score = getNN({mHradius, RMSmjj, RMSdeltaRjj, deltaRH1, RMSetaH, massH1, deltaRH2, deltaRH3, Aplanarity_6j, Transverse_Sphericity_6j},"nonres");
  float heavyresDNN_Score = getNN({mHradius, RMSdeltaRjj, RMSmjj, deltaRH1, deltaRH2, deltaRH3, massH1, RMSetaH, Aplanarity_6j, Sphericity_allj},"heavyres");

  hist_mhradius->Fill(mHradius, weight);
  hist_rmsdeltarjj->Fill(RMSdeltaRjj, weight);
  hist_rmsdeltarjjrivet->Fill(RMSdeltaRjj, weight);
  hist_rmsmjj->Fill(RMSmjj, weight);
  hist_rmsetah->Fill(RMSetaH, weight);
  hist_rmsdeltaajj->Fill(RMSdeltaAjj, weight);
  hist_rmsdeltaajjrivet->Fill(RMSdeltaAjj, weight);
  hist_deltarh1->Fill(deltaRH1, weight);
  hist_deltarh2->Fill(deltaRH2, weight);
  hist_deltarh3->Fill(deltaRH3, weight);
  hist_massh1->Fill(massH1, weight);
  hist_massh1rivet->Fill(massH1, weight);
  hist_ht6j->Fill(HT6j, weight);
  hist_ht6jrivet->Fill(HT6j, weight);
  hist_etamhhhfrac->Fill(eta_mHHH_frac, weight);
  hist_costheta->Fill(cosTheta, weight);
  hist_aplanarity6j->Fill(Aplanarity_6j, weight);
  hist_sphericityallj->Fill(Sphericity_allj, weight);
  hist_sphericity6j->Fill(Sphericity_6j, weight);
  hist_transvsphericty6j->Fill(Transverse_Sphericity_6j);
  hist_resdnnscore->Fill(resDNN_Score, weight);
  hist_nonresdnnscore->Fill(nonresDNN_Score, weight);
  hist_heavyresdnnscore->Fill(heavyresDNN_Score, weight);

  return;

}

void Atlas_2411_02040::finalize() {
  // Whatever should be done after the run goes here
  hfile->Write();
  hfile->Close();
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

std::vector<float> Atlas_2411_02040::Aplan_spher_P4(std::vector<TLorentzVector> input_jets, int r) {

  float mag = 0.;
  for (int k = 0; k < input_jets.size(); k++)
    mag += pow(input_jets[k].Rho(), r);

  TMatrixD st(TMatrixD::kZero, TMatrixD(3,3) );
  for (int k = 0; k < input_jets.size(); k++) {
    float weight = std::pow(input_jets[k].Rho(), r - 2);
    st(0,0) += input_jets[k].X()*input_jets[k].X()*weight;
    st(0,1) += input_jets[k].X()*input_jets[k].Y()*weight;
    st(0,2) += input_jets[k].X()*input_jets[k].Z()*weight;
    st(1,1) += input_jets[k].Y()*input_jets[k].Y()*weight;
    st(1,2) += input_jets[k].Y()*input_jets[k].Z()*weight;
    st(2,2) += input_jets[k].Z()*input_jets[k].Z()*weight;
  }
  st(1,0) = st(0,1);
  st(2,0) = st(0,2);
  st(2,1) = st(1,2);

  st *= 1./mag;
  //cout << "st matrix for r: "<< r << endl;
  //cout << "mag: " << mag << endl;
  //cout << st(0,0) << "  " << st(0,1) << "  " << st(0,2) << "  " << st(1,1) << "  " << st(1,2) << "  " << st(2,2) << endl;
  TMatrixDEigen eigen(st);
  TMatrixD diag = eigen.GetEigenValues();

  std::vector<float> lambdas;
  lambdas.push_back( diag(0,0) );
  lambdas.push_back( diag(1,1) );
  lambdas.push_back( diag(2,2) );
  std::sort (lambdas.begin(), lambdas.end());
  //cout << lambdas[0] << "  " << lambdas[1] << "  " << lambdas[2] << endl;

  return {1.5*lambdas[0], 1.5*(lambdas[0] + lambdas[1]), 2.*lambdas[1]/(lambdas[1] + lambdas[2])};
}

std::vector<float> Atlas_2411_02040::Aplan_spher(std::vector<Jet*> input_jets, int r) {

  float mag = 0.;
  for (int k = 0; k < input_jets.size(); k++)
    mag += pow(input_jets[k]->P4().Rho(), r);

  TMatrixD st(TMatrixD::kZero, TMatrixD(3,3) );
  for (int k = 0; k < input_jets.size(); k++) {
    float weight = std::pow(input_jets[k]->P4().Rho(), r - 2);
    st(0,0) += input_jets[k]->P4().X()*input_jets[k]->P4().X()*weight;
    st(0,1) += input_jets[k]->P4().X()*input_jets[k]->P4().Y()*weight;
    st(0,2) += input_jets[k]->P4().X()*input_jets[k]->P4().Z()*weight;
    st(1,1) += input_jets[k]->P4().Y()*input_jets[k]->P4().Y()*weight;
    st(1,2) += input_jets[k]->P4().Y()*input_jets[k]->P4().Z()*weight;
    st(2,2) += input_jets[k]->P4().Z()*input_jets[k]->P4().Z()*weight;
  }
  st(1,0) = st(0,1);
  st(2,0) = st(0,2);
  st(2,1) = st(1,2);

  st *= 1./mag;
  //cout << "st matrix for r: "<< r << endl;
  //cout << "mag: " << mag << endl;
  //cout << st(0,0) << "  " << st(0,1) << "  " << st(0,2) << "  " << st(1,1) << "  " << st(1,2) << "  " << st(2,2) << endl;
  TMatrixDEigen eigen(st);
  TMatrixD diag = eigen.GetEigenValues();

  std::vector<float> lambdas;
  lambdas.push_back( diag(0,0) );
  lambdas.push_back( diag(1,1) );
  lambdas.push_back( diag(2,2) );
  std::sort (lambdas.begin(), lambdas.end());
  //cout << lambdas[0] << "  " << lambdas[1] << "  " << lambdas[2] << endl;

  return {1.5*lambdas[0], 1.5*(lambdas[0] + lambdas[1]), 2.*lambdas[1]/(lambdas[1] + lambdas[2])};
}

float Atlas_2411_02040::getRMS(std::vector<float> input) {

  if (input.size() == 0) return 0.;
  float sumsq = 0.;
  float sum = 0.;
  for (auto val: input) {
    sumsq += pow(val,2);
    sum += val;
  }
  float Expx2 = sumsq/input.size();
  float Expx = sum/input.size();
  float rms = 0.;
  if (Expx2 > pow(Expx,2)) rms = sqrt(Expx2 - pow(Expx,2));
  return rms;

}

float Atlas_2411_02040::getSkewness(std::vector<float> input) {

    float offset = 0.;
    float rms = 0.;
    float skewness = 0.;
    int entries = 0.;
    for (size_t i = 0; i < input.size(); i++) {
        offset += input[i];
        rms += pow(input[i],2);
    }
    offset /= input.size();
    rms /= input.size();
    rms = (rms-pow(offset,2)>=0?sqrt(rms-pow(offset,2)):0);
    for (size_t i = 0; i < input.size(); i++) {
        skewness += pow((input[i]-offset)/rms,3);
    }
    skewness /= input.size();
    return skewness;


}

float Atlas_2411_02040::getNN(std::vector<float> input, std::string name) {

  #ifdef HAVE_ONNX

  size_t nn;
  std::vector<const char*> input_name;
  std::vector<const char*> output_name;
  std::vector<double> offset;
  std::vector<double> scale;
  Ort::Session *session_run;
  if ((eventNumber % 2) == 1) {
    input_name = {"input_1"};
    if (name == "res") {
      session_run = session[3];
      offset = offsets[3];
      scale = scales[3];
      nn = 3;
      output_name = {"dense_3"};
    }
    if (name == "nonres") {
      session_run = session[1];
      offset = offsets[1];
      scale = scales[1];
      nn = 1;
      output_name = {"dense_3"};
    }
    if (name == "heavyres") {
      session_run = session[5];
      offset = offsets[5];
      scale = scales[5];
      nn = 5;
      output_name = {"dense_4"};
    }
  }
  else {
    input_name = {"input"};
    if (name == "res") {
      session_run = session[2];
      offset = offsets[2];
      scale = scales[2];
      nn = 2;
      output_name = {"dense_3"};
    }
    if (name == "nonres") {
      session_run = session[0];
      offset = offsets[0];
      scale = scales[0];
      nn = 0;
      output_name = {"dense_3"};
    }
    if (name == "heavyres") {
      session_run = session[4];
      offset = offsets[4];
      scale = scales[4];
      nn = 4;
      output_name = {"dense_4"};
    }
  }

  assert(input.size() == 10);

  std::vector<float> input_data = input;
  for (int i = 0; i < 10; i++)
    input_data[i] = (input[i] - offset[i])/scale[i];


  auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
  auto input_tensor = Ort::Value::CreateTensor(memory_info, input_data.data(), input_tensor_size, input_dims.data(), 2);
  auto output_tensors = session_run->Run(Ort::RunOptions{nullptr}, input_name.data(), &input_tensor, 1, output_name.data(), 1);
  float* output = output_tensors.front().GetTensorMutableData<float>();
  return *output;


  #endif

  return 0;


}

#ifdef HAVE_ONNX

template <typename T>
std::vector<T> Atlas_2411_02040::read_tree(boost::property_tree::ptree const& pt, boost::property_tree::ptree::key_type const& key)
{
    std::vector<T> r;
    for (auto& item : pt.get_child(key))
        r.push_back(item.second.get_value<T>());
    return r;
}
#endif
