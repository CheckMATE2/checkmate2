#include "cms_1908_04722.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl

std::string Cms_1908_04722::label[10] = { "300_300", "300_600", "300_1200", "350_350", "350_600", "350_1200", "600_600", "600_1200", "850_850", "850_1700"};
std::string Cms_1908_04722::label_hi[8] = { "300_600", "300_1200", "350_600", "350_1200", "600_600", "600_1200", "850_850", "850_1700"};

void Cms_1908_04722::initialize() {
  setAnalysisName("cms_1908_04722");          
  setInformation(""
     "# hadronic final states with HT, multibin simple fitting with post-fit background numbers\n"
  "");
  setLuminosity(137.0*units::INVFB);      
  bookSignalRegions("A01_2j_0b_600_600;A02_4j_0b_850_1700;A03_6j_0b_600_600;A04_8j_0-1b_600_600;A05_10j_0-1b_850_1700;A06_4j_2b_300_300;A07_2j_2b_600_600;A08_6j_2b_350_350;A09_4j_2b_600_600;A10_8j_3b_300_300;A11_6j_1b_600_600;A12_10j_3b_850_850;B001_2-3j_0b_300_300;B002_2-3j_0b_300_600;B003_2-3j_0b_300_1200;B004_2-3j_0b_350_350;B005_2-3j_0b_350_600;B006_2-3j_0b_350_1200;B007_2-3j_0b_600_600;B008_2-3j_0b_600_1200;B009_2-3j_0b_850_850;B010_2-3j_0b_850_1700;B011_2-3j_1b_300_300;B012_2-3j_1b_300_600;B013_2-3j_1b_300_1200;B014_2-3j_1b_350_350;B015_2-3j_1b_350_600;B016_2-3j_1b_350_1200;B017_2-3j_1b_600_600;B018_2-3j_1b_600_1200;B019_2-3j_1b_850_850;B020_2-3j_1b_850_1700;B021_2-3j_2b_300_300;B022_2-3j_2b_300_600;B023_2-3j_2b_300_1200;B024_2-3j_2b_350_350;B025_2-3j_2b_350_600;B026_2-3j_2b_350_1200;B027_2-3j_2b_600_600;B028_2-3j_2b_600_1200;B029_2-3j_2b_850_850;B030_2-3j_2b_850_1700;B031_4-5j_0b_300_300;B032_4-5j_0b_300_600;B033_4-5j_0b_300_1200;B034_4-5j_0b_350_350;B035_4-5j_0b_350_600;B036_4-5j_0b_350_1200;B037_4-5j_0b_600_600;B038_4-5j_0b_600_1200;B039_4-5j_0b_850_850;B040_4-5j_0b_850_1700;B041_4-5j_1b_300_300;B042_4-5j_1b_300_600;B043_4-5j_1b_300_1200;B044_4-5j_1b_350_350;B045_4-5j_1b_350_600;B046_4-5j_1b_350_1200;B047_4-5j_1b_600_600;B048_4-5j_1b_600_1200;B049_4-5j_1b_850_850;B050_4-5j_1b_850_1700;B051_4-5j_2b_300_300;B052_4-5j_2b_300_600;B053_4-5j_2b_300_1200;B054_4-5j_2b_350_350;B055_4-5j_2b_350_600;B056_4-5j_2b_350_1200;B057_4-5j_2b_600_600;B058_4-5j_2b_600_1200;B059_4-5j_2b_850_850;B060_4-5j_2b_850_1700;B061_4-5j_3b_300_300;B062_4-5j_3b_300_600;B063_4-5j_3b_300_1200;B064_4-5j_3b_350_350;B065_4-5j_3b_350_600;B066_4-5j_3b_350_1200;B067_4-5j_3b_600_600;B068_4-5j_3b_600_1200;B069_4-5j_3b_850_850;B070_4-5j_3b_850_1700;B071_6-7j_0b_300_300;B072_6-7j_0b_300_600;B073_6-7j_0b_300_1200;B074_6-7j_0b_350_350;B075_6-7j_0b_350_600;B076_6-7j_0b_350_1200;B077_6-7j_0b_600_600;B078_6-7j_0b_600_1200;B079_6-7j_0b_850_850;B080_6-7j_0b_850_1700;B081_6-7j_1b_300_300;B082_6-7j_1b_300_600;B083_6-7j_1b_300_1200;B084_6-7j_1b_350_350;B085_6-7j_1b_350_600;B086_6-7j_1b_350_1200;B087_6-7j_1b_600_600;B088_6-7j_1b_600_1200;B089_6-7j_1b_850_850;B090_6-7j_1b_850_1700;B091_6-7j_2b_300_300;B092_6-7j_2b_300_600;B093_6-7j_2b_300_1200;B094_6-7j_2b_350_350;B095_6-7j_2b_350_600;B096_6-7j_2b_350_1200;B097_6-7j_2b_600_600;B098_6-7j_2b_600_1200;B099_6-7j_2b_850_850;B100_6-7j_2b_850_1700;B101_6-7j_3b_300_300;B102_6-7j_3b_300_600;B103_6-7j_3b_300_1200;B104_6-7j_3b_350_350;B105_6-7j_3b_350_600;B106_6-7j_3b_350_1200;B107_6-7j_3b_600_600;B108_6-7j_3b_600_1200;B109_6-7j_3b_850_850;B110_6-7j_3b_850_1700;B111_8-9j_0b_300_600;B112_8-9j_0b_300_1200;B113_8-9j_0b_350_600;B114_8-9j_0b_350_1200;B115_8-9j_0b_600_600;B116_8-9j_0b_600_1200;B117_8-9j_0b_850_850;B118_8-9j_0b_850_1700;B119_8-9j_1b_300_600;B120_8-9j_1b_300_1200;B121_8-9j_1b_350_600;B122_8-9j_1b_350_1200;B123_8-9j_1b_600_600;B124_8-9j_1b_600_1200;B125_8-9j_1b_850_850;B126_8-9j_1b_850_1700;B127_8-9j_2b_300_600;B128_8-9j_2b_300_1200;B129_8-9j_2b_350_600;B130_8-9j_2b_350_1200;B131_8-9j_2b_600_600;B132_8-9j_2b_600_1200;B133_8-9j_2b_850_850;B134_8-9j_2b_850_1700;B135_8-9j_3b_300_600;B136_8-9j_3b_300_1200;B137_8-9j_3b_350_600;B138_8-9j_3b_350_1200;B139_8-9j_3b_600_600;B140_8-9j_3b_600_1200;B141_8-9j_3b_850_850;B142_8-9j_3b_850_1700;B143_10j_0b_300_600;B144_10j_0b_300_1200;B145_10j_0b_350_600;B146_10j_0b_350_1200;B147_10j_0b_600_600;B148_10j_0b_600_1200;B149_10j_0b_850_850;B150_10j_0b_850_1700;B151_10j_1b_300_600;B152_10j_1b_300_1200;B153_10j_1b_350_600;B154_10j_1b_350_1200;B155_10j_1b_600_600;B156_10j_1b_600_1200;B157_10j_1b_850_850;B158_10j_1b_850_1700;B159_10j_2b_300_600;B160_10j_2b_300_1200;B161_10j_2b_350_600;B162_10j_2b_350_1200;B163_10j_2b_600_600;B164_10j_2b_600_1200;B165_10j_2b_850_850;B166_10j_2b_850_1700;B167_10j_3b_300_600;B168_10j_3b_300_1200;B169_10j_3b_350_600;B170_10j_3b_350_1200;B171_10j_3b_600_600;B172_10j_3b_600_1200;B173_10j_3b_850_850;B174_10j_3b_850_1700");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1908_04722::analyze() {

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
  
  jets = filterPhaseSpace(jets, 30., -5.0, 5.0);
  jets = overlapRemoval(jets, electronsMedium, 0.3, "y");
  jets = overlapRemoval(jets, muonsCombined, 0.3, "y");
  jets = overlapRemoval(jets, photonsMedium, 0.3, "y");  
  
  double HT = 0.;
  TLorentzVector Hmiss = TLorentzVector(0.,0.,0.,0.);
  for (int i = 0; i < jets.size(); i++) {
    if ( fabs(jets[i]->Eta) < 2.4)  HT += jets[i]->PT;
    Hmiss -= jets[i]->P4();
  }
  
  countCutflowEvent("00_all");
  std::vector<Jet*> signal_jets = filterPhaseSpace(jets, 30., -2.4, 2.4);
  int Nj = signal_jets.size();
  
  if (Nj < 2) return;
  countCutflowEvent("01_Nj>=2");
  
  if (HT < 300.) return;
  countCutflowEvent("02_HT>300");
  
  double HTmiss = Hmiss.Perp();
  if (HTmiss < 300.) return;
  countCutflowEvent("03_HTmiss>300");
  
  if (HTmiss/HT >= 1.) return;
  countCutflowEvent("04_HTmiss/HT>1");
  
  if (muonsCombined.size() ) return;
  countCutflowEvent("05_muVeto");
  
  if (muonsPF.size() ) return;
  countCutflowEvent("06_mutrackVeto");
  
  if (electronsMedium.size() ) return;
  countCutflowEvent("07_eVeto");  
  
  if (electronsPF.size() ) return;
  countCutflowEvent("08_etrackVeto");    
  
  if (photonsMedium.size() ) return;
  countCutflowEvent("09_photVeto");    
  
  if (fabs(Hmiss.DeltaPhi(signal_jets[0]->P4())) < 0.5) return;
  countCutflowEvent("10_dPhi_j1");   
  
  if (fabs(Hmiss.DeltaPhi(signal_jets[1]->P4())) < 0.5) return;
  countCutflowEvent("11_dPhi_j2");     
  
  if (signal_jets.size() > 2 and fabs(Hmiss.DeltaPhi(signal_jets[2]->P4())) < 0.3) return;
  countCutflowEvent("12_dPhi_j3");     

  if (signal_jets.size() > 3 and fabs(Hmiss.DeltaPhi(signal_jets[3]->P4())) < 0.3) return;
  countCutflowEvent("13_dPhi_j4");     
  
  int Nb = 0;
  for (int i = 0; i < signal_jets.size(); i++) 
    if (checkBTag(signal_jets[i]) ) Nb++;
  
  fill_bins( Nj, Nb, HTmiss, HT );
  
  if (Nj >=2 and Nb == 0 and HTmiss > 600. and HT > 600.) countSignalEvent("A01_2j_0b_600_600");
  if (Nj >=4 and Nb == 0 and HTmiss > 850. and HT > 1700.) countSignalEvent("A02_4j_0b_850_1700");
  if (Nj >=6 and Nb == 0 and HTmiss > 600. and HT > 600.) countSignalEvent("A03_6j_0b_600_600");
  if (Nj >=8 and Nb <= 1 and HTmiss > 600. and HT > 600.) countSignalEvent("A04_8j_0-1b_600_600");
  //if (Nj >=10 and Nb <= 1 and HTmiss > 850. and HT > 1700.) countSignalEvent("A05_10j_0-1b_850_1700"); the numbers in the paper are nonsense
  if (Nj >=4 and Nb >= 2 and HTmiss > 300. and HT > 300.) countSignalEvent("A06_4j_2b_300_300");
  if (Nj >=2 and Nb >= 2 and HTmiss > 600. and HT > 600.) countSignalEvent("A07_2j_2b_600_600");
  if (Nj >=6 and Nb >= 2 and HTmiss > 350. and HT > 350.) countSignalEvent("A08_6j_2b_350_350");
  if (Nj >=4 and Nb >= 2 and HTmiss > 600. and HT > 600.) countSignalEvent("A09_4j_2b_600_600");
  if (Nj >=8 and Nb >= 3 and HTmiss > 300. and HT > 300.) countSignalEvent("A10_8j_3b_300_300");
  if (Nj >=6 and Nb >= 1 and HTmiss > 600. and HT > 600.) countSignalEvent("A11_6j_1b_600_600");
  if (Nj >=10 and Nb >= 3 and HTmiss > 850. and HT > 850.) countSignalEvent("A12_10j_3b_850_850");
  
  return;  
  
}

void Cms_1908_04722::finalize() {
  // Whatever should be done after the run goes here
}       


template <class X>
std::vector<X*> Cms_1908_04722::Isolate_leptons_with_variable_track_isolation_cone_CMS(std::vector<X*> leptons, double dR_track_max, double dR_track_min, double pT_for_inverse_function_track, double pT_amount_track ) {
      
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

void Cms_1908_04722::fill_bins(int Nj, int Nb, double HTmiss, double HT) {
  
  int count = 0;
  int interval = 0;
  
  if (HTmiss < 350. and HT < 600. ) interval = 1;
  if (HTmiss < 350. and HT > 600. and HT < 1200. ) interval = 2;
  if (HTmiss < 350. and HT > 1200. ) interval = 3;
  if (HTmiss > 350. and HTmiss < 600 and HT < 600. ) interval = 4;
  if (HTmiss > 350. and HTmiss < 600 and HT > 600. and HT < 1200. ) interval = 5;
  if (HTmiss > 350. and HTmiss < 600 and HT > 1200. ) interval = 6;
  if (HTmiss > 600. and HTmiss < 850 and HT < 1200. ) interval = 7;  
  if (HTmiss > 600. and HTmiss < 850 and HT > 1200. ) interval = 8;  
  if (HTmiss > 850. and HT < 1700. ) interval = 9;
  if (HTmiss > 850. and HT > 1700. ) interval = 10;
  
  std::string bb;
  if (Nj <= 3) {
    if (Nb == 0) { count = 0; bb = "_2-3j_0b_";}
    else if (Nb == 1) {count = 10; bb = "_2-3j_1b_";}
    else {count = 20; bb = "_2-3j_2b_";}
    count += interval;
    if (count >= 10) countSignalEvent("B0"+std::to_string(count)+bb+label[interval-1]);
    else countSignalEvent("B00"+std::to_string(count)+bb+label[interval-1]);   
    return;
  }
  else if (Nj <= 5) {
    if (Nb == 0) {count = 30; bb = "_4-5j_0b_";}
    else if (Nb == 1) {count = 40; bb = "_4-5j_1b_";}
    else if (Nb == 2) {count = 50; bb = "_4-5j_2b_";}
    else {count = 60; bb = "_4-5j_3b_";}
    count += interval;
    countSignalEvent("B0"+std::to_string(count)+bb+label[interval-1]);
    return;
  }
  else if (Nj <= 7) {
    if (Nb == 0) {count = 70; bb = "_6-7j_0b_";}
    else if (Nb == 1) {count = 80; bb = "_6-7j_1b_";}
    else if (Nb == 2) {count = 90; bb = "_6-7j_2b_";}
    else {count = 100; bb = "_6-7j_3b_";}
    count += interval;
    if (count < 100) countSignalEvent("B0"+std::to_string(count)+bb+label[interval-1]);
    else countSignalEvent("B"+std::to_string(count)+bb+label[interval-1]);
    return;
  }
  
  int interval_hi;
  if (Nj >= 8 and (interval == 1 or interval == 4) ) return;
  if (interval < 5) interval_hi = interval - 1;
  else interval_hi = interval - 2;
  
  if (Nj >=8 and Nj <= 9) {
    if (Nb == 0) {count = 110; bb = "_8-9j_0b_";}
    else if (Nb == 1) {count = 118; bb = "_8-9j_1b_";}
    else if (Nb == 2) {count = 126; bb = "_8-9j_2b_";}
    else {count = 134; bb = "_8-9j_3b_";}    
    count += interval_hi;
    countSignalEvent("B"+std::to_string(count)+bb+label_hi[interval_hi-1]);
    return;    
  }
  else if(Nj >= 10) {
    if (Nb == 0) {count = 142; bb = "_10j_0b_";}
    else if (Nb == 1) {count = 150; bb = "_10j_1b_";}
    else if (Nb == 2) {count = 158; bb = "_10j_2b_";}
    else {count = 166; bb = "_10j_3b_";}    
    count += interval_hi;
    countSignalEvent("B"+std::to_string(count)+bb+label_hi[interval_hi-1]);
    return;    
  }
    
  
}
