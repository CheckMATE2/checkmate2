#include "cms_sus_13_016.h"
// AUTHOR: Jamie Tattersall
//  EMAIL: tattersall@thphys.uni-heidelberg.de
void Cms_sus_13_016::initialize() {
  setAnalysisName("cms_sus_13_016");          
  setInformation(""
    "@#Opposite sign leptons\n"
     "@#3+ b-jets\n"
     "@#Gluino -> Stop decay search\n"
  "");
  setLuminosity(19.5*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SR1;");
  // You should initialize any declared variables here
}

void Cms_sus_13_016::analyze() {
  // Your eventwise analysis code goes here
    missingET->addMuons(muonsCombined);
    jets = filterPhaseSpace(jets, 30., -2.4, 2.4);
    int nJets = jets.size();   
    //photons = filterPhaseSpace(photons, 100., -2.0, 2.0);
    //int nPhotons = photons.size();
    electronsTight = filterPhaseSpace(electronsTight, 20., -2.4, 2.4);
    electronsTight = filterIsolation(electronsTight);
    int nElectronsTight = electronsTight.size();    
    muonsCombined = filterPhaseSpace(muonsCombined, 20., -2.4, 2.4);
    muonsCombined = filterIsolation(muonsCombined);
    int nMuonsCombined = muonsCombined.size();  
    
    //------------------------
    // Remove jets that overlap with electrons
    jets = overlapRemoval(jets, electronsTight, 0.3);
    nJets = jets.size();
    //---------------------------
    // Remove any baseline muon that overlaps with a jett
    muonsCombined = overlapRemoval(muonsCombined, jets, 0.5);
    nMuonsCombined = muonsCombined.size();
    //---------------------------
    // Remove any baseline electron that overlaps with a jet
    electronsTight = overlapRemoval(electronsTight, jets, 0.5);
    nElectronsTight = electronsTight.size();
    
    countCutflowEvent("a_Input");
     
    if( (nMuonsCombined+nElectronsTight) < 2){
      return;
    }
    countCutflowEvent("b_2lep");

    //Check for opposite sign electrons
    bool oppSign = false;
    if(nElectronsTight > 1){
      for(int i=0; i < nElectronsTight; i++){
    for(int j=1; j < nElectronsTight; j++){
      if((i!=j) && (electronsTight[i]->Charge*electronsTight[j]->Charge <0)){
         oppSign = true;
      }
    }
      }
    }

    //Check for opposite sign muons
    if(nMuonsCombined > 1){
      for(int i=0; i < nMuonsCombined; i++){
    for(int j=1; j < nMuonsCombined; j++){
      if((i!=j) && (muonsCombined[i]->Charge*muonsCombined[j]->Charge <0)){
         oppSign = true;
      }
    }
      }
    }
    
    //Check for opposite sign electron and muon
    if((nElectronsTight > 0) && (nMuonsCombined >0)){
      for(int i=0; i < nElectronsTight; i++){
    for(int j=0; j < nMuonsCombined; j++){
      if((i!=j) && (electronsTight[i]->Charge*muonsCombined[j]->Charge <0)){
         oppSign = true;
      }
    }
      }
    }
    countCutflowEvent("c_OppSignLep");
        
    //MET > 180
    if(missingET->P4().Et() < 180.)
      return;
    countCutflowEvent("d_Met180");
    
    //At least five jets
    if(nJets < 5)
      return;
    countCutflowEvent("e_5jets");
    
    //At least 3 b-jets
    int nBJets=0;
    for(int i=0; i < nJets; i++){
      if(checkBTag(jets[i]) == true){
    nBJets += 1;
      }
    }
    if(nBJets < 3)
      return;
    countCutflowEvent("f_3bjets");
    
    //Leading jet rapidity < 1.
    if( fabs(jets[0]->Eta)>1.)
      return;
    countCutflowEvent("g_jet1Rap");
    
    //Second jet rapidity <1.
    if( fabs(jets[1]->Eta)>1.)
      return;
    countCutflowEvent("h_jet2Rap");
    countSignalEvent("SR1");
    
}

void Cms_sus_13_016::finalize() {
  // Whatever should be done after the run goes here
}       
