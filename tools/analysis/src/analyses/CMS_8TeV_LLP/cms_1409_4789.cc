#include "cms_1409_4789.h"
// AUTHOR: Mangesh Sonawane
//  EMAIL: son.arnav95@gmail.com

void Cms_1409_4789::initialize() {
  setAnalysisName("cms_1409_4789");          
  setInformation(""
    "# CMS\n"
    "# SUSY\n"
    "# displaced leptons\n"
    "# e/mu\n"
    "# 8TeV\n"
  "");
  setLuminosity(19.7*units::INVFB);      
  bookSignalRegions("SR3;SR2;SR1");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here

  EventCount = 0;
  n_e = 0, n_mu = 0, n_tau = 0;

  BR = 2./3;

  SR1 = 0., SR2 = 0., SR3 = 0.;

  eff_file = TFile::Open("../data/tables/cms_1409_4789.root");
  if(!eff_file->IsOpen()){
    cout << "Error: Unable to open file!!!";
  }

  elec_d0_eff_hist = (TH1F*) eff_file->Get("elec_d0_eff");
  elec_pt_eff_hist = (TH1F*) eff_file->Get("elec_pt_eff");
  muon_d0_eff_hist = (TH1F*) eff_file->Get("muon_d0_eff");
  muon_pt_eff_hist = (TH1F*) eff_file->Get("muon_pt_eff");
}

//User defined functions;

bool Cms_1409_4789::lep_selection(GenParticle* part, int ID, double pT, double eta, bool overlap){
  if (abs(part->PID) != ID) return false;
  if (part->PT < pT) return false;
  if (fabs(part->Eta) > eta) return false;
  if (fabs(part->Z) > 300.) return false;
  double v0 = sqrt(part->X*part->X + part->Y*part->Y);
  if (v0 > 40.) return false;
  if (overlap){
    if (fabs(part->Eta) > 1.44 && fabs(part->Eta) < 1.56) return false;
  }
  return true;
}

bool Cms_1409_4789::is_isolated(GenParticle* lep, vector <GenParticle*> &stable, double epsilon, double dR_cone)
{
  bool isol = false;

  TLorentzVector p1, p2;
  p1.SetPtEtaPhiE(lep->PT,lep->Eta,lep->Phi,lep->E);
  double pT_sum=0;

  //loop over all stable particles
  for (int i = 0; i < stable.size(); ++i){

    if (stable[i]->PT < 0.1) continue;   
    if (abs(stable[i]->PID) == 12 || abs(stable[i]->PID) == 14 || abs(stable[i]->PID) == 16) continue;                  //minimum pt for particle to be considered for isolation

    p2.SetPtEtaPhiE(stable[i]->PT,stable[i]->Eta,stable[i]->Phi,stable[i]->E);
    double dR = p1.DeltaR(p2);

    if (dR >= dR_cone) continue;                          //skip particles outside cone
    if (dR == 0) continue;                            //skip same particle

    pT_sum+= stable[i]->PT;                           //scalar pT sum
  }

  if (pT_sum < (lep->PT*epsilon)) isol = true;      

  return isol;
}

bool Cms_1409_4789::is_isolated_from_jet(GenParticle* lep, vector <Jet*> &jets, double dR_cone)
{

  //loop over all jets
  for (int i = 0; i < jets.size(); ++i){
    double dR = lep->P4().DeltaR(jets[i]->P4());
    bool flag = false;

    if (dR < dR_cone){ 

      return false;
    }
  }//End loop over jets   

  return true;
}

void Cms_1409_4789::analyze() {
  EventCount++;

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  finalleptons.clear();
  stable.clear();
  el.clear();
  mu.clear();
  gen_jets.clear();

  //loop over all true particles
  for (int i = 0 ; i < true_particles.size(); ++i){
    auto part = true_particles[i];

    if (part->Status != 1) continue;        //Run over only stable particles

    stable.push_back(part);

    auto M_index = part->M1;

    auto M = part;

    //Working backwards from final state particle to get exotic mother
    while (M_index >= 0){               //Break the loop if M_index is -1
      M = (GenParticle*)true_particles[M_index];
      if (abs(M->PID) == 5) break;          //Break loop if stable particle emerges from b quark.
      if (abs(M->PID) >= 1000000) break;        //Break the loop if exotic particle
      else M_index = M->M1;
    }

    if (M_index < 0) continue;          

    if (abs(M->PID) < 1000000) continue;         //Disregard final state particle if it does not originate from exotic particle

    // finaldaughters.push_back(part);           //store final state daughter if originating from an exotic
    if (abs(part->PID)==11 && part->PT > 22.){  
      n_e++;
      finalleptons.push_back(part);

    }// end electron check block

    if (abs(part->PID)==13 && part->PT > 22.){
      n_mu++;
      finalleptons.push_back(part);

    }// end muon check block

    if (abs(part->PID)==15){
      n_tau++;
      finalleptons.push_back(part);

    }// end tau check block

  }//end loop over all true particles

  if (n_e < 1 || n_mu < 1) return;
  countCutflowEvent("Cut 1: At lease 1 e and mu");

  //Loop over stable lepton candidates
  for (int i = 0; i < finalleptons.size(); ++i){
    auto lep = finalleptons[i];

    //Selection of electron candidates
    if (lep_selection(lep, 11, 25, 2.5, true)){
      bool isol = false;

      //Implementing electron isolation
      if (fabs(lep->Eta < 1.44)) isol = is_isolated(lep, stable, 0.035, 0.3);
      if (fabs(lep->Eta > 1.56)) isol = is_isolated(lep, stable, 0.065, 0.3);

      if (isol) el.push_back(lep);
    }//end electron selection block

    //Selection of muon candidates
    if (lep_selection(lep, 13, 25, 2.5)){
      bool isol = false;

      //Implementing muon isolation
      isol = is_isolated(lep, stable, 0.15, 0.5);

      if (isol) mu.push_back(lep);
    }//end muon selection block


  }//End Loop over stable lepton candidates

  if (el.size()!=1 || mu.size()!=1) return;

  countCutflowEvent("Cut 2: Exactly 1 e and 1 mu");

  if (el[0]->Charge*mu[0]->Charge != -1) return;
  countCutflowEvent("Cut 3: e mu opposite sign");

  // //Loop over gen jets
  for (int i = 0; i < genjets.size(); ++i){
  	Jet* jet = (Jet*) genjets[i];
  	if (jet->PT > 10.) gen_jets.push_back(jet);
  }


  //jet overlap removal
  gen_jets = overlapRemoval(gen_jets, el, 0.01);
  gen_jets = overlapRemoval(gen_jets, mu, 0.01);

  // Check isolation from jet
  if (!is_isolated_from_jet(el[0],gen_jets,0.5)) return;
  if (!is_isolated_from_jet(mu[0],gen_jets,0.5)) return;
  
  countCutflowEvent("Cut 4: Jet-Lepton isolation dR > 0.5");

  double dR = el[0]->P4().DeltaR(mu[0]->P4());

  if (dR < 0.5) return;
  countCutflowEvent("Cut 5: Electron muon separation dR > 0.5");

  //Preselection complete

  double e_d0 = fabs(el[0]->Px*el[0]->Y - el[0]->Py*el[0]->X)/el[0]->PT;
  double mu_d0 = fabs(mu[0]->Px*mu[0]->Y - mu[0]->Py*mu[0]->X)/mu[0]->PT;

  double e_d0_eff2 = elec_d0_eff_hist->GetBinContent(elec_d0_eff_hist->FindBin(e_d0));
  double e_pt_eff2 = elec_pt_eff_hist->GetBinContent(elec_pt_eff_hist->FindBin(el[0]->PT));
  double mu_d0_eff2 = muon_d0_eff_hist->GetBinContent(muon_d0_eff_hist->FindBin(mu_d0));
  double mu_pt_eff2 = muon_pt_eff_hist->GetBinContent(muon_pt_eff_hist->FindBin(mu[0]->PT));


  double evt_weight = 0.95*e_d0_eff2*e_pt_eff2*mu_d0_eff2*mu_pt_eff2;

  if (fabs(e_d0) > 20. || fabs(mu_d0) > 20.) return;
  if (fabs(e_d0) < 0.2 || fabs(mu_d0) < 0.2) return;

  countCutflowEvent("Cut 6: lepton d0 bounds [0.2, 20] mm");

  weight = weight*evt_weight*BR;


  if (fabs(e_d0) > 1. && fabs(mu_d0) > 1.) {SR3+=evt_weight; countSignalEvent("SR3");}
  else if (fabs(e_d0) > 0.5 && fabs(mu_d0) > 0.5) {SR2+=evt_weight; countSignalEvent("SR2");}
  else if (fabs(e_d0) > 0.2 && fabs(mu_d0) > 0.2) {SR1+=evt_weight; countSignalEvent("SR1");}

}

void Cms_1409_4789::finalize() {
  // Whatever should be done after the run goes here

}       


