#include "atlas_conf_2013_061.h"

void Atlas_conf_2013_061_CR::initialize() {
  setInformation(""
  "@#ATLAS\n"
  "@#ATLAS-CONF-2013-061\n"
  "@#0-1 leptons + >= 3 b-jets + etmiss\n"
  "@#sqrt(s) = 8 TeV\n"
  "@#int(L) = 20.1 fb^-1\n"
  "");
  setLuminosity(20.1*units::INVFB);      
  setAnalysisName("atlas_conf_2013_061_CR");    
  ignore("towers");
  
  bookControlRegions("CR0L4JA;CR0L4JB;CR0L4JC;CR0L7JA;CR0L7JB;CR0L7JC;CR1L6JA;CR1L6JB;CR1L6JC;");
  bookCutflowRegions("0;1;2;3;4;5");
  bookCutflowRegions("0L1;0L2;0L3;0L4");
  bookCutflowRegions("0L4JA1;0L4JA2;0L4JA3;0L4JA3;0L4JBC0;0L4JBC1;0L4JB2;0L4JB3;0L4JC2;0L4JC3");
  bookCutflowRegions("0L7J1;0L7J2;0L7JA1;0L7JA2;0L7JB1;0L7JB2;0L7JC1;0L7JC2");
  bookCutflowRegions("1L1;1L2;1L3;1LA1;1LA2;1LA3;1LA4;1LB1;1LB2;1LB3;1LB4;1LC1;1LC2;1LC3;1LC4");
}

void Atlas_conf_2013_061_CR::analyze() {  
  ///UNVALIDATED
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
  int nJets = jets.size();
  photonsMedium = filterPhaseSpace(photonsMedium, 100., -2.0, 2.0);
  int nPhotons = photonsMedium.size();
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  int nElectronsMedium = electronsMedium.size();    
  electronsTight = filterPhaseSpace(electronsTight, 20., -2.47, 2.47);
  electronsTight = filterIsolation(electronsTight);
  int nElectronsTight = electronsTight.size(); 
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  int nMuonsCombined = muonsCombined.size();  
  std::vector<Muon*> muonsIso = filterPhaseSpace(muonsCombined, 20., -2.4, 2.4);
  muonsIso = filterIsolation(muonsIso);
  int nMuonsIso = muonsIso.size();     
  countCutflowEvent("0");
  
  //Veto events with photons > 100 GeV and would be on of two leading jets
  //Except for Met, only jets with |eta| < 2.8 considered.
  //b-tagging efficiency point of 70%, |eta| < 2.5
  //Two kinds of electron, basline is medium with Pt > 20, |eta| < 2.47
  //Signal electrons tight, isolated <0.16 chargerd tracks or <0.18 calo in dR=0.2
  //Two kinds of combined muons
  //Baseline muons, pt > 10 GeV, |eta| < 2.4
  //Signal muons, pt>20 GeV, |eta| < 2.4, isolation cut 12% of muon momentum calo
  //Jets within 0.2 of electron rejected
  //Lepton within 0.4 of jet rejected
  //------------------------
  //Start with photon veto, only for hard photons > 100 GeV and |eta| < 2.0
  if (nPhotons > 0)
    return;
  countCutflowEvent("1");   
  //------------------------
  // Remove jets that overlap with electrons
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  nJets = jets.size();
  
  //---------------------------
  // Remove any baseline muon that overlaps with a jet
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  nMuonsCombined = muonsCombined.size();
  
  //---------------------------
  muonsIso = overlapRemoval(muonsIso, jets, 0.4);
  nMuonsIso = muonsIso.size();
  
  //---------------------------
  // Remove any baseline electron that overlaps with a jet
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  nElectronsMedium = electronsMedium.size();
  
  //---------------------------
  // Remove any tight electron that overlaps with a jet
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  nElectronsTight = electronsTight.size();
  
  //------------------------
  // Only jets with pt > 30 GeV are now considered
  jets = filterPhaseSpace(jets, 30., -2.8, 2.8);
  nJets = jets.size();
  
  //------------------------
  // Events required to have at least four jets, pt > 30 GeV
  if (nJets < 4)
    return;
  countCutflowEvent("2");
  if (jets[3]->PT < 30.0)
    return;
  countCutflowEvent("3");
  
  //------------------------
  // Initial trigger (strange order to compare with published cutflow
  if (jets[0]->PT < 90.0)
    return;
  countCutflowEvent("4");
  if (missingET->P4().Et() < 150.0) 
    return;
  countCutflowEvent("5");
  
  //****************************************************************************************** 
  //--------------------------
  //------0 baseline lepton cutflow------
  //--------------------------
  // O baseline leptons required
  bool flag0Lep = false;
  if (nElectronsMedium == 0) {
    countCutflowEvent("0L1");
    flag0Lep = true;
  }
  if ( (flag0Lep ) && (nMuonsCombined == 0) ) 
    countCutflowEvent("0L2");
  else 
    flag0Lep = false;
  
  //--------------------------
  // minimum separation between leading four jets and MET
  if (flag0Lep ) {
    for (int i=0; i < 4; i++) {
      if (fabs(jets[i]->P4().DeltaPhi(missingET->P4())) < 0.5)
        flag0Lep  = false;
    }
  }
  if (flag0Lep )  
    countCutflowEvent("0L3");
  
  //--------------------------
  // MET/m4jEff should be greater than 0.2
  if (flag0Lep ) {
    double m4jEff = missingET->P4().Et();
    for (int i=0; i < 4; i++) {
      m4jEff += jets[i]->PT;
    }
    if ( (missingET->P4().Et()/m4jEff) < 0.2) 
      flag0Lep = false;
    else 
      countCutflowEvent("0L4");
  }
  
  //****************************************************************************************** 
  //--------------------------
  //------0 lepton, 4 jet cutflow------
  //--------------------------
  bool flag0Lep4J= false;
  bool flag0Lep4JA= false;
  bool flag0Lep4JB= false;
  
  //--------------------------    
  // Control regions 0lepton 4jet B require 4 jets > 50 GeV
  if ( (flag0Lep ) && (jets[4]->PT > 50.0) ) { 
    flag0Lep4JB = true;
    countCutflowEvent("0L4JB0");
  }
  
  //--------------------------
  // Control regions require 3 bjets jets > 30 GeV (A), 50GeV (B)
  std::vector<Jet*> bJets;  
  for (int i=0; i < nJets; i++) {
    if ( (jets[i]->PT > 30.0) && (fabs(jets[i]->Eta) < 2.5) && (checkBTag(jets[i]) ) ) 
      bJets.push_back(jets[i]);
  }
  if ((flag0Lep ) && ( bJets.size() > 2) ) {
    flag0Lep4J = true;
    countCutflowEvent("0L4JA1");
    if ( (flag0Lep4JB ) && (bJets[2]->PT > 50)) 
      countCutflowEvent("0L4JB1");
    else 
      flag0Lep4JB = false;
  }     
  
  //--------------------------
  // Control regions require ETMiss > 150 GeV    
  if ( (flag0Lep4J ) && (missingET->P4().Et() > 150.)) {
    countCutflowEvent("0L4JA2");
    countCutflowEvent("0L4JB2");
  }   
  else 
    flag0Lep4J = false;
  
  //--------------------------
  // Control region B requires mEffInc < 1000 GeV    
  if (flag0Lep4JB ) { 
    double mEffInc = missingET->P4().Et();
    for (int i=0; i < nJets; i++) {
      mEffInc += jets[i]->PT;
    }
    if (mEffInc < 1000.) {
      countCutflowEvent("0L4JB3");
    }
    else 
      flag0Lep4JB = false;
  }   
  
  //--------------------------
  // Control region A requires ETmiss/sqrt(HT4j) < 16
  if (flag0Lep4J ) { 
    double HT4j = jets[0]->PT;
    for (int i=1; i < 4; i++) {
      HT4j += jets[i]->PT;
    }
    if ( (missingET->P4().Et()/(sqrt(HT4j))) < 16.0) 
      flag0Lep4JA = true;
  }
  
  //****************************************************************************************** 
  //--------------------------
  //------0 lepton, 7 jet cutflow------
  //--------------------------
  bool flag0Lep7J = false;
  bool flag0Lep7JA = false;
  bool flag0Lep7JB = false;
  
  //--------------------------    
  // Control regions requires 7 jets with > 30 GeV
  if ( (flag0Lep ) && (nJets > 6)) {
    flag0Lep7J = true;
    countCutflowEvent("0L7J1");
  }
  else 
    flag0Lep7J = false;
  
  //--------------------------    
  // Control regions requires 3 b-jets with > 30 GeV
  if ((flag0Lep7J ) && ( bJets.size() > 2) ) 
    countCutflowEvent("0L7J2");
  else 
    flag0Lep7J = false;
  
  //--------------------------
  // Control regions require ETMiss > 150 (A) and also < 350 (B)    
  if ( (flag0Lep7J ) && (missingET->P4().Et() > 150.)) {
    countCutflowEvent("0L7JA1");
    if (missingET->P4().Et() < 350.) {
      countCutflowEvent("0L7JB1");
      flag0Lep7JB =true;
    }
  }   
  else 
    flag0Lep7J = false;
  
  //--------------------------
  // Signal regions require mEffInc < 1000 (A), < 1500 (B)    
  if (flag0Lep7J ) { 
    double mEffInc = missingET->P4().Et();
    for (int i=0; i < nJets; i++) {
      mEffInc += jets[i]->PT;
    }
    if ( (mEffInc < 1500.) && (flag0Lep7JB ) ) 
      countCutflowEvent("0L7JB2");
    else 
      flag0Lep7JB = false;
    if (mEffInc < 1000.) {
      flag0Lep7JA = true;
      countCutflowEvent("0L7JA2");
    }
  } 
  
  //******************************************************************************************  
  //--------------------------
  //------1 lepton cutflow------
  //--------------------------
  bool flag1Lep = false;
  bool flag1Lep4J = false;
  bool flag1Lep6J = false;
  if ( (nElectronsTight > 0) || (nMuonsIso > 0) ) { 
    flag1Lep = true;
    countCutflowEvent("1L1");
  }
  
  //--------------------------    
  // Control regions '6J' requires 6 jets with > 30 GeV
  if ( (flag1Lep ) && (nJets > 5)) {
    countCutflowEvent("1L6J0");
    flag1Lep6J = true;
  }
  
  //--------------------------    
  // Control regions requires 3 b-jets with > 30 GeV
  if ( (flag1Lep ) && (bJets.size() > 2)) {
    countCutflowEvent("1L6J1");
    countCutflowEvent("1L4J1");
  }
  else 
    flag1Lep = false;
  
  //--------------------------
  // Control regions require mT > 100 (4J), and also < 140 (6J)    
  double lepPT =0.;
  double lepPhi =0.;
  double lepDeltaPhi = 0;
  if (flag1Lep ) {
    if (nElectronsTight > 0) {
      lepPT = electronsTight[0]->PT;
      lepPhi = electronsTight[0]->Phi;
      lepDeltaPhi = fabs(electronsTight[0]->P4().DeltaPhi(missingET->P4()));
    }
    if ( (nMuonsIso > 0) && (muonsIso[0]->PT > lepPT)) {
      lepPT = muonsIso[0]->PT;
      lepPhi = muonsIso[0]->Phi;
      lepDeltaPhi = fabs(muonsIso[0]->P4().DeltaPhi(missingET->P4()));
    }
    double mT = sqrt(2.*lepPT*missingET->P4().Et()*(1.-cos(lepDeltaPhi)));
    if (mT > 100.) {
      countCutflowEvent("1L6J2");
      countCutflowEvent("1L4J2");
      if ( (flag1Lep6J ) && (mT < 140.)) 
        countCutflowEvent("1L6J2");
      else 
        flag1Lep6J = false;
    }
    else 
      flag1Lep = false;
  } 
  
  //--------------------------
  // Control regions require MET > 150  
  if ( (flag1Lep ) && (missingET->P4().Et() > 150.0) ) {
    countCutflowEvent("1L6J3");
    countCutflowEvent("1L4J3");
    flag1Lep4J = true;
  } 
  else 
    flag1Lep = false;
  
  //--------------------------
  // Control region 6J require MET/sqrt(HT) > 5  
  double HT = lepPT;
  if (flag1Lep6J ) {
    for (int i=0; i < nJets; i++) {
      HT += jets[i]->PT;
    }
    if ((missingET->P4().Et()/sqrt(HT)) > 5.0) 
      countCutflowEvent("1L6J4");
    else 
      flag1Lep6J = false;
  }
  
  //--------------------------
  // Signal region 6J requires mEff > 600  
  if (flag1Lep6J ) {
    double mEffInc = HT + missingET->P4().Et();
    if (mEffInc > 600.0)
      countCutflowEvent("1L6J5");
    else 
      flag1Lep6J = false;
  }  
  
  //--------------------------
  //------Signal Regions------
  //--------------------------
  // Signal region 0 lepton, 4 jet
  if (flag0Lep4J ) {
    if (flag0Lep4JA )
      countControlEvent("CR0L4JA");
    if (flag0Lep4JB )
      countControlEvent("CR0L4JB");        
  }
  
  //--------------------------
  // Signal region 0 lepton, 7 jet
  if (flag0Lep7J ) {
    if (flag0Lep7JA )
      countControlEvent("CR0L7JA");
    if (flag0Lep7JB )
      countControlEvent("CR0L7JB");        
  }
  
  //--------------------------
  // Signal region 0 lepton, 4 jet
  if (flag1Lep ) {
    if (flag1Lep4J )
      countControlEvent("CR1L4J");
    if (flag1Lep6J )
      countControlEvent("CR1L6J");
  }
}

void Atlas_conf_2013_061_CR::finalize() {
}       
