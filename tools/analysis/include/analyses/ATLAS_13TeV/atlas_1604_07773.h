#ifndef ATLAS_1604_07773_H_
#define ATLAS_1604_07773_H_
// AUTHOR: Tim Keller
//  EMAIL: tim.keller@rwth-aachen.de
#include "AnalysisBase.h"

class Atlas_1604_07773 : public AnalysisBase {
  public:
    Atlas_1604_07773() : AnalysisBase()  {}               
    ~Atlas_1604_07773() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    
    float dPhi(std::vector<Jet*> jets, ETMiss* missingET);
    int countMatchingTracks(std::vector<Track*> tracks, Jet* jet, float minpt);
    void overlapRemoval_jm();
    static std::string signal_regions [13];
    static int signal_region_borders [13][2];
    static std::string cut_flow_x_hist [24];
};



#endif
