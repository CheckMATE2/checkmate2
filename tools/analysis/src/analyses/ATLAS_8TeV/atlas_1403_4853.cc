#include "atlas_1403_4853.h"

std::string Atlas_1403_4853::cf_index[12] = {"0","1","2","3","4","5","6","7","8","9","10","11"};

void Atlas_1403_4853::initialize() {
  setAnalysisName("atlas_1403_4853");          
  setInformation(""
    "@#Search for direct top-squark pair production in final states with two leptons in pp collisions at \\sqrt{s}=8 TeV with the ATLAS detector\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("L90;L100;L110;L120;H160;SR1;SR2;SR3;SR4;SR5;SR6;SR7;");
  for (int i=0; i<12; i++)
    bookCutflowRegions("SFM90"+cf_index[i]+";SFM100"+cf_index[i]+";SFM110"+cf_index[i]+";SFM120"+cf_index[i]+";DFM90"+cf_index[i]+";DFM100"+cf_index[i]+";DFM110"+cf_index[i]+";DFM120"+cf_index[i]+";SFH160"+cf_index[i]+";DFH160"+cf_index[i]);
  // You should initialize any declared variables here
}

void Atlas_1403_4853::analyze() {
  // Your eventwise analysis code goes here
  missingET->addMuons(muonsCombined);  
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47);
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
  countCutflowEvent("SFH160"+cf_index[0]);
  countCutflowEvent("DFH160"+cf_index[0]);

  //trigger cuts

   trigger = false;

   cutSingleTriggerElectronPT = 25.;
   cutDoubleTriggerElectronPT = 14.;
   cutMixedTriggerElectronPT = 10.;

   cutSingleTriggerMuonPT = 25.;
   cutDoubleTriggerMuonPT = 14.;
   cutMixedTriggerMuonPT = 18.;


   // Trigger Cuts
   if( electronsTight.size() > 0 && electronsTight[0]->PT > cutSingleTriggerElectronPT &&
       (double) rand() / (double) RAND_MAX < 0.80 )
     trigger=true;
   else if( muonsCombined.size() > 0 && muonsCombined[0]->PT > cutSingleTriggerMuonPT  &&
        (double) rand() / (double) RAND_MAX < 0.99 ) 
     trigger=true;
   else if( electronsTight.size() > 1 && electronsTight[0]->PT > cutDoubleTriggerElectronPT &&
        electronsTight[1]->PT > cutDoubleTriggerElectronPT  &&
        (double) rand() / (double) RAND_MAX < 0.85 )
     trigger=true;
   else if( muonsCombined.size() > 1 && muonsCombined[0]->PT > cutDoubleTriggerMuonPT &&
        muonsCombined[1]->PT > cutDoubleTriggerMuonPT  &&
        (double) rand() / (double) RAND_MAX < 0.70 )
     trigger=true;
   else if( muonsCombined.size() > 0 && electronsTight.size() > 0 && muonsCombined[0]->PT > cutMixedTriggerMuonPT &&
        electronsTight[0]->PT > cutMixedTriggerElectronPT  &&
        (double) rand() / (double) RAND_MAX < 0.70 )
     trigger=true;
   
  // trigger = true;

//   if( !trigger )
//     return; (*moved down*)

   //   if (  (double) rand() / (double) RAND_MAX < 0.40 )
   //     return;

  countCutflowEvent("SFM90"+cf_index[1]);
  countCutflowEvent("SFM100"+cf_index[1]);
  countCutflowEvent("SFM110"+cf_index[1]);
  countCutflowEvent("SFM120"+cf_index[1]);
  countCutflowEvent("DFM90"+cf_index[1]);
  countCutflowEvent("DFM100"+cf_index[1]);
  countCutflowEvent("DFM110"+cf_index[1]);
  countCutflowEvent("DFM120"+cf_index[1]);
  countCutflowEvent("SFH160"+cf_index[1]);
  countCutflowEvent("DFH160"+cf_index[1]);

  //performing the analysis for SF


  if ( electronsMedium.size() + muonsCombined.size() != 2 )
    return;

  countCutflowEvent("SFM90"+cf_index[2]);
  countCutflowEvent("SFM100"+cf_index[2]);
  countCutflowEvent("SFM110"+cf_index[2]);
  countCutflowEvent("SFM120"+cf_index[2]);
  countCutflowEvent("DFM90"+cf_index[2]);
  countCutflowEvent("DFM100"+cf_index[2]);
  countCutflowEvent("DFM110"+cf_index[2]);
  countCutflowEvent("DFM120"+cf_index[2]);
  countCutflowEvent("SFH160"+cf_index[2]);
  countCutflowEvent("DFH160"+cf_index[2]);

  
  if(  electronsMedium.size() == 2  || muonsCombined.size() == 2  ){
    countCutflowEvent("SFM90"+cf_index[3]);
    countCutflowEvent("SFM100"+cf_index[3]);
    countCutflowEvent("SFM110"+cf_index[3]);
    countCutflowEvent("SFM120"+cf_index[3]);
    countCutflowEvent("SFH160"+cf_index[3]);
   

    electronsTight = filterIsolation(electronsTight);
    muonsCombined = filterIsolation(muonsCombined);
//    electronsTight = overlapRemoval(electronsTight, jets, 0.4);
//    muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);

    
    if( electronsTight.size() + muonsCombined.size() != 2 )
      return;

    countCutflowEvent("SFM90"+cf_index[4]);
    countCutflowEvent("SFM100"+cf_index[4]);
    countCutflowEvent("SFM110"+cf_index[4]);
    countCutflowEvent("SFM120"+cf_index[4]);
    countCutflowEvent("SFH160"+cf_index[4]);
    
    if(  ( electronsTight.size() == 2 && electronsTight[0]->Charge*electronsTight[1]->Charge > 0 ) || ( muonsCombined.size() == 2  && muonsCombined[0]->Charge*muonsCombined[1]->Charge > 0  )  )
      return;

    countCutflowEvent("SFM90"+cf_index[5]);
    countCutflowEvent("SFM100"+cf_index[5]);
    countCutflowEvent("SFM110"+cf_index[5]);
    countCutflowEvent("SFM120"+cf_index[5]);
    countCutflowEvent("SFH160"+cf_index[5]);

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
    if( mtest < 20. ) 
      return;
    
    countCutflowEvent("SFM90"+cf_index[6]);
    countCutflowEvent("SFM100"+cf_index[6]);
    countCutflowEvent("SFM110"+cf_index[6]);
    countCutflowEvent("SFM120"+cf_index[6]);
    countCutflowEvent("SFH160"+cf_index[6]);
    
    if( !trigger )
      return;
    
    if( leadingleptonpt < 25. )
      return;

    countCutflowEvent("SFM90"+cf_index[7]);
    countCutflowEvent("SFM100"+cf_index[7]);
    countCutflowEvent("SFM110"+cf_index[7]);
    countCutflowEvent("SFM120"+cf_index[7]);
    countCutflowEvent("SFH160"+cf_index[7]);
    

    bool zveto = false;
    if( fabs( mtest-91. ) > 20. ) {
      zveto = true;
      countCutflowEvent("SFM90"+cf_index[8]);
      countCutflowEvent("SFM100"+cf_index[8]);
      countCutflowEvent("SFM110"+cf_index[8]);
      countCutflowEvent("SFM120"+cf_index[8]);
    }
    
    deltamin=10000.;
      
    for( int i=0; i<jets.size(); i++ ){
     deltatemp=missingET->P4().DeltaR(jets[i]->P4());
//      deltatemp=missingET->P4().DeltaPhi(jets[i]->P4());
      if(deltatemp < deltamin){
    deltamin=deltatemp;
    i_closestjet=i;
      }
    }

    bool deltaphiveto= false;
    if( (zveto && jets.size() != 0 && fabs(missingET->P4().DeltaPhi(jets[i_closestjet]->P4())) > 1.) || (zveto && jets.size() == 0 ) ){
      deltaphiveto = true;
      countCutflowEvent("SFM90"+cf_index[9]);
      countCutflowEvent("SFM100"+cf_index[9]);
      countCutflowEvent("SFM110"+cf_index[9]);
      countCutflowEvent("SFM120"+cf_index[9]);
    }

    bool deltaphibveto = false;
    if( deltaphiveto && fabs(pllTb.DeltaPhi(missingET->P4())) < 1.5 ){
      deltaphibveto = true;
      countCutflowEvent("SFM90"+cf_index[10]);
      countCutflowEvent("SFM100"+cf_index[10]);
      countCutflowEvent("SFM110"+cf_index[10]);
      countCutflowEvent("SFM120"+cf_index[10]);
    }
    
    //--------------------------
    //------Signal Regions------
    //--------------------------

    if( deltaphibveto && electronsTight.size() == 2 ){
//    double pa[3] = { electronsTight[0]->P4().M(), electronsTight[0]->P4().Px(), electronsTight[0]->P4().Py() };
//   double pb[3] = { electronsTight[1]->P4().M(), electronsTight[1]->P4().Px(), electronsTight[1]->P4().Py() };
      double pa[3] = { 0. , electronsTight[0]->P4().Px(), electronsTight[0]->P4().Py() };
      double pb[3] = { 0. , electronsTight[1]->P4().Px(), electronsTight[1]->P4().Py() };
      double pmiss[3] = {0.,missingET->P4().Px(), missingET->P4().Py() };
      mt2_bisect::mt2 mt2_event;
      mt2_event.set_momenta( pa, pb, pmiss );
      mt2_event.set_mn( 0. );
      double m_t2 = mt2_event.get_mt2();

      if(  m_t2 > 90. ){
    countCutflowEvent("SFM90"+cf_index[11]);
    countSignalEvent("L90");
      }
      if( jets.size() >  1 && m_t2 > 100. && jets[0]->PT > 100. && jets[1]->PT > 50. ){
    countCutflowEvent("SFM100"+cf_index[11]);
    countSignalEvent("L100");
      }
      if( jets.size() >  1 && m_t2 > 110. && jets[0]->PT > 20. && jets[1]->PT > 20. ){
    countCutflowEvent("SFM110"+cf_index[11]);
    countSignalEvent("L110");
      }
      if(  m_t2 > 120. ){
    countCutflowEvent("SFM120"+cf_index[11]);
    countSignalEvent("L120");
      }
      
      //exclusion SR

      if ( m_t2 > 90 ){
    int jetsel=10;
    if( jets.size() > 1 && jets[0]->PT > 100. && jets[1]->PT > 50. )
      jetsel = 0;
    if( jets.size() > 1 && jetsel > 0 )
      jetsel = 1;
    if( jets.size() < 2  )
      jetsel = 2;
    
    if( (jetsel == 0 && m_t2 < 100.) || (jetsel == 1 && m_t2 < 110.) || (jetsel == 2 && m_t2 < 120.) )
      countSignalEvent("SR1");
    if( jetsel == 0 && m_t2 > 100. && m_t2 < 110. )
      countSignalEvent("SR2");
    if( jetsel == 0 && m_t2 > 110. && m_t2 < 120. )
      countSignalEvent("SR3");
    if( jetsel == 0 && m_t2 > 120. )
      countSignalEvent("SR5");
    if( jetsel == 1 && m_t2 > 110. && m_t2 < 120.)
      countSignalEvent("SR4");
    if( jetsel == 1 && m_t2 > 120. )
      countSignalEvent("SR6");
    if( jetsel == 2 && m_t2 > 120. )
      countSignalEvent("SR7");
      }

      

      
    }
    if( deltaphibveto && muonsCombined.size() == 2 ){
//   double pa[3] = { muonsCombined[0]->P4().M(), muonsCombined[0]->P4().Px(), muonsCombined[0]->P4().Py() };
//   double pb[3] = { muonsCombined[1]->P4().M(), muonsCombined[1]->P4().Px(), muonsCombined[1]->P4().Py() };
      double pa[3] = { 0. , muonsCombined[0]->P4().Px(), muonsCombined[0]->P4().Py() };
      double pb[3] = { 0. , muonsCombined[1]->P4().Px(), muonsCombined[1]->P4().Py() };
      double pmiss[3] = {0.,missingET->P4().Px(), missingET->P4().Py() };
      mt2_bisect::mt2 mt2_event;
      mt2_event.set_momenta( pa, pb, pmiss );
      mt2_event.set_mn( 0. );
      double m_t2 = mt2_event.get_mt2();
      if(  m_t2 > 90. ){
    countCutflowEvent("SFM90"+cf_index[11]);
    countSignalEvent("L90");
      }
      if( jets.size() >  1 && m_t2 > 100. && jets[0]->PT > 100. && jets[1]->PT > 50. ){
    countCutflowEvent("SFM100"+cf_index[11]);
    countSignalEvent("L100");
      }
      if( jets.size() >  1 && m_t2 > 110. && jets[0]->PT > 20. && jets[1]->PT > 20. ){
    countCutflowEvent("SFM110"+cf_index[11]);
    countSignalEvent("L110");
      }
      if(  m_t2 > 120. ){
    countCutflowEvent("SFM120"+cf_index[11]);
    countSignalEvent("L120");
      }

      //exclusion SR

      if ( m_t2 > 90 ){
    int jetsel=10;
    if( jets.size() > 1 && jets[0]->PT > 100. && jets[1]->PT > 50. )
      jetsel = 0;
    if( jets.size() > 1 && jetsel > 0 )
      jetsel = 1;
    if( jets.size() < 2  )
      jetsel = 2;
    
    if( (jetsel == 0 && m_t2 < 100.) || (jetsel == 1 && m_t2 < 110.) || (jetsel == 2 && m_t2 < 120.) )
      countSignalEvent("SR1");
    if( jetsel == 0 && m_t2 > 100. && m_t2 < 110. )
      countSignalEvent("SR2");
    if( jetsel == 0 && m_t2 > 110. && m_t2 < 120. )
      countSignalEvent("SR3");
    if( jetsel == 0 && m_t2 > 120. )
      countSignalEvent("SR5");
    if( jetsel == 1 && m_t2 > 110. && m_t2 < 120.)
      countSignalEvent("SR4");
    if( jetsel == 1 && m_t2 > 120. )
      countSignalEvent("SR6");
    if( jetsel == 2 && m_t2 > 120. )
      countSignalEvent("SR7");
      }

    }
    

    // H160 Signalregion

    int nbJets = 0;
    std::vector<Jet*> bjets;
    for (unsigned i=0; i < jets.size(); i++) {
      if ( checkBTag(jets[i]) ) {
    nbJets += 1;
    bjets.push_back(jets[i]);
      }
      
    }

    if ( nbJets == 2 &&  electronsTight.size() == 2 ){
      countCutflowEvent("SFH160"+cf_index[8]);
      double pa_b[3] = { bjets[0]->P4().M(), bjets[0]->P4().Px(), bjets[0]->P4().Py() };
      double pb_b[3] = { bjets[1]->P4().M(), bjets[1]->P4().Px(), bjets[1]->P4().Py() };
      double pmiss_b[3] = {0., missingET->P4().Px() + electronsTight[0]->P4().Px() + electronsTight[1]->P4().Px(), missingET->P4().Py() + electronsTight[0]->P4().Py() + electronsTight[1]->P4().Py() };
      mt2_bisect::mt2 mt2b_event;
      mt2b_event.set_momenta( pa_b, pb_b, pmiss_b );
      mt2b_event.set_mn( 0. );
      double m_t2b = mt2b_event.get_mt2();

      bool mt2b_cut = false;
      if( m_t2b > 160. ){
      countCutflowEvent("SFH160"+cf_index[9]);
      mt2b_cut = true;
      }

      double pa[3] = { electronsTight[0]->P4().M(), electronsTight[0]->P4().Px(), electronsTight[0]->P4().Py() };
      double pb[3] = { electronsTight[1]->P4().M(), electronsTight[1]->P4().Px(), electronsTight[1]->P4().Py() };
      double pmiss[3] = {0.,missingET->P4().Px(), missingET->P4().Py() };
      mt2_bisect::mt2 mt2_event;
      mt2_event.set_momenta( pa, pb, pmiss );
      mt2_event.set_mn( 0. );
      double m_t2 = mt2_event.get_mt2();
      bool mt2_cut = false;
      if( mt2b_cut && m_t2 < 90. ){
      countCutflowEvent("SFH160"+cf_index[10]);
      mt2_cut = true;
      }

      if( mt2_cut && leadingleptonpt < 60. ){
      countCutflowEvent("SFH160"+cf_index[11]);
      countSignalEvent("H160");
      }
    }

    if ( nbJets == 2 &&  muonsCombined.size() == 2 ){
      countCutflowEvent("SFH160"+cf_index[8]);
      double pa_b[3] = { bjets[0]->P4().M(), bjets[0]->P4().Px(), bjets[0]->P4().Py() };
      double pb_b[3] = { bjets[1]->P4().M(), bjets[1]->P4().Px(), bjets[1]->P4().Py() };
      double pmiss_b[3] = {0., missingET->P4().Px() + muonsCombined[0]->P4().Px() + muonsCombined[1]->P4().Px(), missingET->P4().Py() + muonsCombined[0]->P4().Py() + muonsCombined[1]->P4().Py() };
      mt2_bisect::mt2 mt2b_event;
      mt2b_event.set_momenta( pa_b, pb_b, pmiss_b );
      mt2b_event.set_mn( 0. );
      double m_t2b = mt2b_event.get_mt2();
      
      bool mt2b_cut = false;
      if( m_t2b > 160. ){
    countCutflowEvent("SFH160"+cf_index[9]);
    mt2b_cut = true;
      }
      
      double pa[3] = { muonsCombined[0]->P4().M(), muonsCombined[0]->P4().Px(), muonsCombined[0]->P4().Py() };
      double pb[3] = { muonsCombined[1]->P4().M(), muonsCombined[1]->P4().Px(), muonsCombined[1]->P4().Py() };
      double pmiss[3] = {0., missingET->P4().Px(), missingET->P4().Py() };
      mt2_bisect::mt2 mt2_event;
      mt2_event.set_momenta( pa, pb, pmiss );
      mt2_event.set_mn( 0. );
      double m_t2 = mt2_event.get_mt2();
      bool mt2_cut = false;
      if( mt2b_cut && m_t2 < 90. ){
    countCutflowEvent("SFH160"+cf_index[10]);
      mt2_cut = true;
      }

      if( mt2_cut && leadingleptonpt < 60. ){
      countCutflowEvent("SFH160"+cf_index[11]);
      countSignalEvent("H160");
      }
    }
      
  }
    

    
  

  //performing analysis for DF


  else if ( electronsMedium.size() == 1 && muonsCombined.size() == 1 ){
    
    
    countCutflowEvent("DFM90"+cf_index[3]);
    countCutflowEvent("DFM100"+cf_index[3]);
    countCutflowEvent("DFM110"+cf_index[3]);
    countCutflowEvent("DFM120"+cf_index[3]);
    countCutflowEvent("DFH160"+cf_index[3]);

    electronsTight = overlapRemoval(electronsTight, jets, 0.4);
    muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);

    electronsTight = filterIsolation(electronsTight);
    muonsCombined=filterIsolation(muonsCombined);
    
    if( electronsTight.size() + muonsCombined.size() != 2 )
      return;

    countCutflowEvent("DFM90"+cf_index[4]);
    countCutflowEvent("DFM100"+cf_index[4]);
    countCutflowEvent("DFM110"+cf_index[4]);
    countCutflowEvent("DFM120"+cf_index[4]);
    countCutflowEvent("DFH160"+cf_index[4]);


    if( electronsTight[0]->Charge*muonsCombined[0]->Charge > 0 )
      return;

    countCutflowEvent("DFM90"+cf_index[5]);
    countCutflowEvent("DFM100"+cf_index[5]);
    countCutflowEvent("DFM110"+cf_index[5]);
    countCutflowEvent("DFM120"+cf_index[5]);
    countCutflowEvent("DFH160"+cf_index[5]);

    mtest=(muonsCombined[0]->P4() + electronsTight[0]->P4()).M();
    pllTb = muonsCombined[0]->P4()+electronsTight[0]->P4()+missingET->P4();
    
    if( mtest < 20. ) 
      return;

    countCutflowEvent("DFM90"+cf_index[6]);
    countCutflowEvent("DFM100"+cf_index[6]);
    countCutflowEvent("DFM110"+cf_index[6]);
    countCutflowEvent("DFM120"+cf_index[6]);
    countCutflowEvent("DFH160"+cf_index[6]);
    
   if( !trigger )
      return;    

    if( electronsTight[0]->PT >= muonsCombined[0]->PT )
      leadingleptonpt = electronsTight[0]->PT;
    else
      leadingleptonpt = muonsCombined[0]->PT;
    
    if( leadingleptonpt < 25. )
      return;

    countCutflowEvent("DFM90"+cf_index[7]);
    countCutflowEvent("DFM100"+cf_index[7]);
    countCutflowEvent("DFM110"+cf_index[7]);
    countCutflowEvent("DFM120"+cf_index[7]);
    countCutflowEvent("DFH160"+cf_index[7]);

    //    if( fabs( mtest - 91. ) < 20. ) 
    //      return;
    
    countCutflowEvent("DFM90"+cf_index[8]);
    countCutflowEvent("DFM100"+cf_index[8]);
    countCutflowEvent("DFM110"+cf_index[8]);
    countCutflowEvent("DFM120"+cf_index[8]);

    deltamin=10000.;



    for( int i=0; i<jets.size(); i++ ){
    deltatemp=missingET->P4().DeltaR(jets[i]->P4());
//        deltatemp=missingET->P4().DeltaPhi(jets[i]->P4());
    if(deltatemp < deltamin){
      deltamin=deltatemp;
      i_closestjet=i;
    }
    }

    bool deltaphiveto= false;
    if( (jets.size() != 0 && fabs(missingET->P4().DeltaPhi(jets[i_closestjet]->P4())) > 1.) || jets.size() == 0  ){
      deltaphiveto = true;
      countCutflowEvent("DFM90"+cf_index[9]);
      countCutflowEvent("DFM100"+cf_index[9]);
      countCutflowEvent("DFM110"+cf_index[9]);
      countCutflowEvent("DFM120"+cf_index[9]);
    }

    bool deltaphibveto = false;
    if( deltaphiveto && fabs(pllTb.DeltaPhi(missingET->P4())) < 1.5 ){
      deltaphibveto = true;
      countCutflowEvent("DFM90"+cf_index[10]);
      countCutflowEvent("DFM100"+cf_index[10]);
      countCutflowEvent("DFM110"+cf_index[10]);
      countCutflowEvent("DFM120"+cf_index[10]);
    }
    
// change KR    double pa[3] = { electronsTight[0]->P4().M(), electronsTight[0]->P4().Px(), electronsTight[0]->P4().Py() };
// change KR    double pb[3] = { muonsCombined[0]->P4().M(), muonsCombined[0]->P4().Px(), muonsCombined[0]->P4().Py() };
    double pa[3] = { 0. , electronsTight[0]->P4().Px(), electronsTight[0]->P4().Py() };
    double pb[3] = { 0. , muonsCombined[0]->P4().Px(), muonsCombined[0]->P4().Py() };
    double pmiss[3] = { 0., missingET->P4().Px(), missingET->P4().Py() };
    mt2_bisect::mt2 mt2_event;
    mt2_event.set_momenta( pa, pb, pmiss );
    mt2_event.set_mn( 0. );
    double m_t2 = mt2_event.get_mt2();
    

    if( deltaphibveto && m_t2 > 90. ){
      countCutflowEvent("DFM90"+cf_index[11]);
      countSignalEvent("L90");
    }
    if( deltaphibveto && jets.size() > 1 && m_t2 > 100. && jets[0]->PT > 100. && jets[1]->PT > 50. ){
      countCutflowEvent("DFM100"+cf_index[11]);
      countSignalEvent("L100");
    }
    if( deltaphibveto && jets.size() > 1 && m_t2 > 110. && jets[0]->PT > 20. && jets[1]->PT > 20. ){
      countCutflowEvent("DFM110"+cf_index[11]);
      countSignalEvent("L110");
    }
    if( deltaphibveto && m_t2 > 120.  ){
      countCutflowEvent("DFM120"+cf_index[11]);
      countSignalEvent("L120");
    }

      //exclusion SR

      if ( deltaphibveto && m_t2 > 90 ){
    int jetsel=10;
    if( jets.size() > 1 && jets[0]->PT > 100. && jets[1]->PT > 50. )
      jetsel = 0;
    if( jets.size() > 1 && jetsel > 0 )
      jetsel = 1;
    if( jets.size() < 2  )
      jetsel = 2;
    
    if( (jetsel == 0 && m_t2 < 100.) || (jetsel == 1 && m_t2 < 110.) || (jetsel == 2 && m_t2 < 120.) )
      countSignalEvent("SR1");
    if( jetsel == 0 && m_t2 > 100. && m_t2 < 110. )
      countSignalEvent("SR2");
    if( jetsel == 0 && m_t2 > 110. && m_t2 < 120. )
      countSignalEvent("SR3");
    if( jetsel == 0 && m_t2 > 120. )
      countSignalEvent("SR5");
    if( jetsel == 1 && m_t2 > 110. && m_t2 < 120.)
      countSignalEvent("SR4");
    if( jetsel == 1 && m_t2 > 120. )
      countSignalEvent("SR6");
    if( jetsel == 2 && m_t2 > 120. )
      countSignalEvent("SR7");
      }

  

    // H160 Signalregion
    
    int nbJets = 0;
    std::vector<Jet*> bjets;
    for (unsigned i=0; i < jets.size(); i++) {
      if ( checkBTag(jets[i]) ) {
    nbJets += 1;
    bjets.push_back(jets[i]);
      }
      
    }
    if ( nbJets == 2 ){
      countCutflowEvent("DFH160"+cf_index[8]);
      double pa_b[3] = { bjets[0]->P4().M(), bjets[0]->P4().Px(), bjets[0]->P4().Py() };
      double pb_b[3] = { bjets[1]->P4().M(), bjets[1]->P4().Px(), bjets[1]->P4().Py() };
      double pmiss_b[3] = {0., missingET->P4().Px() + muonsCombined[0]->P4().Px() + electronsTight[0]->P4().Px(), missingET->P4().Py() + muonsCombined[0]->P4().Py() + electronsTight[0]->P4().Py() };
      mt2_bisect::mt2 mt2b_event;
      mt2b_event.set_momenta( pa_b, pb_b, pmiss_b );
      mt2b_event.set_mn( 0. );
      double m_t2b = mt2b_event.get_mt2();
      
      bool mt2b_cut = false;
      if( m_t2b > 160. ){
    countCutflowEvent("DFH160"+cf_index[9]);
    mt2b_cut = true;
      }
      
      double pa[3] = { muonsCombined[0]->P4().M(), muonsCombined[0]->P4().Px(), muonsCombined[0]->P4().Py() };
      double pb[3] = { electronsTight[0]->P4().M(), electronsTight[0]->P4().Px(), electronsTight[0]->P4().Py() };
      double pmiss[3] = {0., missingET->P4().Px(), missingET->P4().Py() };
      mt2_bisect::mt2 mt2_event;
      mt2_event.set_momenta( pa, pb, pmiss );
      mt2_event.set_mn( 0. );
      double m_t2 = mt2_event.get_mt2();
      bool mt2_cut = false;
      if( mt2b_cut && m_t2 < 90. ){
    countCutflowEvent("DFH160"+cf_index[10]);
      mt2_cut = true;
      }

      if( mt2_cut && leadingleptonpt < 60. ){
      countCutflowEvent("DFH160"+cf_index[11]);
      countSignalEvent("H160");
      }
    }

  }  



  

  else
    return;

}

void Atlas_1403_4853::finalize() {
  // Whatever should be done after the run goes here
}       
