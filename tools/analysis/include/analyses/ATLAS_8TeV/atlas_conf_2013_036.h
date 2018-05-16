#ifndef _ATLAS_CONF_2013_036
#define _ATLAS_CONF_2013_036

#include "AnalysisBase.h"

class Atlas_conf_2013_036 : public AnalysisBase {
  public:
    Atlas_conf_2013_036() : AnalysisBase() {};
    ~Atlas_conf_2013_036() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    static std::string id[5];
    double meff;
    bool trigger;
   // TH1F * h;
};


class Atlas_conf_2013_036_CR : public AnalysisBase {
  public:
    Atlas_conf_2013_036_CR() : AnalysisBase() {};
    ~Atlas_conf_2013_036_CR() {}
    
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
