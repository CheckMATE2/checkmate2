#ifndef ATLAS_1803_02762_H_
#define ATLAS_1803_02762_H_
// AUTHOR: Ipsita Saha
//  EMAIL: ipsita.saha@ipmu.jp
#include "AnalysisBase.h"

class Atlas_1803_02762 : public AnalysisBase {
  public:
    Atlas_1803_02762() : AnalysisBase()  {}               
    ~Atlas_1803_02762() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;
      std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
};

#endif
