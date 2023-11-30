#include "atlas_2202_07953.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_2202_07953::initialize() {
  setAnalysisName("atlas_2202_07953");          
  setInformation(""
    "# invisible Higgs decays in VBF from ATLAS-EXOT\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("SR00;Bin01;Bin02;Bin03;Bin04;Bin05;Bin06;Bin07;Bin08;Bin09;Bin10;Bin11;Bin12;Bin13;Bin14;Bin15;Bin16");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_2202_07953::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsLoose = filterPhaseSpace(electronsLoose, 4.5, -2.47, 2.47);
  muonsLoose = filterPhaseSpace(muonsLoose, 4., -2.7, 2.7);
  photonsMedium = filterPhaseSpace(photonsMedium, 20., -2.37, 2.37, true);
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  
  photonsMedium = overlapRemoval(photonsMedium, electronsLoose, 0.4, "y");
  photonsMedium = overlapRemoval(photonsMedium, muonsLoose, 0.4, "y");
  jets = overlapRemoval(jets, electronsLoose, 0.2, "y");
  electronsLoose = specialoverlap(electronsLoose, jets);
  jets = overlapRemoval_muon_jet_tracks(jets, muonsLoose, 0.2, 2); 
  muonsLoose = specialoverlap(muonsLoose, jets);
  jets = overlapRemoval(jets, photonsMedium, 0.4, "y");
  
  std::vector<Jet*> sigjets = filterPhaseSpace(jets, 25., -4.5, 4.5);
  
  countCutflowEvent("00_all");
  
  double met = missingET->P4().Et();
  
  TLorentzVector pTmiss_soft = TLorentzVector(0., 0., 0., 0.);
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
    bool used = false;
    for (int i = 0; i < jets.size() && !used; i++)
      for (int part = 0; part < jets[i]->Particles.GetEntries(); part++)
        if ((*it)->PT > 0.4 && jets[i]->Particles.At(part) == (*it)->Particle) {
	  used = true;
	  break;
	}
    if (!used)  pTmiss_soft -= (*it)->P4();
  }

// uncomment for validation only!
  GenParticle* higgs;
  for(int i = 0; i < true_particles.size(); ++i)   //cout <<  true_particles[i]->PID << "   " << true_particles[i]->PT << "\n";;           
    if( true_particles[i]->PID == 25 and true_particles[true_particles[i]->D1]->PID != 25 ) 
      higgs = true_particles[i];
  
//  if (higgs->PT < 75.) return;  
//  countCutflowEvent("01_pTH>75");  
    
  if (sigjets.size() < 2 or sigjets[0]->PT < 50. or sigjets[1]->PT < 40. or fabs(sigjets[0]->Eta - sigjets[1]->Eta) < 2.) return;  
  if ( fabs(sigjets[0]->Eta) > 2.5 and sigjets[0]->PT < 120.) {
    if (met < 200. and rand()/(RAND_MAX+1.) > 0.7) return;
    if (met > 200. and rand()/(RAND_MAX+1.) > 0.93) return;
  }
  if ( fabs(sigjets[1]->Eta) > 2.5 and sigjets[1]->PT < 120.) {
    if (met < 200. and rand()/(RAND_MAX+1.) > 0.7) return;
    if (met > 200. and rand()/(RAND_MAX+1.) > 0.93) return;
  }  
  countCutflowEvent("02_jets");  
  
  if (met < 110.) return;
  countCutflowEvent("03_METtrigger");
  
  if ( muonsLoose.size() > 0 and muonsLoose[0]->PT > 30.) return;
  if ( electronsLoose.size() > 0 and electronsLoose[0]->PT > 30.) return;
  countCutflowEvent("04_signal_lep_veto");
  
  if (photonsMedium.size() > 0 ) return;
  countCutflowEvent("05_photon_veto");
  
  if (sigjets.size() > 4 ) return;
  countCutflowEvent("06_Njet<5");
  
  if ( sigjets.size() > 2) 
     for (int i = 2; i < sigjets.size(); i++) { 
        double ci = exp(-4./pow(sigjets[0]->Eta - sigjets[1]->Eta, 2) * pow(sigjets[i]->Eta - (sigjets[0]->Eta + sigjets[1]->Eta)/2., 2 ) );
        if (ci > 0.6) return;
     }
  countCutflowEvent("07_Ci<0.6");
  
  double mjj = (sigjets[0]->P4() + sigjets[1]->P4()).M();
  if ( sigjets.size() > 2) 
     for (int i = 2; i < sigjets.size(); i++) { 
        double m1 = (sigjets[0]->P4() + sigjets[i]->P4()).M();
        double m2 = (sigjets[1]->P4() + sigjets[i]->P4()).M();
        if (std::min(m1, m2)/mjj > 0.05) return;
     }
  countCutflowEvent("08_mi<0.05");  
  
  if (jets[0]->PT < 80.) return;
  countCutflowEvent("09_pTj1>80");
  
  if (jets[1]->PT < 50.) return;
  countCutflowEvent("10_pTj2>50");  
  
  int nb = 0;
  for (int i = 0; i < sigjets.size(); i++) 
    if ( fabs(sigjets[i]->Eta) < 2.5 && checkBTag(sigjets[i]) ) nb++;  
  if (nb > 1) return;  
  countCutflowEvent("11_Nb<2");  
  
  if ( muonsLoose.size() > 0 or electronsLoose.size() > 0) return;
  countCutflowEvent("12_cand_lep_veto");  
  
  if (pTmiss_soft.Perp() > 20.) return;
  countCutflowEvent("13_softMET<20");  
  
  if (met < 160.) return;
  countCutflowEvent("14_MET>160");  
  
  TLorentzVector alljets = TLorentzVector(0., 0., 0., 0.);
  for (int i = 0; i < sigjets.size(); i++)
      alljets += sigjets[i]->P4();
  if (alljets.Perp() < 140.) return;
  countCutflowEvent("15_pTjets>140");

  double dphijj = fabs(sigjets[0]->P4().DeltaPhi(sigjets[1]->P4()));
  if ( dphijj > 2.) return;
  countCutflowEvent("16_DPhijj<2");  
  
  if ( sigjets[0]->Eta * sigjets[1]->Eta > 0) return;
  countCutflowEvent("17_eta1*eta2<0");  
  
  if ( fabs(sigjets[0]->Eta - sigjets[1]->Eta) < 3.8 ) return;
  countCutflowEvent("18_Detajj>3.8");

  if (mjj < 800.) return;
  countCutflowEvent("19_mjj>800");  
  
  if (sigjets.size() > 2 and mjj < 1500.) return;
  countCutflowEvent("20_mjj(3,4)>1500");  
  
  if (met < 200. and mjj < 1500.) return;
  countCutflowEvent("21_mjj(met)>1500");  
  
  countSignalEvent("SR00");
  
  if (met < 200. and sigjets.size() == 2 ) {
      if (mjj  <  2000. ) countSignalEvent("Bin14");
      else if (mjj  <  3500. ) countSignalEvent("Bin15");
      else countSignalEvent("Bin16");
  }
  
  if (met > 200. and sigjets.size() > 2 ) {
      if (mjj  <  2000. ) countSignalEvent("Bin11");
      else if (mjj  <  3500. ) countSignalEvent("Bin12");
      else countSignalEvent("Bin13");
  }      
  
  if (met > 200. and sigjets.size() == 2 ) {
      if (mjj < 1000.) {
          if (dphijj < 1.0) countSignalEvent("Bin01");
          else countSignalEvent("Bin06");
      }
      else if (mjj < 1500.) {
          if (dphijj < 1.0) countSignalEvent("Bin02");
          else countSignalEvent("Bin07");
      }      
      else if (mjj < 2000.) {
          if (dphijj < 1.0) countSignalEvent("Bin03");
          else countSignalEvent("Bin08");
      }
      else if (mjj < 3500.) {
          if (dphijj < 1.0) countSignalEvent("Bin04");
          else countSignalEvent("Bin09");
      }
      else {
          if (dphijj < 1.0) countSignalEvent("Bin05");
          else countSignalEvent("Bin10");
      }      
  }
  
  return;
  
}

void Atlas_2202_07953::finalize() {
  // Whatever should be done after the run goes here
}       


template <class X, class Y>
std::vector<X*> Atlas_2202_07953::specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours) {
      // If neighbours are empty, return candidates
      if(neighbours.size() == 0)
        return candidates;
      std::vector<X*> passed_candidates;
      // Loop over candidates
      for(int i = 0; i < candidates.size(); i++) {
        bool overlap = false;
        // If a neighbour is too close, declare overlap, break and don't save candidate
        for(int j = 0; j < neighbours.size(); j++) {
          if (candidates[i]->P4().DeltaR(neighbours[j]->P4()) > 0.2 and candidates[i]->P4().DeltaR(neighbours[j]->P4()) < std::min(0.4, 0.04 + 10./candidates[i]->PT) ) {
            overlap = true;
            break;
          }
        }
        if (!overlap)
          passed_candidates.push_back(candidates[i]);
      }
      return passed_candidates;
    }
    
bool Atlas_2202_07953::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}
    
std::vector<Jet*> Atlas_2202_07953::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
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

std::vector<Photon*> Atlas_2202_07953::isolatePhotons(std::vector<Photon*> photons, double deltaR) {
  
    std::vector<Photon*> isolated;
    for (std::vector<Photon*>::iterator it = photons.begin(); it != photons.end(); it++) 
        if ( isolatePhoton(*it, towers, 2.45 + 0.022*((*it)->PT), deltaR) )  
            isolated.push_back(*it);        
    
    return isolated;
}
