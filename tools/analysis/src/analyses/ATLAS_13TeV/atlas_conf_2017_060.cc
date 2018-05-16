#include "atlas_conf_2017_060.h"
// AUTHOR: J.S.Kim
//  EMAIL: jsk@th.physik.uni-bonn.de


std::string Atlas_conf_2017_060::signal_regions [20] = {"IM1","IM2","IM3","IM4","IM5","IM6","IM7","IM8","IM9","IM10","EM1","EM2","EM3","EM4","EM5","EM6","EM7","EM8","EM9","EM10"};
int Atlas_conf_2017_060::signal_region_borders [20][2] = {{250,-1},{300,-1},{350,-1},{400,-1},{500,-1},{600,-1},{700,-1},{800,-1},{900,-1},{1000,-1},{250,300},{300,350},{350,400},{400,500},{500,600},{600,700},{700,800},{800,900},{900,1000},{1000,-1}};


void Atlas_conf_2017_060::initialize() {
  setAnalysisName("atlas_conf_2017_060");          
  setInformation(""
    "# monojet search\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("IM1;IM2;IM3;IM4;IM5;IM6;IM7;IM8;IM9;IM10;EM1;EM2;EM3;EM4;EM5;EM6;EM7;EM8;EM9;EM10");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_conf_2017_060::analyze() {
  // Your eventwise analysis code goes here
  // The following objects are always defined unless they are 'ignored' above. They form std::vector objects of the respective Delphes class type (except for Etmiss which is a single object)
  // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
  // Within a std::vector, all members are ordered with highest pt coming first.

  // electronsLoose, electronsMedium, electronsTight   are list of electrons that passed respective efficiency and reconstruction cuts
  // muonsCombinedPlus, muonsCombined                  as above for muons
  // photonsMedium                                     as above for photons
  // jets are all reconstructed jets                   as above for jets. Note that electrons are most certainly also reconstructed as a jet -> overlap removal do avoid double counting necessary!
  // tracks, towers                                    calorimeter and tracker information. Usually not needed.
  // missingET                                         rec missing ET EXCLUDING muons.

  
  // Here is a couple of useful functions and lines:  
  //------------Phase Space Cuts (defined for jets, electronsXYZ, muonsXYZ, photonsXYZ)
  // jets = filterPhaseSpace(jets, 20., -2.8, 2.8)  // The vector 'jets' only contains jets with pt >= 20 GeV and -2.8 < eta < 2.8. This function is applicable to other particles too (electronsMedium, ... ).
  // jets = overlapRemoval(jets, electronsLoose, 0.2) Removes all jets for which there exists any electron in 'electronsLoose' with deltaR < 0.2.
  // jets = overlapRemovel(jets, 0.2) If two jets overlap within deltaR < 0.2, only the harder jet is stored.
  
  //------------Isolation Checks (defined for electronsXYZ, muonsXYZ, photonsXYZ
  //------------        For each object, if the user entered N isolation conditions, they can be
  //------------        checked individually be the second argument (running from 0 to N-1).
  // electronsMedium = filterIsolation(electronsMedium, 0)            Removes electrons that do not pass the first isolation condition entered into the AnalysisManager by the user
  // std::vector<int> flags; flags.push_back(0); flags.push_back(2);
  // electronsMedium = filterIsolation(electronsMedium, flags)        Same as above, but both the first and the third condition have to be fulfilled
  // electronsMedium = filterIsolation(electronsMedium)               Same as above, but all conditions have to be fulfilled.
  
  //-----------Flavour Tag Checks (defined for jets only)
  //----------          Tau tags "loose", "medium" or "tight" can be individually checked if the user enabled tau tagging in the AM.
  //----------          For b-tags, if N working points have been defined, the ith condition can be tested by putting i-1 into the second argument (if there is only one, the argument can be omitted)
  // if checkTauTag(jets[0], "tight") leadingJetIsTagged = True;
  // if checkBTag(jets[0], 0) leadingJetIsBTagged = True;


  //-----------Auxiliary Information
  // - Always ensure that you don't access vectors out of bounds. E.g. 'if(jets[1]->PT > 150)' should rather be if (jets.size() > 1 && jets[1]->PT > 150). 
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in CheckMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  

  //  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  missingET->addMuons(muonsCombined);  
 
  electronsLoose = filterPhaseSpace(electronsLoose, 20., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
     
  jets = overlapRemoval(jets, electronsLoose, 0.2);

  electronsLoose = overlapRemoval_2(electronsLoose,jets,0.2,0.4);
  
  overlapRemoval_jm();
  
  countCutflowEvent("0_nocuts");
  
  if (missingET->P4().Et()<70) return;
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
  
  //  int k=0;
  //for(int i=275;i<1450;i=i+50){
  //if( fabs(float(missingET->P4().Et())-float(i)) < 25 ) {
      //	  countCutflowEvent(cut_flow_x_hist[k]);
  //}
  //k++;
  // }
  
  for(int i=0;i<20;i++){
    if (missingET->P4().Et()<signal_region_borders[i][0]) continue;
    if (missingET->P4().Et()>signal_region_borders[i][1] && signal_region_borders[i][1]!=-1) continue;
    countSignalEvent(signal_regions[i]);
  }
  
 

  
}

//Muon Jet Overlap removal. Stolen from thorsten :D

void Atlas_conf_2017_060::overlapRemoval_jm() {
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
            if (Atlas_conf_2017_060::countMatchingTracks(tracks, jets[j], 0.4) >= 3) {
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
int Atlas_conf_2017_060::countMatchingTracks(std::vector<Track*> tracks, Jet* jet, float minpt) {
    int ntracks = 0;
    for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
        if (jet->P4().DeltaR((*it)->P4()) < 0.4 && ((*it)->PT > minpt || minpt==-1.0)) {
            ntracks++;
        }
    }
    return ntracks;
}

float Atlas_conf_2017_060::dPhi(std::vector<Jet*> jets, ETMiss* missingET){  
  
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

void Atlas_conf_2017_060::finalize() {
  // Whatever should be done after the run goes here
}       
