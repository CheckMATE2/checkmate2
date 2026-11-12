#include "atlas_2404_01996.h"
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
//
// ATLAS SUSY-2020-17, arXiv:2404.01996, Phys. Lett. B 856 (2024) 138938
// Search for pair-produced higgsinos decaying via Higgs or Z bosons to final
// states containing a pair of photons and a pair of b-jets (hh/hZ -> yybb + MET).
// Implementation follows the paper and the SimpleAnalysis code (yybb) from HEPData.

void Atlas_2404_01996::initialize() {
  setAnalysisName("atlas_2404_01996");
  setInformation(""
    "# higgsinos in GMSB, h(yy)h/Z(bb) + MET\n"
  "");
  setLuminosity(139.0*units::INVFB);
  bookSignalRegions("SR1h;SR1Z;SR2");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_2404_01996::analyze() {

  //missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  // --- Baseline objects ---
  // Loose photons (used in overlap removal) and signal photons ('Tight' + FixedCutTight isolation)
  std::vector<Photon*> photonsBase = filterPhaseSpace(photonsLoose, 25., -2.37, 2.37, true);
  std::vector<Photon*> photonsSig = filterPhaseSpace(photonsMedium, 25., -2.37, 2.37, true);
  photonsSig = filterIsolation(photonsSig);

  // Electrons: LooseAndBLayer, pT > 4.5 GeV, |eta| < 2.47; Muons: Medium, pT > 4 GeV, |eta| < 2.7
  std::vector<Electron*> electronsBase = filterPhaseSpace(electronsLoose, 4.5, -2.47, 2.47);
  std::vector<Muon*> muonsBase = filterPhaseSpace(muonsCombined, 4., -2.7, 2.7);
  jets = filterPhaseSpace(jets, 30., -2.8, 2.8);

  // --- Overlap removal ---
  // e, mu and jets within dR = 0.4 of a loose photon are removed
  electronsBase = overlapRemoval(electronsBase, photonsBase, 0.4);
  muonsBase = overlapRemoval(muonsBase, photonsBase, 0.4);
  jets = overlapRemoval(jets, photonsBase, 0.4);
  // e and mu within dR = 0.4 of a jet are removed
  electronsBase = overlapRemoval(electronsBase, jets, 0.4);
  muonsBase = overlapRemoval(muonsBase, jets, 0.4);

  // b-jets: DL1r 77% WP
  std::vector<Jet*> bjets;
  for (int i = 0; i < jets.size(); i++)
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i]) )
      bjets.push_back(jets[i]);

  countCutflowEvent("00_all");

  // --- Common selection ---
  // Trigger (diphoton) + exactly two signal photons, pT(y1) > 35 GeV, pT(y2) > 25 GeV
  if ( photonsSig.size() != 2 ) return;
  if ( photonsSig[0]->PT < 35. ) return;
  countCutflowEvent("01_2photons");

  if ( electronsBase.size() + muonsBase.size() > 0 ) return;
  countCutflowEvent("02_lepton_veto");

  TLorentzVector pyy = photonsSig[0]->P4() + photonsSig[1]->P4();
  double myy = pyy.M();
  if ( myy < 95. or myy > 160. ) return;
  countCutflowEvent("03_95<myy<160");

  if ( photonsSig[1]->PT/myy < 0.2 ) return;
  countCutflowEvent("04_pTy2/myy>0.2");

  if ( jets.size() < 2 ) return;
  countCutflowEvent("05_nj>=2");

  // --- Signal regions ---
  // Exactly two b-tagged jets are required; their invariant mass defines mbb.
  // In the published cutflow this requirement enters together with the mbb cut.
  double mbb = -1.;
  if ( bjets.size() == 2 ) mbb = (bjets[0]->P4() + bjets[1]->P4()).M();

  double met = missingET->P4().Et();
  double ptyy = pyy.Pt();
  double pty2myy = photonsSig[1]->PT/myy;
  bool myyH = fabs(myy - 125.) < 5.;

  if ( met <= 100. ) {
    // SR1h
    countCutflowEvent("SR1h_01_MET<=100");
    if ( myyH ) {
      countCutflowEvent("SR1h_02_|myy-125|<5");
      if ( mbb > 100. and mbb < 140. ) {
        countCutflowEvent("SR1h_03_100<mbb<140");
        if ( pty2myy > 0.4 ) {
          countCutflowEvent("SR1h_04_pTy2/myy>0.4");
          if ( ptyy > 90. ) {
            countCutflowEvent("SR1h_05_pTyy>90");
            countSignalEvent("SR1h");
          }
        }
      }
    }
    // SR1Z
    countCutflowEvent("SR1Z_01_MET<=100");
    if ( myyH ) {
      countCutflowEvent("SR1Z_02_|myy-125|<5");
      if ( mbb > 60. and mbb <= 100. ) {
        countCutflowEvent("SR1Z_03_60<mbb<100");
        if ( pty2myy > 0.4 ) {
          countCutflowEvent("SR1Z_04_pTy2/myy>0.4");
          if ( ptyy > 90. ) {
            countCutflowEvent("SR1Z_05_pTyy>90");
            countSignalEvent("SR1Z");
          }
        }
      }
    }
  }
  else {
    // SR2
    countCutflowEvent("SR2_01_MET>100");
    if ( myyH ) {
      countCutflowEvent("SR2_02_|myy-125|<5");
      if ( mbb > 35. and mbb < 145. ) {
        countCutflowEvent("SR2_03_35<mbb<145");
        countSignalEvent("SR2");
      }
    }
  }

  return;
}

void Atlas_2404_01996::finalize() {
  // Whatever should be done after the run goes here
}

