#include "atlas_conf_2014_014.h"

void Atlas_conf_2014_014::initialize() {
  setAnalysisName("atlas_conf_2014_014");          
  setInformation(""
    "@#stop decaying to stau, neutrino, and b jet, stau to tau and gravitino\n"
     "@#2 letpons, 2 jets and b-jet in the final state\n"
     "@#possible sensitive to small mass difference between stop and stau\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SRa;");
  bookCutflowRegions("00_preselected;01_isolation;02_opposite_sign;03_mll>20;04_leading_pt>25;05_Z_veto;06_dphi_Etmiss_j>1;07_dphi_pllTb<1.5;08_jets>2;09_btag;10_mjl<180;11_mT2<70;12_Emiss/Emiss+leptons>0.45;13_HT/meff<0.4;14_dx<0.04;");
  // You should initialize any declared variables here
/*  histo_etadiff = new TH1D("etadiff", "rapidity distance between leptons", 20, -1 , 1);
  histo_etadiff->SetMinimum(0.);
  histo_etasum = new TH1D("etasum", "rapidity sum of leptons", 8, 0 , 1);
  histo_etasum->SetMinimum(0.);
  histo_dx = new TH1D("dx", "dx by Melia", 10, 0 , 0.1);
  histo_dx->SetMinimum(0.); */
}

void Atlas_conf_2014_014::analyze() {
  // Your eventwise analysis code goes here
  // isolation criteria from 2013-048
  
  missingET->addMuons(muonsCombined); 
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 20., -2.5, 2.5);
  jets = overlapRemoval(jets, electronsTight, 0.2);
  
  if ( electronsTight.size() + muonsCombined.size() != 2) return;
  countCutflowEvent("00_preselected");
  
  std::vector<Electron*> isoElecs = filterIsolation(electronsTight);
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);
  
  if ( isoElecs.size() + isoMuons.size() != 2) return;
  countCutflowEvent("01_isolation");
  
  bool SF = false; bool OS = false;
  std::vector<TLorentzVector> leptons;
  if (isoElecs.size() == 2) {
      leptons.push_back( isoElecs[0]->P4() );
      leptons.push_back( isoElecs[1]->P4() );
      SF = true;
      if ( isoElecs[0]->Charge*isoElecs[1]->Charge < 0 ) OS = true;
  }
  else if (isoMuons.size() == 2) {
      leptons.push_back( isoMuons[0]->P4() );
      leptons.push_back( isoMuons[1]->P4() );
      SF = true;
      if ( isoMuons[0]->Charge*isoMuons[1]->Charge < 0 ) OS = true;
  }  
  else {
      if ( isoElecs[0]->PT > isoMuons[0]->PT ) {
      leptons.push_back( isoElecs[0]->P4() );
      leptons.push_back( isoMuons[0]->P4() );
      }
      else {
      leptons.push_back( isoMuons[0]->P4() );
      leptons.push_back( isoElecs[0]->P4() );
      }
      if ( isoMuons[0]->Charge*isoElecs[0]->Charge < 0 ) OS = true;
  }
  
  if ( !OS ) return;
  countCutflowEvent("02_opposite_sign");
  
  if ( (leptons[0] + leptons[1]).M() < 20. ) return;
  countCutflowEvent("03_mll>20");
  
  if ( leptons[0].Perp() < 25. ) return;
  countCutflowEvent("04_leading_pt>25");
  
  if ( SF && fabs( (leptons[0] + leptons[1]).M() - 91.2 ) < 20.) return;
  countCutflowEvent("05_Z_veto");
  
  double deltaphi_min = 100.;
  double deltaphi_temp = 1000.;
  for ( int i = 0; i < jets.size(); i++ ) {
    deltaphi_temp = fabs( jets[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
  }    
  if ( deltaphi_min < 1. ) return;
  countCutflowEvent("06_dphi_Etmiss_j>1");
  
  if ( missingET->P4().DeltaPhi( missingET->P4() + leptons[0] + leptons[1] ) > 1.5) return;
  countCutflowEvent("07_dphi_pllTb<1.5");
  
  if ( jets.size() < 2 ) return;
  countCutflowEvent("08_jets>2");
  
  bool btag = false; bool b0 = false;
  std::vector<Jet*> taggedbJets;
  std::vector<Jet*> untaggedbJets;
  for (int i = 0; i < jets.size(); i++) { 
      if ( checkBTag(jets[i],1) ) {
      if ( checkBTag(jets[i],0) ) {
          btag = true;
          if ( i == 0 ) b0 = true;
          taggedbJets.push_back( jets[i] );
      }
      else untaggedbJets.push_back( jets[i] );    
      }
  }
  if ( !btag ) return;
  countCutflowEvent("09_btag");
  
  std::vector<TLorentzVector> analysis_jets;
  if ( taggedbJets.size() > 1 ) {
      analysis_jets.push_back( taggedbJets[0]->P4() );
      analysis_jets.push_back( taggedbJets[1]->P4() );    
  }
  else if ( taggedbJets.size() == 1 && untaggedbJets.size() > 1) {
      if ( taggedbJets[0]->PT > untaggedbJets[0]->PT) {
      analysis_jets.push_back( taggedbJets[0]->P4() );
      analysis_jets.push_back( untaggedbJets[0]->P4() );
      }
      else {
      analysis_jets.push_back( untaggedbJets[0]->P4() );
      analysis_jets.push_back( taggedbJets[0]->P4() );        
      }
  }
  else {
      if ( b0 ) {
      analysis_jets.push_back( taggedbJets[0]->P4() );
      analysis_jets.push_back( jets[1]->P4() );       
      }
      else {
      analysis_jets.push_back( jets[1]->P4() );
      analysis_jets.push_back( taggedbJets[0]->P4() );       
      }
  }
  
  std::vector<TLorentzVector> combinations1;
  std::vector<TLorentzVector> combinations2;
  
  if ( (analysis_jets[0] + leptons[0]).M() < 180. && (analysis_jets[1] + leptons[1]).M() < 180. ) {
      combinations1.push_back( analysis_jets[0] + leptons[0] );
      combinations2.push_back( analysis_jets[1] + leptons[1] );
  }
  
  if ( (analysis_jets[0] + leptons[1]).M() < 180. && (analysis_jets[1] + leptons[0]).M() < 180. ) {
      combinations1.push_back( analysis_jets[0] + leptons[1] );
      combinations2.push_back( analysis_jets[1] + leptons[0] );
  }
  
  if (combinations1.size() == 0) return;
  countCutflowEvent("10_mjl<180");
  
  double m_t2_min = 1000.;
  for (int i = 0; i < combinations1.size(); i++) {
    double pa[3] = {combinations1[i].M(), combinations1[i].Px(), combinations1[i].Py() };
    double pb[3] = {combinations2[i].M(), combinations2[i].Px(), combinations2[i].Py() };
    double pmiss[3] = {0.,missingET->P4().Px(), missingET->P4().Py() };
    mt2_bisect::mt2 mt2_event;
    mt2_event.set_momenta( pa, pb, pmiss );
    mt2_event.set_mn( 0. );
    double m_t2 = mt2_event.get_mt2(); 
    if ( m_t2 < m_t2_min ) m_t2_min = m_t2;
  }
  
  if ( m_t2_min > 70. ) return;
  countCutflowEvent("11_mT2<70");
  
  if ( missingET->P4().Perp()/(missingET->P4().Perp() + leptons[0].Perp() + leptons[1].Perp() ) < 0.45 ) return;
  countCutflowEvent("12_Emiss/Emiss+leptons>0.45");
  
  double HT = jets[0]->PT + jets[1]->PT;
  double meff = HT + leptons[0].Perp() + leptons[1].Perp() + missingET->P4().Perp();
  if ( HT/meff > 0.4 ) return;
  countCutflowEvent("13_HT/meff<0.4");
  
  double deltax = 2.*( leptons[0].Pz() + leptons[1].Pz() )/8000.;
  //histo_dx->Fill( fabs(deltax) ) ;
  if ( fabs(deltax) > 0.04 ) return;
  countCutflowEvent("14_dx<0.04");

  //double etadiff = tanh(0.5*( leptons[0].Eta() - leptons[1].Eta() ));
  //histo_etadiff->Fill( etadiff );
  //histo_etasum->Fill( fabs(tanh(0.5*( leptons[0].Eta() + leptons[1].Eta() ))) );
  
  countSignalEvent("SRa");
  
}

void Atlas_conf_2014_014::finalize() {
  // Whatever should be done after the run goes here
  //TFile f1("2014_014.root","recreate");
  //histo_etasum->Write();
  //histo_etadiff->Write();
  //histo_dx->Write();
  //f1.Close();
}       
