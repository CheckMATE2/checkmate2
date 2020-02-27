#ifndef _ATLAS_CONF_2013_047
#define _ATLAS_CONF_2013_047

#include "AnalysisBase.h"

class Atlas_conf_2013_047 : public AnalysisBase {
  public:
    Atlas_conf_2013_047() : AnalysisBase() {};
    ~Atlas_conf_2013_047() {}
    
    void initialize();
    void analyze();        
    void finalize();

  private:
    double cutTriggerJet, cutSecondJet, phiCutTight, phiCutLoose,  mEffCut;
    double mEffA, mEffB, mEffC, mEffD, mEffE;
    bool flag3J, flag4J, flag5J, flag6J;
};


class Atlas_conf_2013_047_CR : public AnalysisBase {
  public:
    Atlas_conf_2013_047_CR() : AnalysisBase() {};
    ~Atlas_conf_2013_047_CR() {}
    
    void initialize();
    void analyze();        
    void finalize();

  private:    
    double cutTriggerJet, cutSecondJet, phiCutTight, phiCutLoose,  mEffCut;
    double mEffA, mEffB, mEffC, mEffD, mEffE;
    bool cry, crq, crw, crt;
    bool crBTag, crWTag;
    static std::string cr[4];
    static std::string sr[10];
    
};


#endif
