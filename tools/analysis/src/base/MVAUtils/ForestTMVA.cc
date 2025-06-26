/*
  Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
*/

#include "MVAUtils/ForestTMVA.h"
#include "TTree.h"
#include <iostream>
#include <stdexcept>

using namespace MVAUtils;

ForestTMVA::ForestTMVA(TTree* tree)
  : ForestWeighted<NodeTMVA>()
  , m_max_var(0)
{
    // variables read from the TTree
    std::vector<int> *vars = nullptr;
    std::vector<float> *values = nullptr;
    float offset;  // the offset is the weight

    std::vector<NodeTMVA> nodes;

    tree->SetBranchAddress("vars", &vars);
    tree->SetBranchAddress("values", &values);
    tree->SetBranchAddress("offset", &offset);

    int numEntries = tree->GetEntries();
    for (int entry = 0; entry < numEntries; ++entry) {
      // each entry in the TTree is a decision tree
      tree->GetEntry(entry);
      if (!vars) {
        throw std::runtime_error(
          "vars pointer is null in ForestTMVA constructor");
      }
      if (!values) {
        throw std::runtime_error(
          "values pointers is null in ForestTMVA constructor");
      }
      if (vars->size() != values->size()) {
        throw std::runtime_error(
          "inconsistent size for vars and values in ForestTMVA constructor");
      }

      nodes.clear();

      std::vector<MVAUtils::index_t> right = detail::computeRight(*vars);

      for (size_t i = 0; i < vars->size(); ++i) {
        nodes.emplace_back(vars->at(i), values->at(i), right[i]);
        if (vars->at(i) > m_max_var) {
          m_max_var = vars->at(i);
        }
      }
      newTree(nodes, offset);
    } // end loop on TTree, all decision tree loaded
    delete vars;
    delete values;
}


TTree* ForestTMVA::WriteTree(TString name) const
{
    TTree *tree = new TTree(name.Data(), "creator=TMVA");

    std::vector<int> vars;
    std::vector<float> values;
    float offset;

    tree->Branch("offset", &offset);
    tree->Branch("vars", &vars);
    tree->Branch("values", &values);

    for (size_t itree = 0; itree < GetNTrees(); ++itree) {
        vars.clear();
        values.clear();
        for(const auto& node : GetTree(itree)) {
            vars.push_back(node.GetVar());
            values.push_back(node.GetVal());
        }
        offset = GetTreeWeight(itree);
        tree->Fill();
    }
    return tree;
}

void ForestTMVA::PrintForest() const
{
    std::cout << "***BDT TMVA: Printing entire forest***" << std::endl;
    Forest::PrintForest();
}
