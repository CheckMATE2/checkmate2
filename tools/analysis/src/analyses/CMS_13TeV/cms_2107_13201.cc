#include "cms_2107_13201.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Cms_2107_13201::initialize() {
  setAnalysisName("cms_2107_13201");          
  setInformation(""
    "# monojet with multibin\n"
  "");
  setLuminosity(137.0*units::INVFB);      
  bookSignalRegions("A_00;A_01;A_02;A_03;A_04;A_05;A_06;A_07;A_08;A_09;A_10;A_11;A_12;A_13;A_14;A_15;A_16;A_17;A_18;A_19;A_20;A_21;B_2016_00;B_2016_01;B_2016_02;B_2016_03;B_2016_04;B_2016_05;B_2016_06;B_2016_07;B_2016_08;B_2016_09;B_2016_10;B_2016_11;B_2016_12;B_2016_13;B_2016_14;B_2016_15;B_2016_16;B_2016_17;B_2016_18;B_2016_19;B_2016_20;B_2016_21;B_2017_00;B_2017_01;B_2017_02;B_2017_03;B_2017_04;B_2017_05;B_2017_06;B_2017_07;B_2017_08;B_2017_09;B_2017_10;B_2017_11;B_2017_12;B_2017_13;B_2017_14;B_2017_15;B_2017_16;B_2017_17;B_2017_18;B_2017_19;B_2017_20;B_2017_21;B_2018_00;B_2018_01;B_2018_02;B_2018_03;B_2018_04;B_2018_05;B_2018_06;B_2018_07;B_2018_08;B_2018_09;B_2018_10;B_2018_11;B_2018_12;B_2018_13;B_2018_14;B_2018_15;B_2018_16;B_2018_17;B_2018_18;B_2018_19;B_2018_20;B_2018_21");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_2107_13201::analyze() {
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

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
}

void Cms_2107_13201::finalize() {
  // Whatever should be done after the run goes here
}       
