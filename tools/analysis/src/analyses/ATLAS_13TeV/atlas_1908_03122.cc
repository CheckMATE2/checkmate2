#include "atlas_1908_03122.h"
// AUTHOR: Marvin M. Flores
//  EMAIL: mflores@nip.upd.edu.ph
void Atlas_1908_03122::initialize() {
  setAnalysisName("atlas_1908_03122");          
  setInformation(""
    "# ATLAS\n"
    "# atlas_1908_03122\n"
    "# 0 leptons, 3 or more b-jets, etmiss\n"
    "# sqrt(s) = 13 TeV\n"
    "# int(L) = 139 fb^-1\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("SRA;SRAL;SRAM;SRAH;SRB;SRC;SRC22;SRC24;SRC26;SRC28");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1908_03122::analyze() {
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
  //electronsLoose = filterPhaseSpace(electronsLoose, 4.5, -2.47, 2.47);
  //muonsCombined = filterPhaseSpace(muonsCombined, 4., -2.5, 2.5);
  //jets = filterPhaseSpace(jets, 30., -2.8, 2.8);
  
  //baseline objects
  //for jet, paper says 4.8 but pseudocode uses 2.8
  //for muons, paper says 2.5 but pseudocode uses 2.7
  std::vector<Jet*> preOR_baselineJets = filterPhaseSpace(jets,20.,-4.8,4.8);
  std::vector<Electron*> preOR_baselineElectrons = filterPhaseSpace(electronsLoose, 4.5, -2.47, 2.47);
  std::vector<Muon*> preOR_baselineMuons = filterPhaseSpace(muonsCombined, 4., -2.5, 2.5);
  
  //overlap removal of baseline objects
  std::vector<Jet*> baselineJets = overlapRemoval(preOR_baselineJets, preOR_baselineElectrons, 0.2);
  std::vector<Electron*> baselineElectrons = overlapRemoval(preOR_baselineElectrons, baselineJets, 0.4);
  std::vector<Muon*> baselineMuons = overlapRemoval(preOR_baselineMuons, baselineJets, 0.4);
  
  //jets with tighter requirements after overlap removal
  std::vector<Jet*> signalJets = filterPhaseSpace(baselineJets, 30., -2.8, 2.8);
  
  //b-tag signal jets
  std::vector<Jet*> prebtagsignalJets = filterPhaseSpace(signalJets, 30., -2.5, 2.5);
  std::vector<Jet*> notBTagSignalJets, BTagSignalJets;
  for(int i=0; i<prebtagsignalJets.size(); i++) {
    if(checkBTag(prebtagsignalJets[i])) {
      BTagSignalJets.push_back(prebtagsignalJets[i]);
    }
    else {
      notBTagSignalJets.push_back(prebtagsignalJets[i]);
    }
  }
  
  //nocuts
  countCutflowEvent("00_no_cuts");
  
  //missing et trigger, the paper uses less than 250
  //if (missingET->P4().Et() < 115.) return;
  //countCutflowEvent("SRA_01_trigger_etmiss");
  
  //0 baseline leptons
  //if (baselineElectrons.size() + baselineMuons.size() != 0) return;
  //countCutflowEvent("SRA_02_N_baseline_leptons");
  //countCutflowEvent("SRB_01_N_baseline_leptons");
  
  //at least 6 signal jets (SRA_03)
  //if (signalJets.size() >= 6) countCutflowEvent("SRA_03_N_signal_jets");
  
  //at least 5 signal jets (SRB_02)
  //if (signalJets.size() >= 5) countCutflowEvent("SRB_02_N_signal_jets");
  
  //at least 4 b-jets
  //if (BTagSignalJets.size() < 4) return;
  //countCutflowEvent("SRA_04_N_bjets");
  //countCutflowEvent("SRB_03_N_bjets");
  
  //et miss greater than 350 GeV
  //if (missingET->P4().Et() <= 350.) return;
  //countCutflowEvent("SRA_05_etmiss");
  //countCutflowEvent("SRB_04_etmiss");
  
  //minimum azimuthal distance between the 4 highest-pT jets & etmiss
  //double deltaphi = 0;
  double deltaphi_min = 99;
  double deltaphi_j1_met = 99;
  for (int i=0; i<=3; i++) {
   if (signalJets.size() >= i+1) {
    double deltaphi = fabs(signalJets[i]->P4().DeltaPhi(missingET->P4()));
    if (deltaphi < deltaphi_min) {
     deltaphi_min = deltaphi;
    }
   }
  }
  //delta phi between ET Miss and j1
  if (signalJets.size() > 0) deltaphi_j1_met = fabs(missingET->P4().DeltaPhi(signalJets[0]->P4()));
  //deltaphi_min > 0.4
  //if (deltaphi_min <= 0.4) return;
  //countCutflowEvent("SRA_06_deltaphi_min");
  //countCutflowEvent("SRB_05_deltaphi_min");
  
  //tau veto (2nd paragraph before Section 5)
  //tau candidate
  //This cut is not present on the pseudocode.
  //Block below is derived from atlas_1709_04183 which have very similar cuts
  bool tauveto = false;
  for (int i = 0; i < notBTagSignalJets.size(); i++) {
    if ( fabs(notBTagSignalJets[i]->Eta) < 2.5 && notBTagSignalJets[i]->NCharged  < 5 && fabs(notBTagSignalJets[i]->P4().DeltaPhi(missingET->P4())) < M_PI/3. ) tauveto = true;
  }
  //countCutflowEvent("SRA_07_tau_veto");
  //countCutflowEvent("SRB_06_tau_veto");
  
  //leading b-tag pT > 200 GeV (SRA_08)
  //if (BTagSignalJets[0]->PT > 200.) countCutflowEvent("SRA_08_leading_btag_pT");
  
  //leading pT
  
  //deltaR bb, maximal angular separation
  double deltaRbb = 0;
  double deltaRbb_max = 0;
  double deltaRbb_max_2nd = 0;
  int imax = 0;
  int jmax = 0;
  int imax_2nd = 0;
  int jmax_2nd = 0;
  for (int i = 0; i < BTagSignalJets.size(); i++) {
   for (int j = 0; j <= i; j++) {
    if (i != j) {
    deltaRbb = BTagSignalJets[i]->P4().DeltaR(BTagSignalJets[j]->P4());
    if (deltaRbb_max == 0) {
     deltaRbb_max = deltaRbb;
    }
    else if (deltaRbb > deltaRbb_max) {
     deltaRbb_max = deltaRbb;
     imax = i;
     jmax = j;
    }
    else if ((deltaRbb > deltaRbb_max_2nd) && (deltaRbb < deltaRbb_max)) {
      deltaRbb_max_2nd = deltaRbb;
      imax_2nd = i;
      jmax_2nd = j;
    }
    }
   }
  }
  //if (deltaRbb_max <= 2.5) return;
  //countCutflowEvent("SRA_09_deltaRbb_max");
  
  //deltaR bb, minimal angular separation
  //remove the two bjets with max angular separation
  std::vector<Jet*> BTagSignalJets_min;
  for (int i=0; i < BTagSignalJets.size(); i++) {
   if (!(i == imax || i == jmax)) {
     BTagSignalJets_min.push_back(BTagSignalJets[i]);
   }
  }
  //find minimum angular separation from remaining bjets
  double deltaRbb_min = 99;
  double deltaRbb2 = 0;
  int imin = 0;
  int jmin = 0;
  for (int i=0; i < BTagSignalJets_min.size(); i++) {
   for (int j=0; j <= i; j++) {
    if (i != j) {
     deltaRbb2 = BTagSignalJets_min[i]->P4().DeltaR(BTagSignalJets_min[j]->P4());
     if (deltaRbb2 < deltaRbb_min) {
      deltaRbb_min = deltaRbb2;
      imin = i;
      jmin = j;
     }
    }
   }
  }
  //if (deltaRbb_min >= 2.5) return;
  //countCutflowEvent("SRA_10_deltaRbb_max_min");
  
  //Higgs candidate, invariant mass of bb_min
  double m_hcand = -99;
  if (BTagSignalJets_min.size() > 0) {
    m_hcand = (BTagSignalJets_min[imin]->P4() + BTagSignalJets_min[jmin]->P4()).M();
  }
  
  //Higgs candidate for SRB
  double m_hcand1 = -99;
  double m_hcand2 = -99;
  if (BTagSignalJets.size() > 0) {
    m_hcand1 = (BTagSignalJets[imax]->P4() + BTagSignalJets[jmax]->P4()).M();
    m_hcand2 = (BTagSignalJets[imax_2nd]->P4() + BTagSignalJets[jmax_2nd]->P4()).M();
  }
  double m_ave = (m_hcand1 + m_hcand2)/2;
  
  //Leading pT not b-tagged for SRB
  bool nonBLead = false;
  if (notBTagSignalJets.size() > 0 && BTagSignalJets.size() > 0) {
    if (notBTagSignalJets[0]->PT > BTagSignalJets[0]->PT) {
      nonBLead = true;
    }
    else nonBLead = false;
  }
  
  //if (m_hcand <= 80.) return;
  //countCutflowEvent("SRA_11_m_hcand");
  
  //effective mass
  double meff = -99;
  double HT = -99;
  if (signalJets.size() > 0) {
    HT = 0;
    for(int i=0; i<signalJets.size(); i++) {
      HT += signalJets[i]->PT;
    }
  }
  meff = HT + missingET->PT;
  
  double metsig_HT =-99;
  //METSig
  if (signalJets.size()>0){
    metsig_HT = missingET->P4().Et()/sqrt(HT);
  }
  
  
  //Signal Region A
  if (missingET->P4().Et() > 115.) { //SRA1
    countCutflowEvent("SRA_01_trigger_etmiss");
    if (baselineElectrons.size() + baselineMuons.size() == 0) { //SRA2
      countCutflowEvent("SRA_02_N_baseline_leptons");
      if (signalJets.size() >= 6) { //SRA3
        countCutflowEvent("SRA_03_N_signal_jets");
        if (BTagSignalJets.size() >= 4) { //SRA4
          countCutflowEvent("SRA_04_N_bjets");
          if (missingET->P4().Et() > 350.) { //SRA5
            countCutflowEvent("SRA_05_etmiss");
            if (deltaphi_min > 0.4) { //SRA6
              countCutflowEvent("SRA_06_deltaphi_min");
              if (!tauveto) { //SRA7
                countCutflowEvent("SRA_07_tau_veto");
                if (BTagSignalJets[0]->PT > 200.) { //SRA8
                  countCutflowEvent("SRA_08_leading_btag_pT");
                  if (deltaRbb_max > 2.5) { //SRA9
                    countCutflowEvent("SRA_09_deltaRbb_max");
                    if (deltaRbb_min < 2.5) { //SRA10
                      countCutflowEvent("SRA_10_deltaRbb_max_min");
                      if (m_hcand > 80.) { //SRA11
                        countCutflowEvent("SRA_11_m_hcand");
                        if (meff > 1000) { //SRA12
                          countCutflowEvent("SRA_12_meff");
                          countSignalEvent("SRA");
                        } //SRA12
                        if (meff >= 1000 && meff <= 1500) { //SRAL12
                          countCutflowEvent("SRA_12_meff_01_SRAL");
                          countSignalEvent("SRAL");
                        } //SRAL12
                        if (meff > 1500 && meff <= 2000) { //SRAM12
                          countCutflowEvent("SRA_12_meff_02_SRAM");
                          countSignalEvent("SRAM");
                        } //SRAM12
                        if (meff > 2000) { //SRAH12
                          countCutflowEvent("SRA_12_meff_03_SRAH");
                          countSignalEvent("SRAH");
                        } //SRAH12
                      } //SRA11
                    } //SRA10
                  } //SRA9
                } //SRA8
              } //SRA7
            } //SRA6
          } //SRA5
        } //SRA4
      } //SRA3
    } //SRA2
  } //SRA1
  
  //Signal Region B
  if (missingET->P4().Et() > 250.) { //SRB1
    countCutflowEvent("SRB_01_trigger_etmiss");
    if (baselineElectrons.size() + baselineMuons.size() == 0) { //SRB2
      countCutflowEvent("SRB_02_N_baseline_leptons");
      if (signalJets.size() >= 5) { //SRB3
        countCutflowEvent("SRB_03_N_signal_jets");
        if (BTagSignalJets.size() >= 4) { //SRB4
          countCutflowEvent("SRB_04_N_bjets");
          if (missingET->P4().Et() > 350.) { //SRB5
            countCutflowEvent("SRB_05_etmiss");
            if (deltaphi_min > 0.4) { //SRB6
              countCutflowEvent("SRB_06_deltaphi_min");
              if (!tauveto) { //SRB7
                countCutflowEvent("SRB_07_tau_veto");
                if (m_ave > 75 && m_ave < 175) { //SRB8
                  countCutflowEvent("SRB_08_m_ave");
                  if (nonBLead == true) { //SRB9
                    countCutflowEvent("SRB_09_leading_jet_not_btagged");
                    if (signalJets[0]->PT > 350.) { //SRB10
                      countCutflowEvent("SRB_10_pT(j1)");
                      if (deltaphi_j1_met > 2.8) { //SRB11
                        countCutflowEvent("SRB_11_deltaphi_j1_MET");
                        if (meff > 1000) { //SRB12
                          countCutflowEvent("SRB_12_meff");
                          countSignalEvent("SRB");
                        } //SRB12
                      } //SRB11
                    } //SRB10
                  } //SRB9
                } //SRB8
              } //SRB7
            } //SRB6
          } //SRB5
        } //SRB4
      } //SRB3
    } //SRB2
  } //SRB1
  
  //Signal Region C
  if (baselineElectrons.size() + baselineMuons.size() == 0) { //SRC1
    countCutflowEvent("SRC_01_N_baseline_leptons");
    if (signalJets.size() >= 4) { //SRC2
      countCutflowEvent("SRC_02_N_signal_jets");
      if (BTagSignalJets.size() >= 3) { //SRC3
        countCutflowEvent("SRC_03_N_bjets");
        if (missingET->P4().Et() > 250.) { //SRC4
          countCutflowEvent("SRC_04_etmiss");
          if (deltaphi_min > 0.4) { //SRC5
            countCutflowEvent("SRC_05_deltaphi_min");
            if (metsig_HT > 22) { //SRC6
              countCutflowEvent("SRC_06_SRC");
              countSignalEvent("SRC");
            } //SRC6
            if (metsig_HT >= 22 && metsig_HT <= 24) { //SRC6-22
              countCutflowEvent("SRC_06_SRC_22");
              countSignalEvent("SRC22");
            } //SRC6-22
            if (metsig_HT > 24 && metsig_HT <= 26) { //SRC6-24
              countCutflowEvent("SRC_06_SRC_24");
              countSignalEvent("SRC24");
            } //SRC6-24
            if (metsig_HT > 26 && metsig_HT <= 28) { //SRC6-26
              countCutflowEvent("SRC_06_SRC_26");
              countSignalEvent("SRC26");
            } //SRC6-26
            if (metsig_HT > 28) { //SRC6-28
              countCutflowEvent("SRC_06_SRC_28");
              countSignalEvent("SRC28");
            } //SRC6-28
          } //SRC5
        } //SRC4
      } //SRC3
    } //SRC2
  } //SRC1
    
  
}

void Atlas_1908_03122::finalize() {
  // Whatever should be done after the run goes here
}       
