#include "cms_1502_06031.h"
// AUTHOR: kitpc
//  EMAIL: kitpc

void Cms_1502_06031::initialize() {
  setAnalysisName("cms_1502_06031");          
  setInformation(""
    "# cms\n"
     "# cms_1502_06031\n"
     "# two leptons, jets, missingET\n"
     "# sqrt()\x1b[D\n"
     "# sqrt(s))=8TeV\n"
     "# int(L)=19.4 fb^-1\n"
  "");
  setLuminosity(19.4*units::INVFB);      
  ignore("towers");
  ignore("tracks");
  
  std::string nameSR[3] = {"a.lowMET", "b.mediumMET", "c.highMET"};
  for( int i=0; i < 3; i++) {
    bookSignalRegions("SR01_GE2jets_"+nameSR[i]);
    bookSignalRegions("SR02_GE3jets_"+nameSR[i]);
    bookCutflowRegions("CR03_GE2jets_"+nameSR[i]);
    bookCutflowRegions("CR04_GE3jets_"+nameSR[i]);
  }

  bookCutflowRegions("CR00_all;CR01_GE2lep;CR02_mll;CR03_GE2jets;CR04_GE3jets");

}

void Cms_1502_06031::analyze() {
  countCutflowEvent("CR00_all");

  missingET->addMuons(muonsCombined);

//reconstruction
  std::vector<Electron*> electronsTemp = electronsLoose;
  std::vector<Electron*> electrons;
  std::vector<Muon*> muons;
  double eta;

  for ( int i=0; i < electronsTemp.size(); i++ ) {
    eta = electronsTemp[i]->Eta;
    if ( electronsTemp[i]->PT > 10. && (fabs(eta) < 1.4 || ( fabs(eta)>1.6 && fabs(eta)<2.4 ) ) )
      electrons.push_back(electronsTemp[i]);
  }

  for ( int i=0; i < muonsCombined.size(); i++ ) {
    eta = muonsCombined[i]->Eta;
    if ( muonsCombined[i]->PT > 10. && (fabs(eta) < 1.4 || ( fabs(eta)>1.6 && fabs(eta)<2.4 ) ) )
      muons.push_back(muonsCombined[i]);
  }

  //electrons = filterPhaseSpace( electrons, 10., -2.4, 2.4);
  //muons     = filterPhaseSpace( muons, 10., -2.4, 2.4);
  electrons = filterIsolation( electrons );
  muons     = filterIsolation( muons );

  jets = filterPhaseSpace( jets, 40., -3., 3. );
  jets = overlapRemoval( jets, electrons, 0.4 );
  jets = overlapRemoval( jets, muons, 0.4 );
  electrons = overlapRemoval( electrons, jets, 0.4);
  muons = overlapRemoval( muons, jets, 0.4);

  // Include trigger efficiencies 
  bool triggerFlag = false;
  double triggerRatio = (double) rand() / (double) (RAND_MAX + 1.);
  if((electrons.size() > 1 && electrons[0]->PT > 17. && electrons[1]->PT > 8. ) && (triggerRatio < 0.98)) triggerFlag = true;
  else if((muons.size() > 1 && muons[0]->PT > 17. && muons[0]->PT > 8.) && (triggerRatio < 0.99)) triggerFlag = true;

  if(triggerFlag == false)
    return;

  //for leptons with PT order
  std::vector<FinalStateObject*> isoleps;
  FinalStateObject* lep;
  for( int i=0; i < electrons.size(); i++) {
    lep = newFinalStateObject( electrons[i] );
    isoleps.push_back(lep);
  }
  for( int i=0; i < muons.size(); i++) {
    lep = newFinalStateObject( muons[i] );
    isoleps.push_back(lep);
  }
  std::sort( isoleps.begin(), isoleps.end(), sortByPT );

  //for checking if isoleps[0]-PT > isoleps[1]->PT > ...
  for(int i=0; i < isoleps.size(); i++) {
    fout1 << isoleps[i]->PT << "  " << isoleps[i]->Type << "  ";
  }
  fout1 << std::endl;

  //for veto where no e+e- or mu+mu-
  if( isoleps[0]->Type != isoleps[1]->Type ) return;
  if( isoleps[0]->Charge * isoleps[1]->Charge > 0 ) return;
  //for delta_R > 0.3
  if( isoleps[0]->P4().DeltaR( isoleps[1]->P4() ) < 0.3 ) return;
  //for veto where pt[0] < 20. or pt[1] < 20.
  if( isoleps[1]->PT < 20. ) return;
  countCutflowEvent("CR01_GE2lep");

  //for SF and opposite charge
  bool eeflag = false, mmflag = false;
  std::string flavour;
  if( isoleps[0]->Type == "electron" ) {
    eeflag = true;
    flavour = "ee";
  }
  else {
    mmflag = true;
    flavour = "mm";
  }

  //for veto where mll not on Z-mass region
  double mll = ( isoleps[0]->P4() + isoleps[1]->P4() ).M();
  if( mll < 81. || mll > 101. ) return;
  countCutflowEvent("CR02_mll");

  //for multi-jets
  if ( jets.size() < 2 ) return;
  countCutflowEvent("CR03_GE2jets");

  bool GE3jetsFlag = false;
  if ( jets.size() >= 3 ) {
    countCutflowEvent("CR04_GE3jets");
    GE3jetsFlag = true;
  }

  if( missingET->P4().Et() > 300. ) {
    countCutflowEvent("CR03_GE2jets_c.highMET");
    countSignalEvent("SR01_GE2jets_c.highMET");
    countCutflowEvent("SR01_GE2jets_c.highMET");
    if(GE3jetsFlag) {
      countCutflowEvent("CR04_GE3jets_c.highMET");
      countSignalEvent("SR02_GE3jets_c.highMET");
      countCutflowEvent("SR02_GE3jets_c.highMET");
    }
  }
  if( missingET->P4().Et() > 200. ) {
    countCutflowEvent("CR03_GE2jets_b.mediumMET");
    if( missingET->P4().Et() < 300. ) {
      countSignalEvent("SR01_GE2jets_b.mediumMET");
      countCutflowEvent("SR01_GE2jets_b.mediumMET");
    }
    if(GE3jetsFlag) {
      countCutflowEvent("CR04_GE3jets_b.mediumMET");
      if( missingET->P4().Et() < 300. ) {
        countSignalEvent("SR02_GE3jets_b.mediumMET");
        countCutflowEvent("SR02_GE3jets_b.mediumMET");
      }
    }
  }
  if( missingET->P4().Et() > 100. ) {
    countCutflowEvent("CR03_GE2jets_a.lowMET");
    if( missingET->P4().Et() < 200. ) {
      countSignalEvent("SR01_GE2jets_a.lowMET");
      countCutflowEvent("SR01_GE2jets_a.lowMET");
    }
    if(GE3jetsFlag) {
      countCutflowEvent("CR04_GE3jets_a.lowMET");
      if( missingET->P4().Et() < 200. ) {
        countSignalEvent("SR02_GE3jets_a.lowMET");
        countCutflowEvent("SR02_GE3jets_a.lowMET");
      }
    }
  }

}

void Cms_1502_06031::finalize() {
} 
