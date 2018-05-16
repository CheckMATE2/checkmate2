#ifndef _ATLAS_CONF_2013_021
#define _ATLAS_CONF_2013_021

#include "AnalysisBase.h"

class Atlas_conf_2013_021 : public AnalysisBase {
  public:
    Atlas_conf_2013_021() : AnalysisBase() {};
    ~Atlas_conf_2013_021() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
