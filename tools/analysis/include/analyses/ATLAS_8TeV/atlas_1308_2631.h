#ifndef _ATLAS_1308_2631
#define _ATLAS_1308_2631

#include "AnalysisBase.h"

class Atlas_1308_2631 : public AnalysisBase {
  public:
    Atlas_1308_2631() : AnalysisBase() {};
    ~Atlas_1308_2631() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


class Atlas_1308_2631_CR : public AnalysisBase {
  public:
    Atlas_1308_2631_CR() : AnalysisBase() {};
    ~Atlas_1308_2631_CR() {}
    
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
