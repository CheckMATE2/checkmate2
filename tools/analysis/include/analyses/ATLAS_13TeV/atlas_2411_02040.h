#ifndef ATLAS_2411_02040_H_
#define ATLAS_2411_02040_H_
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"
#ifdef HAVE_ONNX
#include "onnxruntime_cxx_api.h"
#endif

class Atlas_2411_02040 : public AnalysisBase {
  public:
    Atlas_2411_02040() : AnalysisBase()  {}               
    ~Atlas_2411_02040() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:

    TFile *hfile;  TH1F *hist_mhradius; TH1F *hist_rmsdeltarjj;

#ifdef HAVE_ONNX
    Ort::Session *session[6];
    std::vector<int64_t> input_node_dims;
    const int input_tensor_size = 10;
    std::vector<const char*> input_names = {"input"};
    std::vector<const char*> output_names = {"dense_3"};
    std::vector<int64_t> input_dims = {1, 10};
#endif

    static bool sortByPT(Jet *i, Jet *j);
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin);
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
};

#endif
