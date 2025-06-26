#ifndef ATLAS_2106_01676_H_
#define ATLAS_2106_01676_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_2106_01676 : public AnalysisBase {
  public:
    Atlas_2106_01676() : AnalysisBase()  {}               
    ~Atlas_2106_01676() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    TLorentzVector pair;
    TLorentzVector pTmiss;
    TLorentzVector truemet;
    FinalStateObject* lepton3;
    std::vector<Muon*> muons_off;
    std::vector<Electron*> electrons_off;
    std::vector<FinalStateObject*> leptons;
    std::vector<Jet*> jets_off, jets_signal;
    double mllmin, HT, mllmax, drmin;
    bool trigger, bveto, SFOS; 
    static bool sortByPTEl(Electron *i, Electron *j);
    static bool sortByPTMu(Muon *i, Muon *j);
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;    
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);  
    bool preselection_offWZ(bool cutflow);
    bool preselection_onWZ(bool cutflow);
    bool preselection_Wh(bool cutflow);
    bool preselection_WhDF(bool cutflow);
    bool pass_SRWhDF0j(bool cutflow);
    bool pass_SRoffWZhighnj( double min, double max, double ptl, std::string bin, bool cutflow);
    bool pass_SRoffWZhigh0j( double min, double max, double mTminmll, std::string bin, bool cutflow);
    bool pass_SRoffWZlow( double min, double max, double mTminmll, double ptl, bool jets, std::string bin, bool cutflow);
    double mT2chi2(const TLorentzVector & vis1, const TLorentzVector & vis2, double m_inv, const TLorentzVector & invis);
};

#endif
