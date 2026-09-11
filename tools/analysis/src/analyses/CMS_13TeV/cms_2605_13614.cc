#include "cms_2605_13614.h"
#ifdef HAVE_ONNX
#include "onnxruntime_cxx_api.h"
#endif

// AUTHOR: Krzysztof Rolbiecki
//  EMAIL: krolb@fuw.edu.pl

void Cms_2605_13614::initialize() {
  setAnalysisName("cms_2605_13614");          
  setInformation(""
    "# Inert doublet IDM search in Z+MET\n"
  "");
  setLuminosity(138.0*units::INVFB);      
  bookSignalRegions("SR_presel");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here

#ifdef HAVE_ONNX  
  Ort::AllocatorWithDefaultOptions allocator;
  std::vector<const char*> input_node_names;
  std::vector<const char*> output_node_names;
  std::vector<int64_t> input_node_dims;
  char *a = Global::get_maindir();
  std::string maindir(a, strlen(a));
  std::string file = maindir  + std::string("/data/cms_2605_13614/pnn_model.onnx");
  

  Ort::SessionOptions session_options;
  session = new Ort::Session(env, file.c_str(), session_options);
  
  const size_t num_input_nodes = session->GetInputCount();
  cout << "Number input nodes: " << num_input_nodes << endl;
  input_node_names.reserve(num_input_nodes);    
  
  for (size_t i = 0; i < num_input_nodes; i++) {
    auto type_info = session->GetInputTypeInfo(i);
    auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
    auto input_name = session->GetInputNameAllocated(i, allocator);
    std::cout << "Input " << i << " : name = " << input_name.get() << std::endl;
    input_node_names.push_back(input_name.get());
    //input_names.push_back(input_name.get());
    ONNXTensorElementDataType type = tensor_info.GetElementType(); 
    std::cout << "Input " << i << " : type = " << type << std::endl;
    input_node_dims = tensor_info.GetShape();
    std::cout << "Input " << i << " : num_dims = " << input_node_dims.size() << '\n';
    for (size_t j = 0; j < input_node_dims.size(); j++) 
        std::cout << "Input " << i << " : dim[" << j << "] = " << input_node_dims[j] << '\n';
  }   // input_tensor_size = input_node_dims[0] * input_node_dims[1] * .... 
  
  //asume just 1 output node 
  
  const size_t num_output_nodes = session->GetOutputCount();
  cout << "Number output nodes: " << num_output_nodes << endl;
  output_node_names.reserve(num_output_nodes);
  auto output_name = session->GetOutputNameAllocated(0, allocator);
  std::cout << "Output " << "0" << " : name = " << output_name.get() << std::endl;
  output_node_names.push_back(output_name.get());
  auto type_info = session->GetOutputTypeInfo(0);
  auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
  ONNXTensorElementDataType type = tensor_info.GetElementType(); 
  std::cout << "Output 0 : type = " << type << std::endl;
  std::vector<int64_t> output_node_dims = tensor_info.GetShape();
  std::cout << "Output " << 0 << " : num_dims = " << output_node_dims.size() << '\n';
  for (size_t j = 0; j < input_node_dims.size(); j++) 
    std::cout << "Output " << 0 << " : dim[" << j << "] = " << output_node_dims[j] << '\n';  
#endif  

float mH; float mA;
for (const auto year : years) {
  for (const auto& [mH, mA] : mHmA) {
    std::ostringstream ss;
    ss << "mH" << std::fixed << std::setprecision(1) << mH << "_mA" << mA << "_" << year << "_EE_bin_" ;
    bookControlRegions(ss.str() + "0");
    bookControlRegions(ss.str() + "1");
    bookControlRegions(ss.str() + "2");
    bookControlRegions(ss.str() + "3");
    bookControlRegions(ss.str() + "4");
    bookControlRegions(ss.str() + "5");
    bookControlRegions(ss.str() + "6");

    ss.str("");
    ss.clear();
    ss << "mH" << std::fixed << std::setprecision(1) << mH << "_mA" << mA << "_" << year << "_MM_bin_" ;
    bookControlRegions(ss.str() + "00");
    bookControlRegions(ss.str() + "01");
    bookControlRegions(ss.str() + "02");
    bookControlRegions(ss.str() + "03");
    bookControlRegions(ss.str() + "04");
    bookControlRegions(ss.str() + "05");
    bookControlRegions(ss.str() + "06");
    bookControlRegions(ss.str() + "07");
    bookControlRegions(ss.str() + "08");
    bookControlRegions(ss.str() + "09");
    bookControlRegions(ss.str() + "10");
  }
}  
  /*int ifile = bookFile("atlas_2211_08028.root", true);
  const char *rootFileName = fNames[ifile].c_str() ;
  hfile = new TFile(rootFileName, "RECREATE", "Saving Histograms");*/

}

void Cms_2605_13614::analyze() {
 
  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out. Probably not since 3.4.2
  
  std::string year = "1900";
  double y = rand()/(RAND_MAX+1.);
  if (y < 0.253) year = "2016";
  else if (y > 0.531) year = "2018";
  else year = "2017";
  
  countCutflowEvent("00_all");
  
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.5, 2.5);
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.5, 2.5, true);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  electronsLoose = filterIsolation(electronsLoose, 0);
  electronsTight = filterIsolation(electronsTight, 0);
  muonsCombined = filterIsolation(muonsCombined, 0);
  jets = filterPhaseSpace(jets, 20., -2.4, 2.4);
  
  jets = overlapRemoval(jets, electronsLoose, 0.4);
  jets = overlapRemoval(jets, muonsCombined, 0.4);
  electronsLoose = overlapRemoval(electronsLoose, jets, 0.4);
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  
  std::vector<Electron*> electronsSignal = filterPhaseSpace(electronsTight, 20., -2.5, 2.5);
  std::vector<Muon*> muonsSignal = filterPhaseSpace(muonsCombined, 20., -2.4, 2.4);
  std::vector<Jet*> jetsSignal = filterPhaseSpace(jets, 30., -2.4, 2.4);
  if (electronsSignal.size() + muonsSignal.size() < 2) return;
  countCutflowEvent("01_2leptons");
  
  bool preselection = false;
  if (year == "2016")
    {if ( (electronsSignal.size() and electronsSignal[0]->PT > 29.) or (muonsSignal.size() and muonsSignal[0]->PT > 26.) ) preselection = true;}
  else if (year == "2017")   
    {if ( (electronsSignal.size() and electronsSignal[0]->PT > 34.) or (muonsSignal.size() and muonsSignal[0]->PT > 29.) ) preselection = true;}
  else if (year == "2018")   
    {if ( (electronsSignal.size() and electronsSignal[0]->PT > 32.) or (muonsSignal.size() and muonsSignal[0]->PT > 26.) ) preselection = true;}  
    
  if (!preselection) return;
  countCutflowEvent("02_leadlepton");
  
  std::vector<FinalStateObject*> leptons;
  for ( int i = 0; i <  electronsSignal.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(electronsSignal[i]);
    leptons.push_back(lep);
    //cout << "e " ;
  }
  for ( int i = 0; i < muonsSignal.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(muonsSignal[i]);
    leptons.push_back(lep);
    //cout << "mu " ;
  }
  std::sort(leptons.begin(), leptons.end(), FinalStateObject::sortByPT);

  double mll = (leptons[0]->P4() + leptons[1]->P4()).M();
  double ptll = (leptons[0]->P4() + leptons[1]->P4()).Pt();
  double dphimetll = fabs(missingET->P4().DeltaPhi(leptons[0]->P4() + leptons[1]->P4()));
  double dphill = cos(leptons[0]->P4().DeltaPhi(leptons[1]->P4()));
  double ptmiss = missingET->PT;

  if (mll < 12. or mll > 80. or ptll < 15. or dphill < -0.75 or dphimetll < 1. or jetsSignal.size() > 1) return;
  countCutflowEvent("03_presel");

  if (leptons.size() < 2 || leptons[0]->Charge * leptons[1]->Charge > 0 || leptons[0]->Type != leptons[1]->Type) return;  
  countCutflowEvent("04_OSSF");
  if (muonsCombined.size() + electronsLoose.size() > 2) return;
  countCutflowEvent("05_3l_veto");

  std::string flavour = "??";
  if (leptons[0]->Type ==  "electron") flavour = "EE";
  else if (leptons[0]->Type ==  "muon") flavour = "MM";

  double mt2ll0 = mT2(leptons[0]->P4(), leptons[1]->P4(), 0., missingET->P4(), false);
  double mt2ll80 = mT2(leptons[0]->P4(), leptons[1]->P4(), 80., missingET->P4(), false);
  double mtll = mT(leptons[0]->P4() + leptons[1]->P4(), missingET->P4());
  double mtllwE = mTwE(leptons[0]->P4() + leptons[1]->P4(), missingET->P4(), mll);

  TLorentzVector pll = leptons[0]->P4() + leptons[1]->P4();
  double JZB = (missingET->P4().Vect() + pll.Vect()).Perp() - pll.Perp();
#ifdef HAVE_ONNX  
//"Dilepton_JZB","Dilepton_LeadEta","Dilepton_LeadPT","Dilepton_LeadPhi","Dilepton_MT","Dilepton_MT2_0","Dilepton_MT2_80","Dilepton_MT_wE","Dilepton_SubleadEta","Dilepton_SubleadPT","Dilepton_SubleadPhi","Dilepton_dR","Dilepton_eta","Dilepton_mass","Dilepton_phi","Dilepton_pt","MET_phi","MET_pt","jet_1_eta","jet_1_phi","jet_1_pt","jet_2_eta","jet_2_phi","jet_2_pt","n_jets_20",”run”
  std::vector<float> x_values;  
  x_values.push_back(JZB);
  x_values.push_back(leptons[0]->Eta);
  x_values.push_back(leptons[0]->PT);
  x_values.push_back(leptons[0]->Phi);
  x_values.push_back(mtll);
  x_values.push_back(mt2ll0);
  x_values.push_back(mt2ll80);
  x_values.push_back(mtllwE);
  x_values.push_back(leptons[1]->Eta);
  x_values.push_back(leptons[1]->PT);
  x_values.push_back(leptons[1]->Phi);
  x_values.push_back(jets.size() > 0 ? fabs(jets[0]->P4().DeltaPhi(missingET->P4())) : 0.);
  x_values.push_back(jets.size() > 0 ? fabs(jets[0]->P4().DeltaPhi(pll)) : 0.);
  x_values.push_back(leptons[0]->P4().DeltaR(leptons[1]->P4()));
  x_values.push_back(pll.Eta());
  x_values.push_back(pll.M());
  x_values.push_back(pll.Phi());
  x_values.push_back(pll.Pt());
  x_values.push_back(missingET->Phi);
  x_values.push_back(missingET->PT);
  x_values.push_back(jets.size() > 0 ? jets[0]->Eta : 0.); 
  x_values.push_back(jets.size() > 0 ? jets[0]->Phi : 0.);
  x_values.push_back(jets.size() > 0 ? jets[0]->PT : 0.);
  x_values.push_back(jets.size() > 1 ? jets[1]->Eta : 0.);
  x_values.push_back(jets.size() > 1 ? jets[1]->Phi : 0.);
  x_values.push_back(jets.size() > 1 ? jets[1]->PT : 0.);
  x_values.push_back(jets.size());
  x_values.push_back(0);
  
  assert(x_values.size() == 28);

  auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

  for (const auto& [mH, mA] : mHmA) {  
    std::vector<float> masses_values;
    masses_values.push_back(70.); //mH
    masses_values.push_back(160.); //mA
    auto input_tensor_x = Ort::Value::CreateTensor(memory_info, x_values.data(), input_tensor_size_x, input_dims_x.data(), 2); //rank = 2
    auto input_tensor_masses = Ort::Value::CreateTensor(memory_info, masses_values.data(), input_tensor_size_masses, input_dims_masses.data(), 2); //rank = 2
  
    std::array<Ort::Value, 2> input_tensors = {std::move(input_tensor_x), std::move(input_tensor_masses)};

    const float* check = input_tensors[0].GetTensorData<float>();
    for (size_t i = 0; i < 28; ++i) cout << check[i] << " ";

    auto output_tensors = session->Run(Ort::RunOptions{nullptr}, input_names.data(), input_tensors.data(), 2, output_names.data(), 1);
    
    float* output = output_tensors[0].GetTensorMutableData<float>();

    float result = output[0];
    cout << "result: " << result << endl;
    if (result < 0.9) return;
    std::ostringstream ss;
    ss << "mH" << std::fixed << std::setprecision(1) << mH << "_mA" << mA << "_" << year << "_" << flavour; 
    countCutflowEvent("06_pNN>0.9_" + ss.str());
    
    if (flavour == "EE") {
      if (result < electron_bins[1]) countControlEvent(ss.str() + "_bin_0");
      else if (result < electron_bins[2]) countControlEvent(ss.str() + "_bin_1");
      else if (result < electron_bins[3]) countControlEvent(ss.str() + "_bin_2");
      else if (result < electron_bins[4]) countControlEvent(ss.str() + "_bin_3");
      else if (result < electron_bins[5]) countControlEvent(ss.str() + "_bin_4");
      else if (result < electron_bins[6]) countControlEvent(ss.str() + "_bin_5");
      else countControlEvent(ss.str() + "_bin_6");
    }
    if (flavour == "MM") {
      if (result < muon_bins[1]) countControlEvent(ss.str() + "_bin_00");
      else if (result < muon_bins[2]) countControlEvent(ss.str() + "_bin_01");
      else if (result < muon_bins[3]) countControlEvent(ss.str() + "_bin_02");
      else if (result < muon_bins[4]) countControlEvent(ss.str() + "_bin_03");
      else if (result < muon_bins[5]) countControlEvent(ss.str() + "_bin_04");
      else if (result < muon_bins[6]) countControlEvent(ss.str() + "_bin_05");
      else if (result < muon_bins[7]) countControlEvent(ss.str() + "_bin_06");
      else if (result < muon_bins[8]) countControlEvent(ss.str() + "_bin_07");
      else if (result < muon_bins[9]) countControlEvent(ss.str() + "_bin_08");
      else if (result < muon_bins[10]) countControlEvent(ss.str() + "_bin_09");
      else countControlEvent(ss.str() + "_bin_10");
    }
    //countSignalEvent("SR_presel");
  }

#endif

return;
}

void Cms_2605_13614::finalize() {
  // Whatever should be done after the run goes here

#ifdef HAVE_ONNX 
  delete session;
#endif  
}       
