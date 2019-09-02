#ifndef ATLAS_CONF_2019_018_H_
#define ATLAS_CONF_2019_018_H_
// AUTHOR: Weiwei, Di Zhang, Liangliang Shang, Junjie cao
// EMAIL: wjw4001@163.com and dz481655@gmail.com
#include "AnalysisBase.h"

class Atlas_conf_2019_018 : public AnalysisBase {
  public:
    Atlas_conf_2019_018() : AnalysisBase()  {}               
    ~Atlas_conf_2019_018() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    void overlapRemoval_electrons_jets(std::vector<Electron*>& electrons, std::vector<Jet*>& jets);
    void overlapRemoval_muons_jets(std::vector<Muon*>& muons, std::vector<Jet*>& jets);

};

#endif
