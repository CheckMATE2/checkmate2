#ifndef ATLAS_HIGG_2013_03_H_
#define ATLAS_HIGG_2013_03_H_
// AUTHOR: Arran Freegard
//  EMAIL: acf1g14@soton.ac.uk
#include "AnalysisBase.h"

class Atlas_higg_2013_03 : public AnalysisBase {
  public:
    Atlas_higg_2013_03() : AnalysisBase()  {}               
    ~Atlas_higg_2013_03() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
