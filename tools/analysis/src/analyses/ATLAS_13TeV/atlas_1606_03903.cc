#include "atlas_1606_03903.h"
#include "fastjet/tools/Filter.hh"
// AUTHOR: K Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1606_03903::initialize() {
  setAnalysisName("atlas_1606_03903");          
  setInformation(""
    "# search for stops and gluinos\n"
    "# 1 lepton + (b) jets + MET\n"
    "# 3.2/fb, 13 TeV, 2015\n"
  "");
  setLuminosity(3.2*units::INVFB);      
  bookSignalRegions("SR1;SR2;SR3;");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.
  bookCutflowRegions("00_all;01_>1baselep;02_>1siglep;03_=1siglep;04_=1baselep;05_met>100;06_>3sigjets;07_met>200;09_jet1phi;10_jet2phi;11_tauveto;SR1_12_jet1>80;SR1_13_jet2>50;SR1_14_jet3>40;SR1_15_jet4>40;SR1_16_met>260;SR1_17_missHTsig;SR1_18_mT;SR1_19_amT2;SR1_20_topness;SR1_21_mchitop;SR1_22_dRbl;SR1_23_btag>0;");
  bookCutflowRegions("SR2_12_jet1>120;SR2_13_jet2>80;SR2_14_jet3>50;SR2_15_met;SR2_16_missHTsig;SR2_17_mT;SR2_18_amT2;SR2_19_dRbl;SR2_20_btag;SR2_21_fatjet>200;SR2_22_fatjetmet;");
  bookCutflowRegions("SR3_12_jet1>120;SR3_13_jet2>80;SR3_14_jet3>50;SR3_15_met;SR3_16_missHTsig;SR3_17_mT;SR2_18_amT2;SR3_19_topness;SR3_20_dRbl;SR3_21_btag;SR3_22_fatjet>280;");
//  bookCutflowRegions("MET260;MET340;MET420;MET500;MET580;");
//  bookCutflowRegions("MT120;MT220;MT320;MT420;MT520;MT620;");  

  // You should initialize any declared variables here
//  histo_met = new TH1D("met", "met", 8, 260, 900);
//  histo_mt = new TH1D("mt", "mt", 8, 120, 920);
//  histo_top= new TH1D("top", "topness", 25, -10, 15);
//  histo_mtop= new TH1D("mtop", "mtop", 25, 0, 500);
//  histo_ht= new TH1D("ht", "ht", 25, 200, 700);
//  histo_st= new TH1D("st", "st", 25, 0, 30);  
}

void Atlas_1606_03903::analyze() {
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
  countCutflowEvent("00_all");
  
  std::vector<Electron*> electronsBase = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47);     
  std::vector<Muon*> muonsBase = filterPhaseSpace(muonsCombined, 6., -2.7, 2.7);   
  std::vector<Jet*> jetsBase = filterPhaseSpace(jets, 20., -2.5, 2.5);
  std::vector<Jet*> jetsTau;
  std::vector<Jet*> jetsBtagBase;
  std::vector<Jet*> jetsLightBase;
  std::vector<Jet*> jetsSig;
  
  for (int i = 0; i < jetsBase.size(); i++)
    if ( checkTauTag(jetsBase[i],"loose") ) 
      jetsTau.push_back(jetsBase[i]);
    
  for (int i = 0; i < jetsBase.size(); i++)
    if ( checkBTag(jetsBase[i]) ) 
      jetsBtagBase.push_back(jetsBase[i]);  
    else 
      jetsLightBase.push_back(jetsBase[i]);
    
  muonsBase = overlapRemoval(muonsBase, electronsBase, 0.01) ; 
  jetsLightBase = overlapRemoval(jetsLightBase, electronsBase, 0.2);
  jetsLightBase = Atlas_1606_03903::specialoverlap2(jetsLightBase, muonsBase, 0.2, 0.7);
  muonsBase = Atlas_1606_03903::specialoverlap(muonsBase, jetsLightBase);    
  muonsBase = Atlas_1606_03903::specialoverlap(muonsBase, jetsBtagBase);    
  electronsBase = Atlas_1606_03903::specialoverlap(electronsBase, jetsLightBase);    
  electronsBase = Atlas_1606_03903::specialoverlap(electronsBase, jetsBtagBase);      
  jetsTau = overlapRemoval(jetsTau, electronsBase, 0.1);

  std::vector<Electron*> electronsSig = filterPhaseSpace(electronsBase, 25., -2.47, 2.47);  
  electronsSig = filterIsolation(electronsSig);
  std::vector<Muon*> muonsSig = filterPhaseSpace(muonsBase, 25., -2.7, 2.7);
  muonsSig = filterIsolation(muonsSig);
  std::vector<Jet*> jetsLightSig = filterPhaseSpace(jetsLightBase, 25., -2.5, 2.5);
  std::vector<Jet*> jetsBSig = filterPhaseSpace(jetsBtagBase, 25., -2.5, 2.5);		
  
  double met = missingET->P4().Et();
  
  if (electronsBase.size() + muonsBase.size() < 1 ) return;
  countCutflowEvent("01_>1baselep");
  
  if ( muonsSig.size() + electronsSig.size() < 1) return;
  countCutflowEvent("02_>1siglep");	
  
  if ( muonsSig.size() + electronsSig.size() != 1) return;
  countCutflowEvent("03_=1siglep");
  
  if (electronsBase.size() + muonsBase.size() != 1 ) return;
  countCutflowEvent("04_=1baselep")  ;

  if (met < 100. ) return;
  countCutflowEvent("05_met>100");
  
  jetsSig = jetsLightSig;
  jetsSig.insert(jetsSig.end(), jetsBSig.begin(), jetsBSig.end());
  std::sort(jetsSig.begin(), jetsSig.end(), Atlas_1606_03903::sortByPT );  
    
  if (jetsSig.size() < 4) return;
  countCutflowEvent("06_>3sigjets");

  if (met < 200. ) return;
  countCutflowEvent("07_met>200");
  
  TLorentzVector lepton;
  if ( muonsSig.size() == 1 ) lepton = muonsSig[0]->P4();
  else lepton = electronsSig[0]->P4();	
  
  if( fabs(jetsSig[0]->P4().DeltaPhi(missingET->P4())) < 0.4 ) return;
  countCutflowEvent("09_jet1phi");
    
  if( fabs(jetsSig[1]->P4().DeltaPhi(missingET->P4())) < 0.4 ) return;  
  countCutflowEvent("10_jet2phi");
  
  for (int i = 0; i < jetsTau.size(); i++) 
    if ( AnalysisBase::mT2(jetsTau[i]->P4(), lepton, 0., missingET->P4()) < 80. ) return;
  countCutflowEvent("11_tauveto");  

// now calculate remaining quantities

// https://atlas.web.cern.ch/Atlas/GROUPS/PHYSICS/PLOTS/JETM-2016-010/
  double sigma2 = 0.;
  TLorentzVector missHT = -lepton;
  for (int i = 0; i < jetsSig.size(); i++) {
    if (jetsSig[i]->PT <100.) {
      sigma2 += pow( (jetsSig[i]->PT*(((jetsSig[i]->PT - 20.)*(-0.04/80.)) + 0.06 )), 2);
//      sigma2 += pow( (jetsSig[i]->PT*0.05) ,2);
      missHT -= jetsSig[i]->P4();
    }
    else {
      sigma2 += pow( (jetsSig[i]->PT*0.02) ,2);
//      sigma2 += pow( (jetsSig[i]->PT*0.035) ,2);
      missHT -= jetsSig[i]->P4();
    }
  }
// have to add a MET-dependent correction factor to match cutflows
//  if (met < 500.) sigma2 *= pow(500./met, 2); 
  if (missHT.Pt() < 400.) sigma2 *= 4; 
  else if (missHT.Pt() < 600.) sigma2 *= 10. - 3./200.*missHT.Pt() ;
  double missHT_Sig = (missHT.Pt() - 100.)/sqrt(sigma2);
  
  double mT = AnalysisBase::mT(lepton, missingET->P4());
  
  // Need to calculate aMT2 -> need two b-jets (use 77% eff) (if not two b-jets, use hardest other jet)
  double amT2 = 0.;
  if ( jetsBSig.size() > 1 ) {	
    double amT2_1 = AnalysisBase::mT2_bl(lepton, jetsBSig[0]->P4(), jetsBSig[1]->P4(), missingET->P4());
    double amT2_2 = AnalysisBase::mT2_bl(lepton, jetsBSig[1]->P4(), jetsBSig[0]->P4(), missingET->P4());
    amT2 = std::min(amT2_1, amT2_2);
  }
  else if ( jetsBSig.size() == 1 ) {	
    double amT2_1 = AnalysisBase::mT2_bl(lepton, jetsBSig[0]->P4(), jetsLightSig[0]->P4(), missingET->P4());
    double amT2_2 = AnalysisBase::mT2_bl(lepton, jetsLightSig[0]->P4(), jetsBSig[0]->P4(), missingET->P4());
    amT2 = std::min(amT2_1, amT2_2);
  }
  else if ( jetsBSig.size() == 0 ) {	
    double amT2_1 = AnalysisBase::mT2_bl(lepton, jetsLightSig[0]->P4(), jetsLightSig[1]->P4(), missingET->P4());
    double amT2_2 = AnalysisBase::mT2_bl(lepton, jetsLightSig[1]->P4(), jetsLightSig[0]->P4(), missingET->P4());
    amT2 = std::min(amT2_1, amT2_2);
  }  
  
  TLorentzVector top_cand, w_cand; 
  double mchitop = 500.; 
  double chi2 = 10000.;
  
    
  if ( jetsLightSig.size() > 1) //appendix of 1407.0583
    for ( int i = 0; i < jetsBSig.size(); i++ ) 
      for ( int j = 0; j < jetsLightSig.size()-1 ; j++ ) 
	for ( int k = j+1 ; k < jetsLightSig.size() ; k++ ) {
	  top_cand = jetsBSig[i]->P4() + jetsLightSig[j]->P4() + jetsLightSig[k]->P4();
	  w_cand = jetsLightSig[j]->P4() + jetsLightSig[k]->P4();
	  double sigmat2 = top_cand.M()*top_cand.M()*(3.*0.01); //assumed r = 0.1
	  double sigmaw2 = w_cand.M()*w_cand.M()*(2.*0.01);
	  double chi2_temp = pow( top_cand.M() - 173., 2)/sigmat2;
	  if ( chi2_temp < chi2 ) { 
	    mchitop = top_cand.M();
	    chi2 = chi2_temp;
	  }
	}      
    
//  histo_mtop->Fill(mchitop);  

  if ( jetsBSig.size() == 0) {
    top_cand = jetsLightSig[0]->P4() + jetsLightSig[1]->P4() + jetsLightSig[2]->P4();
    mchitop = top_cand.M();
  }  
  
  double dRbl = 0.;
  if ( jetsBSig.size() > 0 ) dRbl = jetsBSig[0]->P4().DeltaR(lepton);
  else dRbl = jetsLightSig[0]->P4().DeltaR(lepton);

  double top_ness;
  if ( jetsBSig.size() > 1 ) 
    top_ness = topness(lepton, jetsBSig[0]->P4(), jetsBSig[1]->P4(), missingET->P4());
  else if ( jetsBSig.size() == 1 ) {
    double top_ness1 = topness(lepton, jetsBSig[0]->P4(), jetsLightSig[0]->P4(), missingET->P4());
    double top_ness2 = topness(lepton, jetsBSig[0]->P4(), jetsLightSig[1]->P4(), missingET->P4());
    top_ness = std::min(top_ness1, top_ness2);
  }
  else
    top_ness = topness(lepton, jetsSig[0]->P4(), jetsSig[1]->P4(), missingET->P4());
  
//  if ( jetsBSig.size() > 0 ) histo_top->Fill(top_ness[0]);
  
// do fatjets
  std::vector<fastjet::PseudoJet> particles;
  for ( int i = 0; i < jetsSig.size(); i++ ) {
    double tmppx = jetsSig[i]->P4().Px(); 
    double tmppy = jetsSig[i]->P4().Py(); 
    double tmppz = jetsSig[i]->P4().Pz(); 
    double tmpe  = jetsSig[i]->P4().E(); 
    particles.push_back( fastjet::PseudoJet(tmppx, tmppy, tmppz, tmpe) );
  }  
  
  fastjet::JetDefinition jet_def_1(fastjet::antikt_algorithm, 1.0);
  fastjet::JetDefinition jet_def_12(fastjet::antikt_algorithm, 1.2);
  
  fastjet::ClusterSequence cs1(particles, jet_def_1);
  fastjet::ClusterSequence cs12(particles, jet_def_12);
  std::vector<fastjet::PseudoJet> compjets1 = sorted_by_pt(cs1.inclusive_jets());
  std::vector<fastjet::PseudoJet> compjets12 = sorted_by_pt(cs12.inclusive_jets());
  
  fastjet::Filter trimmer1(0.4, fastjet::SelectorPtFractionMin(0.05));
  fastjet::Filter trimmer12(0.4, fastjet::SelectorPtFractionMin(0.05));
  
  std::vector<fastjet::PseudoJet> temp1;
  std::vector<fastjet::PseudoJet> temp12;
  for ( int i = 0; i < compjets1.size(); i++ ) 
    temp1.push_back(trimmer1(compjets1[i]));
  for ( int i = 0; i < compjets12.size(); i++ ) 
    temp12.push_back(trimmer12(compjets12[i]));  
  
  std::vector<fastjet::PseudoJet> large_jets1 = sorted_by_pt(temp1);
  std::vector<fastjet::PseudoJet> large_jets12 = sorted_by_pt(temp12);  
  
// now signal regions
  if ( jetsSig[0]->PT > 80.) { //SR1
    countCutflowEvent("SR1_12_jet1>80");
    bool sr1 = true;
    
    if ( jetsSig[1]->PT > 50.) countCutflowEvent("SR1_13_jet2>50");
    else sr1 = false;
    
    if (sr1 && jetsSig[2]->PT > 40. ) countCutflowEvent("SR1_14_jet3>40");
    else sr1 = false;      
      
    if (sr1 && jetsSig[3]->PT > 40. ) countCutflowEvent("SR1_15_jet4>40");
    else sr1 = false;    
    
    if (sr1 && met > 260. ) countCutflowEvent("SR1_16_met>260");
    else sr1 = false;
	
    if (sr1 && missHT_Sig > 14. ) countCutflowEvent("SR1_17_missHTsig");	
    else sr1 = false;

    if (sr1 && mT > 170. )  countCutflowEvent("SR1_18_mT");	
    else sr1 = false;	

    if (sr1 && amT2 > 175. )  countCutflowEvent("SR1_19_amT2");	
    else sr1 = false;	

    if (sr1 && top_ness > 6.5 )  {
      countCutflowEvent("SR1_20_topness"); 
      //histo_mtop->Fill(mchitop);        
    } 
    else sr1 = false;		
	
    if (sr1 && mchitop < 270. ) countCutflowEvent("SR1_21_mchitop"); 	
    else sr1 = false;	
    
    if (sr1 && dRbl < 3. ) countCutflowEvent("SR1_22_dRbl");
    else sr1 = false;

    if (sr1 && jetsBSig.size() > 0 ) {
      countCutflowEvent("SR1_23_btag>0");
      countSignalEvent("SR1");
//    histo_met->Fill(met);
//    histo_mt->Fill(mT);
      
/*    if (met < 340.) countCutflowEvent("MET260");
      else if (met < 420.) countCutflowEvent("MET340");
      else if (met < 500.) countCutflowEvent("MET420");
      else if (met < 580.) countCutflowEvent("MET500");
      else countCutflowEvent("MET580");
      
      if (mT < 220.) countCutflowEvent("MT120");
      else if (mT < 320.) countCutflowEvent("MT220");
      else if (mT < 420.) countCutflowEvent("MT320");
      else if (mT < 520.) countCutflowEvent("MT420");
      else if (mT < 620.) countCutflowEvent("MT520");
      else countCutflowEvent("MT620");      */
    }
    
  }
  
  if ( jetsSig[0]->PT > 120.) {  //SR2	&& SR3
    
    countCutflowEvent("SR2_12_jet1>120");
    countCutflowEvent("SR3_12_jet1>120");

    bool sr2 = true;
    bool sr3 = true;
    
    if (jetsSig[1]->PT < 80.) return; 
    countCutflowEvent("SR2_13_jet2>80");
    countCutflowEvent("SR3_13_jet2>80");
    
    if ( jetsSig[2]->PT < 50.) return; 
    countCutflowEvent("SR2_14_jet3>50");
    countCutflowEvent("SR3_14_jet3>50");
  
    
    if (met > 350. ) {
      countCutflowEvent("SR2_15_met"); 
//      histo_ht->Fill(missHT.Pt()); 
//      histo_st->Fill(sqrt(sigma2));      
    }
    else sr2 = false;
   
    if (sr2 && missHT_Sig > 20. ) countCutflowEvent("SR2_16_missHTsig");	
    else sr2 = false;

    if (sr2 && mT > 200. )  countCutflowEvent("SR2_17_mT");	
    else sr2 = false;	

    if (sr2 && amT2 > 175. )  countCutflowEvent("SR2_18_amT2");	
    else sr2 = false;		

    if (sr2 && dRbl < 2.5 )  countCutflowEvent("SR2_19_dRbl");	
    else sr2 = false;		
    
    if (sr2 && jetsBSig.size() > 0) countCutflowEvent("SR2_20_btag");	
    else sr2 = false;	    

    if ( sr2 && large_jets12.size() > 0 && large_jets12[0].pt() > 200. && large_jets12[0].m() > 140. )  countCutflowEvent("SR2_21_fatjet>200");
    else sr2 = false;

    if ( sr2 && ( large_jets12.size() == 1 || (large_jets12.size() > 1 && fabs(missingET->P4().DeltaPhi(TLorentzVector( large_jets12[1].four_mom()[0], large_jets12[1].four_mom()[1], large_jets12[1].four_mom()[2], large_jets12[1].four_mom()[3]))) > 1. ) ) ) {
      countCutflowEvent("SR2_22_fatjetmet");
      countSignalEvent("SR2");
    }
    
    
    if (met > 480. ) countCutflowEvent("SR3_15_met");
    else sr3 = false;
   
    if (sr3 && missHT_Sig > 14. ) countCutflowEvent("SR3_16_missHTsig");	
    else sr3 = false;

    if (sr3 && mT > 190. )  countCutflowEvent("SR3_17_mT");	
    else sr3 = false;	

    if (sr3 && amT2 > 175. )  countCutflowEvent("SR3_18_amT2");	
    else sr3 = false;	   
    
    if (sr3 && top_ness > 9.5 )  countCutflowEvent("SR3_19_topness");	
    else sr3 = false;    
    
    if (sr3 && dRbl < 2.8 )  countCutflowEvent("SR3_20_dRbl");	
    else sr3 = false;	
    
    if (sr3 && jetsBSig.size() > 0) countCutflowEvent("SR3_21_btag");	
    else sr3 = false;	
	
    if ( sr3 && large_jets1.size() > 0 && large_jets1[0].pt() > 280. && large_jets1[0].m() > 70.) {
      countCutflowEvent("SR3_22_fatjet>280");
      countSignalEvent("SR3"); 
    }
	
  }  
  
}

void Atlas_1606_03903::finalize() {
  // Whatever should be done after the run goes here
/*  TFile f1("atlas_conf_2016_007.root","recreate");
  histo_met->Write();
  histo_mt->Write();
  histo_top->Write();
  histo_mtop->Write();
  histo_ht->Write();
  histo_st->Write();
  f1.Close();  */
}       

bool Atlas_1606_03903::sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }

template <class X, class Y>
std::vector<X*> Atlas_1606_03903::specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours) {
      // If neighbours are empty, return candidates
      if(neighbours.size() == 0)
        return candidates;
      std::vector<X*> passed_candidates;
      // Loop over candidates
      for(int i = 0; i < candidates.size(); i++) {
        bool overlap = false;
        // If a neighbour is too close, declare overlap, break and don't save candidate
        for(int j = 0; j < neighbours.size(); j++) {
          if (candidates[i]->P4().DeltaR(neighbours[j]->P4()) < std::min(0.4, 0.04 + 10./candidates[i]->PT) ) {
            overlap = true;
            break;
          }
        }
        if (!overlap)
          passed_candidates.push_back(candidates[i]);
      }
      return passed_candidates;
    }
    
template <class X, class Y>
std::vector<X*> Atlas_1606_03903::specialoverlap2(std::vector<X*> candidates, std::vector<Y*> neighbours, double dR, double ratio) {
      // If neighbours are empty, return candidates
      if(neighbours.size() == 0)
        return candidates;
      std::vector<X*> passed_candidates;
      // Loop over candidates
      for(int i = 0; i < candidates.size(); i++) {
        bool overlap = false;
        // If a neighbour is too close, declare overlap, break and don't save candidate
        for(int j = 0; j < neighbours.size(); j++) {
          if (candidates[i]->P4().DeltaR(neighbours[j]->P4()) < dR && neighbours[j]->PT/candidates[i]->PT > ratio ) {
            overlap = true;
            break;
          }
        }
        if (!overlap)
          passed_candidates.push_back(candidates[i]);
      }
      return passed_candidates;
    }    
