#include "atlas_conf_2013_037.h"

void Atlas_conf_2013_037::initialize() {
  setAnalysisName("atlas_conf_2013_037");          
  setInformation(""
    "@#ATLAS\n"
     "@#ATLAS-CONF-2013_037\n"
     "@#1 lepton, 4 jets, etmiss\n"
     "@#sqrt(s) = 8 TeV\n"
     "@#int(L) = 20.7 fb^-1\n"
  "");
  setLuminosity(20.7*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
//  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SRtN1_shape01;SRtN1_shape02;SRtN1_shape03;SRtN2;SRtN3;SRbC1;SRbC2;SRbC3;");
  // You should initialize any declared variables here
  bookCutflowRegions("a_SingleLepton;");
}

void Atlas_conf_2013_037::analyze() {
  // Your eventwise analysis code goes here
  missingET->addMuons(muonsCombined);
  
  jets = filterPhaseSpace(jets, 20., -4.7, 4.7);
  
  electronsTight = filterPhaseSpace(electronsTight, 25., -2.47, 2.47);
  std::vector<Electron*> isoElecs = filterIsolation(electronsTight);
  muonsCombined = filterPhaseSpace(muonsCombined, 25., -2.4, 2.4);
  std::vector<Muon*> isoMuons = filterIsolation(muonsCombined);
  
  //Remove jets that overlp electrons
  jets = overlapRemoval(jets, isoElecs, 0.2);
  
  //Remove electrons that overlap with jets
  isoElecs = overlapRemoval(isoElecs, jets, 0.4);
  
  // Find exactly one lepton and store in new 4-vector
  TLorentzVector lepton;
  
  if( (isoMuons.size()==1) && (isoElecs.size() == 0) )
    lepton = isoMuons[0]->P4();
  else if( (isoMuons.size()==0) && (isoElecs.size() == 1) ) 
    lepton = isoElecs[0]->P4();
  else
    return;
  countCutflowEvent("a_SingleLepton");
  
  //Find four jets
  if (jets.size() < 4)
    return;
  countCutflowEvent("b_FourJets");
  
  //Four jets must have pT > (80,60,40,25)
  if ( jets[0]->PT < 80. ) 
    return;
  if ( jets[1]->PT < 60. ) 
    return;
  if ( jets[2]->PT < 40. )
    return;
  if ( jets[3]->PT < 25. ) 
    return;
  countCutflowEvent("c_FourJetPT");
  
  //Need at least one b-tag
  std::vector<Jet*> bJets;
  for (int i = 0; i < jets.size(); i++){
    if( (jets[i]->PT>25.) && (jets[i]->Eta<2.5) && (checkBTag(jets[i])))
    bJets.push_back(jets[i]);  
  }
  if (bJets.size() == 0)
    return;
  countCutflowEvent("d_BTag");
  
  // Missing ET > 100 GeV
  if(missingET->P4().Et() < 100.)
    return;
  countCutflowEvent("e_MissingET");
  
  //Calculate mT and HT, where HT is sum of 4 jets and mT (check really mT!!!)
  /// PAPER SAYS HT IS SCALAR SUM OF JET MOMENTUM, ASSUME THEY MEAN TRANSVERSE  
  double mT = sqrt(2.*lepton.Pt()*missingET->P4().Et()*(1.-cos(lepton.DeltaPhi(missingET->P4()))));
  double HT = mT;
  for (int i = 0; i < 4; i++){
    HT += jets[i]->PT;
  }
  
  //Etmiss/sqrt(HT) > 5
  double etmiss_over_sqrt_ht = missingET->P4().Et()/(sqrt(HT));
  if (etmiss_over_sqrt_ht<5.)
    return;
  countCutflowEvent("f_EtMiss/sqrt(HT)");
  
  //dPhi(jet2,Etmiss)>0.8
  if( fabs(jets[1]->P4().DeltaPhi(missingET->P4()))<0.8)
    return;
  countCutflowEvent("g_dPhi(etmiss,jet2)");
  
  //--------------------------
  //------Signal Regions------
  //--------------------------
  
  //asymmetric mT2 (mT2_bl) needed by most signal regions so we calculate now
  /// IN REAL STUDY, ALL EVENTS ARE TAKEN AND TWO HIGHEST MVA B CANDIDATES ARE CHOSEN
  /// WE CANNOT IMPLEMENT THIS SO ONLY CALCULATE MT2 IN CASE WHERE AT LEAST 2 BJETS ARE FOUND
  /// FOR MORE THAN 2 BJETS, WE TAKE HARDEST (pt) JETS FOR CALCULATION
  double mt2_bl=0.;
  if(bJets.size() > 1){
    mt2_bl = mT2_bl(lepton, bJets[0]->P4(), bJets[1]->P4(), missingET->P4());
    double mt2_bl_b = mT2_bl(lepton, bJets[1]->P4(), bJets[0]->P4(), missingET->P4()); // Calculate with opposite assignment of bjets
    if(mt2_bl_b < mt2_bl)  //Lower value of mt2_bl is chosen
      mt2_bl = mt2_bl_b;
  }
  
  //mjjj is almost certainly required (used by SRtN1_shape that has weakest cuts) so calculate now
  // Begin by calculating W candidate (two closest jets that have m_jj > 60 GeV)
  bool top_found = false;
  bool W_found = false;
  TLorentzVector W_cand;
  double min_dR_W=1000.;
  int jet_W_id1=-1;
  int jet_W_id2=-1;
  for (int i = 0; i < (jets.size()-1); i++){
    for (int j = 1; j < (jets.size()); j++){
      if(j>i){
    TLorentzVector W_cand_temp = jets[i]->P4() + jets[j]->P4();
    if( (W_cand_temp.M()>60.) && ( fabs(jets[i]->P4().DeltaR(jets[j]->P4())) < min_dR_W) ){
      jet_W_id1=i;
      jet_W_id2=j;
      min_dR_W=fabs(jets[i]->P4().DeltaR(jets[j]->P4()));
      W_cand = W_cand_temp;
      W_found = true;
    }
      }
    }
  }
  // Now calculate m_jjj (third jet is closest in dR to W vector
  if(W_found == true){
    double min_dR_t=1000.;
    int jet_t_id=-1;
    for (int i = 0; i < jets.size(); i++){
      if( (i!=jet_W_id1) && (i!=jet_W_id2) ){
    if( fabs(jets[i]->P4().DeltaR(W_cand)) < min_dR_t){
      min_dR_t=fabs(jets[i]->P4().DeltaR(W_cand));
      jet_t_id=i;
    }
      }
    }
    double t_cand_M = (W_cand+jets[jet_t_id]->P4()).M();
    if( (t_cand_M > 130.) && (t_cand_M < 205.) )
      top_found =true;
  }
  
  //*************************
  // SRtN2
  // Start with SRtN2 since only region that does not require dPhi(jet2,Etmiss)>0.8
  ///NEED TO CONFIRM ORDER OF ADDITIONAL CUTS NOT GIVEN IN CUTFLOW TABLES
  if(missingET->P4().Et() > 200.){
    countCutflowEvent("i_CFtN2_a_etMiss");
    if(etmiss_over_sqrt_ht > 13.){
      countCutflowEvent("i_CFtN2_b_EtMiss/sqrt(HT)");
      if(mT>140.){
    countCutflowEvent("i_CFtN2_c_mT");
    if((bJets.size() > 1) && (mt2_bl > 170.)){
      countCutflowEvent("i_CFtN2_d_amT2");
      if (top_found==true){
        countCutflowEvent("i_CFtN2_e_top");
        //Note b-jets cuts have already been applied since we require amT2
        countSignalEvent("SRtN2");
      }
    }
      }
    }
  }
 
  //Most signal regions require dPhi(jet1,Etmiss)>0.8
  if( fabs(jets[0]->P4().DeltaPhi(missingET->P4())) < 0.8)
    return;
  countCutflowEvent("h_dPhi(etmiss,jet1)");
  
  //*************************
  // SRtN1_shape (EtMiss > 100 GeV and Etmiss/sqrt(HT) > 5 already done)
  if(mT>60.){
    countCutflowEvent("i_CFtN1_a_mT");
    if (top_found==true){
      countCutflowEvent("i_CFtN1_b_top");
      if(bJets.size()>1){
    countCutflowEvent("i_CFtN1_c_bnum");
    if(mT>140.){
      countCutflowEvent("i_CFtN1_d_mT(140)");
      if(missingET->P4().Et() < 125.)
        countSignalEvent("SRtN1_shape01");
      else if(missingET->P4().Et() < 150.)
        countSignalEvent("SRtN1_shape02");
      else if(missingET->P4().Et() > 150.)
        countSignalEvent("SRtN1_shape03");        
    }
      }
    }
  }
  
  
  ///IGNORE SRtN3 FOR NOW SINCE WE NEED TO CALCULATE MT2_TAU
/*  //*************************
  // SRtN3
  
  if(missingET->P4().Et() > 275.){
    countCutflowEvent("i_CFtN3_a_etMiss");
    if(etmiss_over_sqrt_ht > 11.){
      countCutflowEvent("i_CFtN3_b_EtMiss/sqrt(HT)");
      if(mT>200.){
    countCutflowEvent("i_CFtN3_c_mT");
    if((bJets.size() > 1) && (mt2_bl > 175.)){
      countCutflowEvent("i_CFtN3_d_amT2");
      if (top_found==true){
        countCutflowEvent("i_CFtN2_e_top");
        //Note b-jets cuts have already been applied since we require amT2
        countSignalEvent("SRtN2");
      }
    }
      }
    }
  }
  */
  
  //*************************
  // SRb can now start returning with minimal cuts
  bool SRbC2_flag = false;
  bool SRbC3_flag = false;
  
  //Missing Et cut
  if(missingET->P4().Et() < 150.)
    return;
  countCutflowEvent("i_CFbC1_a_etMiss");
  if(missingET->P4().Et() >160.){
    countCutflowEvent("i_CFbC2_a_etMiss");
    countCutflowEvent("i_CFbC3_a_etMiss");
    SRbC2_flag = true;
    SRbC3_flag = true;
  }
  
  //Etmiss/sqrt(HT) cut  
  if(etmiss_over_sqrt_ht < 7.)
    return;
  countCutflowEvent("i_CFbC1_b_sqrt(ht)");
  if((SRbC2_flag == true) && (etmiss_over_sqrt_ht > 8.)){
    countCutflowEvent("i_CFbC2_b_sqrt(ht)");
    countCutflowEvent("i_CFbC3_b_sqrt(ht)");
  }
  else{
    SRbC2_flag = false;
    SRbC3_flag = false;
  }
  
  //mt > 120
  if(mT<120.)
    return;
  countCutflowEvent("i_CFbC1_c_mT");
  if(SRbC2_flag == true){
    countCutflowEvent("i_CFbC2_c_mT");
    countCutflowEvent("i_CFbC3_c_mT");
  }
  
  //meff for C2 and C3
  if(SRbC2_flag == true){
    double meff = missingET->P4().Et() + lepton.Pt();
    for (int i = 0; i < jets.size(); i++){
      if (jets[i]->PT > 30.){
    meff+=jets[i]->PT;
      }
    }
    if(meff > 550.){
      countCutflowEvent("i_CFbC2_d_meff");
      if(meff > 700.)
    countCutflowEvent("i_CFbC3_d_meff");
      else
    SRbC3_flag = false;
    }
    else{
      SRbC2_flag = false;
      SRbC3_flag = false;
    }
  }
  
  //asymmetric mT2 for C2 and C3
  if((SRbC2_flag == true) && (mt2_bl > 175.))
    countCutflowEvent("i_CFbC2_e_amt");
  else
    SRbC2_flag = false;
  if((SRbC3_flag == true) && (mt2_bl > 200.))
    countCutflowEvent("i_CFbC3_e_amt");
  else
    SRbC3_flag = false;
  
  //Number of isolated tracks apart from reconstructed lepton =0
  std::vector<Track*> tracks_3 = filterPhaseSpace(tracks, 3., -2.47, 2.47);
  std::vector<Track*> tracks_10 = filterPhaseSpace(tracks_3, 10., -2.47, 2.47);
  std::vector<Track*> tracks_310;
  for (int i = 0; i < tracks_3.size(); i++){
    if( tracks_3[i]->PT <10.)
      tracks_310.push_back(tracks_3[i]);
  }
  
  std::vector<Track*> tracks_10_iso_a;
  for (int i = 0; i < tracks_10.size(); i++){
    bool track_iso =true;
    for (int j = 0; j < tracks_310.size(); j++){
      if(fabs(tracks_10[i]->P4().DeltaR(tracks_310[j]->P4())) < 0.4)
    track_iso =false;
    }
    if (track_iso == true)
      tracks_10_iso_a.push_back(tracks_10[i]);
  }
  
  std::vector<Track*> tracks_10_iso_b;
  if(tracks_10_iso_a.size() > 1){
    for (int i = 0; i < tracks_10_iso_a.size()-1; i++){
      bool track_iso =true;
      for (int j = 1; j < (tracks_10_iso_a.size()); j++){
    if((j>i) && (fabs(tracks_10_iso_a[i]->P4().DeltaR(tracks_10_iso_a[j]->P4())) < 0.4))
      track_iso =false;
      }
      if (track_iso == true)
    tracks_10_iso_b.push_back(tracks_10_iso_a[i]);
    }
  }
  else
    tracks_10_iso_b = tracks_10_iso_a;
  // if size is greater than 1 we must have more than just lepton
  if (tracks_10_iso_b.size()>1)
    return;
  //now check that lepton and track are same (isolation means if within 0.2 considered same object)
  if((tracks_10_iso_a.size() == 1) && (fabs(tracks_10_iso_a[0]->P4().DeltaR(lepton)) > 0.2) )
    return;
  countCutflowEvent("i_CFbC1_f_iso");
  if(SRbC2_flag == true)
    countCutflowEvent("i_CFbC2_f_iso");
  if(SRbC3_flag == true)
    countCutflowEvent("i_CFbC3_f_iso");
  
  //Can now count signal region SRbC1
  countSignalEvent("SRbC1");
  
  //btags > 2 for C2 and C3
  if((SRbC2_flag == true) && (bJets.size() > 1))
    countCutflowEvent("i_CFbC2_g_bjet");
  else
    return;
  if((SRbC3_flag == true) &&  (bJets.size() > 1))
    countCutflowEvent("i_CFbC3_g_bjet");
  else
    SRbC3_flag = false;
  
  //bjet[0] cut for C2 and C3   
  if(bJets[0]->PT < 100.)
    return;
  countCutflowEvent("i_CFbC2_h_bjet_pt");
  if((SRbC3_flag == true) &&  (bJets[0]->PT > 120.))
    countCutflowEvent("i_CFbC3_h_bjet_pt");
  else
    SRbC3_flag = false;
  
    //bjet[1] cut for C2 and C3   
  if(bJets[1]->PT < 50.)
    return;
  countCutflowEvent("i_CFbC2_i_bjet_pt2");
  countSignalEvent("SRbC2");
  if((SRbC3_flag == true) &&  (bJets[1]->PT > 120.)){
    countCutflowEvent("i_CFbC3_i_bjet_pt2");
    countSignalEvent("SRbC3");
  }
}

void Atlas_conf_2013_037::finalize() {
  // Whatever should be done after the run goes here
}       
