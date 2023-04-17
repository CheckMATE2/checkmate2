#ifndef CMS_2107_13201_H_
#define CMS_2107_13201_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Cms_2107_13201 : public AnalysisBase {
  public:
    Cms_2107_13201() : AnalysisBase()  {}               
    ~Cms_2107_13201() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
