#include "atlas_1908_08215.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1908_08215::initialize() {
  setAnalysisName("atlas_1908_08215");          
  setInformation(""
    "# search for electroweak production of charginos and sleptons decaying into\n"
    "# final states with 2 leptons and MET, 13 TeV, 139/fb\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("SR-DF-0J-INC100;SR-DF-0J-INC160;SR-DF-0J-EXC100;SR-DF-0J-EXC120;SR-DF-1J-INC100;SR-DF-1J-INC160;SR-DF-1J-EXC100;SR-DF-1J-EXC120;SR-SF-0J-INC100;SR-SF-0J-INC160;SR-SF-0J-EXC100;SR-SF-0J-EXC120;SR-SF-1J-INC100;SR-SF-1J-INC160;SR-SF-1J-EXC100;SR-SF-1J-EXC120;SR-SF-0Ja;SR-SF-0Jb;SR-SF-0Jc;SR-SF-0Jd;SR-SF-0Je;SR-SF-0Jf;SR-SF-0Jg;SR-SF-0Jh;SR-SF-0Ji;SR-DF-0Ja;SR-DF-0Jb;SR-DF-0Jc;SR-DF-0Jd;SR-DF-0Je;SR-DF-0Jf;SR-DF-0Jg;SR-DF-0Jh;SR-DF-0Ji;SR-SF-1Ja;SR-SF-1Jb;SR-SF-1Jc;SR-SF-1Jd;SR-SF-1Je;SR-SF-1Jf;SR-SF-1Jg;SR-SF-1Jh;SR-SF-1Ji;SR-DF-1Ja;SR-DF-1Jb;SR-DF-1Jc;SR-DF-1Jd;SR-DF-1Je;SR-DF-1Jf;SR-DF-1Jg;SR-DF-1Jh;SR-DF-1Ji");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1908_08215::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47, false);
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47, false);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5, false); 
  jets = filterPhaseSpace(jets, 20., -2.4, 2.4);
  
  electronsTight = filterIsolation(electronsTight);
  muonsCombined = filterIsolation(muonsCombined);
  electronsTight = Isolate_leptons_with_inverse_track_isolation_cone(electronsTight, tracks, towers, 0.2, 10., 0.2, 0.06, 0.2, false);
  muonsCombined = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined, tracks, towers, 0.3, 10., 0.2, 0.06, 0.2, false);
  
  jets = overlapRemoval(jets, electronsLoose, 0.2);
  jets = overlapRemoval_muon_jet_tracks(jets, muonsCombined, 0.2, 2); 
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  electronsLoose = overlapRemoval(electronsLoose, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  electronsTight = overlapRemoval(electronsTight, muonsCombined, 0.05);
  
  countCutflowEvent("00_all");
  //trigger arXiv:1611.09661
  bool trigger = false;
  if (electronsLoose.size() > 1 and electronsLoose[1]->PT > 12.) trigger = true;
  else if (muonsCombined.size() > 1 and muonsCombined[1]->PT > 10.) trigger = true;
  else if (electronsLoose.size() > 0 and muonsCombined.size() > 0 and electronsLoose[0]->PT > 17. and  muonsCombined[0]->PT > 14.) trigger = true;
  
  if (!trigger) return;
  countCutflowEvent("01_trigger");
  
  std::string sf = "SF";
  std::vector<TLorentzVector> leptons;
  if ( electronsTight.size() == 2 and muonsCombined.size() == 0 and electronsTight[0]->Charge * electronsTight[1]->Charge < 0) {
    leptons.push_back(electronsTight[0]->P4());
    leptons.push_back(electronsTight[1]->P4());
  }
  else if ( electronsTight.size() == 0 and muonsCombined.size() == 2 and muonsCombined[0]->Charge * muonsCombined[1]->Charge < 0) {
    leptons.push_back(muonsCombined[0]->P4());
    leptons.push_back(muonsCombined[1]->P4());
  } 
  else if ( electronsTight.size() == 1 and muonsCombined.size() == 1 and muonsCombined[0]->Charge * electronsTight[0]->Charge < 0) {
    if ( electronsTight[0]->PT > muonsCombined[0]->PT ) {
      leptons.push_back(electronsTight[0]->P4());
      leptons.push_back(muonsCombined[0]->P4());
    }
    else {
      leptons.push_back(muonsCombined[0]->P4());
      leptons.push_back(electronsTight[0]->P4());
    }
    sf = "DF";
  }
  else return;
  if (rand()/(RAND_MAX+1.) > 0.85) return; //correction for the apparent too high efficiency
  countCutflowEvent("02_2OS_signal_lep");
  
  if (leptons[1].Perp() < 25.) return;
  countCutflowEvent("03_lepton_pt");
  
  double mlep = (leptons[0] + leptons[1]).M() ;
  if ( mlep < 25. ) return;
  countCutflowEvent("04_mll");
  
  for (int i = 0; i < jets.size(); i++) 
    if (checkBTag(jets[i])) return;
  countCutflowEvent("05_bveto");  
    
  std::string nj;
  if (jets.size() == 0) nj = "0J";
  else if (jets.size() == 1) nj = "1J";
  else return;
  countCutflowEvent(sf+nj+"_06_nj");
  
  if ( (sf == "DF" and mlep < 100.) or (sf == "SF" and mlep < 121.2) )return;
  countCutflowEvent(sf+nj+"_07_mll");    
  
  double met = missingET->P4().Et();
  if (met < 110.) return;
  countCutflowEvent(sf+nj+"_08_MET");   
  
  // a rough guess based on ATLAS-CONF-2018-038 tab.1 fig.3
  double sigma = sqrt( pow(0.03*leptons[0].Perp(),2) + pow(0.03*leptons[1].Perp(),2) + pow(8.9,2));
  if (nj == "1J") sigma = sqrt( pow(sigma,2) + pow( 0.08*jets[0]->PT,2));
  if ( met/sigma < 10.) return;
  countCutflowEvent(sf+nj+"_09_METsig");   
  
  double mt2_val = mT2( leptons[0], leptons[1], 0., missingET->P4());
  if (mt2_val < 100.) return;
  countCutflowEvent(sf+nj+"_10_mT2"); 
  
  countSignalEvent("SR-"+sf+"-"+nj+"-INC100");
  if (mt2_val > 160.) countSignalEvent("SR-"+sf+"-"+nj+"-INC160");
  if (mt2_val < 120.) countSignalEvent("SR-"+sf+"-"+nj+"-EXC100");
  if (mt2_val > 120. and mt2_val < 160. ) countSignalEvent("SR-"+sf+"-"+nj+"-EXC120");
  
  if (mt2_val < 105.) countSignalEvent("SR-"+sf+"-"+nj+"a");
  else if (mt2_val < 110.) countSignalEvent("SR-"+sf+"-"+nj+"b");
  else if (mt2_val < 120.) countSignalEvent("SR-"+sf+"-"+nj+"c");
  else if (mt2_val < 140.) countSignalEvent("SR-"+sf+"-"+nj+"d");
  else if (mt2_val < 160.) countSignalEvent("SR-"+sf+"-"+nj+"e");
  else if (mt2_val < 180.) countSignalEvent("SR-"+sf+"-"+nj+"f");
  else if (mt2_val < 220.) countSignalEvent("SR-"+sf+"-"+nj+"g");
  else if (mt2_val < 260.) countSignalEvent("SR-"+sf+"-"+nj+"h");
  else countSignalEvent("SR-"+sf+"-"+nj+"i");
  
  return;
  
}

void Atlas_1908_08215::finalize() {
  // Whatever should be done after the run goes here
}       


bool Atlas_1908_08215::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

std::vector<Jet*> Atlas_1908_08215::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
  std::vector<Jet*> passed;
  for (std::vector<Jet*>::iterator jet = cand_jets.begin(); jet != cand_jets.end(); jet++) {
  
    if (check_nTrack_jet(*jet, tracks, nTracks)) {
      passed.push_back(*jet);
      continue;
    }
    double dR = deltaR;
    bool iso = true;
        
    for (std::vector<Muon*>::iterator mu=cand_muons.begin(); mu!=cand_muons.end(); mu++) 
      if ((*jet)->P4().DeltaR((*mu)->P4()) < dR) {
	iso = false;
	break;
      }
        		    
    if (iso) passed.push_back(*jet);
  }
  
  return passed;
}
