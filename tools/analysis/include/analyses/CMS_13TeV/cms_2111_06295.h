#ifndef CMS_2111_06295_H_
#define CMS_2111_06295_H_
// AUTHOR: kr
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Cms_2111_06295 : public AnalysisBase {
  public:
    Cms_2111_06295() : AnalysisBase()  {}               
    ~Cms_2111_06295() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
