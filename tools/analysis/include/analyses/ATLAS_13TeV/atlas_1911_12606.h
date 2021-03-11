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
      
    int n;  
    TLorentzVector pTmiss;
    std::vector<FinalStateObject*> leptons;
    std::vector<Track*> signal_tracks;
    std::vector<Jet*> signal_jets;
    std::vector<Electron> electrons_soft_true;
    std::vector<Muon> muons_soft_true;
    int nb, flavour;
    double mll, mlt, met, mt2;
    bool Pass_Cuts_1L1T();
    bool Pass_Preselection();
    bool Pass_EW_high();
    bool Pass_EW_med();
    bool Pass_EW_low();
    bool Pass_S_high();
    bool Pass_S_low();
    double mtautau(std::vector<FinalStateObject*> lep);
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;    
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);  
    std::vector<Track*> filterLeptons(std::vector<Track*> cand_tracks);
    std::vector<Track*> Isolate_tracks(std::vector<Track*> cand_tracks, double dR, double mom);
    static bool sortByPT(Jet *i, Jet *j);
    double mT2chi2(const TLorentzVector & vis1, const TLorentzVector & vis2, double m_inv, const TLorentzVector & invis);
};

#endif
