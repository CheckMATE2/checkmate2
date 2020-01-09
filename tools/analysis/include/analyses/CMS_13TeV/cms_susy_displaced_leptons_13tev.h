#ifndef CMS_SUSY_DISPLACED_LEPTONS_13TEV_H_
#define CMS_SUSY_DISPLACED_LEPTONS_13TEV_H_
// AUTHOR: Mangesh Sonawane
//  EMAIL: son.arnav95@gmail.com
#include "AnalysisBase.h"
#include <random>

class Cms_susy_displaced_leptons_13tev : public AnalysisBase {
  public:
    Cms_susy_displaced_leptons_13tev() : AnalysisBase()  {}               
    ~Cms_susy_displaced_leptons_13tev() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:

  	int EventCount;
  	int n_e, n_mu, n_tau;

    TFile *eff_file;

    TH1F *elec_d0_eff_hist;
    TH1F *elec_pt_eff_hist;
    TH1F *muon_d0_eff_hist;
    TH1F *muon_pt_eff_hist;

  	// ofstream debug;

  	double SR3, SR2, SR1;
	
    double BR;

  	double xsec;
  	double i_lumi;

  	vector <GenParticle*> finalleptons, el, mu, stable;

    //User defined functions for selection and isolation

    bool lep_selection(GenParticle* part, int ID = 0, double pT = 0., double eta = 5., bool overlap = false);
    bool is_isolated(GenParticle* lep, vector <GenParticle*> &stable, double epsilon=0., double dR_cone = 0.);
    bool is_isolated_from_jet(GenParticle* lep, vector <Jet*> &jets, double dR_cone = 0.);
};

#endif
