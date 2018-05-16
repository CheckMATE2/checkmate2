#ifndef _DILEPTON_HL
#define _DILEPTON_HL
// AUTHOR: kr
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Dilepton_hl : public AnalysisBase {
  public:
    Dilepton_hl()  {}               
    ~Dilepton_hl() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};


#endif
