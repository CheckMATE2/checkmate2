#include "atlas_1708_07875.h"
// AUTHOR:Junjie Cao, K. Rolbiecki, Liangliang Shang and Di Zhang
void Atlas_1708_07875::initialize() {
  setAnalysisName("atlas_1708_07875"); 
  setInformation(""
    "# search for susy\n"
    "# at least two taus and missing et\n"
    "# sqrt(s)=13TeV\n"
    "# 36.1fb^-1\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("sr1-lowmass;sr2-highmass;");
}

void Atlas_1708_07875::analyze() {


  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47);
  electronsLoose = filterIsolation(electronsLoose);
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 10., -2.7, 2.7);
  muonsCombinedPlus = filterIsolation(muonsCombinedPlus);
  jets = filterPhaseSpace(jets, 10., -2.8, 2.8);

  std::vector<Jet*> tauJets;
  for(int i=0; i<jets.size(); i++) {
    if(fabs(jets[i]->Eta) < 2.5 and checkTauTag(jets[i], "medium") and fabs(jets[i]->Charge)==1) {
      tauJets.push_back(jets[i]);
    }
  }
  tauJets=filterPhaseSpace(tauJets, 20., -2.47, 2.47, true);


  tauJets=overlapRemoval(tauJets, electronsLoose, 0.2);
  tauJets=overlapRemoval(tauJets, muonsCombinedPlus, 0.2);
  electronsLoose=overlapRemoval(electronsLoose, muonsCombinedPlus, 0.2);
  overlapRemoval_electrons_jets(electronsLoose, jets);
  jets = overlapRemoval(jets, tauJets, 0.4);
  jets = overlapRemoval(jets, muonsCombinedPlus, 0.4);


  countCutflowEvent("00_all");

  //some cuts with reference of atlas_1407_0350.cc
/*  bool low_resonance = false;
  for(int i=0; i< electronsLoose.size(); i++) {
    for(int j=0; j<electronsLoose.size(); j++) {
      if(i!=j) {
        if(electronsLoose[i]->Charge*electronsLoose[j]->Charge<0) {
          if((electronsLoose[i]->P4()+electronsLoose[j]->P4()).M()<12.) {
            low_resonance = true;
          }
        }
      }
    }
  }
  for(int i=0; i< muonsCombinedPlus.size(); i++) {
    for(int j=0; j< muonsCombinedPlus.size(); j++) {
      if(i!=j) {
        if(muonsCombinedPlus[i]->Charge*muonsCombinedPlus[j]->Charge<0) {
          if((muonsCombinedPlus[i]->P4()+muonsCombinedPlus[j]->P4()).M()<12.) {
            low_resonance = true;
          }
        }
      }
    }
  }

  if( (electronsLoose.size()+muonsCombinedPlus.size()
       +tauJets.size()) < 2 or low_resonance ) return;
 

  countCutflowEvent("cut01_2leptons");*/

  if( tauJets.size() < 2 ) return;
  bool trigger_asy_pre = tauJets[0]->PT > 85. and tauJets[1]->PT > 50.;
  bool trigger_met_pre = missingET->PT > 50. and tauJets[0]->PT > 35. and tauJets[1]->PT > 25.;
  bool trigger_asy = tauJets[0]->PT > 95. and tauJets[1]->PT > 65.;
  bool trigger_met = missingET->PT > 150. and tauJets[0]->PT > 50. and tauJets[1]->PT > 40.;
  if (!trigger_asy_pre and !trigger_met_pre) return;
 
  countCutflowEvent("01_2taus");


  int num_OSTau_pair = 0;
  bool z_veto = false;
  for(int i=0; i<tauJets.size(); i++) {
    for(int j=0; j<tauJets.size(); j++) {
      if(i>j) {
        if(tauJets[i]->Charge * tauJets[j]->Charge < 0) {
          //veto taus from low mass resonance
          num_OSTau_pair++;
          //mz=81GeV
          if( fabs( (tauJets[i]->P4()+tauJets[j]->P4()).M()-79.) < 10. ) z_veto = true;
        }
      }
    }
  }

  if (num_OSTau_pair < 1) return;
  countCutflowEvent("02_OStaus");

  int num_bjet = 0;
  for(int i=0; i<jets.size(); i++) {
    if(jets[i]->PT > 20. and fabs(jets[i]->Eta)<2.5) {
      if(checkBTag(jets[i])) {
        num_bjet++;
      }
    }
  }

  if (num_bjet > 0) return;
  countCutflowEvent("03_bveto");

  if (z_veto) return;
  countCutflowEvent("04_Zveto");

  int num_tight_tau=0;
  for(int i=0; i<tauJets.size(); i++) {
    if (checkTauTag(tauJets[i], "tight") ) num_tight_tau++;
  }

  double mt2max=-99.;
  for(int i=0; i<tauJets.size(); i++) { 
    for(int j=0; j<tauJets.size(); j++) {
      if(i>j) {
        if( mT2(tauJets[i]->P4(), tauJets[j]->P4(), 0.) > mt2max) mt2max=mT2(tauJets[i]->P4(), tauJets[j]->P4(), 0.);
      }
    }
  }

  double mtautau=(tauJets[0]->P4() + tauJets[1]->P4() ).M();
  
  //signal regions
  //sr-lowmass
  if(trigger_met) {
    countCutflowEvent("srL_01_Trigger");
    if(mt2max > 70.) {
      countCutflowEvent("srL_02_mt2");
      countSignalEvent("sr1-lowmass");
    }
  } 


  //sr-highmass
   
  if (tauJets.size() > 1 and num_tight_tau > 0) {
    countCutflowEvent("srH_01_TightTau");
    if(trigger_met) {
      if (tauJets[0]->PT > 80.) {
        countCutflowEvent("srH_02_PT");
        if (mtautau>=110) {
          countCutflowEvent("srH_03_mass");
          if (trigger_asy ) {
            if( missingET->PT > 110.) {
              countCutflowEvent("srH_04_MET");
              if(mt2max>90.) {
                countCutflowEvent("srH_04_MT2");
              } 
            }
          } else {
            countCutflowEvent("srH_04_MET");
            if(mt2max>90.) {
              countCutflowEvent("srH_04_MT2");
            } 
          }
        }
      }
    } else {
      countCutflowEvent("srH_02_PT");
      if (mtautau>=110) {
        countCutflowEvent("srH_03_mass");
        if (trigger_asy ) {
          if( missingET->PT > 110.) {
            countCutflowEvent("srH_04_MET");
            if(mt2max>90.) {
              countCutflowEvent("srH_04_MT2");
            } 
          }
        } else {
          countCutflowEvent("srH_04_MET");
          if(mt2max>90.) {
            countCutflowEvent("srH_04_MT2");
          } 
        }
      }
    }
  }
}

void Atlas_1708_07875::finalize() {
  // Whatever should be done after the run goes here                            
}


void Atlas_1708_07875::overlapRemoval_electrons_jets(std::vector<Electron*>& electrons, std::vector<Jet*>& jets) {
  std::vector<bool> del_electrons;
  std::vector<bool> del_jets;
  //fill bool vector
  for(int i=0; i<electrons.size(); i++){
    del_electrons.push_back(false);
  }
  for(int i=0; i<jets.size(); i++){
    del_jets.push_back(false);
  }
  for(int i=0; i< electrons.size(); i++) {
    for(int j=0; j<jets.size(); j++) {
      double y_e = electrons[i]->P4().Rapidity();
      double y_jet = jets[j]->P4().Rapidity();

      double dr_y=sqrt(pow(y_e-y_jet,2)+pow(electrons[i]->P4().DeltaPhi(jets[j]->P4()),2));
      if(dr_y<0.4) {
        if(dr_y<0.2 and not checkBTag(jets[j])) {
          del_jets[j]=true;
        } else {
          del_electrons[i]=true;
        }
      }
    }
  }

  std::vector<Electron*> filtered_electrons;
  std::vector<Jet*> filtered_jets;
  for(int i=0;i<electrons.size();i++) {
    if(del_electrons[i]==false) {
      filtered_electrons.push_back(electrons[i]);
    }
  }
  for(int i=0;i<jets.size();i++) {
    if(del_jets[i]==false) {
      filtered_jets.push_back(jets[i]);
    }
  }

  electrons = filtered_electrons;
  jets = filtered_jets;
}
