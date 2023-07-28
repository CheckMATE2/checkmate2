#ifndef CMS_2205_09597_H_
#define CMS_2205_09597_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Cms_2205_09597 : public AnalysisBase {
  public:
    Cms_2205_09597() : AnalysisBase()  {}               
    ~Cms_2205_09597() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    TFile *hfile;  TH1F *mj, *mjsd;  
    bool checkWtag(Jet* jet);
    bool checkVtag(Jet* jet);
    bool checkBBtag(Jet* jet);
};

#endif
