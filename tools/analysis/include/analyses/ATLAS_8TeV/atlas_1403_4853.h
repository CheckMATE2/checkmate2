#ifndef _ATLAS_1403_4853
#define _ATLAS_1403_4853

#include "AnalysisBase.h"

class Atlas_1403_4853 : public AnalysisBase {
  public:
    Atlas_1403_4853() : AnalysisBase() {};
    ~Atlas_1403_4853() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    static std::string cf_index[12];
    double cutSingleTriggerElectronPT,cutSingleTriggerMuonPT,cutDoubleTriggerElectronPT,cutDoubleTriggerMuonPT,cutMixedTriggerMuonPT,cutMixedTriggerElectronPT;
    
    double leadingleptonpt;
    
    TLorentzVector pllTb;
    double mtest;
    
    double deltamin,deltatemp;
    int i_closestjet;

    bool trigger;
    

};


#endif
