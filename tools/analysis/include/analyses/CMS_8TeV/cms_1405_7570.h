#ifndef _CMS_1405_7570
#define _CMS_1405_7570
// AUTHOR: Krzysztof
//  EMAIL: krzysztof.rolbiecki@desy.de
#include "AnalysisBase.h"

class Cms_1405_7570 : public AnalysisBase {
  public:
    Cms_1405_7570() : AnalysisBase() {};
    ~Cms_1405_7570() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
