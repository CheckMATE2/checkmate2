#ifndef ATLAS_2202_07953_H_
#define ATLAS_2202_07953_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_2202_07953 : public AnalysisBase {
  public:
    Atlas_2202_07953() : AnalysisBase()  {}               
    ~Atlas_2202_07953() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
