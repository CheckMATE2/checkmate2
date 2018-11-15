#ifndef ATLAS_1712_02118_RE_H_
#define ATLAS_1712_02118_RE_H_
// AUTHOR: J.S.Kim and D.Dercks
//  EMAIL: jsk@th.physik.uni-bonn.de
#include "AnalysisBase.h"

class Atlas_1712_02118_re : public AnalysisBase {
  public:
    Atlas_1712_02118_re() : AnalysisBase()  {}               
    ~Atlas_1712_02118_re() {}
  
    void initialize();
    void analyze();        
    void finalize();

  
  private:
    double PixelTrackletSmearingFunction(double *x, double *par);
    TFile *acceffmapFile                        = 0;
    TH2 *acceffmapHist                          = 0;
};

#endif
