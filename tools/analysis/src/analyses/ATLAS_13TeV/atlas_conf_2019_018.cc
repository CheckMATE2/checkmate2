#include "atlas_conf_2019_018.h"
// AUTHOR: Weiwei, Zhang Di, Liangliang Shang, Junjie cao
// EMAIL: wjw4001@163.com and dz481655@gmail.com

template<class T> std::vector<T*> isolateLeptons(std::vector<T*> leptons, double deltaR);

void Atlas_conf_2019_018::initialize() {
  setAnalysisName("atlas_conf_2019_018");          
  setInformation(""
    "# ATLAS\n"
    "# ATLAS_CONF_2019_018\n"
    "# 2taus and missing et\n"
    "# sqrt(s)=13TeV\n"
    "# int(L)=139fb-1\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("sr_lowmass;sr_highmass");
}

void Atlas_conf_2019_018::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.


//reconstruction
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
  tauJets=filterPhaseSpace(tauJets, 20., -2.47, 2.47, true);//tau jet

//b jet veto 
  int num_bjet = 0;
  for(int i=0; i<jets.size(); i++){
    if(jets[i]->PT > 20. and fabs(jets[i]->Eta)<2.5){
            if(checkBTag(jets[i])){
                num_bjet++;
            }
        }
  }
  


//overlap removal
  tauJets=overlapRemoval(tauJets, electronsLoose, 0.2, "y");
  tauJets=overlapRemoval(tauJets, muonsCombinedPlus, 0.2, "y");
  overlapRemoval_electrons_jets(electronsLoose, jets);
  overlapRemoval_muons_jets(muonsCombinedPlus, jets);
  jets = overlapRemoval(jets, tauJets, 0.2, "y");

  countCutflowEvent("00_all");

  // OStau
  if(tauJets.size() != 2) return;
  if(tauJets[0]->Charge * tauJets[1]->Charge > 0) return;
  
  //trigger
  bool trigger1 = false;
  bool trigger2 = false;
  if(missingET->PT > 50. and tauJets[0]->PT > 35. and tauJets[1]->PT > 25.) {
    trigger1 = true;
  } 
  if(tauJets[0]->PT > 85. and tauJets[1]->PT > 50.) {
    trigger2 = true;
  }

  if (!trigger1 and !trigger2) return;

  countCutflowEvent("01_ditaus");

  int num_tight_tau = 0;
            for(int i=0; i<tauJets.size(); i++) {
              if(checkTauTag(tauJets[i], "tight")) {
                num_tight_tau++;
              }         
            }


  //additonal medium tau and lepton veto
  if(tauJets.size() > 2 or electronsLoose.size() > 0 or muonsCombinedPlus.size() > 0) return;
  
  countCutflowEvent("02_additional_veto");

  if(((tauJets[0]->P4()+tauJets[1]->P4()).M()) < 120. ) return;
  
  countCutflowEvent("03_Z_H_veto");

  if (num_bjet > 0) return;
  
  countCutflowEvent("04_bveto");

  
  double mt2=mT2(tauJets[0]->P4(), tauJets[1]->P4(), 0.);


// delta R(tau1, tau2)
  double deltaR = tauJets[0]->P4().DeltaR(tauJets[1]->P4());
// |delta phi(tau1, tau2)|
  double deltaphi = fabs(tauJets[0]->P4().DeltaPhi(tauJets[1]->P4()));  

  //signal regions
  if(true){//SR_lowmass
    if(num_tight_tau > 1) {
      countCutflowEvent("srL_01_2tau_tight");
      if(trigger2) {
        countCutflowEvent("srL_02_trigger_asy");
        if(missingET->PT > 75.) {
          countCutflowEvent("srL_03_MET_75-150");
          if(tauJets[0]->PT > 95. and tauJets[1]->PT > 60.) {
            countCutflowEvent("srL_04_tau_PT");
            if(deltaR < 3.2){
              if(deltaphi > 0.8) {
                if(mt2 > 70.) {
                  countCutflowEvent("srL_05_dr_dphi_mt2");
                  countSignalEvent("sr_lowmass");
                }
              }
            }    
          }
        }
      }
    }
  }
          
  if(true){ //SR_highmass
    if(num_tight_tau > 0) {
      countCutflowEvent("srH_01_more1tau_tight");
      if(trigger1){ 
        countCutflowEvent("srH_02_trigger");
        if(missingET->PT > 150.) {
          countCutflowEvent("srH_03_MET_more150");
          if(tauJets[0]->PT > 50. and tauJets[1]->PT > 40.) {
            countCutflowEvent("srH_04_tau_PT");
            if(deltaR < 3.2) {
              if(deltaphi > 0.8) {
                if(mt2 > 70.) {
                  countCutflowEvent("srH_05_dr_dphi_mt2");
                  countSignalEvent("sr_highmass");
                }
              }
            } 
          }
        }
      }
    }
  }
}

void Atlas_conf_2019_018::finalize() {
  // Whatever should be done after the run goes here
}

void Atlas_conf_2019_018::overlapRemoval_electrons_jets(std::vector<Electron*>& electrons, std::vector<Jet*>& jets) {
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

void Atlas_conf_2019_018::overlapRemoval_muons_jets(std::vector<Muon*>& muons, std::vector<Jet*>& jets) {
  std::vector<bool> del_muons;
  std::vector<bool> del_jets;
  //fill bool vector
  for(int i=0; i<muons.size(); i++){
    del_muons.push_back(false);
  }
  for(int i=0; i<jets.size(); i++){
    del_jets.push_back(false);
  }
  for(int i=0; i< muons.size(); i++) {
    for(int j=0; j<jets.size(); j++) {
      double y_mu = muons[i]->P4().Rapidity();
      double y_jet = jets[j]->P4().Rapidity();

      double dr_y=sqrt(pow(y_mu-y_jet,2)+pow(muons[i]->P4().DeltaPhi(jets[j]->P4()),2));
      if(dr_y<0.4) {
        if(dr_y>0.2) {
          del_muons[i]=true;
        }
      }
    }
  }

  std::vector<Muon*> filtered_muons;
  std::vector<Jet*> filtered_jets;
  for(int i=0;i<muons.size();i++) {
    if(del_muons[i]==false) {
      filtered_muons.push_back(muons[i]);
    }
  }
  for(int i=0;i<jets.size();i++) {
      filtered_jets.push_back(jets[i]);
  }

  muons = filtered_muons;
  jets = filtered_jets;
}
