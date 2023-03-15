#ifndef CMS_SUS_19_006_H_
#define CMS_SUS_19_006_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Cms_sus_19_006 : public AnalysisBase {
  public:
    Cms_sus_19_006() : AnalysisBase()  {}               
    ~Cms_sus_19_006() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
