#ifndef _ATLAS_CONF_2012_147
#define _ATLAS_CONF_2012_147

#include "AnalysisBase.h"

class Atlas_conf_2012_147 : public AnalysisBase {
  public:
    Atlas_conf_2012_147() : AnalysisBase() {};
    ~Atlas_conf_2012_147() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    TH1 *hist_Met_1, *hist_Met_2, *hist_Met_3, *hist_Met_4;
    TH1 *hist_PT_1, *hist_PT_2, *hist_PT_3, *hist_PT_4;
};


class Atlas_conf_2012_147_CR : public AnalysisBase {
  public:
    Atlas_conf_2012_147_CR() : AnalysisBase() {};
    ~Atlas_conf_2012_147_CR() {}
    
    void initialize();
    void analyze();        
    void finalize();

  private:
    static std::string cr[3];
    static std::string sr[4];
};

#endif
