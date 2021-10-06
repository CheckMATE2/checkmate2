#ifndef ATLAS_2106_09609_H_
#define ATLAS_2106_09609_H_
// AUTHOR: JSK
//  EMAIL: jsk@th.physik.uni-bonn.de
#include "AnalysisBase.h"

class Atlas_2106_09609 : public AnalysisBase {
  public:
    Atlas_2106_09609() : AnalysisBase()  {}               
    ~Atlas_2106_09609() {}

    template <class X>
      std::vector <X*> removeLowMassResonances(std::vector <X*> candidates, double pTmin, double pTMax);

    template <class X>
      void findZ(std::vector<X*> candidates, std::vector <std::pair<double, double>> & zmassdiff);

    
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
