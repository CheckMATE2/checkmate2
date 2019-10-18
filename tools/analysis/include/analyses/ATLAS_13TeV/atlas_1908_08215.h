#ifndef ATLAS_1908_08215_H_
#define ATLAS_1908_08215_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1908_08215 : public AnalysisBase {
  public:
    Atlas_1908_08215() : AnalysisBase()  {}               
    ~Atlas_1908_08215() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;    
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);    
};

#endif
