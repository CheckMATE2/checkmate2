/*
 * AnalysisHandlerATLAS_7TeV.h
 *
 *  Created on: Feb 23, 2015
 *      Author: daschm
 */

#ifndef ANALYSISHANDLERATLAS_7TEV_H_
#define ANALYSISHANDLERATLAS_7TEV_H_

#include "AnalysisHandler.h"

#include "atlas_1210_2979.h"
//@extraheader@
#include "AnalysisBase.h"

#include "Global.h"

class AnalysisHandlerATLAS_7TeV : public AnalysisHandler {
   //! Function pointer for 2dim tagging efficiencies
   typedef double (*Eff_Fun_Ptr2)(double x,
                                  double y);
   //! Function pointer for 3dim tagging efficiencies
   typedef double (*Eff_Fun_Ptr3)(double x,
                                  double y,
                                  double z);
public:
    //! Standard Constructor
    AnalysisHandlerATLAS_7TeV();

    //! Standard Destructor
    ~AnalysisHandlerATLAS_7TeV();

protected:
    //! ATLAS specific initialisation (currently empty)
    void initialize();
    //! ATLAS specific finalisation (currently empty)
    void finalize();

    //! booking of ATLAS analyses
    void bookAnalysis(std::string analysisName,
                      Tag_Map whichTags,
                      Param_Map eventParameters);

    //! Applies ATLAS reconstructions on electrons, muons, photons, jets
    void postProcessParticles();

    //! links ATLAS specific particle vectors to ATLAS analyses
    void linkObjects();

private:
    //! Apply ATLAS photon identification efficiencies
    void identifyPhotons();
    //! Apply ATLAS electron identification efficiencies
    void identifyElectrons();
    //! Apply ATLAS muon identification efficiencies
    void identifyMuons();
    //! Apply ATLAS b tagging efficiencies
    void tagBJets();
    //! Apply ATLAS tau tagging efficiencies
    void tagTauJets();

    //! ATLAS photon identification efficiency, medium
    static double photonEffMedium(double pt,
                                  double eta);

    //! ATLAS electron reconstruction efficiency
    static double electronRecEff(double pt,
                                 double eta);

    //! ATLAS electron identification efficiency, medium
    static double electronIDEffMedium(double pt,
                                    double eta);
    //! ATLAS electron identification efficiency, tight / medium
    static double electronIDEffTightOverMedium(double pt,
                                             double eta);

    //! ATLAS muon identification efficiency, 'combined plus SA'
    static double muonEffCombPlus(double phi,
                                  double eta);
    //! ATLAS muon identification efficiency, 'combined' / 'combined plus SA'
    static double muonEffCombOverCombPlus(double phi,
                                          double eta);

    //! ATLAS tautagging signal efficiency, loose, 1 prong
    static double tauSigEffSingleLoose(double pt,
                                       double eta);
    //! ATLAS tautagging signal efficiency, medium, 1 prong
    static double tauSigEffSingleMedium(double pt,
                                        double eta);
    //! ATLAS tautagging signal efficiency, tight, 1 prong
    static double tauSigEffSingleTight(double pt,
                                       double eta);
    //! ATLAS tautagging signal efficiency, loose, >1 prong
    static double tauSigEffMultiLoose(double pt,
                                      double eta);
    //! ATLAS tautagging signal efficiency, medium, >1 prong
    static double tauSigEffMultiMedium(double pt,
                                       double eta);
    //! ATLAS tautagging signal efficiency, tight, >1 prong
    static double tauSigEffMultiTight(double pt,
                                      double eta);
    //! ATLAS tautagging background efficiency, loose, 1 prong
    static double tauBkgEffSingleLoose(double pt,
                                       double eta);
    //! ATLAS tautagging background efficiency, medium, 1 prong
    static double tauBkgEffSingleMedium(double pt,
                                        double eta);
    //! ATLAS tautagging background efficiency, tight, 1 prong
    static double tauBkgEffSingleTight(double pt,
                                       double eta);
    //! ATLAS tautagging background efficiency, loose, >1 prong
    static double tauBkgEffMultiLoose(double pt,
                                      double eta);
    //! ATLAS tautagging background efficiency, medium, >1 prong
    static double tauBkgEffMultiMedium(double pt,
                                       double eta);
    //! ATLAS tautagging background efficiency, tight, >1 prong
    static double tauBkgEffMultiTight(double pt,
                                      double eta);

    //! ATLAS btagging signal efficiency (eff in percent!)
    static double bSigEff(double pt,
                          double eta,
                          double eff);
    //! ATLAS btagging background c jet efficiency
    static double bBkgCJetEff(double pt,
                              double eta,
                              double eff);
    //! ATLAS btagging background light jet efficiency
    static double bBkgLJetEff(double pt,
                              double eta,
                              double eff);
    //! ATLAS btagging background c jet rejection
    static double bBkgCJetRej(double eff);
    //! ATLAS btagging background light jet efficiency
    static double bBkgLJetRej(double eff);

    //! DR(jet, track) for tracks to be associated to tau candidate
    static const double DR_TAU_TRACK;
    //! ptmin for tracks to be associated to tau candidate
    static const double PTMIN_TAU_TRACK;
    //! DR(jet, true_tau) for taus to be matched to jet candidate
    static const double DR_TAU_TRUTH;

    //! maximum eta for truth taus to be considered for matching
    static const double ETAMAX_TAU_TRUTH;
    //! ptmin for truth taus to be considered for matching
    static const double PTMIN_TAU_TRUTH;

    //! DR(jet, b) for b quarks to be associated to jet candidate
    static const double DR_B_TRUTH;
    //! maximum eta for truth b quarks to be considered for matching
    static const double ETAMAX_B_TRUTH;
    //! ptmin for truth bs to be considered for matching
    static const double PTMIN_B_TRUTH;

    //! ATLAS muon detector map, detector types
    enum muonDetector {cscsm, cscla, bee, endsm, endla, trans, feet, barov, barsm, barla};
    //! ATLAS muon detector map, type for each eta and phi position
    static const muonDetector detMap[53][21];
    //! ATLAS muon detector map, eta positions in array
    static const double etaProj[21];
    //! ATLAS muon detector map, phi positions in array
    static const double phiProj[53];

    //! list of loose reconstructed ATLAS electrons
    std::vector<Electron*> electronsLoose;
    //! list of medium reconstructed ATLAS electrons
    std::vector<Electron*> electronsMedium;
    //! list of tight reconstructed ATLAS electrons
    std::vector<Electron*> electronsTight;
    //! list of loose reconstructed ATLAS muons
    std::vector<Muon*> muonsLoose;
    //! list of 'combined' reconstructed ATLAS muons
    std::vector<Muon*> muonsCombined;
    //! list of 'combined + SA' reconstructed ATLAS muons
    std::vector<Muon*> muonsCombinedPlus;
    //! list of 'loose' reconstructed ATLAS photons
    std::vector<Photon*> photonsLoose;
    //! list of 'medium' reconstructed ATLAS photons
    std::vector<Photon*> photonsMedium;
};

#endif /* ANALYSISHANDLERATLAS_7TEV_H_ */
