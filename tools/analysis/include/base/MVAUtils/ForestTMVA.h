/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

#ifndef MVAUtils_ForestTMVA_H
#define MVAUtils_ForestTMVA_H

#include "MVAUtils/Forest.h"
#include "TTree.h"
#include <vector>

namespace MVAUtils
{

    /** Implement a Forest with weighted nodes
     * This a general Forest class which implement the strategy used by TMVA
     * in some cases.
     * Each node has a weight that can be used to compute GetTreeResponseWeighted
     * In some cases an offset is needed, which is just the first weight (actually
     * in TMVA all the weights are identical when the offset is used).
     **/
    template<typename Node_t>
    class ForestWeighted : public Forest<Node_t>
    {
    public:
        ForestWeighted() : m_sumWeights(0.) { }

        using Forest<Node_t>::GetNTrees;
        using Forest<Node_t>::newTree;

        float GetTreeResponseWeighted(const std::vector<float>& values, unsigned int itree) const;
        float GetTreeResponseWeighted(const std::vector<float*>& pointers, unsigned int itree) const;

        float GetWeightedResponse(const std::vector<float>& values) const;
        float GetWeightedResponse(const std::vector<float*>& pointers) const;
       
        void newTree(const std::vector<Node_t>& nodes, float weight);
        float GetTreeWeight(unsigned int itree) const { return m_weights[itree]; }
        float GetSumWeights() const { return m_sumWeights; }

        virtual float GetOffset() const override { return m_weights[0]; }

        virtual void PrintTree(unsigned int itree) const override {
          std::cout << "weight: " << m_weights[itree] << std::endl;
          Forest<Node_t>::PrintTree(itree);
        }
 
    private:
        std::vector<float> m_weights; //!< boost weights
        float m_sumWeights; //!< the sumOfBoostWeights--no need to recompute each call
    };


    /*
     * Support TMVA processing
     * 
     * Forest implementing the TMVA forest.
     * 
     * Regression (GetResponse): offset + raw-response
     * Classification (GetClassification): weighted average of the nodes
     * MultiClassification: softmax of the raw-response
     */
    class ForestTMVA final : public ForestWeighted<NodeTMVA>
    {
    public:

        using ForestWeighted<NodeTMVA>::newTree;
        explicit ForestTMVA(TTree* tree);
        ForestTMVA() = default;
        ForestTMVA (const ForestTMVA&) = default;
        ForestTMVA& operator=(const ForestTMVA&)=default;
        ForestTMVA (ForestTMVA&&) = default;
        ForestTMVA& operator=(ForestTMVA&&) = default;
        ~ForestTMVA()=default;
        virtual TTree* WriteTree(TString name) const override;
        virtual float GetResponse(const std::vector<float>& values) const override ;
        virtual float GetResponse(const std::vector<float*>& pointers) const override;
        virtual float GetClassification(const std::vector<float>& values) const override;
        virtual float GetClassification(const std::vector<float*>& pointers) const override ;
        virtual void PrintForest() const override;
        virtual int GetNVars() const override { return m_max_var + 1; }
        void setNVars(const int max_var) {m_max_var=max_var;}
    private:
        int m_max_var=0;
    };

}
#include "MVAUtils/ForestTMVA.icc"
#endif
