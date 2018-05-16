#include "atlas_1403_5294.h"

void Atlas_1403_5294::initialize() {
  setAnalysisName("atlas_1403_5294");          
  setInformation(""
    "@#direct production of charginos, neutralinos and sleptons\n"
     "@#two leptons and missing transverse momentum\n"
     "@#8 TeV, 20.3 fb^-1\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookCutflowRegions("ee_dilepton;ee_jet_veto;ee_Z_veto;ee_mT2_90;ee_mT2_120;ee_mT2_150;ee_mT2_100;ee_mll_170;ee_ptll_80;ee_mll_120;");
  bookCutflowRegions("mumu_dilepton;mumu_jet_veto;mumu_Z_veto;mumu_mT2_90;mumu_mT2_120;emu_mT2_150;mumu_mT2_100;mumu_mll_170;mumu_ptll_80;mumu_mll_120;");
  bookCutflowRegions("emu_dilepton;emu_jet_veto;emu_Z_veto;emu_mT2_90;emu_mT2_120;emu_mT2_150;emu_mT2_100;emu_mll_170;emu_ptll_80;emu_mll_120;emu_Etmiss;");
  bookCutflowRegions("ee_jets;ee_b_veto;ee_Z_window;ee_pTll;ee_Etmiss;ee_DRll;ee_mjj;ee_jet_pT;");
  bookCutflowRegions("mumu_jets;mumu_b_veto;mumu_Z_window;mumu_pTll;mumu_Etmiss;mumu_DRll;mumu_mjj;mumu_jet_pT;");
  bookSignalRegions("mT2_90_SF;mT2_90_DF;mT2_120_SF;mT2_120_DF;mT2_150_SF;mT2_150_DF;WWa_SF;WWa_DF;WWb_SF;WWb_DF;WWc_SF;WWc_DF;Zjets;");
  // You should initialize any declared variables here
}

void Atlas_1403_5294::analyze() {
  // Your eventwise analysis code goes here
  missingET->addMuons(muonsCombined); 
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 17., -4.5, 4.5);
  
  electronsTight = overlapRemoval(electronsTight, 0.05);
  jets = overlapRemoval(jets, electronsTight, 0.2);
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  
// skipped removing two electrons/muons if DR < 0.05
  
  std::vector<Electron*> isoElecs = filterIsolation(electronsTight);
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);
  
  bool ee = false; bool mumu = false; bool emu = false;
  std::string flavour;
  std::vector<TLorentzVector> leptons;
  if (isoElecs.size() + isoMuons.size() != 2) return;
  if (isoElecs.size() == 2) {
    ee = true;
    flavour = "ee";
    leptons.push_back( isoElecs[0]->P4() );
    leptons.push_back( isoElecs[1]->P4() );
  }
  else if (isoMuons.size() == 2) {
    mumu = true;
    flavour = "mumu";
    leptons.push_back( isoMuons[0]->P4() );
    leptons.push_back( isoMuons[1]->P4() );
  }
  else {
    emu = true;
    flavour = "emu";
    if ( isoElecs[0]->PT > isoMuons[0]->PT ) {
      leptons.push_back( isoElecs[0]->P4() );
      leptons.push_back( isoMuons[0]->P4() );
    }
    else {
      leptons.push_back( isoMuons[0]->P4() );
      leptons.push_back( isoElecs[0]->P4() );
    }
  }
  
  TLorentzVector pll = leptons[0] + leptons[1];
  
  double mll = pll.M();
  if ( leptons[0].Perp() < 35. || leptons[1].Perp() < 20. || mll < 20.) return;
  
  double triggerRatio = (double) rand() / (double) (RAND_MAX + 1.);
  if (ee && triggerRatio > 0.90 ) return;
  if (mumu && triggerRatio > 0.80 ) return;
  if (emu && triggerRatio > 0.75 ) return;
  
  countCutflowEvent(flavour+"_dilepton");
  
  bool btag = false; bool centraljet = false; bool forwardjet = false;
  std::vector<TLorentzVector> centraljets;
  for (int i = 0; i < jets.size(); i++) {
    if ( (fabs(jets[i]->Eta) < 2.4) && (jets[i]->PT > 20.) ){
      if (checkBTag(jets[i]) ) btag = true;
      else {
        centraljet = true;
        centraljets.push_back(jets[i]->P4() );
      }
    }
    else if (jets[i]->PT > 30. ) forwardjet = true;  
  }
  
  double deltaphi_min = 1000.;
  double deltaphi_temp = 10000.;
  double missingETrel;
  
  for ( int i = 0; i < jets.size(); i++ ) {
    if ( (fabs(jets[i]->Eta) < 2.4) &&  (jets[i]->PT > 20.) ) {
      deltaphi_temp = fabs( jets[i]->P4().DeltaPhi(missingET->P4()));
      if ( deltaphi_temp < deltaphi_min )
        deltaphi_min = deltaphi_temp;
    }
  }
  for ( int i = 0; i < isoMuons.size(); i++ ) {
    deltaphi_temp = fabs( isoMuons[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
  }
  for ( int i = 0; i < isoElecs.size(); i++ ) {
    deltaphi_temp = fabs( isoElecs[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
  }
  if ( deltaphi_min < 1.57079632679 )
    missingETrel = missingET->P4().Et()*sin(deltaphi_min);
  else
    missingETrel = missingET->P4().Et();
  
  
  if ( !btag && !centraljet && !forwardjet) {
    countCutflowEvent(flavour+"_jet_veto");
    
    if ( !emu && fabs(mll - 91.2) < 10. ) return;
    countCutflowEvent(flavour+"_Z_veto");  
    
    double pa[3] = {leptons[0].M(), leptons[0].Px(), leptons[0].Py() };
    double pb[3] = {leptons[1].M(), leptons[1].Px(), leptons[1].Py() };
    double pmiss[3] = {0.,missingET->P4().Px(), missingET->P4().Py() };
    mt2_bisect::mt2 mt2_event;
    mt2_event.set_momenta( pa, pb, pmiss );
    mt2_event.set_mn( 0. );
    double m_t2 = mt2_event.get_mt2(); 
    if ( m_t2 > 90. ) {
      countCutflowEvent(flavour+"_mT2_90");
      if (ee || mumu ) countSignalEvent("mT2_90_SF");
      else countSignalEvent("mT2_90_DF");
      
      if ( mll < 170.) {
    countCutflowEvent(flavour+"_mll_170");
    if (ee || mumu ) countSignalEvent("WWb_SF");
        else countSignalEvent("WWb_DF");
      }
    }  
    
    if ( m_t2 > 100. ) {
      countCutflowEvent(flavour+"_mT2_100");
      if (ee || mumu ) countSignalEvent("WWc_SF");
      else countSignalEvent("WWc_DF");
    }  
   
    if ( m_t2 > 120. ) {
      countCutflowEvent(flavour+"_mT2_120");
      if (ee || mumu ) countSignalEvent("mT2_120_SF");
      else countSignalEvent("mT2_120_DF");
    }  
    
    if ( m_t2 > 150. ) {
      countCutflowEvent(flavour+"_mT2_150");
      if (ee || mumu ) countSignalEvent("mT2_150_SF");
      else countSignalEvent("mT2_150_DF");
    }      
    
    if ( pll.Perp() < 80. ) return;
    countCutflowEvent(flavour+"_pTll");
    
    if (missingETrel < 80. ) return;
    countCutflowEvent(flavour+"_Etmiss");
    
    if ( mll > 120.) return;
    countCutflowEvent(flavour+"_mll_120");
    if (ee || mumu ) countSignalEvent("WWa_SF");
    else countSignalEvent("WWa_DF");
    
  }
  else if ( centraljets.size() > 1 && (ee || mumu) ) {
    countCutflowEvent(flavour+"_jets");
    
    if ( btag || forwardjet ) return;
    countCutflowEvent(flavour+"_b_veto");

    if ( fabs(mll - 91.2) > 10. ) return;
    countCutflowEvent(flavour+"_Z_window");  
    
    if ( pll.Perp() < 80. ) return;
    countCutflowEvent(flavour+"_pTll");
    
    if (missingETrel < 80. ) return;
    countCutflowEvent(flavour+"_Etmiss");
    
    if ( leptons[0].DeltaR(leptons[1]) < 0.3 || leptons[0].DeltaR(leptons[1]) > 1.5 ) return;
    countCutflowEvent(flavour+"_DRll");
    
    double mjj = (centraljets[0] + centraljets[1]).M();
    if ( mjj < 50. || mjj > 100. ) return;
    countCutflowEvent(flavour+"_mjj");
    
    if ( centraljets[1].Perp() < 45. ) return;
    countCutflowEvent(flavour+"_jet_pT");
    
    countSignalEvent("Zjets");
  }
  else return;
}

void Atlas_1403_5294::finalize() {
  // Whatever should be done after the run goes here
}       
