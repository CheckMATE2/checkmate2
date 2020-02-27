/*
 * AnalysisHandlerCMS_14TeV_projected.cc
 *
 *  Created on: Feb 23, 2015
 *      Author: daschm
 */

#include "AnalysisHandlerCMS_14TeV_projected.h"

const double AnalysisHandlerCMS_14TeV_projected::DR_TAU_TRACK = 0.2;
const double AnalysisHandlerCMS_14TeV_projected::PTMIN_TAU_TRACK = 1.0;

const double AnalysisHandlerCMS_14TeV_projected::DR_TAU_TRUTH = 0.2;
const double AnalysisHandlerCMS_14TeV_projected::ETAMAX_TAU_TRUTH = 2.5;
const double AnalysisHandlerCMS_14TeV_projected::PTMIN_TAU_TRUTH = 1.0;

const double AnalysisHandlerCMS_14TeV_projected::DR_B_TRUTH = 0.4;
const double AnalysisHandlerCMS_14TeV_projected::ETAMAX_B_TRUTH = 2.5;
const double AnalysisHandlerCMS_14TeV_projected::PTMIN_B_TRUTH = 1.0;


AnalysisHandlerCMS_14TeV_projected::AnalysisHandlerCMS_14TeV_projected() : AnalysisHandler() {
}

AnalysisHandlerCMS_14TeV_projected::~AnalysisHandlerCMS_14TeV_projected() {
}

void AnalysisHandlerCMS_14TeV_projected::initialize() {
}

void AnalysisHandlerCMS_14TeV_projected::finalize() {
}

void AnalysisHandlerCMS_14TeV_projected::bookAnalysis(std::string analysisName,
                                      Tag_Map whichTags,
                                      Param_Map eventParameters) {
    Global::print(name, "Loading Analysis "+analysisName);

    AnalysisBase* a = NULL;
    //@@extracode@@
        Global::abort(name,
                      "Cannot load analysis "+analysisName+
                          " - analysis unknown.");

    a->setup(whichTags, eventParameters);
    listOfAnalyses.push_back(a);
    Global::print(name,
                  "Successfully loaded analysis "+analysisName);
}

void AnalysisHandlerCMS_14TeV_projected::postProcessParticles() {
    // First do all the mother handler processions
    AnalysisHandler::postProcessParticles();

    // Then take care of the rest
    identifyPhotons();
    identifyElectrons();
    identifyMuons();
    tagBJets();
    tagTauJets();
}

void AnalysisHandlerCMS_14TeV_projected::identifyPhotons() {
    double pEffMed = 0;
    Photon* cand = NULL;

    photonsLoose.clear();
    photonsMedium.clear();
    for (int p = 0; p < photons.size(); p++) {
        cand = photons[p];
        // tag 0 is the loose isolation condition
        if (photonIsolationTags[cand][0]) {
            photonsLoose.push_back(cand);
            pEffMed = photonEffMedium(cand->PT, cand->Eta);
            if ( rand()/(RAND_MAX+1.) < pEffMed )
                photonsMedium.push_back(cand);
        }
    }
}

void AnalysisHandlerCMS_14TeV_projected::identifyElectrons() {
    double eEffMed = 0, eEffTigOvMed = 0;
    Electron* cand = NULL;

    electronsLoose.clear();
    electronsMedium.clear();
    electronsTight.clear();
    for (int e = 0; e < electrons.size(); e++) {
        cand = electrons[e];
        // tag 0 is the loose isolation condition
        if (electronIsolationTags[cand][0]) {
            electronsLoose.push_back(cand);
            eEffMed = electronRecEff(cand->PT, cand->Eta) *
                      electronIDEffMedium(cand->PT, cand->Eta);
            if (rand()/(RAND_MAX+1.) <  eEffMed) {
                electronsMedium.push_back(cand);
                eEffTigOvMed = electronIDEffTightOverMedium(cand->PT,
                                                            cand->Eta);
                if (rand()/(RAND_MAX+1.) <  eEffTigOvMed)
                    electronsTight.push_back(cand);
            }
        }
    }
}

void AnalysisHandlerCMS_14TeV_projected::identifyMuons()  {
    double mEffComb = 0, mEffCombPlus = 0;
    Muon* cand = NULL;

    muonsLoose.clear();
    muonsCombined.clear();
    muonsCombinedPlus.clear();
    // Reconstruct Muons
    for (int m = 0; m < muons.size(); m++) {
        cand = muons[m];
        if (muonIsolationTags[muons[m]][0]) {
            muonsLoose.push_back(muons[m]);
            mEffCombPlus = muonEffCombPlus(muons[m]->Phi, muons[m]->Eta);
            if (rand()/((double)RAND_MAX+1) < mEffCombPlus ) {
                muonsCombinedPlus.push_back(muons[m]);
                mEffComb = muonEffCombOverCombPlus(muons[m]->Phi, muons[m]->Eta);
                if (rand()/((double)RAND_MAX+1) < mEffComb ) {
                    muonsCombined.push_back(muons[m]);
                }
            }
        }
    }
}

void AnalysisHandlerCMS_14TeV_projected::tagBJets() {
   Jet* cand = NULL; // The to-be-checked candidate
   // The candidate's probability and its pass-limit to be tagged
   double prob = 0, pass_prob = 0;
   Eff_Fun_Ptr3 eff_function = NULL; // Efficiency function for the candidate
   std::vector<bool> bTags; // The candidate's list of passed bTags

   jetBTags.clear();
   jetTauTags.clear();
   if (listOfJetBTags.empty())
       return; // Don't do anything if no btags are required

   for(int j = 0; j < jets.size(); j++) {
          cand = jets[j];
          prob = rand()/(RAND_MAX+1.);
          eff_function = NULL;
          bTags.clear();

          /* Loop over bs and try to find an overlap.
           * If there is one, use b signal efficiency*/
          for(int b = 0; b < true_b.size(); b++) {
              if(true_b[b]->PT > PTMIN_B_TRUTH &&
                fabs(true_b[b]->Eta) < ETAMAX_B_TRUTH &&
                 true_b[b]->P4().DeltaR(cand->P4()) < DR_B_TRUTH) {
                  eff_function = &AnalysisHandlerCMS_14TeV_projected::bSigEff;
                  break;
              }
          }
          // If no b overlap, test with truth c's and maybe use c-efficiency
          if (eff_function == NULL) {
              for(int c = 0; c < true_c.size(); c++) {
                  if(true_c[c]->PT > PTMIN_B_TRUTH &&
                     fabs(true_c[c]->Eta) < ETAMAX_B_TRUTH &&
                     true_c[c]->P4().DeltaR(cand->P4()) < DR_B_TRUTH) {
                      eff_function = &AnalysisHandlerCMS_14TeV_projected::bBkgCJetEff;
                      break;
                  }
              }
          }
          // If no b and no c overlap, use light jet Rej
          if (eff_function == NULL)
              eff_function = &AnalysisHandlerCMS_14TeV_projected::bBkgLJetEff;

          // Now that we know the right function to use, lets tag
          for(int btag = 0; btag < listOfJetBTags.size(); btag++) {
              pass_prob = (*eff_function)(cand->PT, cand->Eta,
                                                listOfJetBTags[btag]->eff);
              if (fabs(cand->P4().Eta()) < ETAMAX_B_TRUTH && prob < pass_prob)
                      bTags.push_back(true);
              else
                  bTags.push_back(false);
          }
          jetBTags[jets[j]] = bTags;
      }
}

void AnalysisHandlerCMS_14TeV_projected::tagTauJets() {
    Jet* cand = NULL; // currently tested jet candidate
    // pointer to the right efficiency functions
    Eff_Fun_Ptr2 effFunLoose = NULL, effFunMedium = NULL, effFunTight = NULL;
    double prob = 0, pass_prob = 0;
    int prongs = 0;

    std::vector<bool> tauTags, stdTags;
    // These are the standard values for all candidates
    stdTags.push_back(false); //loose
    stdTags.push_back(false); // medium
    stdTags.push_back(false); //tight

    // Stop if no taus are needed
    if(!doJetTauTags)
        return;

    for(int j = 0; j < jets.size(); j++) {
        // Reset function pointers for this jet
        effFunLoose = NULL;
        effFunMedium = NULL;
        effFunTight = NULL;
        cand = jets[j];
        prob = rand()/(RAND_MAX+1.);
        tauTags = stdTags;
        prongs = 0;

       /* First, find the prong and the charge of the potential tau by
        * looping over all tracks*/
       cand->Charge = 0;
       for(int t = 0; t < tracks.size(); t++) {
           if(tracks[t]->PT < PTMIN_TAU_TRACK)
               continue;
           if(cand->P4().DeltaR(tracks[t]->P4()) < DR_TAU_TRACK) {
               prongs += 1;
               cand->Charge  += tracks[t]->Charge;
           }
       }
       // If there are 0 or more than 3 prongs, all tags are 'false'
       if(prongs == 0 || prongs > 3) {
           jetTauTags[jets[j]] = tauTags;
           continue;
       }
       // If it's not, let's try to find an overlapping tau
       for(int t = 0; t < true_tau.size(); t++) {
           if(true_tau[t]->PT > PTMIN_TAU_TRUTH &&
              fabs(true_tau[t]->Eta) < ETAMAX_TAU_TRUTH  &&
              cand->P4().DeltaR(true_tau[t]->P4()) < DR_TAU_TRUTH) {
               if(prongs > 1) {
                   effFunLoose = &AnalysisHandlerCMS_14TeV_projected::tauSigEffMultiLoose;
                   effFunMedium = &AnalysisHandlerCMS_14TeV_projected::tauSigEffMultiMedium;
                   effFunTight = &AnalysisHandlerCMS_14TeV_projected::tauSigEffMultiTight;
               }
               else {
                   effFunLoose = &AnalysisHandlerCMS_14TeV_projected::tauSigEffSingleLoose;
                   effFunMedium = &AnalysisHandlerCMS_14TeV_projected::tauSigEffSingleMedium;
                   effFunTight = &AnalysisHandlerCMS_14TeV_projected::tauSigEffSingleTight;
               }
               break;
           }
       }
       // In case no overlap was found, use background efficiencies
       if(effFunLoose == NULL) {
           if(prongs > 1) {
               effFunLoose = &AnalysisHandlerCMS_14TeV_projected::tauBkgEffMultiLoose;
               effFunMedium = &AnalysisHandlerCMS_14TeV_projected::tauBkgEffMultiMedium;
               effFunTight = &AnalysisHandlerCMS_14TeV_projected::tauBkgEffMultiTight;
           }
           else {
               effFunLoose = &AnalysisHandlerCMS_14TeV_projected::tauBkgEffSingleLoose;
               effFunMedium = &AnalysisHandlerCMS_14TeV_projected::tauBkgEffSingleMedium;
               effFunTight = &AnalysisHandlerCMS_14TeV_projected::tauBkgEffSingleTight;
           }
       }
       // Now that we know the right function to use, lets tag
       // We only need to check "medium" if we passed "loose" etc.
       pass_prob = (*effFunLoose)(cand->PT, cand->Eta);
       if(prob < pass_prob) {
           tauTags[0] = true;
           pass_prob = (*effFunMedium)(cand->PT, cand->Eta);
           if (prob < pass_prob) {
               tauTags[1] = true;
               pass_prob = (*effFunTight)(cand->PT, cand->Eta);
               if (prob < pass_prob)
                   tauTags[2] = true;
           }
       }
       jetTauTags[cand] = tauTags;
   }
}

void AnalysisHandlerCMS_14TeV_projected::linkObjects() {
    AnalysisHandler::linkObjects();
    // Linking Particle Objects and run analyses
    for (int a = 0; a < listOfAnalyses.size(); a++) {
        std::vector<Electron*> tempElectronsLoose = electronsLoose;
        listOfAnalyses[a]->electronsLoose = tempElectronsLoose;
        std::vector<Electron*> tempElectronsMedium = electronsMedium;
        listOfAnalyses[a]->electronsMedium = tempElectronsMedium;
        std::vector<Electron*> tempElectronsTight = electronsTight;
        listOfAnalyses[a]->electronsTight = tempElectronsTight;;

        std::vector<Muon*> tempMuonsLoose = muonsLoose;
        listOfAnalyses[a]->muonsLoose = tempMuonsLoose;


        std::vector<Muon*> tempMuonsCombined = muonsCombined;
        listOfAnalyses[a]->muonsCombined = tempMuonsCombined;;
        std::vector<Muon*> tempMuonsCombinedPlus = muonsCombinedPlus;
        listOfAnalyses[a]->muonsCombinedPlus = tempMuonsCombinedPlus;

        std::vector<Photon*> tempPhotonsLoose = photonsLoose;
        listOfAnalyses[a]->photonsLoose = tempPhotonsLoose;
        std::vector<Photon*> tempPhotonsMedium = photonsMedium;
        listOfAnalyses[a]->photonsMedium = tempPhotonsMedium;

        listOfAnalyses[a]->jetBTags = jetBTags;
        listOfAnalyses[a]->jetTauTags = jetTauTags;
    }
}


double AnalysisHandlerCMS_14TeV_projected::photonEffMedium(double pt,
                                             double eta) {
    // TODO: Implement proper function (currently this is standard Delphes)
    return  (fabs(eta) <= 1.5) * (pt > 10.0)  * (0.95) +
            (fabs(eta) > 1.5)*(fabs(eta) <= 2.5)*(pt > 10.0) * (0.85);
}


double AnalysisHandlerCMS_14TeV_projected::electronRecEff(double pt,
                                            double eta) {
    const double r1 = 0.98741;
    const double r2 = 0.0127525;
    const double r3 = -0.0175679;
    const double r4 = 0.00521283;
    const double r5 = -0.000449114;
    // Reconstruction: ATL-COM-PHYS-2013-1287
    double rec_eff = r1+r2*pow(eta, 2)+r3*pow(eta, 4)+
                        r4*pow(eta, 6)+r5*pow(eta, 8);
    return (fabs(eta) < 2.5)*(pt>7.0)*rec_eff;
}

double AnalysisHandlerCMS_14TeV_projected::electronIDEffMedium(double pt,
                                               double eta) {
    const double m1 = 0.766841;
    const double m2 = 0.1509;
    const double m3 = 0.145237;
    const double m4 = 29.1152;
    // Identification: ATL-COM-PHYS-2013-1287
    double id_eff_medium = (pt<80)*(m1+m2/(1.+exp(-m3*(pt-m4))))+
                           (pt>80)*0.945052;
    return id_eff_medium;
}

double AnalysisHandlerCMS_14TeV_projected::electronIDEffTightOverMedium(double pt,
                                                        double eta) {

    const double u1 = 0.674979;
    const double u2 = 0.160224;
    const double u3 = 1.91773;
    double id_eff_tight_eta = (u1+u2*exp(-pow(eta/u3, 2)))/0.776796;

    const double t1 = 0.564986;
    const double t2 = 0.279235;
    const double t3 = 0.078647;
    const double t4 = 22.2707;
    // Identification_Tight: ATL-COM-PHYS-2013-1287
    //double id_eff_tight_ET = (pt<80)*(t1+t2/(1.+exp(-t3*(pt-t4))))+
    //                         (pt>80)*0.883427;
    //return id_eff_tight_ET*id_eff_tight_eta/electronIDEffMedium(pt, eta);

    // TODO The following version is a buggy version which was present in
    // CM1 and is kept for compatibility. It should be tested and
    // maybe set to the actual version above at some point

    double id_eff_tight_ET = (pt<80)*(t1+t2/(1.+exp(-t3*(pt-t4))))+
                             (pt>80)*0.883427*id_eff_tight_eta;
    return id_eff_tight_ET/electronIDEffMedium(pt, eta);
}


double AnalysisHandlerCMS_14TeV_projected::muonEffCombOverCombPlus(double phi,
                                                     double eta) {
    // TODO This is to ensure compatibility with CM1, but should
    // be retested and removed at some point
    phi = 0;
    // Reconstruction efficiency mainly depends on position in MS
    // {CMS-CONF-2011-063}
    if(fabs(eta) > 2.5)
        return 0;
    if(fabs(phi) > M_PI)
        return 0;
    if(fabs(eta) < 0.1)
        return 1;

    // Determine position in discrete eta-phi-map [Fig 2]
    int eta_i, phi_i;
    for(eta_i = 0; eta_i < 21; eta_i++) {
        if(etaProj[eta_i] >= eta)
            break;
    }
    for(phi_i = 0; phi_i < 53; phi_i++) {
        if(phiProj[phi_i] < phi)
            break;
    }
    phi_i--;
    // [ Fig 6b) ] divided by CombinedPlus
    switch(detMap[phi_i][eta_i]) {
    case cscsm: return 0.959972/0.983203;
    case cscla: return 0.966359/0.988252;
    case bee: return 0.967215/0.993096;
    case endsm: return 0.975844/0.994126;
    case endla: return 0.969557/0.992271;
    case trans: return 0.968593/0.992683;
    case feet: return 0.943484/0.959913;
    case barov: return 0.943484/0.959913;
    case barsm: return 0.977322/0.993508;
    case barla: return  0.919635/0.926731;
    }
    return 0;
}

double AnalysisHandlerCMS_14TeV_projected::muonEffCombPlus(double phi,
                                             double eta) {
    // TODO This is to ensure compatibility with CM1, but should
    // be retested and removed at some point
    phi = 0;

    // Reconstruction efficiency mainly depends on position in MS
    // {CMS-CONF-2011-063}
    if(fabs(eta) > 2.5)
        return 0;
    if(fabs(phi) > M_PI)
        return 0;
    if(fabs(eta) < 0.1)
        return 0.83; // [Fig 6 f)]

    // Determine position in discrete eta-phi-map [Fig 2]
    int eta_i, phi_i;
    for(eta_i = 0; eta_i < 21; eta_i++) {
        if(etaProj[eta_i] >= eta)
            break;
    }
    for(phi_i = 0; phi_i < 53; phi_i++) {
        if(phiProj[phi_i] < phi)
            break;
    }
    phi_i--;

    // [ Fig 6b) ]
        switch(detMap[phi_i][eta_i]) {
        case cscsm: return 0.983203;
        case cscla: return 0.988252;
        case bee: return 0.993096;
        case endsm: return 0.994126;
        case endla: return 0.992271;
        case trans: return 0.992683;
        case feet: return 0.984439;
        case barov: return 0.959913;
        case barsm: return 0.993508;
        case barla: return 0.926731;
        }
        return 0;
}


double AnalysisHandlerCMS_14TeV_projected::tauSigEffSingleLoose(double pt,
                                                  double eta) {
    double eff_s1loose_eta;
    {
        const double x0 = 1.2;
        const double A = -0.875;
        const double s = 0.294;
        const double A2 = 0.886;
        const double s2 = 0.286;
        const double A3 = -0.0268;
        const double s3 = 1.16;
        const double A4 = 0.00419;
        const double s4 = 10.9;
        const double k = 0.94;
        const double average = 0.955623;
        eff_s1loose_eta = (k +
                           A*exp(-pow(fabs(eta)-x0,2)/pow(s,2)) +
                           A2*exp(-pow(fabs(eta)-x0,2)/pow(s2,2)) +
                           A3*sin(eta)/eta*exp(-pow(eta,2)/pow(s3,2)) +
                           A4*pow(eta,2)*exp(-pow(eta,2)/pow(s4,2))
                          )/average;
    }
    double eff_s1loose_pt1;
    {
        const double A1 = 0.0354;
        const double B1 = 0.0994;
        const double C1 = 0.928;
        const double D1 = -5.52e-05;
        const double E1 = 0.966;
        const double F1 = 55.1;
        eff_s1loose_pt1 = (pt < 80.)*(C1+A1/(1.+exp(B1*(pt-F1)))) +
                          (pt >= 80.)*(D1*pt+E1);
    }
    double eff_s1loose_pt2;
    {
        const double x0 = 15;
        const double A1 = 0.670;
        const double A2 = 100;
        const double A3 = 0.0974;
        const double A4 = 2.34;
        const double A5 = 2.04;
        const double B1 = -2.55;
        const double B2 = 1.16;
        const double B3 = 0.427;
        const double B4 = 0.846;
        const double C1 = 2.14e-05;
        const double C2 = 0.0223;
        eff_s1loose_pt2 = A1 +
                          (pt < 105.)*A2*sin(A3*(pt-x0)+A4)/pow(pt,A5) +
                          (pt >= 105.)*A2*sin(A3*(105.-x0)+A4)/pow(105.,A5) +
                          B1*pow(pt,B2)*exp(-B3*pow(pt,B4)) +
                          (pt < 80.+2./C2)*C1*pow(pt-80.,2)*exp(-C2*(pt-80.)) +
                          (pt >= 80.+2./C2)*C1*pow(2./C2,2)*exp(-2.);
    }
    return (eff_s1loose_eta*eff_s1loose_pt1*eff_s1loose_pt2);
          // Formerly, there used to be a track-eff factor
          //((fabs(eta)<=1.5)*0.95 + (fabs(eta)>1.5)*0.85);
}

double AnalysisHandlerCMS_14TeV_projected::tauSigEffSingleMedium(double pt,
                                                   double eta) {
    double eff_s1medium_eta;
    {
        const double x0 = 1.2;
        const double A = -0.283;
        const double s = -0.303;
        const double A2 = 0.309;
        const double s2 = -0.256;
        const double A3 = 0.0346;
        const double s3 = 0.113;
        const double A4 = -0.0796;
        const double s4 = 1.25;
        const double k = 0.91;
        const double average = 0.877616;
        eff_s1medium_eta = (k +
                            A*exp(-pow(fabs(eta)-x0,2)/pow(s,2)) +
                            A2*exp(-pow(fabs(eta)-x0,2)/pow(s2,2)) +
                            A3*sin(eta)/eta*exp(-pow(eta,2)/pow(s3,2)) +
                            A4*pow(eta,2)*exp(-pow(eta,2)/pow(s4,2))
                           )/average;
    }
    double eff_s1medium_pt1;
    {
        const double A1 = 0.0613;
        const double B1 = 0.0922;
        const double C1 = 0.833;
        const double D1 = -0.000190;
        const double E1 = 0.893;
        const double F1 = 57.8;
        eff_s1medium_pt1 = (pt < 80)*(C1+A1/(1.+exp(B1*(pt-F1)))) +
                           (pt >= 80)*(D1*pt+E1);
    }
    double eff_s1medium_pt2;
    {
        const double x0 = 15;
        const double A1 = 0.586;
        const double A2 = 100;
        const double A3 = 0.0997;
        const double A4 = 2.23;
        const double A5 = 1.91;
        const double B1 = -2.69;
        const double B2 = 1.23;
        const double B3 = 0.483;
        const double B4 = 0.791;
        const double C1 = 3.46e-05;
        const double C2 = 0.0223;
        eff_s1medium_pt2 = A1 +
                           (pt < 105.)*A2*sin(A3*(pt-x0)+A4)/pow(pt,A5) +
                           (pt >= 105.)*A2*sin(A3*(105.-x0)+A4)/pow(105,A5) +
                           B1*pow(pt,B2)*exp(-B3*pow(pt,B4)) +
                           (pt < 80.+2./C2)*C1*pow(pt-80.,2)*exp(-C2*(pt-80.))+
                           (pt >= 80.+2./C2)*C1*pow(2./C2,2)*exp(-2.);
    }
    return (eff_s1medium_eta * eff_s1medium_pt1 * eff_s1medium_pt2 );
          // Formerly, there used to be a track-eff factor
          //((fabs(eta)<=1.5)*0.95 + (fabs(eta)>1.5)*0.85);
}

double AnalysisHandlerCMS_14TeV_projected::tauSigEffSingleTight(double pt,
                                                  double eta) {
    double eff_s1tight_eta;
    {
        const double x0 = 1.2;
        const double A = -0.472;
        const double s = 0.304;
        const double A2 = 0.503;
        const double s2 = 0.258;
        const double A3 = 0.0594;
        const double s3 = 0.102;
        const double A4 = -0.0851;
        const double s4 = 1.42;
        const double k = 0.84;
        const double average = 0.789361;
        eff_s1tight_eta = (k +
                           A*exp(-pow(fabs(eta)-x0,2)/pow(s,2)) +
                           A2*exp(-pow(fabs(eta)-x0,2)/pow(s2,2)) -
                           A3*sin(eta)/eta*exp(-pow(eta,2)/pow(s3,2)) +
                           A4*pow(eta,2)*exp(-pow(eta,2)/pow(s4,2))
                          )/average;
    }

    double eff_s1tight_pt1;
    {
        const double A1 = 0.0706;
        const double B1 = 0.140;
        const double C1 = 0.738;
        const double D1 = -0.000259;
        const double E1 = 0.812;
        const double F1 = 61.4;
        eff_s1tight_pt1 = (pt < 80)*(C1+A1/(1.+exp(B1*(pt-F1)))) +
                          (pt >= 80)*(D1*pt+E1);
    }

    double eff_s1tight_pt2;
    {
        const double x0 = 15;
        const double A1 = 0.388;
        const double A2 = 100;
        const double A3 = 0.101;
        const double A4 = 2.16;
        const double A5 = 1.79;
        const double B1 = -2.78;
        const double B2 = 1.17;
        const double B3 = 0.377;
        const double B4 = 0.799;
        const double C1 = 6.17e-05;
        const double C2 = 0.0292;
        eff_s1tight_pt2 = A1 +
                          (pt < 105)*A2*sin(A3*(pt-x0)+A4)/pow(pt,A5) +
                          (pt >= 105)*A2*sin(A3*(105-x0)+A4)/pow(105,A5) +
                          B1*pow(pt,B2)*exp(-B3*pow(pt,B4)) +
                          (pt < 80 + 2./C2)*C1*pow(pt-80,2)*exp(-C2*(pt-80)) +
                          (pt >= 80 + 2./C2)*C1*pow(2./C2,2)*exp(-2.);
    }
    return (eff_s1tight_eta * eff_s1tight_pt1 * eff_s1tight_pt2);
          // Formerly, there used to be a track-eff factor
          //((fabs(eta)<=1.5)*0.95 + (fabs(eta)>1.5)*0.85);
}


double AnalysisHandlerCMS_14TeV_projected::tauSigEffMultiLoose(double pt,
                                                 double eta) {
    const double x0 = 15;
    const double A1 = 0.594;
    const double A2 = 99.6;
    const double A3 = 0.0958;
    const double A4 = 2.09;
    const double A5 = 1.92;
    const double B1 = -6.44;
    const double B2 = 0.143;
    const double B3 = 0.106;
    const double B4 = 1.08;
    const double C1 = 4.22e-05;
    const double C2 = 0.0192;
    return A1 +
           (pt < 105.)*A2*sin(A3*(pt-x0)+A4)/pow(pt, A5) +
           (pt >= 105.)*A2*sin(A3*(105.-x0)+A4)/pow(105,A5) +
           B1*pow(pt, B2)*exp(-B3*pow(pt, B4)) +
           (pt < 80. + 2./C2)*C1*pow(pt-80.,2)*exp(-C2*(pt-80.)) +
           (pt >= 80. + 2./C2)*C1*pow(2./C2,2)*exp(-2.);
}

double AnalysisHandlerCMS_14TeV_projected::tauSigEffMultiMedium(double pt,
                                                  double eta) {
    const double x0 = 15;
    const double A1 = 0.510;
    const double A2 = 100;
    const double A3 = 0.0950;
    const double A4 = 2.18;
    const double A5 = 1.86;
    const double B1 = -2.322;
    const double B2 = 0.924;
    const double B3 = 0.233;
    const double B4 = 0.940;
    const double C1 = 5.00e-05;
    const double C2 = 0.0191;
    return A1 +
           (pt < 105.)*A2*sin(A3*(pt-x0)+A4)/pow(pt, A5) +
           (pt >= 105.)*A2*sin(A3*(105-x0)+A4)/pow(105,A5) +
           B1*pow(pt, B2)*exp(-B3*pow(pt, B4)) +
           (pt < 80. + 2./C2)*C1*pow(pt-80,2)*exp(-C2*(pt-80)) +
           (pt >= 80. + 2./C2)*C1*pow(2./C2,2)*exp(-2.);
}

double AnalysisHandlerCMS_14TeV_projected::tauSigEffMultiTight(double pt,
                                                 double eta) {
    const double x0 = 15;
    const double A1 = 0.324;
    const double A2 = 100;
    const double A3 = 0.0971;
    const double A4 = 2.09;
    const double A5 = 1.78;
    const double B1 = -2.29;
    const double B2 = 0.671;
    const double B3 = 0.104;
    const double B4 = 1.096;
    const double C1 = 6.30e-05;
    const double C2 = 0.0212;
    return A1 +
           (pt < 105)*A2*sin(A3*(pt-x0)+A4)/pow(pt, A5) +
           (pt >= 105)*A2*sin(A3*(105-x0)+A4)/pow(105,A5) +
           B1*pow(pt, B2)*exp(-B3*pow(pt, B4)) +
           (pt < 80 + 2./C2)*C1*pow(pt-80,2)*exp(-C2*(pt-80)) +
           (pt >= 80 + 2./C2)*C1*pow(2./C2,2)*exp(-2.);
}


double AnalysisHandlerCMS_14TeV_projected::tauBkgEffSingleLoose(double pt,
                                                  double eta) {
    const double A1 = 0.717;
    const double B1 = 0.0789;
    const double C1 = 0.00182;
    const double D1 = 0.106;
    const double E1 = 0.0973;
    const double F1 = 64.3;
    return (
            (pt < 80)*(A1*exp(-B1*pt)+C1*pt)+
            (pt >= 80)*(D1*(1.-1./(1.+exp(E1*(pt-F1)))))
       );
          // Formerly, there used to be a track-eff factor
          //((fabs(eta)<=1.5)*0.95 + (fabs(eta)>1.5)*0.85);
}

double AnalysisHandlerCMS_14TeV_projected::tauBkgEffSingleMedium(double pt,
                                                   double eta) {
    const double A1 = 0.301;
    const double B1 = 0.0720;
    const double C1 = 0.000935;
    const double D1 = 0.0627;
    const double E1 = 0.0144;
    const double F1 = 20.0;
    return ((pt < 80)*(A1*exp(-B1*pt)+C1*pt)+
            (pt >= 80)*(D1*(1.-1./(1.+exp(E1*(pt-F1)))))
       );
          // Formerly, there used to be a track-eff factor
          //((fabs(eta)<=1.5)*0.95 + (fabs(eta)>1.5)*0.85);
}

double AnalysisHandlerCMS_14TeV_projected::tauBkgEffSingleTight(double pt,
                                                  double eta) {
    const double A1 = 0.117;
    const double B1 = 0.0742;
    const double C1 = 0.000359;
    const double D1 = 0.0192;
    const double E1 = 0.0247;
    const double F1 = 46.8;
    return (
            (pt < 80)*(A1*exp(-B1*pt)+C1*pt) +
            (pt >= 80)*(D1*(1.-1./(1.+exp(E1*(pt-F1)))))
       );
          // Formerly, there used to be a track-eff factor
          //((fabs(eta)<=1.5)*0.95 + (fabs(eta)>1.5)*0.85);
}


double AnalysisHandlerCMS_14TeV_projected::tauBkgEffMultiLoose(double pt,
                                                 double eta) {
    const double A1 = 0.265;
    const double B1 = 0.0827;
    const double C1 = 0.000226;
    const double D1 = 0.0124;
    const double E1 = 0.0228;
    const double F1 = 4.08;
    return (pt < 80)*(A1*exp(-B1*pt)+C1*pt)+
           (pt >= 80)*(D1*(1.-1./(1.+exp(E1*(pt-F1)))));
}

double AnalysisHandlerCMS_14TeV_projected::tauBkgEffMultiMedium(double pt,
                                                  double eta) {
    const double A1 = 0.154;
    const double B1 = 0.0832;
    const double C1 = 0.000136;
    const double D1 = 0.00906;
    const double E1 = 0.0119;
    const double F1 = 20.0;
    return (pt < 80)*(A1*exp(-B1*pt)+C1*pt)+
           (pt >= 80)*(D1*(1.-1./(1.+exp(E1*(pt-F1)))));
}

double AnalysisHandlerCMS_14TeV_projected::tauBkgEffMultiTight(double pt,
                                                 double eta) {
    const double A1 = 0.0579;
    const double B1 = 0.0880;
    const double C1 = 4.08e-05;
    const double D1 = 0.00314;
    const double E1 = 0.0173;
    const double F1 = 78.9;
    return (pt < 80)*(A1*exp(-B1*pt)+C1*pt)+
           (pt >= 80)*(D1*(1.-1./(1.+exp(E1*(pt-F1)))));
}


double AnalysisHandlerCMS_14TeV_projected::bSigEff(double pt,
                                     double eta,
                                     double eff) {
    const double y0 = 0.5523;
    const double x0 = 47.6071;
    const double A = 0.2102;
    const double k = 0.1258;
    const double r = 308.197;
    return eff/0.82*(y0+A*1./(1.+exp(-k*(pt-x0))))*(0.7+0.05*exp(-pt/r))/0.75*
           ((pt>100)*(1.+(pt-100.)*(-0.0007))+(pt<=100)*1);
}

double AnalysisHandlerCMS_14TeV_projected::bBkgCJetRej(double eff) {
    const double A = 29.2836;
    const double c = -4.57183;
    const double d = 8.49607;
    const double f = -7.25358;
    const double g = 2.33026;
    return pow(10,(A*(eff+c*pow(eff,2)+d*pow(eff,3)+
                          f*pow(eff,4)+g*pow(eff,5))));
}

double AnalysisHandlerCMS_14TeV_projected::bBkgCJetEff(double pt,
                                         double eta,
                                         double eff) {
    return 0.4*(0.460622*1./(1.+exp(-0.0463993*(pt-20.4359))))*
           bBkgCJetRej(0.7)/bBkgCJetRej(eff);
}

double AnalysisHandlerCMS_14TeV_projected::bBkgLJetRej(double eff) {
    const double A  = 54.3809;
    const double c  = -4.15601;
    const double d  = 7.59943;
    const double f  = -6.72996;
    const double g  = 2.28954;
    const double A2 = 5806.98;
    const double B2 = -24674.2;
    const double C2 = 39321.6;
    const double D2 = -27849.7;
    const double E2 = 7395.3;
    if (eff < 0.86) {
        return pow(10, (A*(eff+c*pow(eff, 2)+d*pow(eff, 3)+
                               f*pow(eff, 4)+g*pow(eff, 5))));
    }
    else {
        return pow(10, A2+B2*eff+C2*pow(eff, 2)+D2*pow(eff,3)+E2*pow(eff,4));
    }
}

double AnalysisHandlerCMS_14TeV_projected::bBkgLJetEff(double pt,
                                         double eta,
                                         double eff) {
    const double f0 = 0.0105802;
    const double f1 = 6.46503e-06;
    const double f2 = 4.02822e-08;
    const double g0 = 0.00661067;
    const double g1 = 6.48582e-05;
    const double g2 = -3.123e-08;
    return 0.8*((fabs(eta)<=1.3)*(f0+f1*pt+f2*pt*pt)+
                (fabs(eta)>1.3)*(fabs(eta)<2.5)*(g0+g1*pt+g2*pt*pt)
               )*bBkgLJetRej(0.7)/bBkgLJetRej(eff);
}


const AnalysisHandlerCMS_14TeV_projected::muonDetector AnalysisHandlerCMS_14TeV_projected::detMap[53][21] = {{cscla, endla, endla, endla, endla, endla, endla, trans, barla, barla, barla, barla, barla, trans, endla, endla, endla, endla, endla, endla, cscla},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, bee, endsm, trans, trans, trans, barsm, barsm, barsm, barsm, barsm, trans, trans, trans, endsm, bee, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscla, endla, endla, endla, endla, endla, endla, trans, barla, barla, barla, barla, barla, trans, endla, endla, endla, endla, endla, endla, cscla},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, bee, endsm, trans, trans, trans, barsm, barsm, barsm, barsm, barsm, trans, trans, trans, endsm, bee, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscla, endla, endla, endla, endla, endla, endla, trans, barla, barla, barla, barla, barla, trans, endla, endla, endla, endla, endla, endla, cscla},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, bee, endsm, trans, trans, trans, barsm, barsm, barsm, barsm, barsm, trans, trans, trans, endsm, bee, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscla, endla, endla, endla, endla, endla, endla, trans, barla, barla, barla, barla, barla, trans, endla, endla, endla, endla, endla, endla, cscla},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, bee, endsm, trans, trans, trans, barsm, barsm, barsm, barsm, barsm, trans, trans, trans, endsm, bee, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscla, endla, endla, endla, endla, endla, endla, trans, barla, barla, barla, barla, barla, trans, endla, endla, endla, endla, endla, endla, cscla},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, bee, endsm, trans, trans, trans, barsm, barsm, barsm, barsm, barsm, trans, trans, trans, endsm, bee, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscla, endla, endla, endla, endla, endla, endla, trans, barla, barla, barla, barla, barla, trans, endla, endla, endla, endla, endla, endla, cscla},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, feet, feet, feet, feet, feet, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, bee, endsm, trans, trans, trans, feet, feet, feet, feet, feet, trans, trans, trans, endsm, bee, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, feet, feet, feet, feet, feet, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, feet, barov, feet, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, feet, barla, feet, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscla, endla, endla, endla, endla, endla, endla, trans, barla, feet, barla, feet, barla, trans, endla, endla, endla, endla, endla, endla, cscla},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, feet, barla, feet, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, feet, barov, feet, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, feet, feet, feet, feet, feet, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, bee, endsm, trans, trans, trans, feet, feet, feet, feet, feet, trans, trans, trans, endsm, bee, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, feet, feet, feet, feet, feet, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscla, endla, endla, endla, endla, endla, endla, trans, barla, barla, barla, barla, barla, trans, endla, endla, endla, endla, endla, endla, cscla},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, bee, endsm, trans, trans, trans, barsm, barsm, barsm, barsm, barsm, trans, trans, trans, endsm, bee, endsm, cscsm, cscsm},
        {cscsm, cscsm, endsm, endsm, endsm, endsm, trans, trans, barov, barov, barov, barov, barov, trans, trans, endsm, endsm, endsm, endsm, cscsm, cscsm},
        {cscsm, endsm, endsm, endsm, endsm, endsm, endsm, trans, barla, barla, barla, barla, barla, trans, endsm, endsm, endsm, endsm, endsm, endsm, cscsm},
        {cscla, endla, endla, endla, endla, endla, endla, trans, barla, barla, barla, barla, barla, trans, endla, endla, endla, endla, endla, endla, cscla}};
const double AnalysisHandlerCMS_14TeV_projected::etaProj[21] = {-2.005, -1.955, -1.709, -1.411, -1.238, -1.163, -1.113, -0.965, -0.817, -0.519, 0.519, 0.817, 0.965, 1.113, 1.163, 1.238, 1.411, 1.709, 1.955, 2.005, 2.500};
const double AnalysisHandlerCMS_14TeV_projected::phiProj[53] = {3.142, 3.009, 2.948, 2.849, 2.629, 2.529, 2.471, 2.210, 2.174, 2.065, 1.855, 1.745, 1.695, 1.436, 1.386, 1.276, 1.066, 0.957, 0.916, 0.657, 0.596, 0.496, 0.277, 0.178, 0.142, -0.142, -0.178, -0.277, -0.496, -0.596, -0.657, -0.916, -0.957, -1.018, -1.066, -1.276, -1.357, -1.386, -1.436, -1.695, -1.745, -1.797, -1.855, -2.065, -2.134, -2.174, -2.210, -2.471, -2.529, -2.629, -2.849, -2.948, -3.009};

