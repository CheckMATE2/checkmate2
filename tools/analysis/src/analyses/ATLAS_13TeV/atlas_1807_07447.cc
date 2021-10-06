#include "atlas_1807_07447.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1807_07447::initialize() {
  setAnalysisName("atlas_1807_07447");          
  setInformation(""
    "# model-independent search for whatever\n"
    "# numbers from histograms, errors might happen\n"
    "# interpret results with caution\n"
    "# additional signal regions in cutflow\n"
  "");
  setLuminosity(3.21*units::INVFB);      
  sigregions = "2j;3j;4j;5j;6j;7j;8j;9j;10j;11j;12j;13j;14j;1b1j;1b2j;1b3j;1b4j;1b5j;1b6j;1b7j;1b8j;1b9j;1b10j;1b11j;2b;2b1j;2b2j;2b3j;2b4j;2b5j;2b6j;2b7j;2b8j;2b9j;3b;3b1j;3b2j;3b3j;3b4j;3b5j;3b6j;3b7j;3b8j;3b9j;4b;4b1j;4b2j;4b3j;4b4j;4b5j;4b6j;5b;5b1j;5b2j;5b3j;5b4j;6b;6b1j;6b2j;6b3j;7b;8b1j;1g1j;1g2j;1g3j;1g4j;1g5j;1g6j;1g7j;1g8j;1g9j;1g10j;1g1b;1g1b1j;1g1b2j;1g1b3j;1g1b4j;1g1b5j;1g1b6j;1g1b7j;1g1b8j;1g2b;1g2b1j;1g2b2j;1g2b3j;1g2b4j;1g2b5j;1g2b6j;1g2b7j;1g3b;1g3b1j;1g3b2j;1g3b3j;1g3b4j;1g4b;1g4b1j;1g4b2j;2g;2g1j;2g2j;2g3j;2g4j;2g5j;2g6j;2g7j;2g1b;2g1b1j;2g1b2j;2g1b3j;2g1b4j;2g1b5j;2g2b;2g2b1j;2g2b2j;2g2b3j;3g;3g1j;1e5j;1e6j;1e7j;1e8j;1e9j;1e1b4j;1e1b5j;1e1b6j;1e1b7j;131b8j;1e1b9j;1e2b;1e2b1j;1e2b2j;1e2b3j;1e2b4j;1e2b5j;1e2b6j;1e2b7j;1e2b8j;1e3b;1e3b1j;1e3b2j;1e3b3j;1e3b4j;1e3b5j;1e3b6j;1e4b;1e4b1j;1e4b2j;1e4b3j;1e4b4j;1e4b5j;1e5b;1e5b1j;1e5b2j;1m1j;1m2j;1m3j;1m4j;1m5j;1m6j;1m7j;1m8j;1m9j;1m1b;1m1b1j;1m1b2j;1m1b3j;1m1b4j;1m1b5j;1m1b6j;1m1b7j;1m1b8j;1m1b9j;1m2b;1m2b1j;1m2b2j;1m2b3j;1m2b4j;1m2b5j;1m2b6j;1m2b7j;1m2b8j;1m3b;1m3b1j;1m3b2j;1m3b3j;1m3b4j;1m3b5j;1m3b6j;1m3b7j;1m4b;1m4b1j;1m4b2j;1m4b3j;1m4b4j;1m4b5j;1m5b;1m5b1j;1m5b2j;1m1e;1m1e1j;1m1e2j;1m1e3j;1m1e4j;1m1e5j;1m1e6j;1m1e7j;1m1e1b;1m1e1b1j;1m1e1b2j;1m1e1b3j;1m1e1b4j;1m1e1b5j;1m1e1b6j;1m1e1b7j;1m1e2b;1m1e2b1j;1m1e2b2j;1m1e2b3j;1m1e2b4j;1m1e2b5j;1m1e2b6j;1m1e3b;1m1e3b1j;1m1e3b2j;1m1e3b3j;1m1e3b4j;1m1e4b;1m1e4b1j;1m1e4b2j;2e;2e1j;2e2j;2e3j;2e4j;2e5j;2e6j;2e7j;2e8j;2e1b;2e1b1j;2e1b2j;2e1b3j;2e1b4j;2e1b5j;2e1b6j;2e1b7j;2e2b;2e2b1j;2e2b2j;2e2b3j;2e2b4j;2e2b5j;2e3b;2e3b1j;2e3b2j;2e3b3j;2e3b4j;2e4b;2e4b1j;2m;2m1j;2m2j;2m3j;2m4j;2m5j;2m6j;2m7j;2m8j;2m1b;2m1b1j;2m1b2j;2m1b3j;2m1b4j;2m1b5j;2m1b6j;2m1b7j;2m2b;2m2b1j;2m2b2j;2m2b3j;2m2b4j;2m2b5j;2m2b6j;2m3b;2m3b1j;2m3b2j;2m3b3j;2m4b;2m4b1j;3e;3e1j;3e2j;3e3j;3e4j;3e1b;3e1b1j;3e1b2j;3e1b3j;3e2b;3e2b1j;3e2b2j;1m2e;1m2e1j;1m2e2j;1m2e3j;1m2e4j;1m2e1b;1m2e1b1j;1m2e1b2j;1m2e1b3j;1m2e2b;1m2e2b1j;1m2e2b2j;1m2e2b5j;2m1e;2m1e1j;2m1e2j;2m1e3j;2m1e4j;2m1e1b;2m1e1b1j;2m1e1b2j;2m1e1b3j;2m1e2b;2m1e2b1j;2m1e2b2j;3m;3m1j;3m2j;3m3j;3m4j;3m5j;3m1b;3m1b1j;3m1b2j;3m1b3j;3m1b4j;3m2b;3m2b1j;3m2b2j;4e;4e1j;4e1b;1m3e;2m2e;2m2e1j;2m2e2j;2m2e1b;2m2e1b1j;3m1e2b;4m;4m1j;4m2j;1e1g;1e1g1j;1e1g2j;1e1g3j;1e1g4j;1e1g5j;1e1g6j;1e1g7j;1e1g1b;1e1g1b1j;1e1g1b2j;1e1g1b3j;1e1g1b4j;1e1g1b5j;1e1g2b;1e1g2b1j;1e1g2b2j;1e1g2b3j;1e1g2b4j;1e1g2b5j;1e1g3b;1e1g3b1j;1e1g3b2j;1e1g3b3j;1e2g;1e2g1j;1e2g2j;1e2g3j;1e2g1b1j;1e2g1b2j;1m1g;1m1g1j;1m1g2j;1m1g3j;1m1g4j;1m1g5j;1m1g6j;1m1g1b1j;1m1g1b;1m1g1b2j;1m1g1b3j;1m1g1b4j;1m1g1b5j;1m1g2b;1m1g2b1j;1m1g2b2j;1m1g2b3j;1m1g2b4j;1m1g2b5j;1m1g3b;1m1g3b1j;1m1g3b2j;1m2g;1m2g1j;1m2g3j;1m2g1b2j;2e1g;2e1g1j;2e1g2j;2e1g3j;2e1g4j;2e1g5j;2e1g1b;2e1g1b1j;2e1g1b2j;2e1g1b3j;2e1g2b;2e1g2b1j;2e1g3b;2e2g;1m1e1g;1m1e1g1j;1m1e1g2j;1m1e1g3j;1m1e1g1b;1m1e1g1b1j;1m1e1g1b2j;1m1e1g1b3j;1m1e1g1b4j;1m1e1g2b;1m1e1g2b1j;1m1e1g2b2j;2m1g;2m1g1j;2m1g2j;2m1g3j;2m1g4j;2m1g1b;2m1g1b1j;2m1g1b2j;2m1g1b3j;2m1g2b;2m1g2b1j;2m1g2b2j;2m2g;2m2g2j;2m1e1g1j;MET1e1j;MET1e2j;MET1e3j;MET1e4j;MET1e5j;MET1e6j;MET1e7j;MET1e8j;MET1e1b;MET1e1b1j;MET1e1b2j;MET1e1b3j;MET1e1b4j;MET1e1b5j;MET1e1b6j;MET1e1b7j;MET1e2b;MET1e2b1j;MET1e2b2j;MET1e2b3j;MET1e2b4j;MET1e2b5j;MET1e3b;MET1e3b1j;MET1e3b2j;MET1e3b3j;MET1e3b4j;MET1e4b1j;MET1e1g;MET1e1g1j;MET1e1g2j;MET1e1g3j;MET1e1g4j;MET1e1g1b;MET1e1g1b1j;MET1e1g1b2j;MET1e1g1b3j;MET1e1g2b2j;MET2e;MET2e1j;MET2e2j;MET2e3j;MET2e4j;MET2e1b;MET2e1b1j;MET2e1b2j;MET2e1b3j;MET2e1b4j;MET2e2b;MET2e2b1j;MET2e2b2j;MET2e2b3j;MET3e;MET3e1j;MET3e2j;MET1m1j;MET1m2j;MET1m3j;MET1m4j;MET1m5j;MET1m6j;MET1m7j;MET1m1b;MET1m1b1j;MET1m1b2j;MET1m1b3j;MET1m1b4j;MET1m1b5j;MET1m1b6j;MET1m1b7j;MET1m2b;MET1m2b1j;MET1m2b2j;MET1m2b3j;MET1m2b4j;MET1m2b5j;MET1m2b6j;MET1m3b;MET1m3b1j;MET1m3b2j;MET1m3b3j;MET1m3b4j;MET1m1g;MET1m1g1j;MET1m1g2j;MET1m1g3j;MET1m1g4j;MET1m1g5j;MET1m1g1b;MET1m1g1b1j;MET1m1g1b2j;MET1m1g1b3j;MET1m1e;MET1m1e1j;MET1m1e2j;MET1m1e3j;MET1m1e4j;MET1m1e5j;MET1m1e1b;MET1m1e1b1j;MET1m1e1b2j;MET1m1e1b3j;MET1m1e1b4j;MET1m1e2b;MET1m1e2b1j;MET1m1e2b2j;MET1m1e2b3j;MET1m1e2b4j;MET1m1e1g;MET1m1e1g1j;MET1m1e1g1b;MET1m2e;MET1m2e1j;MET1m2e2j;MET2m;MET2m1j;MET2m2j;MET2m3j;MET2m4j;MET2m5j;MET2m1b;MET2m1b1j;MET2m1b2j;MET2m1b3j;MET2m1b4j;MET2m2b;MET2m2b1j;MET2m2b2j;MET2m2b3j;MET2m1e;MET2m1e1j;MET2m1e2j;MET3m1j;MET3m2j;MET1j;MET2j;MET3j;MET4j;MET5j;MET6j;MET7j;MET8j;MET9j;MET1b;MET1b1j;MET1b2j;MET1b3j;MET1b4j;MET1b5j;MET1b6j;MET1b7j;MET1b8j;MET2b;MET2b1j;MET2b2j;MET2b3j;MET2b4j;MET2b5j;MET2b6j;MET3b;MET3b1j;MET3b2j;MET3b3j;MET3b4j;MET3b5j;MET4b;MET4b1j;MET4b2j;MET4b3j;MET1g;MET1g1j;MET1g2j;MET1g3j;MET1g4j;MET1g5j;MET1g6j;MET1g1b;MET1g1b1j;MET1g1b2j;MET1g1b3j;MET1g1b4j;MET1g2b;MET1g2b1j;MET1g2b2j;MET1g2b3j;MET2g;MET2g1j;MET2g2j;MET2g1b2j";
  bookSignalRegions(sigregions);
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1807_07447::analyze() {
  missingET->addMuons(muons); 
  
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47, true);
  electronsTight = filterPhaseSpace(electronsTight, 25., -2.47, 2.47, true);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.7, 2.7);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
  photonsMedium = filterPhaseSpace(photonsMedium, 25., -2.37, 2.37, true);

  std::vector<Jet*> bjets;
  std::vector<Jet*> lightjets;
  
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta) < 2.5 && checkBTag(jets[i]) ) bjets.push_back(jets[i]);
    else lightjets.push_back(jets[i]);  
    
  lightjets = overlapRemoval(lightjets, electronsLoose, 0.2);
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.4);  
  electronsLoose = overlapRemoval(electronsLoose, lightjets, 0.4); 
  electronsTight = overlapRemoval(electronsTight, bjets, 0.4);  
  electronsTight = overlapRemoval(electronsTight, lightjets, 0.4);   
  lightjets = overlapRemoval_muon_jet_tracks(lightjets, muonsCombined, 0.4, 2); 
  bjets = overlapRemoval_muon_jet_tracks(bjets, muonsCombined, 0.4, 2);  
  muonsCombined = overlapRemoval(muonsCombined, bjets, 0.4);  
  muonsCombined = overlapRemoval(muonsCombined, lightjets, 0.4);  
  lightjets = overlapRemoval(lightjets, photonsMedium, 0.4); 
  bjets = overlapRemoval(bjets, photonsMedium, 0.4); 
  photonsMedium = overlapRemoval(photonsMedium, electronsLoose, 0.4);
  photonsMedium = overlapRemoval(photonsMedium, muonsCombined, 0.4);
  
  electronsTight = filterIsolation(electronsTight);
  muonsCombined = filterIsolation(muonsCombined);
  electronsTight = isolateLeptons(electronsTight, 0.2);
  muonsCombined = isolateLeptons(muonsCombined, 0.3);
  photonsMedium = filterIsolation(photonsMedium);
  photonsMedium = isolatePhotons(photonsMedium, 0.4);
  
  muonsCombined = filterPhaseSpace(muonsCombined, 25., -2.7, 2.7);
  photonsMedium = filterPhaseSpace(photonsMedium, 50., -2.37, 2.37, true);  
  bjets = filterPhaseSpace(bjets, 60., -2.5, 2.5);
  lightjets = filterPhaseSpace(lightjets, 60., -2.8, 2.8);
  
  std::string label = "";
  int objects = 0;
  
  if (missingET->P4().Et() > 200.) {
    label += "MET"; 
    objects++;
  }
  
  if (muonsCombined.size() > 0) {
    label += std::to_string(muonsCombined.size()) + "m";
    objects += muonsCombined.size();
  }
  
  if (electronsTight.size() > 0) {
    label += std::to_string(electronsTight.size()) + "e";
    objects += electronsTight.size();
  } 
  
  if (photonsMedium.size() > 0) {
    label += std::to_string(photonsMedium.size()) + "g";
    objects += photonsMedium.size();
  } 
  
  if (bjets.size() > 0) {
    label += std::to_string(bjets.size()) + "b";
    objects += bjets.size();
  }   
  
  if (lightjets.size() > 0) {
    label += std::to_string(lightjets.size()) + "j";
    objects += lightjets.size();
  }   
  
  if (objects < 2 ) return;

//  if ( signalRegions.find(label) == signalRegions.end() )   
//  if (signalRegions.count(label) ) 
// The potential new signal regions are stored in cutflow. Note that since they are not in the original list
// the expected background is less than 0.1 events and 0 observed events so they might provide additional exclusion  
  std::string tmp = ";"+label+";";
  std::size_t found = sigregions.find(tmp);
  if ( label == "2j" or label == "MET2g1b2j" or found != std::string::npos ) 
    countSignalEvent(label);
  else
    countCutflowEvent(label);
    
  return;
}

void Atlas_1807_07447::finalize() {
  // Whatever should be done after the run goes here
}       

bool Atlas_1807_07447::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

std::vector<Jet*> Atlas_1807_07447::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
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

template<class T>
static bool isolateLepton(T* lepton, std::vector<Track*> tracks, double maxPT, double deltaR) {
    double pTsum = -lepton->PT;
    for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
        if ((*it)->P4().DeltaR(lepton->P4()) > deltaR) 
            continue;
        
        if ((*it)->P4().Perp() > 0.5)   // min track pT
	  pTsum += (*it)->PT;
    }
    return pTsum/lepton->PT < maxPT;
}

template<class T>
std::vector<T*> Atlas_1807_07447::isolateLeptons(std::vector<T*> leptons, double deltaR) {
  
    std::vector<T*> isolated;
    for (typename std::vector<T*>::iterator it=leptons.begin(); it!=leptons.end(); it++) {
        double dR = std::min(deltaR, (double)(10/((*it)->PT)));
        if (isolateLepton(*it, tracks, 0.16, dR)) { //using value from atlas-conf-2014-006
            isolated.push_back(*it);
        }
    }
    return isolated;
}

static bool isolatePhoton(Photon* photon, std::vector<Tower*> towers, double maxET, double deltaR) {
  
    double ETsum = -photon->PT;
    for (std::vector<Tower*>::iterator it = towers.begin(); it != towers.end(); it++) {
        if ((*it)->P4().DeltaR(photon->P4()) > deltaR) 
            continue;
        
        if ((*it)->ET > 0.1)  
	  ETsum += (*it)->ET;
    }
    return ETsum < maxET;
}

std::vector<Photon*> Atlas_1807_07447::isolatePhotons(std::vector<Photon*> photons, double deltaR) {
  
    std::vector<Photon*> isolated;
    for (std::vector<Photon*>::iterator it = photons.begin(); it != photons.end(); it++) 
        if ( isolatePhoton(*it, towers, 2.45 + 0.022*((*it)->PT), deltaR) )  
            isolated.push_back(*it);        
    
    return isolated;
}
