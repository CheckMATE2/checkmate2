#ifndef _ATLAS_2014_010_HL_3L
#define _ATLAS_2014_010_HL_3L
// AUTHOR: Roberto Ruiz
//  EMAIL: rruiz@ific.uv.es
#include "AnalysisBase.h"

class Atlas_2014_010_hl_3l : public AnalysisBase {
  public:
    Atlas_2014_010_hl_3l() : AnalysisBase()  {}               
    ~Atlas_2014_010_hl_3l() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
