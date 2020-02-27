#include "atlas_conf_2013_062.h"

std::string Atlas_conf_2013_062::id[numCutDiMu] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
std::string Atlas_conf_2013_062::idLep[numLep] = {"El", "Mu"};
std::string Atlas_conf_2013_062::idHardLepJet[numHardLepJet] = {"J3", "J5", "J6"};
std::string Atlas_conf_2013_062::idHardLepOrtho[numHardLepOrtho] = { "Inclu", "Ortho"};

void Atlas_conf_2013_062::initialize() {
  setInformation(""
    "@#ATLAS\n"
     "@#ATLAS-CONF-2013-062\n"
     "@#1-2 leptons + 3-6 jets + Etmiss [Incl, squarks & gluinos, mUED]\n"
     "@#Energy: 8 TeV\n"
     "@#Luminosity: 20.1 fb^-1\n"
     "@#UNVALIDATED !!!!!!!!!!!!!!!!;;\n"
  "");
  setLuminosity(20.1*units::INVFB);      
  setAnalysisName("atlas_conf_2013_062");          
//  ignore("genJets");
  ignore("towers");
//  ignore("genParticles");
//  ignore("photons");
  bookSignalRegions("SoftLep1BLow;SoftLep1BHigh;SoftLep2BLow;SoftLep2BHigh;SoftLepJ3;SoftLepJ5;DiMuon;HardElecJ3Inclu;HardElecJ3Ortho;HardElecJ5Inclu;HardElecJ5Ortho;HardElecJ6Inclu;HardElecJ6Ortho;HardMuonJ3Inclu;HardMuonJ3Ortho;HardMuonJ3Inclu;HardMuonJ5Inclu;HardMuonJ5Ortho;HardMuonJ6Inclu;HardMuonJ6Ortho;");
  
  bookCutflowRegions("0;1;2");  
  bookCutflowRegions("CFSingSoftLep0;CFSingSoftLep1;CFSingSoftLep2");
    for(int i = 0; i < numMass; i++){
        for(int j = 0; j < numCutSingSoftLep1B; j++) bookCutflowRegions("CFSingSoftLep1B"+id[i]+id[j]);    
    for(int j = 0; j < numCutSingSoftLep2B; j++) bookCutflowRegions("CFSingSoftLep2B"+id[i]+id[j]);    
    for(int j = 0; j < numCutSingSoftLepMulti; j++)  bookCutflowRegions("CFSingSoftLepMulti"+id[i]+id[j]);
    }  
    for(int i = 0; i < numLep; i++){
    for(int j = 0; j < numHardLepOrtho; j++){
        for(int k = 0; k < numHardLepJet; k++){
        for(int l = 0; l < numCutHardLep; l++) bookCutflowRegions("CFHardLep"+idLep[i]+idHardLepJet[k]+idHardLepOrtho[j]+id[l]);
        }
    }
    } 
    for(int i = 0; i < numCutDiMu; i++) bookCutflowRegions("CFDiMu"+id[i]);
    
  // You should initialize any declared variables here
}

void Atlas_conf_2013_062::analyze() {
    // Your eventwise analysis code goes here
    missingET->addMuons(muonsCombined);
    electronsMedium = filterPhaseSpace(electronsMedium, 7., -2.47, 2.47);
    int nElectronsMedium = electronsMedium.size();       
    muonsCombined = filterPhaseSpace(muonsCombined, 6., -2.4, 2.4);
    int nMuonsCombined = muonsCombined.size();    
    jets = filterPhaseSpace(jets, 20., -2.5, 2.5);
    int nJets = jets.size();           
    countCutflowEvent("0");

    /// NEED TO ASK WHAT ETA IS REQUIRED FOR JETS AND B-JETS!!!!!!!!!!!!!!!!!!!!!!!
    /// Jet eta is 2.5
    /// b-Jet eta is 2.5
    /// 3-jet is not only 3-jet, 4th-jet is also allowed in this sample
    // pre-selected jets required to have pT > 20 GeV
    // signal jets, soft: pT > 25 GeV, hard: pT > 30 GeV
    // b-jets, 60% (Soft/hard single lep, Soft single lep +2b), 70% (Soft single lep + 1b) and 80% (Soft dimuon) algorithms used.
    // pre-selected electrons are 'medium', |eta| < 2.47, soft: pT > 7 GeV, hard: pT > 10 GeV
    // soft signal electrons, pT > 10 GeV, tracks < 16% in dR=0.3
    // hard signal electrons, pT > 25 GeV, tracks < 10% in dR=0.2
    // pre-selected muons, |eta| < 2.4, soft: pT > 6 GeV, hard: pT > 10 GeV
    // soft signal muons, tracks < 12% in dR=0.3
    // hard signal muons, tracks < 1.8 GeV in dR=0.2
  
    //------------------------
    // To start remove jets that overlap with electrons
    jets = overlapRemoval(jets, electronsMedium, 0.2);
    nJets = jets.size();
    
    //---------------------------
    // Remove any baseline muon that overlaps with a jet
    muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
    nMuonsCombined = muonsCombined.size();
    
    //---------------------------
    // Remove any baseline electron that overlaps with a jet
    electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
    nElectronsMedium = electronsMedium.size();
  
    //------------------------
    // Veto events with electrons in transition region
    for(int e = 0; e < nElectronsMedium; e++) { 
        double absEta = fabs(electronsMedium[e]->Eta);
        if( (absEta > 1.37) && (absEta < 1.52) )
        return;
    } 
    countCutflowEvent("1");
    
    //------------------------
    // Define signal leptons
    std::vector<Electron*> electronsSoft = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
    electronsSoft = filterIsolation(electronsSoft,0);
    int nElectronsSoft = electronsSoft.size();   
    std::vector<Electron*> electronsHard = filterPhaseSpace(electronsMedium, 25., -2.47, 2.47);
    electronsHard = filterIsolation(electronsHard,1);
    int nElectronsHard = electronsHard.size(); 
    std::vector<Muon*> muonsSoft = filterPhaseSpace(muonsCombined, 6., -2.4, 2.4);
    muonsSoft = filterIsolation(muonsSoft,0);
    int nMuonsSoft = muonsSoft.size();   
    std::vector<Muon*> muonsHard = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
    muonsHard = filterIsolation(muonsHard,1);
    int nMuonsHard = muonsHard.size();  
    
    //------------------------
    // Triggers used
    bool trigHardElec = false;
    bool trigHardMuon = false;
    bool trigSoftLep = false;
    if( (nElectronsHard>0) && (missingET->PT > 80.0) ) trigHardElec = true;
    if( (nMuonsHard>0) && (muonsHard[0]->PT>25.0) && (nJets>0) && (jets[0]->PT>80.0) && (missingET->PT > 100.0) ) trigHardMuon = true;
//    if( (nJets>0) && (jets[0]->PT>60.0) && (missingET->PT > 150.0) ) trigSoftLep = true;
    if(  missingET->PT > 80.0 ) trigSoftLep = true;
    if( (trigHardElec == false) && (trigHardMuon == false) && (trigSoftLep == false))
        return;
    countCutflowEvent("2");
    
    //*****************************************************************************************
    //--------------------------
    //------Soft single lepton cutflow------
    //--------------------------
    // Exactly one soft lepton
    TLorentzVector singSoftLepton;
    bool flagSingSoftLep = false;
    if((trigSoftLep == true) && ((nMuonsSoft + nElectronsSoft) == 1)){
        if(nMuonsSoft==1) singSoftLepton = muonsSoft[0]->P4();
        else singSoftLepton = electronsSoft[0]->P4();
        countCutflowEvent("CFSingSoftLep0");
    flagSingSoftLep = true;
    }
    //--------------------------
    // No other leptons
    if((flagSingSoftLep == true) && ((nMuonsCombined + nElectronsMedium) == 1)) countCutflowEvent("CFSingSoftLep1"); 
    else flagSingSoftLep = false;
    //--------------------------
    // Leptons < 25GeV
    if((flagSingSoftLep == true) && (singSoftLepton.Pt() < 25.0)) countCutflowEvent("CFSingSoftLep2");
    else flagSingSoftLep = false;
    //--------------------------
    //------Soft single lepton + 1 b-jet cutflow------
    //--------------------------
    bool flagSingLep1B = true;
    bool flagSingLep1BLow = false;
    bool flagSingLep1BHigh = false;
    if(flagSingSoftLep == true){
    //--------------------------
    // Leptons should be separated from jet (|eta|<2.5) by dR > 1
        for(int j = 0; j < nJets; j++) {
            if( (fabs(jets[j]->Eta) < 2.5 ) && (jets[j]->P4().DeltaR(singSoftLepton) < 1.0)) {
                flagSingLep1B = false;
                break;
            }
        }
        //--------------------------
    // MissingET > 250 GeV (low), 300 GeV (high)
        if(flagSingLep1B == true){
        countCutflowEvent("CFSingSoftLep1B00");
        countCutflowEvent("CFSingSoftLep1B10");
        if(missingET->PT >250.0){
            flagSingLep1BLow = true;
            countCutflowEvent("CFSingSoftLep1B01");
        if(missingET->PT >300.0){
            countCutflowEvent("CFSingSoftLep1B11");
            flagSingLep1BHigh = true;
        }
        }
    }
    //--------------------------
    // jet PT > 180 GeV
    if( (flagSingLep1BLow == true) && (nJets > 0) && (jets[0]->PT > 180.0)){
        countCutflowEvent("CFSingSoftLep1B02");
        if(flagSingLep1BHigh == true) countCutflowEvent("CFSingSoftLep1B12");
    }
    else{
        flagSingLep1BLow = false;
        flagSingLep1BHigh = false;
    }
    //--------------------------
    // mT > 100 GeV    
    if( flagSingLep1BLow == true){
        double mT = sqrt(2.*singSoftLepton.Pt()*missingET->PT*(1.-cos(singSoftLepton.DeltaPhi(missingET->P4()))));
        if(mT > 100.0){
        countCutflowEvent("CFSingSoftLep1B03");
        if(flagSingLep1BHigh == true) countCutflowEvent("CFSingSoftLep1B13");
        }
        else{
        flagSingLep1BLow = false;
        flagSingLep1BHigh = false;
        }
    }
    //--------------------------
    // number of jets > 2
    if(( flagSingLep1BLow == true) && (nJets > 2)){
        countCutflowEvent("CFSingSoftLep1B04");
        if(flagSingLep1BHigh == true) countCutflowEvent("CFSingSoftLep1B14");
    }
    else{
        flagSingLep1BLow = false;
        flagSingLep1BHigh = false;
    }
    //--------------------------
    // jets 3 > 40 GeV (low), > 25 GeV (high)
    if(( flagSingLep1BLow == true) && (jets[2]->PT > 40.0)) countCutflowEvent("CFSingSoftLep1B05");
    else flagSingLep1BLow = false;
    if((flagSingLep1BHigh == true) && (jets[2]->PT > 25.0)) countCutflowEvent("CFSingSoftLep1B15");
    else flagSingLep1BHigh = false;
    //--------------------------
    // 1 b-tag (not leading jet)
    bool singLepBTag = false;
    for(int j = 1; j < nJets; j++) {
            if( checkBTag(jets[j],1) == true) {   // 1 index refers to 70% BTag efficiency
                singLepBTag = true;
                break;
            }
        }
        if(( flagSingLep1BLow == true) && (singLepBTag == true)) countCutflowEvent("CFSingSoftLep1B06");
    else flagSingLep1BLow = false;
    if(( flagSingLep1BHigh == true) && (singLepBTag == true)) countCutflowEvent("CFSingSoftLep1B16");
    else flagSingLep1BHigh = false;
    //--------------------------
    // ETmiss/meff > 0.35
    if(( flagSingLep1BLow == true) || ( flagSingLep1BHigh == true)){
        double meff = missingET->PT + singSoftLepton.Pt();
        for(int j = 0; j < nJets; j++) {
        meff += jets[j]->PT;
        }
        if(missingET->PT/meff > 0.35){
        if( flagSingLep1BLow == true) countCutflowEvent("CFSingSoftLep1B07");
        if( flagSingLep1BHigh == true) countCutflowEvent("CFSingSoftLep1B17");
        }
        else{
        flagSingLep1BLow = false;
        flagSingLep1BHigh = false;
        }
    }
    }
    
    //--------------------------
    //------Soft single lepton + 2 b-jets cutflow------
    //--------------------------
    bool flagSingLep2BLow = false;
    bool flagSingLep2BHigh =false;
    if(flagSingSoftLep == true){
    //--------------------------
    // ETmiss > 200 (low), > 300 (high)
    if(missingET->PT >200.0){ 
        flagSingLep2BLow = true;
        countCutflowEvent("CFSingSoftLep2B00");
        if(missingET->PT >300.0){
        flagSingLep2BHigh = true;
        countCutflowEvent("CFSingSoftLep2B10");
        }
    }
    //--------------------------
    // At least 2 jets, jet 2, PT > 60 GeV    
    if((flagSingLep2BLow == true) && (nJets>1) && (jets[1]->PT > 60.0) ){
        countCutflowEvent("CFSingSoftLep2B01");
        if( flagSingLep2BHigh == true) countCutflowEvent("CFSingSoftLep2B11");
    }
    else{
        flagSingLep2BLow = false;
        flagSingLep2BHigh = false;
    }
    //--------------------------
    // if third jet, PT < 50 GeV
    if((flagSingLep2BLow == true) && (nJets>2) && (jets[2]->PT < 50.0) ){
        countCutflowEvent("CFSingSoftLep2B02");
        if( flagSingLep2BHigh == true)countCutflowEvent("CFSingSoftLep2B12");
    }
    else{
        flagSingLep2BLow = false;
        flagSingLep2BHigh = false;
    }
    //--------------------------
    // 2 b-tags
    std::vector<Jet*> bJets;
    if(flagSingLep2BLow == true ){
        for(int j = 0; j < nJets; j++) {
        if( checkBTag(jets[j],0) == true) { // 0 index refers to 60% BTag efficiency
            bJets.push_back(jets[j]);
        }    
            }
        if( bJets.size() == 2){
        countCutflowEvent("CFSingSoftLep2B03");
        if( flagSingLep2BHigh == true) countCutflowEvent("CFSingSoftLep2B13");
        }
        else{
        flagSingLep2BLow = false;
        flagSingLep2BHigh = false;
        }
    }
    //--------------------------
    // phi separation between leading 2 jets and MET > 0.4
    if((flagSingLep2BLow == true) && (jets[0]->P4().DeltaPhi(missingET->P4()) > 0.4) && (jets[1]->P4().DeltaPhi(missingET->P4()) > 0.4) ){
        countCutflowEvent("CFSingSoftLep2B04");
        if( flagSingLep2BHigh == true) countCutflowEvent("CFSingSoftLep2B14");
    }
    else{
        flagSingLep2BLow = false;
        flagSingLep2BHigh = false;
    }
    //--------------------------
    // mCT > 150 GeV (low), 200 GeV (high)
    if(flagSingLep2BLow == true){
        double mCT = sqrt(pow(bJets[0]->P4().Et() + bJets[1]->P4().Et(),2) - pow(bJets[0]->PT - bJets[1]->PT,2));
        if (mCT > 150.0){
        countCutflowEvent("CFSingSoftLep2B05");
        if( (flagSingLep2BHigh == true) && (mCT > 200.0)) countCutflowEvent("CFSingSoftLep2B15");
        else flagSingLep2BHigh = false;
        }
        else{
        flagSingLep2BLow = false;
        flagSingLep2BHigh = false;
        }
    }
    //--------------------------
    // HT,2 < 50 GeV (low)
    if(flagSingLep2BLow == true){
        double HT2 = 0.;
        for(int j = 2; j < nJets; j++) {
        HT2 += jets[j]->PT;
        }
        if(HT2 < 50) countCutflowEvent("CFSingSoftLep2B06");
        else flagSingLep2BLow = false;
    }
    }
    
    //--------------------------
    //------Soft single lepton + >= 3 jets cutflow------
    //--------------------------
    bool flagSingLep3J =false;
    bool flagSingLep5J =false;
    //--------------------------
    // Common cutflow, ETmiss > 200 GeV, jet1 PT > 180 GeV
    if( (flagSingSoftLep == true) && (missingET->PT > 200.) && (nJets >0) && (jets[0]->PT > 180.0)){
        countCutflowEvent("CFSingSoftLepMulti00");
    countCutflowEvent("CFSingSoftLepMulti10");
    std::vector<Jet*> jetsMulti = filterPhaseSpace(jets, 25., -4.5, 4.5);
    int nJetsMulti = jetsMulti.size();
    //--------------------------
    //Signal regions decided by number of jets (3,4) or (5+)
    if( (nJetsMulti==3) || (nJetsMulti==4) ){
        flagSingLep3J =true;
        countCutflowEvent("CFSingSoftLepMulti01");
    }
    else if (nJetsMulti>4){
        flagSingLep5J =true;
        countCutflowEvent("CFSingSoftLepMulti11");
    }        
    //--------------------------
    // mT > 100 GeV    
    if((flagSingLep3J ==true) || (flagSingLep5J ==true)){
        double mT = sqrt(2.*singSoftLepton.Pt()*missingET->PT*(1.-cos(singSoftLepton.DeltaPhi(missingET->P4()))));
        if(mT > 100.0){
            if(flagSingLep3J ==true) countCutflowEvent("CFSingSoftLepMulti02");
                if(flagSingLep5J ==true) countCutflowEvent("CFSingSoftLepMulti12");
        }
        else{
                   flagSingLep3J =false;
            flagSingLep5J =false;
        }
    }
    //--------------------------
    // ETmiss/meff > 0.3
    if((flagSingLep3J ==true) || (flagSingLep5J ==true)){
        double meff = missingET->PT + singSoftLepton.Pt();
        for(int j = 0; j < nJetsMulti; j++) {
                meff += jetsMulti[j]->PT;
            }
            if(missingET->PT/meff > 0.30){
            if(flagSingLep3J ==true) countCutflowEvent("CFSingSoftLepMulti03");
            if(flagSingLep5J ==true) countCutflowEvent("CFSingSoftLepMulti13");
        }
        else{
            flagSingLep3J =false;
            flagSingLep5J =false;
        }
    }
    //--------------------------
    // ETMiss > 400 GeV (3J), > 300 GeV (5J)    
    if( (flagSingLep3J ==true) && (missingET->PT > 400.0)) countCutflowEvent("CFSingSoftLepMulti04");
    else flagSingLep3J =false;
    if( (flagSingLep5J ==true) && (missingET->PT > 300.0)) countCutflowEvent("CFSingSoftLepMulti14");
    else flagSingLep5J =false;    
    //--------------------------
    // Leptons should be separated from jet (|eta|<2.5) by dR > 1 (3J)
    if (flagSingLep3J ==true){
        for(int j = 0; j < nJetsMulti; j++) {
        if( (fabs(jetsMulti[j]->Eta) < 2.5 ) && (jetsMulti[j]->P4().DeltaR(singSoftLepton) < 1.0)) {
            flagSingLep3J = false;
            break;
        }
        }
        if(flagSingLep3J ==true) countCutflowEvent("CFSingSoftLepMulti05");
        }    
    }
    
    //--------------------------
    //------Soft di-muon cutflow ------
    //--------------------------
    bool flagDiMuon = false;
    if((trigSoftLep == true) && (nMuonsSoft == 2) && (muonsSoft[0]->Charge)*(muonsSoft[1]->Charge) < 0){
        countCutflowEvent("CFDiMu0");
    //--------------------------
    // ETMiss > 170 GeV    
    if(missingET->PT > 170.0){
        flagDiMuon = true;
        countCutflowEvent("CFDiMu1");
    }
    //--------------------------
    // More than 2 jets > 25 GeV
    std::vector<Jet*> jetsDiMu = filterPhaseSpace(jets, 25., -4.5, 4.5);
    int nJetsDiMu = jetsDiMu.size();
    if((flagDiMuon == true) && (nJetsDiMu > 1)) countCutflowEvent("CFDiMu2");
    else flagDiMuon = false;  
    //--------------------------
    // jet 1 pt > 70
    if((flagDiMuon == true) && (jetsDiMu[0]->PT > 70.0)) countCutflowEvent("CFDiMu3");
    else flagDiMuon = false;    
    //--------------------------
    // di-muon mass > 15 GeV
    TLorentzVector diMuVec;
    if(flagDiMuon == true){
        diMuVec = muonsSoft[0]->P4() + muonsSoft[1]->P4();
        if(diMuVec.M() > 15.0) countCutflowEvent("CFDiMu4");
        else flagDiMuon = false;
    }
    //--------------------------
    // |di-muon mass - Zmass| > 10 GeV
    if(flagDiMuon == true){
        if( (diMuVec.M() < 81.1876) || (diMuVec.M() > 101.1876) ) countCutflowEvent("CFDiMu5");
        else flagDiMuon = false;
    }
    //--------------------------
    // Softer muon should be separated from jet (|eta|<2.5) by dR > 1 (3J)
    if (flagDiMuon ==true){
        for(int j = 0; j < nJetsDiMu; j++) {
        if( (fabs(jetsDiMu[j]->Eta) < 2.5 ) && (jetsDiMu[j]->P4().DeltaR(muonsSoft[1]->P4()) < 1.0)) {
            flagDiMuon = false;
            break;
        }
        }
        if (flagDiMuon ==true) countCutflowEvent("CFDiMu6");
        }
        //--------------------------
    // Hardest muon pt < 25 GeV
    if( (flagDiMuon ==true) && (muonsSoft[0]->PT < 25.0)) countCutflowEvent("CFDiMu7");
    else flagDiMuon =false;
    //--------------------------
    // mT > 80 GeV    
    if(flagDiMuon ==true){
        double mT = sqrt(2.*muonsSoft[1]->PT*missingET->PT*(1.-cos(muonsSoft[1]->P4().DeltaPhi(missingET->P4()))));
        if(mT > 80.0) countCutflowEvent("CFDiMu8");
        else flagDiMuon = false;
    }
    //--------------------------
    // b-veto
    if(flagDiMuon ==true){
        for(int j = 0; j < nJetsDiMu; j++) {
            if(fabs(jetsDiMu[j]->Eta)< 2.5 &&  checkBTag(jetsDiMu[j],2) == true){ // 2 index refers to 80% BTag efficiency
            flagDiMuon = false;
            break;
        }
        }
    }        
    if(flagDiMuon) countCutflowEvent("CFDiMu9");
    }
    
    //--------------------------
    //------Inclusive hard single-lepton cutflow ------
    //--------------------------
    int nVetoLep = 0;
    int flagLep =0;
    bool flagHardLepJ3 = false;
    bool flagHardLepJ5 = false;
    bool flagHardLepJ6 = false;
    bool flagHardLepJ3Ortho = false;
    bool flagHardLepJ5Ortho = false;
    bool flagHardLepJ6Ortho = false;
    for(int e = 0; e < nElectronsMedium; e++) {
        if(electronsMedium[e]->PT > 10.) nVetoLep +=1;
    }
    for(int e = 0; e < nMuonsCombined; e++) {
        if(muonsCombined[e]->PT > 10.) nVetoLep +=1;
    }
    //--------------------------
    // Require trigger and single reconstructed lepton (pt > 10 GeV)
    if( ((trigHardElec == true) || (trigHardMuon==true)) && (nVetoLep == 1)){
        //--------------------------
    // Store lepton 4 momentum
          TLorentzVector leptonHard;
    if(nMuonsHard == 1){
        flagLep=1;    
        leptonHard = muonsHard[0]->P4();
    }
    else leptonHard = electronsHard[0]->P4();
    //--------------------------
    // Initial cutflow
    countCutflowEvent("CFHardLep0000");
    //--------------------------
    // Require minimum number of jets 3,5,6
    if(nJets >2){
        flagHardLepJ3 = true;
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[0]+"1");
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[1]+"1");        
        if(nJets >4){
            flagHardLepJ5 = true;
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[0]+"1");
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[1]+"1");    
        if(nJets >5){
            flagHardLepJ6 = true;
            countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[0]+"1");
            countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[1]+"1");            
        }
        }
    }
    //--------------------------
    // Require minimum jet pt (80,80,30:3jet), (80,50,40,40,40:5jet), (80,50,40,40,40,40:6jet)
    if((flagHardLepJ3 == true) && (jets[0]->PT > 80.0) && (jets[1]->PT > 80.0) && (jets[2]->PT > 30.0)){
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[0]+"2");
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[1]+"2");
    }
    else {
        flagHardLepJ3 = false;
    }        
    if((flagHardLepJ5 == true) && (jets[0]->PT > 80.0) && (jets[1]->PT > 50.0) && (jets[2]->PT > 40.0) && (jets[3]->PT > 40.0) && (jets[4]->PT > 40.0)){ 
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[0]+"2");
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[1]+"2");
        if((flagHardLepJ6 == true) && (jets[5]->PT > 40.0)){
            countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[0]+"2");
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[1]+"2");    
        }
        else {
        flagHardLepJ6 = false;
        }            
    }
    else { 
        flagHardLepJ5 = false;
        flagHardLepJ6 = false;
    }
    //--------------------------
    // Binned analyis has orthogonal signal regions (for now 6jet region is same as inclusive)
    // Jet 3 has both 3 and 4 jet sample
    if(flagHardLepJ3 == true){
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[0]+"3");
        if( (nJets == 3) || (nJets == 4) || ((nJets > 4) && (jets[4]->PT < 40.0))){
            countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[1]+"3");
        flagHardLepJ3Ortho = true;
        }
    }
    if(flagHardLepJ5 == true){
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[0]+"3");
        if((nJets == 5) || (( nJets > 5) && (jets[5]->PT < 40.0))){
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[1]+"3");
        flagHardLepJ5Ortho = true;
        }
    }
    if(flagHardLepJ6 == true) {
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[0]+"3");
        countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[1]+"3");
        flagHardLepJ6Ortho = true;
    }
    //--------------------------
    // EtMiss > 500 (3j), > 300 (3j Ortho), > 300 (5j both), > 350 (6j), > 250 (6j Ortho)
    if((flagHardLepJ3 == true) && (missingET->PT > 500.0)) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[0]+"4");
    else flagHardLepJ3 = false;
    if((flagHardLepJ3Ortho == true) && (missingET->PT > 300.0)) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[1]+"4");
    else flagHardLepJ3Ortho = false;
    if((flagHardLepJ5 == true) && (missingET->PT > 300.0)) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[0]+"4");
    else flagHardLepJ5 = false;
    if((flagHardLepJ5Ortho == true) && (missingET->PT > 300.0)) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[1]+"4");
    else flagHardLepJ5Ortho = false;
    if((flagHardLepJ6 == true) && (missingET->PT > 350.0)) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[0]+"4");
    else flagHardLepJ6 = false;
    if((flagHardLepJ6Ortho == true) && (missingET->PT > 250.0)) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[1]+"4");
    else flagHardLepJ6Ortho = false;
    //--------------------------
    // mT > 150 (3j both), > 200 (5j), > 150 (5j Ortho), > 150 (6j both)
    if((flagHardLepJ3 == true) || (flagHardLepJ3Ortho == true) || (flagHardLepJ5 == true) || (flagHardLepJ5Ortho == true) || (flagHardLepJ6 == true) || (flagHardLepJ6Ortho == true)){
        double mT = sqrt(2.*leptonHard.Pt()*missingET->PT*(1.-cos(leptonHard.DeltaPhi(missingET->P4()))));
        if(mT > 150.){
        if(flagHardLepJ3 == true) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[0]+"5");
            if(flagHardLepJ3Ortho == true) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[1]+"5");
            if(flagHardLepJ5Ortho == true)  countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[1]+"5");
            if(flagHardLepJ6 == true)  countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[0]+"5");
            if(flagHardLepJ6Ortho == true) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[1]+"5");
        if( (mT > 200.) && (flagHardLepJ5 == true)) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[0]+"5");
        else flagHardLepJ5 = false;
        }
        else { 
        flagHardLepJ3 = false;
        flagHardLepJ3Ortho = false;
        flagHardLepJ5 = false;
        flagHardLepJ5Ortho = false;
        flagHardLepJ6 = false;
        flagHardLepJ6Ortho = false;
        }
    }
    //--------------------------
    // EtMiss/(meff^exclu) >0.3 (3j Both)
    if(flagHardLepJ5 == true) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[0]+"6");
    if(flagHardLepJ5Ortho == true) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[1]+"6");
    if(flagHardLepJ6 == true) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[0]+"6");
    if(flagHardLepJ6Ortho == true) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[1]+"6");
    if((flagHardLepJ3 == true) || (flagHardLepJ3Ortho == true) ){
          double meffExclu = missingET->PT + leptonHard.Pt();
        for(int j = 0; j < 3; j++) {
                meffExclu += jets[j]->PT;
            }
            if( (missingET->PT/meffExclu) > 0.3){
            if(flagHardLepJ3 == true) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[0]+"6");
        if(flagHardLepJ3Ortho == true) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[1]+"6");
        }
        else{
              flagHardLepJ3 = false;
        flagHardLepJ3Ortho = false;
        }
    }
    //--------------------------
    // meff^Inclu > 1400 (3j, 5j), > 800 (3jOrtho, 5jOrtho), > 600 (6j, 6jOrtho)
    if((flagHardLepJ3Ortho == true) || (flagHardLepJ5Ortho == true) || (flagHardLepJ6Ortho == true)){
        double meffInclu = missingET->PT + leptonHard.Pt();
        for(int j = 0; j < nJets; j++) {
                meffInclu += jets[j]->PT;
            }
            if((flagHardLepJ3 == true) && (meffInclu > 1400.0))countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[0]+"7");
        else flagHardLepJ3 = false;
        if((flagHardLepJ3Ortho == true) && (meffInclu > 800.0)) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[0]+idHardLepOrtho[1]+"7");
        else flagHardLepJ3Ortho = false;
        if((flagHardLepJ5 == true) && (meffInclu > 1400.0))countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[0]+"7");
        else flagHardLepJ5 = false;
        if((flagHardLepJ5Ortho == true) && (meffInclu > 800.0)) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[1]+idHardLepOrtho[1]+"7");
        else flagHardLepJ5Ortho = false;
        if((flagHardLepJ6 == true) && (meffInclu > 600.0)) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[0]+"7");
        else flagHardLepJ6 = false;
        if((flagHardLepJ6Ortho == true) && (meffInclu > 600.0)) countCutflowEvent("CFHardLep"+idLep[flagLep]+idHardLepJet[2]+idHardLepOrtho[1]+"7");
        else flagHardLepJ6Ortho = false;
    }       
    }
    
    //*********************************************************************************************
    //--------------------------
    //------Fill signal region numbers ------
    //--------------------------
    
    //--------------------------
    // Soft single lepton + 1bjet signal regions 
    if(flagSingLep1BLow == true) countSignalEvent("SoftLep1BLow");
    if(flagSingLep1BHigh == true) countSignalEvent("SoftLep1BHigh");   
    //--------------------------
    // Soft single lepton + 2bjet signal regions 
    if(flagSingLep2BLow == true) countSignalEvent("SoftLep2BLow");
    if(flagSingLep2BHigh == true) countSignalEvent("SoftLep2BHigh");
    //--------------------------
    // Soft single lepton signal regions   
    if(flagSingLep3J == true) countSignalEvent("SoftLepJ3");
    if(flagSingLep5J == true) countSignalEvent("SoftLepJ5");
    //--------------------------
    // Soft diMuon signal regions   
    if(flagDiMuon == true) countSignalEvent("DiMuon");
    //--------------------------
    // Hard lepton signal regions
    if(flagLep==0){
    if(flagHardLepJ3 == true) countSignalEvent("HardElecJ3Inclu");
    if(flagHardLepJ3Ortho == true) countSignalEvent("HardElecJ3Ortho");
    if(flagHardLepJ5 == true) countSignalEvent("HardElecJ5Inclu");
    if(flagHardLepJ5Ortho == true) countSignalEvent("HardElecJ5Ortho");
    if(flagHardLepJ6 == true)countSignalEvent("HardElecJ6Inclu");
    if(flagHardLepJ6Ortho == true)countSignalEvent("HardElecJ6Ortho");
    }
    if(flagLep==1){
    if(flagHardLepJ3 == true) countSignalEvent("HardMuonJ3Inclu");
    if(flagHardLepJ3Ortho == true) countSignalEvent("HardMuonJ3Ortho");
    if(flagHardLepJ5 == true) countSignalEvent("HardMuonJ5Inclu");
    if(flagHardLepJ5Ortho == true) countSignalEvent("HardMuonJ5Ortho");
    if(flagHardLepJ6 == true)countSignalEvent("HardMuonJ6Inclu");
    if(flagHardLepJ6Ortho == true)countSignalEvent("HardMuonJ6Ortho");
    }  
    
}

void Atlas_conf_2013_062::finalize() {
  // Whatever should be done after the run goes here
}       
