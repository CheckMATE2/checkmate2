#include "atlas_1404_2500.h"

std::string Atlas_1404_2500::cf_index[10] = {"0","1","2","3","4","5","6","7","8","9"};

void Atlas_1404_2500::initialize() {
  setAnalysisName("atlas_1404_2500");          
  setInformation(""
    "@#Search for supersymmetry at \\sqrt{s}=8 TeV in final states with jets and two same-sign leptons or three leptons with the ATLAS detector\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SR3b;SR0b;SR1b;SR3Llow;SR3Lhigh;");
  for(int i=0; i<10; i++)
    bookCutflowRegions("SR3b"+cf_index[i]+";SR0b"+cf_index[i]+";SR1b"+cf_index[i]+";SR3Llow"+cf_index[i]+";SR3Lhigh"+cf_index[i]);

  // You should initialize any declared variables here
  trigger_single_el_pt = 60.;
  trigger_single_mu_pt = 36.;
  trigger_di_el_pt = 12.;
  trigger_di_mu_pt = 8.;
  trigger_ETmiss = 80.;
}

void Atlas_1404_2500::analyze() {
  // Your eventwise analysis code goes here

  missingET->addMuons(muonsCombined);
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 15., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);

  //object removal
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  electronsMedium = overlapRemoval(electronsMedium, muonsCombined, 0.1);
  electronsTight = overlapRemoval(electronsTight, muonsCombined, 0.1);

  electronsMedium = filterIsolation(electronsMedium,0);
  electronsTight = filterIsolation(electronsTight,0);
  muonsCombined = filterIsolation(muonsCombined,0);

  electronsMedium = filterIsolation(electronsMedium,1);
  electronsTight = filterIsolation(electronsTight,1);
  muonsCombined = filterIsolation(muonsCombined,1);

  electronsMedium = filterIsolation(electronsMedium,2);
  electronsTight = filterIsolation(electronsTight,2);
  muonsCombined = filterIsolation(muonsCombined,2);

  electronsMedium = filterIsolation(electronsMedium,3);
  electronsTight = filterIsolation(electronsTight,3);
  muonsCombined = filterIsolation(muonsCombined,3);

  
  //number of bjets

  nbjets=0;
  for ( int j = 0; j < jets.size(); j++ ) {
    if ( jets[j]->PT > 20. && checkBTag( jets[j] ) && fabs( jets[j]->Eta ) < 2.5 )
      nbjets++;
  }      


  //signal jets
  jets = filterPhaseSpace(jets, 40., -2.8, 2.8);
  

  //trigger cuts

  trigger=false;

  if( missingET->P4().Et() > trigger_ETmiss )
    trigger=true;
  if( electronsTight.size() > 0 && electronsTight[0]->PT > trigger_single_el_pt )
    trigger=true;
  if( muonsCombined.size() > 0 && muonsCombined[0]->PT > trigger_single_mu_pt )
    trigger=true;
  if( electronsTight.size() > 1 && electronsTight[0]->PT > trigger_di_el_pt && electronsTight[1]->PT > trigger_di_el_pt )
    trigger=true;
  if( muonsCombined.size() > 1 && muonsCombined[0]->PT > trigger_di_mu_pt && muonsCombined[1]->PT > trigger_di_mu_pt )
    trigger=true;
  if( electronsTight.size() > 0 && muonsCombined.size() > 0 && electronsTight[0]->PT > trigger_di_el_pt && muonsCombined[0]->PT > trigger_di_mu_pt )
    trigger=true;
  

  //create a vector of leptons
  leptons.clear();
  for( int i=0; i< electronsTight.size(); i++ )
    leptons.push_back( electronsTight[i]->P4() );
  for( int i=0; i< muonsCombined.size(); i++ )
    leptons.push_back( muonsCombined[i]->P4() );


  //sort leptons with PT
  int flag = 1;    
  TLorentzVector temp;             
  for( int i = 1; ( i <= leptons.size() ) && flag; i++ ){
      flag = 0;
      for ( int j=0; j < ( leptons.size()-1 ); j++ ){
    if (leptons[j+1].Pt() > leptons[j].Pt() ){ 
      temp = leptons[j];             
      leptons[j] = leptons[j+1];
      leptons[j+1] = temp;
      flag = 1;               
    }
      }
  }


  countCutflowEvent("SR3b"+cf_index[0]);
  countCutflowEvent("SR1b"+cf_index[0]);
  countCutflowEvent("SR0b"+cf_index[0]);
  countCutflowEvent("SR3Llow"+cf_index[0]);
  countCutflowEvent("SR3Lhigh"+cf_index[0]);


  bool two_signal_leptons=false;
  if( leptons.size() > 1 && leptons[0].Pt() > 20. && leptons[1].Pt() > 15. ){
    if( leptons.size() > 2 && leptons[2].Pt() < 15. )
      two_signal_leptons=true;
    else if ( leptons.size() > 2 && leptons[2].Pt() > 15. )
      two_signal_leptons=false;
    else if( leptons.size() < 3)
      two_signal_leptons=true;
  }

  bool three_signal_leptons=false;
  if( leptons.size() > 2 && leptons[0].Pt() > 20. && leptons[1].Pt() > 15. && leptons[2].Pt() > 15. ){
    three_signal_leptons=true;
  }
  
  if( two_signal_leptons || three_signal_leptons ){
    countCutflowEvent("SR3b"+cf_index[1]);
    countCutflowEvent("SR1b"+cf_index[1]);
    countCutflowEvent("SR0b"+cf_index[1]);
    countCutflowEvent("SR3Llow"+cf_index[1]);
    countCutflowEvent("SR3Lhigh"+cf_index[1]);
  }
    
  
  bool trigger_cut=false;
  if( ( three_signal_leptons || two_signal_leptons ) && trigger ){
    trigger_cut=true;
    countCutflowEvent("SR3b"+cf_index[2]);
    countCutflowEvent("SR1b"+cf_index[2]);
    countCutflowEvent("SR0b"+cf_index[2]);
    countCutflowEvent("SR3Llow"+cf_index[2]);
    countCutflowEvent("SR3Lhigh"+cf_index[2]);
  }
    

  
  mll=0.;
  sign=0.;
  double mlltemp;

  if( electronsTight.size() >= 2 ){
    mll=(electronsTight[0]->P4()+electronsTight[1]->P4()).M();
    sign=electronsTight[0]->Charge*electronsTight[1]->Charge;
  }
  else if( muonsCombined.size() >= 2 ){
    mlltemp=( muonsCombined[0]->P4()+muonsCombined[1]->P4() ).M();
    if( mlltemp > mll )
      mll = mlltemp;
    sign=muonsCombined[0]->Charge*muonsCombined[1]->Charge;
  }

  else if( electronsTight.size() >= 1 && muonsCombined.size() >= 1){
    mlltemp = ( muonsCombined[0]->P4()+electronsTight[0]->P4() ).M();
    if( mlltemp > mll )
      mll = mlltemp;
    sign=muonsCombined[0]->Charge*electronsTight[0]->Charge;
  }


  bool mll_cut=false;
  if( trigger_cut && mll > 12. ) {
    mll_cut=true;
    countCutflowEvent("SR3b"+cf_index[3]);
    countCutflowEvent("SR1b"+cf_index[3]);
    countCutflowEvent("SR0b"+cf_index[3]);
    countCutflowEvent("SR3Llow"+cf_index[3]);
    countCutflowEvent("SR3Lhigh"+cf_index[3]);
  }



  //  if( two_signal_leptons && mll_cut && electronsTight.size() + muonsCombined.size() >= 2 ){
  if( ( two_signal_leptons || three_signal_leptons ) && mll_cut ){

    bool SS_cut=false;
    if( ( mll_cut && sign > 0 ) || three_signal_leptons ){
      SS_cut=true;
      countCutflowEvent("SR3b"+cf_index[4]);
    }

    //calculation of mT and meff

    deltaphi = fabs( leptons[0].DeltaPhi(missingET->P4()) );
    mT = sqrt( 2.*leptons[0].Pt()*missingET->P4().Et()*(1.-cos(deltaphi)) );
    
    meff=missingET->P4().Et();
    for( int i=0; i<jets.size(); i++ ){
      meff=meff+jets[i]->PT;
    }
    for( int i=0; i<leptons.size(); i++ ){
      if( leptons[i].Pt() > 15. )
    meff=meff+leptons[i].Pt();
    }


    //SR3b
    if( SS_cut && jets.size() >= 5 ){
      
      countCutflowEvent("SR3b"+cf_index[5]);
      
      bool bjets_cut=false;
      if( nbjets >= 3 ){
    bjets_cut=true;
    countCutflowEvent("SR3b"+cf_index[6]);
      }

      if( bjets_cut && meff > 350. ){
    countCutflowEvent("SR3b"+cf_index[7]);    
    countSignalEvent("SR3b");
      }
    }
  }

  //  if( two_signal_leptons && mll_cut && electronsTight.size() + muonsCombined.size() == 2 ){
  if( two_signal_leptons && mll_cut ){

    bool SS_cut=false;
    if( mll_cut && sign > 0 ){
      SS_cut=true;
      countCutflowEvent("SR1b"+cf_index[4]);
      countCutflowEvent("SR0b"+cf_index[4]);
    }

    //calculation of mT and meff

    deltaphi = fabs( leptons[0].DeltaPhi(missingET->P4()) );
    mT = sqrt( 2.*leptons[0].Pt()*missingET->P4().Et()*(1.-cos(deltaphi)) );
    
    meff=leptons[0].Pt()+leptons[1].Pt()+missingET->P4().Et();
    for( int i=0; i<jets.size(); i++ ){
      meff=meff+jets[i]->PT;
    }


    //SR1b,SR0b
    if( SS_cut && jets.size() >= 3 ){
      countCutflowEvent("SR1b"+cf_index[5]);
      countCutflowEvent("SR0b"+cf_index[5]);    


      if( nbjets >= 1 ){
    countCutflowEvent("SR1b"+cf_index[6]);
    bool met_cut=false;
    if( missingET->P4().Et() > 150. ){
      met_cut=true;
      countCutflowEvent("SR1b"+cf_index[7]);
    }
    
    bool mT_cut=false;
    if( met_cut && mT > 100. ){
      mT_cut=true;
      countCutflowEvent("SR1b"+cf_index[8]);
    }

    //SR1b
    if( mT_cut && meff > 700. ){
      countCutflowEvent("SR1b"+cf_index[9]);
      countSignalEvent("SR1b");      
    }
      }
    
      else if( nbjets == 0 ){
    countCutflowEvent("SR0b"+cf_index[6]);    
    bool met_cut=false;
    if( missingET->P4().Et() > 150. ){
      met_cut=true;
      countCutflowEvent("SR0b"+cf_index[7]);
    }
    
    bool mT_cut=false;
    if( met_cut && mT > 100. ){
      mT_cut=true;
      countCutflowEvent("SR0b"+cf_index[8]);
    }
    
    //SR0b
    if( mT_cut && meff > 400. ){
      countCutflowEvent("SR0b"+cf_index[9]);
      countSignalEvent("SR0b");      
    }
    
      }
    }
    
  }

  //  if( three_signal_leptons && mll_cut && electronsTight.size() + muonsCombined.size() >= 3 ){e

  if( three_signal_leptons && mll_cut ){

    countCutflowEvent("SR3Llow"+cf_index[4]);
    countCutflowEvent("SR3Lhigh"+cf_index[4]);



   if( jets.size() >= 4 ){
     countCutflowEvent("SR3Llow"+cf_index[5]);
     countCutflowEvent("SR3Lhigh"+cf_index[5]);     

     bool zveto=false;
     if( mll < 84. || mll > 98. ){
       zveto=true;
       countCutflowEvent("SR3Llow"+cf_index[6]);
     }

     bool met_cut=false;
     if( zveto && missingET->P4().Et() > 50 && missingET->P4().Et() < 150. ){
       met_cut=true;
       countCutflowEvent("SR3Llow"+cf_index[7]);
     }
     
     meff=0.;
     for( int i=0; i<leptons.size(); i++ ){
       if( leptons[i].Pt() > 15. )
     meff=meff+leptons[i].Pt();
     }

     meff=meff+missingET->P4().Et();
     for( int i=0; i<jets.size(); i++){
       meff=meff+jets[i]->PT;
     }

     //     if( met_cut && meff > 400. && ( jets.size() < 5 && nbjets < 3) ){
     if( met_cut && meff > 400.  ){
    countCutflowEvent("SR3Llow"+cf_index[8]);
    countSignalEvent("SR3Llow");
      }

      met_cut=false;
      if( missingET->P4().Et() > 150. ){
    met_cut=true;     
    countCutflowEvent("SR3Lhigh"+cf_index[6]);     
      }

      //      if( met_cut && meff > 400. &&  ( jets.size() < 5 && nbjets < 3) ){
      if( met_cut && meff > 400.  ){
    countCutflowEvent("SR3Lhigh"+cf_index[7]);
    countSignalEvent("SR3Lhigh");
      }
      


     
   }
    

    
  }
  else
    return;
  
}

void Atlas_1404_2500::finalize() {
  // Whatever should be done after the run goes here
}       
