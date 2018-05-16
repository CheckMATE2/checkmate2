#include "atlas_1503_03290.h"
// AUTHOR: kitpc
//  EMAIL: kitpc

void Atlas_1503_03290::initialize() {
  setAnalysisName("atlas_1503_03290");          
  setInformation(""
    "# ATLAS\n"
     "# ATLAS_1503_03290\n"
     "# 2 leptons + jets + missingET\n"
     "# sqrt(s) = 8 TeV\n"
     "# int(L) = 20.3 fb^-1\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); 
  ignore("tracks");
  bookSignalRegions("SR-Z;");

  bookCutflowRegions("CR00_noCuts;CR01_leptonTriggerRequirement;\
CR02_rejectEventsWithBadJets;CR03_rejectEventsWithBadMuons;\
CR04_primaryVertexGE4tracks;CR05_vetoEventsWithCosmicMuons;\
CR06_GE2leptons;CR07_fakeMissET;\
\
CRee08_leptonIsolationTriggerMatching;CRee09_leptonFlavour;\
CRee10_promptLeptons;CRee11_oppositeChargeLeptons;\
CRee12_GT1jet;CRee13_electronCrackVeto;\
CRee14_mllGT15;CRee15_dPhi(leadingjet,missET)GT0.4;\
CRee16_dPhi(subleadingjet,missET)GT0.4;CRee17_mllIn(81,101);\
CRee18_HTGT600;CRee19_missETGT225;\
\
CRmm08_leptonIsolationTriggerMatching;CRmm09_leptonFlavour;\
CRmm10_promptLeptons;CRmm11_oppositeChargeLeptons;\
CRmm12_GT1jet;CRmm13_electronCrackVeto;\
CRmm14_mllGT15;CRmm15_dPhi(leadingjet,missET)GT0.4;\
CRmm16_dPhi(subleadingjet,missET)GT0.4;CRmm17_mllIn(81,101);\
CRmm18_HTGT600;CRmm19_missETGT225;");

}

void Atlas_1503_03290::analyze() {
  missingET->addMuons(muonsCombined);

  countCutflowEvent("CR00_noCuts");
    
  //for baseline particles and removing overlap between them
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47, true);
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47, true);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 20., -2.5, 2.5);

  electronsMedium = overlapRemoval(electronsMedium, 0.05);
  electronsTight  = overlapRemoval(electronsTight, 0.05);
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  electronsTight  = overlapRemoval(electronsTight, jets, 0.4);
  muonsCombined   = overlapRemoval(muonsCombined, jets, 0.4);
  electronsMedium = overlapRemoval(electronsMedium, muonsCombined, 0.01);
  electronsTight  = overlapRemoval(electronsTight, muonsCombined, 0.01);

  //countCutflowEvent("CR01_leptonTriggerRequirement");
  //countCutflowEvent("CR02_rejectEventsWithBadJets");
  //countCutflowEvent("CR03_rejectEventsWithBadMuons");

  //for at least five tracks
  //countCutflowEvent("CR04_primaryVertexGE4tracks")

  //for removing cosmmic-ray
  //countCutflowEvent("CR05_vetoEventsWithCosmicMuons")

  //for at least two baseline leptons 
  if ( electronsMedium.size()+muonsCombined.size() < 2 ) return;
    countCutflowEvent("CR06_GE2leptons");

  //for to reject fake missET
  //countCutflow("CR07_fakeMissET")

/*  //for to classify baseline leptons
  std::vector<FinalStateObject*> baselineLeptons;
  for ( int i = 0; i < electronsMedium.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(electronsMedium[i]);
    baselineLeptons.push_back(lep);
  }
  for ( int i = 0; i < muonsCombined.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(muonsCombined[i]);
    baselineLeptons.push_back(lep);
  }

  std::sort(baselineLeptons.begin(), baselineLeptons.end(), sortByPT);
                       
  bool eebase = false; bool mmbase = false;
  if ( baselineLeptons[0]->Type() == baselineLeptons[1]->Type() && baselineLeptons[0]->Type() == "electron"
    eebase = true
  else if ( baselineLetpons[0]->Type() == baselineLeptons[1]->Type() && baselineLeptons[0]->Type() == "muon" )
    mmbase = true */

  //for signal particles, i.e. isolation
  std::vector<FinalStateObject*> signalLeptons;

  electronsTight = filterIsolation(electronsTight, 0);
  for ( int i = 0; i < electronsTight.size(); i++ ) {
    if ( electronsTight[i]->P4().Et() < 25. ) {
      FinalStateObject* lep = newFinalStateObject(electronsTight[i]);
      signalLeptons.push_back(lep);
    }
  }
  electronsMedium = filterIsolation(electronsMedium, 1);
  for ( int i = 0; i < electronsMedium.size(); i++) {
    if ( electronsMedium[i]->P4().Et() >= 25. ) {
      FinalStateObject* lep = newFinalStateObject(electronsMedium[i]);
      signalLeptons.push_back(lep);
    } 
  }

  std::vector<Muon*> muonsEtLT25 = filterIsolation(muonsCombined, 0);
  for ( int i = 0; i < muonsEtLT25.size(); i++ ) {
    if ( muonsEtLT25[i]->P4().Et() < 25. ) {
      FinalStateObject* lep = newFinalStateObject(muonsEtLT25[i]);
      signalLeptons.push_back(lep);
    }
  }
  std::vector<Muon*> muonsEtGE25 = filterIsolation(muonsCombined, 1);
  for ( int i = 0; i < muonsEtGE25.size(); i++ ) {
    if ( muonsEtGE25[i]->P4().Et() >= 25.) {
      FinalStateObject* lep = newFinalStateObject(muonsEtGE25[i]);
      signalLeptons.push_back(lep);
    }
  }

  std::vector<Jet*> jetsSignal;
  for ( int i = 0; i < jets.size(); i++ ) {
    if ( jets[i]->PT > 35. and !checkBTag(jets[i]) )
      jetsSignal.push_back(jets[i]);
  }

  //for able to manipulate two leading leptons
  std::sort(signalLeptons.begin(), signalLeptons.end(), sortByPT);
  for (std::vector<FinalStateObject>::size_type i = 0; i < signalLeptons.size(); i++) {
    outfile1 << signalLeptons[i]->PT << "  ";
  }
  outfile1 << std::endl;

  //for trigger and lepton flavour
  if( signalLeptons.size() < 2 || signalLeptons[0]->Type != signalLeptons[1]->Type ) return;

  bool ee = false; bool mm = false;
  std::string flavour;
  if ( signalLeptons[0]->Type == "electron") {
    ee = true;
    flavour = "ee";
  }
  else {
    mm = true;
    flavour = "mm";
  }

  double triggerRatio = (double) rand() / (double) (RAND_MAX + 1.);
  if ( ee ) {
    if ( triggerRatio > 0.99 )
      return;
  }
  if ( mm ) { 
    if ( triggerRatio > 0.85 )
      return;
  }
 
  //countCutflowEvent("CR"+flavour+"08_leptonIsolationTriggerMatching");
  countCutflowEvent("CR"+flavour+"09_leptonFlavour");

  //for to restrict two leading leptons PT
  if ( signalLeptons[0]->PT <= 25. || signalLeptons[1]->PT <= 10. ) return;
  countCutflowEvent("CR"+flavour+"10_promptLeptons");
           
  //for opposite charge
  if ( signalLeptons[0]->Charge*signalLeptons[1]->Charge > 0 ) return;
  countCutflowEvent("CR"+flavour+"11_oppositeChargeLeptons");
                  
  //for amount of jetsSignal >= 2
  if ( jetsSignal.size() < 2 ) return;
  countCutflowEvent("CR"+flavour+"12_GT1jet");

  //Electron crack veto
  //countCutflowEvent("CR"+flavour+"13_electronCrackVeto");

  //for mll > 15 
  double mll=(signalLeptons[0]->P4()+signalLeptons[1]->P4()).M();
  if ( mll <= 15. ) return;
  countCutflowEvent("CR"+flavour+"14_mllGT15");

  //for delta phi
  double dphi1 = 0.; double dphi2 = 0.;
  dphi1 = fabs( jetsSignal[0]->P4().DeltaPhi( missingET->P4() ) );
  dphi2 = fabs( jetsSignal[1]->P4().DeltaPhi( missingET->P4() ) );
  if ( dphi1 <= 0.4 ) return;
  countCutflowEvent("CR"+flavour+"15_dPhi(leadingjet,missET)GT0.4");
  if ( dphi2 <= 0.4 ) return;
  countCutflowEvent("CR"+flavour+"16_dPhi(subleadingjet,missET)GT0.4");

  //for mll in (81, 101)
  mll=(signalLeptons[0]->P4()+signalLeptons[1]->P4()).M();
  if ( !( mll > 81 && mll < 101 ) ) return;
  countCutflowEvent("CR"+flavour+"17_mllIn(81,101)");

  //for HT
  double HT = 0.;
  for ( int i = 0; i < jetsSignal.size(); i++ ) {
    HT += jetsSignal[i]->PT;      
  }
  HT += signalLeptons[0]->PT + signalLeptons[1]->PT;
  if ( HT <= 600. ) return;
  countCutflowEvent("CR"+flavour+"18_HTGT600");

  //for missingET
  if ( missingET->P4().Et() <= 225.) return;
  countCutflowEvent("CR"+flavour+"19_missETGT225");

  //for counting in signal region
  if ( ee || mm ) countSignalEvent("SR-Z");

}

void Atlas_1503_03290::finalize() {

}
