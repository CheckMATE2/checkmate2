#ifndef _ATLAS_CONF_2013_062
#define _ATLAS_CONF_2013_062

#include "AnalysisBase.h"

class Atlas_conf_2013_062 : public AnalysisBase {
  public:
    Atlas_conf_2013_062() : AnalysisBase() {};
    ~Atlas_conf_2013_062() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    const static int numMass =2;
    const static int numCutSingSoftLep1B =7;
    const static int numCutSingSoftLep2B =6;
    const static int numCutSingSoftLepMulti =6;
    const static int numCutDiMu =10;
    const static int numCutHardLep =8;
    const static int numLep =2;
    const static int numHardLepJet =3;
    const static int numHardLepOrtho =2;

    
    static std::string id[numCutDiMu];
    static std::string idLep[numLep];
    static std::string idHardLepJet[numHardLepJet];
    static std::string idHardLepOrtho[numHardLepOrtho];
};


class Atlas_conf_2013_062_CR : public AnalysisBase {
  public:
    Atlas_conf_2013_062_CR() : AnalysisBase() {};
    ~Atlas_conf_2013_062_CR() {}
    
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
