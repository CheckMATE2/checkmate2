#ifndef _ATLAS_1605_03814
#define _ATLAS_1605_03814
// AUTHOR: Tim Keller
//  EMAIL: tim.keller@rwth-aachen.de
#include "AnalysisBase.h"

class Atlas_1605_03814 : public AnalysisBase {
  public:
    Atlas_1605_03814() : AnalysisBase()  {}               
    ~Atlas_1605_03814() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    bool Passes_Cuts(std::vector<Jet*> jets, ETMiss* missingET, float METCut,float PT1Cut, float PT2Cut, float NJet, float dPhiCut1, float dPhiCut2, float METHTCut, float AplanarityCut, float METMeffCut, float MeffCut, bool cutflow);
    float HT(std::vector<Jet*> jets);
    float dPhi(std::vector<Jet*> jets, ETMiss* missingET, int NJet, int j);
    float M_eff(std::vector<Jet*> jets, ETMiss* missingET, float j);
    bool Aplanarity_Cut( std::vector<Jet*> jets,float cut);
    
    
};


#endif
