#ifndef ATLAS_1507_05493_H_
#define ATLAS_1507_05493_H_
// AUTHOR: J.S. Kim
//  EMAIL: jsk@th.physik.uni-bonn.de
#include "AnalysisBase.h"

class Atlas_1507_05493 : public AnalysisBase {
  public:
    Atlas_1507_05493() : AnalysisBase()  {}               
    ~Atlas_1507_05493() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
