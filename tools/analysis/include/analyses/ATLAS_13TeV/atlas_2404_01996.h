#ifndef ATLAS_2404_01996_H_
#define ATLAS_2404_01996_H_
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_2404_01996 : public AnalysisBase {
  public:
    Atlas_2404_01996() : AnalysisBase()  {}
    ~Atlas_2404_01996() {}

    void initialize();
    void analyze();
    void finalize();

  private:
};

#endif
