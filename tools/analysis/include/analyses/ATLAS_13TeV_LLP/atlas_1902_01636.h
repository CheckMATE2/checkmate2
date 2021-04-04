#ifndef ATLAS_1902_01636_H_
#define ATLAS_1902_01636_H_
// AUTHOR: Roberto Ruiz
//  EMAIL: rruiz@ific.uv.es
#include "AnalysisBase.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TH2.h"

class Atlas_1902_01636 : public AnalysisBase {
  public:
    Atlas_1902_01636() : AnalysisBase()  {}               
    ~Atlas_1902_01636() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:

    TRandom3 myRandom;

    TFile *SingleMuTurnOn_file;
    TFile *EtmissTurnOn_file;
    TFile *LooseEff_file;
    TFile *TightPromotionEff_file;
    TFile *MToFFullDet_file;

    TH2F *SingleMuTurnOn;
    TH1F *EtmissTurnOn;
    TH2F *LooseEff;
    TH2F *TightPromotionEff;
    TH1F *MToFFullDet_mean;
    TH1F *MToFFullDet_std;
    
    bool decayInsideAtlas(GenParticle* part); 

};

//All this is taken from rivet
enum Location { nj=1, nq3, nq2, nq1, nl, nr, n, n8, n9, n10 };

/// Split the PID into constituent integers
inline unsigned short _digit(Location loc, int pid) {
      //  PID digits (base 10) are: n nr nl nq1 nq2 nq3 nj (cf. Location)
      int numerator = (int) std::pow(10.0, (loc-1));
      return (abs(pid)/numerator) % 10;
}

inline int _extraBits(int pid) {
      return abs(pid)/10000000;
}

/// @brief Return the first two digits if this is a "fundamental" particle
/// @note ID = 100 is a special case (internal generator ID's are 81-100)
inline int _fundamentalID(int pid) {
      if (_extraBits(pid) > 0) return 0;
      if (_digit(nq2,pid) == 0 && _digit(nq1,pid) == 0) {
        return abs(pid) % 10000;
      } else if (abs(pid) <= 100) {
        return abs(pid);
      } else {
        return 0;
      }
}

inline bool isSUSY(int pid) {

      // Fundamental SUSY particles have n = 1 or 2
      if (_extraBits(pid) > 0) return false;
      if (_digit(n,pid) != 1 && _digit(n,pid) != 2)  return false;
      if (_digit(nr,pid) != 0)  return false;
      // Check fundamental part for SM PID on which it is based
      if (_fundamentalID(pid) == 0)  return false;
      return true;
    
} 

#endif
