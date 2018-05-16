#ifndef _CMS_PAS_SUS_15_011
#define _CMS_PAS_SUS_15_011
// AUTHOR: Anke Biekoetter
//  EMAIL: biekoetter@physik.rwth-aachen.de
#include "AnalysisBase.h"

class Cms_pas_sus_15_011 : public AnalysisBase {
  public:
    Cms_pas_sus_15_011() : AnalysisBase()  {}               
    ~Cms_pas_sus_15_011() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
  
  // isolation
        template<class T>
        std::vector<T*> isolateLeptons(std::vector<T*> leptons, double maxpTrel);
};


#endif
