#ifndef ATLAS_2106_09609_H_
#define ATLAS_2106_09609_H_
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"
#ifdef HAVE_ONNX
#include "onnxruntime_cxx_api.h"
#endif

class Atlas_2106_09609 : public AnalysisBase {
  public:
    Atlas_2106_09609() : AnalysisBase()  {}               
    ~Atlas_2106_09609() {}

    
    void initialize();
    void analyze();        
    void finalize();

  private:
    template <class X>
      std::vector <X*> removeLowMassResonances(std::vector <X*> candidates, double pTmin, double pTMax);

    template <class X>
      void findZ(std::vector<X*> candidates, std::vector <std::pair<double, double>> & zmassdiff);    
      
    TFile *hfile;  TH1F *nn4j, *nn5j, *nn6j, *nn7j, *nn8j;  

#ifdef HAVE_ONNX    
    Ort::Session *session[5];
    std::vector<int64_t> input_node_dims;
    const int input_tensor_size = 65;
    std::vector<const char*> input_names = {"input.1"};
    std::vector<const char*> output_names = {"39"};
    std::vector<int64_t> input_dims = {1, 65};
#endif
    
    std::vector<Jet*> overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks);
    
    template <class X, class Y>
    std::vector<X*> specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours);
    
    static bool jetsortByPT(Jet *i, Jet *j);
    bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin);
    
    float Passes_Cuts_NNSR(const std::vector<Jet*>, const std::vector<Jet*>, const std::vector<FinalStateObject*>);
    float calc_three_jet_max_pt_mass(const std::vector<Jet*>, const TLorentzVector lepton = TLorentzVector(0,0,0,0), const TLorentzVector met = TLorentzVector(0,0,0,0));
    double HT(const std::vector<Jet*> jets);
    double min_dr(const std::vector<Jet*> sigjets, TLorentzVector lepton);
    double calc_minmax_mass(const std::vector<Jet*> sigjets);
    std::vector<float> jet_btag_category(const std::vector<Jet*> sigjets, const std::vector<Jet*> bjets);
    unsigned int countSetBits(unsigned int n);
};

#endif
