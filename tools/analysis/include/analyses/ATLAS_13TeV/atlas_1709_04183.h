#ifndef ATLAS_1709_04183_H_
#define ATLAS_1709_04183_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1709_04183 : public AnalysisBase {
  public:
    Atlas_1709_04183() : AnalysisBase()  {}               
    ~Atlas_1709_04183() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    TFile *hfile;  TH1F *massW1, *massW2, *masst1, *masst2;
    
    TLorentzVector pTmiss;
    
    std::vector<TLorentzVector> find_Wcandidates( std::vector<Jet*> &jets_in );
    std::vector<TLorentzVector> find_Wcandidates3( std::vector<Jet*> &jets_in );
    std::vector<TLorentzVector> find_tcandidates( std::vector<TLorentzVector> Wcandidates, std::vector<Jet*> &jets_in );
    double mT2chi2(const TLorentzVector & vis1, const TLorentzVector & vis2, double m_inv, const TLorentzVector & invis);
    double mTbmin(std::vector<Jet*> bjets);
    double mTbmax(std::vector<Jet*> bjets);
    bool SRA( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::vector<fastjet::PseudoJet> largeR12, std::vector<fastjet::PseudoJet> largeR08, double lR1_cutlow, double lR1_cuthigh, double deltaRbb, double mchi_T2, double met, std::string cf);
    bool SRB( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, std::vector<fastjet::PseudoJet> largeR12, double lR1_cutlow, double lR1_cuthigh, std::string cf);
    bool SRD( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<Jet*> nonbjets, double pTb_sum, double pTj3, double mTbmin_cut, double mTbmax_cut, std::string cf);
    bool SRE( std::vector<Jet*> jets, std::vector<Jet*> bjets, std::vector<fastjet::PseudoJet> largeR08, std::string cf);    
};

#endif
