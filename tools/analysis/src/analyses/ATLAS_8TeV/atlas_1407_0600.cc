#include "atlas_1407_0600.h"

std::string Atlas_1407_0600::cf_index[12] = {"0","1","2","3","4","5","6","7","8","9","10","11"};

void Atlas_1407_0600::initialize() {
  setAnalysisName("atlas_1407_0600");          
  setInformation(""
    "@#Search for strong production of supersymmetric particles in\n"
     "@#final states with missing transverse momentum and at least three b-jets at sqrt{s}=8 TeV\n"
     "@#proton-proton collisions with the ATLAS detector\n"
  "");
  setLuminosity(20.1*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SR0l4jA;SR0l4jB;SR0l4jC;SR0l7jA;SR0l7jB;SR0l7jC;SR1l6jA;SR1l6jB;SR1l6jC;");
  // You should initialize any declared variables here
  for(int i=0; i<11; i++)
    bookCutflowRegions("SR0l4jA"+cf_index[i]+";SR0l4jB"+cf_index[i]+";SR0l4jC"+cf_index[i]+";SR0l7jA"+cf_index[i]+";SR0l7jB"+cf_index[i]+";SR0l7jC"+cf_index[i]+";SR1l6jA"+cf_index[i]+";SR1l6jB"+cf_index[i]+";SR1l6jC"+cf_index[i]);

}

void Atlas_1407_0600::analyze() {
  // Your eventwise analysis code goes here
  missingET->addMuons(muonsCombined);
  electronsMedium = filterPhaseSpace(electronsMedium, 20., -2.47, 2.47);
  electronsTight = filterPhaseSpace(electronsTight, 20., -2.47, 2.47);           
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5);         
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);


  //object removal
  jets = overlapRemoval(jets, electronsTight, 0.2);
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
         
  //Isolation Criterium
  electronsTight = filterIsolation(electronsTight,0);
  electronsTight = filterIsolation(electronsTight,1);
  muonsCombined = filterIsolation(muonsCombined,0);
  muonsCombined = filterIsolation(muonsCombined,1);

  int nJets = jets.size();

  //Trigger cuts
  bool trigger_flag = false;
  bool JetTrigger = false;
  for( int i=0; i< nJets; i++ ){
    if( fabs( jets[i]->Eta ) < 2.8 && jets[i]->PT >90. )
      JetTrigger = true;
  }

  if( JetTrigger && missingET->P4().Et() > 150 )
    trigger_flag=true;
    

  //Preselection Cuts
  //Demand four jets with pT>30 GeV
  bool fourjets_flag = false;
  if( trigger_flag && nJets > 3 && jets[3]->PT > 30. ){
    countCutflowEvent("SR0l4jA"+cf_index[0]);
    countCutflowEvent("SR0l4jB"+cf_index[0]);
    countCutflowEvent("SR0l4jC"+cf_index[0]);
    countCutflowEvent("SR0l7jA"+cf_index[0]);
    countCutflowEvent("SR0l7jB"+cf_index[0]);
    countCutflowEvent("SR0l7jC"+cf_index[0]);
    countCutflowEvent("SR1l6jA"+cf_index[0]);
    countCutflowEvent("SR1l6jB"+cf_index[0]);
    countCutflowEvent("SR1l6jC"+cf_index[0]);
    fourjets_flag = true;
  }
  //leading jet pT > 90 GeV
  bool leadingjet_flag = false;
  if( fourjets_flag && jets[0]->PT > 90. ){
    leadingjet_flag = true;
    countCutflowEvent("SR0l4jA"+cf_index[1]);
    countCutflowEvent("SR0l4jB"+cf_index[1]);
    countCutflowEvent("SR0l4jC"+cf_index[1]);
    countCutflowEvent("SR0l7jA"+cf_index[1]);
    countCutflowEvent("SR0l7jB"+cf_index[1]);
    countCutflowEvent("SR0l7jC"+cf_index[1]);
    countCutflowEvent("SR1l6jA"+cf_index[1]);
    countCutflowEvent("SR1l6jB"+cf_index[1]);
    countCutflowEvent("SR1l6jC"+cf_index[1]);
  }

  //MET cut
  bool MET_flag = false;
  if( leadingjet_flag &&  missingET->P4().Et() > 150. ){
    MET_flag = true;
    countCutflowEvent("SR0l4jA"+cf_index[2]);
    countCutflowEvent("SR0l4jB"+cf_index[2]);
    countCutflowEvent("SR0l4jC"+cf_index[2]);
    countCutflowEvent("SR0l7jA"+cf_index[2]);
    countCutflowEvent("SR0l7jB"+cf_index[2]);
    countCutflowEvent("SR0l7jC"+cf_index[2]);
    countCutflowEvent("SR1l6jA"+cf_index[2]);
    countCutflowEvent("SR1l6jB"+cf_index[2]);
    countCutflowEvent("SR1l6jC"+cf_index[2]);
  }


  int nbjets = 0;
  for ( int j = 0; j < jets.size(); j++ ) {
    if ( jets[j]->PT > 30. && checkBTag( jets[j] ) )
      //&& fabs( jets[j]->Eta ) < 2.5 )
      nbjets++;
  }   

  bool nbjets_flag = false;
  if( MET_flag && nbjets == 3 ){
    nbjets_flag = true;
    countCutflowEvent("SR0l4jA"+cf_index[3]);
    countCutflowEvent("SR0l4jB"+cf_index[3]);
    countCutflowEvent("SR0l4jC"+cf_index[3]);
    countCutflowEvent("SR0l7jA"+cf_index[3]);
    countCutflowEvent("SR0l7jB"+cf_index[3]);
    countCutflowEvent("SR0l7jC"+cf_index[3]);
    countCutflowEvent("SR1l6jA"+cf_index[3]);
    countCutflowEvent("SR1l6jB"+cf_index[3]);
    countCutflowEvent("SR1l6jC"+cf_index[3]);
  }

  //0 lepton preselection
  
  //lepton veto

  if( nbjets_flag && electronsMedium.size() + muonsCombined.size() == 0 ){

    countCutflowEvent("SR0l4jA"+cf_index[4]);
    countCutflowEvent("SR0l4jB"+cf_index[4]);
    countCutflowEvent("SR0l4jC"+cf_index[4]);
    countCutflowEvent("SR0l7jA"+cf_index[4]);
    countCutflowEvent("SR0l7jB"+cf_index[4]);
    countCutflowEvent("SR0l7jC"+cf_index[4]);


    //deltaPhijet cut
    double deltaphi_temp;
    DeltaPhi4jmin = 3.14;
    for( int i=0; i < 4; i++ ){
      deltaphi_temp = fabs( jets[i]->P4().DeltaPhi(missingET->P4()) );
       if ( deltaphi_temp <  DeltaPhi4jmin )
     DeltaPhi4jmin = deltaphi_temp;
    }
    if( DeltaPhi4jmin < 0.5 ){
      countCutflowEvent("SR0l4jA"+cf_index[5]);
      countCutflowEvent("SR0l4jB"+cf_index[5]);
      countCutflowEvent("SR0l4jC"+cf_index[5]);
      countCutflowEvent("SR0l7jA"+cf_index[5]);
      countCutflowEvent("SR0l7jB"+cf_index[5]);
      countCutflowEvent("SR0l7jC"+cf_index[5]);
      return;
    }
    
    meff4j = missingET->P4().Et();
    for( int i=0; i < 4; i++)
      meff4j = meff4j + jets[i]->PT;
    
    if( missingET->P4().Et() / meff4j < 0.2 )
      return;

    countCutflowEvent("SR0l4jA"+cf_index[6]);
    countCutflowEvent("SR0l4jB"+cf_index[6]);
    countCutflowEvent("SR0l4jC"+cf_index[6]);
    countCutflowEvent("SR0l7jA"+cf_index[6]);
    countCutflowEvent("SR0l7jB"+cf_index[6]);
    countCutflowEvent("SR0l7jC"+cf_index[6]);


    //Signal regions

    if( nJets > 6 && jets[6]->PT > 30.  ){


      countCutflowEvent("SR0l7jA"+cf_index[7]);
      countCutflowEvent("SR0l7jB"+cf_index[7]);
      countCutflowEvent("SR0l7jC"+cf_index[7]);


      meffincl = missingET->P4().Et();
      for( int i=0; i < nJets; i++)
    if( jets[0]->PT > 30. )
      meffincl = meffincl + jets[i]->PT;
      
      //SR-0l-7j-A
      if( missingET->P4().Et() > 200. ){
    countCutflowEvent("SR0l7jA"+cf_index[8]);
    if( meffincl > 1000. ){
      countCutflowEvent("SR0l7jA"+cf_index[9]);
      countSignalEvent("SR0l7jA");
    }
      }
      
      //SR-0l-7j-B
      if( missingET->P4().Et() > 350. ){
    countCutflowEvent("SR0l7jB"+cf_index[8]);
    if( meffincl > 1000. ){
      countCutflowEvent("SR0l7jB"+cf_index[9]);
      countSignalEvent("SR0l7jB");
    }
      }
      
      //SR-0l-7j-C
      if( missingET->P4().Et() > 250. ){
    countCutflowEvent("SR0l7jC"+cf_index[8]);
    if( meffincl > 1500. ){
      countCutflowEvent("SR0l7jC"+cf_index[9]);
      countSignalEvent("SR0l7jC");
    }
      }
    }


      

    if( nJets > 3 && jets[3]->PT > 50.  ){
      countCutflowEvent("SR0l4jA"+cf_index[7]);
      countCutflowEvent("SR0l4jB"+cf_index[7]);

      meffincl = missingET->P4().Et();
      for( int i=0; i < nJets; i++)
    if( jets[0]->PT > 30. )
      meffincl = meffincl + jets[i]->PT;
      
      //SR-0l-4j-A
      if( missingET->P4().Et() > 250. ){
    countCutflowEvent("SR0l4jA"+cf_index[8]);
    if( meff4j > 1300. ){
      countCutflowEvent("SR0l4jA"+cf_index[9]);
      countSignalEvent("SR0l4jA");
    }
      }
      
      //SR-0l-4j-B
      if( missingET->P4().Et() > 350. ){
    countCutflowEvent("SR0l4jB"+cf_index[8]);
    if( meff4j > 1100. ){
      countCutflowEvent("SR0l4jB"+cf_index[9]);
      countSignalEvent("SR0l4jB");
    }
      }
    }

    if( nJets > 3 && jets[3]->PT > 30.  ){
      HT4j = 0.;
      for( int i=0; i<4; i++ ){
    HT4j = HT4j+jets[i]->PT;
      }
      countCutflowEvent("SR0l4jC"+cf_index[7]);
      //SR-0l-4j-C
      if(jets[0]->PT > 30. && checkBTag( jets[0] ) != 1 ){
    countCutflowEvent("SR0l4jC"+cf_index[8]);
    if( missingET->P4().Et() > 400. ){
      countCutflowEvent("SR0l4jC"+cf_index[9]);
      if( meff4j > 1000. ){
        countCutflowEvent("SR0l4jC"+cf_index[10]);
        if( (missingET->P4().Et() / sqrt(HT4j)) > 16. ){
          countCutflowEvent("SR0l4jC"+cf_index[11]);
          countSignalEvent("SR0l4jC");
        }
      }
    }
      }
    }
    



  }

  //1 lepton preselection
  
  //lepton veto

  if( nbjets_flag && electronsTight.size() + muonsCombined.size() >= 1 ){
    
    if( electronsTight.size() == 0 &&  muonsCombined[0]->PT < 25.  ){
      return;
    }
    else if( muonsCombined.size() == 0 &&  electronsTight[0]->PT < 25.  ){
      return;
    }    
    else if( electronsTight.size() > 0 && muonsCombined.size() > 0 && muonsCombined[0]->PT < 25. && electronsTight[0]->PT < 25.) { return;}
    
    countCutflowEvent("SR1l6jA"+cf_index[4]);
    countCutflowEvent("SR1l6jB"+cf_index[4]);
    countCutflowEvent("SR1l6jC"+cf_index[4]);
    
    if( nJets > 5 && jets[5]->PT > 30. ){

      countCutflowEvent("SR1l6jA"+cf_index[5]);
      countCutflowEvent("SR1l6jB"+cf_index[5]);
      countCutflowEvent("SR1l6jC"+cf_index[5]);
    
      //Signal regions

      double deltaphi;
      if( (electronsTight.size() > 0) && (muonsCombined.size() == 0 )){
        deltaphi = fabs( electronsTight[0]->P4().DeltaPhi(missingET->P4()) );
        mT = sqrt( 2.*electronsTight[0]->P4().Pt()*missingET->P4().Et()*(1.-cos(deltaphi)) );
      }
      else if( (electronsTight.size() == 0) && (muonsCombined.size() > 0 )){
        deltaphi = fabs( muonsCombined[0]->P4().DeltaPhi(missingET->P4()) );
        mT = sqrt( 2.*muonsCombined[0]->P4().Pt()*missingET->P4().Et()*(1.-cos(deltaphi)) );
      }
      else if (electronsTight[0]->PT > muonsCombined[0]->PT ){
        deltaphi = fabs( electronsTight[0]->P4().DeltaPhi(missingET->P4()) );
        mT = sqrt( 2.*electronsTight[0]->P4().Pt()*missingET->P4().Et()*(1.-cos(deltaphi)) );
      }
      else{
        deltaphi = fabs( muonsCombined[0]->P4().DeltaPhi(missingET->P4()) );
        mT = sqrt( 2.*muonsCombined[0]->P4().Pt()*missingET->P4().Et()*(1.-cos(deltaphi)) );
      }
    
    //SR-1l-6j-A
      
    if( missingET->P4().Et() > 175. ){
      countCutflowEvent("SR1l6jA"+cf_index[6]);
    if( mT > 140. ){
      countCutflowEvent("SR1l6jA"+cf_index[7]);
      if( meffincl > 700. ){
        countCutflowEvent("SR1l6jA"+cf_index[8]);
        countSignalEvent("SR1l6jA");
      }
    }
  }

  //SR-1l-6j-B

  if( missingET->P4().Et() > 225. ){
    countCutflowEvent("SR1l6jB"+cf_index[6]);
    if( mT > 140. ){
      countCutflowEvent("SR1l6jB"+cf_index[7]);
      if( meffincl > 800. ){
        countCutflowEvent("SR1l6jB"+cf_index[8]);
        countSignalEvent("SR1l6jB");
      }
    }
  }

  //SR-1l-6j-C

  if( missingET->P4().Et() > 175. ){
    countCutflowEvent("SR1l6jC"+cf_index[6]);
    if( mT > 160. ){
      countCutflowEvent("SR1l6jC"+cf_index[7]);
      if( meffincl > 900. ){
        countCutflowEvent("SR1l6jC"+cf_index[8]);
        countSignalEvent("SR1l6jC");
      }
    }
  }


}

    
    
}
  
    

}

void Atlas_1407_0600::finalize() {
  // Whatever should be done after the run goes here
}       
