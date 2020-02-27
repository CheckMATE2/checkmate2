#ifndef _ATLAS_1506_08616
#define _ATLAS_1506_08616
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1506_08616 : public AnalysisBase {
  public:
    Atlas_1506_08616()  {}
    ~Atlas_1506_08616() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
