#ifndef ATLAS_2004_14060_H_
#define ATLAS_2004_14060_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_2004_14060 : public AnalysisBase {
  public:
    Atlas_2004_14060() : AnalysisBase()  {}               
    ~Atlas_2004_14060() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
      
    TLorentzVector pTmiss, pTmiss_track; double ht, met, resolution;
    std::vector<TLorentzVector> trackjets, btrackjets; std::vector<Track*> softtracks;
    
    std::vector<TLorentzVector> find_Wcandidates( std::vector<Jet*> &jets_in );
    std::vector<TLorentzVector> find_Wcandidates3( std::vector<Jet*> &jets_in );
    std::vector<TLorentzVector> find_tcandidates( std::vector<TLorentzVector> Wcandidates, std::vector<Jet*> &jets_in );
    double mT2chi2(const TLorentzVector & vis1, const TLorentzVector & vis2, double m_inv, const TLorentzVector & invis);
    double mTbmin(std::vector<Jet*> bjets);
    double mTbmax(std::vector<Jet*> bjets);
    bool SRA( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::vector<fastjet::PseudoJet> largeR12, std::vector<fastjet::PseudoJet> largeR08, double lR1_cutlow, double lR1_cuthigh, double deltaRbb, std::string cf);
    bool SRB( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::vector<fastjet::PseudoJet> largeR12, double lR1_cutlow, double lR1_cuthigh, std::string cf);
    bool SRD( std::vector<Jet*> nonbjets, std::string cf); 
    bool SRD0(  std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::string cf );
    bool SRD1( std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::string cf );
    bool SRD2( std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::string cf );
    std::vector<TLorentzVector> findtrackjets(Jet* leading );
    bool trackbtag( TLorentzVector jetin);
    void rotateXY(TMatrix &mat, TMatrix &mat_new, double phi);
    double calcMETSignificance(std::vector<Jet*> jets);
};

#endif
