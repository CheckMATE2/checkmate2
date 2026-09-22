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
  
  electronsLoose = filterPhaseSpace(electronsLoose, 5., -2.5, 2.5);
  electronsTight = filterPhaseSpace(electronsTight, 5., -2.5, 2.5);
  muonsCombined = filterPhaseSpace(muonsCombined, 3.5, -2.4, 2.4);
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
  
  std::vector<FinalStateObject*> leptons;
  for ( int i = 0; i <  electronsLoose.size(); i++ ) { //we later check that Loose survives to Tight
    FinalStateObject* lep = newFinalStateObject(electronsLoose[i]);
    leptons.push_back(lep);
    //cout << "e " ;
  }
  for ( int i = 0; i < muonsSignal.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(muonsSignal[i]);
    leptons.push_back(lep);
    //cout << "mu " ;
  }
  std::sort(leptons.begin(), leptons.end(), FinalStateObject::sortByPT);

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
  
  double mllOSmin = 999999.;
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
  }

  //eventually OS check is actually later in the cutflow... need to adjust here
  if (leptons.size() == 2 and mllmin < 1000.) {
    countCutflowEvent("02_tt_dilep"); //found OS pair
    // run stop selections
    if (mllOSmin < 1000.) {
      countCutflowEvent("02_2l_dilep");
      countCutflowEvent("03_2l_subleppt"); //dodgy requirement
      // run 2l selections
    }
  }
  else if  (leptons.size() == 3 and mllOSmin < 1000.) {
    countCutflowEvent("02_3l_dilep");
    // run 3l selections
  }
  else return; // 3 SS leptons or something weird



  //ht
  double ht=0.;
  for(int i=0; i<jets.size(); i++) {
    ht += jets[i]->PT;
  }  
 
  bool softmu = false; //soft muon requires higher met threshold
  //for leptons with PT order
  std::vector<FinalStateObject*> leptons;
  for( int i=0; i < electrons_g.size(); i++) {
    FinalStateObject* lep = new FinalStateObject( electrons_g[i] );
    leptons.push_back(lep);
  }
  for( int i=0; i < muons_g.size(); i++) {
    if (muons_g[i]->PT < 5.) softmu = true; 
    FinalStateObject* lep = new FinalStateObject( muons_g[i] );
    leptons.push_back(lep);
  }
  std::sort( leptons.begin(), leptons.end(), FinalStateObject::sortByPT );

  //for two leptons at the trigger level
  if(leptons.size() != 2) return;

  //for two opposite-sign leptons 
  if(leptons[0]->Charge * leptons[1]->Charge > 0 ) return;
  
  bool SF = false;
  if(leptons[0]->Type == leptons[1]->Type ) SF = true;

  countCutflowEvent("cut_02_dilep"); //lep pt was ok from start 
  countCutflowEvent("cut_03_sublepPt"); 
  
  PxPyPzEVector dilepton = leptons[0]->P4() + leptons[0]->P4();
  double mll = dilepton.M();
  double pll = dilpeton.Pt();
 
}

void Cms_2111_06295::finalize() {
  // Whatever should be done after the run goes here
}       
