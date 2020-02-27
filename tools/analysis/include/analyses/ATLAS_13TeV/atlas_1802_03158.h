#ifndef ATLAS_1802_03158_H_
#define ATLAS_1802_03158_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_1802_03158 : public AnalysisBase {
  public:
    Atlas_1802_03158() : AnalysisBase()  {}               
    ~Atlas_1802_03158() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    double HT;
    bool SR_single_a(std::vector<Photon*> photons, std::vector<Jet*> jets, double PTa1Cut, int NJet, double METCut, double MeffCut, double R4TCut, bool cutflow, std::string sr);
    bool SR_double_a(std::vector<Photon*> photons, std::vector<Jet*> jets, double PTa1Cut, double METCut, double HTCut, double dPhiCut, bool cutflow, std::string sr);
};

#endif
