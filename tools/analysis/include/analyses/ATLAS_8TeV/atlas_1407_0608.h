#ifndef _ATLAS_1407_0608
#define _ATLAS_1407_0608

#include "AnalysisBase.h"

class Atlas_1407_0608 : public AnalysisBase {
  public:
    Atlas_1407_0608() : AnalysisBase() {};
    ~Atlas_1407_0608() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
