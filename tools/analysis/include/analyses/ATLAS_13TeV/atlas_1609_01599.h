#ifndef ATLAS_1609_01599_H_
#define ATLAS_1609_01599_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1609_01599 : public AnalysisBase {
  public:
    Atlas_1609_01599() : AnalysisBase()  {}               
    ~Atlas_1609_01599() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    template<class T> std::vector<T*> isolateLeptons(std::vector<T*> leptons, double deltaR);    
};

#endif
