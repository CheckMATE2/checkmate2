#include "cms1703_01651.h"
// AUTHOR: Avinash Verma
//  EMAIL: avinash.verma@students.iiserpune.ac.in
void Cms1703_01651::initialize() {
  setAnalysisName("cms1703_01651");          
  setInformation(""
    "# Search for dark matter produced with an energetic jet or a hadronically decaying W or Z boson\n"
  "");
  setLuminosity(12.9*units::INVFB);      
  bookSignalRegions("MONOJ510_550;MONOJ550_590;MONOJ590_640;MONOJ640_690;MONOJ690_740;MONOJ740_790;MONOJ790_840;MONOJ840_900;MONOJ900_960;MONOJ960_1020;MONOJ1020_1090;MONOJ1090_1160;MONOJ1160;MONOV250_300;MONOV300_350;MONOV350_400;MONOV400_500;MONOV500_600;MONOV600_750;MONOV750");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.
  //bookCutflowRegions("no_lepton;AK4;Etmiss200;no_bjet;angle;all;start")

  // You should initialize any declared variables here
}

void Cms1703_01651::analyze() {
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
  // reconstruction of electron, muon, jets, photon
  electrons = filterPhaseSpace(electrons, 10., -2.5, 2.5);
  muons = filterPhaseSpace(muons, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 20., -5, 5);
  photons = filterPhaseSpace(photons, 15., -2.5, 2.5);
  // isolation of electron and muon for background removal of W and Zjets
  // isolation criteria is already added in delphes part
  electrons = filterIsolation(electrons);
  muons = filterIsolation(muons);
  //construct tau
  std::vector<Jet *> taus;
  for (int i = 0; i < jets.size(); i++)
  {
    //if (checkTauTag(jets[i], "tight")) {
    //if (checkTauTag(jets[i], "medium")) {
    if (checkTauTag(jets[i], "loose") and fabs(jets[i]->Charge) == 1)
    {
      taus.push_back(jets[i]);
    }
  }
  taus = filterPhaseSpace(taus, 18., -2.3, 2.3);
  // b-jet construction
  std::vector<Jet *> bjets;
  for (int i = 0; i < jets.size(); i++)
  {
    if (checkBTag(jets[i]))
    {
      bjets.push_back(jets[i]);
    }
  }
  bjets = filterPhaseSpace(bjets, 15., -2.4, 2.4);
  // count all events
  //countCutflowEvent("all");
  // EmissT trigger for selecting event
  if(missingET->PT < 200.0)
    return;
  //countCutflowEvent("Etmiss200");
  // Cut if no leading AK4 jet with PT > 100
  int i_central=1000;
  bool is_central_jets=false;
  for(int i=0;i<jets.size();i++){
        
    if (!is_central_jets && jets[i]->PT > 100 && fabs(jets[i]->Eta) < 2.5 ){
        i_central=i;
        is_central_jets=true;
    }
  }
  if(!is_central_jets)
      return;
  //countCutflowEvent("AK4");
// no leptons and phtons and 
  if(!photons.empty() || !electrons.empty() || !muons.empty() || !taus.empty())
    return;
  //countCutflowEvent("no_lepton");  
  //no b-jet cut
  if(bjets.size() > 0)
    return;
  //countCutflowEvent("no_bjet");
  // veto for events in which four leadind AK4 jets>30 , should make an azimuthal
  // angle of >.5 with emisst
  std::vector<Jet*> jet30 = filterPhaseSpace(jets, 30., -2.5, 2.5);
  int i=jet30.size();
  if (i>4){
    i=4;
  }
  for (int j=0;j<i;j++){
    if (fabs(jet30[j]->P4().DeltaPhi(missingET->P4())) < 0.5){
      return;
    }
  }
  //countCutflowEvent("angle");
  // before start of signal region, we drop events with emisst<510 
  if(missingET->PT < 510.)
    return;
  //countCutflowEvent("start");

  /////////////////////////////////////////////////////////////////////////////////////////////////
                   ////////////signal regions////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////////

  if(missingET->PT < 550.0)
    return;
  countSignalEvent("MONOJ510_550");

  if(missingET->PT < 590.0)
    return;
  countSignalEvent("MONOJ550_590");

  if(missingET->PT < 640.0)
    return;
  countSignalEvent("MONOJ590_640");

  if(missingET->PT < 690.0)
    return;
  countSignalEvent("MONOJ640_690");

  if(missingET->PT < 740.0)
    return;
  countSignalEvent("MONOJ690_740");

  if(missingET->PT < 790.0)
    return;
  countSignalEvent("MONOJ740_790");

  if(missingET->PT < 840.0)
    return;
  countSignalEvent("MONOJ790_840");

  if(missingET->PT < 900.0)
    return;
  countSignalEvent("MONOJ840_900");

  if(missingET->PT < 1020.0)
    return;
  countSignalEvent("MONOJ900_1020");

  if(missingET->PT < 1160.0)
    return;
  countSignalEvent("MONOJ1020_1160");

  if(missingET->PT > 1160.0)
    return;
  countSignalEvent("MONOJ1160");


}

void Cms1703_01651::finalize() {
  // Whatever should be done after the run goes here
}       
