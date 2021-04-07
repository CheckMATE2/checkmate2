#ifndef CMS_PAS_EXO_16_022_H_
#define CMS_PAS_EXO_16_022_H_
// AUTHOR: Mangesh Sonawane
//  EMAIL: son.arnav95@gmail.com
#include "AnalysisBase.h"

class Cms_pas_exo_16_022 : public AnalysisBase {
  public:
    Cms_pas_exo_16_022() : AnalysisBase()  {}               
    ~Cms_pas_exo_16_022() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:

    int EventCount;
    int n_e, n_mu, n_tau;

    TFile *eff_file;

    int llp_pid = 1000006; 

    TH1F *elec_d0_eff_hist;
    TH1F *elec_pt_eff_hist;
    TH1F *muon_d0_eff_hist;
    TH1F *muon_pt_eff_hist;

    double SR3, SR2, SR1;
  
    double BR, llp_mass;

    double xsec;
    double i_lumi;

    vector <GenParticle*> finalleptons, el, mu, stable;

    //User defined functions for selection and isolation

    bool lep_selection(GenParticle* part, int ID = 0, double pT = 0., double eta = 5., bool overlap = false);
    bool is_isolated(GenParticle* lep, vector <GenParticle*> &stable, double epsilon=0., double dR_cone = 0.);
    bool is_isolated_from_jet(GenParticle* lep, vector <Jet*> &jets, double dR_cone = 0.);
};

#endif
