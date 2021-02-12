#include "atlas_1908_03122.h"
// AUTHOR: Marvin M. Flores, Krzysztof Rolbiecki
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
//    metsig_HT = missingET->P4().Et()/sqrt(HT);
      metsig_HT = calcMETSignificance(signalJets);
  }
  
  
  //Signal Region A
  if (missingET->P4().Et() > 240.) { //SRA1
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
  if (missingET->P4().Et() > 150.) { //SRB1
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
  if (missingET->P4().Et() > 150.) { //SRA1
      countCutflowEvent("SRC_00_trigger");
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
  
}

void Atlas_1908_03122::finalize() {
  // Whatever should be done after the run goes here
}       

// code from ATLAS
void Atlas_1908_03122::rotateXY(TMatrix &mat, TMatrix &mat_new, double phi) {
  double c = cos(phi);
  double s = sin(phi);
  double cc = c*c;
  double ss = s*s;
  double cs = c*s;

  mat_new(0,0) = mat(0,0)*cc + mat(1,1)*ss - cs*(mat(1,0) + mat(0,1));
  mat_new(0,1) = mat(0,1)*cc - mat(1,0)*ss + cs*(mat(0,0) - mat(1,1));
  mat_new(1,0) = mat(1,0)*cc - mat(0,1)*ss + cs*(mat(0,0) - mat(1,1));
  mat_new(1,1) = mat(0,0)*ss + mat(1,1)*cc + cs*(mat(1,0) + mat(0,1));
}

double Atlas_1908_03122::calcMETSignificance(std::vector<Jet*> jets) {

  TMatrix cov_sum(2,2);

  TLorentzVector softVec = missingET->P4();
  
  TMatrix particle_u(2,2),particle_u_rot(2,2);
  for( int i = 0; i < jets.size(); i++ ) {
    softVec += jets[i]->P4();  // soft term is everything not included in hard objects
    //double pt_reso = 0.77*pow(jets[i]->PT, -0.39);
    //double pt_reso = jets[i]->PT < 200 ? 1.85*pow(jets[i]->PT, -0.71) : 0.05;  //cf. https://cds.cern.ch/record/2630948/files/ATLAS-CONF-2018-038.pdf Fig. 3
    double pt_reso = 1.05729 - 0.452141*log(jets[i]->PT) + 0.067873*pow(log(jets[i]->PT),2) - 0.00343522*pow(log(jets[i]->PT),3);
    double phi_reso = jets[i]->PT < 100 ? 1.23*pow(jets[i]->PT, -0.95) : 0.017;
    particle_u(0,0) = pow(pt_reso*jets[i]->PT, 2);
    particle_u(1,1) = pow(phi_reso*jets[i]->PT,2);
    rotateXY(particle_u, particle_u_rot, missingET->P4().DeltaPhi(jets[i]->P4()));
    cov_sum += particle_u_rot;
  }
  
 //add soft term resolution (fixed 10 GeV)
  particle_u(0,0) = 10*10;
  particle_u(1,1) = 10*10;
  rotateXY(particle_u, particle_u_rot, missingET->P4().DeltaPhi(softVec));
  cov_sum+=particle_u_rot;  

  //calculate significance
  double varL = cov_sum(0,0);
  double varT = cov_sum(1,1);
  double covLT = cov_sum(0,1);

  double significance = 0;
  double rho = 0;
  if( varL != 0 ){
    rho = covLT / sqrt( varL * varT ) ;
    if (fabs( rho ) >= 0.9 ) rho = 0; //too large - ignore it
    significance = missingET->P4().Et()/sqrt((varL*(1-pow(rho,2))));
  }
  return significance;  
    
}
