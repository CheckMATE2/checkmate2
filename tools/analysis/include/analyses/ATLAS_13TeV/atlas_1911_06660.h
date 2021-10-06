#ifndef ATLAS_1911_06660_H_
#define ATLAS_1911_06660_H_
// AUTHOR: JSK
//  EMAIL: jsk@th.physik.uni-bonn.de
#include "AnalysisBase.h"

class Atlas_1911_06660 : public AnalysisBase {
  public:
    Atlas_1911_06660() : AnalysisBase()  {}               
    ~Atlas_1911_06660() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
