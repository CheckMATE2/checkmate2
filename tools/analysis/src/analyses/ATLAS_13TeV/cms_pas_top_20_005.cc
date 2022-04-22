#include "cms_pas_top_20_005.h"
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
void Cms_pas_top_20_005::initialize() {
  setAnalysisName("cms_pas_top_20_005");          
  setInformation(""
    "# CP violation in top pair production\n"
  "");
  setLuminosity(137.0*units::INVFB);      
  bookSignalRegions("dummy");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  int ifile = bookFile("cms_pas_top_20_005.root", true);
  const char *rootFileName = fNames[ifile].c_str() ;
  hfile = new TFile(rootFileName, "RECREATE", "Saving Histograms");
  chi2_hist = new TH1F("", "chi2", 50, 0., 150.);
  chi2_hist2 = new TH1F("", "chi2", 50, 0., 150.);  
}

void Cms_pas_top_20_005::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsMedium = filterPhaseSpace(electronsMedium, 30., -2.4, 2.4);
  electronsMedium = filterIsolation(electronsMedium);
  muonsCombined = filterPhaseSpace(muonsCombined, 30., -2.4, 2.4);
  muonsCombined = filterIsolation(muonsCombined);
  jets = filterPhaseSpace(jets, 30., -2.4, 2.4);
  
  jets = overlapRemoval(jets, electronsMedium, 0.4);
  jets = overlapRemoval(jets, muonsCombined, 0.4);
  electronsMedium = filterPhaseSpace(electronsMedium, 38., -2.4, 2.4);
  
  std::vector<Jet*> bjets;
  std::vector<Jet*> lightjets;
  
  for (int i = 0; i < jets.size(); i++) 
    if ( checkBTag(jets[i]) ) bjets.push_back(jets[i]); 
    else lightjets.push_back(jets[i]);  
  
  countCutflowEvent("00_all");  
    
  if ( electronsMedium.size() + muonsCombined.size() != 1 ) return; 
  countCutflowEvent("01_1lepton");  
  
  TLorentzVector lepton(0.,0.,0.,0.);
  int flavour = -1;
  if ( electronsMedium.size() == 1 ) {
      lepton = electronsMedium[0]->P4();
      flavour = 0;
  }
  else {
      lepton = muonsCombined[0]->P4();
      flavour = 1;
  }      
  
  if ( bjets.size() != 2 ) return;
  countCutflowEvent("02_2bjets");  
  
  if (lightjets.size() < 2) return;
  countCutflowEvent("03_lightjets");
  
  double chi2 = 1000.;
  double sigmat = sqrt(172.5);
  double mt = 172.5;
  double sigmaw = sqrt(80.4);
  double mw = 80.4;
  double mjj = (lightjets[0]->P4() + lightjets[1]->P4()).M();
  int pick = -1;
  for (int i = 0; i < bjets.size(); i++) {
      double mjjb = (bjets[i]->P4() + lightjets[0]->P4() + lightjets[1]->P4()).M();
      double chi2_temp = pow( (mjjb - mt)/sigmat, 2) + pow( (mjj - mw)/sigmaw, 2);
      if (chi2_temp < chi2) {
          chi2 = chi2_temp;
          pick = i;
      }
  }
  chi2_hist->Fill(chi2);
  chi2_hist2->Fill(chi2, weight);
  if (chi2 > 20.) return;
  countCutflowEvent("04_chi2<20");
  
  if (pick = 0 and (bjets[1]->P4() + lepton).M() > 150.  ) return; 
  if (pick = 1 and (bjets[0]->P4() + lepton).M() > 150.  ) return; 
  countCutflowEvent("05_Mbl<150");
  
  GenParticle* top;
  int ntop = 0;
  for(int i = 0; i < true_particles.size(); ++i)   //cout <<  true_particles[i]->PID << "   " << true_particles[i]->PT << "\n";;           
    if( true_particles[i]->PID == 6 and true_particles[true_particles[i]->D1]->PID != 6 ) {
      ntop++;
      top = true_particles[i];
    }
    
  GenParticle* antitop;
  int nantitop = 0;
  for(int i = 0; i < true_particles.size(); ++i)   //cout <<  true_particles[i]->PID << "   " << true_particles[i]->PT << "\n";;           
    if( true_particles[i]->PID == -6 and true_particles[true_particles[i]->D1]->PID != -6 ) {
      nantitop++;
      antitop = true_particles[i];
    }    
  
  //cout << ntop << "   " << nantitop << endl;
  
}

void Cms_pas_top_20_005::finalize() {
  // Whatever should be done after the run goes here
  TCanvas can1;
  chi2_hist->Draw();
  chi2_hist->SetTitle("Chi2 minimum score");
  can1.Close();

  TCanvas can2;
  chi2_hist2->Draw();
  chi2_hist2->SetTitle("Chi2 minimum score");
  can2.Close();  
  
  hfile->Write();

  hfile->Close();
}       
