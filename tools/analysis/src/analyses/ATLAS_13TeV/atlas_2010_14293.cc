#include "atlas_2010_14293.h"
#include "TMVA/Reader.h"

using namespace TMVA;

// AUTHOR: K. Rolbiecki
//  EMAIL: a
void Atlas_2010_14293::initialize() {
  setAnalysisName("atlas_2010_14293");          
  setInformation(""
    "# search for squarks and gluinos in MET_jet final states\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("MB-SSd-2-1000-10;MB-SSd-2-1000-16;MB-SSd-2-1000-22;MB-SSd-2-1600-10;MB-SSd-2-1600-16;MB-SSd-2-1600-22;MB-SSd-2-2200-16;MB-SSd-2-2200-22;MB-SSd-2-2800-16;MB-SSd-2-2800-22;MB-SSd-2-3400-22;MB-SSd-2-3400-28;MB-SSd-2-4000-22;MB-SSd-2-4000-28;MB-SSd-4-1000-10;MB-SSd-4-1000-16;MB-SSd-4-1000-22;MB-SSd-4-1600-10;MB-SSd-4-1600-16;MB-SSd-4-1600-22;MB-SSd-4-2200-16;MB-SSd-4-2200-22;MB-SSd-4-2800-16;MB-SSd-4-2800-22;MB-GGd-4-1000-10;MB-GGd-4-1000-16;MB-GGd-4-1000-22;MB-GGd-4-1600-10;MB-GGd-4-1600-16;MB-GGd-4-1600-22;MB-GGd-4-2200-10;MB-GGd-4-2200-16;MB-GGd-4-2200-22;MB-GGd-4-2800-10;MB-GGd-4-2800-16;MB-GGd-4-2800-22;MB-GGd-4-3400-10;MB-GGd-4-3400-16;MB-GGd-4-3400-22;MB-GGd-4-4000-10;MB-GGd-4-4000-16;MB-GGd-4-4000-22;MB-C-2-1600-16;MB-C-2-1600-22;MB-C-2-2200-16;MB-C-2-2200-22;MB-C-2-2800-16;MB-C-2-2800-22;MB-C-4-1600-16;MB-C-4-1600-22;MB-C-4-2200-16;MB-C-4-2200-22;MB-C-4-2800-16;MB-C-4-2800-22;MB-C-5-1600-16;MB-C-5-1600-22;MB-C-5-2200-16;MB-C-5-2200-22;MB-C-5-2800-16;MB-C-5-2800-22;SR-2j-1600;SR-2j-2200;SR-2j-2800;SR-4j-1000;SR-4j-2200;SR-4j-3400;SR-5j-1600;SR-6j-1000;SR-6j-2200;SR-6j-3400;BDT-GGd1;BDT-GGd2;BDT-GGd3;BDT-GGd4;BDT-GGo1;BDT-GGo2;BDT-GGo3;BDT-GGo4");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.
  bookControlRegions("MB-SSd-CRQ_2_1000_cuts;MB-SSd-CRQ_2_1600_cuts;MB-SSd-CRQ_2_2200_cuts;MB-SSd-CRQ_2_2800_cuts;MB-SSd-CRQ_2_3400_cuts;MB-SSd-CRQ_2_4000_cuts;MB-SSd-CRQ_4_1000_cuts;MB-SSd-CRQ_4_1600_cuts;MB-SSd-CRQ_4_2200_cuts;MB-SSd-CRQ_4_2800_cuts;MB-GGd-CRQ_4_1000_cuts;MB-GGd-CRQ_4_1600_cuts;MB-GGd-CRQ_4_2200_cuts;MB-GGd-CRQ_4_2800_cuts;MB-GGd-CRQ_4_3400_cuts;MB-GGd-CRQ_4_4000_cuts;MB-C-CRQ_2_1600_cuts;MB-C-CRQ_2_2200_cuts;MB-C-CRQ_2_2800_cuts;MB-C-CRQ_4_1600_cuts;MB-C-CRQ_4_2200_cuts;MB-C-CRQ_4_2800_cuts;MB-C-CRQ_5_1600_cuts;MB-C-CRQ_5_2200_cuts;MB-C-CRQ_5_2800_cuts");
  bookControlRegions("MB-SSd-CRY_2_1000_cuts;MB-SSd-CRY_2_1600_cuts;MB-SSd-CRY_2_2200_cuts;MB-SSd-CRY_2_2800_cuts;MB-SSd-CRY_2_3400_cuts;MB-SSd-CRY_2_4000_cuts;MB-SSd-CRY_4_1000_cuts;MB-SSd-CRY_4_1600_cuts;MB-SSd-CRY_4_2200_cuts;MB-SSd-CRY_4_2800_cuts;MB-GGd-CRY_4_1000_cuts;MB-GGd-CRY_4_1600_cuts;MB-GGd-CRY_4_2200_cuts;MB-GGd-CRY_4_2800_cuts;MB-GGd-CRY_4_3400_cuts;MB-GGd-CRY_4_4000_cuts;MB-C-CRY_2_1600_cuts;MB-C-CRY_2_2200_cuts;MB-C-CRY_2_2800_cuts;MB-C-CRY_4_1600_cuts;MB-C-CRY_4_2200_cuts;MB-C-CRY_4_2800_cuts;MB-C-CRY_5_1600_cuts;MB-C-CRY_5_2200_cuts;MB-C-CRY_5_2800_cuts");
  bookControlRegions("MB-SSd-CRT_2_1000_cuts;MB-SSd-CRT_2_1600_cuts;MB-SSd-CRT_2_2200_cuts;MB-SSd-CRT_2_2800_cuts;MB-SSd-CRT_2_3400_cuts;MB-SSd-CRT_2_4000_cuts;MB-SSd-CRT_4_1000_cuts;MB-SSd-CRT_4_1600_cuts;MB-SSd-CRT_4_2200_cuts;MB-SSd-CRT_4_2800_cuts;MB-GGd-CRT_4_1000_cuts;MB-GGd-CRT_4_1600_cuts;MB-GGd-CRT_4_2200_cuts;MB-GGd-CRT_4_2800_cuts;MB-GGd-CRT_4_3400_cuts;MB-GGd-CRT_4_4000_cuts;MB-C-CRT_2_1600_cuts;MB-C-CRT_2_2200_cuts;MB-C-CRT_2_2800_cuts;MB-C-CRT_4_1600_cuts;MB-C-CRT_4_2200_cuts;MB-C-CRT_4_2800_cuts;MB-C-CRT_5_1600_cuts;MB-C-CRT_5_2200_cuts;MB-C-CRT_5_2800_cuts");
  bookControlRegions("MB-SSd-CRW_2_1000_cuts;MB-SSd-CRW_2_1600_cuts;MB-SSd-CRW_2_2200_cuts;MB-SSd-CRW_2_2800_cuts;MB-SSd-CRW_2_3400_cuts;MB-SSd-CRW_2_4000_cuts;MB-SSd-CRW_4_1000_cuts;MB-SSd-CRW_4_1600_cuts;MB-SSd-CRW_4_2200_cuts;MB-SSd-CRW_4_2800_cuts;MB-GGd-CRW_4_1000_cuts;MB-GGd-CRW_4_1600_cuts;MB-GGd-CRW_4_2200_cuts;MB-GGd-CRW_4_2800_cuts;MB-GGd-CRW_4_3400_cuts;MB-GGd-CRW_4_4000_cuts;MB-C-CRW_2_1600_cuts;MB-C-CRW_2_2200_cuts;MB-C-CRW_2_2800_cuts;MB-C-CRW_4_1600_cuts;MB-C-CRW_4_2200_cuts;MB-C-CRW_4_2800_cuts;MB-C-CRW_5_1600_cuts;MB-C-CRW_5_2200_cuts;MB-C-CRW_5_2800_cuts");

  // You should initialize any declared variables here
  char *a = Global::get_maindir();
  std::string maindir(a, strlen(a));
  
  met_aux = 0.; meff_aux = 0.; Ap = 0.; jetPt0 = 0.; jetPt1 = 0.; jetPt2 = 0.; jetPt3 = 0.; jetPt4 = 0.; jetPt5 = 0.; jetEta0 = 0.; jetEta1 = 0.; jetEta2 = 0.; jetEta3 = 0.; jetEta4  = 0.; jetEta5  = 0.;
  
  for (int i = 0; i < 2; i++) {
    reader[0][i] = new TMVA::Reader(  );
    reader[0][i]->AddVariable( "met", &met_aux );
    reader[0][i]->AddVariable( "meff", &meff_aux );
    reader[0][i]->AddVariable( "Ap", &Ap );
    reader[0][i]->AddVariable( "jetPt0", &jetPt0 );
    reader[0][i]->AddVariable( "jetPt1", &jetPt1 );
    reader[0][i]->AddVariable( "jetPt2", &jetPt2 );
    reader[0][i]->AddVariable( "jetPt3", &jetPt3 );
    reader[0][i]->AddVariable( "jetEta0", &jetEta0 );
    reader[0][i]->AddVariable( "jetEta1", &jetEta1 );
    reader[0][i]->AddVariable( "jetEta2", &jetEta2 );
    reader[0][i]->AddVariable( "jetEta3", &jetEta3 );
    std::string file = maindir  + std::string("/data/atlas_2010_14293/ZeroLepton2018-SRBDT-GGd1_weight"  + std::to_string(i+1) +".xml");
    reader[0][i]->BookMVA( "BDT", file );
  }
  
  for (int i = 0; i < 2; i++) {
    reader[1][i] = new TMVA::Reader(  );
    reader[1][i]->AddVariable( "meff", &meff_aux );
    reader[1][i]->AddVariable( "Ap", &Ap );
    reader[1][i]->AddVariable( "jetPt0", &jetPt0 );
    reader[1][i]->AddVariable( "jetPt1", &jetPt1 );
    reader[1][i]->AddVariable( "jetPt2", &jetPt2 );
    reader[1][i]->AddVariable( "jetPt3", &jetPt3 );
    reader[1][i]->AddVariable( "jetEta0", &jetEta0 );
    reader[1][i]->AddVariable( "jetEta1", &jetEta1 );
    reader[1][i]->AddVariable( "jetEta2", &jetEta2 );
    reader[1][i]->AddVariable( "jetEta3", &jetEta3 );
    std::string file = maindir  + std::string("/data/atlas_2010_14293/ZeroLepton2018-SRBDT-GGd2_weight"  + std::to_string(i+1) +".xml");
    reader[1][i]->BookMVA( "BDT", file );
  }
  
  for (int i = 0; i < 2; i++) {
    reader[2][i] = new TMVA::Reader(  );
    reader[2][i]->AddVariable( "met", &met_aux );
    reader[2][i]->AddVariable( "meff", &meff_aux );
    reader[2][i]->AddVariable( "Ap", &Ap );
    reader[2][i]->AddVariable( "jetPt0", &jetPt0 );
    reader[2][i]->AddVariable( "jetPt1", &jetPt1 );
    reader[2][i]->AddVariable( "jetPt2", &jetPt2 );
    reader[2][i]->AddVariable( "jetPt3", &jetPt3 );
    reader[2][i]->AddVariable( "jetEta0", &jetEta0 );
    reader[2][i]->AddVariable( "jetEta1", &jetEta1 );
    reader[2][i]->AddVariable( "jetEta2", &jetEta2 );
    reader[2][i]->AddVariable( "jetEta3", &jetEta3 );
    std::string file = maindir  + std::string("/data/atlas_2010_14293/ZeroLepton2018-SRBDT-GGd3_weight"  + std::to_string(i+1) +".xml");
    reader[2][i]->BookMVA( "BDT", file );
  }
  
  for (int i = 0; i < 2; i++) {
    reader[3][i] = new TMVA::Reader(  );
    reader[3][i]->AddVariable( "met", &met_aux );
    reader[3][i]->AddVariable( "meff", &meff_aux );
    reader[3][i]->AddVariable( "jetPt0", &jetPt0 );
    reader[3][i]->AddVariable( "jetPt1", &jetPt1 );
    reader[3][i]->AddVariable( "jetPt2", &jetPt2 );
    reader[3][i]->AddVariable( "jetPt3", &jetPt3 );
    reader[3][i]->AddVariable( "jetEta0", &jetEta0 );
    reader[3][i]->AddVariable( "jetEta1", &jetEta1 );
    reader[3][i]->AddVariable( "jetEta2", &jetEta2 );
    reader[3][i]->AddVariable( "jetEta3", &jetEta3 );
    std::string file = maindir  + std::string("/data/atlas_2010_14293/ZeroLepton2018-SRBDT-GGd4_weight"  + std::to_string(i+1) +".xml");
    reader[3][i]->BookMVA( "BDT", file );  
  }
  
  for (int i = 0; i < 2; i++) {
    reader[4][i] = new TMVA::Reader(  );
    reader[4][i]->AddVariable( "meff", &meff_aux );
    reader[4][i]->AddVariable( "Ap", &Ap );
    reader[4][i]->AddVariable( "jetPt0", &jetPt0 );
    reader[4][i]->AddVariable( "jetPt1", &jetPt1 );
    reader[4][i]->AddVariable( "jetPt2", &jetPt2 );
    reader[4][i]->AddVariable( "jetPt3", &jetPt3 );
    reader[4][i]->AddVariable( "jetPt4", &jetPt4 );
    reader[4][i]->AddVariable( "jetEta0", &jetEta0 );
    reader[4][i]->AddVariable( "jetEta1", &jetEta1 );
    reader[4][i]->AddVariable( "jetEta2", &jetEta2 );
    reader[4][i]->AddVariable( "jetEta3", &jetEta3 );
    reader[4][i]->AddVariable( "jetEta4", &jetEta4 );
    std::string file = maindir  + std::string("/data/atlas_2010_14293/ZeroLepton2018-SRBDT-GGo1_weight"  + std::to_string(i+1) +".xml");
    reader[4][i]->BookMVA( "BDT", file );  
  }  
  
  for (int i = 0; i < 2; i++) {
    reader[5][i] = new TMVA::Reader(  );
    reader[5][i]->AddVariable( "meff", &meff_aux );
    reader[5][i]->AddVariable( "Ap", &Ap );
    reader[5][i]->AddVariable( "jetPt0", &jetPt0 );
    reader[5][i]->AddVariable( "jetPt1", &jetPt1 );
    reader[5][i]->AddVariable( "jetPt2", &jetPt2 );
    reader[5][i]->AddVariable( "jetPt3", &jetPt3 );
    reader[5][i]->AddVariable( "jetEta0", &jetEta0 );
    reader[5][i]->AddVariable( "jetEta1", &jetEta1 );
    reader[5][i]->AddVariable( "jetEta2", &jetEta2 );
    reader[5][i]->AddVariable( "jetEta3", &jetEta3 );
    std::string file = maindir  + std::string("/data/atlas_2010_14293/ZeroLepton2018-SRBDT-GGo2_weight"  + std::to_string(i+1) +".xml");
    reader[5][i]->BookMVA( "BDT", file );  
  }    
  
  for (int i = 0; i < 2; i++) {
    reader[6][i] = new TMVA::Reader(  );
    reader[6][i]->AddVariable( "meff", &meff_aux );
    reader[6][i]->AddVariable( "Ap", &Ap );
    reader[6][i]->AddVariable( "jetPt0", &jetPt0 );
    reader[6][i]->AddVariable( "jetPt1", &jetPt1 );
    reader[6][i]->AddVariable( "jetPt2", &jetPt2 );
    reader[6][i]->AddVariable( "jetPt4", &jetPt4 );
    reader[6][i]->AddVariable( "jetPt5", &jetPt5 );
    reader[6][i]->AddVariable( "jetEta0", &jetEta0 );
    reader[6][i]->AddVariable( "jetEta1", &jetEta1 );
    reader[6][i]->AddVariable( "jetEta2", &jetEta2 );
    reader[6][i]->AddVariable( "jetEta4", &jetEta4 );
    reader[6][i]->AddVariable( "jetEta5", &jetEta5 );
    std::string file = maindir  + std::string("/data/atlas_2010_14293/ZeroLepton2018-SRBDT-GGo3_weight"  + std::to_string(i+1) +".xml");
    reader[6][i]->BookMVA( "BDT", file );  
  }    
  
  for (int i = 0; i < 2; i++) {
    reader[7][i] = new TMVA::Reader(  );    
    reader[7][i]->AddVariable( "meff", &meff_aux );
    reader[7][i]->AddVariable( "met", &met_aux );
    reader[7][i]->AddVariable( "jetPt0", &jetPt0 );
    reader[7][i]->AddVariable( "jetPt1", &jetPt1 );
    reader[7][i]->AddVariable( "jetPt2", &jetPt2 );
    reader[7][i]->AddVariable( "jetPt3", &jetPt3 );
    reader[7][i]->AddVariable( "jetEta0", &jetEta0 );
    reader[7][i]->AddVariable( "jetEta1", &jetEta1 );
    reader[7][i]->AddVariable( "jetEta2", &jetEta2 );
    reader[7][i]->AddVariable( "jetEta3", &jetEta3 );
    std::string file = maindir  + std::string("/data/atlas_2010_14293/ZeroLepton2018-SRBDT-GGo4_weight"  + std::to_string(i+1) +".xml");
    reader[7][i]->BookMVA( "BDT", file );  
  }      
  
  int ifile = bookFile("atlas_2010_14293.root", true);
  const char *rootFileName = fNames[ifile].c_str() ;
  hfile = new TFile(rootFileName, "RECREATE", "Saving Histograms");
  ggd1 = new TH1F("ggd1", "BDT GGd1 output", 20, -1., 1.);
  ggo1 = new TH1F("ggo1", "BDT GGo1 output", 20, -1., 1.);
  
}

void Atlas_2010_14293::analyze() {
  missingET->addMuons(muonsCombined);  
  
  electronsLoose = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 7., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 6., -2.7, 2.7);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
  photonsMedium = filterPhaseSpace(photonsMedium, 150., -2.37, 2.37, true);
  std::vector<Photon*> CRphotons = filterIsolation(photonsMedium);
  
  jets = overlapRemoval(jets, electronsLoose, 0.2, "y");
  electronsLoose = specialoverlap(electronsLoose, jets);
  electronsTight = specialoverlap(electronsTight, jets);
  muonsCombined = specialoverlap(muonsCombined, jets);
  jets = overlapRemoval_muon_jet_tracks(jets, muonsCombined, 0.2, 2); 
  
  std::vector<Electron*> CRelectrons = Isolate_leptons_with_inverse_track_isolation_cone(electronsTight, tracks, towers, 0.2, 10., 0.2, 0.06, 0.06, true);
  std::vector<Muon*> CRmuons = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined, tracks, towers, 0.3, 10., 0.2, 0.15, 0.3, true);
  
  std::vector<Jet*> CRjets;
  if (CRphotons.size()) {
    CRjets = overlapRemoval(jets, photonsMedium, 0.4);
    CRjets = filterPhaseSpace(CRjets, 50., -2.8, 2.8);
  }
  auto newjet = new Jet;
  
  std::vector<Jet*> sigjets = filterPhaseSpace(jets, 50., -2.8, 2.8);
  
  if ( CRjets.size() > 1 and CRphotons.size() == 1 and electronsLoose.size() == 0 and muonsCombined.size() == 0 and (missingET->P4() + CRphotons[0]->P4()).Perp() > 300. and M_eff(CRjets, 0) > 800. )  {
      met = (missingET->P4() + CRphotons[0]->P4()).Perp();  // photon treated as MET for evaluation of Z CR
      evaluateCRs(CRjets, "CRY");
  }
  else if (CRmuons.size() + CRelectrons.size() == 1 and missingET->P4().Et() > 300. ) {
    
    met = missingET->P4().Et();
    
    if (CRmuons.size() ) {
      newjet->PT = CRmuons[0]->PT;
      newjet->Eta = CRmuons[0]->Eta;
      newjet->Phi = CRmuons[0]->Phi;
      newjet->Mass = 0;
    }
    else if (CRelectrons.size() > 0 ) {
      newjet->PT = CRelectrons[0]->PT;
      newjet->Eta = CRelectrons[0]->Eta;
      newjet->Phi = CRelectrons[0]->Phi;
      newjet->Mass = 0;
    }
    
    bool btag = false;
    for (int i = 0; i < sigjets.size(); i++) 
      if ( fabs(sigjets[i]->Eta) < 2.5 && checkBTag(sigjets[i]) )  {btag = true; break;}
    
    CRjets = sigjets;
    CRjets.push_back(newjet);
    std::sort(CRjets.begin(), CRjets.end(), sortByPT ); 
    CRjets = filterPhaseSpace(CRjets, 50., -2.8, 2.8);
    
    if ( CRjets.size() > 1 and CRjets[0]->PT > 200. and M_eff(CRjets, 0) > 800. and missingET->P4().Et() > 300.) {
      if (btag) evaluateCRs(CRjets, "CRT");
      else evaluateCRs(CRjets, "CRW");
    }  
  }
  else if ( sigjets.size() > 1 and sigjets[0]->PT > 200. and electronsLoose.size() == 0 and muonsCombined.size() == 0 and M_eff(sigjets, 0) > 800. and missingET->P4().Et() > 300. ) {
    
    met = missingET->P4().Et();
    evaluateCRQs(sigjets); 
    
  }  
  
  countCutflowEvent("00_all");
  
  if ( sigjets.size() < 2 || sigjets[0]->PT < 200. ) return; //too early?
  if ( M_eff(sigjets, 0) < 800. ) return;
    
  met = missingET->P4().Et();
  
  if(electronsLoose.size() > 0) return;
  if(muonsCombined.size() > 0) return; 
  if (met < 300.) return;

  
  countCutflowEvent("01_Preselection");
  
//  if( dPhi(sigjets, 0) < 0.4 ) return;
 
//                        PTj1  PTj2  Nj  Eta Phi1 Phi2 ET/HT Apl   Meff
  if (Passes_Cuts(sigjets, 250., 250., 2, 2.0, 0.8, 0.4, 16., 0.,   1600., false, "2j-1600") ) countSignalEvent("SR-2j-1600");
  if (Passes_Cuts(sigjets, 600., 50.,  2, 2.8, 0.4, 0.2, 16., 0.,   2200., false, "2j-2200") ) countSignalEvent("SR-2j-2200");  
  if (Passes_Cuts(sigjets, 250., 250., 2, 1.2, 0.8, 0.4, 16., 0.,   2800., false, "2j-2800") ) countSignalEvent("SR-2j-2800");    
  if (Passes_Cuts(sigjets, 200., 100., 4, 2.0, 0.4, 0.4, 16., 0.04, 1000., false, "4j-1000") ) countSignalEvent("SR-4j-1000");
  if (Passes_Cuts(sigjets, 200., 100., 4, 2.0, 0.4, 0.4, 16., 0.04, 2200., false, "4j-2200") ) countSignalEvent("SR-4j-2200");
  if (Passes_Cuts(sigjets, 200., 100., 4, 2.0, 0.4, 0.4, 10., 0.04, 3400., false, "4j-3400") ) countSignalEvent("SR-4j-3400");  
  if (Passes_Cuts(sigjets, 600., 50.,  5, 2.8, 0.4, 0.2, 16., 0.,   1600., false, "5j-1600") ) countSignalEvent("SR-5j-1600");  
  if (Passes_Cuts(sigjets, 200., 75.,  6, 2.0, 0.4, 0.2, 16., 0.08, 1000., false, "6j-1000") ) countSignalEvent("SR-6j-1000");  
  if (Passes_Cuts(sigjets, 200., 75.,  6, 2.0, 0.4, 0.2, 16., 0.08, 2200., false, "6j-2200") ) countSignalEvent("SR-6j-2200");
  if (Passes_Cuts(sigjets, 200., 75.,  6, 2.0, 0.4, 0.2, 10., 0.08, 3400., false, "6j-3400") ) countSignalEvent("SR-6j-3400");  
  
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., 16., 0.,   1000., 1600., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-1000-10");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 16., 22., 0.,   1000., 1600., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-1000-16");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 22., -1., 0.,   1000., 1600., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-1000-22");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., 16., 0.,   1600., -1.0 , false, "MB-SSd") ) countSignalEvent("MB-SSd-2-1600-10");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 16., 22., 0.,   1600., 2200., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-1600-16");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 22., -1., 0.,   1600., 2200., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-1600-22");  
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 16., 22., 0.,   2200., 2800., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-2200-16");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 22., -1., 0.,   2200., 2800., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-2200-22");    
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 16., 22., 0.,   2800., -1.0 , false, "MB-SSd") ) countSignalEvent("MB-SSd-2-2800-16");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 22., -1., 0.,   2800., 3400., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-2800-22");  
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 22., 28., 0.,   3400., 4000., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-3400-22");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 28., -1., 0.,   3400., 4000., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-3400-28");    
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 22., 28., 0.,   4000., -1.0 , false, "MB-SSd") ) countSignalEvent("MB-SSd-2-4000-22");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 28., -1., 0.,   4000., -1.0 , false, "MB-SSd") ) countSignalEvent("MB-SSd-2-4000-28"); 
  
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., 16., 0.,   1000., 1600., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-1000-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 16., 22., 0.,   1000., 1600., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-1000-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 22., -1., 0.,   1000., 1600., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-1000-22");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., 16., 0.,   1600., -1.0 , false, "MB-SSd") ) countSignalEvent("MB-SSd-4-1600-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 16., 22., 0.,   1600., 2200., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-1600-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 22., -1., 0.,   1600., 2200., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-1600-22");  
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 16., 22., 0.,   2200., 2800., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-2200-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 22., -1., 0.,   2200., 2800., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-2200-22");   
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 16., 22., 0.,   2800., -1.0 , false, "MB-SSd") ) countSignalEvent("MB-SSd-4-2800-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 22., -1., 0.,   2800., 3400 , false, "MB-SSd") ) countSignalEvent("MB-SSd-4-2800-22");    
  
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 16., 0.04,   1000., 1600., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-1000-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 16., 22., 0.04,   1000., 1600., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-1000-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 22., -1., 0.04,   1000., 1600., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-1000-22");  
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 16., 0.04,   1600., 2200., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-1600-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 16., 22., 0.04,   1600., 2200., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-1600-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 22., -1., 0.04,   1600., 2200., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-1600-22");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 16., 0.04,   2200., 2800., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-2200-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 16., 22., 0.04,   2200., 2800., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-2200-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 22., -1., 0.04,   2200., 2800., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-2200-22");    
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 16., 0.04,   2800., 3400., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-2800-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 16., 22., 0.04,   2800., 3400., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-2800-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 22., -1., 0.04,   2800., 3400., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-2800-22");    
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 16., 0.04,   3400., 4000., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-3400-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 16., 22., 0.04,   3400., 4000., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-3400-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 22., -1., 0.04,   3400., 4000., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-3400-22");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 16., 0.04,   4000., -1.0 , false, "MB-GGd") ) countSignalEvent("MB-GGd-4-4000-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 16., 22., 0.04,   4000., -1.0 , false, "MB-GGd") ) countSignalEvent("MB-GGd-4-4000-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 22., -1., 0.04,   4000., -1.0 , false, "MB-GGd") ) countSignalEvent("MB-GGd-4-4000-22");  

  if (Passes_Cuts_MB(sigjets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., 22., 0.0,   1600., 2200., false, "MB-C") ) countSignalEvent("MB-C-2-1600-16");
  if (Passes_Cuts_MB(sigjets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 22., -1., 0.0,   1600., 2200., false, "MB-C") ) countSignalEvent("MB-C-2-1600-22");
  if (Passes_Cuts_MB(sigjets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., 22., 0.0,   2200., 2800., false, "MB-C") ) countSignalEvent("MB-C-2-2200-16");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 22., -1., 0.0,   2200., 2800., false, "MB-C") ) countSignalEvent("MB-C-2-2200-22");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., 22., 0.0,   2800., -1.0 , false, "MB-C") ) countSignalEvent("MB-C-2-2800-16");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 22., -1., 0.0,   2800., -1.0 , false, "MB-C") ) countSignalEvent("MB-C-2-2800-22");     
  
  if (Passes_Cuts_MB(sigjets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., 22., 0.0,   1600., 2200., false, "MB-C") ) countSignalEvent("MB-C-4-1600-16");
  if (Passes_Cuts_MB(sigjets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 22., -1., 0.0,   1600., 2200., false, "MB-C") ) countSignalEvent("MB-C-4-1600-22");
  if (Passes_Cuts_MB(sigjets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., 22., 0.0,   2200., 2800., false, "MB-C") ) countSignalEvent("MB-C-4-2200-16");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 22., -1., 0.0,   2200., 2800., false, "MB-C") ) countSignalEvent("MB-C-4-2200-22");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., 22., 0.0,   2800., -1.0 , false, "MB-C") ) countSignalEvent("MB-C-4-2800-16");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 22., -1., 0.0,   2800., -1.0 , false, "MB-C") ) countSignalEvent("MB-C-4-2800-22");  
  
  if (Passes_Cuts_MB(sigjets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., 22., 0.0,   1600., 2200., false, "MB-C") ) countSignalEvent("MB-C-5-1600-16");
  if (Passes_Cuts_MB(sigjets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 22., -1., 0.0,   1600., 2200., false, "MB-C") ) countSignalEvent("MB-C-5-1600-22");
  if (Passes_Cuts_MB(sigjets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., 22., 0.0,   2200., 2800., false, "MB-C") ) countSignalEvent("MB-C-5-2200-16");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 22., -1., 0.0,   2200., 2800., false, "MB-C") ) countSignalEvent("MB-C-5-2200-22");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., 22., 0.0,   2800., -1.0 , false, "MB-C") ) countSignalEvent("MB-C-5-2800-16");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 22., -1., 0.0,   2800., -1.0 , false, "MB-C") ) countSignalEvent("MB-C-5-2800-22");     
  
  //                         N_j , DPhi13, DPhi4, MET/Meff, Meff,  BDT
  if (Passes_Cuts_BDT(sigjets, 4,   0.4,    0.4,     0.2,   1400., 0.97, 0, false, "BDT-GGd1") ) countSignalEvent("BDT-GGd1");
  if (Passes_Cuts_BDT(sigjets, 4,   0.4,    0.4,     0.2,   1400., 0.94, 1, false, "BDT-GGd2") ) countSignalEvent("BDT-GGd2");
  if (Passes_Cuts_BDT(sigjets, 4,   0.4,    0.4,     0.2,    800., 0.94, 2, false, "BDT-GGd3") ) countSignalEvent("BDT-GGd3");
  if (Passes_Cuts_BDT(sigjets, 4,   0.4,    0.4,     0.2,    800., 0.87, 3, false, "BDT-GGd4") ) countSignalEvent("BDT-GGd4");
  if (Passes_Cuts_BDT(sigjets, 6,   0.4,    0.4,     0.2,   1400., 0.96, 4, true, "BDT-GGo1") ) countSignalEvent("BDT-GGo1");
  if (Passes_Cuts_BDT(sigjets, 6,   0.4,    0.4,     0.2,   1400., 0.87, 5, true, "BDT-GGo2") ) countSignalEvent("BDT-GGo2");
  if (Passes_Cuts_BDT(sigjets, 5,   0.4,    0.4,     0.2,    800., 0.92, 6, true, "BDT-GGo3") ) countSignalEvent("BDT-GGo3");
  if (Passes_Cuts_BDT(sigjets, 5,   0.2,    0.2,     0.2,    800., 0.84, 7, true, "BDT-GGo4") ) countSignalEvent("BDT-GGo4");  
  
  return;  
}

void Atlas_2010_14293::finalize() {
  // Whatever should be done after the run goes here
  for (int j = 0; j < 8; j++)
    for (int i = 0; i < 2; i++) 
      delete reader[j][i];  
  
  TCanvas can1;
  //ggd1->SetMinimum(0.1);
  can1.SetLogy();
  ggd1->Draw("hist");
  can1.Close();
  
  TCanvas can2;
  //ggo1->SetMinimum(0.1);
  can2.SetLogy();
  ggo1->Draw("hist");
  can2.Close();  
  
  hfile->Write();
  hfile->Close();
}  


bool Atlas_2010_14293::sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }

double Atlas_2010_14293::Aplanarity(std::vector<Jet*> input_jets) {
  
  double mag = 0.;
  for (int k = 0; k < input_jets.size(); k++)
    mag += pow(input_jets[k]->P4().Rho(),2);
  
  TMatrixD st(TMatrixD::kZero, TMatrixD(3,3) );
  for (int k = 0; k < input_jets.size(); k++) {
    st(0,0) += input_jets[k]->P4().X()*input_jets[k]->P4().X()/mag;
    st(0,1) += input_jets[k]->P4().X()*input_jets[k]->P4().Y()/mag;
    st(0,2) += input_jets[k]->P4().X()*input_jets[k]->P4().Z()/mag;
    st(1,1) += input_jets[k]->P4().Y()*input_jets[k]->P4().Y()/mag;
    st(1,2) += input_jets[k]->P4().Y()*input_jets[k]->P4().Z()/mag;
    st(2,2) += input_jets[k]->P4().Z()*input_jets[k]->P4().Z()/mag;
  }
  st(1,0) = st(0,1);
  st(2,0) = st(0,2);
  st(2,1) = st(1,2);
  
  TMatrixDEigen eigen(st);
  TMatrixD diag = eigen.GetEigenValues();
  
  std::vector<double> lambdas;
  lambdas.push_back( diag(0,0) );
  lambdas.push_back( diag(1,1) );
  lambdas.push_back( diag(2,2) );
  std::sort (lambdas.begin(), lambdas.end());
  
  return 1.5*lambdas[0];
}  

bool Atlas_2010_14293::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

double Atlas_2010_14293::check_track_pt(Jet* jet, std::vector<Track*> tracks) {
  
  double track_pt = 0.;
  
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
       if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) track_pt += (*it)->PT;
       
  return track_pt;
    
}

std::vector<Jet*> Atlas_2010_14293::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
  std::vector<Jet*> passed;
  for (std::vector<Jet*>::iterator jet = cand_jets.begin(); jet != cand_jets.end(); jet++) {
  
    if (check_nTrack_jet(*jet, tracks, nTracks)) {
      passed.push_back(*jet);
      continue;
    }
    double dR = deltaR;
    bool iso = true;
        
    for (std::vector<Muon*>::iterator mu=cand_muons.begin(); mu!=cand_muons.end(); mu++) 
      if ((*jet)->P4().DeltaR((*mu)->P4()) < dR) {
	iso = false;
	break;
      }
        		    
    if (iso) passed.push_back(*jet);
  }
  
  return passed;
}

std::vector<Jet*> Atlas_2010_14293::overlapRemoval_muon_jet_tracks2(std::vector<Jet*> cand_jets,std::vector<Muon*> cand_muons, double deltaR){
  
  std::vector<Jet*> passed;
  for (std::vector<Jet*>::iterator jet = cand_jets.begin(); jet != cand_jets.end(); jet++) {
    
    double dR = deltaR;
    bool iso = true;
        
    for (std::vector<Muon*>::iterator mu=cand_muons.begin(); mu!=cand_muons.end(); mu++) 
       if ((*jet)->P4().DeltaR((*mu)->P4()) < dR && 0.7*check_track_pt(*jet, tracks) < (*mu)->PT && (*jet)->PT < 0.5*(*mu)->PT ) {
	 iso = false;
	 break;
       }
       
     if (iso) passed.push_back(*jet);
  }
  
  return passed;
}

template <class X, class Y>
std::vector<X*> Atlas_2010_14293::specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours) {
      // If neighbours are empty, return candidates
      if(neighbours.size() == 0)
        return candidates;
      std::vector<X*> passed_candidates;
      // Loop over candidates
      for(int i = 0; i < candidates.size(); i++) {
        bool overlap = false;
        // If a neighbour is too close, declare overlap, break and don't save candidate
        for(int j = 0; j < neighbours.size(); j++) {
          if (candidates[i]->P4().DeltaR(neighbours[j]->P4()) > 0.2 and candidates[i]->P4().DeltaR(neighbours[j]->P4()) < std::min(0.4, 0.04 + 10./candidates[i]->PT) ) {
            overlap = true;
            break;
          }
        }
        if (!overlap)
          passed_candidates.push_back(candidates[i]);
      }
      return passed_candidates;
    }
    
double Atlas_2010_14293::dPhi(std::vector<Jet*> jets, int j) {

  int kmax = 0; int kmin = 0;
  if ( !j ) { 
    kmax = std::min(int(jets.size()), 3); 
    kmin = 0;     
  }
  else { 
    kmax = jets.size(); 
    kmin = 3;    
  }
  double dphimin = 10.;

  for (int k = kmin; k < kmax; k++)
      dphimin = std::min(fabs(jets[k]->P4().DeltaPhi( missingET->P4() )), dphimin);
     
  return dphimin;
}    
    
double Atlas_2010_14293::M_eff(std::vector<Jet*> jets, int j) {
  
  double Meff = met;
  int kmax = 0;
  if( !j ) kmax = jets.size(); else kmax=j; 
  
  for(int i = 0; i < kmax; i++) 
    if(jets[i]->PT > 50)  Meff+=jets[i]->PT;
  
  return Meff;
}    

double Atlas_2010_14293::HT(std::vector<Jet*> jets) {
  
  double  PTSum = 0.;
  for (int i = 0; i < jets.size(); i++) PTSum+=jets[i]->PT;
  return PTSum;
}
    
bool Atlas_2010_14293::Passes_Cuts(std::vector<Jet*> jets, double PT1Cut, double PT2Cut, int NJet, double EtaCut, double dPhiCut1, double dPhiCut2, double METHTCut, double AplanarityCut, double MeffCut, bool cutflow, std::string sr) {
  
  if ( jets.size() < NJet ) return false;
  if (cutflow) countCutflowEvent(sr+"_02_jetmulti");  
  
  if( dPhi(jets, 0) < dPhiCut1 ) return false;
  if (cutflow) countCutflowEvent(sr+"_03_dPhilow");  
  
  if( dPhi(jets, 1) < dPhiCut2 ) return false;
  if (cutflow) countCutflowEvent(sr+"_04_dPhihigh");    
  
  if ( jets[0]->PT < PT1Cut || jets[NJet-1]->PT < PT2Cut ) return false;
  
  if (cutflow) countCutflowEvent(sr+"_05_PTjets");
  
  for (int i = 0; i < NJet; i++)
    if ( fabs(jets[i]->Eta) > EtaCut ) return false;
    
  if (cutflow) countCutflowEvent(sr+"_06_Etajets");  
  
//  if ( !Aplanarity_Cut(jets, AplanarityCut) ) return false;
  if ( AplanarityCut > 0. and  aplanarity(jets) < AplanarityCut ) return false;
  if (cutflow) countCutflowEvent(sr+"_07_aplanarity");  
  
  if( met/sqrt( HT(jets) ) < METHTCut ) return false;
  
  if (cutflow) countCutflowEvent(sr+"_08_METrelative");  
  
  if( M_eff(jets, 0) < MeffCut) return false;
  if (cutflow) countCutflowEvent(sr+"_09_Meff");  
  
  return true;
}


bool Atlas_2010_14293::Passes_Cuts_MB(std::vector<Jet*> jets, double PT1Cut, double PT2Cut, int NJetMin, int NJetMax, double EtaCut, double dPhiCut1, double dPhiCut2, double METHTMin, double METHTMax, double AplanarityCut, double MeffMin, double MeffMax, bool cutflow, std::string sr) {
  
  if ( jets.size() < NJetMin ) return false;
  if ( NJetMax and jets.size() > NJetMax ) return false;
  if (cutflow) countCutflowEvent(sr+"_02_jetmulti");  
  
  if( dPhi(jets, 0) < dPhiCut1 ) return false;
  if (cutflow) countCutflowEvent(sr+"_03_dPhilow");  
  
  if( dPhi(jets, 1) < dPhiCut2 ) return false;
  if (cutflow) countCutflowEvent(sr+"_04_dPhihigh");    
  
  if ( jets[0]->PT < PT1Cut || jets[NJetMin-1]->PT < PT2Cut ) return false;
  
  if (cutflow) countCutflowEvent(sr+"_05_PTjets");
  
  for (int i = 0; i < NJetMin; i++)
    if ( fabs(jets[i]->Eta) > EtaCut ) return false;
    
  if (cutflow) countCutflowEvent(sr+"_06_Etajets");  
  
//  if ( !Aplanarity_Cut(jets, AplanarityCut) ) return false;
  if ( AplanarityCut > 0. and  aplanarity(jets) < AplanarityCut ) return false;
  if (cutflow) countCutflowEvent(sr+"_07_aplanarity");  
  
  if( met/sqrt( HT(jets) ) < METHTMin ) return false;
  if( METHTMax > 0. and met/sqrt( HT(jets) ) > METHTMax ) return false;
  
  if (cutflow) countCutflowEvent(sr+"_08_METrelative");  
  
  if( M_eff(jets, 0) < MeffMin) return false;
  if( MeffMax > 0. and M_eff(jets, 0) > MeffMax) return false;
  if (cutflow) countCutflowEvent(sr+"_09_Meff");  
  
  return true;
}


bool Atlas_2010_14293::Passes_Cuts_BDT(std::vector<Jet*> jets, int NJetMin, double dPhiCut1, double dPhiCut2, double METMeff, double MeffMin, double BDTscore, int rder, bool cutflow, std::string sr) {
  
  if ( jets.size() < NJetMin ) return false;  
  if (cutflow) countCutflowEvent(sr+"_02_jetmulti");  
  
  if( dPhi(jets, 0) < dPhiCut1 ) return false;
  if (cutflow) countCutflowEvent(sr+"_03_dPhilow");  
  
  if( dPhi(jets, 1) < dPhiCut2 ) return false;
  if (cutflow) countCutflowEvent(sr+"_04_dPhihigh");    

  met_aux = met;
  meff_aux = M_eff(jets, NJetMin);  
  
  if ( met_aux/meff_aux < METMeff ) return false;
  if (cutflow) countCutflowEvent(sr+"_05_METMeff");
  
  meff_aux = M_eff(jets, 0); 
  if( meff_aux < MeffMin) return false;
  if (cutflow) countCutflowEvent(sr+"_06_Meff");    

  Ap = aplanarity(jets);
  jetPt0 = jets[0]->PT;
  jetPt1 = jets[1]->PT;
  jetPt2 = jets[2]->PT;
  jetPt3 = jets[3]->PT;
  jetEta0 = jets[0]->Eta;
  jetEta1 = jets[1]->Eta;
  jetEta2 = jets[2]->Eta;
  jetEta3 = jets[3]->Eta;
  if( jets.size() > 4 ) {
    jetPt4 = jets[4]->PT;
    jetEta4 = jets[4]->Eta;
  } else {
    jetPt4 = 0.;
    jetEta4 = 0.;
  }
  if( jets.size() > 5 ) {
    jetPt5 = jets[5]->PT;
    jetEta5 = jets[5]->Eta;
  } else {
    jetPt5 = 0.; //needed for GGo3; communication with ATLAS authors
    jetEta5 = 0.;
  }  

  
  double bdt = (rand()/(RAND_MAX+1.) < 0.5) ? reader[rder][0]->EvaluateMVA( "BDT" ) : reader[rder][1]->EvaluateMVA( "BDT" );; 
  
  if (sr == "BDT-GGd1") ggd1->Fill( bdt, weight);
  if (sr == "BDT-GGo1") ggo1->Fill( bdt, weight);
  
  if (bdt < BDTscore) return false;
  if (cutflow) countCutflowEvent(sr+"_07_BDT");    
  
  return true;
}

void Atlas_2010_14293::evaluateCRs(std::vector<Jet*> jets, std::string type) {
  
  if (Passes_Cuts_MB(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., -1., 0.,   1000., 1600., false, "CRY-2") ) countControlEvent("MB-SSd-"+type+"_2_1000_cuts");
  if (Passes_Cuts_MB(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., -1., 0.,   1600., 2200., false, "CRY-2") ) countControlEvent("MB-SSd-"+type+"_2_1600_cuts"); 
  if (Passes_Cuts_MB(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., -1., 0.,   2200., 2800., false, "CRY-2") ) countControlEvent("MB-SSd-"+type+"_2_2200_cuts");
  if (Passes_Cuts_MB(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., -1., 0.,   2800., 3400., false, "CRY-2") ) countControlEvent("MB-SSd-"+type+"_2_2800_cuts");  
  if (Passes_Cuts_MB(jets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 10., -1., 0.,   3400., 4000., false, "CRY-2") ) countControlEvent("MB-SSd-"+type+"_2_3400_cuts");  
  if (Passes_Cuts_MB(jets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 10., -1., 0.,   4000., -1.0 , false, "CRY-2") ) countControlEvent("MB-SSd-"+type+"_2_4000_cuts"); 
  
  if (Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., -1., 0.,   1000., 1600., false, "CRY-4") ) countControlEvent("MB-SSd-"+type+"_4_1000_cuts");
  if (Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., -1., 0.,   1600., 2200., false, "CRY-4") ) countControlEvent("MB-SSd-"+type+"_4_1600_cuts");
  if (Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., -1., 0.,   2200., 2800., false, "CRY-4") ) countControlEvent("MB-SSd-"+type+"_4_2200_cuts");
  if (Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., -1., 0.,   2800., -1.0 , false, "CRY-4") ) countControlEvent("MB-SSd-"+type+"_4_2800_cuts");
  
  if (Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., -1., 0.04,   1000., 1600., false, "CRY-4") ) countControlEvent("MB-GGd-"+type+"_4_1000_cuts");
  if (Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., -1., 0.04,   1600., 2200., false, "CRY-4") ) countControlEvent("MB-GGd-"+type+"_4_1600_cuts");
  if (Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., -1., 0.04,   2200., 2800., false, "CRY-4") ) countControlEvent("MB-GGd-"+type+"_4_2200_cuts");
  if (Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., -1., 0.04,   2800., 3400., false, "CRY-4") ) countControlEvent("MB-GGd-"+type+"_4_2800_cuts");
  if (Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., -1., 0.04,   3400., 4000., false, "CRY-4") ) countControlEvent("MB-GGd-"+type+"_4_3400_cuts");
  if (Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., -1., 0.04,   4000., -1.0 , false, "CRY-4") ) countControlEvent("MB-GGd-"+type+"_4_4000_cuts");

  if (Passes_Cuts_MB(jets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., -1., 0.0,   1600., 2200., false, "CRY-4") ) countControlEvent("MB-C-"+type+"_2_1600_cuts");
  if (Passes_Cuts_MB(jets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., -1., 0.0,   2200., 2800., false, "CRY-4") ) countControlEvent("MB-C-"+type+"_2_2200_cuts");    
  if (Passes_Cuts_MB(jets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., -1., 0.0,   2800., -1.0 , false, "CRY-4") ) countControlEvent("MB-C-"+type+"_2_2800_cuts");       
  
  if (Passes_Cuts_MB(jets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., -1., 0.0,   1600., 2200., false, "CRY-4") ) countControlEvent("MB-C-"+type+"_4_1600_cuts");
  if (Passes_Cuts_MB(jets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., -1., 0.0,   2200., 2800., false, "CRY-4") ) countControlEvent("MB-C-"+type+"_4_2200_cuts");    
  if (Passes_Cuts_MB(jets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., -1., 0.0,   2800., -1.0 , false, "CRY-4") ) countControlEvent("MB-C-"+type+"_4_2800_cuts");    
  
  if (Passes_Cuts_MB(jets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., -1., 0.0,   1600., 2200., false, "CRY-4") ) countControlEvent("MB-C-"+type+"_5_1600_cuts");
  if (Passes_Cuts_MB(jets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., -1., 0.0,   2200., 2800., false, "CRY-4") ) countControlEvent("MB-C-"+type+"_5_2200_cuts");        
  if (Passes_Cuts_MB(jets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., -1., 0.0,   2800., -1.0 , false, "CRY-4") ) countControlEvent("MB-C-"+type+"_5_2800_cuts");    
  
  return;
  
}

bool Atlas_2010_14293::Passes_Cuts_CRQ(std::vector<Jet*> jets, double PT1Cut, double PT2Cut, int NJetMin, int NJetMax, double EtaCut, double dPhiCut1, double dPhiCut2, double METHTMax, double AplanarityCut, double MeffMin, double MeffMax, bool cutflow, std::string sr) {
  
  if ( jets.size() < NJetMin ) return false;
  if ( NJetMax and jets.size() > NJetMax ) return false;
  if (cutflow) countCutflowEvent(sr+"_02_jetmulti");  
  
  if( dPhi(jets, 0) > dPhiCut1 ) return false; // reversed condition
  if (cutflow) countCutflowEvent(sr+"_03_dPhilow");  
  
  if( dPhi(jets, 1) > dPhiCut2 ) return false; // reversed condition
  if (cutflow) countCutflowEvent(sr+"_04_dPhihigh");    
  
  if ( jets[0]->PT < PT1Cut || jets[NJetMin-1]->PT < PT2Cut ) return false;
  
  if (cutflow) countCutflowEvent(sr+"_05_PTjets");
  
  for (int i = 0; i < NJetMin; i++)
    if ( fabs(jets[i]->Eta) > EtaCut ) return false;
    
  if (cutflow) countCutflowEvent(sr+"_06_Etajets");  
  
//  if ( !Aplanarity_Cut(jets, AplanarityCut) ) return false;
  if ( AplanarityCut > 0. and  aplanarity(jets) < AplanarityCut ) return false;
  if (cutflow) countCutflowEvent(sr+"_07_aplanarity");  
  
  if( missingET->P4().Et()/sqrt( HT(jets) ) > METHTMax ) return false; // reversed condition (preselection)
  
  if (cutflow) countCutflowEvent(sr+"_08_METrelative");  
  
  if( M_eff(jets, 0) < MeffMin) return false;
  if( MeffMax > 0. and M_eff(jets, 0) > MeffMax) return false;
  if (cutflow) countCutflowEvent(sr+"_09_Meff");  
  
  return true;
}

void Atlas_2010_14293::evaluateCRQs(std::vector<Jet*> jets) {
  
  if (Passes_Cuts_CRQ(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., 0.,   1000., 1600. , false, "CRQ-SSd") ) countControlEvent("MB-SSd-CRQ_2_1000_cuts");
  if (Passes_Cuts_CRQ(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., 0.,   1600., 2200. , false, "CRQ-SSd") ) countControlEvent("MB-SSd-CRQ_2_1600_cuts");
  if (Passes_Cuts_CRQ(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., 0.,   2200., 2800. , false, "CRQ-SSd") ) countControlEvent("MB-SSd-CRQ_2_2200_cuts");
  if (Passes_Cuts_CRQ(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., 0.,   2800., 3400. , false, "CRQ-SSd") ) countControlEvent("MB-SSd-CRQ_2_2800_cuts");
  if (Passes_Cuts_CRQ(jets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 10., 0.,   3400., 4000. , false, "CRQ-SSd") ) countControlEvent("MB-SSd-CRQ_2_3400_cuts");
  if (Passes_Cuts_CRQ(jets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 10., 0.,   4000., -1.0  , false, "CRQ-SSd") ) countControlEvent("MB-SSd-CRQ_2_4000_cuts");
  
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., 0.,   1000., 1600. , false, "CRQ-SSd") ) countControlEvent("MB-SSd-CRQ_4_1000_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., 0.,   1600., 2200. , false, "CRQ-SSd") ) countControlEvent("MB-SSd-CRQ_4_1600_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., 0.,   2200., 2800. , false, "CRQ-SSd") ) countControlEvent("MB-SSd-CRQ_4_2200_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., 0.,   2800., -1.0  , false, "CRQ-SSd") ) countControlEvent("MB-SSd-CRQ_4_2800_cuts");  
  
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 0.04,   1000., 1600., false, "CRQ-GGd") ) countControlEvent("MB-GGd-CRQ_4_1000_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 0.04,   1600., 2200., false, "CRQ-GGd") ) countControlEvent("MB-GGd-CRQ_4_1600_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 0.04,   2200., 2800., false, "CRQ-GGd") ) countControlEvent("MB-GGd-CRQ_4_2200_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 0.04,   2800., 3400., false, "CRQ-GGd") ) countControlEvent("MB-GGd-CRQ_4_2800_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 0.04,   3400., 4000., false, "CRQ-GGd") ) countControlEvent("MB-GGd-CRQ_4_3400_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 0.04,   4000., -1.0 , false, "CRQ-GGd") ) countControlEvent("MB-GGd-CRQ_4_4000_cuts");
  
  if (Passes_Cuts_CRQ(jets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., 0.0,   1600., 2200., false, "CRQ-C") ) countControlEvent("MB-C-CRQ_2_1600_cuts");
  if (Passes_Cuts_CRQ(jets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., 0.0,   2200., 2800., false, "CRQ-C") ) countControlEvent("MB-C-CRQ_2_2200_cuts");     
  if (Passes_Cuts_CRQ(jets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., 0.0,   2800., -1.0 , false, "CRQ-C") ) countControlEvent("MB-C-CRQ_2_2800_cuts");     
  
  if (Passes_Cuts_CRQ(jets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., 0.0,   1600., 2200., false, "CRQ-C") ) countControlEvent("MB-C-CRQ_4_1600_cuts");
  if (Passes_Cuts_CRQ(jets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., 0.0,   2200., 2800., false, "CRQ-C") ) countControlEvent("MB-C-CRQ_4_2200_cuts");      
  if (Passes_Cuts_CRQ(jets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., 0.0,   2800., -1.0 , false, "CRQ-C") ) countControlEvent("MB-C-CRQ_4_2800_cuts");    
  
  if (Passes_Cuts_CRQ(jets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., 0.0,   1600., 2200., false, "CRQ-C") ) countControlEvent("MB-C-CRQ_5_1600_cuts");
  if (Passes_Cuts_CRQ(jets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., 0.0,   2200., 2800., false, "CRQ-C") ) countControlEvent("MB-C-CRQ_5_2200_cuts");    
  if (Passes_Cuts_CRQ(jets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., 0.0,   2800., -1.0 , false, "CRQ-C") ) countControlEvent("MB-C-CRQ_5_2800_cuts");      
  
}
