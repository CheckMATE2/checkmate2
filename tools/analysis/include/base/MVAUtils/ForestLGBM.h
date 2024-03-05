/*
  Copyright (C) 2002-2019 CERN for the benefit of the ATLAS collaboration
*/

#ifndef MVAUtils_ForestLGBM_H
#define MVAUtils_ForestLGBM_H

#include "MVAUtils/Forest.h"
#include <cmath>
#include <algorithm>
#include <numeric>
#include <vector>
namespace MVAUtils
{
    /* 
     * Support LGBM processing of the forest response.
     * 
     * User should use ForestLGBM (for nan input support) or ForestSimple
     * 
     * Implement only the classification as: sigmoid(raw-reponse)
     * Other methods are from Forest:
     * Multiclassification as softmax(raw-response)
     * Regression (GetResponse) as raw-response
     * */
    template<typename Node_t>
    class ForestLGBMBase : public Forest<Node_t>
    {
    public:
        using Forest<Node_t>::GetResponse;

        virtual float GetClassification(const std::vector<float>& values) const final
        {
            return detail::sigmoid(GetResponse(values));
        }
        virtual float GetClassification(const std::vector<float*>& pointers) const final
        {
            return detail::sigmoid(GetResponse(pointers));
        }
    };

    /** Implement LGBM Forest without nan support **/
    class ForestLGBMSimple final : public ForestLGBMBase<NodeLGBMSimple>
    {
    public:
        explicit ForestLGBMSimple(TTree* tree);
        ForestLGBMSimple() = default;
        ForestLGBMSimple (const ForestLGBMSimple&) = default;
        ForestLGBMSimple& operator=(const ForestLGBMSimple&) = default;
        ForestLGBMSimple (ForestLGBMSimple&&) = default;
        ForestLGBMSimple& operator=(ForestLGBMSimple&&) = default;
        ~ForestLGBMSimple()=default; 
        virtual TTree* WriteTree(TString name) const override;
        virtual void PrintForest() const override;
        virtual int GetNVars() const override { return m_max_var + 1; }
    private:
        int m_max_var=0;

    };

    /** Implement LGBM with nan support **/
    class ForestLGBM final : public ForestLGBMBase<NodeLGBM>
    {
    public:
        explicit ForestLGBM(TTree* tree);
        ForestLGBM() = default;
        ForestLGBM (const ForestLGBM&) = default;
        ForestLGBM& operator=(const ForestLGBM&) = default;
        ForestLGBM (ForestLGBM&&) = default;
        ForestLGBM& operator=(ForestLGBM&&) = default;
        ~ForestLGBM()=default;
        virtual TTree* WriteTree(TString name) const override;
        virtual void PrintForest() const override;
        virtual int GetNVars() const override { return m_max_var + 1; }
    private:
        int m_max_var=0;
    };
}

#endif
