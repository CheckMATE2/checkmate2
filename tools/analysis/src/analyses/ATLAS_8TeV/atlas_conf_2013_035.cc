#include "atlas_conf_2013_035.h"

std::string Atlas_conf_2013_035::cf_index[9] = {"0","1","2","3","4","5","6","7","8"};

void Atlas_conf_2013_035::initialize() {
  setInformation(""
  "@#ATLAS\n"
  "@#ATLAS-CONF-2013-035\n"
  "@#3 leptons + etmiss\n"
  "@#sqrt(s) = 8 TeV\n"
  "@#int(L) = 20.7 fb^-1\n"
  "");
  setLuminosity(20.7*units::INVFB);      
  setAnalysisName("atlas_conf_2013_035");          
  ignore("towers");
  ignore("tracks");
  bookSignalRegions("SRnoZa;SRnoZb;SRnoZc;SRZa;SRZb;SRZc;");
  bookCutflowRegions("0;1;2;3;4;5;6;7");
  for (int i=0; i<9; i++)
    bookCutflowRegions("SRnoZa"+cf_index[i]+";SRnoZb"+cf_index[i]+";SRnoZc"+cf_index[i]+";SRZa"+cf_index[i]+";SRZb"+cf_index[i]+";SRZc"+cf_index[i]);
  deltaphi = 0.;
  
  cutSingleTriggerElectronPT = 25.;
  cutSymTriggerElectronPT = 14.;
  cutAsymTriggerElectronPT1 = 25.;
  cutAsymTriggerElectronPT2 = 10.;
  cutMixedTriggerElectronPT1 = 14.;
  cutMixedTriggerElectronPT2 = 10.;
  
  cutSingleTriggerMuonPT = 25.;
  cutSymTriggerMuonPT = 14.0;
  cutAsymTriggerMuonPT1 = 18.0;
  cutAsymTriggerMuonPT2 = 10.;
  cutMixedTriggerMuonPT1 = 10.;
  cutMixedTriggerMuonPT2 = 18.;
  
  //    ignore("electrons_looseIsolation");
  //    ignore("muonsCombinedPlus");
  
}

void Atlas_conf_2013_035::analyze() {
  missingET->addMuons(muonsCombined);  
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 20., -2.5, 2.5);
    
  countCutflowEvent("SRnoZa"+cf_index[0]);
  countCutflowEvent("SRnoZb"+cf_index[0]);
  countCutflowEvent("SRnoZc"+cf_index[0]);
  countCutflowEvent("SRZa"+cf_index[0]);
  countCutflowEvent("SRZb"+cf_index[0]);
  countCutflowEvent("SRZc"+cf_index[0]);
  
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
  
  electronsMedium=isoElecsM;
  electronsTight=isoElecsT;
  muonsCombined=isoMuons;
  
  electronsMedium=filterIsolation(electronsMedium,0);
  electronsTight=filterIsolation(electronsTight,1);
  muonsCombined=filterIsolation(muonsCombined);
  
  trigger = false;
  double random_trigger = (double) rand() / (double) (RAND_MAX + 1.);
  //    Trigger Cuts
  // single electron trigger
  if( electronsTight.size() > 0 && electronsTight[0]->PT > cutSingleTriggerElectronPT && random_trigger < 1.0)
    trigger = true;
  // single muon trigger
  else if( muonsCombined.size() > 0 && muonsCombined[0]->PT > cutSingleTriggerMuonPT  && random_trigger < 0.9 ) 
    trigger = true;
  //symmetric dielectron trigger
  else if( electronsTight.size() > 1 && electronsTight[0]->PT > cutSymTriggerElectronPT && electronsTight[1]->PT > cutSymTriggerElectronPT  && random_trigger < 0.98 )
    trigger = true;
  //asymmetric dielectron trigger
  else if( electronsTight.size() > 1 && electronsTight[0]->PT > cutAsymTriggerElectronPT1 && electronsTight[1]->PT > cutAsymTriggerElectronPT2 && random_trigger < 0.85 )
    trigger = true;
  //symmetric dimuon trigger
  else if( muonsCombined.size() > 1 && muonsCombined[0]->PT > cutSymTriggerMuonPT && muonsCombined[1]->PT > cutSymTriggerMuonPT && random_trigger < 0.52 )
    trigger = true;
  
  //asymmetric dimuon trigger
  else if( muonsCombined.size() > 1 && muonsCombined[0]->PT > cutAsymTriggerMuonPT1 && muonsCombined[1]->PT > cutAsymTriggerMuonPT2 && random_trigger < 0.8)
    trigger=true;
  //mixed electron-muon trigger
  else if( electronsTight.size() > 0 && muonsCombined.size() > 0 && electronsTight[0]->PT && cutMixedTriggerElectronPT1 && muons[0]->PT > cutMixedTriggerMuonPT1 && random_trigger < 0.65 )
    trigger = true;
  //mixed electron-muon trigger
  else if( electronsTight.size() > 0 && muonsCombined.size() > 0 && electronsTight[0]->PT && cutMixedTriggerElectronPT2 && muonsCombined[0]->PT > cutMixedTriggerMuonPT2 && random_trigger < 0.65 )
    trigger = true;


  
  //object removal is applied
  
  //--------------------------
  //------Signal Regions------
  //--------------------------
  if( ( electronsTight.size() + muonsCombined.size() ) != 3 || !trigger )
    return;
  if( ( electronsMedium.size() + muonsCombined.size() ) != 3 )
    return;
  
  
  countCutflowEvent("SRnoZa"+cf_index[1]);
  countCutflowEvent("SRnoZb"+cf_index[1]);
  countCutflowEvent("SRnoZc"+cf_index[1]);
  countCutflowEvent("SRZa"+cf_index[1]);
  countCutflowEvent("SRZb"+cf_index[1]);
  countCutflowEvent("SRZc"+cf_index[1]);
  
  //SFOS invariant mass calculation
  
  mmin = 100000.;
  SFOS = false;
  thirdleptonpt = 10000.;


  
  int ei,ej;
  ei=10;
  ej=10;
  
  if(electronsTight.size() > 1){
    for (int i = 0; i < electronsTight.size(); i++) {
      if( electronsTight[i]->PT < thirdleptonpt ) 
    thirdleptonpt = electronsTight[i]->PT;
      for (int j = i+1; j < electronsTight.size(); j++) {
    if(electronsTight[i]->Charge*electronsTight[j]->Charge > 0) 
      continue;
    mtest = (electronsTight[i]->P4() + electronsTight[j]->P4()).M();
    SFOS = true;
    if (mtest<12.) 
      return;
    if (fabs(mtest-91.2) < mmin) {
      mSFOS = mtest;
      mmin = fabs(mtest-91.2);
      ei=i;
      ej=j;
    }
      }
    }
  }

  
  int mi,mj;
  mi = 10;
  mj = 10;

  if(muonsCombined.size() > 1){
    for (int i = 0; i < muonsCombined.size(); i++) {
      if ( muonsCombined[i]->PT < thirdleptonpt ) 
    thirdleptonpt = muonsCombined[i]->PT;
      for(int j = i+1; j < muonsCombined.size(); j++) {
    if (muonsCombined[i]->Charge*muonsCombined[j]->Charge > 0)
      continue;
    mtest = (muonsCombined[i]->P4() + muonsCombined[j]->P4()).M();
    if (mtest < 12.) 
      return;
    SFOS = true;
    if (fabs(mtest-91.2)<mmin) {
      mSFOS = mtest;
      mmin = fabs(mtest-91.2);
      mi = i;
      mj = j;
    }
      }
    }
  }


  if (!SFOS) 
    return;
  
  
  countCutflowEvent("SRnoZa"+cf_index[2]);
  countCutflowEvent("SRnoZb"+cf_index[2]);
  countCutflowEvent("SRnoZc"+cf_index[2]);
  countCutflowEvent("SRZa"+cf_index[2]);
  countCutflowEvent("SRZb"+cf_index[2]);
  countCutflowEvent("SRZc"+cf_index[2]);
  
  
  
  for (int i = 0; i < jets.size(); i++) {
    if( checkBTag(jets[i]))
      return;
  }
  
  countCutflowEvent("SRnoZa"+cf_index[3]);
  countCutflowEvent("SRnoZb"+cf_index[3]);
  countCutflowEvent("SRnoZc"+cf_index[3]);
  countCutflowEvent("SRZa"+cf_index[3]);
  countCutflowEvent("SRZb"+cf_index[3]);
  countCutflowEvent("SRZc"+cf_index[3]);
  
  for (int i = 0; i < electronsTight.size(); i++) {    
    if ( i != ei && i != ej ){
      deltaphi = fabs(electronsTight[i]->P4().DeltaPhi(missingET->P4()));
      mT = sqrt(2.*electronsTight[i]->PT*missingET->P4().Et()*(1.-cos(deltaphi)));
    }
  }
  
  for (int i = 0; i < muonsCombined.size(); i++) {
    if (i != mi && i != mj ) {
      deltaphi = fabs(muonsCombined[i]->P4().DeltaPhi(missingET->P4()));
      mT = sqrt(2.*muonsCombined[i]->PT*missingET->P4().Et()*(1.-cos(deltaphi)));
    }
  }
  
  //Signal Regions
  if (mmin > 10) {    
    countCutflowEvent("SRnoZa"+cf_index[4]);
    countCutflowEvent("SRnoZb"+cf_index[4]);
    countCutflowEvent("SRnoZc"+cf_index[4]);
  
    if ( missingET->P4().Et() > 50. ) {
    countCutflowEvent("SRnoZa"+cf_index[5]);
      if( mSFOS < 60. ){
        countCutflowEvent("SRnoZa"+cf_index[6]);
        countCutflowEvent("SRnoZa"+cf_index[7]);
        countCutflowEvent("SRnoZa"+cf_index[8]);
        bool SRZcveto = true;
        if(missingET->P4().Et() > 75. && mT > 110. && thirdleptonpt > 30. && (mSFOS < 81.2 || mSFOS > 101.2 ) ) 
          SRZcveto = false;
        if(SRZcveto)
          countSignalEvent("SRnoZa");
      }
    }
    
    if ( missingET->P4().Et() > 75. ) {
      countCutflowEvent("SRnoZb"+cf_index[5]);
      if( mSFOS >= 60. && mSFOS < 81.2 ){
        countCutflowEvent("SRnoZb"+cf_index[6]);
        countCutflowEvent("SRnoZb"+cf_index[7]);
        countCutflowEvent("SRnoZb"+cf_index[8]);
        bool SRZcveto = true;
        if(missingET->P4().Et() > 75. && mT > 110. && thirdleptonpt > 30. && (mSFOS < 81.2 || mSFOS > 101.2 ) ) 
          SRZcveto = false;
        if( SRZcveto )
          countSignalEvent("SRnoZb");
      }
    }
    
    if( missingET->P4().Et() > 75. ){
      countCutflowEvent("SRnoZc"+cf_index[5]);
      if ( mSFOS < 81.2 || mSFOS > 101.2 ) {
        countCutflowEvent("SRnoZc"+cf_index[6]);
        bool flag_mT = false;
        if( mT > 110. ){
          countCutflowEvent("SRnoZc"+cf_index[7]);
          flag_mT = true;
        }
        if(flag_mT == true && thirdleptonpt > 30. ){
          countCutflowEvent("SRnoZc"+cf_index[8]);
          countSignalEvent("SRnoZc");
        }
      }
    }
  }  

  else if( mmin < 10 ) {
    countCutflowEvent("SRZa"+cf_index[4]);
    countCutflowEvent("SRZb"+cf_index[4]);
    countCutflowEvent("SRZc"+cf_index[4]);
    
    if ( missingET->P4().Et() > 75. && missingET->P4().Et() < 120. ) {
      countCutflowEvent("SRZa"+cf_index[5]);
      if ( mSFOS > 81.2 && mSFOS < 101.2) {
        countCutflowEvent("SRZa"+cf_index[6]);
        bool flag_mT = false;
        if ( mT < 110. ) {
          countCutflowEvent("SRZa"+cf_index[7]);
          flag_mT = true;
        }
        if (flag_mT == true && thirdleptonpt > 10. ) {
          countCutflowEvent("SRZa"+cf_index[8]);
          countSignalEvent("SRZa");
        }        
      }
    }
    
    if ( missingET->P4().Et() > 75. && missingET->P4().Et() < 120. ) {
      countCutflowEvent("SRZb"+cf_index[5]);
      if ( mSFOS > 81.2 && mSFOS < 101.2) {
        countCutflowEvent("SRZb"+cf_index[6]);
        bool flag_mT = false;
        if ( mT > 110. ) {
          countCutflowEvent("SRZb"+cf_index[7]);
          flag_mT = true;
        }
        if (flag_mT && thirdleptonpt > 10. ) {
          countCutflowEvent("SRZb"+cf_index[8]);
          countSignalEvent("SRZb");
        }
        
      }
    }
    if ( missingET->P4().Et() > 120. ) {
      countCutflowEvent("SRZc"+cf_index[5]);
      if ( mSFOS > 81.2 && mSFOS < 101.2) {
        countCutflowEvent("SRZc"+cf_index[6]);
        bool flag_mT = false;
        if ( mT > 110. ) {
          countCutflowEvent("SRZc"+cf_index[7]);
          flag_mT = true;
        }
        if (flag_mT && thirdleptonpt > 10. ) {
          countCutflowEvent("SRZc"+cf_index[8]);
          countSignalEvent("SRZc");
        }
        
      }
    }
  }
}

void Atlas_conf_2013_035::finalize() {
}       
