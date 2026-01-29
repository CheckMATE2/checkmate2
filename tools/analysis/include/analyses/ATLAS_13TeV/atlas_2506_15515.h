#ifndef ATLAS_2506_15515_H_
#define ATLAS_2506_15515_H_
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"


class Atlas_2506_15515 : public AnalysisBase {
  public:
    Atlas_2506_15515() : AnalysisBase()  {}               
    ~Atlas_2506_15515() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
      // Helper methods for kinematic reconstruction
    double getWmass(PxPyPzEVector lepton);
    std::string getBinLabel(double w_pt);  // Returns "low", "mid", "high"


};

#endif
