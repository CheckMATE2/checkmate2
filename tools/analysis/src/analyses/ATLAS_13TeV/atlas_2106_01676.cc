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

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  std::vector<Electron*> electrons_off = filterPhaseSpace(electronsMedium, 8., -2.47, 2.47); 
  std::vector<Muon*> muons_off = filterPhaseSpace(muonsCombined, 8., -2.5, 2.5);    
  
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
              //cout << "Mom: " << ele.P4().X() << "\n";
              ele.Particle = true_particles[t];
              ele.Charge = true_particles[t]->Charge;
              //electrons_soft_true.push_back(ele);
              //electrons_soft.push_back(&(*(electrons_soft_true.end()-1)));
              //electronsMedium.push_back(&(*(electrons_soft_true.end()-1)));
              electrons_off.push_back(&ele);
              //cout << "Mom: " << electrons_soft[electrons_soft.size()-1]->P4().X() << "\n";
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
              //muons_soft_true.push_back(muo);
              //muons_soft.push_back(&(*(muons_soft_true.end()-1)));
              //muonsCombined.push_back(&(*(muons_soft_true.end()-1)));
              muons_off.push_back(&muo);
        }
    }
  }
  
  std::sort(electrons_off.begin(), electrons_off.end(), sortByPTEl);
  std::sort(muons_off.begin(), muons_off.end(), sortByPTMu);
  
  electronsLoose = filterPhaseSpace(electronsLoose, 4.5, -2.47, 2.47);
  std::vector<Jet*> jets_off = filterPhaseSpace(jets, 20., -2.8, 2.8); 
  
  jets_off = overlapRemoval(jets_off, electronsLoose, 0.2, "y");
  jets_off = overlapRemoval_muon_jet_tracks(jets_off, muonsCombined, 0.4, 2);
  electronsLoose = overlapRemoval(electronsLoose, jets_off, 0.4, "y"); 
  electrons_off = overlapRemoval(electrons_off, jets_off, 0.4, "y");   
  muons_off = overlapRemoval(muons_off, jets_off, 0.4, "y"); 
  
  electrons_off = Isolate_leptons_with_inverse_track_isolation_cone(electrons_off, tracks, towers, 0.3, 10., 0.2, 0.15, 0.2, true);
  muons_off = Isolate_leptons_with_inverse_track_isolation_cone(muons_off,  tracks, towers, 0.3, 10., 0.2, 0.06, 0.06, false);
  
  double met = missingET->P4().Perp();
  
  bool trigger = false;
  if (electrons_off.size() > 1 and electrons_off[1]->PT > 18.) trigger = true;
  else if (muons_off.size() and muons_off[0]->PT > 27.3) trigger = true;
  else if (muons_off.size() > 1 and muons_off[1]->PT > 14.7) trigger = true;
  else if (muons_off.size() > 2 and muons_off[2]->PT > 6.5) trigger = true;
  else if ( met > 200. ) trigger = true;
  
  countCutflowEvent("00_all");
  
  if (electrons_off.size() + muons_off.size() != 3) return;
  countCutflowEvent("soft_01_3leptons");
  
  for (int i = 0; i < jets_off.size(); i++) 
    if ( fabs(jets_off[i]->Eta) < 2.5 && checkBTag(jets_off[i]) ) return;
  countCutflowEvent("soft_02_bveto");  
  
  if (!trigger) return;
  countCutflowEvent("soft_03_trigger");
    
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
