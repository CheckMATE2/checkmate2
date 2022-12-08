#include "atlas_2211_08028.h"
#include "fastjet/tools/Filter.hh"
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
}

void Atlas_2211_08028::analyze() {

  signal_jets.clear();
  bjets.clear();
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
  std::vector<fastjet::PseudoJet> trimmedJets;
  
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
    
  }
  
  if (electronsLoose.size() + muonsCombined.size() == 0) {
    countCutflowEvent("01_ObaseLep");
    
    if (dphimin < 0.4) return;
    countCutflowEvent("02_phimin");
    
    if (PassesCuts_Gtt0L(5, 3, 600., 2900., 120., 300., "SR-Gtt-0L-B")) countSignalEvent("SR-Gtt-0L-B");
    if (PassesCuts_Gtt0L(9, 3, 600., 1700., 120., 300., "SR-Gtt-0L-M1")) countSignalEvent("SR-Gtt-0L-M1");
    if (PassesCuts_Gtt0L(10, 3, 500., 1100., 120., 200., "SR-Gtt-0L-M2")) countSignalEvent("SR-Gtt-0L-M2");
    if (PassesCuts_Gtt0L(10, 4, 400.,  800., 180., 100., "SR-Gtt-0L-C")) countSignalEvent("SR-Gtt-0L-C");
    
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
