#include "atlas_conf_2013_024.h"

void Atlas_conf_2013_024::initialize() {
  setInformation(""
  "@# ATLAS\n"
  "@# ATLAS-CONF-2013-024\n"
  "@# 0 leptons + 6 (2 b-)jets + etmiss\n"
  "@# sqrt(s) = 8 TeV\n"
  "@# int(L) = 20.5 fb^-1\n"
  "");
  setLuminosity(20.5*units::INVFB*0.985); // Value of 0.985 is to account for event cleaning and primary vertex finding  
  setAnalysisName("atlas_conf_2013_024");          
  ignore("towers");
  ignore("photons");
  ignore("electronsMedium");
  ignore("muonsCombined");
  
  bookSignalRegions("SR1;SR2;SR3;");
  bookCutflowRegions("00;01;02;03;04;05;06;07;08;09;10;11;12;13;14;15;16");
}

void Atlas_conf_2013_024::analyze() {
  missingET->addMuons(muonsCombinedPlus);
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47);
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 10., -2.4, 2.4);
  
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  tracks = filterPhaseSpace(tracks, 0.5, -2.5, 2.5); 
  countCutflowEvent("00");
  
  //------------------------
  // Initial trigger
  // Normalised to cutflow efficiency
  // Trigger is fully efficient for signal selection however.
  if (missingET->P4().Et() < 80.0) 
    return;
  countCutflowEvent("01");
  
  //------------------------
  // 0 lepton cuts
  // Remove jets that overlap with electrons
  jets = overlapRemoval(jets, electronsLoose, 0.2);
  
  //---------------------------
  // Veto on isolated muons
  if (!(overlapRemoval(muonsCombinedPlus, jets, 0.4)).empty())
    return;
  
  countCutflowEvent("02");
  //---------------------------
  // Veto on isolated electrons
  if(!(overlapRemoval(electronsLoose, jets, 0.4)).empty())
    return;
  
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
  // CutFlow includes 130 GeV cut on MET so let's do it as well
  if (objMET.Pt() < 130.0)
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
  // Missing energy is also calculated from reconstructed tracks (assume 75% reconstructed, matched to cutflow)
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
  // Require that the three highest pt jets do not lie collinear with MET
  for (int i = 0; i < 3; i++) {
    if (fabs(jets[i]->P4().DeltaPhi(objMET)) < (0.2*TMath::Pi()))
      return;
  }
  countCutflowEvent("08");
  
  //------------------------------
  // Require that if any other jets (not b's) lie collinear with MET they contain more than 4 tracks
  // Called 'tau veto' in cutflow
  for (int i = 3; i < jets.size(); i++) {
    if ( (fabs(jets[i]->P4().DeltaPhi(objMET)) < (0.2*TMath::Pi())) && (!checkBTag(jets[i])) ) {
      int nJetTracks = 0;
      for (int j = 0; j < jets[i]->Constituents.GetEntriesFast(); ++j)
      {
        TObject *object = jets[i]->Constituents.At(j);  
        // Check if the constituent is accessible
        if (object == 0) continue;
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
  // mT calculated with b jet closest to met vector and be greater than 175GeV.
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
  countCutflowEvent("11");
  
  //------------------------------
  // Need to find three closest jets in eta phi plane to form top system.
  // B-jet information is NOT used for top mass reconstruction
  double finDist = 10.;
  double ijDist, ikDist, jkDist;
  int iNum, jNum, kNum;
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
            kNum = k;
          }
        }
      }
    }
  }
  TLorentzVector triJet = jets[iNum]->P4() + jets[jNum]->P4() + jets[kNum]->P4();
  double triMass = triJet.M();
  if ((triMass<80.) || (triMass>270.))
    return; 
  countCutflowEvent("12");
  
  //------------------------------
  // Find the second three closest jets in eta phi plane to form 2nd top system.
  finDist = 10.;
  int iNum2, jNum2, kNum2;
  for (int i = 0; i < jets.size(); i++) {
    for (int j = 0; j < jets.size(); j++) {
      for (int k = 0; k < jets.size(); k++) {
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
  triJet = jets[iNum2]->P4() + jets[jNum2]->P4() + jets[kNum2]->P4();
  triMass = triJet.M();
  if ((triMass<80.) || (triMass>270.))
    return; 
  countCutflowEvent("13");
  
  //------------------------------
  // CutFlow also includes an EtMiss > 150
  if (objMET.Pt() < 150.)
    return;
  countCutflowEvent("14");    
  
  // CutFlow also includes an EtMiss > 200
  if (objMET.Pt() < 200.)
    return;
  countCutflowEvent("15");   
  
  //--------------------------
  //------Signal Regions------
  //--------------------------
  // Signal region 1, already given (MET > 200 GeV)
  countSignalEvent("SR1");
  if (objMET.Pt() < 250.)
    return;
  countCutflowEvent("16"); 
  
  //--------------------------
  // Signal region 2, (MET > 300 GeV)
  if (objMET.Pt() < 300.)
    return;
  countSignalEvent("SR2");     
  
  //--------------------------
  // Signal region 3, (MET > 350 GeV)
  if (objMET.Pt() < 350.)
    return;
  countSignalEvent("SR3");     
}

void Atlas_conf_2013_024::finalize() {
}       
