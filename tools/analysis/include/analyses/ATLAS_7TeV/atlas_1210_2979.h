#ifndef _ATLAS_1210_2979
#define _ATLAS_1210_2979

#include "AnalysisBase.h"

class Atlas_1210_2979 : public AnalysisBase {
  public:
    Atlas_1210_2979() : AnalysisBase() {};
    ~Atlas_1210_2979() {}
  
    void initialize();
    void analyze();        
    void finalize();
    void outputHistFile(TH1F* inHist);

  private:
    
    TH1F *hist_lep_pt, *hist_delta_phi, *hist_lletmiss_pt, *hist_lletmiss_mT;
};



#endif
