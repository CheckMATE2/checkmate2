#include "atlas_1712_02332.h"
#include "fastjet/tools/Filter.hh"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1712_02332::initialize() {
  setAnalysisName("atlas_1712_02332");          
  setInformation(""
    "# search for squarks and gluinos\n"
    "# 0 leptons, 2-6 jets, MET\n"
    "# only Meff, no jigsaw\n"
    "# Moriond 2017, 40/fb\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("2j-1200;2j-1600;2j-2000;2j-2400;2j-2800;2j-3600;2j-B1600;2j-B2400;2j-2100;3j-1300;4j-1000;4j-1400;4j-1800;4j-2200;4j-2600;4j-3000;5j-1600;5j-1700;5j-2000;5j-2600;6j-1200;6j-1800;6j-2200;6j-2600");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1712_02332::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsLoose = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 7., -2.7, 2.7);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);

  std::vector<Jet*> bjets;
  std::vector<Jet*> sigjets;
  
  for (int i = 0; i < jets.size(); i++) 
    if ( jets[i]->PT > 50. && fabs(jets[i]->Eta) < 2.5 && checkBTag(jets[i]) ) bjets.push_back(jets[i]);
    else sigjets.push_back(jets[i]);
  
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.2, "y");  
  sigjets = overlapRemoval(sigjets, electronsLoose, 0.2, "y");
  electronsLoose  = overlapRemoval(electronsLoose, sigjets, 0.4, "y");
  electronsLoose  = overlapRemoval(electronsLoose, bjets, 0.4, "y");
  
  muonsCombined   = specialoverlap(muonsCombined, sigjets);
  muonsCombined   = specialoverlap(muonsCombined, bjets);
  
  sigjets = overlapRemoval_muon_jet_tracks(sigjets, muonsCombined, 0.2, 2); 
  bjets = overlapRemoval_muon_jet_tracks(bjets, muonsCombined, 0.2, 2); 
  
//  sigjets = overlapRemoval_muon_jet_tracks2(sigjets, muonsCombined, 0.2); 
//  bjets = overlapRemoval_muon_jet_tracks2(bjets, muonsCombined, 0.2); 
  
  countCutflowEvent("00_all");
  
  double met = missingET->P4().Et();
  
  if (met < 100.) return;
  countCutflowEvent("01_trigger");
  
  if(electronsLoose.size() > 0) return;
  if(muonsCombined.size() > 0) return;  

  countCutflowEvent("02_lepton_veto");
  
  sigjets = filterPhaseSpace(sigjets, 50., -2.8, 2.8);
  sigjets.insert(sigjets.end(), bjets.begin(), bjets.end());
  std::sort(sigjets.begin(), sigjets.end(), Atlas_1712_02332::sortByPT ); 

  if (met < 250.) return;
  countCutflowEvent("03_met>250");

  if ( M_eff(jets, 0) < 800. ) return;
  if ( jets.size() < 1 || jets[0]->PT < 200.) return;
  countCutflowEvent("04_meff>800");
  
  
  if (sigjets.size() < 2) return;
  
  countCutflowEvent("05_2j_pt>50");
 
//                        PTj1  PTj2  Nj  Eta Phi1 Phi2 ET/HT Apl ET/meff Meff
  if (Passes_Cuts(sigjets, 250., 250., 2, 0.8, 0.8, 0.4, 14., 0.,   0.,   1200., true, "2j-1200") ) countSignalEvent("2j-1200");
  if (Passes_Cuts(sigjets, 300., 300., 2, 1.2, 0.8, 0.4, 18., 0.,   0.,   1600., true, "2j-1600") ) countSignalEvent("2j-1600");
  if (Passes_Cuts(sigjets, 350., 350., 2, 1.2, 0.8, 0.4, 18., 0.,   0.,   2000., true, "2j-2000") ) countSignalEvent("2j-2000");
  if (Passes_Cuts(sigjets, 350., 350., 2, 1.2, 0.8, 0.4, 18., 0.,   0.,   2400., true, "2j-2400") ) countSignalEvent("2j-2400");
  if (Passes_Cuts(sigjets, 350., 350., 2, 1.2, 0.8, 0.4, 18., 0.,   0.,   2800., true, "2j-2800") ) countSignalEvent("2j-2800");  
  if (Passes_Cuts(sigjets, 350., 350., 2, 5.0, 0.8, 0.4, 18., 0.,   0.,   3600., true, "2j-3600") ) countSignalEvent("2j-3600");
  if (Passes_Cuts(sigjets, 600., 50.,  2, 5.0, 0.4, 0.2, 26., 0.,   0.,   2100., true, "2j-2100") ) countSignalEvent("2j-2100");  
  if (Passes_Cuts(sigjets, 700., 50.,  3, 5.0, 0.4, 0.2, 16., 0.,   0.,   1300., true, "3j-1300") ) countSignalEvent("3j-1300");
  if (Passes_Cuts(sigjets, 200., 100., 4, 1.2, 0.4, 0.4, 0.,  0.04, 0.3,  1000., true, "4j-1000") ) countSignalEvent("4j-1000");
  if (Passes_Cuts(sigjets, 200., 100., 4, 2.0, 0.4, 0.4, 0.,  0.04, 0.25, 1400., true, "4j-1400") ) countSignalEvent("4j-1400");
  if (Passes_Cuts(sigjets, 200., 100., 4, 2.0, 0.4, 0.4, 0.,  0.04, 0.25, 1800., true, "4j-1800") ) countSignalEvent("4j-1800");
  if (Passes_Cuts(sigjets, 200., 100., 4, 2.0, 0.4, 0.4, 0.,  0.04, 0.25, 2200., true, "4j-2200") ) countSignalEvent("4j-2200");
  if (Passes_Cuts(sigjets, 200., 150., 4, 2.0, 0.4, 0.4, 0.,  0.04, 0.2,  2600., true, "4j-2600") ) countSignalEvent("4j-2600");
  if (Passes_Cuts(sigjets, 200., 150., 4, 2.0, 0.4, 0.4, 0.,  0.04, 0.2,  3000., true, "4j-3000") ) countSignalEvent("4j-3000");  
  if (Passes_Cuts(sigjets, 700., 50.,  5, 5.0, 0.4, 0.2, 0.,  0.,   0.3,  1700., true, "5j-1700") ) countSignalEvent("5j-1700");
  if (Passes_Cuts(sigjets, 200., 50.,  5, 5.0, 0.4, 0.2, 0.,  0.08, 0.15, 1600., true, "5j-1600") ) countSignalEvent("5j-1600");  
  if (Passes_Cuts(sigjets, 200., 50.,  5, 5.0, 0.4, 0.4, 15,  0.,   0.,   2000., true, "5j-2000") ) countSignalEvent("5j-2000");
  if (Passes_Cuts(sigjets, 200., 50.,  5, 5.0, 0.8, 0.4, 18,  0.,   0.,   2600., true, "5j-2600") ) countSignalEvent("5j-2600");
  if (Passes_Cuts(sigjets, 200., 50.,  6, 2.0, 0.4, 0.2, 0.,  0.,   0.25, 1200., true, "6j-1200") ) countSignalEvent("6j-1200");  
  if (Passes_Cuts(sigjets, 200., 100., 6, 2.0, 0.4, 0.2, 0.,  0.04, 0.2,  1800., true, "6j-1800") ) countSignalEvent("6j-1800");
  if (Passes_Cuts(sigjets, 200., 100., 6, 5.0, 0.4, 0.2, 0.,  0.08, 0.2,  2200., true, "6j-2200") ) countSignalEvent("6j-2200");
  if (Passes_Cuts(sigjets, 200., 100., 6, 5.0, 0.4, 0.2, 0.,  0.08, 0.15, 2600., true, "6j-2600") ) countSignalEvent("6j-2600");  
  
// boosted channels separately
  std::vector<fastjet::PseudoJet> particles;
  for ( int i = 0; i < sigjets.size() && sigjets[i]->PT > 25. ; i++ ) 
    particles.push_back( fastjet::PseudoJet(sigjets[i]->P4().Px(), sigjets[i]->P4().Py(), sigjets[i]->P4().Pz(), sigjets[i]->P4().E()) );


  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, 1.0);
 
  fastjet::ClusterSequence cs(particles, jet_def);
  std::vector<fastjet::PseudoJet> largeRJets = sorted_by_pt(cs.inclusive_jets());
  std::vector<fastjet::PseudoJet> largeRJets_trimmed;
 
  double Rfilt = 0.4;
  fastjet::Selector selector_trimmer = fastjet::SelectorPtFractionMin(0.05);
  fastjet::Filter trimmer(Rfilt, selector_trimmer);
  for(int i=0; i<largeRJets.size(); i++) {
    fastjet::PseudoJet trimmed_jet = trimmer(largeRJets[i]);
    largeRJets_trimmed.push_back(trimmed_jet);
  }    
  
  if ( largeRJets_trimmed.size() < 2 or largeRJets_trimmed[1].pt() < 200. ) return;
  countCutflowEvent("2j-B0000_00_2LRjets");
  if ( fabs(largeRJets_trimmed[0].m() - 85.) > 25. or fabs(largeRJets_trimmed[1].m() - 85.) > 25. ) return; 
  countCutflowEvent("2j-B0000_01_LRjets_mass");
  
  if (Passes_Cuts(sigjets, 25., 25., 2, 5.0, 0.6, 0.4, 20., 0.,   0.,   1600., true, "2j-B1600") ) countSignalEvent("2j-B1600");
  if (Passes_Cuts(sigjets, 25., 25., 2, 5.0, 0.6, 0.4, 20., 0.,   0.,   2400., true, "2j-B2400") ) countSignalEvent("2j-B2400");
  
  return;  
  
}

void Atlas_1712_02332::finalize() {
  // Whatever should be done after the run goes here
}       

bool Atlas_1712_02332::sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }

double Atlas_1712_02332::Aplanarity(std::vector<Jet*> input_jets) {
  
  double mag = 0.;
  for (int k = 0; k < input_jets.size(); k++)
    mag += pow(input_jets[k]->P4().Rho(),2);
  
  TMatrixD st(TMatrixD::kZero, TMatrixD(3,3) );
  for (int k = 0; k < input_jets.size(); k++) {
    st(0,0) += input_jets[k]->P4().X()*input_jets[k]->P4().X()/mag;
    st(0,1) += input_jets[k]->P4().X()*input_jets[k]->P4().Y()/mag;
    st(0,2) += input_jets[k]->P4().X()*input_jets[k]->P4().Z()/mag;
    st(1,1) += input_jets[k]->P4().Y()*input_jets[k]->P4().Y()/mag;
    st(1,2) += input_jets[k]->P4().Y()*input_jets[k]->P4().Z()/mag;
    st(2,2) += input_jets[k]->P4().Z()*input_jets[k]->P4().Z()/mag;
  }
  st(1,0) = st(0,1);
  st(2,0) = st(0,2);
  st(2,1) = st(1,2);
  
  TMatrixDEigen eigen(st);
  TMatrixD diag = eigen.GetEigenValues();
  
  std::vector<double> lambdas;
  lambdas.push_back( diag(0,0) );
  lambdas.push_back( diag(1,1) );
  lambdas.push_back( diag(2,2) );
  std::sort (lambdas.begin(), lambdas.end());
  
  return 1.5*lambdas[0];
}  

bool Atlas_1712_02332::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

double Atlas_1712_02332::check_track_pt(Jet* jet, std::vector<Track*> tracks) {
  
  double track_pt = 0.;
  
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
       if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) track_pt += (*it)->PT;
       
  return track_pt;
    
}

std::vector<Jet*> Atlas_1712_02332::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
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

std::vector<Jet*> Atlas_1712_02332::overlapRemoval_muon_jet_tracks2(std::vector<Jet*> cand_jets,std::vector<Muon*> cand_muons, double deltaR){
  
  std::vector<Jet*> passed;
  for (std::vector<Jet*>::iterator jet = cand_jets.begin(); jet != cand_jets.end(); jet++) {
    
    double dR = deltaR;
    bool iso = true;
        
    for (std::vector<Muon*>::iterator mu=cand_muons.begin(); mu!=cand_muons.end(); mu++) 
       if ((*jet)->P4().DeltaR((*mu)->P4()) < dR && 0.7*check_track_pt(*jet, tracks) < (*mu)->PT && (*jet)->PT < 0.5*(*mu)->PT ) {
	 iso = false;
	 break;
       }
       
     if (iso) passed.push_back(*jet);
  }
  
  return passed;
}

template <class X, class Y>
std::vector<X*> Atlas_1712_02332::specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours) {
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
    
double Atlas_1712_02332::dPhi(std::vector<Jet*> jets, int j) {

  int kmax = 0; int kmin = 0;
  if ( !j ) { 
    kmax = std::min(int(jets.size()), 3); 
    kmin = 0;     
  }
  else { 
    kmax = jets.size(); 
    kmin = 3;    
  }
  double dphimin = 10.;

  for (int k = kmin; k < kmax; k++)
      dphimin = std::min(fabs(jets[k]->P4().DeltaPhi( missingET->P4() )), dphimin);
     
  return dphimin;
}    
    
double Atlas_1712_02332::M_eff(std::vector<Jet*> jets, int j) {
  
  double Meff = missingET->P4().Et();
  int kmax = 0;
  if( !j ) kmax = jets.size(); else kmax=j; 
  
  for(int i = 0; i < kmax; i++) 
    if(jets[i]->PT > 50)  Meff+=jets[i]->PT;
  
  return Meff;
}    

double Atlas_1712_02332::HT(std::vector<Jet*> jets) {
  
  double  PTSum = 0.;
  for (int i = 0; i < jets.size(); i++) PTSum+=jets[i]->PT;
  return PTSum;
}
    
bool Atlas_1712_02332::Passes_Cuts(std::vector<Jet*> jets, double PT1Cut, double PT2Cut, int NJet, double EtaCut, double dPhiCut1, double dPhiCut2, double METHTCut, double AplanarityCut, double METMeffCut, double MeffCut, bool cutflow, std::string sr) {
  
  if ( jets.size() < NJet ) return false;
  if (cutflow) countCutflowEvent(sr+"_06_jetmulti");  
  
  if( dPhi(jets, 0) < dPhiCut1 ) return false;
  if (cutflow) countCutflowEvent(sr+"_07_dPhilow");  
  
  if( dPhi(jets, 1) < dPhiCut2 ) return false;
  if (cutflow) countCutflowEvent(sr+"_08_dPhihigh");    
  
  if ( jets[0]->PT < PT1Cut || jets[NJet-1]->PT < PT2Cut ) return false;
  
  if (cutflow) countCutflowEvent(sr+"_09_PTjets");
  
  for (int i = 0; i < NJet; i++)
    if ( fabs(jets[i]->Eta) > EtaCut ) return false;
    
  if (cutflow) countCutflowEvent(sr+"_10_Etajets");  
  
//  if ( !Aplanarity_Cut(jets, AplanarityCut) ) return false;
  if ( AplanarityCut > 0. and  aplanarity(jets) < AplanarityCut ) return false;
  if (cutflow) countCutflowEvent(sr+"_11_aplanarity");  
  
  if( missingET->P4().Et()/M_eff(jets, NJet) < METMeffCut) return false;
  if( missingET->P4().Et()/sqrt( HT(jets) ) < METHTCut ) return false;
  
  if (cutflow) countCutflowEvent(sr+"_12_METrelative");  
  
  if( M_eff(jets, 0) < MeffCut) return false;
  if (cutflow) countCutflowEvent(sr+"_13_Meff");  
  
  return true;
}