#ifndef ATLAS_2106_01676_H_
#define ATLAS_2106_01676_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_2106_01676 : public AnalysisBase {
  public:
    Atlas_2106_01676() : AnalysisBase()  {}               
    ~Atlas_2106_01676() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    static bool sortByPTEl(Electron *i, Electron *j);
    static bool sortByPTMu(Muon *i, Muon *j);
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;    
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);  
};

#endif
