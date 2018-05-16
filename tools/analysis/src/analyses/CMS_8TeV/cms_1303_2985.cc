#include "cms_1303_2985.h"

std::string Cms_1303_2985::nj[2] = { "23j", "4j"};
std::string Cms_1303_2985::nbj[5] = { "0b", "1b", "2b", "3b", "4b"};
std::string Cms_1303_2985::binHT[8] = { "275", "325", "375", "475", "575", "675", "775", "875"};

void Cms_1303_2985::initialize() {
  setInformation(""
  "@#CMS\n"
  "@#1303:2985\n"
  "@#Hadronic alpha_T + b-jets\n"
  "@#sqrt(s) = 8 TeV\n"
  "@#int(L) = 11.7 fb^-1\n"
  "");
  setLuminosity(11.7*units::INVFB);      
  setAnalysisName("cms_1303_2985");          
  ignore("towers");
  
  bookCutflowRegions("00;01;02;03;04;05;06;07;08;09;10;11;12");
  bookSignalRegions("23j_0b_275;23j_0b_325;23j_0b_375;23j_0b_475;23j_0b_575;23j_0b_675;23j_0b_775;23j_0b_875;23j_1b_275;23j_1b_325;23j_1b_375;23j_1b_475;23j_1b_575;23j_1b_675;23j_1b_775;23j_1b_875;23j_2b_275;23j_2b_325;23j_2b_375;23j_2b_475;23j_2b_575;23j_2b_675;23j_2b_775;23j_2b_875;4j_0b_275;4j_0b_325;4j_0b_375;4j_0b_475;4j_0b_575;4j_0b_675;4j_0b_775;4j_0b_875;4j_1b_275;4j_1b_325;4j_1b_375;4j_1b_475;4j_1b_575;4j_1b_675;4j_1b_775;4j_1b_875;4j_2b_275;4j_2b_325;4j_2b_375;4j_2b_475;4j_2b_575;4j_2b_675;4j_2b_775;4j_2b_875;4j_3b_275;4j_3b_325;4j_3b_375;4j_3b_475;4j_3b_575;4j_3b_675;4j_3b_775;4j_3b_875;4j_4b_275;4j_4b_325;4j_4b_375;");
}

void Cms_1303_2985::analyze() {
  missingET->addMuons(muonsCombined);
  std::vector<Jet*> jetsEta;
  int nJetsEta;
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.5, 2.5); 
  electronsMedium = filterIsolation(electronsMedium);
  int nElectronsMedium = electronsMedium.size(); 
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  muonsCombined = filterIsolation(muonsCombined);
  int nMuonsCombined = muonsCombined.size();    
  jetsEta = filterPhaseSpace(jets, 50., -5., 5.);
  nJetsEta = jetsEta.size();  
  jets = filterPhaseSpace(jets, 20., -3., 3.);
  int nJets = jets.size();    
  photonsMedium = filterPhaseSpace(photonsMedium, 25., -2.5, 2.5);
  photonsMedium = filterIsolation(photonsMedium);
  int nPhotons = photonsMedium.size();    
  countCutflowEvent("00");
  
  //------------------------
  // Remove jets that overlap with electrons or photons
  jets = overlapRemoval(jets, electronsMedium, 0.3);
  jets = overlapRemoval(jets, photonsMedium, 0.3);
  nJets = jets.size();
  
  //------------------------
  // alphaT requires at least 2 jets in the event
  if (nJets < 2)
    return;
  countCutflowEvent("01");
  
  //------------------------
  // Trigger already requires both HT and alpha_T to be calculated
  // Start with HT, HT is not the same definition as delphes, instead scalar sum of jet ET.
  // Implement lower HT trigger first so we don't have to calculate alpha_T 
  HT275 = HT325 = HT = 0.;
  mHT275 = mHT325 = mHTNorm = 0.;
  std::vector<Jet*> jetsNorm, jets275, jets325;
  TLorentzVector vecHT275, vecHT325, vecHTNorm;
  for (int i = 0; i < nJets; i++) {
    double ET = jets[i]->P4().Et();
    if (ET > 37.0) {
      jets275.push_back(jets[i]);
      HT275 += ET;
      vecHT275 += jets[i]->P4();
      if (ET > 43.0) {
        jets325.push_back(jets[i]);
        HT325 += ET;
        vecHT325 += jets[i]->P4();
        if (ET > 50.0) {
          jetsNorm.push_back(jets[i]);
          HT += ET;
          vecHTNorm += jets[i]->P4();
        }
      }
    }
  }    
  mHT275 = vecHT275.Pt();
  mHT325 = vecHT325.Pt();
  mHTNorm = vecHTNorm.Pt();
  if ((HT275 < 250.0) || (jets275.size() < 2)) //use 250GeV because this is the trigger condition
    return;
  countCutflowEvent("02");
  
  //------------------------
  // Now calculate alpha_T separately for each region, use code supplied by cms, tested against self-written code.
  // HT275 region
  cut325 = cutNorm = false;
  std::vector<double> diff275( 1<<(jets275.size()-1) , 0. );
  for (unsigned i=0; i < diff275.size(); i++) {
    for (unsigned j=0; j < jets275.size(); j++) {
      diff275[i] += jets275[j]->P4().Et() * ( 1 - 2 * (int(i>>j)&1) ) ;
    }
  }
  double DHT275 = fabs( *min_element( diff275.begin(), diff275.end(), fabs_less() ) );
  alphaT275 = 0.5 * ( HT275 - DHT275 ) / sqrt( HT275*HT275 - mHT275*mHT275 );
  
 
  //------------------------
  // HT325 region
  if ( (HT325 > 300.0) && (jets325.size() > 1)) {  //use 300GeV because this is the trigger condition
    cut325 = true;
    std::vector<double> diff325( 1<<(jets325.size()-1) , 0. );
    for (unsigned i=0; i < diff325.size(); i++) {
      for (unsigned j=0; j < jets325.size(); j++) {
        diff325[i] += jets325[j]->P4().Et() * ( 1 - 2 * (int(i>>j)&1) ) ;
      }
    }
    double DHT325 = fabs( *min_element( diff325.begin(), diff325.end(), fabs_less() ) );
    alphaT325 = 0.5 * ( HT325 - DHT325 ) / sqrt( HT325*HT325 - mHT325*mHT325 );
  }
  
  //-----------------------
  // 'Normal' HT region
  if ( (HT > 350.0) && (jetsNorm.size() > 1)) {  //use 300GeV because this is the trigger condition
    cutNorm = true;
    std::vector<double> diff( 1<<(jetsNorm.size()-1) , 0. );
    for (unsigned i=0; i < diff.size(); i++) {
      for (unsigned j=0; j < jetsNorm.size(); j++) {
        diff[i] += jetsNorm[j]->P4().Et() * ( 1 - 2 * (int(i>>j)&1) ) ;
      }
    }
    double DHT = fabs( *min_element( diff.begin(), diff.end(), fabs_less() ) );
    alphaT = 0.5 * ( HT - DHT ) / sqrt( HT*HT - mHTNorm*mHTNorm );
  }
  
  //------------------------
  // Implement full trigger conditions
  triggerPass275 = triggerPass325 = triggerPassNorm = false;
  if ( (HT275>250.) && (alphaT275 >0.55) ) 
    triggerPass275 = true;
  else if ( (cut325 ==true) && (HT325>300.) && (alphaT325 >0.53) ) 
    triggerPass325 = true;
  else if ( (cutNorm ==true) && (HT>350.) && (alphaT >0.52) ) 
    triggerPassNorm = true;
  else if ( (cutNorm ==true) && (HT>400.) && (alphaT >0.51) ) 
    triggerPassNorm = true;
  if ( (!triggerPass275) && (!triggerPass325) && (!triggerPassNorm) )
    return;
  countCutflowEvent("03");
  
  //------------------------
  // Trigger efficiencies for each signal region
  // Events thrown away for simplest cut-flow comparison
  double randEff = rand()/static_cast<double>(RAND_MAX+1.);      
  if ( (triggerPass275) && (HT275<325)) {
    if ( (jets275.size()<4) && (randEff > 0.891))
      return;
    else if (randEff > 0.837)
      return;
  }
  else if ( (triggerPass325) && (HT325<375)) {
    if ( (jets325.size()<4) && (randEff > 0.987))
      return;
    else if (randEff > 0.982)
      return;
  }
  else if ( (triggerPassNorm) && (HT<475)) {
    if ( (jetsNorm.size()<4) && (randEff > 0.99))
      return;
    else if (randEff > 0.997)
      return;
  }
  countCutflowEvent("04");  
  
  //------------------------
  // Photon veto    
  if ( nPhotons > 0)
    return;
  countCutflowEvent("05");   
  
  //------------------------
  // Electron veto
  if ( nElectronsMedium > 0)
    return;
  countCutflowEvent("06");
  
  //------------------------
  // Muon veto
  if ( nMuonsCombined > 0)
    return;    
  countCutflowEvent("07");
  
  //------------------------
  //Require two highest ET jets > 73 for 275 bin, > 87 for 325 bin and > 100GeV for 'normal' bins
  if (triggerPass275) {
    if ( jets275[1]->P4().Et()<73.) 
      triggerPass275 = false;
  }
  if (triggerPass325) {
    if ( jets325[1]->P4().Et()<87.) 
      triggerPass325 = false;
  }
  if (triggerPassNorm) {
    if ( jetsNorm[1]->P4().Et()<100.) 
      triggerPassNorm = false;
  }
  if ( (!triggerPass275) && (!triggerPass325) && (!triggerPassNorm) )
    return;
  countCutflowEvent("08");
  
  //------------------------
  //Highest ET jet, eta < 2.5          
  if ( fabs(jets275[0]->Eta) > 2.5) 
    return;
  countCutflowEvent("09");
  
  //------------------------
  //No event with a ET>50, eta > 3 jet allowed         
  for (unsigned i=0; i < nJetsEta; i++) {
    if ( fabs(jetsEta[i]->Eta) > 3.0) 
      return;
  }
  countCutflowEvent("10");
  
  //------------------------
  //Count number of b-jets
  //B-jets only reconstructed in rapidity range <2.4
  bJet275 = bJet325 = bJetNorm = 0;
  for (unsigned i=0; i < jets275.size(); i++) {
    if ((fabs(jets275[i]->Eta) < 2.4) && (checkBTag(jets275[i]))) 
      bJet275 += 1;
  }
  for (unsigned i=0; i < jets325.size(); i++) {
    if ((fabs(jets325[i]->Eta) < 2.4) && (checkBTag(jets325[i]))) 
      bJet325 += 1;
  }
  for (unsigned i=0; i < jetsNorm.size(); i++) {
    if ((fabs(jetsNorm[i]->Eta) < 2.4) && (checkBTag(jetsNorm[i]))) 
      bJetNorm += 1;
  }
  
  //------------------------
  //Jet mis-measurement veto
  //Events vetoed if they could fake large alphaT and lie close to affected ECAL regions 
  //Throw away events at random to simulate effect.
  ///EFFECTIVE SIZE OF REGION IS NOT KNOWN, WAITING FOR CMS ANSWER, USING 10% NOW
  if (triggerPass275) {
    for (unsigned i=0; i < jets275.size(); i++) {
      double jetEta = jets275[i]->Eta;
      TLorentzVector testMissHT;
      for (unsigned j=0; j < jets275.size(); j++) {
        if (j != i) 
          testMissHT += -jets275[j]->P4(); 
      }
      if ( fabs(jets275[i]->P4().DeltaPhi(testMissHT)) < 0.5) {
        if (rand()/static_cast<double>(RAND_MAX + 1.) > 0.90)
          triggerPass275 = false;
        else if ( fabs(fabs(jetEta) - 1.48) < 0.3)  //if affected jet lie close barrel/endcap transition
          triggerPass275 = false;        
      }
    }
  }    
  if (triggerPass325) {
    for (unsigned i=0; i < jets325.size(); i++) {
      double jetEta = jets325[i]->Eta;
      TLorentzVector testMissHT;
      for (unsigned j=0; j < jets325.size(); j++) {
        if (j != i) 
          testMissHT += -jets325[j]->P4(); 
      }
      if ( fabs(jets325[i]->P4().DeltaPhi(testMissHT)) < 0.5) {
        if (rand()/static_cast<double>(RAND_MAX + 1.) > 0.90)
          triggerPass325 = false;
        else if ( fabs(fabs(jetEta) - 1.48) < 0.3)  //if affected jet lie close barrel/endcap transition
          triggerPass325 = false;
      }
    }
  } 
  if (triggerPassNorm) {
    for (unsigned i=0; i < jetsNorm.size(); i++) {
      double jetEta = jetsNorm[i]->Eta;
      TLorentzVector testMissHT;
      for (unsigned j=0; j < jetsNorm.size(); j++) {
        if (j != i) 
          testMissHT += -jetsNorm[j]->P4(); 
      }
      if ( fabs(jetsNorm[i]->P4().DeltaPhi(testMissHT)) < 0.5) {
        if (rand()/static_cast<double>(RAND_MAX) > 0.90)
          triggerPassNorm = false;
        else if ( fabs(fabs(jetEta) - 1.48) < 0.3)  //if affected jet lie close barrel/endcap transition
          triggerPassNorm = false;
      }
    }
  }  
  if ( (!triggerPass275) && (!triggerPass325) && (!triggerPassNorm) )
    return;
  countCutflowEvent("11");
  
  //------------------------
  //Missing energy mis-measurement veto
  //Events vetoed if large difference between mHT and normal missingET
  if ((triggerPass275) && ( (mHT275/missingET->P4().Et())>1.25) )
    triggerPass275 = false;
  
  if ((triggerPass325) && ( (mHT325/missingET->P4().Et())>1.25) )
    triggerPass325 = false;
  
  if ((triggerPassNorm) && ( (mHTNorm/missingET->P4().Et())>1.25) )
    triggerPassNorm = false;
  
  if ( (!triggerPass275) && (!triggerPass325) && (!triggerPassNorm) )
    return;
  countCutflowEvent("12");  
  
  
  //--------------------------
  //------Signal Regions------
  //--------------------------
  //---------------------------
  //signal regions start with HT>275
  if (HT275 < 275.0)
    return;
  
  //signal regions require alphaT > 0.55
  if ( (alphaT275 < 0.55) && (alphaT325 < 0.55) && (alphaT < 0.55) )
    return;
  
  //jet and bjet index definitions
  int iJets, bJets;
  
  //--------------------------
  // Signal region HT275-325
  if ((alphaT275 > 0.55) && (HT275 < 325.0)) {
    if (jets275.size()<=3) {
      iJets = 0;
      if (bJet275 == 0) 
        bJets = 0;
      else if (bJet275 == 1) 
        bJets = 1;
      else if (bJet275 == 2) 
        bJets = 2;
    }
    else if (jets275.size()>=4) { 
      iJets = 1;
      if (bJet275 == 0) 
        bJets = 0;
      else if (bJet275 == 1) 
        bJets = 1;
      else if (bJet275 == 2) 
        bJets = 2;
      else if (bJet275 == 3) 
        bJets = 3;
      else if (bJet275 >= 4) 
        bJets = 4;
    }
    countSignalEvent(nj[iJets]+"_"+nbj[bJets]+"_"+binHT[0]);
    return;
  }
  //--------------------------
  // Signal region HT325-375
  if ((alphaT325 > 0.55) && (HT325 < 375.0)) {
    if (jets325.size()<=3) {
      iJets =0;
      if (bJet325 == 0) 
        bJets =0;
      else if (bJet325 == 1) 
        bJets =1;
      else if (bJet325 == 2) 
        bJets =2;
    }
    else if (jets325.size()>=4) { 
      iJets =1;
      if (bJet325 == 0) 
        bJets =0;
      else if (bJet325 == 1) 
        bJets =1;
      else if (bJet325 == 2) 
        bJets =2;
      else if (bJet325 == 3) 
        bJets =3;
      else if (bJet325 >= 4) 
        bJets =4;
    }
    countSignalEvent(nj[iJets]+"_"+nbj[bJets]+"_"+binHT[1]);
    return;
  }
  
  //--------------------------
  // Signal region HT375 + 4+ bjets (special inclusive HT region for more than 4 bjets)
  if ((alphaT > 0.55) && (HT > 375.0) && (jetsNorm.size()>=4) && (bJet325 >= 4)) { 
    countSignalEvent(nj[1]+"_"+nbj[4]+"_"+binHT[2]);
    return;      
  }
  
  //--------------------------
  // Higher signal regions
  // First find HT region
  int sig = 0;
  if ((alphaT > 0.55) && (HT > 375.0)) {
    if (HT < 475.0) 
      sig =2; 
    else if (HT < 575.0) 
      sig =3; 
    else if (HT < 675.0) 
      sig =4; 
    else if (HT < 775.0) 
      sig =5; 
    else if (HT < 875.0) 
      sig =6;
    else if (HT > 875.0) 
      sig =7;
    // Find number of jets and b-jets
    if (jetsNorm.size()<=3) {
      iJets = 0;
      if (bJetNorm == 0) 
        bJets = 0;
      else if (bJetNorm == 1) 
        bJets = 1;
      else if (bJetNorm == 2) 
        bJets = 2;
    }
    else if (jetsNorm.size()>=4) { 
      iJets = 1;
      if (bJetNorm == 0) 
        bJets = 0;
      else if (bJetNorm == 1) 
        bJets = 1;
      else if (bJetNorm == 2) 
        bJets = 2;
      else if (bJetNorm == 3) 
        bJets = 3;
    }    
    countSignalEvent(nj[iJets]+"_"+nbj[bJets]+"_"+binHT[sig]);
  }
  
}

void Cms_1303_2985::finalize() {
}       
