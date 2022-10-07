#ifndef ATLAS_2106_09609_H_
#define ATLAS_2106_09609_H_
// AUTHOR: JSK
//  EMAIL: jsk@th.physik.uni-bonn.de
#include "AnalysisBase.h"
#include </home/krolb/tools/CheckMATE/ONNX/tools/onnxruntime-linux-x64-1.12.1/include/onnxruntime_cxx_api.h>

class Atlas_2106_09609 : public AnalysisBase {
  public:
    Atlas_2106_09609() : AnalysisBase()  {}               
    ~Atlas_2106_09609() {}

    template <class X>
      std::vector <X*> removeLowMassResonances(std::vector <X*> candidates, double pTmin, double pTMax);

    template <class X>
      void findZ(std::vector<X*> candidates, std::vector <std::pair<double, double>> & zmassdiff);

    
    void initialize();
    void analyze();        
    void finalize();

  private:
    Ort::Session *session_4j;
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
    
    template <class X, class Y>
    std::vector<X*> specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours);
    
    static bool jetsortByPT(Jet *i, Jet *j);
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin);
};

#endif
