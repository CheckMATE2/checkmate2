#ifndef CMS_SUS_19_005_H_
#define CMS_SUS_19_005_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class LundCluster {
  public:
    TLorentzVector P4(void) {return momentum;};
    std::vector<Jet*> constituents;
    void add_jet(Jet* jet);
    void remove_jet(Jet* jet);
    double Lund_dist(Jet* jet) {return sqrt(2.*(jet->P4().Rho() * momentum.Rho() - jet->P4().Vect().Dot(momentum.Vect())) * jet->P4().Rho() * momentum.Rho())/(jet->P4().Rho() + momentum.Rho()) ;};
    LundCluster(Jet* jet);
    ~LundCluster() {};
  
private:
    TLorentzVector momentum;
    static bool sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }
};


class Cms_sus_19_005 : public AnalysisBase {
  public:
    Cms_sus_19_005() : AnalysisBase()  {}               
    ~Cms_sus_19_005() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    template <class X>
std::vector<X*> Isolate_leptons_with_variable_track_isolation_cone_CMS(std::vector<X*> leptons, double dR_track_max, double dR_track_min, double pT_for_inverse_function_track, double pT_amount_track );
    double dPhi(std::vector<Jet*> jets, int j);
    static bool sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }
    std::vector<LundCluster> cluster_jets(std::vector<Jet*> input);
};

#endif
