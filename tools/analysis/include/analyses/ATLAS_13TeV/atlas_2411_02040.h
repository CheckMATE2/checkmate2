#ifndef ATLAS_2411_02040_H_
#define ATLAS_2411_02040_H_
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_2411_02040 : public AnalysisBase {
  public:
    Atlas_2411_02040() : AnalysisBase()  {}               
    ~Atlas_2411_02040() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    static bool sortByPT(Jet *i, Jet *j);
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin);
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
};

#endif
