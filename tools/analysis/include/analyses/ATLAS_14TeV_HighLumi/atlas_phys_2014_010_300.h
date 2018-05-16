#ifndef _ATLAS_PHYS_2014_010_300
#define _ATLAS_PHYS_2014_010_300
// AUTHOR: kr
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_phys_2014_010_300 : public AnalysisBase {
  public:
    Atlas_phys_2014_010_300() : AnalysisBase()  {}               
    ~Atlas_phys_2014_010_300() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    double meff2J, meff3J, meff4J, meff5J, meff6J;
    bool flag3J, flag4J, flag5J, flag6J;
    static std::string nameSR[10];

    ofstream fout1, fout2;
    TH1F *h01, *h02, *h03, *h04, *h05, *h06, *h07, *h08, *h09, *h10;
    TCanvas *canvas;
};


#endif
