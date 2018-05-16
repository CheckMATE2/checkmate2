#ifndef _ATLAS_1403_5294
#define _ATLAS_1403_5294

#include "AnalysisBase.h"

class Atlas_1403_5294 : public AnalysisBase {
  public:
    Atlas_1403_5294() : AnalysisBase() {};
    ~Atlas_1403_5294() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


class Atlas_1403_5294_CR : public AnalysisBase {
  public:
    Atlas_1403_5294_CR() : AnalysisBase() {};
    ~Atlas_1403_5294_CR() {}
    
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
