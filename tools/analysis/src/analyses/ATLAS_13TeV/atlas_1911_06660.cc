#include "atlas_1911_06660.h"
// AUTHOR: JSK
//  EMAIL: jsk@th.physik.uni-bonn.de
void Atlas_1911_06660::initialize() {
  setAnalysisName("atlas_1911_06660");          
  setInformation(""
    "# search for direct stau production\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("SRlowMass;SRhighMass");
  bookControlRegions("QCR1cut_cuts;QCR2cut_cuts");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1911_06660::analyze() {
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

  electronsTight = filterPhaseSpace(electronsTight, 15., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.7, 2.7);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);

  std::vector<Jet*> looseTauJets, mediumTauJets, tightTauJets;
  for(int i=0; i<jets.size(); i++) {
    if(fabs(jets[i]->Eta) < 2.5 and checkTauTag(jets[i], "loose") and fabs(jets[i]->Charge)==1) {
      looseTauJets.push_back(jets[i]);
    }    
    if(fabs(jets[i]->Eta) < 2.5 and checkTauTag(jets[i], "medium") and fabs(jets[i]->Charge)==1) {
      mediumTauJets.push_back(jets[i]);
    }
    if(fabs(jets[i]->Eta) < 2.5 and checkTauTag(jets[i], "tight") and fabs(jets[i]->Charge)==1) {
      tightTauJets.push_back(jets[i]);
    }

  }

  tightTauJets=filterPhaseSpace(tightTauJets, 20., -2.47, 2.47, true);//tau jet

  electronsTight = filterIsolation(electronsTight);
  muonsCombined = filterIsolation(muonsCombined);
  
  //  photons = overlapRemoval(photons, electronsMedium, 0.01);
  jets = overlapRemoval(jets, photons, 0.4);
  
  electronsTight = overlapRemoval(electronsTight, electronsTight, 0.05);
  mediumTauJets = overlapRemoval(mediumTauJets, electronsTight, 0.2);
  tightTauJets = overlapRemoval(tightTauJets, electronsTight, 0.2);
  mediumTauJets = overlapRemoval(mediumTauJets, muonsCombined, 0.2);
  tightTauJets = overlapRemoval(tightTauJets, muonsCombined, 0.2);
  electronsTight = overlapRemoval(electronsTight, muonsCombined, 0.01);
  jets = overlapRemoval(jets, electronsTight, 0.2); 
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  jets = overlapRemoval(jets, muonsCombined, 0.2);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  jets = overlapRemoval(jets, looseTauJets, 0.2);
  jets = overlapRemoval(jets, mediumTauJets, 0.2);
  jets = overlapRemoval(jets, tightTauJets, 0.2);

  countCutflowEvent("01_overlap removal");
  
  //KR: I made a mess here :o
  std::vector<Jet*> bjets;
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta) < 2.5 && checkBTag(jets[i]) ) bjets.push_back(jets[i]);
    
  double met=missingET->P4().Et();     
    
  if (looseTauJets.size() > 1 and mediumTauJets.size() < 2 and looseTauJets[0]->Charge * looseTauJets[1]->Charge < 0 and electronsTight.size() + muonsCombined.size() == 0 and bjets.size() == 0 and (looseTauJets[0]->P4() + looseTauJets[1]->P4()).M() > 120.) {
    double dphi_CR = fabs(looseTauJets[0]->P4().DeltaPhi(looseTauJets[1]->P4()));
    double dR_CR   = looseTauJets[0]->P4().DeltaR(looseTauJets[1]->P4());
    double trig1_CR = (looseTauJets[0]->PT >50 && looseTauJets[1]->PT >40 && met>150);
    double trig2_CR = (looseTauJets[0]->PT >95 && looseTauJets[1]->PT >60);
    double mt2_CR = mT2(looseTauJets[0]->P4(), looseTauJets[1]->P4(), 0);
    
    if ( trig1_CR and met > 150. and dphi_CR > 0.8 and dR_CR < 3.2 and mt2_CR > 70. ) countControlEvent("QCR1cut_cuts");
    if ( trig2_CR and met < 150. and met > 75. and dphi_CR > 0.8 and dR_CR < 3.2 and mt2_CR > 70. ) countControlEvent("QCR2cut_cuts");    
  }

  
  if(mediumTauJets.size() != 2)
    return;

    countCutflowEvent("02_nrTauJets != 2");

  if(mediumTauJets[0]->Charge == mediumTauJets[1]->Charge)
    return;

  countCutflowEvent("03_OS_taujets");
  
  if(mediumTauJets[0]->PT < 50 && mediumTauJets[1]->PT < 40)
    return;

  countCutflowEvent("04_taujets pt cuts");

  if(electronsTight.size() > 0 || muonsCombined.size() > 0)
    return;

  countCutflowEvent("05_lepton veto");

  if(bjets.size() != 0)
    return;

  countCutflowEvent("06_bjets veto");
  
  if( (mediumTauJets[0]->P4()+mediumTauJets[1]->P4()).M() < 120)
    return;

  countCutflowEvent("07_tau pair invariant mass cut");

  float dphi = fabs(mediumTauJets[0]->P4().DeltaPhi(mediumTauJets[1]->P4()));
  float dR   = mediumTauJets[0]->P4().DeltaR(mediumTauJets[1]->P4());
  bool trig1 = (mediumTauJets[0]->PT >50 && mediumTauJets[1]->PT >40 && met>150);
  bool trig2 = (mediumTauJets[0]->PT >95 && mediumTauJets[1]->PT >60);

  float mt2;

  bool alternativeMT2 = false;
  
  if(alternativeMT2)
    {
      double pa[3] = { 0. , mediumTauJets[0]->P4().Px(), mediumTauJets[0]->P4().Py() };
      double pb[3] = { 0. , mediumTauJets[1]->P4().Px(), mediumTauJets[1]->P4().Py() };
      double pmiss[3] = {0.,missingET->P4().Px(), missingET->P4().Py() };
      mt2_bisect::mt2 mt2_event;
      mt2_event.set_momenta( pa, pb, pmiss );
      mt2_event.set_mn( 0. );
      mt2 = mt2_event.get_mt2();
    }
  else
    {
      //  float mt2 = mT2(mediumTauJets[0]->P4(), mediumTauJets[1]->P4(), 0, missingET->P4());
      mt2 = mT2(mediumTauJets[0]->P4(), mediumTauJets[1]->P4(), 0);
    }   
  

  const bool switchCutflow1 = false;
  const bool switchCutflow2 = false;
  
  if(switchCutflow1)
    {
      if(! (trig1))
	return;

      countCutflowEvent("08a_trig1");
  
      if(met < 150)
	return;

      countCutflowEvent("09a_met");
  
      if(tightTauJets.size() <1)
	return;

      countCutflowEvent("10a_tightaujet >= 1");
  
      if(dphi < 0.8)
	return;

      countCutflowEvent("11a_phi cut");

      if(dR>3.2)
	return;

      countCutflowEvent("12a_dR");
  
      if(mt2<70)
	return;

      countCutflowEvent("13a_mT2");
      countSignalEvent("SRhighMass");
    }
  //  --------------------------------------------------------

  if(switchCutflow2)
    {    
      if(!(trig2))
	return;

      countCutflowEvent("08b_trig2");
  
      if(met > 150 || met < 75)
	return;

      countCutflowEvent("09b_met");
  
      if(tightTauJets.size() != 2)
	return;

      countCutflowEvent("10b_tightaujets = 2");
  
      if(dphi < 0.8)
	return;

      countCutflowEvent("11b_phi cut");

      if(dR>3.2)
	return;

      countCutflowEvent("12b_dR");
  
      if(mt2<70)
	return;

      countCutflowEvent("13b_mT2");
      countSignalEvent("SRlowMass");
    }
  if(!switchCutflow2 && !switchCutflow2)
    {
      if ( trig1 && met>150 && tightTauJets.size()>=1 && dphi>0.8 && dR<3.2 && mt2>70 ) countSignalEvent("SRhighMass");
      if ( trig2 && met<150 && met>75 && tightTauJets.size()==2 && dphi>0.8 && dR<3.2 && mt2>70 ) countSignalEvent("SRlowMass");
    }

}

void Atlas_1911_06660::finalize() {
  // Whatever should be done after the run goes here
}       
