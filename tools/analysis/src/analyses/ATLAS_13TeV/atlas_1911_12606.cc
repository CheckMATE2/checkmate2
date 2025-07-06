#include "atlas_1911_12606.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1911_12606::initialize() {
  setAnalysisName("atlas_1911_12606");          
  setInformation(""
    "# search for sleptons and electroweakinos with soft leptons\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("E-inc-01;E-inc-02;E-inc-03;E-inc-05;E-inc-10;E-inc-20;E-inc-30;E-inc-40;E-inc-60;VBF-inc-02;VBF-inc-03;VBF-inc-05;VBF-inc-10;VBF-inc-20;VBF-inc-30;VBF-inc-40;VBF-high-inc-02;VBF-high-inc-03;VBF-high-inc-05;VBF-high-inc-10;VBF-high-inc-20;VBF-high-inc-30;VBF-high-inc-40;S-inc-00;S-inc-01;S-inc-02;S-inc-05;S-inc-10;S-inc-20;S-inc-30;S-inc-40;E-high-ee-05;E-high-ee-10;E-high-ee-20;E-high-ee-30;E-high-ee-40;E-high-ee-60;E-high-mm-02;E-high-mm-03;E-high-mm-05;E-high-mm-10;E-high-mm-20;E-high-mm-30;E-high-mm-40;E-high-mm-60;E-med-ee-05;E-med-ee-10;E-med-ee-20;E-med-ee-30;E-med-mm-02;E-med-mm-03;E-med-mm-05;E-med-mm-10;E-med-mm-20;E-med-mm-30;E-low-ee-05;E-low-ee-10;E-low-ee-20;E-low-ee-30;E-low-ee-40;E-low-ee-60;E-low-mm-02;E-low-mm-03;E-low-mm-05;E-low-mm-10;E-low-mm-20;E-low-mm-30;E-low-mm-40;E-low-mm-60;SR-1L1T-1.0;SR-1L1T-1.5;SR-1L1T-2.0;SR-1L1T-3.0;SR-1L1T-4.0;SR-1L1T-5.0;VBF-low-02;VBF-low-03;VBF-low-05;VBF-low-10;VBF-low-20;VBF-low-30;VBF-low-40;VBF-high-02;VBF-high-03;VBF-high-05;VBF-high-10;VBF-high-20;VBF-high-30;VBF-high-40;S-high-ee-00;S-high-ee-01;S-high-ee-02;S-high-ee-05;S-high-ee-10;S-high-ee-20;S-high-ee-30;S-high-ee-40;S-high-mm-00;S-high-mm-01;S-high-mm-02;S-high-mm-05;S-high-mm-10;S-high-mm-20;S-high-mm-30;S-high-mm-40;S-low-ee-00;S-low-ee-01;S-low-ee-02;S-low-ee-05;S-low-ee-10;S-low-ee-20;S-low-ee-30;S-low-ee-40;S-low-mm-00;S-low-mm-01;S-low-mm-02;S-low-mm-05;S-low-mm-10;S-low-mm-20;S-low-mm-30;S-low-mm-40");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  n = 0;
  int ifile = bookFile("atlas_1911_12606.root", true);
  const char *rootFileName = fNames[ifile].c_str() ;
  hfile = new TFile(rootFileName, "RECREATE", "Saving Histograms");
  llinv = new TH1F("", "mll", 20, 0., 30.);
}

void Atlas_1911_12606::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsLoose = filterPhaseSpace(electronsLoose, 4.5, -2.47, 2.47);
//  electronsTight = filterPhaseSpace(electronsTight, 4.5, -2.47, 2.47); //Tight fits efficiency better
//  muonsCombined = filterPhaseSpace(muonsCombined, 3., -2.5, 2.5);  
  electronsMedium = filterPhaseSpace(electronsMedium, 8., -2.47, 2.47); 
  muonsCombined = filterPhaseSpace(muonsCombined, 8., -2.5, 2.5);    
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5); 
  signal_tracks = filterPhaseSpace(tracks, 2., -2.5, 2.5); 
  signal_tracks = filterLeptons(signal_tracks);
  
  int nel = 0; int nmu = 0;
  for (int t = 0; t < tracks.size(); t++ ) {
    if ( abs(tracks[t]->PID) == 11) nel++;
    if ( abs(tracks[t]->PID) == 13) nmu++;
  }
  
  TLorentzVector missing_true = TLorentzVector(0.,0.,0.,0.);
  std::vector<Electron*> electrons_soft;
  electrons_soft_true.clear();
  std::vector<Electron*> electrons_tracks;
  electrons_tracks_true.clear();  
  std::vector<Muon*> muons_soft;
  muons_soft_true.clear();
  std::vector<Muon*> muons_tracks;
  muons_tracks_true.clear();  
  int nel_true = 0; int nmu_true = 0;
  for (int t = 0; t < true_particles.size(); t++ ) {
    if ( abs(true_particles[t]->PID) == 11 and true_particles[t]->Status == 1 ) {
        double eff1 = rand()/(RAND_MAX + 1.);
        double eff2 = rand()/(RAND_MAX + 1.);
        double pt = true_particles[t]->PT;
        if ( pt >  2.) nel_true++;
        for (int i = 0; i < electronsMedium.size(); i++)
            if ( electronsMedium[i]->Particle == true_particles[t] ) break;
        if (eff1 < ( (pt > 4.5)*(pt < 5.0)*0.20 + (pt > 5)*(pt < 6.0)*0.37 + (pt > 6)*(pt < 8.0)*0.48 ) and fabs(true_particles[t]->Eta) < 2.47) {
              Electron ele = Electron();
              ele.PT = pt;
              ele.Phi = true_particles[t]->Phi;
              ele.Eta = true_particles[t]->Eta;
              ele.P4() = true_particles[t]->P4();
              //cout << "Mom: " << ele.P4().X() << "\n";
              ele.Particle = true_particles[t];
              ele.Charge = true_particles[t]->Charge;
              electrons_soft_true.push_back(ele);
              electrons_soft.push_back(&(*(electrons_soft_true.end()-1)));
              electronsMedium.push_back(&(*(electrons_soft_true.end()-1)));
              //cout << "Mom: " << electrons_soft[electrons_soft.size()-1]->P4().X() << "\n";
        }
        else if ( eff2 < ( (pt < 2.5) *(pt > 2.) * 0.1 + (pt > 2.5) * (pt < 8.) * 0.3 ) and fabs(true_particles[t]->Eta) < 2.47 ) {
              Electron ele = Electron();
              ele.PT = pt;
              ele.Phi = true_particles[t]->Phi;
              ele.Eta = true_particles[t]->Eta;
              ele.P4() = true_particles[t]->P4();
              //cout << "Mom: " << ele.P4().X() << "\n";
              ele.Particle = true_particles[t];
              ele.Charge = true_particles[t]->Charge;
              electrons_tracks_true.push_back(ele);
              electrons_tracks.push_back(&(*(electrons_tracks_true.end()-1)));
        }
        else continue;
    }
    if ( abs(true_particles[t]->PID) == 13 and true_particles[t]->Status == 1 ) {
        double eff1 = rand()/(RAND_MAX + 1.);
        double eff2 = rand()/(RAND_MAX + 1.);
        double pt = true_particles[t]->PT;
        if ( pt >  2.) nmu_true++;
        for (int i = 0; i < muonsCombined.size(); i++)
            if ( muonsCombined[i]->Particle == true_particles[t] ) break;
        if (eff1 < ((pt > 3.0)*(pt < 3.5)*0.48 + (pt > 3.5)*(pt < 4.5)*0.62 + (pt > 4.5)*(pt < 8.)*0.70 ) and fabs(true_particles[t]->Eta) < 2.5 ) {
              Muon muo = Muon();
              muo.PT = pt;
              muo.Phi = true_particles[t]->Phi;
              muo.Eta = true_particles[t]->Eta;
              muo.P4() = true_particles[t]->P4();
              muo.Particle = true_particles[t];
              muo.Charge = true_particles[t]->Charge;
              muons_soft_true.push_back(muo);
              muons_soft.push_back(&(*(muons_soft_true.end()-1)));
              muonsCombined.push_back(&(*(muons_soft_true.end()-1)));
        }
        else if ( eff2 < ( (pt < 2.5) *(pt > 2.) * 0.3 + (pt > 2.5) * (pt < 8.) * 0.4 ) and fabs(true_particles[t]->Eta) < 2.5) {
              Muon muo = Muon();
              muo.PT = pt;
              muo.Phi = true_particles[t]->Phi;
              muo.Eta = true_particles[t]->Eta;
              muo.P4() = true_particles[t]->P4();
              //cout << "Mom: " << ele.P4().X() << "\n";
              muo.Particle = true_particles[t];
              muo.Charge = true_particles[t]->Charge;
              muons_tracks_true.push_back(muo);
              muons_tracks.push_back(&(*(muons_tracks_true.end()-1)));
        }
        else continue;        
    }
    if ( (abs(true_particles[t]->PID) == 1000022 or abs(true_particles[t]->PID) == 5100022)and true_particles[t]->Status == 1 ) 
        missing_true += true_particles[t]->P4();
  }  
  
  leptons_tracks.clear();
  for(int e = 0; e < electrons_tracks.size(); e++) {
    FinalStateObject* lep = newFinalStateObject(electrons_tracks[e]);
    leptons_tracks.push_back(lep);
  }  
  for(int m = 0; m < muons_tracks.size(); m++) {
    FinalStateObject* lep = newFinalStateObject(muons_tracks[m]);
    leptons_tracks.push_back(lep);
  }
  std::sort(leptons_tracks.begin(), leptons_tracks.end(), FinalStateObject::sortByPT);  
  
  n++;
  //cout << "Event: " << n << "\n";
  //cout << "True_part: " << true_particles.size() << "\n";
  //cout << "electronsTight: " << electronsTight.size() << "\n";
  //cout << "electronTracks: " << nel << "\n";
  //cout << "electronTrue: " << nel_true << "\n";
  //cout << "muonsCombined: " << muonsCombined.size() << "\n";
  //cout << "muonTracks: " << nmu << "\n";
  //cout << "muonTrue: " << nmu_true << "\n";
//  cout << "tracks: " << tracks.size() << "\n"; 
  //cout << "leptons_tracks: " << leptons_tracks.size() << "\n";    
  
  std::vector<Jet*> bjets;
  std::vector<Jet*> lightjets;
  
  nb = 0;
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta) < 2.5 && checkBTag(jets[i]) ) {bjets.push_back(jets[i]); nb++; }
    else lightjets.push_back(jets[i]);  
    
  lightjets = overlapRemoval(lightjets, electronsLoose, 0.2, "y");
  bjets = overlapRemoval_muon_jet_tracks(bjets, muonsCombined, 0.4, 2);    
  lightjets = overlapRemoval_muon_jet_tracks(lightjets, muonsCombined, 0.4, 2); 
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.4, "y");  
  electronsLoose = overlapRemoval(electronsLoose, lightjets, 0.4, "y"); 
  electronsMedium = overlapRemoval(electronsMedium, bjets, 0.4, "y");  
  electronsMedium = overlapRemoval(electronsMedium, lightjets, 0.4, "y");    
  muonsCombined = overlapRemoval(muonsCombined, bjets, 0.4, "y");  
  muonsCombined = overlapRemoval(muonsCombined, lightjets, 0.4, "y");     
//  signal_tracks = overlapRemoval(signal_tracks, bjets, 0.5, "y");
//  signal_tracks = overlapRemoval(signal_tracks, lightjets, 0.5, "y");
  leptons_tracks = overlapRemoval(leptons_tracks, bjets, 0.5, "y");
  leptons_tracks = overlapRemoval(leptons_tracks, lightjets, 0.5, "y");  
  //cout << "leptons_tracks: " << leptons_tracks.size() << "\n";    
  
  pTmiss = TLorentzVector(0., 0., 0., 0.);
  for (int i = 0; i < lightjets.size(); i++) 
    pTmiss -= lightjets[i]->P4();      
  
  for (int i = 0; i < bjets.size(); i++) 
    pTmiss -= bjets[i]->P4();        
  
//  for (int i = 0; i < muonsCombined.size(); i++) //later
//    pTmiss -= muonsCombined[i]->P4();      
  
//  for (int i = 0; i < electronsLoose.size(); i++) 
//    pTmiss -= electronsLoose[i]->P4();      
  
  TLorentzVector pTmiss_soft = TLorentzVector(0., 0., 0., 0.);
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
    bool used = false;
    for (int i = 0; i < lightjets.size() && !used; i++)
      for (int part = 0; part < lightjets[i]->Particles.GetEntries(); part++)
        if ((*it)->PT > 0.4 && lightjets[i]->Particles.At(part) == (*it)->Particle) {
	  used = true;
	  break;
	}
    if (!used)  pTmiss_soft -= (*it)->P4();
    //softtracks.push_back(*it);
  }
  
  pTmiss += pTmiss_soft;  
  
  std::vector<Electron*> electrons_base = electronsLoose;
  std::vector<Muon*> muons_base = muonsCombined;
    
  std::vector<Electron*> electrons_signal = Isolate_leptons_with_inverse_track_isolation_cone(electronsMedium, tracks, towers, 0.3, 10., 0.2, 0.15, 0.2, true);
  std::vector<Muon*> muons_signal = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined,  tracks, towers, 0.3, 10., 0.2, 0.06, 0.06, false);
//  signal_tracks = Isolate_tracks(signal_tracks, 0.3, 0.5);
  leptons_tracks = Isolate_tracks(leptons_tracks, 0.3, 0.5);
  //cout << "leptons_tracks: " << leptons_tracks.size() << "\n";    
  
  //cout << "muonsSignal: " << muons_signal.size() << "\n";
  //cout << "electronsSignal: " << electrons_signal.size() << "\n";
  
  lightjets = filterPhaseSpace(lightjets, 30., -2.8, 2.8);
  
  signal_jets = lightjets;
  signal_jets.insert(signal_jets.end(), bjets.begin(), bjets.end());
  std::sort(signal_jets.begin(), signal_jets.end(), sortByPT );     
  
  leptons.clear();
  for(int e = 0; e < electrons_signal.size(); e++) {
    FinalStateObject* lep = newFinalStateObject(electrons_signal[e]);
    leptons.push_back(lep);
  }  
  for(int m = 0; m < muons_signal.size(); m++) {
    FinalStateObject* lep = newFinalStateObject(muons_signal[m]);
    leptons.push_back(lep);
  }
  std::sort(leptons.begin(), leptons.end(), FinalStateObject::sortByPT);

//  cout << "leptons: " << leptons.size() << "\n";

//  for (int i = 0; i < signal_jets.size(); i++) 
//    pTmiss -= signal_jets[i]->P4(); 
  for (int i = 0; i < leptons.size(); i++) 
    pTmiss -= leptons[i]->P4(); 

  countCutflowEvent("00_0all");
  
  if ( !signal_jets.size() or signal_jets[0]->PT < 50.) return;
  countCutflowEvent("00_1jet50");
  
  //if (nel_true + nmu_true < 2 or missing_true.Perp() < 75. ) return; // generator filter 2l
  //if (nel_true + nmu_true < 1 or missing_true.Perp() < 50. ) return; // generator filter 1l1t
  //countCutflowEvent("00_filter");
  
  met = missingET->P4().Perp();
  met = pTmiss.Perp();
  if (met < 110. ) return;
  countCutflowEvent("01_trigger");  
  
  if ( leptons.size() == 1 and leptons_tracks.size() > 0) {
      //if (rand()/(RAND_MAX + 1.) < 0.3) return; //correction for efficiency
      countCutflowEvent("1L1T_02_1L1T");
      mlt = (leptons[0]->P4() + leptons_tracks[0]->P4()).M();
      if (Pass_Cuts_1L1T()) { //doesn't match cutflows
          if (mlt < 5.) {countCutflowEvent("1L1T_17_mlt<5"); if (mlt > 4.) countSignalEvent("SR-1L1T-5.0") ; }
          if (mlt < 4.) {countCutflowEvent("1L1T_18_mlt<4"); if (mlt > 3.2) countSignalEvent("SR-1L1T-4.0") ; }
          if (mlt < 3.) {countCutflowEvent("1L1T_19_mlt<3"); if (mlt > 2.) countSignalEvent("SR-1L1T-3.0") ; }
          if (mlt < 2.) {countCutflowEvent("1L1T_20_mlt<2"); if (mlt > 1.5) countSignalEvent("SR-1L1T-2.0") ; }
          if (mlt < 1.5) {countCutflowEvent("1L1T_21_mlt<1.5"); if (mlt > 1.) countSignalEvent("SR-1L1T-1.5") ; }
          if (mlt < 1.) {countCutflowEvent("1L1T_22_mlt<1"); if (mlt > 0.5) countSignalEvent("SR-1L1T-1.0") ; }
          return;          
    }
  }
      
  if ( leptons.size() == 2 ) countCutflowEvent("2L_02_2L"); else return; 
  
  mll = (leptons[0]->P4() +  leptons[1]->P4()).M();
  mt2 = 0.;
  flavour = -1;
  if (electrons_signal.size() == 2) flavour = 1;
  else if (muons_signal.size() == 2) flavour = 2;
  else flavour = 0;
  
  if (Pass_Preselection()) {
    
    bool high = Pass_EW_high();
    bool med = Pass_EW_med();
    bool low = Pass_EW_low();
  
    if (high or med or low ) {//note that inclusive SRs include 1L1T events in ATLAS analysis
      if ( mll < 1.) countSignalEvent("E-inc-01");   
      if ( mll < 2.) countSignalEvent("E-inc-02"); 
      if ( mll < 3.) countSignalEvent("E-inc-03"); 
      if ( mll < 5.) countSignalEvent("E-inc-05"); 
      if ( mll < 10.) countSignalEvent("E-inc-10");
      if ( mll < 20.) countSignalEvent("E-inc-20"); 
      if ( mll < 30.) countSignalEvent("E-inc-30"); 
      if ( mll < 40.) countSignalEvent("E-inc-40"); 
      if ( mll < 60.) countSignalEvent("E-inc-60"); 
      llinv->Fill(mll, weight);
    }
  
    if (Pass_S_high() or   Pass_S_low()) {
      if ( mt2 < 100.5) countSignalEvent("S-inc-00");   
      if ( mt2 < 101.) countSignalEvent("S-inc-01"); 
      if ( mt2 < 102.) countSignalEvent("S-inc-02"); 
      if ( mt2 < 105.) countSignalEvent("S-inc-05"); 
      if ( mt2 < 110.) countSignalEvent("S-inc-10");
      if ( mt2 < 120.) countSignalEvent("S-inc-20"); 
      if ( mt2 < 130.) countSignalEvent("S-inc-30");         
      if ( mt2 < 140.) countSignalEvent("S-inc-40");         
    }
  
  
  }
  
  return;
  
}

void Atlas_1911_12606::finalize() {
  // Whatever should be done after the run goes here
    
  TCanvas can1;
  llinv->Draw();
  llinv->SetTitle("lepton-lepton invariant");
  can1.Close();

  hfile->Write();

  hfile->Close();
    
}       

bool Atlas_1911_12606::Pass_Cuts_1L1T() {
    
    if (mlt > 3.0 and mlt < 3.2)  return false;
    countCutflowEvent("1L1T_03_jpsi_veto");
    
    if (met < 200.) return false;
    countCutflowEvent("1L1T_04_met>200");
    
    for (int i = 0; i < signal_jets.size(); i++)
       //if ( fabs(signal_jets[i]->P4().DeltaPhi(missingET->P4())) < 0.4 ) return false;
       if ( fabs(signal_jets[i]->P4().DeltaPhi(pTmiss)) < 0.4 ) return false;
    countCutflowEvent("1L1T_05_dphi_min>0.4");
    
    if ( signal_jets.size() > 0  and fabs(signal_jets[0]->P4().DeltaPhi(missingET->P4())) < 2. ) return false;
    countCutflowEvent("1L1T_06_dphi_jet1>2");   
    
    if (mlt < 0.5 or mlt > 5.) return false;
    countCutflowEvent("1L1T_07_mlt<5");
    
    double deltar = leptons[0]->P4().DeltaR(leptons_tracks[0]->P4());
    if ( deltar < 0.05) return false;
    countCutflowEvent("1L1T_08_deltaR>0.05");
    
    if ( signal_jets.size() == 0 ) return false;
    countCutflowEvent("1L1T_09_jet");
    
    if ( signal_jets[0]->PT < 100. ) return false;
    countCutflowEvent("1L1T_10_jet>100");    
    
    double htlt = leptons[0]->PT + leptons_tracks[0]->PT;
    if (met/htlt < 30.) return false;
    countCutflowEvent("1L1T_11_MET/HTl");    
    
    if ( deltar > 1.5) return false;
    countCutflowEvent("1L1T_12_deltaR<1.5");    
    
    if ( leptons[0]->PT > 10.) return false;
    countCutflowEvent("1L1T_13_lepton<10");    
    
    if ( leptons_tracks[0]->PT > 5.) return false;
    countCutflowEvent("1L1T_14_track<5");   
    
    if ( fabs(leptons[0]->P4().DeltaPhi(pTmiss)) > 1.0 ) return false;
    countCutflowEvent("1L1T_15_dphilep");
    
//    if ( (leptons[0]->Type == "electron" and abs(leptons_tracks[0]->PID) != 11) or (leptons[0]->Type == "muon" and abs(leptons_tracks[0]->PID) != 13) ) return false;
    if ( leptons[0]->Type != leptons_tracks[0]->Type ) return false;
    countCutflowEvent("1L1T_16_SF");
    
    if ( leptons[0]->Charge * leptons_tracks[0]->Charge > 0 ) return false;
    countCutflowEvent("1L1T_17_OS");
    
    return true;
}


bool Atlas_1911_12606::Pass_Preselection() {
    
    if (mll > 3.0 and mll < 3.2)  return false;
    countCutflowEvent("2L_03_jpsi_veto");
    
    for (int i = 0; i < signal_jets.size(); i++)
       //if ( fabs(signal_jets[i]->P4().DeltaPhi(missingET->P4())) < 0.4 ) return false;
        if ( fabs(signal_jets[i]->P4().DeltaPhi(pTmiss)) < 0.4 ) return false;
    countCutflowEvent("2L_04_dphi_min>0.4");
    
    //if ( signal_jets.size() > 0  and fabs(signal_jets[0]->P4().DeltaPhi(missingET->P4())) < 2. ) return false;
    if ( signal_jets.size() > 0  and fabs(signal_jets[0]->P4().DeltaPhi(pTmiss)) < 2. ) return false;
    countCutflowEvent("2L_05_dphi_jet1>2");
    
    if (mll < 1 or (flavour == 1 and mll < 3) or mll > 60.) return false;
    countCutflowEvent("2L_06_mll");
    
    double deltar = leptons[0]->P4().DeltaR(leptons[1]->P4());
    if ( (flavour == 1 and deltar < 0.3) or (flavour == 2 and deltar < 0.05) or (flavour == 0 and deltar < 0.2) ) return false; 
    countCutflowEvent("2L_07_deltaR");
    
    if ( leptons[0]->PT < 5.) return false;
    countCutflowEvent("2L_08_lepton>5");
    
    if ( signal_jets.size() == 0 ) return false;
    countCutflowEvent("2L_09_jet");
    
    if ( signal_jets[0]->PT < 100. ) return false;
    countCutflowEvent("2L_10_jet>100");
    
    if (nb) return false;
    countCutflowEvent("2L_11_bveto");
    
    double mtt = mtautau(leptons);
    //cout << "mtautau: " << mtt << "\n"; 
    if (mtt > 0. and mtt < 160.) return false;  
    countCutflowEvent("2L_12_mtautau");
    
    if (!flavour) return false;
    countCutflowEvent("2L_13_eemumu");
    
    return true;
    
}

bool Atlas_1911_12606::Pass_EW_high() {

    if (met < 200.) return false;
    countCutflowEvent("2L_E-high_14_met>200");
    
    double mT = AnalysisBase::mT(leptons[0]->P4(), missingET->P4());
    if (mT > 60.) return false;
    countCutflowEvent("2L_E-high_15_mT<60");
    
    TLorentzVector ISR(0.,0.,0.,0.);
    for (int i = 0; i < signal_jets.size(); i++)
      ISR += signal_jets[i]->P4();     
    TVector3 pTjet(ISR.X(), ISR.Y(), 0.);
    double RISR2 = fabs(pTmiss.Vect().Dot(pTjet.Unit()))/ISR.Perp();
    
    if (RISR2 < std::max(0.85, 0.98 - 0.02*mll) or RISR2 > 1.1) return false;//approx
    countCutflowEvent("2L_E-high_16_RISR");    
    
    if (leptons[1]->PT < std::min(10.,2.+mll/3.) ) return false;
    countCutflowEvent("2L_E-high_17_lepton2PT"); 

    if (flavour==1) {
        if (mll < 5.) countSignalEvent("E-high-ee-05");
        else if (mll < 10.) countSignalEvent("E-high-ee-10");
        else if (mll < 20.) countSignalEvent("E-high-ee-20");
        else if (mll < 30.) countSignalEvent("E-high-ee-30");
        else if (mll < 40.) countSignalEvent("E-high-ee-40");
        else if (mll < 60.) countSignalEvent("E-high-ee-60");
        else return false;
    }
    
    if (flavour==2) {
        if (mll < 2.) countSignalEvent("E-high-mm-02");
        else if (mll < 3.) countSignalEvent("E-high-mm-03");
        else if (mll < 5.) countSignalEvent("E-high-mm-05");
        else if (mll < 10.) countSignalEvent("E-high-mm-10");
        else if (mll < 20.) countSignalEvent("E-high-mm-20");
        else if (mll < 30.) countSignalEvent("E-high-mm-30");
        else if (mll < 40.) countSignalEvent("E-high-mm-40");
        else if (mll < 60.) countSignalEvent("E-high-mm-60");
        else return false;
    }    
    
    return true;

}


bool Atlas_1911_12606::Pass_EW_med() {
    
    if (met < 120. or met > 200.) return false;
    countCutflowEvent("2L_E-med_14_met<200");
    
    double htlep = leptons[0]->PT + leptons[1]->PT;  
    if (met/htlep < 10.) return false;
    countCutflowEvent("2L_E-med_15_HTlep");
    
    double mT = AnalysisBase::mT(leptons[0]->P4()+leptons[1]->P4(), missingET->P4());
    if (mT > 50.) return false;
    countCutflowEvent("2L_E-med_16_mT<50");
    
    if (flavour==1) {
        if (mll < 5.) countSignalEvent("E-med-ee-05");
        else if (mll < 10.) countSignalEvent("E-med-ee-10");
        else if (mll < 20.) countSignalEvent("E-med-ee-20");
        else if (mll < 30.) countSignalEvent("E-med-ee-30");
        else return false;
    }
    
    if (flavour==2) {
        if (mll < 2.) countSignalEvent("E-med-mm-02");
        else if (mll < 3.) countSignalEvent("E-med-mm-03");
        else if (mll < 5.) countSignalEvent("E-med-mm-05");
        else if (mll < 10.) countSignalEvent("E-med-mm-10");
        else if (mll < 20.) countSignalEvent("E-med-mm-20");
        else if (mll < 30.) countSignalEvent("E-med-mm-30");
        else return false;
    }    
    
    return true;

}


bool Atlas_1911_12606::Pass_EW_low() {
    
    if (met < 120. or met > 200.) return false;
    countCutflowEvent("2L_E-low_14_met<200");
    
    double htlep = leptons[0]->PT + leptons[1]->PT;  
    if (met/htlep > 10.) return false;
    countCutflowEvent("2L_E-low_15_HTlep");
    
    TLorentzVector ISR(0.,0.,0.,0.);
    for (int i = 0; i < signal_jets.size(); i++)
      ISR += signal_jets[i]->P4();     
    TVector3 pTjet(ISR.X(), ISR.Y(), 0.);
    double RISR2 = fabs(pTmiss.Vect().Dot(pTjet.Unit()))/ISR.Perp();
    
    if (RISR2 < 0.8 or RISR2 > 1.1) return false;//cheating
    countCutflowEvent("2L_E-low_16_RISR");     
    
    if (leptons[1]->PT < 5. + mll/4.)  return false;
    countCutflowEvent("2L_E-low_17_lepton2PT");     
    
    double mT = AnalysisBase::mT(leptons[0]->P4(), missingET->P4());
    if (mT > 60. or mT < 10.) return false;
    countCutflowEvent("2L_E-low_18_mT<60");    
    
    if (flavour==1) {
        if (mll < 5.) countSignalEvent("E-low-ee-05");
        else if (mll < 10.) countSignalEvent("E-low-ee-10");
        else if (mll < 20.) countSignalEvent("E-low-ee-20");
        else if (mll < 30.) countSignalEvent("E-low-ee-30");
        else if (mll < 40.) countSignalEvent("E-low-ee-40");
        else if (mll < 60.) countSignalEvent("E-low-ee-60");
        else return false;
    }
    
    if (flavour==2) {
        if (mll < 2.) countSignalEvent("E-low-mm-02");
        else if (mll < 3.) countSignalEvent("E-low-mm-03");
        else if (mll < 5.) countSignalEvent("E-low-mm-05");
        else if (mll < 10.) countSignalEvent("E-low-mm-10");
        else if (mll < 20.) countSignalEvent("E-low-mm-20");
        else if (mll < 30.) countSignalEvent("E-low-mm-30");
        else if (mll < 40.) countSignalEvent("E-low-mm-40");
        else if (mll < 60.) countSignalEvent("E-low-mm-60");
        else return false;
    }        
    
    
}


bool Atlas_1911_12606::Pass_S_high() {
       
    if (met < 200.) return false;
    countCutflowEvent("2L_S-high_14_met>200");
    
    //double mt2 = mT2chi2( leptons[0]->P4(), leptons[1]->P4(), 100., missingET->P4() );
    mt2 = mT2chi2( leptons[0]->P4(), leptons[1]->P4(), 100., pTmiss );
    
    TLorentzVector ISR(0.,0.,0.,0.);
    for (int i = 0; i < signal_jets.size(); i++)
      ISR += signal_jets[i]->P4(); 
    
    
    TVector3 pTjet(ISR.X(), ISR.Y(), 0.);
    double RISR2 = fabs(pTmiss.Vect().Dot(pTjet.Unit()))/ISR.Perp();
    if (RISR2  < std::max(0.85, 0.98 - 0.02*mt2) or RISR2 > 1.) return false;//approx   
    countCutflowEvent("2L_S-high_15_RISR");    
    
    if (leptons[1]->PT < std::min(20.,2.5 + (mt2 - 100.)*2.5) ) return false;
    countCutflowEvent("2L_S-high_16_lepton2PT"); 
    
    if (mt2 > 140.) return false;
    countCutflowEvent("2L_S-high_17_mt2<140"); 
    
    if (flavour==1) {
        if (mt2 < 100.5) countSignalEvent("S-high-ee-00");
        else if (mt2 < 101.) countSignalEvent("S-high-ee-01");
        else if (mt2 < 102.) countSignalEvent("S-high-ee-02");
        else if (mt2 < 105.) countSignalEvent("S-high-ee-05");
        else if (mt2 < 110.) countSignalEvent("S-high-ee-10");
        else if (mt2 < 120.) countSignalEvent("S-high-ee-20");
        else if (mt2 < 130.) countSignalEvent("S-high-ee-30");
        else if (mt2 < 140.) countSignalEvent("S-high-ee-40");
        else return false;
    }
    
    if (flavour==2) {
        if (mt2 < 100.5) countSignalEvent("S-high-mm-00");
        else if (mt2 < 101.) countSignalEvent("S-high-mm-01");
        else if (mt2 < 102.) countSignalEvent("S-high-mm-02");
        else if (mt2 < 105.) countSignalEvent("S-high-mm-05");
        else if (mt2 < 110.) countSignalEvent("S-high-mm-10");
        else if (mt2 < 120.) countSignalEvent("S-high-mm-20");
        else if (mt2 < 130.) countSignalEvent("S-high-mm-30");
        else if (mt2 < 140.) countSignalEvent("S-high-mm-40");
        else return false;
    }   
    
    
    return true;
}

bool Atlas_1911_12606::Pass_S_low() {
    
    if (met > 200. or met < 150.) return false;
    countCutflowEvent("2L_S-low_14_met<200");
    
    mt2 = mT2chi2( leptons[0]->P4(), leptons[1]->P4(), 100., missingET->P4() );
    
    TLorentzVector ISR(0.,0.,0.,0.);
    for (int i = 0; i < signal_jets.size(); i++)
      ISR += signal_jets[i]->P4(); 
    
    TVector3 pTjet(ISR.X(), ISR.Y(), 0.);
    double RISR2 = fabs(pTmiss.Vect().Dot(pTjet.Unit()))/ISR.Perp();

    if (RISR2 < 0.8 or RISR2 > 1.1  ) return false;//approx
    countCutflowEvent("2L_S-low_15_RISR");    
    
    if (leptons[1]->PT < std::min(15.,7.5 + (mt2 - 100.)*0.75) ) return false;
    countCutflowEvent("2L_S-low_16_lepton2PT"); 
    
    if (mt2 > 140.) return false;
    countCutflowEvent("2L_S-low_17_mt2<140"); 
    
    if (flavour==1) {
        if (mt2 < 100.5) countSignalEvent("S-low-ee-00");
        else if (mt2 < 101.) countSignalEvent("S-low-ee-01");
        else if (mt2 < 102.) countSignalEvent("S-low-ee-02");
        else if (mt2 < 105.) countSignalEvent("S-low-ee-05");
        else if (mt2 < 110.) countSignalEvent("S-low-ee-10");
        else if (mt2 < 120.) countSignalEvent("S-low-ee-20");
        else if (mt2 < 130.) countSignalEvent("S-low-ee-30");
        else if (mt2 < 140.) countSignalEvent("S-low-ee-40");
        else return false;
    }
    
    if (flavour==2) {
        if (mt2 < 100.5) countSignalEvent("S-low-mm-00");
        else if (mt2 < 101.) countSignalEvent("S-low-mm-01");
        else if (mt2 < 102.) countSignalEvent("S-low-mm-02");
        else if (mt2 < 105.) countSignalEvent("S-low-mm-05");
        else if (mt2 < 110.) countSignalEvent("S-low-mm-10");
        else if (mt2 < 120.) countSignalEvent("S-low-mm-20");
        else if (mt2 < 130.) countSignalEvent("S-low-mm-30");
        else if (mt2 < 140.) countSignalEvent("S-low-mm-40");
        else return false;
    }        
    
    return true;
}


double Atlas_1911_12606::mtautau(std::vector<FinalStateObject*> lep) {
   
   if (lep.size() < 2) return 0.;
   
   double det0 = lep[0]->P4().X() * lep[1]->P4().Y() - lep[1]->P4().X() * lep[0]->P4().Y() ;
   if (det0 == 0.) return 0.;
   
   double det1 = missingET->P4().X() * lep[1]->P4().Y() - lep[1]->P4().X() * missingET->P4().Y() ;
   double det2 = lep[0]->P4().X() * missingET->P4().Y() - missingET->P4().X() * lep[0]->P4().Y() ;
   
   double alpha = det1/det0;
   double beta = det2/det0;
   
   double mtt = 2. * lep[0]->P4().Dot(lep[1]->P4()) * (1. + alpha) * (1. + beta);
   double sign = (mtt > 0.) ? 1. : -1.;
   
   return sign*sqrt(fabs(mtt));
    
}

bool Atlas_1911_12606::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

std::vector<Jet*> Atlas_1911_12606::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
  std::vector<Jet*> passed;
  for (std::vector<Jet*>::iterator jet = cand_jets.begin(); jet != cand_jets.end(); jet++) {
  
    if (check_nTrack_jet(*jet, tracks, nTracks)) {
      passed.push_back(*jet);
      continue;
    }
    
    bool iso = true;
        
    for (std::vector<Muon*>::iterator mu = cand_muons.begin(); mu != cand_muons.end(); mu++) 
      if ((*jet)->P4().DeltaR((*mu)->P4()) < deltaR ) {
	iso = false;
	break;
      }
        		    
    if (iso) passed.push_back(*jet);
  }
  
  return passed;
}

std::vector<Track*> Atlas_1911_12606::filterLeptons(std::vector<Track*> cand_tracks) {

  std::vector<Track*> accepted;  
  for (int t = 0; t < cand_tracks.size(); t++ ) {
    if ( cand_tracks[t]->PT < 8. and abs(cand_tracks[t]->PID) == 11)
        for (int i = 0; i < electronsMedium.size(); i++ ) {
            if (cand_tracks[t]->P4().DeltaR(electronsMedium[i]->P4()) < 0.01) break;
            double eff = rand()/(RAND_MAX + 1.);
            if (cand_tracks[t]->PT < 2.5  and eff < 0.4) accepted.push_back( cand_tracks[t]);
            if (cand_tracks[t]->PT > 2.5  and eff < 0.8) accepted.push_back( cand_tracks[t]);
        }
    if ( cand_tracks[t]->PT < 8. and abs(cand_tracks[t]->PID) == 13 )
        for (int i = 0; i < muonsCombined.size(); i++ ) {
            if (cand_tracks[t]->P4().DeltaR(muonsCombined[i]->P4()) < 0.01) break;
            double eff = rand()/(RAND_MAX + 1.);
            if (cand_tracks[t]->PT < 2.5  and eff < 0.4) accepted.push_back( cand_tracks[t]);
            if (cand_tracks[t]->PT > 2.5  and eff < 0.8) accepted.push_back( cand_tracks[t]);
        }
  }
    
  return accepted;  
}

std::vector<FinalStateObject*> Atlas_1911_12606::Isolate_tracks(std::vector<FinalStateObject*> cand_tracks, double dR, double mom) {
    
  std::vector<FinalStateObject*> accepted;    
  
  for (int i = 0; i < cand_tracks.size(); i++ ) {
    bool accept = true;  
    for (int t = 0; t < cand_tracks.size(); t++ ) 
      if ( t != i and cand_tracks[t]->PT > mom and cand_tracks[i]->P4().DeltaR(cand_tracks[t]->P4()) < dR) {
         accept = false;
         break;
      }
    if (accept) accepted.push_back(cand_tracks[i]); 
  }
    
  return accepted;  
    
}

bool Atlas_1911_12606::sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }

double Atlas_1911_12606::mT2chi2(const TLorentzVector & vis1, const TLorentzVector & vis2, double m_inv, const TLorentzVector & invis) {
    // Setup mt2 evaluation object with top quarks. Modfified from AnalysisBase
    mt2_bisect::mt2 mt2_event;
    TLorentzVector zeroVector = TLorentzVector(0. ,0. ,0. ,0.);
    // If no invis is given, use missingET. Note that pmiss[0] is irrelvant, which is why we set it to -1.
    double pmiss[] = {-1, missingET->P4().Px(), missingET->P4().Py()};
    if (invis != zeroVector) {
        pmiss[0] = -1;
        pmiss[1] = invis.Px();
        pmiss[2] = invis.Py();
    }
 
    // Construct arrays that mt2_bisect needs as input and start evaluation
    double p1[3] = {0.0, vis1.Px(), vis1.Py()};
    double p2[3] = {0.0, vis2.Px(), vis2.Py()};
    mt2_event.set_momenta( p1, p2, pmiss );
    mt2_event.set_mn( m_inv );
    return mt2_event.get_mt2();  
}
