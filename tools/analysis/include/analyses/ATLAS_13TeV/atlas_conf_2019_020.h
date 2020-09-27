#ifndef ATLAS_CONF_2019_020_H_
#define ATLAS_CONF_2019_020_H_
// AUTHOR: Ipsita Saha
//  EMAIL: ipsita.saha@ipmu.jp
#include "AnalysisBase.h"

class Atlas_conf_2019_020 : public AnalysisBase {
  public:
    Atlas_conf_2019_020() : AnalysisBase()  {}               
    ~Atlas_conf_2019_020() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;
      std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);


    template <class X, class Y>
      static std::vector<X*> overlapRemoval_inverse_cone(std::vector<X*> candidates, std::vector<Y*> neighbours, double dR_max, double pT_max, double pT_for_inverse_function, std::string pseudorapidity="eta") {
      // If neighbours are empty, return candidates
      if(neighbours.size() == 0)
        return candidates;
      std::vector<X*> passed_candidates;
      // Loop over candidates
      for(int i = 0; i < candidates.size(); i++) {
	if(candidates[i]->PT > 25.) dR_max = max(0.2, 0.6-0.0080*candidates[i]->PT); 
        bool overlap = false;
        // If a neighbour is too close, declare overlap, break and don't save candidate
        for(int j = 0; j < neighbours.size(); j++) {
          if(pseudorapidity=="eta"){
            if (candidates[i]->P4().DeltaR(neighbours[j]->P4()) < dR_max) {
              overlap = true;
              break;
            }
          }
          if(pseudorapidity=="y"){
	    double dR = 0;
            double y1   = 1./2.*log(  ((candidates[i]->P4()).E() + (candidates[i]->P4()).Pz())
            						/((candidates[i]->P4()).E() -(candidates[i]->P4()).Pz()) );
            double y2   = 1./2.*log(  ((neighbours[j]->P4()).E() + (neighbours[j]->P4()).Pz())
            						/((neighbours[j]->P4()).E() -(neighbours[j]->P4()).Pz()) );
        	double dy   = fabs(y1-y2);
        	double dPhi = candidates[i]->P4().DeltaPhi(neighbours[j]->P4());
        	double dR_y = sqrt(dy*dy + dPhi*dPhi);

		if(candidates[i]->PT < pT_max){
     			dR = pT_for_inverse_function/candidates[i]->PT;
               	 if(dR >dR_max){
               	 dR=dR_max;
               	 }

		      if (dR_y < dR) {
        	      overlap = true;
             		 break;
		      }
        	}
	   }
        if (!overlap)
          passed_candidates.push_back(candidates[i]);
      }
     }

	return passed_candidates;
      }


    template <class X>
    std::vector<X*> Special_Isolate_muons(std::vector<X*> leptons,std::vector<Track*> tracks,std::vector<Tower*> towers,double dR_track_max,double pT_for_inverse_function_track,double dR_tower,double pT_amount_track,double pT_amount_tower,bool checkTower){
      std::vector<X*> filtered_leptons;
      for(int i=0;i<leptons.size();i++){
		if(leptons[i]->PT > 33.) dR_track_max = max(0.2,0.4941-0.005882*leptons[i]->PT);
	        double dR_track=0;
        	double sumPT=0;
	        double sumET=0;
        	dR_track=pT_for_inverse_function_track/leptons[i]->PT;
	        if(dR_track >dR_track_max){
        		  dR_track=dR_track_max;
	        }
        	for (int t = 0; t < tracks.size(); t++) {
	      	    Track* neighbour = tracks[t];

	  // Ignore the lepton's track itself
	          if(neighbour->Particle == leptons[i]->Particle)
        	    continue;
	          if (neighbour->P4().DeltaR(leptons[i]->P4()) > dR_track)
        	    continue;
	          sumPT += neighbour->PT;
        	}
	        if((leptons[i]->PT)*pT_amount_track<=sumPT){
        	  continue;
        	}
	        if(checkTower){
        	  for (int t = 0; t < towers.size(); t++) {
	            Tower* neighbour = towers[t];
	    
            // check tower has 'some' momentum and check dR
        	    if (neighbour->ET < 0.00001 || neighbour->P4().DeltaR(leptons[i]->P4()) > dR_tower)
	              continue;
        	    // Ignore the lepton's tower
           	 bool candidatesTower = false;//This testing is different from the testing in the tracks case, because to one track there corresponds one particle, but for one tower there is not only one particle.
	            for(int p = 0; p < neighbour->Particles.GetEntries(); p++){
        	      if (neighbour->Particles.At(p) == leptons[i]->Particle) {
                // break the loop and ignore the tower
               		candidatesTower = true;
	                break;
        	      }
	            }
        	    if (candidatesTower)
	              continue;
       	     sumET += neighbour->ET;
        	  }
	          if((leptons[i]->PT)*pT_amount_tower<=sumET){
       	     continue;
        	  }
	        }
       	 filtered_leptons.push_back(leptons[i]);
	}
      return filtered_leptons;
    }
};
#endif
