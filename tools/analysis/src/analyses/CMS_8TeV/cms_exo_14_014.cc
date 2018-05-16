#include "cms_exo_14_014.h"
#include <stdio.h>

// AUTHOR: Ahmed Hammad
//  EMAIL: ahammad@zewailcity.edu.eg
void Cms_exo_14_014::initialize() {
  setAnalysisName("cms_exo_14_014");          
  setInformation(""
		 "# Based on arXiv:1603.02248 [hep-ex]\n"
		 "# Cuts are in Table:2\n"
		 "# Number of obs and bkgs are in Table:7\n"
		 "");
  setLuminosity(19.7*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SR_low_emu;SR_emu_90;SR_emu_150;SR_emu_200;SR_emu_250;SR_emu_300;SR_emu_350;SR_emu_400;SR_emu_500");
  bookSignalRegions("SR_low_ee;SR_ee_90;SR_ee_150;SR_ee_200;SR_ee_250;SR_ee_300;SR_ee_500");
  bookCutflowRegions("CRlow_ee;CRlow_emu");
  bookCutflowRegions("CR90_ee;CR90_emu");
  bookCutflowRegions("CR150_ee;CR150_emu");
  bookCutflowRegions("CR200_ee;CR200_emu");
  bookCutflowRegions("CR250_ee;CR250_emu");
  bookCutflowRegions("CR300_ee;CR300_emu");
  bookCutflowRegions("CR350_emu");
  bookCutflowRegions("CR400_emu");
  bookCutflowRegions("CR500_ee;CR500_emu");
}
void Cms_exo_14_014::analyze() {
  
  missingET->addMuons(muonsCombined);
  electrons = filterPhaseSpace(electrons, 15., -2.5, 2.5);
  muonsCombined = filterPhaseSpace(muonsCombined, 15., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 20., -2.5, 2.5);
  jets = overlapRemoval(jets, electrons, 0.2);
  electrons = overlapRemoval(electrons, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);

  std::vector<Electron*> isoElecs = filterIsolation(electrons);
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);
  std::vector<Jet*> hardjets = filterPhaseSpace(jets);
  std::vector<TLorentzVector> leptons;
  
  bool ee = false; bool mumu = false; bool emu = false;
  //Arange both electrons and munos according to their momentum
  if ( isoElecs.size() + isoMuons.size() != 2  || isoElecs.empty()  ) return;   
  if (isoElecs.size() == 2) {
    ee = true ;
    leptons.push_back( isoElecs[0]->P4() );
    leptons.push_back( isoElecs[1]->P4() );
  }
  else if (isoMuons.size() == 2) {
    mumu = true ;
    leptons.push_back( isoMuons[0]->P4() );
    leptons.push_back( isoMuons[1]->P4() );
  }
  else {
    emu = true ;
    if ( isoElecs[0]->PT > isoMuons[0]->PT ) {
      leptons.push_back( isoElecs[0]->P4() );
      leptons.push_back( isoMuons[0]->P4() );
    }
    else {
      leptons.push_back( isoMuons[0]->P4() );
      leptons.push_back( isoElecs[0]->P4() );
    }
  }
  
  
  //Be sure that you have at least two hard leptons and two hard jets
  if (leptons.size() < 2 ) return;
  if (hardjets.size() < 2) return;
  TLorentzVector ll = leptons[0] + leptons[1];
  TLorentzVector jj = hardjets[0]->P4() + hardjets[1]->P4();
  TLorentzVector ljj = leptons[1] + hardjets[0]->P4() + hardjets[1]->P4();
  TLorentzVector lljj = leptons[0] + leptons[1] + hardjets[0]->P4() + hardjets[1]->P4();
  double mll = ll.M();
  double mjj = jj.M();
  double mljj = ljj.M();
  double mlljj = lljj.M();
  
  // Final state electron+muon+jj  
  
  if (emu == true){
    if ( mlljj > 80. && mlljj < 150.) {
      if (missingET->PT > 30. || leptons[0].Pt() < 20. || leptons[1].Pt() < 15. || hardjets[0]->PT < 20. || hardjets[1]->PT < 20. || mll < 10. || mjj > 120.) return;
      countCutflowEvent("CRlow_emu");   
      countSignalEvent("SR_low_emu");     
    }
    if ( mljj < 130. ) {
      if (missingET->PT > 35. ||leptons[0].Pt() < 40. || leptons[1].Pt() < 15. || hardjets[0]->PT < 30. || hardjets[1]->PT < 30. || mll < 45. || mlljj < 120. || mjj > 110. ||  mjj < 50.) return;
      countCutflowEvent("CR90_emu");   
      countSignalEvent("SR_emu_90");     
    }
    if ( mljj < 230. ) {
      if (missingET->PT > 35.|| leptons[0].Pt() < 40. || leptons[1].Pt() < 30. || hardjets[0]->PT < 30. || hardjets[1]->PT < 30. || mll < 45. || mlljj < 150. || mjj > 110. ||  mjj < 50.) return;
      countCutflowEvent("CR150_emu");
      countSignalEvent("SR_emu_150");
    }
    if ( mljj < 330. ) {
      if (missingET->PT > 35.||leptons[0].Pt() < 75. || leptons[1].Pt() < 35. || hardjets[0]->PT < 35. || hardjets[1]->PT < 35. || mll < 45. || mlljj < 200. || mjj > 110. ||  mjj < 50.) return;
      countCutflowEvent("CR200_emu");
      countSignalEvent("SR_emu_200");
    }
    if ( mljj < 390. ) {
      if (missingET->PT > 35.||leptons[0].Pt() < 80. || leptons[1].Pt() < 40. || hardjets[0]->PT < 35. || hardjets[1]->PT < 35. || mll < 45. || mlljj < 260.|| mjj > 110. ||  mjj < 50. ) return;
      countCutflowEvent("CR250_emu");
      countSignalEvent("SR_emu_250");
    }
    if ( mljj < 490. ) {
      if (missingET->PT > 35.||leptons[0].Pt() < 110. || leptons[1].Pt() < 40. || hardjets[0]->PT < 35. || hardjets[1]->PT < 35. || mll < 45. || mlljj < 310.|| mjj > 110. ||  mjj < 50.) return;
      countCutflowEvent("CR300_emu");
      countSignalEvent("SR_emu_300");
    }
    if ( mljj < 550. ) {
      if (missingET->PT > 35.||leptons[0].Pt() < 110. || leptons[1].Pt() < 40. || hardjets[0]->PT < 35. || hardjets[1]->PT < 35. || mll < 45. || mlljj < 360. || mjj > 110. ||  mjj < 50.) return;
      countCutflowEvent("CR350_emu");
      countSignalEvent("SR_emu_350");
    }
    if ( mljj < 600. ) {
      if (missingET->PT > 35.||leptons[0].Pt() < 120. || leptons[1].Pt() < 40. || hardjets[0]->PT < 35. || hardjets[1]->PT < 35. || mll < 45. || mlljj < 380.|| mjj > 110. ||  mjj < 50.) return;
      countCutflowEvent("CR400_emu");
      countSignalEvent("SR_emu_400");   }
    if ( mljj < 700. ) {
      if (missingET->PT > 35.||leptons[0].Pt() < 120. || leptons[1].Pt() < 40. || hardjets[0]->PT < 35. || hardjets[1]->PT < 35. || mll < 45. || mlljj < 380.|| mjj > 110. ||  mjj < 50.) return;
      countCutflowEvent("CR500_emu");
      countSignalEvent("SR_emu_500");}
  }
  
  // Final state di-electrons+jj
  
  if (ee == true) {
    if(   mlljj > 80. && mlljj < 160. ) {
      if (missingET->PT > 30. || leptons[0].Pt() < 20. || leptons[1].Pt() < 15. || hardjets[0]->PT < 20. || hardjets[1]->PT < 20. ||  mljj > 120.|| mll < 10. || mll > 60.  || mjj > 120. ) return;
      countCutflowEvent("CRlow_ee");   
      countSignalEvent("SR_low_ee");     
    }
     if (mljj > 80. && mljj < 120.) {
      if (leptons[0].Pt() < 20. || leptons[1].Pt() < 15. || hardjets[0]->PT < 30. || hardjets[1]->PT < 30. ||  mlljj < 120.  || mll < 15.|| mjj > 110. ||  mjj < 50.) return;
      countCutflowEvent("CR90_ee");   
      countSignalEvent("SR_ee_90");     
    }
    if (mljj > 125. && mljj < 175.) {
      if (missingET->PT > 35.||leptons[0].Pt() < 40. || leptons[1].Pt() < 20. || hardjets[0]->PT < 30. || hardjets[1]->PT < 30. ||  mlljj < 195. || mll < 15.|| mjj > 110. ||  mjj < 50.) return;
      countCutflowEvent("CR150_ee");   
      countSignalEvent("SR_ee_150");     
    }
    if ( mljj > 160. && mljj < 255.) {
      if (missingET->PT > 35.||leptons[0].Pt() < 65. || leptons[1].Pt() < 40. || hardjets[0]->PT < 30. || hardjets[1]->PT < 30. || mljj < 280. || mll < 15.|| mjj > 110. ||  mjj < 50.) return;
      countCutflowEvent("CR200_ee");
      countSignalEvent("SR_ee_200");
    }
    if ( mlljj > 300.) {
    if (missingET->PT > 35.||leptons[0].Pt() < 110. || leptons[1].Pt() < 40. || hardjets[0]->PT < 40. || hardjets[1]->PT < 40. || mll < 15.|| mjj > 110. ||  mjj < 50.) return;
    countCutflowEvent("CR250_ee");
    countSignalEvent("SR_ee_250");
    }
    if ( mlljj > 320.) {
      if (missingET->PT > 35.||leptons[0].Pt() < 120. || leptons[1].Pt() < 40. || hardjets[0]->PT < 40. || hardjets[1]->PT < 40. || mll < 15.|| mjj > 110. ||  mjj < 50.) return;
      countCutflowEvent("CR300_ee");
      countSignalEvent("SR_ee_300");
    }
    if ( mlljj > 360.) {
      if (missingET->PT > 35.||leptons[0].Pt() < 120. || leptons[1].Pt() < 40. || hardjets[0]->PT < 40. || hardjets[1]->PT < 40. || mll < 15.|| mjj > 110. ||  mjj < 50.) return;
      countCutflowEvent("CR500_ee");
      countSignalEvent("SR_ee_500");}
  }
}

void Cms_exo_14_014::finalize() {
  // Whatever should be done after the run goes here
}       
