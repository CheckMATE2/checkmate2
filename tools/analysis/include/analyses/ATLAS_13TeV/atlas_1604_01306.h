#ifndef _ATLAS_1604_01306
#define _ATLAS_1604_01306
// AUTHOR: kr
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1604_01306 : public AnalysisBase {
  public:
    Atlas_1604_01306() : AnalysisBase()  {}               
    ~Atlas_1604_01306() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    std::vector<Photon*> isolation(std::vector<Photon*> candidates);
};


#endif
