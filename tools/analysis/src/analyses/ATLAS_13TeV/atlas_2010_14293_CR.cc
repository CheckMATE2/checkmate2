#include "atlas_2010_14293.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_2010_14293_CR::initialize() {
  setInformation(""
    "# search for squarks and gluinos in MET_jet final states\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  setAnalysisName("atlas_2010_14293_CR");
  // You should initialize any declared variables here
}

void Atlas_2010_14293_CR::analyze() {
  
  missingET->addMuons(muonsCombined);  
  
  electronsLoose = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 7., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 6., -2.7, 2.7);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
  photonsMedium = filterPhaseSpace(photonsMedium, 150., -2.37, 2.37, true);
  std::vector<Photon*> CRphotons = filterIsolation(photonsMedium);
  
  jets = overlapRemoval(jets, electronsLoose, 0.2, "y");
  electronsLoose = main.specialoverlap(electronsLoose, jets);
  electronsTight = main.specialoverlap(electronsTight, jets);
  muonsCombined = main.specialoverlap(muonsCombined, jets);
  jets = main.overlapRemoval_muon_jet_tracks(jets, muonsCombined, 0.2, 2); 
  
  std::vector<Electron*> CRelectrons = Isolate_leptons_with_inverse_track_isolation_cone(electronsTight, tracks, towers, 0.2, 10., 0.2, 0.06, 0.06, true);
  std::vector<Muon*> CRmuons = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined, tracks, towers, 0.3, 10., 0.2, 0.15, 0.3, true);
  
  std::vector<Jet*> CRjets;
  if (CRphotons.size()) {
    CRjets = overlapRemoval(jets, photonsMedium, 0.4);
    CRjets = filterPhaseSpace(CRjets, 50., -2.8, 2.8);
  }
  Jet *newjet;
  
  std::vector<Jet*> sigjets = filterPhaseSpace(jets, 50., -2.8, 2.8);
  met = 0;
  
  if ( CRjets.size() > 1 and CRphotons.size() == 1 and electronsLoose.size() == 0 and muonsCombined.size() == 0 and (missingET->P4() + CRphotons[0]->P4()).Perp() > 300. and main.M_eff(CRjets, 0) > 800. )  {
      met = (missingET->P4() + CRphotons[0]->P4()).Perp();  // photon treated as MET for evaluation of Z CR
      evaluateCRs(CRjets, "CRY");
  }
  else if (CRmuons.size() + CRelectrons.size() == 1 and missingET->P4().Et() > 300. ) {
    
    met = missingET->P4().Et();
    
    if (CRmuons.size() ) newjet->P4() = CRmuons[0]->P4();
    else if (CRelectrons.size() ) newjet->P4() = CRelectrons[0]->P4();
    
    bool btag = false;
    for (int i = 0; i < sigjets.size(); i++) 
      if ( fabs(sigjets[i]->Eta) < 2.5 && checkBTag(sigjets[i]) )  {btag = true; break;}
    
    CRjets = sigjets;
    CRjets.push_back(newjet);
    std::sort(CRjets.begin(), CRjets.end(), main.sortByPT ); 
    CRjets = filterPhaseSpace(CRjets, 50., -2.8, 2.8);
    
    if ( CRjets.size() < 2 or CRjets[0]->PT < 200. or main.M_eff(CRjets, 0) < 800. or missingET->P4().Et() > 300.) return;
      
    if (btag) evaluateCRs(CRjets, "CRT");
    else evaluateCRs(CRjets, "CRW");
    
  }
  else if ( sigjets.size() > 1 and sigjets[0]->PT > 200. and electronsLoose.size() == 0 and muonsCombined.size() == 0 and main.M_eff(sigjets, 0) > 800. and missingET->P4().Et() > 300. ) {
    
    met = missingET->P4().Et();
    evaluateCRQs(sigjets); 
    
  }
  
  return;
  
}

void Atlas_2010_14293_CR::finalize() {
  // Whatever should be done after the run goes here
}   


void Atlas_2010_14293_CR::evaluateCRs(std::vector<Jet*> jets, std::string type) {
  
  if (main.Passes_Cuts_MB(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., -1., 0.,   1000., 1600., false, "CRY-2") ) countSignalEvent("MB-SSd-"+type+"_2_1000_cuts");
  if (main.Passes_Cuts_MB(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., -1., 0.,   1600., 2200., false, "CRY-2") ) countSignalEvent("MB-SSd-"+type+"_2_1600_cuts"); 
  if (main.Passes_Cuts_MB(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., -1., 0.,   2200., 2800., false, "CRY-2") ) countSignalEvent("MB-SSd-"+type+"_2_2200_cuts");
  if (main.Passes_Cuts_MB(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., -1., 0.,   2800., 3400., false, "CRY-2") ) countSignalEvent("MB-SSd-"+type+"_2_2800_cuts");  
  if (main.Passes_Cuts_MB(jets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 10., -1., 0.,   3400., 4000., false, "CRY-2") ) countSignalEvent("MB-SSd-"+type+"_2_3400_cuts");  
  if (main.Passes_Cuts_MB(jets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 10., -1., 0.,   4000., -1.0 , false, "CRY-2") ) countSignalEvent("MB-SSd-"+type+"_2_4000_cuts"); 
  
  if (main.Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., -1., 0.,   1000., 1600., false, "CRY-4") ) countSignalEvent("MB-SSd-"+type+"_4_1000_cuts");
  if (main.Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., -1., 0.,   1600., 2200., false, "CRY-4") ) countSignalEvent("MB-SSd-"+type+"_4_1600_cuts");
  if (main.Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., -1., 0.,   2200., 2800., false, "CRY-4") ) countSignalEvent("MB-SSd-"+type+"_4_2200_cuts");
  if (main.Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., -1., 0.,   2800., -1.0 , false, "CRY-4") ) countSignalEvent("MB-SSd-"+type+"_4_2800_cuts");
  
  if (main.Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., -1., 0.04,   1000., 1600., false, "CRY-4") ) countSignalEvent("MB-GGd-"+type+"_4_1000_cuts");
  if (main.Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., -1., 0.04,   1600., 2200., false, "CRY-4") ) countSignalEvent("MB-GGd-"+type+"_4_1600_cuts");
  if (main.Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., -1., 0.04,   2200., 2800., false, "CRY-4") ) countSignalEvent("MB-GGd-"+type+"_4_2200_cuts");
  if (main.Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., -1., 0.04,   2800., 3400., false, "CRY-4") ) countSignalEvent("MB-GGd-"+type+"_4_2800_cuts");
  if (main.Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., -1., 0.04,   3400., 4000., false, "CRY-4") ) countSignalEvent("MB-GGd-"+type+"_4_3400_cuts");
  if (main.Passes_Cuts_MB(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., -1., 0.04,   4000., -1.0 , false, "CRY-4") ) countSignalEvent("MB-GGd-"+type+"_4_4000_cuts");

  if (main.Passes_Cuts_MB(jets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., -1., 0.0,   1600., 2200., false, "CRY-4") ) countSignalEvent("MB-C-"+type+"_2_1600_cuts");
  if (main.Passes_Cuts_MB(jets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., -1., 0.0,   2200., 2800., false, "CRY-4") ) countSignalEvent("MB-C-"+type+"_2_2200_cuts");    
  if (main.Passes_Cuts_MB(jets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., -1., 0.0,   2800., -1.0 , false, "CRY-4") ) countSignalEvent("MB-C-"+type+"_2_2800_cuts");       
  
  if (main.Passes_Cuts_MB(jets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., -1., 0.0,   1600., 2200., false, "CRY-4") ) countSignalEvent("MB-C-"+type+"_4_1600_cuts");
  if (main.Passes_Cuts_MB(jets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., -1., 0.0,   2200., 2800., false, "CRY-4") ) countSignalEvent("MB-C-"+type+"_4_2200_cuts");    
  if (main.Passes_Cuts_MB(jets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., -1., 0.0,   2800., -1.0 , false, "CRY-4") ) countSignalEvent("MB-C-"+type+"_4_2800_cuts");    
  
  if (main.Passes_Cuts_MB(jets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., -1., 0.0,   1600., 2200., false, "CRY-4") ) countSignalEvent("MB-C-"+type+"_5_1600_cuts");
  if (main.Passes_Cuts_MB(jets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., -1., 0.0,   2200., 2800., false, "CRY-4") ) countSignalEvent("MB-C-"+type+"_5_2200_cuts");        
  if (main.Passes_Cuts_MB(jets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., -1., 0.0,   2800., -1.0 , false, "CRY-4") ) countSignalEvent("MB-C-"+type+"_5_2800_cuts");    
  
  return;
  
}

bool Atlas_2010_14293_CR::Passes_Cuts_CRQ(std::vector<Jet*> jets, double PT1Cut, double PT2Cut, int NJetMin, int NJetMax, double EtaCut, double dPhiCut1, double dPhiCut2, double METHTMax, double AplanarityCut, double MeffMin, double MeffMax, bool cutflow, std::string sr) {
  
  if ( jets.size() < NJetMin ) return false;
  if ( NJetMax and jets.size() > NJetMax ) return false;
  if (cutflow) countCutflowEvent(sr+"_02_jetmulti");  
  
  if( main.dPhi(jets, 0) > dPhiCut1 ) return false; // reversed condition
  if (cutflow) countCutflowEvent(sr+"_03_dPhilow");  
  
  if( main.dPhi(jets, 1) > dPhiCut2 ) return false; // reversed condition
  if (cutflow) countCutflowEvent(sr+"_04_dPhihigh");    
  
  if ( jets[0]->PT < PT1Cut || jets[NJetMin-1]->PT < PT2Cut ) return false;
  
  if (cutflow) countCutflowEvent(sr+"_05_PTjets");
  
  for (int i = 0; i < NJetMin; i++)
    if ( fabs(jets[i]->Eta) > EtaCut ) return false;
    
  if (cutflow) countCutflowEvent(sr+"_06_Etajets");  
  
//  if ( !Aplanarity_Cut(jets, AplanarityCut) ) return false;
  if ( AplanarityCut > 0. and  main.aplanarity(jets) < AplanarityCut ) return false;
  if (cutflow) countCutflowEvent(sr+"_07_aplanarity");  
  
  if( missingET->P4().Et()/sqrt( main.HT(jets) ) > METHTMax ) return false; // reversed condition (preselection)
  
  if (cutflow) countCutflowEvent(sr+"_08_METrelative");  
  
  if( main.M_eff(jets, 0) < MeffMin) return false;
  if( MeffMax > 0. and main.M_eff(jets, 0) > MeffMax) return false;
  if (cutflow) countCutflowEvent(sr+"_09_Meff");  
  
  return true;
}

void Atlas_2010_14293_CR::evaluateCRQs(std::vector<Jet*> jets) {
  
  if (Passes_Cuts_CRQ(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., 0.,   1000., 1600. , false, "CRQ-SSd") ) countSignalEvent("MB-SSd-CRQ_2_1000_cuts");
  if (Passes_Cuts_CRQ(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., 0.,   1600., 2200. , false, "CRQ-SSd") ) countSignalEvent("MB-SSd-CRQ_2_1600_cuts");
  if (Passes_Cuts_CRQ(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., 0.,   2200., 2800. , false, "CRQ-SSd") ) countSignalEvent("MB-SSd-CRQ_2_2200_cuts");
  if (Passes_Cuts_CRQ(jets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., 0.,   2800., 3400. , false, "CRQ-SSd") ) countSignalEvent("MB-SSd-CRQ_2_2800_cuts");
  if (Passes_Cuts_CRQ(jets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 10., 0.,   3400., 4000. , false, "CRQ-SSd") ) countSignalEvent("MB-SSd-CRQ_2_3400_cuts");
  if (Passes_Cuts_CRQ(jets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 10., 0.,   4000., -1.0  , false, "CRQ-SSd") ) countSignalEvent("MB-SSd-CRQ_2_4000_cuts");
  
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., 0.,   1000., 1600. , false, "CRQ-SSd") ) countSignalEvent("MB-SSd-CRQ_4_1000_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., 0.,   1600., 2200. , false, "CRQ-SSd") ) countSignalEvent("MB-SSd-CRQ_4_1600_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., 0.,   2200., 2800. , false, "CRQ-SSd") ) countSignalEvent("MB-SSd-CRQ_2_2200_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., 0.,   2800., -1.0  , false, "CRQ-SSd") ) countSignalEvent("MB-SSd-CRQ_2_2800_cuts");  
  
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 0.04,   1000., 1600., false, "CRQ-GGd") ) countSignalEvent("MB-GGd-CRQ_4_1000_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 0.04,   1600., 2200., false, "CRQ-GGd") ) countSignalEvent("MB-GGd-CRQ_4_1600_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 0.04,   2200., 2800., false, "CRQ-GGd") ) countSignalEvent("MB-GGd-CRQ_4_2200_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 0.04,   2800., 3400., false, "CRQ-GGd") ) countSignalEvent("MB-GGd-CRQ_4_2800_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 0.04,   3400., 4000., false, "CRQ-GGd") ) countSignalEvent("MB-GGd-CRQ_4_3400_cuts");
  if (Passes_Cuts_CRQ(jets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 0.04,   4000., -1.0 , false, "CRQ-GGd") ) countSignalEvent("MB-GGd-CRQ_4_4000_cuts");
  
  if (Passes_Cuts_CRQ(jets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., 0.0,   1600., 2200., false, "CRQ-C") ) countSignalEvent("MB-C-CRQ_2_1600_cuts");
  if (Passes_Cuts_CRQ(jets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., 0.0,   2200., 2800., false, "CRQ-C") ) countSignalEvent("MB-C-CRQ_2_2200_cuts");     
  if (Passes_Cuts_CRQ(jets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., 0.0,   2800., -1.0 , false, "CRQ-C") ) countSignalEvent("MB-C-CRQ_2_2800_cuts");     
  
  if (Passes_Cuts_CRQ(jets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., 0.0,   1600., 2200., false, "CRQ-C") ) countSignalEvent("MB-C-CRQ_4_1600_cuts");
  if (Passes_Cuts_CRQ(jets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., 0.0,   2200., 2800., false, "CRQ-C") ) countSignalEvent("MB-C-CRQ_4_2200_cuts");      
  if (Passes_Cuts_CRQ(jets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., 0.0,   2800., -1.0 , false, "CRQ-C") ) countSignalEvent("MB-C-CRQ_4_2800_cuts");    
  
  if (Passes_Cuts_CRQ(jets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., 0.0,   1600., 2200., false, "CRQ-C") ) countSignalEvent("MB-C-CRQ_5_1600_cuts");
  if (Passes_Cuts_CRQ(jets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., 0.0,   2200., 2800., false, "CRQ-C") ) countSignalEvent("MB-C-CRQ_5_2200_cuts");    
  if (Passes_Cuts_CRQ(jets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., 0.0,   2800., -1.0 , false, "CRQ-C") ) countSignalEvent("MB-C-CRQ_5_2800_cuts");    
  
}

