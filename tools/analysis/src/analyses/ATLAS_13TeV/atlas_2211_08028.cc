#include "atlas_2211_08028.h"
#include "fastjet/tools/Filter.hh"
#include </home/krolb/tools/CheckMATE/ONNX/tools/onnxruntime-linux-x64-1.12.1/include/onnxruntime_cxx_api.h>
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_2211_08028::initialize() {
  setAnalysisName("atlas_2211_08028");          
  setInformation(""
    "# search for stops and gluinos; at least 3 b-jets 0-1 leptons\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("SR-Gtt-0L-B;SR-Gtt-0L-M1;SR-Gtt-0L-M2;SR-Gtt-0L-C;SR-Gtt-1L-B;SR-Gtt-1L-M1;SR-Gtt-1L-M2;SR-Gtt-1L-C;SR-Gbb-B;SR-Gbb-M;SR-Gbb-C;SR-Gtb-B;SR-Gtb-M;SR-Gtb-C;SR-Gtt-2100-1;SR-Gtt-1800-1;SR-Gtt-2300-1200;SR-Gtt-1900-1400;SR-Gbb-2800-1400;SR-Gbb-2300-1000;SR-Gbb-2100-1600;SR-Gbb-2000-1800");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  
  Ort::AllocatorWithDefaultOptions allocator;
  std::vector<const char*> input_node_names;
  std::vector<const char*> output_node_names;
  std::vector<int64_t> input_node_dims;

  Ort::Env env;
  env = Ort::Env(ORT_LOGGING_LEVEL_WARNING, "GG");

  Ort::SessionOptions session_options;
  session = new Ort::Session(env, "/home/krolb/tools/CheckMATE/ONNX/data/ANA-SUSY-2018-30_model.onnx", session_options);
  
  const size_t num_input_nodes = session->GetInputCount();
  cout << "Number input nodes: " << num_input_nodes << endl;
  input_node_names.reserve(num_input_nodes);    
  
  for (size_t i = 0; i < num_input_nodes; i++) {
    auto type_info = session->GetInputTypeInfo(i);
    auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
    auto input_name = session->GetInputNameAllocated(i, allocator);
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
  
  const size_t num_output_nodes = session->GetOutputCount();
  cout << "Number output nodes: " << num_output_nodes << endl;
  output_node_names.reserve(num_output_nodes);
  auto output_name = session->GetOutputNameAllocated(0, allocator);
  std::cout << "Output " << "0" << " : name = " << output_name.get() << std::endl;
  output_node_names.push_back(output_name.get());
  auto type_info = session->GetOutputTypeInfo(0);
  std::cout << "Input: type = " << type_info << std::endl;
  auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
  std::vector<int64_t> output_node_dims = tensor_info.GetShape();
  std::cout << "Output " << 0 << " : num_dims = " << output_node_dims.size() << '\n';
  for (size_t j = 0; j < input_node_dims.size(); j++) 
    std::cout << "Output " << 0 << " : dim[" << j << "] = " << output_node_dims[j] << '\n';  

  
  int ifile = bookFile("atlas_2211_08028.root", true);
  const char *rootFileName = fNames[ifile].c_str() ;
  hfile = new TFile(rootFileName, "RECREATE", "Saving Histograms");
  nngtt = new TH1F("gtt", "NN score", 20, 0., 1.);  
  nngbb = new TH1F("gbb", "NN score", 20, 0., 1.);  
}

void Atlas_2211_08028::analyze() {

  signal_jets.clear();
  bjets.clear();
  trimmedJets.clear();
  signal_el.clear();
  signal_mu.clear();
  signalLeps.clear();
  pTmiss = TLorentzVector(0.,0.,0.,0.);
  mT_lep = 0.;
  mT_b = 0.;
  meff = 0.;
  MJ = 0.;
  dphimin = 0.;
  
  electronsLoose = filterPhaseSpace(electronsLoose, 20, -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 20, -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 20, -2.5, 2.5);
  photonsLoose = filterPhaseSpace(photonsLoose, 25., -2.37, 2.37, true);
  photonsLoose = filterIsolation(photonsLoose);
    
  jets = filterPhaseSpace(jets, 30., -2.8, 2.8);
  
  std::vector<Jet*> nonbjets;
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i],0) ) 
      bjets.push_back(jets[i]);
    else nonbjets.push_back(jets[i]);  
    
  //cout << electrons.size() + muons.size() << "  " << electronsTight.size() + muonsCombined.size() << "  ";  

  nonbjets = overlapRemoval(nonbjets, electronsLoose, 0.2, "y");      
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.4, "y");      
  electronsTight = overlapRemoval(electronsTight, bjets, 0.4, "y");          
  electronsLoose = overlapRemoval(electronsLoose, nonbjets, 0.4, "y");      
  electronsTight = overlapRemoval(electronsTight, nonbjets, 0.4, "y");      
  nonbjets = overlapRemoval_muon_jet_tracks(nonbjets, muonsCombined, 0.2, 2);   
  bjets = overlapRemoval_muon_jet_tracks(bjets, muonsCombined, 0.2, 2);   
  muonsCombined = overlapRemoval(muonsCombined, nonbjets, 0.4, "y");
  muonsCombined = overlapRemoval(muonsCombined, bjets, 0.4, "y");
  
  signal_jets = bjets;
  signal_jets.insert(signal_jets.end(), nonbjets.begin(), nonbjets.end());
  std::sort(signal_jets.begin(), signal_jets.end(), Atlas_2211_08028::sortByPT ); 
  
  //cout << electronsTight.size() + muonsCombined.size() << "  ";
  signal_el = Isolate_leptons_with_inverse_track_isolation_cone(electronsTight, tracks, towers, 0.2, 10., 0.2, 0.15, 0.2, true);
  signal_mu = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined, tracks, towers, 0.3, 10., 0.2, 0.06, 1., false);
  //cout << signal_el.size() + signal_mu.size() << "  ";
  
  for ( int i = 0; i <  signal_el.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(signal_el[i]);
    signalLeps.push_back(lep);
    //cout << "e " ;
  }
  for ( int i = 0; i < signal_mu.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(signal_mu[i]);
    signalLeps.push_back(lep);
    //cout << "mu " ;
  }
  std::sort(signalLeps.begin(), signalLeps.end(), FinalStateObject::sortByPT);
  //cout << endl;
  
  TLorentzVector pTmiss_soft = TLorentzVector(0., 0., 0., 0.);
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
    bool used = false;
    for (int i = 0; i < signal_el.size(); i++) {
      if ( (*it)->Particle == signal_el[i]->Particle )
        used = true;
        break;      
    }
    for (int i = 0; i < signal_mu.size() && !used; i++) {
      if ( (*it)->Particle == signal_mu[i]->Particle )
        used = true;
        break;      
    }        
    for (int i = 0; i < signal_jets.size() && !used; i++)
      for (int part = 0; part < signal_jets[i]->Particles.GetEntries(); part++) 
        if ((*it)->PT > 0.4 && signal_jets[i]->Particles.At(part) == (*it)->Particle) {
          used = true;
          break;
        }       
    if (!used)  pTmiss_soft -= (*it)->P4();
  }
  
  pTmiss = pTmiss_soft;
  //resolution = 0.;
  for (int i = 0; i < signal_jets.size(); i++) 
    pTmiss -= signal_jets[i]->P4();     
  for (int i = 0; i < signal_el.size(); i++)
    pTmiss -= signal_el[i]->P4();
  for (int i = 0; i < signal_mu.size(); i++)
    pTmiss -= signal_mu[i]->P4();  
  for (int i = 0; i < photonsLoose.size(); i++)
    pTmiss -= photonsLoose[i]->P4();
  
  //pTmiss = missingET->P4();
  
  std::vector<fastjet::PseudoJet> particles;
  for ( int i = 0; i < signal_jets.size(); i++ ) 
    particles.push_back( fastjet::PseudoJet(signal_jets[i]->P4().Px(), signal_jets[i]->P4().Py(), signal_jets[i]->P4().Pz(), signal_jets[i]->P4().E()) );  
  
  fastjet::JetDefinition jet_def08(fastjet::antikt_algorithm, 0.8);  
  fastjet::ClusterSequence cs08(particles, jet_def08);
  std::vector<fastjet::PseudoJet> largeR08 = sorted_by_pt(cs08.inclusive_jets());  
  //std::vector<fastjet::PseudoJet> trimmedJets;
  
  fastjet::Filter trimmer(fastjet::JetDefinition(fastjet::antikt_algorithm, 0.2), fastjet::SelectorPtFractionMin(0.1));  
  
  MJ = 0.;
  for (std::vector<fastjet::PseudoJet>::iterator it = largeR08.begin(); it!=largeR08.end(); it++) {
    fastjet::PseudoJet temp = trimmer(*it);
    if (fabs(temp.eta()) < 2.0 and temp.pt() > 100.) {
      trimmedJets.push_back(temp);
      MJ += temp.m();
    }
  }
  
  meff = get_meff();
  
  if (bjets.size() >= 3) {
    mT_b = 1000.;
    for (int i = 0; i < 3; i++) {
      double temp = sqrt(2.*(bjets[i]->PT)*pTmiss.Perp()*(1.-cos(fabs(bjets[i]->P4().DeltaPhi(pTmiss)))));
      if (temp < mT_b) mT_b = temp;
    }
  }  
  
  if (signal_jets.size() >= 4) {
    dphimin = 100.;
    for (int i = 0; i < 4; i++) {
      double temp = fabs(signal_jets[i]->P4().DeltaPhi(pTmiss));
      if (temp < dphimin) dphimin = temp;      
    }
  }
  
  countCutflowEvent("00_all");
  
  if (signal_jets.size() < 4 or bjets.size() < 3 or pTmiss.Perp() < 200.) return;
  
  countCutflowEvent("00_preselection");
  
  if (signalLeps.size() >= 1) {
    countCutflowEvent("01_1signalLep");
    mT_lep = sqrt(2.*(signalLeps[0]->PT)*pTmiss.Perp()*(1.-cos(fabs(signalLeps[0]->P4().DeltaPhi(pTmiss)))));
    
    if (PassesCuts_Gtt1L(4, 3, 600., 2300., 150., 120., 200., "SR-Gtt-1L-B")) countSignalEvent("SR-Gtt-1L-B");
    if (PassesCuts_Gtt1L(5, 3, 600., 2000., 200., 120., 200., "SR-Gtt-1L-M1")) countSignalEvent("SR-Gtt-1L-M1");
    if (PassesCuts_Gtt1L(8, 3, 500., 1100., 200., 120., 100., "SR-Gtt-1L-M2")) countSignalEvent("SR-Gtt-1L-M2");
    if (PassesCuts_Gtt1L(9, 3, 300.,  800., 150., 120.,   0., "SR-Gtt-1L-C")) countSignalEvent("SR-Gtt-1L-C");
    
    if (PassesCuts_NN(1, 1900, 1400, 0.9987, 0, "SR-NN-Gtt-1900-1400")) countSignalEvent("SR-Gtt-1900-1400");
    if (PassesCuts_NN(1, 1800, 1, 0.9997, 1, "SR-NN-Gtt-1800-1")) countSignalEvent("SR-Gtt-1800-1");    
    if (PassesCuts_NN(1, 2100, 1, 0.9997, 2, "SR-NN-Gtt-2100-1")) countSignalEvent("SR-Gtt-2100-1");
    if (PassesCuts_NN(1, 2300, 1200, 0.9993, 3, "SR-NN-Gtt-2300-1200")) countSignalEvent("SR-Gtt-2300-1200");   
    
  }
  
  if (electronsLoose.size() + muonsCombined.size() == 0) {
    countCutflowEvent("01_ObaseLep");
    
    if (dphimin < 0.4) return;
    countCutflowEvent("02_phimin");
    
    if (PassesCuts_Gtt0L(5, 3, 600., 2900., 120., 300., "SR-Gtt-0L-B")) countSignalEvent("SR-Gtt-0L-B");
    if (PassesCuts_Gtt0L(9, 3, 600., 1700., 120., 300., "SR-Gtt-0L-M1")) countSignalEvent("SR-Gtt-0L-M1");
    if (PassesCuts_Gtt0L(10, 3, 500., 1100., 120., 200., "SR-Gtt-0L-M2")) countSignalEvent("SR-Gtt-0L-M2");
    if (PassesCuts_Gtt0L(10, 4, 400.,  800., 180., 100., "SR-Gtt-0L-C")) countSignalEvent("SR-Gtt-0L-C");
        
    if (PassesCuts_NN(1, 1900, 1400, 0.9987, 0, "SR-NN-Gtt-1900-1400")) countSignalEvent("SR-Gtt-1900-1400");    
    if (PassesCuts_NN(1, 1800, 1, 0.9997, 1, "SR-NN-Gtt-1800-1")) countSignalEvent("SR-Gtt-1800-1");    
    if (PassesCuts_NN(1, 2100, 1, 0.9997, 2, "SR-NN-Gtt-2100-1")) countSignalEvent("SR-Gtt-2100-1");        
    if (PassesCuts_NN(1, 2300, 1200, 0.9993, 3, "SR-NN-Gtt-2300-1200")) countSignalEvent("SR-Gtt-2300-1200");   
    if (PassesCuts_NN(0, 2100, 1600, 0.9993, 0, "SR-NN-Gbb-2100-1600")) countSignalEvent("SR-Gbb-2100-1600");    
    if (PassesCuts_NN(0, 2000, 1800, 0.997, 0, "SR-NN-Gbb-2000-1800")) countSignalEvent("SR-Gbb-2000-1800");    
    
    if (dphimin > 0.6) {
      if (PassesCuts_NN(0, 2800, 1400, 0.999, 0, "SR-NN-Gbb-2800-1400")) countSignalEvent("SR-Gbb-2800-1400"); 
      if (PassesCuts_NN(0, 2300, 1000, 0.9994, 0, "SR-NN-Gbb-2300-1000")) countSignalEvent("SR-Gbb-2300-1000");    
    }
    
    
    if (mT_b < 130.) return;
    countCutflowEvent("03_mTb>130");
    
    if (PassesCuts_Gtb(4, 3, 2500., 550., 200., "SR-Gtb-B")) countSignalEvent("SR-Gtb-B");
    if (PassesCuts_Gtb(6, 4, 2000., 550., 200., "SR-Gtb-M")) countSignalEvent("SR-Gtb-M");
    if (PassesCuts_Gtb(7, 4, 1300., 500.,  50., "SR-Gtb-C")) countSignalEvent("SR-Gtb-C");
    
    if (PassesCuts_Gbb(4, 3, 550., 65., 2600., "SR-Gbb-B")) countSignalEvent("SR-Gbb-B");
    if (PassesCuts_Gbb(4, 3, 550., 30., 2000., "SR-Gbb-M")) countSignalEvent("SR-Gbb-M");
    if (PassesCuts_Gbb(4, 3, 550., 30., 1600., "SR-Gbb-C")) countSignalEvent("SR-Gbb-C");    
    
  }  
  
}

void Atlas_2211_08028::finalize() {
  // Whatever should be done after the run goes here
  
  TCanvas can1;
  //ggd1->SetMinimum(0.1);
  can1.SetLogy();
  nngtt->Draw("hist");
  can1.Close();
  
  TCanvas can2;
  //ggo1->SetMinimum(0.1);
  can2.SetLogy();
  nngbb->Draw("hist");
  can2.Close();  
  
  hfile->Write();
  hfile->Close();
}       


bool Atlas_2211_08028::sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }

std::vector<Jet*> Atlas_2211_08028::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
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


bool Atlas_2211_08028::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

double Atlas_2211_08028::get_meff() {
  
  double tmp = pTmiss.Pt();
  
  for (int i = 0; i < signalLeps.size(); i++)
    tmp += signalLeps[i]->PT;
  
  for (int i = 0; i < signal_jets.size(); i++)
    tmp += signal_jets[i]->PT;
  
  return tmp;
}

bool Atlas_2211_08028::PassesCuts_Gtt1L(int Njet, int Nbjet, double met, double meff_cut, double mT_cut, double mTb_cut, double MJ_cut, std::string sr) {
  
  if (signal_jets.size() < Njet or bjets.size() < Nbjet) return false;
  countCutflowEvent(sr+"_02_jets");
  
  if( pTmiss.Pt() < met) return false;
  countCutflowEvent(sr+"_03_met");
  
  if( meff < meff_cut) return false;
  countCutflowEvent(sr+"_04_meff");
  
  if( mT_lep < mT_cut) return false;
  countCutflowEvent(sr+"_05_mT");  
  
  if( mT_b < mTb_cut) return false;
  countCutflowEvent(sr+"_06_mTb");    
  
  if( MJ < MJ_cut) return false;
  countCutflowEvent(sr+"_07_MJ");      
  
  return true;
  
}

bool Atlas_2211_08028::PassesCuts_Gtt0L(int Njet, int Nbjet, double met, double meff_cut, double mTb_cut, double MJ_cut, std::string sr) {
  
  if (signal_jets.size() < Njet or bjets.size() < Nbjet) return false;
  countCutflowEvent(sr+"_03_jets");
  
  if( pTmiss.Pt() < met) return false;
  countCutflowEvent(sr+"_04_met");
  
  if( meff < meff_cut) return false;
  countCutflowEvent(sr+"_05_meff");
  
  if( mT_b < mTb_cut) return false;
  countCutflowEvent(sr+"_06_mTb");    
  
  if( MJ < MJ_cut) return false;
  countCutflowEvent(sr+"_07_MJ");      
  
  return true;
  
}


bool Atlas_2211_08028::PassesCuts_Gtb(int Njet, int Nbjet, double meff_cut, double met, double MJ_cut, std::string sr) {
  
  if (signal_jets.size() < Njet ) return false;
  countCutflowEvent(sr+"_04_jets");
  
  if (bjets.size() < Nbjet) return false;
  countCutflowEvent(sr+"_05_bjets");  
  
  if( meff < meff_cut) return false;
  countCutflowEvent(sr+"_06_meff");  
  
  if( pTmiss.Pt() < met) return false;
  countCutflowEvent(sr+"_07_met");
  
    if( MJ < MJ_cut) return false;
  countCutflowEvent(sr+"_08_MJ");    
  
  return true;
  
}


bool Atlas_2211_08028::PassesCuts_Gbb(int Njet, int Nbjet, double met, double ptj1, double meff_cut, std::string sr) {
  
  if (signal_jets.size() < Njet or bjets.size() < Nbjet) return false;
  countCutflowEvent(sr+"_04_jets");
  
  if( pTmiss.Pt() < met) return false;
  countCutflowEvent(sr+"_05_met");
  
  if( signal_jets[0]->PT < ptj1) return false;
  countCutflowEvent(sr+"_06_pTj1");    
  
  if( meff < meff_cut) return false;
  countCutflowEvent(sr+"_07_meff");  
  
  return true;
  
}

bool Atlas_2211_08028::PassesCuts_NN(int Gtt, double mgluino, double mneut, double cutoff, int srnum, std::string sr) {
  
  std::vector<float> input_tensor_values;  
  
  assert(signal_jets.size() >= 4); 
  assert(bjets.size() >= 3);
  std::vector<double> b_cat = jet_btag_category();
  
  for (int i = 0; i < 10; i++) {
    input_tensor_values.push_back(signal_jets.size() > i ? signal_jets[i]->PT : 0.);
    input_tensor_values.push_back(signal_jets.size() > i ? signal_jets[i]->Eta : 0.);
    input_tensor_values.push_back(signal_jets.size() > i ? signal_jets[i]->Phi : 0.);
    input_tensor_values.push_back(signal_jets.size() > i ? signal_jets[i]->P4().M() : 0.);
    input_tensor_values.push_back(signal_jets.size() > i ? b_cat[i] : 0);  
  }
  
  for (int i = 0; i < 4; i++) {
    input_tensor_values.push_back(trimmedJets.size() > i ? trimmedJets[i].pt() : 0.);
    input_tensor_values.push_back(trimmedJets.size() > i ? trimmedJets[i].eta() : 0.);
    input_tensor_values.push_back(trimmedJets.size() > i ? trimmedJets[i].phi() : 0.);
    input_tensor_values.push_back(trimmedJets.size() > i ? trimmedJets[i].m() : 0.);  
  }
  
  for (int i = 0; i < 4; i++) {
    input_tensor_values.push_back(signalLeps.size() > i ? signalLeps[i]->PT : 0.);
    input_tensor_values.push_back(signalLeps.size() > i ? signalLeps[i]->Eta : 0.);
    input_tensor_values.push_back(signalLeps.size() > i ? signalLeps[i]->Phi : 0.);
    input_tensor_values.push_back(signalLeps.size() > i ? signalLeps[i]->P4().M() : 0.);    
  }
  
  input_tensor_values.push_back(pTmiss.Perp() );
  input_tensor_values.push_back(pTmiss.Phi() );  
  
  input_tensor_values.push_back(Gtt);
  input_tensor_values.push_back(mgluino);
  input_tensor_values.push_back(mneut);
  
  assert(input_tensor_values.size() == 87);
  
//  for(int i = 0; i < 87; i++)
//    cout << "Input " << i << ": " << input_tensor_values[i] << std::endl;
  
  for (int i = 0; i < 87; i++)
    input_tensor_values[i] = (input_tensor_values[i] - mean[i])/std[i];
  
//  for(int i = 0; i < 87; i++)
//    cout << "Input " << i << ": " << input_tensor_values[i] << std::endl;  
  
  auto memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
  auto input_tensor = Ort::Value::CreateTensor(memory_info, input_tensor_values.data(), input_tensor_size, input_dims.data(), 2); //rank = 2
  
  auto output_tensors = session->Run(Ort::RunOptions{nullptr}, input_names.data(), &input_tensor, 1, output_names.data(), 1);
    
  float* output = output_tensors.front().GetTensorMutableData<float>();
  //cout << *output << endl;
  std::vector<float> result{output, output + 8};
  cout << sr << ": " << result[0] << "  " << result [1] << endl;
  
  /*cout << "output_tensor_shape: " << print_shape(output_tensors[0].GetTensorTypeAndShapeInfo().GetShape()) << endl;
  
  auto outputInfo = output_tensors[0].GetTensorTypeAndShapeInfo();
  std::cout << "GetElementType: " << outputInfo.GetElementType() << "\n";
  std::cout << "Dimensions of the output: " << outputInfo.GetShape().size() << "\n";
  std::cout << "Shape of the output: ";
  for (unsigned int shapeI = 0; shapeI < outputInfo.GetShape().size(); shapeI++)
          std::cout << outputInfo.GetShape()[shapeI] << ", ";
  cout << endl;*/
  
  countCutflowEvent(sr+"_02_dphi_common");
  
  if (sr == "SR-NN-Gbb-2100-1600") nngbb->Fill(result[1], weight);
  if (sr == "SR-NN-Gtt-2300-1200") nngtt->Fill(result[0], weight);
  
  if( Gtt and result[0] > cutoff) {
    countCutflowEvent(sr+"_03_score");
    return true;
  }
  
  if( !Gtt and result[1] > cutoff) {
    countCutflowEvent(sr+"_03_score");
    return true;
  }  
  
  return false;
}


std::vector<double> Atlas_2211_08028::jet_btag_category() {
  
  std::vector<double> cat;
  int k = 0; //reduce number of loops
  for (int i = 0; i < signal_jets.size(); i++) {
    bool btag = false;
    for (int j = k; j < bjets.size(); j++)
      if (signal_jets[i] == bjets[j]) {
        k++;
        btag = true;
        break;
      }
    if (btag) cat.push_back(1.); else cat.push_back(0.);
  }
  
  return cat;
  
}

std::string Atlas_2211_08028::print_shape(const std::vector<int64_t>& v) {
  std::stringstream ss("");
  for (size_t i = 0; i < v.size() - 1; i++)
    ss << v[i] << "x";
  ss << v[v.size() - 1];
  return ss.str();
}
