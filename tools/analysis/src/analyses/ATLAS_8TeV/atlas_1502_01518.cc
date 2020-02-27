#include "atlas_1502_01518.h"
// AUTHOR: Jamie Tattersall
//  EMAIL: tattersall@thphys.uni-heidelberg.de
void Atlas_1502_01518::initialize() {
  setAnalysisName("atlas_1502_01518");          
  setInformation(""
    "# ATLAS\n"
     "# arXiv:1502.01518\n"
     "# CERN-PH-EP-2014-299\n"
     "# Monojet and missing energy\n"
     "# Energy: 8 TeV\n"
     "# Luminosity: 20.3 fb^-1\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  //ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SR1;SR2;SR3;SR4;SR5;SR6;SR7;SR8;SR9;");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1502_01518::analyze() {
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
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in ChekćkMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  electronsMedium = filterPhaseSpace(electronsMedium, 20., -2.47, 2.47);           
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5);         
  jets = filterPhaseSpace(jets, 25., -4.5, 4.5);
  photonsMedium = filterPhaseSpace(photonsMedium, 25., -4.5, 4.5); 
  
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);
  
  //jet-electron overlap removal
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  
  //trigger
  if (missingET->P4().Et() < 80.) return;
  countCutflowEvent("a_trigger");

  //Etmiss > 150.
  if (missingET->P4().Et() < 150.) return;
  countCutflowEvent("b_EtMiss_150");
  
  //Jet quality requirements (interpret as a photon veto)
  if ( photonsMedium.size() > 0 && photonsMedium[0]->PT > 30. ) return;
  countCutflowEvent("c_jetquality_(photon)");
  
  // jet1 > 30 GeV and |eta| < 4.5
  if ( jets.size() == 0 || jets[0]->PT < 30. ) return;
  countCutflowEvent("d_leadingjet>30");

  // muon veto
  if ( isoMuons.size() != 0 ) return;
  countCutflowEvent("e_muonveto");
  
  // electron veto
  if ( electronsMedium.size() != 0 ) return;
  countCutflowEvent("f_electronveto");  

    //Veto on isolated tracks
    std::vector<Track*> tracksSoft = filterPhaseSpace(tracks, 3., -2.5, 2.5);  
  std::vector<Track*> tracksHard = filterPhaseSpace(tracksSoft, 10., -2.5, 2.5);
          
    //Check no other hard track lies close
    std::vector<Track*> tracksIso_a;
    for(int i = 0; i < tracksHard.size(); i++){
    bool isoTest = true;
    for(int j = 0; j < tracksHard.size(); j++){
      if(i!=j) {
        if(tracksHard[i]->P4().DeltaR(tracksHard[j]->P4()) <0.4) isoTest = false;
      }
    }
    if(isoTest==true) tracksIso_a.push_back(tracksHard[i]);
    }
            
    //Check no soft track lies close
    std::vector<Track*> tracksIso_b;
    for(int i = 0; i < tracksIso_a.size(); i++){
      bool isoTest = true;
      for(int j = 0; j < tracksSoft.size(); j++){
          if( tracksSoft[j]->PT <10.){
            if(tracksIso_a[i]->P4().DeltaR(tracksSoft[j]->P4()) <0.4) isoTest = false;
          }
        }
        if(isoTest==true) return;
    }
    countCutflowEvent("g_trackveto");

  // jet1 > 150 GeV and |eta| < 2.0
  if ( (jets[0]->PT < 120.) || (fabs(jets[0]->Eta) > 2.0)  ) return;
  countCutflowEvent("h_leadingjet>120");

  // jet1_pt/etMiss > 0.5
  if ( (jets[0]->PT / missingET->P4().Et()) < 0.5) return;
  countCutflowEvent("i_jet/missET>0.5");  
  
  // dPhi(jet, met) > 1.0
  for ( int i = 0; i < jets.size() && jets[i]->PT > 30.; i++)
    if ( fabs(missingET->P4().DeltaPhi(jets[i]->P4())) < 1.0) return;
  countCutflowEvent("j_phijetET");
  
  //--------------------------
  //------Signal Regions------
  //--------------------------
  //--------------------------
  
  // Signal region 1 (EtMiss > 150 (already done)
  //--------------------------
  countCutflowEvent("sr1(EtMiss > 150)");
  countSignalEvent("SR1");

  // Signal region 2 (EtMiss > 150 (already done)
  //--------------------------
  if (missingET->P4().Et() < 200.) return;
  countCutflowEvent("sr2(EtMiss > 200)");
  countSignalEvent("SR2");  

  // Signal region 3 (EtMiss > 250 (already done)
  //--------------------------
  if (missingET->P4().Et() < 250.) return;
  countCutflowEvent("sr2(EtMiss > 250)");
  countSignalEvent("SR3"); 

  // Signal region 4 (EtMiss > 300 (already done)
  //--------------------------
  if (missingET->P4().Et() < 300.) return;
  countCutflowEvent("sr4(EtMiss > 300)");
  countSignalEvent("SR4"); 

  // Signal region 5 (EtMiss > 350 (already done)
  //--------------------------
  if (missingET->P4().Et() < 350.) return;
  countCutflowEvent("sr5(EtMiss > 350)");
  countSignalEvent("SR5"); 

  // Signal region 6 (EtMiss > 400 (already done)
  //--------------------------
  if (missingET->P4().Et() < 400.) return;
  countCutflowEvent("sr6(EtMiss > 400)");
  countSignalEvent("SR6"); 

  // Signal region 7 (EtMiss > 500 (already done)
  //--------------------------
  if (missingET->P4().Et() < 500.) return;
  countCutflowEvent("sr7(EtMiss > 500)");
  countSignalEvent("SR7");   
  
  // Signal region 8 (EtMiss > 150 (already done)
  //--------------------------
  if (missingET->P4().Et() < 600.) return;
  countCutflowEvent("sr8(EtMiss > 600)");
  countSignalEvent("SR8"); 
  
  // Signal region 9 (EtMiss > 150 (already done)
  //--------------------------
  if (missingET->P4().Et() < 700.) return;
  countCutflowEvent("sr9(EtMiss > 700)");
  countSignalEvent("SR9"); 
  
}

void Atlas_1502_01518::finalize() {
  // Whatever should be done after the run goes here
}       
