#ifndef _ATLAS_1411_1559
#define _ATLAS_1411_1559
// AUTHOR: Cristina Marcos
//  EMAIL: mcristina.marcosm@gmail.com
#include "AnalysisBase.h"

class Atlas_1411_1559 : public AnalysisBase {
  public:
    Atlas_1411_1559() : AnalysisBase()  {}               
    ~Atlas_1411_1559() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
