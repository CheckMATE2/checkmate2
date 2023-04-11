#ifndef CMS_1908_04722_H_
#define CMS_1908_04722_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Cms_1908_04722 : public AnalysisBase {
  public:
    Cms_1908_04722() : AnalysisBase()  {}               
    ~Cms_1908_04722() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    static std::string label[10];
    static std::string label_hi[8];
    template <class X>
std::vector<X*> Isolate_leptons_with_variable_track_isolation_cone_CMS(std::vector<X*> leptons, double dR_track_max, double dR_track_min, double pT_for_inverse_function_track, double pT_amount_track );
    void fill_bins(int Nj, int Nb, double HTmiss, double HT);
};

#endif
