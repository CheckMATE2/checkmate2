#include "dilepton_hl.h"
// AUTHOR: Krzysztof Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Dilepton_hl::initialize() {
  setAnalysisName("dilepton_hl");          
  setInformation(""
    "# custom slepton/chargino pair production\n"
     "# ATLAS High Luminosity 3000/fb\n"
     "# di-lepton + MET\n"
  "");
  setLuminosity(3000.0*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookCutflowRegions("ee_dilepton;ee_jet_veto;ee_Z_veto;ee_mT2_200;ee_mT2_250;ee_mT2_270;ee_mT2_300;");
  bookCutflowRegions("mumu_dilepton;mumu_jet_veto;mumu_Z_veto;mumu_mT2_200;mumu_mT2_250;emu_mT2_270;mumu_mT2_300;");
  bookCutflowRegions("emu_dilepton;emu_jet_veto;emu_Z_veto;emu_mT2_200;emu_mT2_250;emu_mT2_270;emu_mT2_300;");  
  bookCutflowRegions("ee_jets;ee_b_veto;ee_Z_window;ee_pTll;ee_Etmiss;ee_DRll;ee_mjj;ee_jet_pT;");
  bookCutflowRegions("mumu_jets;mumu_b_veto;mumu_Z_window;mumu_pTll;mumu_Etmiss;mumu_DRll;mumu_mjj;mumu_jet_pT;");
  bookSignalRegions("mT2_200_SF;mT2_200_DF;mT2_250_SF;mT2_250_DF;mT2_270_SF;mT2_270_DF;mT2_300_SF;mT2_300_DF;Zjets;");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Dilepton_hl::analyze() {
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
  
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 17., -4.5, 4.5);
  
  electronsTight = overlapRemoval(electronsTight, 0.05);
  jets = overlapRemoval(jets, electronsTight, 0.2);
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  
// skipped removing two electrons/muons if DR < 0.05
  
  std::vector<Electron*> isoElecs = filterIsolation(electronsTight);
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);
  
  bool ee = false; bool mumu = false; bool emu = false;
  std::string flavour;
  std::vector<TLorentzVector> leptons;
  if (isoElecs.size() + isoMuons.size() != 2) return;
  if (isoElecs.size() == 2) {
    ee = true;
    flavour = "ee";
    leptons.push_back( isoElecs[0]->P4() );
    leptons.push_back( isoElecs[1]->P4() );
  }
  else if (isoMuons.size() == 2) {
    mumu = true;
    flavour = "mumu";
    leptons.push_back( isoMuons[0]->P4() );
    leptons.push_back( isoMuons[1]->P4() );
  }
  else {
    emu = true;
    flavour = "emu";
    if ( isoElecs[0]->PT > isoMuons[0]->PT ) {
      leptons.push_back( isoElecs[0]->P4() );
      leptons.push_back( isoMuons[0]->P4() );
    }
    else {
      leptons.push_back( isoMuons[0]->P4() );
      leptons.push_back( isoElecs[0]->P4() );
    }
  }
  
  TLorentzVector pll = leptons[0] + leptons[1];
  
  double mll = pll.M();
  if ( leptons[0].Perp() < 35. || leptons[1].Perp() < 20. || mll < 20.) return;
  
  double triggerRatio = (double) rand() / (double) (RAND_MAX + 1.);
  if (ee && triggerRatio > 0.90 ) return;
  if (mumu && triggerRatio > 0.80 ) return;
  if (emu && triggerRatio > 0.75 ) return;
  
  countCutflowEvent(flavour+"_dilepton");
  
  bool btag = false; bool centraljet = false; bool forwardjet = false;
  std::vector<TLorentzVector> centraljets;
  for (int i = 0; i < jets.size(); i++) {
    if ( (fabs(jets[i]->Eta) < 2.4) && (jets[i]->PT > 20.) ){
      if (checkBTag(jets[i]) ) btag = true;
      else {
        centraljet = true;
        centraljets.push_back(jets[i]->P4() );
      }
    }
    else if (jets[i]->PT > 30. ) forwardjet = true;  
  }
  
  double deltaphi_min = 1000.;
  double deltaphi_temp = 10000.;
  double missingETrel;
  
  for ( int i = 0; i < jets.size(); i++ ) {
    if ( (fabs(jets[i]->Eta) < 2.4) &&  (jets[i]->PT > 20.) ) {
      deltaphi_temp = fabs( jets[i]->P4().DeltaPhi(missingET->P4()));
      if ( deltaphi_temp < deltaphi_min )
        deltaphi_min = deltaphi_temp;
    }
  }
  for ( int i = 0; i < isoMuons.size(); i++ ) {
    deltaphi_temp = fabs( isoMuons[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
  }
  for ( int i = 0; i < isoElecs.size(); i++ ) {
    deltaphi_temp = fabs( isoElecs[i]->P4().DeltaPhi(missingET->P4()));
    if ( deltaphi_temp < deltaphi_min )
      deltaphi_min = deltaphi_temp;
  }
  if ( deltaphi_min < 1.57079632679 )
    missingETrel = missingET->P4().Et()*sin(deltaphi_min);
  else
    missingETrel = missingET->P4().Et();
  
  
  if ( !btag && !centraljet && !forwardjet) {
    countCutflowEvent(flavour+"_jet_veto");
    
    if ( !emu && fabs(mll - 91.2) < 10. ) return;
    countCutflowEvent(flavour+"_Z_veto");  
    
    double pa[3] = {leptons[0].M(), leptons[0].Px(), leptons[0].Py() };
    double pb[3] = {leptons[1].M(), leptons[1].Px(), leptons[1].Py() };
    double pmiss[3] = {0.,missingET->P4().Px(), missingET->P4().Py() };
    mt2_bisect::mt2 mt2_event;
    mt2_event.set_momenta( pa, pb, pmiss );
    mt2_event.set_mn( 0. );
    double m_t2 = mt2_event.get_mt2(); 
    
    if ( m_t2 > 200. ) {
      countCutflowEvent(flavour+"_mT2_200");
      if (ee || mumu ) countSignalEvent("mT2_200_SF");
      else countSignalEvent("mT2_200_DF");
    }   
    
    if ( m_t2 > 250. ) {
      countCutflowEvent(flavour+"_mT2_250");
      if (ee || mumu ) countSignalEvent("mT2_250_SF");
      else countSignalEvent("mT2_250_DF");
    }      
    
    if ( m_t2 > 270. ) {
      countCutflowEvent(flavour+"_mT2_270");
      if (ee || mumu ) countSignalEvent("mT2_270_SF");
      else countSignalEvent("mT2_270_DF");
    }   
    
    if ( m_t2 > 300. ) {
      countCutflowEvent(flavour+"_mT2_300");
      if (ee || mumu ) countSignalEvent("mT2_300_SF");
      else countSignalEvent("mT2_300_DF");
    }          
    
  }
  else if ( centraljets.size() > 1 && (ee || mumu) ) {
    countCutflowEvent(flavour+"_jets");
    
    if ( btag || forwardjet ) return;
    countCutflowEvent(flavour+"_b_veto");

    if ( fabs(mll - 91.2) > 10. ) return;
    countCutflowEvent(flavour+"_Z_window");  
    
    if ( pll.Perp() < 80. ) return;
    countCutflowEvent(flavour+"_pTll");
    
    if (missingETrel < 120. ) return;
    countCutflowEvent(flavour+"_Etmiss");
    
    if ( leptons[0].DeltaR(leptons[1]) < 0.3 || leptons[0].DeltaR(leptons[1]) > 1.5 ) return;
    countCutflowEvent(flavour+"_DRll");
    
    double mjj = (centraljets[0] + centraljets[1]).M();
    if ( mjj < 50. || mjj > 100. ) return;
    countCutflowEvent(flavour+"_mjj");
    
    if ( centraljets[1].Perp() < 45. ) return;
    countCutflowEvent(flavour+"_jet_pT");
    
    countSignalEvent("Zjets");
  }
  else return;  
}

void Dilepton_hl::finalize() {
  // Whatever should be done after the run goes here
}       
