#ifndef _CMS_SMP_12_006
#define _CMS_SMP_12_006

#include "AnalysisBase.h"

class Cms_smp_12_006 : public AnalysisBase {
  public:
    Cms_smp_12_006() : AnalysisBase() {};
    ~Cms_smp_12_006() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
