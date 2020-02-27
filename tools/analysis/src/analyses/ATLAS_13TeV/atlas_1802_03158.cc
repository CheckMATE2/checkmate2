#include "atlas_1802_03158.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1802_03158::initialize() {
  setAnalysisName("atlas_1802_03158");          
  setInformation(""
    "# search for GMSB with photons\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("SRaaSL;SRaaSH;SRaaWL;SRaaWH;SRajL;SRajL200;SRajH");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1802_03158::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  jets = filterPhaseSpace(jets, 30., -2.8, 2.8);  
  electronsTight = filterPhaseSpace(electronsTight, 25., -2.47, 2.47, true);
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 25., -2.7, 2.7);

  photonsMedium = filterPhaseSpace(photonsMedium, 25., -2.37, 2.37, true);
  
  jets = overlapRemoval(jets, electronsTight, 0.2, "y"); 
  jets = overlapRemoval(jets, photonsMedium, 0.2, "y");
  electronsTight = overlapRemoval(electronsTight, jets, 0.4, "y");
  photonsMedium = overlapRemoval(photonsMedium, jets, 0.4, "y");
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4, "y");
  
  electronsTight = filterIsolation(electronsTight);
  muonsCombined = filterIsolation(muonsCombined);  
  photonsMedium = filterIsolation(photonsMedium);
  
  countCutflowEvent("00_all");
  
  if ( photonsMedium.size() > 0 and (electronsTight.size() + muonsCombinedPlus.size() == 0) ) {
    countCutflowEvent("SRaj_01_1photon");
    if (SR_single_a(photonsMedium, jets, 145., 5, 300., 2000., 0.9, true, "SRajL") ) countSignalEvent("SRajL");
    if (SR_single_a(photonsMedium, jets, 145., 5, 200., 2000., 0.9, true, "SRajL200") ) countSignalEvent("SRajL200");
    if (SR_single_a(photonsMedium, jets,  145., 3, 400., 2400., 1.0, true, "SRajH") ) countSignalEvent("SRajH");    
  }
  
  if ( photonsMedium.size() > 1 ) {
    countCutflowEvent("SRaa_01_2photon");
    HT = 0.;
    for (int i = 0; i < muonsCombined.size(); i++ ) HT += muonsCombined[i]->PT;
    for (int i = 0; i < electronsTight.size(); i++ ) HT += electronsTight[i]->PT;
    if (SR_double_a(photonsMedium, jets, 75., 150., 2750., 0., true, "SRaaSL") ) countSignalEvent("SRaaSL");
    if (SR_double_a(photonsMedium, jets, 75., 250., 2000., 0.5, true, "SRaaSH") ) countSignalEvent("SRaaSH");
    if (SR_double_a(photonsMedium, jets,  75., 150., 1500., 0., true, "SRaaWL") ) countSignalEvent("SRaaWL");    
    if (SR_double_a(photonsMedium, jets,  75., 250., 1000., 0.5, true, "SRaaWH") ) countSignalEvent("SRaaWH");    
  }  
  
  return;
  
}

void Atlas_1802_03158::finalize() {
  // Whatever should be done after the run goes here
}       


bool Atlas_1802_03158::SR_single_a(std::vector<Photon*> photons, std::vector<Jet*> jets, double PTa1Cut, int NJet, double METCut, double MeffCut, double R4TCut, bool cutflow, std::string sr) {
  
  jets = filterPhaseSpace(jets, 30., -2.5, 2.5);
  
  if ( photons[0]->PT < PTa1Cut ) return false;
  if ( photons.size() > 1 and photons[1]->PT > PTa1Cut ) return false;
  if (cutflow) countCutflowEvent(sr+"_02_photon_pt");  
  
  if ( jets.size() < NJet ) return false;
  if (cutflow) countCutflowEvent(sr+"_03_njets");  
 
  double met = missingET->P4().Et();
  if (met < METCut) return false;
  if (cutflow) countCutflowEvent(sr+"_04_MET");  
  
  double meff = met;
  for (int i = 0; i < jets.size(); i++ ) meff += jets[i]->PT;
  for (int i = 0; i < photons.size(); i++ ) meff += photons[i]->PT;
  
  if ( meff < MeffCut ) return false;
  if (cutflow) countCutflowEvent(sr+"_05_Meff");  
  
  if ( NJet > 3) {
    double jet_4 = 0.;
    for (int i = 0; i < 4; i++ ) jet_4 += jets[i]->PT;
    double jet_all = jet_4;
    for (int i = 4; i < jets.size(); i++ ) jet_all += jets[i]->PT;
    if (jet_4/jet_all > R4TCut) return false;
  }
  if (cutflow) countCutflowEvent(sr+"_06_R4T");  
  
  for (int i = 0; i < jets.size(); i++ )
    if ( fabs(jets[i]->P4().DeltaPhi( missingET->P4() )) < 0.4 ) return false;
  if (cutflow) countCutflowEvent(sr+"_07_dphi_jet");    
    
  if ( fabs(photons[0]->P4().DeltaPhi( missingET->P4() )) < 0.4 ) return false;
  if (cutflow) countCutflowEvent(sr+"_08_dphi_photon");  
  
  return true;
  
}

bool Atlas_1802_03158::SR_double_a(std::vector<Photon*> photons, std::vector<Jet*> jets, double PTa1Cut, double METCut, double HTCut, double dPhiCut, bool cutflow, std::string sr) {
  
  if ( photons[1]->PT < PTa1Cut ) return false;
  if (cutflow) countCutflowEvent(sr+"_02_photon_pt");  
 
  double met = missingET->P4().Et();
  if (met < METCut) return false;
  if (cutflow) countCutflowEvent(sr+"_03_MET");  
  
  for (int i = 0; i < jets.size(); i++ ) HT += jets[i]->PT;
  for (int i = 0; i < photons.size(); i++ ) HT += photons[i]->PT;
  
  if ( HT < HTCut ) return false;
  if (cutflow) countCutflowEvent(sr+"_04_HT");  
  
  for (int i = 0; i < jets.size() and jets[i]->PT > 75. ; i++ )
    if ( fabs(jets[i]->P4().DeltaPhi( missingET->P4() )) < 0.5 ) return false;
  if (cutflow) countCutflowEvent(sr+"_05_dphi_jet");    
    
  for (int i = 0; i < 2; i++ )
    if ( fabs(photons[i]->P4().DeltaPhi( missingET->P4() )) < dPhiCut ) return false;
  if (cutflow) countCutflowEvent(sr+"_06_dphi_photon");  
  
  return true;
  
}