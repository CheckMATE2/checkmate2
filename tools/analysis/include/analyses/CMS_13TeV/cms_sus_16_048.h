#ifndef CMS_SUS_16_048_H_
#define CMS_SUS_16_048_H_
// AUTHOR: Junjie Cao, Liangliang Shang, Yuanfang Yue and Yang Zhang 
// EMAIL: junjiec@itp.ac.cn, shlwell1988@gmail.com, yuanfang405@gmail.com and zhangyang@itp.ac.cn 


#include "AnalysisBase.h"

class Cms_sus_16_048 : public AnalysisBase {
  public:
    Cms_sus_16_048() : AnalysisBase()  {}               
    ~Cms_sus_16_048() {}
  
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
