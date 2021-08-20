#ifndef ATLAS_1807_07447_H_
#define ATLAS_1807_07447_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1807_07447 : public AnalysisBase {
  public:
    Atlas_1807_07447() : AnalysisBase()  {}               
    ~Atlas_1807_07447() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    std::string sigregions;
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;
    
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
    
    template<class T> std::vector<T*> isolateLeptons(std::vector<T*> leptons, double deltaR);
    
    std::vector<Photon*> isolatePhotons(std::vector<Photon*> photons, double deltaR);
};

#endif
