#include "atlas_conf_2013_089.h"

std::string Atlas_conf_2013_089_CR::ind[numCut] = { "1", "2", "3"};
std::string Atlas_conf_2013_089_CR::lep[numLep] = { "ee", "mumu", "emu"};
std::string Atlas_conf_2013_089_CR::jet[numJet] = { "J2", "J3"};
std::string Atlas_conf_2013_089_CR::z[numZ] = { "Z", "noZ"};
//std::string Atlas_conf_2013_089_CR::r05[numR05] = { "R035", "R05"};
//std::string Atlas_conf_2013_089_CR::mr[numMR] = { "MR400", "MR600", "MR800"};

void Atlas_conf_2013_089_CR::initialize() {
  setInformation(""
    "@#ATLAS\n"
     "@#ATLAS-CONF-2013-089\n"
     "@#2 leptons, razor\n"
     "@#sqrt(s) = 8 TeV\n"
     "@#int(L) = 20.3 fb^-1\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  setAnalysisName("atlas_conf_2013_089_CR");    
  ignore("towers");
  ignore("tracks");
  bookControlRegions("CRZ1;CRZ2;CRTopSF1;CRTopSF2;CRTopOF1;CRTopOF2;VRZ1;VRZ2;VRTopSF1;VRTopSF2;VRTopOF1;VRTopOF2;");
        
  bookCutflowRegions("0;1;2;3;4");
    for(int i = 0; i < numCut; i++){
        for(int j = 0; j < numLep; j++){  
            bookCutflowRegions("Lep_"+ind[i]+"_"+lep[j]);        
        }
    }         
    for(int i = 0; i < numLep; i++){
        for(int j = 0; j < numJet; j++){  
            for(int k = 0; k < numZ; k++){ 
                bookCutflowRegions("Z_"+lep[i]+"_"+jet[j]+"_"+z[k]);
            }
            bookCutflowRegions("Jet_"+lep[i]+"_"+jet[j]);
        }
    }
}

void Atlas_conf_2013_089_CR::analyze() {
    ///UNVALIDATED
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47, true);  //true removes electrons in transition region of detector
  int nElectronsMedium = electronsMedium.size();    
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47, true);  //true removes electrons in transition region of detector
  electronsTight = filterIsolation(electronsTight);
  int nElectronsTight = electronsTight.size();  
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  int nMuonsCombined = muonsCombined.size();
  std::vector<Muon*> muonsCombinedIso = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  muonsCombinedIso = filterIsolation(muonsCombinedIso);
  int nMuonsCombinedIso = muonsCombinedIso.size(); 
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  int nJets = jets.size();            
  tracks = filterPhaseSpace(tracks, 0.5, -2.5, 2.5); 
  int nTracks = tracks.size();
  countCutflowEvent("0");
        
    // Baseline electrons medium, pT > 10 GeV, |eta| < 2.47, Not within 1.37 < |eta| < 1.52
    // Signal electrons tight,  pT > 10 GeV, |eta| < 2.47, Not within 1.37 < |eta| < 1.52, Sum charged tracks < 10% within dR=0.2
    // Baseline muons, pT > 10 GeV, |eta| < 2.4 
    // Signal muons, pT > 10 GeV, |eta| < 2.4, sum charged tracks < 1.8 GeV within dR=0.2
    // Jets anti-kt, R=0.4, pT > 20 GeV
    // Signal jets, pT > 30 GeV, |eta| < 2.5
    // b-tagging eff = 60%, pT > 25 GeV
    // If two electrons lie within dR < 0.05, lowest energy electron is rejected: cut performed in delphes iso
    // Jets within dr < 0.2 of baseline electron rejected
    // Electrons and muons within dR < 0.4 of jets rejected
    // If muon and electron within dR < 0.01, electron rejected, momentum added to muon : cut performed in delphes iso
      
    // More than two baseline leptons required
    if(nElectronsMedium + nMuonsCombined < 2)
        return;
    countCutflowEvent("1");
    
    //------------------------
    // Remove jets that overlap with electrons
    jets = overlapRemoval(jets, electronsMedium, 0.2);
        nJets = jets.size();
        
    //---------------------------
    // Remove any baseline muon that overlaps with a jet
        muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
        nMuonsCombined = muonsCombined.size();
        
    //---------------------------
    // Remove any signal muon that overlaps with a jet
        muonsCombinedIso = overlapRemoval(muonsCombinedIso, jets, 0.4);
        nMuonsCombinedIso = muonsCombinedIso.size();
        
    //---------------------------
    // Remove any baseline electron that overlaps with a jet or lies in transition detector region (1.37 - 1.52)
        electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
        nElectronsMedium = electronsMedium.size();
        
    //---------------------------
    // Remove any signal electron that overlaps with a jet or lies in transition detector region (1.37 - 1.52)
        electronsTight = overlapRemoval(electronsTight, jets, 0.4);
        nElectronsTight = electronsTight.size();
        
    //---------------------------
    // Cutflow requires exactly 2 baseline leptons        
    if( nMuonsCombined+nElectronsMedium != 2)
        return;
    countCutflowEvent("2");
    
  //*********************************************************
  // Apply trigger cuts to follow cutflow
  /// NEED ANSWER ABOUT EXACT TRIGGER EFFICIENCIES!!!!!!
  bool triggerFlag = false;
  double triggerRatio = (double) rand() / (double) (RAND_MAX + 1.);
  // Leading electron trigger (ignore leading muon trigger since more restrictive)
  if( (nElectronsMedium == 1) && (nMuonsCombined == 1) && (electronsMedium[0]->PT > 14.0) && (muonsCombined[0]->PT > 8.0) && (triggerRatio < 0.65))
    triggerFlag = true;
  // Asymmetric electron trigger
  if( (nElectronsMedium == 2) && (electronsMedium[0]->PT > 25.0) && (electronsMedium[1]->PT > 8.0) && (triggerRatio < 0.85))
    triggerFlag = true;
  // Symmetric electron trigger
  if( (nElectronsMedium == 2) && (electronsMedium[1]->PT > 14.0) && (triggerRatio < 0.98) )  
    triggerFlag = true;  
  // Asymmetric muon trigger
  if( (nMuonsCombined == 2) && (electronsMedium[0]->PT > 18.0) && (muonsCombined[1]->PT > 8.0) && (triggerRatio < 0.8) )
    triggerFlag = true;
  // Symmetric electron trigger
  if( (nMuonsCombined == 2) && (muonsCombined[1]->PT > 14.0) && (triggerRatio < 0.52) )
    triggerFlag = true;  
  if (!triggerFlag)
    return;
  countCutflowEvent("3");
    
    //---------------------------
    // Di-lepton invariant mass > 20 GeV
    TLorentzVector diLepton;
    if(nElectronsMedium == 2){
        diLepton = electronsMedium[0]->P4() + electronsMedium[1]->P4();
        if (diLepton.M() < 20.0)
            return;
    }
    else if(nMuonsCombined == 2){
        diLepton = muonsCombined[0]->P4() + muonsCombined[1]->P4();
        if (diLepton.M() < 20.0)
            return;
    }
    else if( (nMuonsCombined == 1) && (nElectronsMedium == 1)){
        diLepton = muonsCombined[0]->P4() + electronsMedium[0]->P4();
        if (diLepton.M() < 20.0)
            return;
    }
    countCutflowEvent("4");
    
    //---------------------------
    // CutFlow is split by lepton flavour here
    int flagLep;
    int flagSF=0;
    int flagOS = 0;
    if (nElectronsMedium == 2) {
        flagLep = 0;
        flagSF = 1;
        if((electronsMedium[0]->Charge)*(electronsMedium[1]->Charge) < 0)
          flagOS = 1;
    }
    else if (nMuonsCombined == 2) { 
        flagLep = 1;
        flagSF = 1;
        if ((muonsCombined[0]->Charge)*(muonsCombined[1]->Charge) < 0)
          flagOS = 1;
    }
    else {
        flagLep = 2;
        if((muonsCombined[0]->Charge)*(electronsMedium[0]->Charge) < 0)
          flagOS = 1;
    }
    countCutflowEvent("Lep_"+ind[0]+"_"+lep[flagLep]);
    
    //---------------------------
    // From here on signal leptons are used
    if( (nMuonsCombinedIso+nElectronsTight) != 2)
        return;
    countCutflowEvent("Lep_"+ind[1]+"_"+lep[flagLep]);
    
    //-----------------------------
    //b-veto
    int flagB = 0;
    for(int j = 0; j < nJets; j++) {
        if(checkBTag(jets[j]))
          flagB = 1;
    }
    countCutflowEvent("Lep_"+ind[2]+"_"+lep[flagLep]);
    
    
    //*********************************************************
    //-----------------------------
    // Analysis cutflow splits between different signal regions here
    //-----------------------------
    // Signal regions split between <= 2 jets and > 2 jets, jet pT > 50 GeV
    int flag3jets = 0;
    std::vector<Jet*> jets50 = filterPhaseSpace(jets, 50., -4.5, 4.5);
    if(jets50.size() > 2)
      flag3jets = 1;
    countCutflowEvent("Jet_"+lep[flagLep]+"_"+jet[flag3jets]);
    
    //-----------------------------
    // Signal regions split between reconstructed Z and Z-veto
    int flagZ = 0;
    if( (diLepton.M() > 81.0) && (diLepton.M() < 101.0) )
      flagZ = 1;
    countCutflowEvent("Z_"+lep[flagLep]+"_"+jet[flag3jets]+"_"+z[flagZ]);
    
    //-----------------------------
    // Compute Razor 'MegaJets', includes final state leptons as well
    // --- Code courtesy of Maurizio Pierini ------
    // Signal jets are required to have pT > 30 GeV
    jets = filterPhaseSpace(jets, 30., -4.5, 4.5);
    nJets = jets.size();  
    std::vector<TLorentzVector> finalStateObj;
    for(int j = 0; j < nJets; j++) {
          finalStateObj.push_back(jets[j]->P4());
    }
    for(int e = 0; e < nElectronsTight; e++) { 
          finalStateObj.push_back(electronsTight[e]->P4());
    }
    for(int e = 0; e < nMuonsCombinedIso; e++) { 
          finalStateObj.push_back(muonsCombinedIso[e]->P4());
    }   
    TLorentzVector j1, j2;
    bool foundGood = false;
    int N_comb = 1;
    for(int i = 0; i < finalStateObj.size(); i++){
        N_comb *= 2;
    }
    double M_min = 9999999999.0;
    int j_count;
    for(int i = 1; i < N_comb-1; i++){
        TLorentzVector j_temp1, j_temp2;
        int itemp = i;
        j_count = N_comb/2;
        int count = 0;
        while (j_count > 0) {
            if (itemp/j_count == 1) {
            j_temp1 += finalStateObj[count];
            } 
            else {
              j_temp2 += finalStateObj[count];
            }
           itemp -= j_count*(itemp/j_count);
          j_count /= 2;
          count++;
        }
        double M_temp = j_temp1.M2()+j_temp2.M2();
        // smallest mass
        if (M_temp < M_min) {
            M_min = M_temp;
            j1 = j_temp1;
            j2 = j_temp2;
        }
    }  
    if (j2.Pt() > j1.Pt()) {
        TLorentzVector temp = j1;
        j1 = j2;
        j2 = temp;
    }
    
    //-----------------------------
    // Calculate M_R
    // --- Code courtesy of Maurizio Pierini ------
    double A = j1.P();
    double B = j2.P();
    double az = j1.Pz();
    double bz = j2.Pz();
    TVector3 j1T, j2T;
    j1T.SetXYZ(j1.Px(),j1.Py(),0.0);
    j2T.SetXYZ(j2.Px(),j2.Py(),0.0);
    double ATBT = (j1T+j2T).Mag2();
    double MR = sqrt((A+B)*(A+B)-(az+bz)*(az+bz)-(j2T.Dot(j2T)-j1T.Dot(j1T))*(j2T.Dot(j2T)-j1T.Dot(j1T))/(j1T+j2T).Mag2());
    double mybeta = (j2T.Dot(j2T)-j1T.Dot(j1T))/sqrt(ATBT*((A+B)*(A+B)-(az+bz)*(az+bz)));
    double mygamma = 1./sqrt(1.-mybeta*mybeta);
    //gamma times MRstar  
    MR = MR*mygamma;
    
    //-----------------------------
    // Calculate M^R_T
    // --- Code courtesy of Maurizio Pierini ------
    double MRT = missingET->P4().Et()*(j1.Pt()+j2.Pt()) - missingET->P4().Vect().Dot(j1.Vect()+j2.Vect());
    MRT /= 2.;
    MRT = sqrt(MRT);
    
    //-----------------------------
    // Calculate R
    // --- Code courtesy of Maurizio Pierini ------
    double R = MRT/MR;
    
    //-----------------------------
    // R > 0.05 for control regions 
    if(R < 0.05)
        return;
    
    //-----------------------------
    // MR > 200 for control regions
    int flagMR =0;
    if(MR < 200.0)
        return;
    
    //--------------------------
    //------Control Regions------
    //--------------------------
    // Z control regions
    if ( (flagSF == 1) && (flagB == 0) && (flagZ ==0)) {
        if ( (flag3jets =0) && (R > 0.15) && (R < 0.3) && (MR>400.0) && (MR<1200.0))
          countControlEvent("CRZ1");
        if ( (flag3jets =1) && (R > 0.05) && (R < 0.2) && (MR>800.0) && (MR<1600.0))
          countControlEvent("CRZ2");
    }
    
    //--------------------------
    // Top same flavour lepton control regions
    if ( (flagSF == 1) && (flagB == 1) && (flagZ ==0)) {
        if ( (flag3jets =0) && (R > 0.2) && (R < 0.4) && (MR>400.0) && (MR<1200.0))
          countControlEvent("CRTopSF1");
        if ( (flag3jets =1) && (R > 0.1) && (R < 0.3) && (MR>800.0) && (MR<1600.0))
          countControlEvent("CRTopSF2");
    }
    
    //--------------------------
    // Top opposite flavour lepton control regions
    if ( (flagSF == 0) && (flagB == 1) ) {
        if ( (flag3jets =0) && (R > 0.2) && (R < 0.4) && (MR>400.0) && (MR<1200.0))
          countControlEvent("CRTopOF1");
        if ( (flag3jets =1) && (R > 0.1) && (R < 0.3) && (MR>800.0) && (MR<1600.0))
          countControlEvent("CRTopOF2");
    }  
    
    
    //--------------------------
    //------Validation Regions------
    //--------------------------
    // Z validation regions
    if ( (flagSF == 1) && (flagB == 0) && (flagZ ==0)) {
        if ( (flag3jets =0) && (R > 0.25) && (R < 1.0) && (MR>200.0) && (MR<400.0))
          countControlEvent("VRZ1");
        if ( (flag3jets =1) && (R > 0.2) && (R < 1.0) && (MR>200.0) && (MR<800.0))
          countControlEvent("VRZ2");
    }
    
    //--------------------------
    // Top same flavour lepton validation regions
    if ( (flagSF == 1) && (flagB == 1) && (flagZ ==0)) {
        if ( (flag3jets =0) && (R > 0.5) && (R < 1.0) && (MR>200.0) && (MR<400.0))
          countControlEvent("VRTopSF1");
        if ( (flag3jets =1) && (R > 0.35) && (R < 1.0) && (MR>200.0) && (MR<800.0))
          countControlEvent("VRTopSF2");
    }
    
    //--------------------------
    // Top opposite flavour lepton validation regions
    if ( (flagSF == 0) && (flagB == 1) ) {
        if ( (flag3jets =0) && (R > 0.5) && (R < 1.0) && (MR>200.0) && (MR<400.0))
          countControlEvent("VRTopOF1");
        if ( (flag3jets =1) && (R > 0.35) && (R < 1.0) && (MR>200.0) && (MR<800.0))
          countControlEvent("VRTopOF2");
    }
}

void Atlas_conf_2013_089_CR::finalize() {
}       
