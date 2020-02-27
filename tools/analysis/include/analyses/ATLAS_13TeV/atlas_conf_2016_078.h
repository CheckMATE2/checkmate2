#ifndef ATLAS_CONF_2016_078_H_
#define ATLAS_CONF_2016_078_H_
// AUTHOR: J. S. Kim
//  EMAIL: jongsoo.kim@tu-dortmund.de
#include "AnalysisBase.h"

class Atlas_conf_2016_078 : public AnalysisBase {
  public:
    Atlas_conf_2016_078() : AnalysisBase()  {}               
    ~Atlas_conf_2016_078() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    bool Passes_Cuts(std::vector<Jet*> jets, ETMiss* missingET, float METCut,float PT1Cut, float PT2Cut,float PT3Cut, float PT4Cut,float PT5Cut, float PT6Cut, float NJet, float etaCut, float dPhiCut1, float dPhiCut2, float METHTCut, float AplanarityCut, float METMeffCut, float MeffCut, bool cutflow);
    float HT(std::vector<Jet*> jets);
    float dPhi(std::vector<Jet*> jets, ETMiss* missingET, int NJet, int j);
    float M_eff(std::vector<Jet*> jets, ETMiss* missingET, float j);
    bool Aplanarity_Cut( std::vector<Jet*> jets,float cut);
};

#endif
