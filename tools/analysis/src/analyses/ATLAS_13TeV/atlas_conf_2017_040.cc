#include "atlas_conf_2017_040.h"
// AUTHOR: Daniel Dercks
//  EMAIL: daniel.dercks@desy.de
void Atlas_conf_2017_040::initialize() {
  setAnalysisName("atlas_conf_2017_040");          
  setInformation(""
    "# Search for an invisibly decaying Higgs\n"
    "# or dark matter candidate in association\n"
    "# with a Z boson\n"
    "# sqrt(s) = 13 TeV\n"
    "# int(L) = 36.1 invfb\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("ee;mm");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_conf_2017_040::analyze() {
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
  electronsLoose = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47);
  std::vector<Muon*> muonsLoose = filterPhaseSpace(muonsCombined, 7., -2.5, 2.5);
  
  countCutflowEvent("00_0_All");
  if (electronsLoose.size() + muonsLoose.size() < 2)
    return;
  countCutflowEvent("00_1_PrincipallyTwoLeptons");
  electronsMedium = filterPhaseSpace(electronsLoose, 20., -2.47, 2.47); // Should actually be medium, but gives comparably too few electrons compared to muons. Hence, 'loose' was chosen instead
  muonsCombined = filterPhaseSpace(muonsCombined, 20., -2.5, 2.5);
  jets = filterPhaseSpace(jets, 20., -2.5, 2.5);
  std::vector<Jet*> bjets, nobjets;
  for (Jet* j: jets) {
    if (checkBTag(j))
      bjets.push_back(j);
    else
      nobjets.push_back(j);
  }
  // Not explicitly mentioned, but important since Delphes will 
  // reconstruct all leptons as jets!
  jets = overlapRemoval(jets, electronsLoose, 0.1);
  jets = overlapRemoval(jets, muonsLoose, 0.1);
  bjets = overlapRemoval(bjets, electronsLoose, 0.1);
  bjets = overlapRemoval(bjets, muonsLoose, 0.1);
  
  // Copied from 2017_039
  /*
  electronsLoose = overlapRemoval(electronsLoose, muonsLoose, 0.05); // "sharing an ID track"
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.2);
  nobjets = overlapRemoval(nobjets, electronsLoose, 0.2);
  electronsLoose = overlapRemoval(electronsLoose, nobjets, 0.4);
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.4);
  bjets = overlapRemoval(bjets, muonsLoose, 0.2);
  nobjets = overlapRemoval(nobjets, muonsLoose, 0.2);
  muonsLoose = overlapRemoval(muonsLoose, bjets, 0.4);
  muonsLoose = overlapRemoval(muonsLoose, nobjets, 0.4);
  

  // tighten electron cuts
  std::vector<Electron*> selected;
  for (Electron* e: electronsLoose) {
    for (Electron* et: electronsMedium) {
      if (et == e) {
	selected.push_back(e);
	break;
      }
    }
  }
  electronsMedium = selected;

  std::vector<Muon*> selectedm;
  for (Muon* m: muonsLoose) {
    for (Muon* mt: muonsCombined) {
      if (mt == m) {
	selectedm.push_back(m);
	break;
      }
    }
  }
  muonsCombined = selectedm;
  
  
  // Reconstruct jets from nobjets and bjets
  jets.clear();
  for (Jet* j: nobjets)
    jets.push_back(j);
  for (Jet* j: jets)
    jets.push_back(j);
  */
    

  std::vector<FinalStateObject*> leptons;
  for (Electron* e : electronsMedium) {
    FinalStateObject* fso = new FinalStateObject(e);
    leptons.push_back(fso);
  }
  for (Muon* m : muonsCombined) {
    FinalStateObject* fso = new FinalStateObject(m);
    leptons.push_back(fso);
  }

  if (leptons.size() < 2)
    return;
  countCutflowEvent("01_AtLeastTwoLeptons");
  if (leptons[0]->Type != leptons[1]->Type)
    return;
  countCutflowEvent("02_SameFlavour");
  if (leptons[0]->Charge * leptons[1]->Charge > 0)
    return;
  countCutflowEvent("03_OppositeCharge");
  if (leptons[0]->PT < 30.) // Note that leptons contains either 2e or 2m and hence is automatically pt-sorted
    return;
  countCutflowEvent("04_signalLeptonPT");
  if (electronsLoose.size() + muonsLoose.size() > 2)
    return;
  countCutflowEvent("05_3rdLeptonVeto");
  TLorentzVector pll = leptons[0]->P4() + leptons[1]->P4();
  double mll = pll.M();
  if (mll < 76. || 106 < mll) 
    return;
  countCutflowEvent("06_mll");
  if (missingET->PT < 90.)
    return;
  countCutflowEvent("07_etmiss");
  double HT = leptons[0]->PT + leptons[1]->PT;
  for (Jet* j: jets)
    HT += j->PT;
  if (missingET->PT / HT < 0.6)
    return;
  countCutflowEvent("08_METoverHT");
  if (fabs(pll.DeltaPhi(missingET->P4())) < 2.7)
    return;
  countCutflowEvent("09_DeltaPhillMET");
  if (leptons[0]->P4().DeltaR(leptons[1]->P4()) > 1.8)
    return;
  countCutflowEvent("10_DeltaLL");
  TLorentzVector jetsPlusMET = missingET->P4();
  for (Jet* j: jets)
    jetsPlusMET += j->P4();
  if (fabs(jetsPlusMET.Pt() - pll.Pt()) > 0.2*pll.Pt())
    return;
  countCutflowEvent("11_ptdiff");
  if (bjets.size() > 0)
    return;
  countCutflowEvent("12_bveto");
  if (electronsMedium.size() == 2)
    countSignalEvent("ee");
  else if (muonsCombined.size() == 2)
    countSignalEvent("mm");    
}

void Atlas_conf_2017_040::finalize() {
  // Whatever should be done after the run goes here
}       
