#include "cms_sus_12_019.h"

void Cms_sus_12_019::initialize() {
  setAnalysisName("cms_sus_12_019");          
  setInformation(""
    "@#2-leptons, jets and missing energy\n"
     "@#Opposite flavour leptons\n"
  "");
  setLuminosity(19.4*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("Cen_SF;For_SF;Cen_OF;For_OF");
  // You should initialize any declared variables here
}

void Cms_sus_12_019::analyze() {
  // Your eventwise analysis code goes here
  
  missingET->addMuons(muonsCombined);
  
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.4, 2.4);  
  electronsLoose = filterIsolation(electronsLoose);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);  
  muonsCombined = filterIsolation(muonsCombined);
  
  jets = filterPhaseSpace(jets, 40., -3.0, 3.0);
  jets = overlapRemoval(jets, electronsLoose, 0.4);
  electronsLoose = overlapRemoval(electronsLoose, jets, 0.4);
  int nJets = jets.size();            
  countCutflowEvent("a_initial");
  
  //Always need at least 2 leptons
  if( (electronsLoose.size() + muonsCombined.size()) < 2)
    return;
  countCutflowEvent("b_2lep");
  
  // Include trigger efficiencies
  
  bool triggerFlag = false;
  double triggerRatio = (double) rand() / (double) (RAND_MAX + 1.);
  if((electronsLoose.size() >1) && (triggerRatio < 0.97)) triggerFlag =true;
  else if((muonsCombined.size() >1) && (triggerRatio < 0.97)) triggerFlag =true;
  else if((electronsLoose.size() == 1) && (muonsCombined.size() == 1)){ 
    if((fabs(muonsCombined[0]->Eta) < 1.4) && (fabs(electronsLoose[0]->Eta) <1.4) && (triggerRatio < 0.94)) triggerFlag =true;
    else if (triggerRatio < 0.88) triggerFlag =true;
  }
  
  if(triggerFlag == false)
    return;
  countCutflowEvent("c_trigger"); 

  //Signal leptons should not lie in transition region
  std::vector<Electron*> electronsSignal;
  std::vector<Muon*> muonsSignal;
    
  for(int i = 0; i < electronsLoose.size(); i++){
    if( (fabs(electronsLoose[i]->Eta) < 1.4) || (fabs(electronsLoose[i]->Eta) > 1.6) ) electronsSignal.push_back(electronsLoose[i]);
  }
  for(int i = 0; i < muonsCombined.size(); i++){
    if( (fabs(muonsCombined[i]->Eta) < 1.4) || (fabs(muonsCombined[i]->Eta) > 1.6) ) muonsSignal.push_back(muonsCombined[i]);
  }
  
  if( (electronsSignal.size() + muonsSignal.size()) < 2)
    return;
  countCutflowEvent("e_2lep_transition");
  
  //If more than 2 leptons, only highest pair with highest pT selected
  if( (electronsSignal.size() + muonsSignal.size()) > 2){
    if ((electronsSignal.size() > 2) && (muonsSignal.size() == 0)) electronsSignal.erase(electronsSignal.begin()+2,electronsSignal.end());
    else if ((muonsSignal.size() > 2) && (electronsSignal.size() == 0)) muonsSignal.erase(muonsSignal.begin()+2,muonsSignal.end());   
    
    else if ((electronsSignal.size() > 1) && (muonsSignal.size() == 1)){
      if(electronsSignal[1]->PT > muonsSignal[0]->PT){
    muonsSignal.erase(muonsSignal.begin(),muonsSignal.end());
    electronsSignal.erase(electronsSignal.begin()+2,electronsSignal.end());
      }
      else {
    electronsSignal.erase(electronsSignal.begin()+1,electronsSignal.end());
      }
    }
    else if ((muonsSignal.size() > 1) && (electronsSignal.size() == 1)){
      if(muonsSignal[1]->PT > electronsSignal[0]->PT){
    electronsSignal.erase(electronsSignal.begin(),electronsSignal.end());
     muonsSignal.erase(muonsSignal.begin()+2,muonsSignal.end());
      }
      else {
    muonsSignal.erase(muonsSignal.begin()+1,muonsSignal.end());
      }
    } 
    else if ((electronsSignal.size() > 1) && (muonsSignal.size() > 1)){
      if(electronsSignal[1]->PT > muonsSignal[0]->PT) {
    muonsSignal.erase(muonsSignal.begin(),muonsSignal.end());
    electronsSignal.erase(electronsSignal.begin()+2,electronsSignal.end());
      }
      else if(muonsSignal[1]->PT > electronsSignal[0]->PT){
    electronsSignal.erase(electronsSignal.begin(),electronsSignal.end());
    muonsSignal.erase(muonsSignal.begin()+2,muonsSignal.end()); 
      }
      else {
    electronsSignal.erase(electronsSignal.begin()+1,electronsSignal.end());
    muonsSignal.erase(muonsSignal.begin()+1,muonsSignal.end());
      }
    } 
      
  }

  
  //Flags to allow testing of various cutflows
  bool lep_OSSF_cen = false;
  bool lep_OSSF_for = false;
  bool lep_pt_cen = false;
  bool lep_pt_for = false;
  bool lep_mll_cen = false;
  bool lep_mll_for = false;
  
  ///NEED TO SAVE BOTH OPPOSITE FLAVOUR AND SAME FLAVOUR: EXCESS MUST BE SUBTRACTED!!!!!!!!!!
  
  //First check electrons
  if(electronsSignal.size() > 1){
    //Check all combinations for successful pair
    for(int i = 0; i < electronsSignal.size()-1; i++){
      for(int j = 1; j < electronsSignal.size(); j++){
    if((electronsSignal[i]->Charge*electronsSignal[j]->Charge<0) && (fabs(electronsSignal[i]->P4().DeltaR(electronsSignal[j]->P4())) > 0.3)){
      if((fabs(electronsSignal[i]->Eta) < 1.4) && (fabs(electronsSignal[j]->Eta) < 1.4)) lep_OSSF_cen = true;
      else lep_OSSF_for = true;
      if((electronsSignal[i]->PT > 20.0) && (electronsSignal[j]->PT > 20.0)){
        if((fabs(electronsSignal[i]->Eta) < 1.4) && (fabs(electronsSignal[j]->Eta) < 1.4)) lep_pt_cen = true;
        else lep_pt_for = true;
        double mll = (electronsSignal[i]->P4() + electronsSignal[j]->P4()).M();
        if((mll > 20.) && (mll < 70.)){
          if((fabs(electronsSignal[i]->Eta) < 1.4) && (fabs(electronsSignal[j]->Eta) < 1.4)) lep_mll_cen = true;
          else lep_mll_for = true;
        }
      }
    }
      }
    }
  }

  //Now check muons
  if(muonsSignal.size() > 1){
    //Check all combinations for successful pair
    for(int i = 0; i < muonsSignal.size()-1; i++){
      for(int j = 1; j < muonsSignal.size(); j++){
    if((muonsSignal[i]->Charge*muonsSignal[j]->Charge<0) && (fabs(muonsSignal[i]->P4().DeltaR(muonsSignal[j]->P4())) > 0.3)){
      if((fabs(muonsSignal[i]->Eta) < 1.4) && (fabs(muonsSignal[j]->Eta) < 1.4)) lep_OSSF_cen = true;
      else lep_OSSF_for = true;
      if((muonsSignal[i]->PT > 20.0) && (muonsSignal[j]->PT > 20.0)){
        if((fabs(muonsSignal[i]->Eta) < 1.4) && (fabs(muonsSignal[j]->Eta) < 1.4)) lep_pt_cen = true;
        else lep_pt_for = true;
        double mll = (muonsSignal[i]->P4() + muonsSignal[j]->P4()).M();
        if((mll > 20.) && (mll < 70.)){
          if((fabs(muonsSignal[i]->Eta) < 1.4) && (fabs(muonsSignal[j]->Eta) < 1.4)) lep_mll_cen = true;
          else lep_mll_for = true;
        }
      }
    }
      }
    }
  }
  
  //Use flags for cutflow
  if(lep_OSSF_cen == true) countCutflowEvent("f_OSSF_cen");
  if(lep_OSSF_for == true) countCutflowEvent("f_OSSF_for");
  if(lep_pt_cen == true) countCutflowEvent("g_pt_cen");
  if(lep_pt_for == true) countCutflowEvent("g_pt_for");
  if(lep_mll_cen == true) countCutflowEvent("h_mll_cen");
  if(lep_mll_for == true) countCutflowEvent("h_mll_for");
  
  //Now opposite but no need for cutflow      
  bool lep_OF_cen = false;
  bool lep_OF_for = false;
  if((muonsSignal.size() > 0) && (electronsSignal.size() >0)){
    //Check all combinations for successful pair
    for(int i = 0; i < muonsSignal.size(); i++){
      for(int j = 0; j < electronsSignal.size(); j++){
    if((muonsSignal[i]->Charge*electronsSignal[j]->Charge<0) && (fabs(muonsSignal[i]->P4().DeltaR(electronsSignal[j]->P4())) > 0.3)){
      bool cen = false;
      if((fabs(muonsSignal[i]->Eta) < 1.4) && (fabs(electronsSignal[j]->Eta) < 1.4)) cen = true;
      if( (muonsSignal[i]->PT > 20.0) && (electronsSignal[j]->PT > 20.0) ){
        double mll = (muonsSignal[i]->P4() + electronsSignal[j]->P4()).M();
        if((mll > 20.) && (mll < 70.)){
          if(cen ==true) lep_OF_cen = true;
          else lep_OF_for = true;
        }
      }
    }
      }
    }
  }
  
  // Veto if no signal region lepton pair found
  if((lep_mll_cen == false) && (lep_mll_for == false) && (lep_OF_cen == false) && (lep_OF_for==false))
    return;
  
  //Require two jets pT >40
  if(jets.size() < 2)
    return;
  
  if(lep_mll_cen == true) countCutflowEvent("i_2j_cen");
  if(lep_mll_for == true) countCutflowEvent("i_2j_for");
  
  //Cutflow if Njets > 2
  if(jets.size() > 2){
    if(lep_mll_cen == true) countCutflowEvent("i_3j_cen");
    if(lep_mll_for == true) countCutflowEvent("i_3j_for");
  }
  
  
  // Require ETmiss > 100
  if( missingET->P4().Et() < 100.)
    return;
  
  if(lep_mll_cen == true) countCutflowEvent("j_met100_cen");
  if(lep_mll_for == true) countCutflowEvent("j_met100_for");  
  
  if( missingET->P4().Et() > 150.){
    if(lep_mll_cen == true) countCutflowEvent("j_met150_cen");
    if(lep_mll_for == true) countCutflowEvent("j_met150_for");     
  }
  
  // Define signal regions 
  bool jetMet = false;
  if( (jets.size() > 1) && ( missingET->P4().Et() > 150.)) jetMet = true;
  else if( (jets.size() > 2) && ( missingET->P4().Et() > 100.)) jetMet = true;
  if(jetMet == false)
    return;

  if(lep_mll_cen == true){
    countCutflowEvent("k_sig_cen");
    countSignalEvent("Cen_SF");
  }
  if(lep_mll_for == true){
    countCutflowEvent("k_sig_for");
    countSignalEvent("For_SF");
  }
  if(lep_OF_cen == true){
    countSignalEvent("Cen_OF");
  }
  if(lep_OF_for == true){
    countSignalEvent("For_OF");
  }  
  
}

void Cms_sus_12_019::finalize() {
  // Whatever should be done after the run goes here
}       
