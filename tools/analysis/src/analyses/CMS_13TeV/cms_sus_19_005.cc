#include "cms_sus_19_005.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Cms_sus_19_005::initialize() {
  setAnalysisName("cms_sus_19_005");          
  setInformation(""
    "# search for new physics with hadronic final states with MT2 variable\n"
    "# only prompt decays\n"
  "");
  setLuminosity(137.0*units::INVFB);      
  bookSignalRegions("2j_loose;2j_tight;4j_loose;4j_tight;7j_loose;7j_tight;10j_loose;10j_tight;2b_loose;2b_tight;3b_loose;3b_tight;4b_loose;4b_tight;7j_3b_loose;7j_3b_tight;7j_4b_loose;7j_4b_tight;10j_4b_loose;10j_4b_tight;Mono-phi;1j_0b_250;1j_0b_350;1j_0b_450;1j_0b_575;1j_0b_750;1j_0b_1000;1j_0b_1200;1j_1b_250;1j_1b_350;1j_1b_450;1j_1b_575;1j_1b_700;2-3j_0b_250_200;2-3j_0b_250_300;2-3j_0b_250_400;2-3j_1b_250_200;2-3j_1b_250_300;2-3j_1b_250_400;2-3j_2b_250_200;2-3j_2b_250_300;2-3j_2b_250_400;2-6j_3b_250_200;2-6j_3b_250_300;2-6j_3b_250_400;4-6j_0b_250_200;4-6j_0b_250_300;4-6j_0b_250_400;4-6j_1b_250_200;4-6j_1b_250_300;4-6j_1b_250_400;4-6j_2b_250_200;4-6j_2b_250_300;4-6j_2b_250_400;7j_0b_250_200;7j_0b_250_300;7j_0b_250_500;7j_1b_250_200;7j_1b_250_300;7j_2b_250_200;7j_2b_250_300;7j_3b_250_200;7j_3b_250_300");
  bookSignalRegions("2-3j_0b_450_200;2-3j_0b_450_300;2-3j_0b_450_400;2-3j_0b_450_500;2-3j_1b_450_200;2-3j_1b_450_300;2-3j_1b_450_400;2-3j_1b_450_500;2-3j_2b_450_200;2-3j_2b_450_300;2-3j_2b_450_400;2-3j_2b_450_500;2-6j_3b_450_200;2-6j_3b_450_300;2-6j_3b_450_400;2-6j_3b_450_500;4-6j_0b_450_200;4-6j_0b_450_300;4-6j_0b_450_400;4-6j_0b_450_500;4-6j_1b_450_200;4-6j_1b_450_300;4-6j_1b_450_400;4-6j_1b_450_500;4-6j_2b_450_200;4-6j_2b_450_300;4-6j_2b_450_400;4-6j_2b_450_500;7j_0b_450_200;7j_0b_450_300;7j_0b_450_400;7j_1b_450_200;7j_1b_450_300;7j_1b_450_400;7j_2b_450_200;7j_2b_450_300;7j_2b_450_400;7j_3b_450_200;7j_3b_450_300;7j_3b_450_400");  
  bookSignalRegions("2-3j_0b_575_200;2-3j_0b_575_300;2-3j_0b_575_400;2-3j_0b_575_500;2-3j_0b_575_600;2-3j_0b_575_700;2-3j_0b_575_800;2-3j_0b_575_900;2-3j_0b_575_1000;2-3j_0b_575_1100;2-3j_1b_575_200;2-3j_1b_575_300;2-3j_1b_575_400;2-3j_1b_575_600;2-3j_1b_575_800;2-3j_1b_575_1000;2-3j_2b_575_200;2-3j_2b_575_300;2-3j_2b_575_400;2-3j_2b_575_600;2-3j_2b_575_800");  
  bookSignalRegions("2-6j_3b_575_200;2-6j_3b_575_300;2-6j_3b_575_400;2-6j_3b_575_600;2-6j_3b_575_800;4-6j_0b_575_200;4-6j_0b_575_300;4-6j_0b_575_400;4-6j_0b_575_500;4-6j_0b_575_600;4-6j_0b_575_700;4-6j_0b_575_800;4-6j_0b_575_900;4-6j_0b_575_1000;4-6j_0b_575_1100;4-6j_1b_575_200;4-6j_1b_575_300;4-6j_1b_575_400;4-6j_1b_575_600;4-6j_1b_575_800;4-6j_1b_575_1000;4-6j_2b_575_200;4-6j_2b_575_300;4-6j_2b_575_400;4-6j_2b_575_600;4-6j_2b_575_800");  
  bookSignalRegions("7-9j_0b_575_200;7-9j_0b_575_300;7-9j_0b_575_400;7-9j_0b_575_600;7-9j_0b_575_800;7-9j_1b_575_200;7-9j_1b_575_300;7-9j_1b_575_400;7-9j_1b_575_600;7-9j_1b_575_800;7-9j_2b_575_200;7-9j_2b_575_300;7-9j_2b_575_400;7-9j_2b_575_600;7-9j_2b_575_800;7-9j_3b_575_200;7-9j_3b_575_300;7-9j_3b_575_400;7-9j_3b_575_600;7-9j_4b_575_200;7-9j_4b_575_300;7-9j_4b_575_400;10j_0b_575_200;10j_0b_575_300;10j_0b_575_500;10j_1b_575_200;10j_1b_575_300;10j_1b_575_500;10j_2b_575_200;10j_2b_575_300;10j_2b_575_500;10j_3b_575_200;10j_3b_575_300;10j_4b_575_200");
  bookSignalRegions("2-3j_0b_1200_200;2-3j_0b_1200_400;2-3j_0b_1200_600;2-3j_0b_1200_800;2-3j_0b_1200_1000;2-3j_0b_1200_1200;2-3j_1b_1200_200;2-3j_1b_1200_400;2-3j_1b_1200_600;2-3j_1b_1200_800;2-3j_1b_1200_1000;2-3j_1b_1200_1200;2-3j_2b_1200_200;2-3j_2b_1200_400;2-3j_2b_1200_600;2-3j_2b_1200_800;2-3j_2b_1200_1000;2-6j_3b_1200_200;2-6j_3b_1200_400;2-6j_3b_1200_600;4-6j_0b_1200_200;4-6j_0b_1200_400;4-6j_0b_1200_600;4-6j_0b_1200_800;4-6j_0b_1200_1000;4-6j_0b_1200_1200;4-6j_1b_1200_200;4-6j_1b_1200_400;4-6j_1b_1200_600;4-6j_1b_1200_800;4-6j_1b_1200_1000;4-6j_1b_1200_1200;4-6j_2b_1200_200;4-6j_2b_1200_400;4-6j_2b_1200_600;4-6j_2b_1200_800;4-6j_2b_1200_1000");  
  bookSignalRegions("7-9j_0b_1200_200;7-9j_0b_1200_400;7-9j_0b_1200_600;7-9j_0b_1200_800;7-9j_0b_1200_1000;7-9j_1b_1200_200;7-9j_1b_1200_400;7-9j_1b_1200_600;7-9j_1b_1200_800;7-9j_1b_1200_1000;7-9j_2b_1200_200;7-9j_2b_1200_400;7-9j_2b_1200_600;7-9j_2b_1200_800;7-9j_3b_1200_200;7-9j_3b_1200_400;7-9j_3b_1200_600;7-9j_4b_1200_200;7-9j_4b_1200_400;10j_0b_1200_200;10j_0b_1200_400;10j_0b_1200_600;10j_1b_1200_200;10j_1b_1200_400;10j_1b_1200_600;10j_2b_1200_200;10j_2b_1200_400;10j_2b_1200_600;10j_3b_1200_200;10j_3b_1200_400;10j_4b_1200_200");
  bookSignalRegions("2-3j_0b_1500_400;2-3j_0b_1500_600;2-3j_0b_1500_800;2-3j_0b_1500_1000;2-3j_0b_1500_1200;2-3j_0b_1500_1400;2-3j_0b_1500_1800;2-3j_1b_1500_400;2-3j_1b_1500_600;2-3j_1b_1500_800;2-3j_1b_1500_1000;2-3j_1b_1500_1200;2-3j_2b_1500_400;2-6j_3b_1500_400;2-6j_3b_1500_600;4-6j_0b_1500_400;4-6j_0b_1500_600;4-6j_0b_1500_800;4-6j_0b_1500_1000;4-6j_0b_1500_1200;4-6j_0b_1500_1400;4-6j_0b_1500_1600;4-6j_1b_1500_400;4-6j_1b_1500_600;4-6j_1b_1500_800;4-6j_1b_1500_1000;4-6j_1b_1500_1400;4-6j_2b_1500_400;4-6j_2b_1500_600;4-6j_2b_1500_800");
  bookSignalRegions("7-9j_0b_1500_400;7-9j_0b_1500_600;7-9j_0b_1500_800;7-9j_0b_1500_1000;7-9j_0b_1500_1400;7-9j_1b_1500_400;7-9j_1b_1500_600;7-9j_1b_1500_800;7-9j_2b_1500_400;7-9j_2b_1500_600;7-9j_2b_1500_800;7-9j_3b_1500_400;7-9j_3b_1500_800;7-9j_4b_1500_400;10j_0b_1500_400;10j_0b_1500_800;10j_1b_1500_400;10j_1b_1500_800;10j_2b_1500_400;10j_3b_1500_400;10j_4b_1500_400");  
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_sus_19_005::analyze() {
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

void Cms_sus_19_005::finalize() {
  // Whatever should be done after the run goes here
}       
