#ifndef CMS_2107_13201_H_
#define CMS_2107_13201_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Cms_2107_13201 : public AnalysisBase {
  public:
    Cms_2107_13201() : AnalysisBase()  {}               
    ~Cms_2107_13201() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    template <class X>
std::vector<X*> Isolate_leptons_with_variable_track_isolation_cone_CMS(std::vector<X*> leptons, double dR_track_max, double dR_track_min, double pT_for_inverse_function_track, double pT_amount_track );
};

#endif
