#ifndef CMS_2605_13614_H_
#define CMS_2605_13614_H_
// AUTHOR: Krzysztof Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
#include "AnalysisBase.h"
#ifdef HAVE_ONNX
#include <onnxruntime_cxx_api.h>
#endif

class Cms_2605_13614 : public AnalysisBase {
  public:
    Cms_2605_13614() : AnalysisBase()  {}               
    ~Cms_2605_13614() {}
  
    void initialize();
    void analyze();        
    void finalize();

  private:
  #ifdef HAVE_ONNX    
    Ort::Session *session;
    std::vector<int64_t> input_node_dims;
    const int input_tensor_size = 28;
    std::vector<const char*> input_names = {"input_1"};
    std::vector<const char*> output_names = {"dense_4"};
    std::vector<int64_t> input_dims = {1, 28};
  #endif    
};

#endif
