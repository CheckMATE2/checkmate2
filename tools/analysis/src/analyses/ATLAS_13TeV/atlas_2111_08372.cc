#include "atlas_2111_08372.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_2111_08372::initialize() {
  setAnalysisName("atlas_2111_08372");          
  setInformation(""
    "# associated production of a Z\n"
    "#  boson with an invisibly decaying Higgs boson or dark matter candidates \n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("SR1;mT_0200;mT_0250;mT_0300;mT_0350;mT_0400;mT_0450;mT_0500;mT_0550;mT_0600;mT_0650;mT_0700;mT_0750;mT_0800;mT_0850;mT_0900;mT_0950;mT_1000;mT_1100;mT_1200;mT_1300;mT_1400;mT_1600");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  
  int ifile = bookFile("atlas_2111_08372.root", true);
  const char *rootFileName = fNames[ifile].c_str() ;
  hfile = new TFile(rootFileName, "RECREATE", "Saving Histograms");
  mT_hist = new TH1F("mt", "mT", 25, 150., 1650.);
  MET_hist = new TH1F("met", "MET", 20, 90., 290.);  
  mT_MET = new TH2D("mt_MET", "mt vs MET", 25, 150., 1650., 20, 90., 290. );
}

void Atlas_2111_08372::analyze() {
  missingET->addMuons(muonsCombined);
  
  electronsLoose = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47);
  electronsMedium = filterPhaseSpace(electronsMedium, 7., -2.47, 2.47);
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 7., -2.5, 2.5);
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  
  electronsLoose = Isolate_leptons_with_inverse_track_isolation_cone(electronsLoose, tracks, towers, 0.2, 10., 0.2, 0.2, 0.15, true);
  electronsMedium = Isolate_leptons_with_inverse_track_isolation_cone(electronsMedium, tracks, towers, 0.2, 10., 0.2, 0.1, 0.1, true);
  electronsLoose = overlapRemoval( electronsLoose, electronsMedium, 0.05);
  std::vector<Muon*> PFmuonsLoose = PFisolation(muonsCombinedPlus, tracks, towers, 0.3, 10., 0.2, 0.16);
  std::vector<Muon*> PFmuonsTight = PFisolation(muonsCombinedPlus, tracks, towers, 0.3, 10., 0.2, 0.12);
  PFmuonsLoose = overlapRemoval( PFmuonsLoose, PFmuonsTight, 0.05);
  
  jets = overlapRemoval(jets, electronsLoose, 0.2);
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  jets = overlapRemoval(jets, PFmuonsLoose, 0.2);
  jets = overlapRemoval(jets, PFmuonsTight, 0.2);
  electronsLoose = overlapRemoval(electronsLoose, jets, 0.4);
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  PFmuonsLoose = overlapRemoval(PFmuonsLoose, jets, 0.4);
  PFmuonsTight = overlapRemoval(PFmuonsTight, jets, 0.4);

  countCutflowEvent("00_all");
  
  //cout << "MET: " << missingET->P4().X() << " " << missingET->P4().Y() << "\n";
  //for (int t = 0; t < true_particles.size(); t++ ) 
  //  if ( abs(true_particles[t]->PID) == 52 and true_particles[t]->Status == 1 ) 
  //    cout << true_particles[t]->P4().X() << " " << true_particles[t]->P4().Y() << '\n'; 
  
  std::string lepton = "?";
  bool ossf = false;
  double mll = 0.;
  double Rll = 10.;
  TLorentzVector pll;
  if (electronsMedium.size() > 1 and electronsMedium[1]->PT > 20. and electronsMedium[0]->PT > 30.) {
    lepton = "el";
    if (electronsMedium[0]->Charge * electronsMedium[1]->Charge < 0) ossf = true;
    pll = electronsMedium[0]->P4() + electronsMedium[1]->P4();
    mll = (electronsMedium[0]->P4() + electronsMedium[1]->P4()).M();
    Rll = electronsMedium[0]->P4().DeltaR(electronsMedium[1]->P4());
  }
  else if (PFmuonsTight.size() > 1 and PFmuonsTight[1]->PT > 20. and PFmuonsTight[0]->PT > 30.) {
    lepton = "mu";
    if (PFmuonsTight[0]->Charge * PFmuonsTight[1]->Charge < 0) ossf = true;
    pll = PFmuonsTight[0]->P4() + PFmuonsTight[1]->P4();
    mll = (PFmuonsTight[0]->P4() + PFmuonsTight[1]->P4()).M();
    Rll = PFmuonsTight[0]->P4().DeltaR(PFmuonsTight[1]->P4());
  }
  
  if (lepton == "?") return;
  countCutflowEvent("01_"+lepton+"_2SFleptons");
  
  if (electronsMedium.size() + PFmuonsTight.size() > 2) return;
  if (electronsLoose.size() or PFmuonsLoose.size() ) return;
  countCutflowEvent("02_"+lepton+"_3lepveto");
    
  if (!ossf) return;
  countCutflowEvent("03_"+lepton+"_OSSF");
  
  if (mll < 76. or mll > 106.) return;
  countCutflowEvent("04_"+lepton+"_mll");
  
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta) < 2.5 && checkBTag(jets[i]) ) return;
  countCutflowEvent("05_"+lepton+"_bveto");
  
  double met = missingET->P4().Et();
  if (met < 90.) return;
  countCutflowEvent("06_"+lepton+"_MET");
    
  if (Rll > 1.8) return;
  countCutflowEvent("06_"+lepton+"_Rll");
  
  double HT = 0.;
  for (int i = 0; i < jets.size(); i++) {
    HT += jets[i]->PT;
    //cout << "jet: " << jets[i]->P4().X() << " " << jets[i]->P4().Y() << '\n'; 
  }
  
  if ( HT != 0 and met/sqrt(HT) < 9.) return;
  countCutflowEvent("07_"+lepton+"_METsig");
  
  countSignalEvent("SR1");
  
  double mZ2 = 91.1876*91.1876;
  double mt = sqrt( pow(sqrt(mZ2 + pll.Perp2()) + sqrt(mZ2 + met*met), 2) - ( pow(pll.X() + missingET->P4().X(), 2) + pow(pll.Y() + missingET->P4().Y(), 2) ) );
  
  mT_hist->Fill(mt, weight);
  MET_hist->Fill(met, weight);
  mT_MET->Fill(mt, met, weight);
  
  if (mt < 200.) return;
  countCutflowEvent("08_"+lepton+"_mT");
  
  
  if (mt < 250.) countSignalEvent("mT_0200");
  else if (mt < 300.) countSignalEvent("mT_0250");
  else if (mt < 350.) countSignalEvent("mT_0300");
  else if (mt < 400.) countSignalEvent("mT_0350");
  else if (mt < 450.) countSignalEvent("mT_0400");
  else if (mt < 500.) countSignalEvent("mT_0450");
  else if (mt < 550.) countSignalEvent("mT_0500");
  else if (mt < 600.) countSignalEvent("mT_0550");
  else if (mt < 650.) countSignalEvent("mT_0600");
  else if (mt < 700.) countSignalEvent("mT_0650");
  else if (mt < 750.) countSignalEvent("mT_0700");
  else if (mt < 800.) countSignalEvent("mT_0750");
  else if (mt < 850.) countSignalEvent("mT_0800");
  else if (mt < 900.) countSignalEvent("mT_0850");
  else if (mt < 950.) countSignalEvent("mT_0900");
  else if (mt < 1000.) countSignalEvent("mT_0950");
  else if (mt < 1100.) countSignalEvent("mT_1000");
  else if (mt < 1200.) countSignalEvent("mT_1100");
  else if (mt < 1300.) countSignalEvent("mT_1200");
  else if (mt < 1400.) countSignalEvent("mT_1300");
  else if (mt < 1600.) countSignalEvent("mT_1400");
  else countSignalEvent("mT_1600");
  
  return;
  
}

void Atlas_2111_08372::finalize() {
  // Whatever should be done after the run goes here
  hfile->Write();
  hfile->Close();
}       


template <class X>
std::vector<X*> Atlas_2111_08372::PFisolation(std::vector<X*> leptons, std::vector<Track*> tracks, std::vector<Tower*> towers, double dR_track_max, double pT_for_inverse_function_track, double dR_tower, double pT_amount) {
  
// see: arXiv:2012.00578
  
  
std::vector<X*> filtered_leptons;
    
for(int i=0;i<leptons.size();i++){
  
  double dR_track = 0.;
  double sumPT = 0.;
  double sumET = 0.;
  
  dR_track = std::min(std::max(pT_for_inverse_function_track/leptons[i]->PT, 0.2), dR_track_max);
        
  for (int t = 0; t < tracks.size(); t++) {
    Track* neighbour = tracks[t];

    // Ignore the lepton's track itself
    if (neighbour->Particle == leptons[i]->Particle) continue;
    if (neighbour->PT < 0.5) continue;
    if (neighbour->P4().DeltaR(leptons[i]->P4()) > dR_track) continue;
    sumPT += neighbour->PT;
  }

  for (int t = 0; t < towers.size(); t++) {
    Tower* neighbour = towers[t];
	    
    // check tower has 'some' momentum and check dR
    if (neighbour->ET < 0.1 || neighbour->P4().DeltaR(leptons[i]->P4()) > dR_tower) continue;
    // Ignore the lepton's tower
    bool candidatesTower = false;//This testing is different from the testing in the tracks case, because to one track there corresponds one particle, but for one tower there is not only one particle.
    for (int p = 0; p < neighbour->Particles.GetEntries(); p++) 
      
      if (neighbour->Particles.At(p) == leptons[i]->Particle) {
        // break the loop and ignore the tower
        candidatesTower = true;
        break;
      }

      if (candidatesTower) continue;
      
      sumET += neighbour->ET;
    }
    
    if ( (leptons[i]->PT)*pT_amount <= sumPT + 0.4 * sumET) continue;
        
    filtered_leptons.push_back(leptons[i]);
}
      
return filtered_leptons;

}
