#include "atlas_conf_2013_049.h"

std::string Atlas_conf_2013_049::id[10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
void Atlas_conf_2013_049::initialize() {
  setInformation(""
  "@#ATLAS\n"
  "@#ATLAS-CONF-2013-049\n"
  "@#2 leptons + etmiss\n"
  "@#sqrt(s) = 8 TeV\n"
  "@#int(L) = 20.3 fb^-1\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  setAnalysisName("atlas_conf_2013_049");          
  ignore("towers");
  ignore("tracks");
  bookSignalRegions("SR_mT2_90_elel;SR_mT2_110_elel;SR_mT2_90_mumu;SR_mT2_110_mumu;SR_mT2_90_elmu;SR_mT2_110_elmu;SR_WWa;SR_WWb;SR_WWc;");
  for (int i=0; i<10 ; i++) {
    if (i<5) {
      bookCutflowRegions("SR90ee"+id[i]+";SR90mumu"+id[i]+";SR90emu"+id[i]+";SR110ee"+id[i]+";SR110mumu"+id[i]+";SR110emu"+id[i]);
    }
    if (i<9) {
      bookCutflowRegions("SRWWc"+id[i]);
    }
    bookCutflowRegions("SRWWa"+id[i]+";SRWWb"+id[i]);
  }
  
  cutTriggerElectronPT = 14.;
  cutTriggerElectronPT1 = 25.;
  cutTriggerElectronPT2 = 8.;
  cutTriggerMuonPT = 14.;
  cutTriggerMuonPT1 = 18.;
  cutTriggerMuonPT2 = 8.;
  cutTriggermixedElectronPT1 = 14.;
  cutTriggermixedMuonPT1 = 8.;
  cutTriggermixedElectronPT2 = 8.;
  cutTriggermixedMuonPT2 = 18.;
  
  hist_minv = new TH1F("minv","",100,0,20);
}

void Atlas_conf_2013_049::analyze() {
  
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 15., -4.5, 4.5);
  
  electronsMedium = filterIsolation(electronsMedium);
  electronsTight = filterIsolation(electronsTight);
  muonsCombined = filterIsolation(muonsCombined);
  
  //remove electrons within 0.1 of any reconstructed electron
  electronsMedium = overlapRemoval(electronsMedium, 0.1);
  electronsTight = overlapRemoval(electronsTight, 0.1);
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  std::vector<Electron*> isoElecsM = overlapRemoval(electronsMedium, muonsCombined, 0.1);
  std::vector<Electron*> isoElecsT = overlapRemoval(electronsTight, muonsCombined, 0.1);
  std::vector<Muon*> isoMuons = overlapRemoval(muonsCombined, electronsMedium, 0.1);
  
  electronsMedium = isoElecsM;
  electronsTight = isoElecsT;
  muonsCombined = isoMuons;
  
  muonsCombined = overlapRemoval(muonsCombined, 0.05);
  
  missingET->addMuons(muonsCombined);
  
  countCutflowEvent("SR90ee"+id[0]);
  countCutflowEvent("SR90mumu"+id[0]);
  countCutflowEvent("SR90emu"+id[0]);
  countCutflowEvent("SR110ee"+id[0]);
  countCutflowEvent("SR110mumu"+id[0]);
  countCutflowEvent("SR110emu"+id[0]);
  countCutflowEvent("SRWWa"+id[0]);
  countCutflowEvent("SRWWb"+id[0]);
  countCutflowEvent("SRWWc"+id[0]);
  countCutflowEvent("SR90ee"+id[0]);
  
  if ( jets.size() > 0 )
    hist_minv->Fill(jets[0]->PT, weight);
  
  //if( electronsMedium.size() + muonsCombined.size() != 2 )
  //  return;
  if ( electronsTight.size() + muonsCombined.size() != 2 )
    return;
  
  countCutflowEvent("SRWWa"+id[1]);
  countCutflowEvent("SRWWb"+id[1]);
  countCutflowEvent("SRWWc"+id[1]);
  
  // Trigger Cuts
  trigger = false;
  double triggerRatio = (double) rand() / (double) (RAND_MAX + 1.);
  
  // Asymmetric dielectron trigger
  if ( (electronsTight.size() > 1) && (electronsTight[0]->PT > cutTriggerElectronPT1) && (electronsTight[1]->PT > cutTriggerElectronPT2) && (triggerRatio < 0.85) )
    trigger = true;    
  // Symmetric dielectron trigger
  else if ( (electronsTight.size() > 1) && (electronsTight[0]->PT > cutTriggerElectronPT) && (electronsTight[1]->PT > cutTriggerElectronPT) && (triggerRatio < 0.98 ) )
    trigger = true;    
  // Asymmetric dimuon trigger
  else if ( (muonsCombined.size() > 1) && (muonsCombined[0]->PT > cutTriggerMuonPT1) && (muonsCombined[1]->PT > cutTriggerMuonPT2) && (triggerRatio < 0.8 ) )
    trigger=true;    
  // Symmetric dimuon trigger
  else if ( (muonsCombined.size() > 1) && (muonsCombined[0]->PT > cutTriggerMuonPT)  && (muonsCombined[1]->PT > cutTriggerMuonPT) &&  (triggerRatio < 0.52 ) )
    trigger=true;    
  // Mixed trigger
  else if ( (muonsCombined.size() > 0) && (electronsTight.size() > 0) && (electronsTight[0]->PT > cutTriggermixedElectronPT1) && (muonsCombined[0]->PT > cutTriggermixedMuonPT1) && (triggerRatio < 0.65) )
    trigger=true;    
  // Mixed trigger
  else if ( (muonsCombined.size() > 0) && (electronsTight.size() > 0) && (electronsTight[0]->PT > cutTriggermixedElectronPT2) && (muonsCombined[0]->PT > cutTriggermixedMuonPT2) && (triggerRatio < 0.65 ) )
    trigger=true;
  
  // Calculation of etmiss,rel    
  deltaphi_min = 1000.;
  deltaphi_temp = 10000.;
  
  for ( int i = 0; i < jets.size(); i++ ) {
    deltaphi_temp = fabs( jets[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
  }
  for ( int i = 0; i < muonsCombined.size(); i++ ) {
    deltaphi_temp = fabs( muonsCombined[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
  }
  for ( int i = 0; i < electronsTight.size(); i++ ) {
    deltaphi_temp = fabs( electronsTight[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
  }
  if ( deltaphi_min < 1.57079632679 )
    missingETrel = missingET->P4().Et()*sin(deltaphi_min);
  else
    missingETrel = missingET->P4().Et();
  
  
  std::vector<Jet*> signalJets;
  for (int j = 0; j < jets.size(); j++) {
    if ( (jets[j]->PT > 15.) && (fabs( jets[j]->Eta ) < 2.4) )
      signalJets.push_back(jets[j]);
    else if ( (jets[j]->PT > 22.5) && (fabs( jets[j]->Eta ) > 2.4) && ( fabs( jets[j]->Eta ) < 4.5 ) )
      signalJets.push_back(jets[j]);
  }      
  jets = signalJets;
  
  
  //--------------------------
  //------Signal Regions------
  //--------------------------
  if ( (electronsTight.size() == 2) && (electronsTight[0]->Charge*electronsTight[1]->Charge < 0) && trigger ) {
    mt2 = mT2( electronsTight[0]->P4(), electronsTight[1]->P4(), 0.);
    mll = ( electronsTight[0]->P4() + electronsTight[1]->P4() ).M();
    ptll = ( electronsTight[0]->P4() + electronsTight[1]->P4() ).Pt();
    deltaphi = fabs( electronsTight[0]->P4().DeltaPhi(electronsTight[1]->P4()));
    bool minv_flag = false;
    if ( mll > 20. ) {
      minv_flag = true;
      countCutflowEvent("SR90ee"+id[1]);
      countCutflowEvent("SR110ee"+id[1]);
    }
    
    bool zveto_flag = false;
    if ( minv_flag && fabs( mll-91.118 ) > 10. ) {
      zveto_flag = true;
      countCutflowEvent("SR90ee"+id[2]);
      countCutflowEvent("SR110ee"+id[2]);
    }
    
    bool jetveto_flag = false;
    if ( zveto_flag && jets.size() == 0 ) {
      jetveto_flag = true;
      countCutflowEvent("SR90ee"+id[3]);
      countCutflowEvent("SR110ee"+id[3]);
    }
    
    bool met_flag = false;
    if (  jetveto_flag && missingETrel > 40. ) {
      countCutflowEvent("SR90ee"+id[4]);
      countCutflowEvent("SR110ee"+id[4]);
      met_flag = true;
    }
    
    if (  met_flag  && mt2 > 90. )
      countSignalEvent("SR_mT2_90_elel");
    
    if ( met_flag && mt2 > 110. )
      countSignalEvent("SR_mT2_110_elel");
  }
  
  if ( (muonsCombined.size() == 2) && (muonsCombined[0]->Charge*muonsCombined[1]->Charge < 0) && trigger ) {
    mt2 = mT2( muonsCombined[0]->P4(), muonsCombined[1]->P4(), 0.);
    mll = ( muonsCombined[0]->P4() + muonsCombined[1]->P4() ).M();
    ptll = ( muonsCombined[0]->P4() + muonsCombined[1]->P4() ).Pt();
    deltaphi = fabs( muonsCombined[0]->P4().DeltaPhi(muonsCombined[1]->P4()) );
    
    bool minv_flag = false;
    if ( mll > 20. ) {
      countCutflowEvent("SR90mumu"+id[1]);
      countCutflowEvent("SR110mumu"+id[1]);
      minv_flag = true;
    }
    
    bool zveto_flag = false;
    if ( minv_flag && fabs( mll-91.118 ) > 10. )  {
      countCutflowEvent("SR90mumu"+id[2]);
      countCutflowEvent("SR110mumu"+id[2]);
      zveto_flag = true;
    }
    
    bool jetveto_flag = false;
    if ( zveto_flag && jets.size() == 0 ) {
      countCutflowEvent("SR90mumu"+id[3]);
      countCutflowEvent("SR110mumu"+id[3]);
      jetveto_flag = true;
    }
    
    bool met_flag = false;
    if ( jetveto_flag && missingETrel > 40. ) {
      countCutflowEvent("SR90mumu"+id[4]);
      countCutflowEvent("SR110mumu"+id[4]);
      met_flag = true;
    }
    if ( met_flag && mt2 > 90. )
      countSignalEvent("SR_mT2_90_mumu");
    
    if ( met_flag && mt2 > 110. )
      countSignalEvent("SR_mT2_110_mumu");
  }
  
  if ( (electronsTight.size() == 1) && (muonsCombined.size() == 1) &&  (electronsTight[0]->Charge*muonsCombined[0]->Charge < 0) && trigger ) {
    mt2 = mT2( electronsTight[0]->P4(), muonsCombined[0]->P4(), 0.);
    mll = ( electronsTight[0]->P4() + muonsCombined[0]->P4() ).M();
    ptll = ( electronsTight[0]->P4() + muonsCombined[0]->P4() ).Pt();
    deltaphi = fabs( electronsTight[0]->P4().DeltaPhi(muonsCombined[0]->P4()));
    
    bool minv_flag = false;
    if ( mll > 20.) {
      countCutflowEvent("SR90emu"+id[1]);
      countCutflowEvent("SR110emu"+id[1]);
      minv_flag = true;
    }      
    
    bool zveto_flag = false;
    if (  minv_flag && (fabs( mll-91.118 ) > 10.) ) {
      countCutflowEvent("SR90emu"+id[2]);
      countCutflowEvent("SR110emu"+id[2]);
      zveto_flag = true;
    }
    
    bool jetveto_flag = false;
    if ( zveto_flag && (jets.size() == 0) ) {
      countCutflowEvent("SR90emu"+id[3]);
      countCutflowEvent("SR110emu"+id[3]);
      jetveto_flag = true;
    }      
    
    bool met_flag = false;
    if ( jetveto_flag && (missingETrel > 40.) ) {
      countCutflowEvent("SR90emu"+id[4]);
      countCutflowEvent("SR110emu"+id[4]);
      met_flag = true;
    }
    
    if ( met_flag && (mt2 > 90.) )
      countSignalEvent("SR_mT2_90_elmu");
    if ( met_flag && (mt2 > 110.) )
      countSignalEvent("SR_mT2_110_elmu");
  }
  
  //SRWWa, SRWWb, SRWWc    
  bool trigger_flag = false;
  
  if ( trigger ) {
    countCutflowEvent("SRWWa"+id[2]);
    countCutflowEvent("SRWWb"+id[2]);
    countCutflowEvent("SRWWc"+id[2]);
    trigger_flag = true;
  }
  
  if ( (trigger_flag) && (electronsTight.size() == 1) && (muonsCombined.size() == 1) && (electronsTight[0]->Charge*muonsCombined[0]->Charge < 0) ) {      
    mt2 = mT2( electronsTight[0]->P4(), muonsCombined[0]->P4(), 0.);
    mll = ( electronsTight[0]->P4() + muonsCombined[0]->P4() ).M();
    ptll = ( electronsTight[0]->P4() + muonsCombined[0]->P4() ).Pt();
    deltaphi = fabs( electronsTight[0]->P4().DeltaPhi(muonsCombined[0]->P4()));
    bool minv_flag = false;
    if ( mll > 20. ) {
      countCutflowEvent("SRWWa"+id[3]);
      countCutflowEvent("SRWWb"+id[3]);
      countCutflowEvent("SRWWc"+id[3]);
      minv_flag = true;
    }            
    
    bool jetveto_flag = false;
    if ( minv_flag && (jets.size() == 0 )) {
      countCutflowEvent("SRWWa"+id[4]);
      countCutflowEvent("SRWWb"+id[4]);
      countCutflowEvent("SRWWc"+id[4]);
      jetveto_flag = true;
    }    
    
    bool ptlepton_flag = false;
    if ( jetveto_flag && ( ( electronsTight[0]->PT > 35. && muonsCombined[0]->PT > 20. ) || ( electronsTight[0]->PT > 20. && muonsCombined[0]->PT > 35. ) )  ) {
      countCutflowEvent("SRWWa"+id[5]);
      countCutflowEvent("SRWWb"+id[5]);
      countCutflowEvent("SRWWc"+id[5]);
      ptlepton_flag = true;
    }
    
    if ( ptlepton_flag && (mll < 80.)) {
      countCutflowEvent("SRWWa"+id[6]);
      if (ptll > 70.) {
    countCutflowEvent("SRWWa"+id[7]);
    if (missingETrel > 70.) {
      countCutflowEvent("SRWWa"+id[8]);
      if (deltaphi < 1.8) {
        countCutflowEvent("SRWWa"+id[9]);
        countSignalEvent("SR_WWa");
      }
    }
      }
    }
    
    if ( ptlepton_flag && (mt2 > 90.) ){
      countCutflowEvent("SRWWb"+id[6]);
      if (mll < 130.) { 
    countCutflowEvent("SRWWb"+id[7]);
    if (ptll < 170.) {
      countCutflowEvent("SRWWb"+id[8]);
      if (deltaphi < 1.8) {
        countCutflowEvent("SRWWb"+id[9]);
        countSignalEvent("SR_WWb");
      }
    }
      }
    }
    
    if ( ptlepton_flag && (ptll < 190.) ) {
      countCutflowEvent("SRWWc"+id[6]);
      if (mt2 > 100.) {
    countCutflowEvent("SRWWc"+id[7]);
    if (deltaphi < 1.8) {
      countCutflowEvent("SRWWc"+id[8]);
      countSignalEvent("SR_WWc");
    }
      }
    }
  }
}

void Atlas_conf_2013_049::finalize() {
}       
