#include "atlas_phys_2014_007.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl

std::string Atlas_phys_2014_007::signal_regions [21] =
{"E040","E045","E050","E060","E070","E080","E090","E100","E120","E140","E160","I040","I045","I050","I060","I070","I080","I090","I100","I120","I140"};
int Atlas_phys_2014_007::signal_region_borders [21][2] = {{400,450},{450,500},{500,600},{600,700},{700,800},{800,900},{900,1000},{1000,1200},{1200,1400},{1400,1600},{1600,10000},{400,10000},{450,10000},{500,10000},{600,10000},{700,10000},{800,10000},{900,10000},{1000,10000},{1200,10000},{1400,10000}};

void Atlas_phys_2014_007::initialize() {
  setAnalysisName("atlas_phys_2014_007");          
  setInformation(""
    "# monojet\n"
  "");
  setLuminosity(3000.0*units::INVFB);      
  bookSignalRegions("E040;E045;E050;E060;E070;E080;E090;E100;E120;E140;E160;I040;I045;I050;I060;I070;I080;I090;I100;I120;I140");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_phys_2014_007::analyze() {
    
  // background taken from fig 3b; errors guessed based on Atlas_conf_2020_048; initial selection of objects also from Atlas_conf_2020_048  
 
  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  electronsLoose = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 7., -2.5, 2.5);
  jets = filterPhaseSpace(jets, 50., -3.6, 3.6);
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
  std::sort(sigjets.begin(), sigjets.end(), Atlas_phys_2014_007::sortByPT );   
  
  countCutflowEvent("00_all");
  
  double met = missingET->P4().Et() ;
  if ( met < 110.) return;
  countCutflowEvent("01_trigger");
  
  if( sigjets.size() < 1 ) return;
  countCutflowEvent("02_1jet");  
  
  if( sigjets.size() > 2 ) return;
  countCutflowEvent("03_2jets");    
  
  if( electronsLoose.size() + muonsCombined.size() > 0) return;
  countCutflowEvent("04_leptonveto");
  
  if ( taus.size() > 0) return;
  countCutflowEvent("05_tauveto");
  
  if ( photonsCand.size() > 0) return;
  countCutflowEvent("06_photonveto");  
  
  if ( met < 400.) return;
  countCutflowEvent("07_met>400");
  
  for (int i = 0; i < sigjets.size(); i++)
      if ( fabs(missingET->P4().DeltaPhi(sigjets[i]->P4())) < 0.5 ) return;
  countCutflowEvent("08_dphi");
  
  if (sigjets[0]->PT < 300.) return;
  countCutflowEvent("09_pt>300");
  
  if (fabs(sigjets[0]->Eta) > 2.0) return;
  countCutflowEvent("10_eta<2.0");
  
  for(int i = 0; i < 21; i++)
    if (met > signal_region_borders[i][0] and met < signal_region_borders[i][1]) countSignalEvent(signal_regions[i]);

  return;
  
}

void Atlas_phys_2014_007::finalize() {
  // Whatever should be done after the run goes here
}       

bool Atlas_phys_2014_007::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

std::vector<Jet*> Atlas_phys_2014_007::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
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

bool Atlas_phys_2014_007::sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }
