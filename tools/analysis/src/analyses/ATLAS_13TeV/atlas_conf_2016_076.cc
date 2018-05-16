#include "atlas_conf_2016_076.h"
// AUTHOR: Junjie Cao, Krzysztof Rolbiecki, Liangliang Shang, Jamie Tattersall, Peiwen Wu, JinMin Yang, Yuanfang Yue and Yang Zhang
// EMAIL: junjiec@itp.ac.cn, krzysztof.rolbiecki@desy.de, shlwell1988@gmail.com, tattersall@physik.rwth-aachen.de, pwwu@kias.re.kr, jmyang@itp.ac.cn and zhangyang@itp.ac.cn 
void Atlas_conf_2016_076::initialize() {
  setAnalysisName("atlas_conf_2016_076");          
  setInformation(""
  "@#ATLAS\n"
  "@#ATLAS-CONF-2016-076\n"
  "@#2 leptons + jets + etmiss\n"
  "@#sqrt(s) = 13 TeV\n"
  "@#int(L) = 13.3 fb^-1\n"
  "");
  setLuminosity(13.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SR_low;SR_high;SR_w_sf;SR_w_df;SR_t_sf;SR_t_df;");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
/*  f1.open("2016_076.txt");

  hfile = new TFile("2016_076.root", "RECREATE", "Saving Histograms");
  
  low_mt2bb = new TH1F("", "", 20, 0., 500.);
  high_etbl = new TH1F("", "", 20, 0., 500.);
  w_sf_mrdel = new TH1F("", "", 6, 80., 180.);
  t_sf_mrdel = new TH1F("", "", 7, 70., 180.);
  w_df_mrdel = new TH1F("", "", 6, 80., 180.);
  t_df_mrdel = new TH1F("", "", 7, 70., 180.);
*/
}

void Atlas_conf_2016_076::analyze() {
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

  std::vector<Electron*> baselineElectrons = filterPhaseSpace(electronsMedium, 7., -2.47, 2.47);
  std::vector<Muon*> baselineMuons = filterPhaseSpace(muonsCombined, 5., -2.5, 2.5);
  std::vector<Jet*> baselineJets = filterPhaseSpace(jets, 20., -2.8, 2.8);

  //overlap remove
  //baselineElectrons = overlapRemoval(baselineElectrons, baselineMuons, 0.4);

  baselineJets = overlapRemoval(baselineJets, baselineElectrons, 0.2);
  //baselineJets = overlapRemoval(baselineJets, baselineMuons, 0.4);

  baselineElectrons = overlapRemoval(baselineElectrons, baselineJets, 0.4);
  baselineMuons = overlapRemoval(baselineMuons, baselineJets, 0.4);

  //construct finalobject of leptons
  std::vector<Electron*> signalElectrons;
  signalElectrons = filterIsolation(baselineElectrons);
  std::vector<Muon*> signalMuons;
  signalMuons = filterIsolation(baselineMuons);

  std::vector<FinalStateObject*> signalLeps;
  for ( int i = 0; i <  signalElectrons.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(signalElectrons[i]);
    signalLeps.push_back(lep);
  }
  for ( int i = 0; i < signalMuons.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(signalMuons[i]);
    signalLeps.push_back(lep);
  }
  std::sort(signalLeps.begin(), signalLeps.end(), FinalStateObject::sortByPT);

  //signal jets and b-taggins jets
  std::vector<Jet*> signalJets = baselineJets;
  std::vector<Jet*> notBTagSignalJets, BTagSignalJets;
  for(int i=0; i<signalJets.size(); i++) {
    if(checkBTag(signalJets[i])) {
      BTagSignalJets.push_back(signalJets[i]);
    }
    else {
      notBTagSignalJets.push_back(signalJets[i]);
    }
  }

  countCutflowEvent("00_all");

  //trigger
  bool trigger_3body_hadmt2 = false, trigger_hadmt2 = false;
  if( signalLeps.size()==2 ) {
    countCutflowEvent("01_2leptons");
    if (signalLeps[0]->Charge*signalLeps[1]->Charge<0) {
      if ( (signalLeps[0]->P4()+signalLeps[1]->P4()).M()>20. ) {
        if (signalLeps[0]->PT>25. and signalLeps[1]->PT>20. and missingET->P4().Et()>100.) trigger_3body_hadmt2 = true;
        if (signalLeps[0]->PT>10. and signalLeps[1]->PT>10. and missingET->P4().Et()>200.) trigger_hadmt2 = true;
      }
    }
  }

  if (trigger_3body_hadmt2 or trigger_hadmt2) {

    countCutflowEvent("02_trigger");

    //variables about super-razor
    double szor_r, szor_inv_r, szor_dphi, szor_m;
    std::vector<double> szor;
    szor = superRazor(signalLeps[0]->P4(), signalLeps[1]->P4(), missingET->P4());
    szor_r=szor[0];
    szor_inv_r=szor[1];
    szor_dphi=szor[2];
    szor_m=szor[3];

    //mt2_ll and mt2_bb
    double mT2_ll = AnalysisBase::mT2(signalLeps[0]->P4(), signalLeps[1]->P4(), 0., missingET->P4());
    double mT2_bb = 0.;
    TLorentzVector vec = signalLeps[0]->P4() + signalLeps[1]->P4() + missingET->P4();
    if(BTagSignalJets.size()>1) {
      mT2_bb = AnalysisBase::mT2(BTagSignalJets[0]->P4(), BTagSignalJets[1]->P4(), 0., vec);
    }

    //ET_bl_min
    double ET_bl=0.;
    if(BTagSignalJets.size()==2) {
      int iLoop=0;
      double chi2[4];
      for(int i=0; i<signalLeps.size(); i++) {
        for(int j=0; j<BTagSignalJets.size(); j++) {
          TLorentzVector vec = signalLeps[i]->P4() + BTagSignalJets[j]->P4();   
          chi2[iLoop] = pow(vec.E(),2) - pow(vec.Pz(),2);
          iLoop++;
        }
      }
      ET_bl = std::min(chi2[2], chi2[3]);
      ET_bl = std::min(chi2[1], ET_bl);
      ET_bl = std::min(chi2[0], ET_bl);
      ET_bl = sqrt(ET_bl);
    }

    //dphi_boost and ptboost_ll
    double dphi_boost = missingET->P4().DeltaPhi( missingET->P4() + signalLeps[0]->P4() + signalLeps[1]->P4() );
   
    //meff
    double meff=0.;
    if (signalJets.size()>1) {
      meff = missingET->P4().Et() + signalLeps[0]->PT + signalLeps[1]->PT + signalJets[0]->PT + signalJets[1]->PT;
    }

    //cos_theta_b
    TLorentzVector ll_lab = signalLeps[0]->P4() + signalLeps[1]->P4();
    TVector3 v1 = -ll_lab.BoostVector();
    TLorentzVector l0 = signalLeps[0]->P4();
    TLorentzVector l1 = signalLeps[1]->P4();
    l0.Boost(v1);
    l1.Boost(v1);

    //f1 << "l0.costheta, " << l0.CosTheta() << std::endl;
    //f1 << "l1.costheta, " << l1.CosTheta() << std::endl;
 
    double cos_theta_b = l0.CosTheta();

//****************************************************
//for signal region SR_hadMT2_low and SR_hadMT2_high**
//****************************************************
     if(true) {
      
      bool offZ = true;
      if(signalLeps[0]->Type == signalLeps[1]->Type) {
        double mll = (signalLeps[0]->P4() + signalLeps[1]->P4()).M();
        if (fabs(mll - 91.)<20.) offZ = false;
      }

      if(offZ) {
        countCutflowEvent("SR_low_01_offZ");
        countCutflowEvent("SR_high_01_offZ");
        if(BTagSignalJets.size()==2) {
          countCutflowEvent("SR_low_02_nbjets");
          countCutflowEvent("SR_high_02_nbjets");
          if(mT2_bb>220.) {
            countCutflowEvent("SR_low_03_mt2bb");
            countCutflowEvent("SR_high_03_mt2bb");
            if(signalLeps[0]->PT<120.) {
              countCutflowEvent("SR_low_04_ptlep1");
              countCutflowEvent("SR_high_04_ptlep1");

              countCutflowEvent("SR_low_05_no");
              countSignalEvent("SR_low");

              //low_mt2bb->Fill(mT2_bb);

              if(ET_bl>180.) {
                countCutflowEvent("SR_high_05_etbl");
                countSignalEvent("SR_high");

                //high_etbl->Fill(ET_bl);
              }
            }//end if(leading lepton PT)
          }//end if(mT2_bb)
        }//enf if(b-jets size)
      }//end if(offZ)
    }//end if(true)
//****************************************************
//end signal region SR_hadMT2_low and SR_hadMT2_high**
//****************************************************

//****************************
//for signal region SR_3body**
//****************************
    if(trigger_3body_hadmt2) {
      if(signalLeps[0]->Type == signalLeps[1]->Type) {
        bool mll = (signalLeps[0]->P4() + signalLeps[1]->P4()).M();
        if (fabs(mll - 91.)>10.) {
          countCutflowEvent("SR_w_sf_01_offZ");
          countCutflowEvent("SR_t_sf_01_offZ");
          if(szor_r > 0.5) {
            countCutflowEvent("SR_w_sf_02_rpt");
            countCutflowEvent("SR_t_sf_02_rpt");
            if(szor_inv_r>0.8) {
              countCutflowEvent("SR_w_sf_03_invr");
              countCutflowEvent("SR_t_sf_03_invr");
              if(szor_dphi > 0.85*fabs(cos_theta_b) + 1.8) {
                countCutflowEvent("SR_w_sf_04_dphi");
                countCutflowEvent("SR_t_sf_04_dphi");
                if(BTagSignalJets.size()==0) {
                  countCutflowEvent("SR_w_sf_05_nbjets");
                  if(szor_m>95.) {
                    countCutflowEvent("SR_w_sf_06_mdr");

                    countSignalEvent("SR_w_sf");
                   
                    //w_sf_mrdel->Fill(szor_m);
                  }
                } else {
                  countCutflowEvent("SR_t_sf_05_nbjets");
                  if(szor_m>110.) {
                    countCutflowEvent("SR_t_sf_06_mdr");

                    countSignalEvent("SR_t_sf");

                    //t_sf_mrdel->Fill(szor_m);
                  }
                }//end if(wheter sr_w or sr_t) 
              }//end if(szor_dphi)
            }//end if(inverse r)
          }//end if(szor_r)
        }//end if(offZ)

      } else {

        //no cut for offZ
        countCutflowEvent("SR_w_df_01_no");
        countCutflowEvent("SR_t_df_01_no");
        if(szor_r > 0.5) {
          countCutflowEvent("SR_w_df_02_rpt");
          countCutflowEvent("SR_t_df_02_rpt");
          if(szor_inv_r>0.8) {
            countCutflowEvent("SR_w_df_03_invr");
            countCutflowEvent("SR_t_df_03_invr");
            if(szor_dphi > 0.85*fabs(cos_theta_b) + 1.8) {
              countCutflowEvent("SR_w_df_04_dphi");
              countCutflowEvent("SR_t_df_04_dphi");
              if(BTagSignalJets.size()==0) {
                countCutflowEvent("SR_w_df_05_njets");
                if(szor_m>95.) {
                  countCutflowEvent("SR_w_df_06_mdr");

                  countSignalEvent("SR_w_df");

                  //w_df_mrdel->Fill(szor_m);
                }
              } else {
                countCutflowEvent("SR_t_df_05_njets");
                if(szor_m>110.) {
                  countCutflowEvent("SR_t_df_06_mdr");

                  countSignalEvent("SR_t_df");

                  //t_df_mrdel->Fill(szor_m);
                }
              }//end if(wheter sr_w or sr_t) 
            }//end if(szor_dphi)
          }//end if(inverse r)
        }//end if(szor_r)

      }//end if(whether sf or df)
    }//end if(true)
//****************************
//for signal region SR_3body**
//****************************

  }//end if(trigger)

} //end void Atlas_conf_2016_076::analyze()

void Atlas_conf_2016_076::finalize() {
  // Whatever should be done after the run goes here
/*  f1.close();

  TCanvas can1;
  low_mt2bb->Draw();
  low_mt2bb->SetTitle("low_mt2bb");
  can1.Close();

  TCanvas can2;
  high_etbl->Draw();
  high_etbl->SetTitle("high_etbl");
  can2.Close();

  TCanvas can3;
  w_sf_mrdel->Draw();
  w_sf_mrdel->SetTitle("w_sf_mrdel");
  can3.Close();

  TCanvas can4;
  t_sf_mrdel->Draw();
  t_sf_mrdel->SetTitle("t_sf_mrdel");
  can4.Close();

  TCanvas can5;
  w_df_mrdel->Draw();
  w_df_mrdel->SetTitle("w_df_mrdel");
  can5.Close();

  TCanvas can6;
  t_df_mrdel->Draw();
  t_df_mrdel->SetTitle("t_df_mrdel");
  can6.Close();

  hfile->Write();

  hfile->Close(); */
}


