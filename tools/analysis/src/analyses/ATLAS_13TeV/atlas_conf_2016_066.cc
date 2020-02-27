#include "atlas_conf_2016_066.h"
// AUTHOR: J.S.K.
//  EMAIL: jsk@th.physik.uni-bonn.de
void Atlas_conf_2016_066::initialize() {
  setAnalysisName("atlas_conf_2016_066");          
  setInformation(""
    "# Search for Supersymmetry in events with photons, jets and missing transverse energy with the ATLAS detector in 13 TeV pp collisions\n"
  "");
  setLuminosity(13.3*units::INVFB);      
  bookSignalRegions("SRL;SRH");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_conf_2016_066::analyze() {
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

  //  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  jets = filterPhaseSpace(jets, 30., -2.8, 2.8);  
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47);

  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 10., -2.4, 2.4);

  photons = filterPhaseSpace(photons, 25., -2.37, 2.37, true);
  
  jets = overlapRemoval(jets, electronsTight, 0.2); 

  jets = overlapRemoval(jets, photons, 0.2);
  electronsTight = overlapRemoval(electronsTight,photons, 0.01);
  electronsTight = overlapRemoval(electronsTight,jets, 0.4);

  photons = overlapRemoval(photons,jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined,jets, 0.4);
  
  electronsTight = filterIsolation(electronsTight);
  muonsCombined = filterIsolation(muonsCombined);
  

  missingET->addMuons(muonsCombined);

  double HT=0.;
  double meff=0.;

  //calculation of scalar sum
  for(int i=0; i<jets.size(); i++) {
    HT += jets[i]->PT;
  }
  
  for(int i=0; i<photons.size(); i++) {
    HT += photons[i]->PT;
  }

  meff = HT;
  meff += missingET->P4().Et();
  

  

  if( photons.size() > 0){
    if( photons[0]->PT > 145. ){
      if( electronsTight.size() + muonsCombined.size() == 0 ){
        if ( jets.size() > 3 ){

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
	    
	    
            if( R4T < 0.9 ){
	      
	      //	      std::cout << "R4T: " << R4T << std::endl;
              if( missingET->P4().Et() > 200. ){



		double deltaphi;
		double tempphi;
		tempphi=1000.;
		
		for ( int i=0; i<photons.size(); i++ ){
		  deltaphi = fabs(photons[i]->P4().DeltaPhi(missingET->P4()));
		  if( deltaphi < tempphi && i < 2 ) tempphi=deltaphi;
		}
		deltaphi=tempphi;



	    
		if( deltaphi > 0.4 ){
		  if ( meff > 2000. ){
		      countSignalEvent("SRL");
		  }
		}
	      }
	    }
	  }
	}
      }
      
    }

    
    if( photons[0]->PT > 400. ){
      if( electronsTight.size() + muonsCombined.size() == 0 ){
        if ( jets.size() > 1 ){

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

	    double deltaphi;
	    double tempphi;
	    tempphi=1000.;
	    
	    for ( int i=0; i<photons.size(); i++ ){
	      deltaphi = fabs(photons[i]->P4().DeltaPhi(missingET->P4()));
	      if( deltaphi < tempphi && i < 2 ) tempphi=deltaphi;
	    }
	    deltaphi=tempphi;



	    
            if( deltaphi> 0.4 ){
              if ( HT > 2000. ){
                if ( missingET->P4().Et() > 400. ){
                  countSignalEvent("SRH");
                }
              }
            }
          }
        }
      }
    }    
  }


  
  
}

void Atlas_conf_2016_066::finalize() {
  // Whatever should be done after the run goes here
}       
