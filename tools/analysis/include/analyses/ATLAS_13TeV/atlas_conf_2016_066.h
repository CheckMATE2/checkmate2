#ifndef ATLAS_CONF_2016_066_H_
#define ATLAS_CONF_2016_066_H_
// AUTHOR: J.S.K
//  EMAIL: jsk@th.physik.uni-bonn.de
#include "AnalysisBase.h"

class Atlas_conf_2016_066 : public AnalysisBase {
  public:
    Atlas_conf_2016_066() : AnalysisBase()  {}               
    ~Atlas_conf_2016_066() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
