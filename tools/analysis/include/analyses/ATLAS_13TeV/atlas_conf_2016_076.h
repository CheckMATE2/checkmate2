#ifndef _ATLAS_CONF_2016_076
#define _ATLAS_CONF_2016_076
// AUTHOR: Junjie Cao, Krzysztof Rolbiecki, Liangliang Shang, Jamie Tattersall, Peiwen Wu, JinMin Yang, Yuanfang Yue and Yang Zhang
// EMAIL: junjiec@itp.ac.cn, krzysztof.rolbiecki@desy.de, shlwell1988@gmail.com, tattersall@physik.rwth-aachen.de, pwwu@kias.re.kr, jmyang@itp.ac.cn and zhangyang@itp.ac.cn
#include "AnalysisBase.h"
#include <fstream>

class Atlas_conf_2016_076 : public AnalysisBase {
  public:
    Atlas_conf_2016_076() : AnalysisBase()  {}            
    ~Atlas_conf_2016_076() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    //ofstream f1;
    //TH1F *low_mt2bb, *high_etbl, *w_sf_mrdel, *t_sf_mrdel, *w_df_mrdel, *t_df_mrdel;

    //TFile *hfile;
};


#endif
