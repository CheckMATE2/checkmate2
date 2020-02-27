#ifndef _CMS_SUS_13_016
#define _CMS_SUS_13_016
// AUTHOR: Jamie Tattersall
//  EMAIL: tattersall@thphys.uni-heidelberg.de
#include "AnalysisBase.h"

class Cms_sus_13_016 : public AnalysisBase {
  public:
    Cms_sus_13_016() : AnalysisBase() {};
    ~Cms_sus_13_016() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
