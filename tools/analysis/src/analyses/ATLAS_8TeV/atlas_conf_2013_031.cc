#include "atlas_conf_2013_031.h"

void Atlas_conf_2013_031::initialize() {
  setAnalysisName("atlas_conf_2013_031");          
  setInformation(""
    "@#Higgs spin, 8 TeV\n"
  "");
  setLuminosity(20.7*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("Higgs;WWCR;");
  bookCutflowRegions("jet_veto;pTll;mll;dphill;");
  // You should initialize any declared variables here
}

void Atlas_conf_2013_031::analyze() {
  // Your eventwise analysis code goes here
  missingET->addMuons(muonsCombined);
  
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  
  electronsTight = filterPhaseSpace(electronsTight, 15, -2.47, 2.47, true);
  std::vector<Electron*> isoElecs = filterIsolation(electronsTight);
  
  muonsCombined = filterPhaseSpace(muonsCombined, 15, -2.5, 2.5);
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);
 
  jets = overlapRemoval(jets, isoElecs, 0.3);  
  
  double mll, pTll;
  
  std::vector<TLorentzVector> leptons; 
  
  if (isoElecs.size() != 1 || isoMuons.size() != 1) return;
  if (isoElecs[0]->Charge * isoMuons[0]->Charge > 0) return;
  
  bool trig = false;
  if (isoElecs[0]->PT > 24. && rand()/static_cast<double>(RAND_MAX) < 0.90 ) trig = true;
  else if ( isoMuons[0]->PT > 24. && fabs(isoMuons[0]->Eta) < 2.4 && fabs(isoMuons[0]->Eta) > 1.1 && rand()/static_cast<double>(RAND_MAX) < 0.90 ) trig = true;
  else if (isoMuons[0]->PT > 24. && fabs(isoMuons[0]->Eta) < 1.1 && rand()/static_cast<double>(RAND_MAX) < 0.70 ) trig = true;
  
  if (!trig) return;
  
  if (isoElecs[0]->PT > 25. || isoMuons[0]->PT > 25.) 
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
  }
  else return;
  
  if ( mll < 10. ) return;
  
  for ( int i = 0; i < jets.size(); i++ ) {
    if ( fabs(jets[i]->Eta) < 2.5 && jets[i]->PT > 25. ) return;
    if ( 2.5 < fabs(jets[i]->Eta) && fabs(jets[i]->Eta) < 4.5 && jets[i]->PT > 30. ) return;
  }
  
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
  for ( int i = 0; i < isoElecs.size(); i++ ) {
    deltaphi_temp = fabs( isoElecs[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
  }
  
  if ( deltaphi_min < 1.57079632679 )
    missingETrel = missingET->P4().Et()*sin(deltaphi_min);
  else
    missingETrel = missingET->P4().Et();
 
  if ( missingETrel < 20.) return;  
  
  countCutflowEvent("jet_veto");
  
  if (pTll < 20. ) return;
  countCutflowEvent("pTll");
  
  if ( mll > 80. ) countSignalEvent("WWCR");
  else {
    countCutflowEvent("mll");
    if ( isoElecs[0]->P4().DeltaPhi(isoMuons[0]->P4()) < 2.8 ) {
      countCutflowEvent("dphill");
      countSignalEvent("Higgs");     
    }
  }
    
}

void Atlas_conf_2013_031::finalize() {
  // Whatever should be done after the run goes here
}       
