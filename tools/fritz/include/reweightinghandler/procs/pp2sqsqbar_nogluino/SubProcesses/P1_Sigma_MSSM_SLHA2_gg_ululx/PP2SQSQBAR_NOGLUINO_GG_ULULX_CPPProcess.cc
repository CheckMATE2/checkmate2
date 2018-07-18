//==========================================================================
// This file has been automatically generated for C++ Standalone by
// MadGraph5_aMC@NLO v. 2.5.1, 2016-11-04
// By the MadGraph5_aMC@NLO Development Team
// Visit launchpad.net/madgraph5 and amcatnlo.web.cern.ch
//==========================================================================

#include "../../src/pp2sqsqbar_nogluino_HelAmps_MSSM_SLHA2.h"
#include "PP2SQSQBAR_NOGLUINO_GG_ULULX_CPPProcess.h"

using namespace PP2SQSQBAR_NOGLUINO_MG5_MSSM_SLHA2; 

//==========================================================================
// Class member functions for calculating the matrix elements for
// Process: g g > ul ul~ WEIGHTED<=2 / go @1

//--------------------------------------------------------------------------
// Initialize process.

void PP2SQSQBAR_NOGLUINO_GG_ULULX_CPPProcess::initProc(string param_card_name) 
{
  // Instantiate the model class and set parameters that stay fixed during run
  pars = pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2::getInstance(); 
  SLHAReader slha(param_card_name); 
  pars->setIndependentParameters(slha); 
  pars->setIndependentCouplings(); 


  // Set external particle masses for this matrix element
  mME.push_back(pars->ZERO); 
  mME.push_back(pars->ZERO); 
  mME.push_back(pars->mdl_Msu1); 
  mME.push_back(pars->mdl_Msu1); 
  jamp2[0] = new double[2]; 
}

//--------------------------------------------------------------------------
// Evaluate |M|^2, part independent of incoming flavour.

void PP2SQSQBAR_NOGLUINO_GG_ULULX_CPPProcess::sigmaKin() 
{
  // Set the parameters which change event by event
  pars->setDependentParameters(); 
  pars->setDependentCouplings(); 
  static bool firsttime = true; 
  if (firsttime)
  {


    firsttime = false; 
  }

  // Reset color flows
  for(int i = 0; i < 2; i++ )
    jamp2[0][i] = 0.; 

  // Local variables and constants
  const int ncomb = 4; 
  static bool goodhel[ncomb] = {ncomb * false}; 
  static int ntry = 0, sum_hel = 0, ngood = 0; 
  static int igood[ncomb]; 
  static int jhel; 
  std::complex<double> * * wfs; 
  double t[nprocesses]; 
  // Helicities for the process
  static const int helicities[ncomb][nexternal] = {{-1, -1, 0, 0}, {-1, 1, 0,
      0}, {1, -1, 0, 0}, {1, 1, 0, 0}};
  // Denominators: spins, colors and identical particles
  const int denominators[nprocesses] = {256}; 

  ntry = ntry + 1; 

  // Reset the matrix elements
  for(int i = 0; i < nprocesses; i++ )
  {
    matrix_element[i] = 0.; 
  }
  // Define permutation
  int perm[nexternal]; 
  for(int i = 0; i < nexternal; i++ )
  {
    perm[i] = i; 
  }

  if (sum_hel == 0 || ntry < 10)
  {
    // Calculate the matrix element for all helicities
    for(int ihel = 0; ihel < ncomb; ihel++ )
    {
      if (goodhel[ihel] || ntry < 2)
      {
        calculate_wavefunctions(perm, helicities[ihel]); 
        t[0] = matrix_1_gg_ululx_no_go(); 

        double tsum = 0; 
        for(int iproc = 0; iproc < nprocesses; iproc++ )
        {
          matrix_element[iproc] += t[iproc]; 
          tsum += t[iproc]; 
        }
        // Store which helicities give non-zero result
        if (tsum != 0. && !goodhel[ihel])
        {
          goodhel[ihel] = true; 
          ngood++; 
          igood[ngood] = ihel; 
        }
      }
    }
    jhel = 0; 
    sum_hel = min(sum_hel, ngood); 
  }
  else
  {
    // Only use the "good" helicities
    for(int j = 0; j < sum_hel; j++ )
    {
      jhel++; 
      if (jhel >= ngood)
        jhel = 0; 
      double hwgt = double(ngood)/double(sum_hel); 
      int ihel = igood[jhel]; 
      calculate_wavefunctions(perm, helicities[ihel]); 
      t[0] = matrix_1_gg_ululx_no_go(); 

      for(int iproc = 0; iproc < nprocesses; iproc++ )
      {
        matrix_element[iproc] += t[iproc] * hwgt; 
      }
    }
  }

  for (int i = 0; i < nprocesses; i++ )
    matrix_element[i] /= denominators[i]; 



}

//--------------------------------------------------------------------------
// Evaluate |M|^2, including incoming flavour dependence.

double PP2SQSQBAR_NOGLUINO_GG_ULULX_CPPProcess::sigmaHat() 
{
  // Select between the different processes
  if(id1 == 21 && id2 == 21)
  {
    // Add matrix elements for processes with beams (21, 21)
    return matrix_element[0]; 
  }
  else
  {
    // Return 0 if not correct initial state assignment
    return 0.; 
  }
}

//==========================================================================
// Private class member functions

//--------------------------------------------------------------------------
// Evaluate |M|^2 for each subprocess

void PP2SQSQBAR_NOGLUINO_GG_ULULX_CPPProcess::calculate_wavefunctions(const int perm[], const int hel[])
{
  // Calculate wavefunctions for all processes
  int i, j; 

  // Calculate all wavefunctions
  vxxxxx(p[perm[0]], mME[0], hel[0], -1, w[0]); 
  vxxxxx(p[perm[1]], mME[1], hel[1], -1, w[1]); 
  sxxxxx(p[perm[2]], +1, w[2]); 
  sxxxxx(p[perm[3]], +1, w[3]); 
  VVV1P0_1(w[0], w[1], pars->GC_6, pars->ZERO, pars->ZERO, w[4]); 
  VSS1_2(w[0], w[2], pars->GC_53, pars->mdl_Msu1, pars->mdl_Wsu1, w[5]); 
  VSS1_3(w[0], w[3], pars->GC_53, pars->mdl_Msu1, pars->mdl_Wsu1, w[6]); 

  // Calculate all amplitudes
  // Amplitude(s) for diagram number 0
  VVSS1_0(w[0], w[1], w[3], w[2], pars->GC_96, amp[0]); 
  VVSS1_0(w[0], w[1], w[3], w[2], pars->GC_96, amp[1]); 
  VSS1_0(w[4], w[3], w[2], pars->GC_53, amp[2]); 
  VSS1_0(w[1], w[3], w[5], pars->GC_53, amp[3]); 
  VSS1_0(w[1], w[6], w[2], pars->GC_53, amp[4]); 

}
double PP2SQSQBAR_NOGLUINO_GG_ULULX_CPPProcess::matrix_1_gg_ululx_no_go() 
{
  int i, j; 
  // Local variables
  const int ngraphs = 5; 
  const int ncolor = 2; 
  std::complex<double> ztemp; 
  std::complex<double> jamp[ncolor]; 
  // The color matrix;
  static const double denom[ncolor] = {3, 3}; 
  static const double cf[ncolor][ncolor] = {{16, -2}, {-2, 16}}; 

  // Calculate color flows
  jamp[0] = +amp[1] - std::complex<double> (0, 1) * amp[2] + amp[3]; 
  jamp[1] = +amp[0] + std::complex<double> (0, 1) * amp[2] + amp[4]; 

  // Sum and square the color flows to get the matrix element
  double matrix = 0; 
  for(i = 0; i < ncolor; i++ )
  {
    ztemp = 0.; 
    for(j = 0; j < ncolor; j++ )
      ztemp = ztemp + cf[i][j] * jamp[j]; 
    matrix = matrix + real(ztemp * conj(jamp[i]))/denom[i]; 
  }

  // Store the leading color flows for choice of color
  for(i = 0; i < ncolor; i++ )
    jamp2[0][i] += real(jamp[i] * conj(jamp[i])); 

  return matrix; 
}



