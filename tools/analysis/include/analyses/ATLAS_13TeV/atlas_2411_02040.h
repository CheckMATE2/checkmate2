#ifndef ATLAS_2411_02040_H_
#define ATLAS_2411_02040_H_
// AUTHOR: KR
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"
#ifdef HAVE_ONNX
#include "onnxruntime_cxx_api.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#endif

class Atlas_2411_02040 : public AnalysisBase {
  public:
    Atlas_2411_02040() : AnalysisBase()  {}               
    ~Atlas_2411_02040() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:

    TFile *hfile;  TH1F *hist_mhradius; TH1F *hist_rmsdeltarjj; TH1F  *hist_rmsdeltarjjrivet, *hist_rmsmjj, *hist_rmsetah, *hist_rmsdeltaajj, *hist_rmsdeltaajjrivet, *hist_deltarh1, *hist_deltarh2, *hist_deltarh3, *hist_massh1, *hist_massh1rivet, *hist_ht6j, *hist_ht6jrivet, *hist_etamhhhfrac, *hist_costheta, *hist_aplanarity6j, *hist_sphericityallj, *hist_sphericity6j, *hist_transvsphericty6j, *hist_resdnnscore, *hist_nonresdnnscore, *hist_heavyresdnnscore;

#ifdef HAVE_ONNX
    Ort::Session *session[6];
    std::vector<int64_t> input_node_dims;
    const int input_tensor_size = 10;
    std::vector<const char*> input_names ;
    std::vector<const char*> output_names = {"dense_3", "dense_3", "dense_3", "dense_3", "dense_4", "dense_4"};
    std::vector<int64_t> input_dims = {1, 10};
    std::vector<std::vector<double>> scales;
    std::vector<std::vector<double>> offsets;
    template <typename T> std::vector<T> read_tree(boost::property_tree::ptree const& pt, boost::property_tree::ptree::key_type const& key);

#endif

    int eventNumber;
    static bool sortByPT(Jet *i, Jet *j);
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin);
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
    std::vector<float> Aplan_spher(std::vector<Jet*> input_jets, int r);
    float getRMS(std::vector<float> input);
    float getSkewness(std::vector<float> input);
    float getNN(std::vector<float> input, std::string name);
};

#endif
