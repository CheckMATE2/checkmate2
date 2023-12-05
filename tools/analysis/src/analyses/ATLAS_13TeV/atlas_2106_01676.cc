#include "atlas_2106_01676.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_2106_01676::initialize() {
  setAnalysisName("atlas_2106_01676");          
  setInformation(""
    "# electroweakinos, 3 leptons, WZ, WH, on+off-shell\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("SR-WZ-01;SR-WZ-02;SR-WZ-03;SR-WZ-04;SR-WZ-05;SR-WZ-06;SR-WZ-07;SR-WZ-08;SR-WZ-09;SR-WZ-10;SR-WZ-11;SR-WZ-12;SR-WZ-13;SR-WZ-14;SR-WZ-15;SR-WZ-16;SR-WZ-17;SR-WZ-18;SR-WZ-19;SR-WZ-20;SR-Wh-SF-01;SR-Wh-SF-02;SR-Wh-SF-03;SR-Wh-SF-04;SR-Wh-SF-05;SR-Wh-SF-06;SR-Wh-SF-07;SR-Wh-SF-08;SR-Wh-SF-09;SR-Wh-SF-10;SR-Wh-SF-11;SR-Wh-SF-12;SR-Wh-SF-13;SR-Wh-SF-14;SR-Wh-SF-15;SR-Wh-SF-16;SR-Wh-SF-17;SR-Wh-SF-18;SR-Wh-SF-19;SR-Wh-DF-1;SR-Wh-DF-2;SR-offWZ-low-0jb;SR-offWZ-low-0jc;SR-offWZ-low-0jd;SR-offWZ-low-0je;SR-offWZ-low-0jf1;SR-offWZ-low-0jf2;SR-offWZ-low-0jg1;SR-offWZ-low-0jg2;SR-offWZ-low-njb;SR-offWZ-low-njc;SR-offWZ-low-njd;SR-offWZ-low-nje;SR-offWZ-low-njf1;SR-offWZ-low-njf2;SR-offWZ-low-njg1;SR-offWZ-low-njg2;SR-offWZ-high-0jb;SR-offWZ-high-0jc;SR-offWZ-high-0jd;SR-offWZ-high-0je;SR-offWZ-high-0jf1;SR-offWZ-high-0jf2;SR-offWZ-high-0jg1;SR-offWZ-high-0jg2;SR-offWZ-high-nja;SR-offWZ-high-njb;SR-offWZ-high-njc;SR-offWZ-high-njd;SR-offWZ-high-nje;SR-offWZ-high-njf;SR-offWZ-high-njg");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_2106_01676::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out. Probably not since 3.4.2
  
  pTmiss = missingET->P4();
  
  electrons_off.clear();
  muons_off.clear();
  electrons_off = filterPhaseSpace(electronsMedium, 8., -2.47, 2.47); 
  muons_off = filterPhaseSpace(muonsCombined, 8., -2.5, 2.5);    
  std::vector<Electron*> electrons_signal = electrons_off;
  std::vector<Muon*> muons_signal = muons_off; 
  
  int nel_true = 0; int nmu_true = 0;
  for (int t = 0; t < true_particles.size(); t++ ) {
    if ( abs(true_particles[t]->PID) == 11 and true_particles[t]->Status == 1 ) {
        double eff1 = rand()/(RAND_MAX + 1.);
        double pt = true_particles[t]->PT;
        if ( pt >  4.5) nel_true++;
        for (int i = 0; i < electrons_off.size(); i++)
            if ( electrons_off[i]->Particle == true_particles[t] ) break;
        if (eff1 < ( (pt > 4.5)*(pt < 5.0)*0.1 + (pt > 5)*(pt < 6.0)*0.19 + (pt > 6)*(pt < 8.0)*0.27 ) and fabs(true_particles[t]->Eta) < 2.47) {
              Electron ele = Electron();
              ele.PT = pt;
              ele.Phi = true_particles[t]->Phi;
              ele.Eta = true_particles[t]->Eta;
              ele.P4() = true_particles[t]->P4();
              ele.Particle = true_particles[t];
              ele.Charge = true_particles[t]->Charge;
              electrons_off.push_back(&ele);
        }
    }
    if ( abs(true_particles[t]->PID) == 13 and true_particles[t]->Status == 1 ) {
        double eff1 = rand()/(RAND_MAX + 1.);
        double pt = true_particles[t]->PT;
        if ( pt >  3.) nmu_true++;
        for (int i = 0; i < muons_off.size(); i++)
            if ( muons_off[i]->Particle == true_particles[t] ) break;
        if (eff1 < ((pt > 3.0)*(pt < 4.0)*0.18 + (pt > 4.0)*(pt < 5.0)*0.32 + (pt > 5.0)*(pt < 6.0)*0.43 + (pt > 6.0)*(pt < 8.0)*0.47 ) and fabs(true_particles[t]->Eta) < 2.5 ) {
              Muon muo = Muon();
              muo.PT = pt;
              muo.Phi = true_particles[t]->Phi;
              muo.Eta = true_particles[t]->Eta;
              muo.P4() = true_particles[t]->P4();
              muo.Particle = true_particles[t];
              muo.Charge = true_particles[t]->Charge;
              muons_off.push_back(&muo);
        }
    }
  }
  
  truemet = TLorentzVector(0.,0.,0.,0.);
  /*int nlsp = 0;
  for (int t = 0; t < true_particles.size(); t++ ) 
    if ( true_particles[t]->PID == 1000022 and true_particles[t]->Status == 1 ) {
      truemet += true_particles[t]->P4();
      nlsp++;
    }
  pTmiss = truemet;
  cout << nlsp;*/
  
  std::sort(electrons_off.begin(), electrons_off.end(), sortByPTEl);
  std::sort(muons_off.begin(), muons_off.end(), sortByPTMu);
  
  electronsLoose = filterPhaseSpace(electronsLoose, 4.5, -2.47, 2.47);
  jets_off.clear();
  jets_signal.clear();
  jets_off = filterPhaseSpace(jets, 20., -2.8, 2.8); 
  jets_signal = filterPhaseSpace(jets, 20., -2.8, 2.8); 
  
  jets_off = overlapRemoval(jets_off, electronsLoose, 0.2, "y");
  jets_off = overlapRemoval_muon_jet_tracks(jets_off, muonsCombined, 0.4, 2);
  jets_signal = overlapRemoval(jets_signal, electronsLoose, 0.2, "y");
  jets_signal = overlapRemoval_muon_jet_tracks(jets_signal, muonsCombined, 0.4, 2);  
  electronsLoose = overlapRemoval(electronsLoose, jets_off, 0.4, "y"); 
  electrons_off = overlapRemoval(electrons_off, jets_off, 0.4, "y");   
  electrons_signal = overlapRemoval(electrons_signal, jets_signal, 0.4, "y");   
  muons_off = overlapRemoval(muons_off, jets_off, 0.4, "y"); 
  muons_signal = overlapRemoval(muons_signal, jets_signal, 0.4, "y"); 
  //cout << jets_off.size() << endl;
  
  electrons_off = Isolate_leptons_with_inverse_track_isolation_cone(electrons_off, tracks, towers, 0.3, 10., 0.2, 0.15, 0.2, true);
  muons_off = Isolate_leptons_with_inverse_track_isolation_cone(muons_off,  tracks, towers, 0.3, 10., 0.2, 0.06, 0.06, false);
  electrons_signal = Isolate_leptons_with_inverse_track_isolation_cone(electrons_signal, tracks, towers, 0.3, 10., 0.2, 0.15, 0.2, true);
  muons_signal = Isolate_leptons_with_inverse_track_isolation_cone(muons_signal,  tracks, towers, 0.3, 10., 0.2, 0.06, 0.06, false);  
  
  double met = missingET->P4().Perp();
  
  trigger = false;
  if (electrons_off.size() > 1 and electrons_off[1]->PT > 18.) trigger = true;
  else if (muons_off.size() and muons_off[0]->PT > 27.3) trigger = true;
  else if (muons_off.size() > 1 and muons_off[1]->PT > 14.7) trigger = true;
  else if (muons_off.size() > 2 and muons_off[2]->PT > 6.5) trigger = true;
  else if ( met > 200. ) trigger = true;
  
  countCutflowEvent("00_all");
  
  if (electrons_off.size() + muons_off.size() != 3) return;
  countCutflowEvent("01_3leptons");
  
  bveto = false;
  for (int i = 0; i < jets_off.size(); i++) 
    if ( fabs(jets_off[i]->Eta) < 2.5 && checkBTag(jets_off[i]) ) {bveto = true; break;}
  countCutflowEvent("02_bveto");  
  
  if (!trigger) return;
  countCutflowEvent("03_trigger");
  
  mllmin = 10000.;
  mllmax = 0.;
  drmin = 100.;
  SFOS = false;
  if ( electrons_off.size() == 2 and electrons_off[0]->Charge * electrons_off[1]->Charge < 0) {
    SFOS = true;
    lepton3 = new FinalStateObject(muons_off[0]);
    pair = electrons_off[0]->P4() + electrons_off[1]->P4();
    mllmin = mllmax = pair.M();
    drmin =  electrons_off[0]->P4().DeltaR(electrons_off[1]->P4());
  }
  else if ( muons_off.size() == 2 and muons_off[0]->Charge * muons_off[1]->Charge < 0) {
    SFOS = true;
    lepton3 = new FinalStateObject(electrons_off[0]);
    pair = muons_off[0]->P4() + muons_off[1]->P4();
    mllmin = mllmax = pair.M();
    drmin =  muons_off[0]->P4().DeltaR(muons_off[1]->P4());
  }   
  else if ( electrons_off.size() == 3 ) {
    for (int i = 0; i < electrons_off.size() - 1; i++)
      for (int j = 0; j < electrons_off.size(); j++) {
          if ( electrons_off[i]->Charge * electrons_off[j]->Charge < 0)  {
            double mll = (electrons_off[i]->P4() + electrons_off[j]->P4()).M();
            double dr =  electrons_off[i]->P4().DeltaR(electrons_off[j]->P4());
            SFOS = true;
            if (dr < drmin) drmin = dr;
            if (mll > mllmax) mllmax = mll;
            if (mll < mllmin) {
              mllmin = mll;
              pair = electrons_off[i]->P4() + electrons_off[1]->P4();
              for (int k = 0; k < electrons_off.size(); k++)
                if ( k!= i and k != j) lepton3 = new FinalStateObject(electrons_off[k]);
            }
          }
      }
  }
  else if ( muons_off.size() == 3 ) {
    for (int i = 0; i < muons_off.size() - 1; i++)
      for (int j = 0; j < muons_off.size(); j++) {
          if ( muons_off[i]->Charge * muons_off[j]->Charge < 0)  {
            double mll = (muons_off[i]->P4() + muons_off[j]->P4()).M();
            double dr =  muons_off[i]->P4().DeltaR(muons_off[j]->P4());
            SFOS = true;
            if (dr < drmin) drmin = dr;
            if (mll > mllmax) mllmax = mll;
            if (mll < mllmin) {
              mllmin = mll;
              pair = muons_off[i]->P4() + muons_off[1]->P4();
              for (int k = 0; k < muons_off.size(); k++)
                if ( k!= i and k != j) lepton3 = new FinalStateObject(muons_off[k]);
            }
          }
      }
  }
  
  if ( !SFOS or mllmax > 75.) return;
  countCutflowEvent("04_mllmax");
  
  if ( drmin < 0.4) return;
  countCutflowEvent("05_dr>0.4");
  
  HT = 0.;
  for (int i = 0; i < jets_off.size(); i++) 
    HT += jets_off[i]->PT;
  
  leptons.clear();
  for(int e = 0; e < electrons_off.size(); e++) {
    FinalStateObject* lep = newFinalStateObject(electrons_off[e]);
    leptons.push_back(lep);
  }
  for(int m = 0; m < muons_off.size(); m++) {
    FinalStateObject* lep = newFinalStateObject(muons_off[m]);
    leptons.push_back(lep);
  }
  std::sort( leptons.begin(), leptons.end(), sortByPT );  
  
  if (preselection_offWZ()) {
  
    if (pass_SRoffWZhighnj( 1. , 12., 0.2, "a", true)) countSignalEvent("SR-offWZ-high-nja");
    if (pass_SRoffWZhighnj( 12., 15., 0.2, "b", true)) countSignalEvent("SR-offWZ-high-njb");
    if (pass_SRoffWZhighnj( 15., 20., 0.3, "c", false)) countSignalEvent("SR-offWZ-high-njc");
    if (pass_SRoffWZhighnj( 20., 30., 0.3, "d", false)) countSignalEvent("SR-offWZ-high-njd");
    if (pass_SRoffWZhighnj( 30., 40., 0.3, "e", false)) countSignalEvent("SR-offWZ-high-nje");
    if (pass_SRoffWZhighnj( 40., 60., 1.0, "f", false)) countSignalEvent("SR-offWZ-high-njf");
    if (pass_SRoffWZhighnj( 60., 75., 1.0, "g", false)) countSignalEvent("SR-offWZ-high-njg");
  
    //if ( mllmin > 1. and mllmin < 12. ) countCutflowEvent("soft_05_bina");
    //if ( mllmin > 12. and mllmin < 15. ) countCutflowEvent("soft_05_binb");
  
    if (pass_SRoffWZhigh0j( 12., 15., 50., "b", true)) countSignalEvent("SR-offWZ-high-0jb");
    if (pass_SRoffWZhigh0j( 15., 20., 50., "c", false)) countSignalEvent("SR-offWZ-high-0jc");
    if (pass_SRoffWZhigh0j( 20., 30., 60., "d", false)) countSignalEvent("SR-offWZ-high-0jd");
    if (pass_SRoffWZhigh0j( 30., 40., 60., "e", false)) countSignalEvent("SR-offWZ-high-0je");
    if (pass_SRoffWZhigh0j( 40., 60., 70., "f1", false)) countSignalEvent("SR-offWZ-high-0jf1");
    if (pass_SRoffWZhigh0j( 40., 60., 90., "f2", false)) countSignalEvent("SR-offWZ-high-0jf2");
    if (pass_SRoffWZhigh0j( 60., 75., 70., "g1", false)) countSignalEvent("SR-offWZ-high-0jg1");  
    if (pass_SRoffWZhigh0j( 60., 75., 90., "g2", false)) countSignalEvent("SR-offWZ-high-0jg2");  
  
    if (pass_SRoffWZlow( 12., 15., 50., 1.1, false, "b", true)) countSignalEvent("SR-offWZ-low-0jb");
    if (pass_SRoffWZlow( 15., 20., 50., 1.1, false, "c", false)) countSignalEvent("SR-offWZ-low-0jc");
    if (pass_SRoffWZlow( 20., 30., 50., 1.1, false, "d", false)) countSignalEvent("SR-offWZ-low-0jd");
    if (pass_SRoffWZlow( 30., 40., 60., 1.3, false, "e", false)) countSignalEvent("SR-offWZ-low-0je");
    if (pass_SRoffWZlow( 40., 60., 60., 1.4, false, "f1", false)) countSignalEvent("SR-offWZ-low-0jf1");
    if (pass_SRoffWZlow( 40., 60., 90., 1.4, false, "f2", false)) countSignalEvent("SR-offWZ-low-0jf2");
    if (pass_SRoffWZlow( 60., 75., 60., 1.4, false, "g1", false)) countSignalEvent("SR-offWZ-low-0jg1");  
    if (pass_SRoffWZlow( 60., 75., 90., 1.4, false, "g2", false)) countSignalEvent("SR-offWZ-low-0jg2");    
  
    if (pass_SRoffWZlow( 12., 15., 50., 1.0, true, "b", true)) countSignalEvent("SR-offWZ-low-njb");
    if (pass_SRoffWZlow( 15., 20., 50., 1.0, true, "c", false)) countSignalEvent("SR-offWZ-low-njc");
    if (pass_SRoffWZlow( 20., 30., 50., 1.0, true, "d", false)) countSignalEvent("SR-offWZ-low-njd");
    if (pass_SRoffWZlow( 30., 40., 60., 1.0, true, "e", false)) countSignalEvent("SR-offWZ-low-nje");
    if (pass_SRoffWZlow( 40., 60., 60., 1.2, true, "f1", false)) countSignalEvent("SR-offWZ-low-njf1");
    if (pass_SRoffWZlow( 40., 60., 90., 1.2, true, "f2", false)) countSignalEvent("SR-offWZ-low-njf2");
    if (pass_SRoffWZlow( 60., 75., 60., 1.2, true, "g1", false)) countSignalEvent("SR-offWZ-low-njg1");  
    if (pass_SRoffWZlow( 60., 75., 90., 1.2, true, "g2", false)) countSignalEvent("SR-offWZ-low-njg2");    
  }
  
  return;
  
}

void Atlas_2106_01676::finalize() {
  // Whatever should be done after the run goes here
}       

bool Atlas_2106_01676::sortByPTEl(Electron *i, Electron *j) { return (i->PT > j->PT); }
bool Atlas_2106_01676::sortByPTMu(Muon *i, Muon *j) { return (i->PT > j->PT); }

bool Atlas_2106_01676::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

std::vector<Jet*> Atlas_2106_01676::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
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

bool Atlas_2106_01676::preselection_onWZ() {
  
  
  return true;
}

bool Atlas_2106_01676::preselection_Wh() {
  
  
  return true;
}

bool Atlas_2106_01676::preselection_offWZ() {

  if (bveto) return false;
  countCutflowEvent("offall_02_bveto");  
  
  if (!trigger) return false;
  countCutflowEvent("offall_03_trigger");
  
  if ( !SFOS or mllmax > 75.) return false;
  countCutflowEvent("offall_04_mllmax");
  
  if ( drmin < 0.4) return false;
  countCutflowEvent("offall_05_dr>0.4");  
  
  
  return true;  
}

bool Atlas_2106_01676::pass_SRoffWZhighnj( double min, double max, double ptl, std::string bin, bool cutflow) {
  
  if (mllmin < min or mllmin > max) return false;
  if (cutflow) countCutflowEvent("offhighnj_"+bin+"_05_mll");
  
  double mt2 = mT2chi2( pair, lepton3->P4(), 100., pTmiss );
  if ( mt2 > 100.+max) return false;
  if (cutflow) countCutflowEvent("offhighnj_"+bin+"_06_mT2");
  
  if ( !jets_off.size() or jets_off[0]->PT < 30.) return false;
  if (cutflow) countCutflowEvent("offhighnj_"+bin+"_07_nj");
  
  if (pTmiss.Perp() < 200.) return false;
  if (cutflow) countCutflowEvent("offhighnj_"+bin+"_08_MET");
  
  if ( HT != 0. and pTmiss.Perp()/sqrt(HT) < 3. ) return false;
  if (cutflow) countCutflowEvent("offhighnj_"+bin+"_09_METsig");
  
  if (lepton3->PT/pTmiss.Perp() > ptl) return false;
  if (cutflow) countCutflowEvent("offhighnj_"+bin+"_10_PTl3");
  
  return true;
}

bool Atlas_2106_01676::pass_SRoffWZhigh0j( double min, double max, double mTminmll, std::string bin, bool cutflow) {
  
  if (leptons.size() < 3 or leptons[0]->PT < 25. or leptons[1]->PT < 15. or leptons[2]->PT < 10.) return false;
  if (cutflow) countCutflowEvent("offhigh0j_"+bin+"_05_pTlep");
  
  if (mllmin < min or mllmin > max) return false;
  if (cutflow) countCutflowEvent("offhigh0j_"+bin+"_06_mll");
  
  double mt2 = mT2chi2( pair, lepton3->P4(), 100., pTmiss );
  if ( mt2 > 100.+max) return false;
  if (cutflow) countCutflowEvent("offhigh0j_"+bin+"_07_mT2");
  
  if ( jets_off.size() and jets_off[0]->PT > 30.) return false;
  if (cutflow) countCutflowEvent("offhigh0j_"+bin+"_08_0j");
  
  if (pTmiss.Perp() < 50.) return false;
  if (cutflow) countCutflowEvent("offhigh0j_"+bin+"_09_MET");
  
  if (  HT != 0. and pTmiss.Perp()/sqrt(HT) < 3. ) return false;
  if (cutflow) countCutflowEvent("offhigh0j_"+bin+"_10_METsig");
  
  if ( bin != "f2" and bin != "g2" and  mT(lepton3->P4(), pTmiss, 0.) > mTminmll) return false;
  if ( (bin == "f2" or bin == "g2") and  mT(lepton3->P4(), pTmiss, 0.) < mTminmll) return false;
  if (cutflow) countCutflowEvent("offhigh0j_"+bin+"_11_mTminMll");
  
  return true;
}

bool Atlas_2106_01676::pass_SRoffWZlow( double min, double max, double mTminmll, double ptl, bool jets, std::string bin, bool cutflow) {
  
  std::string label;
  if (jets) label = "offlownj_"; else label = "offlow0j_";
  
  if (bin == "b" or bin == "c" or bin == "d" or bin == "e")
    if (mllmin < 12. or mllmin > 40. ) return false;
  else 
    if (mllmin < 40. or mllmin > 75. ) return false;
  if (cutflow) countCutflowEvent(label+bin+"_05_mllmin");
  
  if (lepton3->Type == "electron") {
    if ( fabs( (leptons[0]->P4() + leptons[1]->P4() + leptons[2]->P4()).M() - 91.2) <  20. ) return false;
    if ( drmin < 0.6  or drmin > 2.4) return false;
  }
  if (cutflow) countCutflowEvent(label+bin+"_06_conversion");
    
  if (bin == "b" or bin == "c" or bin == "d" or bin == "e")
    if (leptons.size() < 3 or leptons[2]->PT < 10.) return false;
  else 
    if (leptons.size() < 3 or leptons[2]->PT < 15.) return false;
  if (cutflow) countCutflowEvent(label+bin+"_07_pTlep");
  
  if (bin == "b" or bin == "c" or bin == "d" or bin == "e")
    if (mllmax > 60.) return false;
  if (cutflow) countCutflowEvent(label+bin+"_08_mllmax");
  
  if (mllmin < min or mllmin > max) return false;
  if (cutflow) countCutflowEvent(label+bin+"_09_mll");
  
  double mt2 = mT2chi2( pair, lepton3->P4(), 100., pTmiss );
  if (bin == "b" or bin == "c" or bin == "d")
    if ( mt2 > 100.+max) return false;
  if (cutflow) countCutflowEvent(label+bin+"_10_mT2");
  
  if (bin == "b" or bin == "c" or bin == "d")
    if ( drmin > 1.6) return false;
  if (cutflow) countCutflowEvent(label+bin+"_11_drmin");
  
  if ( (!jets and jets_off.size() and jets_off[0]->PT > 30.) or (jets and (!jets_off.size() or jets_off[0]->PT < 30.)) ) return false;
  if (cutflow) countCutflowEvent(label+bin+"_12_jets");
  
  if ((!jets and pTmiss.Perp() > 50.) or (jets and pTmiss.Perp() > 200.) ) return false;
  if (cutflow) countCutflowEvent(label+bin+"_13_MET");
  
  if ((!jets and  HT != 0. and pTmiss.Perp()/sqrt(HT) < 1.5) or (jets and  HT != 0. and pTmiss.Perp()/sqrt(HT) < 3.0) ) return false;
  if (cutflow) countCutflowEvent(label+bin+"_14_METsig");
  
  if ( !jets and (bin == "f2" or bin == "g2" or bin == "f1" or bin == "g1") and  (leptons[0]->P4() + leptons[1]->P4() + leptons[2]->P4()).M() < 100.)
  if (cutflow) countCutflowEvent(label+bin+"_15_m3l");  
  
  if ( bin != "f2" and bin != "g2" and  mT(lepton3->P4(), pTmiss, 0.) > mTminmll) return false;
  if ( (bin == "f2" or bin == "g2") and  mT(lepton3->P4(), pTmiss, 0.) < mTminmll) return false;
  if (cutflow) countCutflowEvent(label+bin+"_16_mTminMll");
  
  if (lepton3->PT/pTmiss.Perp() > ptl) return false;
  if (cutflow) countCutflowEvent(label+bin+"_17_PTl3");
  
  return true;
}

double Atlas_2106_01676::mT2chi2(const TLorentzVector & vis1, const TLorentzVector & vis2, double m_inv, const TLorentzVector & invis) {
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
