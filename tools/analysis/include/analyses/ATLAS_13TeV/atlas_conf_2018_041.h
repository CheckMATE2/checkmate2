#ifndef ATLAS_CONF_2018_041_H_
#define ATLAS_CONF_2018_041_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"

class Atlas_conf_2018_041 : public AnalysisBase {
  public:
    Atlas_conf_2018_041() : AnalysisBase()  {}               
    ~Atlas_conf_2018_041() {}
  
    void initialize();
    void analyze();        
    void finalize();

    private:
        std::vector<Jet*> bjets;
        std::vector<Jet*> filterBJets(std::vector<Jet*> jets);
	std::vector<fastjet::PseudoJet> reclustered_jets;
	static bool sortJetsByPT(Jet* a, Jet* b);

        // overlap removal
        void overlapRemoval_ej();
        void overlapRemoval_jm();
        void overlapRemoval_em();

        // isolation
        template<class T>
        std::vector<T*> isolateLeptons(std::vector<T*> leptons);

        // Count events in signal regions
        bool preselection();
        void sr_gbb();
        void sr_gbb_regions(std::string region, int nbjets, double mTminb, double minETmiss, double minMeff);

        void sr_gtt_0l();
        void sr_gtt_0l_regions(std::string region, int minNBjets, int minNjets, double mTminb, double minETmiss, double minMeff, double minMJsum);

        void sr_gtt_1l();
        void sr_gtt_1l_regions(std::string region, int minNjet, double minmT, double mTminb, double minETmiss, double minMeff, double minMJsum);
        double mBjets(std::vector<Jet*> bjets, ETMiss* missingET);
	
	
public:	
        static std::vector<fastjet::PseudoJet> convertJets(std::vector<Jet*> jets);
	static std::vector<fastjet::PseudoJet> recluster_jets(std::vector<Jet*> jets, double R, double fcut);
        static int countMatchingTracks(std::vector<Track*> tracks, Jet* jet);
        static double delta_phi_4j(std::vector<Jet*> jets, ETMiss* missingET);
        static double meff_4j(std::vector<Jet*> jets, ETMiss* missingET);
        static double meff_incl(
            std::vector<Jet*> jets,
            std::vector<Electron*> electrons,
            std::vector<Muon*> muons,
            ETMiss* missingET
            );

        static TLorentzVector leadingLeptonP4(std::vector<Electron*> electrons, std::vector<Muon*> muons);

        // The only difference to the original overlapRemoval is that this is safe to use if the two
        // input vectors are equal.
        template <class X, class Y>
        static std::vector<X*> overlapRemovalEqual(std::vector<X*> candidates, std::vector<Y*> neighbours, double dR) {
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
};

#endif
