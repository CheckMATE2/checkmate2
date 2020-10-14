#ifndef ATLAS_1911_12606_H_
#define ATLAS_1911_12606_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1911_12606 : public AnalysisBase {
  public:
    Atlas_1911_12606() : AnalysisBase()  {}               
    ~Atlas_1911_12606() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
