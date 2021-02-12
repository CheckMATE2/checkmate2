#ifndef ATLAS_2101_01629_H_
#define ATLAS_2101_01629_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_2101_01629 : public AnalysisBase {
  public:
    Atlas_2101_01629() : AnalysisBase()  {}               
    ~Atlas_2101_01629() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
      
      std::vector<Jet*> jets_signal, bjets;
      double met, meff, mT;
      TLorentzVector lepton;
      
      double Aplanarity();
      bool Passes_Cuts_2J(bool cutflow, std::string sr);
      bool Passes_Cuts_4J(bool cutflow, std::string sr);
      bool Passes_Cuts_6J(bool cutflow, std::string sr);
      
    template <class X, class Y>
    std::vector<X*> specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours);      
};

#endif
