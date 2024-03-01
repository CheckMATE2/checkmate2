#ifndef ATLAS_2209_13935_H_
#define ATLAS_2209_13935_H_
// AUTHOR: Inaki Lara
//  EMAIL: i.laraperez@uw.edu.pl
#include "AnalysisBase.h"
#include "MVAUtils/BDT.h"
#include "TTree.h"

class Atlas_2209_13935 : public AnalysisBase {
  public:
    Atlas_2209_13935() : AnalysisBase()  {}               
    ~Atlas_2209_13935() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    ofstream f1;
    std::vector<Electron*> signal_el = {};
    std::vector<Muon*> signal_mu = {};
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) ;
    double  check_track_pt(Jet* jet, std::vector<Track*> tracks) ;
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
    void  rotateXY(TMatrix &mat, TMatrix &mat_new, double phi);
    double calcMETSignificance(std::vector<FinalStateObject*> objects,double met);
    
    int eventNumber;
    /*
    MVAUtils::BDT *bdt_DF[2];
    MVAUtils::BDT *bdt_SF[2];
    void MVAUtilsReader(const std::string &name,
                               const std::string fname1,
                               const std::string fname2, MVAUtils::BDT *m_bdt1, MVAUtils::BDT *m_bdt2);
    std::vector<double> evaluateBDT(const std::vector<double> &values,
                              int numClasses, MVAUtils::BDT *m_bdt[2]);
                              */
    
};

#endif
