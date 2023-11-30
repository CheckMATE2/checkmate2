#ifndef ATLAS_2006_05880_H_
#define ATLAS_2006_05880_H_
// AUTHOR: Inaki Lara Perez
//  EMAIL: Inaki.Lara@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_2006_05880 : public AnalysisBase {
  public:
    Atlas_2006_05880() : AnalysisBase()  {}               
    ~Atlas_2006_05880() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    template <class X>
    bool is_track_isolated(X* lep, std::vector <GenParticle*> &stable, double epsilon=0., double dR_cone = 0.);
    template <class X>
    bool is_cal_isolated(X* lep, std::vector<Tower*> towers, double epsilon=0., double dR_cone = 0.);
    int calc_nhcand(std::vector<Jet*> bjets);
    int calc_nhcand(std::vector<GenParticle*> stable);
    float calc_S_score(std::vector<Jet*> jets,std::vector<FinalStateObject*> leptons,TLorentzVector pTmiss);
    void rotateXY(TMatrix &mat, TMatrix &mat_new, double phi);
};

#endif
