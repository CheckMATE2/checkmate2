#ifndef ATLAS_2010_14293_H_
#define ATLAS_2010_14293_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"
#include "TMVA/Reader.h"

class Atlas_2010_14293 : public AnalysisBase {
  public:
    Atlas_2010_14293() : AnalysisBase()  {}               
    ~Atlas_2010_14293() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    TFile *hfile;  TH1F *ggd1, *ggd2, *ggd3, *ggd4, *ggo1, *ggo2, *ggo3, *ggo4;  
    
    double met;
    
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
    bool Passes_Cuts(std::vector<Jet*> jets, double PT1Cut, double PT2Cut, int NJet, double EtaCut, double dPhiCut1, double dPhiCut2, double METHTCut, double AplanarityCut, double MeffCut, bool cutflow, std::string sr); 
    bool Passes_Cuts_MB(std::vector<Jet*> jets, double PT1Cut, double PT2Cut, int NJetMin, int NJetMax, double EtaCut, double dPhiCut1, double dPhiCut2, double METHTMin, double METHTMax, double AplanarityCut, double MeffCutMin, double MeffCutMax, bool cutflow, std::string sr);
    bool Passes_Cuts_BDT(std::vector<Jet*> jets, int NJetMin, double dPhiCut1, double dPhiCut2, double METMeff, double MeffMin, double BDTscore, int rder, bool cutflow, std::string sr);
    void evaluateCRs(std::vector<Jet*> jets, std::string type);
    bool Passes_Cuts_CRQ(std::vector<Jet*> jets, double PT1Cut, double PT2Cut, int NJetMin, int NJetMax, double EtaCut, double dPhiCut1, double dPhiCut2, double METHTMax, double AplanarityCut, double MeffMin, double MeffMax, bool cutflow, std::string sr);
    void evaluateCRQs(std::vector<Jet*> jets);
    
    Float_t met_aux, meff_aux, Ap, jetPt0, jetPt1, jetPt2, jetPt3, jetPt4, jetPt5, jetEta0, jetEta1, jetEta2, jetEta3, jetEta4, jetEta5;
    TMVA::Reader *reader[8][2];
};

#endif
