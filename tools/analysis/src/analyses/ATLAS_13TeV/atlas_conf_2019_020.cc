#include "atlas_conf_2019_020.h"
// AUTHOR: Ipsita Saha
//  EMAIL: ipsita.saha@ipmu.jp
void Atlas_conf_2019_020::initialize() {
  setAnalysisName("atlas_conf_2019_020");          
  setInformation(""
    "# ATLAS\n"
    "# ATLAS-CONF-2019-020\n"
    "# 3 leps final state, chargino-neutralino production\n"
    "# sqrt(s) = 13 TeV\n"
    "# luminosity =139 fb^-1\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("SR-low;SR-ISR");
  bookCutflowRegions("00_all;01_3l_sfos;02_trigger;03_bveto;04_m3l>105_all;05_ptlep;06_msfos_75_105;07_01_jetsgt0;07_02_jetslt4;08_delphi;09_Rmetjet;10_ptjets;11_met;12_mtThird;13_pT_isr;14_ptlep_low;15_msfos_75_105_low;16_jetveto_low;17_hboost_low;18_ptsoft_ratio_low;19_meff_hbratio_low;20_mtthird_low");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_conf_2019_020::analyze() {
  // Your eventwise analysis code goes here
  // The following objects are always defined unless they are 'ignored' above. They form std::vector objects of the respective Delphes class type (except for Etmiss which is a single object)
  // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
  // Within a std::vector, all members are ordered with highest pt coming first.

  // electronsLoose, electronsMedium, electronsTight   are list of electrons that passed respective efficiency and reconstruction cuts
  // muonsCombinedPlus, muonsCombined                  as above for muons
  // photonsMedium                                     as above for photons
  // jets are all reconstructed jets                   as above for jets. Note that electrons are most certainly also reconstructed as a jet -> overlap removal do avoid double counting necessary!
  // tracks, towers                                    calorimeter and tracker information. Usually not needed.
  // missingET                                         rec missing ET EXCLUDING muons.

  
  // Here is a couple of useful functions and lines:  
  //------------Phase Space Cuts (defined for jets, electronsXYZ, muonsXYZ, photonsXYZ)
  // jets = filterPhaseSpace(jets, 20., -2.8, 2.8)  // The vector 'jets' only contains jets with pt >= 20 GeV and -2.8 < eta < 2.8. This function is applicable to other particles too (electronsMedium, ... ).
  // jets = overlapRemoval(jets, electronsLoose, 0.2) Removes all jets for which there exists any electron in 'electronsLoose' with deltaR < 0.2.
  // jets = overlapRemovel(jets, 0.2) If two jets overlap within deltaR < 0.2, only the harder jet is stored.
  
  //------------Isolation Checks (defined for electronsXYZ, muonsXYZ, photonsXYZ
  //------------        For each object, if the user entered N isolation conditions, they can be
  //------------        checked individually be the second argument (running from 0 to N-1).
  // electronsMedium = filterIsolation(electronsMedium, 0)            Removes electrons that do not pass the first isolation condition entered into the AnalysisManager by the user
  // std::vector<int> flags; flags.push_back(0); flags.push_back(2);
  // electronsMedium = filterIsolation(electronsMedium, flags)        Same as above, but both the first and the third condition have to be fulfilled
  // electronsMedium = filterIsolation(electronsMedium)               Same as above, but all conditions have to be fulfilled.
  
  //-----------Flavour Tag Checks (defined for jets only)
  //----------          Tau tags "loose", "medium" or "tight" can be individually checked if the user enabled tau tagging in the AM.
  //----------          For b-tags, if N working points have been defined, the ith condition can be tested by putting i-1 into the second argument (if there is only one, the argument can be omitted)
  // if checkTauTag(jets[0], "tight") leadingJetIsTagged = True;
  // if checkBTag(jets[0], 0) leadingJetIsBTagged = True;


  //-----------Auxiliary Information
  // - Always ensure that you don't access vectors out of bounds. E.g. 'if(jets[1]->PT > 150)' should rather be if (jets.size() > 1 && jets[1]->PT > 150). 
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in CheckMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  

  electronsLoose = filterPhaseSpace(electronsLoose, 10.0, -2.47, 2.47);
  electronsMedium = filterPhaseSpace(electronsMedium, 10.0, -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 10.0, -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.


  jets = overlapRemoval(jets, electronsLoose, 0.2);
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  jets = overlapRemoval(jets, electronsTight, 0.2);
  jets = overlapRemoval_muon_jet_tracks(jets, muonsCombined, 0.2, 2);

  
  electronsLoose = overlapRemoval_inverse_cone(electronsLoose, jets, 0.4, 50.0, 10.0, "y");
  electronsMedium = overlapRemoval_inverse_cone(electronsMedium, jets, 0.4, 50.0, 10.0, "y");
  electronsTight = overlapRemoval_inverse_cone(electronsTight, jets, 0.4, 50.0, 10.0, "y");

  muonsCombined = overlapRemoval_inverse_cone(muonsCombined, jets, 0.4, 50.0, 10.0, "y");

  std::vector<Electron*> electrons_signal = Isolate_leptons_with_inverse_track_isolation_cone(electronsTight, tracks, towers, 0.2, 10., 0.2, 0.06, 0.06, true);
  std::vector<Muon*> muons_signal = Special_Isolate_muons(muonsCombined, tracks, towers, 0.3, 10., 0.2, 0.04, 0.15, true);

  jets = filterPhaseSpace(jets, 20., -2.4, 2.4);
  std::vector<Jet*> bjets;
  std::vector<Jet*> lightjets;

  for (int i = 0; i < jets.size(); i++)
    if (checkBTag(jets[i]) ) bjets.push_back(jets[i]);
    else lightjets.push_back(jets[i]);
 

//  electrons_signal = filterIsolation(electrons_signal);
//  muons_signal = filterIsolation(muons_signal);

  // fill muons and electrons into lepton vector and sort by pT                                                                                              
    std::vector<FinalStateObject*> leps;
    FinalStateObject* lep;
    for( int i=0; i < electrons_signal.size(); i++) {
        lep = new FinalStateObject( electrons_signal[i] );
        leps.push_back(lep);
    }
    for( int i=0; i < muons_signal.size(); i++) {
        lep = new FinalStateObject( muons_signal[i] );
        leps.push_back(lep);
    }
    std::sort( leps.begin(), leps.end(), sortByPT );

    double met = missingET->P4().Et();


     countCutflowEvent("00_all");

//    if (muons_signal.size() + electrons_signal.size()  !=3 ) return;
    if (leps.size() !=3 ) return;

    bool sfos = false;
    if (leps[0]->Charge * leps[1]->Charge < 0 && leps[0]->Type == leps[1]->Type)
        sfos = true;
    else if (leps[0]->Charge * leps[2]->Charge < 0 && leps[0]->Type == leps[2]->Type)
        sfos = true;
    else if (leps[2]->Charge * leps[1]->Charge < 0 && leps[2]->Type == leps[1]->Type)
        sfos = true;

    if(!sfos) return;
      
      countCutflowEvent("01_3l_sfos");

    //Trigger//                                                                                                                                                                                                                                                         
    bool trigger = false;
       if(leps.size() > 1 && leps[0]->PT >= 22. && leps[1]->PT >= 8.) trigger =true;

    if (!trigger) return;
     countCutflowEvent("02_trigger");
  //=======//

        if (bjets.size() != 0 ) return;
        countCutflowEvent("03_bveto");

 
          // Find the sfos pair with inv mass closest to the z-boson                                                                                                                                                                                                    
        double msfos = -1E10;
        double mtThird = 0;
        if (leps[0]->Charge * leps[1]->Charge < 0 && leps[0]->Type == leps[1]->Type) {
        double minv = (leps[0]->P4() + leps[1]->P4()).M();
        if (fabs(minv - 91.) < fabs(msfos - 91.)) {
            msfos = minv;
            mtThird = mT(leps[2]->P4(), missingET->P4());
        }
        }
        if (leps[0]->Charge * leps[2]->Charge < 0 && leps[0]->Type == leps[2]->Type) {
        double minv = (leps[0]->P4() + leps[2]->P4()).M();
        if (fabs(minv - 91.) < fabs(msfos - 91.)) {
            msfos = minv;
            mtThird = mT(leps[1]->P4(), missingET->P4());
        }
        }
        if (leps[2]->Charge * leps[1]->Charge < 0 && leps[2]->Type == leps[1]->Type){
        double minv = (leps[2]->P4() + leps[1]->P4()).M();
        if (fabs(minv - 91.) < fabs(msfos - 91.)) {
            msfos = minv;
            mtThird = mT(leps[0]->P4(), missingET->P4());
        }
        }


        // inv mass of the three lepton system                                                                                                                                                                                 
        double m3l = (leps[0]->P4() + leps[1]->P4() + leps[2]->P4()).M();
        bool msfos75_105 = ( msfos >= 75.  && msfos <= 105.);
        bool m3l105 =  (m3l > 105.);
        if(!m3l105) return;
        countCutflowEvent("04_m3l>105_all");

        //Variables for SR-low

	//Hboost, meff

         TLorentzVector P_siglep;
         double Pz_siglep = 0.;
         double PT_siglep = 0.;	 
         double Hboost=0.;
	 double meff=0.;
         double PT_soft=0.;
         double var1 =0.;
         double var2 =0.;
	 double varx = 0.;
	 double vary=0.;
	 double Pz_miss =0.; 

        for(int i=0; i < leps.size() ; i++) {
          P_siglep += leps[i]->P4();
          meff += leps[i]->PT;
          }
          meff += met; //missingET->P4().Et();

	Pz_siglep = P_siglep.Pz();
        PT_siglep = P_siglep.Pt();	
	
	varx = sqrt(pow(PT_siglep,2) + pow(P_siglep.M(),2)); // denominator sqrt(p_{V,T}^2 +m_V^2)
	vary = Pz_siglep * fabs(missingET->P4().Pt());    // numerator p_{V,||} * |p_Tmiss|
	Pz_miss = vary/varx;

	TLorentzVector pmiss(missingET->P4().Px(),missingET->P4().Py(),Pz_miss,missingET->P4().E());	

	TLorentzVector b1 = P_siglep + pmiss;
        TVector3 b2 = b1.BoostVector();
	
        P_siglep.Boost(b2);
	pmiss.Boost(b2);
	 
	Hboost = P_siglep.M() + pmiss.M();
	//PT_soft
	
     TLorentzVector sum_ptsoft(0.,0.,0.,0.);
     for(int i=0;i<leps.size();i++){
	sum_ptsoft += leps[i]->P4();
	 }
     sum_ptsoft += missingET->P4();
     PT_soft = sum_ptsoft.Pt();
     
     var1 = meff/Hboost;
     var2 = PT_soft/(PT_soft + meff);


        //Variables for SR-ISR                                                                                                                                                                                                                                        
  	double PT_jets = 0.;
    	double R_met_jet = 0.;
     	double PT_soft_ISR = 0.;
 	double deltaphi1 = 0.0;
     TLorentzVector sum_jet(0.,0.,0.,0.);
     TLorentzVector uni(0.,0.,0.,0.);

	if(lightjets.size() > 0) {
	     for(int i=0;i<lightjets.size();i++){
	  	sum_jet += lightjets[i]->P4();
		}
       PT_jets = sum_jet.Pt(); 
       
       deltaphi1 = fabs(missingET->P4().DeltaPhi(sum_jet));

       uni.SetPxPyPzE(sum_jet.Px()/PT_jets,sum_jet.Py()/PT_jets,0.0,0.0);
       R_met_jet=fabs(missingET->P4().Dot(uni))/PT_jets;
	}


     TLorentzVector sum_ptsoft_ISR;
     for(int i=0; i<leps.size(); i++){                                                                                                                                                                             
          sum_ptsoft_ISR += leps[i]->P4();
     }
	if(lightjets.size() > 0){
	     for(int i=0; i<lightjets.size(); i++){                                                                                                                                                                             
      		 sum_ptsoft_ISR += lightjets[i]->P4();
    		}
	}
     sum_ptsoft_ISR += missingET->P4();
     PT_soft_ISR = sum_ptsoft_ISR.Pt();

        //SR-ISR cuts                                                                                                                                                                                                                                        

        if(leps[0]->PT > 25. && leps[1]->PT > 25. && leps[2]->PT > 20.){ countCutflowEvent("05_ptlep");
  	 	     if( msfos >= 75. && msfos <= 105.){ countCutflowEvent("06_msfos_75_105");
				if(lightjets.size() > 0){ countCutflowEvent("07_01_jetsgt0");
				if(lightjets.size() < 4){ countCutflowEvent("07_02_jetslt4");
  		      			if(deltaphi1 > 2.){ countCutflowEvent("08_delphi");			     
					        if(R_met_jet >= 0.55 && R_met_jet <= 1.0){ countCutflowEvent("09_Rmetjet");
							if(PT_jets > 100.0){ countCutflowEvent("10_ptjets");			     
								if(met > 80.){ countCutflowEvent("11_met");			     
  								      if(mtThird > 100.){countCutflowEvent("12_mtThird");			     
  									      if(PT_soft_ISR < 25.){ countCutflowEvent("13_pT_isr");			     
										  countSignalEvent("SR-ISR");
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

	//Final cutflow  for SR_Low
        if(leps[0]->PT > 60. && leps[1]->PT > 40. && leps[2]->PT > 30.){
		countCutflowEvent("14_ptlep_low");
		if(msfos75_105){
			countCutflowEvent("15_msfos_75_105_low");
			if(lightjets.size() == 0){
				countCutflowEvent("16_jetveto_low");
				if(Hboost > 250.){
					countCutflowEvent("17_hboost_low");
					if(var2 < 0.05){
						countCutflowEvent("18_ptsoft_ratio_low");
						if(var1 > 0.9){
							countCutflowEvent("19_meff_hbratio_low");
							if(mtThird > 100.){
								countCutflowEvent("20_mtthird_low");
	  							countSignalEvent("SR-low");
    							}
						}
					}
				}
			}
		}
	}

}

void Atlas_conf_2019_020::finalize() {
  // Whatever should be done after the run goes here
}       

bool Atlas_conf_2019_020::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {

  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++)
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

std::vector<Jet*> Atlas_conf_2019_020::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){

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





