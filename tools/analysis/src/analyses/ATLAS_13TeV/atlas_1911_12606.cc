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
}

void Atlas_1911_12606::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsLoose = filterPhaseSpace(electronsLoose, 4.5, -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 4.5, -2.47, 2.47); //Tight fits efficiency better
  muonsCombined = filterPhaseSpace(muonsCombined, 3., -2.5, 2.5);  
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5); 
  std::vector<Track*> signal_tracks = filterPhaseSpace(tracks, 2., -2.5, 2.5); 
  signal_tracks = filterLeptons(signal_tracks);
  
  std::vector<Jet*> bjets;
  std::vector<Jet*> lightjets;
  
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta) < 2.5 && checkBTag(jets[i]) ) bjets.push_back(jets[i]);
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
  
  std::vector<Electron*> electrons_base = electronsLoose;
  std::vector<Muon*> muons_base = muonsCombined;
    
  std::vector<Electron*> electrons_signal = Isolate_leptons_with_inverse_track_isolation_cone(electronsTight, tracks, towers, 0.3, 10., 0.2, 0.15, 0.2, true);
  std::vector<Muon*> muons_signal = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined,  tracks, towers, 0.3, 10., 0.2, 0.06, 0.06, false);
  signal_tracks = Isolate_tracks(signal_tracks, 0.3, 0.5);
  
  lightjets = filterPhaseSpace(lightjets, 30., -2.8, 2.8); 
  
}

void Atlas_1911_12606::finalize() {
  // Whatever should be done after the run goes here
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
  for (int t = 0; t < cand_tracks.size(); t++ ) 
    if ( cand_tracks[t]->PT < 5. and (abs(cand_tracks[t]->PID) == 11 or abs(cand_tracks[t]->PID) == 13 ))
        accepted.push_back( cand_tracks[t]);
    
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
