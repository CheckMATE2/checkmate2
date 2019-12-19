#include "cms_susy_displaced_leptons_8tev.h"
// AUTHOR: Mangesh Sonawane
//  EMAIL: son.arnav95@gmail.com

void Cms_susy_displaced_leptons_8tev::initialize() {
  setAnalysisName("cms_susy_displaced_leptons_8tev");          
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

  xsec = 85.6; //fb
  i_lumi = 19.7; //fb-1

  SR1 = 0., SR2 = 0., SR3 = 0.;

  //Debug output file
  // debug.open("debug.txt");
  // signal.open("signal.txt");

  //Define histograms
  // n_elec = new TH1F("n_e", "n_e", 10, 0, 10);
  // n_muon = new TH1F("n_mu", "n_mu", 10, 0, 10);
  // n_ta = new TH1F("n_tau", "n_tau", 10, 0, 10);

  //TFile
  // hists = new TFile("hists.root","recreate");

  elec_d0_eff.clear();
  muon_d0_eff.clear();
  elec_pt_eff.clear();
  muon_pt_eff.clear();

  int isinit = 0;

  isinit = isinit*initEff("8TeV_eff/elec-eff.dat", elec_d0_eff);
  isinit = isinit*initEff("8TeV_eff/elec-pt-eff.dat", elec_pt_eff);
  isinit = isinit*initEff("8TeV_eff/muon-eff.dat", muon_d0_eff);
  isinit = isinit*initEff("8TeV_eff/muon-pt-eff.dat", muon_pt_eff);

  if (!isinit) {
    cout << endl << "Efficiency file read error!!!";
    // debug << endl << "Efficiency file read error!!!";
    return;
  }
}

//User defined functions;

int Cms_susy_displaced_leptons_8tev::initEff(string filename, vector< vector<double> > &arrayname){

  ifstream file_eff(filename);

  if(!file_eff.is_open()) return 0;
  
  double val;
  int count = 0;
  vector<double> eff;
  while ( file_eff >> val){
    count++;
    eff.push_back(val);
    if (count==4) {
      count=0; 
      arrayname.push_back(eff);
      eff.clear();
    }
  }

  return 1;
}

double Cms_susy_displaced_leptons_8tev::getEff(double value, vector< vector<double> > &arrayname){

  if(arrayname.size() < 1) return 0.0;

  double eff = 0.;
  int arrSize = arrayname.size();
  int i = 0;
  for (; i < arrSize; i++){
    eff = arrayname[i][3];
    if(value <= arrayname[i][2]) break;
  }

  // Interpolate
  // if (i > 0 && i < arrSize) 
  //  eff = arrayname[i-1][3] + (value - arrayname[i-1][2]) *
  //    (arrayname[i][3]-arrayname[i-1][3]) / (arrayname[i][2] - arrayname[i-1][2]);

  if(eff > 1.0) cout << "Returning Eff = " << eff 
         << "  for value = " << value 
         << endl; 

  return eff;
} 

bool Cms_susy_displaced_leptons_8tev::lep_selection(GenParticle* part, int ID, double pT, double eta, bool overlap){
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

bool Cms_susy_displaced_leptons_8tev::is_isolated(GenParticle* lep, vector <GenParticle*> &stable, double epsilon, double dR_cone)
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

bool Cms_susy_displaced_leptons_8tev::is_isolated_from_jet(GenParticle* lep, vector <Jet*> &jets, double dR_cone)
{
  // TLorentzVector p1, p2;
  // p1.SetPtEtaPhiE(lep->PT,lep->Eta,lep->Phi,lep->E);

  //loop over all jets
  for (int i = 0; i < jets.size(); ++i){
    // p2.SetPtEtaPhiE(jets[i]->PT,jets[i]->Eta,jets[i]->Phi,jets[i]->Mass);
    double dR = lep->P4().DeltaR(jets[i]->P4());
    bool flag = false;

    if (dR < dR_cone){ 

      return false;
    }
  }//End loop over jets   

  return true;
}

void Cms_susy_displaced_leptons_8tev::analyze() {
  EventCount++;

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  // debug << endl << true_particles size: " << true_particles.size() << endl;

  finalleptons.clear();
  stable.clear();
  el.clear();
  mu.clear();

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
      // if (abs(M->PID) >= 1000000) exotic.push_back(M);
    }// end electron check block

    if (abs(part->PID)==13 && part->PT > 22.){
      n_mu++;
      finalleptons.push_back(part);
      // if (abs(M->PID) >= 1000000) exotic.push_back(M);
    }// end muon check block

    if (abs(part->PID)==15){
      n_tau++;
      finalleptons.push_back(part);
      // if (abs(M->PID) >= 1000000) exotic.push_back(M);
    }// end tau check block

  }//end loop over all true particles

  // n_elec->Fill(n_e);
  // n_muon->Fill(n_mu);
  // n_ta->Fill(n_tau);

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
      isol = is_isolated(lep, stable, 0.15, 0.4);

      if (isol) mu.push_back(lep);
    }//end muon selection block


  }//End Loop over stable lepton candidates

  if (el.size()!=1 || mu.size()!=1) return;

  countCutflowEvent("Cut 2: Exactly 1 e and 1 mu");

  if (el[0]->Charge*mu[0]->Charge != -1) return;
  countCutflowEvent("Cut 3: e mu opposite sign");


  //jet overlap removal
  jets = overlapRemoval(jets, el, 0.1);
  jets = overlapRemoval(jets, mu, 0.1);

  // Check isolation from jet
  if (!is_isolated_from_jet(el[0],jets,0.5)) return;
  if (!is_isolated_from_jet(mu[0],jets,0.5)) return;
  countCutflowEvent("Cut 4: Jet-Lepton isolation dR > 0.5");

  double dR = el[0]->P4().DeltaR(mu[0]->P4());

  if (dR < 0.5) return;
  countCutflowEvent("Cut 5: Electron muon separation dR > 0.5");

  //Preselection complete

  double e_d0_eff = getEff(el[0]->D0, elec_d0_eff);
  double e_pt_eff = getEff(el[0]->PT, elec_pt_eff);
  double mu_d0_eff = getEff(mu[0]->D0, muon_d0_eff);
  double mu_pt_eff = getEff(mu[0]->PT, muon_pt_eff);

  double evt_weight = 0.95*e_d0_eff*e_pt_eff*mu_d0_eff*mu_pt_eff;

  if (fabs(el[0]->D0) > 20. || fabs(mu[0]->D0) > 20.) return;
  if (fabs(el[0]->D0) < 0.2 || fabs(mu[0]->D0) < 0.2) return;

  countCutflowEvent("Cut 6: lepton d0 bounds [0.2, 20] mm");

  // double rno = rand()/(RAND_MAX+1.);

  // if (evt_weight > rno) return;

  // countCutflowEvent("Cut 7: Combined efficiency cut");

  weight = weight*evt_weight*BR;


  if (fabs(el[0]->D0) > 1. && fabs(mu[0]->D0) > 1.) {SR3+=evt_weight; countSignalEvent("SR3");}
  else if (fabs(el[0]->D0) > 0.5 && fabs(mu[0]->D0) > 0.5) {SR2+=evt_weight; countSignalEvent("SR2");}
  else if (fabs(el[0]->D0) > 0.2 && fabs(mu[0]->D0) > 0.2) {SR1+=evt_weight; countSignalEvent("SR1");}

}

void Cms_susy_displaced_leptons_8tev::finalize() {
  // Whatever should be done after the run goes here

  // signal << "Output Signal regions" << endl;
  // signal << endl << "SR1: \t" << SR1*xsec*i_lumi*2./3 /EventCount;
  // signal << endl << "SR2: \t" << SR2*xsec*i_lumi*2./3 /EventCount;
  // signal << endl << "SR3: \t" << SR3*xsec*i_lumi*2./3 /EventCount;

  // hists->cd();
  // n_elec->Write();
  // n_muon->Write();
  // n_ta->Write();

  //Closing files
  // hists->Close();
  // debug.close();
  // signal.close();
}       


