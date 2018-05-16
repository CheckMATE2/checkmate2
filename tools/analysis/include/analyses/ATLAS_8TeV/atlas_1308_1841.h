#ifndef _ATLAS_1308_1841
#define _ATLAS_1308_1841
// AUTHOR: kitpc
// EMAIL: kitpc
#include "AnalysisBase.h"

class Atlas_1308_1841 : public AnalysisBase {
  public:
    Atlas_1308_1841()  {}
    ~Atlas_1308_1841() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    ofstream fout1, fout2, fout3, fout4;
    static std::string namesCutflow[10];
};


#endif
