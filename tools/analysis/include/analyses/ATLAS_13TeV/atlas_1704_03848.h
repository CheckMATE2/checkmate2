#ifndef ATLAS_1704_03848_H_
#define ATLAS_1704_03848_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1704_03848 : public AnalysisBase {
  public:
    Atlas_1704_03848() : AnalysisBase()  {}               
    ~Atlas_1704_03848() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    std::vector<Photon*> isolation(std::vector<Photon*> candidates);
};

#endif
