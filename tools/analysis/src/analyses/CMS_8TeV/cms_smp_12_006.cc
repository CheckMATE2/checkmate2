#include "cms_smp_12_006.h"

void Cms_smp_12_006::initialize() {
  setAnalysisName("cms_smp_12_006");          
  setInformation(""
    "@#measurement of WZ production in 3-leptons and missinig ET\n"
     "@#this part 8 TeV with 19.6 fb^-1\n"
  "");
  setLuminosity(19.6*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("3e;2e;1e;0e;");
  // You should initialize any declared variables here
}

void Cms_smp_12_006::analyze() {
  // Your eventwise analysis code goes here
  missingET->addMuons(muonsCombined); 
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.5, 2.5);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  jets = overlapRemoval(jets, electronsTight, 0.2);
  
  std::vector<Electron*> isoElecs = filterIsolation(electronsTight);
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);  
  
  if (isoElecs.size() + isoMuons.size() != 3) return;
  
  if ( missingET->P4().Et() < 30. ) return;
  
  double mz_diff = 10000.;
  int i_Z = 10; int j_Z = 10;
  TLorentzVector Wlepton;
  
  if (isoElecs.size() == 3) {
    for (int i = 0; i < isoElecs.size() - 1; i++) {
      for (int j = i + 1; j < isoElecs.size(); j++) {
    double mll = (isoElecs[i]->P4() + isoElecs[j]->P4()).M();
    if ( fabs(mll - 91.2 ) < mz_diff && isoElecs[i]->Charge*isoElecs[j]->Charge < 0 ) {
      mz_diff = fabs(mll - 91.2);
      i_Z = i;
      j_Z = j;
    }
      }
    }
    if ( mz_diff > 20. ) return;
    Wlepton = isoElecs[0]->P4() + isoElecs[1]->P4() + isoElecs[2]->P4() - isoElecs[i_Z]->P4() - isoElecs[j_Z]->P4();
    if ( isoElecs[i_Z]->PT < 20. || Wlepton.Perp() < 20. ) return;
    countSignalEvent("3e");
  }
  
  if (isoMuons.size() == 3) {
    for (int i = 0; i < isoMuons.size() - 1; i++) {
      for (int j = i + 1; j < isoMuons.size(); j++) {
    double mll = (isoMuons[i]->P4() + isoMuons[j]->P4()).M();
    if ( fabs(mll - 91.2 ) < mz_diff && isoMuons[i]->Charge*isoMuons[j]->Charge < 0 ) {
      mz_diff = fabs(mll - 91.2 );
      i_Z = i;
      j_Z = j;
    }
      }
    }
    if ( mz_diff > 20. ) return;
    Wlepton = isoMuons[0]->P4() + isoMuons[1]->P4() + isoMuons[2]->P4() - isoMuons[i_Z]->P4() - isoMuons[j_Z]->P4();
    if ( isoMuons[i_Z]->PT < 20. || Wlepton.Perp() < 20. ) return;
    countSignalEvent("0e");
  }
  
  if (isoElecs.size() == 2 && isoElecs[0]->Charge*isoElecs[1]->Charge < 0 && fabs((isoElecs[0]->P4() + isoElecs[1]->P4()).M() - 91.2) < 20. && isoElecs[0]->PT > 20. && isoMuons[0]->PT > 20.) 
    countSignalEvent("2e");
  
  if (isoMuons.size() == 2 && isoMuons[0]->Charge*isoMuons[1]->Charge < 0 && fabs((isoMuons[0]->P4() + isoMuons[1]->P4()).M() - 91.2) < 20. && isoMuons[0]->PT > 20. && isoElecs[0]->PT > 20.) 
    countSignalEvent("1e");
}

void Cms_smp_12_006::finalize() {
  // Whatever should be done after the run goes here
}       
