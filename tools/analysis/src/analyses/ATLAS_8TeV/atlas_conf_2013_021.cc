#include "atlas_conf_2013_021.h"

void Atlas_conf_2013_021::initialize() {
  setAnalysisName("atlas_conf_2013_021");          
  setInformation(""
    "@#WZ production in 3 leptons final state\n"
     "@#8 TeV, 13 fb^-1\n"
  "");
  setLuminosity(13*units::INVFB);      
//  ignore("towers"); // These won't read tower or track information from the
//  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("eee;eemu;emumu;mumumu;total;");
  // You should initialize any declared variables here
}

void Atlas_conf_2013_021::analyze() {
  // Your eventwise analysis code goes here
  missingET->addMuons(muonsCombined); 
  electronsTight = filterPhaseSpace(electronsTight, 15., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 15., -2.5, 2.5);
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  jets = overlapRemoval(jets, electronsTight, 0.3);
  jets = overlapRemoval(jets, muonsCombined, 0.3);
  
  std::vector<Electron*> isoElecs = filterIsolation(electronsTight,0);
  isoElecs = filterIsolation(isoElecs,1);
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined,0);  
  
  isoElecs = overlapRemoval( isoElecs, isoMuons, 0.1);
  
  //At least 3 isolated leptons required
  if (isoElecs.size() + isoMuons.size() != 3) return;
  
  //One 'super' isolated lepton with pt > 25GeV also required
  std::vector<Electron*> isoElecsTrigger = filterPhaseSpace(isoElecs, 25., -2.47, 2.47); 
  isoElecsTrigger = filterIsolation(isoElecsTrigger,2);
  
  std::vector<Muon*> isoMuonsTrigger = filterPhaseSpace(isoMuons, 25., -2.5, 2.5); 
  isoMuonsTrigger = filterIsolation(isoMuonsTrigger,1);
  
  if( (isoElecsTrigger.size() + isoMuonsTrigger.size()) < 1)
      return;
  
  //Missing ET > 25 GeV
  if ( missingET->P4().Et() < 25. ) return;
  
  double mz_diff = 10000.;
  int i_Z = 10; int j_Z = 10;
  TLorentzVector Wlepton;
  
  //------------------------------------------------------------------
  // 3 electron signal region
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
    if ( mz_diff > 10. ) return;
    if ( isoElecs[0]->PT < 25. ) return; //trigger
    
    //W lepton has tighter isolation conditions
    std::vector<Electron*> wElec;
    for (int i = 0; i < isoElecs.size(); i++) {
      if ((i_Z != i) && (j_Z != i))
    wElec.push_back(isoElecs[i]);
    }
    wElec = filterIsolation(wElec,3);
    wElec = filterIsolation(wElec,4);
    
    if (wElec.size()<1)
      return;
    
    if(wElec[0]->PT < 20.)
      return;
        
    double mTW2 = 2.*wElec[0]->PT*missingET->P4().Et() - 2.*wElec[0]->P4().X()*missingET->P4().X() - 2.*wElec[0]->P4().Y()*missingET->P4().Y();
    if (sqrt(mTW2) < 20.) 
      return;
    
    countSignalEvent("eee");
    countSignalEvent("total");
  }
  
  
  //------------------------------------------------------------------
  // 3 muon signal region
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
    if ( mz_diff > 10. ) return;
    if (isoMuons[0]->PT < 25. ) return; //trigger
    
    //W lepton has tighter isolation conditions
    std::vector<Muon*> wMuon;
    for (int i = 0; i < isoMuons.size(); i++) {
      if ((i_Z != i) && (j_Z != i))
    wMuon.push_back(isoMuons[i]);
    }
    wMuon = filterIsolation(wMuon,2);
    
    if (wMuon.size()<1)
      return;
    
    if(wMuon[0]->PT < 20.)
      return;

    double mTW2 = 2.*wMuon[0]->PT*missingET->P4().Et() - 2.*wMuon[0]->P4().X()*missingET->P4().X() - 2.*wMuon[0]->P4().Y()*missingET->P4().Y();
    if (sqrt(mTW2) < 20.) return;
    
    countSignalEvent("mumumu");
    countSignalEvent("total");
  }
  
  
  //------------------------------------------------------------------
  // 2 electron, 1 muon signal region
  
  if (isoElecs.size() == 2 && isoElecs[0]->Charge*isoElecs[1]->Charge < 0 && fabs((isoElecs[0]->P4() + isoElecs[1]->P4()).M() - 91.2) < 10. && isoMuons[0]->PT > 20.) {    
    
    isoMuons = filterIsolation(isoMuons,2);
    
    if (isoMuons.size() != 1) return;
    
    double mTW2 = 2.*isoMuons[0]->P4().Et()*missingET->P4().Et() - 2.*isoMuons[0]->P4().X()*missingET->P4().X() - 2.*isoMuons[0]->P4().Y()*missingET->P4().Y();
    if (sqrt(mTW2) < 20.) return;    
    
    countSignalEvent("eemu");
    countSignalEvent("total");
  }
  
  //------------------------------------------------------------------
  // 1 electron, 2 muon signal region
  
  if (isoMuons.size() == 2 && isoMuons[0]->Charge*isoMuons[1]->Charge < 0 && fabs((isoMuons[0]->P4() + isoMuons[1]->P4()).M() - 91.2) < 10. && isoElecs[0]->PT > 20.) {
    
    isoElecs = filterIsolation(isoElecs,3);
    isoElecs = filterIsolation(isoElecs,4);
 
    if (isoElecs.size() != 1) return;
    
    double mTW2 = 2.*isoElecs[0]->P4().Et()*missingET->P4().Et() - 2.*isoElecs[0]->P4().X()*missingET->P4().X() - 2.*isoElecs[0]->P4().Y()*missingET->P4().Y();
    if (sqrt(mTW2) < 20.) return;    
    
    countSignalEvent("emumu");
    countSignalEvent("total");
  }
}

void Atlas_conf_2013_021::finalize() {
  // Whatever should be done after the run goes here
}       
