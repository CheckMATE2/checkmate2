#ifndef ATLAS_CONF_2016_013_H_
#define ATLAS_CONF_2016_013_H_
// AUTHOR: Anke Biekoetter
//  EMAIL: biekoetter@physik.rwth-aachen.de
#include "AnalysisBase.h"

class Atlas_conf_2016_013 : public AnalysisBase {
  public:
    Atlas_conf_2016_013() : AnalysisBase()  {}               
    ~Atlas_conf_2016_013() {}
  
    void initialize();
    void analyze();        
    void finalize();
    void outputHistFile(TH1F* inHist);
    void outputHistFileI(TH1I* inHist);
    
  private:
  	std::vector<Jet*> bjets;
    std::vector<Jet*> filterBJets(std::vector<Jet*> jets);
  	int masstagged;
  	std::vector<double> masslargeRjet; 	

    // overlap removal
    void overlapRemoval_ej();
    void overlapRemoval_jm();
    void overlapRemoval_em();

    // reclustering
    void recluster_top_jets();
    
    // isolation
        template<class T>
        std::vector<T*> isolateLeptons(std::vector<T*> leptons, double deltaR);
    
    double invMass_minDeltaR(std::vector<Jet*> bjets);
    
    //histograms
    TH1I *hist_a_jet, *hist_b_bjet, *hist_d_masstagged;
    TH1F *hist_c_largeRmass, *hist_e_mbb, *hist_f_meff, *hist_g_meff2b, *hist_g_meff3b, *hist_g_meffm4b;
    
  public:
    static double meff(
            std::vector<Jet*> jets,
            std::vector<Electron*> electrons,
            std::vector<Muon*> muons,
            ETMiss* missingET
            );
    
};

static int countMatchingTracks(std::vector<Track*> tracks, Jet* jet);

// The only difference to the original overlapRemoval is that this is safe to use if the two
// input vectors are equal.
template <class X, class Y>
std::vector<X*> overlapRemovalEqual(std::vector<X*> candidates, std::vector<Y*> neighbours, double dR) {
// If neighbours are empty, return candidates
    if(neighbours.size() == 0) {
        return candidates;
    }
    std::vector<X*> passed_candidates;
    // Loop over candidates
    for(int i = 0; i < candidates.size(); i++) {
        bool overlap = false;
        // If a neighbour is too close, declare overlap, break and don't save candidate
        for(int j = 0; j < neighbours.size(); j++) {
            // Test if the candidate is the same as the neighbor, if so skip the test
            if (candidates[i] == neighbours[j]) {
                continue;
            }
            if (candidates[i]->P4().DeltaR(neighbours[j]->P4()) < dR) {
                overlap = true;
                break;
            }
        }
        if (!overlap) {
            passed_candidates.push_back(candidates[i]);
        }
    }
    return passed_candidates;
}

#endif
