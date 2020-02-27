#include "atlas_1507_05493.h"
// AUTHOR: J.S. Kim
//  EMAIL: jsk@th.physik.uni-bonn.de
void Atlas_1507_05493::initialize() {
  setAnalysisName("atlas_1507_05493");          
  setInformation(""
    "# Search for photonic signatures of gauge-mediated supersymmetry in 8 TeV pp collisions with the ATLAS detector\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  bookSignalRegions("gaga_SL;gaga_SH;gaga_WL;gaga_WH;gab_L;gab_H;gaj_L;gaj_H;gal_e;gal_mu;");
  bookCutflowRegions("gaga_SL_1_photon_selec;gaga_SL_2_Deltaphi(j,etmiss);gaga_SL_3_meff;gaga_SL_4_etmiss;");
  bookCutflowRegions("gaga_SH_1_photon_selec;gaga_SH_2_Deltaphi(jet,etmiss);gaga_SH_3_Deltaphi(ga,etmiss);gaga_SH_4_meff;gaga_SH_5_etmiss;");
  bookCutflowRegions("gaga_WL_1_photon_selec;gaga_WL_2_Deltaphi(jet,etmiss);gaga_WL_3_HT;gaga_WL_etmiss;");
  bookCutflowRegions("gaga_WH_1_photon_selec;gaga_WH_2_Deltaphi(jet,etmiss);gaga_WH_3_Deltaphi(ga,etmiss);gaga_WH_4_HT;gaga_WH_5_etmiss;");
  bookCutflowRegions("gal_e_1_photon_selec;gal_e_2_leptons;gal_e_3_mTel;gal_e_4_HT;gal_e_5_etmiss;");
  bookCutflowRegions("gal_mu_1_photon_selec;gal_mu_2_leptons;gal_mu_3_mTmu;gal_mu_4_HT;gal_mu_5_etmiss;");
  bookCutflowRegions("gaj_L_1_photon_selec;gaj_L_2_lepton_jet;gaj_L_3_Deltaphi(jet,etmiss);gaj_L_4_R4T;gaj_L_5_etmiss;");
  bookCutflowRegions("gaj_H_1_photon_selec;gaj_H_2_lepton_jet;gaj_H_3_Deltaphi(jet,etmiss);gaj_H_4_Deltaphi(jet,ga);gaj_H_5_HT;gaj_H_6_etmiss;");
  bookCutflowRegions("gab_L_1_cleaning;gab_L_2_photon_pt;gab_L_3_njets;gab_L_4_btag1;gab_L_5_btag2;gab_L_6_leptonveto;gab_L_7_etmiss;gab_L_8_mTga;gab_L_9_Deltaphi(jet,etmiss);gab_L_10_mbb;");
  bookCutflowRegions("gab_H_1_cleaning;gab_H_2_photon_pt;gab_H_3_njets;gab_H_4_btag;gab_H_5_etmiss;gab_H_6_mTga;gab_H_7_Deltaphi(jet,etmiss);gab_H_8_HT;");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1507_05493::analyze() {
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);  
  electronsMedium = filterPhaseSpace(electronsMedium, 20., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 20., -2.47, 2.47);

  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 20., -2.4, 2.4);

  photons = filterPhaseSpace(photons, 20., -2.37, 2.37, true);
  
  jets = overlapRemoval(jets, electronsTight, 0.2); 

  jets = overlapRemoval(jets, photons, 0.2);
  electronsTight = overlapRemoval(electronsTight,photons, 0.01);
  electronsTight = overlapRemoval(electronsTight,jets, 0.4);

  photons = overlapRemoval(photons,jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined,jets, 0.4);
  
  electronsTight = filterIsolation(electronsTight);
  muonsCombined = filterIsolation(muonsCombined);
  
  std::vector<Photon*> photonstight = filterIsolation(photons,0);
  std::vector<Photon*> photonsloose = filterIsolation(photons,1);
  

  missingET->addMuons(muonsCombined);
  
  //Photontrigger
  
  bool singlephotontrigger = false;
  bool doublephotontrigger = false;
    
  if( photonsloose.size()>=1 && photonsloose[0]->PT > 120. )
    singlephotontrigger = true;
  if ( photonsloose.size()>=2 && photonsloose[1]->PT > 40. )
    doublephotontrigger = true;

  std::vector<Jet*> bjets;
  for( int i=0; i<jets.size(); i++ ){
    if( checkBTag(jets[i]) )  bjets.push_back(jets[i]);
  }

  
  if( photonsloose.size() == 0 || !( singlephotontrigger || doublephotontrigger )  )
    return;


  //definitions of kinematical variables. Since Diphoton and single photon signal regions have different requirements on photon isolation, I define several kinematic observables for loose and tight photons

  //definition of effective mass (meff) and scalar mass (HT). Effective mass is equal to scalar mass + missing ET

  //definition of scalar mass: scalar sum of transverse momenta of the selected photons and any leptons and jets in event
  
  double meffloose=0.;
  double mefftight=0.;
  double HTloose=0.;
  double HTtight=0;


  //calculation of scalar sum
  for(int i=0; i<jets.size(); i++) {
    HTloose += jets[i]->PT;
  }
  
  for(int i=0; i<electronsTight.size(); i++) {
    HTloose += electronsTight[i]->PT;
  }

  for(int i=0; i<muonsCombined.size(); i++) {
   HTloose += muonsCombined[i]->PT;
  }

  HTtight=HTloose;
  
  for(int i=0; i<photonsloose.size(); i++) {
    HTloose += photonsloose[i]->PT;
  }

  meffloose = HTloose;
  meffloose += missingET->P4().Et();
  
  for(int i=0; i<photonstight.size(); i++) {
    HTtight += photonstight[i]->PT;
  }

  mefftight = HTtight;
  mefftight += missingET->P4().Et();

  // defintion of following angular kinematic variables wil be done in the respective signal regions

  //deltaphi=deltaPhi(gamma,MET)
  //deltaphi_j=deltaPhi(jet,MET)
  //deltaphi_ga_j=deltaPhi(gamma,jet)
    


  //definition of the transverse masses for gamma, electron and muon with missing transverse momentum. respectively
  
  double mTga,mTel,mTmu;

  
  //diphoton signal regions (table 2 in arxiv:1507.05493)
  //four signal regions

  
  if( photonstight.size() > 1 ){

    //calculation of delta(gamma,MET), this kinematic variable only be used in diphoton signal regions

    double deltaphi;
    double tempphi;
    tempphi=1000.;
    
    for ( int i=0; i<photonstight.size(); i++ ){
      deltaphi = fabs(photonstight[i]->P4().DeltaPhi(missingET->P4()));
      if( deltaphi < tempphi && i < 2 ) tempphi=deltaphi;
    }
    deltaphi=tempphi;

    
    
    //construction of delta phi(jet,MET)

    double deltaphi_j;
    
    tempphi=100.;
    
    for ( int i=0; i<jets.size(); i++ ){
      if( jets[i]->PT > 75. && i < 3 ) {
	deltaphi_j = fabs(jets[i]->P4().DeltaPhi(missingET->P4()));
	if( deltaphi_j < tempphi )
	  tempphi=deltaphi_j;
      }
    }
    
    //for leading jets in diphoton signal region apply deltaphi cut
   deltaphi_j=tempphi;

    
    if ( photonstight[0]->PT > 75. ){
      countCutflowEvent("gaga_SL_1_photon_selec");
      if( jets.size() > 1 && deltaphi_j > 0.5 ){
        countCutflowEvent("gaga_SL_2_Deltaphi(j,etmiss)");
        if( mefftight > 1800. ){
          countCutflowEvent("gaga_SL_3_meff");
          if( missingET->P4().Et() > 150. ){
            countCutflowEvent("gaga_SL_4_etmiss");
            countSignalEvent("gaga_SL");
          }
        }
      }  //gaga_SL_1_photon_selec;gaga_SL_2_Deltaphi(j,etmiss);gaga_SL_3_meff;gaga_SL_4_etmiss

      countCutflowEvent("gaga_SH_1_photon_selec");
      if ( jets.size() > 1 && deltaphi_j > 0.5 ){
        countCutflowEvent("gaga_SH_2_Deltaphi(jet,etmiss)");
        if(deltaphi >0.5){
          countCutflowEvent("gaga_SH_3_Deltaphi(ga,etmiss)");
          if( mefftight > 1500. ){
            countCutflowEvent("gaga_SH_4_meff");
            if( missingET->P4().Et() > 250. ){
              countCutflowEvent("gaga_SH_5_etmiss");
              countSignalEvent("gaga_SH");
            }
          }
        }  
      }//gaga_SH_1_photon_selec;gaga_SH_2_Deltaphi(jet,etmiss);gaga_SH_3_Deltaphi(ga,etmiss);gaga_SH_4_meff;gaga_SH_5_etmiss

      countCutflowEvent("gaga_WL_1_photon_selec");
      if( jets.size() > 1 && deltaphi_j > 0.5 ){
        countCutflowEvent("gaga_WL_2_Deltaphi(jet,etmiss)");
	if( HTtight > 600. ){
          countCutflowEvent("gaga_WL_3_HT");
	  if ( missingET->P4().Et() > 150. ){
            countCutflowEvent("gaga_WL_etmiss");
	    countSignalEvent("gaga_WL");
          }
        }
      }//gaga_WL_1_photon_selec;gaga_WL_2_Deltaphi(jet,etmiss);gaga_WL_3_HT;gaga_WL_etmiss

      countCutflowEvent("gaga_WH_1_photon_selec");
      if( jets.size() > 1 && deltaphi_j > 0.5 ){
        countCutflowEvent("gaga_WH_2_Deltaphi(jet,etmiss)");
	if( deltaphi > 0.5 ){
          countCutflowEvent("gaga_WH_3_Deltaphi(ga,etmiss)");
	  if ( HTtight > 400. ){
            countCutflowEvent("gaga_WH_4_HT");
	    if( missingET->P4().Et() > 200.){
              countCutflowEvent("gaga_WH_5_etmiss");
	      countSignalEvent("gaga_WH");
            }
          }
        }
      }
    }//gaga_WH_1_photon_selec;gaga_WH_2_Deltaphi(jet,etmiss);gaga_WH_3_Deltaphi(ga,etmiss);gaga_WH_4_HT;gaga_WH_5_etmiss

  }


  //single photon and one lepton signal regions  (table 4 in arxiv:1507.05493)

  
  if( photonsloose.size() > 0 ){

    double HTjets=0.;
    for(int i=0;i<jets.size();i++){
      HTjets+=jets[i]->PT;
    }
    //countCutflowEvent("gal_e_photonlep");

    if( photonsloose[0]->PT > 125. ){
      countCutflowEvent("gal_e_1_photon_selec");
      if(electronsTight.size() > 0 ){
        if( fabs( (photonsloose[0]->P4() + electronsTight[0]->P4() ).M() - 91. ) > 15. ){
          if( photonsloose[0]->P4().DeltaR(electronsTight[0]->P4()) > 0.7 ){
            countCutflowEvent("gal_e_2_leptons");
            mTel=mT(electronsTight[0]->P4(),missingET->P4(), 0.);
            if( mTel > 120. ){
              countCutflowEvent("gal_e_3_mTel");
              if( HTjets < 100. ){
                countCutflowEvent("gal_e_4_HT");
                if( missingET->P4().Et() > 120. ){
                  countCutflowEvent("gal_e_5_etmiss");
                  countSignalEvent("gal_e");
                }
              }
            }
          }
        }
      }//gal_e_1_photon_selec;gal_e_2_leptons;gal_e_3_mTel;gal_e_4_HT;gal_e_5_etmiss
    }
  }

  if( photonsloose.size() > 0 ){
    
    double HTjets=0.;
    for(int i=0;i<jets.size();i++){
      HTjets+=jets[i]->PT;
    }
    if ( photonsloose[0]->PT > 125. ){
      countCutflowEvent("gal_mu_1_photon_selec");
      if(muonsCombined.size() > 0){
        if( photons[0]->P4().DeltaR(muonsCombined[0]->P4()) > 0.7 ){
          countCutflowEvent("gal_mu_2_leptons");
          mTmu=mT(muonsCombined[0]->P4(),missingET->P4(), 0.);
	  if( mTmu > 120. ){
            countCutflowEvent("gal_mu_3_mTmu");
            if ( HTjets < 100. ){
              countCutflowEvent("gal_mu_4_HT");
              if ( missingET->P4().Et() > 120. ){
                countCutflowEvent("gal_mu_5_etmiss");
	        countSignalEvent("gal_mu");
              }
            }
          }
        }
      }
    }//gal_mu_1_photon_selec;gal_mu_2_leptons;gal_mu_3_mTmu;gal_mu_4_HT;gal_mu_5_etmiss
  }
  
  //gamma jet signal regions with lepton veto   (table 3 in arxiv:1507.05493)
  
  if( photonsloose.size() > 0 && photonsloose[0]->PT > 25. ){
    countCutflowEvent("gab_L_1_cleaning");
    if( photonsloose[0]->PT > 125. ){
      countCutflowEvent("gab_L_2_photon_pt");
      if ( jets.size() > 1 && jets.size() < 5){
        countCutflowEvent("gab_L_3_njets");
        if (bjets.size() > 0 ){
          countCutflowEvent("gab_L_4_btag1");
          if (bjets.size() > 1 ){
            countCutflowEvent("gab_L_5_btag2");
            if ( electronsTight.size() + muonsCombined.size() == 0 ){
              countCutflowEvent("gab_L_6_leptonveto");
              if ( missingET->P4().Et() > 100. ){
                countCutflowEvent("gab_L_7_etmiss");
                mTga = mT(photons[0]->P4(),missingET->P4(), 0.);
	        if( mTga > 90. ){
                  countCutflowEvent("gab_L_8_mTga");
                  //construction of delta phi(jet,MET)
	          double deltaphi_j;
	      
	          double tempphi;
	      
	          tempphi=100.;
	      
	          for ( int i=0; i<jets.size(); i++ ){
		    if( i<=1 ) {	  
		    deltaphi_j = fabs(jets[i]->P4().DeltaPhi(missingET->P4()));
		    if( deltaphi_j < tempphi ) tempphi=deltaphi_j;
		    }
	          }
	          //demand leading jet in order to apply deltaphi cut
	          deltaphi_j=tempphi;
                  if(deltaphi_j > 0.3 ){
                    countCutflowEvent("gab_L_9_Deltaphi(jet,etmiss)");
                    double mbb;
	            mbb=( bjets[0]->P4() + bjets[1]->P4() ).M();
                    if( mbb > 75. && mbb < 150. ){
                    countCutflowEvent("gab_L_10_mbb");
		    countSignalEvent("gab_L");
                    }//gab_L_1_cleaning;gab_L_2_photon_pt;gab_L_3_njets;gab_L_4_btag1;gab_L_5_btag2;gab_L_6_leptonveto;gab_L_7_etmiss;gab_L_8_mTga;gab_L_9_Deltaphi(jet,etmiss);gab_L_10_mbb
                  }
                }
              } 
            }
          }
        }
      }
    }
  }  

  if( photonsloose.size() > 0){
    if( photonsloose[0]->PT > 125. ){
      countCutflowEvent("gaj_L_1_photon_selec");
      if( electronsTight.size() + muonsCombined.size() == 0 ){
        if ( jets.size() > 3  && jets[0]->PT > 100. && jets[1]->PT > 40. ){
          countCutflowEvent("gaj_L_2_lepton_jet");

	  //construction of delta phi(jet,MET)
	  double deltaphi_j;
	  
	  double tempphi;
	  
	  tempphi=100.;
	  
	  for ( int i=0; i<jets.size(); i++ ){
	    if( i<=1 ) {	  
	      deltaphi_j = fabs(jets[i]->P4().DeltaPhi(missingET->P4()));
	      if( deltaphi_j < tempphi ) tempphi=deltaphi_j;
	    }
	  }
	  
	  //demand leading jet in order to apply deltaphi cut
	  deltaphi_j=tempphi;


	  
          if(deltaphi_j > 0.4){
	    countCutflowEvent("gaj_L_3_Deltaphi(jet,etmiss)");

	    //in the following, I want to calculate the quantity R4T defined as scalar sum of transverse momentum of four highest pT jets divided by the transverse momenta of all jets
	    
	    double R4T=0.;

	    double fourjets=0.;
	    double alljets=0.;
  
	    //calculation of scalar sum of pT of the first 4 jets and of all jets
	    
	    for(int i=0;i<jets.size();i++){
	      alljets+=jets[i]->PT;
	      if(i<5) fourjets +=jets[i]->PT;
	    }
	    
	    //definition of the R4T variable
	    
	    if(jets.size()>3)
	      R4T=fourjets/alljets;
	    else
	      R4T=0.;
	    
	    
            if( R4T < 0.85 ){
	      
	      //	      std::cout << "R4T: " << R4T << std::endl;
              countCutflowEvent("gaj_L_4_R4T");
              if( missingET->P4().Et() > 200. ){
                countCutflowEvent("gaj_L_5_etmiss");
                countSignalEvent("gaj_L");
              }
            }
          }
        }
      }
    }
    if( photonsloose[0]->PT > 300. ){
      countCutflowEvent("gaj_H_1_photon_selec");
      if( electronsTight.size() + muonsCombined.size() == 0 ){
        if ( jets.size() > 1 && jets[0]->PT > 100. && jets[1]->PT > 40. ){
          countCutflowEvent("gaj_H_2_lepton_jet");

	  //construction of delta phi(jet,MET)
	  double deltaphi_j;

	  double tempphi;
	  
	  tempphi=100.;
	  
	  for ( int i=0; i<jets.size(); i++ ){
	    if( i<=1 ) {	  
	      deltaphi_j = fabs(jets[i]->P4().DeltaPhi(missingET->P4()));
	      if( deltaphi_j < tempphi ) tempphi=deltaphi_j;
	    }
	  }
	  
	  //demand leading jet in order to apply deltaphi cut
	  deltaphi_j=tempphi;
	  
	  
          if(deltaphi_j > 0.4){
            countCutflowEvent("gaj_H_3_Deltaphi(jet,etmiss)");

	    //delta(gamma,jets) only the first two leading jets are taken into account
	    
	    double deltaphi_ga_j;
	    
	    double tempphi;
	    
	    tempphi=1000.;
	    
	    for ( int i=0; i<jets.size(); i++ ){
	      for ( int j=0; j<photonsloose.size(); j++ ){
		deltaphi_ga_j = fabs(jets[i]->P4().DeltaPhi(photonsloose[j]->P4()));
		if( deltaphi_ga_j < tempphi && i<2 && j<1 ) tempphi=deltaphi_ga_j;
		deltaphi_ga_j=tempphi;
	      }
	      
	    }

	    
            if( deltaphi_ga_j < 2.0 ){
              countCutflowEvent("gaj_H_4_Deltaphi(jet,ga)");
              if ( HTloose > 800. ){
                countCutflowEvent("gaj_H_5_HT");
                if ( missingET->P4().Et() > 300. ){
                  countCutflowEvent("gaj_H_6_etmiss");
                  countSignalEvent("gaj_H");
                }
              }
            }
          }
        }
      }
    }    
  }


  //gamma jet with no lepton veto signal region  (table 3 in arxiv:1507.05493)
  
  if( photonsloose.size() > 0 ){
    
    //construction of delta phi(jet,MET)

    double deltaphi_j;
    
    double tempphi;
    
    tempphi=100.;

    for ( int i=0; i<jets.size(); i++ ){
      if( jets[i]->PT > 75. && i<=1 ) {
	deltaphi_j = fabs(jets[i]->P4().DeltaPhi(missingET->P4()));
	if( deltaphi_j < tempphi ) tempphi=deltaphi_j;
      }
    }
    
    deltaphi_j=tempphi;
     
    if ( photonsloose[0]->PT > 25. ){
      countCutflowEvent("gab_H_1_cleaning");
      if ( photonsloose[0]->PT > 150. ){
        countCutflowEvent("gab_H_2_photon_pt");
        if( jets.size() > 3){
          countCutflowEvent("gab_H_3_njets");
          if(bjets.size() > 0){
            countCutflowEvent("gab_H_4_btag");
            if ( missingET->P4().Et() > 200. ){
              countCutflowEvent("gab_H_5_etmiss");
              mTga=mT(photons[0]->P4(),missingET->P4(), 0.);
	      if( mTga > 90. ){
                countCutflowEvent("gab_H_6_mTga");
                if( deltaphi_j > 0.3){
                  countCutflowEvent("gab_H_7_Deltaphi(jet,etmiss)");
                  if ( HTloose > 1000. ){
                    countCutflowEvent("gab_H_8_HT");
                  }//gab_H_1_cleaning;gab_H_2_photon_pt;gab_H_3_njets;gab_H_4_btag;gab_H_5_etmiss;gab_H_6_mTga;gab_H_7_Deltaphi(jet,etmiss);gab_H_8_HT
                }
              }
            }
          }
        }
      }
    }
  }


  
}

  
  

void Atlas_1507_05493::finalize() {
  // Whatever should be done after the run goes here
}       
