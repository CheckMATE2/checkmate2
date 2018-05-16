#include "atlas_conf_2013_047.h"

void Atlas_conf_2013_047::initialize() {
  setInformation(""
  "@# ATLAS\n"
  "@# ATLAS-CONF-2013-047\n"
  "@# 0 leptons, 2-6 jets, etmiss\n"
  "@# sqrt(s) = 8 TeV\n"
  "@# int(L) = 20.3 fb^-1\n"
  "");
  setLuminosity(0.996*20.3*units::INVFB); //0.996 to conservatively account for jet cleaning       
  setAnalysisName("atlas_conf_2013_047");          
  ignore("towers");
  ignore("tracks");
  bookSignalRegions("AL;AM;BM;BT;CM;CT;D;EL;EM;ET;");  
  bookCutflowRegions("0;1;2;3;4;5;4J1;5J1;6J1;3J2;4J2;5J2;6J2;AL1;AM1;BM1;BT1;CM1;D1;EL1;EM1;ET1;CM2;D2;EL2;EM2;ET2");
  
  mEffA = mEffB = mEffC = mEffD = mEffE = 0;
  cutTriggerJet = 130.;
  cutSecondJet = 60.;
  phiCutTight = 0.2;
  phiCutLoose = 0.4;
  
}

void Atlas_conf_2013_047::analyze() {
  missingET->addMuons(muonsCombined);
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47);
  int nElectronsLoose = electronsLoose.size();            
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  int nMuonsCombined = muonsCombined.size();            
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
  int nJets = jets.size();            
  countCutflowEvent("0");
  flag3J = flag4J = flag5J = flag6J = false;
  
  //------------------------
  // Events containing hard phtons are likely to fail jet cleaning cut
  photonsMedium = filterPhaseSpace(photonsMedium, 130., -2.47, 2.47, true);
  int nPhotons = photonsMedium.size();
  if(nPhotons > 0) 
    return;
  
  //------------------------
  // 0 lepton cuts
  // Remove jets that overlap with electrons
  jets = overlapRemoval(jets, electronsLoose, 0.2);
  nJets = jets.size();
  
  // Veto on isolated electrons and muons
  electronsLoose = overlapRemoval(electronsLoose, jets, 0.4);
  if(!electronsLoose.empty())
    return;
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  if(!muonsCombined.empty())
    return;
  countCutflowEvent("1");
  
  //---------------------------
  // Apply pTCut for jets
  jets = filterPhaseSpace(jets, 40.);
  nJets = jets.size();
  
  //------------------------
  // Veto event if too little missing energy
  if(missingET->P4().Et() < 160.0) 
    return;
  countCutflowEvent("2");    
  
  //------------------------
  // Veto event if leading jet has too little energy
  if(nJets < 1) 
    return;
  if(jets[0]->PT < cutTriggerJet) 
    return;
  countCutflowEvent("3");
  
  //------------------------
  // Veto event if second jet has too little energy
  if(nJets < 2) 
    return;
  if(jets[1]->PT < cutSecondJet) 
    return;    
  countCutflowEvent("4");
  
  //------------------------
  // CutFlow for additional jet signal regions (jet pt > 60 GeV)
  if(nJets > 2){ 
    if(jets[2]->PT > 60.0){            
      countCutflowEvent("3J1");
      flag3J = true;
    }
  }
  if(nJets > 3){ 
    if(jets[3]->PT > 60.0){      
      countCutflowEvent("4J1");
      flag4J = true;
    }
  }
  if(nJets > 4){ 
    if(jets[4]->PT > 60.0){      
      countCutflowEvent("5J1");
      flag5J = true;
    }
  }
  if(nJets > 5){ 
    if(jets[5]->PT > 60.0){      
      countCutflowEvent("6J1");
      flag6J = true;
    }
  }
  
  //---------------------------
  // Jet separation from missing ET,
  if(fabs(jets[0]->P4().DeltaPhi(missingET->P4())) < phiCutLoose)
    return;
  if(fabs(jets[1]->P4().DeltaPhi(missingET->P4())) < phiCutLoose) 
    return;
  if((nJets > 2) && (fabs(jets[2]->P4().DeltaPhi(missingET->P4())) < phiCutLoose)) 
    return;
  countCutflowEvent("5");
  if (flag3J) 
    countCutflowEvent("3J2");
  if (flag4J) 
    countCutflowEvent("4J2");
  if (flag5J) 
    countCutflowEvent("5J2");
  if (flag6J) 
    countCutflowEvent("6J2");
  
  //---------------------------
  // Calculate HT and mEffInc
  double HT = 0.;
  for(int j = 0; j < nJets; j++) 
    HT += jets[j]->PT;
  double mEffInc = missingET->P4().Et() + HT;
  
  
  //--------------------------
  //------Signal Regions------
  //--------------------------
  // Signal region A
  mEffA = missingET->P4().Et() + jets[0]->PT + jets[1]->PT;
  if (missingET->P4().Et()/mEffA > 0.2) {
    countCutflowEvent("AL1");
    if (mEffInc > 1000.)
      countSignalEvent("AL");
  }
  
  double effHT = missingET->P4().Et()/(sqrt(HT));
  if (effHT > 15.){
    countCutflowEvent("AM1");
    if(mEffInc > 1600.)
      countSignalEvent("AM");
  }
  
  //---------------------------
  // Signal region B
  if(flag3J){
    mEffB = mEffA + jets[2]->PT;
    if(missingET->P4().Et()/mEffB > 0.3){
      countCutflowEvent("BM1");
      if(mEffInc > 1800.) 
        countSignalEvent("BM");
      if(missingET->P4().Et()/mEffB > 0.4){
        countCutflowEvent("BT1");
        if (mEffInc > 2200.)
          countSignalEvent("BT");
      }
    }
  }
  
  //---------------------------
  // Signal region C
  for(int j = 3; j < nJets; j++){
    if(fabs(jets[j]->P4().DeltaPhi(missingET->P4())) < phiCutTight) 
      return;
  }
  if (flag4J) {
    countCutflowEvent("CM1");
    mEffC = mEffB + jets[3]->PT;
    if(missingET->P4().Et()/mEffC > 0.25) {
      countCutflowEvent("CM2");
      if (mEffInc > 1200.) 
        countSignalEvent("CM");
      if (mEffInc > 2200.) 
        countSignalEvent("CT");
    }
  }
  
  //---------------------------
  // Signal region D
  if (flag5J) {
    countCutflowEvent("D1");
    mEffD = mEffC + jets[4]->PT;
    if(missingET->P4().Et()/mEffD > 0.2){
      countCutflowEvent("D2");
      if (mEffInc > 1600.) 
        countSignalEvent("D");
    }
  }
  
  //---------------------------
  // Signal region E
  if (flag6J) {
    countCutflowEvent("EL1");
    countCutflowEvent("EM1");
    countCutflowEvent("ET1");
    mEffE = mEffD + jets[5]->PT;
    if(missingET->P4().Et()/mEffE > 0.15){
      countCutflowEvent("EL2");
      if (mEffInc > 1000.) 
        countSignalEvent("EL");
      if(missingET->P4().Et()/mEffE > 0.2){
        countCutflowEvent("EM2");
        if (mEffInc > 1200.) 
          countSignalEvent("EM");
        if(missingET->P4().Et()/mEffE > 0.25){
          countCutflowEvent("ET2");
          if (mEffInc > 1500.) 
            countSignalEvent("ET");
        }
      }
    }
  }
}

void Atlas_conf_2013_047::finalize() {
}       
