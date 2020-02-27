#ifndef _ATLAS_CONF_2013_089
#define _ATLAS_CONF_2013_089

#include "AnalysisBase.h"
#include <sstream>

class Atlas_conf_2013_089 : public AnalysisBase {
  public:
    Atlas_conf_2013_089() : AnalysisBase() {};
    ~Atlas_conf_2013_089() {}
    
    void initialize();
    void analyze();        
    void finalize();
    
  private:
    const static int numCut = 3;
    const static int numLep =3;
    const static int numJet =2;
    const static int numZ =2;
    const static int numR05 =2;
    const static int numMR =3;
    static std::string ind[numCut];
    static std::string lep [numLep];
    static std::string jet [numJet];
    static std::string z [numZ];
    static std::string r05 [numR05];
    static std::string mr [numMR];
};


class Atlas_conf_2013_089_CR : public AnalysisBase {
  public:
    Atlas_conf_2013_089_CR() : AnalysisBase() {};
    ~Atlas_conf_2013_089_CR() {}
    
    void initialize();
    void analyze();        
    void finalize();
    
  private:
    const static int numCut = 3;
    const static int numLep =3;
    const static int numJet =2;
    const static int numZ =2;
    static std::string ind[numCut];
    static std::string lep [numLep];
    static std::string jet [numJet];
    static std::string z [numZ];
};



#endif
