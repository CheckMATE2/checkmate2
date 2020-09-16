#ifndef ATLAS_2003_11956_H_
#define ATLAS_2003_11956_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class DVertex {
   
    public: 
   
       double X(void);
       double Y(void);
       double Z(void);
       double D0(void);
       TLorentzVector P4(void);
       std::vector<GenParticle*> particles;
       void addtrack(GenParticle* track);
       void merge(DVertex vertex);
       DVertex(GenParticle* track);  //constructor
       ~DVertex() {}
       
    private:
        
        double xpos;
        double ypos;
        double zpos;
        double impd;
        TLorentzVector momentum;
        
};

class Atlas_2003_11956 : public AnalysisBase {
  public:
    Atlas_2003_11956() : AnalysisBase()  {}               
    ~Atlas_2003_11956() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
    void selection(int muons_count, double muon_ptmin, double muon_etamax, std::string sr);  
    std::vector< DVertex > DVfinder(double d0min, double ptmin, double distmin);
    std::vector<GenParticle*> Isolate_truemuons_with_inverse_track_isolation_cone(std::vector<GenParticle*> leptons, std::vector<Track*> tracks, std::vector<Tower*> towers, double dR_track_max, double pT_for_inverse_function_track, double dR_tower, double pT_amount_track, double pT_amount_tower, bool checkTower);
      
};


#endif
