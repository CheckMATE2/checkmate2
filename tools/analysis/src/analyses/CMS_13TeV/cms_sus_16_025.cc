#include "cms_sus_16_025.h"
// AUTHOR: Junjie Cao, Liangliang Shang, Yuanfang Yue and Yang Zhang 
// EMAIL: junjiec@itp.ac.cn, shlwell1988@gmail.com, yuanfang405@gmail.com and zhangyang@itp.ac.cn 
void Cms_sus_16_025::initialize() {
  setAnalysisName("cms_sus_16_025");          
  setInformation(""
		 "# cms\n"
		 "# cms_sus_16_025\n"
		 "# two soft leptons, jets, missingET\n"
		 "# sqrt(s)=13TeV\n"
		 "# int(L)=12.9 fb^-1 and 10.1 fb^-1\n"
  "");
  setLuminosity(12.9*units::INVFB);      

  bookSignalRegions("SR1_weakino_1low_mll_1;SR1_weakino_1low_mll_2;SR1_weakino_1low_mll_3;SR1_weakino_1low_mll_4;SR1_weakino_2high_mll_1;SR1_weakino_2high_mll_2;SR1_weakino_2high_mll_3;SR1_weakino_2high_mll_4;SR2_stop_1low_pt_1;SR2_stop_1low_pt_2;SR2_stop_1low_pt_3;SR2_stop_2high_pt_1;SR2_stop_2high_pt_2;SR2_stop_2high_pt_3");

  fout1.open("log");

  h1 = new TH1F("h1", "mll", 100, 0, 100);
  h2 = new TH1F("h2", "leading lepton pt", 100, 0, 100);
  h3 = new TH1F("h3", "ht", 100, 0, 500);
  h4 = new TH1F("h4", "misset", 100, 0, 500);
  h5 = new TH1F("h5", "mll_met>200", 100, 0, 100);

  f1 = new TFile("log.root", "RECREATE");
  //f1 = new TFile("bin/log.root", "UPDATE");
  
  i1 = i2 = i3 = i4 = 0;
  
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_sus_16_025::analyze() {
  // Your eventwise analysis code goes here
  // The following objects are always defined unless they are 'ignored' above. They form std::vector objects of the respective Delphes class type (except for Etmiss which is a single object)
  // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
  // Within a std::vector, all members are ordered with highest pt coming first.

  // electronsLoose, electronsMedium, electronsTight   are list of electrons that passed respective efficiency and reconstruction cuts
  // muonsCombinedPlus, muonsCombined                  as above for muons
  // photonsMedium                                     as above for photons
  // jets are all reconstructed jets                   as above for jets. Note that electrons are most certainly also reconstructed as a jet -> overlap removal do avoid double counting necessary!
  // tracks, towers                                    calorimeter and tracker information. Usually not needed.
  // missingET                                         rec missing ET EXCLUDING muons.

  
  // Here is a couple of useful functions and lines:  
  //------------Phase Space Cuts (defined for jets, electronsXYZ, muonsXYZ, photonsXYZ)
  // jets = filterPhaseSpace(jets, 20., -2.8, 2.8)  // The vector 'jets' only contains jets with pt >= 20 GeV and -2.8 < eta < 2.8. This function is applicable to other particles too (electronsMedium, ... ).
  // jets = overlapRemoval(jets, electronsLoose, 0.2) Removes all jets for which there exists any electron in 'electronsLoose' with deltaR < 0.2.
  // jets = overlapRemovel(jets, 0.2) If two jets overlap within deltaR < 0.2, only the harder jet is stored.
  
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
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in CheckMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  

  //  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  countCutflowEvent("CR00_all");

  double mET_no_muon = missingET->PT;

  missingET->addMuons(muons);

  double mET = missingET->PT;
  
  h4->Fill(mET);
  fout1 << "muon.size()=" << muons.size() << std::endl;
  fout1 << mET << ", " << mET_no_muon << std::endl;
  
  //reconstruction
  //muonsCombined could not be used in CMS
  //electrons could be used but with worse case compared to electronsLoose and electronsMedium 
  std::vector<Electron*> electrons_g;
  std::vector<Muon*> muons_g;
  
  for ( int i=0; i < electronsLoose.size(); i++ ) {
    if ( electronsLoose[i]->PT > 3 and electronsLoose[i]->PT < 10 ) {
      if ( fabs(electronsLoose[i]->Eta) < 2.5 ) {
        electrons_g.push_back(electronsLoose[i]);
      }
    } 
  }
  
  for ( int i=0; i < electronsMedium.size(); i++ ) {
    if ( electronsMedium[i]->PT > 10 ) {
      if ( fabs(electronsMedium[i]->Eta) < 2.5 ) {
        electrons_g.push_back(electronsLoose[i]);
      }
    } 
  }
  
  muons_g = filterPhaseSpace(muons, 3.5, -2.4, 2.4);
  
  //llllllllllllllllll
  electrons_g = filterIsolation( electrons_g );
  muons_g     = filterIsolation( muons_g );

  jets = filterPhaseSpace( jets, 25., -2.4, 2.4);
  jets = overlapRemoval( jets, electrons_g, 0.4 );
  jets = overlapRemoval( jets, muons_g, 0.4 );
  electrons_g = overlapRemoval( electrons_g, jets, 0.4);
  muons_g = overlapRemoval( muons_g, jets, 0.4);
  
  std::vector<Jet*> bjets;
  for(int i=0; i<jets.size(); i++) {
    if ( checkBTag(jets[i]) ) {
      bjets.push_back(jets[i]);
    }
  }

  double ht=0.;
  for(int i=0; i<jets.size(); i++) {
    ht += jets[i]->PT;
  }

  h3->Fill(ht);
/*  //trick for 10.1fb^-1 for low mET and 12.9fb^-1 for high mET regions
  double triggerRatio = (double) rand() / (double) (RAND_MAX + 1.);
  if(mET->PT<200) {
    if( triggerRatio > 10.1/12.9 ) return;
  }
*/
  //for leptons with PT order
  std::vector<FinalStateObject*> leptons;
  for( int i=0; i < electrons_g.size(); i++) {
    FinalStateObject* lep = new FinalStateObject( electrons_g[i] );
    leptons.push_back(lep);
  }
  for( int i=0; i < muons_g.size(); i++) {
    FinalStateObject* lep = new FinalStateObject( muons_g[i] );
    leptons.push_back(lep);
  }
  std::sort( leptons.begin(), leptons.end(), FinalStateObject::sortByPT );

  //for two leptons at the trigger level
  if(leptons.size() < 2) return;

  h2->Fill(leptons[0]->PT);

  if(leptons[0]->PT < 3. or leptons[1]->PT < 3.) return;

  for(int i=0; i<2; i++) {
    fout1 << leptons[i]->Type << std::endl;
  }

  //for two opposite-sign leptons 
  if(leptons[0]->Charge * leptons[1]->Charge > 0 ) return;

  countCutflowEvent("CR01_twoLep");

  //for leading lepton's pt
  if(leptons[0]->PT < 5. or leptons[0]->PT > 30.) return;
  
  //for sub-leading lepton's pt
  if(leptons[1]->PT < 3.5 or leptons[1]->PT > 30.) return;

  countCutflowEvent("CR02_twoLepPT");

  bool lep2th_pt_good =true;
  bool lep2th_pt_good_stop =true;
  if(leptons[1]->PT < 5. or leptons[1]->PT > 30.) {
    lep2th_pt_good = false;
  }
  if(leptons[1]->Type=="muon") {
    if(leptons[1]->PT < 3.5 or leptons[1]->PT > 30.) {
      lep2th_pt_good_stop = false;
    }
  }

  //for classification of flavor of leptons
  bool eeflag = false, mmflag = false, emflag;
  std::string flavour;
  if( leptons[0]->Type == "electron" and leptons[1]->Type == "electron" ) {
    eeflag = true;
    flavour = "ee";
  } else if (leptons[0]->Type == "muon" and leptons[1]->Type == "muon") {
    mmflag = true;
    flavour = "mm";
  } else {
    emflag = true;
    flavour = "em";
  }

  //for at least one jet
  if (jets.size()==0) return;

  countCutflowEvent("CR03_jets");

  //for no b-tagged jets
  if (bjets.size()>0) return;

  countCutflowEvent("CR04_nobjets");

  //for M(ll) < 50GeV
  if (leptons.size() != 2) return;
  double mll = ( leptons[0]->P4()+leptons[1]->P4() ).M();

  h1->Fill(mll);

  if ( mll > 50.) return;

  countCutflowEvent("CR05_mll_up");

  //for pt(ll) > 3GeV
  if ( ( leptons[0]->P4()+leptons[1]->P4() ).Pt() < 3. ) return;

  countCutflowEvent("CR06_ptll");

  //for mET > 125GeV and mET_no_muon < 125GeV
  if (mET < 125.) return;

  countCutflowEvent("CR07_mET");
  
  if (mET_no_muon < 125.) return;

  countCutflowEvent("CR08_mET_no_muon");

  //for mET/ht in (0.6, 1.4)
  if (mET/ht < 0.6 or mET/ht > 1.4) return;

  countCutflowEvent("CR09_mET_ht");

  //for ht > 100GeV
  if (ht<100.) return;

  countCutflowEvent("CR10_ht");

  //for mll > 4GeV and veto events with mll in (9., 10.5)GeV
  if (mll < 4.) return;
  if (mll > 9. and mll<10.5) return;

  countCutflowEvent("CR11_mll_low");

  //for mtata
  //The magnitude of the lepton vectors are then scaled so that the lepton pair balances the hadronic recoil.
  double scale_a = -(jets[0]->P4().Px()*leptons[1]->P4().Py() - leptons[1]->P4().Px()*jets[0]->P4().Py())
                  /(leptons[1]->P4().Px()*leptons[0]->P4().Py() - leptons[0]->P4().Px()*leptons[1]->P4().Py());
  double scale_b = (jets[0]->P4().Px()*leptons[0]->P4().Py() - leptons[0]->P4().Px()*jets[0]->P4().Py())
                  /(leptons[1]->P4().Px()*leptons[0]->P4().Py() - leptons[0]->P4().Px()*leptons[1]->P4().Py());
  double mtata = ( scale_a*leptons[0]->P4() + scale_b*leptons[1]->P4() ).M();

  //for veto events with mtata in (0,160)GeV
  if (mtata>0 and mtata<160.) return;

  countCutflowEvent("CR12_mtata");

  //for mt(lep1/lep2, Et_miss) < 70GeV for electroweakino case
  bool mt_good = false;
  if ( mT(leptons[0]->P4(), missingET->P4()) < 70. ) {
    if ( mT(leptons[0]->P4(), missingET->P4()) < 70. ) {
      mt_good = true;
    }
  }

  //for Signal regions of electroweakino case
  if(lep2th_pt_good and mt_good) {

    countCutflowEvent("CR13_mT_weakino");

    if(mET<200.) {
      if (mll < 10.) {
        countCutflowEvent("SR1_weakino_1low_mll_1");
        countSignalEvent("SR1_weakino_1low_mll_1");
        
      } else if (mll < 20.) {
        countCutflowEvent("SR1_weakino_1low_mll_2");
        countSignalEvent("SR1_weakino_1low_mll_2");
      } else if (mll < 30.) {
        countCutflowEvent("SR1_weakino_1low_mll_3");
        countSignalEvent("SR1_weakino_1low_mll_3");
      } else {
        countCutflowEvent("SR1_weakino_1low_mll_4");
        countSignalEvent("SR1_weakino_1low_mll_4");
      }

    } else {
      h5->Fill(mll);
      if (mll < 10.) {
        countCutflowEvent("SR1_weakino_2high_mll_1");
        countSignalEvent("SR1_weakino_2high_mll_1");
      } else if (mll < 20.) {
        countCutflowEvent("SR1_weakino_2high_mll_2");
        countSignalEvent("SR1_weakino_2high_mll_2");
      } else if (mll < 30.) {
        countCutflowEvent("SR1_weakino_2high_mll_3");
        countSignalEvent("SR1_weakino_2high_mll_3");
      } else {
        countCutflowEvent("SR1_weakino_2high_mll_4");
        countSignalEvent("SR1_weakino_2high_mll_4");
      }
    }
  }
   
  //for Signal regions of stop case
  if(mET<200.) {
    if (leptons[0]->PT < 12.) {
      countCutflowEvent("SR2_stop_1low_pt_1");
      countSignalEvent("SR2_stop_1low_pt_1");
    } else if (leptons[0]->PT < 20.) {
      countCutflowEvent("SR2_stop_1low_pt_2");
      countSignalEvent("SR2_stop_1low_pt_2");
    } else {
      countCutflowEvent("SR2_stop_1low_pt_3");
      countSignalEvent("SR2_stop_1low_pt_3");
    }

  } else {
    if (lep2th_pt_good_stop) {
      if (leptons[0]->PT < 12.) {
        countCutflowEvent("SR2_stop_2high_pt_1");
        countSignalEvent("SR2_stop_2high_pt_1");
      } else if (leptons[0]->PT < 20.) {
        countCutflowEvent("SR2_stop_2high_pt_2");
        countSignalEvent("SR2_stop_2high_pt_2");
      } else {
        countCutflowEvent("SR2_stop_2high_pt_3");
        countSignalEvent("SR2_stop_2high_pt_3");
      }
    }
  }

  
}

void Cms_sus_16_025::finalize() {
  // Whatever should be done after the run goes here

    int n4 = h1->FindBin(4);
  int n10 = h1->FindBin(10);
  int n20 = h1->FindBin(20);
  int n30 = h1->FindBin(30);
  int n50 = h1->FindBin(50);
  double NT0410 = h1->Integral(n4,n10);
  double NT1020 = h1->Integral(n10,n20);
  double NT2030 = h1->Integral(n20,n30);
  double NT3050 = h1->Integral(n30,n50);

  fout1 << "NT1, 2, 3, 4: " << NT0410 << ", "<< NT1020 << ", "<< NT2030 << ", "<< NT3050 << std::endl;

  h1->Write();
  h2->Write();
  h3->Write();
  h4->Write();
  h5->Write();

  f1->Write();

  fout1.close();
  
}       
