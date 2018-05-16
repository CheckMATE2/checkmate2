#include "atlas_1709_04183.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1709_04183::initialize() {
  setAnalysisName("atlas_1709_04183");          
  setInformation(""
    "# search for stop/gluino pair production\n"
    "# at least 4 jets, no leptons and MET\n"
    "# 36/fb\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  // C SIGNAL REGIONS NOT INCLUDED YET!!!!
  bookSignalRegions("SRA-T0;SRA-TT;SRA-TW;SRB-T0;SRB-TT;SRB-TW;SRC1;SRC2;SRC3;SRC4;SRC5;SRD-high;SRD-low;SRE");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  
/*  hfile = new TFile("2017_020.root", "RECREATE", "Saving Histograms");
  massW1 = new TH1F("", "", 20, 0., 200.);
  massW2 = new TH1F("", "", 20, 0., 200.);
  masst1 = new TH1F("", "", 20, 100., 300.);
  masst2 = new TH1F("", "", 20, 100., 300.); */ 
}

void Atlas_1709_04183::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsLoose = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 6., -2.7, 2.7);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);  

  countCutflowEvent("00_all");
  
  std::vector<Jet*> bjets;  
  std::vector<Jet*> nonbjets;
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i]) ) 
      bjets.push_back(jets[i]);
    else nonbjets.push_back(jets[i]);
    
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.2, "y");
  jets = overlapRemoval(jets, electronsLoose, 0.2, "y");    
  nonbjets = overlapRemoval(nonbjets, electronsLoose, 0.2, "y");  
  electronsLoose = overlapRemoval(electronsLoose, jets, 0.4, "y");   
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4, "y");
 
  
  TLorentzVector track_MET;
  for (int i = 0; i < tracks.size(); i++) 
     if ( tracks[i]->PT > 0.5 and fabs(tracks[i]->Eta) < 2.5 ) track_MET -= tracks[i]->P4();   
     
  double ht = 0.;   
  for (int i = 0; i < jets.size(); i++)   
    ht += jets[i]->PT;
  if (ht < 150. ) return;
  countCutflowEvent("00_deriv_skim"); 
  
  // COSMIC MUONS, BAD JETS, ETC.
  if (  rand()/(RAND_MAX+1.) > 0.975 ) return;
  countCutflowEvent("00_jet_clean");

// common selections  
  
  if (electronsLoose.size() + muonsCombined.size() != 0) return;
  countCutflowEvent("01_lepton_veto");     
     
  if (jets.size() < 4) return;
  countCutflowEvent("02_4jets");
  
  if (track_MET.Perp() < 30. ) return;
  countCutflowEvent("03_METtrack>30");  
  
  if (bjets.size() == 0) return;
  countCutflowEvent("04_btag");  
  
//another calculation of MET 
  pTmiss = TLorentzVector(0., 0., 0., 0.);
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
  }
  
  pTmiss += pTmiss_soft;
  
//  double met = missingET->P4().Et();
  double met = pTmiss.Perp();
  
  if (met < 250.) return;
  countCutflowEvent("05_MET>250");
  
  //if ( fabs(missingET->P4().DeltaPhi(track_MET) ) > M_PI/3. ) return;
  if ( fabs(pTmiss.DeltaPhi(track_MET) ) > M_PI/3. ) return;
  countCutflowEvent("06_dPhi(METs)");  
  
// THIS CONDITION IS DIFFERENT FOR SRC WHICH IS NOT INCLUDED HERE  
//  if ( fabs(jets[0]->P4().DeltaPhi(missingET->P4())) < 0.4 or fabs(jets[1]->P4().DeltaPhi(missingET->P4())) < 0.4 ) return;
  if ( fabs(jets[0]->P4().DeltaPhi(pTmiss)) < 0.4 or fabs(jets[1]->P4().DeltaPhi(pTmiss)) < 0.4 or fabs(jets[2]->P4().DeltaPhi(pTmiss)) < 0.4) return;
  countCutflowEvent("07_dPhi(jet,MET)");
  
  if (jets[1]->PT < 80.) return;
  countCutflowEvent("08a_jets_pT");  
  
  if (jets[3]->PT < 40.) return;
  countCutflowEvent("08b_jets_pT");    
  
  std::vector<fastjet::PseudoJet> particles;
  for ( int i = 0; i < jets.size(); i++ ) 
    particles.push_back( fastjet::PseudoJet(jets[i]->P4().Px(), jets[i]->P4().Py(), jets[i]->P4().Pz(), jets[i]->P4().E()) );

  fastjet::JetDefinition jet_def12(fastjet::antikt_algorithm, 1.2);
  fastjet::JetDefinition jet_def08(fastjet::antikt_algorithm, 0.8);  
 
  fastjet::ClusterSequence cs12(particles, jet_def12);
  fastjet::ClusterSequence cs08(particles, jet_def08);
  std::vector<fastjet::PseudoJet> largeR12 = sorted_by_pt(cs12.inclusive_jets());  
  std::vector<fastjet::PseudoJet> largeR08 = sorted_by_pt(cs08.inclusive_jets());  
  
  if ( SRA( jets, bjets, nonbjets, largeR12, largeR08, 120., 10000., 1., 400., 400., "SRA-TT") ) countSignalEvent("SRA-TT");
  if ( SRA( jets, bjets, nonbjets, largeR12, largeR08,  60.,   120., 0., 400., 500., "SRA-TW") ) countSignalEvent("SRA-TW");
  if ( SRA( jets, bjets, nonbjets, largeR12, largeR08,   0.,    60., 0., 500., 550., "SRA-T0") ) countSignalEvent("SRA-T0");
  if ( SRB( jets, bjets, nonbjets, largeR12, 120., 10000., "SRB-TT") ) countSignalEvent("SRB-TT");
  if ( SRB( jets, bjets, nonbjets, largeR12,  60.,   120., "SRB-TW") ) countSignalEvent("SRB-TW");
  if ( SRB( jets, bjets, nonbjets, largeR12,   0.,    60., "SRB-T0") ) countSignalEvent("SRB-T0");  
  if ( SRD( jets, bjets, nonbjets, 300., 100., 250., 300., "SRD-low") ) countSignalEvent("SRD-low"); 
  if ( SRD( jets, bjets, nonbjets, 400.,  80., 350., 450., "SRD-high") ) countSignalEvent("SRD-high"); 
  if ( SRE( jets, bjets, largeR08, "SRE") ) countSignalEvent("SRE"); 

  return;    
  
}

void Atlas_1709_04183::finalize() {
  // Whatever should be done after the run goes here
  
/*  TCanvas can1;
  massW1->Draw();
  massW1->SetTitle("mass W1");
  can1.Close();

  TCanvas can2;
  massW2->Draw();
  massW2->SetTitle("mass W2");
  can2.Close();

  TCanvas can3;
  masst1->Draw();
  masst1->SetTitle("mass t1");
  can3.Close();

  TCanvas can4;
  masst2->Draw();
  masst2->SetTitle("mass t2");
  can4.Close();

  hfile->Write();

  hfile->Close();  */
}       


std::vector<TLorentzVector> Atlas_1709_04183::find_Wcandidates3( std::vector<Jet*> &jets_in ) {

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


std::vector<TLorentzVector> Atlas_1709_04183::find_Wcandidates( std::vector<Jet*> &jets_in ) {

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


std::vector<TLorentzVector> Atlas_1709_04183::find_tcandidates( std::vector<TLorentzVector> Wcandidates, std::vector<Jet*> &jets_in ) {

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

double Atlas_1709_04183::mT2chi2(const TLorentzVector & vis1, const TLorentzVector & vis2, double m_inv, const TLorentzVector & invis) {
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


double Atlas_1709_04183::mTbmin(std::vector<Jet*> bjets) {
  
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

double Atlas_1709_04183::mTbmax(std::vector<Jet*> bjets) {

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

bool Atlas_1709_04183::SRA( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::vector<fastjet::PseudoJet> largeR12, std::vector<fastjet::PseudoJet> largeR08, double lR1_cutlow, double lR1_cuthigh, double deltaRbb, double mchiT2_cut, double met, std::string cf) {
  
  if ( largeR12.size() < 2) return false;
  if (cf != "") countCutflowEvent(cf+"_09_2Ljets");
  
  if ( largeR12[0].m() < 120.) return false;
  if (cf != "") countCutflowEvent(cf+"_10_m_0Lj>120");  
  
  if ( largeR12[1].m() < lR1_cutlow or largeR12[1].m() > lR1_cuthigh ) return false;
  if (cf != "") countCutflowEvent(cf+"_11_m_1Lj");     
  
//  if (missingET->P4().Et() < met) return false;
  if ( pTmiss.Perp() < met) return false;
  if (cf != "") countCutflowEvent(cf+"_12_MET");    
  
  if ( largeR08.size() > 0 && largeR08[0].m() < 60. ) return false;  
  if  (cf != "") countCutflowEvent(cf+"_13_m0j0.8");  
  
  if ( mTbmin(bjets) < 200. ) return false;
  if (cf != "") countCutflowEvent(cf+"_14_mTbmin");

// "tau"-veto  
  for (int i = 0; i < nonbjets.size(); i++ ) 
//    if ( fabs(nonbjets[i]->Eta) < 2.5 && nonbjets[i]->NCharged  < 4 && fabs( nonbjets[i]->P4().DeltaPhi(missingET->P4())) < M_PI/5. ) return false;
    if ( fabs(nonbjets[i]->Eta) < 2.5 && nonbjets[i]->NCharged  < 4 && fabs( nonbjets[i]->P4().DeltaPhi(pTmiss)) < M_PI/5. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_15_tauveto");  
  
  if ( bjets.size() > 1 and  bjets[0]->P4().DeltaR(bjets[1]->P4()) < deltaRbb ) return false;   
  if  (cf != "") countCutflowEvent(cf+"_16_DRbb");  
  
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

  if (mt2 < mchiT2_cut ) return false;
  if (cf != "") countCutflowEvent(cf+"_18_mchi2_T2");  
  
  if ( bjets.size() < 2 ) return false;
  if  (cf != "") countCutflowEvent(cf+"_19_2bjets");
    
// MIGHT BE NEEDED ONCE SRC ADDED  
//  if ( fabs(jets[2]->P4().DeltaPhi(missingET->P4())) < 0.4 ) return false;    
//  if ( fabs(jets[2]->P4().DeltaPhi(pTmiss)) < 0.4 ) return false;   
//  if  (cf != "") countCutflowEvent(cf+"_15_dphi_jet3");  
  
  
  return true;
}

bool Atlas_1709_04183::SRB( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::vector<fastjet::PseudoJet> largeR12, double lR1_cutlow, double lR1_cuthigh, std::string cf) {
  
  if ( largeR12.size() < 2) return false;
  if (cf != "") countCutflowEvent(cf+"_09_2Ljets");
  
  if ( largeR12[0].m() < 120.) return false;
  if (cf != "") countCutflowEvent(cf+"_10_m_0Lj>120");  
  
  if ( largeR12[1].m() > lR1_cuthigh ) return false;
  if (cf != "") countCutflowEvent(cf+"_11a_m_1Lj");   
  
  if ( largeR12[1].m() < lR1_cutlow) return false;
  if (cf != "") countCutflowEvent(cf+"_11b_m_1Lj");     
  
  if ( bjets.size() > 1 and bjets[0]->P4().DeltaR(bjets[1]->P4()) < 1.2 ) return false;  
  if ( bjets.size() == 1 and bjets[0]->P4().DeltaR(nonbjets[0]->P4()) < 1.2 ) return false;  
  if  (cf != "") countCutflowEvent(cf+"_12_DRbb");    

  if ( mTbmax(bjets) < 200. ) return false;
  if (cf != "") countCutflowEvent(cf+"_13_mTbmax");
  
  if ( mTbmin(bjets) < 200. ) return false;
  if (cf != "") countCutflowEvent(cf+"_14_mTbmin");
  
// "tau"-veto  
  for (int i = 0; i < nonbjets.size(); i++ ) 
//    if ( fabs(nonbjets[i]->Eta) < 2.5 && nonbjets[i]->NCharged  < 4 && fabs( nonbjets[i]->P4().DeltaPhi(missingET->P4())) < M_PI/5. ) return false;
      if ( fabs(nonbjets[i]->Eta) < 2.5 && nonbjets[i]->NCharged  < 4 && fabs( nonbjets[i]->P4().DeltaPhi(pTmiss)) < M_PI/5. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_15_tauveto");
    
// MIGHT BE NEEDED ONCE SRC ADDED    
//  if ( fabs(jets[2]->P4().DeltaPhi(missingET->P4())) < 0.4 ) return false;    
//  if ( fabs(jets[2]->P4().DeltaPhi(pTmiss)) < 0.4 ) return false;      
//  if  (cf != "") countCutflowEvent(cf+"_15_dphi_jet3");  
  
  if ( bjets.size() < 2 ) return false;
  if  (cf != "") countCutflowEvent(cf+"_16_2bjets");
  
  return true;
}

bool Atlas_1709_04183::SRD( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, double pTb_sum, double pTj3, double mTbmin_cut, double mTbmax_cut, std::string cf) {
  
  if ( jets.size() < 5 ) return false; 
  if (cf != "") countCutflowEvent(cf+"_09_njet>4");  
  
// MIGHT BE NEEDED ONCE SRC ADDED    
//  if ( fabs(jets[2]->P4().DeltaPhi(missingET->P4())) < 0.4 ) return false;    
//  if ( fabs(jets[2]->P4().DeltaPhi( pTmiss )) < 0.4 ) return false;      
//  if  (cf != "") countCutflowEvent(cf+"_10_dphi_jet2");  
  
  if ( jets[1]->PT < 150. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_10_pTj1");

  if ( jets[3]->PT < pTj3 ) return false;
  if  (cf != "") countCutflowEvent(cf+"_11_pTj3");
  
  if ( jets[4]->PT < 60. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_12_pTj4");   
  
  if ( mTbmin(bjets) < mTbmin_cut ) return false;
  if (cf != "") countCutflowEvent(cf+"_13_mTbmin");  
  
  if ( mTbmax(bjets) < mTbmax_cut ) return false;
  if (cf != "") countCutflowEvent(cf+"_14_mTbmax");    
  
  if ( bjets.size() < 2 ) return false; 
  if (cf != "") countCutflowEvent(cf+"_15_2bjets");   
  
  if ( bjets[0]->P4().DeltaR(bjets[1]->P4()) < 0.8 ) return false;   
  if  (cf != "") countCutflowEvent(cf+"_16_DRbb");   
  
  if ( bjets[0]->PT + bjets[1]->PT < pTb_sum ) return false;
  if  (cf != "") countCutflowEvent(cf+"_17_pTbSum");
  
// "tau"-veto  
  for (int i = 0; i < nonbjets.size(); i++ ) 
//    if ( fabs(nonbjets[i]->Eta) < 2.5 && nonbjets[i]->NCharged  < 4  && fabs( nonbjets[i]->P4().DeltaPhi(missingET->P4())) < M_PI/5. ) return false;
    if ( fabs(nonbjets[i]->Eta) < 2.5 && nonbjets[i]->NCharged  < 4  && fabs( nonbjets[i]->P4().DeltaPhi(pTmiss)) < M_PI/5. ) return false;
  if  (cf != "") countCutflowEvent(cf+"_18_tauveto");  
  
  
  return true;
}

bool Atlas_1709_04183::SRE( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<fastjet::PseudoJet> largeR08, std::string cf) {
  
//  double met = missingET->P4().Et();
  double met = pTmiss.Perp();
  if ( met < 550. ) return false;
  if (cf != "") countCutflowEvent(cf+"_09_MET>550");   
  
  if ( largeR08.size() < 1 or largeR08[0].m() < 120. ) return false;  
  if  (cf != "") countCutflowEvent(cf+"_10_m0j0.8");  
  
  if ( largeR08.size() < 2 or largeR08[1].m() < 80. ) return false;  
  if  (cf != "") countCutflowEvent(cf+"_11_m1j0.8");      
  
  double HT = 0.;
  for (int i = 0; i < jets.size(); i++) 
    HT += jets[i]->PT;
  
  if (HT < 800. ) return false;
  if (cf != "") countCutflowEvent(cf+"_12_HT>800");     

  if (met/sqrt(HT) < 18. ) return false;
  if (cf != "") countCutflowEvent(cf+"_13_MET/sqrt(HT)>18");       
  
  if ( mTbmin(bjets) < 200. ) return false;
  if (cf != "") countCutflowEvent(cf+"_14_mTbmin");         
  
//  if ( fabs(jets[2]->P4().DeltaPhi(missingET->P4())) < 0.4 ) return false;    
//  if  (cf != "") countCutflowEvent(cf+"_09_dphi_jet2");  
  
  if ( bjets.size() < 2 ) return false; 
  if (cf != "") countCutflowEvent(cf+"_15_2bjets");     
  
  return true;
}  
