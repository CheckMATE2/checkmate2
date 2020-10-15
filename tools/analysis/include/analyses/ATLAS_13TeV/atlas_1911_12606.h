#ifndef ATLAS_1911_12606_H_
#define ATLAS_1911_12606_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1911_12606 : public AnalysisBase {
  public:
    Atlas_1911_12606() : AnalysisBase()  {}               
    ~Atlas_1911_12606() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
      
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;    
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);  
    std::vector<Track*> filterLeptons(std::vector<Track*> cand_tracks);
    std::vector<Track*> Isolate_tracks(std::vector<Track*> cand_tracks, double dR, double mom);
};

#endif
