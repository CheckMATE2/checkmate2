#ifndef ATLAS_1710_04901_H_
#define ATLAS_1710_04901_H_
// AUTHOR: Boogeyman
//  EMAIL: Boogeyman@horror.hell
#include "AnalysisBase.h"

class Atlas_1710_04901 : public AnalysisBase {
  public:
    Atlas_1710_04901() : AnalysisBase()  {}               
    ~Atlas_1710_04901() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
