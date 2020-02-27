#ifndef ATLAS_CONF_2017_060_H_
#define ATLAS_CONF_2017_060_H_
// AUTHOR: J.S.Kim
//  EMAIL: jsk@th.physik.uni-bonn.de
#include "AnalysisBase.h"

class Atlas_conf_2017_060 : public AnalysisBase {
  public:
    Atlas_conf_2017_060() : AnalysisBase()  {}               
    ~Atlas_conf_2017_060() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:

    float dPhi(std::vector<Jet*> jets, ETMiss* missingET);
    int countMatchingTracks(std::vector<Track*> tracks, Jet* jet, float minpt);
    void overlapRemoval_jm();
    static std::string signal_regions [20];
    static int signal_region_borders [20][2];
    
};

#endif
