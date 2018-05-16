#include "atlas_1604_01306.h"
// AUTHOR: kr
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1604_01306::initialize() {
  setAnalysisName("atlas_1604_01306");          
  setInformation(""
    "# photon+MET search 13 TeV\n"
  "");
  setLuminosity(3.2*units::INVFB);      
//  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SR1;");
//  bookCutflowRegions("MET300;MET450;PT150;PT300;PT450;");
  bookCutflowRegions("00_all;01_trigger;02_MET>150;03_pT>150;04_leadingTight;05_isolated;06_dphimet>0.4;07_0-1jet;08_leptonveto;");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1604_01306::analyze() {
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
  
  electronsMedium = filterPhaseSpace(electronsMedium, 7., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 6., -2.7, 2.7);
  std::vector<Photon*> photonsCand = filterPhaseSpace(photonsLoose, 10., -2.37, 2.37);
  std::vector<Photon*> photonsSig = filterPhaseSpace(photonsMedium, 10., -2.37, 2.37, true);
  std::vector<Jet*> jetsCand = filterPhaseSpace(jets, 20., -4.5, 4.5);

  jetsCand = overlapRemoval(jetsCand, electronsMedium, 0.2);
  electronsMedium = overlapRemoval(electronsMedium, jetsCand, 0.4);  
  muonsCombined = overlapRemoval(muonsCombined, jetsCand, 0.4);  
  jetsCand = overlapRemoval(jetsCand, photonsCand, 0.4);
  
  std::vector<Jet*> jetsSig = filterPhaseSpace(jetsCand, 30., -4.5, 4.5);
  
  countCutflowEvent("00_all");
  
  if (photonsCand.size() == 0 || photonsCand[0]->PT < 120.) return;
  countCutflowEvent("01_trigger");

  double met = missingET->P4().Et();
  if (missingET->P4().Et() < 150.0) return;
  countCutflowEvent("02_MET>150");

  if (photonsCand[0]->PT < 150. ) return;
  countCutflowEvent("03_pT>150");
  
  if (photonsSig.size() == 0 || photonsSig[0]->PT != photonsCand[0]->PT ) return;
  countCutflowEvent("04_leadingTight");
  
  photonsSig = Atlas_1604_01306::isolation(photonsSig);
  
  if (photonsSig.size() == 0 || photonsSig[0]->PT != photonsCand[0]->PT ) return;
  countCutflowEvent("05_isolated");  
  
  if (fabs(missingET->P4().DeltaPhi(photonsSig[0]->P4())) < 0.4) return;  
  countCutflowEvent("06_dphimet>0.4");
  
  if (jetsSig.size() > 1) return;
  if (jetsSig.size() == 1 && fabs(missingET->P4().DeltaPhi(jetsSig[0]->P4())) < 0.4) return;
  countCutflowEvent("07_0-1jet");
  
  if (electronsMedium.size() + muonsCombined.size() > 0 ) return; 
  countCutflowEvent("08_leptonveto");
  
//  double pt = photons[0]->PT;
  countSignalEvent("SR1");
//  if ( met < 300. ) countCutflowEvent("MET150");
//  else if ( met < 450. ) countCutflowEvent("MET300");
//  else countCutflowEvent("MET450");
//  if ( pt < 300. ) countCutflowEvent("PT150");
//  else if ( pt < 450. ) countCutflowEvent("PT300");
//  else countCutflowEvent("PT450");

  return;  
  
}

void Atlas_1604_01306::finalize() {
  // Whatever should be done after the run goes here
}       

std::vector<Photon*> Atlas_1604_01306::isolation(std::vector<Photon*> candidates) {
  
  std::vector<Photon*> passed_candidates;
  
  for(int i = 0; i < candidates.size(); i++) {
    double ET = -candidates[i]->PT;
    for(int j = 0; j < towers.size(); j++) 
      if ( candidates[i]->P4().DeltaR(towers[j]->P4()) < 0.4 )
	ET += towers[j]->ET;
    if ( ET < 2.45 + 0.022*candidates[i]->PT )
      passed_candidates.push_back(candidates[i]);
  }
  
  return passed_candidates;
  
}
