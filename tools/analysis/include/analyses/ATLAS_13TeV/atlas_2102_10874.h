#ifndef ATLAS_2102_10874_H_
#define ATLAS_2102_10874_H_
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_2102_10874 : public AnalysisBase {
  public:
    Atlas_2102_10874() : AnalysisBase()  {}               
    ~Atlas_2102_10874() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:

      int nel, nmu, ngam, ntau, nb;
      bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin);
      std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
      static bool sortByPT(Jet *i, Jet *j);
      static std::string signal_regions[26];
      static int signal_region_borders[26][2];
      void pass_SR(std::vector<Jet*> sigjets) ;
      void pass_CR1el(std::vector<Jet*> sigjets, std::vector<Electron*> electrons);
      void pass_CR1mu(std::vector<Jet*> sigjets) ;
      void pass_CR2el(std::vector<Jet*> sigjets, std::vector<Electron*> electrons) ;
      void pass_CR2mu(std::vector<Jet*> sigjets) ;
};

#endif
