#include "atlas_conf_2015_004.h"
// AUTHOR: Jong Soo Kim
//  EMAIL: jsk@th.physik.uni-bonn.de
void Atlas_conf_2015_004::initialize() {
  setAnalysisName("atlas_conf_2015_004");          
  setInformation(""
    "# Search for an invisibly decaying Higgs boson produced via vector boson fusion.\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("M1;");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.
  bookCutflowRegions("0_2jets;1_btag;2_tautag;3_leptonveto;4_MET;5_DeltaPhijet;6_DeltaPhi_jetMET;7_DeltaETA;8_Mcut;9_jetveto");

  // You should initialize any declared variables here
}

void Atlas_conf_2015_004::analyze() {
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
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  electronsTight = filterPhaseSpace(electronsTight,10.,-2.47,2.47);
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus,5.,-2.47,2.47);
  

  // jets = overlapRemoval(jets, electronsLoose, 0.2) Removes all jets for which there exists any electron in 'electronsLoose' with deltaR < 0.2.
  // jets = overlapRemovel(jets, 0.2) If two jets overlap within deltaR < 0.2, only the harder jet is stored.
  
  jets = overlapRemoval(jets, electronsTight, 0.2);

  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  muonsCombinedPlus = overlapRemoval(muonsCombinedPlus, jets, 0.4);

  //------------Isolation Checks (defined for electronsXYZ, muonsXYZ, photonsXYZ
  //------------        For each object, if the user entered N isolation conditions, they can be
  //------------        checked individually be the second argument (running from 0 to N-1).
  // electronsMedium = filterIsolation(electronsMedium, 0)            Removes electrons that do not pass the first isolation condition entered into the AnalysisManager by the user
  // std::vector<int> flags; flags.push_back(0); flags.push_back(2);
  // electronsMedium = filterIsolation(electronsMedium, flags)        Same as above, but both the first and the third condition have to be fulfilled
  // electronsMedium = filterIsolation(electronsMedium)               Same as above, but all conditions have to be fulfilled.
  
  electronsTight = filterIsolation(electronsTight);
  muonsCombinedPlus = filterIsolation(muonsCombinedPlus);

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
  
  if (jets.size() < 2 )
    return;

  if ( jets[0]->PT < 75. || jets[1]->PT < 50. )
    return;

  countCutflowEvent("0_2jets");

  for(int i=0;i < jets.size();i++){
    if (checkBTag(jets[i], 0) ) return;
  }

  countCutflowEvent("1_btag");

  for(int i=0;i < jets.size();i++){
    if (checkTauTag(jets[i], "tight") ) return;
  }
  
  countCutflowEvent("2_tautag");

  if(electronsTight.size()+muonsCombinedPlus.size() != 0 ) return;

  countCutflowEvent("3_leptonveto");

  if( missingET->P4().Et() < 150. ) return;

  countCutflowEvent("4_MET");
    
  if( fabs(jets[0]->P4().DeltaPhi(jets[1]->P4())) > 2.5) return;

  countCutflowEvent("5_DeltaPhijet");

  if( fabs(jets[0]->P4().DeltaPhi(missingET->P4())) < 1.6 || fabs(jets[1]->P4().DeltaPhi(missingET->P4())) < 1. ) return;

  countCutflowEvent("6_DeltaPhi_jetMET");


  if( fabs(jets[0]->P4().PseudoRapidity()-jets[1]->P4().PseudoRapidity() ) < 4.8 || jets[0]->P4().PseudoRapidity()*jets[1]->P4().PseudoRapidity() > 0.) return;

  countCutflowEvent("7_DeltaETA");

  if( (jets[0]->P4() + jets[1]->P4()).M() < 1000. ) return;

  countCutflowEvent("8_Mcut");

  if( jets.size() > 2  && jets[2]->PT > 30. && jets[2]->Eta < 4.5 ) return;

  countCutflowEvent("9_jetveto");

  countSignalEvent("M1");


}

void Atlas_conf_2015_004::finalize() {
  // Whatever should be done after the run goes here
}       
