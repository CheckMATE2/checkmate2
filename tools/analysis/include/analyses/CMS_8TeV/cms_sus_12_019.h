#ifndef _CMS_SUS_12_019
#define _CMS_SUS_12_019

#include "AnalysisBase.h"

class Cms_sus_12_019 : public AnalysisBase {
  public:
    Cms_sus_12_019() : AnalysisBase() {};
    ~Cms_sus_12_019() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
