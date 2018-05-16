#ifndef _ATLAS_1407_0600
#define _ATLAS_1407_0600

#include "AnalysisBase.h"

class Atlas_1407_0600 : public AnalysisBase {
  public:
    Atlas_1407_0600() : AnalysisBase() {};
    ~Atlas_1407_0600() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    double DeltaPhi4jmin;
    double meff4j;
    double HT4j;
    double meff;
    double meffincl;
    double mT;
    static std::string cf_index[12];
};


#endif
