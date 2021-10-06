#ifndef ATLAS_2103_11684_H_
#define ATLAS_2103_11684_H_
// AUTHOR: J.S.Kim
//  EMAIL: jsk@th.physik.uni-bonn.de
#include "AnalysisBase.h"
#include <vector>
#include <set>
#include <utility>

class Atlas_2103_11684 : public AnalysisBase {
  public:
    Atlas_2103_11684() : AnalysisBase()  {}               
    ~Atlas_2103_11684() {}

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
