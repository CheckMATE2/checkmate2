#include "atlas_1407_0583.h"

std::string Atlas_1407_0583::ind_CF[2] = {"f_Baseline", "g_Signal"};
std::string Atlas_1407_0583::name_CF[5] = {"Hard", "bCb_med1", "bCb_high", "bCa_med", "bCa_low"};
std::string Atlas_1407_0583::lep[2] = {"El", "Mu"};

std::string Atlas_1407_0583::hard_CF[2] = {"h_3jets", "i_4jets"};

std::string Atlas_1407_0583::bCa_Num[8] = {"00_softLep", "01_jetMult25", "02_jet180", "03_bTag", "04_softBTag", "05_mT", "06_missEff", "07_softLep"};
std::string Atlas_1407_0583::bCb_Num[8] = {"00_softLep", "01_jet1_60", "02_jet2_60", "03_2bTag", "04_deltaPhi", "05_HT", "06_mbb", "07_amT2"};

std::string Atlas_1407_0583::tN_diag_Num[11] = {"00_jet1", "01_jet2", "02_jet3", "03_bTag", "04_dPhiJet1", "05_dPhiJet2", "06_EtMiss", "07_met/ht", "08_mT", "09_hadTop", "10_tauVeto"};
std::string Atlas_1407_0583::bCc_diag_Num[11] = {"00_jet1", "01_jet2", "02_jet3", "03_noBtag", "04_lepEta", "05_dPhi1", "06_dPhi2", "07_dR", "08_Met", "09_met/ht", "10_mT"};
std::string Atlas_1407_0583::tN_med_Num[10] = {"00_jet1", "01_jet2", "02_jet3", "03_bTag", "04_dPhiJet2", "05_EtMiss", "06_missHT_Sig", "07_mT", "08_hadTop", "09_aMT2"};
std::string Atlas_1407_0583::tN_high_Num[11] = {"00_jet1", "01_jet2", "02_jet3", "03_bTag", "04_dRLepBJet", "05_EtMiss", "06_missHT_Sig", "07_mT", "08_hadTop", "09_aMT2", "10_MT2_Tau"};
std::string Atlas_1407_0583::bCb_med2_Num[14] = {"00_jet1", "01_jet2", "02_jet3", "03_bTag", "04_bJet1PT", "05_bJet2PT", "06_dPhiJet1", "07_dPhiJet2", "08_EtMiss", "09_met/ht", "10_mT", "11_aMT2", "12_trackIso", "13_tauVeto"};
std::string Atlas_1407_0583::bCd_bulk_Num[12] = {"00_jet1", "01_jet2", "02_jet3", "03_bTag", "04_dPhiJet1", "05_dPhiJet2", "06_EtMiss", "07_met/ht", "08_mT", "09_aMT2", "10_trackIso", "11_tauVeto"};
std::string Atlas_1407_0583::bCd_high1_Num[15] = {"00_jet1", "01_jet2", "02_jet3", "03_bTag", "04_bJet1PT", "05_bJet2PT", "06_dPhiJet1", "07_dPhiJet2", "08_EtMiss", "09_met/ht", "10_mT", "11_meff", "12_aMT2", "13_isoTrack", "14_tauVeto"};
std::string Atlas_1407_0583::bCd_high2_Num[15] = {"00_jet1", "01_jet2", "02_jet3", "03_bTag", "04_bJet1PT", "05_bJet2PT", "06_dPhiJet1", "07_dPhiJet2", "08_EtMiss", "09_met/ht", "10_dummy", "11_dummy", "12_aMT2", "13_isoTrack", "14_tauVeto"};
std::string Atlas_1407_0583::threeBody_Num[14] = {"00_jet1", "01_jet2", "02_jet3", "03_bTag", "04_dPhiJet1", "05_dPhiJet2", "06_dPhiLepETMiss", "07_dRLepJet1", "08_dRLepJet2", "09_EtMiss", "10_met/ht", "11_mT", "12_aMT2", "13_tauVeto"};
std::string Atlas_1407_0583::tNbC_mix_Num[13] = {"00_jet1", "01_jet2", "02_jet3", "03_bTag", "04_bJetPT", "05_dPhiJet1", "06_dPhiJet2", "07_dPhiLepETMiss", "08_dRLepJet1", "09_dRLepBJet", "10_EtMiss", "11_met/ht", "12_mT"};


void Atlas_1407_0583::initialize() {
  setAnalysisName("atlas_1407_0583");          
  setInformation(""
    "@#ATLAS\n"
     "@#arXiv:1407.0583\n"
     "@#1-lepton + (b-)jets + etmiss\n"
     "@#sqrt(s) = 8 TeV\n"
     "@#int(L) = 20.3 fb^-1\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  //ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("tN_med;tN_high;tN_boost;bCa_low;bCa_med;bCb_med1;bCb_high;bCc_diag;bCd_high1;bCd_high2;tNbC_mix;tN_diag_a;tN_diag_b;tN_diag_c;tN_diag_d;bCb_med2_a;bCb_med2_b;bCb_med2_c;bCb_med2_d;bCd_bulk_a;bCd_bulk_b;bCd_bulk_c;bCd_bulk_d;threeBody_a;threeBody_b;threeBody_c;threeBody_d;");
  bookCutflowRegions("a;b_trigger;b_trigger_EtMiss;c_DQ;c_DQ_ETMiss");
  bookCutflowRegions("e_150;e_250;e_300;e_370");
  
  //Book initial loop cutflows here
  for(int i = 0; i < 2; i++){
    std::string CFi = ind_CF[i];
    for(int j = 0; j < 5; j++){
      std::string CFj = CFi+"_"+name_CF[j];
      for(int k = 0; k < 2; k++){
    std::string CFk = CFj+"_"+lep[k];
    bookCutflowRegions(CFk);
      }
    }
  }
  
  //Book bCa cutflows here
  for(int i = 3; i < 5; i++){
    std::string CFi = name_CF[i];
    for(int j = 0; j < 2; j++){
      std::string CFj = CFi+"_"+lep[j];
      for(int k = 0; k < 8; k++){
    std::string CFk = CFj+"_"+bCa_Num[k];
    bookCutflowRegions(CFk);
      }
    }
  }
  
  //Book bCb cutflows here
  for(int i = 1; i < 3; i++){
    std::string CFi = name_CF[i];
    for(int j = 0; j < 2; j++){
      std::string CFj = CFi+"_"+lep[j];
      for(int k = 0; k < 7; k++){
    std::string CFk = CFj+"_"+bCb_Num[k];
    bookCutflowRegions(CFk);
      }
    }
  }
  
  //Book all other cutflows here
  
  for(int i = 0; i < 2; i++){
    for(int j = 0; j < 11; j++) bookCutflowRegions("tN_diag_"+lep[i]+"_"+tN_diag_Num[j]);
    for(int j = 0; j < 11; j++) bookCutflowRegions("bCc_diag_"+lep[i]+"_"+bCc_diag_Num[j]);
    for(int j = 0; j < 10; j++) bookCutflowRegions("tN_med_"+lep[i]+"_"+tN_med_Num[j]);
    for(int j = 0; j < 11; j++) bookCutflowRegions("tN_high_"+lep[i]+"_"+tN_high_Num[j]);
    for(int j = 0; j < 14; j++) bookCutflowRegions("bCb_med2_"+lep[i]+"_"+bCb_med2_Num[j]);
    for(int j = 0; j < 12; j++) bookCutflowRegions("bCd_bulk_"+lep[i]+"_"+bCd_bulk_Num[j]);
    for(int j = 0; j < 15; j++) bookCutflowRegions("bCd_high1_"+lep[i]+"_"+bCd_high1_Num[j]);
    for(int j = 0; j < 15; j++) bookCutflowRegions("bCd_high2_"+lep[i]+"_"+bCd_high2_Num[j]);
    for(int j = 0; j < 14; j++) bookCutflowRegions("threeBody_"+lep[i]+"_"+threeBody_Num[j]);
    for(int j = 0; j < 13; j++) bookCutflowRegions("tNbC_mix_"+lep[i]+"_"+tNbC_mix_Num[j]);
  }

  // You should initialize any declared variables here

  // Cutflow names declared in header
 
}

void Atlas_1407_0583::analyze() {
  missingET->addMuons(muonsCombined);
  electronsLoose = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47);           
  muonsCombined = filterPhaseSpace(muonsCombined, 6., -2.4, 2.4);         
  jets = filterPhaseSpace(jets, 15., -2.5, 2.5);
  int nJets = jets.size();            
  countCutflowEvent("a");

  ///BOOSTED ANALYSIS IS IGNORED!!!!!!!!!!!
  //Run analysis with Flags because of very different signal regions and cutflow
  bool bCb_med1_Flag, bCb_high_Flag,  bCa_med_Flag, bCa_low_Flag;
  bCb_med1_Flag = bCb_high_Flag = bCa_med_Flag = bCa_low_Flag = false;
  
  bool baseNormFlag, baseSoftFlag;
  baseNormFlag = baseSoftFlag = false;
  
  //-----------------------------------------------------
  // Begin with Trigger
  bool triggerFlag, triggerETMissFlag;
  triggerFlag = triggerETMissFlag = false;
  
  // Electron trigger (pt > 24 GeV, Iso: dR=0.2, tracks < 10%), also allow weak isolated electrons with PT>60GeV
  std::vector<Electron*> electronsTrigger = filterPhaseSpace(electronsLoose, 24., -2.47, 2.47);
  electronsTrigger = filterIsolation(electronsTrigger,0);
  if( (electronsTrigger.size() > 0) || ((electronsLoose.size() >0) && (electronsLoose[0]->PT > 60)) ){
    triggerFlag=true;
  }
  
  // Muon trigger (pt > 25 GeV, Iso: dR=0.2, tracks < 10%), also allow weak isolated muons with PT>36GeV
  std::vector<Muon*> muonsTrigger = filterPhaseSpace(muonsCombined, 24., -2.47, 2.47);
  muonsTrigger = filterIsolation(muonsTrigger,0);
  if( (muonsTrigger.size() > 0) || ((muonsCombined.size() >0) && (muonsCombined[0]->PT > 60)) ){
    triggerFlag=true;
  }
  
  // MET trigger, fully efficient for ETmiss > 150 GeV, 0.2 fb loss of lumi due to bunch crossing constraint
  // Cutflow for ETmiss cannot be trusted because begins working at 80 GeV, soft lepton cut flows all require higher etmiss though.
  // Also MET trigger was not functioning for first two bunch crossings
  if( missingET->P4().Et() > 150.){
    double random_trigger = (double) rand() / (double) (RAND_MAX + 1.);
    if(random_trigger > 0.01){
      triggerFlag=true;
      triggerETMissFlag = true;
      countCutflowEvent("b_trigger_EtMiss");
    }
  }
  
  if(triggerFlag == false)
    return;
 
  countCutflowEvent("b_trigger");

  
  //-----------------------------------------------------
  // Take 2.5% of events fail data quality (conservatively from cutflows)
  double random_DQ = (double) rand() / (double) (RAND_MAX + 1.);
  if(random_DQ > 0.975)
    return;

  countCutflowEvent("c_DQ"); 
  if(triggerETMissFlag == true) countCutflowEvent("c_DQ_ETMiss");
 
  
  //-----------------------------------------------------
  // Soft lepton cutflows have an early MET cut 
  if( (triggerETMissFlag == true) && (missingET->P4().Et() > 150.)){
    countCutflowEvent("e_150");
    bCb_med1_Flag = true;
    if(missingET->P4().Et() > 250.){
      countCutflowEvent("e_250");
      bCb_high_Flag = true;
        if(missingET->P4().Et() > 300.){
      countCutflowEvent("e_300");
      bCa_med_Flag = true;
        if(missingET->P4().Et() > 370.){
          countCutflowEvent("e_370");
          bCa_low_Flag = true;
        }
    }
    }
  }
  
  //-----------------------------------------------------
  // Basline lepton definition and object removal
  // Two kinds of baseline leptons 'norm -> loose' and 'soft -> medium'
  std::vector<Electron*> electronsBaseNorm = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47);       
  std::vector<Electron*> electronsBaseSoft = filterPhaseSpace(electronsMedium, 7., -2.47, 2.47);  
  std::vector<Muon*> muonsBaseNorm = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);     
  std::vector<Muon*> muonsBaseSoft = filterPhaseSpace(muonsCombined, 6., -2.4, 2.4);
  std::vector<Jet*> jetsNorm = overlapRemoval(jets, electronsBaseNorm, 0.2);
  std::vector<Jet*> jetsSoft = overlapRemoval(jets, electronsBaseSoft, 0.2);
  std::vector<Jet*> jetsTau = jetsNorm;
  jetsNorm = filterPhaseSpace(jetsNorm, 20., -2.5, 2.5);
  jetsSoft = filterPhaseSpace(jetsSoft, 20., -2.5, 2.5);
  electronsBaseNorm = overlapRemoval(electronsBaseNorm, jetsNorm, 0.4);
  electronsBaseSoft = overlapRemoval(electronsBaseSoft, jetsSoft, 0.4);
  muonsBaseNorm = overlapRemoval(muonsBaseNorm, jetsNorm, 0.4);
  muonsBaseSoft = overlapRemoval(muonsBaseSoft, jetsSoft, 0.4);
 
  //-----------------------------------------------------
  // Exactly 1 baseline lepton required  
  int lepNormFlag = 0; 
  int lepSoftFlag = 0;
  
  if( (electronsBaseNorm.size() + muonsBaseNorm.size()) == 1){
    if(electronsBaseNorm.size() == 1) lepNormFlag = 0;
    else lepNormFlag = 1;
    baseNormFlag = true;
    countCutflowEvent(ind_CF[0]+"_"+name_CF[0]+"_"+lep[lepNormFlag]);
  }
  if((triggerETMissFlag == true) && ((electronsBaseSoft.size() + muonsBaseSoft.size()) == 1)){
    baseSoftFlag = true;
    if(electronsBaseSoft.size() == 1) lepSoftFlag = 0;
    else lepSoftFlag = 1;
    if(bCb_med1_Flag ==true) countCutflowEvent(ind_CF[0]+"_"+name_CF[1]+"_"+lep[lepSoftFlag]);
    if(bCb_high_Flag ==true) countCutflowEvent(ind_CF[0]+"_"+name_CF[2]+"_"+lep[lepSoftFlag]);
    if(bCa_med_Flag ==true) countCutflowEvent(ind_CF[0]+"_"+name_CF[3]+"_"+lep[lepSoftFlag]);
    if(bCa_low_Flag ==true) countCutflowEvent(ind_CF[0]+"_"+name_CF[4]+"_"+lep[lepSoftFlag]);
  }  
  
  if( (baseNormFlag == false) && (baseSoftFlag == false) )
    return;

  //-----------------------------------------------------
  // Signal lepton definition and object removal
  // Two kinds of baseline leptons 'norm -> tight' and 'soft -> medium'
  std::vector<Electron*> electronsSelNorm = filterPhaseSpace(electronsTight, 25., -2.47, 2.47); 
  electronsSelNorm = filterIsolation(electronsSelNorm,0);
  electronsSelNorm = overlapRemoval(electronsSelNorm, jetsNorm, 0.4); //Need to redo overlap removal because now tight electron
  
  std::vector<Muon*> muonsSelNorm = filterPhaseSpace(muonsBaseNorm, 25., -2.4, 2.4); 
  muonsSelNorm = filterIsolation(muonsSelNorm,1);
  
  
  ///Testing to see if isolation is the problem
  std::vector<Electron*> electronsSelSoft = filterIsolation(electronsBaseSoft,1);
  electronsSelSoft = electronsBaseSoft;
  
  std::vector<Muon*> muonsSelSoft = filterIsolation(muonsBaseSoft,2);
  muonsSelSoft = muonsBaseSoft;
  
  //-----------------------------------------------------
  // Exactly 1 signal lepton required  
 
  if( (baseNormFlag == true) && ((electronsSelNorm.size() + muonsSelNorm.size()) == 1) )
    countCutflowEvent(ind_CF[1]+"_"+name_CF[0]+"_"+lep[lepNormFlag]);
  else baseNormFlag = false;
  
  if( (baseSoftFlag == true) && ((triggerETMissFlag == true) && ((electronsSelSoft.size() + muonsSelSoft.size()) == 1)) ){ 
    if(bCb_med1_Flag ==true) countCutflowEvent(ind_CF[1]+"_"+name_CF[1]+"_"+lep[lepSoftFlag]);
    if(bCb_high_Flag ==true) countCutflowEvent(ind_CF[1]+"_"+name_CF[2]+"_"+lep[lepSoftFlag]);
    if(bCa_med_Flag ==true) countCutflowEvent(ind_CF[1]+"_"+name_CF[3]+"_"+lep[lepSoftFlag]);
    if(bCa_low_Flag ==true) countCutflowEvent(ind_CF[1]+"_"+name_CF[4]+"_"+lep[lepSoftFlag]);
  }
  else{
    baseSoftFlag = false;
    bCb_med1_Flag = false;
    bCb_high_Flag = false;
    bCa_med_Flag = false;
    bCa_low_Flag = false;
  }
  
  if( (baseNormFlag == false) && (baseSoftFlag == false) )
    return;

  //*************************************************************************************
  // Common Cutflow ends here
  //*************************************************************************************  
  //-------------------------------------------------------------------
  // bCa cutflow
  if((bCa_med_Flag ==true) || (bCa_low_Flag ==true)) {
    TLorentzVector softLepton;
    if(electronsSelSoft.size() == 1) softLepton = electronsSelSoft[0]->P4();
    else softLepton = muonsSelSoft[0]->P4();
    
    // Lepton PT should be < 50
    if(softLepton.Pt() < 50.0){
      if(bCa_med_Flag ==true) countCutflowEvent(name_CF[3]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[0]);
      if(bCa_low_Flag ==true) countCutflowEvent(name_CF[4]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[0]);
    }
    else {
      bCa_med_Flag = false;
      bCa_low_Flag = false;
    }

    // 2 jets > 25 GeV for bCa_low, 3 jets > 25 GeV bCa_med
    if( ((bCa_low_Flag ==true) || (bCa_med_Flag ==true)) && (jetsSoft.size() > 1) && (jetsSoft[1]->PT > 25.)){
      if(bCa_low_Flag ==true) countCutflowEvent(name_CF[4]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[1]);
      if( (jetsSoft.size() > 2) && (jetsSoft[2]->PT > 25.)){
    if(bCa_med_Flag ==true) countCutflowEvent(name_CF[3]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[1]);
      }
      else  bCa_med_Flag = false;
    }
    else {
      bCa_med_Flag = false;
      bCa_low_Flag = false;
    }    
    
    // Hardest jet > 180 GeV
    if( ((bCa_low_Flag ==true) || (bCa_med_Flag ==true)) && (jetsSoft.size() > 0) && (jetsSoft[0]->PT > 180.)){
      if(bCa_med_Flag ==true) countCutflowEvent(name_CF[3]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[2]);
      if(bCa_low_Flag ==true) countCutflowEvent(name_CF[4]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[2]);
    }
    else {
      bCa_med_Flag = false;
      bCa_low_Flag = false;
    }
    
    // At least one b-tag (70%, index 1)
    if( (bCa_low_Flag == true) || (bCa_med_Flag == true)){
      bool bTag = false;
      for(int i = 0; i < jetsSoft.size(); i++){
    if(checkBTag(jetsSoft[i],1) == true) bTag = true;
      }
      if((bCa_med_Flag ==true) && (bTag == true)) countCutflowEvent(name_CF[3]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[3]);
      else bCa_med_Flag = false;
      if((bCa_low_Flag ==true) && (bTag == true)) countCutflowEvent(name_CF[4]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[3]);
      else bCa_low_Flag = false;
    }

    // Hardest jet should not be b-tagged
    if( ((bCa_low_Flag == true) || (bCa_med_Flag == true)) && (jetsSoft.size() > 0) && (checkBTag(jetsSoft[0],1)==false) ){
      if(bCa_med_Flag ==true) countCutflowEvent(name_CF[3]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[4]);
      if(bCa_low_Flag ==true) countCutflowEvent(name_CF[4]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[4]);
    }
    else {
      bCa_med_Flag = false;
      bCa_low_Flag = false;
    }
    
    // mT > 90 (bCa_low), mT > 100 (bCa_med)
    if( (bCa_low_Flag == true) || (bCa_med_Flag == true)){
      double mT = AnalysisBase::mT(softLepton, missingET->P4());
      if((bCa_med_Flag ==true) && (mT > 100.0)) countCutflowEvent(name_CF[3]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[5]);
      else bCa_med_Flag = false;
      if((bCa_low_Flag ==true) && (mT > 90.0)) countCutflowEvent(name_CF[4]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[5]);
      else bCa_low_Flag = false;
    }

    // ETmiss/meff > 0.35 (bCa_low), > 0.3 (bCa_med)
    if( (bCa_low_Flag == true) || (bCa_med_Flag == true)){     
      double meff = softLepton.Pt() + missingET->P4().Et();
      for(int i = 0; i < jetsSoft.size(); i++){
    if(i<4) meff += jetsSoft[i]->PT;
      } 
      if((bCa_med_Flag ==true) && (missingET->P4().Et()/meff > 0.3)) countCutflowEvent(name_CF[3]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[6]);
      else bCa_med_Flag = false;
      if((bCa_low_Flag ==true) && (missingET->P4().Et()/meff > 0.35)) countCutflowEvent(name_CF[4]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[6]);
      else bCa_low_Flag = false;
    }
    
    // Lepton PT should be < 25GeV for signal regions
    if(((bCa_low_Flag == true) || (bCa_med_Flag == true)) && (softLepton.Pt() < 25.0)){
      if(bCa_med_Flag ==true) {
    countCutflowEvent(name_CF[3]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[7]);
    countSignalEvent("bCa_med");
      }
      if(bCa_low_Flag ==true) {
    countCutflowEvent(name_CF[4]+"_"+lep[lepSoftFlag]+"_"+bCa_Num[7]);
    countSignalEvent("bCa_low");
      }
    }      
  }
  
  //*************************************************************************************  
  //-------------------------------------------------------------------
  // bCb_high and bCb_med1 cutflow
  if((bCb_med1_Flag ==true) || (bCb_high_Flag ==true)) {
    TLorentzVector softLepton;
    if(electronsSelSoft.size() == 1) softLepton = electronsSelSoft[0]->P4();
    else softLepton = muonsSelSoft[0]->P4();
    
    // Lepton PT should be < 25
    if(softLepton.Pt() < 25.0){
      if(bCb_med1_Flag ==true) countCutflowEvent(name_CF[1]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[0]);
      if(bCb_high_Flag ==true) countCutflowEvent(name_CF[2]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[0]);
    }
    else {
      bCb_med1_Flag = false;
      bCb_high_Flag = false;
    }
    
    // Jet1 PT should be > 60
    if(((bCb_med1_Flag ==true) || (bCb_high_Flag ==true)) && (jetsSoft.size() >0) && (jetsSoft[0]->PT > 60.0) ){
      if(bCb_med1_Flag ==true) countCutflowEvent(name_CF[1]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[1]);
      if(bCb_high_Flag ==true) countCutflowEvent(name_CF[2]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[1]);
    }
    else {
      bCb_med1_Flag = false;
      bCb_high_Flag = false;
    }
    
    // Jet2 PT should be > 60
    if(((bCb_med1_Flag ==true) || (bCb_high_Flag ==true)) && (jetsSoft.size() >1) && (jetsSoft[1]->PT > 60.0) ){
      if(bCb_med1_Flag ==true) countCutflowEvent(name_CF[1]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[2]);
      if(bCb_high_Flag ==true) countCutflowEvent(name_CF[2]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[2]);
    }
    else {
      bCb_med1_Flag = false;
      bCb_high_Flag = false;
    }
    
    // Leading 2 jets b-tagged (60% efficiency)
    /// Switched to 75% for cutflow agreement - tagger is optimized for higher QCD background events
    if(((bCb_med1_Flag ==true) || (bCb_high_Flag ==true)) && (checkBTag(jetsSoft[0],2)==true) && (checkBTag(jetsSoft[1],2)==true) ){
      if(bCb_med1_Flag ==true) countCutflowEvent(name_CF[1]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[3]);
      if(bCb_high_Flag ==true) countCutflowEvent(name_CF[2]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[3]);
    }
    else {
      bCb_med1_Flag = false;
      bCb_high_Flag = false;
    }
    
    // deltaPhi (jet, ETMiss) > 0.4
    /// This cut is NOT mentioned in the paper but is included
    /// Till said all jets but this seems to cut too many events, Try only 2
    if((bCb_med1_Flag ==true) || (bCb_high_Flag ==true) ){
      bool jetDelPhi = false;
      //for(int i = 0; i < jetsSoft.size(); i++){
      for(int i = 0; i < 2; i++){
    if(fabs(jetsSoft[i]->P4().DeltaPhi(missingET->P4())) < 0.4) jetDelPhi = true;
      }     
      if((bCb_med1_Flag ==true) && (jetDelPhi == false)) countCutflowEvent(name_CF[1]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[4]);
      else bCb_med1_Flag = false;
      if((bCb_high_Flag ==true) && (jetDelPhi == false)) countCutflowEvent(name_CF[2]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[4]);
      else bCb_high_Flag = false;
    }
    
    // H_{T,2} < 50 GeV (only for bCb_med1)  (H_T but without leading two jets, only jet 3 and 4)
    if(bCb_med1_Flag ==true){
      double HT2 = 0.;
      for(int i = 2; i < jetsSoft.size(); i++){
    if(i< 4) HT2 += jetsSoft[i]->PT;
      }   
      if((bCb_med1_Flag ==true) && (HT2 < 50)) countCutflowEvent(name_CF[1]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[5]);
      else bCb_med1_Flag = false;
    }
    
    // m_bb > 150. GeV (leading 2 jets have already been idetified as b's
    if((bCb_med1_Flag ==true) || (bCb_high_Flag ==true) ){
      TLorentzVector pbb = jetsSoft[0]->P4() + jetsSoft[1]->P4();
      double mbb = pbb.M();    
      if((bCb_med1_Flag ==true) && (mbb > 150.)) countCutflowEvent(name_CF[1]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[6]);
      else bCb_med1_Flag = false;
      if((bCb_high_Flag ==true) && (mbb > 150.)) countCutflowEvent(name_CF[2]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[6]);
      else bCb_high_Flag = false;
    }

    // aMT2 > 170. (bCb_med1) > 200 (bCb_high)
    if((bCb_med1_Flag ==true) || (bCb_high_Flag ==true) ){
      double amT2_1 = AnalysisBase::mT2_bl(softLepton, jetsSoft[0]->P4(), jetsSoft[1]->P4(), missingET->P4());
      double amT2_2 = AnalysisBase::mT2_bl(softLepton, jetsSoft[1]->P4(), jetsSoft[0]->P4(), missingET->P4());
      double amT2 = std::min(amT2_1,amT2_2);
      if((bCb_med1_Flag ==true) && (amT2 > 170.)){
    countCutflowEvent(name_CF[1]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[7]);
    countSignalEvent("bCb_med1");
      }
      if((bCb_high_Flag ==true) && (amT2 > 200.)){
    countCutflowEvent(name_CF[2]+"_"+lep[lepSoftFlag]+"_"+bCb_Num[7]);
    countSignalEvent("bCb_high");
      }
    }
  }
  
  //*************************************************************************************
  // All following seaches have common jet requirements
  //-------------------------------------------------------------------

  //-------------------------------------------------------------------
  // Now require 'normal' lepton
  if(baseNormFlag == false)
    return;
  
  //-------------------------------------------------------------------
  // At least 3 jets, all with PT > 25 GeV
  if( (jetsNorm.size() > 2) && (jetsNorm[2]->PT > 25.))
    countCutflowEvent(hard_CF[0]+"_"+lep[lepNormFlag]);
  else
    return;
  
  //-------------------------------------------------------------------
  // All apart from bCc_diag require 4 jets, all with PT > 25 GeV
  bool fourJetFlag = true;  
  if( (jetsNorm.size() > 3) && (jetsNorm[3]->PT > 25.))
    countCutflowEvent(hard_CF[1]+"_"+lep[lepNormFlag]);
  else
    fourJetFlag = false;
  
  //-------------------------------------------------------------------
  // Hardest jet pT cut (> 60, tN_diag), (> 80, tN_med, bCb_med2, bCc_diag, bCd_bulk, bCd_high1, bCd_high2, threeBody, tNbC_mix), (> 100, tN_high)   
  bool jet0_80 = false;
  bool jet0_100 = false;
  if(jetsNorm[0]->PT < 60)
    return;
  
  if(fourJetFlag == true)    // tN_diag also required 4 jets
    countCutflowEvent("tN_diag_"+lep[lepNormFlag]+"_00_jet1");
  
  if(jetsNorm[0]->PT > 80){
    jet0_80 = true;
    countCutflowEvent("bCc_diag_"+lep[lepNormFlag]+"_00_jet1");
    if(fourJetFlag == true){
      countCutflowEvent("tN_med_"+lep[lepNormFlag]+"_00_jet1"); countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_00_jet1"); countCutflowEvent("bCd_bulk_"+lep[lepNormFlag]+"_00_jet1"); countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_00_jet1"); countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_00_jet1"); countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_00_jet1"); countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_00_jet1");
      if(jetsNorm[0]->PT > 100){
    jet0_100 = true;
    countCutflowEvent("tN_high_"+lep[lepNormFlag]+"_00_jet1");
      }
    }
  }
  
  //-------------------------------------------------------------------
  // Second jet pT cut (>25 threeBody - already done), (>40 bCc_diag), (> 60 tN_diag, tN_med, bCb_med2, bCd_bulk, bCd_high1), (>70 tNbC_mix), (> 80, tN_high, bCd_high2)  
  countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_01_jet2");
  bool threeBody = true;
  // bCc_diag > 40
  bool bCc_diag = false;
  if( (jet0_80 == true) && (jetsNorm[1]->PT > 40.) ){
    bCc_diag = true;
    countCutflowEvent("bCc_diag_"+lep[lepNormFlag]+"_01_jet2");
  }
  
  //(> 60 tN_diag, tN_med, bCb_med2, bCd_bulk, bCd_high1)
  bool tN_diag = false; bool tN_med = false; bool bCb_med2 = false; bool bCd_bulk = false; bool bCd_high1 = false;
  bool tNbC_mix = false; bool bCd_high2 = false; bool tN_high = false;
  if((fourJetFlag == true) && (jetsNorm[1]->PT > 60.)){
    tN_diag = true;
    countCutflowEvent("tN_diag_"+lep[lepNormFlag]+"_01_jet2");
    if(jet0_80 == true) {
      tN_med = true; bCb_med2 = true; bCd_bulk = true; bCd_high1 = true;
      countCutflowEvent("tN_med_"+lep[lepNormFlag]+"_01_jet2"); countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_01_jet2"); countCutflowEvent("bCd_bulk_"+lep[lepNormFlag]+"_01_jet2"); countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_01_jet2");
      
      //(>70 tNbC_mix)
      if (jetsNorm[1]->PT > 70.){
    tNbC_mix = true;
    countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_01_jet2");
    
    //(> 80, tN_high, bCd_high2) 
    if (jetsNorm[1]->PT > 80.){
      bCd_high2 = true;
      countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_01_jet2");
      if(jet0_100 == true){
        tN_high = true;
        countCutflowEvent("tN_high_"+lep[lepNormFlag]+"_01_jet2");
      }
    }
      }
    }
  }
  
  //-------------------------------------------------------------------
  // Third jet pT cut (>25 threeBody - already done), (> 30 bCc_diag), (> 40 tN_diag, tN_med, tN_high, bCb_med2, bCd_bulk, bCd_high1, bCd_high2), (> 50 tNbC_mix)  
  countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_02_jet3");  
  
  if( (bCc_diag == true) && (jetsNorm[2]->PT > 30.)) countCutflowEvent("bCc_diag_"+lep[lepNormFlag]+"_02_jet3");
  else bCc_diag = false;
  
  if(jetsNorm[2]->PT > 40){
    if(tN_diag == true) countCutflowEvent("tN_diag_"+lep[lepNormFlag]+"_02_jet3");
    if(tN_med == true) countCutflowEvent("tN_med_"+lep[lepNormFlag]+"_02_jet3");
    if(tN_high == true) countCutflowEvent("tN_high_"+lep[lepNormFlag]+"_02_jet3");
    if(bCb_med2 == true) countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_02_jet3");
    if(bCd_bulk == true) countCutflowEvent("bCd_bulk_"+lep[lepNormFlag]+"_02_jet3");
    if(bCd_high1 == true) countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_02_jet3");
    if(bCd_high2 == true) countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_02_jet3");
  }
  else{
    tN_diag = false; tN_med = false; tN_high = false; bCb_med2 = false; bCd_bulk = false; bCd_high1 = false; bCd_high2 = false; 
  }
   
  if( (jetsNorm[2]->PT > 50) && (tNbC_mix == true) ) countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_02_jet3");
  else tNbC_mix = false;
  
  //*************************************************************************************
  // Split cutflows by b-tags
  //-------------------------------------------------------------------
  // (0 (70%) bCc_diag) (>= 1 (70%) tN_diag, tN_med, tN_high, bCd_bulk, threeBody, tNbC_mix), (>= 2 (80%) bCb_med2, bCd_high1, bCd_high2) 
  
  //count 70% b-jets
  int nBTags70 =0;
  for(int i = 0; i < jetsNorm.size(); i++){
    if(checkBTag(jetsNorm[i],1) == true) nBTags70 += 1;
  }
  
  //Store lepton momentum
  TLorentzVector lepNorm;
  if(lepNormFlag==0) lepNorm = electronsSelNorm[0]->P4();
  else lepNorm = muonsSelNorm[0]->P4();
  //************************************************************************************
  // bCc_diag requires no b-tags
  //-------------------------------------------------------------------
  if( (bCc_diag == true) && (nBTags70 == 0)){
    countCutflowEvent("bCc_diag_"+lep[lepNormFlag]+"_03_noBtag");
    
    // lepton |eta| < 1.2
    if(fabs(lepNorm.Eta())<1.2){
      countCutflowEvent("bCc_diag_"+lep[lepNormFlag]+"_04_lepEta");
      
      //dPhi (jet_1,ETmiss) >2
      if( fabs(jetsNorm[0]->P4().DeltaPhi(missingET->P4())) > 2.){
    countCutflowEvent("bCc_diag_"+lep[lepNormFlag]+"_05_dPhi1");
    
    //dPhi (jet2,ETmiss) >2
    if( fabs(jetsNorm[1]->P4().DeltaPhi(missingET->P4())) > 2.){
      countCutflowEvent("bCc_diag_"+lep[lepNormFlag]+"_06_dPhi2");

      //dR (lepton,jet1) >0.8
      if( (fabs(jetsNorm[0]->P4().DeltaR(lepNorm)) > 0.8) && (fabs(jetsNorm[0]->P4().DeltaR(lepNorm)) < 2.4)){
        countCutflowEvent("bCc_diag_"+lep[lepNormFlag]+"_07_dR");
              
        // EtMiss > 140 GeV
        if(missingET->P4().Et() > 140){
          countCutflowEvent("bCc_diag_"+lep[lepNormFlag]+"_08_Met");
          
          // EtMiss/sqrt(HT) > 5
          // HT is sum of four jets
          double HT = 0.;
          for(int i = 0; i < jetsNorm.size(); i++){
        if( i < 4) HT += jetsNorm[i]->PT;
          } 
          if( (missingET->P4().Et()/ sqrt(HT)) > 5) {
        countCutflowEvent("bCc_diag_"+lep[lepNormFlag]+"_09_met/ht");
        
        //**************************************
        //SIGNAL REGION
        // mT > 120
        double mT = AnalysisBase::mT(lepNorm, missingET->P4());
        if(mT > 120){
          countCutflowEvent("bCc_diag_"+lep[lepNormFlag]+"_10_mT");
          countSignalEvent("bCc_diag");
        }
          }
        }
      }
    }
      }
    }
  }
  
  //************************************************************************************
  // tN_diag require > 1 bjet (70% eff)
  //-------------------------------------------------------------------  
  
  if( (nBTags70 > 0) && (tN_diag == true) ){
    countCutflowEvent("tN_diag_"+lep[lepNormFlag]+"_03_bTag");
    
    // deltaPhi(jet1,EtMiss) > 0.8
    if( fabs(jetsNorm[0]->P4().DeltaPhi(missingET->P4())) > 0.8){
      countCutflowEvent("tN_diag_"+lep[lepNormFlag]+"_04_dPhiJet1");
      
      // deltaPhi(jet1,EtMiss) > 0.8
      if( fabs(jetsNorm[1]->P4().DeltaPhi(missingET->P4())) > 0.8){
    countCutflowEvent("tN_diag_"+lep[lepNormFlag]+"_05_dPhiJet2");
    
    
    //EtMiss > 125
    if( missingET->P4().Et() > 125.){    
      bool tN_diag_MET150=true;
      //EtMiss > 150. is in cutflow
      if( missingET->P4().Et() > 150.) countCutflowEvent("tN_diag_"+lep[lepNormFlag]+"_06_EtMiss");
      else tN_diag_MET150=false;
        
      // EtMiss/sqrt(HT) > 5
      // HT is sum over four jets
      double HT = 0.;
      for(int i = 0; i < jetsNorm.size(); i++){
        if( i < 4) HT += jetsNorm[i]->PT;
      } 
      if( (missingET->P4().Et()/ sqrt(HT)) > 5) {
        if(tN_diag_MET150 == true) countCutflowEvent("tN_diag_"+lep[lepNormFlag]+"_07_met/ht");      
            
        // mT > 120
        double mT = AnalysisBase::mT(lepNorm, missingET->P4());
        if(mT > 120.){
          
          // mT > 140 in cutflow
          bool tN_diag_mT140 = false;
          if(mT > 140.){
        tN_diag_mT140 = true;
        if(tN_diag_MET150 == true) countCutflowEvent("tN_diag_"+lep[lepNormFlag]+"_08_mT");     
          }
          
          //m_had-top = 130-205 
          /// Reconstruction was slightly too good so tighten window.
          // Try to reconstruct top mass -> use higher reco probability to match b-tagging weights procedure
          bool hadTop = false;
          TLorentzVector top_cand; 
          std::vector<Jet*>  bTag_jets;
          std::vector<Jet*>  notbTag_jets;
          for(int i = 0; i < jetsNorm.size(); i++){
        if(checkBTag(jetsNorm[i],3) == true) bTag_jets.push_back(jetsNorm[i]);
        else notbTag_jets.push_back(jetsNorm[i]);
          }

          //need at at least two non bjets to reconstruct hadronic tops    
          if(notbTag_jets.size() >1){
        for(int i = 0; i < bTag_jets.size(); i++){ 
          for(int j = 0; j < notbTag_jets.size(); j++){ 
            for(int k = 0; k < notbTag_jets.size(); k++){
              if (j !=k ) top_cand = bTag_jets[i]->P4() + notbTag_jets[j]->P4() + notbTag_jets[k]->P4();
              if( (top_cand.M() >150.) && (top_cand.M() < 190.)){
            hadTop = true;
                break;
              }
            }
          }
        }
          }
          
          if (hadTop == true){
        if((tN_diag_mT140==true) && (tN_diag_MET150==true)) countCutflowEvent("tN_diag_"+lep[lepNormFlag]+"_09_hadTop");
        
        //Tau veto
        //First get lepton charge
        bool tauVeto = false;
        int lepCharge=0;  
        if (lepNormFlag == 0) lepCharge = electronsBaseNorm[0]->Charge;
        else lepCharge = muonsBaseNorm[0]->Charge;
        for(int i = 0; i < jetsTau.size(); i++){
          if( (checkTauTag(jetsTau[i],"loose")==true) && (jetsTau[i]->Charge*lepCharge < 0)) tauVeto=true;
        }
        
        if (tauVeto == false){
          
          //*********************************
          // Signal Regions
          if((tN_diag_mT140==true) && (tN_diag_MET150==true)) {
            countCutflowEvent("tN_diag_"+lep[lepNormFlag]+"_10_tauVeto");
            countSignalEvent("tN_diag_d");
          }
          else if ((tN_diag_mT140==false) && (tN_diag_MET150==true)) countSignalEvent("tN_diag_c");
          else if ((tN_diag_mT140==true) && (tN_diag_MET150=false)) countSignalEvent("tN_diag_b");  
          else if ((tN_diag_mT140==false) && (tN_diag_MET150==false)) countSignalEvent("tN_diag_a");
        }
          }
        }
      }
    }
      }
    }
  }
  
  //************************************************************************************
  // tN_med requires > 1 bjet (70% eff)
  //-------------------------------------------------------------------

  if( (nBTags70 > 0) && (tN_med == true) ){
    countCutflowEvent("tN_med_"+lep[lepNormFlag]+"_03_bTag");

    // deltaPhi(jet1,EtMiss) > 0.8
    if( fabs(jetsNorm[1]->P4().DeltaPhi(missingET->P4())) > 0.8){
      countCutflowEvent("tN_med_"+lep[lepNormFlag]+"_04_dPhiJet2");
      
      //EtMiss > 200
      if(missingET->P4().Et() >200.){
    countCutflowEvent("tN_med_"+lep[lepNormFlag]+"_05_EtMiss");
    
    //HTMISS_SIG requires per event jet resolution.   Take 0.04 at 20 GeV -> 0.02 at 100 GeV and above -> ((jetPT-20)*(-0.02/80))+0.04 below 100GeV
    //https://twiki.cern.ch/twiki/bin/view/AtlasPublic/JetEtmissApproved2013JESUncertainty
    //HTMISS_SIG > 12.5
    double chi2=0;
    TLorentzVector missHT = -lepNorm;
    for(int i = 0; i < jetsNorm.size(); i++){
      if(jetsNorm[i]->PT <100.){
        //chi2 += pow( (jetsNorm[i]->PT*(((jetsNorm[i]->PT-20)*(-0.05/80))+0.10)) ,2);
        chi2 += pow( (jetsNorm[i]->PT*(((jetsNorm[i]->PT-20)*((0.04-0.08)/80))+0.08)) ,2);
        missHT -= jetsNorm[i]->P4();
      }
      else{
        //chi2 += pow( (jetsNorm[i]->PT*0.05) ,2);
        chi2 += pow( (jetsNorm[i]->PT*0.04) ,2);
        missHT -= jetsNorm[i]->P4();
      }
    }
    double missHT_Sig = (missHT.Pt()-100.)/sqrt(chi2);
    
    if(missHT_Sig > 12.5){
      countCutflowEvent("tN_med_"+lep[lepNormFlag]+"_06_missHT_Sig");
      
      // mT > 140
      double mT = AnalysisBase::mT(lepNorm, missingET->P4());
      if(mT > 140.){
        countCutflowEvent("tN_med_"+lep[lepNormFlag]+"_07_mT");
        
        //m_had-top = 130-195 
        ///Tighter for resolution
        // Try to reconstruct top mass -> use higher reco probability to match b-tagging weights procedure
        bool hadTop = false;
        TLorentzVector top_cand; 
        std::vector<Jet*>  bTag_jets;
        std::vector<Jet*>  notbTag_jets;
        for(int i = 0; i < jetsNorm.size(); i++){
          if(checkBTag(jetsNorm[i],3) == true) bTag_jets.push_back(jetsNorm[i]);
          else notbTag_jets.push_back(jetsNorm[i]);
        }

        //need at at least two non bjets to reconstruct hadronic tops    
        if(notbTag_jets.size() >1){
          for(int i = 0; i < bTag_jets.size(); i++){ 
        for(int j = 0; j < notbTag_jets.size(); j++){ 
          for(int k = 0; k < notbTag_jets.size(); k++){
            if (j !=k ) top_cand = bTag_jets[i]->P4() + notbTag_jets[j]->P4() + notbTag_jets[k]->P4();
            if( (top_cand.M() >140.) && (top_cand.M() < 185.)){
              hadTop = true;
              break;
            }
          }
        }
          }
        }
          
        if (hadTop == true){
          countCutflowEvent("tN_med_"+lep[lepNormFlag]+"_08_hadTop");
          
          //*********************************
          // Signal Region
          //---------------------------------
          
          //---------------------------------
          // Need to calculate aMT2 -> need two b-jets (use 80% eff) (if not two b-jets, use hardest other jet)
          double amT2=0.;
          if(bTag_jets.size() >1){    
        double amT2_1 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[0]->P4(),bTag_jets[1]->P4(), missingET->P4());
        double amT2_2 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[1]->P4(), bTag_jets[0]->P4(), missingET->P4());
        amT2 = std::min(amT2_1,amT2_2);
          }
          else if(bTag_jets.size() == 1){    
        double amT2_1 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[0]->P4(),notbTag_jets[0]->P4(), missingET->P4());
        double amT2_2 = AnalysisBase::mT2_bl(lepNorm, notbTag_jets[0]->P4(), bTag_jets[0]->P4(), missingET->P4());
        amT2 = std::min(amT2_1,amT2_2);
          }
          if(amT2 > 170.){
        countCutflowEvent("tN_med_"+lep[lepNormFlag]+"_09_aMT2");
        countSignalEvent("tN_med");
          }
        }
      }
    }
      }
    }
  }
  
  //************************************************************************************
  // tN_high requires > 1 bjet (70% eff)
  //-------------------------------------------------------------------

  if( (nBTags70 > 0) && (tN_high == true) ){
    countCutflowEvent("tN_high_"+lep[lepNormFlag]+"_03_bTag");

    // deltaR(lepton,hardest b jet) < 3
    // Need to find hardest b-tag first
    
    /// Paper says hardest b-jet but it is impossible (also by just thinking about physics) for this to be correct to match cutflow
    double dRMin =  10.;
    TLorentzVector bJet;
    for(int i = 0; i < jetsNorm.size(); i++){
    //for(int i = 0; i < 1; i++){  
      if(checkBTag(jetsNorm[i],3) == true){
        bJet = jetsNorm[i]->P4();
        double dRTest = fabs(lepNorm.DeltaR(bJet));
        if (dRTest < dRMin) dRMin = dRTest;
      }
    } 
    
    
    if( dRMin < 3.){   
      countCutflowEvent("tN_high_"+lep[lepNormFlag]+"_04_dRLepBJet");

      //EtMiss > 320
      if(missingET->P4().Et() >320.){
        countCutflowEvent("tN_high_"+lep[lepNormFlag]+"_05_EtMiss");
    
    //HTMISS_SIG requires per event jet resolution.   Take 0.04 at 20 GeV -> 0.02 at 100 GeV and above -> ((jetPT-20)*(-0.02/80))+0.04 below 100GeV
    //https://twiki.cern.ch/twiki/bin/view/AtlasPublic/JetEtmissApproved2013JESUncertainty
    //HTMISS_SIG > 12.5
    double sigma2=0;
    TLorentzVector missHT = -lepNorm;
    for(int i = 0; i < jetsNorm.size(); i++){
      if(jetsNorm[i]->PT <100.){
        //sigma2 += pow( (jetsNorm[i]->PT*(((jetsNorm[i]->PT-20)*((0.05-0.1)/80))+0.10)) ,2);
        sigma2 += pow( (jetsNorm[i]->PT*(((jetsNorm[i]->PT-20)*((0.04-0.08)/80))+0.08)) ,2);
        missHT -= jetsNorm[i]->P4();
      }
      else{
        //sigma2 += pow( (jetsNorm[i]->PT*0.05) ,2);
        sigma2 += pow( (jetsNorm[i]->PT*0.04) ,2);
        missHT -= jetsNorm[i]->P4();
      }
    }
    double missHT_Sig = (missHT.Pt()-100.)/sqrt(sigma2);
    
    if(missHT_Sig > 12.5){
      countCutflowEvent("tN_high_"+lep[lepNormFlag]+"_06_missHT_Sig");
      
      // mT > 200
      double mT = AnalysisBase::mT(lepNorm, missingET->P4());
      if(mT > 200.){
        countCutflowEvent("tN_high_"+lep[lepNormFlag]+"_07_mT");
        
        //m_had-top = 130-250
        // Try to reconstruct top mass -> use higher reco probability to match b-tagging weights procedure
        bool hadTop = false;
        TLorentzVector top_cand; 
        std::vector<Jet*>  bTag_jets;
        std::vector<Jet*>  notbTag_jets;
        for(int i = 0; i < jetsNorm.size(); i++){
          if(checkBTag(jetsNorm[i],3) == true) bTag_jets.push_back(jetsNorm[i]);
          else notbTag_jets.push_back(jetsNorm[i]);
        }

        //need at at least two non bjets to reconstruct hadronic tops    
        if(notbTag_jets.size() >1){
          for(int i = 0; i < bTag_jets.size(); i++){ 
        for(int j = 0; j < notbTag_jets.size(); j++){ 
          for(int k = 0; k < notbTag_jets.size(); k++){
            if (j !=k ) top_cand = bTag_jets[i]->P4() + notbTag_jets[j]->P4() + notbTag_jets[k]->P4();
            if( (top_cand.M() >130.) && (top_cand.M() < 250.)){
              hadTop = true;
              break;
            }
          }
        }
          }
        }
          
        if (hadTop == true){
          countCutflowEvent("tN_high_"+lep[lepNormFlag]+"_08_hadTop");
          
          //---------------------------------
          // Need to calculate aMT2 -> need two b-jets (use 80% eff) (if not two b-jets, use hardest other jet)
          double amT2=0.;
          if(bTag_jets.size() >1){    
        double amT2_1 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[0]->P4(),bTag_jets[1]->P4(), missingET->P4());
        double amT2_2 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[1]->P4(), bTag_jets[0]->P4(), missingET->P4());
        amT2 = std::min(amT2_1,amT2_2);
          }
          else if(bTag_jets.size() == 1){    
        double amT2_1 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[0]->P4(),notbTag_jets[0]->P4(), missingET->P4());
        double amT2_2 = AnalysisBase::mT2_bl(lepNorm, notbTag_jets[0]->P4(), bTag_jets[0]->P4(), missingET->P4());
        amT2 = std::min(amT2_1,amT2_2);
          }
          // aMT2 > 170. 
          if(amT2 > 170.){
        countCutflowEvent("tN_high_"+lep[lepNormFlag]+"_09_aMT2");
        
        //*********************************
        // Signal Region
        //---------------------------------
        // mtau_T2 > 120 (normal mT2 with zero mass, target W's so ignore b-jets)
        double mT2_tau = AnalysisBase::mT2(notbTag_jets[0]->P4(), lepNorm, 0., missingET->P4());
        if(mT2_tau > 120.){
          countCutflowEvent("tN_high_"+lep[lepNormFlag]+"_10_MT2_Tau");
          countSignalEvent("tN_high"); 
        }
          }
        }
      }
    }
      }
    }
  }  
  
  //************************************************************************************
  // bCd_bulk requires > 1 bjet (70% eff)
  //-------------------------------------------------------------------

  if( (nBTags70 > 0) && (bCd_bulk == true) ){
    countCutflowEvent("bCd_bulk_"+lep[lepNormFlag]+"_03_bTag");

    // dPhi(jet1, EtMiss) > 0.8
    if( fabs(jetsNorm[0]->P4().DeltaPhi(missingET->P4())) > 0.8){
      countCutflowEvent("bCd_bulk_"+lep[lepNormFlag]+"_04_dPhiJet1");

      // dPhi(jet2, EtMiss) > 0.8
      if( fabs(jetsNorm[1]->P4().DeltaPhi(missingET->P4())) > 0.8){
    countCutflowEvent("bCd_bulk_"+lep[lepNormFlag]+"_05_dPhiJet2");
      
    //EtMiss > 150
    if(missingET->P4().Et() >150.){
      countCutflowEvent("bCd_bulk_"+lep[lepNormFlag]+"_06_EtMiss");
      
      // EtMiss/sqrt(HT) > 7
      // HT is sum over four jets
      double HT = 0.;
      for(int i = 0; i < jetsNorm.size(); i++){
        if( i < 4) HT += jetsNorm[i]->PT;
      } 
      if( (missingET->P4().Et()/ sqrt(HT)) > 7) {
        countCutflowEvent("bCd_bulk_"+lep[lepNormFlag]+"_07_met/ht");      
    
        // mT > 90 (120 for cutflow)
        double mT = AnalysisBase::mT(lepNorm, missingET->P4());
        if(mT > 90){
          bool mT_cutflow = false;
          if(mT > 120){
        countCutflowEvent("bCd_bulk_"+lep[lepNormFlag]+"_08_mT");
        mT_cutflow = true;
          }

          //---------------------------------
          // Need to calculate aMT2 -> need two b-jets (use 80% eff)
          std::vector<Jet*>  bTag_jets;
          std::vector<Jet*>  notbTag_jets;
          for(int i = 0; i < jetsNorm.size(); i++){
        if(checkBTag(jetsNorm[i],3) == true) bTag_jets.push_back(jetsNorm[i]);
        else notbTag_jets.push_back(jetsNorm[i]);
          }
          
          //---------------------------------
          // Need to calculate aMT2 -> need two b-jets (use 80% eff) (if not two b-jets, use hardest other jet)
          double amT2=0.;
          if(bTag_jets.size() >1){    
        double amT2_1 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[0]->P4(),bTag_jets[1]->P4(), missingET->P4());
        double amT2_2 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[1]->P4(), bTag_jets[0]->P4(), missingET->P4());
        amT2 = std::min(amT2_1,amT2_2);
          }
          else if(bTag_jets.size() == 1){    
        double amT2_1 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[0]->P4(),notbTag_jets[0]->P4(), missingET->P4());
        double amT2_2 = AnalysisBase::mT2_bl(lepNorm, notbTag_jets[0]->P4(), bTag_jets[0]->P4(), missingET->P4());
        amT2 = std::min(amT2_1,amT2_2);
          }
          if(amT2 > 175.){
        if(mT_cutflow == true) countCutflowEvent("bCd_bulk_"+lep[lepNormFlag]+"_09_aMT2");
        
        //Veto on isolated tracks
        std::vector<Track*> tracksHard = filterPhaseSpace(tracks, 10., -2.5, 2.5);  
        tracksHard = overlapRemoval(tracksHard, electronsBaseNorm, 0.4);
        tracksHard = overlapRemoval(tracksHard, muonsBaseNorm, 0.4);
          
        //Check no other hard track lies close
        std::vector<Track*> tracksIso_a;
        for(int i = 0; i < tracksHard.size(); i++){
        bool isoTest = true;
        for(int j = 0; j < tracksHard.size(); j++){
          if(i!=j) {
            if(tracksHard[i]->P4().DeltaR(tracksHard[j]->P4()) <0.4) isoTest = false;
          }
        }
        if(isoTest==true) tracksIso_a.push_back(tracksHard[i]);
        }
            
        //Check no soft track lies close
        std::vector<Track*> tracksIso_b;
        for(int i = 0; i < tracksIso_a.size(); i++){
          bool isoTest = true;
          for(int j = 0; j < tracks.size(); j++){
            if( (tracks[j]->PT >3.) && (tracks[j]->PT <10.) && (fabs(tracks[j]->Eta) <2.5)){
              if(tracksIso_a[i]->P4().DeltaR(tracks[j]->P4()) <0.4) isoTest = false;
            }
          }
          if(isoTest==true) tracksIso_b.push_back(tracksIso_a[i]);
        }
          
        //if remaining track, check if charge is opposite
        int lepCharge=0;
        bool isoCheck =true;
        if (lepNormFlag == 0) lepCharge = electronsBaseNorm[0]->Charge;
        else lepCharge = muonsBaseNorm[0]->Charge;
        for(int i = 0; i < tracksIso_b.size(); i++){
          if(lepCharge*tracksIso_b[i]->Charge < 0.) isoCheck = false;
        }
          
        if(isoCheck == true){
          if(mT_cutflow == true) countCutflowEvent("bCd_bulk_"+lep[lepNormFlag]+"_10_trackIso");
            
          //*********************************
          // Signal Region
          //---------------------------------
          //Tau veto
          bool tauVeto = false;
          for(int i = 0; i < jetsTau.size(); i++){
            if( (checkTauTag(jetsTau[i],"tight")==true) && (jetsTau[i]->Charge*lepCharge <0.)) tauVeto=true;
          }
        
          if (tauVeto == false){
            if(mT_cutflow == true) countCutflowEvent("bCd_bulk_"+lep[lepNormFlag]+"_11_tauVeto");
            
            if( (amT2>175.) && (amT2<250.) && (mT > 90.) && (mT < 120.0)) countSignalEvent("bCd_bulk_a");
            if( (amT2>175.) && (amT2<250.) && (mT > 120.0)) countSignalEvent("bCd_bulk_b");
            if( (amT2>250.) && (mT > 90.) && (mT < 120.0)) countSignalEvent("bCd_bulk_c");
            if( (amT2>250.) && (mT > 120.0)) countSignalEvent("bCd_bulk_d");
          }
        }
          }
        }
      }
    }
      }
    }
  }  

  //************************************************************************************
  // threeBody requires > 1 bjet (70% eff)
  //-------------------------------------------------------------------
  
    if( (nBTags70 > 0) && (threeBody == true) ){
    countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_03_bTag");

      // dPhi(jet1, EtMiss) > 0.2
      if( fabs(jetsNorm[0]->P4().DeltaPhi(missingET->P4())) > 0.2){
    countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_04_dPhiJet1");

    // dPhi(jet2, EtMiss) > 0.2
    if( fabs(jetsNorm[1]->P4().DeltaPhi(missingET->P4())) > 0.2){
      countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_05_dPhiJet2");
      
      // dPhi(lep, EtMiss) > 1.25
      if( fabs(lepNorm.DeltaPhi(missingET->P4())) > 1.25){
        countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_06_dPhiLepETMiss");
        
        // dR(lep, jet1) > 1.25
        if( fabs(lepNorm.DeltaR(jetsNorm[0]->P4())) > 1.25){
          countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_07_dRLepJet1");

          // dR(lep, jet2) > 2.
          if( fabs(lepNorm.DeltaR(jetsNorm[1]->P4())) > 2.){
        countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_08_dRLepJet2");
      
        //EtMiss > 150
        if(missingET->P4().Et() >150.){
          countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_09_EtMiss");
          
          // EtMiss/sqrt(HT) > 5
          // HT is sum of four jets
          double HT = 0.;
          for(int i = 0; i < jetsNorm.size(); i++){
            if( i < 4) HT += jetsNorm[i]->PT;
          } 
          if( (missingET->P4().Et()/ sqrt(HT)) > 5) {
            countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_10_met/ht");
            
            // mT > 90
            double mT = AnalysisBase::mT(lepNorm, missingET->P4());
            if(mT > 90){
              bool mT_120 = false;
              if(mT > 120){
            countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_11_mT");
            mT_120 = true;
              }
              
              //---------------------------------
              // Need to calculate aMT2 -> need two b-jets (use 80% eff)
              std::vector<Jet*>  bTag_jets;
              std::vector<Jet*>  notbTag_jets;
              for(int i = 0; i < jetsNorm.size(); i++){
            if(checkBTag(jetsNorm[i],3) == true) bTag_jets.push_back(jetsNorm[i]);
            else notbTag_jets.push_back(jetsNorm[i]);
              }
              
              //---------------------------------
              // Need to calculate aMT2 -> need two b-jets (use 80% eff) (if not two b-jets, use hardest other jet)
              double amT2=0.;
              if(bTag_jets.size() >1){    
            double amT2_1 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[0]->P4(),bTag_jets[1]->P4(), missingET->P4());
            double amT2_2 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[1]->P4(), bTag_jets[0]->P4(), missingET->P4());
            amT2 = std::min(amT2_1,amT2_2);
              }
              else if(bTag_jets.size() == 1){    
            double amT2_1 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[0]->P4(),notbTag_jets[0]->P4(), missingET->P4());
            double amT2_2 = AnalysisBase::mT2_bl(lepNorm, notbTag_jets[0]->P4(), bTag_jets[0]->P4(), missingET->P4());
            amT2 = std::min(amT2_1,amT2_2);
              }
              if( (amT2 > 80.) && (amT2 < 100)){
            if(mT_120 == true) countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_12_aMT2");
                
            //*********************************
            // Signal Region
            //---------------------------------
            //Tau veto
            bool tauVeto = false;
            int lepCharge=0;
            if (lepNormFlag == 0) lepCharge = electronsBaseNorm[0]->Charge;
            else lepCharge = muonsBaseNorm[0]->Charge;
              
            for(int i = 0; i < jetsTau.size(); i++){
              if( (checkTauTag(jetsTau[i],"tight")==true) && (jetsTau[i]->Charge*lepCharge <0.)) tauVeto=true;
            }
            if(tauVeto == false){  
              countCutflowEvent("threeBody_"+lep[lepNormFlag]+"_13_tauVeto");
              if(mT_120 == true){
                if (amT2>90.) countSignalEvent("threeBody_d");
                else countSignalEvent("threeBody_b");
              }
              else{
                if(amT2>90.) countSignalEvent("threeBody_c");
                else countSignalEvent("threeBody_a");          
              }
            }
              }
            }
          }
        }
          }
        }
      }
    }
      }
    }
    
    
  //************************************************************************************
  // tNbC_mix requires > 1 bjet (70% eff)
  //-------------------------------------------------------------------
    if( (nBTags70 > 0) && (tNbC_mix == true) ){
      countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_03_bTag");
    
      //b-jet PT > 60
      std::vector<Jet*>  bTag_jets;
      std::vector<Jet*>  notbTag_jets;
      for(int i = 0; i < jetsNorm.size(); i++){
    if(checkBTag(jetsNorm[i],3) == true) bTag_jets.push_back(jetsNorm[i]);
    else notbTag_jets.push_back(jetsNorm[i]);
      }
      
      if( (bTag_jets.size()>0) && (bTag_jets[0]->PT > 60)){
    countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_04_bJetPT");
      
    // dPhi(jet1, EtMiss) > 0.6
    if( fabs(jetsNorm[0]->P4().DeltaPhi(missingET->P4())) > 0.6){
      countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_05_dPhiJet1");

      // dPhi(jet2, EtMiss) > 0.6
      if( fabs(jetsNorm[1]->P4().DeltaPhi(missingET->P4())) > 0.6){
        countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_06_dPhiJet2");
      
        // dPhi(lep, EtMiss) > 0.6
        if( fabs(lepNorm.DeltaPhi(missingET->P4())) > 0.6){
          countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_07_dPhiLepETMiss");
        
          // dR(lep, jet1) < 2.75
          if( fabs(lepNorm.DeltaR(jetsNorm[0]->P4())) < 2.75){
        countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_08_dRLepJet1");

        // dR(lep, bjet1) < 3.
        if( fabs(lepNorm.DeltaR(bTag_jets[0]->P4())) < 3.){
          countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_09_dRLepJet2");
      
          //EtMiss > 270
          if(missingET->P4().Et() >270.){
            countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_10_EtMiss");
          
            // EtMiss/sqrt(HT) > 9
            // HT is sum of four jets
            double HT = 0.;
            for(int i = 0; i < jetsNorm.size(); i++){
              if( i < 4) HT += jetsNorm[i]->PT;
            } 
            if( (missingET->P4().Et()/ sqrt(HT)) > 9) {
              countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_11_met/ht");
            
              // mT > 130
              double mT = AnalysisBase::mT(lepNorm, missingET->P4());
              if(mT > 130){
            countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_12_mT");

            // mjjj < 360
            // First two jets defined by pair that have smallest dR with mass >60
            // Third jet is closest in dR to previous 2jet vector
            double deltaRTest =10.;
            TLorentzVector diJet;
            int diJet_i =0;
            int diJet_j =0;
            for(int i = 0; i < jetsNorm.size(); i++){
              for(int j = 0; j < jetsNorm.size(); j++){
                if(i != j){
                  TLorentzVector diJetTest = jetsNorm[i]->P4() + jetsNorm[j]->P4();
                  if((diJetTest.M() > 60.) && ( jetsNorm[i]->P4().DeltaR(jetsNorm[j]->P4()) < deltaRTest)){
                deltaRTest = jetsNorm[i]->P4().DeltaR(jetsNorm[j]->P4());
                diJet = diJetTest;
                diJet_i = i;
                diJet_j =j;
                  }
                }
              }
            }
            
            double mjjj =0.;
            deltaRTest =10.;   //reset deltaRTest
            if(diJet.M() > 60.){
              for(int i = 0; i < jetsNorm.size(); i++){
                if( (i != diJet_i) && (i != diJet_j) && (jetsNorm[i]->P4().DeltaR(diJet) < deltaRTest) ){
                  deltaRTest = jetsNorm[i]->P4().DeltaR(diJet);
                  mjjj = (jetsNorm[i]->P4() + diJet).M();
                }
              }
            }
            
            if( (diJet.M() > 60.) && (mjjj < 360)){
              countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_13_mjjj");
              
              //---------------------------------
              // Need to calculate aMT2 -> need two b-jets (use 80% eff) (if not two b-jets, use hardest other jet)
              double amT2=0.;
              if(bTag_jets.size() >1){    
                double amT2_1 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[0]->P4(),bTag_jets[1]->P4(), missingET->P4());
                double amT2_2 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[1]->P4(), bTag_jets[0]->P4(), missingET->P4());
                amT2 = std::min(amT2_1,amT2_2);
              }
              else if(bTag_jets.size() == 1){    
                double amT2_1 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[0]->P4(),notbTag_jets[0]->P4(), missingET->P4());
                double amT2_2 = AnalysisBase::mT2_bl(lepNorm, notbTag_jets[0]->P4(), bTag_jets[0]->P4(), missingET->P4());
                amT2 = std::min(amT2_1,amT2_2);
              }
              
              if(amT2 > 190.){
                countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_14_aMT2");
              
                //Tau veto
                bool tauVeto = false;
                int lepCharge=0;
                if (lepNormFlag == 0) lepCharge = electronsBaseNorm[0]->Charge;
                else lepCharge = muonsBaseNorm[0]->Charge;
              
                for(int i = 0; i < jetsTau.size(); i++){
                  if( (checkTauTag(jetsTau[i],"tight")==true) && (jetsTau[i]->Charge*lepCharge <0.)) tauVeto=true;
                }
                if(tauVeto == false){  
                  countCutflowEvent("tNbC_mix_"+lep[lepNormFlag]+"_15_tauVeto");

                  /// Topness variable should come now
                  /// Note included yet so simply count signal events here - looking at cutflows, this should only be few% error
                
                  countSignalEvent("tNbC_mix");
                }
              }
            }
              }
            }
          }
        }
          }
        }
      }
    }
      }
    }
    
  //************************************************************************************
  // (>= 2 b-jets) (80%) bCb_med2, bCd_high1, bCd_high2) 
  //-------------------------------------------------------------------
  
  //make sure remaing signal regions are still allowed
  if( (bCb_med2 == false) && (bCd_high1 == false) && (bCd_high2 == false) )
    return;
  
  //b-jet PT > 60
  ///Have chosen 85% tagging efficiency for better cutflow agreement
  std::vector<Jet*>  bTag_jets;
  std::vector<Jet*>  notbTag_jets;
  for(int i = 0; i < jetsNorm.size(); i++){
    if(checkBTag(jetsNorm[i],4) == true) bTag_jets.push_back(jetsNorm[i]);
    else notbTag_jets.push_back(jetsNorm[i]);
  }
  
  if(bTag_jets.size() <2)
    return;
  
  if(bCb_med2 == true) countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_03_bTag"); 
  if(bCd_high1 == true) countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_03_bTag"); 
  if(bCd_high2 == true) countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_03_bTag");
  
  //************************************************************************************
  // bCb_med2 cutflow
  //-------------------------------------------------------------------
  
  //bJet1 > 140.
  if((bCb_med2 == true) && (bTag_jets[0]->PT > 140.)){
    countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_04_bJet1PT"); 
    
    //bJet2 > 75.
    if(bTag_jets[1]->PT > 75.){
      countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_05_bJet2PT"); 
      
      // dPhi(jet1, EtMiss) > 0.8
      if( fabs(jetsNorm[0]->P4().DeltaPhi(missingET->P4())) > 0.8){
    countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_06_dPhiJet1");

    // dPhi(jet2, EtMiss) > 0.8
    if( fabs(jetsNorm[1]->P4().DeltaPhi(missingET->P4())) > 0.8){
      countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_07_dPhiJet2");
      
      //EtMiss > 170
      if(missingET->P4().Et() >170.){
        countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_08_EtMiss");
          
        // EtMiss/sqrt(HT) > 6
        // HT is sum of four jets
        double HT = 0.;
        for(int i = 0; i < jetsNorm.size(); i++){
          if( i < 4) HT += jetsNorm[i]->PT;
        } 
        if( (missingET->P4().Et()/ sqrt(HT)) > 6) {
          countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_09_met/ht");
            
          // mT > 60
          /// Did they really mean 60???? Signal regions have 90 and 120!!!!!!!!!!
          double mT = AnalysisBase::mT(lepNorm, missingET->P4());
          if(mT > 60){
        countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_10_mT");
        
        // aMT2 > 175. (250 for cutflow) 
        double amT2_1 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[0]->P4(),bTag_jets[1]->P4(), missingET->P4());
        double amT2_2 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[1]->P4(), bTag_jets[0]->P4(), missingET->P4());
        double amT2 = std::min(amT2_1,amT2_2);
              
        if(amT2 > 175.){
          bool amT2_250=false;
          if(amT2 > 250.){
            countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_11_aMT2");
            amT2_250 = true;
          }
          
          //Veto on isolated tracks
          std::vector<Track*> tracksHard = filterPhaseSpace(tracks, 10., -2.5, 2.5);  
          tracksHard = overlapRemoval(tracksHard, electronsBaseNorm, 0.4);
          tracksHard = overlapRemoval(tracksHard, muonsBaseNorm, 0.4);
          
          //Check no other hard track lies close
          std::vector<Track*> tracksIso_a;
          for(int i = 0; i < tracksHard.size(); i++){
            bool isoTest = true;
            for(int j = 0; j < tracksHard.size(); j++){
              if(i!=j) {
            if(tracksHard[i]->P4().DeltaR(tracksHard[j]->P4()) <0.4) isoTest = false;
              }
            }
            if(isoTest==true) tracksIso_a.push_back(tracksHard[i]);
          }
            
          //Check no soft track lies close
          std::vector<Track*> tracksIso_b;
          for(int i = 0; i < tracksIso_a.size(); i++){
            bool isoTest = true;
            for(int j = 0; j < tracks.size(); j++){
              if( (tracks[j]->PT >3.) && (tracks[j]->PT <10.) && (fabs(tracks[j]->Eta) <2.5)){
            if(tracksIso_a[i]->P4().DeltaR(tracks[j]->P4()) <0.4) isoTest = false;
              }
            }
            if(isoTest==true) tracksIso_b.push_back(tracksIso_a[i]);
          }
          
          //if remaining track, check if charge is opposite
          int lepCharge=0;
          bool isoCheck =true;
          if (lepNormFlag == 0) lepCharge = electronsBaseNorm[0]->Charge;
          else lepCharge = muonsBaseNorm[0]->Charge;
          for(int i = 0; i < tracksIso_b.size(); i++){
            if(lepCharge*tracksIso_b[i]->Charge < 0.) isoCheck = false;
          }
          
          if(isoCheck == true){
            if(amT2_250 == true) countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_12_trackIso");
            
            //Tau veto
            bool tauVeto = false;
            lepCharge=0;  //reset lepCharge
            if (lepNormFlag == 0) lepCharge = electronsBaseNorm[0]->Charge;
            else lepCharge = muonsBaseNorm[0]->Charge;
              
            for(int i = 0; i < jetsTau.size(); i++){
              if( (checkTauTag(jetsTau[i],"tight")==true) && (jetsTau[i]->Charge*lepCharge <0.)) tauVeto=true;
            }
            if(tauVeto == false){  
              if(amT2_250 == true) countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_13_tauVeto");

              /// Topness variable should come now > 7.5
              /// Note included yet so simply count signal events here - looking at cutflows, this should only be few% error
              
              /// Final signal regions also have m_T > 90 GeV and m_bb > 150 GeV.
              double m_bb = (bTag_jets[0]->P4()+bTag_jets[1]->P4()).M(); 
              if( (mT > 90.0) && (m_bb > 150.0) ){
                //countCutflowEvent("bCb_med2_"+lep[lepNormFlag]+"_14_mbb");
                if(amT2_250 == true){  
                  if(mT > 120) countSignalEvent("bCb_med2_d");
                  else  countSignalEvent("bCb_med2_c");
                }
                else{
                  if(mT > 120) countSignalEvent("bCb_med2_b");
                  else countSignalEvent("bCb_med2_a");
                }
              }
            }
          }
        }
          }
        }
      }
    }
      }
    }    
  }

  
    
  //************************************************************************************
  //  bCd_high1, bCd_high2 >= 2 b-jets (80%)  
  //-------------------------------------------------------------------
  
   //bJet1 > 75 (bCd_high1) > 170 (bCd_high2) .
  if((bCd_high1 == true) && (bTag_jets[0]->PT > 75.)){
    countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_04_bJet1PT"); 
    if((bCd_high2 == true) && (bTag_jets[0]->PT > 170.)) countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_04_bJet1PT");
    else bCd_high2 = false;
    
    //bJet2 > 75 (bCd_high1) > 80 (bCd_high2) ..
    if(bTag_jets[1]->PT > 75.){
      countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_05_bJet2PT"); 
      if((bCd_high2 == true) && (bTag_jets[0]->PT > 80.)) countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_05_bJet2PT");
      else bCd_high2 = false;
      
      // dPhi(jet1, EtMiss) > 0.8
      if( fabs(jetsNorm[0]->P4().DeltaPhi(missingET->P4())) > 0.8){
    countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_06_dPhiJet1");
    if(bCd_high2 == true) countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_06_dPhiJet1");

    // dPhi(jet2, EtMiss) > 0.8
    if( fabs(jetsNorm[1]->P4().DeltaPhi(missingET->P4())) > 0.8){
      countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_07_dPhiJet2");
      if(bCd_high2 == true) countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_07_dPhiJet2");
      
      //EtMiss > 150 (bCd_high1) > 160 (bCd_high2) 
      if(missingET->P4().Et() >150.){
        countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_08_EtMiss");
        if((bCd_high2 == true) && (missingET->P4().Et() >160.)) countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_08_EtMiss");
        else bCd_high2 = false;
          
        // EtMiss/sqrt(HT) > 9 (bCd_high1) > 8 (bCd_high2) 
        // HT is sum of four jets
        double HT = 0.;
        for(int i = 0; i < jetsNorm.size(); i++){
          if( i < 4) HT += jetsNorm[i]->PT;
        } 
        if((bCd_high1 == true) && ((missingET->P4().Et()/ sqrt(HT)) > 9))  countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_09_met/ht");
        else (bCd_high1 = false);
        if((bCd_high2 == true) && ((missingET->P4().Et()/ sqrt(HT)) > 8))  countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_09_met/ht");
        else (bCd_high2 = false);
        if((bCd_high1 == false) && (bCd_high2 == false))
          return;
        
        // mT > 120
        double mT = AnalysisBase::mT(lepNorm, missingET->P4());
        if(mT < 120)
          return;
        countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_10_mT");
        countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_10_mT");    
        
        // meff > 600
        double meff = lepNorm.Pt() + missingET->P4().Et();
        for(int i = 0; i < jetsNorm.size(); i++){
          if(i<4) meff += jetsNorm[i]->PT;
        } 
        if((bCd_high1 == true) && (meff > 600.)) countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_11_meff");
        else (bCd_high1 = false);
        if((bCd_high1 == false) && (bCd_high2 == false))
          return;
        countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_11_dummy");
          
        // aMT2 > 200 (bCd_high1) > 250 (bCd_high2) 
        double amT2_1 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[0]->P4(),bTag_jets[1]->P4(), missingET->P4());
        double amT2_2 = AnalysisBase::mT2_bl(lepNorm, bTag_jets[1]->P4(), bTag_jets[0]->P4(), missingET->P4());
        double amT2 = std::min(amT2_1,amT2_2);  
        if((bCd_high1 == true) && (amT2 > 200.))  countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_12_aMT2");
        else (bCd_high1 = false);
        if((bCd_high2 == true) && (amT2 > 250.))  countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_12_aMT2");
        else (bCd_high2 = false);
        if((bCd_high1 == false) && (bCd_high2 == false))
          return;
            
        //Veto on isolated tracks
        std::vector<Track*> tracksHard = filterPhaseSpace(tracks, 10., -2.5, 2.5);  
        tracksHard = overlapRemoval(tracksHard, electronsBaseNorm, 0.4);
        tracksHard = overlapRemoval(tracksHard, muonsBaseNorm, 0.4);
          
        //Check no other hard track lies close
        std::vector<Track*> tracksIso_a;
        for(int i = 0; i < tracksHard.size(); i++){
          bool isoTest = true;
          for(int j = 0; j < tracksHard.size(); j++){
        if(i!=j) {
          if(tracksHard[i]->P4().DeltaR(tracksHard[j]->P4()) <0.4) isoTest = false;
        }
          }
          if(isoTest==true) tracksIso_a.push_back(tracksHard[i]);
        }
            
        //Check no soft track lies close
        std::vector<Track*> tracksIso_b;
        for(int i = 0; i < tracksIso_a.size(); i++){
          bool isoTest = true;
          for(int j = 0; j < tracks.size(); j++){
        if( (tracks[j]->PT >3.) && (tracks[j]->PT <10.) && (fabs(tracks[j]->Eta) <2.5)){
          if(tracksIso_a[i]->P4().DeltaR(tracks[j]->P4()) <0.4) isoTest = false;
        }
          }
          if(isoTest==true) tracksIso_b.push_back(tracksIso_a[i]);
        }
          
        //if remaining track, check if charge is opposite
        int lepCharge=0;
        bool isoCheck =true;
        if (lepNormFlag == 0) lepCharge = electronsBaseNorm[0]->Charge;
        else lepCharge = muonsBaseNorm[0]->Charge;
        for(int i = 0; i < tracksIso_b.size(); i++){
          if(lepCharge*tracksIso_b[i]->Charge < 0.) 
        return;
        }
          
        if(bCd_high1 == true) countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_13_isoTrack");
        if(bCd_high2 == true) countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_13_isoTrack");
            
        //Tau veto
        bool tauVeto = false;
        lepCharge=0;   //reset lepCharge
        if (lepNormFlag == 0) lepCharge = electronsBaseNorm[0]->Charge;
        else lepCharge = muonsBaseNorm[0]->Charge;
        
        for(int i = 0; i < jetsTau.size(); i++){
          if( (checkTauTag(jetsTau[i],"tight")==true) && (jetsTau[i]->Charge*lepCharge <0.)) tauVeto=true;
        }
        if(tauVeto == false){  
          if(bCd_high1 == true){
        countCutflowEvent("bCd_high1_"+lep[lepNormFlag]+"_14_tauVeto");
        countSignalEvent("bCd_high1");
          }
          if(bCd_high2 == true){
        countCutflowEvent("bCd_high2_"+lep[lepNormFlag]+"_14_tauVeto");
        countSignalEvent("bCd_high2");
          }
        }
      }
    }
      }
    }
  }

}

void Atlas_1407_0583::finalize() {
  // Whatever should be done after the run goes here
}       
