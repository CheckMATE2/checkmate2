#include "atlas_1604_07773.h"
#include <math.h> 
#include <stdio.h>
// AUTHOR: Tim Keller
//  EMAIL: tim.keller@rwth-aachen.de

  
std::string Atlas_1604_07773::signal_regions [13] = {"IM1","IM2","IM3","IM4","IM5","IM6","IM7","EM1","EM2","EM3","EM4","EM5","EM6"};
int Atlas_1604_07773::signal_region_borders [13][2] = {{250,-1},{300,-1},{350,-1},{400,-1},{500,-1},{600,-1},{700,-1},{250,300},{300,350},{350,400},{400,500},{500,600},{600,700}};
std::string Atlas_1604_07773::cut_flow_x_hist [24] = {"x_hist_1","x_hist_2","x_hist_3","x_hist_4","x_hist_5","x_hist_6","x_hist_7","x_hist_8","x_hist_9","x_hist_10","x_hist_11","x_hist_12","x_hist_13","x_hist_14","x_hist_15","x_hist_16","x_hist_17","x_hist_18","x_hist_19","x_hist_20","x_hist_21","x_hist_22","x_hist_23","x_hist_24"}; 


void Atlas_1604_07773::initialize() {
  setAnalysisName("atlas_1604_07773");          
  setInformation(""
    "# Search for new phenomena in final states with an energetic jet and large missing transverse momentum in pp colliATLAS detector\n"
  "");
  setLuminosity(3.2*units::INVFB);      
  bookSignalRegions("IM1;IM2;IM3;IM4;IM5;IM6;IM7;EM1;EM2;EM3;EM4;EM5;EM6");
  bookCutflowRegions("a_All;b_Vertex;g_MeT1;c_NoLep;f_LeadingJetCut;d_SubleadingJetCut;e_dPhiCut;x_hist_1;x_hist_2;x_hist_3;x_hist_4;x_hist_5;x_hist_6;x_hist_7;x_hist_8;x_hist_9;x_hist_10;x_hist_11;x_hist_12;x_hist_13;x_hist_14;x_hist_15;x_hist_16;x_hist_17;x_hist_18;x_hist_19;x_hist_20;x_hist_21;x_hist_22;x_hist_23;x_hist_24");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1604_07773::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
 
  electronsLoose = filterPhaseSpace(electronsLoose, 20., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
     
  jets = overlapRemoval(jets, electronsLoose, 0.2);

  electronsLoose = overlapRemoval_2(electronsLoose,jets,0.2,0.4);
  
  overlapRemoval_jm();
    
  if (missingET->P4().Et()<100) return;
  if( jets.size() < 1 ) return;
  
  countCutflowEvent("a_All");
  
  //Require primary vertex with atleast 2 0.4 GeV Tracks
  countCutflowEvent("b_Vertex");
  
  if(electronsLoose.size()>0) return;
  
  if(muonsCombined.size()>0) return;
  countCutflowEvent("c_NoLep");
  
    std::vector<FinalStateObject*> jetsSignal;
  for ( int i = 0; i < jets.size(); i++ ) {
    if ( jets[i]->PT > 30. and abs(jets[i]->P4().Eta())<2.8 ){
      FinalStateObject* fso = newFinalStateObject(jets[i]);
      jetsSignal.push_back(fso);
    }
  }
  
  std::sort(jetsSignal.begin(), jetsSignal.end(), sortByPT);
    
  if (jetsSignal.size() > 4) return;
  countCutflowEvent("d_SubleadingJetCut");
  
  if( fabs(dPhi(jets,missingET)) < 0.4 ) return;
  countCutflowEvent("e_dPhiCut");
  
  if( !(jets[0]->PT > 250 && fabs(jets[0]->P4().Eta())<2.4 ) ) return;
  countCutflowEvent("f_LeadingJetCut");
  
  if (missingET->P4().Et()<250) return;
  countCutflowEvent("g_MeT1");
  
  int k=0;
  for(int i=275;i<1450;i=i+50){
    if( fabs(float(missingET->P4().Et())-float(i)) < 25 ) {
	  countCutflowEvent(cut_flow_x_hist[k]);
    }
    k++;
  }
  
  for(int i=0;i<13;i++){
    if (missingET->P4().Et()<signal_region_borders[i][0]) continue;
    if (missingET->P4().Et()>signal_region_borders[i][1] && signal_region_borders[i][1]!=-1) continue;
    countSignalEvent(signal_regions[i]);
  }
  
 
}

//Muon Jet Overlap removal. Stolen from thorsten :D

void Atlas_1604_07773::overlapRemoval_jm() {
    // NOTE: Efficiency?
    std::vector<Muon*> surviving_muons;
    std::vector<bool> jet_overlaps(jets.size(), false);
    for (std::vector<Muon*>::const_iterator itm=muonsCombined.begin(); itm!=muonsCombined.end(); itm++) {
        bool muon_overlap = false;
        // NOTE: It's not clear from the paper if this should be the muon-pT or the jet-pT.
        double minDR = 0.4;
        for (int j=0; j<jets.size(); j++) {
            if (fabs((*itm)->P4().DeltaR(jets[j]->P4())) >= minDR) {
                continue;
            }
            if (Atlas_1604_07773::countMatchingTracks(tracks, jets[j], 0.4) >= 3) {
                // remove muon
                muon_overlap = true;
            } else {
                // remove jet
                jet_overlaps[j] = true;
            }

        }
        if (!muon_overlap) {
            surviving_muons.push_back((*itm));
        }
    }
    std::vector<Jet*> surviving_jets;
    for (int i=0; i<jet_overlaps.size(); i++) {
        if (!(jet_overlaps[i])) {
            surviving_jets.push_back(jets[i]);
        }
    }
    muonsCombined=surviving_muons;
    jets=surviving_jets;
}

// The original jet finder algorithm used a DeltaR of 0.4, so do that again
int Atlas_1604_07773::countMatchingTracks(std::vector<Track*> tracks, Jet* jet, float minpt) {
    int ntracks = 0;
    for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
        if (jet->P4().DeltaR((*it)->P4()) < 0.4 && ((*it)->PT > minpt || minpt==-1.0)) {
            ntracks++;
        }
    }
    return ntracks;
}

float Atlas_1604_07773::dPhi(std::vector<Jet*> jets, ETMiss* missingET){  
  
  double dphimin=0;
  for(int k=0;k<std::min(int(jets.size()),4);k++){
    if( k==0 ){ 
      dphimin = fabs( jets[k]->P4().DeltaPhi( missingET->P4() ) ); 
    }else{
      dphimin = std::min(fabs( jets[k]->P4().DeltaPhi( missingET->P4() ) ),dphimin);
    }
  }
  return dphimin;
}

void Atlas_1604_07773::finalize() {
  // Whatever should be done after the run goes here
}       
