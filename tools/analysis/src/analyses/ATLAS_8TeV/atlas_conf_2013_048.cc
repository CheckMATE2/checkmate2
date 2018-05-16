#include "atlas_conf_2013_048.h"

std::string Atlas_conf_2013_048::cf_index[12] = {"0","1","2","3","4","5","6","7","8","9","10","11"};

void Atlas_conf_2013_048::initialize() {
  setAnalysisName("atlas_conf_2013_048");          
  setInformation(""
    "@#search for direct top squark pair production in final states with two leptons\n"
  "");
  setLuminosity(20*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  
  // You should initialize any declared variables here
  bookSignalRegions("SRM90;SRM100;SRM110;SRM120");
  for (int i=0; i<12; i++)
    bookCutflowRegions("SFM90"+cf_index[i]+";SFM100"+cf_index[i]+";SFM110"+cf_index[i]+";SFM120"+cf_index[i]+";DFM90"+cf_index[i]+";DFM100"+cf_index[i]+";DFM110"+cf_index[i]+";DFM120"+cf_index[i]);
}

void Atlas_conf_2013_048::analyze() {
  // Your eventwise analysis code goes here
  missingET->addMuons(muonsCombined);  
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 20., -2.5, 2.5);

  //object removal
  //remove jets within 0.2 of electrons
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  //remove leptons within 0.4 of any reconstructed jets
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);

  countCutflowEvent("SFM90"+cf_index[0]);
  countCutflowEvent("SFM100"+cf_index[0]);
  countCutflowEvent("SFM110"+cf_index[0]);
  countCutflowEvent("SFM120"+cf_index[0]);
  countCutflowEvent("DFM90"+cf_index[0]);
  countCutflowEvent("DFM100"+cf_index[0]);
  countCutflowEvent("DFM110"+cf_index[0]);
  countCutflowEvent("DFM120"+cf_index[0]);

  //trigger cuts

   trigger = false;

   // Trigger Cuts
   if( electronsTight.size() > 0 && electronsTight[0]->PT > cutSingleTriggerElectronPT &&
       (double) rand() / (double) RAND_MAX < 0.99 )
     trigger=true;
   else if( muonsCombined.size() > 0 && muonsCombined[0]->PT > cutSingleTriggerMuonPT  &&
        (double) rand() / (double) RAND_MAX < 0.99 ) 
     trigger=true;
   else if( electronsTight.size() > 1 && electronsTight[0]->PT > cutDoubleTriggerElectronPT &&
        electronsTight[1]->PT > cutDoubleTriggerElectronPT  &&
        (double) rand() / (double) RAND_MAX < 0.99 )
     trigger=true;
   else if( muonsCombined.size() > 1 && muonsCombined[0]->PT > cutDoubleTriggerMuonPT &&
        muonsCombined[1]->PT > cutDoubleTriggerMuonPT  &&
        (double) rand() / (double) RAND_MAX < 0.99 )
     trigger=true;
   else if( muonsCombined.size() > 0 && electronsTight.size() > 0 && muonsCombined[0]->PT > cutMixedTriggerMuonPT &&
        electronsTight[0]->PT > cutMixedTriggerElectronPT  &&
        (double) rand() / (double) RAND_MAX < 0.99 )
     trigger=true;
   
   if( !trigger )
     return;

  countCutflowEvent("SFM90"+cf_index[1]);
  countCutflowEvent("SFM100"+cf_index[1]);
  countCutflowEvent("SFM110"+cf_index[1]);
  countCutflowEvent("SFM120"+cf_index[1]);
  countCutflowEvent("DFM90"+cf_index[1]);
  countCutflowEvent("DFM100"+cf_index[1]);
  countCutflowEvent("DFM110"+cf_index[1]);
  countCutflowEvent("DFM120"+cf_index[1]);

  //performing the analysis for SF
  
  if( electronsMedium.size() == 2 || muonsCombined.size() == 2 ){
    countCutflowEvent("SFM90"+cf_index[2]);
    countCutflowEvent("SFM100"+cf_index[2]);
    countCutflowEvent("SFM110"+cf_index[2]);
    countCutflowEvent("SFM120"+cf_index[2]);
   

    electronsTight = filterIsolation(electronsTight);
    muonsCombined=filterIsolation(muonsCombined);
    
    bool isol_flag = false;
    if( electronsTight.size() == 2 || muonsCombined.size() == 2 )
      isol_flag = true;

    if( isol_flag == true ){
      countCutflowEvent("SFM90"+cf_index[3]);
      countCutflowEvent("SFM100"+cf_index[3]);
      countCutflowEvent("SFM110"+cf_index[3]);
      countCutflowEvent("SFM120"+cf_index[3]);
    }
    
    if( !isol_flag && ( electronsTight.size() == 2 && electronsTight[0]->Charge*electronsTight[1]->Charge > 0 ) ||
    ( muonsCombined.size() == 2  && muonsCombined[0]->Charge*muonsCombined[1]->Charge > 0  ) )
      return;

    countCutflowEvent("SFM90"+cf_index[4]);
    countCutflowEvent("SFM100"+cf_index[4]);
    countCutflowEvent("SFM110"+cf_index[4]);
    countCutflowEvent("SFM120"+cf_index[4]);

    //SFOS invariant mass calculation
    
    if( electronsTight.size() == 2 ){
    mtest=(electronsTight[0]->P4() + electronsTight[1]->P4()).M();
    leadingleptonpt = electronsTight[0]->PT;
    pllTb = electronsTight[0]->P4()+electronsTight[1]->P4()+missingET->P4();
    }
    if( muonsCombined.size() == 2 ){
    mtest=(muonsCombined[0]->P4() + muonsCombined[1]->P4()).M();
    leadingleptonpt = muonsCombined[0]->PT;
    pllTb = muonsCombined[0]->P4()+muonsCombined[1]->P4()+missingET->P4();
    }
    if( mtest<20. ) 
      return;
    
    countCutflowEvent("SFM90"+cf_index[5]);
    countCutflowEvent("SFM100"+cf_index[5]);
    countCutflowEvent("SFM110"+cf_index[5]);
    countCutflowEvent("SFM120"+cf_index[5]);
    
    if( leadingleptonpt < 25. )
      return;

    countCutflowEvent("SFM90"+cf_index[6]);
    countCutflowEvent("SFM100"+cf_index[6]);
    countCutflowEvent("SFM110"+cf_index[6]);
    countCutflowEvent("SFM120"+cf_index[6]);
    
    if( fabs( mtest-91. ) < 20. ) 
      return;

    countCutflowEvent("SFM90"+cf_index[7]);
    countCutflowEvent("SFM100"+cf_index[7]);
    countCutflowEvent("SFM110"+cf_index[7]);
    countCutflowEvent("SFM120"+cf_index[7]);

    deltamin=10000.;
      
    for( int i=0; i<jets.size(); i++ ){
      deltatemp=missingET->P4().DeltaR(jets[i]->P4());
      if(deltatemp < deltamin){
    deltamin=deltatemp;
    i_closestjet=i;
      }
    }

    if( fabs(missingET->P4().DeltaPhi(jets[i_closestjet]->P4())) < 1. )
      return;
    
    countCutflowEvent("SFM90"+cf_index[8]);
    countCutflowEvent("SFM100"+cf_index[8]);
    countCutflowEvent("SFM110"+cf_index[8]);
    countCutflowEvent("SFM120"+cf_index[8]);

    if( fabs(pllTb.DeltaPhi(missingET->P4())) > 1.5 )
      return;
    
    countCutflowEvent("SFM90"+cf_index[9]);
    countCutflowEvent("SFM100"+cf_index[9]);
    countCutflowEvent("SFM110"+cf_index[9]);
    countCutflowEvent("SFM120"+cf_index[9]);
    
    //--------------------------
    //------Signal Regions------
    //--------------------------

    if( electronsTight.size() == 2 ){
      double pa[3] = { electronsTight[0]->P4().M(), electronsTight[0]->P4().Px(), electronsTight[0]->P4().Py() };
      double pb[3] = { electronsTight[1]->P4().M(), electronsTight[1]->P4().Px(), electronsTight[1]->P4().Py() };
      double pmiss[3] = {0.,missingET->P4().Px(), missingET->P4().Py() };
      mt2_bisect::mt2 mt2_event;
      mt2_event.set_momenta( pa, pb, pmiss );
      mt2_event.set_mn( 0. );
      double m_t2 = mt2_event.get_mt2();
      if( jets.size() > 0 && m_t2 > 90. ){
    countCutflowEvent("SFM90"+cf_index[11]);
    countSignalEvent("SRM90");
      }
      if( jets.size() >  1 && m_t2 > 100. && jets[0]->PT > 100. && jets[1]->PT > 50. ){
    countCutflowEvent("SFM100"+cf_index[11]);
    countSignalEvent("SRM100");
      }
      if( jets.size() >  1 && m_t2 > 110. && jets[0]->PT > 20. && jets[1]->PT > 20. ){
    countCutflowEvent("SFM110"+cf_index[11]);
    countSignalEvent("SRM110");
      }
      if( jets.size() > 1 && m_t2 > 120. && jets[0]->PT > 20. && jets[1]->PT > 20. ){
    countCutflowEvent("SFM120"+cf_index[11]);
    countSignalEvent("SRM120");
      }
    }
    if( muonsCombined.size() == 2 ){
      double pa[3] = { muonsCombined[0]->P4().M(), muonsCombined[0]->P4().Px(), muonsCombined[0]->P4().Py() };
      double pb[3] = { muonsCombined[1]->P4().M(), muonsCombined[1]->P4().Px(), muonsCombined[1]->P4().Py() };
      double pmiss[3] = {0.,missingET->P4().Px(), missingET->P4().Py() };
      mt2_bisect::mt2 mt2_event;
      mt2_event.set_momenta( pa, pb, pmiss );
      mt2_event.set_mn( 0. );
      double m_t2 = mt2_event.get_mt2();
      if( jets.size() > 0 && m_t2 > 90. ){
    countCutflowEvent("SFM90"+cf_index[11]);
    countSignalEvent("SRM90");
      }
      if( jets.size() >  1 && m_t2 > 100. && jets[0]->PT > 100. && jets[1]->PT > 50. ){
    countCutflowEvent("SFM100"+cf_index[11]);
    countSignalEvent("SRM100");
      }
      if( jets.size() >  1 && m_t2 > 110. && jets[0]->PT > 20. && jets[1]->PT > 20. ){
    countCutflowEvent("SFM110"+cf_index[11]);
    countSignalEvent("SRM110");
      }
      if( jets.size() > 0 && m_t2 > 120. ){
    countCutflowEvent("SFM120"+cf_index[11]);
    countSignalEvent("SRM120");
      }

    }
    
  }

  //performing analysis for DF


  else if ( electronsMedium.size() == 1 && muonsCombined.size() == 1 ){
    
    
    countCutflowEvent("DFM90"+cf_index[2]);
    countCutflowEvent("DFM100"+cf_index[2]);
    countCutflowEvent("DFM110"+cf_index[2]);
    countCutflowEvent("DFM120"+cf_index[2]);

    electronsTight = filterIsolation(electronsTight);
    muonsCombined=filterIsolation(muonsCombined);
    

    if( electronsTight.size() != 1  || muonsCombined.size() != 1 )
      return;

    countCutflowEvent("DFM90"+cf_index[3]);
    countCutflowEvent("DFM100"+cf_index[3]);
    countCutflowEvent("DFM110"+cf_index[3]);
    countCutflowEvent("DFM120"+cf_index[3]);
    
    if( electronsTight[0]->Charge*muonsCombined[0]->Charge > 0 )
      return;

    countCutflowEvent("DFM90"+cf_index[4]);
    countCutflowEvent("DFM100"+cf_index[4]);
    countCutflowEvent("DFM110"+cf_index[4]);
    countCutflowEvent("DFM120"+cf_index[4]);

    mtest=(muonsCombined[0]->P4() + electronsTight[0]->P4()).M();
    pllTb = muonsCombined[0]->P4()+electronsTight[0]->P4();
    
    if(mtest<20.) 
      return;

    countCutflowEvent("DFM90"+cf_index[5]);
    countCutflowEvent("DFM100"+cf_index[5]);
    countCutflowEvent("DFM110"+cf_index[5]);
    countCutflowEvent("DFM120"+cf_index[5]);

    if( electronsTight[0]->PT >= muonsCombined[0]->PT )
      leadingleptonpt = electronsTight[0]->PT;
    else
      leadingleptonpt = muonsCombined[0]->PT;
    
    if( leadingleptonpt < 25. )
      return;

    countCutflowEvent("DFM90"+cf_index[6]);
    countCutflowEvent("DFM100"+cf_index[6]);
    countCutflowEvent("DFM110"+cf_index[6]);
    countCutflowEvent("DFM120"+cf_index[6]);

    if( fabs( mtest - 91. ) < 20. ) 
      return;
    
    countCutflowEvent("DFM90"+cf_index[7]);
    countCutflowEvent("DFM100"+cf_index[7]);
    countCutflowEvent("DFM110"+cf_index[7]);
    countCutflowEvent("DFM120"+cf_index[7]);

    deltamin=10000.;

    for( int i=0; i<jets.size(); i++ ){
    deltatemp=missingET->P4().DeltaR(jets[i]->P4());
    if(deltatemp < deltamin){
      deltamin=deltatemp;
      i_closestjet=i;
    }
    }
    
    if( fabs(missingET->P4().DeltaPhi(jets[i_closestjet]->P4())) < 1. )
      return;
    
    countCutflowEvent("DFM90"+cf_index[8]);
    countCutflowEvent("DFM100"+cf_index[8]);
    countCutflowEvent("DFM110"+cf_index[8]);
    countCutflowEvent("DFM120"+cf_index[8]);
    
    if( fabs(pllTb.DeltaPhi(missingET->P4())) > 1.5 )
      return;
    
    countCutflowEvent("DFM90"+cf_index[9]);
    countCutflowEvent("DFM100"+cf_index[9]);
    countCutflowEvent("DFM110"+cf_index[9]);
    countCutflowEvent("DFM120"+cf_index[9]);
    
    double pa[3] = { electronsTight[0]->P4().M(), electronsTight[0]->P4().Px(), electronsTight[0]->P4().Py() };
    double pb[3] = { muonsCombined[0]->P4().M(), muonsCombined[0]->P4().Px(), muonsCombined[0]->P4().Py() };
    double pmiss[3] = { 0., missingET->P4().Px(), missingET->P4().Py() };
    mt2_bisect::mt2 mt2_event;
    mt2_event.set_momenta( pa, pb, pmiss );
    mt2_event.set_mn( 0. );
    double m_t2 = mt2_event.get_mt2();


    if( jets.size() > 0 && m_t2 > 90. ){
      countCutflowEvent("DFM90"+cf_index[11]);
      countSignalEvent("SRM90");
    }
    if( jets.size() > 1 && m_t2 > 100. && jets[0]->PT > 100. && jets[1]->PT > 50. ){
      countCutflowEvent("DFM100"+cf_index[11]);
      countSignalEvent("SRM100");
    }
    if( jets.size() > 1 && m_t2 > 110. && jets[0]->PT > 20. && jets[1]->PT > 20. ){
      countCutflowEvent("DFM110"+cf_index[11]);
      countSignalEvent("SRM110");
    }
    if( jets.size() > 1 && m_t2 > 120. && jets[0]->PT > 20. && jets[1]->PT > 20. ){
      countCutflowEvent("DFM120"+cf_index[11]);
      countSignalEvent("SRM120");
    }



  }  

  else
    return;
   
}

void Atlas_conf_2013_048::finalize() {
  // Whatever should be done after the run goes here
}       
