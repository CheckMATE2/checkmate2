#ifndef ATLAS_CONF_2016_032_H_
#define ATLAS_CONF_2016_032_H_
// AUTHOR: Anke Biekoetter
//  EMAIL: biekoetter@physik.rwth-aachen.de
#include "AnalysisBase.h"

class Atlas_conf_2016_032 : public AnalysisBase {
  public:
    Atlas_conf_2016_032() : AnalysisBase()  {}               
    ~Atlas_conf_2016_032() {}
  
    void initialize();
    void analyze();        
    void finalize();
    void outputHistFile(TH1F* inHist);
    void outputHistFileI(TH1I* inHist);

  private:
    TH1I *hist_c_nbjets, *hist_d_njets, *hist_d_njets1, *hist_d_njets2, *hist_d_njets3, *hist_d_njets4, *hist_d_njets5, *hist_d_njets6;
    TH1F *hist_a_HT, *hist_b_MET;
    std::vector<Jet*> bjets;
    std::vector<Jet*> filterBJets(std::vector<Jet*> jets);
    
    // isolation
    template<class T>
    std::vector<T*> isolateLeptons(std::vector<T*> leptons, double deltaR);
    std::vector<Electron*> isolateElectrons(std::vector<Electron*> electrons, double deltaR);
    std::vector<Muon*> isolateMuons(std::vector<Muon*> muons, double deltaR);
    std::vector<Muon*> overlapRemoval_muon_jet_tracks(std::vector<Muon*> muons,std::vector<Jet*> jets, double deltaR, int nTracks);
	std::vector<Jet*> overlapRemoval_jet_muon_tracks(std::vector<Jet*> jets, std::vector<Muon*> muons, double deltaR, int nTracks);
};

#endif
