#ifndef _ATLAS_CONF_2013_061
#define _ATLAS_CONF_2013_061

#include "AnalysisBase.h"

class Atlas_conf_2013_061 : public AnalysisBase {
  public:
    Atlas_conf_2013_061() : AnalysisBase() {};
    ~Atlas_conf_2013_061() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


class Atlas_conf_2013_061_CR : public AnalysisBase {
  public:
    Atlas_conf_2013_061_CR() : AnalysisBase() {};
    ~Atlas_conf_2013_061_CR() {}
    
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
