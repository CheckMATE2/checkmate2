#ifndef _ATLAS_CONF_2015_082
#define _ATLAS_CONF_2015_082
// AUTHOR: Tim Keller
//  EMAIL: tim.keller@rwth-aachen.de
#include "AnalysisBase.h"

#include<fstream>

class Atlas_conf_2015_082 : public AnalysisBase {
  public:
    Atlas_conf_2015_082() : AnalysisBase()  {}               
    ~Atlas_conf_2015_082() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    ofstream outfile1, outfile2;
    
};


#endif
