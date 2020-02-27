#ifndef ATLAS_CONF_2016_054_H_
#define ATLAS_CONF_2016_054_H_
// AUTHOR: J.S. Kim
//  EMAIL: jongsoo.kim@tu-dortmund.de
#include "AnalysisBase.h"

class Atlas_conf_2016_054 : public AnalysisBase {
  public:
    Atlas_conf_2016_054() : AnalysisBase()  {}               
    ~Atlas_conf_2016_054() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

void overlap_muons_jets_with_track_conditions_2(std::vector<Muon*>& muons,std::vector<Jet*>& jets,std::vector<Track*>& tracks,double dR,int numberOfTracks, double pT_threshold);

TMatrixD Get_Momentum_Matrix_2(std::vector<Jet*> jets);


#endif
