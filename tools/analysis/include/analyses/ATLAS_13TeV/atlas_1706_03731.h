#ifndef ATLAS_1706_03731_H_
#define ATLAS_1706_03731_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1706_03731 : public AnalysisBase {
  public:
    Atlas_1706_03731() : AnalysisBase()  {}               
    ~Atlas_1706_03731() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    std::vector<Jet*> sigjets;
    double met, meff;
    int nb;
    static bool sortByPT(Jet *i, Jet *j);
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;
    
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
    
    template <class X, class Y>
    std::vector<X*> specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours);    
    
    void Passes_Cuts(int NbJet, int NJet, double J_PT, double MET_cut, double Meff_cut, double METMeff_cut, bool cutflow, std::string sr); 
};

#endif
