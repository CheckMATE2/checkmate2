#ifndef _ATLAS_1405_7875
#define _ATLAS_1405_7875

#include "AnalysisBase.h"

class Atlas_1405_7875 : public AnalysisBase {
  public:
    Atlas_1405_7875() : AnalysisBase() {};
    ~Atlas_1405_7875() {}
    
    void initialize();
    void analyze();        
    void finalize();

  private:
    double meff2J, meff3J, meff4J, meff4JW, meff5J, meff6J;
    bool flag4JW, flag3J, flag4J, flag5J, flag6J;
    static std::string nameSR[15];

    ofstream fout1, fout2;
    TH1F *h01, *h02, *h03, *h04, *h05, *h06, *h07, *h08, *h09, *h10;
    TCanvas *canvas;
};


#endif
