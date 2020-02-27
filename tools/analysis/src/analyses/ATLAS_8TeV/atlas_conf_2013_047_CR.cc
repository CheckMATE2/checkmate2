#include "atlas_conf_2013_047.h"
  
  
std::string Atlas_conf_2013_047_CR::cr[4] = { "CRY", "CRQ", "CRW", "CRT"};
std::string Atlas_conf_2013_047_CR::sr[10] = { "AL", "AM", "BM", "BT", "CM", "CT", "D", "EL", "EM", "ET"};  

void Atlas_conf_2013_047_CR::initialize() {
  setInformation(""
  "@# ATLAS\n"
  "@# ATLAS-CONF-2013-047\n"
  "@# 0 leptons, 2-6 jets, etmiss\n"
  "@# sqrt(s) = 8 TeV\n"
  "@# int(L) = 20.3 fb^-1\n"
  "");
  setLuminosity(0.996*20.3*units::INVFB);   //0.996 to conservatively account for jet cleaning     
  setAnalysisName("atlas_conf_2013_047_CR");    
  ignore("towers");
  ignore("electronsMedium");
  bookCutflowRegions("0");
  
  mEffA = mEffB = mEffC = mEffD = mEffE = 0;
  cutTriggerJet = 130.;
  cutSecondJet = 60.;
  phiCutTight = 0.2;
  phiCutLoose = 0.4;
  mEffCut = 1000.; 
  for (int i=0;i<4;i++) {
    for (int j=0;j<10;j++) {
      bookControlRegions(cr[i]+"_"+sr[j]);
    }
  }
}

void Atlas_conf_2013_047_CR::analyze() {
  missingET->addMuons(muonsCombined);
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47);
  int nElectronsLoose = electronsLoose.size();            
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  int nMuonsCombined = muonsCombined.size();            
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
  int nJets = jets.size();            
  countCutflowEvent("0");
  cry = crq = crw = crt = false;
  crBTag = crWTag = false;
  
  //------------------------
  // cry requires a hard isolated photon
  photonsMedium = filterPhaseSpace(photonsMedium, 130., -2.47, 2.47);
  std::vector<Photon*> tempPhotons;    
  for (int i = 0; i < photonsMedium.size(); i++) {
    Photon* cand = photonsMedium[i];
    if ((fabs(cand->Eta) < 1.44) || (fabs(cand->Eta > 1.57)))
      tempPhotons.push_back(cand);
  }
  photonsMedium = tempPhotons;
  int nPhotons = photonsMedium.size(); 
  
  //---------------------------
  // if photon hard photon exists, event can only be used for control region
  // photon energy is therefore added to missing energy vector
  if (nPhotons == 1) {
    cry = true;
    double newMissPx = photonsMedium[0]->P4().Px() + missingET->P4().Px();
    double newMissPy = photonsMedium[0]->P4().Py() + missingET->P4().Py();
    double newMissEt = sqrt((newMissPx*newMissPx)+(newMissPy*newMissPy));
    missingET->P4().SetPxPyPzE(newMissPx, newMissPy, 0., newMissEt);
  }
  
  // 0 lepton cuts
  // Remove jets that overlap with electrons and recheck jet conditions
  jets = overlapRemoval(jets, electronsLoose, 0.2);
  nJets = jets.size();
  
  //---------------------------
  // apply pTCut for jets
  jets = filterPhaseSpace(jets, 40.);
  nJets = jets.size();
  
  //------------------------
  // Veto event if too little missing energy
  if (missingET->P4().Et() < 160.0) 
    return;
  
  //------------------------
  // Veto event if leading jet has too little energy
  if (nJets < 1) 
    return;
  if (jets[0]->PT < cutTriggerJet) 
    return;
  
  //------------------------
  // Veto event if second jet has too little energy
  if (nJets < 2) 
    return;
  if (jets[1]->PT < cutSecondJet) 
    return;
  
  //---------------------------
  // Jet separation from missing ET, crq requires no sep, cry requires sep
  bool jetSep = true;
  if (fabs(jets[0]->P4().DeltaPhi(missingET->P4())) < phiCutLoose)
    jetSep = false;
  else if (fabs(jets[1]->P4().DeltaPhi(missingET->P4())) < phiCutLoose)
    jetSep = false;
  else if ((nJets > 2) && (fabs(jets[2]->P4().DeltaPhi(missingET->P4())) < phiCutLoose))
    jetSep = false;
  if (!jetSep) {
    cry = false;
    crq = true;
  } 
  
  //---------------------------
  // cry and crq demand no isolated electrons and muons
  if (!(overlapRemoval(electronsLoose, jets, 0.4).empty())) {
    cry = false;
    crq = false;
  }
  if (!(overlapRemoval(muonsCombined, jets, 0.4).empty())) {
    cry = false;
    crq = false;
  }
  
  //---------------------------
  // Calculate HT and mEffInc, return if too small mEff
  double HT = 0.;
  for (int j = 0; j < nJets; j++)
    HT += jets[j]->PT;
  if (nPhotons > 0)
    HT += photonsMedium[0] -> PT;
  double mEffInc = missingET->P4().Et() + HT;
  if (mEffInc < mEffCut) 
    return;
  
  //---------------------------
  // W reconstruction using mT, cr3 and cr4
  if (nElectronsLoose > 0) {
    double mT = sqrt(2.*electronsLoose[0]->PT*missingET->P4().Et()*(1.-cos(electronsLoose[0]->P4().DeltaPhi(missingET->P4()))));
    if ((mT>30.) && (mT<100.)) {
      crWTag = true;
      for (int j = 0; j < nJets; j++) {
        if (checkBTag(jets[0])) 
          crBTag = true;
      }
    }
  }
  if (nMuonsCombined > 0) {
    double mT = sqrt(2.*muonsCombined[0]->PT*missingET->P4().Et()*(1.-cos(muonsCombined[0]->P4().DeltaPhi(missingET->P4()))));
    if ((mT>30.) && (mT<100.)) {
      crWTag = true;
      for (int j = 0; j < nJets; j++) {
        if (checkBTag(jets[0])) 
          crBTag = true;
      }
    }
  }
  if (crBTag) 
    crt = true;
  else if (crWTag)
    crw = true;
  
  //---------------------------
  // if isolated hard photon, all other control regions not filled
  if (cry) {
    crq = false;
    crw = false;
    crt = false;
  }
  //---------------------------
  //if no control regions are filled, veto event
  if ((!cry)&&(!crq)&&(!crw)&&(!crt)) 
    return;
  
  //--------------------------
  //------Signal Regions------
  //--------------------------
  // Signal region A, for loose crw and crt use jetSep cut and ETmiss/meff
  mEffA = missingET->P4().Et() + jets[0]->PT + jets[1]->PT;
  if (mEffInc > 1000.) {
    if (crq) 
      countControlEvent(cr[1]+"_"+sr[0]);
    if ((missingET->P4().Et()/mEffA > 0.2)) {
      if (jetSep) {
        if (cry) 
          countControlEvent(cr[0]+"_"+sr[0]);
        if (crw) 
          countControlEvent(cr[2]+"_"+sr[0]);
        if (crt) 
          countControlEvent(cr[3]+"_"+sr[0]);
      }
    }
  }
  if (mEffInc > 1600.) {
    if (crq) 
      countControlEvent(cr[1]+"_"+sr[1]);
    if (crw) 
      countControlEvent(cr[2]+"_"+sr[1]);
    if (crt) 
      countControlEvent(cr[3]+"_"+sr[1]);
    if (cry) {
      double effHT = missingET->P4().Et()/(sqrt(HT));
      if ((effHT > 15.)) 
        countControlEvent(cr[0]+"_"+sr[1]);
    }
  }
  //---------------------------
  // Signal region B
  mEffB = mEffA + jets[2]->PT;
  if (nJets < 3) 
    return;
  if (jets[2]->PT < 60.) 
    return;
  if (mEffInc > 1800.) {
    if (crq) countControlEvent(cr[1]+"_"+sr[2]);
    if (crw) countControlEvent(cr[2]+"_"+sr[2]);
    if (crt) countControlEvent(cr[3]+"_"+sr[2]);
    if ((cry) && ((missingET->P4().Et()/mEffB > 0.3)))
      countControlEvent(cr[0]+"_"+sr[2]);
  }
  if (mEffInc > 2200.) {
    if (crq)
      countControlEvent(cr[1]+"_"+sr[3]);
    if (crw)
      countControlEvent(cr[2]+"_"+sr[3]);
    if (crt)
      countControlEvent(cr[3]+"_"+sr[3]);
    if ((cry) && ((missingET->P4().Et()/mEffB > 0.4)))
      countControlEvent(cr[0]+"_"+sr[3]);
  }
  
  //---------------------------
  // Signal region C
  mEffC = mEffB + jets[3]->PT;
  for (int j = 3; j < nJets; j++) {
    if (fabs(jets[3]->P4().DeltaPhi(missingET->P4())) < phiCutTight) 
      cry = false;
  }
  if (nJets < 4) 
    return;
  if (jets[3]->PT < 60.) 
    return;
  if (mEffInc > 1200.) {
    if (crq)
      countControlEvent(cr[1]+"_"+sr[4]);
    if (crw)
      countControlEvent(cr[2]+"_"+sr[4]);
    if (crt)
      countControlEvent(cr[3]+"_"+sr[4]);
    if ((cry) && ((missingET->P4().Et()/mEffB > 0.25))) 
      countControlEvent(cr[0]+"_"+sr[4]);
  }
  if (mEffInc > 2200.) {
    if (crq)
      countControlEvent(cr[1]+"_"+sr[5]);
    if (crw)
      countControlEvent(cr[2]+"_"+sr[5]);
    if (crt)
      countControlEvent(cr[3]+"_"+sr[5]);
    if ((cry) && ((missingET->P4().Et()/mEffB > 0.25)))
      countControlEvent(cr[0]+"_"+sr[5]);
  }
  
  //---------------------------
  // Signal region D
  mEffD = mEffC + jets[4]->PT;
  if (nJets < 5) 
    return;
  if (jets[4]->PT < 60.) 
    return;
  if (mEffInc > 1300.) {
    if (crw) 
      countControlEvent(cr[2]+"_"+sr[6]);
    if (crt) 
      countControlEvent(cr[3]+"_"+sr[6]);
    if (mEffInc > 1600.) {
      if (crq) 
        countControlEvent(cr[1]+"_"+sr[6]);
      if ((cry) && ((missingET->P4().Et()/mEffB > 0.20)))
        countControlEvent(cr[0]+"_"+sr[6]);
    }
  }
  
  //---------------------------
  // Signal region E
  mEffE = mEffD + jets[5]->PT;
  if (nJets < 6) 
    return;
  if (jets[5]->PT < 60.) 
    return;
  if (mEffInc > 1000.) {
    if (crq)
      countControlEvent(cr[1]+"_"+sr[7]);
    if (crw)
      countControlEvent(cr[2]+"_"+sr[7]);
    if (crt)
      countControlEvent(cr[3]+"_"+sr[7]);
    if ((cry) && ((missingET->P4().Et()/mEffB > 0.15)))
      countControlEvent(cr[0]+"_"+sr[7]);
  }
  if (mEffInc > 1200.) {
    if (crq)
      countControlEvent(cr[1]+"_"+sr[8]);
    if (crw)
      countControlEvent(cr[2]+"_"+sr[8]);
    if (crt)
      countControlEvent(cr[3]+"_"+sr[8]);
    if ((cry) && ((missingET->P4().Et()/mEffB > 0.20)))
      countControlEvent(cr[0]+"_"+sr[8]);
  }
  if (mEffInc > 1300.) {
    if (crw) 
      countControlEvent(cr[2]+"_"+sr[9]);
    if (crt) 
      countControlEvent(cr[3]+"_"+sr[9]);
    if (mEffInc > 1500.) {
      if (crq) 
        countControlEvent(cr[1]+"_"+sr[9]);
      if ((cry) && ((missingET->P4().Et()/mEffB > 0.25)))
        countControlEvent(cr[0]+"_"+sr[9]);
    }
  }
}

void Atlas_conf_2013_047_CR::finalize() {
}       
