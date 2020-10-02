#ifndef ATLAS_CONF_2020_048_H_
#define ATLAS_CONF_2020_048_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_conf_2020_048 : public AnalysisBase {
  public:
    Atlas_conf_2020_048() : AnalysisBase()  {}               
    ~Atlas_conf_2020_048() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
      
      bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin);
      std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
      static bool sortByPT(Jet *i, Jet *j);
};

#endif
