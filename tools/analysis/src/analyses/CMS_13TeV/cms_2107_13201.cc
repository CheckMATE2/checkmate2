#include "cms_2107_13201.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Cms_2107_13201::initialize() {
  setAnalysisName("cms_2107_13201");          
  setInformation(""
    "# monojet with multibin\n"
  "");
  setLuminosity(137.0*units::INVFB);      
  bookSignalRegions("A_00;A_01;A_02;A_03;A_04;A_05;A_06;A_07;A_08;A_09;A_10;A_11;A_12;A_13;A_14;A_15;A_16;A_17;A_18;A_19;A_20;A_21;B_2016_00;B_2016_01;B_2016_02;B_2016_03;B_2016_04;B_2016_05;B_2016_06;B_2016_07;B_2016_08;B_2016_09;B_2016_10;B_2016_11;B_2016_12;B_2016_13;B_2016_14;B_2016_15;B_2016_16;B_2016_17;B_2016_18;B_2016_19;B_2016_20;B_2016_21;B_2017_00;B_2017_01;B_2017_02;B_2017_03;B_2017_04;B_2017_05;B_2017_06;B_2017_07;B_2017_08;B_2017_09;B_2017_10;B_2017_11;B_2017_12;B_2017_13;B_2017_14;B_2017_15;B_2017_16;B_2017_17;B_2017_18;B_2017_19;B_2017_20;B_2017_21;B_2018_00;B_2018_01;B_2018_02;B_2018_03;B_2018_04;B_2018_05;B_2018_06;B_2018_07;B_2018_08;B_2018_09;B_2018_10;B_2018_11;B_2018_12;B_2018_13;B_2018_14;B_2018_15;B_2018_16;B_2018_17;B_2018_18;B_2018_19;B_2018_20;B_2018_21");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_2107_13201::analyze() {
 
  //missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
 
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.5, 2.5);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);  
  photonsMedium = filterPhaseSpace(photonsMedium, 15., -2.5, 2.5);  
  
  photonsMedium = filterIsolation( photonsMedium); 
  electronsMedium = Isolate_leptons_with_variable_track_isolation_cone_CMS(electronsMedium, 0.2, 0.05, 10., 0.1);
  muonsCombined = Isolate_leptons_with_variable_track_isolation_cone_CMS(muonsCombined, 0.2, 0.05, 10., 0.2);
  
  jets = filterPhaseSpace(jets, 20., -2.4, 2.4);
  jets = overlapRemoval(jets, electronsMedium, 0.3, "y");
  jets = overlapRemoval(jets, muonsCombined, 0.3, "y");
  jets = overlapRemoval(jets, photonsMedium, 0.3, "y");    
  
  double MET = missingET->PT;
    
  countCutflowEvent("00_All");
  
  if (MET <  120.) return;
  countCutflowEvent("01_Trigger");
  
  if (MET <  250.) return;
  countCutflowEvent("02_MET>250");

  if (electronsMedium.size()) return;
  countCutflowEvent("03_el_veto");  
  
  if (muonsCombined.size()) return;
  countCutflowEvent("04_mu_veto");    
  
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta)<2.3 and checkTauTag(jets[i], "medium") ) return;    
  countCutflowEvent("05_tau_veto");    

  for (int i = 0; i < jets.size(); i++) 
    if (checkBTag(jets[i]) ) return;  
  countCutflowEvent("06_B_veto");    
  
  if (photonsMedium.size()) return;
  countCutflowEvent("07_photon_veto");    
  
  for (int i = 0; i < jets.size(); i++)
    if ( fabs(missingET->P4().DeltaPhi(jets[i]->P4())) < 0.5) return;
  countCutflowEvent("08_DPhi(MET,jet)");      
  
  if (!jets.size()) return;
  if (jets[0]->PT < 100.) return;
  countCutflowEvent("09_jet1>100");      
  
  std::string year;
  double batch = rand()/(RAND_MAX + 1.);
  if (batch < 36./137.2) year = "2016";
  else if (batch < (36.+41.5)/137.2 ) year = "2017";
  else year = "2018";
    
  if (MET < 280.) {
    countSignalEvent("A_00");
    countSignalEvent("B_"+year+"_00");    
  }
  else if (MET < 310.) {
    countSignalEvent("A_01");
    countSignalEvent("B_"+year+"_01");    
  }
  else if (MET < 340.) {
    countSignalEvent("A_02");
    countSignalEvent("B_"+year+"_02");    
  }
  else if (MET < 370.) {
    countSignalEvent("A_03");
    countSignalEvent("B_"+year+"_03");    
  }  
  else if (MET < 400.) {
    countSignalEvent("A_04");
    countSignalEvent("B_"+year+"_04");    
  }
  else if (MET < 430.) {
    countSignalEvent("A_05");
    countSignalEvent("B_"+year+"_05");    
  }  
  else if (MET < 470.) {
    countSignalEvent("A_06");
    countSignalEvent("B_"+year+"_06");    
  } 
  else if (MET < 510.) {
    countSignalEvent("A_07");
    countSignalEvent("B_"+year+"_07");    
  } 
  else if (MET < 550.) {
    countSignalEvent("A_08");
    countSignalEvent("B_"+year+"_08");    
  }  
  else if (MET < 590.) {
    countSignalEvent("A_09");
    countSignalEvent("B_"+year+"_09");    
  }  
  else if (MET < 640.) {
    countSignalEvent("A_10");
    countSignalEvent("B_"+year+"_10");    
  }  
  else if (MET < 690.) {
    countSignalEvent("A_11");
    countSignalEvent("B_"+year+"_11");    
  }
  else if (MET < 740.) {
    countSignalEvent("A_12");
    countSignalEvent("B_"+year+"_12");    
  }  
  else if (MET < 790.) {
    countSignalEvent("A_13");
    countSignalEvent("B_"+year+"_13");    
  }  
  else if (MET < 840.) {
    countSignalEvent("A_14");
    countSignalEvent("B_"+year+"_14");    
  }  
  else if (MET < 900.) {
    countSignalEvent("A_15");
    countSignalEvent("B_"+year+"_15");    
  }  
  else if (MET < 960.) {
    countSignalEvent("A_16");
    countSignalEvent("B_"+year+"_16");    
  }  
  else if (MET < 1020.) {
    countSignalEvent("A_17");
    countSignalEvent("B_"+year+"_17");    
  }  
  else if (MET < 1090.) {
    countSignalEvent("A_18");
    countSignalEvent("B_"+year+"_18");    
  }
  else if (MET < 1160.) {
    countSignalEvent("A_19");
    countSignalEvent("B_"+year+"_19");    
  }  
  else if (MET < 1250.) {
    countSignalEvent("A_20");
    countSignalEvent("B_"+year+"_20");    
  }  
  else  {
    countSignalEvent("A_21");
    countSignalEvent("B_"+year+"_21");    
  }  
  
  return;  
}

void Cms_2107_13201::finalize() {
  // Whatever should be done after the run goes here
}       


template <class X>
std::vector<X*> Cms_2107_13201::Isolate_leptons_with_variable_track_isolation_cone_CMS(std::vector<X*> leptons, double dR_track_max, double dR_track_min, double pT_for_inverse_function_track, double pT_amount_track ) {
      
  std::vector<X*> filtered_leptons;
  for(int i = 0; i < leptons.size(); i++) {
    double dR_track = 0.;
    double sumPT = 0.;
    dR_track = std::min(dR_track_max, std::max(pT_for_inverse_function_track/leptons[i]->PT, dR_track_min));
        
    for (int t = 0; t < tracks.size(); t++) {
      Track* neighbour = tracks[t];

	  // Ignore the lepton's track itself
      if(neighbour->Particle == leptons[i]->Particle) continue;
      if(neighbour->PT < 1.) continue;
      if (neighbour->P4().DeltaR(leptons[i]->P4()) > dR_track) continue;
      sumPT += neighbour->PT;
    }
    
    if( (leptons[i]->PT)*pT_amount_track > sumPT) filtered_leptons.push_back(leptons[i]);
  }
    
    return filtered_leptons;
}
