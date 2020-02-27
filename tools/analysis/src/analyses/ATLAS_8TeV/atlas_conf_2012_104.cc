#include "atlas_conf_2012_104.h"

void Atlas_conf_2012_104::initialize() {
  setInformation(""
    "@#ATLAS\n"
     "@#ATLAS-CONF-2012-104\n"
     "@#1 lepton + >= 4 jets + etmiss\n"
     "@#sqrt(s) = 8 TeV\n"
     "@#int(L) = 5.8 fb^-1\n"
  "");
  setLuminosity(5.8*units::INVFB);      
  setAnalysisName("atlas_conf_2012_104");          
  ignore("towers");
  ignore("tracks");
  bookSignalRegions("el;mu;");
  bookCutflowRegions("0;1;2;3;4;5;6;7");
    
  mEff = mEff_inc = sumjetpt = sumjetpt_4 = mT = deltaphi = 0.;  
  cutTriggerElectronPT_comb = 24.;
  cutTriggerElectron_ETmiss_comb = 35.;
  cutTriggerElectronPT = 60.;
  cutTriggerMuonPT = 24.;
  cutTriggerMuon_JetPT = 65.;
  cutTriggerMuon_ETmiss = 40.;
}

void Atlas_conf_2012_104::analyze() {
  missingET->addMuons(muonsCombinedPlus);
  std::vector<Muon*>muonsCombinedPlusVeto;
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 10., -2.4, 2.4);
  muonsCombinedPlusVeto = muonsCombinedPlus;
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 25., -2.4, 2.4);
  muonsCombinedPlus = filterIsolation(muonsCombinedPlus);
  electronsTight = filterPhaseSpace(electronsTight, 25., -2.47, 2.47);
  electronsTight = filterIsolation(electronsTight);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
  countCutflowEvent("0");

  // Remove jets that overlap with electrons
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  muonsCombinedPlus = overlapRemoval(muonsCombinedPlus, jets, 0.4);
  muonsCombinedPlusVeto = overlapRemoval(muonsCombinedPlusVeto, jets, 0.4);
    
  // Trigger Cuts
  // I assume 100% trigger efficiency for the electrons and 90% for the muons  
  trigger = false;
  double triggerRatio = (double) rand() / (double) RAND_MAX;
  if ( (electronsTight.size() > 0) && (electronsTight[0]->PT > cutTriggerElectronPT_comb) && (missingET->P4().Et() > cutTriggerElectron_ETmiss_comb) && (triggerRatio <= 1.) )
    trigger = true;
  else if ( (electronsTight.size() > 0) && (electronsTight[0]->PT > cutTriggerElectronPT)  && (triggerRatio <= 1.) )
    trigger = true;
  else if ( (muonsCombinedPlus.size() > 0) && (muonsCombinedPlus[0]->PT > cutTriggerMuonPT) && (jets.size() > 0) && (jets[0]->PT > cutTriggerMuon_JetPT) && (missingET->P4().Et() > cutTriggerMuon_ETmiss) && (triggerRatio <= 1.0) )
    trigger = true;
  if ( ( electronsTight.size() + muonsCombinedPlus.size() != 1 ) || !trigger )
    return;
  if ( electronsMedium.size() + muonsCombinedPlusVeto.size() != 1)
    return;  
  countCutflowEvent("1");


  //--------------------------
  //------Signal Regions------
  //--------------------------
  if ( (electronsTight.size() > 0) && (electronsTight[0]->PT < 25.) )
    return;
  if ( (muonsCombinedPlus.size() > 0) && (muonsCombinedPlus[0]->PT < 25.) )
    return;  
  countCutflowEvent("2");

  jets = filterPhaseSpace(jets, 25, -2.5, 2.5);    
  // Demand 4 jets with PT > 80 GeV
  if ( jets.size() < 4 || jets[3]->PT < 80. )
    return;  
  countCutflowEvent("3");
  
  if ( missingET->P4().Et() < 250. )
    return;  
  countCutflowEvent("4");
  
  sumjetpt_4 = 0.;
  sumjetpt = 0.;  
  for (int i=0; i<jets.size(); i++) {
    // pt sum of the 4 leading jets    
    if ( i < 4 )
      sumjetpt_4 += jets[i]->PT;    
    if ( jets[i]->PT > 40. )
      sumjetpt += jets[i]->PT;
  }  
  
  // Calculation of meff and kinematic lepton variables
  if ( electronsTight.size() == 1 ) {
    mEff = electronsTight[0]->PT + missingET->P4().Et() + sumjetpt_4;
    mEff_inc = electronsTight[0]->PT + missingET->P4().Et() + sumjetpt;
    deltaphi = fabs(electronsTight[0]->P4().DeltaPhi(missingET->P4()));
    mT = sqrt(2.*electronsTight[0]->PT*missingET->P4().Et()*(1.-cos(deltaphi)));
  }
  else if ( muonsCombinedPlus.size() == 1 ) {
    mEff = muonsCombinedPlus[0]->PT + missingET->P4().Et() + sumjetpt_4;
    mEff_inc = muonsCombinedPlus[0]->PT + missingET->P4().Et() + sumjetpt;
    deltaphi = fabs(muonsCombinedPlus[0]->P4().DeltaPhi(missingET->P4()));
    mT = sqrt(2.*muonsCombinedPlus[0]->PT*missingET->P4().Et()*(1.-cos(deltaphi)));
  }

  // Cut on transverse mass of lepton
  if ( mT < 100. )
    return;
  countCutflowEvent("5");
  
  // Cut on effective mass
  if ( mEff_inc < 800. )
    return;  
  countCutflowEvent("6");
    
  // Cut on ratio of missing transverse energy and effective mass
  if ( missingET->P4().Et()/mEff < 0.2 )
      return;    
  countCutflowEvent("7");
  
  //Electron signal channel
  if ( electronsTight.size() == 1 ) {
    countSignalEvent("el");
  }
  
  //Muon signal channel
  if ( muonsCombinedPlus.size() == 1 ) {
    countSignalEvent("mu");
  }
}

void Atlas_conf_2012_104::finalize() {
}       
