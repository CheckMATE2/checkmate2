/*
 * AnalysisHandlerATLAS_13TeV.h
 *
 *  Created on: Feb 23, 2015
 *      Author: daschm
 */

#ifndef ANALYSISHANDLERATLAS_13TEV_H_
#define ANALYSISHANDLERATLAS_13TEV_H_

#include "AnalysisHandler.h"

#include "atlas_1602_09058.h"
#include "atlas_1604_07773.h"
#include "atlas_1604_01306.h"
#include "atlas_1605_03814.h"
#include "atlas_1605_04285.h"
#include "atlas_1605_09318.h"
#include "atlas_1606_03903.h"
#include "atlas_1609_01599.h"
#include "atlas_1704_03848.h"
#include "atlas_1709_04183.h"
#include "atlas_conf_2015_082.h"
#include "atlas_conf_2016_013.h"
#include "atlas_conf_2016_050.h"
#include "atlas_conf_2016_054.h"
#include "atlas_conf_2016_076.h"
#include "atlas_conf_2016_078.h"
#include "atlas_conf_2016_096.h"
#include "atlas_1712_08119.h"
#include "atlas_1712_02332.h"
#include "atlas_1802_03158.h"
#include "atlas_1708_07875.h"
#include "atlas_conf_2016_066.h"
#include "atlas_conf_2017_060.h"
#include "atlas_1706_03731.h"

#include "atlas_1908_08215.h"
#include "atlas_1909_08457.h"
#include "atlas_conf_2019_040.h"
#include "atlas_conf_2019_020.h"
#include "atlas_1803_02762.h"
#include "atlas_conf_2018_041.h"
#include "atlas_2101_01629.h"
//@extraheader@
#include "AnalysisBase.h"

#include "Global.h"

class AnalysisHandlerATLAS_13TeV : public AnalysisHandler {
   //! Function pointer for 2dim tagging efficiencies
   typedef double (*Eff_Fun_Ptr2)(double x,
                                  double y);
   //! Function pointer for 3dim tagging efficiencies
   typedef double (*Eff_Fun_Ptr3)(double x,
                                  double y,
                                  double z);
public:
    //! Standard Constructor
    AnalysisHandlerATLAS_13TeV();

    //! Standard Destructor
    ~AnalysisHandlerATLAS_13TeV();

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

    //! ATLAS electron identification efficiency, loose
    static double electronIDEffLoose(double pt,
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

#endif /* ANALYSISHANDLERATLAS_13TEV_H_ */
