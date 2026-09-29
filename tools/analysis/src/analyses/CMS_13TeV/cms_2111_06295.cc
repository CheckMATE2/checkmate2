#include "cms_2111_06295.h"
// AUTHOR: kr
//  EMAIL: krolb@fuw.edu.pl
void Cms_2111_06295::initialize() {
  setAnalysisName("cms_2111_06295");          
  setInformation(""
    "\n"
  "");
  setLuminosity(137.0*units::INVFB);      
  bookSignalRegions("2l_low_04;2l_low_10;2l_low_20;2l_low_30;2l_med_01;2l_med_04;2l_med_10;2l_med_20;2l_med_30;2l_high_01;2l_high_04;2l_high_10;2l_high_20;2l_high_30;2l_ultra_01;2l_ultra_04;2l_ultra_10;2l_ultra_20;2l_ultra_30;3l_low_04;3l_low_10;3l_low_20;3l_low_30;3l_med_01;3l_med_04;3l_med_10;3l_med_20;3l_med_30;3l_WZ_low_04;3l_WZ_low_10;3l_WZ_low_20;3l_WZ_med_01;3l_WZ_med_04;3l_WZ_med_10;3l_WZ_med_20;stop_low_03;stop_low_08;stop_low_12;stop_low_16;stop_low_20;stop_low_25;stop_med_03;stop_med_08;stop_med_12;stop_med_16;stop_med_20;stop_med_25;stop_high_03;stop_high_08;stop_high_12;stop_high_16;stop_high_20;stop_high_25;stop_ultra_03;stop_ultra_08;stop_ultra_12;stop_ultra_16;stop_ultra_20;stop_ultra_25");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_2111_06295::analyze() {
  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out. Probably not since 3.4.2
  
  std::string year = "1900";
  double y = rand()/(RAND_MAX+1.);
  if (y < 0.262) year = "2016";
  else if (y > 0.565) year = "2018";
  else year = "2017";
  
  countCutflowEvent("00_all");
  
  electronsLoose = filterPhaseSpace(electronsLoose, 3., -2.5, 2.5);
  electronsTight = filterPhaseSpace(electronsTight, 3., -2.5, 2.5);
  muonsCombined = filterPhaseSpace(muonsCombined, 3., -2.4, 2.4);
  electronsLoose = filterIsolation(electronsLoose);
  electronsTight = filterIsolation(electronsTight);
  muonsCombined = filterIsolation(muonsCombined);
  jets = filterPhaseSpace(jets, 20., -2.4, 2.4);
  
  jets = overlapRemoval(jets, electronsLoose, 0.4);
  jets = overlapRemoval(jets, muonsCombined, 0.4);
  electronsLoose = overlapRemoval(electronsLoose, jets, 0.4);
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);

  if (electronsLoose.size() + muonsCombined.size() > 3) return;
  countCutflowEvent("00_leptons<4");
  
  electronsLoose = filterPhaseSpace(electronsLoose, 30., -2.5, 2.5, false, true); //this excludes leptons with pt>30
  std::vector<Electron*> electronsSignal = filterPhaseSpace(electronsTight, 30., -2.5, 2.5, false, true); //this excludes leptons with pt>30
  std::vector<Muon*> muonsSignal = filterPhaseSpace(muonsCombined, 30., -2.4, 2.4, false, true);
  std::vector<Jet*> jetsSignal = filterPhaseSpace(jets, 25., -2.4, 2.4);
  if (electronsSignal.size() + muonsSignal.size() < 2) return;
  if (electronsSignal.size() + muonsSignal.size() < electronsTight.size() + muonsCombined.size()) return; //veto events with additional leptons with pt>30
  countCutflowEvent("01_2-3leptons");

  bool SS = false; //this selects events for SS CR
  if (electronsSignal.size() == 2 and muonsSignal.size() == 0 and electronsSignal[0]->Charge * electronsSignal[1]->Charge > 0) SS = true;
  if (muonsSignal.size() == 2 and electronsSignal.size() == 0 and muonsSignal[0]->Charge * muonsSignal[1]->Charge > 0) SS = true;
  if (muonsSignal.size() == 1 and electronsSignal.size() == 1 and muonsSignal[0]->Charge * electronsSignal[0]->Charge > 0) SS = true;
  
  std::vector<FinalStateObject*> leptonsLoose;
  for ( int i = 0; i <  electronsLoose.size(); i++ ) { //we later check that Loose survives to Tight
    FinalStateObject* lep = newFinalStateObject(electronsLoose[i]);
    leptonsLoose.push_back(lep);
    //cout << "e " ;
  }
  for ( int i = 0; i < muonsSignal.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(muonsSignal[i]);
    leptonsLoose.push_back(lep);
    //cout << "mu " ;
  }
  std::sort(leptonsLoose.begin(), leptonsLoose.end(), FinalStateObject::sortByPT);

  std::vector<FinalStateObject*> leptonsTight;
  for ( int i = 0; i <  electronsTight.size(); i++ ) { //we later check that Tight survives
    double eff = rand()/double(RAND_MAX);
    //if (electronsTight[i]->PT < 10. and eff > 0.75) continue;
    FinalStateObject* lep = newFinalStateObject(electronsTight[i]);
    leptonsTight.push_back(lep);
    //cout << "e " ;
  }
  for ( int i = 0; i < muonsSignal.size(); i++ ) {
    double eff = rand()/double(RAND_MAX);
    if (muonsSignal[i]->PT < 10. and eff > 0.75) continue;
    else if (muonsSignal[i]->PT < 20. and eff > 0.80) continue; 
    else if (eff > 0.85) continue; //efficiency correction for muon trigger
    FinalStateObject* lep = newFinalStateObject(muonsSignal[i]);
    leptonsTight.push_back(lep);
    //cout << "mu " ;
  }
  std::sort(leptonsTight.begin(), leptonsTight.end(), FinalStateObject::sortByPT);

  if (SS) {
    // run SS selection and quit
    return;
  }

  std::vector<Jet*> bjets;
  for(int i=0; i<jets.size(); i++) {
    if ( checkBTag(jets[i]) ) {
      bjets.push_back(jets[i]);
    }
  }
  
 /* double mllOSmin = 999999.;
  double mllmin = 999999.;
  double mllmax = 0.;
  for ( int i = 0; i < leptons.size(); i++ ) {
    for ( int j = i+1; j < leptons.size(); j++ ) {
      if (leptons[i]->Charge * leptons[j]->Charge < 0 and leptons[i]->Type == leptons[j]->Type ) {
        double mll = (leptons[i]->P4() + leptons[j]->P4()).M();
        if (mll < mllmin) mllmin = mll;
      }
      if (leptons[i]->Charge * leptons[j]->Charge < 0  ) {
        double mll = (leptons[i]->P4() + leptons[j]->P4()).M();
        if (mll > mllmax) mllmax = mll;

      }
    }
  }*/

  bool SR = false; bool CR = false;
  //eventually OS-check is actually later in the cutflow... need to adjust here.. or maybe not
  // the CMS cutflow is totally stupid
  if (leptonsLoose.size() == 2) {
    countCutflowEvent("02_tt_dilep"); //found OS pair
    // run stop selections
    countCutflowEvent("02_2l_dilep");
    SR = SR_2l_low(leptonsLoose, leptonsTight, jetsSignal);
    SR = SR_2l_med(leptonsLoose, leptonsTight, jetsSignal);
    SR = SR_2l_high(leptonsLoose, leptonsTight, jetsSignal);
    SR = SR_2l_ultra(leptonsLoose, leptonsTight, jetsSignal);

    CR = CR_2l_DY_low(leptonsLoose, leptonsTight, jetsSignal);
  }
  else if  (leptonsTight.size() == 3 ) {
    countCutflowEvent("02_3l_dilep");
    SR = SR_3l_low(leptonsTight, jetsSignal);
    SR = SR_3l_med(leptonsTight, jetsSignal);
    // run 3l selections
  }
  else return; // 3 SS leptons or something weird

 
}

void Cms_2111_06295::finalize() {
  // Whatever should be done after the run goes here
}       

bool Cms_2111_06295::SR_3l_low(std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal) {

  double mllOSSFmin = 999999.;
  double mllSFASmax = 0.;
  bool ismuon = false;
  for ( int i = 0; i < leptonsTight.size(); i++ ) {
    for ( int j = i+1; j < leptonsTight.size(); j++ ) {
      if (leptonsTight[i]->Charge * leptonsTight[j]->Charge < 0 and leptonsTight[i]->Type == leptonsTight[j]->Type ) {
        double mll = (leptonsTight[i]->P4() + leptonsTight[j]->P4()).M();
        if (mll < mllOSSFmin) mllOSSFmin = mll;
        if (leptonsTight[i]->Type == "muon") ismuon = true; else ismuon = false;
      }
      if (leptonsTight[i]->Type == leptonsTight[j]->Type ) {
        double mll = (leptonsTight[i]->P4() + leptonsTight[j]->P4()).M();
        if (mll > mllSFASmax) mllSFASmax = mll;
      }
    }
  }


  double ht=0.;
  for(int i=0; i<jetsSignal.size(); i++) ht += jetsSignal[i]->PT;
  for(int i=0; i<jetsSignal.size(); i++) if ( checkBTag(jetsSignal[i]) ) return false;

  double met = missingET->PT;
  double mll = mllOSSFmin;
  if ( mllOSSFmin < 4. or mllOSSFmin > 50. or mllSFASmax > 60. or (mll > 9. and mll < 10.5) or (mll > 3. and mll < 3.2) or leptonsTight[0]->PT > 30. or leptonsTight[2]->PT < 5. or ht < 100. or !ismuon or met > 200. or met < 125. ) return false;

  countCutflowEvent("3llow_SR");
  if (mll > 4. and mll <  10.) countSignalEvent("3l_low_04");
  if (mll > 10. and mll <  20.) countSignalEvent("3l_low_10");
  if (mll > 20. and mll <  30.) countSignalEvent("3l_low_20");
  if (mll > 30. and mll <  50.) countSignalEvent("3l_low_30");
  return true;

}

bool Cms_2111_06295::SR_3l_med(std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal) {

  double mllOSSFmin = 999999.;
  double mllSFASmax = 0.;
  for ( int i = 0; i < leptonsTight.size(); i++ ) {
    for ( int j = i+1; j < leptonsTight.size(); j++ ) {
      if (leptonsTight[i]->P4().DeltaR(leptonsTight[j]->P4()) < 0.3) return false;
      if (leptonsTight[i]->Charge * leptonsTight[j]->Charge < 0 and leptonsTight[i]->Type == leptonsTight[j]->Type ) {
        double mll = (leptonsTight[i]->P4() + leptonsTight[j]->P4()).M();
        if (mll < mllOSSFmin) mllOSSFmin = mll;
      }
      if (leptonsTight[i]->Type == leptonsTight[j]->Type ) {
        double mll = (leptonsTight[i]->P4() + leptonsTight[j]->P4()).M();
        if (mll > mllSFASmax) mllSFASmax = mll;
      }
    }
  }


  double ht=0.;
  for(int i=0; i<jetsSignal.size(); i++) ht += jetsSignal[i]->PT;
  for(int i=0; i<jetsSignal.size(); i++) if ( checkBTag(jetsSignal[i]) ) return false;

  for ( int i = 0; i < leptonsTight.size(); i++ ) {
    if (leptonsTight[i]->PT < 3.5 and leptonsTight[i]->Type == "muon" ) return false;
    if (leptonsTight[i]->PT < 5. and leptonsTight[i]->Type == "electron" ) return false;
  }

  double met = missingET->PT;
  double mll = mllOSSFmin;
  if ( mllOSSFmin < 1. or mllOSSFmin > 50. or (mll > 9. and mll < 10.5) or (mll > 3. and mll < 3.2) or leptonsTight[0]->PT > 30. or ht < 100. or met < 200. ) return false;

  countCutflowEvent("3lmed_SR");

  if (mll > 1. and mll <  40.) countSignalEvent("3l_med_01");
  if (mll > 4. and mll <  10.) countSignalEvent("3l_med_04");
  if (mll > 10. and mll <  20.) countSignalEvent("3l_med_10");
  if (mll > 20. and mll <  30.) countSignalEvent("3l_med_20");
  if (mll > 30. and mll <  50.) countSignalEvent("3l_med_30");
  return true;

}


bool Cms_2111_06295::SR_2l_low(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal) {

    countCutflowEvent("2llow_02_dilep");
    if ( (leptons[1]->Type == "muon" and leptons[1]->PT < 3.5) or (leptons[1]->Type == "electron" and leptons[1]->PT < 5.) ) return false;
    countCutflowEvent("2llow_03_subleppt"); 
    
    double mll = (leptons[0]->P4() + leptons[1]->P4()).M();
    if ( mll < 4. or mll > 50. ) return false;
    countCutflowEvent("2llow_04_mll");
    
    if ( (mll > 9. and mll < 10.5) ) return false; //veto J/psi and Upsilon
    countCutflowEvent("2llow_05_Ups_veto");
    
    double pll = (leptons[0]->P4() + leptons[1]->P4()).Pt();
    if (  leptons[0]->Type == "muon" and leptons[1]->Type == "muon" and pll < 3. ) return false;
    countCutflowEvent("2llow_06_dilepPt"); //unclear if it's just for muons or any leptons
    
    if ( jetsSignal.size() < 1 ) return false;  
    countCutflowEvent("2llow_07_ISRjet");
    
    double ht=0.;
    double met = missingET->PT;
    for(int i=0; i<jetsSignal.size(); i++) ht += jetsSignal[i]->PT;
    if ( met/ht < 0.66666 or met/ht > 1.6 ) return false;
    countCutflowEvent("2llow_08_METoverHT");
    
    if ( ht < 100. ) return false;
    
    countCutflowEvent("2llow_09_minHT");
    
    if ( met > 200. or met < 125. ) return false;
    countCutflowEvent("2llow_10_MET");
    //double ptrig = 0.4 + 0.5/75.*(met-125.); // some approx of fig.5 in 1903.06078; trigger efficiency for met
    //eventually it seems only muon trigger is used
    //if (rand()/double(RAND_MAX) > ptrig) return false;
    
    if (leptons[0]->Type != "muon" or leptons[1]->Type != "muon") return false; 
    countCutflowEvent("2llow_11_METtrigger");
    
    if ( leptons[0]->Charge * leptons[1]->Charge > 0 ) return false;
    countCutflowEvent("2llow_12_OS");
    
    if (leptons[0]->PT < 5. or leptons[0]->PT > 30.) return false;
    countCutflowEvent("2llow_13_leadlepPT");
    
    if (leptons.size() != leptonsTight.size() ) return false; //veto events with additional leptons with pt>30
    countCutflowEvent("2llow_14_twoTight");
    
    for(int i=0; i<jetsSignal.size(); i++) if ( checkBTag(jetsSignal[i]) ) return false;
    countCutflowEvent("2llow_15_bveto");
    
    double mtata = mtautau(leptons);
    if (mtata > 0. and mtata <  160.) return false;
    countCutflowEvent("2llow_16_mtautau");
    
    double mtl1 = mT(leptons[0]->P4(), missingET->P4());
    double mtl2 = mT(leptons[1]->P4(), missingET->P4());
    if (mtl1 > 70. or mtl2 > 70.) return false;
    countCutflowEvent("2llow_17_mT");
    
    countCutflowEvent("2llow_18_SF");
    countCutflowEvent("2llow_19_mm");
    
    if (leptons[1]->PT < 5.) return false;
    countCutflowEvent("2llow_20_pt5sublep");

    if (mll > 4. and mll <  10.) countSignalEvent("2l_low_04");
    if (mll > 10. and mll <  20.) countSignalEvent("2l_low_10");
    if (mll > 20. and mll <  30.) countSignalEvent("2l_low_20");
    if (mll > 30. and mll <  50.) countSignalEvent("2l_low_30");
    return true;

}

bool Cms_2111_06295::SR_2l_med(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal) {

    countCutflowEvent("2lmed_02_dilep");
    
    if ( (leptons[1]->Type == "muon" and leptons[1]->PT < 3.5) or (leptons[1]->Type == "electron" and leptons[1]->PT < 5.) ) return false;
    countCutflowEvent("2lmed_03_subleppt"); 
    
    double mll = (leptons[0]->P4() + leptons[1]->P4()).M();
    if ( (mll > 9. and mll < 10.5) ) return false; //veto J/psi and Upsilon
    countCutflowEvent("2lmed_04_Ups_veto");

    double pll = (leptons[0]->P4() + leptons[1]->P4()).Pt();
    if ( leptons[0]->Type == "muon" and leptons[1]->Type == "muon" and pll < 3. ) return false;
    countCutflowEvent("2lmed_05_dilepPt"); //unclear if it's just for muons or any leptons
    
    if ( jetsSignal.size() < 1 ) return false;  
    countCutflowEvent("2lmed_06_ISRjet");

    double ht=0.;
    double met = missingET->PT;
    for(int i=0; i<jetsSignal.size(); i++) ht += jetsSignal[i]->PT;
    if ( met/ht < 0.66666 or met/ht > 1.6 ) return false;
    countCutflowEvent("2lmed_07_METoverHT");

    if ( ht < 100. ) return false;
    countCutflowEvent("2lmed_08_minHT");

    if ( met < 200. or met > 240. ) return false;
    countCutflowEvent("2lmed_09_MET");
    
    if (rand()/double(RAND_MAX) > 0.95) return false; //efficency correction
    countCutflowEvent("2lmed_10_METtrigger");
    
    if ( leptons[0]->Charge * leptons[1]->Charge > 0 ) return false;
    countCutflowEvent("2lmed_11_OS");
    
    if (leptons.size() != leptonsTight.size() ) return false; //veto events with additional leptons with pt>30
    countCutflowEvent("2lmed_12_twoTight");

    for(int i=0; i<jetsSignal.size(); i++) if ( checkBTag(jetsSignal[i]) ) return false;
    countCutflowEvent("2lmed_13_bveto");

    double mtata = mtautau(leptons);
    if (mtata > 0. and mtata <  160.) return false;
    countCutflowEvent("2lmed_14_mtautau");

    double mtl1 = mT(leptons[0]->P4(), missingET->P4());
    double mtl2 = mT(leptons[1]->P4(), missingET->P4());
    if (mtl1 > 70. or mtl2 > 70.) return false;
    countCutflowEvent("2lmed_15_mT");

    if (leptons[0]->Type != leptons[1]->Type) return false;
    countCutflowEvent("2lmed_16_SF");

    if ( mll < 1. or mll > 50. ) return false;
    countCutflowEvent("2lmed_17_mll");
    
    if ( (mll > 3. and mll < 3.2) ) return false; //veto J/psi
    countCutflowEvent("2lmed_18_Jpsi_veto");
    
    if ((leptons[0]->Type == "electron" and leptons[0]->PT < 5.) or (leptons[0]->Type == "muon" and leptons[0]->PT < 3.5) or leptons[0]->PT > 30.) return false;
    countCutflowEvent("2lmed_19_leadlepPT");
    
    if( leptons[0]->P4().DeltaR(leptons[1]->P4()) < 0.3 ) return false;
    countCutflowEvent("2lmed_20_mindR");

    if (mll > 1. and mll <  4.) countSignalEvent("2l_med_01");
    if (mll > 4. and mll <  10.) countSignalEvent("2l_med_04");
    if (mll > 10. and mll <  20.) countSignalEvent("2l_med_10");
    if (mll > 20. and mll <  30.) countSignalEvent("2l_med_20");
    if (mll > 30. and mll <  50.) countSignalEvent("2l_med_30");
    return true;

}

bool Cms_2111_06295::SR_2l_high(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal) {

    countCutflowEvent("2lhigh_02_dilep");
    
    if ( (leptons[1]->Type == "muon" and leptons[1]->PT < 3.5) or (leptons[1]->Type == "electron" and leptons[1]->PT < 5.) ) return false;
    countCutflowEvent("2lhigh_03_subleppt"); 
    
    double mll = (leptons[0]->P4() + leptons[1]->P4()).M();
    if ( (mll > 9. and mll < 10.5) ) return false; //veto J/psi and Upsilon
    countCutflowEvent("2lhigh_04_Ups_veto");

    double pll = (leptons[0]->P4() + leptons[1]->P4()).Pt();
    if ( leptons[0]->Type == "muon" and leptons[1]->Type == "muon" and pll < 3. ) return false;
    countCutflowEvent("2lhigh_05_dilepPt"); //unclear if it's just for muons or any leptons
    
    if ( jetsSignal.size() < 1 ) return false;  
    countCutflowEvent("2lhigh_06_ISRjet");

    double ht=0.;
    double met = missingET->PT;
    for(int i=0; i<jetsSignal.size(); i++) ht += jetsSignal[i]->PT;
    if ( met/ht < 0.66666 or met/ht > 1.6 ) return false;
    countCutflowEvent("2lhigh_07_METoverHT");

    if ( ht < 100. ) return false;
    countCutflowEvent("2lhigh_08_minHT");

    if ( met < 240. or met > 290. ) return false;
    countCutflowEvent("2lhigh_09_MET");
    
    if (rand()/double(RAND_MAX) > 1.) return false; //efficency correction
    countCutflowEvent("2lhigh_10_METtrigger");
    
    if ( leptons[0]->Charge * leptons[1]->Charge > 0 ) return false;
    countCutflowEvent("2lhigh_11_OS");
    
    if (leptons.size() != leptonsTight.size() ) return false; //veto events with additional leptons with pt>30
    countCutflowEvent("2lhigh_12_twoTight");

    for(int i=0; i<jetsSignal.size(); i++) if ( checkBTag(jetsSignal[i]) ) return false;
    countCutflowEvent("2lhigh_13_bveto");

    double mtata = mtautau(leptons);
    if (mtata > 0. and mtata <  160.) return false;
    countCutflowEvent("2lhigh_14_mtautau");

    double mtl1 = mT(leptons[0]->P4(), missingET->P4());
    double mtl2 = mT(leptons[1]->P4(), missingET->P4());
    if (mtl1 > 70. or mtl2 > 70.) return false;
    countCutflowEvent("2lhigh_15_mT");

    if (leptons[0]->Type != leptons[1]->Type) return false;
    countCutflowEvent("2lhigh_16_SF");

    if ( mll < 1. or mll > 50. ) return false;
    countCutflowEvent("2lhigh_17_mll");
    
    if ( (mll > 3. and mll < 3.2) ) return false; //veto J/psi
    countCutflowEvent("2lhigh_18_Jpsi_veto");
    
    if ((leptons[0]->Type == "electron" and leptons[0]->PT < 5.) or (leptons[0]->Type == "muon" and leptons[0]->PT < 3.5) or leptons[0]->PT > 30.) return false;
    countCutflowEvent("2lhigh_19_leadlepPT");
    
    if( leptons[0]->P4().DeltaR(leptons[1]->P4()) < 0.3 ) return false;
    countCutflowEvent("2lhigh_20_mindR");

    if (mll > 1. and mll <  4.) countSignalEvent("2l_high_01");
    if (mll > 4. and mll <  10.) countSignalEvent("2l_high_04");
    if (mll > 10. and mll <  20.) countSignalEvent("2l_high_10");
    if (mll > 20. and mll <  30.) countSignalEvent("2l_high_20");
    if (mll > 30. and mll <  50.) countSignalEvent("2l_high_30");
    return true;

}

bool Cms_2111_06295::SR_2l_ultra(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal) {

    countCutflowEvent("2lultra_02_dilep");
    
    if ( (leptons[1]->Type == "muon" and leptons[1]->PT < 3.5) or (leptons[1]->Type == "electron" and leptons[1]->PT < 5.) ) return false;
    countCutflowEvent("2lultra_03_subleppt"); 
    
    double mll = (leptons[0]->P4() + leptons[1]->P4()).M();
    if ( (mll > 9. and mll < 10.5) ) return false; //veto J/psi and Upsilon
    countCutflowEvent("2lultra_04_Ups_veto");

    double pll = (leptons[0]->P4() + leptons[1]->P4()).Pt();
    if ( leptons[0]->Type == "muon" and leptons[1]->Type == "muon" and pll < 3. ) return false;
    countCutflowEvent("2lultra_05_dilepPt"); //unclear if it's just for muons or any leptons
    
    if ( jetsSignal.size() < 1 ) return false;  
    countCutflowEvent("2lultra_06_ISRjet");

    double ht=0.;
    double met = missingET->PT;
    for(int i=0; i<jetsSignal.size(); i++) ht += jetsSignal[i]->PT;
    if ( met/ht < 0.66666 or met/ht > 1.6 ) return false;
    countCutflowEvent("2lultra_07_METoverHT");

    if ( ht < 100. ) return false;
    countCutflowEvent("2lultra_08_minHT");

    if ( met < 290. ) return false;
    countCutflowEvent("2lultra_09_MET");
    
    if (rand()/double(RAND_MAX) > 1.) return false; //efficency correction
    countCutflowEvent("2lultra_10_METtrigger");
    
    if ( leptons[0]->Charge * leptons[1]->Charge > 0 ) return false;
    countCutflowEvent("2lultra_11_OS");
    
    if (leptons.size() != leptonsTight.size() ) return false; //veto events with additional leptons with pt>30
    countCutflowEvent("2lultra_12_twoTight");

    for(int i=0; i<jetsSignal.size(); i++) if ( checkBTag(jetsSignal[i]) ) return false;
    countCutflowEvent("2lultra_13_bveto");

    double mtata = mtautau(leptons);
    if (mtata > 0. and mtata <  160.) return false;
    countCutflowEvent("2lultra_14_mtautau");

    double mtl1 = mT(leptons[0]->P4(), missingET->P4());
    double mtl2 = mT(leptons[1]->P4(), missingET->P4());
    if (mtl1 > 70. or mtl2 > 70.) return false;
    countCutflowEvent("2lultra_15_mT");

    if (leptons[0]->Type != leptons[1]->Type) return false;
    countCutflowEvent("2lultra_16_SF");

    if ( mll < 1. or mll > 50. ) return false;
    countCutflowEvent("2lultra_17_mll");
    
    if ( (mll > 3. and mll < 3.2) ) return false; //veto J/psi
    countCutflowEvent("2lultra_18_Jpsi_veto");
    
    if ((leptons[0]->Type == "electron" and leptons[0]->PT < 5.) or (leptons[0]->Type == "muon" and leptons[0]->PT < 3.5) or leptons[0]->PT > 30.) return false;
    countCutflowEvent("2lultra_19_leadlepPT");
    
    if( leptons[0]->P4().DeltaR(leptons[1]->P4()) < 0.3 ) return false;
    countCutflowEvent("2lultra_20_mindR");

    if (mll > 1. and mll <  4.) countSignalEvent("2l_ultra_01");
    if (mll > 4. and mll <  10.) countSignalEvent("2l_ultra_04");
    if (mll > 10. and mll <  20.) countSignalEvent("2l_ultra_10");
    if (mll > 20. and mll <  30.) countSignalEvent("2l_ultra_20");
    if (mll > 30. and mll <  50.) countSignalEvent("2l_ultra_30");
    return true;

}

bool Cms_2111_06295::CR_2l_DY_low(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal) {
    
    if ( (leptons[1]->Type == "muon" and leptons[1]->PT < 3.5) or (leptons[1]->Type == "electron" and leptons[1]->PT < 5.) ) return false;
    double mll = (leptons[0]->P4() + leptons[1]->P4()).M();
    if ( mll < 4. or mll > 50. ) return false;
    if ( (mll > 9. and mll < 10.5) ) return false; //veto J/psi and Upsilon
    double pll = (leptons[0]->P4() + leptons[1]->P4()).Pt();
    if (  leptons[0]->Type == "muon" and leptons[1]->Type == "muon" and pll < 3. ) return false;
    if ( jetsSignal.size() < 1 ) return false;  
    double ht=0.;
    double met = missingET->PT;
    for(int i=0; i<jetsSignal.size(); i++) ht += jetsSignal[i]->PT;
    if ( met/ht < 0.66666 or met/ht > 1.6 ) return false;
    if ( ht < 100. ) return false;
    if ( met > 200. or met < 125. ) return false;
    if (leptons[0]->Type != "muon" or leptons[1]->Type != "muon") return false; 
    if ( leptons[0]->Charge * leptons[1]->Charge > 0 ) return false;
    if (leptons[0]->PT < 5.) return false;
    if (leptons.size() != leptonsTight.size() ) return false; //veto events with additional leptons with pt>30
    for(int i=0; i<jetsSignal.size(); i++) if ( checkBTag(jetsSignal[i]) ) return false;
    double mtata = mtautau(leptons);
    if (mtata < 0. or mtata >  160.) return false;
    double mtl1 = mT(leptons[0]->P4(), missingET->P4());
    double mtl2 = mT(leptons[1]->P4(), missingET->P4());
    if (mtl1 > 70. or mtl2 > 70.) return false;
    if (leptons[1]->PT < 5.) return false;
    
    return true;
}

double Cms_2111_06295::mtautau(std::vector<FinalStateObject*> leptons) {
  
  TVector3 p1(leptons[0]->P4().Px(), leptons[0]->P4().Py(), 0.);
  TVector3 p2(leptons[1]->P4().Px(), leptons[1]->P4().Py(), 0.);
  TVector3 pmiss(missingET->P4().Px(), missingET->P4().Py(), 0.);

  double a11 =  p1.Dot(p1);
  double a12 =  p1.Dot(p2);
  double a22 =  p2.Dot(p2);
  double b1 =  p1.Dot(pmiss);
  double b2 =  p2.Dot(pmiss);
  double det = a11*a22 - a12*a12;
  if (det != 0.) {
    double x1 = (a22*b1 - a12*b2)/det;
    double x2 = (a11*b2 - a12*b1)/det;
    double mtautau2 = 2.*leptons[0]->P4().Dot(leptons[1]->P4())*(1+x1)*(1+x2);
    if (mtautau2 > 0.) return sqrt(mtautau2);
    else return -sqrt(-mtautau2);
  }
  else return 0.;
}