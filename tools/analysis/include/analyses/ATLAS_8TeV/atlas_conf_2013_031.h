#ifndef _ATLAS_CONF_2013_031
#define _ATLAS_CONF_2013_031

#include "AnalysisBase.h"

class Atlas_conf_2013_031 : public AnalysisBase {
  public:
    Atlas_conf_2013_031() : AnalysisBase() {};
    ~Atlas_conf_2013_031() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
