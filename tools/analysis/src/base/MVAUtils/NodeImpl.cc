/*
  Copyright (C) 2002-2020 CERN for the benefit of the ATLAS collaboration
*/

#include "MVAUtils/NodeImpl.h"
#include <iostream>

using namespace MVAUtils;


void NodeTMVA::Print(index_t index) const
{
  std::cout << "     Variable: " << int(m_var) << ", Cut: " << m_cut
            << " (index = " << index << ")" << std::endl;
}


void NodeLGBMSimple::Print(index_t index) const
{
  std::cout << "     Variable: " << int(m_var) << ", Cut: " << m_cut
            << " (index = " << index << ")" << std::endl;
}


void NodeLGBM::Print(index_t index) const
{
  std::cout << "     Variable: " << int(m_var) << ", Cut: " << m_cut << ", DefaultLeft: " << (int)m_default_left
            << " (index = " << index << ")" << std::endl;
}

void NodeXGBoost::Print(index_t index) const
{
  std::cout << "     Variable: " << int(m_var) << ", Cut: " << m_cut << ", DefaultLeft: " << (int)m_default_left
            << " (index = " << index << ")" << std::endl;
}
