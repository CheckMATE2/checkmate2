#include "atlas_conf_2016_013.h"
#include <string> 
#include <iostream>
// AUTHOR: Anke Biekoetter
//  EMAIL: biekoetter@physik.rwth-aachen.de

template<class T> std::vector<T*> isolateLeptons(std::vector<T*> leptons, double deltaR);

#include<sstream>
    template <typename T>
    std::string to_string(T value)
    {
      //create an output string stream
      std::ostringstream os ;

      //throw the value into the string stream
      os << value ;

      //convert the string stream into a string and return
      return os.str() ;
    }

void Atlas_conf_2016_013::initialize() {
  setAnalysisName("atlas_conf_2016_013");          
  setInformation(""
    "# Search for production of vector-like top quark pairs and of four\n"
    "# top quarks in the lepton-plus-jets final state in pp collisions at\n"
    "# sqrt(s) = 13 TeV with the ATLAS detector\n"
  "");
  setLuminosity(3.2*units::INVFB);      
  bookSignalRegions("0J_6j_2b;0J_6j_3b;0J_6j_m4b;1J_6j_3b_LM;1J_6j_3b-HM;1J_6j_m4b_LM;1J_6j_m4b-HM;m2J_6j_2b;m2J_6j_3b;m2J_6j_m4b");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  //hist_a_jet = new TH1I("hist_a_jet", "jet", 11, 4.5, 15.5);
  //hist_b_bjet = new TH1I("hist_b_bjet", "bjet", 7, 1.5, 8.5);  
  //hist_d_masstagged = new TH1I("hist_d_masstagged", "masstagged", 6, -0.5, 5.5);
  //hist_c_largeRmass = new TH1F("hist_c_largeRmass", "largeRmass", 40, 0., 400.);
  //hist_e_mbb = new TH1F("hist_e_mbb", "mbb", 30, 0., 300.);
  //hist_f_meff = new TH1F("hist_f_meff", "meff", 20, 0., 3000.);
  //hist_g_meff2b = new TH1F("hist_g_meff2b", "meff2b", 6, 0., 3000.);
  //hist_g_meff3b = new TH1F("hist_g_meff3b", "meff3b", 6, 0., 3000.);
  //hist_g_meffm4b = new TH1F("hist_g_meffm4b", "meffm4b", 6, 0., 3000.);
}

void Atlas_conf_2016_013::analyze() {

	missingET->addMuons(muonsCombined);  
	
	// object reconstruction
	
	//Candidate events are required to have a reconstructed vertex with at least two associated tracks with
	//pT > 400 MeV, and the vertex with the highest sum of squared transverse momentum of the tracks is considered as primary vertex.
  // Commented out as all signal events will pass this
	//int pt400MeV=0;
	//for(int i=0;i<tracks.size();i++){
  //  	if(tracks[i]->PT>0.4) pt400MeV++;
	//}
	//if (pt400MeV<2) return;
	
	/////////////////////////
    //     candidates      //
    /////////////////////////
	
	// Electron candidates [49–51] are reconstructed from energy clusters in the EM calorimeter that are matched
	// to reconstructed tracks in the inner detector and have |eta_cluster|<2.47, excluding the transition region
	// 1.37 < |eta_cluster| < 1.52 between sections of the EM calorimeter.  They are also required to satisfy “me-
	// dium” quality requirements [49] based on calorimeter, tracking and combined variables that provide good
	// separation between electrons and jets.
	electronsMedium = filterPhaseSpace(electronsMedium, 20, -2.47, 2.47, true);
	
	// Muon candidates [52, 53] are reconstructed from track segments
	// in the various layers of the muon spectrometer that are matched with tracks found in the inner detector.
	// The resulting muon candidates are refitted using the complete track information from both detector sys-
	// tems and are required to have |eta| < 2.5

	muonsCombined = filterPhaseSpace(muonsCombined, 20, -2.5, 2.5);
	
	//All jets must have pT > 20 GeV and |η| < 2.5.
	jets = filterPhaseSpace(jets, 20, -2.5, 2.5);	
	bjets = filterBJets(jets);
	
	
	/////////////////////////////
    //     overlap removal     //
    /////////////////////////////   

    // electrons + jets
    overlapRemoval_ej();   
    
	// muon + jets
	overlapRemoval_jm();
	
	// Remove overlap between surviving jets and electrons
    overlapRemoval(electronsMedium, jets, 0.4);
	
	// electrons + muons
    overlapRemoval_em();
    
    ///////////////////////////
    //     re-clustering     //
    ///////////////////////////

    recluster_top_jets();
	
	//After resolving the overlaps, the final lepton candidates and small-R jets used in the analysis must have
	// pT > 30 GeV.  
	electronsMedium = filterPhaseSpace(electronsMedium, 30);
	muonsCombined = filterPhaseSpace(muonsCombined, 30);
	jets = filterPhaseSpace(jets, 30);
	bjets = filterBJets(jets);
		
    
    //////////////////////////////
    //     signal particles     //
    //////////////////////////////

    // For each surviving electron see if it passes _tight_ quality criteria
    std::vector<Electron*> tmp(std::min(electronsMedium.size(), electronsTight.size()));
    std::vector<Electron*>::iterator it = std::set_intersection(electronsMedium.begin(), electronsMedium.end(), electronsTight.begin(), electronsTight.end(), tmp.begin());
    tmp.resize(it-tmp.begin());
    electronsTight = tmp;

    ///////////////////////
    //     isolation     //
    ///////////////////////

    electronsTight = isolateLeptons(electronsTight, 0.2);
    muonsCombined = isolateLeptons(muonsCombined, 0.3);

	
	// The resulting electron candidates must also satisfy “tight” quality requirements [49].
	
	// The track isolation cone radius is given by the smaller of delata R = 10 GeV/pT_l, where pT_l is the 
	// lepton pT, and delta R = 0.2 (0.3) for electrons (muons).
	
	// The missing transverse momentum (MET) is defined as the magnitude of the negative vector sum of the
	// pT of all selected and calibrated objects in the event, including a term to account for soft energy in the
	// event that is not associated to any of the selected objects. This soft term is calculated from inner detector
	// tracks matched to the selected primary vertex in the event [68, 69].
	
	
	//////////////////////////////////////////////////
	// event preselection
	//////////////////////////////////////////////////
	if ( !((electronsTight.size() == 1 && muonsCombined.size() == 0) 
		|| (electronsTight.size() == 0 && muonsCombined.size() == 1) ) ) return;
	if (jets.size() < 5) return;
	
	double MET = missingET->P4().Et();
	if (MET <= 20) return;
	
	double mTW;
	double m_eff = meff(jets, electronsTight, muonsCombined, missingET);
	
//	Lepton lepton = electronsTight[0];
	if (electronsTight.size() == 1) 
		mTW = std::sqrt(2*electronsTight[0]->PT*MET*( 1-std::cos( electronsTight[0]->P4().DeltaPhi(missingET->P4()) ) ) );
	if (muonsCombined.size() == 1) 
		mTW = std::sqrt(2*muonsCombined[0]->PT*MET*( 1-std::cos( muonsCombined[0]->P4().DeltaPhi(missingET->P4()) ) ) );
	if (mTW <= 60) return;
	

	int nb_jets = bjets.size();
	double mbb_mindR = invMass_minDeltaR(bjets);
	
	
	//////////////////////////////////////////////////
	// signal regions (search channels)
	//////////////////////////////////////////////////
	//hist_a_jet->Fill(jets.size(), weight);
	
	// validation histograms
	if (jets.size() >= 6 && nb_jets >=2) {
		//hist_b_bjet->Fill(nb_jets, weight);
        for (std::vector<double>::const_iterator it=masslargeRjet.begin(); it < masslargeRjet.end(); it++) {
		    //hist_c_largeRmass->Fill(*it, weight);
        }
        masslargeRjet.clear();
		//hist_d_masstagged->Fill(masstagged, weight);
	}
	
	if (jets.size() >= 6) {		
		// 0 mass-tagged jets
		if (masstagged == 0) {
			if (nb_jets == 2) {
				countSignalEvent("0J_6j_2b");
			} else if (nb_jets == 3) {
				countSignalEvent("0J_6j_3b");
			} else if (nb_jets >= 4) {
				countSignalEvent("0J_6j_m4b");
			}
		
		//	 1 mass-tagged jet
		} else if (masstagged == 1) {
			if (nb_jets == 2) {
				countSignalEvent("1J_6j_2b");
			} else if (nb_jets == 3) {
				if ( mbb_mindR < 100 )
					countSignalEvent("1J_6j_3b_LM");
				else 
					countSignalEvent("1J_6j_3b-HM");
			} else if (nb_jets >= 4 ) {
				//hist_e_mbb->Fill(mbb_mindR, weight);
				if ( mbb_mindR < 100 )
					countSignalEvent("1J_6j_m4b_LM");
				else {
					countSignalEvent("1J_6j_m4b-HM");
					//hist_f_meff->Fill(m_eff, weight);
				}
			}
		
		// more than or exactly 2 mass-tagged jets
		} else {
			if (nb_jets == 2) {
				countSignalEvent("m2J_6j_2b");
				//hist_g_meff2b->Fill(m_eff, weight);
			} else if (nb_jets == 3) {
				countSignalEvent("m2J_6j_3b");
				//hist_g_meff3b->Fill(m_eff, weight);
			} else if (nb_jets >= 4) {
				countSignalEvent("m2J_6j_m4b");
				//hist_g_meffm4b->Fill(m_eff, weight);
			}	
		} 
	} // end if 6 jets
}

void Atlas_conf_2016_013::finalize() {
  // Whatever should be done after the run goes here
  //outputHistFileI(hist_a_jet);
  //outputHistFileI(hist_b_bjet);
  //outputHistFile(hist_c_largeRmass);
  //outputHistFileI(hist_d_masstagged);
  //outputHistFile(hist_e_mbb);
  //outputHistFile(hist_f_meff);
  //outputHistFile(hist_g_meff2b);
  //outputHistFile(hist_g_meff3b);
  //outputHistFile(hist_g_meffm4b);
}
	
	//////////////////////////////////////////
    //          OUTPUT HISTOGRAMS           //
    //////////////////////////////////////////
/*
void Atlas_conf_2016_013::outputHistFile(TH1F* inHist) {
    // Assemble absolute filename
    std::string filename = "/media/anke/Daten/Master/Programme/CM2/branches/new_features/CheckMATE_2.0.0Beta_More_HL/results/hist/"+std::string(inHist->GetTitle())+".dat";
    
    std::cout << "Writing file:" << std::endl;
    std::cout << filename << std::endl;
    // Open stream
    std::ofstream* file = new ofstream(filename.c_str());
    // Write standard information
    *file << "# " << inHist->GetTitle() << " \n";
    *file << "#  Bin    MC_Events    (Error)    Normalised_Events    (Error)    Histogram_Normalised_Events    (Error)    Bin_Width    \n";
    for ( int i=1; i<=inHist->GetNbinsX(); i++)
    	if (i < inHist->GetNbinsX())
      		*file << inHist->GetBinCenter(i) << "    " << inHist->GetBinContent(i) << "    " << inHist->GetBinError(i) << "    " 
      			<< normalize(inHist->GetBinContent(i)) << "    " << normalize(inHist->GetBinError(i)) << "    " 
      			<< (inHist->GetBinContent(i))/(inHist->Integral()) << "    " << (inHist->GetBinError(i))/(inHist->Integral()) << "    " 
      			<< inHist->GetBinWidth(i) << std::endl;
      	else if (i == inHist->GetNbinsX())
      		*file << inHist->GetBinCenter(i) << "    " << inHist->GetBinContent(i) + inHist->GetBinContent(i+1) << "    " 
      			<< inHist->GetBinError(i) << "    " << normalize(inHist->GetBinContent(i) + inHist->GetBinContent(i)) << "    " 
      			<< normalize(inHist->GetBinError(i)) << "    " << (inHist->GetBinContent(i) + inHist->GetBinContent(i))/(inHist->Integral()) 
      			<< "    " << (inHist->GetBinError(i))/(inHist->Integral()) << "    " << inHist->GetBinWidth(i) << std::endl;
    
    file->close();
}

void Atlas_conf_2016_013::outputHistFileI(TH1I* inHist) {
    // Assemble absolute filename
    std::string filename = "/media/anke/Daten/Master/Programme/CM2/branches/new_features/CheckMATE_2.0.0Beta_More_HL/results/hist/"+std::string(inHist->GetTitle())+".dat";
    
    std::cout << "Writing file:" << std::endl;
    std::cout << filename << std::endl;
    // Open stream
    std::ofstream* file = new ofstream(filename.c_str());
    // Write standard information
    *file << "# " << inHist->GetTitle() << " \n";
    *file << "#  Bin    MC_Events    (Error)    Normalised_Events    (Error)    Histogram_Normalised_Events    (Error)    Bin_Width    \n";
    for ( int i=1; i<=inHist->GetNbinsX(); i++)
    	if (i < inHist->GetNbinsX())
      		*file << inHist->GetBinCenter(i) << "    " << inHist->GetBinContent(i) << "    " << inHist->GetBinError(i) << "    " 
      			<< normalize(inHist->GetBinContent(i)) << "    " << normalize(inHist->GetBinError(i)) << "    " 
      			<< (inHist->GetBinContent(i))/(inHist->Integral()) << "    " << (inHist->GetBinError(i))/(inHist->Integral()) << "    " 
      			<< inHist->GetBinWidth(i) << std::endl;
      	else if (i == inHist->GetNbinsX())
      		*file << inHist->GetBinCenter(i) << "    " << inHist->GetBinContent(i) + inHist->GetBinContent(i+1) << "    " 
      			<< inHist->GetBinError(i) << "    " << normalize(inHist->GetBinContent(i) + inHist->GetBinContent(i)) << "    " 
      			<< normalize(inHist->GetBinError(i)) << "    " << (inHist->GetBinContent(i) + inHist->GetBinContent(i))/(inHist->Integral()) 
      			<< "    " << (inHist->GetBinError(i))/(inHist->Integral()) << "    " << inHist->GetBinWidth(i) << std::endl;
    
    file->close();
}
*/
    //////////////////////////////////////////
    //     Overlap removal electron-jet     //
    //////////////////////////////////////////
    
    // In order to avoid double- counting of electrons as jets, the following overlap-removal procedure is used: if any of the non-
	// b-tagged jets lie within delta R = 0.2 of a candidate electron, the closest jet is discarded.  

// The sortByPT function in the AnalysisBase takes FinalStateObjects, so I would need to convert
// my Jets to those things, which is a huge mess.
static bool sortJetsByPT(Jet* a, Jet* b) {
    return a->PT > b->PT;
}

void Atlas_conf_2016_013::overlapRemoval_ej() {
    // Get all jets that have not been b-tagged
    std::vector<Jet*> non_btagged;
    for (std::vector<Jet*>::const_iterator it=jets.begin(); it != jets.end(); it++) {
        if (! checkBTag(*it)) {
            non_btagged.push_back(*it);
        }
    }
    // Remove overlap between non-tagged jets and electrons
    overlapRemoval(non_btagged, electronsMedium, 0.2);

    // Put b-jets and non-b-jets back together
    std::vector<Jet*>::iterator it = std::merge(non_btagged.begin(), non_btagged.end(), bjets.begin(), bjets.end(), jets.begin(), sortJetsByPT);
    jets.resize(it-jets.begin());

}
       


    //////////////////////////////////////
    //     Overlap removal jet-muon     //
    //////////////////////////////////////
    
    // If the angular distance delta R between a muon and a jet is smaller than min(0.4, 0.04 + 10 GeV/pT(mu) ) and the 
	// jet has at least three matching inner detector tracks, the muon is removed. This requirement reduces the background 
	// from muons from heavy-flavour decays inside jets.  On the other hand, if the jet has fewer than three matching tracks, the
	// jet is removed and the muon is kept [65]. This avoids any inefficiency for high-energy muons undergoing
	// significant energy loss in the calorimeter.  

void Atlas_conf_2016_013::overlapRemoval_jm() {
    // NOTE: Efficiency?
    std::vector<Muon*> surviving_muons;
    std::vector<bool> jet_overlaps(jets.size(), false);
    for (std::vector<Muon*>::const_iterator itm=muonsCombined.begin(); itm!=muonsCombined.end(); itm++) {
        bool muon_overlap = false;
        double minDR = std::min(0.4, 0.04+10/((*itm)->PT));
        for (int j=0; j<jets.size(); j++) {
            if (fabs((*itm)->P4().DeltaR(jets[j]->P4())) >= minDR) {
                continue;
            }
            if (countMatchingTracks(tracks, jets[j]) >= 3) {
                // remove muon
                muon_overlap = true;
            } else {
                // remove jet
                jet_overlaps[j] = true;
            }

        }
        if (!muon_overlap) {
            surviving_muons.push_back((*itm));
        }
    }
    std::vector<Jet*> surviving_jets;
    for (int i=0; i<jet_overlaps.size(); i++) {
        if (!(jet_overlaps[i])) {
            surviving_jets.push_back(jets[i]);
        }
    }
    muonsCombined=surviving_muons;
    jets=surviving_jets;
}

// The original jet finder algorithm used a DeltaR of 0.4, so do that again
static int countMatchingTracks(std::vector<Track*> tracks, Jet* jet) {
    int ntracks = 0;
    for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
        if (jet->P4().DeltaR((*it)->P4()) < 0.4) {
            ntracks++;
        }
    }
    return ntracks;
}

    ///////////////////////////////////////////
    //     Overlap removal electron-muon     //
    ///////////////////////////////////////////
    
    // Finally, electrons and muons that are within delta R = 0.01 of each other are both removed
	// from the event. The latter requirement ensures an unambiguous classification into single-electron and
	// single-muon events.

void Atlas_conf_2016_013::overlapRemoval_em() {
    // NOTE: The paper was a bit vague about which leptons this applies to,
    //       i.e. which combinations of e+m, e+e, m+m should be used


    // To remove _both_ particles, the original particles need to be saved, otherwise 
    // the other particles they overlap with cannot be removed
    // NOTE: This is not particularly efficient, but much simpler and less errorprone
    // than the alternative of doing the overlap removal myself
    // Also, the sizes of the vectors should be small, i.e. O(1)
    std::vector<Electron*> tmp_e = electronsMedium;
    std::vector<Muon*> tmp_m = muonsCombined;
    overlapRemoval(electronsMedium, tmp_m, 0.01);
    overlapRemoval(muonsCombined, tmp_e, 0.01);

    // Can't use `overlapRemoval(electronsLoose, 0.01)` because that only removes the electron with lower pT.
    // `overlapRemoval(electronsLoose, electronsLoose, 0.01) doesn't work because that would remove everything,
    // because each electron obviously overlaps with itself.
    overlapRemovalEqual(electronsMedium, tmp_e, 0.01);
    overlapRemovalEqual(muonsCombined, tmp_m, 0.01);
}

    //////////////////////////
    //     Reclustering     //
    //////////////////////////
    
    // The candidate small-R jets surviving the overlap procedure discussed above are used as inputs for further
	// jet re-clustering [66] using the anti-kt algorithm with a radius parameter R = 1.0. In order to suppress
	// contributions from pile-up and soft radiation, the re-clustered large-R jets are trimmed [67] by removing
	// all small-R (sub-)jets within a re-clustered jet that have pT below 5% of the pT of the re-clustered jet 
	// (i.e. f_cut = 0.05). Due to the pileup suppression and pT > 20 GeV requirements made on the small-R jets,
	// the average fraction of small-R jets removed by the trimming requirement is less than 1%. The resulting
	// large-R jets are used to identify high-pT hadronically-decaying top quark and Higgs boson candidates
	// by requiring them to have pT > 300 GeV, |eta| < 2.0 and mass above 100 GeV. In order to ensure that
	// the mass of the large-R jet originates from the pT and angular separation of the sub-jets, instead of from
	// the un-calibrated small-R jet mass, a requirement of >= 2 sub-jets is made.  In this way it is possible to
	// evaluate the uncertainty on the mass of the large-R jets coming from the calibration of its constituents
	// by varying the energy scale and resolution of small-R jets. The fraction of re-clustered jets rejected by
	// this requirement is about 1% in ttbar simulated events and up to about 10% for the highest-mass simulated
	// signals considered in this search. The resulting large-R jets will be referred to as “mass-tagged" jets.
	
static std::vector<fastjet::PseudoJet> convertJets(std::vector<Jet*> jets) {
    std::vector<fastjet::PseudoJet> converted;
    for (std::vector<Jet*>::iterator it=jets.begin(); it!=jets.end(); it++) {
        TLorentzVector p4 = (*it)->P4();
        converted.push_back(fastjet::PseudoJet(p4.Px(), p4.Py(), p4.Pz(), p4.E()));
    }
    return converted;
}

// Perform jet trimming as defined in
// D. Krohn, J. Thaler and L.-T. Wang, Jet Trimming, JHEP 02 (2010) 084,
// arXiv: 0912.1342 [hep-ph]
static fastjet::PseudoJet trim_jet(fastjet::PseudoJet jet, double fcut, double lambdaHard) {
    fastjet::PseudoJet trimmed(0,0,0,0);
    fastjet::PseudoJet zero(0,0,0,0);
    int n_const=0;
    std::vector<fastjet::PseudoJet> constituents = jet.constituents();
    for (std::vector<fastjet::PseudoJet>::iterator it=constituents.begin(); it!=constituents.end(); it++) {
        if (it->pt() < fcut*lambdaHard) {
            continue;
        }
        trimmed += (*it);
        n_const++;
    }
    if (n_const < 2)
    	return zero;
    return trimmed;
}

// Perform jet reclustering as defined in
// B. Nachman et al., Jets from Jets: Re-clustering as a tool for large radius jet reconstruction and
// grooming at the LHC, JHEP 02 (2015) 075, arXiv: 1407.2922 [hep-ph].
static std::vector<fastjet::PseudoJet> recluster_jets(std::vector<Jet*> jets, double R, double fcut) {
    std::vector<fastjet::PseudoJet> pjets = convertJets(jets);
    fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R);
    fastjet::ClusterSequence cs(pjets, jet_def);
    std::vector<fastjet::PseudoJet> largeJets = fastjet::sorted_by_pt(cs.inclusive_jets());

    std::vector<fastjet::PseudoJet> trimmedJets;
    for (std::vector<fastjet::PseudoJet>::iterator it = largeJets.begin(); it!=largeJets.end(); it++) {
        trimmedJets.push_back(trim_jet(*it, fcut, it->pt()));
    }
//    // ask for more than 1 subjet
//    std::vector<fastjet::PseudoJet> constit = trimmedJets.constituents();
//    if (constit.size() < 2)
//    	return nullptr;
    return trimmedJets;
}

void Atlas_conf_2016_013::recluster_top_jets() {
    std::vector<fastjet::PseudoJet> reclustered_jets = recluster_jets(jets, 1.0, 0.05);
    masstagged=0;
//    masslargeRjet=0;
    for (std::vector<fastjet::PseudoJet>::iterator it=reclustered_jets.begin(); it!=reclustered_jets.end(); it++) {
        // NOTE: It's not clear from the paper if there is supposed to be a dedicated top tagging algorithm before this,
        //       or if this is the top tagging
        if (it->pt() < 300) {
            continue;
        }
        if (fabs(it->eta()) >= 2.0) {
            continue;
        }
        masslargeRjet.push_back(it->m());
        if (it->m() <= 100) {
            continue;
        }       
        masstagged++;       
    }
}


    ///////////////////////
    //     Isolation     //
    ///////////////////////

template<class T>
static bool isolateLepton(T* lepton, std::vector<Track*> tracks, double maxPT, double deltaR) {
    double pTsum = 0;
    for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
        if (lepton->Particle == (*it)->Particle) {
            continue;
        }
        if ((*it)->P4().DeltaR(lepton->P4()) > deltaR) {
            continue;
        }
        pTsum += (*it)->PT;
    }
    return pTsum < maxPT;
}

template<class T>
std::vector<T*> Atlas_conf_2016_013::isolateLeptons(std::vector<T*> leptons, double deltaR) {
    std::vector<T*> isolated;
    for (typename std::vector<T*>::iterator it=leptons.begin(); it!=leptons.end(); it++) {
        double dR = std::min(deltaR, (double)(10/((*it)->PT)));
        if (isolateLepton(*it, tracks, 0.05*((*it)->PT), dR)) {
            isolated.push_back(*it);
        }
    }
    return isolated;
}

// filterBJets
std::vector<Jet*> Atlas_conf_2016_013::filterBJets(std::vector<Jet*> jets) {
    std::vector<Jet*> bJets;
    for (std::vector<Jet*>::iterator it=jets.begin(); it!=jets.end(); it++) {
        if (checkBTag(*it)) {
            bJets.push_back(*it);
        }
    }
    return bJets;
}

    /////////////////////////////////
    //     Kinematic variables     //
    /////////////////////////////////


// m_eff
 double Atlas_conf_2016_013::meff(
        std::vector<Jet*> jets,
        std::vector<Electron*> electrons,
        std::vector<Muon*> muons,
        ETMiss* missingET
        ) {
    double m_eff = 0;
    for (std::vector<Jet*>::const_iterator it = jets.begin(); it!=jets.end(); it++) {
        m_eff += (*it)->PT;
    }
    for (std::vector<Electron*>::const_iterator it = electrons.begin(); it!=electrons.end(); it++) {
        m_eff += (*it)->PT;
    }
    for (std::vector<Muon*>::const_iterator it = muons.begin(); it!=muons.end(); it++) {
        m_eff += (*it)->PT;
    }
    m_eff += missingET->P4().Et();
    return m_eff;
}

// invMass_minDeltaR(bjets)
double Atlas_conf_2016_013::invMass_minDeltaR(std::vector<Jet*> bjets) {
    double mbb_mindR = -1;
    double deltaRmin = 100;
    double tmp;
    if (bjets.size() == 2) {
    	mbb_mindR = (bjets[0]->P4(), bjets[1]->P4()).M();
    	}
    else if (bjets.size() > 2) {
    	for (std::vector<Jet*>::iterator it=bjets.begin(); it!=bjets.end(); it++) {
    		for (std::vector<Jet*>::iterator it2=bjets.begin()+1; it2!=bjets.end(); it2++) {
    			tmp = (*it)->P4().DeltaR( (*it2)->P4() );
    			if (tmp < deltaRmin && tmp != 0) {
    				deltaRmin = tmp;
    				mbb_mindR = ((*it)->P4() + (*it2)->P4()).M();
    			}
    		}
    	}
    }
    
    return mbb_mindR;
}



