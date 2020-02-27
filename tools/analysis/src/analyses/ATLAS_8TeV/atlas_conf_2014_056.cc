#include "atlas_conf_2014_056.h"

void Atlas_conf_2014_056::initialize() {
  setAnalysisName("atlas_conf_2014_056");          
  setInformation(""
    "@#distribution of dphi comapred between pure SM and stop decaying to top and massless LSP\n"
     "@#di-lepton with at least two jets and at least one b-jet\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("sig;ee;mumu;emu;");
  bookCutflowRegions("ee;mumu;emu;");
  // You should initialize any declared variables here
  /*
  histo_phill = new TH1D("phill", "delta phi between leptons", 10, 0 , 3.1416);
  histo_phill->SetMinimum(0.);  
  histo_etadiff = new TH1D("etadiff", "rapidity distance between leptons", 10, -1 , 1);
  histo_etadiff->SetMinimum(0.);
  */
}

void Atlas_conf_2014_056::analyze() {
  // Your eventwise analysis code goes here
  
  missingET->addMuons(muonsCombined);
  
  jets = filterPhaseSpace(jets, 25., -2.5, 2.5);
  
  electronsMedium = filterPhaseSpace(electronsMedium, 25., -2.47, 2.47, true);
  std::vector<Electron*> isoElecs = filterIsolation(electronsMedium);
  
  muonsCombined = filterPhaseSpace(muonsCombined, 25., -2.5, 2.5);
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);
  
  jets = overlapRemoval(jets, isoElecs, 0.3);
  
  if (isoElecs.size() + isoMuons.size() != 2) return; 
  
  double mll, pTll;
  bool OSe = false; bool OSmu = false; bool OSemu = false;   
  
  std::vector<TLorentzVector> leptons; 
  
  if (isoElecs.size() == 2 && isoElecs[0]->Charge*isoElecs[1]->Charge < 0) 
  {
    mll = (isoElecs[0]->P4() + isoElecs[1]->P4()).M();
    pTll = (isoElecs[0]->P4() + isoElecs[1]->P4()).Perp();
    leptons.push_back(isoElecs[0]->P4());
    leptons.push_back(isoElecs[1]->P4());
    OSe = true;
  }
  else if (isoMuons.size() == 2 && isoMuons[0]->Charge*isoMuons[1]->Charge < 0 ) 
  {
    mll = (isoMuons[0]->P4() + isoMuons[1]->P4()).M();
    pTll = (isoMuons[0]->P4() + isoMuons[1]->P4()).Perp();
    leptons.push_back(isoMuons[0]->P4());
    leptons.push_back(isoMuons[1]->P4());
    OSmu = true;
  }
  else if (isoElecs.size() == 1 && isoMuons.size() == 1 && isoElecs[0]->Charge*isoMuons[0]->Charge < 0  )
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
  
  if ( (OSe || OSmu) && mll < 15. ) return;
  if ( (OSe || OSmu) && fabs(mll - 91.) < 10. ) return;
  if ( (OSe || OSmu) && missingET->P4().Et() < 30. ) return;
  
  if ( jets.size() < 2 ) return;
  
  bool btag = false;
  for (int i = 0; i < jets.size(); i++) 
    if ( checkBTag(jets[i]) )   btag = true;
  if (!btag) return;  
  
  double sumpt = leptons[0].Perp() + leptons[1].Perp();
  for (int i = 0; i < jets.size(); i++) 
    sumpt += jets[i]->PT;
  
  if ( OSemu && sumpt < 130. ) return;
  
  countSignalEvent("sig");
  if ( OSe ) countCutflowEvent("ee");
  if ( OSmu ) countCutflowEvent("mumu");
  if ( OSemu ) countCutflowEvent("emu");
  
  //histo_phill->Fill( fabs(leptons[0].DeltaPhi(leptons[1])) );
  double etadiff = tanh(0.5*( leptons[0].Eta() - leptons[1].Eta() ));
  //histo_etadiff->Fill( (tanh(0.5*( leptons[0].Eta() - leptons[1].Eta() ))) );
 
}

void Atlas_conf_2014_056::finalize() {
  // Whatever should be done after the run goes here
  /* Daniel comments this out
  int rootFileIndex = bookFile("2014_033.root");
  const char *rootFileName = fNames[rootFileIndex].c_str() ; 
  TFile f1(rootFileName, "recreate");
//  TFile f1("2014_056.root","recreate");
  histo_phill->Write();
  histo_etadiff->Write();
  f1.Close();
  */
}       
