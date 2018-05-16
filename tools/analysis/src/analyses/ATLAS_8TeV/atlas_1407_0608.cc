#include "atlas_1407_0608.h"

void Atlas_1407_0608::initialize() {
  setAnalysisName("atlas_1407_0608");          
  setInformation(""
    "@#search fro stops in monojet channel and charm jet\n"
     "@#targets two-body decay to charm and 4-body decays\n"
     "@#charm SRs not implemented\n"
     "@#8 TeV, 20/fb\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("M1;M2;M3;");
//  bookCutflowRegions("00_trigger;01_lepveto;02_njets<3;03_phijetET;04_leadingjet>150;05_missET>150;06M1_leadingjet>280;07M1_missET>220;06M2_leadingjet>340;07M2_missET>340;06M3_leadingjet>450;07M3_missET>450;");
  // You should initialize any declared variables here
}

void Atlas_1407_0608::analyze() {
  // Your eventwise analysis code goes here
  missingET->addMuons(muonsCombined);
  electronsMedium = filterPhaseSpace(electronsMedium, 20., -2.47, 2.47);           
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);         
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
  
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);
  
  //trigger
  if (missingET->P4().Et() < 80.) return;
  countCutflowEvent("00_trigger");
  
  if (isoMuons.size() > 0 || electronsMedium.size() > 0 ) return;
  countCutflowEvent("01_lepveto");
  
  if ( jets.size() > 3 && jets[3]->PT > 30. ) return;
  countCutflowEvent("02_njets<3");
  
  for ( int i = 0; i < jets.size() && jets[i]->PT > 30.; i++)
    if ( fabs(missingET->P4().DeltaPhi(jets[i]->P4())) < 0.4) return;
  countCutflowEvent("03_phijetET");
  
  if ( jets.size() == 0 || jets[0]->PT < 150. ) return;
  countCutflowEvent("04_leadingjet>150");
  
  if (missingET->P4().Et() < 150.) return;
  countCutflowEvent("05_missET>150");
  
  if (jets[0]->PT > 280.) {
      countCutflowEvent("06M1_leadingjet>280");
      if (missingET->P4().Et() > 220.) {
      countCutflowEvent("07M1_missET>220");
      countSignalEvent("M1");
      }
  }
  
    if (jets[0]->PT > 340.) {
      countCutflowEvent("06M2_leadingjet>340");
      if (missingET->P4().Et() > 340.) {
      countCutflowEvent("07M2_missET>340");
      countSignalEvent("M2");
      }
  }
  
    if (jets[0]->PT > 450.) {
      countCutflowEvent("06M3_leadingjet>450");
      if (missingET->P4().Et() > 450.) {
      countCutflowEvent("07M3_missET>450");
      countSignalEvent("M3");
      }
  }
  
}

void Atlas_1407_0608::finalize() {
  // Whatever should be done after the run goes here
}       
