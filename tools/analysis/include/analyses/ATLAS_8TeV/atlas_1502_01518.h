#ifndef _ATLAS_1502_01518
#define _ATLAS_1502_01518
// AUTHOR: Jamie Tattersall
//  EMAIL: tattersall@thphys.uni-heidelberg.de
#include "AnalysisBase.h"

class Atlas_1502_01518 : public AnalysisBase {
  public:
    Atlas_1502_01518() : AnalysisBase()  {}
    ~Atlas_1502_01518() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
