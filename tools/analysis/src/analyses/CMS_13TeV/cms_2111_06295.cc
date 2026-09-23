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

  //eventually OS-check is actually later in the cutflow... need to adjust here.. or maybe not
  // the CMS cutflow is totally stupid
  if (leptons.size() == 2 and mllmin < 1000.) {
    countCutflowEvent("02_tt_dilep"); //found OS pair
    // run stop selections
    if (mllOSmin < 1000.) {
      countCutflowEvent("02_2l_dilep");
      countCutflowEvent("03_2l_subleppt"); //dodgy requirement
      mll = (leptons[0]->P4() + leptons[1]->P4()).M();
      if ( (mll > 3. and mll < 3.2) or (mll > 9. and mll < 10.5) ) return; //veto J/psi and Upsilon
      countCutflowEvent("04_2l_Jpsi_veto");
      countCutflowEvent("05_subpT"); //another meaningless requirement
      if ( jets.size() < 1 ) return;  
      countCutflowEvent("06_ISRjet");
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


bool Cms_2111_06295::SR_2l_low(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal) {

    countCutflowEvent("02_2llow_dilep");
    countCutflowEvent("03_2llow_subleppt"); //dodgy requirement
    double mll = (leptons[0]->P4() + leptons[1]->P4()).M();
    if ( mll < 4. or mll > 50. ) return false;
    countCutflowEvent("04_2llow_mll");
    if ( (mll > 9. and mll < 10.5) ) return false; //veto J/psi and Upsilon
    countCutflowEvent("05_2llow_Ups_veto");
    double pll = (leptons[0]->P4() + leptons[1]->P4()).Pt();
    if ( pll < 3. ) return false;
    countCutflowEvent("06_2llow_dilepPt"); //unclear if it's just for muons or any leptons
    if ( jetsSignal.size() < 1 ) return false;  
    countCutflowEvent("07_2llow_ISRjet");
    double ht=0.;
    double met = missingET->ET;
    for(int i=0; i<jetsSignal.size(); i++) ht += jetsSignal[i]->PT;
    if ( met/ht < 0.66666 or met/ht > 1.6 ) return false;
    countCutflowEvent("08_2llow_METoverHT");
    if ( ht < 100. ) return false;
    countCutflowEvent("09_2llow_minHT");
    if ( met > 200. or met < 125. ) return false;
    countCutflowEvent("10_2llow_MET");
    double ptrig = 0.4 + 0.5/75.*(met-125.); // some approx of fig.5 in 1903.06078; trigger efficiency for met
    if (rand()/double(RAND_MAX) > ptrig) return false;
    countCutflowEvent("11_2llow_METtrigger");
    if ( leptons[0]->Charge * leptons[1]->Charge > 0 ) return false;
    countCutflowEvent("12_2llow_OS");
    if (leptons[0]->PT < 5. or leptons[0]->PT > 30.) return false;
    countCutflowEvent("13_2llow_leadlepPT");
    if (leptons[0].size() != leptonsTight.size() ) return false; //veto events with additional leptons with pt>30
    countCutflowEvent("14_2llow_twoTight");
    for(int i=0; i<jetsSignal.size(); i++) if ( checkBTag(jetsSignal[i]) ) return false;
    countCutflowEvent("15_2llow_bveto");
    double mtautau = mtautau(leptons);
    if (mtautau > 0. and mtautau <  160.) return false;
    countCutflowEvent("16_2llow_mtautau");



}

double Cms_2111_06295::mtautau(std::vector<FinalStateObject*> leptons) {
  
  XYZVector p1 = XYZVector(leptons[0]->PX(), leptons[0]->PY(), 0.);
  XYZVector p2 = XYZVector(leptons[1]->PX(), leptons[1]->PY(), 0.);
  XYZVector pmiss = XYZVector(missingET->Px(), missingET->Py(), 0.);

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