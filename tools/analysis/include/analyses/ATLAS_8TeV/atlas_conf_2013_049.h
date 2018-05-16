#ifndef _ATLAS_CONF_2013_049
#define _ATLAS_CONF_2013_049

#include "AnalysisBase.h"

class Atlas_conf_2013_049 : public AnalysisBase {
  public:
    Atlas_conf_2013_049() : AnalysisBase() {};
    ~Atlas_conf_2013_049() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    static std::string id[10];
    double cutTriggerElectronPT,cutTriggerElectronPT1,cutTriggerElectronPT2;
    double cutTriggerMuonPT,cutTriggerMuonPT1,cutTriggerMuonPT2;
    double cutTriggermixedElectronPT1,cutTriggermixedMuonPT1,cutTriggermixedElectronPT2,cutTriggermixedMuonPT2;    
    bool trigger;    
    double deltaphi_min,deltaphi_temp,missingETrel;    
    double mll,mt2,ptll,deltaphi;    
    TH1F *hist_minv;
};


class Atlas_conf_2013_049_CR : public AnalysisBase {
  public:
    Atlas_conf_2013_049_CR() : AnalysisBase() {};
    ~Atlas_conf_2013_049_CR() {}
    
    void initialize();
    void analyze();        
    void finalize();

  private:

};

#endif
