#ifndef _ATLAS_1503_03290
#define _ATLAS_1503_03290
// AUTHOR: kitpc
//  EMAIL: kitpc
#include "AnalysisBase.h"

#include<fstream>
#include<TH2.h> 

class Atlas_1503_03290 : public AnalysisBase {
  public:
    Atlas_1503_03290() : AnalysisBase() {};
    ~Atlas_1503_03290() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    ofstream outfile1, outfile2;
    TH1F *histmissET, *histHT, *histJet;
    TCanvas *canv;
    double norml, normc, normTot, normFactor;
    int sBin;
};


#endif
