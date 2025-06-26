/*
  Copyright (C) 2002-2022 CERN for the benefit of the ATLAS collaboration
*/

#include "MVAUtils/ForestXGBoost.h"
#include "TTree.h"
#include <iostream>
#include <stdexcept>

using namespace MVAUtils;

ForestXGBoost::ForestXGBoost(TTree* tree)
  : ForestXGBoostBase<NodeXGBoost>()
  , m_max_var(0)
{
    

    // variables read from the TTree
    std::vector<int> *vars = nullptr;
    std::vector<float> *values = nullptr;
    std::vector<bool> *default_left = nullptr;

    std::vector<NodeXGBoost> nodes;

    tree->SetBranchAddress("vars", &vars);
    tree->SetBranchAddress("values", &values);
    tree->SetBranchAddress("default_left", &default_left);

    for (int i = 0; i < tree->GetEntries(); ++i)
    {
        // each entry in the TTree is a decision tree
        tree->GetEntry(i);
        if (!vars) { throw std::runtime_error("vars pointer is null in ForestXGBoost constructor"); }
        if (!values) { throw std::runtime_error("values pointers is null in ForestXGBoost constructor"); }
        if (!default_left) { throw std::runtime_error("default_left pointers is null in ForestXGBoost constructor"); }
        if (vars->size() != values->size()) { throw std::runtime_error("inconsistent size for vars and values in ForestXGBoost constructor"); }
        if (default_left->size() != values->size()) { throw std::runtime_error("inconsistent size for default_left and values in ForestXGBoost constructor"); }

        nodes.clear();

        std::vector<MVAUtils::index_t> right = detail::computeRight(*vars);

        for (size_t i = 0; i < vars->size(); ++i) {
            nodes.emplace_back(vars->at(i), values->at(i), right[i], default_left->at(i));
            if (vars->at(i) > m_max_var) { m_max_var = vars->at(i); }
        }
        newTree(nodes);
    }  // end loop on TTree, all decision tree loaded
    delete vars;
    delete values;
    delete default_left;
}


TTree* ForestXGBoost::WriteTree(TString name) const
{
    TTree *tree = new TTree(name.Data(), "creator=xgboost");

    std::vector<int> vars;
    std::vector<float> values;
    std::vector<bool> default_left;

    tree->Branch("vars", &vars);
    tree->Branch("values", &values);
    tree->Branch("default_left", &default_left);

    for (size_t itree = 0; itree < GetNTrees(); ++itree) {
        vars.clear();
        values.clear();
        default_left.clear();
        for(const auto& node : GetTree(itree)) {
            vars.push_back(node.GetVar());
            values.push_back(node.GetVal());
            default_left.push_back(node.GetDefaultLeft());
        }
        tree->Fill();
    }
    return tree;
}

void ForestXGBoost::PrintForest() const
{
    std::cout << "***BDT XGBoost: Printing entire forest***" << std::endl;
    Forest::PrintForest();
}
