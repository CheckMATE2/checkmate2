#include "cms_2111_06295.h"
// AUTHOR: kr
//  EMAIL: krolb@fuw.edu.pl
void Cms_2111_06295::initialize() {
  setAnalysisName("cms_2111_06295");          
  setInformation(""
    "# ./configure --with-delphes=/home/krolb/tools/Delphes-current --with-hepmc=/home/krolb/tools/share --with-python=/usr/bin/python3 --with-onnx=/home/krolb/tools/onnxruntime-linux-x64-1.23.2 --with-madgraph=/home/krolb/tools/\n"
  "");
  setLuminosity(137.0*units::INVFB);      
  bookSignalRegions("2l_low_04;2l_low_10;2l_low_20;2l_low_30;2l_med_01;2l_med_04;2l_med_10;2l_med_20;2l_med_30;2l_high_01;2l_high_04;2l_high_10;2l_high_20;2l_high_30;2l_ultra_01;2l_ultra_04;2l_ultra_10;2l_ultra_20;2l_ultra_30;3l_EWK_low_04;3l_EWK_low_10;3l_EWK_low_20;3l_EWK_low_30;3l_EWK_med_01;3l_EWK_med_04;3l_EWK_med_10;3l_EWK_med_20;3l_EWK_med_30;3l_WZ_low_04;3l_WZ_low_10;3l_WZ_low_20;3l_WZ_med_01;3l_WZ_med_04;3l_WZ_med_10;3l_WZ_med_20;stop_low_03;stop_low_08;stop_low_12;stop_low_16;stop_low_20;stop_low_25;stop_med_03;stop_med_08;stop_med_12;stop_med_16;stop_med_20;stop_med_25;stop_high_03;stop_high_08;stop_high_12;stop_high_16;stop_high_20;stop_high_25;stop_ultra_03;stop_ultra_08;stop_ultra_12;stop_ultra_16;stop_ultra_20;stop_ultra_25");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_2111_06295::analyze() {
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

  //missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out. Probably not since 3.4.2
  
}

void Cms_2111_06295::finalize() {
  // Whatever should be done after the run goes here
}       
