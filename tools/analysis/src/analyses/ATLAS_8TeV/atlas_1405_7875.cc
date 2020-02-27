#include "atlas_1405_7875.h"

std::string Atlas_1405_7875::nameSR[15] = {"a.2jl", "b.2jm", "c.2jt","d.2jW", "e.3j", "f.4jl-", "g.4jl", "h.4jm", "i.4jt", "j.4jW", "k.5j", "l.6jl", "m.6jm", "n.6jt", "o.6jt+"};

void Atlas_1405_7875::initialize() {
  setInformation(""
  "@# ATLAS\n"
  "@# ATLAS-1405-7875\n"
  "@# 0 lepton, 2-6 jets, etmiss\n"
  "@# sqrt(s) = 8 TeV\n"
  "@# int(L) = 20.3 fb^-1\n"
  "");
  setLuminosity(0.996*20.3*units::INVFB); //0.996 to conservatively account for jet cleaning       
  setAnalysisName("atlas_1405_7875");          
  ignore("towers");
  ignore("tracks");

  bookSignalRegions("SR05_a.6jl;SR05_b.6jm;SR05_c.6jt;SR05_d.6jt+;\
SR04_5j;\
SR03_a.4jl-;SR03_b.4jl;SR03_c.4jm;SR03_d.4jt;SR03_e.4jW;\
SR02_3j;\
SR01_a.2jl;SR01_b.2jm;SR01_c.2jt;SR01_d.2jW");

  bookCutflowRegions(std::string("j.4jW")+"_"+"CR03_PT3J_for4jW");
  bookCutflowRegions(std::string("j.4jW")+"_"+"CR04_PT4J_for4jW");
  bookCutflowRegions(std::string("d.2jW")+"_"+"CR09_unRW");
  bookCutflowRegions(std::string("j.4jW")+"_"+"CR09_unRW");
  bookCutflowRegions(std::string("j.4jW")+"_"+"CR10_RW");
  for ( int i=0; i < 15; i++) {
    bookCutflowRegions(nameSR[i]+"_"+"CR01_all");
    bookCutflowRegions(nameSR[i]+"_"+"CR02_missETjetsPT");
    bookCutflowRegions(nameSR[i]+"_"+"CR07_dphiMin2J3J");
    bookCutflowRegions(nameSR[i]+"_"+"CR13_meffIncl");
    if ( i > 3 && i != 9 ) bookCutflowRegions(nameSR[i]+"_"+"CR03_PT3J");
    if ( i > 4 ) {
      bookCutflowRegions(nameSR[i]+"_"+"CR08_dphiMinMoreJ");
      if ( i != 9 ) bookCutflowRegions(nameSR[i]+"_"+"CR04_PT4J");
    }
    if ( i > 9 ) bookCutflowRegions(nameSR[i]+"_"+"CR05_PT5J");
    if ( i > 10 ) bookCutflowRegions(nameSR[i]+"_"+"CR06_PT6J");
    if ( i < 3 || ( i > 4 && i < 7 ) ) bookCutflowRegions(nameSR[i]+"_"+"CR11_RHT");
    else bookCutflowRegions(nameSR[i]+"_"+"CR12_Rmeff");
  }
}

void Atlas_1405_7875::analyze() {
  missingET->addMuons(muonsCombined);
  for ( int i=0; i < 15; i++ ) countCutflowEvent(nameSR[i]+"_"+"CR01_all");

//for reconstruction
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);

  std::vector<Electron*> electronsBaseline = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  std::vector<Muon*> muonsBaseline         = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);

  photons = filterPhaseSpace(photons, 130., -2.47, 2.47, true);
  photons = filterIsolation(photons);

  jets = overlapRemoval(jets, electronsBaseline, 0.2);
  electronsBaseline = overlapRemoval(electronsBaseline, jets, 0.4);
  muonsBaseline     = overlapRemoval(muonsBaseline, jets, 0.4);
  jets = overlapRemoval(jets, photons, 0.2);
  
  //for veto where baseline leptons exist
  if ( electronsBaseline.size() + muonsBaseline.size() != 0 ) return;
  //for veto where missingET < 160
  if ( missingET->P4().Et() < 160. ) return;
  //for veto where pt(j1) < 130 or pt(j2) < 60
  if ( jets.size() < 2 || jets[0]->PT < 130. || jets[1]->PT < 60. ) return;
  for ( int i=0; i < 15; i++ ) countCutflowEvent(nameSR[i]+"_"+"CR02_missETjetsPT");

//*************************
//***for event selection***
//*************************
//for PT3J, PT4J, etc. and  meff_NJets
  flag3J = flag4J = flag4JW = flag5J = flag6J = false;
  meff2J = jets[0]->PT + jets[1]->PT + missingET->PT;

  //for that of SR-4JW
  if ( jets.size() > 2 && jets[2]->PT > 40. ) {
    countCutflowEvent(std::string("j.4jW")+"_"+"CR03_PT3J_for4jW");
    if ( jets.size() > 3 && jets[3]->PT > 40. ) {
      flag4JW = true;
      meff4JW  = meff2J + jets[2]->PT + jets[3]->PT;
      countCutflowEvent(std::string("j.4jW")+"_"+"CR04_PT4J_for4jW");
    }
  }
  //for that of SR-3J, SR-4J, SR-5J and SR-6J
  if ( jets.size() > 2 && jets[2]->PT > 60. ) {
    flag3J = true;
    meff3J = meff2J + jets[2]->PT;
    for ( int i=0; i < 15; i++ ) {
      if ( i > 3 &&  i != 9 ) countCutflowEvent(nameSR[i]+"_"+"CR03_PT3J");
    }
    //for that of SR-4J, SR-5J and SR-6J
    if ( jets.size() > 3  && jets[3]->PT > 60. ) {
      flag4J = true;
      meff4J = meff3J + jets[3]->PT;
      for ( int i=0; i < 15; i++ ) {
        if ( i > 4 && i != 9 ) countCutflowEvent(nameSR[i]+"_"+"CR04_PT4J");
      }
      //for that of SR-5J and SR-6J
      if ( jets.size() > 4 && jets[4]->PT > 60. ) {
        flag5J = true;
        meff5J = meff4J + jets[4]->PT;
        for ( int i=0; i < 15; i++ ) {
          if ( i > 9 ) countCutflowEvent(nameSR[i]+"_"+"CR05_PT5J");
        }
        //for that SR-6J only
        if ( jets.size() > 5 && jets[5]->PT > 60. ) {
          flag6J = true;
          meff6J = meff5J + jets[5]->PT;
          for ( int i=0; i < 15; i++ ) {
            if ( i > 10 ) countCutflowEvent(nameSR[i]+"_"+"CR06_PT6J");
          }
        }
      }
    }
  }

//for dphiMin > 0.4 in 2-jet and 3-jet signal regions
  double dphiMin2J3J = 1.E6, dphi;
  for ( int i=0; i < 2; i++ ) {
    dphi = fabs( jets[i]->P4().DeltaPhi( missingET->P4() ) );
    if ( dphi < dphiMin2J3J ) dphiMin2J3J = dphi;
  }
  if ( jets.size() > 2 && jets[2]->PT > 40. ) {
    dphi = fabs( jets[2]->P4().DeltaPhi( missingET->P4() ) );
    if ( dphi < dphiMin2J3J ) dphiMin2J3J = dphi;
  }

//for additional requirement of dphiMin > 0.2 in other signal regions
  double dphiMinMoreJ = 1.E6;
  for ( int i=0; i < jets.size(); i++ ) {
    if ( jets[i]->PT > 40. ) {
      dphi = fabs ( jets[i]->P4().DeltaPhi( missingET->P4() ) );
      if ( dphi < dphiMinMoreJ ) dphiMinMoreJ = dphi;
    }
  }

//for HT and meffIncl
  double HT = 0.;
  for ( int i=0; i < jets.size(); i++ ) {
    if ( jets[i]->PT > 40. ) {
      HT += jets[i]->PT; 
    }
  }
  double meffIncl = HT + missingET->PT ;

//veto where dphiMin2J3J < 0.4
  if ( dphiMin2J3J < 0.4 ) return;
  for ( int i=0; i < 15; i++ ) {
    if ( i < 4 ) countCutflowEvent(nameSR[i]+"_"+"CR07_dphiMin2J3J");
  }
  if ( flag3J )  countCutflowEvent(std::string("e.3j")+"_"+"CR07_dphiMin2J3J");
  if ( flag4JW ) countCutflowEvent(std::string("j.4jW")+"_"+"CR07_dphiMin2J3J");
  if ( flag5J )  countCutflowEvent(std::string("k.5j")+"_"+"CR07_dphiMin2J3J");
  if ( flag4J ) {
    for ( int i = 0; i < 15; i++ ) {
      if ( i > 4 && i < 9 ) countCutflowEvent(nameSR[i]+"_"+"CR07_dphiMin2J3J");
    }
  }
  if ( flag6J ) {
    for ( int i = 0; i < 15; i++ ) {
      if ( i > 10 ) countCutflowEvent(nameSR[i]+"_"+"CR07_dphiMin2J3J");
    }
  }

//for signal region 2jl
  if ( missingET->P4().Et() / sqrt(HT) > 8. ) {
    countCutflowEvent(std::string("a.2jl")+"_"+"CR11_RHT");
    if ( meffIncl > 800. ) {
      countCutflowEvent(std::string("a.2jl")+"_"+"CR13_meffIncl");
      countSignalEvent("SR01_a.2jl");
    }
  }

//for signal region 2jm and 2jt
  if ( missingET->P4().Et() / sqrt(HT) > 15. ) {
    countCutflowEvent(std::string("b.2jm")+"_"+"CR11_RHT");
    countCutflowEvent(std::string("c.2jt")+"_"+"CR11_RHT");
    if ( meffIncl > 1200. ) {
      countCutflowEvent(std::string("b.2jm")+"_"+"CR13_meffIncl");
      countSignalEvent("SR01_b.2jm");
    }
    if ( meffIncl > 1600. ) {
      countCutflowEvent(std::string("c.2jt")+"_"+"CR13_meffIncl");
      countSignalEvent("SR01_c.2jt");
    }
  }

//for signal region 2jW in 
//(1)high-mass condition (used), i.e. two leading-mass jets' mass in (60, 100)
//(2)conservative condition, i.e. jets' mass in (60, 100) as long as having
  int unRW1Label, unRW2Label;
  //int unRWNum;
  double unRW1, unRW2;
  double unRW, unRWMax;

  unRWMax = 0.;
  //unRWNum = 0;
  for ( int i=0; i < jets.size(); i++ ) {
    unRW = jets[i]->P4().M();
    if ( unRW > unRWMax ) {
      unRWMax       = unRW;
      unRW1Label = i;
    }
    //if ( unRW > 60. && unRW < 100. ) unRWNum++;
  }
  unRW1   = unRWMax;

  unRWMax = 0.;
  for ( int i=0; i < jets.size(); i++ ) {
    if ( i != unRW1Label ) {
      unRW = jets[i]->P4().M();
      if ( unRW > unRWMax ) { 
        unRWMax       = unRW;
        unRW2Label = i;
      }
    }
  }
  unRW2 = unRWMax;
 
  //for checking if the two leading-PT jets are also the two leading-mass jets

  if ( unRW1 > 60. && unRW1 < 100. && unRW2 > 60. && unRW2 < 100. ) {
  //if ( unRWNum > 1 ) {
    countCutflowEvent(std::string("d.2jW")+"_"+"CR09_unRW");
    if ( missingET->P4().Et() / meff2J > 0.25 ) {
      countCutflowEvent(std::string("d.2jW")+"_"+"CR12_Rmeff");
      if ( meffIncl > 1800. ) {
        countCutflowEvent(std::string("d.2jW")+"_"+"CR13_meffIncl");
        countSignalEvent("SR01_d.2jW");
      }
    }
  }

//for signal region 3j
  if ( flag3J && missingET->P4().Et() / meff3J > 0.3 ) {
    countCutflowEvent(std::string("e.3j")+"_"+"CR12_Rmeff");
    if ( meffIncl > 2200. ) {
      countCutflowEvent(std::string("e.3j")+"_"+"CR13_meffIncl");
      countSignalEvent("SR02_3j");
    }
  }

//veto where dphiMinMoreJ < 0.2
  if ( dphiMinMoreJ < 0.2 ) return;
  if ( flag4JW ) countCutflowEvent(std::string("j.4jW")+"_"+"CR08_dphiMinMoreJ");
  if ( flag5J )  countCutflowEvent(std::string("k.5j")+"_"+"CR08_dphiMinMoreJ");
  if ( flag4J ) {
    for ( int i = 0; i < 15; i++ ) {
      if ( i > 4 && i < 9 ) countCutflowEvent(nameSR[i]+"_"+"CR08_dphiMinMoreJ");
    }
  }
  if ( flag6J ) {
    for ( int i = 0; i < 15; i++ ) {
      if ( i > 10 ) countCutflowEvent(nameSR[i]+"_"+"CR08_dphiMinMoreJ");
    }
  }

//for signal region 4jl- and 4jl
  if ( flag4J && missingET->P4().Et() / sqrt(HT) > 10. ) {
    countCutflowEvent(std::string("f.4jl-")+"_"+"CR11_RHT");
    countCutflowEvent(std::string("g.4jl")+"_"+"CR11_RHT");
    if ( meffIncl > 700. ) {
      countCutflowEvent(std::string("f.4jl-")+"_"+"CR13_meffIncl");
      countSignalEvent("SR03_a.4jl-");
    }
    if ( meffIncl > 1000. ) {
      countCutflowEvent(std::string("g.4jl")+"_"+"CR13_meffIncl");
      countSignalEvent("SR03_b.4jl");
    }
  }

//for signal region 4jm
  if ( flag4J && missingET->P4().Et() / meff4J > 0.4 ) {
    countCutflowEvent(std::string("h.4jm")+"_"+"CR12_Rmeff");
    if ( meffIncl > 1300. ) {
      countCutflowEvent(std::string("h.4jm")+"_"+"CR13_meffIncl");
      countSignalEvent("SR03_c.4jm");
    }
  }

//for signal region 4jt
  if ( flag4J && missingET->P4().Et() / meff4J > 0.25 ) {
    countCutflowEvent(std::string("i.4jt")+"_"+"CR12_Rmeff");
    if ( meffIncl > 2200. ) {
      countCutflowEvent(std::string("i.4jt")+"_"+"CR13_meffIncl");
      countSignalEvent("SR03_d.4jt");
    }
  }

//for signal region 4jW
//(1)high-mass condition (used), i.e. leading-mass jet's mass in (60, 100) and deltaR based on the rest of jets
//(2)conservative condition, i.e. a jet's mass in (60, 100) and deltaR based on the rest of jets
  //int RWNum;
  double dr, drMin;
  double RW;

  if ( flag4JW ) {

    /*unRWNum = 0;
    RWNum   = 0;
    drMin   = 1.E6;
    for ( int i=0; i < jets.size(); i++ ) {
      unRW = jets[i]->P4().M();
      if ( unRW > 60. && unRW < 100. ) {
        unRWNum++;
        unRW1Label = i;
        for ( int j = 0; j < jets.size(); j++ ) {
          if ( j != unRW1Label ) {
            for ( int k = jets.size()-1; k > j ; k--) {
              if ( k != unRW1Label ) {
                dr = jets[j]->P4().DeltaR( jets[k]->P4() );
                if ( dr < drMin ) {
                  drMin = dr;
                  RW = ( jets[j]->P4() + jets[k]->P4() ).M();
                }
              }
            }
          }
        }
        if ( RW > 60. && RW < 100.) RWNum++;
      }
    }*/


    //for unresolved W
    unRWMax = 0.;
    for ( int i=0; i < jets.size(); i++ ) {
      unRW=jets[i]->P4().M();
      if ( unRW > unRWMax ) {
        unRWMax    = unRW;
        unRW1Label = i;
      }
    }
    unRW1 = unRWMax;

    //for resolved W
    drMin = 1.E6; 
    for ( int i=0; i < jets.size(); i++ ) {
      if ( i != unRW1Label ) {
        for ( int j=jets.size()-1; j > i; j-- ) {
          if ( j != unRW1Label ) {
            dr = jets[i]->P4().DeltaR( jets[j]->P4() );
            if ( dr < drMin ) {
              drMin = dr;
              RW    = ( jets[i]->P4() + jets[j]->P4() ).M();
            }
          }
        }
      }
    }

    /*if ( unRWNum > 0 ) {
      countCutflowEvent(std::string("j.4jW")+"_"+"CR09_unRW");
      if ( RWNum > 0 ) {
        countCutflowEvent(std::string("j.4jW")+"_"+"CR10_RW");*/
    if ( unRW1 > 60. && unRW1 < 100. ) {
      countCutflowEvent(std::string("j.4jW")+"_"+"CR09_unRW");
      if ( RW > 60. && RW < 100. ) {
        countCutflowEvent(std::string("j.4jW")+"_"+"CR10_RW");
        if ( missingET->P4().Et() / meff4J > 0.35 ) {
          countCutflowEvent(std::string("j.4jW")+"_"+"CR12_Rmeff");
          if ( meffIncl > 1100. ) {
            countCutflowEvent(std::string("j.4jW")+"_"+"CR13_meffIncl");
            countSignalEvent("SR03_e.4jW");
          }
        } 
      }
    }

  }

//for signal region 5j
  if ( flag5J && missingET->P4().Et() / meff5J > 0.2 ) {
    countCutflowEvent(std::string("k.5j")+"_"+"CR12_Rmeff");
    if ( meffIncl > 1200. ) {
      countCutflowEvent(std::string("k.5j")+"_"+"CR13_meffIncl");
      countSignalEvent("SR04_5j");
    }
  }

//for signal region 6jl, 6jm
  if ( flag6J && missingET->P4().Et() / meff6J > 0.2 ) {
    countCutflowEvent(std::string("l.6jl")+"_"+"CR12_Rmeff");
    countCutflowEvent(std::string("m.6jm")+"_"+"CR12_Rmeff");
    if ( meffIncl > 900. ) {
      countCutflowEvent(std::string("l.6jl")+"_"+"CR13_meffIncl");
      countSignalEvent("SR05_a.6jl");
    }
    if ( meffIncl > 1200. ) {
      countCutflowEvent(std::string("m.6jm")+"_"+"CR13_meffIncl");
      countSignalEvent("SR05_b.6jm");
    }
  }

//for signal region 6jt
  if ( flag6J && missingET->P4().Et() / meff6J > 0.25 ) {
    countCutflowEvent(std::string("n.6jt")+"_"+"CR12_Rmeff");
    if ( meffIncl > 1500. ) {
      countCutflowEvent(std::string("n.6jt")+"_"+"CR13_meffIncl");
      countSignalEvent("SR05_c.6jt");
    }
  }

//for signal region 6jt+
  if ( flag6J && missingET->P4().Et() / meff6J > 0.15 ) {
    countCutflowEvent(std::string("o.6jt+")+"_"+"CR12_Rmeff");
    if ( meffIncl > 1700. ) {
      countCutflowEvent(std::string("o.6jt+")+"_"+"CR13_meffIncl");
      countSignalEvent("SR05_d.6jt+");
    }
  }

}

void Atlas_1405_7875::finalize() {

}       
