#include "atlas_1403_5222.h"
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1403_5222::initialize() {
  setAnalysisName("atlas_1403_5222");          
  setInformation(""
    "# stop production at 8 TeV with SFOS leptons consistent with Z\n"
     "# plus b-jets and missing transverse energy\n"
     "# constraints for stop_2 -> Z stop_1 -> Z t neu1 and\n"
     "# GMSB with stop_1 -> t neu1 -> t Z gravitino\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SR2A;SR2B;SR2C;SR3A;SR3B;");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1403_5222::analyze() {
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
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
  
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  electronsMedium = overlapRemoval(electronsMedium, 0.1);
  electronsTight = overlapRemoval(electronsTight, 0.1);  
  
  countCutflowEvent("00");
  
  bool trigger = false;
  double cutSingleTriggerElectronPT = 25.;
  double cutSingleTriggerMuonPT = 25.;
     // Trigger Cuts
  if( electronsMedium.size() > 0 && electronsMedium[0]->PT > cutSingleTriggerElectronPT )
    //  && (double) rand() / (double) RAND_MAX < 0.80 )
    trigger=true;
  else if( muonsCombined.size() > 0 && muonsCombined[0]->PT > cutSingleTriggerMuonPT  )
    // &&   (double) rand() / (double) RAND_MAX < 0.99 ) 
    trigger=true;  
  
  if( !trigger ) return; 
  
  countCutflowEvent("01_trigger");
  
  int bjets = 0;
  std::vector<TLorentzVector> signaljets;
  for (int i = 0; i < jets.size(); i++) {
    if (fabs(jets[i]->Eta) < 2.5 && jets[i]->PT > 30. && checkBTag(jets[i]) ) bjets++;
    if (jets[i]->PT > 30. ) signaljets.push_back(jets[i]->P4()) ;
  }  
  
  double met = missingET->P4().Et();
  
  electronsMedium = filterIsolation(electronsMedium);
  muonsCombined = filterIsolation(muonsCombined);  
  
  bool sr2ee = false;
  bool sr2mumu = false;
  bool sr3 = false;
  std::vector<TLorentzVector> leptons;
  
  if ( electronsMedium.size() == 2 && electronsMedium[0]->Charge*electronsMedium[1]->Charge < 0 && muonsCombined.size() == 0 && fabs(91.2 - (electronsMedium[0]->P4() + electronsMedium[1]->P4()).M()) < 10. ) {
    sr2ee = true;  
    leptons.push_back(electronsMedium[0]->P4());
    leptons.push_back(electronsMedium[1]->P4());
    double ptll = (leptons[0] + leptons[1]).Perp();
    double dphill = fabs(leptons[0].DeltaPhi(leptons[1]));
    countCutflowEvent("ee_02_SFOS");
    
    if (bjets > 0) countCutflowEvent("ee_03_bjet"); else return;
    
    if (leptons[0].Perp() > 25. ) countCutflowEvent("ee_04_lep>25"); else return;
    
    if (signaljets.size() > 2 ) countCutflowEvent("ee_05_jets=>3"); else return;
    
    if (met > 160. ) countCutflowEvent("ee_06_met>160"); else return;
    
    if (ptll > 80.)  countCutflowEvent("ee_07_ptll>80"); else return;
    
    if (dphill < 1.5)  countCutflowEvent("ee_08_dphill<1.5"); else return;
    
    if ( signaljets.size() < 5 && fabs(91.2 - (electronsMedium[0]->P4() + electronsMedium[1]->P4()).M()) < 5. ) {
      countCutflowEvent("ee_09_SR2A");
      countSignalEvent("SR2A");
    }

    if ( signaljets.size() < 5 && met > 200. && ptll > 160. ) {
      countCutflowEvent("ee_09_SR2B");
      countSignalEvent("SR2B");
    }    
    
    if ( signaljets.size() > 4 && fabs(91.2 - (electronsMedium[0]->P4() + electronsMedium[1]->P4()).M()) < 5. ) {
      countCutflowEvent("ee_09_SR2C");
      countSignalEvent("SR2C");
    }    
  }
  
  if ( muonsCombined.size() == 2 && muonsCombined[0]->Charge*muonsCombined[1]->Charge < 0 &&  electronsMedium.size() == 0 && fabs(91.2 - (muonsCombined[0]->P4() + muonsCombined[1]->P4()).M()) < 10.) {
    sr2mumu = true;  
    leptons.push_back(muonsCombined[0]->P4());
    leptons.push_back(muonsCombined[1]->P4());
    double ptll = (leptons[0] + leptons[1]).Perp();
    double dphill = fabs(leptons[0].DeltaPhi(leptons[1]));   
    countCutflowEvent("mumu_02_SFOS");
    
    if (bjets > 0) countCutflowEvent("mumu_03_bjet"); else return;
    
    if (leptons[0].Perp() > 25. ) countCutflowEvent("mumu_04_lep>25"); else return;
    
    if (signaljets.size() > 2 ) countCutflowEvent("mumu_05_jets=>3"); else return;
    
    if (met > 160. ) countCutflowEvent("mumu_06_met>160"); else return;
    
    if (ptll > 80.)  countCutflowEvent("mumu_07_ptll>80"); else return;
    
    if (dphill < 1.5)  countCutflowEvent("mumu_08_dphill<1.5"); else return;
    
    if ( signaljets.size() < 5 && fabs(91.2 - (muonsCombined[0]->P4() + muonsCombined[1]->P4()).M()) < 5. ) {
      countCutflowEvent("mumu_09_SR2A");
      countSignalEvent("SR2A");
    }   

    if ( signaljets.size() < 5 && met > 200. && ptll > 160. ) {
      countCutflowEvent("mumu_09_SR2B");
      countSignalEvent("SR2B");
    }       
    
    if ( signaljets.size() > 4 && fabs(91.2 - (muonsCombined[0]->P4() + muonsCombined[1]->P4()).M()) < 5. ) {
      countCutflowEvent("mumu_09_SR2C");
      countSignalEvent("SR2C");
    }    
  
  }
  
  if (electronsMedium.size() + muonsCombined.size() == 3 ){
    
    bool pt1_40 = false;
    bool pt1_60 = false;
    double ptll = 0.;
    double mll = 0.;
    if (electronsMedium.size() == 2 && electronsMedium[0]->Charge*electronsMedium[1]->Charge < 0 ) {
      if ( electronsMedium[0]->PT > 40. || muonsCombined[0]->PT > 40. ) pt1_40 = true;
      if ( electronsMedium[0]->PT > 60. ||  muonsCombined[0]->PT > 60. ) pt1_60 = true;
      ptll = (electronsMedium[0]->P4() + electronsMedium[1]->P4() ).Perp();  
      mll = (electronsMedium[0]->P4() + electronsMedium[1]->P4() ).M();  
    }
    else if (electronsMedium.size() == 3) {
      if ( electronsMedium[0]->PT > 40. ) pt1_40 = true;
      if ( electronsMedium[0]->PT > 60. ) pt1_60 = true;
      int i_temp = 10;
      int j_temp = 10;      
      for (int i = 0; i < electronsMedium.size(); i++)
	for (int j = i + 1; j < electronsMedium.size(); j++)
          if (electronsMedium[i]->Charge*electronsMedium[j]->Charge < 0 ) {
	    double mll_temp = ( electronsMedium[i]->P4() + electronsMedium[j]->P4()).M() ;
	    if (fabs(91.2 - mll_temp) < fabs(91.2 - mll) ) {
	      mll = mll_temp;
	      i_temp = i;
	      j_temp = j;
	    }
	  }
      if ( i_temp > 2) return;  
      ptll = (electronsMedium[i_temp]->P4() + electronsMedium[j_temp]->P4() ).Perp();  
    }
    else if (muonsCombined.size() == 2 && muonsCombined[0]->Charge*muonsCombined[1]->Charge < 0 ) {
      if ( electronsMedium[0]->PT > 40. || muonsCombined[0]->PT > 40. ) pt1_40 = true;
      if ( electronsMedium[0]->PT > 60. ||  muonsCombined[0]->PT > 60. ) pt1_60 = true;
      ptll = (muonsCombined[0]->P4() + muonsCombined[1]->P4() ).Perp();  
      mll = (muonsCombined[0]->P4() + muonsCombined[1]->P4() ).M();  
    } 
    else if (muonsCombined.size() == 3 ) {
      if ( muonsCombined[0]->PT > 40. ) pt1_40 = true;
      if ( muonsCombined[0]->PT > 60. ) pt1_60 = true;      
      int i_temp = 10;
      int j_temp = 10;
      for (int i = 0; i < muonsCombined.size(); i++)
	for (int j = i + 1; j  < muonsCombined.size(); j++)
          if (muonsCombined[i]->Charge*muonsCombined[j]->Charge < 0 ) {
	    double mll_temp = ( muonsCombined[i]->P4() + muonsCombined[j]->P4()).M() ;
	    if ( fabs(91.2 - mll_temp) < fabs(91.2 - mll) ) {
	      mll = mll_temp;
	      i_temp = i;
	      j_temp = j;
	    }
	  }
      if ( i_temp > 2) return;
      ptll = (muonsCombined[i_temp]->P4() + muonsCombined[j_temp]->P4() ).Perp();      
    }
    else return;
    
    countCutflowEvent("3lAll_02_3leptons");
    
    if ( fabs(91.2 - mll) < 10. ) countCutflowEvent("3lAll_03_Zwindow"); else return;
    
    if (bjets > 0) countCutflowEvent("3lAll_04_btag"); else return;
    
    bool sr3b = false;
    bool sr3a = false;
    if ( pt1_60  &&  ptll > 75.) {
      countCutflowEvent("3lSR3B_05_lepreq");
      sr3b = true;
    }
    
    if ( !pt1_40 ) return;
    
    if ( signaljets.size() < 5 ) return;
    
    if ( sr3b && signaljets[0].Perp() > 40. && signaljets[4].Perp() > 40.) countCutflowEvent("3lSR3B_06_jetsel"); else sr3b = false;
    if ( signaljets[0].Perp() > 50. ) {
      countCutflowEvent("3lSR3A_05_jetsel"); 
      sr3a = true;
    }
    
    if ( !sr3a && !sr3b ) return;
    
    if ( met < 60. ) return;
    
    if (sr3a) {
      countCutflowEvent("3lSR3A_06_met");
      countSignalEvent("SR3A");
    }
    
    if (sr3b) {
      countCutflowEvent("3lSR3B_07_met");
      countSignalEvent("SR3B");
    }    
  }
 
}

void Atlas_1403_5222::finalize() {
  // Whatever should be done after the run goes here
}       
