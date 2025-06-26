#include "cms_2205_09597.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Cms_2205_09597::initialize() {
  setAnalysisName("cms_2205_09597");          
  setInformation(""
    "# search for electroweakinos, hadronic final states\n"
    "# fat jest identified os W, V or H\n"
  "");
  setLuminosity(137.0*units::INVFB);      
  bookSignalRegions("SR_bveto_aggr;SR_bveto_200;SR_bveto_250;SR_bveto_300;SR_bveto_350;SR_bveto_400;SR_bveto_450;SR_bveto_500;SR_bveto_600;SR_bveto_800;SR_WH_aggr;SR_WH_200;SR_WH_250;SR_WH_300;SR_WH_350;SR_WH_400;SR_WH_450;SR_WH_500;SR_WH_600;SR_W_aggr;SR_W_200;SR_W_250;SR_W_300;SR_W_350;SR_W_400;SR_W_450;SR_W_500;SR_W_600;SR_W_800;SR_H_aggr;SR_H_200;SR_H_250;SR_H_300;SR_H_350;SR_H_400;SR_H_450;SR_H_500;SR_H_600;SR_H_800");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  int ifile = bookFile("cms_2205_09597.root", true);
  const char *rootFileName = fNames[ifile].c_str() ;
  hfile = new TFile(rootFileName, "RECREATE", "Saving Histograms");
  mj = new TH1F("", "MJ", 60, 0., 120.);  
  mjsd = new TH1F("", "MJ SD", 60, 0., 120.);  
}

void Cms_2205_09597::analyze() {

  //missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsLoose = filterPhaseSpace(electronsLoose, 5., -2.5, 2.5);
  std::vector<Electron*> electronsPF = filterPhaseSpace(electronsLoose, 10., -2.5, 2.5, false, true);   // =>   pT < 10
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.5, 2.5);
  muonsCombined = filterPhaseSpace(muonsCombined, 5., -2.4, 2.4);
  std::vector<Muon*> muonsPF = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4, false, true);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);  
  photonsMedium = filterPhaseSpace(photonsMedium, 100., -2.4, 2.4);
  
  photonsMedium = filterIsolation( photonsMedium); 
  electronsPF = filterIsolation( electronsPF, 0);
  electronsMedium = Isolate_leptons_with_variable_track_isolation_cone_CMS(electronsMedium, 0.2, 0.05, 10., 0.1);
  muonsPF = filterIsolation( muonsPF, 0);
  muonsCombined = Isolate_leptons_with_variable_track_isolation_cone_CMS(muonsCombined, 0.2, 0.05, 10., 0.2);
  
  jets = filterPhaseSpace(jets, 30., -2.4, 2.4);
  jets = overlapRemoval(jets, electronsMedium, 0.3, "y");
  jets = overlapRemoval(jets, muonsCombined, 0.3, "y");
  jets = overlapRemoval(jets, photonsMedium, 0.3, "y");  
  
  fatjets = filterPhaseSpace(fatjets, 200., -2.0, 2.0);
  
  double HT = 0.;
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta) < 2.4)  HT += jets[i]->PT;
    
  int Nj = jets.size();  
  double met = missingET->P4().Et();
  
  countCutflowEvent("00_all");
  
  if (electronsMedium.size() ) return;
  countCutflowEvent("01_eVeto");   
  
  if (muonsCombined.size() ) return;
  countCutflowEvent("02_muVeto");

  if (muonsPF.size() ) return;
  if (electronsPF.size() ) return;
  countCutflowEvent("03_leptrackVeto"); 
  
  if (met < 200.) return;  
  countCutflowEvent("04_MET>200"); 
  
  if (HT < 300.) return;
  countCutflowEvent("04_HT>300"); 
  
  if (Nj < 2 or Nj > 6 ) return;
  countCutflowEvent("06_Njet"); 
  
  if (photonsMedium.size() ) return;
  countCutflowEvent("07_photVeto");     
  
  bool m50 = false;
  for (int i = 0; i < fatjets.size(); i++) 
    if ( fatjets[i]->Mass > 50.) {m50 = true; break;}
    
  if ( !m50) return;  
  countCutflowEvent("08_fatjet_m>50");     
  
  if ( fabs(jets[0]->P4().DeltaPhi(missingET->P4())) < 1.5) return;
  if ( fabs(jets[1]->P4().DeltaPhi(missingET->P4())) < 0.5) return;
  if ( jets.size() > 2 and fabs(jets[2]->P4().DeltaPhi(missingET->P4())) < 0.3) return;
  if ( jets.size() > 3 and fabs(jets[3]->P4().DeltaPhi(missingET->P4())) < 0.3) return;
  if ( fabs(fatjets[0]->P4().DeltaPhi(missingET->P4())) < 1.5) return;
  if ( fatjets.size() > 1 and fabs(fatjets[1]->P4().DeltaPhi(missingET->P4())) < 0.5) return;
  countCutflowEvent("09_deltaPhi");
  
  if (fatjets.size() < 2) return;
  countCutflowEvent("10_Nfatjet");
  
  std::vector<Jet*> bjets;
  for (int i = 0; i < jets.size(); i++) 
    if (checkBTag(jets[i]) ) 
      bjets.push_back(jets[i]);
    
  if (bjets.size() > 0 ) {
    int Wtag = 0;
    int Htag = 0;
    for (int i = 0; i < fatjets.size(); i++) {
      if (fatjets[i]->Mass > 55. and fatjets[i]->Mass < 130. ) {
        bool btag = false;
        for (int j = 0; j < bjets.size(); j++)
          if (bjets[j]->P4().DeltaR( fatjets[i]->P4() ) < 0.8) btag = true;
        if ( !btag and checkWtag(fatjets[i])) Wtag++;      
      }
      if (fatjets[i]->Mass > 55. and fatjets[i]->Mass < 160. ) {
        bool btag = false;
        for (int j = 0; j < bjets.size(); j++)
          if (bjets[j]->P4().DeltaR( fatjets[i]->P4() ) < 0.8) btag = true;
        if ( btag and checkBBtag(fatjets[i])) Htag++;      
      }      
    }
    if (Wtag and Htag) {
      countCutflowEvent("12_WHSR");
      countSignalEvent("SR_WH_aggr");
      if ( met < 250.) countSignalEvent("SR_WH_200");
      else if ( met < 300.) countSignalEvent("SR_WH_250");
      else if ( met < 350.) countSignalEvent("SR_WH_300");
      else if ( met < 400.) countSignalEvent("SR_WH_350");
      else if ( met < 450.) countSignalEvent("SR_WH_400");
      else if ( met < 500.) countSignalEvent("SR_WH_450");
      else if ( met < 600.) countSignalEvent("SR_WH_500");
      else countSignalEvent("SR_WH_600");
    }
    if (Wtag and !Htag) {
      countCutflowEvent("13_WSR");
      countSignalEvent("SR_W_aggr");
      if ( met < 250.) countSignalEvent("SR_W_200");
      else if ( met < 300.) countSignalEvent("SR_W_250");
      else if ( met < 350.) countSignalEvent("SR_W_300");
      else if ( met < 400.) countSignalEvent("SR_W_350");
      else if ( met < 450.) countSignalEvent("SR_W_400");
      else if ( met < 500.) countSignalEvent("SR_W_450");
      else if ( met < 600.) countSignalEvent("SR_W_500");
      else if ( met < 800.) countSignalEvent("SR_W_600");
      else countSignalEvent("SR_W_800");
    }
    if (!Wtag and Htag) {
      countCutflowEvent("14_HSR");
      countSignalEvent("SR_H_aggr");
      if ( met < 250.) countSignalEvent("SR_H_200");
      else if ( met < 300.) countSignalEvent("SR_H_250");
      else if ( met < 350.) countSignalEvent("SR_H_300");
      else if ( met < 400.) countSignalEvent("SR_H_350");
      else if ( met < 450.) countSignalEvent("SR_H_400");
      else if ( met < 500.) countSignalEvent("SR_H_450");
      else if ( met < 600.) countSignalEvent("SR_H_500");
      else if ( met < 800.) countSignalEvent("SR_H_600");
      else countSignalEvent("SR_H_800");
    }    
  }
  else {
    int Vtag = 0;
    int Wtag = 0;
    for (int i = 0; i < fatjets.size(); i++)  {
      //if (fatjets[i]->Mass > 55. and fatjets[i]->Mass < 130. ) 
      //if (fatjets[i]->SoftDroppedP4[0].M() > 65. and fatjets[i]->SoftDroppedP4[0].M() < 105. )   
      mj->Fill( fatjets[i]->Mass,weight);
      mjsd->Fill( fatjets[i]->SoftDroppedP4[0].M(),weight);
      if (fatjets[i]->Mass > 65. and fatjets[i]->Mass < 105. )     
        if (checkWtag(fatjets[i])) Wtag++;
        else if (checkVtag(fatjets[i])) Vtag++;
    }
      
    if ( Vtag + Wtag < 2 ) return; 
    countCutflowEvent("11_bveto");
    countSignalEvent("SR_bveto_aggr");
    if ( met < 250.) countSignalEvent("SR_bveto_200");
    else if ( met < 300.) countSignalEvent("SR_bveto_250");
    else if ( met < 350.) countSignalEvent("SR_bveto_300");
    else if ( met < 400.) countSignalEvent("SR_bveto_350");
    else if ( met < 450.) countSignalEvent("SR_bveto_400");
    else if ( met < 500.) countSignalEvent("SR_bveto_450");
    else if ( met < 600.) countSignalEvent("SR_bveto_500");
    else if ( met < 800.) countSignalEvent("SR_bveto_600");
    else countSignalEvent("SR_bveto_800");
  }
  
}

void Cms_2205_09597::finalize() {
  // Whatever should be done after the run goes here

  hfile->Write();

  hfile->Close();
}       

bool Cms_2205_09597::checkWtag(Jet* jet) {
  
  for(int i = 0; i < true_particles.size(); ++i)
    if( fabs(true_particles[i]->PID) == 24 and fabs(true_particles[true_particles[i]->D1]->PID) != 24 ) 
      //if ( true_particles[i]->P4().DeltaR(jet->P4()) < 0.8 and rand()/(RAND_MAX+1.) < 0.5 ) return true;
      if ( true_particles[i]->P4().DeltaR(jet->P4()) < 0.8 and rand()/(RAND_MAX+1.) < 0.55 ) return true;
  return false;    
}

bool Cms_2205_09597::checkVtag(Jet* jet) {
  
  for(int i = 0; i < true_particles.size(); ++i)
    if( (fabs(true_particles[i]->PID) == 24 and fabs(true_particles[true_particles[i]->D1]->PID) != 24) or
      ( true_particles[i]->PID == 23 and true_particles[true_particles[i]->D1]->PID != 23)
    ) 
      if ( true_particles[i]->P4().DeltaR(jet->P4()) < 0.8 and rand()/(RAND_MAX+1.) < 0.45 ) return true;
  
  return false;    
}

bool Cms_2205_09597::checkBBtag(Jet* jet) {
  
  for(int i = 0; i < true_particles.size(); ++i)
    if( ( true_particles[i]->PID == 25 and abs(true_particles[true_particles[i]->D1]->PID) == 5) or
      ( true_particles[i]->PID == 23 and abs(true_particles[true_particles[i]->D1]->PID) == 5)
    ) 
      if ( true_particles[i]->P4().DeltaR(jet->P4()) < 1.0 and rand()/(RAND_MAX+1.) < 0.8 ) return true;
  
  return false;    
}
