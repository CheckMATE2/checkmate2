#ifndef ATLAS_2004_10894_H_
#define ATLAS_2004_10894_H_
// AUTHOR: JSK
//  EMAIL: jsk@th.ph
#include "AnalysisBase.h"

class Atlas_2004_10894 : public AnalysisBase {
  public:
    Atlas_2004_10894() : AnalysisBase()  {}               
    ~Atlas_2004_10894() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
