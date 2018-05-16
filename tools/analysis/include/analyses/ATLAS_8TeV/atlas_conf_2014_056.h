#ifndef _ATLAS_CONF_2014_056
#define _ATLAS_CONF_2014_056

#include "AnalysisBase.h"

class Atlas_conf_2014_056 : public AnalysisBase {
  public:
    Atlas_conf_2014_056() : AnalysisBase() {};
    ~Atlas_conf_2014_056() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    TH1D *histo_phill, *histo_etadiff;
};


#endif
