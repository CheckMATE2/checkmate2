#include "atlas_1501_07110.h"
// AUTHOR: Junjie Cao, Liangliang Shang, Peiwen Wu, Jin Min Yang, Yang Zhang
//  EMAIL: junjiec@itp.ac.cn, shlwell1988@gmail.com, peiwen.wu123@gmail.com, jmyang@itp.ac.cn, zhangyang@itp.ac.cn

static const double PI=3.1415926;

void Atlas_1501_07110::initialize() {
  setAnalysisName("atlas_1501_07110");          
  setInformation(""
    "# ATLAS\n"
     "# arXiv:1501.07110\n"
     "# an isolated lepton, 125GeV Higgs and missing Et\n"
     "# sqrt(s) = 8 TeV\n"
     "# int(L) = 20.3 fb^-1\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SRlbb-onHiggs-1;SRlbb-onHiggs-2;SRlbb-sidebands-1;SRlbb-sidebands-2;SRlrr-1;SRlrr-2;SRee-1;SRee-2;SRmm-1;SRmm-2;SRem-1;SRem-2;");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  bookCutflowRegions("SRlbb-01-1lep2bjets;SRlbb-02-missingET;SRlbb-03-mct;SRlbb-04-mtw;SRlbb-05-mbb;SRlbb-06-onHiggs-1;SRlbb-08-onHiggs-2;SRlbb-07-sidebands-1;SRlbb-09-sidebands-2;SRlrr-01-leprr;SRlrr-02-mrr;SRlrr-03-et;SRlrr-04-dphi;SRlrr-05-1;SRlrr-06-2;SRll-ee-01-sameSign;SRll-ee-02-veto1;SRll-ee-03-jets;SRll-ee-04-veto2;SRll-ee-05-pt;SRll-ee-06-deltaEta;SRll-ee-07-etRel;SRll-ee-08-meff;SRll-ee-09-mt;SRll-ee-10-mass;SRll-mm-01-sameSign;SRll-mm-02-veto1;SRll-mm-03-jets;SRll-mm-04-veto2;SRll-mm-05-pt;SRll-mm-06-deltaEta;SRll-mm-07-etRel;SRll-mm-08-meff;SRll-mm-09-mt;SRll-mm-10-mass;SRll-em-01-sameSign;SRll-em-02-veto1;SRll-em-03-jets;SRll-em-04-veto2;SRll-em-05-pt;SRll-em-06-deltaEta;SRll-em-07-etRel;SRll-em-08-meff;SRll-em-09-mt;SRll-em-10-mass;"); 
  
/* //for testing
  file1.open("1501_07110.debug");

  hist1 = new TH1F("hist1","delta R for muons", 100, 0., 10.);
  hist2 = new TH1F("hist2", "validation for higgs to a a", 100, 20., 300.);

  hist3 = new TH1F("hist3", "electrons pt", 50, 10., 200.);
  hist4 = new TH1F("hist4", "muons pt", 50, 10., 200.);
*/
}

void Atlas_1501_07110::analyze() {
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
 
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47);

  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 10., -2.5, 2.5);

  photons = filterPhaseSpace(photons, 20., -2.37, 2.37, true);

  std::vector<Jet*> preJets;
  for(int i=0; i<jets.size(); i++) {
    if(fabs(jets[i]->Eta)<2.4 && jets[i]->PT > 20.) preJets.push_back(jets[i]);
    if(fabs(jets[i]->Eta)>2.4 && fabs(jets[i]->Eta)<4.5 && jets[i]->PT > 30.) preJets.push_back(jets[i]);
  }
  std::sort(preJets.begin(), preJets.end(), sortPT());
  
  /* //checking sortPT() effective or not
  for(int i=0; i<jets.size(); i++) {
    if(jets[i]->PT > 20. and jets[i]->PT < 50.) preJets.push_back(jets[i]);
  }
  for(int i=0; i<jets.size(); i++) {
    if(jets[i]->PT > 60. and jets[i]->PT < 100.) preJets.push_back(jets[i]);
  }
  std::sort(preJets.begin(), preJets.end(), sortPT());

  for(int i=0; i< preJets.size(); i++) {
    file1 << preJets[i]->PT << "  ";
  }
  file1 << std::endl;
  */ 

//...........event selection...........
//--- one lepton and two b-jets channel ---
  bool trigger_onelepton_2bjets = false;
  for (int i=0; i<electronsMedium.size(); i++) {
    if ( electronsMedium[i]->PT > 25 ) trigger_onelepton_2bjets = true;

  }
  for (int i=0; i<muonsCombinedPlus.size(); i++) {
    if (muonsCombinedPlus[i]->PT > 25) trigger_onelepton_2bjets = true;

  }

  if( trigger_onelepton_2bjets ) {
    std::vector<Electron*> signalElectrons = filterPhaseSpace(electronsTight, 25., -2.47, 2.47);
    std::vector<Muon*> signalMuons = filterPhaseSpace(muonsCombinedPlus, 25., -2.5, 2.5);

    //overlap removal
    signalElectrons = overlapRemoval(signalElectrons, 0.1);
    preJets = overlapRemoval(preJets, signalElectrons, 0.2);
    signalElectrons = overlapRemoval(signalElectrons, preJets, 0.4);
    signalMuons = overlapRemoval(signalMuons, preJets, 0.4);
    //for both removal on different kinds of objects
    std::vector<Electron*> tempEle = signalElectrons;
    signalElectrons = overlapRemoval(signalElectrons, signalMuons, 0.1);
    signalMuons = overlapRemoval(signalMuons, tempEle, 0.1);
    //for both removal on the same kind of objects
    signalMuons = overlapRemoval(signalMuons, 0.05, true);

    /* //check overlapRemoval both for same kind of objects effective or not
    signalMuons = overlapRemoval(signalMuons, 2.5, true);
    for(int i=0; i<signalMuons.size(); i++) {
      for(int j=signalMuons.size()-1; j>i; j--) {
        hist1->Fill( signalMuons[i]->P4().DeltaR( signalMuons[j]->P4() ) );
      } 
    }
    */

    //isolation
    std::vector<int> isoFlags;
    isoFlags.push_back(0);
    isoFlags.push_back(1);
    signalElectrons = filterIsolation(signalElectrons, isoFlags);
    signalMuons = filterIsolation(signalMuons, isoFlags);

    if (signalElectrons.size()+signalMuons.size()==1) {

      double triggerRatio = (double) rand() / (double) (RAND_MAX + 1.);

      if( (signalElectrons.size()==1 && triggerRatio > 0.2) 
          or (signalMuons.size()==1 && triggerRatio > 0.25) ) {      

      std::vector<Jet*> signalJets;
      for(int i=0; i<preJets.size(); i++) {
        if (fabs(preJets[i]->Eta)<2.4 && preJets[i]->PT > 25.) signalJets.push_back(preJets[i]);
        if (fabs(preJets[i]->Eta)>2.4 && fabs(preJets[i]->Eta) < 4.9 && preJets[i]->PT > 30.)
          signalJets.push_back(preJets[i]);
      }
      std::sort(signalJets.begin(), signalJets.end(), sortPT());

      if(signalJets.size()==2 or signalJets.size() == 3) {
        bool BTag = false;
        if (checkBTag(signalJets[0], 0) and fabs(signalJets[0]->Eta)<2.4 ) {
          if(checkBTag(signalJets[1], 0) and fabs(signalJets[1]->Eta)<2.4)
            BTag = true;
        }
        if( BTag ) {

          countCutflowEvent("SRlbb-01-1lep2bjets");

          if(missingET->P4().Et()>100.) {

            countCutflowEvent("SRlbb-02-missingET");

            double mct = mCT(signalJets[0]->P4(), signalJets[1]->P4());
            if(mct>160.) {

              countCutflowEvent("SRlbb-03-mct");

              double mT_w;
              if(signalElectrons.size()==1)
                mT_w = mT(signalElectrons[0]->P4(), missingET->P4(), 0.);
              else
                mT_w = mT(signalMuons[0]->P4(), missingET->P4(), 0.);
              double mbb=(signalJets[0]->P4()+signalJets[1]->P4()).M();
              if(mT_w>100.) {

                countCutflowEvent("SRlbb-04-mtw");

                if(mbb>45. and mbb<195.) {

                  countCutflowEvent("SRlbb-05-mbb");

                  if(mT_w>100. and mT_w<130.) {
                    if(mbb>105. and mbb<135.) {
                      countCutflowEvent("SRlbb-06-onHiggs-1");
                      countSignalEvent("SRlbb-onHiggs-1");
                    }
                    else {
                      countCutflowEvent("SRlbb-07-sidebands-1");
                      countSignalEvent("SRlbb-sidebands-1");
                    }
                  }
                  else {
                    if(mbb>105. and mbb<135.) {
                      countCutflowEvent("SRlbb-08-onHiggs-2");
                      countSignalEvent("SRlbb-onHiggs-2");
                    }
                    else {
                      countCutflowEvent("SRlbb-09-sidebands-2");
                      countSignalEvent("SRlbb-sidebands-2");
                    }
                  }

                }//end if(mbb>45. and mbb<195.)
              }//end if(mT_w>100.)
            }//end if(mct>160.)
          }//end if(missingET->P4().Et()>100.)
        }//end if(BTag)
      }//end if(signalJets.size()==2 or signalJets.size() == 3)
    }//end if (trigger of single-lepton)
    }//end if (signalElectrons.size()+signalMuons.size()==1)
  } //end if( trigger_onelepton_2bjets )

  bool onelep_2photons_trigger_photon = false;
  bool onelep_2photons_trigger_lep = false;
  double triggerRatio = (double) rand() / (double) (RAND_MAX + 1.);

  if(photons.size()>1 && photons[0]->PT > 35. && photons[1]->PT > 25. && triggerRatio > 0.1)
    onelep_2photons_trigger_photon = true;
  else if(electronsMedium.size()>0 && electronsMedium[0]->PT>24.)
    onelep_2photons_trigger_lep = true;
  else if(muonsCombinedPlus.size()>0 && muonsCombinedPlus[0]->PT>24.)
    onelep_2photons_trigger_lep = true;

  bool onelep_2photons_trigger = onelep_2photons_trigger_photon or onelep_2photons_trigger_lep;
  if(onelep_2photons_trigger) {
    std::vector<Electron*> signalElectrons;
    std::vector<Muon*> signalMuons;
    std::vector<Photon*> signalPhotons;
   
    if(onelep_2photons_trigger_photon){ 
      signalElectrons = filterPhaseSpace(electronsMedium, 15., -2.47, 2.47);
      signalMuons = filterPhaseSpace(muonsCombinedPlus, 10., -2.5, 2.5);
      for(int i=0; i<photons.size(); i++) {
        if(photons[i]->PT>40.) signalPhotons.push_back(photons[i]);
        else if(photons[i]->PT>27.) {
          signalPhotons.push_back(photons[i]);
          break;
        }
        if(signalPhotons.size()==2) break;
      }
    }
    else {
      signalElectrons = filterPhaseSpace(electronsMedium, 25., -2.47, 2.47);
      signalMuons = filterPhaseSpace(muonsCombinedPlus, 25., -2.5, 2.5);
      for(int i=0; i<photons.size(); i++) {
        if(photons[i]->PT>40.) signalPhotons.push_back(photons[i]);
        else if(photons[i]->PT>20.) {
          signalPhotons.push_back(photons[i]);
          break;
        }
        if(signalPhotons.size()==2) break;
      }
    }//end if(onelep_2photons_trigger_photon)

    //overlap removal
    signalElectrons = overlapRemoval(signalElectrons, signalPhotons, 0.4);
    std::vector<Jet*> signalJets = overlapRemoval(preJets, signalPhotons, 0.4);
    signalJets = overlapRemoval(signalJets, signalElectrons, 0.2);
    signalMuons = overlapRemoval(signalMuons, signalPhotons, 0.4);
    signalElectrons = overlapRemoval(signalElectrons, signalJets, 0.4);
    signalMuons = overlapRemoval(signalMuons, signalJets, 0.4);

    //isolation
    std::vector<int> isoFlags;
    isoFlags.push_back(2);
    isoFlags.push_back(3);
    signalElectrons = filterIsolation(signalElectrons, isoFlags);
    signalMuons = filterIsolation(signalMuons, isoFlags);

    signalPhotons = filterIsolation(signalPhotons, 0);
    signalPhotons = filterIsolation(signalPhotons, 1);

/*    //test for pythia could implement that higgs decay to diphotons
    if(photons.size()==2) {
      double mrr = (signalPhotons[0]->P4() + signalPhotons[1]->P4()).M();
      hist2->Fill(mrr);
    } */

    if( (signalElectrons.size() == 1 && triggerRatio > 0.15) or
        (signalMuons.size() == 1 && triggerRatio > 0.2) ) {
 
      if(signalPhotons.size()==2) {

        countCutflowEvent("SRlrr-01-leprr");

        double mrr = (signalPhotons[0]->P4() + signalPhotons[1]->P4()).M();
        if(mrr > 100. and mrr<160.) {

          countCutflowEvent("SRlrr-02-mrr");

          if(missingET->P4().Et()>40.) {

            countCutflowEvent("SRlrr-03-et");

            TLorentzVector vec_lep;
            if(signalElectrons.size()==1) {
              vec_lep = signalElectrons[0]->P4();
            }
            else {
              vec_lep = signalMuons[0]->P4();
            }

            TVector2 vec2_w(vec_lep.Px()+missingET->P4().Px(), vec_lep.Py()+missingET->P4().Py());
            TVector2 vec2_h(signalPhotons[0]->P4().Px()+signalPhotons[1]->P4().Px(),
                            signalPhotons[0]->P4().Py()+signalPhotons[1]->P4().Py());
            double dphi_wh = vec2_w.DeltaPhi(vec2_h);

            if(dphi_wh>2.25){

              countCutflowEvent("SRlrr-04-dphi");

              double mw=80.385;
              double mtw=mT(vec_lep, missingET->P4(), 0.);
              double etw = sqrt( pow(mw,2)+vec2_w.Mod2() ); //et^2 = m^2 + pt^2
              double ptw = vec2_w.Mod();

              TVector2 vec2_r;
              double etr, ptr, mT_wr1, mT_wr2;

              vec2_r = TVector2(signalPhotons[0]->P4().Px(), signalPhotons[0]->P4().Py());
              ptr = vec2_r.Mod();
              etr = ptr;
              mT_wr1 = sqrt( pow(mtw,2) + 2.*etw*etr - 2.*ptw*ptr*cos(fabs( vec2_w.DeltaPhi(vec2_r) )));

              vec2_r = TVector2(signalPhotons[1]->P4().Px(), signalPhotons[1]->P4().Py());
              ptr = vec2_r.Mod();
              etr = ptr;
              mT_wr2 = sqrt( pow(mtw,2) + 2.*etw*etr - 2.*ptw*ptr*cos(fabs( vec2_w.DeltaPhi(vec2_r) )));

              if(mT_wr1>150. and mT_wr2>80.) {
                countCutflowEvent("SRlrr-05-1");
                countSignalEvent("SRlrr-1");
              }
              else {
                countCutflowEvent("SRlrr-06-2");
                countSignalEvent("SRlrr-2");
              }//end if(mT_wr1>150. and mT_wr2>80.)
            }//end if(dphi_wh>2.25)
          }//end if(missingET->P4().Et()>40.)
        }//end if(mrr > 120. and mrr<130.)
      }//end if(photons.size()==2)
    }//end if(lepton size == 1)
  }//end if(onelep_2photons_trigger)

       /*   //test for distribution of electrons and muons
          for(int i=0; i<electronsTight.size(); i++) {
            hist3->Fill(electronsTight[i]->PT);
          }
          for(int i=0; i<muonsCombinedPlus.size(); i++) {
            hist4->Fill(muonsCombinedPlus[i]->PT);
          } */

  bool sameSignDilep_trigger = false;
  bool sameSignDilep_trigger_ee = false;
  bool sameSignDilep_trigger_mm = false;
  bool sameSignDilep_trigger_em = false;

  triggerRatio = (double) rand() / (double) (RAND_MAX + 1.) ;

  if (electronsTight.size()>1 && electronsTight[0]->Charge * electronsTight[1]->Charge > 0
      and triggerRatio > 0.10) { 
    sameSignDilep_trigger_ee = true;
  }
  if (muonsCombinedPlus.size()>1 && muonsCombinedPlus[0]->Charge * muonsCombinedPlus[1]->Charge > 0 
     and triggerRatio > 0.18) { 
    sameSignDilep_trigger_mm = true;
  }
  if (electronsTight.size() > 0 && muonsCombinedPlus.size()>0 &&
       electronsTight[0]->Charge * muonsCombinedPlus[0]->Charge >0 and
      triggerRatio > 0.20 ) {
    sameSignDilep_trigger_em = true;
  }

  sameSignDilep_trigger = sameSignDilep_trigger_ee or sameSignDilep_trigger_mm or sameSignDilep_trigger_em;
  if ( sameSignDilep_trigger ) {
          /* //test for distribution of electrons and muons
          for(int i=0; i<electronsTight.size(); i++) {
            hist3->Fill(electronsTight[i]->PT);
          }
          for(int i=0; i<muonsCombinedPlus.size(); i++) {
            hist4->Fill(muonsCombinedPlus[i]->PT);
          } */
    std::vector<Electron*> signalElectrons = filterPhaseSpace(electronsTight, 20., -2.47, 2.47);
    std::vector<Muon*> signalMuons = filterPhaseSpace(muonsCombinedPlus, 20., -2.5, 2.5);
    // std::vector<Muon*> signalMuons = filterPhaseSpace(muonsCombinedPlus, 20.(,) -2.5, 2.5);
    // ignore the above ',' could give very bad result but unfortunately complier don't complain

    //overlap removal
    std::vector<Jet*> signalJets = filterPhaseSpace(preJets, 20., -2.4, 2.4);
    std::vector<Jet*> tauJets;
    std::vector<Jet*> notTauJets;
    /*for(int i=0; i<preJets.size(); i++) {
      if(checkTauTag(preJets[i], "loose") and preJets[i]->PT > 20. and fabs(preJets[i]->Eta)<2.47)
        tauJets.push_back(preJets[i]);
    }*/
    for(int i=0; i<signalJets.size(); i++) {
      if( checkTauTag(signalJets[i], "loose") )
        tauJets.push_back(signalJets[i]);
      else
        notTauJets.push_back(signalJets[i]);
    } //Require that there no tau-jets in signalJets

    signalElectrons = overlapRemoval(signalElectrons, 0.05);

    //signalJets = overlapRemoval(signalJets, signalElectrons, 0.2);
    signalJets = overlapRemoval(notTauJets, signalElectrons, 0.2);
    tauJets = overlapRemoval(tauJets, signalElectrons, 0.2);
    tauJets = overlapRemoval(tauJets, signalMuons, 0.2);

    signalElectrons = overlapRemoval(signalElectrons, signalJets, 0.4);
    signalMuons = overlapRemoval(signalMuons, signalJets, 0.4);

    std::vector<Electron*> tempEle = signalElectrons;
    signalElectrons = overlapRemoval(signalElectrons, signalMuons, 0.1);
    signalMuons = overlapRemoval(signalMuons, tempEle, 0.1);

    signalMuons = overlapRemoval(signalMuons, 0.05, true);
    
    signalJets = overlapRemoval(signalJets, tauJets, 0.2);

    //isolation
    std::vector<int> isoFlags;
    isoFlags.push_back(4);
    isoFlags.push_back(5);
    signalElectrons = filterIsolation(signalElectrons, isoFlags);
    signalMuons = filterIsolation(signalMuons, isoFlags);
    /*      //test for distribution of electrons and muons
          for(int i=0; i<signalElectrons.size(); i++) {
            hist3->Fill(signalElectrons[i]->PT);
          }
          for(int i=0; i<signalMuons.size(); i++) {
            hist4->Fill(signalMuons[i]->PT);
          } */

    if(tauJets.size()==0) {

      if(sameSignDilep_trigger_ee) countCutflowEvent("SRll-ee-01-sameSign");
      if(sameSignDilep_trigger_mm) countCutflowEvent("SRll-mm-01-sameSign");
      if(sameSignDilep_trigger_em) countCutflowEvent("SRll-em-01-sameSign");

    bool bTag = false;
    for(int i=0; i<signalJets.size(); i++) {
      if(checkBTag(signalJets[i], 1)) bTag = true;
    }
    if ( not bTag) {
      bool forwardJetTag = false;
      for(int i=0; i<signalJets.size(); i++) {
        if(fabs(signalJets[i]->Eta)>2.4 and fabs(signalJets[i]->Eta)<4.9 and signalJets[i]->PT>30.) forwardJetTag=true; 
      }
      if(not forwardJetTag) {

        if(sameSignDilep_trigger_ee) countCutflowEvent("SRll-ee-02-veto1");
        if(sameSignDilep_trigger_mm) countCutflowEvent("SRll-mm-02-veto1");
        if(sameSignDilep_trigger_em) countCutflowEvent("SRll-em-02-veto1");

        bool njetsTag = false;
        if(signalJets.size()>=1 and signalJets.size()<=3) njetsTag = true;
        if(njetsTag){

          if(sameSignDilep_trigger_ee) countCutflowEvent("SRll-ee-03-jets");
          if(sameSignDilep_trigger_mm) countCutflowEvent("SRll-mm-03-jets");
          if(sameSignDilep_trigger_em) countCutflowEvent("SRll-em-03-jets");


          if(sameSignDilep_trigger_ee and signalElectrons.size() > 1) {
            double mll=(signalElectrons[0]->P4()+signalElectrons[1]->P4()).M();
            if(fabs(mll-91.1876)>10.){

              countCutflowEvent("SRll-ee-04-veto2");

              if (signalElectrons[0]->PT>30. and signalElectrons[1]->PT>20. and
                  signalElectrons.size()==2 and muonsCombinedPlus.size()==0 ) {

                countCutflowEvent("SRll-ee-05-pt");
                countCutflowEvent("SRll-ee-06-deltaEta");

                double deltaPhi_min;
                deltaPhi_min = std::min( fabs(signalElectrons[0]->P4().DeltaPhi(missingET->P4())),
                                    fabs(signalElectrons[1]->P4().DeltaPhi(missingET->P4())) );
                for(int i=0; i<signalJets.size(); i++) {
                  double dphi = fabs(signalJets[i]->P4().DeltaPhi(missingET->P4()));
                  if(dphi < deltaPhi_min) deltaPhi_min = dphi;
                }

                double missinget_rel;
                if(deltaPhi_min>PI/2.) missinget_rel = missingET->P4().Et();
                else missinget_rel = missingET->P4().Et()*sin(deltaPhi_min);
                if( (signalJets.size()==1 && missinget_rel>55.) or
                    (signalJets.size()>1 && missinget_rel>30.)) {

                  countCutflowEvent("SRll-ee-07-etRel");

                  double meff = 0.;
                  meff += signalElectrons[0]->PT + signalElectrons[1]->PT;
                  meff += missingET->P4().Et();
                  for(int i=0; i<signalJets.size(); i++) {
                    meff += signalJets[i]->PT;
                  }
                  if( (signalJets.size()==1 && meff>200.) or signalJets.size()>1 ) {

                    countCutflowEvent("SRll-ee-08-meff");

                    double mT_lep1 = mT(signalElectrons[0]->P4(), missingET->P4(), 0.);
                    double mT_lep2 = mT(signalElectrons[1]->P4(), missingET->P4(), 0.);
                    double mT_max = std::max(mT_lep1, mT_lep2);

                    if(signalJets.size()==1 or ( signalJets.size()>1 && mT_max>110.) ) {

                      countCutflowEvent("SRll-ee-09-mt");

                      double deltaR_min = 100.;
                      double m_jl = 1000., m_jjl = 1000.;
                      if(signalJets.size()==1) {
                        if(signalElectrons[0]->P4().DeltaR(signalJets[0]->P4()) <
                            signalElectrons[1]->P4().DeltaR(signalJets[0]->P4())) {
                          m_jl = (signalElectrons[0]->P4()+signalJets[0]->P4()).M();
                        }
                        else 
                          m_jl = (signalElectrons[1]->P4()+signalJets[0]->P4()).M();
                      }
                      else {
                        TLorentzVector vec_jj=signalJets[0]->P4()+signalJets[1]->P4();
                        if(signalElectrons[0]->P4().DeltaR(vec_jj) < signalElectrons[1]->P4().DeltaR(vec_jj))
                          m_jjl = (signalElectrons[0]->P4()+vec_jj).M();
                        else
                          m_jjl = (signalElectrons[1]->P4()+vec_jj).M();
                      }

                      if( (signalJets.size()==1 && m_jl<90.) or (signalJets.size()>1 && m_jjl<120.)) {

                        countCutflowEvent("SRll-ee-10-mass");

                        if(signalJets.size()==1) countSignalEvent("SRee-1");
                        else countSignalEvent("SRee-2");
                      }//end if(m_jl or m_jll)
                    }//end if(mT_max>110.)
                  }//end if(meff>200.)
                }//end if(missinget_rel>30.)
              }//end if(lepton pt and lepton number)
            }//end if(fabs(mll-91.1876)>10.)
          }//end if(sameSignDilep_trigger_ee)

          if(sameSignDilep_trigger_mm and signalMuons.size()>1) {

            bool lepTag = false;
            if(signalJets.size()==1) {
              if(signalMuons[0]->PT>30. and signalMuons[1]->PT>20.
                 and signalMuons.size()==2 and electronsTight.size()==0)
                lepTag = true;
            }
            else {
              if(signalMuons[0]->PT>30. and signalMuons[1]->PT>30. and
                  signalMuons.size()==2 and electronsTight.size()==0)
                lepTag = true;
            }

            if(lepTag) {

              countCutflowEvent("SRll-mm-04-veto2");
              countCutflowEvent("SRll-mm-05-pt");

              double delta_eta = fabs(signalMuons[0]->P4().Eta()-signalMuons[1]->P4().Eta());
              if(delta_eta<1.5) {

                 countCutflowEvent("SRll-mm-06-deltaEta");

                 double meff = 0.;
                 meff += signalMuons[0]->PT + signalMuons[1]->PT;
                 meff += missingET->P4().Et();
                 for(int i=0; i<signalJets.size(); i++) {
                   meff += signalJets[i]->PT;
                 }
                 if(meff>200.) {

                   countCutflowEvent("SRll-mm-07-etRel");
                   countCutflowEvent("SRll-mm-08-meff");

                   double mT_max = std::max( mT(signalMuons[0]->P4(), missingET->P4(), 0.), 
                                             mT(signalMuons[1]->P4(), missingET->P4(), 0.) );
                   if( (signalJets.size()==1 && mT_max>110.) or signalJets.size()>1 ) {

                     countCutflowEvent("SRll-mm-09-mt");

                     double m_lj, m_ljj;
                     if(signalJets.size()==1) {
                        if(signalMuons[0]->P4().DeltaR(signalJets[0]->P4()) 
                             < signalMuons[1]->P4().DeltaR(signalJets[0]->P4()))
                          m_lj = (signalMuons[0]->P4()+signalJets[0]->P4()).M();
                        else
                          m_lj = (signalMuons[1]->P4()+signalJets[0]->P4()).M(); 
                     }
                     else {
                       TLorentzVector vec_jj = signalJets[0]->P4() + signalJets[1]->P4();
                       if(signalMuons[0]->P4().DeltaR(vec_jj) < signalMuons[1]->P4().DeltaR(vec_jj))
                         m_ljj = ( signalMuons[0]->P4() + vec_jj ).M();
                       else
                         m_ljj = (signalMuons[1]->P4()+vec_jj).M();
                     }
                     if((signalJets.size()==1 && m_lj<90.) or
                        (signalJets.size()>1 && m_ljj<120.) ) {

                       countCutflowEvent("SRll-mm-10-mass");

                       if(signalJets.size()==1) countSignalEvent("SRmm-1");
                       else countSignalEvent("SRmm-2");
                     }//end if(m_lj or m_ljj)
                   }//end if(mT_max>110.)
                 }//end if(meff>200.)
              }//end if(delta_eta<1.5)
            }//end if(lepTag)
          }//end if(sameSignDilep_trigger_mm)


          if(sameSignDilep_trigger_em and signalMuons.size()>0 and signalElectrons.size()>0 ) {
            if( (signalMuons[0]->PT>30. && signalMuons.size()==1) and
                (signalElectrons[0]->PT>30. && signalElectrons.size()==1) ) {

              countCutflowEvent("SRll-em-04-veto2");
              countCutflowEvent("SRll-em-05-pt");

              double delta_eta = fabs(signalMuons[0]->P4().Eta() - signalElectrons[0]->P4().Eta());
              if(delta_eta<1.5) {

                countCutflowEvent("SRll-em-06-deltaEta");
                countCutflowEvent("SRll-em-07-etRel");

                double meff=0.;
                meff += signalMuons[0]->PT + signalElectrons[0]->PT ;
                meff += missingET->P4().Et();
                for(int i=0; i<signalJets.size(); i++) {
                  meff += signalJets[i]->PT;
                }

                if(meff>200.) {

                  countCutflowEvent("SRll-em-08-meff");

                  double mT_max = std::max(mT(signalMuons[0]->P4(), missingET->P4()),
                                           mT(signalElectrons[0]->P4(), missingET->P4()));
                  if(mT_max>110.) {

                    countCutflowEvent("SRll-em-09-mt");

                    double m_lj, m_ljj;
                    if(signalJets.size()==1) {
                      if(signalMuons[0]->P4().DeltaR(signalJets[0]->P4())
                          <signalElectrons[0]->P4().DeltaR(signalJets[0]->P4()))
                        m_lj = ( signalMuons[0]->P4()+signalJets[0]->P4() ).M();
                      else
                        m_lj = (signalElectrons[0]->P4()+signalJets[0]->P4()).M();
                    }
                    else {
                      TLorentzVector vec_jj = signalJets[0]->P4() + signalJets[1]->P4();
                      if(signalMuons[0]->P4().DeltaR(vec_jj) < signalElectrons[0]->P4().DeltaR(vec_jj))
                         m_ljj = (signalMuons[0]->P4()+vec_jj).M();
                      else
                         m_ljj = (signalElectrons[0]->P4() + vec_jj).M();
                    }
                    if((signalJets.size()==1 && m_lj<90.) or 
                       (signalJets.size()>1 && m_ljj<120.)) {

                      countCutflowEvent("SRll-em-10-mass");

                      if(signalJets.size()==1) countSignalEvent("SRem-1");
                      else countSignalEvent("SRem-2");
                    }//end if(m_lj or m_ljj)
                  }//end if(mT_max>110.)
                }//end if(meff>200.)
              }//end if(delta_eta)
            }//end if(lepTag)
          }//end if(sameSignDilep_trigger_em)

        }//end if(njetsTag)
      }//end if(not forwardJetTag)
    }//end if(not bTag)
    }//end if(tauJets.size()==0)
  }//end if ( sameSignDilep_trigger )


}

void Atlas_1501_07110::finalize() {
  // Whatever should be done after the run goes here
/* //for testing
  file1.close();

  TCanvas *can1 = new TCanvas("can1", "1501_07110", 600, 600);
  hist1->Draw();
  can1->SaveAs("1501_07110.png");
  
  can1->Close();
  //delete can1;

  can1 = new TCanvas("can1", "1501_07110", 600, 600);
  hist2->Draw();
  can1->SaveAs("1501_07110_hrr.png");

  can1->Close();

  can1 = new TCanvas("can1", "1501_07110", 600, 600);
  can1->Divide(1,2);
  can1->cd(1);
  hist3->Draw();
  can1->cd(2);
  hist4->Draw();
  
  can1->SaveAs("1501_07110_ll_pt.png");
  can1->Close(); */
}       
