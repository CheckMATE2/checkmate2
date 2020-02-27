#ifndef _CMS_1502_06031
#define _CMS_1502_06031
// AUTHOR: kitpc
//  EMAIL: kitpc
#include "AnalysisBase.h"

class Cms_1502_06031 : public AnalysisBase {
  public:
    Cms_1502_06031() : AnalysisBase() {};
    ~Cms_1502_06031() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    ofstream fout1;
};


#endif
