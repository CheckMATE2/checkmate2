#include "atl_phys_pub_2014_010_sbottom.h"
// AUTHOR: asdf
//  EMAIL: asdf
void Atl_phys_pub_2014_010_sbottom::initialize() {
  setAnalysisName("atl_phys_pub_2014_010_sbottom");   
  setInformation(""
    "# 0 leptons + 2 b-jets + Etmiss\n"
     "# ATLAS projection for the high lumi LHC\n"
  "");       
  setLuminosity(300.0*units::INVFB);      
  bookSignalRegions("SRA300;SRA350;SRA450;SRA550;SRA650;SRA750");
  bookCutflowRegions("0_EventCleanup;1_LeptonVeto;2_ETmiss;3_LeadingJetPT;4_ThirdJetPT;5a_b-tagging_2jets;5b_b-tagging_eta1;5c_b-tagging_btag1;5d_b-tagging_eta2;5e_b-tagging_btag2;6a_DeltaPhiMin;6b_DeltaPhiMin;6c_DeltaPhiMin;7_ETmiss/meff;8_mbb");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atl_phys_pub_2014_010_sbottom::analyze() {
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
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in ChekćkMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  // Event cleaning
  // Overlap removal, etc.
  // Values taken from the 8TeV paper
  
  electronsMedium = filterPhaseSpace(electronsMedium, 7, -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 6, -2.4, 2.4);
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  countCutflowEvent("0_EventCleanup");

  int nJets = jets.size();

  // Lepton Veto:
  // No electrons or leptons that survive the above cuts are allowed

  if (!electronsMedium.empty()) {
	  return;
  }
  if (!muonsCombined.empty()) {
	  return;
  }
  countCutflowEvent("1_LeptonVeto");

  // ETmiss
  // ETmiss must be larger than 150GeV
  if (missingET->P4().Et() <= 150) {
	  return;
  }
  countCutflowEvent("2_ETmiss");

  // Leading jet pt
  // The pt of the leading jet must be higher than 130GeV
  if (nJets < 1) {
	  return;
  }
  if (jets[0]->PT <= 130) {
	  return;
  }
  countCutflowEvent("3_LeadingJetPT");
  
  // Third jet pt
  // A possible third jet must have a pt less than 50 GeV
  if (nJets >= 3) {
	  if (jets[2]->PT >= 50) {
		  return;
	  }
  }
  countCutflowEvent("4_ThirdJetPT");

  // b-tagging
  // The leading two jets must be b-tagged
  if (nJets < 2) {
	  return;
  }
  countCutflowEvent("5a_b-tagging_2jets");
  if (jets[0]->Eta >= 2.5 && jets[0]->Eta <= -2.5) {
	  return;
  }
  countCutflowEvent("5b_b-tagging_eta1");
  if (!checkBTag(jets[0])) {
	  return;
  }
  countCutflowEvent("5c_b-tagging_btag1");
  if (jets[0]->Eta >= 2.5 && jets[0]->Eta <= -2.5) {
	  return;
  }
  countCutflowEvent("5d_b-tagging_eta2");
  if (!checkBTag(jets[1])) {
	  return;
  }
  countCutflowEvent("5e_b-tagging_btag2");


  // DeltaPhiMin
  // The distance between any of the three leading jets has to be larger than 0.4
  TLorentzVector etmiss4 = missingET->P4();
  double deltaPhi = etmiss4.DeltaPhi(jets[0]->P4());
  if (deltaPhi <= 0.4 && deltaPhi >= -0.4) {
	  return;
  }
  countCutflowEvent("6a_DeltaPhiMin");

  deltaPhi = etmiss4.DeltaPhi(jets[1]->P4());
  if (deltaPhi <= 0.4 && deltaPhi >= -0.4) {
	  return;
  }
  countCutflowEvent("6b_DeltaPhiMin");

  if (nJets >= 3) {
	  deltaPhi = etmiss4.DeltaPhi(jets[2]->P4());
	  if (deltaPhi <= 0.4 && deltaPhi >= -0.4) {
		  return;
	  }
  }
  countCutflowEvent("6c_DeltaPhiMin");

  // ETmiss / meff
  // The ratio of ETmiss and the effective mass of the leading two jets must be larger than .25
  double meff = missingET->P4().Et() + jets[0]->PT + jets[1]->PT;
  double etMiss = missingET->P4().Et();
  if (etMiss / meff <= 0.25) {
	  return;
  }
  countCutflowEvent("7_ETmiss/meff");

  // mbb
  // The invariant mass of the two b-jets
  double mbb = (jets[0]->P4() + jets[1]->P4()).M();
  if (mbb <= 200) {
	  return;
  }
  countCutflowEvent("8_mbb");

  TLorentzVector vds;
  // I have no idea if this is correct. This is taken directly from the corresponding 8TeV analysis, atlas_1308_2631.
  double mct_corr = mCTcorr(jets[0]->P4(), jets[1]->P4(), vds, missingET->P4(), 14000.0, 0.0 );
  if(mct_corr < 300) {
	  return;
  }
  countSignalEvent("SRA300");

  if(mct_corr < 350) {
	  return;
  }
  countSignalEvent("SRA350");

  if(mct_corr < 450) {
	  return;
  }
  countSignalEvent("SRA450");

  if(mct_corr < 550) {
	  return;
  }
  countSignalEvent("SRA550");

  if(mct_corr < 650) {
	  return;
  }
  countSignalEvent("SRA650");

  if(mct_corr < 750) {
	  return;
  }
  countSignalEvent("SRA750");

}

void Atl_phys_pub_2014_010_sbottom::finalize() {
  // Whatever should be done after the run goes here
}       
