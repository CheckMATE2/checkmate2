#ifndef ATLAS_1502_05686_H_
#define ATLAS_1502_05686_H_
// AUTHOR: JSK
//  EMAIL: jsk@th.physik.uni-bonn.de
#include "AnalysisBase.h"

class Atlas_1502_05686 : public AnalysisBase {
  public:
    Atlas_1502_05686() : AnalysisBase()  {}               
    ~Atlas_1502_05686() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
