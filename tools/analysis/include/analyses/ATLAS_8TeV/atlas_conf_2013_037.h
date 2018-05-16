#ifndef _ATLAS_CONF_2013_037
#define _ATLAS_CONF_2013_037

#include "AnalysisBase.h"

class Atlas_conf_2013_037 : public AnalysisBase {
  public:
    Atlas_conf_2013_037() : AnalysisBase() {};
    ~Atlas_conf_2013_037() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
