#include "atlas_conf_2013_036.h"

std::string Atlas_conf_2013_036::id[5] = { "0", "1", "2", "3", "4"};
void Atlas_conf_2013_036::initialize() {
  setInformation(""
  "@#ATLAS\n"
  "@#ATLAS-CONF-2013-036\n"
  "@#>=4 leptons + etmiss\n"
  "@#sqrt(s) = 8 TeV\n"
  "@#int(L) = 20.7 fb^-1\n"
  "");
  setLuminosity(20.7*units::INVFB);      
  setAnalysisName("atlas_conf_2013_036");          
  ignore("towers");
  ignore("tracks");
  
  bookSignalRegions("SR0noZa;SR0noZb;SR1noZ;SR0Z;SR1Z;");
  bookCutflowRegions("0;1;2");;
  for (int i=0; i<5; i++)
    bookCutflowRegions("SR0noZa"+id[i]+";SR0noZb"+id[i]+";SR1noZ"+id[i]+";SR0Z"+id[i]+";SR1Z"+id[i]);
  
  trigger = false;
  meff = 0.;
  //h = new TH1F("","",100,0,100);
}

void Atlas_conf_2013_036::analyze() {
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 20., -2.5, 2.5);
  countCutflowEvent("0");
  
  //Remove jets that overlap with electrons
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  //  jets = overlapRemoval(jets, electronsTight, 0.2);

  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  electronsMedium = overlapRemoval(electronsMedium, muonsCombined, 0.1);
  electronsMedium = overlapRemoval(electronsMedium, 0.1);

  electronsTight = overlapRemoval(electronsTight, jets, 0.4);
  electronsTight = overlapRemoval(electronsTight, muonsCombined, 0.1);
  electronsTight = overlapRemoval(electronsTight, 0.1);

  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, electronsTight, 0.05);
  muonsCombined = overlapRemoval(muonsCombined, 0.05);  
  muonsCombined = filterIsolation(muonsCombined,0);

  electronsMedium = filterIsolation(electronsMedium,0);
  electronsMedium = filterIsolation(electronsMedium,1);


  electronsTight = filterIsolation(electronsTight,0);
  electronsTight = filterIsolation(electronsTight,1);

  missingET->addMuons(muonsCombined);

  int nloosetau =0;
  int nmediumtau = 0;  
  for (int j = 0; j < jets.size(); j++) {
    if (checkTauTag(jets[j],"loose"))
      nloosetau++;
    if (checkTauTag(jets[j],"medium"))
      nmediumtau++;
  }
  
  int nlightleptons;
  nlightleptons=electronsMedium.size()+muonsCombined.size();

  if ( ( nlightleptons + nloosetau < 4 )  )
    return;  

  countCutflowEvent("SR0Z"+id[1]);
  countCutflowEvent("SR0noZa"+id[1]);
  countCutflowEvent("SR0noZb"+id[1]);
  countCutflowEvent("SR1noZ"+id[1]);


  nlightleptons=electronsTight.size()+muonsCombined.size();


  int nSFOS=0;

  if ( ( nlightleptons + nmediumtau < 4 )  )
  //  if ( ( nlightleptons != 4 )  )
    return;  

  //  if( nmediumtau == 0 & nlightleptons >= 4){ //signal region lepton multiplicity

  std::cout << nmediumtau + nlightleptons<< std::endl;

  
  // No trigger information are given in the paper
  trigger = true;
  
  if ( !trigger )
    return;
  
  countCutflowEvent("SR0Z"+id[2]);
  countCutflowEvent("SR0noZa"+id[2]);
  countCutflowEvent("SR0noZb"+id[2]);
  countCutflowEvent("SR1noZ"+id[2]);
  
  // Low mass resonance veto
  bool veto;
  veto = false;
  int z_candidate = 0;
  int extended_z_veto = 0;
  
  std::cout << "noveto: " << veto << std::endl;

  for (int e = 0; e < electronsTight.size(); e++) {
    for (int e2 = e+1; e2 < electronsTight.size(); e2++) {      
      if (electronsTight[e]->Charge*electronsTight[e2]->Charge > 0) continue;

      TLorentzVector pair = ( electronsTight[e]->P4() + electronsTight[e2]->P4() );
      double mll = pair.M();
      //      h->Fill(mll,weight);
      if (mll < 12.) 
    //continue;
      {
        veto = true;
    break;
      }
      if ( fabs( mll-91.2) < 10.){
    z_candidate++;
    extended_z_veto++;
      }
      for(int e3 = 0; e3 < electronsTight.size(); e3++ ){
    if (e3 == e || e3 == e2 ) continue;
    mll = ( pair +  electronsTight[e3]->P4() ).M();
    if( fabs( mll-91.2 ) < 10. ){
      extended_z_veto++;
    }
    
      }
      
      for(int m = 0; m < muonsCombined.size(); m++ ){
    mll = ( pair + muonsCombined[m]->P4() ).M();
    if ( fabs( mll-91.2 ) < 10. ){
      extended_z_veto++;
    }
      }

      for( int e3 = 0; e3 < electronsTight.size(); e3++ ){
    for( int e4 = e3+1; e4 < electronsTight.size(); e4++ ){
      if( e3 == e || e3 == e2 || e4 == e || e4 == e2 ) continue;
      if(electronsTight[e3]->Charge*electronsTight[e4]->Charge > 0 ) continue;
      mll = ( pair + electronsTight[e3]->P4() + electronsTight[e4]->P4() ).M();
      //      if( mll < 12. ) continue;
      if( fabs( mll-91.2 ) < 10. ){
        extended_z_veto++;
      }
    }
      }
    
      for( int m = 0; m <muonsCombined.size(); m++ ){
    for ( int m2 = m+1; m2 < muonsCombined.size(); m2 ++ ){
      if(muonsCombined[m]->Charge*muonsCombined[m2]->Charge > 0 ) continue;
      nSFOS++;
      mll = ( pair + muonsCombined[m]->P4() + muonsCombined[m2]->P4() ).M();
      //      if( mll < 12. ) continue;
      if( fabs( mll-91.2 ) < 10. ){
        extended_z_veto++;
      }
    }
      }
    }
  }  

  std::cout << "muons: " << muonsCombined.size() << std::endl;

  for (int m = 0; m < muonsCombined.size(); m++) {
    for (int m2 = m+1; m2 < muonsCombined.size(); m2++) {
      if (muonsCombined[m]->Charge*muonsCombined[m2]->Charge > 0) continue;

      TLorentzVector pair = ( muonsCombined[m]->P4() + muonsCombined[m2]->P4() );
      double mll = pair.M();
      //      h->Fill(mll,weight);

      if (mll < 12.) 
    //continue;
    {
      veto = true;
      break;
    }
      if ( fabs( mll-91.2 ) < 10.){
    z_candidate++;
    extended_z_veto++;
      }

      for(int e = 0; e < electronsTight.size(); e++ ){

    mll = ( pair +  electronsTight[e]->P4() ).M();
    if( fabs( mll-91.2 ) < 10. ){
      extended_z_veto++;
    }
    
      }
      
      for(int m3 = 0; m3 < muonsCombined.size(); m3++ ){
    if (m3 == m || m3 == m2 ) continue;
    mll = ( pair + muonsCombined[m3]->P4() ).M();
    if ( fabs( mll-91.2 ) < 10. ){
      extended_z_veto++;
    }
      }

      for( int e = 0; e < electronsTight.size(); e++ ){
    for( int e2 = e+1; e2 < electronsTight.size(); e2++ ){
      if(electronsTight[e]->Charge*electronsTight[e2]->Charge > 0 ) continue;
      mll = ( pair + electronsTight[e]->P4() + electronsTight[e2]->P4() ).M();
      //      if ( mll < 12. ) continue;
      if( fabs( mll-91.2 ) < 10. ){
        extended_z_veto++;
      }
    }
      }
    
      for( int m3 = 0; m3 <muonsCombined.size(); m3++ ){
    for ( int m4 = m3+1; m4 < muonsCombined.size(); m4 ++ ){
      if( m3 == m || m3 == m2 || m4 == m || m4 == m2 ) continue;
      if(muonsCombined[m3]->Charge*muonsCombined[m4]->Charge > 0 ) continue;
      mll = ( pair + muonsCombined[m3]->P4() + muonsCombined[m4]->P4() ).M();
      //      if (mll < 12. ) continue;
      if( fabs( mll-91.2 ) < 10. ){
        extended_z_veto++;
      }
    }
      }

      
    
    }
  }  

  std::cout << "veto: " << veto << std::endl;

  if ( veto )
    return;
  

  
  // Calculation of meff
  for (int e = 0; e < electronsTight.size(); e++)
    meff += electronsTight[e]->PT;              
  for (int m = 0; m < muonsCombined.size(); m++)
    meff += muonsCombined[m]->PT;
  for (int j = 0; j < jets.size(); j++){
    if( ( checkTauTag(jets[j],"loose") == 0  && jets[j]->PT > 40.) || 
    ( checkTauTag(jets[j],"loose") == 1 ) ){
      meff += jets[j]->PT;
    }
  }
  
  meff += missingET->P4().Et();
  
  //Signal regions  
  if ( z_candidate > 0 ) {
    countCutflowEvent("SR0Z"+id[3]);
    countCutflowEvent("SR1Z"+id[3]);
    if ( missingET->P4().Et() > 75.) {
      countCutflowEvent("SR0Z"+id[4]);
      if ( nmediumtau >= 0 && nlightleptons >= 4) 
        countSignalEvent("SR0Z");
    }
    if ( missingET->P4().Et() > 100. ) {
      countCutflowEvent("SR1Z"+id[4]);
      if ( nmediumtau >= 1 && nlightleptons == 3) 
        countSignalEvent("SR1Z");
    }
  }
  
    std::cout << extended_z_veto << std::endl;

    if ( extended_z_veto == 0  ) {
    //    if ( 1 ) {
    countCutflowEvent("SR0noZa"+id[3]);
    countCutflowEvent("SR0noZb"+id[3]);
    countCutflowEvent("SR1noZ"+id[3]);
    if ( missingET->P4().Et() > 50. ) {
      countCutflowEvent("SR0noZa"+id[4]);
      if ( nmediumtau == 0 && nlightleptons >=4 ) 
        countSignalEvent("SR0noZa");
    }
    if ( missingET->P4().Et() > 75. || meff > 600. ) {
      countCutflowEvent("SR0noZb"+id[4]);
      if ( nmediumtau == 0  && nlightleptons >=4 ) 
        countSignalEvent("SR0noZb");
    }
    if ( missingET->P4().Et() > 100. || meff > 400.) {
      countCutflowEvent("SR1noZ"+id[4]);
      if ( nmediumtau >= 1 && nlightleptons == 3) 
        countSignalEvent("SR1noZ");
    }
  }

    //  }//signal region lepton multiplicity

}

void Atlas_conf_2013_036::finalize() {
  //TCanvas *c = new TCanvas();
  //h->Draw("");
  //c->Print("plot.pdf","pdf");
}       
