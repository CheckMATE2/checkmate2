#include "cms_2111_06295.h"
// AUTHOR: kr
//  EMAIL: krolb@fuw.edu.pl
void Cms_2111_06295::initialize() {
  setAnalysisName("cms_2111_06295");          
  setInformation(""
    "\n"
  "");
  setLuminosity(137.0*units::INVFB);      
  bookSignalRegions("2l_low_04;2l_low_10;2l_low_20;2l_low_30;2l_med_01;2l_med_04;2l_med_10;2l_med_20;2l_med_30;2l_high_01;2l_high_04;2l_high_10;2l_high_20;2l_high_30;2l_ultra_01;2l_ultra_04;2l_ultra_10;2l_ultra_20;2l_ultra_30;3l_EWK_low_04;3l_EWK_low_10;3l_EWK_low_20;3l_EWK_low_30;3l_EWK_med_01;3l_EWK_med_04;3l_EWK_med_10;3l_EWK_med_20;3l_EWK_med_30;3l_WZ_low_04;3l_WZ_low_10;3l_WZ_low_20;3l_WZ_med_01;3l_WZ_med_04;3l_WZ_med_10;3l_WZ_med_20;stop_low_03;stop_low_08;stop_low_12;stop_low_16;stop_low_20;stop_low_25;stop_med_03;stop_med_08;stop_med_12;stop_med_16;stop_med_20;stop_med_25;stop_high_03;stop_high_08;stop_high_12;stop_high_16;stop_high_20;stop_high_25;stop_ultra_03;stop_ultra_08;stop_ultra_12;stop_ultra_16;stop_ultra_20;stop_ultra_25");
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
  electronsLoose = filterIsolation(electronsLoose, 0);
  electronsTight = filterIsolation(electronsTight, 0);
  muonsCombined = filterIsolation(muonsCombined, 0);
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
    FinalStateObject* lep = newFinalStateObject(electronsTight[i]);
    leptonsTight.push_back(lep);
    //cout << "e " ;
  }
  for ( int i = 0; i < muonsSignal.size(); i++ ) {
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
  else if  (leptonsLoose.size() == 3 ) {
    countCutflowEvent("02_3l_dilep");
    // run 3l selections
  }
  else return; // 3 SS leptons or something weird

 
}

void Cms_2111_06295::finalize() {
  // Whatever should be done after the run goes here
}       


bool Cms_2111_06295::SR_2l_low(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal) {

    countCutflowEvent("02_2llow_dilep");
    if ( (leptons[1]->Type == "muon" and leptons[1]->PT < 3.5) or (leptons[1]->Type == "electron" and leptons[1]->PT < 5.) ) return false;
    countCutflowEvent("03_2llow_subleppt"); 
    
    double mll = (leptons[0]->P4() + leptons[1]->P4()).M();
    if ( mll < 4. or mll > 50. ) return false;
    countCutflowEvent("04_2llow_mll");
    
    if ( (mll > 9. and mll < 10.5) ) return false; //veto J/psi and Upsilon
    countCutflowEvent("05_2llow_Ups_veto");
    
    double pll = (leptons[0]->P4() + leptons[1]->P4()).Pt();
    if (  leptons[0]->Type == "muon" and leptons[1]->Type == "muon" and pll < 3. ) return false;
    countCutflowEvent("06_2llow_dilepPt"); //unclear if it's just for muons or any leptons
    
    if ( jetsSignal.size() < 1 ) return false;  
    countCutflowEvent("07_2llow_ISRjet");
    
    double ht=0.;
    double met = missingET->PT;
    for(int i=0; i<jetsSignal.size(); i++) ht += jetsSignal[i]->PT;
    if ( met/ht < 0.66666 or met/ht > 1.6 ) return false;
    countCutflowEvent("08_2llow_METoverHT");
    
    if ( ht < 100. ) return false;
    
    countCutflowEvent("09_2llow_minHT");
    
    if ( met > 200. or met < 125. ) return false;
    countCutflowEvent("10_2llow_MET");
    //double ptrig = 0.4 + 0.5/75.*(met-125.); // some approx of fig.5 in 1903.06078; trigger efficiency for met
    //eventually it seems only muon trigger is used
    //if (rand()/double(RAND_MAX) > ptrig) return false;
    
    if (leptons[0]->Type != "muon" or leptons[1]->Type != "muon") return false; 
    countCutflowEvent("11_2llow_METtrigger");
    
    if ( leptons[0]->Charge * leptons[1]->Charge > 0 ) return false;
    countCutflowEvent("12_2llow_OS");
    
    if (leptons[0]->PT < 5. or leptons[0]->PT > 30.) return false;
    countCutflowEvent("13_2llow_leadlepPT");
    
    if (leptons.size() != leptonsTight.size() ) return false; //veto events with additional leptons with pt>30
    countCutflowEvent("14_2llow_twoTight");
    
    for(int i=0; i<jetsSignal.size(); i++) if ( checkBTag(jetsSignal[i]) ) return false;
    countCutflowEvent("15_2llow_bveto");
    
    double mtata = mtautau(leptons);
    if (mtata > 0. and mtata <  160.) return false;
    countCutflowEvent("16_2llow_mtautau");
    
    double mtl1 = mT(leptons[0]->P4(), missingET->P4());
    double mtl2 = mT(leptons[1]->P4(), missingET->P4());
    if (mtl1 > 70. or mtl2 > 70.) return false;
    countCutflowEvent("17_2llow_mT");
    
    countCutflowEvent("18_2llow_SF");
    countCutflowEvent("19_2llow_mm");
    
    if (leptons[1]->PT < 5.) return false;
    countCutflowEvent("20_2llow_pt5sublep");

    if (mll > 4. and mll <  10.) countSignalEvent("2l_low_04");
    if (mll > 10. and mll <  20.) countSignalEvent("2l_low_10");
    if (mll > 20. and mll <  30.) countSignalEvent("2l_low_20");
    if (mll > 30. and mll <  50.) countSignalEvent("2l_low_30");
    return true;

}

bool Cms_2111_06295::SR_2l_med(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal) {

    countCutflowEvent("02_2lmed_dilep");
    
    if ( (leptons[1]->Type == "muon" and leptons[1]->PT < 3.5) or (leptons[1]->Type == "electron" and leptons[1]->PT < 5.) ) return false;
    countCutflowEvent("03_2lmed_subleppt"); 
    
    double mll = (leptons[0]->P4() + leptons[1]->P4()).M();
    if ( (mll > 9. and mll < 10.5) ) return false; //veto J/psi and Upsilon
    countCutflowEvent("04_2lmed_Ups_veto");

    double pll = (leptons[0]->P4() + leptons[1]->P4()).Pt();
    if ( leptons[0]->Type == "muon" and leptons[1]->Type == "muon" and pll < 3. ) return false;
    countCutflowEvent("05_2lmed_dilepPt"); //unclear if it's just for muons or any leptons
    
    if ( jetsSignal.size() < 1 ) return false;  
    countCutflowEvent("06_2lmed_ISRjet");

    double ht=0.;
    double met = missingET->PT;
    for(int i=0; i<jetsSignal.size(); i++) ht += jetsSignal[i]->PT;
    if ( met/ht < 0.66666 or met/ht > 1.6 ) return false;
    countCutflowEvent("07_2lmed_METoverHT");

    if ( ht < 100. ) return false;
    countCutflowEvent("08_2lmed_minHT");

    if ( met < 200. or met > 240. ) return false;
    countCutflowEvent("09_2lmed_MET");
    
    if (rand()/double(RAND_MAX) > 0.95) return false; //efficency correction
    countCutflowEvent("10_2lmed_METtrigger");
    
    if ( leptons[0]->Charge * leptons[1]->Charge > 0 ) return false;
    countCutflowEvent("11_2lmed_OS");
    
    if (leptons.size() != leptonsTight.size() ) return false; //veto events with additional leptons with pt>30
    countCutflowEvent("12_2lmed_twoTight");

    for(int i=0; i<jetsSignal.size(); i++) if ( checkBTag(jetsSignal[i]) ) return false;
    countCutflowEvent("13_2lmed_bveto");

    double mtata = mtautau(leptons);
    if (mtata > 0. and mtata <  160.) return false;
    countCutflowEvent("14_2lmed_mtautau");

    double mtl1 = mT(leptons[0]->P4(), missingET->P4());
    double mtl2 = mT(leptons[1]->P4(), missingET->P4());
    if (mtl1 > 70. or mtl2 > 70.) return false;
    countCutflowEvent("15_2lmed_mT");

    if (leptons[0]->Type != leptons[1]->Type) return false;
    countCutflowEvent("16_2lmed_SF");

    if ( mll < 1. or mll > 50. ) return false;
    countCutflowEvent("17_2lmed_mll");
    
    if ( (mll > 3. and mll < 3.2) ) return false; //veto J/psi
    countCutflowEvent("18_2lmed_Jpsi_veto");
    
    if ((leptons[0]->Type == "electron" and leptons[0]->PT < 5.) or (leptons[0]->Type == "muon" and leptons[0]->PT < 3.5) or leptons[0]->PT > 30.) return false;
    countCutflowEvent("19_2lmed_leadlepPT");
    
    if( leptons[0]->P4().DeltaR(leptons[1]->P4()) < 0.3 ) return false;
    countCutflowEvent("20_2lmed_mindR");

    if (mll > 1. and mll <  4.) countSignalEvent("2l_med_01");
    if (mll > 4. and mll <  10.) countSignalEvent("2l_med_04");
    if (mll > 10. and mll <  20.) countSignalEvent("2l_med_10");
    if (mll > 20. and mll <  30.) countSignalEvent("2l_med_20");
    if (mll > 30. and mll <  50.) countSignalEvent("2l_med_30");
    return true;

}

bool Cms_2111_06295::SR_2l_high(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal) {

    countCutflowEvent("02_2lhigh_dilep");
    
    if ( (leptons[1]->Type == "muon" and leptons[1]->PT < 3.5) or (leptons[1]->Type == "electron" and leptons[1]->PT < 5.) ) return false;
    countCutflowEvent("03_2lhigh_subleppt"); 
    
    double mll = (leptons[0]->P4() + leptons[1]->P4()).M();
    if ( (mll > 9. and mll < 10.5) ) return false; //veto J/psi and Upsilon
    countCutflowEvent("04_2lhigh_Ups_veto");

    double pll = (leptons[0]->P4() + leptons[1]->P4()).Pt();
    if ( leptons[0]->Type == "muon" and leptons[1]->Type == "muon" and pll < 3. ) return false;
    countCutflowEvent("05_2lhigh_dilepPt"); //unclear if it's just for muons or any leptons
    
    if ( jetsSignal.size() < 1 ) return false;  
    countCutflowEvent("06_2lhigh_ISRjet");

    double ht=0.;
    double met = missingET->PT;
    for(int i=0; i<jetsSignal.size(); i++) ht += jetsSignal[i]->PT;
    if ( met/ht < 0.66666 or met/ht > 1.6 ) return false;
    countCutflowEvent("07_2lhigh_METoverHT");

    if ( ht < 100. ) return false;
    countCutflowEvent("08_2lhigh_minHT");

    if ( met < 240. or met > 290. ) return false;
    countCutflowEvent("09_2lhigh_MET");
    
    if (rand()/double(RAND_MAX) > 1.) return false; //efficency correction
    countCutflowEvent("10_2lhigh_METtrigger");
    
    if ( leptons[0]->Charge * leptons[1]->Charge > 0 ) return false;
    countCutflowEvent("11_2lhigh_OS");
    
    if (leptons.size() != leptonsTight.size() ) return false; //veto events with additional leptons with pt>30
    countCutflowEvent("12_2lhigh_twoTight");

    for(int i=0; i<jetsSignal.size(); i++) if ( checkBTag(jetsSignal[i]) ) return false;
    countCutflowEvent("13_2lhigh_bveto");

    double mtata = mtautau(leptons);
    if (mtata > 0. and mtata <  160.) return false;
    countCutflowEvent("14_2lhigh_mtautau");

    double mtl1 = mT(leptons[0]->P4(), missingET->P4());
    double mtl2 = mT(leptons[1]->P4(), missingET->P4());
    if (mtl1 > 70. or mtl2 > 70.) return false;
    countCutflowEvent("15_2lhigh_mT");

    if (leptons[0]->Type != leptons[1]->Type) return false;
    countCutflowEvent("16_2lhigh_SF");

    if ( mll < 1. or mll > 50. ) return false;
    countCutflowEvent("17_2lhigh_mll");
    
    if ( (mll > 3. and mll < 3.2) ) return false; //veto J/psi
    countCutflowEvent("18_2lhigh_Jpsi_veto");
    
    if ((leptons[0]->Type == "electron" and leptons[0]->PT < 5.) or (leptons[0]->Type == "muon" and leptons[0]->PT < 3.5) or leptons[0]->PT > 30.) return false;
    countCutflowEvent("19_2lhigh_leadlepPT");
    
    if( leptons[0]->P4().DeltaR(leptons[1]->P4()) < 0.3 ) return false;
    countCutflowEvent("20_2lhigh_mindR");

    if (mll > 1. and mll <  4.) countSignalEvent("2l_high_01");
    if (mll > 4. and mll <  10.) countSignalEvent("2l_high_04");
    if (mll > 10. and mll <  20.) countSignalEvent("2l_high_10");
    if (mll > 20. and mll <  30.) countSignalEvent("2l_high_20");
    if (mll > 30. and mll <  50.) countSignalEvent("2l_high_30");
    return true;

}

bool Cms_2111_06295::SR_2l_ultra(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal) {

    countCutflowEvent("02_2lultra_dilep");
    
    if ( (leptons[1]->Type == "muon" and leptons[1]->PT < 3.5) or (leptons[1]->Type == "electron" and leptons[1]->PT < 5.) ) return false;
    countCutflowEvent("03_2lultra_subleppt"); 
    
    double mll = (leptons[0]->P4() + leptons[1]->P4()).M();
    if ( (mll > 9. and mll < 10.5) ) return false; //veto J/psi and Upsilon
    countCutflowEvent("04_2lultra_Ups_veto");

    double pll = (leptons[0]->P4() + leptons[1]->P4()).Pt();
    if ( leptons[0]->Type == "muon" and leptons[1]->Type == "muon" and pll < 3. ) return false;
    countCutflowEvent("05_2lultra_dilepPt"); //unclear if it's just for muons or any leptons
    
    if ( jetsSignal.size() < 1 ) return false;  
    countCutflowEvent("06_2lultra_ISRjet");

    double ht=0.;
    double met = missingET->PT;
    for(int i=0; i<jetsSignal.size(); i++) ht += jetsSignal[i]->PT;
    if ( met/ht < 0.66666 or met/ht > 1.6 ) return false;
    countCutflowEvent("07_2lultra_METoverHT");

    if ( ht < 100. ) return false;
    countCutflowEvent("08_2lultra_minHT");

    if ( met < 290. ) return false;
    countCutflowEvent("09_2lultra_MET");
    
    if (rand()/double(RAND_MAX) > 1.) return false; //efficency correction
    countCutflowEvent("10_2lultra_METtrigger");
    
    if ( leptons[0]->Charge * leptons[1]->Charge > 0 ) return false;
    countCutflowEvent("11_2lultra_OS");
    
    if (leptons.size() != leptonsTight.size() ) return false; //veto events with additional leptons with pt>30
    countCutflowEvent("12_2lultra_twoTight");

    for(int i=0; i<jetsSignal.size(); i++) if ( checkBTag(jetsSignal[i]) ) return false;
    countCutflowEvent("13_2lultra_bveto");

    double mtata = mtautau(leptons);
    if (mtata > 0. and mtata <  160.) return false;
    countCutflowEvent("14_2lultra_mtautau");

    double mtl1 = mT(leptons[0]->P4(), missingET->P4());
    double mtl2 = mT(leptons[1]->P4(), missingET->P4());
    if (mtl1 > 70. or mtl2 > 70.) return false;
    countCutflowEvent("15_2lultra_mT");

    if (leptons[0]->Type != leptons[1]->Type) return false;
    countCutflowEvent("16_2lultra_SF");

    if ( mll < 1. or mll > 50. ) return false;
    countCutflowEvent("17_2lultra_mll");
    
    if ( (mll > 3. and mll < 3.2) ) return false; //veto J/psi
    countCutflowEvent("18_2lultra_Jpsi_veto");
    
    if ((leptons[0]->Type == "electron" and leptons[0]->PT < 5.) or (leptons[0]->Type == "muon" and leptons[0]->PT < 3.5) or leptons[0]->PT > 30.) return false;
    countCutflowEvent("19_2lultra_leadlepPT");
    
    if( leptons[0]->P4().DeltaR(leptons[1]->P4()) < 0.3 ) return false;
    countCutflowEvent("20_2lultra_mindR");

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