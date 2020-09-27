#include "atlas_1803_02762.h"
// AUTHOR: Ipsita Saha
//  EMAIL: ipsita.saha@ipmu.jp
void Atlas_1803_02762::initialize() {
  setAnalysisName("atlas_1803_02762");          
  setInformation(""
    "# ATLAS\n"
    "# EW SUSY searches\n"
    "# 2leptons, 2lep+jets+etmiss, 3lep\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("S-2l0j-DF-100;S-2l0j-DF-150;S-2l0j-DF-200;S-2l0j-DF-300;S-2l0j-SF-loose;S-2l0j-SF-tight;S-2lj-SR2-int;S-2lj-SR2-high;S-2lj-SR2-low;S-3l-WZ-0Ja;S-3l-WZ-0Jb;S-3l-WZ-OJc;S-3l-WZ-1Ja;S-3l-WZ-1Jb;S-3l-WZ-1Jc;S-3l-slep-a;S-3l-slep-b;S-3l-slep-c;S-3l-slep-d;S-3l-slep-e");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.
 bookCutflowRegions("00_all;01_trigger");
 bookCutflowRegions("2l_01_lepno;2l_02_leading_leppt;2l_03_no_bjets;2l_04_no_jets;2l_05_mll;2l_06_SFOS;2l_07_SF_mll;2l_08_SF_loose_mT2;2l_06_DFOS;2l_07_DF_mll;2l_08_DF_100_mT2");
 bookCutflowRegions("2l_j_01_nbj;2l_j_02_leading_leppt;2l_j_03_nj;2l_j_04_jetpt;2l_j_05_met;2l_j_06_ptZ;2l_j_07_ptW;2l_j_08_mZ;2l_j_09_mW;2l_j_10_mt2;2l_j_11_delpWmet;2l_j_12_DRjj;2l_j_13_DRll_int");
 bookCutflowRegions("2l_2j_04_met;2l_2j_05_nj;2l_2j_06_ptj;2l_2j_07_mZ;2l_2j_08_mW;2l_2j_09_ptZ;2l_2j_10_dphimetZ;2l_2j_11_dphimetW;2l_2j_12_met_ratio_ptW;2l_2j_13_met_ratio_ptZ");
 bookCutflowRegions("2l_3j_05_nj;2l_3j_06_ptj;2l_3j_07_mZ;2l_3j_08_mW;2l_3j_09_etaZ;2l_3j_10_ptZ;2l_3j_11_dphimetISR;2l_3j_12_dphimetj1;2l_3j_13_dphimetW;2l_3j_14_met_ratio_ISR;2l_3j_15_DRjj");
 bookCutflowRegions("3l_05_SFOS;3l_01_lepno;3l_02_no_bjet;3l_03_ptl1;3l_04_ptl3;3l_06_WZ_m3l;3l_07_WZ_mll_mZ;3l_07_slep_mll_mZ;3l_08_WZ0j_nj;3l_09_WZ0ja_met;3l_10_WZ0ja_mT;3l_09_WZ0jb_met;3l_10_WZ0jb_mT;3l_09_WZ0jc_met;3l_10_WZ0jc_mT;3l_08_WZ1j_nj;3l_09_WZ1ja_met;3l_10_WZ1ja_mT;3l_11_WZ1ja_pt3l;3l_12_WZ1ja_ptj1;3l_09_WZ1jb_met;3l_10_WZ1jb_mT;3l_10_WZ1jc_mT;3l_11_WZ1jc_ptl3;3l_08_slep_mT;3l_09_slep_met;3l_10_slep_mSFOS_low;3l_10_slep_mSFOS_high;3l_11_slepa_ptl3;3l_11_slepb_ptl3;3l_11_slepc_ptl3;3l_11_slepd_ptl3;3l_11_slepe_ptl3");
  // You should initialize any declared variables here
}

void Atlas_1803_02762::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  electronsLoose = filterPhaseSpace(electronsLoose, 10.0, -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.7, 2.7);
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);

  std::vector<Electron*> baseline_e = electronsLoose;
  std::vector<Muon*> baseline_mu = muonsCombined;
  
  electronsLoose = overlapRemoval(electronsLoose,muonsCombined,0.01);


  std::vector<Jet*> bjets;
  std::vector<Jet*> lightjets;
  for (int i = 0; i < jets.size(); i++)
    if ( fabs(jets[i]->Eta) < 4.5 && jets[i]->PT > 20. && checkBTag(jets[i]) ) bjets.push_back(jets[i]);
    else lightjets.push_back(jets[i]);
   
  lightjets = overlapRemoval(lightjets,electronsLoose,0.2);
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.2);

  electronsLoose = overlapRemoval(electronsLoose, lightjets, 0.4);
  lightjets = overlapRemoval_muon_jet_tracks(lightjets, muonsCombined, 0.2, 2);
  bjets = overlapRemoval_muon_jet_tracks(bjets, muonsCombined, 0.2, 2);
  muonsCombined = overlapRemoval(muonsCombined, lightjets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, bjets, 0.4);
  
  lightjets = filterPhaseSpace(lightjets, 20., -2.4, 2.4);
  bjets = filterPhaseSpace(bjets, 20., -2.4, 2.4);
  std::vector<Electron*> signal_electron = filterPhaseSpace(electronsLoose, 20., -2.47, 2.47);
  std::vector<Muon*> signal_muon = filterPhaseSpace(muonsCombined, 20., -2.4, 2.4);	 

  std::vector<Jet*> jets2l = filterPhaseSpace(lightjets, 60., -2.4, 2.4);
  double met = missingET->P4().Et();
   
// Define lepton finalstate 

    std::vector<FinalStateObject*> leps;
    FinalStateObject* lep;
    for( int i=0; i < signal_electron.size(); i++) {
        lep = new FinalStateObject( signal_electron[i] );
        leps.push_back(lep);
    }
    for( int i=0; i < signal_muon.size(); i++) {
        lep = new FinalStateObject( signal_muon[i] );
        leps.push_back(lep);
    }
    std::sort( leps.begin(), leps.end(), sortByPT );

  countCutflowEvent("00_all");
//  lepton trigger
  bool trigger = false;
 // if(leps.size() > 1 && leps[0]->PT > 22. && leps[1]->PT > 8.) trigger = true; 
  if((electronsLoose.size() > 0 && electronsLoose[0]->PT > 22.) || (muonsCombined.size() > 0 && muonsCombined[0]->PT >22.) || (electronsLoose.size() >0 && muonsCombined.size() > 0 && (electrons[0]->PT > 22. || muonsCombined[0]->PT > 22.))) trigger = true; 
  //&& leps[1]->PT > 8.) trigger = true;  
  if(!trigger) return;

  countCutflowEvent("01_trigger");
 // bjet veto
  bool nbjet = false;
  if(bjets.size() == 0) nbjet = true ; 
// 2lep + 0Jets signal region
  bool lep_2l = false;
  if(leps.size() == 2 && baseline_e.size() + baseline_mu.size() == 2 ) lep_2l = true;

  if(lep_2l && leps[0]->Charge* leps[1]->Charge <0){
	countCutflowEvent("2l_01_lepno");

  bool SFll = false;

  if(leps[0]->Type == leps[1]->Type) SFll = true;

  double mll_2l = (leps[0]->P4() + leps[1]->P4()).M();
  double mT2_2l = mT2(leps[0]->P4(), leps[1]->P4(), 0., missingET->P4());

  
  	if(leps[0]->PT > 25.){
		countCutflowEvent("2l_02_leading_leppt"); 
			if(bjets.size() == 0){
				countCutflowEvent("2l_03_no_bjets");
				if(jets2l.size() == 0){
					countCutflowEvent("2l_04_no_jets");
					if(mll_2l > 40.) { 
						countCutflowEvent("2l_05_mll");
						if(SFll){
						countCutflowEvent("2l_06_SFOS");
							if(mll_2l > 111.){
							countCutflowEvent("2l_07_SF_mll");
								if(mT2_2l > 100.){
								countCutflowEvent("2l_08_SF_loose_mT2");
								countSignalEvent("S-2l0j-SF-loose");
								}
							}
							if(mll_2l > 300. && mT2_2l > 130.)countSignalEvent("S-2l0j-SF-tight");	
						}
						if(!SFll){
							countCutflowEvent("2l_06_DFOS");
							if(mll_2l > 111.){
								countCutflowEvent("2l_07_DF_mll");
								if(mT2_2l > 100.){
									countCutflowEvent("2l_08_DF_100_mT2");
									countSignalEvent("S-2l0j-DF-100");
								}
								if(mT2_2l > 150.)countSignalEvent("S-2l0j-DF-150");	
								if(mT2_2l > 200.)countSignalEvent("S-2l0j-DF-200");	
								if(mT2_2l > 300.)countSignalEvent("S-2l0j-DF-300");	
							}
						
						}
					}
				}
			}
		}
				
// 2 leptons + Jets signal region
   int n_leptons = leps.size();
// Leading leptons mll
  if(nbjet){
	countCutflowEvent("2l_j_01_nbj");
  	if(leps[1]->PT > 25.){
		countCutflowEvent("2l_j_02_leading_leppt"); 
 	if(lightjets.size() > 1){
		countCutflowEvent("2l_j_03_nj");
// Kinematic Variables
 	TLorentzVector dilepton = leps[0]->P4()+leps[1]->P4();
	TLorentzVector W01 = lightjets[0]->P4()+ lightjets[1]->P4(); 
	TLorentzVector ZMET = dilepton + missingET->P4();
  	double DRll = leps[0]->P4().DeltaR(leps[1]->P4());
   
  
  double minDphiMETJet = 1e10;
  int WminJ0 = -1;
  for (int j=0;j<lightjets.size();j++) {
    double Dphi = fabs(lightjets[j]->P4().DeltaPhi(ZMET));
    if (Dphi<minDphiMETJet) {
      minDphiMETJet = Dphi;
      WminJ0 = j;
    }
  }
  minDphiMETJet = 1e10;
  int WminJ1 = -1;
  for (int j=0;j<lightjets.size();j++) {
    if (j==WminJ0) continue;
    double Dphi = fabs(lightjets[j]->P4().DeltaPhi(ZMET));
    if (Dphi<minDphiMETJet) {
      minDphiMETJet = Dphi;
      WminJ1 = j;
    }
  }

  TLorentzVector Wmin = lightjets[WminJ0]->P4()+lightjets[WminJ1]->P4();  
  TLorentzVector ISR;
  for (int j=0;j<lightjets.size();j++) {
    if (j==WminJ0) continue;
    if (j==WminJ1) continue;
    ISR += lightjets[j]->P4();
  }

  double ptZ = dilepton.Pt();
  double ptW = Wmin.Pt(); 
  double deltaphi_metW = fabs(missingET->P4().DeltaPhi(Wmin));
  double deltaphi_metZ = fabs(missingET->P4().DeltaPhi(dilepton));
  double DRjj = lightjets[WminJ0]->P4().DeltaR(lightjets[WminJ1]->P4());

//   if 2l + J Int & high ; njets >= 2 
	if (lightjets[1]->PT > 30.){
		countCutflowEvent("2l_j_04_jetpt");		
	 	if(met > 150.){
			countCutflowEvent("2l_j_05_met");
			if(ptZ > 80.){
				countCutflowEvent("2l_j_06_ptZ");
				if(ptW > 100.){
					countCutflowEvent("2l_j_07_ptW");
					if(mll_2l > 81. && mll_2l < 101.){
						countCutflowEvent("2l_j_08_mZ");
						//if(W01.M() > 70. && W01.M() < 100.){
						if(Wmin.M() > 70. && Wmin.M() < 100.){
							countCutflowEvent("2l_j_09_mW");
							if(mT2_2l > 100.){
								countCutflowEvent("2l_j_10_mt2");
								if(deltaphi_metW >= 0.5 && deltaphi_metW <= 3.0){
									countCutflowEvent("2l_j_11_delpWmet");
									if(DRjj < 1.5){
										countCutflowEvent("2l_j_12_DRjj");
										if(DRll < 1.8){
											countCutflowEvent("2l_j_13_DRll_int");
											countSignalEvent("S-2lj-SR2-int");
										}
									}
								}
							}
						}
					}
				}
			}
		}
	 	if(met > 200. && ptZ > 80. && ptW > 100. && mll_2l >= 80. && mll_2l <= 100. && Wmin.M() >= 70. && Wmin.M() <= 100. && mT2_2l > 100. && deltaphi_metW >= 0.5 && deltaphi_metW <= 3.0 && DRjj < 0.5 && DRll < 1.8) countSignalEvent("S-2lj-SR2-high");
	}

// SR2 - low 2j region
	if(met > 100.){
		countCutflowEvent("2l_2j_04_met");
// exact 2 jets
		if(lightjets.size() == 2){
			countCutflowEvent("2l_2j_05_nj");
			if(lightjets[0]->PT > 30. && lightjets[1]->PT > 30.){
				countCutflowEvent("2l_2j_06_ptj");
				if(mll_2l > 81. && mll_2l < 101.){
					countCutflowEvent("2l_2j_07_mZ");
					if(Wmin.M() > 70. && Wmin.M() < 90.){
						countCutflowEvent("2l_2j_08_mW");
						if(ptZ > 60.){
							countCutflowEvent("2l_2j_09_ptZ");
							if(deltaphi_metZ < 0.8){
								countCutflowEvent("2l_2j_10_dphimetZ");
								if(deltaphi_metW > 1.5){
									countCutflowEvent("2l_2j_11_dphimetW");
									if(met/ptW < 0.8){
										countCutflowEvent("2l_2j_12_met_ratio_ptW");
									 	if(met/ptZ > 0.6 && met/ptZ < 1.6){
											countCutflowEvent("2l_2j_13_met_ratio_ptZ");
											countSignalEvent("S-2lj-SR2-low");	
										}
									}
								}
							}
						}
					}
				}
			}
		}
		if(lightjets.size() >= 3 && lightjets.size() <= 5){
			countCutflowEvent("2l_3j_05_nj");
			if(lightjets[2]->PT > 30.){
				countCutflowEvent("2l_3j_06_ptj");
				if(mll_2l >= 81. && mll_2l <= 101.){
					countCutflowEvent("2l_3j_07_mZ");
					if(Wmin.M() >= 70. && Wmin.M() <= 90.){
						countCutflowEvent("2l_3j_08_mW");
						if(fabs(dilepton.Eta()) < 1.6){
							countCutflowEvent("2l_3j_09_etaZ");
							if(ptZ > 40.){
								countCutflowEvent("2l_3j_10_ptZ");
								if(fabs(missingET->P4().DeltaPhi(ISR)) > 2.4){
									countCutflowEvent("2l_3j_11_dphimetISR");
									if(fabs(missingET->P4().DeltaPhi(lightjets[0]->P4())) > 2.6){
										countCutflowEvent("2l_3j_12_dphimetj1");
										if(deltaphi_metW < 2.2){
											countCutflowEvent("2l_3j_13_dphimetW");
											if(met/ISR.Pt() >  0.4 && met/ISR.Pt() <0.8){
												countCutflowEvent("2l_3j_14_met_ratio_ISR");
											 	if(DRjj < 2.2){
												countCutflowEvent("2l_3j_15_DRjj");
												countSignalEvent("S-2lj-SR2-low");	
												}		
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
// Preselection requirements closes
  }
  } 
 } 
}

// 3 lepton signal region

  // exact 3 leptons
  if(leps.size() == 3 && baseline_e.size()+baseline_mu.size()==3) countCutflowEvent("3l_01_lepno"); 
  else return;
//  no b-tagged jets

  if(!nbjet) return;
  countCutflowEvent("3l_02_no_bjet");
// Lepton PT cuts

  if(leps[1]->PT > 25.)  countCutflowEvent("3l_03_ptl1");
  else return;
  if(leps[2]->PT > 20.)  countCutflowEvent("3l_04_ptl3");
  else return;
   
   double pt3l = (leps[0]->P4() + leps[1]->P4() + leps[2]->P4()).Pt();
   double m3l = (leps[0]->P4() + leps[1]->P4() + leps[2]->P4()).M();


 // Pair of SFOS lep and mTmin
    
  int nLeptons = leps.size();
  int nJets = lightjets.size();
  float minMT = 999.;
  float mll_minMT = -999.;
  int nb_SFOS = 0;
  int leadZindex = -1;
  int subZindex = -1;
  int Windex = -1;

  for(int ilep=0; ilep < nLeptons-1; ilep++){
    for(int jlep=ilep+1; jlep<nLeptons; jlep++){
      int klep = nLeptons - ilep - jlep;

      if( (leps[ilep]->Type == leps[jlep]->Type) && (leps[ilep]->Charge * leps[jlep]->Charge < 0) ){
        nb_SFOS++;

        float tmp_mll = (leps[ilep]->P4() + leps[jlep]->P4()).M();
        float tmp_mt = mT(leps[klep]->P4(), missingET->P4());
	if (tmp_mt < minMT){
	  minMT = tmp_mt;
          mll_minMT = tmp_mll;
          leadZindex = ilep;
          subZindex = jlep;
          Windex = klep;
        }
      }
    }
  }

  if(nb_SFOS ==0) return;
  countCutflowEvent("3l_05_SFOS");

// Require Z peak
	float Zmass = 91.188;
// return if no SFOS
   	if (m3l > 20.){ 
	   countCutflowEvent("3l_06_WZ_m3l");
	if(fabs(mll_minMT - Zmass) <  10.){
		 countCutflowEvent("3l_07_WZ_mll_mZ");
// WZ-0J signal region       
	if(mll_minMT > 81.2 && mll_minMT < 101.2 && nJets==0){
		countCutflowEvent("3l_08_WZ0j_nj");
		if(met>60.  && met<120.){
			countCutflowEvent("3l_09_WZ0ja_met"); 
			if(minMT>110.){
			countCutflowEvent("3l_10_WZ0ja_mT"); 
			countSignalEvent("S-3l-WZ-0Ja");
			}
		}
		if(met>120. && met<170.){
			countCutflowEvent("3l_09_WZ0jb_met"); 
			if(minMT>110.){
			countCutflowEvent("3l_10_WZ0jb_mT"); 
			countSignalEvent("S-3l-WZ-0Jb");
			}
		}	
		if(met>170.){
			countCutflowEvent("3l_09_WZ0jc_met"); 
			if(minMT>110.){
			countCutflowEvent("3l_10_WZ0jc_mT"); 
			countSignalEvent("S-3l-WZ-OJc");
			}
		}
	}
	if(mll_minMT > 81.2 && mll_minMT < 101.2 && nJets>0){
		countCutflowEvent("3l_08_WZ1j_nj"); 
		if(met>120 && met<200){
			countCutflowEvent("3l_09_WZ1ja_met");
			if(minMT>110.){
				countCutflowEvent("3l_10_WZ1ja_mT"); 
				if(pt3l<120.){
					countCutflowEvent("3l_11_WZ1ja_pt3l");
					if(lightjets[0]->PT>70.){
						countCutflowEvent("3l_12_WZ1ja_ptj1");
						countSignalEvent("S-3l-WZ-1Ja");
					}
				}
			}
		}
		if(met>200.){
			countCutflowEvent("3l_09_WZ1jb_met"); 
			if(minMT>110. && minMT<160.){
				 countCutflowEvent("3l_10_WZ1jb_mT"); 
				 countSignalEvent("S-3l-WZ-1Jb");
			}
			if(minMT>160.){
				countCutflowEvent("3l_10_WZ1jc_mT");
				if(leps[2]->PT>35.){
					countCutflowEvent("3l_11_WZ1jc_ptl3");
					countSignalEvent("S-3l-WZ-1Jc");
				}
			}
		}
	}
	} // mll-mZ < 10 cond ends here	
	}
// Slep signal region
	if(fabs(mll_minMT - Zmass) >  10.){
		 countCutflowEvent("3l_07_slep_mll_mZ");
	if(minMT > 110.){
		countCutflowEvent("3l_08_slep_mT");
		if(met > 130.){
			countCutflowEvent("3l_09_slep_met");
			if(mll_minMT < 81.2){
				countCutflowEvent("3l_10_slep_mSFOS_low");
				if(leps[2]->PT > 20.  &&  leps[2]->PT < 30.){
					countCutflowEvent("3l_11_slepa_ptl3"); 
					countSignalEvent("S-3l-slep-a");
				}
				if(leps[2]->PT > 30.){
					countCutflowEvent("3l_11_slepb_ptl3"); 
					countSignalEvent("S-3l-slep-b");
				}
			}
			if(mll_minMT > 101.2){
				countCutflowEvent("3l_10_slep_mSFOS_high"); 
				if(leps[2]->PT > 20.   &&   leps[2]->PT < 50.){
					countCutflowEvent("3l_11_slepc_ptl3"); 
					countSignalEvent("S-3l-slep-c");
				}
				if(leps[2]->PT > 50.   &&   leps[2]->PT < 80.){
					countCutflowEvent("3l_11_slepd_ptl3"); 
					countSignalEvent("S-3l-slep-d");
				}
				if(leps[2]->PT > 80.){
					countCutflowEvent("3l_11_slepe_ptl3"); 
					countSignalEvent("S-3l-slep-e");
  				}

			}
		}
	}
	}
	
}			

void Atlas_1803_02762::finalize() {
  // Whatever should be done after the run goes here
}       
bool Atlas_1803_02762::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {

  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++)
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

std::vector<Jet*> Atlas_1803_02762::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){

  std::vector<Jet*> passed;
  double sum_pt_track_jet;
    for( int i = 1; i < cand_jets.size(); i++)
	sum_pt_track_jet += cand_jets[i]->PT;
  for (std::vector<Jet*>::iterator jet = cand_jets.begin(); jet != cand_jets.end(); jet++) {

    if (check_nTrack_jet(*jet, tracks, nTracks)) {
      passed.push_back(*jet);
      continue;
    for( int i = 1; i < nTracks; i++)
	sum_pt_track_jet += tracks[i]->PT;
    }

    bool iso = true;

    for (std::vector<Muon*>::iterator mu = cand_muons.begin(); mu != cand_muons.end(); mu++)
      if ((*mu)->PT > 0.7*sum_pt_track_jet && (*jet)->P4().DeltaR((*mu)->P4()) < deltaR ) {
        iso = false;
        break;
      }

    if (iso) passed.push_back(*jet);
  }

  return passed;
}
