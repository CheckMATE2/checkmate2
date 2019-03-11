#ifndef ATLAS_1708_07875_H_
#define ATLAS_1708_07875_H_
// AUTHOR:Junjie Cao, K. Rolbiecki, Liangliang Shang and Di Zhang
#include "AnalysisBase.h"

class Atlas_1708_07875 : public AnalysisBase {
  public:
    Atlas_1708_07875() : AnalysisBase()  {}               
    ~Atlas_1708_07875() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    void overlapRemoval_electrons_jets(std::vector<Electron*>& electrons, std::vector<Jet*>& jets);
};

#endif
