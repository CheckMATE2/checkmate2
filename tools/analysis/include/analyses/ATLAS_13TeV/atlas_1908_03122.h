#ifndef ATLAS_1908_03122_H_
#define ATLAS_1908_03122_H_
// AUTHOR: Marvin M. Flores
//  EMAIL: mflores@nip.upd.edu.ph
#include "AnalysisBase.h"

class Atlas_1908_03122 : public AnalysisBase {
  public:
    Atlas_1908_03122() : AnalysisBase()  {}               
    ~Atlas_1908_03122() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
      
    void rotateXY(TMatrix &mat, TMatrix &mat_new, double phi);  
    double calcMETSignificance(std::vector<Jet*> jets);
};

#endif
