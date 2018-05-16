#ifndef ATLAS_CONF_2016_096_H_
#define ATLAS_CONF_2016_096_H_
// AUTHOR: Ferdinand Eiteneuer
//  EMAIL: ferdinand.eiteneuer@rwth-aachen.de
#include "AnalysisBase.h"

class Atlas_conf_2016_096 : public AnalysisBase {
  public:
    Atlas_conf_2016_096() : AnalysisBase()  {}               
    ~Atlas_conf_2016_096() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
