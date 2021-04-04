/*
 * AnalysisHandlerATLAS_13TeV_LLP.cc
 *
 *  Created on: Feb 23, 2015
 *      Author: daschm
 */

#include "AnalysisHandlerATLAS_13TeV_LLP.h"

const double AnalysisHandlerATLAS_13TeV_LLP::DR_TAU_TRACK = 0.2;
const double AnalysisHandlerATLAS_13TeV_LLP::PTMIN_TAU_TRACK = 1.0;

const double AnalysisHandlerATLAS_13TeV_LLP::DR_TAU_TRUTH = 0.2;
const double AnalysisHandlerATLAS_13TeV_LLP::ETAMAX_TAU_TRUTH = 2.5;
const double AnalysisHandlerATLAS_13TeV_LLP::PTMIN_TAU_TRUTH = 10.0;

const double AnalysisHandlerATLAS_13TeV_LLP::DR_B_TRUTH = 0.4;
const double AnalysisHandlerATLAS_13TeV_LLP::ETAMAX_B_TRUTH = 2.5;
const double AnalysisHandlerATLAS_13TeV_LLP::PTMIN_B_TRUTH = 1.0;

AnalysisHandlerATLAS_13TeV_LLP::AnalysisHandlerATLAS_13TeV_LLP() : AnalysisHandler() {
}

AnalysisHandlerATLAS_13TeV_LLP::~AnalysisHandlerATLAS_13TeV_LLP() {
}

void AnalysisHandlerATLAS_13TeV_LLP::initialize() {
}

void AnalysisHandlerATLAS_13TeV_LLP::finalize() {
}

void AnalysisHandlerATLAS_13TeV_LLP::bookAnalysis(std::string analysisName,
                                        Tag_Map whichTags,
                                        Param_Map eventParameters) {
    Global::print(name, "Loading Analysis "+analysisName);

    AnalysisBase* a = NULL;
    if(analysisName == "atlas_1712_02118_ew")
        a = new Atlas_1712_02118_ew();
    else if(analysisName == "atlas_1710_04901")
        a = new Atlas_1710_04901();
    else if(analysisName == "atlas_1902_01636")
        a = new Atlas_1902_01636();
    else if(analysisName == "atlas_2003_11956")
        a = new Atlas_2003_11956();
    else //@@extracode@@
        Global::abort(name,
                      "Cannot load analysis "+analysisName+
                          " - analysis unknown.");

    a->setup(whichTags, eventParameters);
    listOfAnalyses.push_back(a);
    Global::print(name,
                  "Successfully loaded analysis "+analysisName);
}


void AnalysisHandlerATLAS_13TeV_LLP::postProcessParticles() {
    // First do all the mother handler processions
    AnalysisHandler::postProcessParticles();

    // Then take care of the rest
    identifyPhotons();
    identifyElectrons();
    identifyMuons();
    tagBJets();
    tagTauJets();
}

void AnalysisHandlerATLAS_13TeV_LLP::identifyPhotons() {
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

void AnalysisHandlerATLAS_13TeV_LLP::identifyElectrons() {
    double eEffMed = 0, eEffTigOvMed = 0, eEffLoo = 0;
    Electron* cand = NULL;

    electronsLoose.clear();
    electronsMedium.clear();
    electronsTight.clear();
    for (int e = 0; e < electrons.size(); e++) {
        cand = electrons[e];
        // tag 0 is the loose isolation condition
        if (electronIsolationTags[cand][0]) {
	  eEffLoo = electronRecEff(cand->PT, cand->Eta) *
                      electronIDEffLoose(cand->PT, cand->Eta);
	  if (rand()/(RAND_MAX+1.) <  eEffLoo) {
            electronsLoose.push_back(cand);
            eEffMed = electronIDEffMedium(cand->PT, cand->Eta);
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
}

void AnalysisHandlerATLAS_13TeV_LLP::identifyMuons()  {
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

static double bBkg_l_eff(double pt, double eta, double wp);
static double bBkg_c_eff(double pt, double eta, double wp);

void AnalysisHandlerATLAS_13TeV_LLP::tagBJets() {
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
                  eff_function = &AnalysisHandlerATLAS_13TeV_LLP::bSigEff;
                  break;
              }
          }
          // If no b overlap, test with truth c's and maybe use c-efficiency
          if (eff_function == NULL) {
              for(int c = 0; c < true_c.size(); c++) {
                  if(true_c[c]->PT > PTMIN_B_TRUTH &&
                     fabs(true_c[c]->Eta) < ETAMAX_B_TRUTH &&
                     true_c[c]->P4().DeltaR(cand->P4()) < DR_B_TRUTH) {
                      eff_function = &bBkg_c_eff;
                      break;
                  }
              }
          }
          // If no b and no c overlap, use light jet Rej
          if (eff_function == NULL) {
              eff_function = &bBkg_l_eff;
	  }

          // Now that we know the right function to use, lets tag
          for(int btag = 0; btag < listOfJetBTags.size(); btag++) {
              pass_prob = (*eff_function)(cand->PT, cand->Eta,
                                                listOfJetBTags[btag]->eff);
              if (fabs(cand->P4().Eta()) < ETAMAX_B_TRUTH && prob < pass_prob) {
                      bTags.push_back(true);
	      } else {
                  bTags.push_back(false);
	      }
          }
          jetBTags[jets[j]] = bTags;
      }
}

void AnalysisHandlerATLAS_13TeV_LLP::tagTauJets() {
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
                   effFunLoose = &AnalysisHandlerATLAS_13TeV_LLP::tauSigEffMultiLoose;
                   effFunMedium = &AnalysisHandlerATLAS_13TeV_LLP::tauSigEffMultiMedium;
                   effFunTight = &AnalysisHandlerATLAS_13TeV_LLP::tauSigEffMultiTight;
               }
               else {
                   effFunLoose = &AnalysisHandlerATLAS_13TeV_LLP::tauSigEffSingleLoose;
                   effFunMedium = &AnalysisHandlerATLAS_13TeV_LLP::tauSigEffSingleMedium;
                   effFunTight = &AnalysisHandlerATLAS_13TeV_LLP::tauSigEffSingleTight;
               }
               break;
           }
       }
       // In case no overlap was found, use background efficiencies
       if(effFunLoose == NULL) {
           if(prongs > 1) {
               effFunLoose = &AnalysisHandlerATLAS_13TeV_LLP::tauBkgEffMultiLoose;
               effFunMedium = &AnalysisHandlerATLAS_13TeV_LLP::tauBkgEffMultiMedium;
               effFunTight = &AnalysisHandlerATLAS_13TeV_LLP::tauBkgEffMultiTight;
           }
           else {
               effFunLoose = &AnalysisHandlerATLAS_13TeV_LLP::tauBkgEffSingleLoose;
               effFunMedium = &AnalysisHandlerATLAS_13TeV_LLP::tauBkgEffSingleMedium;
               effFunTight = &AnalysisHandlerATLAS_13TeV_LLP::tauBkgEffSingleTight;
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

void AnalysisHandlerATLAS_13TeV_LLP::linkObjects() {
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


double AnalysisHandlerATLAS_13TeV_LLP::photonEffMedium(double pt,
                                             double eta) {
    // TODO: Implement proper function 
    //  https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/PLOTS/EGAM-2016-003/fig_02.pdf
    // (converted)
    //  not verified with MC 
    return  (pt > 35.0)  * 0.95 +
            (pt > 30.0) * (pt < 35.) * 0.90 +
	    (pt > 25.0) * (pt < 30.) * 0.85 +
	    (pt > 20.0) * (pt < 25.) * 0.78 +
	    (pt > 15.0) * (pt < 20.) * 0.66;
}


double AnalysisHandlerATLAS_13TeV_LLP::electronRecEff(double pt,
                                            double eta) {
    // https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/CONFNOTES/ATLAS-CONF-2016-024/
    // takes into account intial Delphes inefficiency
    return 1.0;
}


double AnalysisHandlerATLAS_13TeV_LLP::electronIDEffLoose(double pt,
                                               double eta) {
    // tuned to https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/PLOTS/EGAM-2016-002/fig_01.png
    // https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/CONFNOTES/ATLAS-CONF-2016-024/
    double id_eff_loose = 0.976 - 0.0614*exp(1.-pt/29.1) 
                    +  0.024;		//shift for Delphes correction	   
    return id_eff_loose;
}

double AnalysisHandlerATLAS_13TeV_LLP::electronIDEffMedium(double pt,
                                               double eta) {
    // tuned to https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/PLOTS/EGAM-2016-002/fig_01.png
    // https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/CONFNOTES/ATLAS-CONF-2016-024/
    double id_eff_loose = 0.976 - 0.0614*exp(1.-pt/29.1); //truth function without correction
    double id_eff_medium = 0.937 - 0.109*exp(1.-pt/21.);			   
    return id_eff_medium/id_eff_loose;
}

double AnalysisHandlerATLAS_13TeV_LLP::electronIDEffTightOverMedium(double pt,
                                                        double eta) {
    // tuned to https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/PLOTS/EGAM-2016-002/fig_01.png
    // https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/CONFNOTES/ATLAS-CONF-2016-024/
    double id_eff_medium = 0.937 - 0.109*exp(1.-pt/21.);
    double id_eff_tight = 0.8885 - 0.138*exp(1.-pt/27.45);			     
    return id_eff_tight/id_eff_medium;
}


double AnalysisHandlerATLAS_13TeV_LLP::muonEffCombOverCombPlus(double phi,
                                                     double eta) {
    // https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/PAPERS/PERF-2015-10/fig_03b.png
    // tuned to MC
    return 0.98;
}

double AnalysisHandlerATLAS_13TeV_LLP::muonEffCombPlus(double phi,
                                             double eta) {
    // https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/PAPERS/PERF-2015-10/fig_03a.png
    // https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/PAPERS/PERF-2015-10/
    // ignore pt dependence https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/PAPERS/PERF-2015-10/fig_06.png
    if (fabs(eta) < 0.1 ) return 0.6;
    return 1.;
}


double AnalysisHandlerATLAS_13TeV_LLP::tauSigEffSingleLoose(double pt,
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

double AnalysisHandlerATLAS_13TeV_LLP::tauSigEffSingleMedium(double pt,
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

double AnalysisHandlerATLAS_13TeV_LLP::tauSigEffSingleTight(double pt,
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


double AnalysisHandlerATLAS_13TeV_LLP::tauSigEffMultiLoose(double pt,
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

double AnalysisHandlerATLAS_13TeV_LLP::tauSigEffMultiMedium(double pt,
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

double AnalysisHandlerATLAS_13TeV_LLP::tauSigEffMultiTight(double pt,
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


double AnalysisHandlerATLAS_13TeV_LLP::tauBkgEffSingleLoose(double pt,
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

double AnalysisHandlerATLAS_13TeV_LLP::tauBkgEffSingleMedium(double pt,
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

double AnalysisHandlerATLAS_13TeV_LLP::tauBkgEffSingleTight(double pt,
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


double AnalysisHandlerATLAS_13TeV_LLP::tauBkgEffMultiLoose(double pt,
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

double AnalysisHandlerATLAS_13TeV_LLP::tauBkgEffMultiMedium(double pt,
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

double AnalysisHandlerATLAS_13TeV_LLP::tauBkgEffMultiTight(double pt,
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

double AnalysisHandlerATLAS_13TeV_LLP::bSigEff(double pt,
                                     double eta,
                                     double eff) {
    const double y0 = 0.5523;
    const double x0 = 47.6071;
    const double A = 0.2102;
    const double k = 0.1258;
    const double r = 308.197;
    return 1.1*eff/0.82*(y0+A*1./(1.+exp(-k*(pt-x0))))*(0.7+0.05*exp(-pt/r))/0.75*
           ((pt>100)*(1.+(pt-100.)*(-0.0007))+(pt<=100)*1);
}

//! Scaling behavior of the efficiency to tag a c-jet as a b-jet with
//! the working point of the b-tagger
//! This function was fitted to Fig. 11(b) (MV2c20 Run-2) from
//! ATL-PHYS-PUB-2015-022
static double bBkg_c_scale_function(double wp) {
	double quadratic = 7.38943;
	double linear = -19.695;
	double constant = 12.3817;
	return exp(quadratic*pow(wp,2) + linear*wp + constant);
}

//! Dependence of the efficiency to tag a c-jet as a b-jet on the
//! pseudo rapidity
//! This function was fitted to Fig. 10(b), 14(b), 15(b), 16(b)
//! (c-jets) from ATL-PHYS-PUB-2015-022
static double bBkg_c_eff_eta(double eta, double wp) {
	double wp_quadratic = 1.11636810e+02;
	double wp_linear =  -1.42894910e+02;
	double eta_quadratic = 1.28569172e-04;
	double eta_linear = -2.15249612e-06;
	double eta_constant = -2.97456434e-03;
	double aeta = fabs(eta);
	return (
		bBkg_c_scale_function(0.7) / bBkg_c_scale_function(wp)
		* (eta_quadratic*pow(aeta,2) + eta_linear*aeta + eta_constant)
		* (wp_quadratic*pow(wp,2) + wp_linear*wp + 1)
		);
}

static double bBkg_c_pt_piece_1_constant(double wp) {
	double constant_constant = -0.03287989;
	double constant_linear = 0.37516101;
	double constant_quadratic = -0.28836324;
	return constant_constant + constant_linear*wp + constant_quadratic*pow(wp,2);
}

static double bBkg_c_pt_piece_1_linear(double wp) {
	double linear_constant = 1.02399891e-3;
	double linear_linear = -2.0949067e-4;
	double linear_quadratic = -4.9999636e-5;
	return linear_constant + linear_linear*wp + linear_quadratic*pow(wp,2);
}

static double bBkg_c_pt_piece_1(double pt, double wp) {
	return bBkg_c_pt_piece_1_constant(wp) + pt * bBkg_c_pt_piece_1_linear(wp);
}

static double bBkg_c_pt_piece_2_constant(double wp) {
	double constant_constant = 0.51295139;
	double constant_linear = -0.94078135;
	double constant_quadratic = 0.57573368;
	return constant_constant + constant_linear*wp + constant_quadratic*pow(wp, 2);
}

static double bBkg_c_pt_piece_2_linear(double wp) {
	double linear_constant = 0.21311996e-3;
	double linear_linear = -3.51827713e-4;
	double linear_quadratic = -5.19590178e-5;
	return linear_constant + linear_linear*wp + linear_quadratic*pow(wp, 2);
}

static double bBkg_c_pt_piece_2(double pt, double wp) {
	return bBkg_c_pt_piece_2_constant(wp) + pt * bBkg_c_pt_piece_2_linear(wp);
}

static double bBkg_c_pt_piece_3_constant(double wp) {
	double constant_constant = 1.31136903;
	double constant_linear = -2.67236013;
	double constant_quadratic = 1.50970306;
	return constant_constant + constant_linear*wp + constant_quadratic*pow(wp, 2);
}

static double bBkg_c_pt_piece_3_linear(double wp) {
	double linear_constant = -2.13367866e-3;
	double linear_linear = 44.34881246e-4;
	double linear_quadratic = -245.44615397e-5;
	return linear_constant + linear_linear*wp + linear_quadratic*pow(wp, 2);
}

static double bBkg_c_pt_piece_3(double pt, double wp) {
	return bBkg_c_pt_piece_3_constant(wp) + pt * bBkg_c_pt_piece_3_linear(wp);
}

//! Pt dependence of the efficiency to tag a c-jet as a b-jet
//! This function was fitted to Fig. 10(a), 14(a), 15(a), 16(a)
//! (c-jets) from ATL-PHYS-PUB-2015-022
// The result is a piecewise function, where each piece can change its
// shape with the working point, because the shape of the data points
// that were fitted changes with the working point.
static double bBkg_c_eff_pt(double pt, double wp) {
	double scale_factor = bBkg_c_scale_function(0.7) / bBkg_c_scale_function(wp);

	// The scaling of the 3 pieces only works for working points between
	// 60 and 85%. For working points outside of this range the pieces
	// are fixed to working points of either 60 or 85, and are scaled
	// with the scale factor.
	double wp_reg = fmax(0.6, fmin(0.85, wp));

	// Intersection points between the 3 different pieces
	double int12 = (bBkg_c_pt_piece_1_constant(wp_reg)-bBkg_c_pt_piece_2_constant(wp_reg)) / (bBkg_c_pt_piece_2_linear(wp_reg) - bBkg_c_pt_piece_1_linear(wp_reg));
	double int23 = (bBkg_c_pt_piece_2_constant(wp_reg)-bBkg_c_pt_piece_3_constant(wp_reg)) / (bBkg_c_pt_piece_3_linear(wp_reg) - bBkg_c_pt_piece_2_linear(wp_reg));

	if (pt < int12) {
		return scale_factor * bBkg_c_pt_piece_1(pt, wp_reg);
	} else if (pt < int23) {
		return scale_factor * bBkg_c_pt_piece_2(pt, wp_reg);
	} else {
		return scale_factor * bBkg_c_pt_piece_3(pt, wp_reg);
	}
}

//! Compute the efficiency to tag a c-jet as a b-jet
//!
//! \param pt The pt of the c jet
//! \param eta The pseudorapidity of the c jet
//! \param wp The b-tagger working point (between 0 and 1)
//!
//! \return The c-jet efficiency for this specific point
static double bBkg_c_eff(double pt, double eta, double wp) {
	// The scale_function and the "anomalous" scaling are necessary,
	// because the functions for pt and eta are both normalized to match the
	// results from the paper.
	// Their product therefore obviously is much to low, because each function
	// is (sort of) normalized to the correct efficiency, so the product is
	// (again sort of) proportional to the square of the correct normalization.
	return (bBkg_c_eff_pt(pt, wp) * bBkg_c_eff_eta(eta, wp)
			* bBkg_c_scale_function(wp)/bBkg_c_scale_function(0.7)
			* 7.5*(1+(100*wp-60)/130));
}

static double bBkg_l_scale_function(double wp) {
	double quadratic = -21.8718;
	double linear = 14.4912;
	double constant = 7.01694;
	return exp(quadratic*pow(wp, 2) + linear*wp + constant);
}

//! Dependence of the efficiency to tag a light-jet as a b-jet on the
//! pseudo rapidity
//! This function was fitted to Fig. 10(b), 14(b), 15(b), 16(b)
//! (Light-flavour jets) from ATL-PHYS-PUB-2015-022
static double bBkg_l_eff_eta(double eta, double wp) {
	double eta_quadratic = 6.77267369e-06;
	double eta_linear = -9.85698520e-06;
	double eta_constant = 2.79421560e-05;
	double wp_quadratic = -1.98927123e+02;
	double wp_linear = 2.49660023e+02;
	return (bBkg_l_scale_function(0.7) / bBkg_l_scale_function(wp)
			* (wp_quadratic*pow(wp, 2) + wp_linear*wp + 1)
			* (eta_quadratic*pow(eta, 2) + eta_linear*eta + eta_constant));
}

static double bBkg_l_pt_piece_1_constant(double wp) {
	double constant_constant = -0.00636633;
	double constant_linear = 0.02279642;
	double constant_quadratic = -0.01518515;
	return constant_constant + constant_linear*wp + constant_quadratic*pow(wp, 2);
}

static double bBkg_l_pt_piece_1_linear(double wp) {
	double linear_constant = 0.00038402;
	double linear_linear = -0.00102704;
	double linear_quadratic = 0.00068438;
	return linear_constant + linear_linear*wp + linear_quadratic*pow(wp, 2);
}

static double bBkg_l_pt_piece_1(double pt, double wp) {
	return bBkg_l_pt_piece_1_constant(wp) + (pt-30) * bBkg_l_pt_piece_1_linear(wp);
}

static double bBkg_l_pt_piece_2_constant(double wp) {
	double constant_constant = 2.69621921e-03;
	double constant_linear = -2.56834349e-03;
	double constant_quadratic = 2.97800726e-03;
	return constant_constant + constant_linear*wp + constant_quadratic*pow(wp, 2);
}

static double bBkg_l_pt_piece_2_linear(double wp) {
	double linear_constant = 2.25854439e-05;
	double linear_linear = -1.63662972e-05;
	double linear_quadratic = -2.63208760e-05;
	return linear_constant + linear_linear*wp + linear_quadratic*pow(wp, 2);
}

static double bBkg_l_pt_piece_2(double pt, double wp) {
	return bBkg_l_pt_piece_2_constant(wp) + pt * bBkg_l_pt_piece_2_linear(wp);
}

static double bBkg_l_pt_piece_3_constant(double wp) {
	double constant_constant = -4.30612912e-04;
	double constant_linear = 2.39758489e-03;
	return constant_constant + constant_linear*wp;
}

static double bBkg_l_pt_piece_3_linear(double wp) {
	double linear_constant = 3.52623884e-05;
	double linear_linear = -1.63456737e-05;
	double linear_quadratic = -3.04310741e-05;
	return linear_constant + linear_linear*wp + linear_quadratic*pow(wp, 2);
}

static double bBkg_l_pt_piece_3(double pt, double wp) {
	return bBkg_l_pt_piece_3_constant(wp) + pt * bBkg_l_pt_piece_3_linear(wp);
}

static double bBkg_l_pt_piece_4_constant(double wp) {
	double constant_constant = -5.38258976e-03;
	double constant_linear = 7.41930035e-03;
	return constant_constant + constant_linear*wp;
}

static double bBkg_l_pt_piece_4_linear(double wp) {
	double linear_constant = 1.13092746e-04;
	double linear_linear = -1.82133209e-04;
	double linear_quadratic = 6.20045880e-05;
	return linear_constant + linear_linear*wp + linear_quadratic*pow(wp, 2);
}

static double bBkg_l_pt_piece_4(double pt, double wp) {
	return bBkg_l_pt_piece_4_constant(wp) + pt * bBkg_l_pt_piece_4_linear(wp);
}

static double bBkg_l_pt_piece_5_constant(double wp) {
	double constant_constant = 0.0147271;
	double constant_linear = -0.01530835;
	return constant_constant + constant_linear*wp;
}

static double bBkg_l_pt_piece_5_linear(double wp) {
	double linear_constant = 1.42521897e-5;
	double linear_linear = -2.32021032e-5;
	double linear_quadratic = 0.73802226e-5;
	return linear_constant + linear_linear*wp + linear_quadratic*pow(wp, 2);
}

static double bBkg_l_pt_piece_5(double pt, double wp) {
	return bBkg_l_pt_piece_5_constant(wp) + pt * bBkg_l_pt_piece_5_linear(wp);
}

//! Pt dependence of the efficiency to tag a light jet as a b-jet
//! This function was fitted to Fig. 10(a), 14(a), 15(a), 16(a)
//! (Light-flavour jets) from ATL-PHYS-PUB-2015-022
// The result is a piecewise function, where each piece can change its
// shape with the working point, because the shape of the data points
// that were fitted changes with the working point.
static double bBkg_l_eff_pt(double pt, double wp) {
	double scale_factor = bBkg_l_scale_function(0.7) / bBkg_l_scale_function(wp);

	// The scaling of the 5 pieces only works for working points between
	// 60 and 85%. For working points outside of this range the pieces
	// are fixed to working points of either 60 or 85, and are scaled
	// with the scale factor.
	double wp_reg = fmax(0.6, fmin(0.85, wp));

	// Intersection points between the 5 different pieces
	double int12 = (bBkg_l_pt_piece_1_constant(wp_reg)-30*bBkg_l_pt_piece_1_linear(wp_reg)-bBkg_l_pt_piece_2_constant(wp_reg)) / (bBkg_l_pt_piece_2_linear(wp_reg) - bBkg_l_pt_piece_1_linear(wp_reg));
	double int23 = (bBkg_l_pt_piece_2_constant(wp_reg)-bBkg_l_pt_piece_3_constant(wp_reg)) / (bBkg_l_pt_piece_3_linear(wp_reg) - bBkg_l_pt_piece_2_linear(wp_reg));
	double int34 = (bBkg_l_pt_piece_3_constant(wp_reg)-bBkg_l_pt_piece_4_constant(wp_reg)) / (bBkg_l_pt_piece_4_linear(wp_reg) - bBkg_l_pt_piece_3_linear(wp_reg));
	double int45 = (bBkg_l_pt_piece_4_constant(wp_reg)-bBkg_l_pt_piece_5_constant(wp_reg)) / (bBkg_l_pt_piece_5_linear(wp_reg) - bBkg_l_pt_piece_4_linear(wp_reg));

	if (pt < int12) {
		return scale_factor*bBkg_l_pt_piece_1(pt, wp_reg);
	} else if (pt < int23) {
		return scale_factor*bBkg_l_pt_piece_2(pt, wp_reg);
	} else if (pt < int34) {
		return scale_factor*bBkg_l_pt_piece_3(pt, wp_reg);
	} else if (pt < int45) {
		return scale_factor*bBkg_l_pt_piece_4(pt, wp_reg);
	} else {
		return scale_factor*bBkg_l_pt_piece_5(pt, wp_reg);
	}
}

//! Compute the efficiency to tag a c-jet as a b-jet
//!
//! \param pt The pt of the c jet
//! \param eta The pseudorapidity of the c jet
//! \param wp The b-tagger working point (between 0 and 1)
//!
//! \return The c-jet efficiency for this specific point
static double bBkg_l_eff(double pt, double eta, double wp) {
	// The scale_function and the "anomalous" scaling are necessary,
	// because the functions for pt and eta are both normalized to match the
	// results from the paper.
	// Their product therefore obviously is much to low, because each function
	// is (sort of) normalized to the correct efficiency, so the product is
	// (again sort of) proportional to the square of the correct normalization.
	return (bBkg_l_eff_pt(pt, wp) * bBkg_l_eff_eta(eta, wp)
			* bBkg_l_scale_function(wp) / bBkg_l_scale_function(0.7)
			*75*6*(1+(100*wp-60)*0.003));
}

const AnalysisHandlerATLAS_13TeV_LLP::muonDetector AnalysisHandlerATLAS_13TeV_LLP::detMap[53][21] = {{cscla, endla, endla, endla, endla, endla, endla, trans, barla, barla, barla, barla, barla, trans, endla, endla, endla, endla, endla, endla, cscla},
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
const double AnalysisHandlerATLAS_13TeV_LLP::etaProj[21] = {-2.005, -1.955, -1.709, -1.411, -1.238, -1.163, -1.113, -0.965, -0.817, -0.519, 0.519, 0.817, 0.965, 1.113, 1.163, 1.238, 1.411, 1.709, 1.955, 2.005, 2.500};
const double AnalysisHandlerATLAS_13TeV_LLP::phiProj[53] = {3.142, 3.009, 2.948, 2.849, 2.629, 2.529, 2.471, 2.210, 2.174, 2.065, 1.855, 1.745, 1.695, 1.436, 1.386, 1.276, 1.066, 0.957, 0.916, 0.657, 0.596, 0.496, 0.277, 0.178, 0.142, -0.142, -0.178, -0.277, -0.496, -0.596, -0.657, -0.916, -0.957, -1.018, -1.066, -1.276, -1.357, -1.386, -1.436, -1.695, -1.745, -1.797, -1.855, -2.065, -2.134, -2.174, -2.210, -2.471, -2.529, -2.629, -2.849, -2.948, -3.009};

