#ifndef CMS_SUSY_DISPLACED_LEPTONS_8TEV_H_
#define CMS_SUSY_DISPLACED_LEPTONS_8TEV_H_
// AUTHOR: Mangesh Sonawane
//  EMAIL: son.arnav95@gmail.com
#include "AnalysisBase.h"
#include <random>

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

  	ofstream debug, signal;

  	double SR3, SR2, SR1;

  	double xsec;
  	double i_lumi;

  	vector <GenParticle*> finalleptons, el, mu, stable;
  	//Efficiency records
	vector< vector<double> > elec_d0_eff, muon_d0_eff, elec_pt_eff, muon_pt_eff;

  	TFile *hists;

  	TH1F *n_elec, *n_muon, *n_ta;

  	int initEff(string filename, vector< vector<double> > &arrayname);
  	double getEff(double value, vector< vector<double> > &arrayname);
  	bool lep_selection(GenParticle* part, int ID = 0, double pT = 0., double eta = 5., bool overlap = false);
  	bool is_isolated(GenParticle* lep, vector <GenParticle*> &stable, double epsilon=0., double dR_cone = 0.);
  	bool is_isolated_from_jet(GenParticle* lep, vector <Jet*> &jets, double dR_cone = 0.);
};

#endif
