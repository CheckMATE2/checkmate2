#ifndef _ATLAS_1407_0583
#define _ATLAS_1407_0583

#include "AnalysisBase.h"

class Atlas_1407_0583 : public AnalysisBase {
  public:
    Atlas_1407_0583() : AnalysisBase() {};
    ~Atlas_1407_0583() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    

    static std::string ind_CF[2];
    static std::string name_CF[5];
    static std::string lep[2];
    static std::string hard_CF[2];

    static std::string bCa_Num[8];
    static std::string bCb_Num[8];
    
    static std::string tN_diag_Num[11];
    static std::string bCc_diag_Num[11];
    static std::string tN_med_Num[10];    
    static std::string tN_high_Num[11];
    static std::string bCb_med2_Num[14];
    static std::string bCd_bulk_Num[12];
    static std::string bCd_high1_Num[15];
    static std::string bCd_high2_Num[15];
    static std::string threeBody_Num[14];
    static std::string tNbC_mix_Num[13];
    
};


#endif
