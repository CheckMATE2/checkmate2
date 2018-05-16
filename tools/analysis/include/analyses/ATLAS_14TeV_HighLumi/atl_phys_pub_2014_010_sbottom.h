#ifndef _ATL_PHYS_PUB_2014_010_SBOTTOM
#define _ATL_PHYS_PUB_2014_010_SBOTTOM
// AUTHOR: Torsten Weber
//  EMAIL: tweber@physik.rwth-aachen.de
#include "AnalysisBase.h"

class Atl_phys_pub_2014_010_sbottom : public AnalysisBase {
  public:
    Atl_phys_pub_2014_010_sbottom() : AnalysisBase()  {}               
    ~Atl_phys_pub_2014_010_sbottom() {}
  
    void initialize();
    void analyze();        
    void finalize();
};


#endif
