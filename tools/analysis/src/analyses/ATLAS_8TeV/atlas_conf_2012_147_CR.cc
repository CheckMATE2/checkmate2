#include "atlas_conf_2012_147.h"

std::string Atlas_conf_2012_147_CR::cr[3] = { "CRElecW", "CRMuonW", "CRMuonZ"};
std::string Atlas_conf_2012_147_CR::sr[4] = { "1", "2", "3", "4"};
    
void Atlas_conf_2012_147_CR::initialize() {
  setInformation(""
  "@# ATLAS\n"
  "@# ATLAS-CONF-2012-147\n"
  "@# Monojet + etmiss\n"
  "@# sqrt(s) = 8 TeV\n"
  "@# int(L) = 10 fb^-1\n"
  "");
  setLuminosity(10*units::INVFB);      
  setAnalysisName("atlas_conf_2012_147_CR");    
  ignore("towers");
  ignore("tracks");
  ignore("electronsTight");
  ignore("muonsCombined");
  bookCutflowRegions("0;1;2;3;4;5;6");
  for (int i=0;i<3;i++) {
    for (int j=0;j<4;j++) {
      bookControlRegions(cr[i]+"_"+sr[j]);
    }
  }
  
}

void Atlas_conf_2012_147_CR::analyze() {
  ///UNVALIDATED!!!!!!!!!!!!!!!
  bool eWEvent = false;
  bool muWEvent = false;
  bool muZEvent = false;
  
  missingET->addMuons(muonsCombinedPlus);
  electronsMedium = filterPhaseSpace(electronsMedium, 20., -2.47, 2.47);
  electronsMedium = filterIsolation(electronsMedium);
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 7., -2.5, 2.5);
  muonsCombinedPlus = filterIsolation(muonsCombinedPlus);
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  photonsMedium = filterPhaseSpace(photonsMedium, 20., -4.5, 4.5);
  countCutflowEvent("0");
  
  // Remove jets that overlap with electrons
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  
  // All control regions require isolated electrons and muons
  std::vector<Electron*> isoElecs = overlapRemoval(electronsMedium, jets, 0.4);
  std::vector<Muon*> isoMuons = overlapRemoval(muonsCombinedPlus, jets, 0.4);
  
  //------------------------
  // Check we have the correct number of leptons and if 2 muons, they are opposite charge
  if (isoElecs.size() == 1 ) 
    eWEvent = true;
  if (isoMuons.size() == 1 ) 
    muWEvent = true;
  if ((isoMuons.size() == 2 ) && ((isoMuons[0]->Charge)*(isoMuons[1]->Charge) < 0))
    muZEvent = true; 
  if ( (!eWEvent) && (!muWEvent) && (!muZEvent) ) 
    return;
  countCutflowEvent("1");
  if ( (eWEvent) && ((muWEvent) || (muZEvent)) ) 
    return;
  countCutflowEvent("2");
  
  //------------------------
  // Calculate required kinematics for control regions, mT~W for single leptons, mll~Z for pair of muons
  // Return if condition not met
  if (eWEvent) {
    double mT = sqrt(2.*isoElecs[0]->PT*missingET->P4().Et()*(1.-cos(isoElecs[0]->P4().DeltaPhi(missingET->P4()))));
    if ((mT<40.) || (mT>100.))
      return;                                                                                                                                                                     
  }
  
  if (muWEvent) {
    double mT = sqrt(2.*isoMuons[0]->PT*missingET->P4().Et()*(1.-cos(isoMuons[0]->P4().DeltaPhi(missingET->P4()))));
    if ((mT<40.) || (mT>100.))
      return;
  }
  if (muZEvent) {
    TLorentzVector diMuon = isoMuons[0]->P4() + isoMuons[1]->P4();
    double mass = (diMuon).M();
    if ((mass<76.) || (mass>116.))
      return; 
    // Dilpton system is used to probe missing energy, we add to missing energy vector
    double newMissPx = diMuon.Px() + missingET->P4().Px();
    double newMissPy = diMuon.Py() + missingET->P4().Py();
    double newMissEt = sqrt((newMissPx*newMissPx)+(newMissPy*newMissPy));
    missingET->P4().SetPxPyPzE(newMissPx, newMissPy, 0., newMissEt);
  }            
  
  //------------------------
  // Cut if too small missing energy
  if (missingET->P4().Et() < 120.0) 
    return;
  countCutflowEvent("3");
  
  //------------------------
  // Cut if no hard central jet
  std::vector<Jet*> cenJets = filterPhaseSpace(jets, 120., -2., 2.);
  if (cenJets.size() < 1)
    return;
  countCutflowEvent("4");
  
  //------------------------
  // veto event if more than two jets > 30 Gev
  std::vector<Jet*> vetoJets = filterPhaseSpace(jets, 30., -4.5, 4.5);
  if (vetoJets.size() > 2)
    return;
  countCutflowEvent("5");    
  
  //------------------------
  // veto event if second jet lies collinear to missing energy
  if (jets.size() > 1) {
    if (fabs(jets[1]->P4().DeltaPhi(missingET->P4())) < 0.5) 
      return;
  }
  countCutflowEvent("6");
  
  
  //--------------------------
  //------Signal Regions------
  //--------------------------
  //---------------------------
  // Signal region 1
  if ((missingET->P4().Et() < 120) || (jets[0]->PT < 120.)) 
    return;
  if (eWEvent)
    countControlEvent(cr[0]+"_"+sr[0]);
  else if (muWEvent)
    countControlEvent(cr[1]+"_"+sr[0]); 
  else if (muZEvent)
    countControlEvent(cr[2]+"_"+sr[0]); 
  
  //---------------------------
  // Signal region 2
  if ((missingET->P4().Et() < 220) || (jets[0]->PT < 220.))
    return;
  if (eWEvent)
    countControlEvent(cr[0]+"_"+sr[1]);
  else if (muWEvent)
    countControlEvent(cr[1]+"_"+sr[1]); 
  else if (muZEvent)
    countControlEvent(cr[2]+"_"+sr[1]);     
  
  //---------------------------
  // Signal region 3
  if ((missingET->P4().Et() < 350) || (jets[0]->PT < 350.))
    return;
  if (eWEvent)
    countControlEvent(cr[0]+"_"+sr[2]);
  else if (muWEvent)
    countControlEvent(cr[1]+"_"+sr[2]); 
  else if (muZEvent)
    countControlEvent(cr[2]+"_"+sr[2]); 
  //---------------------------
  // Signal region 4
  if ((missingET->P4().Et() < 500) || (jets[0]->PT < 500.))
    return;
  if (eWEvent)
    countControlEvent(cr[0]+"_"+sr[3]);
  else if (muWEvent)
    countControlEvent(cr[1]+"_"+sr[3]); 
  else if (muZEvent)
    countControlEvent(cr[2]+"_"+sr[3]); 
}

void Atlas_conf_2012_147_CR::finalize() {
}       
