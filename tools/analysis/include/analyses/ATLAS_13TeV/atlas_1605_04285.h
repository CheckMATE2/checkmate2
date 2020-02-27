#ifndef _ATLAS_1605_04285
#define _ATLAS_1605_04285
// AUTHOR: Jan Schuteet-Engel
//  EMAIL: schuette@physik.rwth-aachen.de
#include "AnalysisBase.h"

#include<TMatrixD.h>
#include<TMatrixDEigen.h>
#include<TVectorD.h>

class Atlas_1605_04285 : public AnalysisBase {
  public:
    Atlas_1605_04285() : AnalysisBase()  {}               
    ~Atlas_1605_04285() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


void overlap_muons_jets_with_track_conditions(std::vector<Muon*>& muons,std::vector<Jet*>& jets,std::vector<Track*>& tracks,double dR,int numberOfTracks, double pT_threshold);

TMatrixD Get_Momentum_Matrix(std::vector<Jet*> jets);
#endif
