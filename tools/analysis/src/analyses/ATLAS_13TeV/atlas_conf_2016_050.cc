#include "atlas_conf_2016_050.h"
#include "fastjet/tools/Filter.hh"
// AUTHOR: Junjie Cao, Liangliang Shang, Peiwen Wu, Jinmin Yang and Yang Zhang
//  EMAIL: junjiec@itp.ac.cn, shlwell1988@gmail.com, pwwu@kias.re.kr, jmyang@itp.ac.cn and zhangyang@itp.ac.cn 
void Atlas_conf_2016_050::initialize() {
  setAnalysisName("atlas_conf_2016_050");          
  setInformation(""
  "@#ATLAS\n"
  "@#ATLAS-CONF-2016-050\n"
  "@#1 lepton + jets + etmiss\n"
  "@#sqrt(s) = 13 TeV\n"
  "@#int(L) = 13.2 fb^-1\n"
  "");
  setLuminosity(13.2*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SR1;tN_high;bC2x_diag;bC2x_med;bCbv;");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  
//   f1.open("2016_050.txt");
// 
//   hfile = new TFile("2016_050.root", "RECREATE", "Saving Histograms");
// 
//   SR1_met = new TH1F("", "", 15, 170., 620.);
//   SR1_mt = new TH1F("", "", 15, 110., 560.);
// 
//   tN_high_met = new TH1F("", "", 15, 250., 1000.);
//   tN_high_mt = new TH1F("", "", 15, 100., 1600.);
// 
//   bC2x_diag_met = new TH1F("", "", 15, 140., 590.);
//   bC2x_diag_mt = new TH1F("", "", 15, 110., 560.);

}

void Atlas_conf_2016_050::analyze() {
  // Your eventwise analysis code goes here
  // The following objects are always defined unless they are 'ignored' above. They form std::vector objects of the respective Delphes class type (except for Etmiss which is a single object)
  // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
  // Within a std::vector, all members are ordered with highest pt coming first.

  // electronsLoose, electronsMedium, electronsTight   are list of electrons that passed respective efficiency and reconstruction cuts
  // muonsCombinedPlus, muonsCombined                  as above for muons
  // photons                                           as above for photons
  // jets are all reconstructed jets                   as above for jets. Note that electrons are most certainly also reconstructed as a jet -> overlap removal do avoid double counting necessary!
  // tracks, towers                                    calorimeter and tracker information. Usually not needed.
  // missingET                                         rec missing ET EXCLUDING muons.

  
  // Here is a couple of useful functions and lines:  
  //------------Phase Space Cuts (defined for jets, electronsXYZ, muonsXYZ, photonsXYZ)
  // jets = filterPhaseSpace(jets, 20., -2.8, 2.8)  // The vector 'jets' only contains jets with pt >= 20 GeV and -2.8 < eta < 2.8. This function is applicable to other particles too (electronsMedium, ... ).
  // jets = overlapRemoval(jets, electronsLoose, 0.2) Removes all jets for which there exists any electron in 'electronsLoose' with deltaR < 0.2.
  // jets = overlapRemoval(jets, 0.2) If two jets overlap within deltaR < 0.2, only the harder jet is stored.
  
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

  std::vector<Electron*> baselineElectrons = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47);
  std::vector<Muon*> baselineMuons = filterPhaseSpace(muonsCombined, 6., -2.6, 2.6);
  std::vector<Jet*> baselineJets = filterPhaseSpace(jets, 20., -2.5, 2.5);

  std::vector<Jet*> baselineTaus;
  for(int i=0; i<baselineJets.size(); i++) {
    if(checkTauTag(baselineJets[i], "loose")) {
      baselineTaus.push_back(baselineJets[i]);
    }
  }

  //overlap removal
  baselineJets = overlapRemoval(baselineJets, baselineElectrons, 0.2);
  
  baselineElectrons = overlapRemoval(baselineElectrons, baselineJets, 0.4);
  baselineMuons = overlapRemoval(baselineMuons, baselineJets, 0.4);

  baselineTaus = overlapRemoval(baselineTaus, baselineElectrons, 0.1);

  std::vector<Electron*> signalElectrons = filterPhaseSpace(baselineElectrons, 25., -2.47, 2.47);
  signalElectrons = filterIsolation(signalElectrons); 
  std::vector<Muon*> signalMuons = filterPhaseSpace(baselineMuons, 25., -2.6, 2.6);
  signalMuons = filterIsolation(signalMuons);

  std::vector<Jet*> signalJets = filterPhaseSpace(baselineJets, 25., -2.5, 2.5);
  std::vector<Jet*> notBTagSignalJets, BTagSignalJets;
  for(int i=0; i<signalJets.size(); i++) {
    if(checkBTag(signalJets[i])) {
      BTagSignalJets.push_back(signalJets[i]);
    }
    else {
      notBTagSignalJets.push_back(signalJets[i]);
    }
  }

  std::vector<Jet*> signalTaus = baselineTaus;

  //missing et trigger
  if ( missingET->P4().Et() < 200. ) return;
  countCutflowEvent("0_trigger_etmiss");

  //exactly one signal lepton and no additional baseline leptons 
  if ( signalElectrons.size() + signalMuons.size() != 1) return;
  if ( signalElectrons.size() + signalMuons.size() == 1) {
    if ( baselineElectrons.size() + baselineMuons.size() !=1 ) return;
  } 
  countCutflowEvent("1_lepton_onelepton");

  //construct finalobject composited of e or/and mu for convenience
  std::vector<FinalStateObject*> signalLeps;
  
  for ( int i = 0; i < signalElectrons.size(); i++ ) {        
    FinalStateObject* lep = newFinalStateObject(signalElectrons[i]);
    signalLeps.push_back(lep);    
  }

  for ( int i = 0; i < signalMuons.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(signalMuons[i]);
    signalLeps.push_back(lep); 
  }
  
  std::sort(signalLeps.begin(), signalLeps.end(), FinalStateObject::sortByPT); 
  
  //mT
  double mT = AnalysisBase::mT(signalLeps[0]->P4(), missingET->P4());
  if (mT < 30.) return;
  countCutflowEvent("2_mT");

  //at least two signal jets, and |∆φ(jeti, missingET)| > 0.4 for i ∈ {1,2} 
  if (signalJets.size() < 2.) return;
  for (int i=0; i<2; i++) {
    if (fabs(signalJets[i]->P4().DeltaPhi(missingET->P4())) < 0.4) return;
  }
  countCutflowEvent("3_jets");

  //veto events with a hadronic τ decay and mT2_tau < 80GeV
  //note: The hadronic tau veto is not applied to the bCbv SR
  //mT2_tau (normal mT2 with zero mass, target W's so ignore b-jets)
  double mT2_tau = 1.E4;
  if (notBTagSignalJets.size()>0) {
    mT2_tau = AnalysisBase::mT2(notBTagSignalJets[0]->P4(), signalLeps[0]->P4(), 0., missingET->P4());
  }

  bool tau_veto = false;
  if(mT2_tau < 80.) {
    tau_veto = true;
  }
  else {
    tau_veto = false;
  }

  //htmiss_sig
  //ref: atlas_1407_0583.cc
  double sig2=0;
  TLorentzVector htmiss = -signalLeps[0]->P4();
  for(int i = 0; i < signalJets.size(); i++){
    htmiss -= signalJets[i]->P4();
    if(signalJets[i]->PT <100.){
      sig2 += pow( signalJets[i]->PT*((signalJets[i]->PT-20)*(-0.02/80)+0.04), 2);
    }
    else{
      sig2 += pow( signalJets[i]->PT*0.02, 2);
    }
  }
  double htmiss_sig = (htmiss.Pt()-100.)/sqrt(sig2);

  //amt2
  //ref: atlas_1407_0583.cc
  double amT2 = 0.;
  if(BTagSignalJets.size() > 1) {
     double amT2_1 = AnalysisBase::mT2_bl(signalLeps[0]->P4(), BTagSignalJets[0]->P4(), BTagSignalJets[1]->P4(), missingET->P4());
     double amT2_2 = AnalysisBase::mT2_bl(signalLeps[0]->P4(), BTagSignalJets[1]->P4(), BTagSignalJets[0]->P4(), missingET->P4());
     amT2 = std::min(amT2_1,amT2_2);
  }
  else if(BTagSignalJets.size() == 1) {
  //else if(BTagSignalJets.size() == 1 and notBTagSignalJets.size() > 0) {
     double amT2_1 = AnalysisBase::mT2_bl(signalLeps[0]->P4(), BTagSignalJets[0]->P4(), notBTagSignalJets[0]->P4(), missingET->P4());
     double amT2_2 = AnalysisBase::mT2_bl(signalLeps[0]->P4(), notBTagSignalJets[0]->P4(), BTagSignalJets[0]->P4(), missingET->P4());
     amT2 = std::min(amT2_1,amT2_2);
  }

  //mtop_x also named hadronic top
  //ref: atlas_1407_0583.cc and appendix in arxiv:1407.0583
  double mtop_x = 1.E4;
 /* if(BTagSignalJets.size() > 1 and notBTagSignalJets.size() > 1) {
    double chi2[2];
    double mjj = (notBTagSignalJets[0]->P4()+notBTagSignalJets[1]->P4()).M();

    double r2_j1;
    if(notBTagSignalJets[0]->PT <100.){
      r2_j1 = pow( (notBTagSignalJets[0]->PT-20.)*(-0.02/80.)+0.04, 2);
    }
    else{
      r2_j1 = pow( 0.02, 2);
    }

    double r2_j2;
    if(notBTagSignalJets[1]->PT <100.){
      r2_j2 = pow( (notBTagSignalJets[1]->PT-20.)*(-0.02/80.)+0.04, 2);
    }
    else{
      r2_j2 = pow( 0.02, 2);
    }

    double sig2_jj = pow(mjj, 2)*(r2_j1+r2_j2);
    double mjjb[2];
    for(int i=0; i<2; i++) {
      mjjb[i] = (notBTagSignalJets[0]->P4()+notBTagSignalJets[1]->P4()+BTagSignalJets[i]->P4()).M();

      double r2_b;
      if(BTagSignalJets[i]->PT<100.) {
        r2_b = pow( (BTagSignalJets[i]->PT-20.)*(-0.02/80.)+0.04, 2);
      }
      else{
        r2_b = pow( 0.02, 2);
      }
      double sig2_jjb = pow(mjjb[i], 2)*(r2_j1+r2_j2+r2_b);
        
      chi2[i]=pow(mjjb[i]-172., 2)/sig2_jjb+pow(mjj-81., 2)/sig2_jj;
    }

    int btag_mtopx = (chi2[0]>=chi2[1]) ? 1 : 0;
    mtop_x =  mjjb[btag_mtopx];
    f1 << "mtop_x, " << mtop_x << std::endl;
  }
*/
  bool hadTop = false;
  TLorentzVector top_cand;
  //need at at least two non bjets to reconstruct hadronic tops    
  //f1 << "EVENT" << std::endl;
  if(notBTagSignalJets.size()>1){
    for(int i = 0; i < BTagSignalJets.size(); i++){
      for(int j = 0; j < notBTagSignalJets.size(); j++){ 
        for(int k = 0; k < notBTagSignalJets.size(); k++){
          if (j!=k ) { 
            top_cand = BTagSignalJets[i]->P4() + notBTagSignalJets[j]->P4() + notBTagSignalJets[k]->P4();
            //f1 << "top_cand.M()" << top_cand.M() << std::endl;
            if( top_cand.M()<270. ) {
              hadTop = true;
              break;
            }
          }
        }
      }
    } 
  } 
  //f1 << "hadTop, " << hadTop << std::endl;

  //for large radius jets with trimming
  //ref: atlas_1308_1841.cc
  //     fastjet.fr/repo/fastjet-doc-3.2.1.pdf
  //     iopscience.iop.org/article/10.1088/0954-3899/39/6/063001
  std::vector<fastjet::PseudoJet> particles;
  for ( int i = 0; i < signalJets.size(); i++ ) {
    double tmppx = signalJets[i]->P4().Px();
    double tmppy = signalJets[i]->P4().Py();
    double tmppz = signalJets[i]->P4().Pz();
    double tmpe  = signalJets[i]->P4().E();
    particles.push_back( fastjet::PseudoJet(tmppx, tmppy, tmppz, tmpe) );
  }

  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, 1.0);
  //fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, 1.2);
 
  fastjet::ClusterSequence cs(particles, jet_def);
  std::vector<fastjet::PseudoJet> largeRJets = sorted_by_pt(cs.inclusive_jets());
  std::vector<fastjet::PseudoJet> largeRJets_trimmed;
 
  double Rfilt = 0.3;
  fastjet::Selector selector_trimmer = fastjet::SelectorPtFractionMin(0.05);
  fastjet::Filter trimmer(Rfilt, selector_trimmer);
  for(int i=0; i<largeRJets.size(); i++) {
    fastjet::PseudoJet trimmed_jet = trimmer(largeRJets[i]);
    largeRJets_trimmed.push_back(trimmed_jet);
  }

/*    //mass of large radius jet in this analysis
  double LRJET_PT = 0., LRJET_M = 0.;
  if (largeRJets_trimmed.size()>0) {
    LRJET_PT = largeRJets_trimmed[0].pt();

    std::vector<fastjet::PseudoJet> constituents = largeRJets_trimmed[0].constituents();
    TLorentzVector smalljets;
    for (int i=0; i < constituents.size(); i++) {
      TLorentzVector p4(constituents[i].px(), constituents[i].py(),constituents[i].pz(),constituents[i].e());
      smalljets += p4;
    }
    LRJET_M = smalljets.M();
    f1 << "constituents.size, " << constituents.size() << std::endl;
    f1 << "smalljets.m, " << smalljets.M() << std::endl;
    f1 << "largejet.m, " << largeRJets_trimmed[0].m() << std::endl;
  }

 //topness
  double topnessVal=0.;
  if(BTagSignalJets.size()>1) {
    std::vector<topness::LorentzVector> bjets;
    std::vector<topness::LorentzVector> addjets;
    for(int i=0; i<2; i++) {
      topness::LorentzVector mvec(BTagSignalJets[i]->P4().Px(), BTagSignalJets[i]->P4().Py(),
                                 BTagSignalJets[i]->P4().Pz(), BTagSignalJets[i]->P4().E());
      bjets.push_back(mvec);
    }
   
    topness::LorentzVector MET(missingET->P4().Px(), missingET->P4().Py(), missingET->P4().Pz(), missingET->P4().E());
    topness::LorentzVector lep(signalLeps[0]->P4().Px(), signalLeps[0]->P4().Py(), signalLeps[0]->P4().Pz(), signalLeps[0]->P4().E());

    f1 << "addjets.size(), " << addjets.size() << std::endl;
    topnessVal = topness::CalcTopness(0., MET, lep, bjets, addjets);
    f1 << "topness, " << topnessVal << std::endl;
  } */

  double top_ness=0.;
  if(BTagSignalJets.size()>1) {
    top_ness = topness(signalLeps[0]->P4(), BTagSignalJets[0]->P4(), BTagSignalJets[1]->P4(), missingET->P4());
  }
  if(BTagSignalJets.size()==1 and notBTagSignalJets.size()>0) {
    top_ness = topness(signalLeps[0]->P4(), BTagSignalJets[0]->P4(), notBTagSignalJets[0]->P4(), missingET->P4());
    if(notBTagSignalJets.size()>1) {
      double tmptop_ness = topness(signalLeps[0]->P4(), BTagSignalJets[0]->P4(), notBTagSignalJets[1]->P4(), missingET->P4());
      if (tmptop_ness<top_ness) top_ness = tmptop_ness;
    }
  }

//***********************
//for signal region SR1**
//***********************
  if(not tau_veto) {
    countCutflowEvent("SR1_01_tauveto");
  if(signalJets.size()>=4 and BTagSignalJets.size()>=1) {
    countCutflowEvent("SR1_02_nJets");
    if(signalJets[0]->PT>80. and signalJets[1]->PT>50. and signalJets[2]->PT>40. and signalJets[3]->PT > 40.) {
      countCutflowEvent("SR1_03_JetsPT");
      if(missingET->P4().Et()>260.) {
        countCutflowEvent("SR1_04_etmiss");
        //no cuts for etmiss_vertical
        countCutflowEvent("SR1_05_no");
        if(htmiss_sig>14.) {
          countCutflowEvent("SR1_06_htmiss");
          if(mT>170.) {
            countCutflowEvent("SR1_07_mt");
            if(amT2>175.) {
              countCutflowEvent("SR1_08_amt2");
              if(top_ness>6.5) { 
                countCutflowEvent("SR1_09_topness");
                //if(mtop_x<270.) {
                if(hadTop) {
                  countCutflowEvent("SR1_10_mtopx");
                  double dr = BTagSignalJets[0]->P4().DeltaR(signalLeps[0]->P4());
                  if(dr<3.0) {
                    countCutflowEvent("SR1_11_dr");
                    //no cuts for large-R jet pt
                    //no cuts for large-R jet mass
                    //no cuts for large-R jet deltaPhi
                    countCutflowEvent("SR1_12_no");
                    countCutflowEvent("SR1_13_no");
                    countCutflowEvent("SR1_14_no");
                    countSignalEvent("SR1");

                    //SR1_met->Fill(missingET->P4().Et()); 
                    //SR1_mt->Fill(mT); 
                  }//end if(dr)
                }//end if(mtop_x)
              }//end if(topness)
            }//end if(amT2)
          }//end if(mT)
        }//end if(htmiss_sig)
      }//end if(missingET)
    }//end if(signalJets[1,2,3,4]->PT)
  }//end if(signalJets.size()>=4)
  }//end if(not tau_veto)
//***********************
//end signal region SR1**
//***********************

//***************************
//for signal region tN_high**
//***************************
  if(not tau_veto) {
  countCutflowEvent("tN_high_01_tauVeto");
  if(signalJets.size()>=4 and BTagSignalJets.size()>=1) {
    countCutflowEvent("tN_high_02_nJets");
    if(signalJets[0]->PT>120. and signalJets[1]->PT>80. and signalJets[2]->PT>50. and signalJets[3]->PT > 20.) {
      countCutflowEvent("tN_high_03_JetsPT");
      if(missingET->P4().Et()>450.) {
        countCutflowEvent("tN_high_04_etmiss");
        //etmiss_vertical coming soon
          countCutflowEvent("tN_high_05_etmissVcal");
          if(htmiss_sig>22.) {
            countCutflowEvent("tN_high_06_htmiss");
            if(mT>210.) {
              countCutflowEvent("tN_high_07_mt");
              if(amT2>175.) {
                countCutflowEvent("tN_high_08_amt2");
                //no cut for topness
                //no cut for mtop_x
                countCutflowEvent("tN_high_09_no");
                countCutflowEvent("tN_high_10_no");

                double dr = BTagSignalJets[0]->P4().DeltaR(signalLeps[0]->P4());
                if(dr<2.4) {
                  countCutflowEvent("tN_high_11_dR");
                  if(largeRJets_trimmed.size()>0 and largeRJets_trimmed[0].pt()>290.) {
                    countCutflowEvent("tN_high_12_LRJET_PT");
                    if(largeRJets_trimmed[0].m()>70.) {
                      countCutflowEvent("tN_high_13_LRJET_M");
                      double dphi = 0.;
                      if(largeRJets_trimmed.size()>1) { 
                        TVector2 v1(largeRJets_trimmed[1].px(), largeRJets_trimmed[1].py());
                        TVector2 v2(missingET->P4().Px(), missingET->P4().Py());
                        dphi=v1.DeltaPhi(v2);
                      }

                      //if(dphi>0.6) {
                      if(fabs(dphi)>0.6) {
                        countCutflowEvent("tN_high_14_dphi");
                        countSignalEvent("tN_high");

                        //tN_high_met->Fill(missingET->P4().Et()); 
                        //tN_high_mt->Fill(mT); 
                      }//end if(large-R jet dphi)
                    }//end if(large-R jet mass)
                  }//end if(large-R jet pt)                  
                }//end if(dr)
              }//end if(amT2)
            }//end if(mT)
          }//end if(htmiss_sig)
      }//end if(missingET)
    }//end if(signalJets[1,2,3,4]->PT)
  }//end if(signalJets.size()>=4)
  }//end if(not tau_veto)
//***************************
//end signal region tN_high**
//***************************

//*****************************
//for signal region bC2x_diag**
//*****************************
  if(not tau_veto) {
  countCutflowEvent("bC2x_diag_01_tauveto");
  if(signalJets.size()>=4 and BTagSignalJets.size()>=2) {
    countCutflowEvent("bC2x_diag_02_nJets");
    if(signalJets[0]->PT>70. and signalJets[1]->PT>60. and signalJets[2]->PT>55. and signalJets[3]->PT > 25.) {
      countCutflowEvent("bC2x_diag_03_JetsPT");
      if(BTagSignalJets[0]->PT>25. and BTagSignalJets[1]->PT>25.) {
        countCutflowEvent("bC2x_diag_04_bJetsPT");
        if(missingET->P4().Et()>230.) {
          countCutflowEvent("bC2x_diag_05_etmiss");
          if(htmiss_sig>14.) {
            countCutflowEvent("bC2x_diag_06_htmiss");
            if(mT>170.) {
              countCutflowEvent("bC2x_diag_07_mt");
              if(amT2>175.) {
                countCutflowEvent("bC2x_diag_08_amt2");
                if(fabs(signalJets[0]->P4().DeltaPhi(missingET->P4())) > 1.2) {
                  countCutflowEvent("bC2x_diag_09_dphi1");
                  if(fabs(signalJets[1]->P4().DeltaPhi(missingET->P4())) > 0.8) {
                    countCutflowEvent("bC2x_diag_10_dphi2");
                    //no cuts for large-R jet mass
                    //no cuts for deltaPhi of lepton
                    countCutflowEvent("bC2x_diag_11_no");
                    countCutflowEvent("bC2x_diag_12_no");

                    countSignalEvent("bC2x_diag");

                    //bC2x_diag_met->Fill(missingET->P4().Et());
                    //bC2x_diag_mt->Fill(mT);
                  }//end if(dphi_jet2)
                }//end if(dphi_jet1)
              }//end if(amT2)
            }//end if(mT)
          }//end if(htmiss_sig)
        }//end if(missingET)
      }//end if(BTagSignalJets[1,2]->PT)
    }//end if(signalJets[1,2,3,4]->PT)
  }//end if(signalJets.size()>=4)
  }//end if(not tau_veto)
//*****************************
//end signal region bC2x_diag**
//*****************************

//****************************
//for signal region bC2x_med**
//****************************
  if(not tau_veto) {
  countCutflowEvent("bC2x_med_01_tauveto");
  if(signalJets.size()>=4 and BTagSignalJets.size()>=2) {
    countCutflowEvent("bC2x_med_02_nJets");
    if(signalJets[0]->PT>170. and signalJets[1]->PT>110. and signalJets[2]->PT>25. and signalJets[3]->PT > 25.) {
      countCutflowEvent("bC2x_med_03_JetsPT");
      if(BTagSignalJets[0]->PT>105. and BTagSignalJets[1]->PT>100.) {
        countCutflowEvent("bC2x_med_04_bJetsPT");
        if(missingET->P4().Et()>210.) {
          countCutflowEvent("bC2x_med_05_etmiss");
          if(htmiss_sig>7.) {
            countCutflowEvent("bC2x_med_06_htmiss");
            if(mT>140.) {
              countCutflowEvent("bC2x_med_07_mt");
              if(amT2>210.) {
                countCutflowEvent("bC2x_med_08_amt2");
                if(fabs(signalJets[0]->P4().DeltaPhi(missingET->P4())) > 1.0) {
                  countCutflowEvent("bC2x_med_09_dphi1");
                  if(fabs(signalJets[1]->P4().DeltaPhi(missingET->P4())) > 0.8) {
                    countCutflowEvent("bC2x_med_10_dphi2");
                    //no cuts for large-R jet mass
                    //no cuts for deltaPhi of lepton
                    countCutflowEvent("bC2x_med_11_no");
                    countCutflowEvent("bC2x_med_12_no");

                    countSignalEvent("bC2x_med");
                  }//end if(dphi_jet2)
                }//end if(dphi_jet1)
              }//end if(amT2)
            }//end if(mT)
          }//end if(htmiss_sig)
        }//end if(missingET)
      }//end if(BTagSignalJets[1,2]->PT)
    }//end if(signalJets[1,2,3,4]->PT)
  }//end if(signalJets.size()>=4)
  }//end if(not tau_veto)
//****************************
//end signal region bC2x_med**
//****************************

//************************
//for signal region bCbv**
//************************
  //no cut of tau_veto
  countCutflowEvent("bCbv_01_no");
  if(signalJets.size()>=2 and BTagSignalJets.size()==0) {
    countCutflowEvent("bCbv_02_nJets");
    if(signalJets[0]->PT>120. and signalJets[1]->PT>80.) {
      countCutflowEvent("bCbv_03_JetsPT");
      //no cut for Bjets pt 
      countCutflowEvent("bCbv_04_no");

      if(missingET->P4().Et()>360.) {
        countCutflowEvent("bCbv_05_etmiss");
        if(htmiss_sig>16.) {
          countCutflowEvent("bCbv_06_htmiss");
          if(mT>200.) {
            countCutflowEvent("bCbv_07_mt");
            //no cut for amT2
            countCutflowEvent("bCbv_08_no");

            if(fabs(signalJets[0]->P4().DeltaPhi(missingET->P4())) > 2.0) {
              countCutflowEvent("bCbv_09_dphi1");
              if(fabs(signalJets[1]->P4().DeltaPhi(missingET->P4())) > 0.8) {
                countCutflowEvent("bCbv_10_dphi2");
                if(largeRJets_trimmed.size() > 0 and 
                   largeRJets_trimmed[0].m()>70. and largeRJets_trimmed[0].m()<100.) {
                  countCutflowEvent("bCbv_11_LRJET_M");
                  if(fabs(signalLeps[0]->P4().DeltaPhi(missingET->P4())) > 1.2) {
                    countCutflowEvent("bCbv_12_dphi3");
                    countSignalEvent("bCbv");
                  }//end if(dphi_lepton)
                }//end if(large R jet mass)
              }//end if(dphi_jet2)
            }//end if(dphi_jet1)
          }//end if(mT)
        }//end if(htmiss_sig)
      }//end if(missingET)
    }//end if(signalJets[1,2]->PT)
  }//end if(signalJets.size()>=4)
//************************
//end signal region bCbv**
//************************

}//end void Atlas_conf_2016_050::analyze()



void Atlas_conf_2016_050::finalize() {
  // Whatever should be done after the run goes here
//   f1.close();
// 
//   //SR1
//   TCanvas can1;
//   SR1_met->Draw();
//   SR1_met->SetTitle("SR1_missing_et");
//   can1.Close();
// 
//   TCanvas can2;
//   SR1_mt->Draw();
//   SR1_mt->SetTitle("SR1_mt");
//   can2.Close();
// 
//   //tN_high
//   TCanvas can3;
//   tN_high_met->Draw();
//   tN_high_met->SetTitle("tN_high_missing_et");
//   can3.Close();
// 
//   TCanvas can4;
//   tN_high_mt->Draw();
//   tN_high_mt->SetTitle("tN_high_mt");
//   can4.Close();
// 
//   //bC2x_diag
//   TCanvas can5;
//   bC2x_diag_met->Draw();
//   bC2x_diag_met->SetTitle("bC2x_diag_missing_et");
//   can5.Close();
// 
//   TCanvas can6;
//   bC2x_diag_mt->Draw();
//   bC2x_diag_mt->SetTitle("bC2x_diag_mt");
//   can6.Close();
// 
//   hfile->Write();
//   hfile->Close();
}       
