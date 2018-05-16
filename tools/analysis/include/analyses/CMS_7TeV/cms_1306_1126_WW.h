#ifndef _CMS_1306_1126_WW
#define _CMS_1306_1126_WW

#include "AnalysisBase.h"
#include "TH2F.h"

class Cms_1306_1126_ww : public AnalysisBase {
  public:
    Cms_1306_1126_ww() : AnalysisBase() {};
    ~Cms_1306_1126_ww() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    TH1D *histo_ptmax, *histo_ptmin, *histo_ptll, *histo_mll, *histo_etadiff, *histo_etasum, *histo_sqrtsmin;
    TH2D *histo_rap2D;
};


#endif
