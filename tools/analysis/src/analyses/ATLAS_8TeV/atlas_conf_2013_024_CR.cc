#include "atlas_conf_2013_024.h"

void Atlas_conf_2013_024_CR::initialize() {
  setInformation(""
  "@# ATLAS\n"
  "@# ATLAS-CONF-2013-024\n"
  "@# 0 leptons + 6 (2 b-)jets + etmiss\n"
  "@# sqrt(s) = 8 TeV\n"
  "@# int(L) = 20.5 fb^-1\n"
  "");
  setLuminosity(20.5*units::INVFB);      
  setAnalysisName("atlas_conf_2013_024_CR");    
  ignore("muonsCombined");
  ignore("towers");
  ignore("photons");
  
  bookControlRegions("CR1Lep200;CR1Lep300;CR1Lep350;CR2Lep;CRMultiJet");
  bookCutflowRegions("00;01;02;03;04;05;06;07;08;09;10;11;12;13;");
}

void Atlas_conf_2013_024_CR::analyze() {
  
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47);
  int nElectronsLoose = electronsLoose.size();
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47);
  std::vector<int> electronTightIso;
  electronTightIso.push_back(0);
  electronTightIso.push_back(1);
  electronsTight = filterIsolation(electronsTight,electronTightIso);
  int nElectronsTight = electronsTight.size();
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 8., -2.4, 2.4);
  int nMuonsCombinedPlus = muonsCombinedPlus.size();
  std::vector<Muon*> muonsTight = filterIsolation(muonsCombinedPlus);
  int nMuonsTight = muonsTight.size();
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  int nJets = jets.size();
  tracks = filterPhaseSpace(tracks, 0.5, -2.5, 2.5); 
  countCutflowEvent("00");
  
  //-------------------------------------------------------------
  //Control region selection
  //------------------------
  // Electrons are 'tight', track iso: dR=0.3 ratio=0.16, calo iso: dR=0.3 ratio=0.18, removed if dR < 0.4 to jet
  // Muons,  track iso: dR=0.3 ratio=0.12, calo iso: dR=0.3 ratio=0.12, removed if dR < 0.4 to jet
  //-------------------------------------------------------------
  // Control region trigger
  bool metTrigger = false;
  bool singLepTrigger = false;
  bool doubLepTrigger = false;
  
  //-------------------------------------------------------------
  // MET trigger
  if (missingET->P4().Et() < 80.0) 
    metTrigger = true;
  
  //-------------------------------------------------------------
  //Control region selection
  //------------------------
  // Trigger electrons have tighter isolation
  std::vector<Electron*> electronsTrigger;
  int nElectronsTrigger;
  electronsTrigger = filterPhaseSpace(electronsTight, 12., -2.47, 2.47);
  electronsTrigger = filterIsolation(electronsTrigger,2);
  nElectronsTrigger = electronsTrigger.size();
  
  //-------------------------------------------------------------
  // Single lepton trigger
  if ( (nElectronsTrigger > 0) && (electronsTrigger[0]->PT > 24.0) ) 
    singLepTrigger = true;
  else if ( (nElectronsLoose > 0) && (electronsLoose[0]->PT > 60.0) ) 
    singLepTrigger = true;
  else if ( (nMuonsTight > 0) && (muonsTight[0]->PT > 24.0) ) 
    singLepTrigger = true;
  else if ( (nMuonsCombinedPlus > 0) && (muonsCombinedPlus[0]->PT > 24.0) ) 
    singLepTrigger = true;
  //-------------------------------------------------------------
  // Double lepton trigger
  if ( (nElectronsTrigger > 1) && (electronsTrigger[1]->PT > 12.0) ) 
    doubLepTrigger = true;
  else if ( (nMuonsCombinedPlus > 1) && (muonsCombinedPlus[0]->PT > 18.0) && (muonsCombinedPlus[0]->PT > 8.0))
    doubLepTrigger = true;
  //-------------------------------------------------------------
  // Veto if no trigger
  if ( (!metTrigger) && (!singLepTrigger) && (!doubLepTrigger))
    return; 
  countCutflowEvent("01");
  
  //------------------------
  // 0 lepton cuts
  // Remove jets that overlap with electrons
  jets = overlapRemoval(jets, electronsLoose, 0.2);
  nJets = jets.size();
  
  //---------------------------
  // Remove loose muons that overlap with jets
  muonsCombinedPlus = overlapRemoval(muonsCombinedPlus, jets, 0.4);
  nMuonsCombinedPlus = muonsCombinedPlus.size();
  
  //---------------------------
  // Remove tight muons that overlap with jets
  muonsTight = overlapRemoval(muonsTight, jets, 0.4);
  
  nMuonsTight = muonsTight.size();
  
  //---------------------------
  // Remove loose electrons that overlap with jets
  electronsLoose = overlapRemoval(electronsLoose, jets, 0.4);
  nElectronsLoose = electronsLoose.size();
  
  //---------------------------
  // Remove tight electrons that overlap with jets
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  nElectronsTight = electronsTight.size();
  
  //---------------------------
  // Control region flags
  bool crMultiJet = false;
  bool crSingLep = false;
  bool crDoubLep = false;
  
  //---------------------------
  // Fill vector of lepton four-momentum
  std::vector<TLorentzVector> crLeptons;
  for (int e = 0; e < electronsTight.size(); e++)
    crLeptons.push_back(electronsTight[e]->P4());
  for (int e = 0; e < muonsTight.size(); e++) {
    if (muonsTight[e]->PT > 10.)
      crLeptons.push_back(muonsTight[e]->P4());
  }
  
  //---------------------------
  // Find relevant control region and veto if none found
  if ( crLeptons.size() == 0) 
    crMultiJet = true;
  else if ( (crLeptons.size() == 1) && (crLeptons[0].Pt() > 35.))  
    crSingLep = true;
  else if ( (crLeptons.size() == 2) && (electronsTight.size() == 2) && (crLeptons[1].Pt() > 20.))  
    crDoubLep = true;
  if ( (crLeptons.size() == 2) && (muonsTight.size() == 2) && (crLeptons[0].Pt() > 20.) && (crLeptons[1].Pt() > 10.))  
    crDoubLep = true;
  else if ( (!crSingLep) && (!crSingLep) && (!crDoubLep) )
    return;
  countCutflowEvent("02");
  
  //---------------------------
  // Double lepton control region requires reconstructed Z
  TLorentzVector recoZ;
  if ( crDoubLep) {
    recoZ = crLeptons[0] + crLeptons[1];
    if ( (recoZ.M() > 101.) || (recoZ.M() < 81.0) )
      return;
  }
  countCutflowEvent("03");
  
  //------------------------------
  // Misssing energy calculated from pre-selected objects
  TLorentzVector objMET;
  for (int i = 0; i < jets.size(); i++) 
    objMET -= jets[i]->P4();
  for (int i = 0; i < electronsLoose.size(); i++) 
    objMET -= electronsLoose[i]->P4();
  for (int i = 0; i < muonsCombinedPlus.size(); i++) 
    objMET -= muonsCombinedPlus[i]->P4();
  
  //------------------------------
  // Missing energy cut > 200 GeV (single lepton), > 70 GeV (double lepton with Z momentum added), > 160 GeV (multijet)
  if ( (crSingLep) && (objMET.Pt() < 200.0))
    return;
  if (crDoubLep) {
    objMET += recoZ;
    if (objMET.Pt() < 70.0)
      return;
  }
  if ( (crMultiJet) && (objMET.Pt() < 70.0))
    return;
  countCutflowEvent("04");   
  
  //------------------------------
  // Final selection jets required to be harder    
  jets = filterPhaseSpace(jets, 35., -2.8, 2.8);
  if (jets.size() < 6)
    return;
  if (jets[1]->PT < 80.0)
    return;
  countCutflowEvent("05");
  
  //------------------------------
  // Missing energy is also calculated from reconstructed tracks (assume 50% reconstructed, matched to cutflow)
  TLorentzVector trackMET;    
  for (int i = 0; i < tracks.size(); i++) {
    if ( (tracks[i]->PT > 0.5) && ( fabs(tracks[i]->Eta) < 2.5) && (rand()/static_cast<double>(RAND_MAX) > 0.25))  
      trackMET -= tracks[i]->P4();
  }
  if (trackMET.Pt() < 30.0)
    return;
  countCutflowEvent("06");
  
  //------------------------------
  // Calo and track missing energy should point in the same direction
  if (fabs(missingET->P4().DeltaPhi(trackMET)) > (TMath::Pi()/3.))
    return;    
  countCutflowEvent("07");
  
  //------------------------------
  // jet-MET collinearity > pi/10 (single lepton), > pi/5 (double lepton), < pi/5 (multijet)
  bool multiJetPhi = false;
  for (int i = 0; i < 3; i++) {
    double phiDist = fabs(jets[i]->P4().DeltaPhi(objMET));
    if ( (crSingLep) && (phiDist < (0.1*TMath::Pi())))
      return;
    if ( (crDoubLep) && (phiDist < (0.2*TMath::Pi())))
      return;
    if ( (crMultiJet) && (phiDist < (0.2*TMath::Pi())))
      multiJetPhi = true; 
  }
  if ( (crMultiJet) && (multiJetPhi))
    return;
  countCutflowEvent("08");
  
  //------------------------------
  // Require that if any other jets (not b's) lie collinear with MET they contain more than 4 tracks
  // Called 'tau veto' in cutflow
  // Only used in double lepton control region
  if (crDoubLep) {
    for (int i=3; i < jets.size(); i++) {
      if ( (fabs(jets[i]->P4().DeltaPhi(objMET)) < (0.2*TMath::Pi())) && (!checkBTag(jets[i])) ) {
        int nJetTracks = 0;
        for (int j = 0; j < jets[i]->Constituents.GetEntriesFast(); ++j)
        {
          TObject *object = jets[i]->Constituents.At(j);      
          // Check if the constituent is accessible
          if (object == 0) 
            continue;
          else if (object->IsA() == Track::Class())
          {
            double trackPT = ((Track*) object)->PT;
            double trackEtaAbs = fabs( ((Track*) object)->Eta);
            if ( (trackPT > 0.5) && (trackEtaAbs < 2.5) && (rand()/static_cast<double>(RAND_MAX) > 0.25))  
              nJetTracks += 1;
          }
          else if (object->IsA() == Candidate::Class() && ((Candidate*)object)->Charge != 0) {
              double trackPT = ((Candidate*) object)->Momentum.Pt();
              double trackEtaAbs = fabs( ((Candidate*) object)->Momentum.Eta());
              if ( (trackPT > 0.5) && (trackEtaAbs < 2.5) && (rand()/static_cast<double>(RAND_MAX) > 0.25))
                  nJetTracks += 1;
          }
        }
        if (nJetTracks < 4)
          return;
      }
    }
  }
  countCutflowEvent("09"); 
  
  //------------------------------
  // Find b-jets
  // Veto event if 2 bjets are not found
  std::vector<Jet*> bJets, notBJets;
  for (int i=0; i < jets.size(); i++) {
    if ( (fabs(jets[i]->Eta) < 2.5) && (checkBTag(jets[i])) )
      bJets.push_back(jets[i]);
    else 
      notBJets.push_back(jets[i]);
  }
  if (bJets.size() < 2)
    return;
  countCutflowEvent("10"); 
  
  //------------------------------
  // mT calculated with b jet closest to met vector and be greater than 175GeV (only for double lepton and multijet control regions).
  if ( (crSingLep) || (crMultiJet)) {
    double bMetPhi = 10.;
    int bMetInt;
    for (int b = 0; b < bJets.size(); b++) {
      double bMetPhiNew = fabs(bJets[b]->P4().DeltaPhi(objMET));
      if ( bMetPhiNew < bMetPhi) {
        bMetPhi = bMetPhiNew;
        bMetInt = b;
      }
    }
    double mT = sqrt(2.*bJets[bMetInt]->PT*objMET.Pt()*(1.-cos(bJets[bMetInt]->P4().DeltaPhi(objMET))));
    if (mT < 175.)
      return;
  }
  countCutflowEvent("11");
  
  //------------------------------
  // Need to find three closest jets in eta phi plane to form top system.
  // B-jet information is NOT used for top mass reconstruction
  // Mass = 0 - 600 (single lepton), 80 - 270 (double lepton)
  int iNum, jNum, kNum;
  if ( (crSingLep) || (crDoubLep)) {
    double finDist = 10.;
    double ijDist, ikDist, jkDist;
    for (int i = 0; i < jets.size(); i++) {
      for (int j = 0; j < jets.size(); j++) {
        for (int k = 0; k < jets.size(); k++) {
          if ( (i != j) && (i != k) && (j != k)) {
            ijDist = jets[i]->P4().DeltaR(jets[j]->P4());
            ikDist = jets[i]->P4().DeltaR(jets[k]->P4());
            jkDist = jets[j]->P4().DeltaR(jets[k]->P4()); 
            if (ikDist > ijDist)
              ijDist = ikDist;
            if (jkDist > ijDist)
              ijDist = jkDist;
            if (ijDist < finDist) {
              finDist = ijDist;
              iNum = i;
              jNum = j;
              kNum =k;
            }
          }
        }
      }
    }
    
    TLorentzVector triJet = jets[iNum]->P4() + jets[jNum]->P4() + jets[kNum]->P4();
    double triMass = triJet.M();
    if ( (crSingLep) && (triMass>270.))
      return; 
    if ( (crDoubLep) && ((triMass<80.) || (triMass>270.)))
      return; 
  }
  countCutflowEvent("12");
  
  //------------------------------
  // Find the second three closest jets in eta phi plane to form 2nd top system.
  // Mass = 0 - 600 (single lepton), 80 - 270 (double lepton)
  if ( (crSingLep) || (crDoubLep)) {
    double finDist = 10.;
    double ijDist, ikDist, jkDist;
    int iNum2, jNum2, kNum2;
    for (int i=0; i < jets.size(); i++) {
      for (int j=0; j < jets.size(); j++) {
        for (int k=0; k < jets.size(); k++) {
          if ( (i != j) && (i != k) && (j != k) && (i != iNum) && (i != jNum) && (i != kNum) && (j != iNum) && (j != jNum) && (j != kNum) && (k != iNum) && (k != jNum) && (k != kNum) ) {
            ijDist = jets[i]->P4().DeltaR(jets[j]->P4());
            ikDist = jets[i]->P4().DeltaR(jets[k]->P4());
            jkDist = jets[j]->P4().DeltaR(jets[k]->P4());
            if (ikDist > ijDist)
              ijDist = ikDist;
            if (jkDist > ijDist)
              ijDist = jkDist;
            if (ijDist < finDist) {
              finDist = ijDist;
              iNum2 = i;
              jNum2 = j;
              kNum2 =k;
            }
          }
        }
      }
    }
    TLorentzVector triJet = jets[iNum2]->P4() + jets[jNum2]->P4() + jets[kNum2]->P4();
    double triMass = triJet.M();
    if ( (crSingLep) && (triMass>270.))
      return; 
    if ( (crDoubLep) && ((triMass<80.) || (triMass>270.)))
      return; 
  }
  countCutflowEvent("13");
  
  
  
  //--------------------------
  //------Control Regions------
  //--------------------------
  // Single lepton control regions
  if (crSingLep) {
    double mT = sqrt(2.*crLeptons[0].Pt()*objMET.Pt()*(1.-cos(crLeptons[0].DeltaPhi(objMET))));
    if ( (mT > 40.) && (mT<120.)) {
      countControlEvent("CR1Lep200");
      if (objMET.Pt() > 300.) {
        countControlEvent("CR1Lep300");
        if (objMET.Pt() > 350.) {
          countControlEvent("CR1Lep350");
        }
      }
    }
    return;
  }
  
  //--------------------------
  // Double lepton control regions
  if (crDoubLep) {
    countControlEvent("CR2Lep");
    return;
  }
  
  //--------------------------
  // MultiJet control regions
  if (crMultiJet) {
    countControlEvent("CRMultiJet");
    return;
  }
}

void Atlas_conf_2013_024_CR::finalize() {
}       
