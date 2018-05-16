#include "atlas_1210_2979.h"

void Atlas_1210_2979::initialize() {
  setAnalysisName("atlas_1210_2979");          
  setInformation(""
    "@#WW production, 7 TeV, 4.6/fb\n"
  "");
  setLuminosity(4.6*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("combined;");
  bookCutflowRegions("dilepton;mll;mllZ;missETrel;jetveto;pTll");
  // You should initialize any declared variables here
  hist_lep_pt = new TH1F("lep_pt", "Lepton_pT", 27, 25., 160.);
  hist_delta_phi = new TH1F("delta_phi", "DeltaPhi_leplep", 20, 0., 3.2);
  hist_lletmiss_pt = new TH1F("lletmiss_pt", "pT_lletmiss", 11, 0., 110.);
  hist_lletmiss_mT = new TH1F("lletmiss_mT", "mT_lletmiss", 15, 60., 360.);

}

void Atlas_1210_2979::analyze() {
  // Your eventwise analysis code goes here
  missingET->addMuons(muonsCombined);
  
  jets = filterPhaseSpace(jets, 25., -4.5, 4.5);
  
  electronsTight = filterPhaseSpace(electronsTight, 22, -2.47, 2.47, true);
  std::vector<Electron*> isoElecs = filterIsolation(electronsTight);
  
  muonsCombined = filterPhaseSpace(muonsCombined, 20, -2.4, 2.4);
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);
 
  jets = overlapRemoval(jets, isoElecs, 0.3);
  
  double mll, pTll;
  bool OSe = false; bool OSmu = false; bool OSemu = false; 
  
  std::vector<TLorentzVector> leptons; 
  
  if (isoElecs.size() + isoMuons.size() != 2) return;
  if (isoElecs.size() == 2 && isoElecs[0]->Charge*isoElecs[1]->Charge < 0 && isoElecs[0]->PT > 25.) 
  {
    mll = (isoElecs[0]->P4() + isoElecs[1]->P4()).M();
    pTll = (isoElecs[0]->P4() + isoElecs[1]->P4()).Perp();
    leptons.push_back(isoElecs[0]->P4());
    leptons.push_back(isoElecs[1]->P4());
    OSe = true;
  }
  else if (isoMuons.size() == 2 && isoMuons[0]->Charge*isoMuons[1]->Charge < 0 && isoMuons[0]->PT > 25.) 
  {
    mll = (isoMuons[0]->P4() + isoMuons[1]->P4()).M();
    pTll = (isoMuons[0]->P4() + isoMuons[1]->P4()).Perp();
    leptons.push_back(isoMuons[0]->P4());
    leptons.push_back(isoMuons[1]->P4());
    OSmu = true;
  }
  else if (isoElecs.size() == 1 && isoMuons.size() == 1 && isoElecs[0]->Charge*isoMuons[0]->Charge < 0 && (isoElecs[0]->PT > 25. || isoMuons[0]->PT > 25.) )
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
    OSemu = true;
  }
  else return;
  countCutflowEvent("dilepton"); 
  
  if ( (OSe || OSmu) && mll < 15. ) return;
  if ( OSemu && mll < 10. ) return;
  countCutflowEvent("mll");
  
  if ( (OSe || OSmu) && fabs(mll - 91.2) < 15. ) return;
  countCutflowEvent("mllZ");
  
  
    // Calculation of etmiss,rel    
  double deltaphi_min = 1000.;
  double deltaphi_temp = 10000.;
  double missingETrel;
  
  for ( int i = 0; i < jets.size(); i++ ) {
    deltaphi_temp = fabs( jets[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
  }
  for ( int i = 0; i < isoMuons.size(); i++ ) {
    deltaphi_temp = fabs( isoMuons[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
  }
  for ( int i = 0; i < electronsTight.size(); i++ ) {
    deltaphi_temp = fabs( electronsTight[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
  }
  if ( deltaphi_min < 1.57079632679 )
    missingETrel = missingET->P4().Et()*sin(deltaphi_min);
  else
    missingETrel = missingET->P4().Et();
  if ( (OSe || OSmu) && missingETrel < 45.) return;
  if ( OSemu && missingETrel < 25.) return;
  countCutflowEvent("missETrel");
  
  if (jets.size() > 0) return;
  countCutflowEvent("jetveto");
  
  if (pTll < 30.) return;
  countCutflowEvent("pTll");
  
  countSignalEvent("combined");
  
  // Fill histograms
  // pT (leading lepton)
  hist_lep_pt->Fill(leptons[0].Pt(), weight);
  
  // delta_phi between leptons
  double delta_phi = leptons[0].DeltaPhi(leptons[1]);
  hist_delta_phi->Fill(delta_phi, weight);
  
  // pT (lep lep etmiss)
  double pT_lletmiss = (leptons[0] + leptons[1] + missingET->P4()).Pt();
  hist_lletmiss_pt->Fill(pT_lletmiss, weight);
  
  // mT (lep lep etmiss)
  double ET_Tot = leptons[0].Et() + leptons[1].Et() + missingET->P4().Pt();
  double pt_x = leptons[0].Px() + leptons[1].Px() + missingET->P4().Px();
  double pt_y = leptons[0].Py() + leptons[1].Py() + missingET->P4().Py();
  double mT = sqrt(ET_Tot*ET_Tot - (pt_x*pt_x + pt_y*pt_y));
  hist_lletmiss_mT->Fill(mT, weight);

}

void Atlas_1210_2979::finalize() {
  // Whatever should be done after the run goes here
  outputHistFile(hist_lep_pt);
  outputHistFile(hist_delta_phi);
  outputHistFile(hist_lletmiss_pt);
  outputHistFile(hist_lletmiss_mT);
}    

void Atlas_1210_2979::outputHistFile(TH1F* inHist) {
    // Assemble absolute filename
    std::string filename = "results/hist/"+std::string(inHist->GetTitle())+".dat";
    // Open stream
    std::ofstream* file = new ofstream(filename.c_str());
    // Write standard information
    *file << "# " << inHist->GetTitle() << " \n";
    *file << "#  Bin    MC_Events    (Error)    Normalised_Events    (Error)    Histogram_Normalised_Events    (Error)    Bin_Width    \n";
    for ( int i=1; i<=inHist->GetNbinsX(); i++)
      *file << inHist->GetBinCenter(i) << "    " << inHist->GetBinContent(i) << "    " << inHist->GetBinError(i) << "    " << normalize(inHist->GetBinContent(i)) << "    " << normalize(inHist->GetBinError(i)) << "    " << (inHist->GetBinContent(i))/(inHist->Integral()) << "    " << (inHist->GetBinError(i))/(inHist->Integral()) << "    " << inHist->GetBinWidth(i) << std::endl;
    
    file->close();
}
