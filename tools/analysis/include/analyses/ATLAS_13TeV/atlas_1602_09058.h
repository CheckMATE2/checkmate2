#ifndef _ATLAS_1602_09058
#define _ATLAS_1602_09058
// AUTHOR: Jan schuette Engel
//  EMAIL: schuette@physik.rwth-aachen.de
#include "AnalysisBase.h"

class Atlas_1602_09058 : public AnalysisBase {
  public:
    Atlas_1602_09058() : AnalysisBase()  {}               
    ~Atlas_1602_09058() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    int getNumberOfBjets(std::vector<Jet*> jets);
    void overlapRemoval_jets_electrons(std::vector<Jet*>& jets,std::vector<Electron*>& electrons);
    void overlapRemoval_jets_muons(std::vector<Jet*>& jets,std::vector<Jet*>& bjets,std::vector<Muon*>& muons);
    void overlapRemoval_y(std::vector<Electron*>& electrons,std::vector<Jet*> jets, double dR);
  
    static std::string cf_index[7];
};


#endif
