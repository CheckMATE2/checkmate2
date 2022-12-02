#ifndef ATLAS_2211_08028_H_
#define ATLAS_2211_08028_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_2211_08028 : public AnalysisBase {
  public:
    Atlas_2211_08028() : AnalysisBase()  {}               
    ~Atlas_2211_08028() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    std::vector<Jet*> signal_jets = {};
    std::vector<Jet*> bjets = {};
    std::vector<Electron*> signal_el = {};
    std::vector<Muon*> signal_mu = {};
    std::vector<FinalStateObject*> signalLeps = {};
    TLorentzVector pTmiss = TLorentzVector(0.,0.,0.,0.);
    double mT_lep = 0.;
    double mT_b = 0.;
    double meff = 0.;
    double MJ = 0.;
    double dphimin = 0.;
    
    static bool sortByPT(Jet *i, Jet *j);
    double get_meff();
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin);
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
    bool PassesCuts_Gtt1L(int Njet, int Nbjet, double met, double meff_cut, double mT_cut, double mTb_cut, double MJ_cut, std::string sr);
    bool PassesCuts_Gtt0L(int Njet, int Nbjet, double met, double meff_cut, double mTb_cut, double MJ_cut, std::string sr);
    bool PassesCuts_Gtb(int Njet, int Nbjet, double meff_cut, double met, double MJ_cut, std::string sr);
    bool PassesCuts_Gbb(int Njet, int Nbjet, double met, double ptj1, double meff_cut, std::string sr);
};

#endif
