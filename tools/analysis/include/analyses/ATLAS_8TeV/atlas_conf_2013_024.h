#ifndef _ATLAS_CONF_2013_024
#define _ATLAS_CONF_2013_024

#include "AnalysisBase.h"

class Atlas_conf_2013_024 : public AnalysisBase {
  public:
    Atlas_conf_2013_024() : AnalysisBase() {};
    ~Atlas_conf_2013_024() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


class Atlas_conf_2013_024_CR : public AnalysisBase {
  public:
    Atlas_conf_2013_024_CR() : AnalysisBase() {};
    ~Atlas_conf_2013_024_CR() {}
    
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
