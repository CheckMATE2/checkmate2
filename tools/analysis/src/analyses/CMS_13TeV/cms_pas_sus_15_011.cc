#include "cms_pas_sus_15_011.h"
// AUTHOR: Anke Biekoetter
//  EMAIL: biekoetter@physik.rwth-aachen.de

template<class T> std::vector<T*> isolateLeptons(std::vector<T*> leptons, double maxpTrel);

void Cms_pas_sus_15_011::initialize() {
  setAnalysisName("cms_pas_sus_15_011");          
  setInformation(""
    "# CMS\n"
     "# 13 TeV, lumi 2.2 fb^-1\n"
     "# Search for new physics in final states with two\n"
     "# opposite-sign same-flavor leptons, jets and MET in pp\n"
     "# collisions at sqrt(s) = 13 TeV\n"
  "");
  setLuminosity(2.2*units::INVFB);      
  bookSignalRegions("SRA_b0_a.lowMET;SRA_b0_b.mediumMET;SRA_b0_c.highMET;SRA_b0_d.vhighMET;SRA_b1_a.lowMET;SRA_b1_b.mediumMET;SRA_b1_c.highMET;SRA_b1_d.vhighMET;SRB_b0_a.lowMET;SRB_b0_b.mediumMET;SRB_b0_c.highMET;SRB_b0_d.vhighMET;SRB_b1_a.lowMET;SRB_b1_b.mediumMET;SRB_b1_c.highMET;SRB_b1_d.vhighMET;SRC_ATLAS;SRD_cen_a.vlowMll_b_all;SRD_cen_a.vlowMll_b0;SRD_cen_a.vlowMll_b1;SRD_cen_b.lowMll_b_all;SRD_cen_b.lowMll_b0;SRD_cen_b.lowMll_b1;SRD_cen_c.mediumMll_b_all;SRD_cen_c.mediumMll_b0;SRD_cen_c.mediumMll_b1;SRD_cen_d.highMll_b_all;SRD_cen_d.highMll_b0;SRD_cen_d.highMll_b1;SRD_cen_e.vhighMll_b_all;SRD_cen_e.vhighMll_b0;SRD_cen_e.vhighMll_b1;SRE_for_a.vlowMll_b_all;SRE_for_a.vlowMll_b0;SRE_for_a.vlowMll_b1;SRE_for_b.lowMll_b_all;SRE_for_b.lowMll_b0;SRE_for_b.lowMll_b1;SRE_for_c.mediumMll_b_all;SRE_for_c.mediumMll_b0;SRE_for_c.mediumMll_b1;SRE_for_d.highMll_b_all;SRE_for_d.highMll_b0;SRE_for_d.highMll_b1;SRE_for_e.vhighMll_b_all;SRE_for_e.vhighMll_b0;SRE_for_e.vhighMll_b1;");
  // CUTFLOW REGIONS
  bookCutflowRegions("0.dileptonEvents;1.dileptonPT_cen_b0;1.dileptonPT_cen_b1;1.dileptonPT_for_b0;1.dileptonPT_for_b1;\
a.vlow_a.mass_cen_b0;a.vlow_a.mass_cen_b1;a.vlow_a.mass_for_b0;a.vlow_a.mass_for_b1;\
b.low_a.mass_cen_b0;b.low_a.mass_cen_b1;b.low_a.mass_for_b0;b.low_a.mass_for_b1;\
c.medium_a.mass_cen_b0;c.medium_a.mass_cen_b1;c.medium_a.mass_for_b0;c.medium_a.mass_for_b1;\
d.high_a.mass_cen_b0;d.high_a.mass_cen_b1;d.high_a.mass_for_b0;d.high_a.mass_for_b1;\
e.vhigh_a.mass_cen_b0;e.vhigh_a.mass_cen_b1;e.vhigh_a.mass_for_b0;e.vhigh_a.mass_for_b1;\
\
a.vlow_b.jet2_cen_b0;a.vlow_b.jet2_cen_b1;a.vlow_b.jet2_for_b0;a.vlow_b.jet2_for_b1;\
b.low_b.jet2_cen_b0;b.low_b.jet2_cen_b1;b.low_b.jet2_for_b0;b.low_b.jet2_for_b1;\
c.medium_b.jet2_cen_b0;c.medium_b.jet2_cen_b1;c.medium_b.jet2_for_b0;c.medium_b.jet2_for_b1;\
d.high_b.jet2_cen_b0;d.high_b.jet2_cen_b1;d.high_b.jet2_for_b0;d.high_b.jet2_for_b1;\
e.vhigh_b.jet2_cen_b0;e.vhigh_b.jet2_cen_b1;e.vhigh_b.jet2_for_b0;e.vhigh_b.jet2_for_b1;\
\
a.vlow_b.jet3_cen_b0;a.vlow_b.jet3_cen_b1;a.vlow_b.jet3_for_b0;a.vlow_b.jet3_for_b1;\
b.low_b.jet3_cen_b0;b.low_b.jet3_cen_b1;b.low_b.jet3_for_b0;b.low_b.jet3_for_b1;\
c.medium_b.jet3_cen_b0;c.medium_b.jet3_cen_b1;c.medium_b.jet3_for_b0;c.medium_b.jet3_for_b1;\
d.high_b.jet3_cen_b0;d.high_b.jet3_cen_b1;d.high_b.jet3_for_b0;d.high_b.jet3_for_b1;\
e.vhigh_b.jet3_cen_b0;e.vhigh_b.jet3_cen_b1;e.vhigh_b.jet3_for_b0;e.vhigh_b.jet3_for_b1;\
\
a.vlow_c.MET100_cen_b0;a.vlow_c.MET100_cen_b1;a.vlow_c.MET100_for_b0;a.vlow_c.MET100_for_b1;\
b.low_c.MET100_cen_b0;b.low_c.MET100_cen_b1;b.low_c.MET100_for_b0;b.low_c.MET100_for_b1;\
c.medium_c.MET100_cen_b0;c.medium_c.MET100_cen_b1;c.medium_c.MET100_for_b0;c.medium_c.MET100_for_b1;\
d.high_c.MET100_cen_b0;d.high_c.MET100_cen_b1;d.high_c.MET100_for_b0;d.high_c.MET100_for_b1;\
e.vhigh_c.MET100_cen_b0;e.vhigh_c.MET100_cen_b1;e.vhigh_c.MET100_for_b0;e.vhigh_c.MET100_for_b1;\
\
a.vlow_c.MET150_cen_b0;a.vlow_c.MET150_cen_b1;a.vlow_c.MET150_for_b0;a.vlow_c.MET150_for_b1;\
b.low_c.MET150_cen_b0;b.low_c.MET150_cen_b1;b.low_c.MET150_for_b0;b.low_c.MET150_for_b1;\
c.medium_c.MET150_cen_b0;c.medium_c.MET150_cen_b1;c.medium_c.MET150_for_b0;c.medium_c.MET150_for_b1;\
d.high_c.MET150_cen_b0;d.high_c.MET150_cen_b1;d.high_c.MET150_for_b0;d.high_c.MET150_for_b1;\
e.vhigh_c.MET150_cen_b0;e.vhigh_c.MET150_cen_b1;e.vhigh_c.MET150_for_b0;e.vhigh_c.MET150_for_b1;\
\
a.vlow_d.signal_cen_b0;a.vlow_d.signal_cen_b1;a.vlow_d.signal_for_b0;a.vlow_d.signal_for_b1;\
b.low_d.signal_cen_b0;b.low_d.signal_cen_b1;b.low_d.signal_for_b0;b.low_d.signal_for_b1;\
c.medium_d.signal_cen_b0;c.medium_d.signal_cen_b1;c.medium_d.signal_for_b0;c.medium_d.signal_for_b1;\
d.high_d.signal_cen_b0;d.high_d.signal_cen_b1;d.high_d.signal_for_b0;d.high_d.signal_for_b1;\
e.vhigh_d.signal_cen_b0;e.vhigh_d.signal_cen_b1;e.vhigh_d.signal_for_b0;e.vhigh_d.signal_for_b1;");

  // You should initialize any declared variables here
  
}

void Cms_pas_sus_15_011::analyze() {

  missingET->addMuons(muonsCombined);  
  double MET = missingET->P4().Et();

  std::vector<Electron*> elec;
  std::vector<Muon*> muon;
  double eta;
  
  countCutflowEvent("0.dileptonEvents");
  
  // non-isolated leptons
  for ( int i=0; i < electrons.size(); i++ ) {
    eta = electrons[i]->Eta;
    if (fabs(eta) < 1.4 || ( fabs(eta)>1.6 && fabs(eta)<2.4) ) {
    	if (i ==0 && electrons[i]->PT > 33. ) 
      		elec.push_back(electrons[i]);
      	else if (i > 0 && electrons[i]->PT > 33. ) 
      		elec.push_back(electrons[i]);
    }
  }
  
  for ( int i=0; i < muons.size(); i++ ) {
    eta = muons[i]->Eta;
    if (fabs(eta) < 1.4 || ( fabs(eta)>1.6 && fabs(eta)<2.4) ) {
    	if (i ==0 && muons[i]->PT > 27. ) 
      		muon.push_back(muons[i]);
      	else if (i > 0 && muons[i]->PT > 8. ) 
      		muon.push_back(muons[i]);
    }
  }

  // isolated leptons
  for ( int i=0; i < electronsLoose.size(); i++ ) {
    eta = electronsLoose[i]->Eta;
    if (fabs(eta) < 1.4 || ( fabs(eta)>1.6 && fabs(eta)<2.4) ) {
    	if (i ==0 && electronsLoose[i]->PT > 17. && electronsLoose[i]->PT < 33. ) 
      		elec.push_back(electronsLoose[i]);
      	else if (i > 0 && electronsLoose[i]->PT > 12. && electronsLoose[i]->PT < 33. ) 
      		elec.push_back(electronsLoose[i]);
    }
  }

  for ( int i=0; i < muonsCombined.size(); i++ ) {
    eta = muonsCombined[i]->Eta;
    if (fabs(eta) < 1.4 || ( fabs(eta)>1.6 && fabs(eta)<2.4) ) {
    	if (i ==0 && muonsCombined[i]->PT > 17. && muonsCombined[i]->PT < 27.) 
      		muon.push_back(muonsCombined[i]);
    }
  }
  
  // additional lepton isolation criteria for different pT bins
  elec = isolateLeptons(elec, 0.1);
  muon = isolateLeptons(muon, 0.2);

  
  // fill muons and electrons into lepton vector and sort by pT
  std::vector<FinalStateObject*> leps;
  FinalStateObject* lep;
  for( int i=0; i < elec.size(); i++) {
    lep = newFinalStateObject( elec[i] );
    leps.push_back(lep);
  }
  for( int i=0; i < muon.size(); i++) {
    lep = newFinalStateObject( muon[i] );
    leps.push_back(lep);
  }
  std::sort( leps.begin(), leps.end(), sortByPT );
  
  leps = filterPhaseSpace( leps, 20., -2.4, 2.4); 
  if ( leps.size() < 2 ) return;
  
  // veto events without e+e- or mu+mu-
  if( leps[0]->Type != leps[1]->Type ) return;
  if( leps[0]->Charge * leps[1]->Charge > 0 ) return;
  
  // consistency with Z boson
  double mll = ( leps[0]->P4() + leps[1]->P4() ).M();
  bool onZflag = false;
  if (mll > 81 && mll <= 101) 
  	onZflag = true;
  
  // distance between leptons delta_R > 0.1 / 0.3
  if (onZflag) {
  	if( leps[0]->P4().DeltaR( leps[1]->P4() ) < 0.1 ) return;
  } else {
  	if( leps[0]->P4().DeltaR( leps[1]->P4() ) < 0.3 ) return;
  }

  
  jets = filterPhaseSpace( jets, 35., -2.4, 2.4 );
  jets = overlapRemoval(jets, leps, 0.4); // information was missing in the paper
  if ( jets.size() < 2 ) return;
  // find b-tagged jets
  std::vector<Jet*> btagged;
  for (std::vector<Jet*>::const_iterator it=jets.begin(); it != jets.end(); it++) {
      if (checkBTag(*it)) {
         btagged.push_back(*it);
      }
  }
  int nbjets = btagged.size();
  
  // HT variable
  double HT = 0;
    for(int i=0;i<jets.size();i++){
      HT+=jets[i]->PT;
    }
  double HTvar = HT + leps[0]->PT + leps[1]->PT;
  
  // cutflow regions
//if( leps[0]->Type == "muon") {
  if (nbjets == 0) {
  	if (fabs(leps[0]->Eta) < 1.4 && fabs(leps[1]->Eta) < 1.4 ) 
  		countCutflowEvent("1.dileptonPT_cen_b0");
  	else 
  		countCutflowEvent("1.dileptonPT_for_b0");
  }
  else {
  	if (fabs(leps[0]->Eta) < 1.4 && fabs(leps[1]->Eta) < 1.4 ) 
  		countCutflowEvent("1.dileptonPT_cen_b1");
  	else 
  		countCutflowEvent("1.dileptonPT_for_b1");
  }
  		

 ///////////////////////////////////////////////////////
 // SIGNAL REGIONS
 ///////////////////////////////////////////////////////
 
 // on-Z search
 if ( onZflag ) {
 	if ((jets.size() == 2 || jets.size() == 3) && HT > 400 ) {
 		if (nbjets == 0) {
 			if ( MET > 100 && MET < 150 )
 				countSignalEvent("SRA_b0_a.lowMET");
 			else if ( MET > 150 && MET < 225 )
 				countSignalEvent("SRA_b0_b.mediumMET");
 			else if ( MET > 225 && MET < 300 )
 				countSignalEvent("SRA_b0_c.highMET");
 			else if ( MET > 300 )
 				countSignalEvent("SRA_b0_d.vhighMET");
 		}
 		else {
 			if ( MET > 100 && MET < 150 )
 				countSignalEvent("SRA_b1_a.lowMET");
 			else if ( MET > 150 && MET < 225 )
 				countSignalEvent("SRA_b1_b.mediumMET");
 			else if ( MET > 225 && MET < 300 )
 				countSignalEvent("SRA_b1_c.highMET");
 			else if ( MET > 300 )
 				countSignalEvent("SRA_b1_d.vhighMET");
 		}
 	} // end SRA
 	else if ( jets.size() >= 4 ) {
 		if (nbjets == 0) {
 			if ( MET > 100 && MET < 150 )
 				countSignalEvent("SRB_b0_a.lowMET");
 			else if ( MET > 150 && MET < 225 )
 				countSignalEvent("SRB_b0_b.mediumMET");
 			else if ( MET > 225 && MET < 300 )
 				countSignalEvent("SRB_b0_c.highMET");
 			else if ( MET > 300 )
 				countSignalEvent("SRB_b0_d.vhighMET");
 		}
 		else {
 			if ( MET > 100 && MET < 150 )
 				countSignalEvent("SRB_b1_a.lowMET");
 			else if ( MET > 150 && MET < 225 )
 				countSignalEvent("SRB_b1_b.mediumMET");
 			else if ( MET > 225 && MET < 300 )
 				countSignalEvent("SRB_b1_c.highMET");
 			else if ( MET > 300 )
 				countSignalEvent("SRB_b1_d.vhighMET");
 		}
 	} // end SRB
 	// deltaPhi varibles for ATLAS region
 	double deltaPhij1MET = fabs( jets[0]->P4().DeltaPhi( missingET->P4() ) );
 	double deltaPhij2MET = fabs( jets[1]->P4().DeltaPhi( missingET->P4() ) );
 	
 	if ( MET > 225 && deltaPhij1MET > 0.4 && deltaPhij2MET > 0.4 && HTvar > 600)
 		countSignalEvent("SRC_ATLAS");
 		
 } // end on-Z search
 
 // edge-search signal regions
 //////////////////////////////////////////////////////////////
 // very low Mll
 if ( mll > 20 && mll <= 70 ) {
 	if (fabs(leps[0]->Eta) < 1.4 && fabs(leps[1]->Eta) < 1.4) {
 		// check b tags
 		if (nbjets == 0) {
 			countCutflowEvent("a.vlow_a.mass_cen_b0");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("a.vlow_b.jet2_cen_b0");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("a.vlow_c.MET100_cen_b0");
 				}
 				if (MET > 150){
 					countCutflowEvent("a.vlow_c.MET150_cen_b0");
 					
 					countCutflowEvent("a.vlow_d.signal_cen_b0");					
 					countSignalEvent("SRD_cen_a.vlowMll_b_all");
 					countSignalEvent("SRD_cen_a.vlowMll_b0");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("a.vlow_b.jet3_cen_b0");
 				//check MET conditions
 				if (MET > 100 && MET < 150 ) {
 						countCutflowEvent("a.vlow_d.signal_cen_b0");			
 						countSignalEvent("SRD_cen_a.vlowMll_b_all");
 						countSignalEvent("SRD_cen_a.vlowMll_b0");
 				}
 			}
 		}
 		else { // at least on b-tagged jet
 			countCutflowEvent("a.vlow_a.mass_cen_b1");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("a.vlow_b.jet2_cen_b1");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("a.vlow_c.MET100_cen_b1");
 				}
 				if (MET > 150){
 					countCutflowEvent("a.vlow_c.MET150_cen_b1");
 					
 					countCutflowEvent("a.vlow_d.signal_cen_b1");
 					countSignalEvent("SRD_cen_a.vlowMll_b_all");
 					countSignalEvent("SRD_cen_a.vlowMll_b1");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("a.vlow_b.jet3_cen_b1");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
						countCutflowEvent("a.vlow_d.signal_cen_b1");
 						countSignalEvent("SRD_cen_a.vlowMll_b_all");
 						countSignalEvent("SRD_cen_a.vlowMll_b1");
 				}
 			}
 		}		
 	}
 	// forward region
 	else { 
 		// check b tags
 		if (nbjets == 0) {
 			countCutflowEvent("a.vlow_a.mass_for_b0");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("a.vlow_b.jet2_for_b0");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("a.vlow_c.MET100_for_b0");
 				}
 				if (MET > 150){
 					countCutflowEvent("a.vlow_c.MET150_for_b0");
 					
 					countCutflowEvent("a.vlow_d.signal_for_b0");
 					countSignalEvent("SRE_for_a.vlowMll_b_all");
 					countSignalEvent("SRE_for_a.vlowMll_b0");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("a.vlow_b.jet3_for_b0");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
						countCutflowEvent("a.vlow_d.signal_for_b0");
 						countSignalEvent("SRE_for_a.vlowMll_b_all");
 						countSignalEvent("SRE_for_a.vlowMll_b0");
 				}
 			}
 		}
 		else { // at least on b-tagged jet
 			countCutflowEvent("a.vlow_a.mass_for_b1");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("a.vlow_b.jet2_for_b1");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("a.vlow_c.MET100_for_b1");
 					countCutflowEvent("a.vlow_d.signal_for_b1");
 					countSignalEvent("SRE_for_a.vlowMll_b1");
 					countSignalEvent("SRE_for_a.vlowMll_b_all");
 				}
 				if (MET > 150){
 					countCutflowEvent("a.vlow_c.MET150_for_b1");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("a.vlow_b.jet3_for_b1");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
 						countSignalEvent("SRE_for_a.vlowMll_b_all");
 						countSignalEvent("SRE_for_a.vlowMll_b1");
 						countCutflowEvent("a.vlow_d.signal_for_b1");
 				}
 			}
 		}		
 	}
 }

 //////////////////////////////////////////////////////////////
 // low Mll
 if ( mll > 70 && mll <= 81 ) {
 	if (fabs(leps[0]->Eta) < 1.4 && fabs(leps[1]->Eta) < 1.4) {
 		// check b tags
 		if (nbjets == 0) {
 			countCutflowEvent("b.low_a.mass_cen_b0");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("b.low_b.jet2_cen_b0");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("b.low_c.MET100_cen_b0");
 				}
 				if (MET > 150){
 					countCutflowEvent("b.low_c.MET150_cen_b0");
 					
 					countCutflowEvent("b.low_d.signal_cen_b0");					
 					countSignalEvent("SRD_cen_b.lowMll_b_all");
 					countSignalEvent("SRD_cen_b.lowMll_b0");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("b.low_b.jet3_cen_b0");
 				//check MET conditions
 				if (MET > 100 && MET < 150 ) {
 						countCutflowEvent("b.low_d.signal_cen_b0");			
 						countSignalEvent("SRD_cen_b.lowMll_b_all");
 						countSignalEvent("SRD_cen_b.lowMll_b0");
 				}
 			}
 		}
 		else { // at least on b-tagged jet
 			countCutflowEvent("b.low_a.mass_cen_b1");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("b.low_b.jet2_cen_b1");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("b.low_c.MET100_cen_b1");
 				}
 				if (MET > 150){
 					countCutflowEvent("b.low_c.MET150_cen_b1");
 					
 					countCutflowEvent("b.low_d.signal_cen_b1");
 					countSignalEvent("SRD_cen_b.lowMll_b_all");
 					countSignalEvent("SRD_cen_b.lowMll_b1");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("b.low_b.jet3_cen_b1");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
						countCutflowEvent("b.low_d.signal_cen_b1");
 						countSignalEvent("SRD_cen_b.lowMll_b_all");
 						countSignalEvent("SRD_cen_b.lowMll_b1");
 				}
 			}
 		}		
 	}
 	// forward region
 	else { 
 		// check b tags
 		if (nbjets == 0) {
 			countCutflowEvent("b.low_a.mass_for_b0");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("b.low_b.jet2_for_b0");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("b.low_c.MET100_for_b0");
 				}
 				if (MET > 150){
 					countCutflowEvent("b.low_c.MET150_for_b0");
 					
 					countCutflowEvent("b.low_d.signal_for_b0");
 					countSignalEvent("SRE_for_b.lowMll_b_all");
 					countSignalEvent("SRE_for_b.lowMll_b0");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("b.low_b.jet3_for_b0");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
						countCutflowEvent("b.low_d.signal_for_b0");
 						countSignalEvent("SRE_for_b.lowMll_b_all");
 						countSignalEvent("SRE_for_b.lowMll_b0");
 				}
 			}
 		}
 		else { // at least on b-tagged jet
 			countCutflowEvent("b.low_a.mass_for_b1");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("b.low_b.jet2_for_b1");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("b.low_c.MET100_for_b1");
 					countCutflowEvent("b.low_d.signal_for_b1");
 					countSignalEvent("SRE_for_b.lowMll_b1");
 					countSignalEvent("SRE_for_b.lowMll_b_all");
 				}
 				if (MET > 150){
 					countCutflowEvent("b.low_c.MET150_for_b1");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("b.low_b.jet3_for_b1");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
 						countSignalEvent("SRE_for_b.lowMll_b_all");
 						countSignalEvent("SRE_for_b.lowMll_b1");
 						countCutflowEvent("b.low_d.signal_for_b1");
 				}
 			}
 		}		
 	}
 }

 //////////////////////////////////////////////////////////////
 // medium Mll
 if ( mll > 81 && mll <= 101 ) {
 	if (fabs(leps[0]->Eta) < 1.4 && fabs(leps[1]->Eta) < 1.4) {
 		// check b tags
 		if (nbjets == 0) {
 			countCutflowEvent("c.medium_a.mass_cen_b0");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("c.medium_b.jet2_cen_b0");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("c.medium_c.MET100_cen_b0");
 				}
 				if (MET > 150){
 					countCutflowEvent("c.medium_c.MET150_cen_b0");
 					
 					countCutflowEvent("c.medium_d.signal_cen_b0");					
 					countSignalEvent("SRD_cen_c.mediumMll_b_all");
 					countSignalEvent("SRD_cen_c.mediumMll_b0");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("c.medium_b.jet3_cen_b0");
 				//check MET conditions
 				if (MET > 100 && MET < 150 ) {
 						countCutflowEvent("c.medium_d.signal_cen_b0");			
 						countSignalEvent("SRD_cen_c.mediumMll_b_all");
 						countSignalEvent("SRD_cen_c.mediumMll_b0");
 				}
 			}
 		}
 		else { // at least on b-tagged jet
 			countCutflowEvent("c.medium_a.mass_cen_b1");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("c.medium_b.jet2_cen_b1");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("c.medium_c.MET100_cen_b1");
 				}
 				if (MET > 150){
 					countCutflowEvent("c.medium_c.MET150_cen_b1");
 					
 					countCutflowEvent("c.medium_d.signal_cen_b1");
 					countSignalEvent("SRD_cen_c.mediumMll_b_all");
 					countSignalEvent("SRD_cen_c.mediumMll_b1");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("c.medium_b.jet3_cen_b1");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
						countCutflowEvent("c.medium_d.signal_cen_b1");
 						countSignalEvent("SRD_cen_c.mediumMll_b_all");
 						countSignalEvent("SRD_cen_c.mediumMll_b1");
 				}
 			}
 		}		
 	}
 	// forward region
 	else { 
 		// check b tags
 		if (nbjets == 0) {
 			countCutflowEvent("c.medium_a.mass_for_b0");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("c.medium_b.jet2_for_b0");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("c.medium_c.MET100_for_b0");
 				}
 				if (MET > 150){
 					countCutflowEvent("c.medium_c.MET150_for_b0");
 					
 					countCutflowEvent("c.medium_d.signal_for_b0");
 					countSignalEvent("SRE_for_c.mediumMll_b_all");
 					countSignalEvent("SRE_for_c.mediumMll_b0");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("c.medium_b.jet3_for_b0");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
						countCutflowEvent("c.medium_d.signal_for_b0");
 						countSignalEvent("SRE_for_c.mediumMll_b_all");
 						countSignalEvent("SRE_for_c.mediumMll_b0");
 				}
 			}
 		}
 		else { // at least on b-tagged jet
 			countCutflowEvent("c.medium_a.mass_for_b1");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("c.medium_b.jet2_for_b1");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("c.medium_c.MET100_for_b1");
 					countCutflowEvent("c.medium_d.signal_for_b1");
 					countSignalEvent("SRE_for_c.mediumMll_b1");
 					countSignalEvent("SRE_for_c.mediumMll_b_all");
 				}
 				if (MET > 150){
 					countCutflowEvent("c.medium_c.MET150_for_b1");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("c.medium_b.jet3_for_b1");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
 						countSignalEvent("SRE_for_c.mediumMll_b_all");
 						countSignalEvent("SRE_for_c.mediumMll_b1");
 						countCutflowEvent("c.medium_d.signal_for_b1");
 				}
 			}
 		}		
 	}
 }

 //////////////////////////////////////////////////////////////
 // high Mll
 if ( mll > 101 && mll <= 120 ) {
 	if (fabs(leps[0]->Eta) < 1.4 && fabs(leps[1]->Eta) < 1.4) {
 		// check b tags
 		if (nbjets == 0) {
 			countCutflowEvent("d.high_a.mass_cen_b0");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("d.high_b.jet2_cen_b0");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("d.high_c.MET100_cen_b0");
 				}
 				if (MET > 150){
 					countCutflowEvent("d.high_c.MET150_cen_b0");
 					
 					countCutflowEvent("d.high_d.signal_cen_b0");					
 					countSignalEvent("SRD_cen_d.highMll_b_all");
 					countSignalEvent("SRD_cen_d.highMll_b0");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("d.high_b.jet3_cen_b0");
 				//check MET conditions
 				if (MET > 100 && MET < 150 ) {
 						countCutflowEvent("d.high_d.signal_cen_b0");			
 						countSignalEvent("SRD_cen_d.highMll_b_all");
 						countSignalEvent("SRD_cen_d.highMll_b0");
 				}
 			}
 		}
 		else { // at least on b-tagged jet
 			countCutflowEvent("d.high_a.mass_cen_b1");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("d.high_b.jet2_cen_b1");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("d.high_c.MET100_cen_b1");
 				}
 				if (MET > 150){
 					countCutflowEvent("d.high_c.MET150_cen_b1");
 					
 					countCutflowEvent("d.high_d.signal_cen_b1");
 					countSignalEvent("SRD_cen_d.highMll_b_all");
 					countSignalEvent("SRD_cen_d.highMll_b1");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("d.high_b.jet3_cen_b1");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
						countCutflowEvent("d.high_d.signal_cen_b1");
 						countSignalEvent("SRD_cen_d.highMll_b_all");
 						countSignalEvent("SRD_cen_d.highMll_b1");
 				}
 			}
 		}		
 	}
 	// forward region
 	else { 
 		// check b tags
 		if (nbjets == 0) {
 			countCutflowEvent("d.high_a.mass_for_b0");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("d.high_b.jet2_for_b0");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("d.high_c.MET100_for_b0");
 				}
 				if (MET > 150){
 					countCutflowEvent("d.high_c.MET150_for_b0");
 					
 					countCutflowEvent("d.high_d.signal_for_b0");
 					countSignalEvent("SRE_for_d.highMll_b_all");
 					countSignalEvent("SRE_for_d.highMll_b0");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("d.high_b.jet3_for_b0");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
						countCutflowEvent("d.high_d.signal_for_b0");
 						countSignalEvent("SRE_for_d.highMll_b_all");
 						countSignalEvent("SRE_for_d.highMll_b0");
 				}
 			}
 		}
 		else { // at least on b-tagged jet
 			countCutflowEvent("d.high_a.mass_for_b1");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("d.high_b.jet2_for_b1");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("d.high_c.MET100_for_b1");
 					countCutflowEvent("d.high_d.signal_for_b1");
 					countSignalEvent("SRE_for_d.highMll_b1");
 					countSignalEvent("SRE_for_d.highMll_b_all");
 				}
 				if (MET > 150){
 					countCutflowEvent("d.high_c.MET150_for_b1");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("d.high_b.jet3_for_b1");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
 						countSignalEvent("SRE_for_d.highMll_b_all");
 						countSignalEvent("SRE_for_d.highMll_b1");
 						countCutflowEvent("d.high_d.signal_for_b1");
 				}
 			}
 		}		
 	}
 }

 //////////////////////////////////////////////////////////////
 // very high Mll
 if ( mll > 120) {
 	if (fabs(leps[0]->Eta) < 1.4 && fabs(leps[1]->Eta) < 1.4) {
 		// check b tags
 		if (nbjets == 0) {
 			countCutflowEvent("e.vhigh_a.mass_cen_b0");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("e.vhigh_b.jet2_cen_b0");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("e.vhigh_c.MET100_cen_b0");
 				}
 				if (MET > 150){
 					countCutflowEvent("e.vhigh_c.MET150_cen_b0");
 					
 					countCutflowEvent("e.vhigh_d.signal_cen_b0");					
 					countSignalEvent("SRD_cen_e.vhighMll_b_all");
 					countSignalEvent("SRD_cen_e.vhighMll_b0");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("e.vhigh_b.jet3_cen_b0");
 				//check MET conditions
 				if (MET > 100 && MET < 150 ) {
 						countCutflowEvent("e.vhigh_d.signal_cen_b0");			
 						countSignalEvent("SRD_cen_e.vhighMll_b_all");
 						countSignalEvent("SRD_cen_e.vhighMll_b0");
 				}
 			}
 		}
 		else { // at least on b-tagged jet
 			countCutflowEvent("e.vhigh_a.mass_cen_b1");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("e.vhigh_b.jet2_cen_b1");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("e.vhigh_c.MET100_cen_b1");
 				}
 				if (MET > 150){
 					countCutflowEvent("e.vhigh_c.MET150_cen_b1");
 					
 					countCutflowEvent("e.vhigh_d.signal_cen_b1");
 					countSignalEvent("SRD_cen_e.vhighMll_b_all");
 					countSignalEvent("SRD_cen_e.vhighMll_b1");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("e.vhigh_b.jet3_cen_b1");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
						countCutflowEvent("e.vhigh_d.signal_cen_b1");
 						countSignalEvent("SRD_cen_e.vhighMll_b_all");
 						countSignalEvent("SRD_cen_e.vhighMll_b1");
 				}
 			}
 		}		
 	}
 	// forward region
 	else { 
 		// check b tags
 		if (nbjets == 0) {
 			countCutflowEvent("e.vhigh_a.mass_for_b0");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("e.vhigh_b.jet2_for_b0");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("e.vhigh_c.MET100_for_b0");
 				}
 				if (MET > 150){
 					countCutflowEvent("e.vhigh_c.MET150_for_b0");
 					
 					countCutflowEvent("e.vhigh_d.signal_for_b0");
 					countSignalEvent("SRE_for_e.vhighMll_b_all");
 					countSignalEvent("SRE_for_e.vhighMll_b0");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("e.vhigh_b.jet3_for_b0");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
						countCutflowEvent("e.vhigh_d.signal_for_b0");
 						countSignalEvent("SRE_for_e.vhighMll_b_all");
 						countSignalEvent("SRE_for_e.vhighMll_b0");
 				}
 			}
 		}
 		else { // at least on b-tagged jet
 			countCutflowEvent("e.vhigh_a.mass_for_b1");
 			// check jet multiplicity
 			if (jets.size() >= 2) {
 				countCutflowEvent("e.vhigh_b.jet2_for_b1");
 				//check MET conditions
 				if (MET > 100){
 					countCutflowEvent("e.vhigh_c.MET100_for_b1");
 					countCutflowEvent("e.vhigh_d.signal_for_b1");
 					countSignalEvent("SRE_for_e.vhighMll_b1");
 					countSignalEvent("SRE_for_e.vhighMll_b_all");
 				}
 				if (MET > 150){
 					countCutflowEvent("e.vhigh_c.MET150_for_b1");
 				}
 			}
 			if (jets.size() >= 3) {
 				countCutflowEvent("e.vhigh_b.jet3_for_b1");
 				//check MET conditions
				if ( MET > 100 && MET < 150 ) {
 						countSignalEvent("SRE_for_e.vhighMll_b_all");
 						countSignalEvent("SRE_for_e.vhighMll_b1");
 						countCutflowEvent("e.vhigh_d.signal_for_b1");
 				}
 			}
 		}		
 	}
 }
//} // end if muon
 
// if ( !((jets.size() >= 2 && MET > 150) || (jets.size() >= 3 && MET > 100)) ) return;
// // central region
// if ( fabs(leps[0]->Eta) < 1.4 && fabs(leps[1]->Eta) < 1.4 ) {
// 	if ( mll > 20 && mll < 70 ) {
// 		countSignalEvent("SRD_cen_a.vlowMll_b_all");
// 		if (nbjets == 0) 
// 			countSignalEvent("SRD_cen_a.vlowMll_b0");
// 		else
// 			countSignalEvent("SRD_cen_a.vlowMll_b1");
// 	}
// 	else if ( mll > 70 && mll < 81 ) {
// 		countSignalEvent("SRD_cen_b.lowMll_b_all");
// 		if (nbjets == 0) 
// 			countSignalEvent("SRD_cen_b.lowMll_b0");
// 		else
// 			countSignalEvent("SRD_cen_b.lowMll_b1");
// 	}
// 	else if ( mll > 81 && mll < 101 ) {
// 		countSignalEvent("SRD_cen_c.mediumMll_b_all");
// 		if (nbjets == 0) 
// 			countSignalEvent("SRD_cen_c.mediumMll_b0");
// 		else
// 			countSignalEvent("SRD_cen_c.mediumMll_b1");
// 	}
// 	else if ( mll > 101 && mll < 120 ) {
// 		countSignalEvent("SRD_cen_d.highMll_b_all");
// 		if (nbjets == 0) 
// 			countSignalEvent("SRD_cen_d.highMll_b0");
// 		else
// 			countSignalEvent("SRD_cen_d.highMll_b1");
// 	}
// 	if ( mll > 120 ) {
// 		countSignalEvent("SRD_cen_e.vhighMll_b_all");
// 		if (nbjets == 0) 
// 			countSignalEvent("SRD_cen_e.vhighMll_b0");
// 		else
// 			countSignalEvent("SRD_cen_e.vhighMll_b1");
// 	}
// } // end central region
// else {
// 	if ( mll > 20 && mll < 70 ) {
// 		countSignalEvent("SRE_for_a.vlowMll_b_all");
// 		if (nbjets == 0) 
// 			countSignalEvent("SRE_for_a.vlowMll_b0");
// 		else
// 			countSignalEvent("SRE_for_a.vlowMll_b1");
// 	}
// 	else if ( mll > 70 && mll < 81 ) {
// 		countSignalEvent("SRE_for_b.lowMll_b_all");
// 		if (nbjets == 0) 
// 			countSignalEvent("SRE_for_b.lowMll_b0");
// 		else
// 			countSignalEvent("SRE_for_b.lowMll_b1");
// 	}
// 	else if ( mll > 81 && mll < 101 ) {
// 		countSignalEvent("SRE_for_c.mediumMll_b_all");
// 		if (nbjets == 0) 
// 			countSignalEvent("SRE_for_c.mediumMll_b0");
// 		else
// 			countSignalEvent("SRE_for_c.mediumMll_b1");
// 	}
// 	else if ( mll > 101 && mll < 120 ) {
// 		countSignalEvent("SRE_for_d.highMll_b_all");
// 		if (nbjets == 0) 
// 			countSignalEvent("SRE_for_d.highMll_b0");
// 		else
// 			countSignalEvent("SRE_for_d.highMll_b1");
// 	}
// 	if ( mll > 120 ) {
// 		countSignalEvent("SRE_for_e.vhighMll_b_all");
// 		if (nbjets == 0) 
// 			countSignalEvent("SRE_for_e.vhighMll_b0");
// 		else
// 			countSignalEvent("SRE_for_e.vhighMll_b1");
// 	}
// } // end forward region
 
}

void Cms_pas_sus_15_011::finalize() {
  // Whatever should be done after the run goes here
}       

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
template<class T>
bool isolateLepton(T* lepton, std::vector<Track*> tracks, double maxPT, double deltaR) {
    double pTsum = 0;
    double pTmin = 0.2;   

    for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
        if (lepton->Particle == (*it)->Particle) {
            continue;
        }
        if ((*it)->P4().DeltaR(lepton->P4()) > deltaR) {
            continue;
        }
        if ((*it)->PT < pTmin) {
            continue;
        }
        pTsum += (*it)->PT;
    }
    return pTsum < maxPT;
}

template<class T>
std::vector<T*> Cms_pas_sus_15_011::isolateLeptons(std::vector<T*> leptons, double maxpTrel) {
    std::vector<T*> isolated;
    for (typename std::vector<T*>::iterator it=leptons.begin(); it!=leptons.end(); it++) {
    	if ( (*it)->PT < 50 ) {
    		double dR = 0.2;
    		if (isolateLepton(*it, tracks, maxpTrel*((*it)->PT), dR)) {
            	isolated.push_back(*it);
        	}
    	}	
        else if ( (*it)->PT > 50 && (*it)->PT < 200) {
        	double dR = (double)( 10/((*it)->PT) );
        	if (isolateLepton(*it, tracks, maxpTrel*((*it)->PT), dR)) {
            	isolated.push_back(*it);
        	}
        }
        else {
        	double dR = 0.05;
        	if (isolateLepton(*it, tracks, maxpTrel*((*it)->PT), dR)) {
            	isolated.push_back(*it);
        	}
        }
    }
    return isolated;
}


