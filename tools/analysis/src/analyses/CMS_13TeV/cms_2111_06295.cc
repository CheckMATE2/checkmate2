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
    countCutflowEvent("cut_00_alwaystrue");
    countCutflowEvent("cut_01_eventFilters");

  double mET_no_muon = missingET->PT;

  missingET->addMuons(muons);

  double mET = missingET->PT;

  //h4->Fill(mET);
  //fout1 << "muon.size()=" << muons.size() << std::endl;
  //fout1 << mET << ", " << mET_no_muon << std::endl;

//reconstruction
//muonsCombined could not be used in CMS
//electrons could be used but with worse case compared to electronsLoose and electronsMedium 
  std::vector<Electron*> electrons_g;
  std::vector<Muon*> muons_g;

  for ( int i=0; i < electronsLoose.size(); i++ ) {
    if ( electronsLoose[i]->PT > 5. and electronsLoose[i]->PT < 10 ) {
      if ( fabs(electronsLoose[i]->Eta) < 2.5 ) {
        electrons_g.push_back(electronsLoose[i]);
      }
    } 
  }

  for ( int i=0; i < electronsMedium.size(); i++ ) {
    if ( electronsMedium[i]->PT >= 10 ) {
      if ( fabs(electronsMedium[i]->Eta) < 2.5 ) {
        electrons_g.push_back(electronsMedium[i]);
      }
    } 
  }
  
  muons_g = filterPhaseSpace(muons, 3.5, -2.4, 2.4);
  
  //isolation
  electrons_g = filterIsolation( electrons_g );
  muons_g     = filterIsolation( muons_g );

  //jets and b-taging jets
  jets = filterPhaseSpace(jets, 25, -2.4, 2.4);

  jets = overlapRemoval( jets, electrons_g, 0.4 );
  jets = overlapRemoval( jets, muons_g, 0.4 );
  electrons_g = overlapRemoval( electrons_g, jets, 0.4);
  muons_g = overlapRemoval( muons_g, jets, 0.4);

  std::vector<Jet*> bjets;
  for(int i=0; i<jets.size(); i++) {
    if ( checkBTag(jets[i]) ) {
      bjets.push_back(jets[i]);
    }
  }

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
