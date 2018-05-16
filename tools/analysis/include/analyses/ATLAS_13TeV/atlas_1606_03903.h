#ifndef ATLAS_1606_03903_H_
#define ATLAS_1606_03903_H_
// AUTHOR: K Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1606_03903 : public AnalysisBase {
  public:
    Atlas_1606_03903() : AnalysisBase()  {}               
    ~Atlas_1606_03903() {}
  
    void initialize();
    void analyze();        
    void finalize();
    
    static bool sortByPT(Jet *i, Jet *j);
    template <class X, class Y>
    std::vector<X*> specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours);
    
    template <class X, class Y>
    std::vector<X*> specialoverlap2(std::vector<X*> candidates, std::vector<Y*> neighbours, double dR, double ratio);     

  private:
        TH1D *histo_met, *histo_mt, *histo_top, *histo_mtop, *histo_ht, *histo_st;
};

#endif
