#include "atlas_1605_09318.h"

// AUTHOR: Torsten Weber
//  EMAIL: tweber@physik.rwth-aachen.de


template<class T> std::vector<T*> isolateLeptons(std::vector<T*> leptons);

void Atlas_1605_09318::initialize() {
    setAnalysisName("atlas_1605_09318");
    setInformation(""
        "# Search for pair-production of gluinos decaying via stop and\n"
        "# sbottom in events with b-jets and large missing transverse\n"
        "# momentum in sqrt(s) = 13 TeV p p collisions with the ATLAS detector\n"
        "");
    setLuminosity(3.3*units::INVFB);
    bookSignalRegions("SR-Gbb-A;SR-Gbb-B;SR-Gbb-C;SR-Gtt-0L-A;SR-Gtt-0L-B;SR-Gtt-0L-C;SR-Gtt-1L-A;SR-Gtt-1L-B;");
}

void Atlas_1605_09318::analyze() {

    missingET->addMuons(muonsCombined);

    //////////////////////////////////////////////
    //                                          //
    //      PHYSICS OBJECTS RECONSTRUCTION      //
    //                                          //
    //////////////////////////////////////////////


    /////////////////////////
    //     candidates      //
    /////////////////////////

    // electrons
    electronsLoose = filterPhaseSpace(electronsLoose, 20, -2.47, 2.47);

    // muons
    muonsCombined = filterPhaseSpace(muonsCombined, 20, -2.5, 2.5);

    // jets
    jets = filterPhaseSpace(jets, 20, -2.8, 2.8);
    bjets = filterBJets(jets);


    ///////////////////////////
    //     re-clustering     //
    ///////////////////////////

    recluster_top_jets();


    /////////////////////////////
    //     overlap removal     //
    /////////////////////////////

    // electrons + jets
    overlapRemoval_ej();

    // muons + jets
    overlapRemoval_jm();

    // electrons + muons
    overlapRemoval_em();


    //////////////////////////////
    //     signal particles     //
    //////////////////////////////

    // For each surviving electron see if it passes _tight_ quality criteria
    std::vector<Electron*> tmp(std::min(electronsLoose.size(), electronsTight.size()));
    std::vector<Electron*>::iterator it = std::set_intersection(electronsLoose.begin(), electronsLoose.end(), electronsTight.begin(), electronsTight.end(), tmp.begin());
    tmp.resize(it-tmp.begin());
    electronsTight = tmp;

    jets = filterPhaseSpace(jets, 30);


    ///////////////////////
    //     isolation     //
    ///////////////////////

    electronsTight = isolateLeptons(electronsTight);
    muonsCombined = isolateLeptons(muonsCombined);

    // Cuts that are identical for all signal regions
    if (!preselection()) {
        return;
    }

    // Count events in signal regions
    sr_gbb();
    sr_gtt_0l();
    sr_gtt_1l();

}

void Atlas_1605_09318::finalize() {
  // Whatever should be done after the run goes here
}


    //////////////////////////////////////////
    //     Overlap removal electron-jet     //
    //////////////////////////////////////////

// The sortByPT function in the AnalysisBase takes FinalStateObjects, so I would need to convert
// my Jets to those things, which is a huge mess.
bool sortJetsByPT(Jet* a, Jet* b) {
    return a->PT > b->PT;
}

void Atlas_1605_09318::overlapRemoval_ej() {
    // Get all jets that have not been b-tagged
    std::vector<Jet*> non_btagged;
    for (std::vector<Jet*>::const_iterator it=jets.begin(); it != jets.end(); it++) {
        if (! checkBTag(*it)) {
            non_btagged.push_back(*it);
        }
    }
    // Remove overlap between non-tagged jets and electrons
    overlapRemoval(non_btagged, electronsLoose, 0.2);

    // Put b-jets and non-b-jets back together
    std::vector<Jet*>::iterator it = std::merge(non_btagged.begin(), non_btagged.end(), bjets.begin(), bjets.end(), jets.begin(), sortJetsByPT);
    jets.resize(it-jets.begin());

    // Remove overlap between surviving jets and electrons
    overlapRemoval(electronsLoose, jets, 0.4);
}

    //////////////////////////////////////
    //     Overlap removal jet-muon     //
    //////////////////////////////////////

void Atlas_1605_09318::overlapRemoval_jm() {
    // NOTE: Efficiency?
    std::vector<Muon*> surviving_muons;
    std::vector<bool> jet_overlaps(jets.size(), false);
    for (std::vector<Muon*>::const_iterator itm=muonsCombined.begin(); itm!=muonsCombined.end(); itm++) {
        bool muon_overlap = false;
        // NOTE: It's not clear from the paper if this should be the muon-pT or the jet-pT.
        double minDR = std::min(0.4, 0.04+10/((*itm)->PT));
        for (int j=0; j<jets.size(); j++) {
            if (fabs((*itm)->P4().DeltaR(jets[j]->P4())) >= minDR) {
                continue;
            }
            if (Atlas_1605_09318::countMatchingTracks(tracks, jets[j]) >= 3) {
                // remove muon
                muon_overlap = true;
            } else {
                // remove jet
                jet_overlaps[j] = true;
            }

        }
        if (!muon_overlap) {
            surviving_muons.push_back((*itm));
        }
    }
    std::vector<Jet*> surviving_jets;
    for (int i=0; i<jet_overlaps.size(); i++) {
        if (!(jet_overlaps[i])) {
            surviving_jets.push_back(jets[i]);
        }
    }
    muonsCombined=surviving_muons;
    jets=surviving_jets;
}

// The original jet finder algorithm used a DeltaR of 0.4, so do that again
int Atlas_1605_09318::countMatchingTracks(std::vector<Track*> tracks, Jet* jet) {
    int ntracks = 0;
    for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
        if (jet->P4().DeltaR((*it)->P4()) < 0.4) {
            ntracks++;
        }
    }
    return ntracks;
}

    ///////////////////////////////////////////
    //     Overlap removal electron-muon     //
    ///////////////////////////////////////////

void Atlas_1605_09318::overlapRemoval_em() {
    // NOTE: The paper was a bit vague about which leptons this applies to,
    //       i.e. which combinations of e+m, e+e, m+m should be used

    // Remove _both_ particles if there is an overlap
    // Therefore the original particles need to be saved, otherwise the other particles they overlap with
    // can not be removed
    // NOTE: This is not particularly efficient, but much simpler and less errorprone
    // than the alternative of doing the overlap removal myself
    // Also, the sizes of the vectors should be small, i.e. O(1)
    std::vector<Electron*> tmp_e = electronsLoose;
    std::vector<Muon*> tmp_m = muonsCombined;
    overlapRemoval(electronsLoose, tmp_m, 0.01);
    overlapRemoval(muonsCombined, tmp_e, 0.01);

    // Can't use `overlapRemoval(electronsLoose, 0.01)` because that only removes the electron with lower pT.
    // `overlapRemoval(electronsLoose, electronsLoose, 0.01) doesn't work because that would remove everything,
    // because each electron obviously overlaps with itself.
    overlapRemovalEqual(electronsLoose, tmp_e, 0.01);
    overlapRemovalEqual(muonsCombined, tmp_m, 0.01);
}


    ///////////////////////
    //     Isolation     //
    ///////////////////////

template<class T>
bool isolateLepton(T* lepton, std::vector<Track*> tracks, double maxPT, double deltaR) {
    double pTsum = 0;
    for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
        if (lepton->Particle == (*it)->Particle) {
            continue;
        }
        if ((*it)->P4().DeltaR(lepton->P4()) > deltaR) {
            continue;
        }
        pTsum += (*it)->PT;
    }
    return pTsum < maxPT;
}

template<class T>
std::vector<T*> Atlas_1605_09318::isolateLeptons(std::vector<T*> leptons) {
    std::vector<T*> isolated;
    for (typename std::vector<T*>::iterator it=leptons.begin(); it!=leptons.end(); it++) {
        double dR = std::max(0.2, (double)(10/((*it)->PT)));
        if (isolateLepton(*it, tracks, 0.05*((*it)->PT), dR)) {
            isolated.push_back(*it);
        }
    }
    return isolated;
}

    //////////////////////////
    //     Reclustering     //
    //////////////////////////

// Perform jet trimming as defined in
// D. Krohn, J. Thaler and L.-T. Wang, Jet Trimming, JHEP 02 (2010) 084,
// arXiv: 0912.1342 [hep-ph]
static fastjet::PseudoJet trim_jet(fastjet::PseudoJet jet, double fcut, double lambdaHard) {
    fastjet::PseudoJet trimmed(0,0,0,0);
    std::vector<fastjet::PseudoJet> constituents = jet.constituents();
    for (std::vector<fastjet::PseudoJet>::iterator it=constituents.begin(); it!=constituents.end(); it++) {
        if (it->pt() < fcut*lambdaHard) {
            continue;
        }
        trimmed += (*it);
    }
    return trimmed;
}

// Perform jet reclustering as defined in
// B. Nachman et al., Jets from Jets: Re-clustering as a tool for large radius jet reconstruction and
// grooming at the LHC, JHEP 02 (2015) 075, arXiv: 1407.2922 [hep-ph].
static std::vector<fastjet::PseudoJet> recluster_jets(std::vector<Jet*> jets, double R, double fcut) {
    std::vector<fastjet::PseudoJet> pjets = Atlas_1605_09318::convertJets(jets);
    fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R);
    fastjet::ClusterSequence cs(pjets, jet_def);
    std::vector<fastjet::PseudoJet> largeJets = fastjet::sorted_by_pt(cs.inclusive_jets());

    std::vector<fastjet::PseudoJet> trimmedJets;
    for (std::vector<fastjet::PseudoJet>::iterator it = largeJets.begin(); it!=largeJets.end(); it++) {
        trimmedJets.push_back(trim_jet(*it, fcut, it->pt()));
    }
    return trimmedJets;
}

void Atlas_1605_09318::recluster_top_jets() {
    std::vector<fastjet::PseudoJet> reclustered_jets = recluster_jets(jets, 1.0, 0.05);
    ntops=0;
    for (std::vector<fastjet::PseudoJet>::iterator it=reclustered_jets.begin(); it!=reclustered_jets.end(); it++) {
        // NOTE: It's not clear from the paper if there is supposed to be a dedicated top tagging algorithm before this,
        //       or if this is the top tagging
        if (it->pt() < 300) {
            continue;
        }
        if (fabs(it->eta()) >= 2.0) {
            continue;
        }
        if (it->m() <= 100) {
            continue;
        }
        ntops++;
    }
}


    //////////////////////////////
    //                          //
    //      SIGNAL REGIONS      //
    //                          //
    //////////////////////////////


    //////////////////////////
    //     Preselection     //
    //////////////////////////

bool Atlas_1605_09318::preselection() {
    countCutflowEvent("0.0_Preselection_nocuts");

    if (jets.size() < 4) {
        return false;
    }
    countCutflowEvent("0.1_Preselection_nJets");

    if (bjets.size() < 3) {
        return false;
    }
    countCutflowEvent("0.2_Preselection_nBJets");

    if (missingET->P4().Et() <= 200) {
        return false;
    }
    countCutflowEvent("0.3_Preselection_etmiss");

    return true;
}

    /////////////////
    //     Gbb     //
    /////////////////

void Atlas_1605_09318::sr_gbb() {
    if (electronsTight.size() + muonsCombined.size() > 0) {
        return;
    }
    countCutflowEvent("1.1_GbbPreselection_leptonveto");

    if (Atlas_1605_09318::delta_phi_4j(jets,missingET) <= 0.4) {
        return;
    }
    countCutflowEvent("1.2_Gbb_Preselection_dphimin");

    sr_gbb_regions("A", 90, 350, 1600);
    sr_gbb_regions("B", 90, 450, 1400);
    sr_gbb_regions("C", 30, 500, 1400);
}

void Atlas_1605_09318::sr_gbb_regions(
        std::string region,
        double minPT,
        double minETmiss,
        double minMeff
        ) {

    if (Atlas_1605_09318::meff_4j(jets, missingET) <= minMeff) {
        return;
    }
    countCutflowEvent("2."+region+".1_Gbb-"+region+"_Meff");

    if (missingET->P4().Et() <= minETmiss) {
        return;
    }
    countCutflowEvent("2."+region+".2_Gbb-"+region+"_ETmiss");

    // jets is sorted, so if jets[3] >= minPT then the four highest jets are above minPT
    if (jets[3]->PT < minPT) {
        return;
    }
    countCutflowEvent("2."+region+".3_Gbb-"+region+"_jet_PT");

    if (bjets[2]->PT < minPT) {
        return;
    }
    countCutflowEvent("2."+region+".4_Gbb-"+region+"_bjet_PT");

    countSignalEvent("SR-Gbb-"+region);

    double etmiss = missingET->P4().Et();
    for (int etmiss_low=minETmiss; etmiss_low<800; etmiss_low+=50) {
        if (etmiss >= etmiss_low && etmiss < etmiss_low+50) {
            std::stringstream str;
            str << "X_SR-Gbb-" << region << "_" << etmiss_low;
            countCutflowEvent(str.str());
        }
    }
    if (etmiss < 800) {
        countCutflowEvent("X_SR-Gbb-"+region+"_compare");
    }
}


    ///////////////////
    //     Gtt 0L    //
    ///////////////////

void Atlas_1605_09318::sr_gtt_0l() {
    if (electronsTight.size() + muonsCombined.size() > 0) {
        return;
    }
    countCutflowEvent("3.1_Gtt-0L-Preselection_lepton_veto");

    if (Atlas_1605_09318::delta_phi_4j(jets, missingET) <= 0.4) {
        return;
    }
    countCutflowEvent("3.2_SR-Gtt-0L-Preselection_deltaPhi");

    if (jets.size() < 8) {
        return;
    }
    countCutflowEvent("3.3_Gtt-0L-Preselection_nJets");

    if (mBjets(bjets, missingET) <= 80) {
        return;
    }
    countCutflowEvent("3.4_Gtt-0L-Preselection_mBjets");

    sr_gtt_0l_regions("A", 400, 1700, 3, 1);
    sr_gtt_0l_regions("B", 350, 1250, 4, 1);
    sr_gtt_0l_regions("C", 350, 1250, 4, 0);
}

void Atlas_1605_09318::sr_gtt_0l_regions(
        std::string region,
        double minETmiss,
        double minMeff,
        int minNBjets,
        int minNtop
        ) {

    if (Atlas_1605_09318::meff_incl(jets, electronsTight, muonsCombined, missingET) <= minMeff) {
        return;
    }
    countCutflowEvent("4."+region+".1_Gtt-0L-"+region+"_Meff");

    if (missingET->P4().Et() <= minETmiss) {
        return;
    }
    countCutflowEvent("4."+region+".2_Gtt-0L-"+region+"_ETmiss");

    if (bjets.size() < minNBjets) {
        return;
    }
    countCutflowEvent("4."+region+".3_Gtt-0L-"+region+"_NBjets");

    if (minNtop > 0) {
        if (ntops < minNtop) {
            return;
        }
        countCutflowEvent("4."+region+".4_Gtt-0L-"+region+"_Ntops");
    }
    countSignalEvent("SR-Gtt-0L-"+region);
    double etmiss = missingET->P4().Et();
    for (int etmiss_low=minETmiss; etmiss_low<800; etmiss_low+=50) {
        if (etmiss >= etmiss_low && etmiss < etmiss_low+50) {
            std::stringstream str;
            str << "X_SR-Gtt-0L-" << region << "_10_" << etmiss_low;
            countCutflowEvent(str.str());
        }
    }
    if (etmiss < 800) {
        countCutflowEvent("X_SR-Gtt-0L-"+region+"_11_compare");
    }
}


    ///////////////////
    //     Gtt 1L    //
    ///////////////////

void Atlas_1605_09318::sr_gtt_1l() {
    if (electronsTight.empty() && muonsCombined.empty()) {
        return;
    }
    countCutflowEvent("5.1_Gtt-1L-Preselection_nLeptons");

    if (jets.size() < 6) {
        return;
    }
    countCutflowEvent("5.2_Gtt-1L-Preselection_nJets");

    if (mBjets(bjets, missingET) <= 160) {
        return;
    }
    countCutflowEvent("5.3_Gtt-1L-Preselection_mBjets");

    TLorentzVector p4 = Atlas_1605_09318::leadingLeptonP4(electronsTight, muonsCombined);
    if (mT(p4, missingET->P4()) <= 150) {
        return;
    }
    countCutflowEvent("5.4_Gtt-1L_mT");

    sr_gtt_1l_regions("A", 200, 1100, 1);
    sr_gtt_1l_regions("B", 300, 900,  0);
}

void Atlas_1605_09318::sr_gtt_1l_regions(
        std::string region,
        double minETmiss,
        double minMeff,
        int minNtop
        ) {
    if (Atlas_1605_09318::meff_incl(jets, electronsTight, muonsCombined, missingET) <= minMeff) {
        return;
    }
    countCutflowEvent("6."+region+".1_Gtt-1L-"+region+"_Meff");

    if (missingET->P4().Et() <= minETmiss) {
        return;
    }
    countCutflowEvent("6."+region+".2_Gtt-1L-"+region+"_ETmiss");

    if (minNtop > 0) {
        if (ntops < minNtop) {
            return;
        }
        countCutflowEvent("6."+region+".3_Gtt-1L-"+region+"_nTops");
    }
    countSignalEvent("SR-Gtt-1L-"+region);
    double etmiss = missingET->P4().Et();
    for (int etmiss_low=minETmiss; etmiss_low<800; etmiss_low+=50) {
        if (etmiss >= etmiss_low && etmiss < etmiss_low+50) {
            std::stringstream str;
            str << "X_SR-Gtt-1L-" << region << "_9_" << etmiss_low;
            countCutflowEvent(str.str());
        }
    }
    if (etmiss < 800) {
        countCutflowEvent("X_SR-Gtt-1L-"+region+"_10_compare");
    }
}


    /////////////////////////////////
    //     Kinematic variables     //
    /////////////////////////////////


// m_eff^incl, defined on page 7
double Atlas_1605_09318::meff_incl(
        std::vector<Jet*> jets,
        std::vector<Electron*> electrons,
        std::vector<Muon*> muons,
        ETMiss* missingET
        ) {
    double meff = 0;
    for (std::vector<Jet*>::const_iterator it = jets.begin(); it!=jets.end(); it++) {
        meff += (*it)->PT;
    }
    for (std::vector<Electron*>::const_iterator it = electrons.begin(); it!=electrons.end(); it++) {
        meff += (*it)->PT;
    }
    for (std::vector<Muon*>::const_iterator it = muons.begin(); it!=muons.end(); it++) {
        meff += (*it)->PT;
    }
    meff += missingET->P4().Et();
    return meff;
}

// m_eff^4j, defined on page 7
double Atlas_1605_09318::meff_4j(std::vector<Jet*> jets, ETMiss* missingET) {
    // Precondition: There are at least four jets
    assert(jets.size() >= 4);
    double meff = missingET->P4().Et();
    for (int i=0; i<4; i++) {
        meff += jets[i]->PT;
    }
    return meff;
}

// m_T,min^b-jets, defined on page 8
double Atlas_1605_09318::mBjets(std::vector<Jet*> bjets, ETMiss* missingET) {
    // Precondition: There are at least three b-jets
    assert(bjets.size() >= 3);
    std::vector<double> mts;
    for (int i=0; i<3; i++) {
        double mt = mT(bjets[i]->P4(), missingET->P4());
        mts.push_back(mt);
    }
    return *std::min_element(mts.begin(), mts.end());
}

// \Delta\phi_min^4j, defined on page 8
double Atlas_1605_09318::delta_phi_4j(std::vector<Jet*> jets, ETMiss* missingET) {
    // Precondition: There are at least four jets
    assert(jets.size() >= 4);
    std::vector<double> deltas;
    for (int i=0; i<4; i++) {
        deltas.push_back(fabs(jets[i]->P4().DeltaPhi(missingET->P4())));
    }
    return *std::min_element(deltas.begin(), deltas.end());
}


TLorentzVector Atlas_1605_09318::leadingLeptonP4(std::vector<Electron*> electrons, std::vector<Muon*> muons) {
    // Precondition: There is at least one lepton
    assert(electrons.size()+muons.size() >= 1);
    if (electrons.empty()) {
        return muons[0]->P4();
    }
    if (muons.empty()) {
        return electrons[0]->P4();
    }
    if (electrons[0]->PT > muons[0]->PT) {
        return electrons[0]->P4();
    } else {
        return muons[0]->P4();
    }
}

std::vector<fastjet::PseudoJet> Atlas_1605_09318::convertJets(std::vector<Jet*> jets) {
    std::vector<fastjet::PseudoJet> converted;
    for (std::vector<Jet*>::iterator it=jets.begin(); it!=jets.end(); it++) {
        TLorentzVector p4 = (*it)->P4();
        converted.push_back(fastjet::PseudoJet(p4.Px(), p4.Py(), p4.Pz(), p4.E()));
    }
    return converted;
}

std::vector<Jet*> Atlas_1605_09318::filterBJets(std::vector<Jet*> jets) {
    std::vector<Jet*> bJets;
    for (std::vector<Jet*>::iterator it=jets.begin(); it!=jets.end(); it++) {
        if (checkBTag(*it)) {
            bJets.push_back(*it);
        }
    }
    return bJets;
}
