#ifndef _ATLAS_PHYS_2014_010_SQ_HL
#define _ATLAS_PHYS_2014_010_SQ_HL
// AUTHOR: kr
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_phys_2014_010_sq_hl : public AnalysisBase {
  public:
    Atlas_phys_2014_010_sq_hl() : AnalysisBase()  {}               
    ~Atlas_phys_2014_010_sq_hl() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    double meff2J, meff3J, meff4J, meff5J, meff6J;
    bool flag3J, flag4J, flag5J, flag6J;
    static std::string nameSR[10];
};


#endif
