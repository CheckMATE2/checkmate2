#ifndef ATLAS_2202_07953_H_
#define ATLAS_2202_07953_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_2202_07953 : public AnalysisBase {
  public:
    Atlas_2202_07953() : AnalysisBase()  {}               
    ~Atlas_2202_07953() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;  
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);  
    template <class X, class Y>
    std::vector<X*> specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours);    
    template<class T> std::vector<T*> isolateLeptons(std::vector<T*> leptons, double deltaR);
    std::vector<Photon*> isolatePhotons(std::vector<Photon*> photons, double deltaR);    
    
};

#endif
