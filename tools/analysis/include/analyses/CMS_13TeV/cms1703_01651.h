#ifndef CMS1703_01651_H_
#define CMS1703_01651_H_
// AUTHOR: Avinash Verma
//  EMAIL: avinash.verma@students.iiserpune.ac.in
#include "AnalysisBase.h"

class Cms1703_01651 : public AnalysisBase {
  public:
    Cms1703_01651() : AnalysisBase()  {}               
    ~Cms1703_01651() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
};

#endif
