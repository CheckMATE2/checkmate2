#include "cms_sus_19_005.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Cms_sus_19_005::initialize() {
  setAnalysisName("cms_sus_19_005");          
  setInformation(""
    "# search for new physics with hadronic final states with MT2 variable\n"
    "# only prompt decays\n"
  "");
  setLuminosity(137.0*units::INVFB);      
  bookSignalRegions("2j_loose;2j_tight;4j_loose;4j_tight;7j_loose;7j_tight;10j_loose;10j_tight;2b_loose;2b_tight;3b_loose;3b_tight;4b_loose;4b_tight;7j_3b_loose;7j_3b_tight;7j_4b_loose;7j_4b_tight;10j_4b_loose;10j_4b_tight;Mono-phi;1j_0b_250;1j_0b_350;1j_0b_450;1j_0b_575;1j_0b_700;1j_0b_1000;1j_0b_1200;1j_1b_250;1j_1b_350;1j_1b_450;1j_1b_575;1j_1b_700;2-3j_0b_250_200;2-3j_0b_250_300;2-3j_0b_250_400;2-3j_1b_250_200;2-3j_1b_250_300;2-3j_1b_250_400;2-3j_2b_250_200;2-3j_2b_250_300;2-3j_2b_250_400;2-6j_3b_250_200;2-6j_3b_250_300;2-6j_3b_250_400;4-6j_0b_250_200;4-6j_0b_250_300;4-6j_0b_250_400;4-6j_1b_250_200;4-6j_1b_250_300;4-6j_1b_250_400;4-6j_2b_250_200;4-6j_2b_250_300;4-6j_2b_250_400;7j_0b_250_200;7j_0b_250_300;7j_0b_250_500;7j_1b_250_200;7j_1b_250_300;7j_2b_250_200;7j_2b_250_300;7j_3b_250_200;7j_3b_250_300");
  bookSignalRegions("2-3j_0b_450_200;2-3j_0b_450_300;2-3j_0b_450_400;2-3j_0b_450_500;2-3j_1b_450_200;2-3j_1b_450_300;2-3j_1b_450_400;2-3j_1b_450_500;2-3j_2b_450_200;2-3j_2b_450_300;2-3j_2b_450_400;2-3j_2b_450_500;2-6j_3b_450_200;2-6j_3b_450_300;2-6j_3b_450_400;2-6j_3b_450_500;4-6j_0b_450_200;4-6j_0b_450_300;4-6j_0b_450_400;4-6j_0b_450_500;4-6j_1b_450_200;4-6j_1b_450_300;4-6j_1b_450_400;4-6j_1b_450_500;4-6j_2b_450_200;4-6j_2b_450_300;4-6j_2b_450_400;4-6j_2b_450_500;7j_0b_450_200;7j_0b_450_300;7j_0b_450_400;7j_1b_450_200;7j_1b_450_300;7j_1b_450_400;7j_2b_450_200;7j_2b_450_300;7j_2b_450_400;7j_3b_450_200;7j_3b_450_300;7j_3b_450_400");  
  bookSignalRegions("2-3j_0b_575_200;2-3j_0b_575_300;2-3j_0b_575_400;2-3j_0b_575_500;2-3j_0b_575_600;2-3j_0b_575_700;2-3j_0b_575_800;2-3j_0b_575_900;2-3j_0b_575_1000;2-3j_0b_575_1100;2-3j_1b_575_200;2-3j_1b_575_300;2-3j_1b_575_400;2-3j_1b_575_600;2-3j_1b_575_800;2-3j_1b_575_1000;2-3j_2b_575_200;2-3j_2b_575_300;2-3j_2b_575_400;2-3j_2b_575_600;2-3j_2b_575_800");  
  bookSignalRegions("2-6j_3b_575_200;2-6j_3b_575_300;2-6j_3b_575_400;2-6j_3b_575_600;2-6j_3b_575_800;4-6j_0b_575_200;4-6j_0b_575_300;4-6j_0b_575_400;4-6j_0b_575_500;4-6j_0b_575_600;4-6j_0b_575_700;4-6j_0b_575_800;4-6j_0b_575_900;4-6j_0b_575_1000;4-6j_0b_575_1100;4-6j_1b_575_200;4-6j_1b_575_300;4-6j_1b_575_400;4-6j_1b_575_600;4-6j_1b_575_800;4-6j_1b_575_1000;4-6j_2b_575_200;4-6j_2b_575_300;4-6j_2b_575_400;4-6j_2b_575_600;4-6j_2b_575_800");  
  bookSignalRegions("7-9j_0b_575_200;7-9j_0b_575_300;7-9j_0b_575_400;7-9j_0b_575_600;7-9j_0b_575_800;7-9j_1b_575_200;7-9j_1b_575_300;7-9j_1b_575_400;7-9j_1b_575_600;7-9j_1b_575_800;7-9j_2b_575_200;7-9j_2b_575_300;7-9j_2b_575_400;7-9j_2b_575_600;7-9j_2b_575_800;7-9j_3b_575_200;7-9j_3b_575_300;7-9j_3b_575_400;7-9j_3b_575_600;7-9j_4b_575_200;7-9j_4b_575_300;7-9j_4b_575_400;10j_0b_575_200;10j_0b_575_300;10j_0b_575_500;10j_1b_575_200;10j_1b_575_300;10j_1b_575_500;10j_2b_575_200;10j_2b_575_300;10j_2b_575_500;10j_3b_575_200;10j_3b_575_300;10j_4b_575_200");
  bookSignalRegions("2-3j_0b_1200_200;2-3j_0b_1200_400;2-3j_0b_1200_600;2-3j_0b_1200_800;2-3j_0b_1200_1000;2-3j_0b_1200_1200;2-3j_1b_1200_200;2-3j_1b_1200_400;2-3j_1b_1200_600;2-3j_1b_1200_800;2-3j_1b_1200_1000;2-3j_1b_1200_1200;2-3j_2b_1200_200;2-3j_2b_1200_400;2-3j_2b_1200_600;2-3j_2b_1200_800;2-3j_2b_1200_1000;2-6j_3b_1200_200;2-6j_3b_1200_400;2-6j_3b_1200_600;4-6j_0b_1200_200;4-6j_0b_1200_400;4-6j_0b_1200_600;4-6j_0b_1200_800;4-6j_0b_1200_1000;4-6j_0b_1200_1200;4-6j_1b_1200_200;4-6j_1b_1200_400;4-6j_1b_1200_600;4-6j_1b_1200_800;4-6j_1b_1200_1000;4-6j_1b_1200_1200;4-6j_2b_1200_200;4-6j_2b_1200_400;4-6j_2b_1200_600;4-6j_2b_1200_800;4-6j_2b_1200_1000");  
  bookSignalRegions("7-9j_0b_1200_200;7-9j_0b_1200_400;7-9j_0b_1200_600;7-9j_0b_1200_800;7-9j_0b_1200_1000;7-9j_1b_1200_200;7-9j_1b_1200_400;7-9j_1b_1200_600;7-9j_1b_1200_800;7-9j_1b_1200_1000;7-9j_2b_1200_200;7-9j_2b_1200_400;7-9j_2b_1200_600;7-9j_2b_1200_800;7-9j_3b_1200_200;7-9j_3b_1200_400;7-9j_3b_1200_600;7-9j_4b_1200_200;7-9j_4b_1200_400;10j_0b_1200_200;10j_0b_1200_400;10j_0b_1200_600;10j_1b_1200_200;10j_1b_1200_400;10j_1b_1200_600;10j_2b_1200_200;10j_2b_1200_400;10j_2b_1200_600;10j_3b_1200_200;10j_3b_1200_400;10j_4b_1200_200");
  bookSignalRegions("2-3j_0b_1500_400;2-3j_0b_1500_600;2-3j_0b_1500_800;2-3j_0b_1500_1000;2-3j_0b_1500_1200;2-3j_0b_1500_1400;2-3j_0b_1500_1800;2-3j_1b_1500_400;2-3j_1b_1500_600;2-3j_1b_1500_800;2-3j_1b_1500_1000;2-3j_1b_1500_1200;2-3j_2b_1500_400;2-6j_3b_1500_400;2-6j_3b_1500_600;4-6j_0b_1500_400;4-6j_0b_1500_600;4-6j_0b_1500_800;4-6j_0b_1500_1000;4-6j_0b_1500_1200;4-6j_0b_1500_1400;4-6j_0b_1500_1600;4-6j_1b_1500_400;4-6j_1b_1500_600;4-6j_1b_1500_800;4-6j_1b_1500_1000;4-6j_1b_1500_1400;4-6j_2b_1500_400;4-6j_2b_1500_600;4-6j_2b_1500_800");
  bookSignalRegions("7-9j_0b_1500_400;7-9j_0b_1500_600;7-9j_0b_1500_800;7-9j_0b_1500_1000;7-9j_0b_1500_1400;7-9j_1b_1500_400;7-9j_1b_1500_600;7-9j_1b_1500_800;7-9j_2b_1500_400;7-9j_2b_1500_600;7-9j_2b_1500_800;7-9j_3b_1500_400;7-9j_3b_1500_800;7-9j_4b_1500_400;10j_0b_1500_400;10j_0b_1500_800;10j_1b_1500_400;10j_1b_1500_800;10j_2b_1500_400;10j_3b_1500_400;10j_4b_1500_400");  
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_sus_19_005::analyze() {


  //missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsLoose = filterPhaseSpace(electronsLoose, 5., -2.4, 2.4);
  std::vector<Electron*> electronsPF = filterPhaseSpace(electronsLoose, 10., -2.4, 2.4, false, true);   // =>   pT < 10
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.4, 2.4);
  muonsCombined = filterPhaseSpace(muonsCombined, 5., -2.4, 2.4);
  std::vector<Muon*> muonsPF = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4, false, true);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  
  electronsPF = filterIsolation( electronsPF, 0);
  electronsMedium = Isolate_leptons_with_variable_track_isolation_cone_CMS(electronsMedium, 0.2, 0.05, 10., 0.1);
  muonsPF = filterIsolation( muonsPF, 0);
  muonsCombined = Isolate_leptons_with_variable_track_isolation_cone_CMS(muonsCombined, 0.2, 0.05, 10., 0.2);
  
  countCutflowEvent("00_all");
  if ( muonsCombined.size() + electronsMedium.size() ) return;
  countCutflowEvent("01_lep_veto");
  
  for (int i = 0; i < electronsPF.size(); i++) 
    if ( mT(electronsPF[i]->P4(), missingET->P4() ) < 100. ) return;
    
  for (int i = 0; i < muonsPF.size(); i++) 
    if ( mT(muonsPF[i]->P4(), missingET->P4() ) < 100. ) return;  
    
  countCutflowEvent("02_softlep_veto");
  
  jets = filterPhaseSpace(jets, 20., -4.7, 4.7);
  if (dPhi(jets, 0) < 0.3) return;
  countCutflowEvent("03_dPhi_min");
  
  std::vector<Jet*> bjets;
  std::vector<Jet*> lightjets;
  for (int i = 0; i < jets.size();  i++) 
    if ( fabs(jets[i]->Eta) < 2.4 and checkBTag(jets[i]) ) bjets.push_back(jets[i]);
    else lightjets.push_back(jets[i]);
  
  lightjets = filterPhaseSpace(lightjets, 30., -2.4, 2.4);  
  std::vector<Jet*> signaljets = filterPhaseSpace(jets, 30., -2.4, 2.4); ;  
  
  //int Nj = lightjets.size();
  int Nj = signaljets.size();
  int Nb = bjets.size();
  if ( !Nj ) return;
  countCutflowEvent("04_Nj>=1");
  
  double  HT = 0.;
  TLorentzVector HTmiss = TLorentzVector(0.,0.,0.,0.);
  for (int i = 0; i < lightjets.size(); i++) {
    HT += lightjets[i]->PT;
    HTmiss -= lightjets[i]->P4();
  }
  for (int i = 0; i < bjets.size(); i++) {
    HT += bjets[i]->PT;
    HTmiss -= bjets[i]->P4();
  }
  
  double MET = missingET->P4().Perp();
  if ( MET < 30.) return;
  countCutflowEvent("05_MET<30");
  
  if (  (missingET->P4() - HTmiss).Perp()/MET > 0.5) return;
  countCutflowEvent("06_MET-HTmiss");
  
  if ( HT < 250.) return;
  countCutflowEvent("07_HT>250");
  
  if ( (HT < 1200. or Nj == 1) and MET < 250.) return;
  countCutflowEvent("08_MET>250");
  
  if (Nj > 1 ) {
    std::vector<Jet*> sigjets = lightjets; //same as signaljets
    sigjets.insert(sigjets.end(), bjets.begin(), bjets.end());
    std::sort(sigjets.begin(), sigjets.end(), sortByPT ); 
  
    std::vector<LundCluster> clusters = cluster_jets(sigjets);
    
    /*for (int j = 0; j < 2; j++) {
      cout << "Cluster "+ std::to_string(j) << endl;
      cout << "Momentum: " << clusters[j].P4().E() << ", " << clusters[j].P4().X() << ", " << clusters[j].P4().Y() << ", " << clusters[j].P4().Z() << endl;
      cout << "Momentum: " << clusters[j].P4().Perp()  << endl;
      for (int i = 0; i < clusters[j].constituents.size(); i++) {
        cout << "Jet     : " << clusters[j].constituents[i]->P4().E() << ", " << clusters[j].constituents[i]->P4().X() << ", " << clusters[j].constituents[i]->P4().Y() << ", " << clusters[j].constituents[i]->P4().Z() << ", d1: " << clusters[0].Lund_dist(clusters[j].constituents[i]) << ", d2: " << clusters[1].Lund_dist(clusters[j].constituents[i]) << endl; 
      }
    } */
    
    double MT2 = mT2(clusters[0].P4(), clusters[1].P4(), 0., missingET->P4());
    
    if (MT2 < 200.) return;
    countCutflowEvent("09_MT2>200");
    
    fill_bins_2j(Nj, Nb, HT, MT2);
    
    if ( Nj >= 2  and Nb >= 0 and HT > 1200. and MT2 > 1200.) countSignalEvent("2j_loose");
    if ( Nj >= 2  and Nb >= 0 and HT > 1500. and MT2 > 1400.) countSignalEvent("2j_tight");
    if ( Nj >= 4  and Nb >= 0 and HT > 1200. and MT2 > 1000.) countSignalEvent("4j_loose");
    if ( Nj >= 4  and Nb >= 0 and HT > 1500. and MT2 > 1400.) countSignalEvent("4j_tight");
    if ( Nj >= 7  and Nb >= 0 and HT > 1200. and MT2 >  600.) countSignalEvent("7j_loose");
    if ( Nj >= 7  and Nb >= 0 and HT > 1500. and MT2 >  800.) countSignalEvent("7j_tight");    
    if ( Nj >= 10 and Nb >= 0 and HT > 1200. and MT2 >  400.) countSignalEvent("10j_loose");
    if ( Nj >= 10 and Nb >= 0 and HT > 1500. and MT2 >  600.) countSignalEvent("10j_tight");
    if ( Nj >= 2  and Nb >= 2 and HT > 1200. and MT2 >  600.) countSignalEvent("2b_loose");
    if ( Nj >= 2  and Nb >= 2 and HT > 1500. and MT2 >  600.) countSignalEvent("2b_tight");    
    if ( Nj >= 2  and Nb >= 3 and HT > 1200. and MT2 >  400.) countSignalEvent("3b_loose");
    if ( Nj >= 2  and Nb >= 3 and HT > 1500. and MT2 >  400.) countSignalEvent("3b_tight");       
    if ( Nj >= 2  and Nb >= 4 and HT > 1200. and MT2 >  400.) countSignalEvent("4b_loose");
    if ( Nj >= 2  and Nb >= 4 and HT > 1500. and MT2 >  400.) countSignalEvent("4b_tight");        
    if ( Nj >= 7  and Nb >= 3 and HT > 1200. and MT2 >  400.) countSignalEvent("7j_3b_loose");
    if ( Nj >= 7  and Nb >= 3 and HT > 1500. and MT2 >  400.) countSignalEvent("7j_3b_tight");            
    if ( Nj >= 7  and Nb >= 4 and HT > 1200. and MT2 >  400.) countSignalEvent("7j_4b_loose");
    if ( Nj >= 7  and Nb >= 4 and HT > 1500. and MT2 >  400.) countSignalEvent("7j_4b_tight");              
    if ( Nj >=10  and Nb >= 4 and HT > 1200. and MT2 >  400.) countSignalEvent("10j_4b_loose");
    if ( Nj >=10  and Nb >= 4 and HT > 1500. and MT2 >  400.) countSignalEvent("10j_4b_tight");   
    if ( Nj <=3   and Nb == 0 and HT > 250. and HT < 450. and MT2 > 200. and MT2 < 300.) countSignalEvent("Mono-phi");

  }
  
  //double pT1 = lightjets[0]->PT;
  double pT1 = signaljets[0]->PT;
  if (Nj == 1 and pT1 > 250.) {
    
    if ( Nb == 0 and HT < 450. ) countSignalEvent("Mono-phi");
    
    if      ( Nb == 0 and pT1 < 350. ) countSignalEvent("1j_0b_250");
    else if ( Nb == 0 and pT1 < 450. ) countSignalEvent("1j_0b_350");
    else if ( Nb == 0 and pT1 < 575. ) countSignalEvent("1j_0b_450");
    else if ( Nb == 0 and pT1 < 700. ) countSignalEvent("1j_0b_575");
    else if ( Nb == 0 and pT1 < 1000.) countSignalEvent("1j_0b_700");
    else if ( Nb == 0 and pT1 < 1200.) countSignalEvent("1j_0b_1000");
    else if ( Nb == 0 and pT1 >= 1200.) countSignalEvent("1j_0b_1200");
    
    if      ( Nb >= 1 and pT1 < 350. ) countSignalEvent("1j_1b_250");
    else if ( Nb >= 1 and pT1 < 450. ) countSignalEvent("1j_1b_350");
    else if ( Nb >= 1 and pT1 < 575. ) countSignalEvent("1j_1b_450");
    else if ( Nb >= 1 and pT1 < 700. ) countSignalEvent("1j_1b_575");
    else if ( Nb >= 1 and pT1 >= 700.) countSignalEvent("1j_1b_700");

  }
  
  return;
}

void Cms_sus_19_005::finalize() {
  // Whatever should be done after the run goes here
}       


template <class X>
std::vector<X*> Cms_sus_19_005::Isolate_leptons_with_variable_track_isolation_cone_CMS(std::vector<X*> leptons, double dR_track_max, double dR_track_min, double pT_for_inverse_function_track, double pT_amount_track ) {
      
  std::vector<X*> filtered_leptons;
  for(int i = 0; i < leptons.size(); i++) {
    double dR_track = 0.;
    double sumPT = 0.;
    dR_track = std::min(dR_track_max, std::max(pT_for_inverse_function_track/leptons[i]->PT, dR_track_min));
        
    for (int t = 0; t < tracks.size(); t++) {
      Track* neighbour = tracks[t];

	  // Ignore the lepton's track itself
      if(neighbour->Particle == leptons[i]->Particle) continue;
      if(neighbour->PT < 1.) continue;
      if (neighbour->P4().DeltaR(leptons[i]->P4()) > dR_track) continue;
      sumPT += neighbour->PT;
    }
    
    if( (leptons[i]->PT)*pT_amount_track > sumPT) filtered_leptons.push_back(leptons[i]);
  }
    
    return filtered_leptons;
}

double Cms_sus_19_005::dPhi(std::vector<Jet*> jets, int j) {

  int kmax = 0; int kmin = 0;
  if ( !j ) { 
    kmax = std::min(int(jets.size()), 4); 
    kmin = 0;     
  }
  else { 
    kmax = jets.size(); 
    kmin = 3;    
  }
  double dphimin = 10.;

  for (int k = kmin; k < kmax; k++)
      dphimin = std::min(fabs(jets[k]->P4().DeltaPhi( missingET->P4() )), dphimin);
     
  return dphimin;
}    


std::vector<LundCluster> Cms_sus_19_005::cluster_jets(std::vector<Jet*> input) {
  
  //find seeds
  int m = -1; int n = -1;
  double minv = 0.;
  for (int i = 0; i < input.size()-1 ; i++)
    for(int j = i; j < input.size(); j++) {
      double tmp = (input[i]->P4() + input[j]->P4()).M();
      if ( tmp > minv ) {
        minv = tmp;
        n = i;
        m = j;
      }
    }
    
  LundCluster cluster1 = LundCluster(input[n]);  
  LundCluster cluster2 = LundCluster(input[m]);  
  
  input.erase(input.begin() + m); // m > n
  input.erase(input.begin() + n);
  
  for (int i = 0; i < input.size() ; i++)  
    if (cluster1.Lund_dist(input[i]) < cluster2.Lund_dist(input[i]) ) cluster1.add_jet(input[i]);
    else cluster2.add_jet(input[i]);
  
  bool stable = 0; int k = 0; 
  while (!stable and k < 20)  {
    stable = 1;
    k++;
    for (int i = 0; i < cluster1.constituents.size(); i++) 
      if (cluster1.Lund_dist(cluster1.constituents[i]) > cluster2.Lund_dist(cluster1.constituents[i]) ) {
        cluster1.remove_jet(cluster1.constituents[i]);
        cluster2.add_jet(cluster1.constituents[i]);
        stable = 0;
      }
    for (int i = 0; i < cluster2.constituents.size(); i++) 
      if (cluster2.Lund_dist(cluster2.constituents[i]) > cluster1.Lund_dist(cluster2.constituents[i]) ) {
        cluster2.remove_jet(cluster2.constituents[i]);
        cluster1.add_jet(cluster2.constituents[i]);
        stable = 0;
      }         
  }
  
  std::vector<LundCluster> clusters;
  clusters.push_back(cluster1);
  clusters.push_back(cluster2);
  
  if ( k == 20) {
    for (int j = 0; j < 2; j++) {
      cout << "Cluster "+ std::to_string(j) << endl;
      cout << "Momentum: " << clusters[j].P4().E() << ", " << clusters[j].P4().X() << ", " << clusters[j].P4().Y() << ", " << clusters[j].P4().Z() << endl;
      cout << "Momentum: " << clusters[j].P4().Perp()  << endl;
      for (int i = 0; i < clusters[j].constituents.size(); i++) {
        cout << "Jet     : " << clusters[j].constituents[i]->P4().E() << ", " << clusters[j].constituents[i]->P4().X() << ", " << clusters[j].constituents[i]->P4().Y() << ", " << clusters[j].constituents[i]->P4().Z() << ", d1: " << clusters[0].Lund_dist(clusters[j].constituents[i]) << ", d2: " << clusters[1].Lund_dist(clusters[j].constituents[i]) << endl; 
      }
    }    
    
  }
  
  return clusters;
  
}


LundCluster::LundCluster(Jet* jet) {
  
  constituents.push_back(jet);
  momentum = jet->P4();
  
}

void LundCluster::add_jet(Jet* jet) {
  
  constituents.push_back(jet);
  std::sort(constituents.begin(), constituents.end(), sortByPT ); 
  momentum += jet->P4();
  
}

void LundCluster::remove_jet(Jet* jet) {
  
  if (constituents.size() > 1) 
    for (int i = 0; i < constituents.size(); i++)
      if (jet == constituents[i]) { 
        constituents.erase(constituents.begin() + i);
        momentum -= jet->P4();
        return;
      }
  return;
}

  
bool Cms_sus_19_005::fill_bins_2j(int Nj, int Nb, double HT, double MT2) {
  
    std::string labmt2;
    std::string labj;
    std::string labb;

    if ( HT < 450.) {
      if ( (Nb < 3 and Nj < 7) or (Nb == 0 and Nj > 6)) {
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else labmt2 = "400"; 
        if (Nj < 4) labj = "2-3j_"; 
        else if (Nj < 7) labj = "4-6j_";
        else labj = "7j_";  
        countSignalEvent(labj + std::to_string(Nb) + "b_250_" + labmt2);
      }
      else if ( Nb > 2 and Nj < 7) {
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else labmt2 = "400"; 
        labj = "2-6j_" ; 
        countSignalEvent(labj + std::to_string(Nb) + "b_250_" + labmt2);
      }
      else if ( Nb > 0 and Nj > 6) {
        if (MT2 < 300.) labmt2 = "200"; 
        else labmt2 = "300"; 
        labj = "7j_";
        labb = std::to_string(std::min(Nb, 3)); 
        countSignalEvent(labj + labb + "b_250_" + labmt2);
      }      
    }
    else if ( HT < 575.) {
      if ( Nb < 3 and Nj < 7 ) {
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else if (MT2 < 500.) labmt2 = "400";
        else labmt2 = "500";  
        if (Nj < 4) labj = "2-3j_"; 
        else labj = "4-6j_";
        countSignalEvent(labj + std::to_string(Nb) + "b_450_" + labmt2);
      }      
      else if ( Nb > 2 and Nj < 7 ) {
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else if (MT2 < 500.) labmt2 = "400";
        else labmt2 = "500";  
        labj = "2-6j_"; 
        labb = "3"; 
        countSignalEvent(labj + labb + "b_450_" + labmt2);
      } 
      else if ( Nj > 6 ) {
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else labmt2 = "400"  ;
        labj = "7j_"; 
        labb = std::to_string(std::min(Nb, 3)); 
        countSignalEvent(labj + labb + "b_450_" + labmt2);
      }       
    }
    else if ( HT < 1200.) {
      if (Nj < 4 and Nb == 0){
        labmt2 = std::to_string(std::min(int(MT2/100.), 11)) + "00" ;
        //cout << MT2 << "    "  << int(std::floor(MT2/100)) << endl ;
        countSignalEvent("2-3j_0b_575_" + labmt2);
      }
      else if (Nj < 4 and Nb == 1){
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else if (MT2 < 600.) labmt2 = "400";
        else if (MT2 < 800.) labmt2 = "600";
        else if (MT2 < 1000.) labmt2 = "800";
        else labmt2 = "1000";  
        countSignalEvent("2-3j_1b_575_" + labmt2);
      }      
      else if (Nj < 4 and Nb == 2){
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else if (MT2 < 600.) labmt2 = "400";
        else if (MT2 < 800.) labmt2 = "600";
        else labmt2 = "800";  
        countSignalEvent("2-3j_2b_575_" + labmt2);
      }  
      else if (Nj < 7 and Nb == 3){
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else if (MT2 < 600.) labmt2 = "400";
        else if (MT2 < 800.) labmt2 = "600";
        else labmt2 = "800";  
        countSignalEvent("2-6j_3b_575_" + labmt2);
      }
      else if (Nj < 7 and Nb == 0){
        labmt2 = std::to_string(std::min(int(MT2/100.), 11)) + "00" ;
        //cout << MT2 << "    "  << int(std::floor(MT2/100)) << endl;
        countSignalEvent("4-6j_0b_575_" + labmt2);
      }    
      else if (Nj < 7 and Nb == 1){
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else if (MT2 < 600.) labmt2 = "400";
        else if (MT2 < 800.) labmt2 = "600";
        else if (MT2 < 1000.) labmt2 = "800";
        else labmt2 = "1000";  
        countSignalEvent("4-6j_1b_575_" + labmt2);
      }
      else if (Nj < 7 and Nb == 2){
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else if (MT2 < 600.) labmt2 = "400";
        else if (MT2 < 800.) labmt2 = "600";
        else labmt2 = "800";  
        countSignalEvent("4-6j_2b_575_" + labmt2);
      }        
     else if (Nj < 10 and Nb < 3){
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else if (MT2 < 600.) labmt2 = "400";
        else if (MT2 < 800.) labmt2 = "600";
        else labmt2 = "800";  
        countSignalEvent("7-9j_"+ std::to_string(Nb)+"b_575_" + labmt2);
      }    
     else if (Nj < 10 and Nb == 3){
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else if (MT2 < 600.) labmt2 = "400";
        else labmt2 = "600";  
        countSignalEvent("7-9j_"+ std::to_string(Nb)+"b_575_" + labmt2);
      }   
     else if (Nj < 10 and Nb > 3){
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 400.) labmt2 = "300"; 
        else labmt2 = "400";  
        countSignalEvent("7-9j_4b_575_" + labmt2);
      } 
     else if (Nj >= 10 and Nb < 3){
        if (MT2 < 300.) labmt2 = "200"; 
        else if (MT2 < 500.) labmt2 = "300"; 
        else labmt2 = "500";  
        countSignalEvent("7-9j_"+ std::to_string(Nb)+"b_575_" + labmt2);
      }    
     else if (Nj >= 10 and Nb == 3){
        if (MT2 < 300.) labmt2 = "200"; 
        else labmt2 = "300";  
        countSignalEvent("7-9j_"+ std::to_string(Nb)+"b_575_" + labmt2);
      }
      else if (Nj >= 10 and Nb > 3) countSignalEvent("7-9j_4b_575_200");
    }
    else if ( HT < 1500.) {
      if ( Nj < 4 and Nb < 2){
        labmt2 = std::to_string(std::min(2*int(MT2/200.), 12)) + "00" ;
        //cout << MT2 << "    "  << 2*int(std::floor(MT2/200)) << " ...  double bins" << endl;
        countSignalEvent("2-3j_"+std::to_string(Nb)+"b_1200_" + labmt2);
      }
      else if ( Nj < 7 and Nb < 2){
        labmt2 = std::to_string(std::min(2*int(MT2/200.), 12)) + "00" ;
        //cout << MT2 << "    "  << 2*int(std::floor(MT2/200)) << " ...  double bins" << endl;
        countSignalEvent("4-6j_"+std::to_string(Nb)+"b_1200_" + labmt2);
      }  
      else if ( Nj < 4 and Nb == 2){
        labmt2 = std::to_string(std::min(2*int(MT2/200.), 10)) + "00" ;
        //cout << MT2 << "    "  << 2*int(std::floor(MT2/200)) << " ...  double bins" << endl;
        countSignalEvent("2-3j_"+std::to_string(Nb)+"b_1200_" + labmt2);
      }  
      else if ( Nj < 7 and Nb == 2){
        labmt2 = std::to_string(std::min(2*int(MT2/200.), 10)) + "00" ;
        //cout << MT2 << "    "  << 2*int(std::floor(MT2/200)) << " ...  double bins" << endl;
        countSignalEvent("4-6j_"+std::to_string(Nb)+"b_1200_" + labmt2);
      }
      else if ( Nj < 7 and Nb >= 2){
        labmt2 = std::to_string(std::min(2*int(MT2/200.), 6)) + "00" ;
        //cout << MT2 << "    "  << 2*int(std::floor(MT2/200)) << " ...  double bins" << endl;
        countSignalEvent("2-6j_3b_1200_" + labmt2);
      }      
      else if ( Nj < 10 and Nb < 2){
        labmt2 = std::to_string(std::min(2*int(MT2/200.), 10)) + "00" ;
        //cout << MT2 << "    "  << 2*int(std::floor(MT2/200)) << " ...  double bins" << endl;
        countSignalEvent("7-9j_"+std::to_string(Nb)+"b_1200_" + labmt2);
      }
      else if ( Nj < 10 and Nb == 2){
        labmt2 = std::to_string(std::min(2*int(MT2/200.), 8)) + "00" ;
        //cout << MT2 << "    "  << 2*int(std::floor(MT2/200)) << " ...  double bins" << endl;
        countSignalEvent("7-9j_"+std::to_string(Nb)+"b_1200_" + labmt2);
      }
     else if ( Nj < 10 and Nb == 3){
        labmt2 = std::to_string(std::min(2*int(MT2/200.), 6)) + "00" ;
        //cout << MT2 << "    "  << 2*int(std::floor(MT2/200)) << " ...  double bins" << endl;
        countSignalEvent("7-9j_"+std::to_string(Nb)+"b_1200_" + labmt2);
      }
     else if ( Nj < 10 and Nb > 3){
        labmt2 = std::to_string(std::min(2*int(MT2/200.), 4)) + "00" ;
        //cout << MT2 << "    "  << 2*int(std::floor(MT2/200)) << " ...  double bins" << endl;
        countSignalEvent("7-9j_4b_1200_" + labmt2);
      }
      else if ( Nj >= 10 and Nb < 2){
        labmt2 = std::to_string(std::min(2*int(MT2/200.), 6)) + "00" ;
        //cout << MT2 << "    "  << 2*int(std::floor(MT2/200)) << " ...  double bins" << endl;
        countSignalEvent("10j_"+std::to_string(Nb)+"b_1200_" + labmt2);
      }
      else if ( Nj >= 10 and Nb == 3){
        labmt2 = std::to_string(std::min(2*int(MT2/200.), 4)) + "00" ;
        //cout << MT2 << "    "  << 2*int(std::floor(MT2/200)) << " ...  double bins" << endl;
        countSignalEvent("10j_"+std::to_string(Nb)+"b_1200_" + labmt2);
      }  
      else if ( Nj >= 10 and Nb >= 4) countSignalEvent("10j_4b_1200_200");
    }
    else if ( HT >= 1500.) {
      if (Nj < 4 and Nb == 0){
        if (MT2 < 600.) labmt2 = "400"; 
        else if (MT2 < 800.) labmt2 = "600"; 
        else if (MT2 < 1000.) labmt2 = "800";
        else if (MT2 < 1200.) labmt2 = "1000";
        else if (MT2 < 1400.) labmt2 = "1200";
        else if (MT2 < 1800.) labmt2 = "1400";
        else labmt2 = "1800";  
        countSignalEvent("2-3j_"+ std::to_string(Nb)+"b_1500_" + labmt2);
      }
      else if (Nj < 4 and Nb == 1){
        if (MT2 < 600.) labmt2 = "400"; 
        else if (MT2 < 800.) labmt2 = "600"; 
        else if (MT2 < 1000.) labmt2 = "800";
        else if (MT2 < 1200.) labmt2 = "1000";
        else labmt2 = "1200";  
        countSignalEvent("2-3j_"+ std::to_string(Nb)+"b_1500_" + labmt2);
      }
      else if (Nj < 4 and Nb == 2) countSignalEvent("2-3j_"+ std::to_string(Nb)+"b_1500_400");
      else if (Nj < 7 and Nb >= 3){
        if (MT2 < 600.) labmt2 = "400"; 
        else labmt2 = "600";  
        countSignalEvent("2-6j_3b_1500_" + labmt2);
      }
      else if (Nj < 7 and Nb == 0){
        if (MT2 < 600.) labmt2 = "400"; 
        else if (MT2 < 800.) labmt2 = "600"; 
        else if (MT2 < 1000.) labmt2 = "800";
        else if (MT2 < 1200.) labmt2 = "1000";
        else if (MT2 < 1400.) labmt2 = "1200";
        else if (MT2 < 1600.) labmt2 = "1400";
        else labmt2 = "1600";  
        countSignalEvent("4-6j_"+ std::to_string(Nb)+"b_1500_" + labmt2);
      } 
      else if (Nj < 7 and Nb == 1){
        if (MT2 < 600.) labmt2 = "400"; 
        else if (MT2 < 800.) labmt2 = "600"; 
        else if (MT2 < 1000.) labmt2 = "800";
        else if (MT2 < 1400.) labmt2 = "1000";
        else labmt2 = "1400";  
        countSignalEvent("4-6j_"+ std::to_string(Nb)+"b_1500_" + labmt2);
      } 
      else if (Nj < 7 and Nb == 2){
        if (MT2 < 600.) labmt2 = "400"; 
        else if (MT2 < 800.) labmt2 = "600"; 
        else labmt2 = "800";  
        countSignalEvent("4-6j_"+ std::to_string(Nb)+"b_1500_" + labmt2);
      }   
      else if (Nj < 10 and Nb == 0){
        if (MT2 < 600.) labmt2 = "400"; 
        else if (MT2 < 800.) labmt2 = "600"; 
        else if (MT2 < 1000.) labmt2 = "800";
        else if (MT2 < 1400.) labmt2 = "1000";
        else labmt2 = "1400";  
        countSignalEvent("7-9j_"+ std::to_string(Nb)+"b_1500_" + labmt2);
      }
      else if (Nj < 10 and Nb < 2 ){
        if (MT2 < 600.) labmt2 = "400"; 
        else if (MT2 < 800.) labmt2 = "600"; 
        else labmt2 = "800";  
        countSignalEvent("7-9j_"+ std::to_string(Nb)+"b_1500_" + labmt2);
      } 
      else if (Nj < 10 and Nb ==3  ){
        if (MT2 < 800.) labmt2 = "400"; 
        else labmt2 = "800";  
        countSignalEvent("7-9j_"+ std::to_string(Nb)+"b_1500_" + labmt2);
      }       
      else if (Nj < 10 and Nb >= 4  ) countSignalEvent("7-9j_4b_1500_400");
      else if (Nj >= 10 and Nb < 2  ) {
        if (MT2 < 800.) labmt2 = "400"; 
        else labmt2 = "800";  
        countSignalEvent("10j_"+ std::to_string(Nb)+"b_1500_" + labmt2);
      }      
      else if (Nj >= 10 and Nb < 4  ) countSignalEvent("10j_"+ std::to_string(Nb)+"b_1500_400");
      else if (Nj >= 10 and Nb >= 4  ) countSignalEvent("10j_4b_1500_400");
    }

    return true;

}  
  
