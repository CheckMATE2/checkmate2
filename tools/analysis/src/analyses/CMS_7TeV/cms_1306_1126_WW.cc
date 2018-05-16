#include "cms_1306_1126_WW.h"

void Cms_1306_1126_ww::initialize() {
  setAnalysisName("cms_1306_1126_WW");          
  setInformation(""
    "@#CMS\n"
     "@#arXiv:1306.1126 (only WW measurement)\n"
     "@#WW standard model measurement\n"
     "@#sqrt(s) = 7 TeV\n"
     "@#int(L) = 4.92 fb^-1\n"
  "");
    // H->WW added (40 events according to CMS)
  // cryptic isolation criteria, used track isolation from 7 TeV paper
  // paragraph on Drell-Yan was unclear to me (which requirements are for the same flavour and opposite flavour) and I followed 7 TeV paper
  setLuminosity(4.92*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("combined;");
  bookCutflowRegions("dilepton;mll;mllZ;missETrel;jetveto;pTll;ee;mumu;emu;");
  // You should initialize any declared variables here
  /*
  histo_ptmax = new TH1D("lep0pt", "pt of the leading lepton", 33, 0 , 165);
  histo_ptmin = new TH1D("lep1pt", "pt of the trailing lepton", 14, 15 , 85);
  histo_ptll = new TH1D("ptll", "transverse momentum of ll", 17, 40 , 125);
  histo_mll = new TH1D("invll", "invariant mass of ll", 40, 0 , 200);
  histo_etadiff = new TH1D("etadiff", "rapidity distance between leptons", 5, 0 , 1);
  histo_etadiff->SetMinimum(0.);
  histo_etasum = new TH1D("etasum", "rapidity sum of leptons", 5, 0 , 1);
  histo_etasum->SetMinimum(0.);
  histo_rap2D = new TH2D("etaeta", "rapidity lep1 v lep2", 20, -2.5 , 2.5, 20, -2.5, 2.5);
  histo_sqrtsmin = new TH1D("sqrtsmin", "sqrtsmin", 30, 0 , 300);
  */
}

void Cms_1306_1126_ww::analyze() {
    // Your eventwise analysis code goes here
  missingET->addMuons(muonsCombined);
  
  jets = filterPhaseSpace(jets, 15., -4.7, 4.7);
  
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.5, 2.5);
  std::vector<Electron*> isoElecs = filterIsolation(electronsTight);
  
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);
  
  jets = overlapRemoval(jets, isoElecs, 0.3); 
  
  // Oppositely charged leptons with pT > 20 GeV (eta < 2.5 for electrons, eta < 2.4 for muons)
  // Veto events with b-tags (15-30GeV, eta <2.4)
  // Veto events with jets > 30GeV (eta < 5.0)
  // Uses projected EtMiss (think same as ATLAS)
  // Projected EtMiss > 45 GeV for same flavour pairs
  // Projected EtMiss > 20 GeV for different flavour pairs
  
  // For same flavour pairs:
  // Angle between hardest jet and lepton system < 165 degrees
  // Reject events if mass +- 15 GeV of Z
  // Reject events if mass < 20 GeV for ee/mumu or mass < 12 GeV for emu
  // Transverse momentum of pll > 45 GeV
  
  // Reject event if third lepton > 10 GeV
  
  if ( isoElecs.size() + isoMuons.size() != 2) return; 
  countCutflowEvent("dilepton");
  
  double mll, pTll;
  bool OSe = false; bool OSmu = false; bool OSemu = false;   
  TLorentzVector pll, leading, trailing;
  
  if (isoElecs.size() == 2 && isoElecs[0]->Charge*isoElecs[1]->Charge < 0 && isoElecs[1]->PT > 20.) 
  {
    mll = (isoElecs[0]->P4() + isoElecs[1]->P4()).M();
    pTll = (isoElecs[0]->P4() + isoElecs[1]->P4()).Perp();
    pll = isoElecs[0]->P4() + isoElecs[1]->P4();
    leading = isoElecs[0]->P4();
    trailing = isoElecs[1]->P4();
    OSe = true;
    countCutflowEvent("ee");
  }
  else if (isoMuons.size() == 2 && isoMuons[0]->Charge*isoMuons[1]->Charge < 0 && isoMuons[1]->PT > 20.) 
  {
    mll = (isoMuons[0]->P4() + isoMuons[1]->P4()).M();
    pTll = (isoMuons[0]->P4() + isoMuons[1]->P4()).Perp();
    pll = isoMuons[0]->P4() + isoMuons[1]->P4();
    leading = isoMuons[0]->P4();
    trailing = isoMuons[1]->P4();
    OSmu = true;
    countCutflowEvent("mumu");
  }
  else if (isoElecs.size() == 1 && isoMuons.size() == 1 && isoElecs[0]->Charge*isoMuons[0]->Charge < 0 && ( (isoElecs[0]->PT > 27. && isoMuons[0]->PT > 20.) || (isoMuons[0]->PT > 24. && isoElecs[0]->PT > 20.) ) )
  {
    mll = (isoElecs[0]->P4() + isoMuons[0]->P4()).M();
    pTll = (isoElecs[0]->P4() + isoMuons[0]->P4()).Perp();
    pll = isoElecs[0]->P4() + isoMuons[0]->P4();
    if ( isoElecs[0]->PT > isoMuons[0]->PT ) {
      leading = isoElecs[0]->P4();
      trailing = isoMuons[0]->P4();
    }
    else {
      leading = isoMuons[0]->P4();
      trailing = isoElecs[0]->P4();      
    }
    OSemu = true;
    countCutflowEvent("emu");    
  }
  else return;
  
    double deltaphi_min = 1000.;
  double deltaphi_temp = 10000.;
  double missingETrel;

  if (!OSemu && jets.size() > 0 && fabs(jets[0]->P4().DeltaPhi(pll)) > 165./180.*3.14159265359 ) return;
  
  if ( ( mll < 20. && ( OSmu + OSe ) != 0) || (mll < 12. && OSemu == 1) ) return;
  countCutflowEvent("mll");
  
  if ( !OSemu && fabs(mll - 91.2) < 15.) return;
  countCutflowEvent("mllZ");
  
// definition of Etmissrel from 7 TeV paper
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
  if ( (OSe || OSmu) && missingETrel < 45.) return;
  if ( OSemu && missingETrel < 20.) return;

  countCutflowEvent("missETrel");  
  
  if ( jets.size() > 0 && jets[0]->PT > 30.) return;
 
  for ( int i = 0; i < jets.size(); i++ ) {
    if ( (fabs(jets[i]->Eta) < 2.4) && (jets[i]->PT > 15.) && (checkBTag(jets[i])) )  return;
  }
  
  countCutflowEvent("jetveto");  
  
  if ( pTll < 45. ) return;
  countCutflowEvent("pTll");
  
  countSignalEvent("combined");
  /*
  histo_ptmax->Fill( leading.Perp() );
  histo_ptmin->Fill( trailing.Perp() );
  histo_ptll->Fill( pTll );
  histo_mll->Fill( mll );
  histo_rap2D->Fill( leading.Eta(), trailing.Eta() );
  histo_etasum->Fill( fabs(tanh(0.5*( leading.Eta() + trailing.Eta() ))) );
  */
  double sqrtsmin = sqrt( (leading.E() + trailing.E())*(leading.E() + trailing.E()) - (leading.Pz() + trailing.Pz())*(leading.Pz() + trailing.Pz()) ) + missingET->P4().Et() ;
  //histo_sqrtsmin->Fill( sqrtsmin );
  //if (sqrtsmin > 150. ) histo_etadiff->Fill( fabs(tanh(0.5*( leading.Eta() - trailing.Eta() ))) );
  
  
}
  
void Cms_1306_1126_ww::finalize() {
  // Whatever should be done after the run goes here
  /* Daniel comments this out and points out that these things should not appear in analyses!
  TFile f1("1306_1126.root","recreate");
  histo_ptmax->Write();
  histo_ptmin->Write();
  histo_ptll->Write();
  histo_mll->Write();
  histo_rap2D->Write();
  histo_etasum->Write();
  histo_sqrtsmin->Write();
  histo_etadiff->Write();
  f1.Close();
  */
}       
