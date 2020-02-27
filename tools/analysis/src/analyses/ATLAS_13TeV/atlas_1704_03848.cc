#include "atlas_1704_03848.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1704_03848::initialize() {
  setAnalysisName("atlas_1704_03848");          
  setInformation(""
    "# monophoton dark matter search\n"
    "# photon + MET channel\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("SRI1;SRI2;SRI3;SRE1;SRE2");
  bookCutflowRegions("00_all;01_trigger;02_MET>150;03_pT>150;04_leadingTight;05_isolated;06_dphimet>0.4;07_MET/sqrt(ET)>8.5;08_0-1jet;09_leptonveto;");

}

void Atlas_1704_03848::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsMedium = filterPhaseSpace(electronsMedium, 7., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 6., -2.7, 2.7);
  std::vector<Photon*> photonsCand = filterPhaseSpace(photonsLoose, 10., -2.37, 2.37);
  std::vector<Photon*> photonsSig = filterPhaseSpace(photonsMedium, 10., -2.37, 2.37, true); // !!! medium is tight 
  std::vector<Jet*> jetsCand = filterPhaseSpace(jets, 20., -4.5, 4.5);

  jetsCand = overlapRemoval(jetsCand, electronsMedium, 0.2);
  electronsMedium = overlapRemoval(electronsMedium, jetsCand, 0.4);  
  muonsCombined = overlapRemoval(muonsCombined, jetsCand, 0.4);  
//  jetsCand = overlapRemoval_muon_jet_tracks(jetsCand, muonsCombined, 0.4, 2);   //perhaps overdoing...
  jetsCand = overlapRemoval(jetsCand, photonsCand, 0.4);
  
  std::vector<Jet*> jetsSig = filterPhaseSpace(jetsCand, 30., -4.5, 4.5);
  
  countCutflowEvent("00_all");
  
  TLorentzVector pTmiss = TLorentzVector(0., 0., 0., 0.);
  double sumET = 0.;
  for (int i = 0; i < photonsCand.size(); i++)  {
    pTmiss -= photonsCand[i]->P4();  
    sumET += photonsCand[i]->PT; 
  }
  for (int i = 0; i < jetsCand.size(); i++)   {
    pTmiss -= jetsCand[i]->P4();
    sumET += jetsCand[i]->PT;
  }
  
  TLorentzVector pTmiss_soft = TLorentzVector(0., 0., 0., 0.);
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
    bool used = false;
    for (int i = 0; i < jets.size() && !used; i++)
      for (int part = 0; part < jets[i]->Particles.GetEntries(); part++)
        if ((*it)->PT > 0.4 && jets[i]->Particles.At(part) == (*it)->Particle) {
	  used = true;
	  break;
	}
    if (!used)  {
      pTmiss_soft -= (*it)->P4();
      sumET += (*it)->PT;
    }
  }  
  
  pTmiss += pTmiss_soft;  
  double met = pTmiss.Perp();
  
  if (photonsCand.size() == 0 || photonsCand[0]->PT < 140. || met < 70. ) return;
  countCutflowEvent("01_trigger");
  
  if (met < 150.0) return;
  countCutflowEvent("02_MET>150");

  if (photonsCand[0]->PT < 150. ) return;
  countCutflowEvent("03_pT>150");
  
  if (photonsSig.size() == 0 || photonsSig[0]->PT != photonsCand[0]->PT ) return;
  countCutflowEvent("04_leadingTight");
  
  photonsSig = Atlas_1704_03848::isolation(photonsSig);
  photonsSig = filterIsolation(photonsSig);  
  
  if (photonsSig.size() == 0 || photonsSig[0]->PT != photonsCand[0]->PT ) return;
  countCutflowEvent("05_isolated");  
  
  if (fabs(pTmiss.DeltaPhi(photonsSig[0]->P4())) < 0.4) return;  
  countCutflowEvent("06_dphimet>0.4");
  
  if ( met/sqrt(sumET) < 8.5 ) return;
  countCutflowEvent("07_MET/sqrt(ET)>8.5");
  
  if (jetsSig.size() > 1) return;
  if (jetsSig.size() == 1 && fabs(pTmiss.DeltaPhi(jetsSig[0]->P4())) < 0.4) return;
  countCutflowEvent("08_0-1jet");
  
  if (electronsMedium.size() + muonsCombined.size() > 0 ) return; 
  countCutflowEvent("09_leptonveto");
  
  countSignalEvent("SRI1");  
  if ( met > 225. ) countSignalEvent("SRI2"); else countSignalEvent("SRE1");  
  if ( met > 300. ) countSignalEvent("SRI3");
  else if ( met > 225. ) countSignalEvent("SRE2");    
  
}

void Atlas_1704_03848::finalize() {
  // Whatever should be done after the run goes here
}       

std::vector<Photon*> Atlas_1704_03848::isolation(std::vector<Photon*> candidates) {
  
  std::vector<Photon*> passed_candidates;
  
  for(int i = 0; i < candidates.size(); i++) {
    double ET = -candidates[i]->PT;
    for(int j = 0; j < towers.size(); j++) 
      if ( candidates[i]->P4().DeltaR(towers[j]->P4()) < 0.4 )
	ET += towers[j]->ET;
    if ( ET < 2.45 + 0.022*candidates[i]->PT )
      passed_candidates.push_back(candidates[i]);
  }
  
  return passed_candidates;
  
}


/*bool Atlas_1704_03848::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

std::vector<Jet*> Atlas_1704_03848::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
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
}*/
