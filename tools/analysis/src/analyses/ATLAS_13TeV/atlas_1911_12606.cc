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
}

void Atlas_1911_12606::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsLoose = filterPhaseSpace(electronsLoose, 4.5, -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 4.5, -2.47, 2.47); //Tight fits efficiency better
  muonsCombined = filterPhaseSpace(muonsCombined, 3., -2.5, 2.5);  
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5); 
  signal_tracks = filterPhaseSpace(tracks, 2., -2.5, 2.5); 
  signal_tracks = filterLeptons(signal_tracks);
  
  n++;
  cout << "Event: " << n << "\n";
//  cout << "tracks: " << tracks.size() << "\n"; 
//  cout << "signal_tracks: " << signal_tracks.size() << "\n";    
  
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
  electronsTight = overlapRemoval(electronsTight, bjets, 0.4, "y");  
  electronsTight = overlapRemoval(electronsTight, lightjets, 0.4, "y");    
  muonsCombined = overlapRemoval(muonsCombined, bjets, 0.4, "y");  
  muonsCombined = overlapRemoval(muonsCombined, lightjets, 0.4, "y");     
  signal_tracks = overlapRemoval(signal_tracks, bjets, 0.5, "y");
  signal_tracks = overlapRemoval(signal_tracks, lightjets, 0.5, "y");
  
  pTmiss = TLorentzVector(0., 0., 0., 0.);
/*  for (int i = 0; i < lightjets.size(); i++) 
    pTmiss -= lightjets[i]->P4();      
  
  for (int i = 0; i < bjets.size(); i++) 
    pTmiss -= bjets[i]->P4();        
  
  for (int i = 0; i < muonsCombined.size(); i++) 
    pTmiss -= muonsCombined[i]->P4();      
  
  for (int i = 0; i < electronsLoose.size(); i++) 
    pTmiss -= electronsLoose[i]->P4();      
  
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
  
//  pTmiss += pTmiss_soft;  */
  
  std::vector<Electron*> electrons_base = electronsLoose;
  std::vector<Muon*> muons_base = muonsCombined;
    
  std::vector<Electron*> electrons_signal = Isolate_leptons_with_inverse_track_isolation_cone(electronsTight, tracks, towers, 0.3, 10., 0.2, 0.15, 0.2, true);
  std::vector<Muon*> muons_signal = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined,  tracks, towers, 0.3, 10., 0.2, 0.06, 0.06, false);
  signal_tracks = Isolate_tracks(signal_tracks, 0.3, 0.5);
  
//  cout << "muonsSignal: " << muons_signal.size() << "\n";
  
  lightjets = filterPhaseSpace(lightjets, 30., -2.8, 2.8);
  
  signal_jets = lightjets;
  signal_jets.insert(signal_jets.end(), bjets.begin(), bjets.end());
  std::sort(signal_jets.begin(), signal_jets.end(), sortByPT );     
  
  countCutflowEvent("00_all");
  
  met = missingET->P4().Et();
//  met = pTmiss.Perp();
  if (met < 80. ) return;
  countCutflowEvent("01_trigger");
  
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

  for (int i = 0; i < signal_jets.size(); i++) 
    pTmiss -= signal_jets[i]->P4(); 
  for (int i = 0; i < leptons.size(); i++) 
    pTmiss -= leptons[i]->P4(); 
  
  
  if ( leptons.size() == 1 and signal_tracks.size() > 0) {
      countCutflowEvent("1L1T_02_1L1T");
      mlt = sqrt(leptons[0]->P4().Dot(signal_tracks[0]->P4()));
      if (Pass_Cuts_1L1T()) {
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
  
  mll = sqrt(leptons[0]->P4().Dot(leptons[1]->P4()));
  flavour = -1;
  if (electrons_signal.size() == 2) flavour = 1;
  else if (muons_signal.size() == 2) flavour = 2;
  else flavour = 0;
  
  if (Pass_Preselection()) {
  
  Pass_EW_high();
  
  Pass_S_high();
  
  Pass_S_low();
  
  }
  
  return;
  
}

void Atlas_1911_12606::finalize() {
  // Whatever should be done after the run goes here
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
    
    double deltar = leptons[0]->P4().DeltaR(signal_tracks[0]->P4());
    if ( deltar < 0.05) return false;
    countCutflowEvent("1L1T_08_deltaR>0.05");
    
    if ( signal_jets.size() == 0 ) return false;
    countCutflowEvent("1L1T_09_jet");
    
    if ( signal_jets[0]->PT < 100. ) return false;
    countCutflowEvent("1L1T_10_jet>100");    
    
    double htlt = leptons[0]->PT + signal_tracks[0]->PT;
    if (met/htlt < 30.) return false;
    countCutflowEvent("1L1T_11_MET/HTl");    
    
    if ( deltar > 1.5) return false;
    countCutflowEvent("1L1T_12_deltaR<1.5");    
    
    if ( leptons[0]->PT > 10.) return false;
    countCutflowEvent("1L1T_13_lepton<10");    
    
    if ( signal_tracks[0]->PT > 5.) return false;
    countCutflowEvent("1L1T_14_track<5");   
    
    if ( (leptons[0]->Type == "electron" and abs(signal_tracks[0]->PID) != 11) or (leptons[0]->Type == "muon" and abs(signal_tracks[0]->PID) != 13) ) return false;
    countCutflowEvent("1L1T_15_SF");
    
    if ( leptons[0]->Charge * signal_tracks[0]->Charge > 0 ) return false;
    countCutflowEvent("1L1T_16_SS");
    
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
    
    if (mll < 1 or (flavour == 1 and mll < 3) ) return false;
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
    cout << "mtautau: " << mtt << "\n"; 
    if (mtt > 0. and mtt < 160.) return false;  
    countCutflowEvent("2L_12_mtautau");
    
    if (!flavour) return false;
    countCutflowEvent("2L_13_eemumu");
    
    return true;
    
}

bool Atlas_1911_12606::Pass_EW_high() {

    double mT = AnalysisBase::mT(leptons[0]->P4(), missingET->P4());
    if (mT > 60.) return false;
    countCutflowEvent("2L_Ehigh_14_mT<60");
    
    if (met < 200.) return false;
    countCutflowEvent("2L_Ehigh_15_met>200");
    
    if (met/signal_jets[0]->PT < std::max(0.85, 0.98 - 0.02*mll) or met/signal_jets[0]->PT > 1.) return false;//cheating
    countCutflowEvent("2L_Ehigh_16_cheatingISR");    
    
    if (leptons[1]->PT < std::min(10.,2.+mll/3.) ) return false;
    countCutflowEvent("2L_Ehigh_17_lepton2PT"); 
    
    return true;

}

bool Atlas_1911_12606::Pass_S_high() {
       
    if (met < 200.) return false;
    countCutflowEvent("2L_S-high_14_met>200");
    
    //double mt2 = mT2chi2( leptons[0]->P4(), leptons[1]->P4(), 100., missingET->P4() );
    double mt2 = mT2chi2( leptons[0]->P4(), leptons[1]->P4(), 100., pTmiss );
    
    TLorentzVector ISR(0.,0.,0.,0.);
    for (int i = 0; i < signal_jets.size(); i++)
      ISR += signal_jets[i]->P4(); 
    
    TLorentzVector ISR2 = -missingET->P4() - leptons[0]->P4() - leptons[1]->P4();
    
    TVector3 pTjet(ISR.X(), ISR.Y(), 0.);
    TVector3 pTjet2(ISR2.X(), ISR2.Y(), 0.);
    double RISR = fabs(missingET->P4().Vect().Dot(pTjet.Unit()))/ISR.Perp();
    cout << "RISR: " << RISR  << "\n"; 
    double RISR2 = fabs(pTmiss.Vect().Dot(pTjet.Unit()))/ISR.Perp();
    cout << "RISR2: " << RISR2  << "\n"; 
    double RISR3 = fabs(missingET->P4().Vect().Dot(pTjet2.Unit()))/ISR2.Perp();
    cout << "RISR3: " << RISR3  << "\n";     
    if ( RISR > 1.) {
         cout << "Momenta: " << "\n"; 
         cout << "lep1: " << leptons[0]->P4().X() << "   " << leptons[0]->P4().Y() << "\n"; 
         cout << "lep2: " << leptons[1]->P4().X() << "   " << leptons[1]->P4().Y() << "\n"; 
         cout << "MET: " << missingET->P4().X() << "   " << missingET->P4().Y() << "\n"; 
         cout << "MET2: " << pTmiss.X() << "   " << pTmiss.Y() << "\n"; 
         for (int i = 0; i < signal_jets.size(); i++)
             cout << "jet: " << signal_jets[i]->P4().X() << "   " << signal_jets[i]->P4().Y() << "\n"; 
    }
    if (RISR3  < std::max(0.85, 0.98 - 0.02*mt2) ) return false;//cheating   or met/ISR.Perp() > 1.
    countCutflowEvent("2L_S-high_15_cheatingISR");    
    
    if (leptons[1]->PT < std::min(20.,2.5 + (mt2 - 100.)*2.5) ) return false;
    countCutflowEvent("2L_S-high_16_lepton2PT"); 
    
    if (mt2 > 140.) return false;
    countCutflowEvent("2L_S-high_17_mt2<140"); 
    
    //if (falvour==1) countSignalEvent("SR_S
    
    return true;
}

bool Atlas_1911_12606::Pass_S_low() {
    
    if (met > 200. or met < 150.) return false;
    countCutflowEvent("2L_S-low_14_met<200");
    
    double mt2 = mT2chi2( leptons[0]->P4(), leptons[1]->P4(), 100., missingET->P4() );
    
    TLorentzVector ISR(0.,0.,0.,0.);
    for (int i = 0; i < signal_jets.size(); i++)
      ISR += signal_jets[i]->P4(); 
    
    TLorentzVector ISR2 = -missingET->P4() - leptons[0]->P4() - leptons[1]->P4();
    
    TVector3 pTjet(ISR.X(), ISR.Y(), 0.);
    TVector3 pTjet2(ISR2.X(), ISR2.Y(), 0.);
    double RISR = fabs(missingET->P4().Vect().Dot(pTjet.Unit()))/ISR.Perp();
    cout << "RISR: " << RISR  << "\n"; 
    double RISR2 = fabs(pTmiss.Vect().Dot(pTjet.Unit()))/ISR.Perp();
    cout << "RISR2: " << RISR2  << "\n"; 
    double RISR3 = fabs(missingET->P4().Vect().Dot(pTjet2.Unit()))/ISR2.Perp();
    cout << "RISR3: " << RISR3  << "\n";         
    if ( RISR > 1.) {
         cout << "Momenta: " << "\n"; 
         cout << "lep1: " << leptons[0]->P4().X() << "   " << leptons[0]->P4().Y() << "\n"; 
         cout << "lep2: " << leptons[1]->P4().X() << "   " << leptons[1]->P4().Y() << "\n"; 
         cout << "MET: " << missingET->P4().X() << "   " << missingET->P4().Y() << "\n"; 
         cout << "MET2: " << pTmiss.X() << "   " << pTmiss.Y() << "\n"; 
         for (int i = 0; i < signal_jets.size(); i++)
             cout << "jet: " << signal_jets[i]->P4().X() << "   " << signal_jets[i]->P4().Y() << "\n"; 
    }
    if (RISR3 < 0.8  ) return false;//cheating or met/ISR.Perp() > 1.
    countCutflowEvent("2L_S-low_15_cheatingISR");    
    
    if (leptons[1]->PT < std::min(20.,2.5 + (mt2 - 100.)*2.5) ) return false;
    countCutflowEvent("2L_S-low_16_lepton2PT"); 
    
    if (mt2 > 140.) return false;
    countCutflowEvent("2L_S-low_17_mt2<140"); 
    
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
        for (int i = 0; i < electronsTight.size(); i++ ) {
            if (cand_tracks[t]->P4().DeltaR(electronsTight[i]->P4()) < 0.01) break;
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

std::vector<Track*> Atlas_1911_12606::Isolate_tracks(std::vector<Track*> cand_tracks, double dR, double mom) {
    
  std::vector<Track*> accepted;    
  
  for (int i = 0; i < cand_tracks.size(); i++ ) 
    for (int t = 0; t < tracks.size(); t++ ) 
      if ( tracks[t]->PT > mom and cand_tracks[i]->P4().DeltaR(tracks[t]->P4()) < dR)
        break;
    
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
