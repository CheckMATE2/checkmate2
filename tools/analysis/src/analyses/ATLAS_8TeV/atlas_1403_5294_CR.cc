#include "atlas_1403_5294.h"

void Atlas_1403_5294_CR::initialize() {
  setInformation(""
    "@#direct production of charginos, neutralinos and sleptons\n"
     "@#two leptons and missing transverse momentum\n"
     "@#8 TeV, 20.3 fb^-1\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  setAnalysisName("atlas_1403_5294_CR");    
  ignore("towers");
  ignore("tracks");
  bookSignalRegions("CRmT2_top;CRWWa_top;CRmT2_WW;CRWWa_WW;");
  // You should initialize any declared variables here
}

void Atlas_1403_5294_CR::analyze() {
  // Your eventwise analysis code goes here
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
  if (isoElecs.size() != 1 || isoMuons.size() != 1) return;
  if ( isoElecs[0]->PT > isoMuons[0]->PT ) {
    leptons.push_back( isoElecs[0]->P4() );
    leptons.push_back( isoMuons[0]->P4() );
  }
  else {
    leptons.push_back( isoMuons[0]->P4() );
    leptons.push_back( isoElecs[0]->P4() );
  }
  
  TLorentzVector pll = isoElecs[0]->P4() + isoMuons[0]->P4();
  
  double mll = pll.M();
  if ( leptons[0].Perp() < 35. || leptons[1].Perp() < 20. || mll < 20.) return;
  
  double triggerRatio = (double) rand() / (double) (RAND_MAX + 1.);
  if ( triggerRatio > 0.75 ) return;
  
  bool btag = false; bool centraljet = false;
  std::vector<TLorentzVector> centraljets;
  for (int i = 0; i < jets.size(); i++) {
    if ( fabs(jets[i]->Eta) < 2.4 ) {
      if (checkBTag(jets[i]) ) btag = true;
      else {
    centraljet = true;
    centraljets.push_back(jets[i]->P4() );
      }
    }
    else if (jets[i]->PT > 30. ) return;  
  }  
  
  if (centraljet) return;
  
  double pa[3] = {leptons[0].M(), leptons[0].Px(), leptons[0].Py() };
  double pb[3] = {leptons[1].M(), leptons[1].Px(), leptons[1].Py() };
  double pmiss[3] = {0.,missingET->P4().Px(), missingET->P4().Py() };
  mt2_bisect::mt2 mt2_event;
  mt2_event.set_momenta( pa, pb, pmiss );
  mt2_event.set_mn( 0. );
  double m_t2 = mt2_event.get_mt2(); 
  
  double deltaphi_min = 1000.;
  double deltaphi_temp = 10000.;
  double missingETrel;
  
  for ( int i = 0; i < centraljets.size(); i++ ) {
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
  
  if ( btag ) {
    if ( m_t2 > 70. ) countSignalEvent("CRmT2_top");
    if ( mll < 120. && pll.Perp() > 80. && missingETrel > 80. ) countSignalEvent("CRWWa_top");    
  } 
  else {
    if ( 50. < m_t2 && m_t2 < 90.) countSignalEvent("CRmT2_WW");
    if ( mll < 120. && pll.Perp() > 40. && 60. < missingETrel && missingETrel < 80. ) countSignalEvent("CRWWa_WW");
  }
  
}

void Atlas_1403_5294_CR::finalize() {
  // Whatever should be done after the run goes here
}       
