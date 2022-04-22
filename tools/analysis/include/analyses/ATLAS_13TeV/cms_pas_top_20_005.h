#ifndef CMS_PAS_TOP_20_005_H_
#define CMS_PAS_TOP_20_005_H_
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Cms_pas_top_20_005 : public AnalysisBase {
  public:
    Cms_pas_top_20_005() : AnalysisBase()  {}               
    ~Cms_pas_top_20_005() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    TFile *hfile;  TH1F *chi2_hist; TH1F *chi2_hist2;  
};

#endif
