#ifndef ATLAS_1712_02118_EW_H_
#define ATLAS_1712_02118_EW_H_
// AUTHOR: JSK
//  EMAIL: jsk@th.physik.uni-bonn.de
#include "AnalysisBase.h"

class Atlas_1712_02118_ew : public AnalysisBase {
  public:
    Atlas_1712_02118_ew() : AnalysisBase()  {}               
    ~Atlas_1712_02118_ew() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    double PixelTrackletSmearingFunction(double *x, double *par);
    TFile *acceffmapFile                        = 0;
    TH2 *acceffmapHist                          = 0;
};

#endif
