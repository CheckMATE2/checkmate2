#ifndef CMS_SUSY_DISPLACED_LEPTONS_8TEV_H_
#define CMS_SUSY_DISPLACED_LEPTONS_8TEV_H_
// AUTHOR: Mangesh Sonawane
//  EMAIL: son.arnav95@gmail.com
#include "AnalysisBase.h"

class Cms_susy_displaced_leptons_8tev : public AnalysisBase {
  public:
    Cms_susy_displaced_leptons_8tev() : AnalysisBase()  {}               
    ~Cms_susy_displaced_leptons_8tev() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:

  	int EventCount;
  	int n_e, n_mu, n_tau;

  	ofstream debug;

  	double xsec;
  	double i_lumi;

  	vector <GenParticle*> finalleptons, el, mu, stable;
  	//Efficiency records
	vector< vector<double> > elec_d0_eff, muon_d0_eff, elec_pt_eff, muon_pt_eff;

  	TFile *hists;

  	TH1F *n_elec, *n_muon, *n_ta;

};

#endif
