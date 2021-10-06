#include "atlas_1502_05686.h"
// AUTHOR: JSK
//  EMAIL: jsk@th.physik.uni-bonn.de
void Atlas_1502_05686::initialize() {
  setAnalysisName("atlas_1502_05686");          
  setInformation(""
    "# search for massive supersymmetric particles decaying to many jets\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  bookSignalRegions("SR120_7_0;SR180_7_0;SR80_7_2;SR120_7_1;SR80_7_1;SR100_7_1;SR180_7_1;SR120_7_2;SR140_7_2;SR220_6_1;SR140_7_1");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1502_05686::analyze() {
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


//for reconstruction
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);

  std::vector<Electron*> electronsBaseline = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  std::vector<Muon*> muonsBaseline         = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);

  //  photons = filterPhaseSpace(photons, 10., -2.47, 2.47, true);
  //  photons = filterIsolation(photons);

  jets = overlapRemoval(jets, electronsBaseline, 0.2);
  electronsBaseline = overlapRemoval(electronsBaseline, jets, 0.4);
  muonsBaseline     = overlapRemoval(muonsBaseline, jets, 0.4);
  //  jets = overlapRemoval(jets, photons, 0.2);

    //------------------------------
  // Find b-jets
  // Veto event if 2 bjets are not found
  std::vector<Jet*> bJets, notBJets;
  for (int i=0; i < jets.size(); i++) {
    if ( (fabs(jets[i]->Eta) < 2.5) && (checkBTag(jets[i])) )
      bJets.push_back(jets[i]);
    else 
      notBJets.push_back(jets[i]);
  }

  size_t nJets = jets.size();
  size_t nBJets = bJets.size();

  bool sixJetsSR = false;
  bool sevenJetsSR = false;

  if(nJets < 6)   
    return;

  countCutflowEvent("1_nJets > 6");
  
  if(jets[5]->PT < 80.)
    return;

  countCutflowEvent("2_nJets > 6, pT>80");
    
  sixJetsSR = true;
  
  if(nJets > 6 && jets[6]->PT > 80.)
    sevenJetsSR = true;

  countCutflowEvent("3_nJets > 7, pT>80");
  

  if(sixJetsSR)
    {
      if(jets[5]->PT > 220 && nBJets >= 1)
	countSignalEvent("SR220_6_1");
    }
  if(sevenJetsSR)
    {
      if(jets[6]->PT > 80)
	{
	  if(nBJets >= 1)
	    countSignalEvent("SR80_7_1");
	  if(nBJets >= 2)
	    countSignalEvent("SR80_7_2");
	}
      if(jets[6]->PT > 100)
	{
	  if(nBJets >= 1)
	    countSignalEvent("SR100_7_1");
	}
      if(jets[6]->PT > 120)
	{
	  if(nBJets >= 0)
	    countSignalEvent("SR120_7_0");
	  if (nBJets >= 1)
	    countSignalEvent("SR120_7_1");
	  if(nBJets >= 2)
	    countSignalEvent("SR120_7_2");
	}
      if(jets[6]->PT > 140)
	{
	  if(nBJets >= 1)
	    countSignalEvent("SR140_7_1");
	  if(nBJets >= 2)
	    countSignalEvent("SR140_7_2");
	}
      if(jets[6]->PT > 180)
	{
	  if(nBJets >= 0)
	    countSignalEvent("SR180_7_0");
	  if(nBJets >= 1)
	    countSignalEvent("SR180_7_1");
	}
    }
    
  return;

    
}

void Atlas_1502_05686::finalize() {
  // Whatever should be done after the run goes here
}       
