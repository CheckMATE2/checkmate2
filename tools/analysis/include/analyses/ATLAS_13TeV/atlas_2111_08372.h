#ifndef ATLAS_2111_08372_H_
#define ATLAS_2111_08372_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"
#include "TH2F.h"

class Atlas_2111_08372 : public AnalysisBase {
  public:
    Atlas_2111_08372() : AnalysisBase()  {}               
    ~Atlas_2111_08372() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    TFile *hfile;  TH1F *mT_hist, *MET_hist; TH2D *mT_MET;
    
    template <class X>
std::vector<X*> PFisolation(std::vector<X*> leptons, std::vector<Track*> tracks, std::vector<Tower*> towers, double dR_track_max, double pT_for_inverse_function_track, double dR_tower, double pT_amount);
};

#endif
