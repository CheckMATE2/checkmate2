#include "atlas_1706_03731.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1706_03731::initialize() {
  setAnalysisName("atlas_1706_03731");          
  setInformation(""
    "# same-sign or 3 leptons RPC and RPV SUSY\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("Rpc2L2bS;Rpc2L2bH;Rpc2Lsoft1b;Rpc2Lsoft2b;Rpc2L0bS;Rpc2L0bH;Rpc3L0bS;Rpc3L0bH;Rpc3L1bS;Rpc3L1bH;Rpc2L1bS;Rpc2L1bH;Rpc3LSS1b;Rpv2L1bH;Rpv2L0b;Rpv2L2bH;Rpv2L2bS;Rpv2L1bS;Rpv2L1bM");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
    met = 0;
    meff = 0;
}

void Atlas_1706_03731::analyze() { 
  
  missingET->addMuons(muonsCombined);  
  
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);  
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47, true);
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.0, 2.0, true);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5);
  
  std::vector<Jet*> bjets_overlap;
  std::vector<Jet*> lightjets_overlap;  
  
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta) < 2.5 && checkBTag(jets[i], 1) ) bjets_overlap.push_back(jets[i]);
    else lightjets_overlap.push_back(jets[i]);
    
  lightjets_overlap = overlapRemoval(lightjets_overlap, electronsLoose, 0.2, "y");   
  lightjets_overlap = overlapRemoval(lightjets_overlap, muonsCombined, 0.2, "y");   
  electronsMedium  = overlapRemoval(electronsMedium, bjets_overlap, 0.2, "y");
  muonsCombined = overlapRemoval(muonsCombined, bjets_overlap, 0.2, "y");
  
  lightjets_overlap = overlapRemoval_muon_jet_tracks(lightjets_overlap, muonsCombined, 0.4, 2); 
  bjets_overlap = overlapRemoval_muon_jet_tracks(bjets_overlap, muonsCombined, 0.4, 2);   
  
  muonsCombined = specialoverlap(muonsCombined, lightjets_overlap);
  muonsCombined = specialoverlap(muonsCombined, bjets_overlap);  
  electronsMedium  = specialoverlap(electronsMedium, lightjets_overlap);
  electronsMedium  = specialoverlap(electronsMedium, bjets_overlap);
  
  electronsMedium = filterIsolation(electronsMedium);
  electronsMedium = Isolate_leptons_with_inverse_track_isolation_cone(electronsMedium, tracks, towers, 0.2, 10., 0.2, 0.06, 0.2, false);
  muonsCombined = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined, tracks, towers, 0.3, 10., 0.2, 0.06, 0.2, false);
  
  for (int i = 0; i < electronsMedium.size(); i++ )
    if (rand()/(RAND_MAX+1.) > 0.97) {
      electronsMedium.erase(electronsMedium.begin() + i);
      i--;
    }
  
  countCutflowEvent("00_all");
// trigger ???  
    
  if (electronsMedium.size() + muonsCombined.size() < 2) return;
  
  double positive = 0;  double positive_soft = 0;  double negative = 0;  double negative_soft = 0;
  
  for (int i = 0; i < electronsMedium.size(); i++ ) {
    if ( electronsMedium[i]->Charge > 0 ) {
      positive_soft++;
      if ( electronsMedium[i]->PT > 20.) positive++;
    }
    else {
      negative_soft++;
      if ( electronsMedium[i]->PT > 20.) negative++;
    }
  }
  
  for (int i = 0; i < muonsCombined.size(); i++ ) {
    if ( muonsCombined[i]->Charge > 0 ) {
      positive_soft++;
      if ( muonsCombined[i]->PT > 20.) positive++;
    }
    else {
      negative_soft++;
      if ( muonsCombined[i]->PT > 20.) negative++;
    }
  }  
  
  nb = 0;
  sigjets = lightjets_overlap;
  sigjets.insert(sigjets.end(), bjets_overlap.begin(), bjets_overlap.end());
  std::sort(sigjets.begin(), sigjets.end(), Atlas_1706_03731::sortByPT ); 
  for (int i = 0; i < sigjets.size(); i++) 
    if ( fabs(sigjets[i]->Eta) < 2.5 && checkBTag(sigjets[i], 0) ) nb++;
  
  met = missingET->P4().Et();  
  meff = met;
    
  for (int i = 0; i < electronsMedium.size(); i++)
    meff += electronsMedium[i]->PT;
  for (int i = 0; i < muonsCombined.size(); i++)
    meff += muonsCombined[i]->PT;  
  for (int i = 0; i < sigjets.size(); i++)
    meff += sigjets[i]->PT;
 
 
  if (positive > 1 or negative > 1) {
    countCutflowEvent("Rpc2L2bS_01_leptons"); countCutflowEvent("Rpc2L2bH_01_leptons"); countCutflowEvent("Rpc2L0bS_01_leptons"); countCutflowEvent("Rpc2L0bH_01_leptons"); countCutflowEvent("Rpc2L1bS_01_leptons"); countCutflowEvent("Rpc2L1bH_01_leptons"); countCutflowEvent("Rpv2L1bH_01_leptons");
    Passes_Cuts(2, 6, 25., 200.,  600., 0.25, true, "Rpc2L2bS");
    Passes_Cuts(2, 6, 25.,   0., 1800., 0.15, true, "Rpc2L2bH");
    Passes_Cuts(0, 6, 25., 150.,    0., 0.25, true, "Rpc2L0bS");
    Passes_Cuts(0, 6, 40., 250.,  900., 0.00, true, "Rpc2L0bH");
    Passes_Cuts(1, 6, 25., 150.,  600., 0.25, true, "Rpc2L1bS");
    Passes_Cuts(1, 6, 25., 250.,    0., 0.20, true, "Rpc2L1bH");
    Passes_Cuts(1, 6, 50.,   0., 2200., 0.00, true, "Rpv2L1bH"); 
  }
  
  if ( (positive + negative > 1) and (positive_soft + negative_soft > 2) ) {
    countCutflowEvent("Rpc3L0bS_01_leptons"); countCutflowEvent("Rpc3L0bH_01_leptons"); countCutflowEvent("Rpc3L1bS_01_leptons"); countCutflowEvent("Rpc3L1bH_01_leptons");
    Passes_Cuts(0, 4, 40., 200.,  600., 0., true, "Rpc3L0bS");
    Passes_Cuts(0, 4, 40., 200., 1600., 0., true, "Rpc3L0bH");
    Passes_Cuts(1, 4, 40., 200.,  600., 0., true, "Rpc3L1bS");
    Passes_Cuts(1, 4, 40., 200., 1600., 0., true, "Rpc3L1bH");    
  }
  
  if ( (positive > 0 and positive_soft > 1) or ( negative > 0 and negative_soft > 1) )
    if ( (electronsMedium.size() == 0 or electronsMedium[0]->PT < 100.) and (muonsCombined.size() == 0 or muonsCombined[0]->PT < 100.) ) {
      countCutflowEvent("Rpc2Lsoft1b_01_leptons"); countCutflowEvent("Rpc2Lsoft2b_01_leptons");
      Passes_Cuts(1, 6, 25., 100.,    0., 0.30, true, "Rpc2Lsoft1b");
      Passes_Cuts(2, 6, 25., 200.,  600., 0.25, true, "Rpc2Lsoft2b"); 
    }

  if ( negative > 1) { 
    countCutflowEvent("Rpv2L2bS_01_leptons"); countCutflowEvent("Rpv2L1bS_01_leptons"); countCutflowEvent("Rpv2L1bM_01_leptons");
    Passes_Cuts(2, 3, 50.,   0., 1200., 0., true, "Rpv2L2bS");
    Passes_Cuts(1, 4, 50.,   0., 1200., 0., true, "Rpv2L1bS");
    Passes_Cuts(1, 4, 50.,   0., 1800., 0., true, "Rpv2L1bM"); 
  }
       
  bool Zveto = true;  
  for (int i = 0; i + 1 < electronsMedium.size(); i++) 
    for ( int j = i+1 ; j < electronsMedium.size(); j++) 
      if ( electronsMedium[i]->Charge * electronsMedium[j]->Charge > 0 and fabs( (electronsMedium[i]->P4() + electronsMedium[j]->P4()).M() - 91. ) < 10. ) Zveto = false; 
 
  if ( ((positive > 1 and positive_soft > 2) or (negative > 1 and negative_soft > 2)) and Zveto ) { 
    countCutflowEvent("Rpc3LSS1b_01_leptons");
    if (nb > 0) {
      countCutflowEvent("Rpc3LSS1b_02_1bjet");
      countSignalEvent("Rpc3LSS1b"); 
    }
  }
      
  if ( (positive == 2 xor negative == 2) and Zveto) {
    countCutflowEvent("Rpv2L0b_01_leptons");
    Passes_Cuts(0, 6, 40.,   0., 1800., 0., true, "Rpv2L0b"); 
  }
  
  if ( (positive > 1 or negative > 1) and Zveto) {
    countCutflowEvent("Rpv2L2bH_01_leptons");
    Passes_Cuts(2, 6, 40.,   0., 2000., 0., true, "Rpv2L2bH"); 
  }
  
  return;  
}

void Atlas_1706_03731::finalize() {
  // Whatever should be done after the run goes here
}       


void Atlas_1706_03731::Passes_Cuts(int NbJet, int NJet, double J_PT, double MET_cut, double Meff_cut, double METMeff_cut, bool cutflow, std::string sr) {
  
  if (NbJet == 0 and nb > 0) return;
  if (NbJet > 0 and nb < NbJet ) return;
  if (cutflow) countCutflowEvent(sr+"_02_bjets");  
   
  int jet = 0;
  for (int i = 0; i < sigjets.size(); i++)
    if (sigjets[i]->PT > J_PT) jet++;
    
  if (jet < NJet) return;  
  if (cutflow) countCutflowEvent(sr+"_03_jets");  
  
  if (met < MET_cut) return;
  if (cutflow) countCutflowEvent(sr+"_04_MET");  
  
  if (meff < Meff_cut) return; 
  if (cutflow) countCutflowEvent(sr+"_05_Meff");  
  
  if (met/meff < METMeff_cut) return;
  if (cutflow) countCutflowEvent(sr+"_06_METMeff");  

  countSignalEvent(sr);
  return;
  
}


template <class X, class Y>
std::vector<X*> Atlas_1706_03731::specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours) {
      // If neighbours are empty, return candidates
      if(neighbours.size() == 0)
        return candidates;
      std::vector<X*> passed_candidates;
      // Loop over candidates
      for(int i = 0; i < candidates.size(); i++) {
        bool overlap = false;
        // If a neighbour is too close, declare overlap, break and don't save candidate
        for(int j = 0; j < neighbours.size(); j++) {
          if (candidates[i]->P4().DeltaR(neighbours[j]->P4()) > 0.2 and candidates[i]->P4().DeltaR(neighbours[j]->P4()) < std::min(0.4, 0.1 + 9.6/candidates[i]->PT) ) {
            overlap = true;
            break;
          }
        }
        if (!overlap)
          passed_candidates.push_back(candidates[i]);
      }
      return passed_candidates;
    }

std::vector<Jet*> Atlas_1706_03731::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
  std::vector<Jet*> passed;
  for (std::vector<Jet*>::iterator jet = cand_jets.begin(); jet != cand_jets.end(); jet++) {
  
    if (check_nTrack_jet(*jet, tracks, nTracks)) {
      passed.push_back(*jet);
      continue;
    }
    double dR = deltaR;
    bool iso = true;
        
    for (std::vector<Muon*>::iterator mu = cand_muons.begin(); mu != cand_muons.end(); mu++) 
      if ((*jet)->P4().DeltaR((*mu)->P4()) < std::min(dR, 0.1 + 9.6/(*mu)->PT) ){
	iso = false;
	break;
      }
        		    
    if (iso) passed.push_back(*jet);
  }
  
  return passed;
}    

bool Atlas_1706_03731::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

bool Atlas_1706_03731::sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }