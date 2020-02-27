#include "atlas_1712_08119.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1712_08119::initialize() {
  setAnalysisName("atlas_1712_08119");          
  setInformation(""
    "# electroweakinos search with soft leptons\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("SRI-MLL-03;SRI-MLL-05;SRI-MLL-10;SRI-MLL-20;SRI-MLL-30;SRI-MLL-40;SRI-MLL-60;SRI-MT2-102;SRI-MT2-105;SRI-MT2-110;SRI-MT2-120;SRI-MT2-130;SRI-MT2-999;SRE-MLL-03;SRE-MLL-05;SRE-MLL-10;SRE-MLL-20;SRE-MLL-30;SRE-MLL-40;SRE-MLL-60;SRM-MLL-03;SRM-MLL-05;SRM-MLL-10;SRM-MLL-20;SRM-MLL-30;SRM-MLL-40;SRM-MLL-60;SRE-MT2-102;SRE-MT2-105;SRE-MT2-110;SRE-MT2-120;SRE-MT2-130;SRE-MT2-999;SRM-MT2-102;SRM-MT2-105;SRM-MT2-110;SRM-MT2-120;SRM-MT2-130;SRM-MT2-999");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1712_08119::analyze() {
  
  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsLoose = filterPhaseSpace(electronsLoose, 4.5, -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 4.5, -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 4., -2.5, 2.5);  
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5); 
  
  std::vector<Jet*> bjets;
  std::vector<Jet*> lightjets;
  
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta) < 2.5 && checkBTag(jets[i]) ) bjets.push_back(jets[i]);
    else lightjets.push_back(jets[i]);  
    
  lightjets = overlapRemoval(lightjets, electronsLoose, 0.2, "y");
  bjets = overlapRemoval_muon_jet_tracks(bjets, muonsCombined, 0.4, 2);    
  lightjets = overlapRemoval_muon_jet_tracks(lightjets, muonsCombined, 0.4, 2); 
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.4, "y");  
  electronsLoose = overlapRemoval(electronsLoose, lightjets, 0.4, "y"); 
  electronsTight = overlapRemoval(electronsTight, bjets, 0.4, "y");  
  electronsTight = overlapRemoval(electronsTight, lightjets, 0.4, "y");    
  muonsCombined = overlapRemoval(muonsCombined, bjets, 0.4, "y");  
  muonsCombined = overlapRemoval(muonsCombined, lightjets, 0.4, "y");      
  
  std::vector<Electron*> electrons_base = electronsLoose;
  std::vector<Muon*> muons_base = muonsCombined;
    
  std::vector<Electron*> electrons_signal = Isolate_leptons_with_inverse_track_isolation_cone(electronsTight, tracks, towers, 0.3, 10., 0.2, 0.15, 0.2, true);
  std::vector<Muon*> muons_signal = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined,  tracks, towers, 0.3, 10., 0.2, 0.06, 0.06, false);
  
  lightjets = filterPhaseSpace(lightjets, 30., -2.8, 2.8); 
  
  countCutflowEvent("00_all"); 
  
  double met = missingET->P4().Et();
  
  if ( met < 200.) return;  
  countCutflowEvent("01_MET>200"); 
  
  if ( muons_base.size() + electrons_base.size() != 2 ) return;
  countCutflowEvent("02_2_base_leptons"); 
  
  if ( muons_signal.size() + electrons_signal.size() != 2 ) return;  
  countCutflowEvent("03_2_signal_leptons"); 
  
  std::vector<TLorentzVector> leptons;
  std::string tag;
  bool sc = false;
  if ( muons_signal.size() == 2) {
    leptons.push_back(muons_signal[0]->P4());
    leptons.push_back(muons_signal[1]->P4());
    tag = "M";
    if (muons_signal[0]->Charge * muons_signal[1]->Charge < 0) sc = true; 
  }
  else if ( electrons_signal.size() == 2) {
    leptons.push_back(electrons_signal[0]->P4());
    leptons.push_back(electrons_signal[1]->P4());
    tag = "E";
    if (electrons_signal[0]->Charge * electrons_signal[1]->Charge < 0) sc = true; 
  }  
  else return;
  
  countCutflowEvent("04_same_flav");
  
  if ( !sc ) return;  
  countCutflowEvent("05_opposite_charge");
  
  if (leptons[0].Perp() < 5. ) return;  
  countCutflowEvent("06_lep_pt>5");
  
  if (bjets.size() != 0 ) return;  
  countCutflowEvent("07_bveto");
  
  if ( !lightjets.size() or lightjets[0]->PT < 100. ) return;  
  countCutflowEvent("08_ptjet>100");
  
  if ( fabs(lightjets[0]->P4().DeltaPhi(missingET->P4())) < 2. ) return;
  countCutflowEvent("09_dPhi(jet1,MET)>2");  
  
  for ( int i = 1; i < lightjets.size(); i++ )
    if ( fabs(jets[i]->P4().DeltaPhi(missingET->P4())) < 0.4 ) return;
  countCutflowEvent("10_dPhi(jet,MET)>0.4"); 
  
  double mtau = mtautau(leptons[0], leptons[1], missingET->P4());
  if ( mtau >= 0. and mtau < 160. ) return;
  countCutflowEvent("11_tauveto");
  
  double mll = (leptons[0] + leptons[1]).M();
  if ( mll < 1.) return;
  countCutflowEvent("12_mll>1"); 
  
  if ( mll > 3. and mll < 3.2) return;
  countCutflowEvent("13_mllveto");
  
  if ( mll > 60.) return;
  countCutflowEvent("14_mll<60");   
  
  double drll = fabs(leptons[0].DeltaR(leptons[1]));
  if (drll < 0.05) return;
  countCutflowEvent("15_dRll>0.05");   
  
  double htlep = leptons[0].Perp() + leptons[1].Perp();
  if ( met/htlep > std::max(5., 15.-2.*mll) ) {
     countCutflowEvent("MLL_16_MET/HTL");     
     if (drll < 2.) {
       countCutflowEvent("MLL_17_dRll<2");
       double mtl1 = mT( leptons[0],  missingET->P4(), 0.);
       if ( mtl1 < 70. ) {
	 countCutflowEvent("MLL_18_mTl1<70");
	 countSignalEvent("SRI-MLL-60");
	 if ( mll < 40. ) { countCutflowEvent("MLL_19_Mll<40"); countSignalEvent("SRI-MLL-40"); }
	 if ( mll < 30. ) { countCutflowEvent("MLL_20_Mll<30"); countSignalEvent("SRI-MLL-30"); }
	 if ( mll < 20. ) { countCutflowEvent("MLL_21_Mll<20"); countSignalEvent("SRI-MLL-20"); }
	 if ( mll < 10. ) { countCutflowEvent("MLL_22_Mll<10"); countSignalEvent("SRI-MLL-10"); }
	 if ( mll < 5. ) { countCutflowEvent("MLL_23_Mll<5"); countSignalEvent("SRI-MLL-05"); }
	 if ( mll < 3. ) { countCutflowEvent("MLL_24_Mll<3"); countSignalEvent("SRI-MLL-03"); countSignalEvent("SR"+tag+"-MLL-03");}
	 
	 if ( mll > 3.2 and mll < 5.) countSignalEvent("SR"+tag+"-MLL-05");
	 if ( mll > 5. and mll < 10.) countSignalEvent("SR"+tag+"-MLL-10");
	 if ( mll > 10. and mll < 20.) countSignalEvent("SR"+tag+"-MLL-20");
	 if ( mll > 20. and mll < 30.) countSignalEvent("SR"+tag+"-MLL-30");
	 if ( mll > 30. and mll < 40.) countSignalEvent("SR"+tag+"-MLL-40");
	 if ( mll > 40. ) countSignalEvent("SR"+tag+"-MLL-60");
       }
     }
  }
  
  double mt2_val = mT2( leptons[0], leptons[1], 100., missingET->P4());
  if ( met/htlep > std::max(3., 15.-2.*(mt2_val - 100.) ) ) {
     countCutflowEvent("MT2_16_MET/HTL"); 
     countSignalEvent("SRI-MT2-999");
     if ( mt2_val < 130. ) { countCutflowEvent("MT2_17_MT2<130"); countSignalEvent("SRI-MT2-130"); }
     if ( mt2_val < 120. ) { countCutflowEvent("MT2_18_MT2<120"); countSignalEvent("SRI-MT2-120"); }
     if ( mt2_val < 110. ) { countCutflowEvent("MT2_19_MT2<110"); countSignalEvent("SRI-MT2-110"); }
     if ( mt2_val < 105. ) { countCutflowEvent("MT2_20_MT2<105"); countSignalEvent("SRI-MT2-105"); }
     if ( mt2_val < 102. ) { countCutflowEvent("MT2_21_MT2<102"); countSignalEvent("SRI-MT2-102"); countSignalEvent("SR"+tag+"-MT2-102"); }
     
     if ( mt2_val > 102 and mt2_val < 105) countSignalEvent("SR"+tag+"-MT2-105");  
     if ( mt2_val > 105 and mt2_val < 110) countSignalEvent("SR"+tag+"-MT2-110");  
     if ( mt2_val > 110 and mt2_val < 120) countSignalEvent("SR"+tag+"-MT2-120");
     if ( mt2_val > 120 and mt2_val < 130) countSignalEvent("SR"+tag+"-MT2-130");
     if ( mt2_val > 130 ) countSignalEvent("SR"+tag+"-MT2-999"); 
  }  
}

void Atlas_1712_08119::finalize() {
  // Whatever should be done after the run goes here
}       


double Atlas_1712_08119::mtautau(const TLorentzVector lep1, const TLorentzVector lep2, const TLorentzVector ptmiss) {
  
  double wg = lep1.X()*lep2.Y() - lep1.Y()*lep2.X();
  if (wg == 0. ) return 0.;
      
  double w1 = ptmiss.X()*lep2.Y() - ptmiss.Y()*lep2.X();
  double w2 = lep1.X()*ptmiss.Y() - lep1.Y()*ptmiss.X();
  
  double x1 = w1/wg;
  double x2 = w2/wg;
  
  double mtau2 = 2.*(lep1*lep2)*(1. + x1)*(1. + x2);
  
  if (mtau2 >= 0) return sqrt(mtau2); else return -sqrt(-mtau2);
}

bool Atlas_1712_08119::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

std::vector<Jet*> Atlas_1712_08119::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
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