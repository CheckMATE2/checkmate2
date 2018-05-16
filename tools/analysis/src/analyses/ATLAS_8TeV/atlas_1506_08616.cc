#include "atlas_1506_08616.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl

void Atlas_1506_08616::initialize() {
  setAnalysisName("atlas_1506_08616");          
  setInformation(""
    "# two additional searches from stop legacy\n"
     "# di-lepton search with MT2 to exploit WW-like signals\n"
     "# 2b-jets plus lepton for natural SUSY\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("WWSR1;WWSR2;WWSR3;WWSR4;WWSR5;WWSR6;WWSR7;SRinA;SRinB;SRinC;SRexA;");
  bookCutflowRegions("WW_00;WW_01_trigger;WW_02_2leptons;WW_03_OF;WW_04_iso;WW_05_OS;WW_06_mll>20;WW_07_pTl1>25;WW_08_R1;WW_09_mT2>20;WW_10_dX<0.02;WW_11_R2>0.5;WW_12_SR1;WW_12_SR2;WW_12_SR3;WW_12_SR4;WW_12_SR5;WW_12_SR6;WW_12_SR7;");
  bookCutflowRegions("in1_00;in1_01_trigger;in1_02_1lepton;in1_03_met>100;in1_04_2bjets;in1_05_met>200;in1_06_mt>140;in1_07_meff>300;in1_08_metsig>8;in1_09_amt2>180;in1_10_mbl<170;in1_11_dphi<0.4;");
  bookCutflowRegions("in2_00;in2_01_trigger;in2_02_1lepton;in2_03_met>100;in2_04_2bjets;in2_05_met>120;in2_06_mt>140;in2_07_meff>450;in2_08_metsig>12;in2_09_amt2>200;in2_10_mbl<170;in2_11_dphi<0.4;");
  bookCutflowRegions("in3_00;in3_01_trigger;in3_02_1lepton;in3_03_met>100;in3_04_2bjets;in3_05_met>220;in3_06_mt>180;in3_07_meff>650;in3_08_metsig>5;in3_09_amt2>180;in3_10_mbl<170;in3_11_dphi<0.4;");
  bookCutflowRegions("ex1_00;ex1_01_trigger;ex1_02_1lepton;ex1_03_met>100;ex1_04_2bjets;ex1_05_met>160;ex1_06_mt>120;ex1_07_meff>300;ex1_08_metsig>10;ex1_09_amt2>180;ex1_10_mbl<170;ex1_11_dphi<0.4;ex1_12_Nxjets;");
  bookCutflowRegions("WW_20_CRZ;WW_20_CRtt;WW_20_CRWW;in1_20_CRT;in2_20_CRT;in3_20_CRT;ex1_20_CRT;");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  
//  histo_eta1 = new TH1D("etar8", "rapidity distance between leptons r8>450", 10, -1 , 1);
}

void Atlas_1506_08616::analyze() {
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
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 10., -2.4, 2.4);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);

  //object removal
  //remove jets within 0.2 of electrons
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  muonsCombinedPlus = overlapRemoval(muonsCombinedPlus, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  
  countCutflowEvent("WW_00");
  countCutflowEvent("in1_00");
  countCutflowEvent("in2_00");
  countCutflowEvent("in3_00");
  countCutflowEvent("ex1_00");
  
  bool trigger = false;

  double cutSingleTriggerElectronPT = 25.;
  double cutDoubleTriggerElectronPT = 14.;
  double cutMixedTriggerElectronPT = 10.;

  double cutSingleTriggerMuonPT = 25.;
  double cutDoubleTriggerMuonPT = 14.;
  double cutMixedTriggerMuonPT = 18.;
  
     // Trigger Cuts
   if( electronsMedium.size() > 0 && electronsMedium[0]->PT > cutSingleTriggerElectronPT )
    //  && (double) rand() / (double) RAND_MAX < 0.80 )
     trigger=true;
   else if( muonsCombinedPlus.size() > 0 && muonsCombinedPlus[0]->PT > cutSingleTriggerMuonPT  )
    // &&   (double) rand() / (double) RAND_MAX < 0.99 ) 
     trigger=true;
   else if( electronsMedium.size() > 1 && electronsMedium[0]->PT > cutDoubleTriggerElectronPT &&
	    electronsMedium[1]->PT > cutDoubleTriggerElectronPT ) 
    // && (double) rand() / (double) RAND_MAX < 0.85 )
     trigger=true;
   else if( muonsCombinedPlus.size() > 1 && muonsCombinedPlus[0]->PT > cutDoubleTriggerMuonPT &&
	    muonsCombinedPlus[1]->PT > cutDoubleTriggerMuonPT  )
     // &&    (double) rand() / (double) RAND_MAX < 0.70 )
     trigger=true;
   else if( muonsCombinedPlus.size() > 0 && electronsMedium.size() > 0 && muonsCombinedPlus[0]->PT > cutMixedTriggerMuonPT &&
	    electronsMedium[0]->PT > cutMixedTriggerElectronPT )
     // &&    (double) rand() / (double) RAND_MAX < 0.70 )
     trigger=true;
  
   if( !trigger ) return; 
   
  countCutflowEvent("WW_01_trigger");
  countCutflowEvent("in1_01_trigger");
  countCutflowEvent("in2_01_trigger");
  countCutflowEvent("in3_01_trigger");
  countCutflowEvent("ex1_01_trigger");
  
  double met = missingET->P4().Et();
  
  if (electronsMedium.size() + muonsCombinedPlus.size() == 2 ) {
    countCutflowEvent("WW_02_2leptons");
    
    bool flag2l = false;
    
    if (electronsMedium.size() == 1) {
      flag2l = true;
      countCutflowEvent("WW_03_OF");
    }
            
    electronsTight = filterIsolation(electronsTight);
    muonsCombined = filterIsolation(muonsCombined);  
    
    if (flag2l && electronsTight.size() == 1 && muonsCombined.size() == 1 ) {
      countCutflowEvent("WW_04_iso");
    
      if (flag2l && electronsTight[0]->Charge*muonsCombined[0]->Charge < 0 ) 
        countCutflowEvent("WW_05_OS");
      else
        flag2l = false;    
      
      double mll =(electronsTight[0]->P4() + muonsCombined[0]->P4()).M();
      if (flag2l &&  mll > 20.)
        countCutflowEvent("WW_06_mll>20");
      else
        flag2l = false;
    
      if (flag2l && ( electronsTight[0]->PT > 25. || muonsCombined[0]->PT > 25. ) )
        countCutflowEvent("WW_07_pTl1>25");
      else
        flag2l = false;
    
      if (flag2l &&  electronsTight[0]->PT > 20. && muonsCombined[0]->PT > 20.  );
//      countCutflowEvent("3_WW_pTl1>25");
      else
        flag2l = false;
    
      double meff = electronsTight[0]->PT + muonsCombined[0]->PT + met;
      for (int i = 0; i < jets.size(); i++ ) 
        if ( fabs(jets[i]->Eta) < 2.5 && jets[i]->PT > 20. )
	  meff += jets[i]->PT;
    
      double pa[3] = {electronsTight[0]->P4().M(), electronsTight[0]->P4().Px(), electronsTight[0]->P4().Py() };
      double pb[3] = {muonsCombined[0]->P4().M(), muonsCombined[0]->P4().Px(), muonsCombined[0]->P4().Py() };
      double pmiss[3] = {0.,missingET->P4().Px(), missingET->P4().Py() };
      mt2_bisect::mt2 mt2_event;
      mt2_event.set_momenta( pa, pb, pmiss );
      mt2_event.set_mn( 0. );       
      double m_t2 = mt2_event.get_mt2();    
      
      double R1 = met/meff;
      double dxv = fabs((electronsTight[0]->P4().Pz() + muonsCombined[0]->P4().Pz())/8000.);
      double R2 = met/( electronsTight[0]->PT + muonsCombined[0]->PT + met);
    
      if (flag2l && m_t2 < 20. && mll > 30. && mll < 80.  && R1 > (0.3 + meff/1000.) && dxv < 0.02 && R2 > 0.5) countCutflowEvent("WW_20_CRZ");
      if (flag2l && R1 < 0.3 && m_t2 > 35. && dxv < 0.02 && R2 > 0.5 ) countCutflowEvent("WW_20_CRtt");
      if (flag2l && R1 > (0.3 + meff/1000.) && m_t2 > 35. && dxv > 0.04 && R2 > 0.5 ) countCutflowEvent("WW_20_CRWW");
    
      if (flag2l && R1 > (0.3 + meff/1000.) )
        countCutflowEvent("WW_08_R1");
      else
        flag2l = false;    
    
      if (flag2l && m_t2 > 20. )
        countCutflowEvent("WW_09_mT2>20");
      else
        flag2l = false;
    
      if (flag2l && dxv < 0.01 )
        countCutflowEvent("WW_10_dX<0.02");
      else
        flag2l = false;    
    
      if (flag2l && R2 > 0.5 )
        countCutflowEvent("WW_11_R2>0.5");
      else
        flag2l = false;    
    
      TLorentzVector leptons = electronsTight[0]->P4() + muonsCombined[0]->P4();
      TVector3 boost = -leptons.BoostVector();
      TLorentzVector lepton_temp = electronsTight[0]->P4(); 
      lepton_temp.Boost(boost);
      double costhb = fabs(lepton_temp.CosTheta());
    
      if ( flag2l && costhb < 0.8 && m_t2 < 45. ) {
        countCutflowEvent("WW_12_SR1");
        countSignalEvent("WWSR1");
      }
    
      if ( flag2l && costhb < 0.8 && m_t2 > 25. && m_t2 < 55. ) {
        countCutflowEvent("WW_12_SR2");
        countSignalEvent("WWSR2");
      }    
    
      if ( flag2l && costhb < 0.8  ) {
        countCutflowEvent("WW_12_SR3");
        countSignalEvent("WWSR3");
      }    

      if ( flag2l && m_t2 > 70. ) {
        countCutflowEvent("WW_12_SR4");
        countSignalEvent("WWSR4");
      }
    
      if ( flag2l && m_t2 > 90. ) {
        countCutflowEvent("WW_12_SR5");
        countSignalEvent("WWSR5");
      }    

      if ( flag2l && costhb < 0.8 && m_t2 > 25. && m_t2 < 70. ) {
        countCutflowEvent("WW_12_SR6");
        countSignalEvent("WWSR6");
      }      

      if ( flag2l && m_t2 > 80. ) {
        countCutflowEvent("WW_12_SR7");
        countSignalEvent("WWSR7");
      }
    }
  }
 
  electronsTight = filterIsolation(electronsTight);
  muonsCombined = filterIsolation(muonsCombined);   
  if  (electronsTight.size() + muonsCombined.size() == 1 ) {
    
    TLorentzVector lepton;
    if (electronsTight.size() == 1 ) lepton = electronsTight[0]->P4(); else lepton = muonsCombined[0]->P4();
    if (lepton.Perp() < 25.) return;
      
    countCutflowEvent("in1_02_1lepton");
    countCutflowEvent("in2_02_1lepton");
    countCutflowEvent("in3_02_1lepton");
    countCutflowEvent("ex1_02_1lepton");       
    
    if ( met < 100. ) return; 
    countCutflowEvent("in1_03_met>100");
    countCutflowEvent("in2_03_met>100");
    countCutflowEvent("in3_03_met>100");
    countCutflowEvent("ex1_03_met>100");          
    
    bool b2 = false;
    int vetojet = 0;
    double dphi_min = 10.;
    std::vector<TLorentzVector> bjets;
    std::vector<TLorentzVector> lightjets;
    for (int i = 0; i < jets.size(); i++) 
      if (fabs(jets[i]->Eta) < 2.8 && jets[i]->PT > 25.) 
	if (checkBTag(jets[i]) ) {
	  bjets.push_back(jets[i]->P4()) ;
	  double dphi = fabs(jets[i]->P4().DeltaPhi(missingET->P4()));
	  if (dphi < dphi_min) dphi_min = dphi;
	}
	else if (fabs(jets[i]->Eta) < 2.5 ) {
	  lightjets.push_back(jets[i]->P4()) ;
	  if (jets[i]->PT > 50.) vetojet += 1;
	}

//    if ( bjets.size() == 0 || bjets.size() > 2 ) return;
    if ( bjets.size() != 2) return;   // for CR 1-bjet is enough; to be added later
    if ( bjets.size() == 2) b2 = true;
    
    if( b2 ) {
      countCutflowEvent("in1_04_2bjets");
      countCutflowEvent("in2_04_2bjets");
      countCutflowEvent("in3_04_2bjets");
      countCutflowEvent("ex1_04_2bjets");         
    }
    
    
    bool mlb_flag = true;
    double amt2_value;
    double lep[4] = {lepton.E(), lepton.Px(), lepton.Py(), lepton.Pz() };
    double p1[4] = {bjets[0].E(), bjets[0].Px(), bjets[0].Py(), bjets[0].Pz() };
    double p2[4] = {bjets[1].E(), bjets[1].Px(), bjets[1].Py(), bjets[1].Pz() };
    double pmiss[4] = {0.,missingET->P4().Px(), missingET->P4().Py() };
    mt2bl_bisect::mt2bl mt2bl_event;
    if ( (lepton + bjets[0]).M() < 170. && (lepton + bjets[1]).M() > 170. ) {
      mt2bl_event.set_momenta( lep, p1, p2, pmiss );
      amt2_value = mt2bl_event.get_mt2bl();    
    }
    else if ( (lepton + bjets[0]).M() > 170. && (lepton + bjets[1]).M() < 170. ) {
      mt2bl_event.set_momenta( lep, p2, p1, pmiss );
      amt2_value = mt2bl_event.get_mt2bl();    
    }
    else if ( (lepton + bjets[0]).M() < 170. && (lepton + bjets[1]).M() < 170. ) {
      mt2bl_event.set_momenta( lep, p1, p2, pmiss );
      double amt2_temp = mt2bl_event.get_mt2bl();    
      mt2bl_event.set_momenta( lep, p2, p1, pmiss );
      amt2_value = mt2bl_event.get_mt2bl();  
      if (amt2_temp < amt2_value ) amt2_value = amt2_temp;
    }
    else if ( (lepton + bjets[0]).M() > 170. && (lepton + bjets[1]).M() > 170. ) {
      mlb_flag = false;
      mt2bl_event.set_momenta( lep, p1, p2, pmiss );
      double amt2_temp = mt2bl_event.get_mt2bl();    
      mt2bl_event.set_momenta( lep, p2, p1, pmiss );
      amt2_value = mt2bl_event.get_mt2bl(); 
      if (amt2_temp < amt2_value ) amt2_value = amt2_temp;
    }      
    
    double mt = sqrt(2.* (lepton.Perp()*met - lepton.Px()*missingET->P4().Px() - lepton.Py()*missingET->P4().Py()) );
    double HT = 0.;
    for (int i = 0; i < bjets.size(); i++)
      HT += bjets[i].Perp();
    if ( lightjets.size() > 0 ) HT += lightjets[0].Perp();
    double meff = HT + met;
    double metsig = met/sqrt(HT);
    
    bool in1 = true; bool in2 = true; bool in3 = true; bool ex1 = true;
    if (met < 200.) in1 = false; else countCutflowEvent("in1_05_met>200");
    if (met < 120.) in2 = false; else countCutflowEvent("in2_05_met>120");
    if (met < 220.) in3 = false; else countCutflowEvent("in3_05_met>220");
    if (met < 160.) ex1 = false; else countCutflowEvent("ex1_05_met>160");
    
    if (mt > 140. && in1) countCutflowEvent("in1_06_mt>140"); else in1 = false;
    if (mt > 140. && in2) countCutflowEvent("in2_06_mt>140"); else in2 = false;
    if (mt > 180. && in3) countCutflowEvent("in3_06_mt>180"); else in3 = false;
    if (mt > 120. && ex1) countCutflowEvent("ex1_06_mt>120"); else ex1 = false;
    
    if (meff > 300. && in1) countCutflowEvent("in1_07_meff>300"); else in1 = false;
    if (meff > 450. && in2) countCutflowEvent("in2_07_meff>450"); else in2 = false;
    if (meff > 650. && in3) countCutflowEvent("in3_07_meff>650"); else in3 = false;
    if (meff > 300. && ex1) countCutflowEvent("ex1_07_meff>300"); else ex1 = false;

    if (metsig > 8. && in1) countCutflowEvent("in1_08_metsig>8"); else in1 = false;
    if (metsig > 12. && in2) countCutflowEvent("in2_08_metsig>12"); else in2 = false;
    if (metsig > 5. && in3) countCutflowEvent("in3_08_metsig>5"); else in3 = false;
    if (metsig > 10. && ex1) countCutflowEvent("ex1_08_metsig>10"); else ex1 = false;        
    
    if (amt2_value > 180. && in1) countCutflowEvent("in1_09_amt2>180"); else in1 = false;
    if (amt2_value > 200. && in2) countCutflowEvent("in2_09_amt2>200"); else in2 = false;
    if (amt2_value > 180. && in3) countCutflowEvent("in3_09_amt2>180"); else in3 = false;
    if (amt2_value > 180. && ex1) countCutflowEvent("ex1_09_amt2>180"); else ex1 = false;    
    
    if (mlb_flag && in1) countCutflowEvent("in1_10_mbl<170"); else in1 = false;
    if (mlb_flag && in2) countCutflowEvent("in2_10_mbl<170"); else in2 = false;
    if (mlb_flag && in3) countCutflowEvent("in3_10_mbl<170"); else in3 = false;
    if (mlb_flag && ex1) countCutflowEvent("ex1_10_mbl<170"); else ex1 = false;
    
    if (dphi_min > 0.4 && in1) countCutflowEvent("in1_11_dphi<0.4"); else in1 = false;
    if (dphi_min > 0.4 && in2) countCutflowEvent("in2_11_dphi<0.4"); else in2 = false;
    if (dphi_min > 0.4 && in3) countCutflowEvent("in3_11_dphi<0.4"); else in3 = false;
    if (dphi_min > 0.4 && ex1) countCutflowEvent("ex1_11_dphi<0.4"); else ex1 = false;
     
    if ( vetojet < 2 && ex1) countCutflowEvent("ex1_12_Nxjets"); else ex1 = false;
    
    if ( in1 ) countSignalEvent("SRinA");
    if ( in2 ) countSignalEvent("SRinB");
    if ( in3 ) countSignalEvent("SRinC");
    if ( ex1 ) countSignalEvent("SRexA");
    
    if (met > 200. && mt > 140. && meff > 300. && amt2_value < 160. && mlb_flag && dphi_min > 0.4 && metsig > 8 ) countCutflowEvent("in1_20_CRT");
    if (met > 120. && mt > 140. && meff > 450. && amt2_value < 160. && mlb_flag && dphi_min > 0.4 && metsig > 12 ) countCutflowEvent("in2_20_CRT");
    if (met > 220. && mt > 180. && meff > 650. && amt2_value < 160. && mlb_flag && dphi_min > 0.4 && metsig > 5 ) countCutflowEvent("in3_20_CRT");
    if (met > 160. && mt > 120. && meff > 300. && amt2_value < 180. && mlb_flag && dphi_min > 0.4 && metsig > 10 && vetojet < 2 ) countCutflowEvent("ex1_20_CRT");
  }
}

void Atlas_1506_08616::finalize() {
  // Whatever should be done after the run goes here
}       
