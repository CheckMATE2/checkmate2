#include "atlas_phys_2014_010_sq_hl.h"
// AUTHOR: kr
//  EMAIL: krolb@fuw.edu.pl

std::string Atlas_phys_2014_010_sq_hl::nameSR[10] = {"a.2jl", "b.2jm", "c.3j", "d.4jl", "e.4jm", "f.4jt", "g.5j", "h.6jl", "i.6jm", "j.6jt"};

void Atlas_phys_2014_010_sq_hl::initialize() {
  setAnalysisName("atlas_phys_2014_010_sq_hl");          
  setInformation(""
    "# 2-6 jets + met at 14 TeV high lumi 3000/fb\n"
     "# ATLAS projection for high lumi run\n"
  "");
  setLuminosity(3000.0*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SR2jl;SR2jm;SR3j;SR4jl;SR4jm;SR4jt;SR5j;SR6jl;SR6jm;SR6jt;");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  for ( int i=0; i < 10; i++) {
    bookCutflowRegions(nameSR[i]+"_"+"CR01_all");
    bookCutflowRegions(nameSR[i]+"_"+"CR02_missETjetsPT");
    bookCutflowRegions(nameSR[i]+"_"+"CR07_dphiMin2J3J");
    bookCutflowRegions(nameSR[i]+"_"+"CR08_dphiMinMoreJ");
    bookCutflowRegions(nameSR[i]+"_"+"CR13_meffIncl");
    if ( i > 1 ) bookCutflowRegions(nameSR[i]+"_"+"CR03_PT3J");
    if ( i > 2 ) bookCutflowRegions(nameSR[i]+"_"+"CR04_PT4J");
    if ( i > 5 ) bookCutflowRegions(nameSR[i]+"_"+"CR05_PT5J");
    if ( i > 6 ) bookCutflowRegions(nameSR[i]+"_"+"CR06_PT6J");
    if ( i < 2 || i == 5 ) bookCutflowRegions(nameSR[i]+"_"+"CR11_RHT");
    else bookCutflowRegions(nameSR[i]+"_"+"CR12_Rmeff");
    bookCutflowRegions(nameSR[i]+"_"+"CR13_meffIncl");
  }  
  
  // You should initialize any declared variables here
}

void Atlas_phys_2014_010_sq_hl::analyze() {
  // Your eventwise analysis code goes here
  // The following objects are always defined unless they are 'ignored' above. They form std::vector objects of the respective Delphes class type (except for Etmiss which is a single object)
  // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
  // Within a std::vector, all members are ordered with highest pt coming first.

  // electronsLoose, electronsMedium, electronsTight   are list of electrons that passed respective efficiency and reconstruction cuts
  // muonsCombinedPlus, muonsCombined                  as above for muons
  // photons                                           as above for photons
  // jets are all reconstructed jets                   as above for jets. Note that electrons are most certainly also reconstructed as a jet -> overlap removal do avoid double counting necessary!
  // tracks, towers                                    calorimeter and tracker information. Usually not needed.
  // missingET                                         rec missing ET EXCLUDING muons.

  
  // Here is a couple of useful functions and lines:  
  //------------Phase Space Cuts (defined for jets, electronsXYZ, muonsXYZ, photonsXYZ)
  // jets = filterPhaseSpace(jets, 20., -2.8, 2.8)  // The vector 'jets' only contains jets with pt >= 20 GeV and -2.8 < eta < 2.8. This function is applicable to other particles too (electronsMedium, ... ).
  // jets = overlapRemoval(jets, electronsLoose, 0.2) Removes all jets for which there exists any electron in 'electronsLoose' with deltaR < 0.2.
  // jets = overlapRemoval(jets, 0.2) If two jets overlap within deltaR < 0.2, only the harder jet is stored.
  
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
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in ChekćkMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  for ( int i=0; i < 10; i++ ) countCutflowEvent(nameSR[i]+"_"+"CR01_all");

//for reconstruction
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);

  std::vector<Electron*> electronsBaseline = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  std::vector<Muon*> muonsBaseline         = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);

// remnant from the 8 TeV study  
  photons = filterPhaseSpace(photons, 130., -2.47, 2.47, true);
  photons = filterIsolation(photons);

  jets = overlapRemoval(jets, electronsBaseline, 0.2);
  electronsBaseline = overlapRemoval(electronsBaseline, jets, 0.4);
  muonsBaseline     = overlapRemoval(muonsBaseline, jets, 0.4);
  jets = overlapRemoval(jets, photons, 0.2);
  
  //for veto where baseline leptons exist
  if ( electronsBaseline.size() + muonsBaseline.size() != 0 ) return;
  //for veto where missingET < 160
  if ( missingET->P4().Et() < 160. ) return;
  //for veto where pt(j1) < 130 or pt(j2) < 60
  if ( jets.size() < 2 || jets[0]->PT < 160. || jets[1]->PT < 60. ) return;
  for ( int i=0; i < 10; i++ ) countCutflowEvent(nameSR[i]+"_"+"CR02_missETjetsPT");

//*************************
//***for event selection***
//*************************
//for PT3J, PT4J, etc. and  meff_NJets
  flag3J = flag4J = flag5J = flag6J = false;
  meff2J = jets[0]->PT + jets[1]->PT + missingET->PT;

  //for that of SR-3J, SR-4J, SR-5J and SR-6J
  if ( jets.size() > 2 && jets[2]->PT > 60. ) {
    flag3J = true;
    meff3J = meff2J + jets[2]->PT;
    for ( int i=0; i < 10; i++ ) {
      if ( i > 1 ) countCutflowEvent(nameSR[i]+"_"+"CR03_PT3J");
    }
    //for that of SR-4J, SR-5J and SR-6J
    if ( jets.size() > 3  && jets[3]->PT > 60. ) {
      flag4J = true;
      meff4J = meff3J + jets[3]->PT;
      for ( int i=0; i < 10; i++ ) {
        if ( i > 2 ) countCutflowEvent(nameSR[i]+"_"+"CR04_PT4J");
      }
      //for that of SR-5J and SR-6J
      if ( jets.size() > 4 && jets[4]->PT > 60. ) {
        flag5J = true;
        meff5J = meff4J + jets[4]->PT;
        for ( int i=0; i < 10; i++ ) {
          if ( i > 5 ) countCutflowEvent(nameSR[i]+"_"+"CR05_PT5J");
        }
        //for that SR-6J only
        if ( jets.size() > 5 && jets[5]->PT > 60. ) {
          flag6J = true;
          meff6J = meff5J + jets[5]->PT;
          for ( int i=0; i < 10; i++ ) {
            if ( i > 6 ) countCutflowEvent(nameSR[i]+"_"+"CR06_PT6J");
          }
        }
      }
    }
  }

//for dphiMin > 0.4 in 2-jet and 3-jet signal regions
  double dphiMin2J3J = 1.E6, dphi;
  for ( int i=0; i < 2; i++ ) {
    dphi = fabs( jets[i]->P4().DeltaPhi( missingET->P4() ) );
    if ( dphi < dphiMin2J3J ) dphiMin2J3J = dphi;
  }
  if ( flag3J ) {
    dphi = fabs( jets[2]->P4().DeltaPhi( missingET->P4() ) );
    if ( dphi < dphiMin2J3J ) dphiMin2J3J = dphi;    
  }

// not sure if j3 in Tab.7 denotes only signal jets   
//  if ( jets.size() > 2 && jets[2]->PT > 40. ) {
//    dphi = fabs( jets[2]->P4().DeltaPhi( missingET->P4() ) );
//    if ( dphi < dphiMin2J3J ) dphiMin2J3J = dphi;
//  }

//for additional requirement of dphiMin > 0.2 in other signal regions
  double dphiMinMoreJ = 1.E6;
  for ( int i = 2; i < jets.size(); i++ ) {
    if ( jets[i]->PT > 40. ) {
      dphi = fabs ( jets[i]->P4().DeltaPhi( missingET->P4() ) );
      if ( dphi < dphiMinMoreJ ) dphiMinMoreJ = dphi;
    }
  }

//for HT and meffIncl
  double HT = 0.;
  for ( int i=0; i < jets.size(); i++ ) {
    if ( jets[i]->PT > 40. ) {
      HT += jets[i]->PT; 
    }
  }
  double meffIncl = HT + missingET->PT ;

//veto where dphiMin2J3J < 0.4
  if ( dphiMin2J3J < 0.4 ) return;
  countCutflowEvent("a.2jl_CR07_dphiMin2J3J");
  countCutflowEvent("b.2jm_CR07_dphiMin2J3J");
  if (flag3J) countCutflowEvent("c.3j_CR07_dphiMin2J3J");
  if (flag4J) {
    countCutflowEvent("d.4jl_CR07_dphiMin2J3J");
    countCutflowEvent("e.4jm_CR07_dphiMin2J3J");
    countCutflowEvent("f.4jt_CR07_dphiMin2J3J");
  }
  if (flag5J) countCutflowEvent("g.5j_CR07_dphiMin2J3J");
  if (flag6J) {
    countCutflowEvent("h.6jl_CR07_dphiMin2J3J");
    countCutflowEvent("i.6jm_CR07_dphiMin2J3J");
    countCutflowEvent("j.6jt_CR07_dphiMin2J3J");
  }
  

  if ( dphiMinMoreJ < 0.2 ) return;
  countCutflowEvent("a.2jl_CR08_dphiMinMoreJ");
  countCutflowEvent("b.2jm_CR08_dphiMinMoreJ");
  if (flag3J) countCutflowEvent("c.3j_CR08_dphiMinMoreJ");
  if (flag4J) {
    countCutflowEvent("d.4jl_CR08_dphiMinMoreJ");
    countCutflowEvent("e.4jm_CR08_dphiMinMoreJ");
    countCutflowEvent("f.4jt_CR08_dphiMinMoreJ");
  }
  if (flag5J) countCutflowEvent("g.5j_CR08_dphiMinMoreJ");
  if (flag6J) {
    countCutflowEvent("h.6jl_CR08_dphiMinMoreJ");
    countCutflowEvent("i.6jm_CR08_dphiMinMoreJ");
    countCutflowEvent("j.6jt_CR08_dphiMinMoreJ");
  } 

//for signal region 2jl
  if ( missingET->P4().Et() / sqrt(HT) > 8. ) {
    countCutflowEvent(std::string("a.2jl")+"_"+"CR11_RHT");
    if ( meffIncl > 5000. ) {
      countCutflowEvent(std::string("a.2jl")+"_"+"CR13_meffIncl");
      countSignalEvent("SR2jl");
    }
  }

//for signal region 2jm 
  if ( missingET->P4().Et() / sqrt(HT) > 15. ) {
    countCutflowEvent(std::string("b.2jm")+"_"+"CR11_RHT");
    if ( meffIncl > 4900. ) {
      countCutflowEvent(std::string("b.2jm")+"_"+"CR13_meffIncl");
      countSignalEvent("SR2jm");
    }
  }


//for signal region 3j
  if ( flag3J && missingET->P4().Et() / meff3J > 0.3 ) {
    countCutflowEvent(std::string("c.3j")+"_"+"CR12_Rmeff");
    if ( meffIncl > 4000. ) {
      countCutflowEvent(std::string("c.3j")+"_"+"CR13_meffIncl");
      countSignalEvent("SR3j");
    }
  }

//for signal region 4jl
  if ( flag4J && missingET->P4().Et() / meff4J > 0.35 ) {
    countCutflowEvent(std::string("d.4jl")+"_"+"CR12_Rmeff");
    if ( meffIncl > 4000. ) {
      countCutflowEvent(std::string("d.4jl")+"_"+"CR13_meffIncl");
      countSignalEvent("SR4jl");
    }
  }

//for signal region 4jm
  if ( flag4J && missingET->P4().Et() / meff4J > 0.25 ) {
    countCutflowEvent(std::string("e.4jm")+"_"+"CR12_Rmeff");
    if ( meffIncl > 4000. ) {
      countCutflowEvent(std::string("e.4jm")+"_"+"CR13_meffIncl");
      countSignalEvent("SR4jm");
    }
  }

//for signal region 4jt
  if ( flag4J && missingET->P4().Et() / sqrt(HT) > 10. ) {
    countCutflowEvent(std::string("f.4jt")+"_"+"CR11_RHT");
    if ( meffIncl > 4500. ) {
      countCutflowEvent(std::string("f.4jt")+"_"+"CR13_meffIncl");
      countSignalEvent("SR4jt");
    }
  }

//for signal region 5j
  if ( flag5J && missingET->P4().Et() / meff5J > 0.25 ) {
    countCutflowEvent(std::string("g.5j")+"_"+"CR12_Rmeff");
    if ( meffIncl > 4000. ) {
      countCutflowEvent(std::string("g.5j")+"_"+"CR13_meffIncl");
      countSignalEvent("SR5j");
    }
  }

//for signal region 6jl
  if ( flag6J && missingET->P4().Et() / meff6J > 0.25 ) {
    countCutflowEvent(std::string("h.6jl")+"_"+"CR12_Rmeff");
    if ( meffIncl > 3400. ) {
      countCutflowEvent(std::string("h.6jl")+"_"+"CR13_meffIncl");
      countSignalEvent("SR6jl");
    }
  }

//for signal region 6jm
  if ( flag6J && missingET->P4().Et() / meff6J > 0.35 ) {
    countCutflowEvent(std::string("i.6jm")+"_"+"CR12_Rmeff");
    if ( meffIncl > 3500. ) {
      countCutflowEvent(std::string("i.6jm")+"_"+"CR13_meffIncl");
      countSignalEvent("SR6jm");
    }
  }  
  
//for signal region 6jt
  if ( flag6J && missingET->P4().Et() / meff6J > 0.15 ) {
    countCutflowEvent(std::string("j.6jt")+"_"+"CR12_Rmeff");
    if ( meffIncl > 5000. ) {
      countCutflowEvent(std::string("j.6jt")+"_"+"CR13_meffIncl");
      countSignalEvent("SR6jt");
    }
  }
    
  
}

void Atlas_phys_2014_010_sq_hl::finalize() {
  // Whatever should be done after the run goes here
}       
