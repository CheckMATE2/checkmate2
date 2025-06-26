/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "MVAUtils/BDT.h"
#include "MVAUtils/ForestTMVA.h"
#include "MVAUtils/ForestLGBM.h"
#include "MVAUtils/ForestXGBoost.h"

#include "TTree.h"
#include <cmath>

#include <memory>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>

using namespace MVAUtils;

namespace{

/*  utility functions : to split option (e.g. "creator=lgbm;node=lgbm_simple")
*  in a std::map {{"creator", "lgbm"}, {"node", "lgbm_simple"}}
*/
std::string get_default_string_map(const std::map <std::string, std::string> & m,
                                   const std::string& key, const std::string & defval="")
{
   std::map<std::string, std::string>::const_iterator it = m.find(key);
   if (it == m.end()) { return defval; }
   return it->second;
}

std::map<std::string, std::string> parseOptions(const std::string& raw_options)
{
  std::stringstream ss(raw_options);
  std::map<std::string, std::string> options;
  std::string item;
  while (std::getline(ss, item, ';')) {
    auto pos = item.find_first_of('=');
    const auto right = item.substr(pos+1);
    const auto left = item.substr(0, pos);
    if (!options.insert(std::make_pair(left, right)).second)
    {
      throw std::runtime_error(std::string("option ") + left +
                               " duplicated in title of TTree used as input");
    }
  }

  return options;
}
}

/** c-tor from TTree **/
BDT::BDT(::TTree *tree)
{
  // at runtime decide which flavour of BDT we need to build
  // the information is coming from the title of the TTree
  
  if(!tree){
      throw std::runtime_error("nullptr to a TTree passed ");
  }
  tree->SetCacheSize(0); // Avoid unnecessary memory allocations
  std::map<std::string, std::string> options = parseOptions(tree->GetTitle());
  std::string creator = get_default_string_map(options, std::string("creator"));
  if (creator == "lgbm")
  {
    std::string node_type = get_default_string_map (options, std::string("node_type"));
    if (node_type == "lgbm") {
      m_forest = std::make_unique<ForestLGBM>(tree);
    } else if (node_type == "lgbm_simple") {
      m_forest = std::make_unique<ForestLGBMSimple>(
        tree); // this do not support nan as inputs
    } else {
      throw std::runtime_error(
        "the title of the input tree is misformatted: cannot understand which "
        "BDT implementation to use");
    }
  } else if (creator == "xgboost") {
    // this do support nan as inputs
    m_forest = std::make_unique<ForestXGBoost>(tree);
  } else {
    // default for compatibility: old TTree (based on TMVA) don't have a special title
    m_forest = std::make_unique<ForestTMVA>(tree);
  }
}


TTree* BDT::WriteTree(TString name) const { return m_forest->WriteTree(std::move(name)); }
void BDT::PrintForest() const { m_forest->PrintForest(); }
void BDT::PrintTree(unsigned int itree) const { m_forest->PrintTree(itree); }
