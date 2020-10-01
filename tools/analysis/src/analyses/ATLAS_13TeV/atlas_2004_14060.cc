#include "atlas_2004_14060.h"

// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_2004_14060::initialize() {
  setAnalysisName("atlas_2004_14060");          
  setInformation(""
    "# stops, leptoquarks, 0 lepton\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("SRA-TT;SRA-TW;SRA-T0;SRB-TT;SRB-TW;SRB-T0;SRD0;SRD1;SRD2");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_2004_14060::analyze() {

  missingET->addMuons(muonsCombined); 
  
  electronsLoose = filterPhaseSpace(electronsLoose, 4.5, -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 4., -2.7, 2.7);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);  
  
  std::vector<Jet*> bjets;  
  std::vector<Jet*> bjets2;  
  std::vector<Jet*> nonbjets;
  std::vector<Jet*> nonbjets2;
  for (int i = 0; i < jets.size(); i++) {
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i],0) ) 
      bjets.push_back(jets[i]);
    else nonbjets.push_back(jets[i]);  
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i],1) ) 
      bjets2.push_back(jets[i]);
    else nonbjets2.push_back(jets[i]); 
  }
    
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.2, "y");
  jets = overlapRemoval(jets, electronsLoose, 0.2, "y");    
  nonbjets = overlapRemoval(nonbjets, electronsLoose, 0.2, "y");  
  electronsLoose = overlapRemoval(electronsLoose, jets, 0.4, "y");   
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4, "y");    
  
  countCutflowEvent("00_all");  
  
  TLorentzVector track_MET;
  for (int i = 0; i < tracks.size(); i++) 
     if ( tracks[i]->PT > 0.5 and fabs(tracks[i]->Eta) < 2.5 ) track_MET -= tracks[i]->P4();   

  pTmiss = TLorentzVector(0., 0., 0., 0.);
  resolution = 0.;
  for (int i = 0; i < jets.size(); i++) 
    pTmiss -= jets[i]->P4();     
     
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
    softtracks.push_back(*it);
  }
  
  pTmiss += pTmiss_soft;
  
  //for (int i = 0; i < jets.size(); i++)
  //   resolution += pow( 0.77*pow(jets[i]->PT, -0.39) * jets[i]->PT * cos(jets[i]->P4().DeltaPhi(pTmiss)), 2)*0.6; 
   //rough relative jet resolution fit from 2007.02645 fig. 29 : 0.77 * PT^(-0.39)  
     
  ht = 0.;   
  for (int i = 0; i < jets.size(); i++)   
    ht += jets[i]->PT;
  if (ht < 150. ) return;
  countCutflowEvent("01_deriv_skim");   
  
  // COSMIC MUONS, BAD JETS, ETC.
//  if (  rand()/(RAND_MAX+1.) > 0.975 ) return;
  
  //met = missingET->P4().Et();
  met = pTmiss.Perp();
  if (met < 250.) return;
  countCutflowEvent("02_MET>250");  

  std::vector<fastjet::PseudoJet> particles;
  for ( int i = 0; i < jets.size(); i++ ) 
    particles.push_back( fastjet::PseudoJet(jets[i]->P4().Px(), jets[i]->P4().Py(), jets[i]->P4().Pz(), jets[i]->P4().E()) );

  fastjet::JetDefinition jet_def12(fastjet::antikt_algorithm, 1.2);
  fastjet::JetDefinition jet_def08(fastjet::antikt_algorithm, 0.8);  
 
  fastjet::ClusterSequence cs12(particles, jet_def12);
  fastjet::ClusterSequence cs08(particles, jet_def08);
  std::vector<fastjet::PseudoJet> largeR12 = sorted_by_pt(cs12.inclusive_jets());  
  std::vector<fastjet::PseudoJet> largeR08 = sorted_by_pt(cs08.inclusive_jets());  

  if ( SRA( jets, bjets, nonbjets, largeR12, largeR08, 120., 10000., 1.0, "SRA-TT") ) countSignalEvent("SRA-TT");
  if ( SRA( jets, bjets, nonbjets, largeR12, largeR08,  60.,   120.,-1.0, "SRA-TW") ) countSignalEvent("SRA-TW");
  if ( SRA( jets, bjets, nonbjets, largeR12, largeR08,   0.,    60.,-1.0, "SRA-T0") ) countSignalEvent("SRA-T0");
  if ( SRB( jets, bjets, nonbjets, largeR12, 120., 10000., "SRB-TT") ) countSignalEvent("SRB-TT");
  if ( SRB( jets, bjets, nonbjets, largeR12,  60.,   120., "SRB-TW") ) countSignalEvent("SRB-TW");
  if ( SRB( jets, bjets, nonbjets, largeR12,   0.,    60., "SRB-T0") ) countSignalEvent("SRB-T0");  
  if ( SRD( nonbjets, "SRD" ) ) {
     
     btrackjets.clear(); 
     trackjets.clear();
     trackjets = findtrackjets(nonbjets2[0]);
     
     for (int i = 0; i < trackjets.size() and trackjets[i].Perp() > 5.; i++)
         if (trackbtag(trackjets[i])) btrackjets.push_back(trackjets[i]);
     //cout << trackjets.size() << "    " << btrackjets.size() <<  '\n';    
      
     if (SRD0( jets, bjets2, nonbjets2, "SRD0" )) countSignalEvent("SRD0");    
     if (SRD1( bjets2, nonbjets2, "SRD1" )) countSignalEvent("SRD1");  
     if (SRD2( bjets2, nonbjets2, "SRD2" )) countSignalEvent("SRD2"); 
  }
   
  //cout << "End event" << '\n';
  return;
}

void Atlas_2004_14060::finalize() {
  // Whatever should be done after the run goes here
}       




bool Atlas_2004_14060::SRA( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::vector<fastjet::PseudoJet> largeR12, std::vector<fastjet::PseudoJet> largeR08, double lR1_cutlow, double lR1_cuthigh, double deltaRbb, std::string cf) {
    
  if (jets.size() < 4) return false;
  if  (cf != "") countCutflowEvent(cf+"_03_4jets");    
  
  if (bjets.size() < 2) return false;
  if  (cf != "") countCutflowEvent(cf+"_04_2bjets");    
  
  if (electronsLoose.size() + muonsCombined.size() != 0) return false;
  if  (cf != "") countCutflowEvent(cf+"_05_lepton_veto");     

  if (jets[3]->PT < 40.) return false;
  if  (cf != "") countCutflowEvent(cf+"_06_jet4>40");  
  
  if (jets[1]->PT < 80.) return false;
  if  (cf != "") countCutflowEvent(cf+"_07_jet2>80");      
  
  for (int i = 0; i < 4; i++)
    if ( fabs(jets[i]->P4().DeltaPhi(pTmiss)) < 0.4 ) return false;
  if  (cf != "") countCutflowEvent(cf+"_08_dPhi(jet,MET)");  
  
  if ( mTbmin(bjets) < 50. ) return false;
  if (cf != "") countCutflowEvent(cf+"_09_mTbmin>50");  
  
// "tau"-veto  
  for (int i = 0; i < nonbjets.size(); i++ ) 
//    if ( fabs(nonbjets[i]->Eta) < 2.5 && nonbjets[i]->NCharged  < 4 && fabs( nonbjets[i]->P4().DeltaPhi(missingET->P4())) < M_PI/5. ) return false;
    if ( fabs(nonbjets[i]->Eta) < 2.5 && nonbjets[i]->NCharged  < 4 && fabs( nonbjets[i]->P4().DeltaPhi(pTmiss)) < M_PI/5. ) return false;
  if (cf != "") countCutflowEvent(cf+"_10_tauveto");    

  if ( mTbmin(bjets) < 200. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_11_mTbmin>200");    
  
  if ( largeR12.size() < 2) return false;
  if  (cf != "") countCutflowEvent(cf+"_12_2Ljets");
  
  if ( largeR12[0].m() < 120.) return false;
  if  (cf != "") countCutflowEvent(cf+"_13_m0Lj>120");  
  
  if ( largeR12[1].m() < lR1_cutlow or largeR12[1].m() > lR1_cuthigh ) return false;
  if  (cf != "") countCutflowEvent(cf+"_14_m_1Lj");     
  
  double mt2 = 1000.; // events with one b-jet will pass this, but will be vetoed later anyway
  if ( bjets.size() > 1 and nonbjets.size() == 2) {
    std::vector<TLorentzVector> Ws = {nonbjets[0]->P4(), nonbjets[1]->P4()};
//    massW1->Fill(Ws[0].M());
//    massW2->Fill(Ws[1].M());
    std::vector<TLorentzVector> tops = find_tcandidates(Ws, bjets);
//    masst1->Fill(tops[0].M());
//    masst2->Fill(tops[1].M());  
    mt2 = mT2chi2( tops[0], tops[1], 0., pTmiss );
  }
  if ( bjets.size() > 1 and nonbjets.size() == 3) {
    std::vector<TLorentzVector> Ws = find_Wcandidates3(nonbjets);
//    massW1->Fill(Ws[0].M());
//    massW2->Fill(Ws[1].M());
    std::vector<TLorentzVector> tops = find_tcandidates(Ws, bjets);
//    masst1->Fill(tops[0].M());
//    masst2->Fill(tops[1].M());  
    mt2 = mT2chi2( tops[0], tops[1], 0., pTmiss );
  }
  if ( bjets.size() > 1 and nonbjets.size() > 3) {
    std::vector<TLorentzVector> Ws = find_Wcandidates(nonbjets);
//    massW1->Fill(Ws[0].M());
//    massW2->Fill(Ws[1].M());
    std::vector<TLorentzVector> tops = find_tcandidates(Ws, bjets);
//    masst1->Fill(tops[0].M());
//    masst2->Fill(tops[1].M());  
    mt2 = mT2chi2( tops[0], tops[1], 0., pTmiss );
  }    
  
  if (mt2 < 450. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_15_mchi2_T2");    
  
  if ( largeR08.size() == 0 or largeR08[0].m() < 60. ) return false;  
  if  (cf != "") countCutflowEvent(cf+"_16_m0j0.8");  
    
//  if (met/sqrt(ht) < 25.) return false;   //OK substitute here a simple significance aka "event-based"
//  if (met/sqrt(resolution) < 25.) return false;
  if (calcMETSignificance(jets) < 25.) return false;
  if  (cf != "") countCutflowEvent(cf+"_17_S>25");
  
  TLorentzVector jet12 = TLorentzVector( largeR12[0].px(), largeR12[0].py(), largeR12[0].pz(), largeR12[0].e() );
  
  bool dr = false;
  for (int i = 0; i < bjets.size(); i++)
      if (bjets[i]->P4().DeltaR(jet12) < 1.2) { dr = true; break; }
  if (!dr) return false;
  if  (cf != "") countCutflowEvent(cf+"_18_j1b");
  
  if (deltaRbb > 0) {
      
    dr = false;
    jet12 = TLorentzVector( largeR12[1].px(), largeR12[1].py(), largeR12[1].pz(), largeR12[1].e() );  
    for (int i = 0; i < bjets.size(); i++)
      if (bjets[i]->P4().DeltaR(jet12) < 1.2) { dr = true; break; }
    if (!dr) return false;
    if  (cf != "") countCutflowEvent(cf+"_19_j2b");      
      
    if ( bjets[0]->P4().DeltaR(bjets[1]->P4()) < deltaRbb ) return false;   
    if  (cf != "") countCutflowEvent(cf+"_20_DRbb");
      
  }
  
  return true;
}


bool Atlas_2004_14060::SRB( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::vector<fastjet::PseudoJet> largeR12, double lR1_cutlow, double lR1_cuthigh, std::string cf) {
    
  if (jets.size() < 4) return false;
  if  (cf != "") countCutflowEvent(cf+"_03_4jets");    
  
  if (bjets.size() < 2) return false;
  if  (cf != "") countCutflowEvent(cf+"_04_2bjets");    
  
  if (electronsLoose.size() + muonsCombined.size() != 0) return false;
  if  (cf != "") countCutflowEvent(cf+"_05_lepton_veto");     

  if (jets[3]->PT < 40.) return false;
  if  (cf != "") countCutflowEvent(cf+"_06_jet4>40");  
  
  if (jets[1]->PT < 80.) return false;
  if  (cf != "") countCutflowEvent(cf+"_07_jet2>80");      
  
  for (int i = 0; i < 4; i++)
    if ( fabs(jets[i]->P4().DeltaPhi(pTmiss)) < 0.4 ) return false;
  if  (cf != "") countCutflowEvent(cf+"_08_dPhi(jet,MET)");  
  
  if ( mTbmin(bjets) < 50. ) return false;
  if (cf != "") countCutflowEvent(cf+"_09_mTbmin>50");  
  
// "tau"-veto  
  for (int i = 0; i < nonbjets.size(); i++ ) 
//    if ( fabs(nonbjets[i]->Eta) < 2.5 && nonbjets[i]->NCharged  < 4 && fabs( nonbjets[i]->P4().DeltaPhi(missingET->P4())) < M_PI/5. ) return false;
    if ( fabs(nonbjets[i]->Eta) < 2.5 && nonbjets[i]->NCharged  < 4 && fabs( nonbjets[i]->P4().DeltaPhi(pTmiss)) < M_PI/5. ) return false;
  if (cf != "") countCutflowEvent(cf+"_10_tauveto");    

  if ( mTbmin(bjets) < 200. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_11_mTbmin>200");  

  if ( mTbmax(bjets) < 200. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_12_mTbmax>200");    
  
  if ( bjets[0]->P4().DeltaR(bjets[1]->P4()) < 1.4 ) return false;   
  if  (cf != "") countCutflowEvent(cf+"_13_DRbb");  
  
  if ( largeR12.size() < 2) return false;
  if  (cf != "") countCutflowEvent(cf+"_14_2Ljets");
  
  if ( largeR12[0].m() < 120.) return false;
  if  (cf != "") countCutflowEvent(cf+"_15_m0Lj>120");  
  
  double mt2 = 1000.; // events with one b-jet will pass this, but will be vetoed later anyway
  if ( bjets.size() > 1 and nonbjets.size() == 2) {
    std::vector<TLorentzVector> Ws = {nonbjets[0]->P4(), nonbjets[1]->P4()};
//    massW1->Fill(Ws[0].M());
//    massW2->Fill(Ws[1].M());
    std::vector<TLorentzVector> tops = find_tcandidates(Ws, bjets);
//    masst1->Fill(tops[0].M());
//    masst2->Fill(tops[1].M());  
    mt2 = mT2chi2( tops[0], tops[1], 0., pTmiss );
  }
  if ( bjets.size() > 1 and nonbjets.size() == 3) {
    std::vector<TLorentzVector> Ws = find_Wcandidates3(nonbjets);
//    massW1->Fill(Ws[0].M());
//    massW2->Fill(Ws[1].M());
    std::vector<TLorentzVector> tops = find_tcandidates(Ws, bjets);
//    masst1->Fill(tops[0].M());
//    masst2->Fill(tops[1].M());  
    mt2 = mT2chi2( tops[0], tops[1], 0., pTmiss );
  }
  if ( bjets.size() > 1 and nonbjets.size() > 3) {
    std::vector<TLorentzVector> Ws = find_Wcandidates(nonbjets);
//    massW1->Fill(Ws[0].M());
//    massW2->Fill(Ws[1].M());
    std::vector<TLorentzVector> tops = find_tcandidates(Ws, bjets);
//    masst1->Fill(tops[0].M());
//    masst2->Fill(tops[1].M());  
    mt2 = mT2chi2( tops[0], tops[1], 0., pTmiss );
  }    
  
  if (mt2 > 450. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_16_mchi2_T2");    
    
  //if (met/sqrt(ht) < 14.) return false;   //OK substitute here a simple significance aka "event-based"
  if (calcMETSignificance(jets) < 14.) return false;
  if  (cf != "") countCutflowEvent(cf+"_17_S>14");
  
  if ( largeR12[1].m() < lR1_cutlow or largeR12[1].m() > lR1_cuthigh ) return false;
  if  (cf != "") countCutflowEvent(cf+"_18_m_1Lj");     
  
  return true;
}


bool Atlas_2004_14060::SRD( std::vector<Jet*> nonbjets, std::string cf) {
    
  if (electronsLoose.size() + muonsCombined.size() != 0) return false;
  if  (cf != "") countCutflowEvent(cf+"_03_lepton_veto");     
  
  if ( nonbjets.size() == 0 or nonbjets[0]->PT < 250. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_04_jISR>250");

  if ( fabs(nonbjets[0]->P4().DeltaPhi( pTmiss )) < 2.4 ) return false;
  if  (cf != "") countCutflowEvent(cf+"_05_dPhi1>2.4");
  
  pTmiss_track = TLorentzVector(0.,0.,0.,0.);
  for (int i = 0; i < tracks.size(); i++) 
     if ( tracks[i]->PT > 0.5 and fabs(tracks[i]->Eta) < 2.5 ) pTmiss_track -= tracks[i]->P4(); 
     
  if ( fabs(pTmiss_track.DeltaPhi( pTmiss )) > M_PI/3. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_06_dPhi2<pi/3");     
  
  if ( pTmiss_track.Perp() < 30. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_07_trackMET>30");   
  
  return true;
    
}


bool Atlas_2004_14060::SRD0( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::string cf ) {
    
    if (met/sqrt(ht) < 26.) return false;
    if  (cf != "") countCutflowEvent(cf+"_08_MET/HT>26");   
             
    if (btrackjets.size() == 0 or fabs(btrackjets[0].Eta()) > 1.2 )  return false;         
    if  (cf != "") countCutflowEvent(cf+"_09_btrack>0");  
             
    if (bjets.size() != 0 ) return false;         
    if  (cf != "") countCutflowEvent(cf+"_10_bjet=0");
             
    if (btrackjets.size() < 2 or fabs(btrackjets[0].DeltaPhi(btrackjets[1])) > 2.5) return false;
    if  (cf != "") countCutflowEvent(cf+"_11_dPhi_btrack<2.5");    
             
    if (btrackjets[0].Perp() > 50. ) return false;
    if  (cf != "") countCutflowEvent(cf+"_12_btrackpt<50");     
             
    for (int i = 0; i < 4 and i < trackjets.size(); i++)
        if (fabs(trackjets[i].DeltaPhi(nonbjets[0]->P4())) < 1.2 ) return false;
    if  (cf != "") countCutflowEvent(cf+"_13_dPhi_ISR>1.2");          
    
    double phimin = 10.;          // not sure if j_ISR is still included in jets container 
    for (int i = 0; i < 4 and i < jets.size(); i++) {
        double dphi = fabs(jets[i]->P4().DeltaPhi(pTmiss));
        if ( dphi < phimin) phimin = dphi;
    }
    if (phimin < 0.4) return false;
    if  (cf != "") countCutflowEvent(cf+"_14_dPhi_jetMET>1.2");    
    
    double phimax = 0;
    for (int i = 0; i < btrackjets.size(); i++) {
        double dphi = fabs(nonbjets[0]->P4().DeltaPhi(btrackjets[i]));
        if ( dphi > phimax) phimax = dphi;
    }
    if (phimax < 2.2) return false;
    if  (cf != "") countCutflowEvent(cf+"_15_dPhi_bISR>2.2");    
    
    for (int i = 0; i < 1 and i < jets.size(); i++) 
      for (int j = 0; j < 4 and j < trackjets.size(); j++) 
         if (trackjets[j].DeltaR(jets[i]->P4()) < 0.4) return false;  
    if  (cf != "") countCutflowEvent(cf+"_16_nooverlap");      
    
    return true;
    
}


bool Atlas_2004_14060::SRD1( std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::string cf ) {
    
    if (btrackjets.size() < 1 or fabs(btrackjets[0].Perp() ) < 10.) return false;
    if  (cf != "") countCutflowEvent(cf+"_08_btrackPT>10");    
    
    if (met/sqrt(ht) < 22.) return false;
    if  (cf != "") countCutflowEvent(cf+"_09_MET/HT>22");       
    
    if (bjets.size() != 1 ) return false;         
    if  (cf != "") countCutflowEvent(cf+"_10_bjet=1");    
    
    if (trackjets[0].Perp() > 40. ) return false;
    if  (cf != "") countCutflowEvent(cf+"_11_trackpt<40");         
    
    if ( fabs(nonbjets[0]->P4().DeltaPhi(bjets[0]->P4())) < 2.2) return false;
    if  (cf != "") countCutflowEvent(cf+"_12_dPhi>2.2");
    
    if ( fabs(bjets[0]->Eta) > 1.6) return false;
    if  (cf != "") countCutflowEvent(cf+"_13_bEta<1.6");
    
    for (int i = 0; i < 4 and i < trackjets.size(); i++)
        if (fabs(trackjets[i].DeltaPhi(nonbjets[0]->P4())) < 1.2 ) return false;
    if  (cf != "") countCutflowEvent(cf+"_14_dPhi_ISR>1.2");      
    
    for (int i = 0; i < 1 and i < jets.size(); i++) 
      for (int j = 0; j < 4 and j < trackjets.size(); j++) 
         if (trackjets[j].DeltaR(jets[i]->P4()) < 0.4) return false;  
    if  (cf != "") countCutflowEvent(cf+"_15_nooverlap");      
    
    return true;    
    
}

bool Atlas_2004_14060::SRD2( std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::string cf ) { 
    
    if (met/sqrt(ht) < 22.) return false;
    if  (cf != "") countCutflowEvent(cf+"_08_MET/HT>22");       
    
    if (bjets.size() < 2 ) return false;         
    if  (cf != "") countCutflowEvent(cf+"_09_bjet=2");        
    
    if ( fabs(nonbjets[0]->P4().DeltaPhi(bjets[0]->P4())) < 2.2) return false;
    if  (cf != "") countCutflowEvent(cf+"_10_dPhi>2.2");    
    
    if ( fabs(nonbjets[0]->P4().DeltaPhi(bjets[1]->P4())) < 1.6) return false;
    if  (cf != "") countCutflowEvent(cf+"_11_dPhi>1.6");      
    
    if ( bjets[0]->PT > 175.) return false;
    if  (cf != "") countCutflowEvent(cf+"_12_b1PT<175");   
    
    if ( fabs(bjets[1]->Eta) > 1.2) return false;
    if  (cf != "") countCutflowEvent(cf+"_13_b2Eta<1.2");    
    
    return true;
}

std::vector<TLorentzVector> Atlas_2004_14060::find_Wcandidates3( std::vector<Jet*> &jets_in ) {

  double chi2 = 10000.;
  std::vector<TLorentzVector> candidates = {TLorentzVector(0. ,0. ,0. ,0.), TLorentzVector(0. ,0. ,0. ,0.)};
  std::vector<Jet*> remaining;
  for (int i1 = 0; i1 < jets_in.size()-1; i1++ ) //first W from all jets
    for (int j1 = i1+1; j1 < jets_in.size(); j1++ ) {
      std::vector<Jet*> temp_jets(jets_in);
      TLorentzVector temp_candidate1 = jets_in[i1]->P4() + jets_in[j1]->P4();
      temp_jets.erase(temp_jets.begin()+j1);
      temp_jets.erase(temp_jets.begin()+i1);
      TLorentzVector temp_candidate2 = temp_jets[0]->P4();
      double temp_chi2 = pow( temp_candidate1.M() - 80.4, 2)/80.4 + pow( temp_candidate2.M() - 80.4, 2)/80.4 ;
      if (temp_chi2 < chi2) {
	 chi2 = temp_chi2;
	 candidates = {temp_candidate1, temp_candidate2};
      }
    }

  return candidates;
}


std::vector<TLorentzVector> Atlas_2004_14060::find_Wcandidates( std::vector<Jet*> &jets_in ) {

  double chi2 = 10000.;
  std::vector<TLorentzVector> candidates = {TLorentzVector(0. ,0. ,0. ,0.), TLorentzVector(0. ,0. ,0. ,0.)};
  std::vector<Jet*> remaining;
  for (int i1 = 0; i1 < jets_in.size()-1; i1++ ) //first W from all jets
    for (int j1 = i1+1; j1 < jets_in.size(); j1++ ) {
      std::vector<Jet*> temp_jets(jets_in);
      TLorentzVector temp_candidate1 = jets_in[i1]->P4() + jets_in[j1]->P4();
      temp_jets.erase(temp_jets.begin()+j1);
      temp_jets.erase(temp_jets.begin()+i1);
      for (int i2 = 0; i2 < jets_in.size()-3; i2++ ) //second W from remaining jets
        for (int j2 = i2+1; j2 < jets_in.size()-2; j2++ ) {
	  std::vector<Jet*> temp2_jets(temp_jets);
	  TLorentzVector temp_candidate2 = temp2_jets[i2]->P4() + temp2_jets[j2]->P4();
          double temp_chi2 = pow( temp_candidate1.M() - 80.4, 2)/80.4 + pow( temp_candidate2.M() - 80.4, 2)/80.4 ;
          if (temp_chi2 < chi2) {
	    chi2 = temp_chi2;
	    candidates = {temp_candidate1, temp_candidate2};
	    temp2_jets.erase(temp2_jets.begin()+j2);
	    temp2_jets.erase(temp2_jets.begin()+i2);
	    remaining = temp2_jets;
          }
        }
    }
  jets_in = remaining;
  return candidates;
}


std::vector<TLorentzVector> Atlas_2004_14060::find_tcandidates( std::vector<TLorentzVector> Wcandidates, std::vector<Jet*> &jets_in ) {

  double chi2 = 10000.;
  std::vector<TLorentzVector> candidates = {TLorentzVector(0. ,0. ,0. ,0.), TLorentzVector(0. ,0. ,0. ,0.)};
  for (int i = 0; i < jets_in.size(); i++ ) 
    for (int j = 0; j < jets_in.size(); j++ ) 
      if ( i != j ) {
        std::vector<TLorentzVector> temp_candidates = {jets_in[i]->P4() + Wcandidates[0], jets_in[j]->P4() + Wcandidates[1]};
        double temp_chi2 = pow( temp_candidates[0].M() - 173.2, 2)/173.2 + pow( temp_candidates[1].M() - 173.2, 2)/173.2;
        if (temp_chi2 < chi2) {
	  chi2 = temp_chi2;
	  candidates = temp_candidates;
       }
     }

  return candidates;
}

double Atlas_2004_14060::mT2chi2(const TLorentzVector & vis1, const TLorentzVector & vis2, double m_inv, const TLorentzVector & invis) {
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
    double p1[3] = {173.2, vis1.Px(), vis1.Py()};
    double p2[3] = {173.2, vis2.Px(), vis2.Py()};
    mt2_event.set_momenta( p1, p2, pmiss );
    mt2_event.set_mn( m_inv );
    return mt2_event.get_mt2();  
}


double Atlas_2004_14060::mTbmin(std::vector<Jet*> bjets) {
  
  double dphi_min = 100.;
  int i_min = 100;
  for (int i = 0; i < bjets.size(); i++ ) {
//    double temp_dphi_min = fabs(bjets[i]->P4().DeltaPhi( missingET->P4() )) ;
    double temp_dphi_min = fabs(bjets[i]->P4().DeltaPhi( pTmiss )) ;    
    if (temp_dphi_min < dphi_min ) {
      dphi_min = temp_dphi_min;
      i_min = i;
    }
  }
//  return mT(bjets[i_min]->P4(), missingET->P4(), 0.);
  return sqrt(2.*bjets[i_min]->PT*pTmiss.Perp()*(1. - cos(bjets[i_min]->P4().DeltaPhi(pTmiss))));
}

double Atlas_2004_14060::mTbmax(std::vector<Jet*> bjets) {

  double dphi_max = 0.;
  int i_max = 100;
  for (int i = 0; i < bjets.size(); i++ ) {
//    double temp_dphi_max = fabs(bjets[i]->P4().DeltaPhi( missingET->P4() )) ;
    double temp_dphi_max = fabs(bjets[i]->P4().DeltaPhi( pTmiss )) ;
    if (temp_dphi_max > dphi_max ) {
      dphi_max = temp_dphi_max;
      i_max = i;
    }
  }  
//  return mT(bjets[i_max]->P4(), missingET->P4(), 0.);
  return sqrt(2.*bjets[i_max]->PT*pTmiss.Perp()*(1. - cos(bjets[i_max]->P4().DeltaPhi(pTmiss))));
}

std::vector<TLorentzVector> Atlas_2004_14060::findtrackjets(Jet* leading) {
    
  std::vector<fastjet::PseudoJet> particles;
  for ( int i = 0; i < tracks.size(); i++ ) 
    if ( fabs(tracks[i]-> Eta) < 2.5 and tracks[i]->PT > 0.5) 
        particles.push_back( fastjet::PseudoJet(tracks[i]->P4().Px(), tracks[i]->P4().Py(), tracks[i]->P4().Pz(), tracks[i]->P4().E()) );
  
  fastjet::JetDefinition tjet(fastjet::antikt_algorithm, 0.4);
  fastjet::ClusterSequence tj(particles, tjet);

  std::vector<fastjet::PseudoJet> track_jet = sorted_by_pt(tj.inclusive_jets());
  
  std::vector<TLorentzVector> track_out;
  for (int i = 0; i < track_jet.size(); i++) {
      TLorentzVector temp = TLorentzVector( track_jet[i].px(), track_jet[i].py(), track_jet[i].pz(), track_jet[i].e());
      if (temp.Perp() > 5. and  fabs(temp.Eta()) < 2.5 and temp.DeltaR(leading->P4()) > 0.4 ) track_out.push_back(TLorentzVector( track_jet[i].px(), track_jet[i].py(), track_jet[i].pz(), track_jet[i].e())); 
      
  }
  
  return track_out;
  
}


bool Atlas_2004_14060::trackbtag( TLorentzVector jetin) {
   
    bool tag = false;
//    for(int i=0; i < true_particles.size(); i++) {                          // loop over reconstructed
//      
//       auto part = true_particles[i];
//       TLorentzVector mom = TLorentzVector(part->Px, part->Py, part->Pz, part->E);
//       if ( fabs(part->PID) == 5 and  mom.DeltaR(jetin) ) tag = true;
//    }
//    
//    if ( tag and std::rand()/RAND_MAX < 0.7 ) return true;
    
    for(int b = 0; b < true_b.size(); b++) 
       if( fabs(true_b[b]->Eta) < 2.5 && true_b[b]->P4().DeltaR(jetin) < 0.4) tag = true;
       
    if ( tag and std::rand()/RAND_MAX < 0.7 ) return true;   
       
    return false;
    
}

// code from ATLAS
void Atlas_2004_14060::rotateXY(TMatrix &mat, TMatrix &mat_new, double phi) {
  double c = cos(phi);
  double s = sin(phi);
  double cc = c*c;
  double ss = s*s;
  double cs = c*s;

  mat_new(0,0) = mat(0,0)*cc + mat(1,1)*ss - cs*(mat(1,0) + mat(0,1));
  mat_new(0,1) = mat(0,1)*cc - mat(1,0)*ss + cs*(mat(0,0) - mat(1,1));
  mat_new(1,0) = mat(1,0)*cc - mat(0,1)*ss + cs*(mat(0,0) - mat(1,1));
  mat_new(1,1) = mat(0,0)*ss + mat(1,1)*cc + cs*(mat(1,0) + mat(0,1));
}

double Atlas_2004_14060::calcMETSignificance(std::vector<Jet*> jets) {

  TMatrix cov_sum(2,2);

  TLorentzVector softVec = missingET->P4();
  
  TMatrix particle_u(2,2),particle_u_rot(2,2);
  for( int i = 0; i < jets.size(); i++ ) {
    softVec += jets[i]->P4();  // soft term is everything not included in hard objects
    //double pt_reso = 0.77*pow(jets[i]->PT, -0.39);
    double pt_reso = jets[i]->PT < 200 ? 1.85*pow(jets[i]->PT, -0.71) : 0.05;  //cf. https://cds.cern.ch/record/2630948/files/ATLAS-CONF-2018-038.pdf Fig. 3
    double phi_reso = jets[i]->PT < 100 ? 1.23*pow(jets[i]->PT, -0.95) : 0.015;
    particle_u(0,0) = pow(pt_reso*jets[i]->PT, 2);
    particle_u(1,1) = pow(phi_reso*jets[i]->PT,2);
    rotateXY(particle_u, particle_u_rot, missingET->P4().DeltaPhi(jets[i]->P4()));
    cov_sum += particle_u_rot;
  }
  
 //add soft term resolution (fixed 10 GeV)
  particle_u(0,0) = 10*10;
  particle_u(1,1) = 10*10;
  rotateXY(particle_u, particle_u_rot, missingET->P4().DeltaPhi(softVec));
  cov_sum+=particle_u_rot;  

  //calculate significance
  double varL = cov_sum(0,0);
  double varT = cov_sum(1,1);
  double covLT = cov_sum(0,1);

  double significance = 0;
  double rho = 0;
  if( varL != 0 ){
    rho = covLT / sqrt( varL * varT ) ;
    if (fabs( rho ) >= 0.9 ) rho = 0; //too large - ignore it
    significance = met/sqrt((varL*(1-pow(rho,2))));
  }
  return significance;  
    
}
