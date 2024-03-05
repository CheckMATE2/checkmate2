#ifndef MVA_H
#define MVA_H

#include "TRandom3.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "TMVA/MethodCuts.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"

#include "MVAUtils/BDT.h"
#include "TFile.h"
#include "TTree.h"

#include "lwtnn/LightweightGraph.hh"
#include "lwtnn/LightweightNeuralNetwork.hh"
#include "lwtnn/NNLayerConfig.hh"
#include "lwtnn/lightweight_network_config.hh"
#include "lwtnn/parse_json.hh"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include <onnxruntime/core/session/onnxruntime_cxx_api.h>
#pragma GCC diagnostic pop

class MVA {
public:
  MVA(const std::string &name, const std::vector<std::string> variableDefs = {}
      // specifies the order of variables in the input vector
      )
      : m_name(name), m_variableDefs(variableDefs){};

  void setEventNumber(int eventNumber) { m_eventNumber = eventNumber; };
  virtual double evaluate(const std::vector<double> &values,
                          const std::string nodeName = "") = 0;
  virtual double evaluateWeighted(const std::vector<double> & /*values*/,
                                  const std::string /*nodeName*/ = "") {
    throw std::runtime_error(
        "Weighted evaluation not supported for this type of MVA");
  };
  virtual std::vector<double>
  evaluateMulti(const std::vector<double> & /*values */, int /* numClasses */) {
    throw std::runtime_error("multi output not supported for this type of MVA");
  };
  virtual std::vector<float>
  evaluateMulti(const std::vector<float> & /*values */, int /* numClasses */) {
    throw std::runtime_error("multi output not supported for this type of MVA");
  };

protected:
  std::string m_name;
  std::vector<std::string> m_variableDefs;
  int m_eventNumber;
};

class TMVAReader : public MVA {
public:
  TMVAReader(const std::string &name,
             const std::vector<std::string> &variableDefs,
             const std::string fname1, const std::string fname2 = "");
  virtual double evaluate(const std::vector<double> &values,
                          const std::string nodeName = "");
  ~TMVAReader() {
    delete m_bdt1;
    delete m_bdt2;
  };

private:
  TMVA::Reader *m_bdt1; // for even eventnumber sample
  TMVA::Reader *m_bdt2; // for odd  eventnumber sample
  std::vector<Float_t> m_variables;
};

class MVAUtilsReader : public MVA {
public:
  MVAUtilsReader(const std::string &name, const std::string fname1,
                 const std::string fname2 = "");
  virtual double evaluate(const std::vector<double> &values,
                          const std::string nodeName = "");
  virtual double evaluateWeighted(const std::vector<double> &values,
                                  const std::string nodeName = "");
  virtual std::vector<double> evaluateMulti(const std::vector<double> &values,
                                            int numClasses);
  // to avoid warnings about hidden virtual functions
  virtual std::vector<float>
  evaluateMulti(const std::vector<float> & /*values */, int /* numClasses */) {
    throw std::runtime_error(
        "multi output with floats not supported for this type of MVA");
  };

  ~MVAUtilsReader() {
    delete m_bdt1;
    delete m_bdt2;
  };

private:
  MVAUtils::BDT *m_bdt1;
  MVAUtils::BDT *m_bdt2;
};

class LWTNNReader
    : public MVA { // FIXME: This class is not implemented/supported yet
public:
  LWTNNReader(const std::string &name, const std::string fname1,
              const std::string fname2 = "",
              const std::vector<std::string> variableDefs = {});
  virtual double evaluate(const std::vector<double> &values,
                          const std::string nodeName = "");
  ~LWTNNReader() {
    delete m_NN1;
    delete m_NN2;
  };

private:
  lwt::LightweightGraph *m_NN1;
  lwt::LightweightGraph *m_NN2;
};

class ONNXReader : public MVA {
public:
  ONNXReader(const std::string &name, const std::string fname1,
             const std::string fname2 = "",
             const std::vector<double> mean1 = {},
             const std::vector<double> std1 = {},
             const std::vector<double> mean2 = {},
             const std::vector<double> std2 = {});
  virtual double evaluate(const std::vector<double> &values,
                          const std::string nodeName = "");

  virtual std::vector<double> evaluateMulti(const std::vector<double> &values,
                                            int numClasses);
  virtual std::vector<float> evaluateMulti(const std::vector<float> &values,
                                           int numClasses);

  template <typename T>
  std::vector<Ort::Value> _evaluate(const std::vector<T> &values,
                                    const std::string /* nodeName */) {
    ONN *nn = &m_NN1;
    if (m_NN2.NN && ((m_eventNumber % 2) == 1)) {
      nn = &m_NN2;
    }

    auto memory_info =
        Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    std::vector<T> data(values.begin(), values.end());
    if (nn->mean.size() != 0) {
      for (size_t ii = 0; ii < values.size(); ii++) {
        data[ii] = (data.at(ii) - nn->mean.at(ii)) / nn->std.at(ii);
      }
    }

    Ort::Value input_tensor = Ort::Value::CreateTensor<T>(
        memory_info, data.data(), data.size(), nn->input_dimension.data(),
        nn->input_dimension.size());

    std::vector<Ort::Value> output_tensor =
        nn->NN->Run(Ort::RunOptions{nullptr}, nn->input_node_names.data(),
                    &input_tensor, nn->input_node_names.size(),
                    nn->output_node_names.data(), nn->output_node_names.size());
    return output_tensor;
  }

  ~ONNXReader() {
    delete m_NN1.NN;
    delete m_NN2.NN;
    delete m_NN1.env;
    delete m_NN2.env;
  };

private:
  struct ONN {
    Ort::Env *env;
    Ort::Session *NN;
    std::vector<int64_t> input_dimension;
    std::vector<const char *> input_node_names;
    std::vector<const char *> output_node_names;
    std::vector<double> mean;
    std::vector<double> std;
    void loadNN(const std::string &name, const std::vector<double> mean_in,
                const std::vector<double> std_in) {
      env = new Ort::Env(ORT_LOGGING_LEVEL_WARNING, name.c_str());
      Ort::SessionOptions sessionOptions;
      sessionOptions.SetIntraOpNumThreads(1);

      NN = new Ort::Session(*env, name.c_str(), sessionOptions);
      Ort::AllocatorWithDefaultOptions allocator;
      input_node_names.reserve(NN->GetInputCount());
      for (size_t ii = 0; ii < NN->GetInputCount(); ++ii) {
        input_node_names.push_back(NN->GetInputName(ii, allocator));
      }

      // FIXME: Here we are assuming only one input node
      Ort::TypeInfo type_info = NN->GetInputTypeInfo(0);
      auto tensor_info = type_info.GetTensorTypeAndShapeInfo();
      input_dimension = tensor_info.GetShape();
      // when we have a shape with -1, we need to replace with +1 since it
      // typically refers to batch size
      std::replace_if(
          input_dimension.begin(), input_dimension.end(),
          [](int64_t &ii) { return ii == -1; }, 1);

      output_node_names.reserve(NN->GetOutputCount());
      for (size_t ii = 0; ii < NN->GetOutputCount(); ++ii)
        output_node_names.push_back(NN->GetOutputName(ii, allocator));

      mean = mean_in;
      std = std_in;
    }
  };

  ONN m_NN1;
  ONN m_NN2;
};

#endif
