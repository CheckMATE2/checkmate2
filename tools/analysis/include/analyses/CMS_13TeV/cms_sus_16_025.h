#ifndef CMS_SUS_16_025_H_
#define CMS_SUS_16_025_H_
// AUTHOR: none
//  EMAIL: none
#include "AnalysisBase.h"

class Cms_sus_16_025 : public AnalysisBase {
  public:
    Cms_sus_16_025() : AnalysisBase()  {}               
    ~Cms_sus_16_025() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    ofstream fout1;
    TH1F *h1, *h2, *h3, *h4, *h5;
    TFile *f1;
    int i1, i2, i3, i4;

};

#endif
