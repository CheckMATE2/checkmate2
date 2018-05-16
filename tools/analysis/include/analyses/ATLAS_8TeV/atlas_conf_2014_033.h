#ifndef _ATLAS_CONF_2014_033
#define _ATLAS_CONF_2014_033

#include "AnalysisBase.h"
#include "TH2F.h"

class Atlas_conf_2014_033 : public AnalysisBase {
  public:
    Atlas_conf_2014_033() : AnalysisBase() {};
    ~Atlas_conf_2014_033() {}
  
    void initialize();
    void analyze();        
    void finalize();
    void outputHistFile(TH1D* inHist);

  private:
    TH1D *histo_ptmax, *histo_ptmin, *histo_ptll, *histo_mll, *histo_phill, *histo_mTllmiss, *histo_pTllmiss, *histo_etadiff, *histo_etasum, *histo_sqrtsmin, *histo_dx, *histo_njet, *histo_l1tol0;
    TH2D *histo_rap2D; TH2D *histo_pT2D;
    int asy1p, asy1m, asy2p, asy2m;
};


#endif
