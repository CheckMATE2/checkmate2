#include "atlas_1909_08457.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1909_08457::initialize() {
  setAnalysisName("atlas_1909_08457");          
  setInformation(""
    "# search for squarks and gluinos with same-sign leptons\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("Rpc2L0b;Rpc2L1b;Rpc2L2b;Rpc3LSS1b;Rpv2L");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1909_08457::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);  
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47, true);
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.0, 2.0, true);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5);
  
  std::vector<Jet*> bjets_overlap;
  std::vector<Jet*> lightjets_overlap;  
  
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta) < 2.5 && checkBTag(jets[i]) ) bjets_overlap.push_back(jets[i]);
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
  
  met = missingET->P4().Et();  bool trigger = false;
  if (met > 250.) trigger = true;
//  else if (electronsMedium.size() > 1 and electronsMedium[0]->PT > 24. and rand()/(RAND_MAX+1.) < 0.95) trigger = true;
  else if (electronsMedium.size() > 1 and electronsMedium[1]->PT > 18. and rand()/(RAND_MAX+1.) < 0.95) trigger = true;  
//  else if (muonsCombined.size() > 1 and muonsCombined[0]->PT > 22. and rand()/(RAND_MAX+1.) < 0.95) trigger = true;  
  else if (muonsCombined.size() > 1 and muonsCombined[1]->PT > 15. and rand()/(RAND_MAX+1.) < 0.95) trigger = true;  
  else if (muonsCombined.size() > 0 and muonsCombined[0]->PT > 14. and electronsMedium.size() > 0 and electronsMedium[0]->PT > 17. and rand()/(RAND_MAX+1.) < 0.95) trigger = true;  
  
  electronsMedium = filterIsolation(electronsMedium);
  electronsMedium = Isolate_leptons_with_inverse_track_isolation_cone(electronsMedium, tracks, towers, 0.2, 10., 0.2, 0.06, 0.2, false);
  muonsCombined = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined, tracks, towers, 0.3, 10., 0.2, 0.06, 0.2, false);
  
  for (int i = 0; i < electronsMedium.size(); i++ )
    if (rand()/(RAND_MAX+1.) > 0.90) {
      electronsMedium.erase(electronsMedium.begin() + i);
      i--;
    }  
    
  countCutflowEvent("00_all");
  
  if (!trigger) return;
  countCutflowEvent("01_trigger");

  std::vector<Electron*> electrons_hard = filterPhaseSpace(electronsMedium, 20., -2.0, 2.0, true); 
  std::vector<Muon*> muons_hard = filterPhaseSpace(muonsCombined, 20., -2.5, 2.5);
  
  double positive = 0;  double negative = 0;  
  
  for (int i = 0; i < electronsMedium.size(); i++ ) 
    if ( electronsMedium[i]->Charge > 0 ) positive++;
    else negative++;
    
  
  for (int i = 0; i < muonsCombined.size(); i++ ) 
    if ( muonsCombined[i]->Charge > 0 ) positive++;
    else negative++; 
    
  if ( electrons_hard.size() + muons_hard.size() < 2 or (negative < 2 and positive < 2) ) return;
  countCutflowEvent("02_leptons");    
  
  nb = 0;
  sigjets = lightjets_overlap;
  sigjets.insert(sigjets.end(), bjets_overlap.begin(), bjets_overlap.end());
  std::sort(sigjets.begin(), sigjets.end(), Atlas_1909_08457::sortByPT ); 
  for (int i = 0; i < sigjets.size(); i++) 
    if ( fabs(sigjets[i]->Eta) < 2.5 && checkBTag(sigjets[i], 0) ) nb++;  
    
  meff = met;
    
  for (int i = 0; i < electronsMedium.size(); i++)
    meff += electronsMedium[i]->PT;
  for (int i = 0; i < muonsCombined.size(); i++)
    meff += muonsCombined[i]->PT;  
  for (int i = 0; i < sigjets.size(); i++)
    meff += sigjets[i]->PT;    
  
  Passes_Cuts(0, 6, 40.,   0., 2600., 0. , true, "Rpv2L");
  Passes_Cuts(-1, 6, 40., 200., 1000., 0.2, true, "Rpc2L0b");
  Passes_Cuts(1, 6, 40.,   0.,    0., 0.25, true, "Rpc2L1b");
  Passes_Cuts(2, 6, 25., 300., 1400., 0.14, true, "Rpc2L2b");

  if (nb == 0 ) return;
  countCutflowEvent("Rpc3LSS1b_03_bjets");  
  if (negative < 3 and positive < 3) return;
  countCutflowEvent("Rpc3LSS1b_04_3SSleptons");  
  
  bool Zveto = false;
  
  for (int i = 0; i < electronsMedium.size(); i++)
    for (int j = 0; j < electronsMedium.size(); j++)
      if ( (electronsMedium[i]->Charge * electronsMedium[j]->Charge > 0) and fabs( (electronsMedium[i]->P4() + electronsMedium[j]->P4()).M() - 91.) < 10. ) Zveto = true; 
  
  if (Zveto) return;
  countCutflowEvent("Rpc3LSS1b_05_Zveto");    
  if (met/meff < 0.14) return;
  countCutflowEvent("Rpc3LSS1b_06_METMeff");    
  
  countSignalEvent("Rpc3LSS1b");
  return;
}
    
void Atlas_1909_08457::finalize() {
  // Whatever should be done after the run goes here
}       


void Atlas_1909_08457::Passes_Cuts(int NbJet, int NJet, double J_PT, double MET_cut, double Meff_cut, double METMeff_cut, bool cutflow, std::string sr) {
  
  if (NbJet == -1 and nb > 0) return;
  if (NbJet >= 0 and nb < NbJet ) return;
  if (cutflow) countCutflowEvent(sr+"_03_bjets");  
   
  int jet = 0;
  for (int i = 0; i < sigjets.size(); i++)
    if (sigjets[i]->PT > J_PT) jet++;
    
  if (jet < NJet) return;  
  if (cutflow) countCutflowEvent(sr+"_04_jets");  
  
  if (met < MET_cut) return;
  if (cutflow) countCutflowEvent(sr+"_05_MET");  
  
  if (meff < Meff_cut) return; 
  if (cutflow) countCutflowEvent(sr+"_06_Meff");  
  
  if (met/meff < METMeff_cut) return;
  if (cutflow) countCutflowEvent(sr+"_07_METMeff");  

  countSignalEvent(sr);
  return;
  
}

template <class X, class Y>
std::vector<X*> Atlas_1909_08457::specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours) {
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

std::vector<Jet*> Atlas_1909_08457::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
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

bool Atlas_1909_08457::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

bool Atlas_1909_08457::sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }