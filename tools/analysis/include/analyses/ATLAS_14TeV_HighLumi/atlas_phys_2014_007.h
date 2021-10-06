#ifndef ATLAS_PHYS_2014_007_H_
#define ATLAS_PHYS_2014_007_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_phys_2014_007 : public AnalysisBase {
  public:
    Atlas_phys_2014_007() : AnalysisBase()  {}               
    ~Atlas_phys_2014_007() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
      bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin);
      std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
      static bool sortByPT(Jet *i, Jet *j);
      static std::string signal_regions[21];
      static int signal_region_borders[21][2];
};

#endif
