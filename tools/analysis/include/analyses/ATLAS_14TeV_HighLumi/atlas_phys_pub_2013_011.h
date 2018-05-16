#ifndef _ATLAS_PHYS_PUB_2013_011
#define _ATLAS_PHYS_PUB_2013_011
// AUTHOR: Jong Soo Kim
//  EMAIL: jongsoo.kim@tu-dortmund.de
#include "AnalysisBase.h"

class Atlas_phys_pub_2013_011 : public AnalysisBase {
  public:
    Atlas_phys_pub_2013_011() : AnalysisBase()  {}               
    ~Atlas_phys_pub_2013_011() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
