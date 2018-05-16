#include "atlas_1308_1841.h"
// AUTHOR: kitpc
// EMAIL: kitpc

std::string Atlas_1308_1841::namesCutflow[10]=
{"CR05_flavour_8j50","CR06_flavour_9j50",
"CR07_flavour_GE10j50",
"CR08_flavour_7j80","CR09_flavour_GE8j80",
"CR10_m_GE8j50","CR11_m_GE9j50","CR12_m_GE10j50","",""};

void Atlas_1308_1841::initialize() {
  setAnalysisName("atlas_1308_1841");          
  setInformation(""
"# ATLAS\n"
"# atlas_1308_1841\n"
"# 0 lepton + large jets multi + missingET\n"
"# sqrt(s) = 8 TeV\n"
"# int(L) = 20.3 fb^-1\n"
  "");

  setLuminosity(20.3*0.998*units::INVFB); //0.998 for primary vertex and event cleaning

  ignore("towers");
  ignore("tracks");

  bookSignalRegions("SR01_8j50_a.0b;SR01_8j50_b.1b;SR01_8j50_c.GE2b;\
SR02_9j50_a.0b;SR02_9j50_b.1b;SR02_9j50_c.GE2b;\
SR03_GE10j50;\
SR04_7j80_a.0b;SR04_7j80_b.1b;SR04_7j80_c.GE2b;\
SR05_GE8j80_a.0b;SR05_GE8j80_b.1b;SR05_GE8j80_c.GE2b;\
SR06_GE8j50_340;SR06_GE8j50_420;\
SR07_GE9j50_340;SR07_GE9j50_420;\
SR08_GE10j50_340;SR08_GE10j50_420;");

  bookCutflowRegions("CR00_all;CR01_Trigger;CR02_cleaning;CR03_LeptonVeto;CR04_furtherCleaning;");
  for ( int i=0; i < 5; i++ ) {
    bookCutflowRegions(namesCutflow[i]+"_"+"a.base");
    bookCutflowRegions(namesCutflow[i]+"_"+"b.R");
    if ( i != 2 ) {
      bookCutflowRegions(namesCutflow[i]+"_"+"c.0bjet");
      bookCutflowRegions(namesCutflow[i]+"_"+"d.1bjet");
      bookCutflowRegions(namesCutflow[i]+"_"+"e.GE2bjet");
    }
  }
  for ( int i=5; i < 8; i++ ) {
    bookCutflowRegions(namesCutflow[i]+"_"+"a.base");
    bookCutflowRegions(namesCutflow[i]+"_"+"b.R");
    bookCutflowRegions(namesCutflow[i]+"_"+"c.M340");
    bookCutflowRegions(namesCutflow[i]+"_"+"d.M420");
  }

  //fout1.open("log");
}

void Atlas_1308_1841::analyze() {
  missingET->addMuons(muonsCombined);
  countCutflowEvent("CR00_all");

//for trigger
  double random_trigger = (double) rand() / (double) (RAND_MAX + 1.);
  //for jets trigger
  std::vector<Jet*> jetsForTrigger = filterPhaseSpace(jets, 20., -3.2, 3.2);
  int flagjetsTrigger45 = 0, flagjetsTrigger55 = 0;

  for ( int i=0; i < jetsForTrigger.size(); i++ ) {
    if ( jetsForTrigger[i]->P4().Et() > 45. && random_trigger < 1. ) flagjetsTrigger45++; 
    if ( jetsForTrigger[i]->P4().Et() > 55. && random_trigger < 1. ) flagjetsTrigger55++; 
  }

  if ( flagjetsTrigger45 < 6 && flagjetsTrigger55 < 5 ) return;
  countCutflowEvent("CR01_Trigger");

  //countCutflowEvent("CR02_cleaning");

//for reconstruction
  std::vector<Jet*> jetsForBTag = filterPhaseSpace(jets, 40., -2.5, 2.5);

  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  muonsCombined   = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);

  jets = overlapRemoval(jets, electronsMedium, 0.2);
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  muonsCombined   = overlapRemoval(muonsCombined, jets, 0.4);

//***************************
//*****for event selection***
//**************************
  //electronsMedium = filterIsolation(electronsMedium);
  //muonsCombined   = filterIsolation(muonsCombined);

  //for veto where lepton exists
  if ( electronsMedium.size() + muonsCombined.size() != 0 ) return;
  countCutflowEvent("CR03_LeptonVeto");

  //countCutflowEvent("CR04_furtherCleaning");
  
//for multi-jet + flavour stream
  //for b-tagging jets
  int njets0BTag = 0, njets1BTag = 0, njetsGE2BTag = 0;
  for ( int i=0; i < jetsForBTag.size(); i++ ) {
    if ( checkBTag(jetsForBTag[i], 0) ) njets0BTag++;
    if ( checkBTag(jetsForBTag[i], 1) ) njets1BTag++;
    if ( checkBTag(jetsForBTag[i], 2) ) njetsGE2BTag++;
  }

  //for HT and missingET/HT
  double HT = 0.;
  for ( int i=0; i < jets.size(); i++ ) {
    if ( jets[i]->PT > 40. ) HT += jets[i]->PT;
  }
  double R = missingET->P4().Et() / sqrt(HT);

  //for pt_min = 50 and |eta| < 2
  std::vector<Jet*> jets_multiFlavour50 = filterPhaseSpace(jets, 50., -2., 2.);

  int flag8jets_multiFlavour50 = 0, flag9jets_multiFlavour50 = 0, flagGE10jets_multiFlavour50 = 0;
  std::string flagCutflow, flagSignal;

  if ( jets_multiFlavour50.size() == 8 ) {
    flag8jets_multiFlavour50 = 1;
    flagCutflow = "CR05_flavour_8j50";
    flagSignal  = "SR01_8j50";
  }
  if ( jets_multiFlavour50.size() == 9 ) {
    flag9jets_multiFlavour50 = 1;
    flagCutflow = "CR06_flavour_9j50";
    flagSignal  = "SR02_9j50";
  }
  if ( jets_multiFlavour50.size() >= 10 ) flagGE10jets_multiFlavour50 = 1;

  //for signal region 8j50 and 9j50 in multi-jet + flavour
  if ( flag8jets_multiFlavour50 || flag9jets_multiFlavour50 ) {
    countCutflowEvent(flagCutflow+"_"+"a.base");
    //for missingET / sqrt(HT)
    if ( R > 4. ) {
      countCutflowEvent(flagCutflow+"_"+"b.R");
      //for classifying based on b-tagging jets
      if ( njets0BTag == 0 ) {
        countCutflowEvent(flagCutflow+"_"+"c.0bjet");
        countSignalEvent(flagSignal+"_"+"a.0b");
      }
      if ( njets1BTag == 1 ) {
        countCutflowEvent(flagCutflow+"_"+"d.1bjet");
        countSignalEvent(flagSignal+"_"+"b.1b");
      }
      if ( njetsGE2BTag >= 2 ) {
        countCutflowEvent(flagCutflow+"_"+"e.GE2bjet");
        countSignalEvent(flagSignal+"_"+"c.GE2b");
      }
    }
  }

  //for signal region 10j50 in multi-jet + flavour
  if ( flagGE10jets_multiFlavour50 ) {
    countCutflowEvent(std::string("CR07_flavour_GE10j50")+"_"+"a.base");
    //for missingET / sqrt(HT)
    if ( R > 4. ) {
      countCutflowEvent(std::string("CR07_flavour_GE10j50")+"_"+"b.R");
      countSignalEvent("SR03_GE10j50");
    }
  }

  //for pt_min = 80
  std::vector<Jet*> jets_multiFlavour80 = filterPhaseSpace(jets, 80., -2., 2.);
  
  int flag7jets_multiFlavour80 = 0, flagGE8jets_multiFlavour80 = 0;

  if ( jets_multiFlavour80.size() == 7 ) {
    flag7jets_multiFlavour80 = 1;
    flagCutflow = "CR08_flavour_7j80";
    flagSignal  = "SR04_7j80";
  }
  if ( jets_multiFlavour80.size() >= 8 ) {
    flagGE8jets_multiFlavour80  = 1;
    flagCutflow = "CR09_flavour_GE8j80";
    flagSignal  = "SR05_GE8j80";
  }


  //for signal region 7j80 and 8j80 in multi-jet + flavour
  if ( flag7jets_multiFlavour80 || flagGE8jets_multiFlavour80 ) {
    countCutflowEvent(flagCutflow+"_"+"a.base");
    //for missingET / sqrt(HT)
    if ( R > 4. ) {
      countCutflowEvent(flagCutflow+"_"+"b.R");
      //for classifying based on b-tagging jets
      if ( njets0BTag == 0 ) {
        countCutflowEvent(flagCutflow+"_"+"c.0bjet");
        countSignalEvent(flagSignal+"_"+"a.0b");
      }
      if ( njets1BTag == 1 ) {
        countCutflowEvent(flagCutflow+"_"+"d.1bjet");
        countSignalEvent(flagSignal+"_"+"b.1b");
      }
      if ( njetsGE2BTag >= 2 ) {
        countCutflowEvent(flagCutflow+"_"+"e.GE2bjet");
        countSignalEvent(flagSignal+"_"+"c.GE2b");
      }
    }
  }

//for multi-jet + M_J^Sigma steam
  //for composite jets
  std::vector<fastjet::PseudoJet> particles;
  for ( int i = 0; i < jets.size(); i++ ) {
    double tmppx = jets[i]->P4().Px(); 
    double tmppy = jets[i]->P4().Py(); 
    double tmppz = jets[i]->P4().Pz(); 
    double tmpe  = jets[i]->P4().E(); 
    particles.push_back( fastjet::PseudoJet(tmppx, tmppy, tmppz, tmpe) );
  }


  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, 1.0);

  fastjet::ClusterSequence cs(particles, jet_def);
  std::vector<fastjet::PseudoJet> compjets = sorted_by_pt(cs.inclusive_jets());

  //std::vector<Jet*> compjets = birthCompJets(jets, 1.);
  double MJS = 0.;
  for ( int i=0; i < compjets.size(); i++ ) {
    if ( compjets[i].pt() > 100. && fabs(compjets[i].eta()) < 1.5 ) MJS += compjets[i].m();
  }

  //for pt_min = 50 and |eta| < 2.8
  std::vector<Jet*> jets_multiM50 = filterPhaseSpace(jets, 50., -2.8, 2.8);

  int flagGE8jets_multiM50 = 0; int flagGE9jets_multiM50 = 0; int flagGE10jets_multiM50 = 0;
  if ( jets_multiM50.size() >= 8 )  flagGE8jets_multiM50  = 1;
  if ( jets_multiM50.size() >= 9 )  flagGE9jets_multiM50  = 1;
  if ( jets_multiM50.size() >= 10 ) flagGE10jets_multiM50 = 1;

  //for signal region 8j50, 9j50 and 10j50 in multi-jet + M_J^S
  if ( flagGE8jets_multiM50 || flagGE9jets_multiM50 || flagGE10jets_multiM50 ){
    if ( flagGE8jets_multiM50 )  countCutflowEvent("CR10_m_GE8j50_a.base");
    if ( flagGE9jets_multiM50 )  countCutflowEvent("CR11_m_GE9j50_a.base");
    if ( flagGE10jets_multiM50 ) countCutflowEvent("CR12_m_GE10j50_a.base");
    //for missingET / sqrt(HT)
    if ( R > 4. ) {
      if ( flagGE8jets_multiM50 )  countCutflowEvent("CR10_m_GE8j50_b.R");
      if ( flagGE9jets_multiM50 )  countCutflowEvent("CR11_m_GE9j50_b.R");
      if ( flagGE10jets_multiM50 ) countCutflowEvent("CR12_m_GE10j50_b.R");
      //for M_J^S
      if ( MJS > 340. ) {
        if ( flagGE8jets_multiM50 ) {
          countCutflowEvent("CR10_m_GE8j50_c.M340");
          countSignalEvent("SR06_GE8j50_340");
        }
        if ( flagGE9jets_multiM50 ) {
          countCutflowEvent("CR11_m_GE9j50_c.M340");
          countSignalEvent("SR07_GE9j50_340");
        }
        if ( flagGE10jets_multiM50 ) {
          countCutflowEvent("CR12_m_GE10j50_c.M340");
          countSignalEvent("SR08_GE10j50_340");
        }
      }
      if ( MJS > 420. ) {
        if ( flagGE8jets_multiM50 ) {
          countCutflowEvent("CR10_m_GE8j50_d.M420");
          countSignalEvent("SR06_GE8j50_420");
        }
        if ( flagGE9jets_multiM50 ) {
          countCutflowEvent("CR11_m_GE9j50_d.M420");
          countSignalEvent("SR07_GE9j50_420");
        }
        if ( flagGE10jets_multiM50 ) {
          countCutflowEvent("CR12_m_GE10j50_d.M420");
          countSignalEvent("SR08_GE10j50_420");
        }
      }
    }
  }

}

void Atlas_1308_1841::finalize() {
    //fout1.close();
}       
