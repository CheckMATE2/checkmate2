#ifndef CMS_SUS_16_039_H_
#define CMS_SUS_16_039_H_
// AUTHOR: none
//  EMAIL: none
#include "AnalysisBase.h"

class Cms_sus_16_039 : public AnalysisBase {
  public:
    Cms_sus_16_039() : AnalysisBase()  {}               
    ~Cms_sus_16_039() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:

    template<class T>
      std::vector<T*> isolateLeptons(std::vector<T*> leptons, double maxpTrel);

    TH1D *h1, *h2;
  
    const double mZ = 91.0;
    
};

#endif
