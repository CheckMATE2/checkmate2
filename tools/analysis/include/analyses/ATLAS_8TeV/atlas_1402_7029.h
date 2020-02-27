#ifndef _ATLAS_1402_7029
#define _ATLAS_1402_7029

#include "AnalysisBase.h"

class Atlas_1402_7029 : public AnalysisBase {
  public:
    Atlas_1402_7029() : AnalysisBase() {};
    ~Atlas_1402_7029() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    double cutSingleTriggerElectronPT, cutSymTriggerElectronPT, cutAsymTriggerElectronPT1, cutAsymTriggerElectronPT2;
    double cutSingleTriggerMuonPT, cutSymTriggerMuonPT, cutAsymTriggerMuonPT1, cutAsymTriggerMuonPT2;
    double cutMixedTrigger1ElectronPT, cutMixedTrigger2ElectronPT, cutMixedTrigger1MuonPT, cutMixedTrigger2MuonPT;
};


#endif
