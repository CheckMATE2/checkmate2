#ifndef ATLAS_1712_02332_H_
#define ATLAS_1712_02332_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1712_02332 : public AnalysisBase {
  public:
    Atlas_1712_02332() : AnalysisBase()  {}               
    ~Atlas_1712_02332() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    static bool sortByPT(Jet *i, Jet *j);
//    static TMatrixD Get_Momentum_Matrix(std::vector<Jet*> jets);
//    bool Aplanarity_Cut( std::vector<Jet*> jets, double cut);
    double Aplanarity(std::vector<Jet*> input_jets);
    double check_track_pt(Jet* jet, std::vector<Track*> tracks);
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;
    
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
    std::vector<Jet*> overlapRemoval_muon_jet_tracks2(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR );
    
    template <class X, class Y>
    std::vector<X*> specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours);
    
    double dPhi(std::vector<Jet*> jets, int j);
    double M_eff(std::vector<Jet*> jets, int j);
    double HT(std::vector<Jet*> jets);
    bool Passes_Cuts(std::vector<Jet*> jets, double PT1Cut, double PT2Cut, int NJet, double EtaCut, double dPhiCut1, double dPhiCut2, double METHTCut, double AplanarityCut, double METMeffCut, double MeffCut, bool cutflow, std::string sr); 
};

#endif
