#ifndef ATLAS_CONF_2017_039_H_
#define ATLAS_CONF_2017_039_H_
// AUTHOR: Daniel Dercks
//  EMAIL: daniel.dercks@desy.de
#include "AnalysisBase.h"

class Atlas_conf_2017_039 : public AnalysisBase {
  public:
    Atlas_conf_2017_039() : AnalysisBase()  {}               
    ~Atlas_conf_2017_039() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
