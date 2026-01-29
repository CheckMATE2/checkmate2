#include "atlas_2506_15515.h"
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
void Atlas_2506_15515::initialize() {
  setAnalysisName("atlas_2506_15515");          

  setLuminosity(140.0*units::INVFB);      
  bookSignalRegions("SR_low;SR_mid;SR_high");
  setInformation(""
    "# ATLAS EXOT-2018-60\n"
    "# Search for single production of vector-like quarks\n"
    "# Decay mode: Q -> W(l nu) b\n"
    "# sqrt(s) = 13 TeV\n"
    "# Luminosity = 140 fb^-1\n"
   "");


  // Signal regions: split by W pT and lepton channel
  // SR_e/mu_low/mid/high correspond to electron/muon and W pT bins
  bookCutflowRegions(
    "SR_e_low;SR_e_mid;SR_e_high;"
    "SR_mu_low;SR_mu_mid;SR_mu_high"
  );

  // Control regions for background validation
  bookControlRegions(
    "CR_Wjets_e_low;CR_Wjets_e_mid;CR_Wjets_e_high;"
    "CR_Wjets_mu_low;CR_Wjets_mu_mid;CR_Wjets_mu_high;"
    "CR_Wjets_low;CR_Wjets_mid;CR_Wjets_high;"
    "CR_ttbar_e_low;CR_ttbar_e_mid;CR_ttbar_e_high;"
    "CR_ttbar_mu_low;CR_ttbar_mu_mid;CR_ttbar_mu_high"
    "CR_ttbar_low;CR_ttbar_mid;CR_ttbar_high"
  );
}

void Atlas_2506_15515::analyze() {
  missingET->addMuons(muonsCombined);

  // ===============================================
  // BASELINE OBJECT SELECTION (Preselection)
  // ===============================================

  // Electrons: pT > 10 GeV, |eta| < 2.47, loose ID
  std::vector<Electron*> baselineElectrons =
    filterPhaseSpace(electronsTight, 10., -2.47, 2.47, true);

  // Muons: pT > 10 GeV, |eta| < 2.5, combined ID
  std::vector<Muon*> baselineMuons =
    filterPhaseSpace(muonsCombined, 10., -2.5, 2.5);

  // Jets: pT > 20 GeV, |eta| < 4.8
  std::vector<Jet*> baselineJets =
    filterPhaseSpace(jets, 20., -4.5, 4.5);

  // Overlap removal: Remove electrons close to jets
  std::vector<Electron*> selectedElectrons =
    overlapRemoval(baselineElectrons, baselineJets, 0.2);

  // Overlap removal: Remove jets close to electrons
  std::vector<Jet*> cleanedJets =
    overlapRemoval(baselineJets, selectedElectrons, 0.4);

  // Overlap removal: Remove muons close to jets
  std::vector<Muon*> selectedMuons =
    overlapRemoval(baselineMuons, cleanedJets, 0.4);

  // ===============================================
  // SIGNAL OBJECT SELECTION
  // ===============================================

  // Signal electrons: pT > 30 GeV, |eta| < 2.47, tight ID
  // (For simplicity, we filter the baseline with stricter cuts)
  std::vector<Electron*> signalElectrons =
    filterPhaseSpace(selectedElectrons, 27., -2.47, 2.47);

  // Signal muons: pT > 30 GeV, |eta| < 2.5
  std::vector<Muon*> signalMuons =
    filterPhaseSpace(selectedMuons, 27., -2.5, 2.5);

  // Signal jets: pT > 30 GeV, |eta| < 4.8
  std::vector<Jet*> signalJets =
    filterPhaseSpace(cleanedJets, 25., -4.5, 4.5);

  // B-tagged jets (at 85% WP): pT > 30 GeV, |eta| < 2.5
  std::vector<Jet*> bJets =
    filterPhaseSpace(signalJets, 25., -2.5, 2.5);
  std::vector<Jet*> selectedBJets;
  for (auto jet : bJets) {
    if (checkBTag(jet)) {
      selectedBJets.push_back(jet);
    }
  }

  countCutflowEvent("00_no_cuts");

  // ===============================================
  // EVENT SELECTION CUTS
  // ===============================================

  // Exactly 1 lepton (electron or muon)
  int nLeptons = signalElectrons.size() + signalMuons.size();
  if (nLeptons != 1) return;

  countCutflowEvent("01_exactly_1_lepton");

  if (signalJets.size() < 2) return;
  countCutflowEvent("02_>1_jets");

  if (signalJets[0]->PT > 200) return;
  countCutflowEvent("03_>200_leading");

  if (fabs(signalJets[0]->Eta) > 2.5) return;
  countCutflowEvent("04_leadingj_eta");

  if (selectedBJets.size() < 1 or selectedBJets[0] != signalJets[0]) return;
  countCutflowEvent("05_leading_is_b");

  // Missing ET > 50 GeV (W leptonic trigger requirement)
  double met = missingET->P4().Et();
  if (met < 120.) return;

  countCutflowEvent("06_met_120GeV");

  if (fabs(signalJets[0]->P4().DeltaPhi(missingET->P4())) < 2) return;
  countCutflowEvent("07_Dphi>2");

  // ===============================================
  // W BOSON AND VLQ RECONSTRUCTION
  // ===============================================

  // Get lepton 4-vector
  TLorentzVector lepton_p4;
  std::string lepton_type;
  if (signalElectrons.size() == 1) {
    lepton_p4 = signalElectrons[0]->P4();
    lepton_type = "e";
  } else {
    lepton_p4 = signalMuons[0]->P4();
    lepton_type = "mu";
  }

  // check additional hard central jets
  bool central_hard = false;
  if (fabs(signalJets[1]->Eta) < 2.5 and signalJets[1]->PT > 75. and ( signalJets[1]->P4().DeltaR(signalJets[0]->P4()) < 1.2 or signalJets[1]->P4().DeltaR(signalJets[0]->P4()) > 2.7 ))
    central_hard = true;

  double dphi_lepton = fabs(signalJets[0]->P4().DeltaPhi(lepton_p4));

  // W boson: lepton + neutrino (from MET)
  // For simplicity, use MET as neutrino momentum (missing transverse energy)
  TLorentzVector w_p4 = lepton_p4 + missingET->P4();
  double w_pt = w_p4.Pt();
  double w_mass = w_p4.M();

  // Determine W pT bin
  std::string pt_bin = getBinLabel(w_pt);

  // ===============================================
  // VLQ CANDIDATE MASS RECONSTRUCTION
  // ===============================================

  // VLQ = W + b-jet (leading b-jet by pT)
  Jet* leading_bjet = selectedBJets[0];
  TLorentzVector vlq_p4 = w_p4 + leading_bjet->P4();
  double vlq_mass = vlq_p4.M();
  double vlq_pt = vlq_p4.Pt();

  // ===============================================
  // SIGNAL AND CONTROL REGION DEFINITIONS
  // ===============================================

  // SR requirements:
  // - Exactly 1 b-jet (no additional jets required)
  // - Reconstructed W mass close to nominal (60-100 GeV)
  // - VLQ mass in signal region range

  bool is_SR = (signalJets[0]->PT > 350.) && (!central_hard) && (dphi_lepton > 2.5) &&
               (w_mass > 60. && w_mass < 100.) &&
               (vlq_mass > 800.);  // Lower bound for SR

  // CR definitions (for background control):
  // W+jets CR: multiple jets, loose W mass requirement
  bool is_Wjets_CR = (signalJets[0]->PT > 250.) && (dphi_lepton > 2.5) &&
                     (selectedBJets.size() == 1) &&
                     (w_mass > 40. && w_mass < 120.);

  // ttbar CR: multiple b-jets, looser cuts
  bool is_ttbar_CR = (selectedBJets.size() >= 2) &&
                     (vlq_mass > 0.);  // Any mass

  // ===============================================
  // FILL REGIONS
  // ===============================================

   lepton_type = ""; //switch off for now
  std::string region_base = "SR_" + lepton_type + "_" + pt_bin;
  std::string cr_wjets_base = "CR_Wjets_" + lepton_type + "_" + pt_bin;
  std::string cr_ttbar_base = "CR_ttbar_" + lepton_type + "_" + pt_bin;

  if (is_SR) {
    countSignalEvent(region_base);
  } else if (is_Wjets_CR) {
    countControlEvent(cr_wjets_base);
  } else if (is_ttbar_CR) {
    countControlEvent(cr_ttbar_base);
  }
}

void Atlas_2506_15515::finalize() {
  // Final histogram processing if needed
}

// ===============================================
// HELPER METHODS
// ===============================================



std::string Atlas_2506_15515::getBinLabel(double w_pt) {
  if (w_pt < 400.) return "low";
  if (w_pt < 600.) return "mid";
  return "high";
}

double Atlas_2506_15515::getWmass(PxPyPzEVector lepton) {

  double pt2 = (lepton.Px() + missingET->P4().X()) * (lepton.Px() + missingET->P4().X()) + (lepton.Py() + missingET->P4().Y()) * (lepton.Py() + missingET->P4().Y()) -
                missingET->P4().X() * missingET->P4().X() - missingET->P4().Y() * missingET->P4().Y();
  double mW = 80.385;
  double q = mW*mW - lepton.E()*lepton.E()  - pt2 ;
  double a = 4.*(-lepton.E()*lepton.E() + lepton.Pz() * lepton.Pz() );
  double b = 4.*q*lepton.Pz();
  double c = q*q  - 4. * lepton.E() * lepton.E() *( missingET->P4().X() * missingET->P4().X() + missingET->P4().Y() * missingET->P4().Y()) ;

  double delta = b*b - 4. *a *c;
  if (delta > 0. ) {

    double sol = std::min( fabs((-b + sqrt(delta))/(2.*a)), fabs((-b - sqrt(delta))/(2.*a)) );

    cout << "Neutrino momentum: " << missingET->P4().X() << " , " << missingET->P4().Y() << " , " << sol << endl;
    return sol;

  }
  else {
      cout << "No real roots for neutrino momentum; rescaling..." << endl;
      double q1 = mW*mW - lepton.E()*lepton.E() -lepton.Px() * lepton.Px() - lepton.Py() * lepton.Py();
      double q2 = -2. * (lepton.Px() * missingET->P4().X() + lepton.Py() * missingET->P4().Y());
      double b0 = lepton.Pz();
      double c0 = lepton.E() * lepton.E() *( missingET->P4().X() * missingET->P4().X() + missingET->P4().Y() * missingET->P4().Y());
      double anew = 16.*a*c0 - 4. *a *q2 *q2 + 16.*b0*b0*q2*q2;
      double bnew = -8.*a*q1*q2 + 32. *b0*b0 *q1*q2;
      double cnew = -4. * a *q1 + 16.*b0*b0*q1*q1;
      double deltanew = bnew*bnew - 4. *anew *cnew;
      if (deltanew > 0.) {
        std::vector<double> factors;
        factors.push_back((-bnew + sqrt(deltanew))/(2.*a));
        factors.push_back((-bnew - sqrt(deltanew))/(2.*a));
        double qnew = q1 + factors[0]*q2;
        double solution1 = -2. *qnew *b0/a;
        qnew = q1 + factors[1]*q2;
        double solution2 = -2. *qnew *b0/a;

        cout << "Factor: " << factors[0]  << " Neutrino momentum: " << missingET->P4().X() << " , " << missingET->P4().Y() << " , " << solution1 << endl;
        cout << "Factor: " << factors[1]  << " Neutrino momentum: " << missingET->P4().X() << " , " << missingET->P4().Y() << " , " << solution2 << endl;
        return std::min(solution1, solution2);
      }
      else {
        cout << "No rescaling factor found" << endl;
        return -1.;
      }

  }


}
