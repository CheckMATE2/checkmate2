#ifndef _ATLAS_1501_07110
#define _ATLAS_1501_07110
// AUTHOR: kitpc
//  EMAIL: shlwell1988@gmail.com
#include "AnalysisBase.h"

class Atlas_1501_07110 : public AnalysisBase {
  public:
    Atlas_1501_07110() : AnalysisBase() {};             
    ~Atlas_1501_07110() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    struct sortPT {
      inline bool operator() (const Jet *i, const Jet *j) {
        return (i->PT > j->PT);
      }
    };  

    ofstream file1;

    TH1F *hist1, *hist2, *hist3, *hist4;

};


#endif
