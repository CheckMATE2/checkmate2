#include "atlas_conf_2014_033.h"

void Atlas_conf_2014_033::initialize() {
  setAnalysisName("atlas_conf_2014_033");          
  setInformation(""
    "@#WW di-lepton + met\n"
     "@#8 TeV 20/fb by Atlas\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
//  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("ee;mumu;emu;");
  bookCutflowRegions("00_ID;01_mll;02_mllZ;03_missETrel;04_ptmiss;05_dphi;06_jetveto;");
  // You should initialize any declared variables here
/*  histo_ptmax = new TH1D("lep0pt", "pt of the leading lepton", 23, 0 , 140);
  histo_ptmin = new TH1D("lep1pt", "pt of the trailing lepton", 16, 20 , 100);
  histo_ptll = new TH1D("ptll", "transverse momentum of ll", 24, 0 , 120);
  histo_mll = new TH1D("invll", "invariant mass of ll", 29, 10 , 300);
  histo_phill = new TH1D("phill", "delta phi between leptons", 20, 0 , 3.1416);
  histo_mTllmiss = new TH1D("mTllmiss", "mT of ll and ETmiss", 23, 70 ,300);
  histo_pTllmiss = new TH1D("pTllmiss", "pT of ll and ETmiss", 14, 0 ,70);
  histo_etadiff = new TH1D("etadiff", "rapidity distance between leptons", 20, -1 , 1);
  histo_etadiff->SetMinimum(0.);
  histo_etasum = new TH1D("etasum", "rapidity sum of leptons", 5, 0 , 1);
  histo_etasum->SetMinimum(0.);
  histo_rap2D = new TH2D("etaeta", "rapidity lep1 v lep2", 20, -2.5 , 2.5, 20, -2.5, 2.5);
  histo_pT2D = new TH2D("pTpT", "pT of lep1 v lep2", 20, 0 , 100, 20, 0, 100);
  histo_sqrtsmin = new TH1D("sqrtsmin", "sqrtsmin", 30, 0 , 300);
  histo_dx = new TH1D("dx", "dx by Melia", 10, 0 , 0.1);
  histo_dx->SetMinimum(0.);  
  histo_njet = new TH1D("njet", "number of jet after all selections", 6, 0 , 5);
  histo_njet->SetMinimum(0.);
  histo_l1tol0 = new TH1D("l1tol0", "leptons pT ratio", 20, 0 , 1);
  histo_l1tol0->SetMinimum(0.);
  */
  asy1p = 0; asy1m = 0; asy2p = 0; asy2m = 0;  
  
}

void Atlas_conf_2014_033::analyze() {
  
  missingET->addMuons(muonsCombined);
  
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  
  electronsTight = filterPhaseSpace(electronsTight, 7, -2.47, 2.47, true);
  std::vector<Electron*> isoElecs = filterIsolation(electronsTight);
  
  std::vector<Muon*> muonsLow = filterPhaseSpace(muonsCombined, 7, -2.4, 2.4);
  std::vector<Muon*> muonsHigh = filterPhaseSpace(muonsCombined, 25, -2.4, 2.4);
  std::vector<Muon*> isoMuons = filterIsolation(muonsHigh,0);
  std::vector<Muon*> isoMuonsLow = filterIsolation(muonsLow,1);
  
  for (int i = 0; i < isoMuonsLow.size(); i++) {
      if ( isoMuonsLow[i]->PT < 25. ) isoMuons.push_back(isoMuonsLow[i]); 
  }  
 
  jets = overlapRemoval(jets, isoElecs, 0.3);
  
  if (isoElecs.size() + isoMuons.size() != 2) return; // exactly 2 leptons
  
  double mll, pTll;
  bool OSe = false; bool OSmu = false; bool OSemu = false; 
  
  std::vector<TLorentzVector> leptons; 
    
  if (isoElecs.size() == 2 && isoElecs[0]->Charge*isoElecs[1]->Charge < 0 && isoElecs[0]->PT > 25. && isoElecs[1]->PT > 20. ) 
  {
    mll = (isoElecs[0]->P4() + isoElecs[1]->P4()).M();
    pTll = (isoElecs[0]->P4() + isoElecs[1]->P4()).Perp();
    leptons.push_back(isoElecs[0]->P4());
    leptons.push_back(isoElecs[1]->P4());
    OSe = true;
  }
  else if (isoMuons.size() == 2 && isoMuons[0]->Charge*isoMuons[1]->Charge < 0 && isoMuons[0]->PT > 25. && isoMuons[1]->PT > 20.) 
  {
    mll = (isoMuons[0]->P4() + isoMuons[1]->P4()).M();
    pTll = (isoMuons[0]->P4() + isoMuons[1]->P4()).Perp();
    leptons.push_back(isoMuons[0]->P4());
    leptons.push_back(isoMuons[1]->P4());
    OSmu = true;
  }
  else if (isoElecs.size() == 1 && isoMuons.size() == 1 && isoElecs[0]->Charge*isoMuons[0]->Charge < 0 
              && (isoElecs[0]->PT > 25. || isoMuons[0]->PT > 25.) )
  {
    mll = (isoElecs[0]->P4() + isoMuons[0]->P4()).M();
    pTll = (isoElecs[0]->P4() + isoMuons[0]->P4()).Perp();
    if(isoElecs[0]->PT > isoMuons[0]->PT)
    {
      leptons.push_back(isoElecs[0]->P4());
      leptons.push_back(isoMuons[0]->P4());
    }
    else
    {
      leptons.push_back(isoMuons[0]->P4());
      leptons.push_back(isoElecs[0]->P4());
    }
    if (leptons[1].Perp() < 20.) return;
    OSemu = true;
  }
  else return;
  countCutflowEvent("00_ID");
  
  if ( (OSe || OSmu) && mll < 15. ) return;
  if ( OSemu && mll < 10. ) return;
  countCutflowEvent("01_mll");  
  
  if ( (OSe || OSmu) && fabs(mll - 91.2) < 15. ) return;
  countCutflowEvent("02_mllZ");
  
    // Calculation of etmiss,rel    
  double deltaphi_min = 1000.;
  double deltaphi_temp = 10000.;
  double missingETrel;
  
  for ( int i = 0; i < jets.size() && jets[i]->PT > 25. ; i++ ) {
    deltaphi_temp = fabs( jets[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
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
  if ( (OSe || OSmu) && missingETrel < 45.) return;
  if ( OSemu && missingETrel < 15.) return;
  countCutflowEvent("03_missETrel");  
  
  double trackID = (double) rand() / (double) (RAND_MAX + 1.);
  tracks = filterPhaseSpace(tracks, 0.5, -2.5, 2.5); 
  TLorentzVector trackMET;    
  for (int i = 0; i < tracks.size(); i++) {
    if ( (tracks[i]->PT > 0.5) && (tracks[i]->PT < 20.) && ( fabs(tracks[i]->Eta) < 2.5) && trackID < 0.7)  
      trackMET -= tracks[i]->P4(); // don't want signal elecs to be random here
  }  
  for (int i = 0; i < muonsCombined.size(); i++) trackMET -= muonsCombined[i]->P4();
  for (int i = 0; i < isoElecs.size(); i++) trackMET -= isoElecs[i]->P4();
  
  if ( (OSe || OSmu) && trackMET.Perp() < 45.) return;
  if ( OSemu && trackMET.Perp() < 20.) return;
  countCutflowEvent("04_ptmiss");  
  
  if ( (OSe || OSmu) && trackMET.DeltaPhi(missingET->P4()) > 0.3) return;
  if ( OSemu && trackMET.DeltaPhi(missingET->P4()) > 0.6) return;
  countCutflowEvent("05_dphi");    
  
  //histo_njet->Fill( jets.size() );
  
  if (jets.size() > 0 && jets[0]->PT > 25. ) return;
  countCutflowEvent("06_jetveto");
  
  if (OSe) countSignalEvent("ee");
  if (OSmu) countSignalEvent("mumu");
  if (OSemu) countSignalEvent("emu");
  /*
  double ET_Tot = leptons[0].Et() + leptons[1].Et() + missingET->P4().Pt();
  double pt_x = leptons[0].Px() + leptons[1].Px() + missingET->P4().Px();
  double pt_y = leptons[0].Py() + leptons[1].Py() + missingET->P4().Py();
  double mT = sqrt(ET_Tot*ET_Tot - (pt_x*pt_x + pt_y*pt_y));
  
  histo_ptmax->Fill( leptons[0].Perp() );
  histo_ptmin->Fill( leptons[1].Perp() );
  histo_ptll->Fill( pTll );
  histo_mll->Fill( mll );
  histo_phill->Fill( fabs(leptons[0].DeltaPhi(leptons[1])) );
  histo_mTllmiss->Fill( mT );
  histo_pTllmiss->Fill(  (leptons[0] +  leptons[1] + missingET->P4()).Perp() );
  histo_rap2D->Fill( leptons[0].Eta(), leptons[1].Eta() );
  histo_pT2D->Fill( leptons[0].Perp(), leptons[1].Perp() );
  histo_etasum->Fill( fabs(tanh(0.5*( leptons[0].Eta() + leptons[1].Eta() ))) );
  
  double sqrtsmin = sqrt( (leptons[0].E() + leptons[1].E())*(leptons[0].E() + leptons[1].E()) - (leptons[0].Pz() + leptons[1].Pz())*(leptons[0].Pz() + leptons[1].Pz()) ) + missingET->P4().Et() ;
  histo_sqrtsmin->Fill( sqrtsmin );
  
  double etadiff = tanh(0.5*( leptons[0].Eta() - leptons[1].Eta() ));
  if (sqrtsmin > 150. ) {
    histo_etadiff->Fill( (tanh(0.5*( leptons[0].Eta() - leptons[1].Eta() ))) );
    if ( fabs(etadiff) > 0.5 ) asy2p++; else asy2m++;
  }
  if ( fabs(etadiff) > 0.5 ) asy1p++; else asy1m++;
  
  double deltax = 2.*( leptons[0].Pz() + leptons[1].Pz() )/8000.;
  histo_dx->Fill( fabs(deltax) ) ;    
  
  histo_l1tol0->Fill( leptons[1].Perp()/leptons[0].Perp() );
  */
}

void Atlas_conf_2014_033::finalize() {
  // Whatever should be done after the run goes here
/*  TFile f1("2014_033.root","recreate");
  histo_ptmax->Write();
  histo_ptmin->Write();
  histo_ptll->Write();
  histo_mll->Write();
  histo_phill->Write();
  histo_mTllmiss->Write();
  histo_pTllmiss->Write();
  histo_rap2D->Write();
  histo_pT2D->Write();
  histo_etasum->Write();
  histo_sqrtsmin->Write();
  histo_etadiff->Write();
  histo_dx->Write();
  histo_njet->Write();
  histo_l1tol0->Write();
  f1.Close();
  
  std::cout <<  "# Asymmetry \n";  
  std::cout << "all: " << 1.*(asy1p - asy1m)/(asy1p + asy1m) << "   " <<  asy1p + asy1m << std::endl;
  std::cout << "high: " << 1.*(asy2p - asy2m)/(asy2p + asy2m) << "   " <<  asy2p + asy2m << std::endl;
  
  outputHistFile(histo_ptmax);
  outputHistFile(histo_ptmin);
  outputHistFile(histo_ptll);
  outputHistFile(histo_mll);  
  outputHistFile(histo_phill);
  outputHistFile(histo_mTllmiss);
  outputHistFile(histo_pTllmiss);
  */
}       

void Atlas_conf_2014_033::outputHistFile(TH1D* inHist) {
    // Write standard information    
    std::cout << "# " << inHist->GetTitle() << " \n";
    std::cout << "#  Bin    MC_Events    (Error)    Normalised_Events    (Error)    Histogram_Normalised_Events    (Error)    Bin_Width    \n";
    for ( int i=1; i<=inHist->GetNbinsX(); i++)
      std::cout << inHist->GetName() << "_" << i << "   " << inHist->GetBinCenter(i) << "    " << inHist->GetBinContent(i) << "    " << inHist->GetBinError(i) << "    " << normalize(inHist->GetBinContent(i)) << "    " << normalize(inHist->GetBinError(i)) << "    " << (inHist->GetBinContent(i))/(inHist->Integral()) << "    " << (inHist->GetBinError(i))/(inHist->Integral()) << "    " << inHist->GetBinWidth(i) << std::endl;
    
}