#include "atlas_1911_12606.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1911_12606::initialize() {
  setAnalysisName("atlas_1911_12606");          
  setInformation(""
    "# search for sleptons and electroweakinos with soft leptons\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("E-inc-01;E-inc-02;E-inc-03;E-inc-05;E-inc-10;E-inc-20;E-inc-30;E-inc-40;E-inc-60;VBF-inc-02;VBF-inc-03;VBF-inc-05;VBF-inc-10;VBF-inc-20;VBF-inc-30;VBF-inc-40;VBF-high-inc-02;VBF-high-inc-03;VBF-high-inc-05;VBF-high-inc-10;VBF-high-inc-20;VBF-high-inc-30;VBF-high-inc-40;S-inc-00;S-inc-01;S-inc-02;S-inc-05;S-inc-10;S-inc-20;S-inc-30;S-inc-40;E-high-ee-05;E-high-ee-10;E-high-ee-20;E-high-ee-30;E-high-ee-40;E-high-ee-60;E-high-mm-02;E-high-mm-03;E-high-mm-05;E-high-mm-10;E-high-mm-20;E-high-mm-30;E-high-mm-40;E-high-mm-60;E-med-ee-05;E-med-ee-10;E-med-ee-20;E-med-ee-30;E-med-mm-02;E-med-mm-03;E-med-mm-05;E-med-mm-10;E-med-mm-20;E-med-mm-30;E-low-ee-05;E-low-ee-10;E-low-ee-20;E-low-ee-30;E-low-ee-40;E-low-ee-60;E-low-mm-02;E-low-mm-03;E-low-mm-05;E-low-mm-10;E-low-mm-20;E-low-mm-30;E-low-mm-40;E-low-mm-60;SR-1L1T-1.0;SR-1L1T-1.5;SR-1L1T-2.0;SR-1L1T-3.0;SR-1L1T-4.0;SR-1L1T-5.0;VBF-low-02;VBF-low-03;VBF-low-05;VBF-low-10;VBF-low-20;VBF-low-30;VBF-low-40;VBF-high-02;VBF-high-03;VBF-high-05;VBF-high-10;VBF-high-20;VBF-high-30;VBF-high-40;S-high-ee-00;S-high-ee-01;S-high-ee-02;S-high-ee-05;S-high-ee-10;S-high-ee-20;S-high-ee-30;S-high-ee-40;S-high-mm-00;S-high-mm-01;S-high-mm-02;S-high-mm-05;S-high-mm-10;S-high-mm-20;S-high-mm-30;S-high-mm-40;S-low-ee-00;S-low-ee-01;S-low-ee-02;S-low-ee-05;S-low-ee-10;S-low-ee-20;S-low-ee-30;S-low-ee-40;S-low-mm-00;S-low-mm-01;S-low-mm-02;S-low-mm-05;S-low-mm-10;S-low-mm-20;S-low-mm-30;S-low-mm-40");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1911_12606::analyze() {
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

void Atlas_1911_12606::finalize() {
  // Whatever should be done after the run goes here
}       
