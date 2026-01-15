#include "atlas_2102_10874.h"
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl

std::string Atlas_2102_10874::signal_regions [26] = {"IM00","IM01","IM02","IM03","IM04","IM05","IM06","IM07","IM08","IM09","IM10","IM11","IM12","EM00","EM01","EM02","EM03","EM04","EM05","EM06","EM07","EM08","EM09","EM10","EM11","EM12"};
int Atlas_2102_10874::signal_region_borders [26][2] = {{200,10000},{250,10000},{300,10000},{350,10000},{400,10000},{500,10000},{600,10000},{700,10000},{800,10000},{900,10000},{1000,10000},{1100,10000},{1200,10000},{200,250},{250,300},{300,350},{350,400},{400,500},{500,600},{600,700},{700,800},{800,900},{900,1000},{1000,1100},{1100,1200},{1200,10000}};

void Atlas_2102_10874::initialize() {
  setAnalysisName("atlas_2102_10874");          
  setInformation(""
    "# monojet\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("EM00;EM01;EM02;EM03;EM04;EM05;EM06;EM07;EM08;EM09;EM10;EM11;EM12;IM00;IM01;IM02;IM03;IM04;IM05;IM06;IM07;IM08;IM09;IM10;IM11;IM12");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.
  bookControlRegions("CR1e0b_200_cuts;CR1e0b_250_cuts;CR1e0b_300_cuts;CR1e0b_350_cuts;CR1e0b_400_cuts;CR1e0b_500_cuts;CR1e0b_600_cuts;CR1e0b_700_cuts;CR1e0b_800_cuts;CR1e0b_900_cuts;CR1e0b_1000_cuts;CR1e0b_1100_cuts;CR1e0b_1200_cuts;CR2e_200_cuts;CR2e_250_cuts;CR2e_300_cuts;CR2e_350_cuts;CR2e_400_cuts;CR2e_500_cuts;CR2e_600_cuts;CR2e_700_cuts;CR2e_800_cuts;CR2e_900_cuts;CR2e_1000_cuts;CR2e_1100_cuts;CR2e_1200_cuts;CR1mu0b_200_cuts;CR1mu0b_250_cuts;CR1mu0b_300_cuts;CR1mu0b_350_cuts;CR1mu0b_400_cuts;CR1mu0b_500_cuts;CR1mu0b_600_cuts;CR1mu0b_700_cuts;CR1mu0b_800_cuts;CR1mu0b_900_cuts;CR1mu0b_1000_cuts;CR1mu0b_1100_cuts;CR1mu0b_1200_cuts;CR2mu_200_cuts;CR2mu_250_cuts;CR2mu_300_cuts;CR2mu_350_cuts;CR2mu_400_cuts;CR2mu_500_cuts;CR2mu_600_cuts;CR2mu_700_cuts;CR2mu_800_cuts;CR2mu_900_cuts;CR2mu_1000_cuts;CR2mu_1100_cuts;CR2mu_1200_cuts");  
  bookControlRegions("CR1L1b_200_cuts;CR1L1b_250_cuts;CR1L1b_300_cuts;CR1L1b_350_cuts;CR1L1b_400_cuts;CR1L1b_500_cuts;CR1L1b_600_cuts;CR1L1b_700_cuts;CR1L1b_800_cuts;CR1L1b_900_cuts;CR1L1b_1000_cuts;CR1L1b_1100_cuts;CR1L1b_1200_cuts");

  // You should initialize any declared variables here
}

void Atlas_2102_10874::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  electronsLoose = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 30., -2.47, 2.47, true);
  muonsCombined = filterPhaseSpace(muonsCombined, 7., -2.5, 2.5);
  jets = filterPhaseSpace(jets, 30., -2.8, 2.8);
  std::vector<Jet*> taucand = filterPhaseSpace(jets, 10., -2.5, 2.5);
  std::vector<Photon*> photonsCand = filterPhaseSpace(photonsMedium, 10., -2.37, 2.37);

  std::vector<Jet*> bjets;
  std::vector<Jet*> nonbjets;
  for (int i = 0; i < jets.size(); i++)
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i]) )
      bjets.push_back(jets[i]);
    else nonbjets.push_back(jets[i]);

  nonbjets = overlapRemoval(nonbjets, electronsLoose, 0.2);
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.4);
  electronsLoose = overlapRemoval(electronsLoose, nonbjets, 0.4);
  electronsTight = overlapRemoval(electronsTight, bjets, 0.4);
  electronsTight = overlapRemoval(electronsTight, nonbjets, 0.4);
  electronsTight = Isolate_leptons_with_inverse_track_isolation_cone(electronsTight, tracks, towers, 0.3, 10., 0.2, 0.06, 0.06, true);

  bjets = overlapRemoval_muon_jet_tracks(bjets, muonsCombined, 0.4, 2);
  nonbjets = overlapRemoval_muon_jet_tracks(nonbjets, muonsCombined, 0.4, 2);

  muonsCombined = overlapRemoval(muonsCombined, bjets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, nonbjets, 0.4);

  photonsCand = overlapRemoval(photonsCand, electronsLoose, 0.4);
  photonsCand = overlapRemoval(photonsCand, muonsCombined, 0.4);
  bjets = overlapRemoval(bjets, photonsCand, 0.4);
  nonbjets = overlapRemoval(nonbjets, photonsCand, 0.4);

  std::vector<Jet*> taus;
  for(int i=0; i< taucand.size(); i++)
    if( checkTauTag(jets[i], "loose") and fabs(jets[i]->Charge)==1 )
      taus.push_back(taucand[i]);

  taus = filterPhaseSpace(taus, 18., -2.5, 2.5, true);//tau jet

  taus = overlapRemoval(taus, electronsLoose, 0.2);
  taus = overlapRemoval(taus, muonsCombined, 0.2);
  bjets = overlapRemoval(bjets, taus, 0.2);
  nonbjets = overlapRemoval(nonbjets, taus, 0.2);

  std::vector<Jet*> sigjets = nonbjets;
  sigjets.insert(sigjets.end(), bjets.begin(), bjets.end());
  std::sort(sigjets.begin(), sigjets.end(), Atlas_2102_10874::sortByPT );

  countCutflowEvent("00_all");

  nel = electronsLoose.size(); nmu = muonsCombined.size(); ngam = photonsCand.size(); ntau = taus.size(); nb = bjets.size();

  pass_SR(sigjets);

  if (electronsTight.size() == 1)
    pass_CR1el(sigjets, electronsTight);

  if (electronsTight.size() == 2)
    pass_CR2el(sigjets, electronsTight);

  if (nmu == 1)
    pass_CR1mu(sigjets);

  if (nmu == 2)
    pass_CR2mu(sigjets);

  return;
  
}

void Atlas_2102_10874::finalize() {
  // Whatever should be done after the run goes here
}       

bool Atlas_2102_10874::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {

  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++)
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

std::vector<Jet*> Atlas_2102_10874::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){

  std::vector<Jet*> passed;
  for (std::vector<Jet*>::iterator jet = cand_jets.begin(); jet != cand_jets.end(); jet++) {

    if (check_nTrack_jet(*jet, tracks, nTracks)) {
      passed.push_back(*jet);
      continue;
    }

    bool iso = true;

    for (std::vector<Muon*>::iterator mu = cand_muons.begin(); mu != cand_muons.end(); mu++)
      if ((*jet)->P4().DeltaR((*mu)->P4()) < deltaR ) {
	iso = false;
	break;
      }

    if (iso) passed.push_back(*jet);
  }

  return passed;
}

bool Atlas_2102_10874::sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }

void Atlas_2102_10874::pass_SR(std::vector<Jet*> sigjets) {

  double met = missingET->P4().Et() ;
  if ( met < 110.) return;
  countCutflowEvent("01_trigger");

  if( sigjets.size() < 1 ) return;
  countCutflowEvent("02_1jet");

  if( sigjets.size() > 4 ) return;
  countCutflowEvent("03_4jets");

  if( nel + nmu > 0) return;
  countCutflowEvent("04_leptonveto");

  if ( ntau > 0) return;
  countCutflowEvent("05_tauveto");

  if ( ngam > 0) return;
  countCutflowEvent("06_photonveto");

  if ( met < 200.) return;
  countCutflowEvent("07_met>200");

  double dphi = (met < 250.) ? 0.6 : 0.4;
  for (int i = 0; i < sigjets.size(); i++)
      if ( fabs(missingET->P4().DeltaPhi(sigjets[i]->P4())) < dphi ) return;
  countCutflowEvent("08_dphi");

  if (sigjets[0]->PT < 150.) return;
  countCutflowEvent("09_pt>150");

  if (fabs(sigjets[0]->Eta) > 2.4) return;
  countCutflowEvent("10_eta<2.4");

  for(int i = 0; i < 26; i++)
    if (met > signal_region_borders[i][0] and met < signal_region_borders[i][1]) countSignalEvent(signal_regions[i]);

  return;
}

void Atlas_2102_10874::pass_CR1el(std::vector<Jet*> sigjets, std::vector<Electron*> electrons)   {

  double met = (missingET->P4() + electrons[0]->P4()).Et() ;
  bool trigger_met = false;
  bool trigger_top = false;
  if ( missingET->P4().Et() > 110.) trigger_top = true;
  if (electrons[0]->PT > 25.) trigger_met = true;
  if ( !trigger_met and !trigger_top )  return;

  if (electrons[0]->PT < 30.) return;

  double calc_mt = mT(missingET->P4(), electrons[0]->P4());
  if ( calc_mt < 30. or  calc_mt > 100. ) return;

  if( sigjets.size() < 1 ) return;

  if( sigjets.size() > 4 ) return;

  if( nmu > 0) return;

  if ( ntau > 0) return;

  if ( ngam > 0) return;

  if ( met < 200.) return;

  double dphi = (met < 250.) ? 0.6 : 0.4;
  for (int i = 0; i < sigjets.size(); i++)
      if ( fabs((missingET->P4() + electrons[0]->P4()).DeltaPhi(sigjets[i]->P4())) < dphi ) return;

  if (sigjets[0]->PT < 150.) return;

  if (fabs(sigjets[0]->Eta) > 2.4) return;

  if ( trigger_met and nb == 0) {
    for(int i = 14; i < 26; i++)
      if (met > signal_region_borders[i][0] and met < signal_region_borders[i][1]) countControlEvent("CR1e0b_"+std::to_string(signal_region_borders[i][0])+"_cuts");
  }

  if ( (trigger_top or trigger_met) and nb >= 1) {
    for(int i = 14; i < 26; i++)
      if (met > signal_region_borders[i][0] and met < signal_region_borders[i][1]) countControlEvent("CR1L1b_"+std::to_string(signal_region_borders[i][0])+"_cuts");
  }

  return;
}

void Atlas_2102_10874::pass_CR1mu(std::vector<Jet*> sigjets)   {

  double met = (missingET->P4() + muonsCombined[0]->P4()).Et() ;
  if ( missingET->P4().Et() < 110.)  return;

  if (muonsCombined[0]->PT < 10.) return;

  double calc_mt = mT(missingET->P4(), muonsCombined[0]->P4());
  if ( calc_mt < 30. or  calc_mt > 100. ) return;

  if( sigjets.size() < 1 ) return;

  if( sigjets.size() > 4 ) return;

  if( nel > 0) return;

  if ( ntau > 0) return;

  if ( ngam > 0) return;

  if ( met < 200.) return;

  double dphi = (met < 250.) ? 0.6 : 0.4;
  for (int i = 0; i < sigjets.size(); i++)
      if ( fabs((missingET->P4() + muonsCombined[0]->P4()).DeltaPhi(sigjets[i]->P4())) < dphi ) return;

  if (sigjets[0]->PT < 150.) return;

  if (fabs(sigjets[0]->Eta) > 2.4) return;

  if ( nb == 0) {
    for(int i = 14; i < 26; i++)
      if (met > signal_region_borders[i][0] and met < signal_region_borders[i][1]) countControlEvent("CR1mu0b_"+std::to_string(signal_region_borders[i][0])+"_cuts");
  }
  else {
    for(int i = 14; i < 26; i++)
      if (met > signal_region_borders[i][0] and met < signal_region_borders[i][1]) countControlEvent("CR1L1b_"+std::to_string(signal_region_borders[i][0])+"_cuts");
  }

  return;
}

void Atlas_2102_10874::pass_CR2el(std::vector<Jet*> sigjets, std::vector<Electron*> electrons)   {

  double met = (missingET->P4() + electrons[0]->P4() + electrons[1]->P4()).Et() ;
  
  if (electrons[0]->PT < 25.) return;

  if (electrons[1]->PT < 30.) return;

  if( sigjets.size() < 1 ) return;

  if( sigjets.size() > 4 ) return;

  if( nmu > 0) return;

  if ( ntau > 0) return;

  if ( ngam > 0) return;

  if ( met < 200.) return;

  double dphi = (met < 250.) ? 0.6 : 0.4;
  for (int i = 0; i < sigjets.size(); i++)
      if ( fabs((missingET->P4() + electrons[0]->P4() + electrons[1]->P4()).DeltaPhi(sigjets[i]->P4())) < dphi ) return;

  if (sigjets[0]->PT < 150.) return;

  if (fabs(sigjets[0]->Eta) > 2.4) return;

  for(int i = 14; i < 26; i++)
    if (met > signal_region_borders[i][0] and met < signal_region_borders[i][1]) countControlEvent("CR2e_"+std::to_string(signal_region_borders[i][0])+"_cuts");

  return;

}

void Atlas_2102_10874::pass_CR2mu(std::vector<Jet*> sigjets)   {

  double met = (missingET->P4() + muonsCombined[0]->P4() + muonsCombined[1]->P4()).Et() ;
  if ( missingET->P4().Et() < 110.) return;

  if (muonsCombined[1]->PT < 10.) return;

  if( sigjets.size() < 1 ) return;

  if( sigjets.size() > 4 ) return;

  if( nel > 0) return;

  if ( ntau > 0) return;

  if ( ngam > 0) return;

  if ( met < 200.) return;

  double dphi = (met < 250.) ? 0.6 : 0.4;
  for (int i = 0; i < sigjets.size(); i++)
      if ( fabs((missingET->P4() + muonsCombined[0]->P4() + muonsCombined[1]->P4()).DeltaPhi(sigjets[i]->P4())) < dphi ) return;

  if (sigjets[0]->PT < 150.) return;

  if (fabs(sigjets[0]->Eta) > 2.4) return;

  for(int i = 14; i < 26; i++)
    if (met > signal_region_borders[i][0] and met < signal_region_borders[i][1]) countControlEvent("CR2mu_"+std::to_string(signal_region_borders[i][0])+"_cuts");

  return;

}