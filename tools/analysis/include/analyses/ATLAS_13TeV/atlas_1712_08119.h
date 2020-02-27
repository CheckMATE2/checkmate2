#ifndef ATLAS_1712_08119_H_
#define ATLAS_1712_08119_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1712_08119 : public AnalysisBase {
  public:
    Atlas_1712_08119() : AnalysisBase()  {}               
    ~Atlas_1712_08119() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    double mtautau(const TLorentzVector, const TLorentzVector, const TLorentzVector);
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;    
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);    
};

#endif
