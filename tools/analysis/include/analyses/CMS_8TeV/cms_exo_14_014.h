#ifndef CMS_EXO_14_014_H_
#define CMS_EXO_14_014_H_
// AUTHOR: Ahmed Hammad
//  EMAIL: ahammad@zewailcity.edu.eg
#include "AnalysisBase.h"

class Cms_exo_14_014 : public AnalysisBase {
  public:
    Cms_exo_14_014() : AnalysisBase()  {}               
    ~Cms_exo_14_014() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
