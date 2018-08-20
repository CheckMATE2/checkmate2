#include "atlas_conf_2017_039.h"
// AUTHOR: Daniel Dercks
//  EMAIL: daniel.dercks@desy.de
void Atlas_conf_2017_039::initialize() {
  setAnalysisName("atlas_conf_2017_039");          
  setInformation(""
		 "# Search for electroweak production\n"
		 "# of SUSY particles in the two or\n"
		 "# three lepton final state\n"
		 "# ATLAS, 2017\n"
		 "# sqrt(s) = 13 TeV\n"
		 "# int(L) = 36.1 invfb\n"
		 "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("SR2-SF-a;SR2-SF-b;SR2-SF-c;SR2-SF-d;SR2-SF-e;SR2-SF-f;SR2-SF-g;SR2-SF-h;SR2-SF-i;SR2-SF-j;SR2-SF-k;SR2-SF-l;SR2-SF-m;SR2-DF-a;SR2-DF-b;SR2-DF-c;SR2-DF-d;SR2-SF-Loose;SR2-SF-tight;SR2-DF-100;SR2-DF-150;SR2-DF-200;SR2-DF-300;SR2-int;SR2-high;SR2-low-combined;SR3-WZ-0Ja;SR3-WZ-0Jb;SR3-WZ-0Jc;SR3-WZ-1Ja;SR3-WZ-1Jb;SR3-WZ-1Jc;SR3-slep-a;SR3-slep-b;SR3-slep-c;SR3-slep-d;SR3-slep-e");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_conf_2017_039::analyze() {
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

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);

  std::vector<Jet*> bjets;
  std::vector<Jet*> nobjets;
  for (Jet* j: jets) {
    if (checkBTag(j))
      bjets.push_back(j);
    else
      nobjets.push_back(j);
  }

  //--------Overlap Removals
  photonsMedium = filterPhaseSpace(photonsMedium, 37., -2.47, 2.47, true); // Actually, is photonsTight (which we dont have)

  electronsLoose = overlapRemoval(electronsLoose, muonsCombined, 0.05); // "sharing an ID track"
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.2);
  nobjets = overlapRemoval(nobjets, electronsLoose, 0.2);
  electronsLoose = overlapRemoval(electronsLoose, nobjets, 0.4);
  electronsLoose = overlapRemoval(electronsLoose, bjets, 0.4);
  bjets = overlapRemoval(bjets, muonsCombined, 0.2);
  nobjets = overlapRemoval(nobjets, muonsCombined, 0.2);
  muonsCombined = overlapRemoval(muonsCombined, bjets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, nobjets, 0.4);
  
  // tighten electron cuts
  std::vector<Electron*> selected;
  for (Electron* e: electronsLoose) {
    for (Electron* et: electronsTight) {
      if (et == e) {
	selected.push_back(e);
	break;
      }
    }
  }
  electronsLoose = selected;
  //--------Isolation Criterions

  // pt dependent: 95% at 25 GeV to 99% at 60 GeV. Do logisitc curve
  selected.clear();
  for (Electron* e: electronsLoose) {
    double pt = e->PT;
    double randtoss = rand()/(RAND_MAX+1.);
    double eff = 0.95 + 0.04/(1. + 0.04/0.95 * exp(-0.3*(pt-40.)));
    if (randtoss < eff)
      selected.push_back(e);
  }
  electronsLoose = selected;

  selected.clear();
  std::vector<Muon*> selectedm;  
  for (Muon* m: muonsCombined) {
    double pt = m->PT;
    double randtoss = rand()/(RAND_MAX+1.);
    double eff = 0.95 + 0.04/(1. + 0.04/0.95 * exp(-0.3*(pt-40.)));
    if (randtoss < eff)
      selectedm.push_back(m);
  }
  muonsCombined = selectedm;

  

  //--------Triggers

  float  cutSingleTriggerElectronPT = 25.;
  float  cutSymTriggerElectronPT = 14.;
  float  cutAsymTriggerElectronPT1 = 25.;
  float  cutAsymTriggerElectronPT2 = 10.;
  float  cutMixedTrigger1ElectronPT = 14.;
  float  cutMixedTrigger2ElectronPT = 10.;  
  float  cutSingleTriggerMuonPT = 25.;
  float  cutSymTriggerMuonPT = 14.0;
  float  cutAsymTriggerMuonPT1 = 18.0;
  float  cutAsymTriggerMuonPT2 = 10.;
  float  cutMixedTrigger1MuonPT = 10.;
  float  cutMixedTrigger2MuonPT = 18.;


    //    Trigger Cuts
    bool trigger = false;
    // single electron trigger
    if( electronsTight.size() > 0 && electronsTight[0]->PT > cutSingleTriggerElectronPT )
    trigger = true;
    // single muon trigger
    else if( muonsCombined.size() > 0 && muonsCombined[0]->PT > cutSingleTriggerMuonPT  ) 
    trigger = true;
    //symmetric dielectron trigger
    else if( electronsTight.size() > 1 && electronsTight[0]->PT > cutSymTriggerElectronPT && electronsTight[1]->PT > cutSymTriggerElectronPT  )
    trigger = true;
    //asymmetric dielectron trigger
    else if( electronsTight.size() > 1 && electronsTight[0]->PT > cutAsymTriggerElectronPT1 && electronsTight[1]->PT > cutAsymTriggerElectronPT2 )
    trigger = true;
    //symmetric dimuon trigger
    else if( muonsCombined.size() > 1 && muonsCombined[0]->PT > cutSymTriggerMuonPT && muonsCombined[1]->PT > cutSymTriggerMuonPT )
    trigger = true;
  
    //asymmetric dimuon trigger
    else if( muonsCombined.size() > 1 && muonsCombined[0]->PT > cutAsymTriggerMuonPT1 && muonsCombined[1]->PT > cutAsymTriggerMuonPT2 )
    trigger=true;
    //mixed electron-muon trigger
    else if( electronsTight.size() > 0 && muonsCombined.size() > 0 && electronsTight[0]->PT > cutMixedTrigger1ElectronPT && muonsCombined[0]->PT > cutMixedTrigger1MuonPT  )
    trigger = true;
    //mixed electron-muon trigger
    else if( electronsTight.size() > 0 && muonsCombined.size() > 0 && electronsTight[0]->PT > cutMixedTrigger2ElectronPT && muonsCombined[0]->PT > cutMixedTrigger2MuonPT)
    trigger = true;


  // Do later, details also unknown

  countCutflowEvent("CR0_0All");

    if( !trigger )
      return;
    countCutflowEvent("CR0_1Trigger");
  // Lepton counting
  std::vector<FinalStateObject*> leptons;
  std::vector<FinalStateObject*> sortedLeptons;
  for(int e = 0; e < electronsLoose.size(); e++) {
    FinalStateObject* lep = newFinalStateObject(electronsLoose[e]);
    leptons.push_back(lep);
    sortedLeptons.push_back(lep);
  }  
  for(int m = 0; m < muonsCombined.size(); m++) {
    FinalStateObject* lep = newFinalStateObject(muonsCombined[m]);
    leptons.push_back(lep);
    sortedLeptons.push_back(lep);
  }

  if (leptons.size() < 2 || leptons.size() > 3)
    return;
    
  countCutflowEvent("CR0_1_2or3ell");    
  // Check this!
  for (int i = 0; i < sortedLeptons.size(); i++) {
    for (int j = i+1; j < sortedLeptons.size(); j++) {
      if (sortedLeptons[i]->PT < sortedLeptons[j]->PT) {
	FinalStateObject* x = sortedLeptons[i];
	sortedLeptons[i] = sortedLeptons[j];
	sortedLeptons[j] = x;
      }
    }
  }
  std::cout << "Sorted Leptons ";
  for (FinalStateObject* fs : sortedLeptons)
    std::cout << fs->PT << "   ";
  std::cout << std::endl;

  if (sortedLeptons[0]->PT < 25. || sortedLeptons[1]->PT < 20.)
    return;
    
  countCutflowEvent("CR0_2_leppT");    

  // 2l
  if (leptons.size() == 2) {
    countCutflowEvent("CR2l_0_All");    
    if (leptons[0]->Charge*leptons[1]->Charge == 1)
      return;
	
    countCutflowEvent("CR2l_1_OS");
    double minv = (leptons[0]->P4() + leptons[1]->P4()).M();
    bool df = (leptons[0]->Type != leptons[1]->Type);
    if (minv < 40.)
      return;
	
    countCutflowEvent("CR2l_2_minv");
    std::vector<Jet* > centralnob = filterPhaseSpace(nobjets, 60, -2.4, 2.4);
    std::vector<Jet* > centralb = filterPhaseSpace(bjets, 20, -2.4, 2.4);
	
    if (centralnob.size() + centralb.size() == 0) {	
      countCutflowEvent("CR2l0j_0_All");
      double mt2 = mT2(leptons[0]->P4(), leptons[1]->P4(), 0.);
      if (mt2 >= 100 && mt2 <= 150) {
	if (df && minv >= 111.)
	  countSignalEvent("SR2-DF-a");
	else if (!df && minv >= 111. && minv <= 150.)
	  countSignalEvent("SR2-SF-a");
	else if (!df && minv > 150. && minv <= 200.)
	  countSignalEvent("SR2-SF-b");
	else if (!df && minv > 200. && minv <= 300.)
	  countSignalEvent("SR2-SF-c");
	else if (!df && minv > 300.)
	  countSignalEvent("SR2-SF-d");
      }
      else if (mt2 > 150. && mt2 <= 200.) {
	if (df && minv >= 111.)
	  countSignalEvent("SR2-DF-b");
	else if (!df && minv >= 111. && minv <= 150.)
	  countSignalEvent("SR2-SF-e");
	else if (!df && minv > 150. && minv <= 200.)
	  countSignalEvent("SR2-SF-f");
	else if (!df && minv > 200. && minv <= 300.)
	  countSignalEvent("SR2-SF-g");
	else if (!df && minv > 300.)
	  countSignalEvent("SR2-SF-h");
      }
      else if (mt2 > 200. && mt2 <= 300.) {
	if (df && minv >= 111.)
	  countSignalEvent("SR2-DF-c");
	else if (!df && minv >= 111. && minv <= 150.)
	  countSignalEvent("SR2-SF-i");
	else if (!df && minv > 150. && minv <= 200.)
	  countSignalEvent("SR2-SF-j");
	else if (!df && minv > 200. && minv <= 300.)
	  countSignalEvent("SR2-SF-k");
	else if (!df && minv > 300.)
	  countSignalEvent("SR2-SF-l");
      }
      else if (mt2 > 300. && minv > 111) {
	if (df)
	  countSignalEvent("SR2-DF-d");
	else
	  countSignalEvent("SR2-SF-m");
      }

      if (mt2 > 100. && minv > 111 && !df)
	countSignalEvent("SR2-SF-Loose");
      if (mt2 > 130. && minv > 300 && !df)
	countSignalEvent("SR2-SF-tight");
      if (mt2 > 100. && df)
	countSignalEvent("SR2-DF-100");
      if (mt2 > 150. && df)
	countSignalEvent("SR2-DF-150");
      if (mt2 > 200. && df)
	countSignalEvent("SR2-DF-200");
      if (mt2 > 300. && df)
	countSignalEvent("SR2-DF-300");
    }
    else if (centralb.size() == 0) {	    
      countCutflowEvent("CR2lj_0_All");
      // Do one of the 2l+jets SR after the other
      if (sortedLeptons[1]->PT < 25.)
	return;
      countCutflowEvent("CR2lj_1_subleadpt");
      if (df)
	return;
      countCutflowEvent("CR2lj_1_2_SF");
      if (nobjets.size() < 2 || nobjets[1]->PT < 30.)
	return;
      countCutflowEvent("CR2lj_2_twohardjets");

      TLorentzVector pW = nobjets[0]->P4() + nobjets[1]->P4();
      TLorentzVector pZ = leptons[0]->P4() + leptons[1]->P4();
      double pTZ = (pZ).Pt();
      double pTW = (pW).Pt();
	    
      if (nobjets.size() >= 2) {
	countCutflowEvent("CR2lj_int_0_jets");
	if (minv >= 81. &&  minv <= 101) {
	  countCutflowEvent("CR2lj_int_1_minv");
		    
	  // In int, the two leading jets define the W
	  /*
	  // Here we have to redefine pW, which currently is centralnob0+centralnob1.
	  // two jets closest to the zll+etmiss system that have a mass mjj consistent with the mass of the w boson.
	  TLorentzVector zlletmiss = pZ + missingET->P4();
                    
	  double deltaRmin = pW.DeltaR(zlletmiss);
	  int iW1 = 0, iW2 = 0;
	  for (int i = 0; i < centralnob.size(); i++) {
	  for (int j = i+1; j < centralnob.size(); j++) {
	  TLorentzVector pWcand = centralnob[i]->P4() + centralnob[j]->P4();                            
	  if (70 < pWcand.M() && pWcand.M() < 90 && pWcand.DeltaR(zlletmiss) < deltaRmin) {
	  pW = pWcand;
	  iW1 = i;
	  iW2 = j;
	  deltaRmin = pWcand.DeltaR(zlletmiss);
	  }
	  }
	  }
	  */
                    
	  double mjj = (pW).M();
	  std::cout << "|nj mjj| " << nobjets.size() << "  " << nobjets[0]->PT << "  " << nobjets[1]->PT << "  " << mjj << std::endl;
	  if (mjj >= 70. && mjj <= 100.) {
	    countCutflowEvent("CR2lj_int_2_mjj");
	    if (missingET->PT >= 150.) { // The 250 special cut for "high" is tested at the end
	      countCutflowEvent("CR2lj_int_3_etmiss");
	      if (pTZ >= 80.) {
		countCutflowEvent("CR2lj_int_4_ptZ");		
		if (pTW >= 100.) {
		  countCutflowEvent("CR2lj_int_4_ptW");
		  double mt2 = mT2(leptons[0]->P4(), leptons[1]->P4(), 0.);
		  if (mt2 >= 100.) {
		    countCutflowEvent("CR2lj_int_5_mT2");
		    if (nobjets[0]->P4().DeltaR(nobjets[1]->P4()) < 1.5) {
		      countCutflowEvent("CR2lj_int_6_dRjj");					    
		      if (leptons[0]->P4().DeltaR(leptons[1]->P4()) < 1.8) {
			countCutflowEvent("CR2lj_int_7_dRll");	
			double dPhiW = fabs(missingET->P4().DeltaPhi(pW));
			if (0.5 <= dPhiW && dPhiW <= 3.0) {
			  countCutflowEvent("CR2lj_int_8_dPhiW");	
			  countSignalEvent("SR2-int");
			  if (missingET->PT > 250.) {
			    countCutflowEvent("CR2lj_int_9_hardMET");	
			    countSignalEvent("SR2-high");
			  }
			}
		      }
		    }
		  }
		}	    
	      }
	    }
	  }
	}
      }
      if (nobjets.size() == 2) {
	countCutflowEvent("CR2lj_low2j_0_jets");
	if (minv >= 81. && minv <= 101) {
	  countCutflowEvent("CR2lj_low2j_1_minv");
	  // Since there are only two jets, they must originate from the W
	  double mjj = pW.M();
	  if (mjj >= 70 && mjj <= 90) {
	    countCutflowEvent("CR2lj_low2j_2_mjj");
	    if (missingET->PT >= 100.) { // The 250 special cut for "high" is tested at the end
	      countCutflowEvent("CR2lj_low2j_3_etmiss");
	      if (pTZ >= 60.) {
		countCutflowEvent("CR2lj_low2j_4_ptZ");		                                
		double dPhiZ = fabs(missingET->P4().DeltaPhi(pZ));
		std::cout << "|dPhiZ|   " << dPhiZ << std::endl;
		double dPhiW = fabs(missingET->P4().DeltaPhi(pW));
		if (dPhiZ <= 0.8) {
		  countCutflowEvent("CR2lj_low2j_5_dPhiZ");	
		  if (dPhiW > 1.5) {
		    countCutflowEvent("CR2lj_low2j_6_dPhiW");	
		    if (0.6 < missingET->PT / pTZ && missingET->PT / pTZ  < 1.6)
		      countCutflowEvent("CR2lj_low2j_7_METoverpTZ");	                                        
		    if (missingET->PT / pTW  < 0.8) {
		      countCutflowEvent("CR2lj_low2j_8_METoverpTW");	                                                    
		      countSignalEvent("SR2-low-2J");
		      countSignalEvent("SR2-low-combined");
		    }
                                            
		  }
		}	    
	      }
	    }
	  }
	}
      }	    
      if (3 <= nobjets.size() && nobjets.size() <= 5 ) {
	countCutflowEvent("CR2lj_low3j_00_jets");
                
	if (minv >= 86. && minv <= 96) {
	  countCutflowEvent("CR2lj_low3j_01_minv");
	  // Here we have to redefine pW, which currently is centralnob0+centralnob1.
	  // "two jets closest to the zll+etmiss system that have a mass mjj consistent with the mass of the w boson.
	  TLorentzVector zlletmiss = pZ + missingET->P4();
                    
	  double deltaRmin = pW.DeltaR(zlletmiss);
	  int iW1 = 0, iW2 = 1;
	  for (int i = 0; i < nobjets.size(); i++) {
	    for (int j = i+1; j < nobjets.size(); j++) {
	      TLorentzVector pWcand = nobjets[i]->P4() + nobjets[j]->P4();                            
	      std::cout << "|combination|  " << i << "   " << j << "  " << pWcand.M() << "  " <<  pWcand.DeltaR(zlletmiss) << std::endl;
	      if (70 < pWcand.M() && pWcand.M() < 90 && pWcand.DeltaR(zlletmiss) < deltaRmin) {
		pW = pWcand;
		iW1 = i;
		iW2 = j;
		deltaRmin = pWcand.DeltaR(zlletmiss);
	      }
	    }
	  }
                    
	  double mjj = pW.M();
	  std::cout << "|combinationwinner|  " << iW1 << "  " << iW2 << "  " << "  " << mjj << "  " << deltaRmin << std::endl;
	  if (mjj >= 70 && mjj <= 90) {
	    countCutflowEvent("CR2lj_low3j_02_mjj");
	    if (missingET->PT >= 100.) { // The 250 special cut for "high" is tested at the end
	      countCutflowEvent("CR2lj_low3j_03_etmiss");
	      if (pTZ >= 40.) {
		countCutflowEvent("CR2lj_low3j_04_low3j");		
		double dRjj = nobjets[iW1]->P4().DeltaR(nobjets[iW2]->P4());
		if (dRjj < 2.2) {
		  countCutflowEvent("CR2lj_low3j_05_dRjj");					    
		  double dPhiW = fabs(missingET->P4().DeltaPhi(pW));
		  if (dPhiW <= 2.2) {
		    countCutflowEvent("CR2lj_low3j_06_dPhiW");	
		    TLorentzVector pISR;
		    for (int i = 0; i < nobjets.size(); i++) {
		      if (i != iW1 && i != iW2)
			pISR += nobjets[i]->P4();
		    }
		    if (fabs(missingET->P4().DeltaPhi(pISR)) > 2.4) {
		      countCutflowEvent("CR2lj_low3j_07_dPhiMETISR");
		      if (fabs(missingET->P4().DeltaPhi(nobjets[0]->P4())) > 2.6) {
			countCutflowEvent("CR2lj_low3j_08_dPhiMETJet1");
			if (0.4 < missingET->PT/pISR.Pt() && missingET->PT/pISR.Pt() < 0.8) {
			  countCutflowEvent("CR2lj_low3j_09_METoverISR");
			  if (fabs(pZ.PseudoRapidity()) < 1.6) {                                                        
			    countCutflowEvent("CR2lj_low3j_10_zpseudo");
			    if (nobjets[2]->PT > 30.) {                                                                                                           
			      countCutflowEvent("CR2lj_low3j_11_pt3");
			      countSignalEvent("SR2-low-3J");
			      countSignalEvent("SR2-low-combined");
			    }
			  }
			}
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }                                            
    }
  }
  else if (leptons.size() == 3) {
    countCutflowEvent("CR3l_0_All");  
        
    //std::vector<Jet* > centralnob = nobjets; //filterPhaseSpace(nobjets, 60, -2.4, 2.4);
    //std::vector<Jet* > centralb = bjets; //filterPhaseSpace(bjets, 20, -2.4, 2.4);
        
    if (bjets.size() == 0) {
      countCutflowEvent("CR3l_1_bveto");  
                
      // Find the SFOS pair
      double msfos = 1E99;
      int i3 = -1;
      double mTmin = 1E99;
      bool sfos = false;
      for (int i = 0; i < 3; i++) {
	for (int j = i+1; j < 3; j++) {
	  if (leptons[i]->Type == leptons[j]->Type && leptons[i]->Charge != leptons[j]->Charge) {                    
	    sfos = true;
	    // 3 - i - j is the index of the unpaired lepton since i + j + k = 3
	    double mt = mT(leptons[3-i-j]->P4(), missingET->P4());
	    std::cout << "|lepcombination|  " << i << "  " << j << "   " << 3-i-j << "  " << mt << "  " << (leptons[i]->P4()+leptons[j]->P4()).M() << std::endl;
	    if (mt < mTmin) {
	      mTmin = mt;
	      i3 = 3-i-j;
	      msfos = (leptons[i]->P4()+leptons[j]->P4()).M();
	    }
	  }
	}
      }        
      std::cout << "|lepcombinationswinner|  " << mTmin << "   " << i3 << "  " << msfos << std::endl;
      if (sfos) {
	countCutflowEvent("CR3l_2_msfos");  
                
	if (msfos < 81.2) {
	  countCutflowEvent("CR3l_SR3-slep-ab-1-msfos");  
	  if (missingET->PT > 130.) {
	    countCutflowEvent("CR3l_SR3-slep-ab-2-met");
	    if (sortedLeptons[2]->PT > 20) {                            
	      countCutflowEvent("CR3l_SR3-slep-ab-3-ptl3");
	      if (mTmin > 110) {                                            
		countCutflowEvent("CR3l_SR3-slep-ab-4-mtmin");
		if (sortedLeptons[2]->PT < 30) {
		  countSignalEvent("SR3-slep-a");
		}
		else {
		  countSignalEvent("SR3-slep-b");
		}
	      }
	    }
	  }
	}
                
	if (msfos > 101.2) {
	  countCutflowEvent("CR3l_SR3-slep-cde-1-msfos");  
	  if (missingET->PT > 130.) {
	    countCutflowEvent("CR3l_SR3-slep-cde-2-met");
	    if (sortedLeptons[2]->PT > 20) {                            
	      countCutflowEvent("CR3l_SR3-slep-cde-3-ptl3");
	      if (mTmin > 110) {                                            
		countCutflowEvent("CR3l_SR3-slep-cde-4-mtmin");
		if (sortedLeptons[2]->PT < 50) {
		  countSignalEvent("SR3-slep-c");
		}
		else if (sortedLeptons[2]->PT < 80) {
		  countSignalEvent("SR3-slep-d");
		}
		else {
		  countSignalEvent("SR3-slep-e");
		}
	      }
	    }
	  }
	}
                
	if (81.2 < msfos && msfos < 101.2) {
	  countCutflowEvent("CR3l-SR3-WZ-1-msfos");  
	  if (nobjets.size() == 0) {
	    countCutflowEvent("CR3l-WZ-0J-1-All");
	    if (mTmin > 110) {                            
	      countCutflowEvent("CR3l-WZ-0J-2-mTmin");
	      if (60 < missingET->PT)
		countCutflowEvent("CR3l-WZ-0J-3-pT");	
	      std::cout << "|etmiss|  " << missingET->PT << "  " << (60. < missingET->PT && missingET->PT  < 120.) << "  " << "  " <<  (120. < missingET->PT && missingET->PT  < 170.) << "  " << std::endl;
	      if (60. < missingET->PT && missingET->PT  < 120.)
		countSignalEvent("SR3-WZ-0Ja");
	      else if (120. < missingET->PT && missingET->PT  < 170.)
		countSignalEvent("SR3-WZ-0Jb");                            
	      else if (170. < missingET->PT)
		countSignalEvent("SR3-WZ-0Jc");
	    }
	  }
	  else {
	    countCutflowEvent("CR3l-WZ-1J-1-All");
	    if (120 < missingET->PT && missingET->PT < 200) {
	      countCutflowEvent("CR3l-WZ-1Ja-1-All");
	      if (mTmin > 110) {                                
		countCutflowEvent("CR3l-WZ-1Ja-2-mTmin");
		if ((leptons[0]->P4()+leptons[1]->P4()+leptons[2]->P4()).Pt() < 120) {     
		  countCutflowEvent("CR3l-WZ-1Ja-3-pTlll");
		  if (nobjets[0]->PT > 70) {
		    countCutflowEvent("CR3l-WZ-1Ja-4-ptjet1");
		    countSignalEvent("SR3-WZ-1Ja");
		  }
		}
	      }
	    }
                        
	    if (missingET->PT > 200) {
	      countCutflowEvent("CR3l-WZ-1Jab1-All");
	      if (110 < mTmin && mTmin < 160) {                                
		countCutflowEvent("CR3l-WZ-1Jb-1-mTmin");
		countSignalEvent("SR3-WZ-1Jb");
	      }
	      if (mTmin > 160) {
		countCutflowEvent("CR3l-WZ-1Jc-1-mTmin");
		if (sortedLeptons[2]->PT > 35) {
		  countCutflowEvent("CR3l-WZ-1Jc-2-pT3");
		  countSignalEvent("SR3-WZ-1Jc");
		}
	      }
	    }
	  }
	}
      }
    }
  }
}

void Atlas_conf_2017_039::finalize() {
  // Whatever should be done after the run goes here
}       

