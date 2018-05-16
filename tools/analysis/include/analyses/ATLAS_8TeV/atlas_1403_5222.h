#ifndef _ATLAS_1403_5222
#define _ATLAS_1403_5222
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1403_5222 : public AnalysisBase {
  public:
    Atlas_1403_5222()  {}
    ~Atlas_1403_5222() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
