#ifndef ATLAS_1708_07875_H_
#define ATLAS_1708_07875_H_
// AUTHOR:Junjie Cao, Liangliang Shang, Jin Min Yang, Yuanfang Yue and Yang ZhangJunjie Cao, Liangliang Shang, Jin Min Yang, Yuanfang Yue and Yang Zhang
//  EMAIL: junjiec@itp.ac.cn,shlwell1988@gmail.com,jmyang@itp.ac.cn,yuanfang405@gmail.com,zhangyang@itp.ac.cn
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
