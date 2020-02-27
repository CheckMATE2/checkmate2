#ifndef _atlas_conf_2016_050
#define _atlas_conf_2016_050
// AUTHOR: Junjie Cao, Liangliang Shang, Peiwen Wu, Jinmin Yang and Yang Zhang
//  EMAIL: junjiec@itp.ac.cn, shlwell1988@gmail.com, pwwu@kias.re.kr, jmyang@itp.ac.cn and zhangyang@itp.ac.cn
#include "AnalysisBase.h"
#include <fstream>

class Atlas_conf_2016_050 : public AnalysisBase {
  public:
    Atlas_conf_2016_050() : AnalysisBase()  {}               
    ~Atlas_conf_2016_050() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:

  //ofstream f1;
  //TH1F *SR1_met, *SR1_mt, *tN_high_met, *tN_high_mt, *bC2x_diag_met, *bC2x_diag_mt;
  //TFile *hfile;

};


#endif
