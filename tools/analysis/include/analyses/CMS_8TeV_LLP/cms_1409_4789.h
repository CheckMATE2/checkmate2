#ifndef CMS_1409_4789_H_
#define CMS_1409_4789_H_
// AUTHOR: jsk
//  EMAIL: jsk
#include "AnalysisBase.h"

class Cms_1409_4789 : public AnalysisBase {
  public:
    Cms_1409_4789() : AnalysisBase()  {}               
    ~Cms_1409_4789() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
