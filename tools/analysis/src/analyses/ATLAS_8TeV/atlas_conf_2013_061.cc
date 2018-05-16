#include "atlas_conf_2013_061.h"

void Atlas_conf_2013_061::initialize() {
  setInformation(""
  "@#ATLAS\n"
  "@#ATLAS-CONF-2013-061\n"
  "@#0-1 leptons + >= 3 b-jets + etmiss\n"
  "@#sqrt(s) = 8 TeV\n"
  "@#int(L) = 20.1 fb^-1\n"
  "");
  setLuminosity(20.1*units::INVFB);      
  setAnalysisName("atlas_conf_2013_061");          
  ignore("towers");
  
  bookSignalRegions("SR0L4JA;SR0L4JB;SR0L4JC;SR0L7JA;SR0L7JB;SR0L7JC;SR1L6JA;SR1L6JB;SR1L6JC;");
  bookCutflowRegions("0;1;2;3;4;5");
  bookCutflowRegions("0L1;0L2;0L3;0L4");
  bookCutflowRegions("0L4JA1;0L4JA2;0L4JA3;0L4JA3;0L4JBC0;0L4JBC1;0L4JB2;0L4JB3;0L4JC2;0L4JC3");
  bookCutflowRegions("0L7J1;0L7J2;0L7JA1;0L7JA2;0L7JB1;0L7JB2;0L7JC1;0L7JC2");
  bookCutflowRegions("1L1;1L2;1L3;1LA1;1LA2;1LA3;1LA4;1LB1;1LB2;1LB3;1LB4;1LC1;1LC2;1LC3;1LC4");
}

void Atlas_conf_2013_061::analyze() {  
  ///UNVALIDATED
  missingET->addMuons(muonsCombined);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
  int nJets = jets.size();
  photonsMedium = filterPhaseSpace(photonsMedium, 100., -2.0, 2.0);
  int nPhotons = photonsMedium.size();
  electronsMedium = filterPhaseSpace(electronsMedium, 20., -2.47, 2.47);
  int nElectronsMedium = electronsMedium.size();    
  electronsTight = filterPhaseSpace(electronsTight, 25., -2.47, 2.47);
  
  //std::cerr << "Before Iso: " << electronsTight.size() << std::endl;
  electronsTight = filterIsolation(electronsTight);
  int nElectronsTight = electronsTight.size(); 
  
  // std::cerr << "After Iso: " << electronsTight.size() << std::endl;
  // sleep(1);
  // if (nElectronsTight > 0) {
  //  std::cerr << "Electron pt: " << electronsTight[0]->PT << std::endl;
  //sleep(1);
  //}
  
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  int nMuonsCombined = muonsCombined.size();  
  std::vector<Muon*> muonsIso = filterPhaseSpace(muonsCombined, 25., -2.4, 2.4);
  muonsIso = filterIsolation(muonsIso);
  
  int nMuonsIso = muonsIso.size();     
  
  // if (nMuonsIso > 0) {
  //  std::cerr << "Muon pt: " << muonsIso[0]->PT << std::endl;
  // sleep(1);
  //}
  
  countCutflowEvent("0");
  
  //Veto events with photons > 100 GeV and are harder than either of the two leading jets
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
  // To start remove jets that overlap with electrons
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  nJets = jets.size();
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  nMuonsCombined = muonsCombined.size();
  muonsIso = overlapRemoval(muonsIso, jets, 0.4);
  nMuonsIso = muonsIso.size();
  //---------------------------
  // Remove any baseline electron that overlaps with a jet
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  nElectronsMedium = electronsMedium.size();
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  nElectronsTight = electronsTight.size();
  
  //------------------------
  // Only jets with pt > 30 GeV are now considered
  jets = filterPhaseSpace(jets, 30., -2.8, 2.8);
  nJets = jets.size();
  
  //------------------------
  //Start with photon veto, only for hard photons > 100 GeV and |eta| < 2.0
  if (nPhotons > 0)
    return;
  countCutflowEvent("1");   
  
  //------------------------
  // Events required to have at least four jets, pt > 30 GeV
  if (nJets < 4)
    return;
  countCutflowEvent("2");
  
  if (jets[3]->PT < 30.0)
    return;
  countCutflowEvent("3");
  
  //------------------------
  // Initial trigger (strange order to compare with published cutflow)
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
  if ( (flag0Lep) && (nMuonsCombined == 0) ) 
    countCutflowEvent("0L2");
  else 
    flag0Lep = false;
  
  //--------------------------
  // minimum separation between leading four jets and MET
  if (flag0Lep) {
    for (int i=0; i < 4; i++) {
      if (fabs(jets[i]->P4().DeltaPhi(missingET->P4())) < 0.5)
        flag0Lep  = false;
    }
  }
  if (flag0Lep)  
    countCutflowEvent("0L3");
  
  //--------------------------
  // MET/m4jEff should be greater than 0.2
  if (flag0Lep) {
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
  bool flag0Lep4J = false;
  bool flag0Lep4JA = false;
  bool flag0Lep4JB = false;
  bool flag0Lep4JC = false;
  bool flag0Lep4JBC = false;
  //--------------------------    
  // Signal regions 0lepton 4jet B and C require 4 jets > 50 GeV
  if ( (flag0Lep) && (jets[3]->PT > 50.0) ) { 
    flag0Lep4JBC = true;
    countCutflowEvent("0L4JBC0");
  }
  
  //--------------------------
  // Signal regions require 3 bjets jets > 30 GeV (A), 50GeV (BC)
  std::vector<Jet*> bJets;  
  for (int i=0; i < nJets; i++) {
    if ( (jets[i]->PT > 30.0) && (fabs(jets[i]->Eta) < 2.5) && (checkBTag(jets[i]))) 
      bJets.push_back(jets[i]);
  }
  if ((flag0Lep) && ( bJets.size() > 2) ) {
    flag0Lep4J = true;
    countCutflowEvent("0L4JA1");
    if ( (flag0Lep4JBC) && (bJets[2]->PT > 50)) 
      countCutflowEvent("0L4JBC1");
    else 
      flag0Lep4JBC = false;
  }     
  
  //--------------------------
  // Signal regions require ETMiss > 200 (A), 350 (B), 250 (C)    
  if ( (flag0Lep4J) && (missingET->P4().Et() > 200.)) {
    countCutflowEvent("0L4JA2");
    if ( (flag0Lep4JBC) && (missingET->P4().Et() > 250.)) {
      flag0Lep4JC = true;
      countCutflowEvent("0L4JC2");
      if (missingET->P4().Et() > 350.) {
        flag0Lep4JB = true;
        countCutflowEvent("0L4JB2");
      }
    }
  }   
  else 
    flag0Lep4J = false;
  
  //--------------------------
  // Signal regions require mEffInc > 1000 (A), 1100 (B), 1300 (C)    
  if (flag0Lep4J) { 
    double mEffInc = missingET->P4().Et();
    for (int i=0; i < nJets; i++)
      mEffInc += jets[i]->PT;
    
    if (mEffInc > 1000.) {
      countCutflowEvent("0L4JA3");
      if ( (flag0Lep4JB) && (mEffInc > 1100.)) 
        countCutflowEvent("0L4JB3");
      else 
        flag0Lep4JB = false;
      if ( (flag0Lep4JC) && (mEffInc > 1300.)) 
        countCutflowEvent("0L4JC3");
      else 
        flag0Lep4JC = false;
    }
    else 
      flag0Lep4J = false;
  }   
  
  //--------------------------
  // Signal region A requires ETmiss/sqrt(HT4j) > 16
  if (flag0Lep4J) { 
    double HT4j = jets[0]->PT;
    for (int i=1; i < 4; i++)
      HT4j += jets[i]->PT;
    if ( (missingET->P4().Et()/(sqrt(HT4j))) > 16.0) {
      countCutflowEvent("0L4JA4");
      flag0Lep4JA = true;
    }
  }
  
  //****************************************************************************************** 
  //--------------------------
  //------0 lepton, 7 jet cutflow------
  //--------------------------
  bool flag0Lep7J = false;
  bool flag0Lep7JA = false;
  bool flag0Lep7JB = false;
  bool flag0Lep7JC = false;
  
  //--------------------------    
  // Signal regions requires 7 jets with > 30 GeV
  if ( (flag0Lep) && (nJets > 6)) {
    flag0Lep7J = true;
    countCutflowEvent("0L7J1");
  }
  else 
    flag0Lep7J = false;
  
  //--------------------------    
  // Signal regions requires 3 b-jets with > 30 GeV
  if ((flag0Lep7J) && ( bJets.size() > 2) ) 
    countCutflowEvent("0L7J2");
  else 
    flag0Lep7J = false;
  
  //--------------------------
  // Signal regions require ETMiss > 200 (A), 350 (B), 250 (C)    
  if ( (flag0Lep7J) && (missingET->P4().Et() > 200.)) {
    countCutflowEvent("0L7JA1");
    if (missingET->P4().Et() > 250.) {
      flag0Lep7JC = true;
      countCutflowEvent("0L7JC1");
      if (missingET->P4().Et() > 350.) {
        flag0Lep7JB = true;
        countCutflowEvent("0L7JB1");
      }
    }
  }   
  else 
    flag0Lep7J = false;
  
  //--------------------------
  // Signal regions require mEffInc > 1000 (A + B), 1500 (C)    
  if (flag0Lep7J) { 
    double mEffInc = missingET->P4().Et();
    for (int i=0; i < nJets; i++)
      mEffInc += jets[i]->PT;
    
    if (mEffInc > 1000.) {
      flag0Lep7JA = true;
      countCutflowEvent("0L7JA2");
      if (flag0Lep7JB) 
        countCutflowEvent("0L7JB2");
      else 
        flag0Lep7JB = false;
      
      if ( (flag0Lep7JC) && (mEffInc > 1500.)) 
        countCutflowEvent("0L7JC2");
      else 
        flag0Lep7JC = false;
    }
    else 
      flag0Lep7J = false;
  } 
  
  //******************************************************************************************  
  //--------------------------
  //------1 lepton cutflow------
  //--------------------------
  bool flag1Lep = false;
  bool flag1LepA = false;
  bool flag1LepB = false;
  bool flag1LepC = false;
  if ( (nElectronsTight > 0) || (nMuonsIso > 0) ) { 
    flag1Lep = true;
    countCutflowEvent("1L1");
  }
  
  //--------------------------    
  // Signal regions requires 6 jets with > 30 GeV
  if ( (flag1Lep) && (nJets > 5)) 
    countCutflowEvent("1L2");
  else 
    flag1Lep = false;
  
  //--------------------------    
  // Signal regions requires 3 b-jets with > 30 GeV
  if ( (flag1Lep) && (bJets.size() > 2)) 
    countCutflowEvent("1L3");
  else 
    flag1Lep = false;
  
  //--------------------------
  // Signal regions require mT > 140 (A + B), 160 (C)    
  double lepPT = 0.;
  double lepPhi = 0.;
  double lepDeltaPhi = 0;
  if (flag1Lep) {
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
    if (mT > 140.) {
      countCutflowEvent("1LA1");
      countCutflowEvent("1LB1");
      flag1LepA = true;
      flag1LepB = true;
      if (mT> 160.) {
        countCutflowEvent("1LC1");
        flag1LepC = true;
      }
    }
    else 
      flag1Lep = false;
  } 
  
  //--------------------------
  // Signal regions require MET > 175 (A), 225 (B), 275 (C)    
  if ( (flag1Lep) && (missingET->P4().Et() > 175.0) ) {
    countCutflowEvent("1LA2");
    if (missingET->P4().Et() > 225.0) 
      countCutflowEvent("1LB2");
    else 
      flag1LepB = false;
    if ((flag1LepC) && (missingET->P4().Et() > 275.0)) 
      countCutflowEvent("1LC2");
    else 
      flag1LepC = false;
  } 
  else 
    flag1Lep = false;
  
  //--------------------------
  // Signal regions require MET/sqrt(HT) > 5  
  double HT = lepPT;
  if (flag1Lep) {
    for (int i=0; i < nJets; i++) {
      HT += jets[i]->PT;
    }
    if ((missingET->P4().Et()/sqrt(HT)) > 5.0) { 
      countCutflowEvent("1LA3");
      if (flag1LepB) 
        countCutflowEvent("1LB3");
      if (flag1LepC) 
        countCutflowEvent("1LC3");
    }
    else 
      flag1Lep = false;
  }
  //--------------------------
  // Signal regions require mEff > 700 (A), 800 (B), 900 (C)   
  if (flag1Lep) {
    double mEffInc = HT + missingET->P4().Et();
    if (mEffInc > 700.0) { 
      countCutflowEvent("1LA4");
      if ((flag1LepB) && (mEffInc > 800.0)) 
        countCutflowEvent("1LB4");
      else 
        flag1LepB = false;
      if ((flag1LepC) && (mEffInc > 900.0)) 
        countCutflowEvent("1LC4");
      else 
        flag1LepC = false;
    }
    else 
      flag1Lep = false;
  }  
  //--------------------------
  //------Signal Regions------
  //--------------------------
  // Signal region 0 lepton, 4 jet
  if (flag0Lep4J ) {
    if (flag0Lep4JA ) 
      countSignalEvent("SR0L4JA");
    if (flag0Lep4JB ) 
      countSignalEvent("SR0L4JB");
    if (flag0Lep4JC ) 
      countSignalEvent("SR0L4JC"); 
  }
  
  //--------------------------
  // Signal region 0 lepton, 7 jet
  if (flag0Lep7J ) {
    if (flag0Lep7JA ) 
      countSignalEvent("SR0L7JA");
    if (flag0Lep7JB ) 
      countSignalEvent("SR0L7JB");
    if (flag0Lep7JC ) 
      countSignalEvent("SR0L7JC"); 
  }
  
  //--------------------------
  // Signal region 1 lepton, 4 jet
  if (flag1Lep ) {
    if (flag1LepA ) 
      countSignalEvent("SR1L6JA");
    if (flag1LepB ) 
      countSignalEvent("SR1L6JB");
    if (flag1LepC ) 
      countSignalEvent("SR1L6JC");   
  }
}

void Atlas_conf_2013_061::finalize() {
}       
