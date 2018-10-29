#ifndef ATLAS_1712_02118_H_
#define ATLAS_1712_02118_H_
// AUTHOR: J.S.Kim
//  EMAIL: jsk@th.physik.uni-bonn.de
#include "AnalysisBase.h"

class Atlas_1712_02118 : public AnalysisBase {
  public:
    Atlas_1712_02118() : AnalysisBase()  {}               
    ~Atlas_1712_02118() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
