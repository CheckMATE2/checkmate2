#ifndef CMS_2111_06296_H_
#define CMS_2111_06296_H_
// AUTHOR: kr
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Cms_2111_06296 : public AnalysisBase {
  public:
    Cms_2111_06296() : AnalysisBase()  {}               
    ~Cms_2111_06296() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:

    bool SR_2l_low(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal);
    bool SR_2l_med(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal);
    bool SR_2l_high(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal);
    bool SR_2l_ultra(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal);
    bool SR_3l_low(std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal);
    bool SR_3l_med(std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal);

    bool CR_2l_DY_low(std::vector<FinalStateObject*> leptons, std::vector<FinalStateObject*> leptonsTight, std::vector<Jet*> jetsSignal);
    
    double mtautau(std::vector<FinalStateObject*> leptons);
};

#endif
