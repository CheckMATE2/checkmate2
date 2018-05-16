#include "cms_1405_7570.h"

void Cms_1405_7570::initialize() {
  setAnalysisName("cms_1405_7570");          
  setInformation(""
    "@#search for chargino neutralino production\n"
     "@#only tri-lepton search included\n"
  "");
  setLuminosity(19.5*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("0_50_0;0_100_0;0_150_0;0_200_0;0_50_75;0_100_75;0_150_75;0_200_75;0_50_105;0_100_105;0_150_105;0_200_105;120_50_0;120_100_0;120_150_0;120_200_0;120_50_75;120_100_75;120_150_75;120_200_75;120_50_105;120_100_105;120_150_105;120_200_105;160_50_0;160_100_0;160_150_0;160_200_0;160_50_75;160_100_75;160_150_75;160_200_75;160_50_105;160_100_105;160_150_105;160_200_105;Zjj_000;Zjj_030;Zjj_060;Zjj_080;Zjj_100;Zjj_120;Zjj_150;Zjj_200;");
  bookSignalRegions("SS_200;SS_200_l3v;SS_120;SS_120_l3v;");
  bookSignalRegions("Wbb_100;Wbb_125;Wbb_150;Wbb_175;");
  bookSignalRegions("noOSSF_000;noOSSF_050;noOSSF_100;");
  bookSignalRegions("MCT120_SF;MCT120_OF;");
  bookCutflowRegions("0_0_0;0_0_75;0_0_105;120_0_0;120_0_75;120_0_105;160_0_0;160_0_75;160_0_105;eemu;emumu;eee;mumumu;");
  bookCutflowRegions("ee_00;ee_01;ee_02;ee_03;ee_04;ee_05;ee_06;ee_07;mumu_00;mumu_01;mumu_02;mumu_03;mumu_04;mumu_05;mumu_06;mumu_07;");
  bookCutflowRegions("SS_00;SS_01;SS_02;SS_03;SS_04;");
  bookCutflowRegions("WH_00;WH_01;WH_02;WH_03;WH_04;WH_05;WH_06;WH_07;WH_08;WH_09;WH_10;");
  bookCutflowRegions("WHSS_01;WHSS_02;WHSS_03;WHSS_04;WHSS_05;");
  bookCutflowRegions("noOSSF_00;noOSSF_01;noOSSF_02;noOSSF_03;");
  // You should initialize any declared variables here
}

void Cms_1405_7570::analyze() {
  // Your eventwise analysis code goes here
  missingET->addMuons(muonsCombinedPlus);  
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.4, 2.4);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);  
  jets = filterPhaseSpace(jets, 30., -2.5, 2.5);
  std::vector<Jet*> jets2 = filterPhaseSpace(jets, 30., -4.7, 4.7);
  
  jets = overlapRemoval(jets, electronsMedium, 0.4);
  jets = overlapRemoval(jets, muonsCombined, 0.4);
  
  std::vector<Electron*> isoElecs = filterIsolation(electronsMedium);
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);

// tri-lepton  (validated with cut-flows)
  
  while (isoElecs.size() + isoMuons.size() == 3) {
    
    double mOSSF = 0.; double mT = 0.; double mOSSFZ = 10000.; int i_min = 10; int j_min = 10;
    if (isoElecs.size() == 2) {
      if ( isoElecs[0]->Charge * isoElecs[1]->Charge > 0 ) break;
      if ( isoElecs[0]->PT < 20. && isoMuons[0]->PT < 20. ) break;
      mOSSF = (isoElecs[0]->P4() + isoElecs[1]->P4() ).M() ;
      if (mOSSF < 12.) break;
      if ( isoElecs[0]->Charge * isoMuons[0]->Charge < 0 && (isoElecs[0]->P4() + isoMuons[0]->P4() ).M() < 12. ) break;
      if ( isoElecs[1]->Charge * isoMuons[0]->Charge < 0 && (isoElecs[1]->P4() + isoMuons[0]->P4() ).M() < 12. ) break;
      mT = sqrt( 2.*missingET->P4().Et()*isoMuons[0]->PT*(1.-cos( missingET->P4().DeltaPhi(isoMuons[0]->P4()) ) ));
      countCutflowEvent("eemu");
    }  
    
    else if (isoMuons.size() == 2) {
      if ( isoMuons[0]->Charge * isoMuons[1]->Charge > 0 ) break;
      if ( isoMuons[0]->PT < 20. && isoElecs[0]->PT < 20. ) break;
      mOSSF = (isoMuons[0]->P4() + isoMuons[1]->P4() ).M() ;
      if (mOSSF < 12.) break;
      if ( isoMuons[0]->Charge * isoElecs[0]->Charge < 0 && (isoMuons[0]->P4() + isoElecs[0]->P4() ).M() < 12. ) break;
      if ( isoMuons[1]->Charge * isoElecs[0]->Charge < 0 && (isoMuons[1]->P4() + isoElecs[0]->P4() ).M() < 12. ) break;
      mT = sqrt( 2.*missingET->P4().Et()*isoElecs[0]->PT*(1.-cos( missingET->P4().DeltaPhi(isoElecs[0]->P4()) ) ));
      countCutflowEvent("emumu");
    }  
    
    else if ( isoElecs.size() == 3 ) {
      if (isoElecs[0]->PT < 20. ) break;
      for (int  i = 0; i < 2 ; i++)
    for (int j = i + 1; j < 3; j++) 
      if ( isoElecs[i]->Charge * isoElecs[j]->Charge < 0 ) {
        double mOSSF_temp = (isoElecs[i]->P4() + isoElecs[j]->P4() ).M();
        if ( mOSSF_temp < 12. ) return;
        if ( fabs(mOSSF_temp - 91.2) < mOSSFZ) {
          mOSSFZ = fabs(mOSSF_temp - 91.2);
          i_min = i;
          j_min = j;
        }
      }
      if ( i_min == 10) {return;} else mOSSF = (isoElecs[i_min]->P4() + isoElecs[j_min]->P4() ).M();
      for (int  i = 0; i < 3 ; i++)
    if ( i != i_min && i != j_min ) mT = sqrt( 2.*missingET->P4().Et()*isoElecs[i]->PT*(1.-cos( missingET->P4().DeltaPhi(isoElecs[i]->P4()) ) ));     
      countCutflowEvent("eee");
    }
  
    else if ( isoMuons.size() == 3 ) {
      if (isoMuons[0]->PT < 20. ) break;
      for (int  i = 0; i < 2 ; i++)
    for (int j = i + 1; j < 3; j++) 
      if ( isoMuons[i]->Charge * isoMuons[j]->Charge < 0 ) {
        double mOSSF_temp = (isoMuons[i]->P4() + isoMuons[j]->P4() ).M();
        if ( mOSSF_temp < 12. ) return;
        if ( fabs(mOSSF_temp - 91.2) < mOSSFZ) {
          mOSSFZ = fabs(mOSSF_temp - 91.2);
          i_min = i;
          j_min = j;
        }
      }
      if ( i_min == 10) {return;} else mOSSF = (isoMuons[i_min]->P4() + isoMuons[j_min]->P4() ).M();
      for (int  i = 0; i < 3 ; i++)
    if ( i != i_min && i != j_min ) mT = sqrt( 2.*missingET->P4().Et()*isoMuons[i]->PT*(1.-cos( missingET->P4().DeltaPhi(isoMuons[i]->P4()) ) ));      
      countCutflowEvent("mumumu");
    }
  
    else break;
    
    bool btag = false;
    for (int i = 0; i < jets.size(); i++) 
      if ( checkBTag(jets[i]) )   btag = true;
    if (btag) break;  
                          
    double MET = missingET->P4().Perp();
    std::string met_label; std::string mll_label; std::string mt_label;
    if ( 50. < MET && MET < 100. )   met_label = "50";
    if ( 100. < MET && MET < 150. )   met_label = "100";
    if ( 150. < MET && MET < 200. )   met_label = "150";
    if ( 200. < MET  )   met_label = "200";
                          
    if ( mOSSF < 75. ) mll_label = "0";
    if ( 75. < mOSSF && mOSSF < 105. ) mll_label = "75";
    if ( mOSSF > 105. ) mll_label = "105";
                          
    if ( mT < 120. ) mt_label = "0";
    if ( 120. < mT && mT < 160. ) mt_label = "120";
    if ( mT > 160. ) mt_label = "160";
                          
    if (MET < 50.) countCutflowEvent(mt_label+"_0_"+mll_label); else countSignalEvent(mt_label+"_"+met_label+"_"+mll_label);
    
    break;
  
  }

// Z + jj  (validated )
  while (isoElecs.size() >= 2 && isoElecs[1]->PT > 20. && isoElecs[0]->Charge * isoElecs[1]->Charge < 0 ) {
    
    countCutflowEvent("ee_00");
    if ( fabs( (isoElecs[0]->P4() + isoElecs[1]->P4()).M() - 91.) > 10. ) break;
    countCutflowEvent("ee_01");
    if ( isoElecs.size() > 2 || isoMuons.size() > 0 ) break;
    countCutflowEvent("ee_02");
    if (jets.size() < 2 ) break;
    countCutflowEvent("ee_03");
    
    bool btag = false;
    for (int i = 0; i < jets.size(); i++) 
      if ( checkBTag(jets[i]) )   btag = true;
    if (btag) break;  
    countCutflowEvent("ee_04");  
    
    if ( fabs( (jets[0]->P4() + jets[1]->P4()).M() - 90. ) > 20. ) break;    
    countCutflowEvent("ee_05");  
    
    double MET = missingET->P4().Perp();
    
    if (MET > 100. ) countCutflowEvent("ee_06"); 
    if (MET > 200. ) countCutflowEvent("ee_07"); 
    
    if (MET < 30.) countSignalEvent("Zjj_000");
    else if (MET < 60.) countSignalEvent("Zjj_030");
    else if (MET < 80.) countSignalEvent("Zjj_060");
    else if (MET < 100.) countSignalEvent("Zjj_080");
    else if (MET < 120.) countSignalEvent("Zjj_100");
    else if (MET < 150.) countSignalEvent("Zjj_120");
    else if (MET < 200.) countSignalEvent("Zjj_150");
    else countSignalEvent("Zjj_200");
    
    break;
  }

// Z+jj   (validated )
  while (isoMuons.size() >= 2 && isoMuons[1]->PT > 20. && isoMuons[0]->Charge * isoMuons[1]->Charge < 0 ) {
    
    countCutflowEvent("mumu_00");
    if ( fabs( (isoMuons[0]->P4() + isoMuons[1]->P4()).M() - 91.) > 10. ) break;
    countCutflowEvent("mumu_01");
    if ( isoMuons.size() > 2 || isoElecs.size() > 0 ) break;
    countCutflowEvent("mumu_02");
    if (jets.size() < 2 ) break;
    countCutflowEvent("mumu_03");
    
    bool btag = false;
    for (int i = 0; i < jets.size(); i++) 
      if ( checkBTag(jets[i]) )   btag = true;
    if (btag) break;  
    countCutflowEvent("mumu_04");  
    
    if ( fabs( (jets[0]->P4() + jets[1]->P4()).M() - 90. ) > 20. ) break;    
    countCutflowEvent("mumu_05");  
    
    double MET = missingET->P4().Perp();
    
    if (MET > 100. ) countCutflowEvent("mumu_06"); 
    if (MET > 200. ) countCutflowEvent("mumu_07"); 
    
    if (MET < 30.) countSignalEvent("Zjj_000");
    else if (MET < 60.) countSignalEvent("Zjj_030");
    else if (MET < 80.) countSignalEvent("Zjj_060");
    else if (MET < 100.) countSignalEvent("Zjj_080");
    else if (MET < 120.) countSignalEvent("Zjj_100");
    else if (MET < 150.) countSignalEvent("Zjj_120");
    else if (MET < 200.) countSignalEvent("Zjj_150");
    else countSignalEvent("Zjj_200");
    
    break;
  }  
  
// SS leptons cha1neu2 via sleptons sec. 5 (likely buggy)

  while ( isoElecs.size() + isoMuons.size() >= 2 ) {
    
    std::vector<TLorentzVector> leptonpt;
    std::vector<int> leptoncharge;
    
    for (int i = 0; i < isoElecs.size(); i++) {
      leptonpt.push_back(isoElecs[i]->P4());
      leptoncharge.push_back(11*isoElecs[i]->Charge);
    }
    for (int i = 0; i < isoMuons.size(); i++) {
      leptonpt.push_back(isoMuons[i]->P4());
      leptoncharge.push_back(13*isoMuons[i]->Charge);
    }
    
    int SSpairs = 0;
    int iSS,jSS;
    
    for (int i = 0; i < leptonpt.size()-1; i++) 
      for (int j = i+1; j < leptonpt.size(); j++) {
    if ( leptonpt[i].Perp() > 20. && leptonpt[j].Perp() > 20. && leptoncharge[i]*leptoncharge[j] > 0 ) {
      SSpairs++;
      iSS = i;
      jSS = j;
    }
      }
    if (SSpairs != 1)  break;
     
    for (int i = 0; i < leptonpt.size(); i++) 
      if ( i != iSS && i != jSS && ( (leptoncharge[i]+leptoncharge[iSS] == 0 && fabs((leptonpt[i]+leptonpt[iSS]).M()-91.2) < 15. ) || ( (leptoncharge[i]+leptoncharge[jSS] == 0 && fabs((leptonpt[i]+leptonpt[jSS]).M()-91.2) < 15. ) ) ) ) break;
      
    countCutflowEvent("SS_00");
    
    bool lep3 = false;
    if (leptonpt.size() > 2) lep3 = true;    
    if (!lep3) countCutflowEvent("SS_01");
    
    bool sig_jets = false;
    if (jets.size() > 0 && jets[0]->PT > 40. ) sig_jets = true;
     
    double MET = missingET->P4().Perp();
    if ( MET > 120. && !lep3 ) countCutflowEvent("SS_02");
     
    if ( MET > 200. ) {
      if (!lep3 ) countCutflowEvent("SS_03");
      countSignalEvent("SS_200");
      if (!lep3 ) countSignalEvent("SS_200_l3v");
    }
     
    if ( MET > 120. && MET < 200. && !sig_jets) {
      if (!lep3 ) countCutflowEvent("SS_04");
      countSignalEvent("SS_120");
      if (!lep3 ) countSignalEvent("SS_120_l3v");
    }
      
    break;
  }

  
// WH 1lepton+2 b-jets   (cut-flow looks impossible)
  while ( (isoElecs.size() == 1 && isoMuons.size() == 0 && isoElecs[0]->PT > 30. && fabs(isoElecs[0]->Eta) < 1.4442 ) || (isoMuons.size() == 1 && isoElecs.size() == 0 && isoMuons[0]->PT > 25. && isoMuons[0]->Eta < 2.1 )  )  {
    
    countCutflowEvent("WH_00"); 
    
    int n_jets = 0;
    for (int i = 0; i < jets.size(); i++)
      if ( fabs(jets[i]->Eta) < 2.4 ) n_jets++;
      
    if ( n_jets >= 2 ) countCutflowEvent("WH_01"); 
    else break;
    
    bool forwj = false;
    for (int i = 0; i < jets2.size(); i++)
      if ( fabs(jets2[i]->Eta) > 2.4 ) forwj = true;
    if (forwj) break;  
    if (n_jets == 2) countCutflowEvent("WH_02");
      else break;
    
    if ( checkBTag(jets[0]) && checkBTag(jets[1]) ) countCutflowEvent("WH_03");    
      else break;
      
    if ( 100. < (jets[0]->P4() +  jets[1]->P4()).M()   && (jets[0]->P4() +  jets[1]->P4()).M() < 150. )  countCutflowEvent("WH_04");    
      else break;
      
    TLorentzVector lepton;  
    if (  isoElecs.size() == 1 ) lepton = isoElecs[0]->P4();
      else lepton = isoMuons[0]->P4();
    double mT = sqrt( 2.*missingET->P4().Et()*lepton.Perp()*(1.-cos( missingET->P4().DeltaPhi(lepton) ) ));  
    
    if ( mT < 100.) break;
    countCutflowEvent("WH_05");  
    
    double mt2bl = mT2_bl(lepton, jets[0]->P4(), jets[1]->P4(),  missingET->P4());
    if (mt2bl < 200. ) break;
    countCutflowEvent("WH_06");
    
    double MET = missingET->P4().Perp();
    if ( MET > 100.) {countCutflowEvent("WH_07"); countSignalEvent("Wbb_100");}
    if ( MET > 125.) {countCutflowEvent("WH_08"); countSignalEvent("Wbb_125");}
    if ( MET > 150.) {countCutflowEvent("WH_09"); countSignalEvent("Wbb_150");}
    if ( MET > 175.) {countCutflowEvent("WH_10"); countSignalEvent("Wbb_175");}
    
    break;
  }  
  
// WH in 2SS + jets  (M_T2^J variable unclear)
  
  while ( isoElecs.size() + isoMuons.size() == 2)  {
    
    std::vector<TLorentzVector> leptons;
    if (isoElecs.size() == 2 && isoElecs[0]->Charge *  isoElecs[1]->Charge > 0 &&  isoElecs[1]->PT > 20.) {
      leptons.push_back(isoElecs[0]->P4() );
      leptons.push_back(isoElecs[1]->P4() );
    }
    else if  (isoMuons.size() == 2 && isoMuons[0]->Charge *  isoMuons[1]->Charge > 0 &&  isoMuons[1]->PT > 20.) {
      leptons.push_back(isoMuons[0]->P4() );
      leptons.push_back(isoMuons[1]->P4() );
    }
    else if  (isoMuons.size() == 1 && isoElecs.size() == 1 && isoMuons[0]->Charge *  isoElecs[0]->Charge > 0 &&  isoMuons[0]->PT > 20. && isoElecs[0]->PT > 20.) {
      if ( isoMuons[0]->PT > isoElecs[0]->PT ) {
    leptons.push_back(isoMuons[0]->P4() );
    leptons.push_back(isoElecs[0]->P4() );
      }
      else {
        leptons.push_back(isoElecs[0]->P4() );
    leptons.push_back(isoMuons[0]->P4() );
      }
    }
    else break;
    
    countCutflowEvent("WHSS_01");
    
    if (jets.size() == 2 || jets.size() == 3 )  countCutflowEvent("WHSS_02");
    else break;
    
    bool btag = false;
    for (int i = 0; i < jets.size(); i++ )
      if ( checkBTag(jets[i]) ) btag = true;
    if (btag) break;  
      
    countCutflowEvent("WHSS_03");
    
    if (   missingET->P4().Perp() > 40. )  countCutflowEvent("WHSS_04");
    else break;
    
    if ( fabs(leptons[0].Eta() - leptons[1].Eta() ) < 1.6 ) countCutflowEvent("WHSS_05");
    else break;
    
    break;
  }
  
// WH 3-lepton no OSSF pair, section 7.3; with OSSF very similar to other trilepton, table 7 roughly in agreement
  while ( isoElecs.size() + isoMuons.size() == 3)  {
     countCutflowEvent("noOSSF_00");
    
    if ( isoElecs.size() == 2 && (isoElecs[0]->Charge) * (isoElecs[1]->Charge) < 0 ) { break;}
    if ( isoMuons.size() == 2 && (isoMuons[0]->Charge) * (isoMuons[1]->Charge) < 0 ) { break;}
    if ( isoMuons.size() == 3 && abs(isoMuons[0]->Charge + isoMuons[1]->Charge + isoMuons[2]->Charge) < 2 ) {break;}
    if ( isoElecs.size() == 3 && abs(isoElecs[0]->Charge + isoElecs[1]->Charge + isoElecs[2]->Charge) < 2 ) {break;}
    
    countCutflowEvent("noOSSF_01");
    
    double HT = 0;
    for (int i = 0; i < jets.size(); i++ ) HT += jets[i]->PT;
    
    if (HT > 200. ) break;
    
    countCutflowEvent("noOSSF_02");
    
    bool btag = false;
    for (int i = 0; i < jets.size(); i++) 
      if ( checkBTag(jets[i]) )  btag = true;
    if (btag) break;  
      
    countCutflowEvent("noOSSF_03");  
    double MET = missingET->P4().Perp();
    if (MET < 50. ) countSignalEvent("noOSSF_000");
    else if ( MET < 100. ) countSignalEvent("noOSSF_050");
    else if ( MET < 150. ) countSignalEvent("noOSSF_100");
    
    break;
  }

// non-resonant di-lepton, Sec. 8 (numbers in signal regions to Fig. 10, cut-flows not available)

  while (isoElecs.size() + isoMuons.size() == 2 ) {
    
    if ( isoElecs.size() == 2 && ( isoElecs[1]->PT < 20. || isoElecs[0]->Charge * isoElecs[1]->Charge > 0 || fabs( (isoElecs[0]->P4() + isoElecs[1]->P4()).M() - 91.2) < 15. ) ) break;
    if ( isoMuons.size() == 2 && ( isoMuons[1]->PT < 20. || isoMuons[0]->Charge * isoMuons[1]->Charge > 0 || fabs( (isoMuons[0]->P4() + isoMuons[1]->P4()).M() - 91.2) < 15. ) ) break;
    if ( isoElecs.size() == 1 && ( isoElecs[0]->PT < 20. || isoMuons[0]->PT < 20. || isoElecs[0]->Charge * isoMuons[0]->Charge > 0 ) ) break;
    
    bool btag = false; TLorentzVector vds(0.,0.,0.,0.);
    for (int i = 0; i < jets.size(); i++) {
      if ( checkBTag(jets[i]) )  btag = true;
      vds += jets[i]->P4();
    }
    if (btag) break;
    
    double MET = missingET->P4().Perp();
    if ( MET < 60. ) break;
    
    if (isoElecs.size() == 2 && mCTperp( isoElecs[0]->P4(), isoElecs[1]->P4(), vds, missingET->P4() ) > 120. ) countSignalEvent("MCT120_SF"); 
    if (isoMuons.size() == 2 && mCTperp( isoMuons[0]->P4(), isoMuons[1]->P4(), vds, missingET->P4() ) > 120. ) countSignalEvent("MCT120_SF"); 
    if (isoElecs.size() == 1 && mCTperp( isoElecs[0]->P4(), isoMuons[0]->P4(), vds, missingET->P4() ) > 120. ) countSignalEvent("MCT120_OF"); 
  
    break;
  }
  
  
  
}

void Cms_1405_7570::finalize() {
  // Whatever should be done after the run goes here
}       
