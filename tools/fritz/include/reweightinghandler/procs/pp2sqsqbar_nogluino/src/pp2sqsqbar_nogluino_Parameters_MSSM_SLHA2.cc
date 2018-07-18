//==========================================================================
// This file has been automatically generated for C++ by
// MadGraph5_aMC@NLO v. 2.5.1, 2016-11-04
// By the MadGraph5_aMC@NLO Development Team
// Visit launchpad.net/madgraph5 and amcatnlo.web.cern.ch
//==========================================================================

#include <iostream> 
#include <iomanip> 
#include "pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2.h"

// Initialize static instance
pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2 * pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2::instance = 0; 

// Function to get static instance - only one instance per program
pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2 * pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2::getInstance()
{
	//if (instance == 0)
    instance = new pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2(); 

  return instance; 
}

void pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2::setIndependentParameters(SLHAReader& slha)
{
  // Define "zero"
  zero = 0; 
  ZERO = 0; 
  // Prepare a vector for indices
  vector<int> indices(2, 0); 
  mdl_Wsl6 = slha.get_block_entry("decay", 2000015, 2.699061e-01); 
  mdl_Wsl5 = slha.get_block_entry("decay", 2000013, 2.161216e-01); 
  mdl_Wsl4 = slha.get_block_entry("decay", 2000011, 2.161216e-01); 
  mdl_Wsu6 = slha.get_block_entry("decay", 2000006, 7.373133e+00); 
  mdl_Wsd6 = slha.get_block_entry("decay", 2000005, 8.015663e-01); 
  mdl_Wsu5 = slha.get_block_entry("decay", 2000004, 1.152973e+00); 
  mdl_Wsd5 = slha.get_block_entry("decay", 2000003, 2.858123e-01); 
  mdl_Wsu4 = slha.get_block_entry("decay", 2000002, 1.152973e+00); 
  mdl_Wsd4 = slha.get_block_entry("decay", 2000001, 2.858123e-01); 
  mdl_Wch2 = slha.get_block_entry("decay", 1000037, 2.486895e+00); 
  mdl_Wneu4 = slha.get_block_entry("decay", 1000035, 2.585851e+00); 
  mdl_Wneu3 = slha.get_block_entry("decay", 1000025, 1.915985e+00); 
  mdl_Wch1 = slha.get_block_entry("decay", 1000024, 1.704145e-02); 
  mdl_Wneu2 = slha.get_block_entry("decay", 1000023, 2.077700e-02); 
  mdl_Wgo = slha.get_block_entry("decay", 1000021, 5.506754e+00); 
  mdl_Wsn3 = slha.get_block_entry("decay", 1000016, 1.475190e-01); 
  mdl_Wsl3 = slha.get_block_entry("decay", 1000015, 1.483273e-01); 
  mdl_Wsn2 = slha.get_block_entry("decay", 1000014, 1.498816e-01); 
  mdl_Wsl2 = slha.get_block_entry("decay", 1000013, 2.136822e-01); 
  mdl_Wsn1 = slha.get_block_entry("decay", 1000012, 1.498816e-01); 
  mdl_Wsl1 = slha.get_block_entry("decay", 1000011, 2.136822e-01); 
  mdl_Wsu3 = slha.get_block_entry("decay", 1000006, 2.021596e+00); 
  mdl_Wsd3 = slha.get_block_entry("decay", 1000005, 3.736276e+00); 
  mdl_Wsu2 = slha.get_block_entry("decay", 1000004, 5.477195e+00); 
  mdl_Wsd2 = slha.get_block_entry("decay", 1000003, 5.312788e+00); 
  mdl_Wsu1 = slha.get_block_entry("decay", 1000002, 5.477195e+00); 
  mdl_Wsd1 = slha.get_block_entry("decay", 1000001, 5.312788e+00); 
  mdl_WH = slha.get_block_entry("decay", 37, 5.469628e-01); 
  mdl_WA0 = slha.get_block_entry("decay", 36, 6.321785e-01); 
  mdl_WH02 = slha.get_block_entry("decay", 35, 5.748014e-01); 
  mdl_WH01 = slha.get_block_entry("decay", 25, 1.986108e-03); 
  mdl_WW = slha.get_block_entry("decay", 24, 2.002822e+00); 
  mdl_WZ = slha.get_block_entry("decay", 23, 2.411433e+00); 
  mdl_WT = slha.get_block_entry("decay", 6, 1.561950e+00); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_Ryu3x3 = slha.get_block_entry("yu", indices, 8.928445e-01); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_Rye3x3 = slha.get_block_entry("ye", indices, 1.008908e-01); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_Ryd3x3 = slha.get_block_entry("yd", indices, 1.388402e-01); 
  indices[0] = 2; 
  indices[1] = 2; 
  mdl_RVV2x2 = slha.get_block_entry("vmix", indices, 9.725578e-01); 
  indices[0] = 2; 
  indices[1] = 1; 
  mdl_RVV2x1 = slha.get_block_entry("vmix", indices, 2.326612e-01); 
  indices[0] = 1; 
  indices[1] = 2; 
  mdl_RVV1x2 = slha.get_block_entry("vmix", indices, -2.326612e-01); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RVV1x1 = slha.get_block_entry("vmix", indices, 9.725578e-01); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_RCKM3x3 = slha.get_block_entry("vckm", indices, 1.000000e+00); 
  indices[0] = 2; 
  indices[1] = 2; 
  mdl_RCKM2x2 = slha.get_block_entry("vckm", indices, 1.000000e+00); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RCKM1x1 = slha.get_block_entry("vckm", indices, 1.000000e+00); 
  indices[0] = 6; 
  indices[1] = 6; 
  mdl_RRu6x6 = slha.get_block_entry("usqmix", indices, -5.536450e-01); 
  indices[0] = 6; 
  indices[1] = 3; 
  mdl_RRu6x3 = slha.get_block_entry("usqmix", indices, 8.327528e-01); 
  indices[0] = 5; 
  indices[1] = 5; 
  mdl_RRu5x5 = slha.get_block_entry("usqmix", indices, 1.000000e+00); 
  indices[0] = 4; 
  indices[1] = 4; 
  mdl_RRu4x4 = slha.get_block_entry("usqmix", indices, 1.000000e+00); 
  indices[0] = 3; 
  indices[1] = 6; 
  mdl_RRu3x6 = slha.get_block_entry("usqmix", indices, 8.327528e-01); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_RRu3x3 = slha.get_block_entry("usqmix", indices, 5.536450e-01); 
  indices[0] = 2; 
  indices[1] = 2; 
  mdl_RRu2x2 = slha.get_block_entry("usqmix", indices, 1.000000e+00); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RRu1x1 = slha.get_block_entry("usqmix", indices, 1.000000e+00); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_RMNS3x3 = slha.get_block_entry("upmns", indices, 1.000000e+00); 
  indices[0] = 2; 
  indices[1] = 2; 
  mdl_RMNS2x2 = slha.get_block_entry("upmns", indices, 1.000000e+00); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RMNS1x1 = slha.get_block_entry("upmns", indices, 1.000000e+00); 
  indices[0] = 2; 
  indices[1] = 2; 
  mdl_RUU2x2 = slha.get_block_entry("umix", indices, 9.168349e-01); 
  indices[0] = 2; 
  indices[1] = 1; 
  mdl_RUU2x1 = slha.get_block_entry("umix", indices, 3.992666e-01); 
  indices[0] = 1; 
  indices[1] = 2; 
  mdl_RUU1x2 = slha.get_block_entry("umix", indices, -3.992666e-01); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RUU1x1 = slha.get_block_entry("umix", indices, 9.168349e-01); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_Rtu3x3 = slha.get_block_entry("tu", indices, -4.447525e+02); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_Rte3x3 = slha.get_block_entry("te", indices, -2.540197e+01); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_Rtd3x3 = slha.get_block_entry("td", indices, -1.106937e+02); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_RRn3x3 = slha.get_block_entry("snumix", indices, 1.000000e+00); 
  indices[0] = 2; 
  indices[1] = 2; 
  mdl_RRn2x2 = slha.get_block_entry("snumix", indices, 1.000000e+00); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RRn1x1 = slha.get_block_entry("snumix", indices, 1.000000e+00); 
  aS = slha.get_block_entry("sminputs", 3, 1.180000e-01); 
  aEWM1 = slha.get_block_entry("sminputs", 1, 1.279340e+02); 
  indices[0] = 6; 
  indices[1] = 6; 
  mdl_RRl6x6 = slha.get_block_entry("selmix", indices, -2.824872e-01); 
  indices[0] = 6; 
  indices[1] = 3; 
  mdl_RRl6x3 = slha.get_block_entry("selmix", indices, 9.592711e-01); 
  indices[0] = 5; 
  indices[1] = 5; 
  mdl_RRl5x5 = slha.get_block_entry("selmix", indices, 1.000000e+00); 
  indices[0] = 4; 
  indices[1] = 4; 
  mdl_RRl4x4 = slha.get_block_entry("selmix", indices, 1.000000e+00); 
  indices[0] = 3; 
  indices[1] = 6; 
  mdl_RRl3x6 = slha.get_block_entry("selmix", indices, 9.592711e-01); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_RRl3x3 = slha.get_block_entry("selmix", indices, 2.824872e-01); 
  indices[0] = 2; 
  indices[1] = 2; 
  mdl_RRl2x2 = slha.get_block_entry("selmix", indices, 1.000000e+00); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RRl1x1 = slha.get_block_entry("selmix", indices, 1.000000e+00); 
  indices[0] = 4; 
  indices[1] = 4; 
  mdl_RNN4x4 = slha.get_block_entry("nmix", indices, -6.843778e-01); 
  indices[0] = 4; 
  indices[1] = 3; 
  mdl_RNN4x3 = slha.get_block_entry("nmix", indices, 6.492260e-01); 
  indices[0] = 4; 
  indices[1] = 2; 
  mdl_RNN4x2 = slha.get_block_entry("nmix", indices, 3.107390e-01); 
  indices[0] = 4; 
  indices[1] = 1; 
  mdl_RNN4x1 = slha.get_block_entry("nmix", indices, -1.165071e-01); 
  indices[0] = 3; 
  indices[1] = 4; 
  mdl_RNN3x4 = slha.get_block_entry("nmix", indices, 7.102270e-01); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_RNN3x3 = slha.get_block_entry("nmix", indices, 6.958775e-01); 
  indices[0] = 3; 
  indices[1] = 2; 
  mdl_RNN3x2 = slha.get_block_entry("nmix", indices, 8.770049e-02); 
  indices[0] = 3; 
  indices[1] = 1; 
  mdl_RNN3x1 = slha.get_block_entry("nmix", indices, -6.033880e-02); 
  indices[0] = 2; 
  indices[1] = 4; 
  mdl_RNN2x4 = slha.get_block_entry("nmix", indices, 1.561507e-01); 
  indices[0] = 2; 
  indices[1] = 3; 
  mdl_RNN2x3 = slha.get_block_entry("nmix", indices, -2.698467e-01); 
  indices[0] = 2; 
  indices[1] = 2; 
  mdl_RNN2x2 = slha.get_block_entry("nmix", indices, 9.449493e-01); 
  indices[0] = 2; 
  indices[1] = 1; 
  mdl_RNN2x1 = slha.get_block_entry("nmix", indices, 9.935054e-02); 
  indices[0] = 1; 
  indices[1] = 4; 
  mdl_RNN1x4 = slha.get_block_entry("nmix", indices, -5.311861e-02); 
  indices[0] = 1; 
  indices[1] = 3; 
  mdl_RNN1x3 = slha.get_block_entry("nmix", indices, 1.464340e-01); 
  indices[0] = 1; 
  indices[1] = 2; 
  mdl_RNN1x2 = slha.get_block_entry("nmix", indices, -5.311036e-02); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RNN1x1 = slha.get_block_entry("nmix", indices, 9.863644e-01); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_RmU23x3 = slha.get_block_entry("msu2", indices, 1.791371e+05); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RmU21x1 = slha.get_block_entry("msu2", indices, 2.803821e+05); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_RmQ23x3 = slha.get_block_entry("msq2", indices, 2.487654e+05); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RmQ21x1 = slha.get_block_entry("msq2", indices, 2.998367e+05); 
  mdl_mHu2 = slha.get_block_entry("msoft", 22, -1.288001e+05); 
  mdl_mHd2 = slha.get_block_entry("msoft", 21, 3.233749e+04); 
  mdl_RMx3 = slha.get_block_entry("msoft", 3, 5.882630e+02); 
  mdl_RMx2 = slha.get_block_entry("msoft", 2, 1.915042e+02); 
  mdl_RMx1 = slha.get_block_entry("msoft", 1, 1.013965e+02); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_RmL23x3 = slha.get_block_entry("msl2", indices, 3.782868e+04); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RmL21x1 = slha.get_block_entry("msl2", indices, 3.815567e+04); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_RmE23x3 = slha.get_block_entry("mse2", indices, 1.796764e+04); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RmE21x1 = slha.get_block_entry("mse2", indices, 1.863063e+04); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_RmD23x3 = slha.get_block_entry("msd2", indices, 2.702620e+05); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RmD21x1 = slha.get_block_entry("msd2", indices, 2.736847e+05); 
  mdl_Msl6 = slha.get_block_entry("mass", 2000015, 2.068678e+02); 
  mdl_Msl4 = slha.get_block_entry("mass", 2000011, 1.441028e+02); 
  mdl_Msu6 = slha.get_block_entry("mass", 2000006, 5.857858e+02); 
  mdl_Msd6 = slha.get_block_entry("mass", 2000005, 5.437267e+02); 
  mdl_Msu4 = slha.get_block_entry("mass", 2000002, 5.492593e+02); 
  mdl_Msd4 = slha.get_block_entry("mass", 2000001, 5.452285e+02); 
  mdl_Mch2 = slha.get_block_entry("mass", 1000037, 3.799393e+02); 
  mdl_Mneu4 = slha.get_block_entry("mass", 1000035, 3.817294e+02); 
  mdl_Mneu3 = slha.get_block_entry("mass", 1000025, -3.637560e+02); 
  mdl_Mch1 = slha.get_block_entry("mass", 1000024, 1.816965e+02); 
  mdl_Mneu2 = slha.get_block_entry("mass", 1000023, 1.810882e+02); 
  mdl_Mneu1 = slha.get_block_entry("mass", 1000022, 9.668807e+01); 
  mdl_Mgo = slha.get_block_entry("mass", 1000021, 6.077137e+02); 
  mdl_Msn3 = slha.get_block_entry("mass", 1000016, 1.847085e+02); 
  mdl_Msl3 = slha.get_block_entry("mass", 1000015, 1.344909e+02); 
  mdl_Msn1 = slha.get_block_entry("mass", 1000012, 1.852583e+02); 
  mdl_Msl1 = slha.get_block_entry("mass", 1000011, 2.029157e+02); 
  mdl_Msu3 = slha.get_block_entry("mass", 1000006, 3.996685e+02); 
  mdl_Msd3 = slha.get_block_entry("mass", 1000005, 5.130652e+02); 
  mdl_Msu1 = slha.get_block_entry("mass", 1000002, 5.611190e+02); 
  mdl_Msd1 = slha.get_block_entry("mass", 1000001, 5.684411e+02); 
  mdl_MH = slha.get_block_entry("mass", 37, 4.078790e+02); 
  mdl_MA0 = slha.get_block_entry("mass", 36, 3.995839e+02); 
  mdl_MH02 = slha.get_block_entry("mass", 35, 3.999601e+02); 
  mdl_MH01 = slha.get_block_entry("mass", 25, 1.108991e+02); 
  mdl_MW = slha.get_block_entry("mass", 24, 7.982901e+01); 
  mdl_MZ = slha.get_block_entry("mass", 23, 9.118760e+01); 
  mdl_Mta = slha.get_block_entry("mass", 15, 1.777000e+00); 
  mdl_MT = slha.get_block_entry("mass", 6, 1.750000e+02); 
  mdl_MB = slha.get_block_entry("mass", 5, 4.889917e+00); 
  mdl_MA2 = slha.get_block_entry("hmix", 4, 1.664391e+05); 
  mdl_tb = slha.get_block_entry("hmix", 2, 9.748624e+00); 
  mdl_RMUH = slha.get_block_entry("hmix", 1, 3.576810e+02); 
  mdl_alp = slha.get_block_entry("fralpha", 1, -1.138252e-01); 
  indices[0] = 6; 
  indices[1] = 6; 
  mdl_RRd6x6 = slha.get_block_entry("dsqmix", indices, 9.387379e-01); 
  indices[0] = 6; 
  indices[1] = 3; 
  mdl_RRd6x3 = slha.get_block_entry("dsqmix", indices, -3.446319e-01); 
  indices[0] = 5; 
  indices[1] = 5; 
  mdl_RRd5x5 = slha.get_block_entry("dsqmix", indices, 1.000000e+00); 
  indices[0] = 4; 
  indices[1] = 4; 
  mdl_RRd4x4 = slha.get_block_entry("dsqmix", indices, 1.000000e+00); 
  indices[0] = 3; 
  indices[1] = 6; 
  mdl_RRd3x6 = slha.get_block_entry("dsqmix", indices, 3.446319e-01); 
  indices[0] = 3; 
  indices[1] = 3; 
  mdl_RRd3x3 = slha.get_block_entry("dsqmix", indices, 9.387379e-01); 
  indices[0] = 2; 
  indices[1] = 2; 
  mdl_RRd2x2 = slha.get_block_entry("dsqmix", indices, 1.000000e+00); 
  indices[0] = 1; 
  indices[1] = 1; 
  mdl_RRd1x1 = slha.get_block_entry("dsqmix", indices, 1.000000e+00); 
  mdl_Msu5 = 1. * mdl_Msu4; 
  mdl_Msu2 = 1. * mdl_Msu1; 
  mdl_RmD22x2 = 1. * mdl_RmD21x1; 
  mdl_Msl2 = 1. * mdl_Msl1; 
  mdl_Msl5 = 1. * mdl_Msl4; 
  mdl_RmL22x2 = 1. * mdl_RmL21x1; 
  mdl_Msd2 = 1. * mdl_Msd1; 
  mdl_RmQ22x2 = 1. * mdl_RmQ21x1; 
  mdl_Msd5 = 1. * mdl_Msd4; 
  mdl_RmU22x2 = 1. * mdl_RmU21x1; 
  mdl_Msn2 = 1. * mdl_Msn1; 
  mdl_RmE22x2 = 1. * mdl_RmE21x1; 
  mdl_conjg__CKM3x3 = 1.; 
  mdl_I93x22 = 1.; 
  mdl_I97x11 = 1.; 
  mdl_I43x22 = 1.; 
  mdl_I51x11 = 1.; 
  mdl_I75x55 = 1.; 
  mdl_I92x22 = 1.; 
  mdl_I96x11 = 1.; 
  mdl_I5x11 = 1.; 
  mdl_I100x44 = 1.; 
  mdl_conjg__Rn3x3 = 1.; 
  mdl_I45x11 = 1.; 
  mdl_I75x44 = 1.; 
  mdl_I25x22 = 1.; 
  mdl_I52x44 = 1.; 
  mdl_conjg__Ru1x1 = 1.; 
  mdl_I32x44 = 1.; 
  mdl_conjg__Ru4x4 = 1.; 
  mdl_I6x44 = 1.; 
  mdl_I53x11 = 1.; 
  mdl_I74x11 = 1.; 
  mdl_conjg__Rd5x5 = 1.; 
  mdl_conjg__Rl1x1 = 1.; 
  mdl_I5x22 = 1.; 
  mdl_I101x55 = 1.; 
  mdl_I102x44 = 1.; 
  mdl_I52x55 = 1.; 
  mdl_I26x44 = 1.; 
  mdl_conjg__Ru2x2 = 1.; 
  mdl_I26x55 = 1.; 
  mdl_I96x22 = 1.; 
  mdl_I13x44 = 1.; 
  mdl_I51x22 = 1.; 
  mdl_I89x11 = 1.; 
  mdl_I98x11 = 1.; 
  mdl_I100x55 = 1.; 
  mdl_Ru5x5 = 1.; 
  mdl_Rl1x1 = 1.; 
  mdl_conjg__Rl4x4 = 1.; 
  mdl_Ru2x2 = 1.; 
  mdl_Ru4x4 = 1.; 
  mdl_conjg__Rd4x4 = 1.; 
  mdl_Rd1x1 = 1.; 
  mdl_I7x22 = 1.; 
  mdl_I25x11 = 1.; 
  mdl_I6x55 = 1.; 
  mdl_CKM3x3 = 1.; 
  mdl_Rn3x3 = 1.; 
  mdl_conjg__Rd1x1 = 1.; 
  mdl_I85x22 = 1.; 
  mdl_conjg__Rn2x2 = 1.; 
  mdl_I39x11 = 1.; 
  mdl_I101x44 = 1.; 
  mdl_I102x55 = 1.; 
  mdl_atan__tb = atan(mdl_tb); 
  mdl_beta = mdl_atan__tb; 
  mdl_cw = mdl_MW/mdl_MZ; 
  mdl_mD21x1 = mdl_RmD21x1; 
  mdl_mD22x2 = mdl_RmD22x2; 
  mdl_mD23x3 = mdl_RmD23x3; 
  mdl_mE21x1 = mdl_RmE21x1; 
  mdl_mE22x2 = mdl_RmE22x2; 
  mdl_mE23x3 = mdl_RmE23x3; 
  mdl_mL21x1 = mdl_RmL21x1; 
  mdl_mL22x2 = mdl_RmL22x2; 
  mdl_mL23x3 = mdl_RmL23x3; 
  mdl_mQ21x1 = mdl_RmQ21x1; 
  mdl_mQ22x2 = mdl_RmQ22x2; 
  mdl_mQ23x3 = mdl_RmQ23x3; 
  mdl_mU21x1 = mdl_RmU21x1; 
  mdl_mU22x2 = mdl_RmU22x2; 
  mdl_mU23x3 = mdl_RmU23x3; 
  mdl_MUH = mdl_RMUH; 
  mdl_Mx1 = mdl_RMx1; 
  mdl_Mx2 = mdl_RMx2; 
  mdl_Mx3 = mdl_RMx3; 
  mdl_NN1x1 = mdl_RNN1x1; 
  mdl_NN1x2 = mdl_RNN1x2; 
  mdl_NN1x3 = mdl_RNN1x3; 
  mdl_NN1x4 = mdl_RNN1x4; 
  mdl_NN2x1 = mdl_RNN2x1; 
  mdl_NN2x2 = mdl_RNN2x2; 
  mdl_NN2x3 = mdl_RNN2x3; 
  mdl_NN2x4 = mdl_RNN2x4; 
  mdl_NN3x1 = mdl_RNN3x1; 
  mdl_NN3x2 = mdl_RNN3x2; 
  mdl_NN3x3 = mdl_RNN3x3; 
  mdl_NN3x4 = mdl_RNN3x4; 
  mdl_NN4x1 = mdl_RNN4x1; 
  mdl_NN4x2 = mdl_RNN4x2; 
  mdl_NN4x3 = mdl_RNN4x3; 
  mdl_NN4x4 = mdl_RNN4x4; 
  mdl_Rd3x3 = mdl_RRd3x3; 
  mdl_Rd3x6 = mdl_RRd3x6; 
  mdl_Rd6x3 = mdl_RRd6x3; 
  mdl_Rd6x6 = mdl_RRd6x6; 
  mdl_Rl3x3 = mdl_RRl3x3; 
  mdl_Rl3x6 = mdl_RRl3x6; 
  mdl_Rl6x3 = mdl_RRl6x3; 
  mdl_Rl6x6 = mdl_RRl6x6; 
  mdl_Ru3x3 = mdl_RRu3x3; 
  mdl_Ru3x6 = mdl_RRu3x6; 
  mdl_Ru6x3 = mdl_RRu6x3; 
  mdl_Ru6x6 = mdl_RRu6x6; 
  mdl_UU1x1 = mdl_RUU1x1; 
  mdl_UU1x2 = mdl_RUU1x2; 
  mdl_UU2x1 = mdl_RUU2x1; 
  mdl_UU2x2 = mdl_RUU2x2; 
  mdl_VV1x1 = mdl_RVV1x1; 
  mdl_VV1x2 = mdl_RVV1x2; 
  mdl_VV2x1 = mdl_RVV2x1; 
  mdl_VV2x2 = mdl_RVV2x2; 
  mdl_td3x3 = mdl_Rtd3x3; 
  mdl_te3x3 = mdl_Rte3x3; 
  mdl_tu3x3 = mdl_Rtu3x3; 
  mdl_yd3x3 = mdl_Ryd3x3; 
  mdl_ye3x3 = mdl_Rye3x3; 
  mdl_yu3x3 = mdl_Ryu3x3; 
  mdl_MZ__exp__2 = ((mdl_MZ) * (mdl_MZ)); 
  mdl_bb = ((-mdl_mHd2 + mdl_mHu2 - mdl_MZ__exp__2 * cos(2. * mdl_beta)) *
      tan(2. * mdl_beta))/2.;
  mdl_cw__exp__2 = ((mdl_cw) * (mdl_cw)); 
  mdl_sw = sqrt(1. - mdl_cw__exp__2); 
  mdl_cos__beta = cos(mdl_beta); 
  mdl_sin__beta = sin(mdl_beta); 
  mdl_conjg__yu3x3 = conj(mdl_yu3x3); 
  mdl_I1x33 = mdl_conjg__CKM3x3 * mdl_conjg__yu3x3; 
  mdl_conjg__yd3x3 = conj(mdl_yd3x3); 
  mdl_I10x33 = mdl_Rd3x3 * mdl_conjg__yd3x3; 
  mdl_I10x36 = mdl_Rd6x3 * mdl_conjg__yd3x3; 
  mdl_conjg__Rd3x6 = conj(mdl_Rd3x6); 
  mdl_I100x33 = mdl_Rd3x6 * mdl_conjg__Rd3x6; 
  mdl_I100x36 = mdl_Rd6x6 * mdl_conjg__Rd3x6; 
  mdl_conjg__Rd6x6 = conj(mdl_Rd6x6); 
  mdl_I100x63 = mdl_Rd3x6 * mdl_conjg__Rd6x6; 
  mdl_I100x66 = mdl_Rd6x6 * mdl_conjg__Rd6x6; 
  mdl_conjg__Rl3x6 = conj(mdl_Rl3x6); 
  mdl_I101x33 = mdl_Rl3x6 * mdl_conjg__Rl3x6; 
  mdl_I101x36 = mdl_Rl6x6 * mdl_conjg__Rl3x6; 
  mdl_conjg__Rl6x6 = conj(mdl_Rl6x6); 
  mdl_I101x63 = mdl_Rl3x6 * mdl_conjg__Rl6x6; 
  mdl_I101x66 = mdl_Rl6x6 * mdl_conjg__Rl6x6; 
  mdl_conjg__Ru3x6 = conj(mdl_Ru3x6); 
  mdl_I102x33 = mdl_Ru3x6 * mdl_conjg__Ru3x6; 
  mdl_I102x36 = mdl_Ru6x6 * mdl_conjg__Ru3x6; 
  mdl_conjg__Ru6x6 = conj(mdl_Ru6x6); 
  mdl_I102x63 = mdl_Ru3x6 * mdl_conjg__Ru6x6; 
  mdl_I102x66 = mdl_Ru6x6 * mdl_conjg__Ru6x6; 
  mdl_I11x33 = mdl_Rd3x6 * mdl_yd3x3; 
  mdl_I11x36 = mdl_Rd6x6 * mdl_yd3x3; 
  mdl_conjg__Rd3x3 = conj(mdl_Rd3x3); 
  mdl_I12x33 = mdl_Rd3x3 * mdl_conjg__Rd3x3; 
  mdl_I12x36 = mdl_Rd6x3 * mdl_conjg__Rd3x3; 
  mdl_conjg__Rd6x3 = conj(mdl_Rd6x3); 
  mdl_I12x63 = mdl_Rd3x3 * mdl_conjg__Rd6x3; 
  mdl_I12x66 = mdl_Rd6x3 * mdl_conjg__Rd6x3; 
  mdl_I13x33 = mdl_Rd3x6 * mdl_conjg__Rd3x6; 
  mdl_I13x36 = mdl_Rd6x6 * mdl_conjg__Rd3x6; 
  mdl_I13x63 = mdl_Rd3x6 * mdl_conjg__Rd6x6; 
  mdl_I13x66 = mdl_Rd6x6 * mdl_conjg__Rd6x6; 
  mdl_conjg__td3x3 = conj(mdl_td3x3); 
  mdl_I14x33 = mdl_Rd3x3 * mdl_conjg__Rd3x6 * mdl_conjg__td3x3; 
  mdl_I14x36 = mdl_Rd6x3 * mdl_conjg__Rd3x6 * mdl_conjg__td3x3; 
  mdl_I14x63 = mdl_Rd3x3 * mdl_conjg__Rd6x6 * mdl_conjg__td3x3; 
  mdl_I14x66 = mdl_Rd6x3 * mdl_conjg__Rd6x6 * mdl_conjg__td3x3; 
  mdl_I15x33 = mdl_Rd3x3 * mdl_conjg__Rd3x6 * mdl_conjg__yd3x3; 
  mdl_I15x36 = mdl_Rd6x3 * mdl_conjg__Rd3x6 * mdl_conjg__yd3x3; 
  mdl_I15x63 = mdl_Rd3x3 * mdl_conjg__Rd6x6 * mdl_conjg__yd3x3; 
  mdl_I15x66 = mdl_Rd6x3 * mdl_conjg__Rd6x6 * mdl_conjg__yd3x3; 
  mdl_I16x33 = mdl_Rd3x6 * mdl_td3x3 * mdl_conjg__Rd3x3; 
  mdl_I16x36 = mdl_Rd6x6 * mdl_td3x3 * mdl_conjg__Rd3x3; 
  mdl_I16x63 = mdl_Rd3x6 * mdl_td3x3 * mdl_conjg__Rd6x3; 
  mdl_I16x66 = mdl_Rd6x6 * mdl_td3x3 * mdl_conjg__Rd6x3; 
  mdl_I17x33 = mdl_Rd3x3 * mdl_yd3x3 * mdl_conjg__Rd3x3 * mdl_conjg__yd3x3; 
  mdl_I17x36 = mdl_Rd6x3 * mdl_yd3x3 * mdl_conjg__Rd3x3 * mdl_conjg__yd3x3; 
  mdl_I17x63 = mdl_Rd3x3 * mdl_yd3x3 * mdl_conjg__Rd6x3 * mdl_conjg__yd3x3; 
  mdl_I17x66 = mdl_Rd6x3 * mdl_yd3x3 * mdl_conjg__Rd6x3 * mdl_conjg__yd3x3; 
  mdl_I18x33 = mdl_Rd3x6 * mdl_yd3x3 * mdl_conjg__Rd3x3; 
  mdl_I18x36 = mdl_Rd6x6 * mdl_yd3x3 * mdl_conjg__Rd3x3; 
  mdl_I18x63 = mdl_Rd3x6 * mdl_yd3x3 * mdl_conjg__Rd6x3; 
  mdl_I18x66 = mdl_Rd6x6 * mdl_yd3x3 * mdl_conjg__Rd6x3; 
  mdl_I19x33 = mdl_Rd3x6 * mdl_yd3x3 * mdl_conjg__Rd3x6 * mdl_conjg__yd3x3; 
  mdl_I19x36 = mdl_Rd6x6 * mdl_yd3x3 * mdl_conjg__Rd3x6 * mdl_conjg__yd3x3; 
  mdl_I19x63 = mdl_Rd3x6 * mdl_yd3x3 * mdl_conjg__Rd6x6 * mdl_conjg__yd3x3; 
  mdl_I19x66 = mdl_Rd6x6 * mdl_yd3x3 * mdl_conjg__Rd6x6 * mdl_conjg__yd3x3; 
  mdl_I2x33 = mdl_yd3x3 * mdl_conjg__CKM3x3; 
  mdl_I20x33 = mdl_CKM3x3 * mdl_conjg__yd3x3; 
  mdl_I21x33 = mdl_CKM3x3 * mdl_yu3x3; 
  mdl_conjg__ye3x3 = conj(mdl_ye3x3); 
  mdl_I22x33 = mdl_conjg__ye3x3; 
  mdl_I23x33 = mdl_conjg__Rl3x6 * mdl_conjg__ye3x3; 
  mdl_I23x36 = mdl_conjg__Rl6x6 * mdl_conjg__ye3x3; 
  mdl_conjg__Rl3x3 = conj(mdl_Rl3x3); 
  mdl_I24x33 = mdl_ye3x3 * mdl_conjg__Rl3x3; 
  mdl_conjg__Rl6x3 = conj(mdl_Rl6x3); 
  mdl_I24x36 = mdl_ye3x3 * mdl_conjg__Rl6x3; 
  mdl_I25x33 = mdl_Rl3x3 * mdl_conjg__Rl3x3; 
  mdl_I25x36 = mdl_Rl6x3 * mdl_conjg__Rl3x3; 
  mdl_I25x63 = mdl_Rl3x3 * mdl_conjg__Rl6x3; 
  mdl_I25x66 = mdl_Rl6x3 * mdl_conjg__Rl6x3; 
  mdl_I26x33 = mdl_Rl3x6 * mdl_conjg__Rl3x6; 
  mdl_I26x36 = mdl_Rl6x6 * mdl_conjg__Rl3x6; 
  mdl_I26x63 = mdl_Rl3x6 * mdl_conjg__Rl6x6; 
  mdl_I26x66 = mdl_Rl6x6 * mdl_conjg__Rl6x6; 
  mdl_I27x33 = mdl_Rl3x3 * mdl_conjg__ye3x3; 
  mdl_I27x36 = mdl_Rl6x3 * mdl_conjg__ye3x3; 
  mdl_I28x33 = mdl_Rl3x6 * mdl_ye3x3; 
  mdl_I28x36 = mdl_Rl6x6 * mdl_ye3x3; 
  mdl_I29x33 = mdl_Rl3x3; 
  mdl_I29x36 = mdl_Rl6x3; 
  mdl_I3x33 = mdl_conjg__Rd3x6 * mdl_conjg__yd3x3; 
  mdl_I3x36 = mdl_conjg__Rd6x6 * mdl_conjg__yd3x3; 
  mdl_I30x33 = mdl_Rl3x6 * mdl_ye3x3; 
  mdl_I30x36 = mdl_Rl6x6 * mdl_ye3x3; 
  mdl_I31x33 = mdl_Rl3x3 * mdl_conjg__Rl3x3; 
  mdl_I31x36 = mdl_Rl6x3 * mdl_conjg__Rl3x3; 
  mdl_I31x63 = mdl_Rl3x3 * mdl_conjg__Rl6x3; 
  mdl_I31x66 = mdl_Rl6x3 * mdl_conjg__Rl6x3; 
  mdl_I32x33 = mdl_Rl3x6 * mdl_conjg__Rl3x6; 
  mdl_I32x36 = mdl_Rl6x6 * mdl_conjg__Rl3x6; 
  mdl_I32x63 = mdl_Rl3x6 * mdl_conjg__Rl6x6; 
  mdl_I32x66 = mdl_Rl6x6 * mdl_conjg__Rl6x6; 
  mdl_conjg__te3x3 = conj(mdl_te3x3); 
  mdl_I33x33 = mdl_Rl3x3 * mdl_conjg__Rl3x6 * mdl_conjg__te3x3; 
  mdl_I33x36 = mdl_Rl6x3 * mdl_conjg__Rl3x6 * mdl_conjg__te3x3; 
  mdl_I33x63 = mdl_Rl3x3 * mdl_conjg__Rl6x6 * mdl_conjg__te3x3; 
  mdl_I33x66 = mdl_Rl6x3 * mdl_conjg__Rl6x6 * mdl_conjg__te3x3; 
  mdl_I34x33 = mdl_Rl3x3 * mdl_conjg__Rl3x6 * mdl_conjg__ye3x3; 
  mdl_I34x36 = mdl_Rl6x3 * mdl_conjg__Rl3x6 * mdl_conjg__ye3x3; 
  mdl_I34x63 = mdl_Rl3x3 * mdl_conjg__Rl6x6 * mdl_conjg__ye3x3; 
  mdl_I34x66 = mdl_Rl6x3 * mdl_conjg__Rl6x6 * mdl_conjg__ye3x3; 
  mdl_I35x33 = mdl_Rl3x6 * mdl_te3x3 * mdl_conjg__Rl3x3; 
  mdl_I35x36 = mdl_Rl6x6 * mdl_te3x3 * mdl_conjg__Rl3x3; 
  mdl_I35x63 = mdl_Rl3x6 * mdl_te3x3 * mdl_conjg__Rl6x3; 
  mdl_I35x66 = mdl_Rl6x6 * mdl_te3x3 * mdl_conjg__Rl6x3; 
  mdl_I36x33 = mdl_Rl3x3 * mdl_ye3x3 * mdl_conjg__Rl3x3 * mdl_conjg__ye3x3; 
  mdl_I36x36 = mdl_Rl6x3 * mdl_ye3x3 * mdl_conjg__Rl3x3 * mdl_conjg__ye3x3; 
  mdl_I36x63 = mdl_Rl3x3 * mdl_ye3x3 * mdl_conjg__Rl6x3 * mdl_conjg__ye3x3; 
  mdl_I36x66 = mdl_Rl6x3 * mdl_ye3x3 * mdl_conjg__Rl6x3 * mdl_conjg__ye3x3; 
  mdl_I37x33 = mdl_Rl3x6 * mdl_ye3x3 * mdl_conjg__Rl3x3; 
  mdl_I37x36 = mdl_Rl6x6 * mdl_ye3x3 * mdl_conjg__Rl3x3; 
  mdl_I37x63 = mdl_Rl3x6 * mdl_ye3x3 * mdl_conjg__Rl6x3; 
  mdl_I37x66 = mdl_Rl6x6 * mdl_ye3x3 * mdl_conjg__Rl6x3; 
  mdl_I38x33 = mdl_Rl3x6 * mdl_ye3x3 * mdl_conjg__Rl3x6 * mdl_conjg__ye3x3; 
  mdl_I38x36 = mdl_Rl6x6 * mdl_ye3x3 * mdl_conjg__Rl3x6 * mdl_conjg__ye3x3; 
  mdl_I38x63 = mdl_Rl3x6 * mdl_ye3x3 * mdl_conjg__Rl6x6 * mdl_conjg__ye3x3; 
  mdl_I38x66 = mdl_Rl6x6 * mdl_ye3x3 * mdl_conjg__Rl6x6 * mdl_conjg__ye3x3; 
  mdl_I39x33 = mdl_Rl3x3 * mdl_conjg__Rn3x3; 
  mdl_I39x36 = mdl_Rl6x3 * mdl_conjg__Rn3x3; 
  mdl_I4x33 = mdl_yd3x3 * mdl_conjg__Rd3x3; 
  mdl_I4x36 = mdl_yd3x3 * mdl_conjg__Rd6x3; 
  mdl_I40x33 = mdl_Rl3x6 * mdl_te3x3 * mdl_conjg__Rn3x3; 
  mdl_I40x36 = mdl_Rl6x6 * mdl_te3x3 * mdl_conjg__Rn3x3; 
  mdl_I41x33 = mdl_Rl3x3 * mdl_ye3x3 * mdl_conjg__Rn3x3 * mdl_conjg__ye3x3; 
  mdl_I41x36 = mdl_Rl6x3 * mdl_ye3x3 * mdl_conjg__Rn3x3 * mdl_conjg__ye3x3; 
  mdl_I42x33 = mdl_Rl3x6 * mdl_ye3x3 * mdl_conjg__Rn3x3; 
  mdl_I42x36 = mdl_Rl6x6 * mdl_ye3x3 * mdl_conjg__Rn3x3; 
  mdl_I44x33 = mdl_Rn3x3 * mdl_conjg__ye3x3; 
  mdl_I45x33 = mdl_Rn3x3 * mdl_conjg__Rl3x3; 
  mdl_I45x36 = mdl_Rn3x3 * mdl_conjg__Rl6x3; 
  mdl_I46x33 = mdl_Rn3x3 * mdl_conjg__Rl3x6 * mdl_conjg__ye3x3; 
  mdl_I46x36 = mdl_Rn3x3 * mdl_conjg__Rl6x6 * mdl_conjg__ye3x3; 
  mdl_I47x33 = mdl_Rn3x3 * mdl_conjg__Rl3x6 * mdl_conjg__te3x3; 
  mdl_I47x36 = mdl_Rn3x3 * mdl_conjg__Rl6x6 * mdl_conjg__te3x3; 
  mdl_I48x33 = mdl_Rn3x3 * mdl_ye3x3 * mdl_conjg__Rl3x3 * mdl_conjg__ye3x3; 
  mdl_I48x36 = mdl_Rn3x3 * mdl_ye3x3 * mdl_conjg__Rl6x3 * mdl_conjg__ye3x3; 
  mdl_I49x33 = mdl_conjg__Ru3x6 * mdl_conjg__yu3x3; 
  mdl_I49x36 = mdl_conjg__Ru6x6 * mdl_conjg__yu3x3; 
  mdl_I5x33 = mdl_Rd3x3 * mdl_conjg__Rd3x3; 
  mdl_I5x36 = mdl_Rd6x3 * mdl_conjg__Rd3x3; 
  mdl_I5x63 = mdl_Rd3x3 * mdl_conjg__Rd6x3; 
  mdl_I5x66 = mdl_Rd6x3 * mdl_conjg__Rd6x3; 
  mdl_conjg__Ru3x3 = conj(mdl_Ru3x3); 
  mdl_I50x33 = mdl_yu3x3 * mdl_conjg__Ru3x3; 
  mdl_conjg__Ru6x3 = conj(mdl_Ru6x3); 
  mdl_I50x36 = mdl_yu3x3 * mdl_conjg__Ru6x3; 
  mdl_I51x33 = mdl_Ru3x3 * mdl_conjg__Ru3x3; 
  mdl_I51x36 = mdl_Ru6x3 * mdl_conjg__Ru3x3; 
  mdl_I51x63 = mdl_Ru3x3 * mdl_conjg__Ru6x3; 
  mdl_I51x66 = mdl_Ru6x3 * mdl_conjg__Ru6x3; 
  mdl_I52x33 = mdl_Ru3x6 * mdl_conjg__Ru3x6; 
  mdl_I52x36 = mdl_Ru6x6 * mdl_conjg__Ru3x6; 
  mdl_I52x63 = mdl_Ru3x6 * mdl_conjg__Ru6x6; 
  mdl_I52x66 = mdl_Ru6x6 * mdl_conjg__Ru6x6; 
  mdl_I53x33 = mdl_Rd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3; 
  mdl_I53x36 = mdl_Rd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3; 
  mdl_I53x63 = mdl_Rd6x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3; 
  mdl_I53x66 = mdl_Rd6x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3; 
  mdl_conjg__tu3x3 = conj(mdl_tu3x3); 
  mdl_I54x33 = mdl_Rd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x6 *
      mdl_conjg__tu3x3;
  mdl_I54x36 = mdl_Rd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x6 *
      mdl_conjg__tu3x3;
  mdl_I54x63 = mdl_Rd6x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x6 *
      mdl_conjg__tu3x3;
  mdl_I54x66 = mdl_Rd6x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x6 *
      mdl_conjg__tu3x3;
  mdl_I55x33 = mdl_Rd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x6 *
      mdl_conjg__yu3x3;
  mdl_I55x36 = mdl_Rd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x6 *
      mdl_conjg__yu3x3;
  mdl_I55x63 = mdl_Rd6x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x6 *
      mdl_conjg__yu3x3;
  mdl_I55x66 = mdl_Rd6x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x6 *
      mdl_conjg__yu3x3;
  mdl_I56x33 = mdl_Rd3x6 * mdl_td3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3; 
  mdl_I56x36 = mdl_Rd3x6 * mdl_td3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3; 
  mdl_I56x63 = mdl_Rd6x6 * mdl_td3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3; 
  mdl_I56x66 = mdl_Rd6x6 * mdl_td3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3; 
  mdl_I57x33 = mdl_Rd3x6 * mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3; 
  mdl_I57x36 = mdl_Rd3x6 * mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3; 
  mdl_I57x63 = mdl_Rd6x6 * mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3; 
  mdl_I57x66 = mdl_Rd6x6 * mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3; 
  mdl_I58x33 = mdl_Rd3x3 * mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3 *
      mdl_conjg__yd3x3;
  mdl_I58x36 = mdl_Rd3x3 * mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3 *
      mdl_conjg__yd3x3;
  mdl_I58x63 = mdl_Rd6x3 * mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3 *
      mdl_conjg__yd3x3;
  mdl_I58x66 = mdl_Rd6x3 * mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3 *
      mdl_conjg__yd3x3;
  mdl_I59x33 = mdl_Rd3x6 * mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x6 *
      mdl_conjg__yu3x3;
  mdl_I59x36 = mdl_Rd3x6 * mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x6 *
      mdl_conjg__yu3x3;
  mdl_I59x63 = mdl_Rd6x6 * mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x6 *
      mdl_conjg__yu3x3;
  mdl_I59x66 = mdl_Rd6x6 * mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x6 *
      mdl_conjg__yu3x3;
  mdl_I6x33 = mdl_Rd3x6 * mdl_conjg__Rd3x6; 
  mdl_I6x36 = mdl_Rd6x6 * mdl_conjg__Rd3x6; 
  mdl_I6x63 = mdl_Rd3x6 * mdl_conjg__Rd6x6; 
  mdl_I6x66 = mdl_Rd6x6 * mdl_conjg__Rd6x6; 
  mdl_I60x33 = mdl_Rd3x3 * mdl_yu3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3 *
      mdl_conjg__yu3x3;
  mdl_I60x36 = mdl_Rd3x3 * mdl_yu3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3 *
      mdl_conjg__yu3x3;
  mdl_I60x63 = mdl_Rd6x3 * mdl_yu3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3 *
      mdl_conjg__yu3x3;
  mdl_I60x66 = mdl_Rd6x3 * mdl_yu3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3 *
      mdl_conjg__yu3x3;
  mdl_I61x33 = mdl_Ru3x3 * mdl_conjg__yu3x3; 
  mdl_I61x36 = mdl_Ru6x3 * mdl_conjg__yu3x3; 
  mdl_I62x33 = mdl_Ru3x6 * mdl_yu3x3; 
  mdl_I62x36 = mdl_Ru6x6 * mdl_yu3x3; 
  mdl_I63x33 = mdl_CKM3x3 * mdl_Ru3x3; 
  mdl_I63x36 = mdl_CKM3x3 * mdl_Ru6x3; 
  mdl_I64x33 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_conjg__yd3x3; 
  mdl_I64x36 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_conjg__yd3x3; 
  mdl_I65x33 = mdl_CKM3x3 * mdl_Ru3x6 * mdl_yu3x3; 
  mdl_I65x36 = mdl_CKM3x3 * mdl_Ru6x6 * mdl_yu3x3; 
  mdl_I66x33 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_conjg__Rd3x3; 
  mdl_I66x36 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_conjg__Rd3x3; 
  mdl_I66x63 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_conjg__Rd6x3; 
  mdl_I66x66 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_conjg__Rd6x3; 
  mdl_I67x33 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_conjg__Rd3x6 * mdl_conjg__yd3x3; 
  mdl_I67x36 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_conjg__Rd3x6 * mdl_conjg__yd3x3; 
  mdl_I67x63 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_conjg__Rd6x6 * mdl_conjg__yd3x3; 
  mdl_I67x66 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_conjg__Rd6x6 * mdl_conjg__yd3x3; 
  mdl_I68x33 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_conjg__Rd3x6 * mdl_conjg__td3x3; 
  mdl_I68x36 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_conjg__Rd3x6 * mdl_conjg__td3x3; 
  mdl_I68x63 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_conjg__Rd6x6 * mdl_conjg__td3x3; 
  mdl_I68x66 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_conjg__Rd6x6 * mdl_conjg__td3x3; 
  mdl_I69x33 = mdl_CKM3x3 * mdl_Ru3x6 * mdl_tu3x3 * mdl_conjg__Rd3x3; 
  mdl_I69x36 = mdl_CKM3x3 * mdl_Ru6x6 * mdl_tu3x3 * mdl_conjg__Rd3x3; 
  mdl_I69x63 = mdl_CKM3x3 * mdl_Ru3x6 * mdl_tu3x3 * mdl_conjg__Rd6x3; 
  mdl_I69x66 = mdl_CKM3x3 * mdl_Ru6x6 * mdl_tu3x3 * mdl_conjg__Rd6x3; 
  mdl_I7x33 = mdl_Rd3x3 * mdl_conjg__CKM3x3; 
  mdl_I7x36 = mdl_Rd6x3 * mdl_conjg__CKM3x3; 
  mdl_I70x33 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_yd3x3 * mdl_conjg__Rd3x3 *
      mdl_conjg__yd3x3;
  mdl_I70x36 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_yd3x3 * mdl_conjg__Rd3x3 *
      mdl_conjg__yd3x3;
  mdl_I70x63 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_yd3x3 * mdl_conjg__Rd6x3 *
      mdl_conjg__yd3x3;
  mdl_I70x66 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_yd3x3 * mdl_conjg__Rd6x3 *
      mdl_conjg__yd3x3;
  mdl_I71x33 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_yu3x3 * mdl_conjg__Rd3x3 *
      mdl_conjg__yu3x3;
  mdl_I71x36 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_yu3x3 * mdl_conjg__Rd3x3 *
      mdl_conjg__yu3x3;
  mdl_I71x63 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_yu3x3 * mdl_conjg__Rd6x3 *
      mdl_conjg__yu3x3;
  mdl_I71x66 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_yu3x3 * mdl_conjg__Rd6x3 *
      mdl_conjg__yu3x3;
  mdl_I72x33 = mdl_CKM3x3 * mdl_Ru3x6 * mdl_yu3x3 * mdl_conjg__Rd3x3; 
  mdl_I72x36 = mdl_CKM3x3 * mdl_Ru6x6 * mdl_yu3x3 * mdl_conjg__Rd3x3; 
  mdl_I72x63 = mdl_CKM3x3 * mdl_Ru3x6 * mdl_yu3x3 * mdl_conjg__Rd6x3; 
  mdl_I72x66 = mdl_CKM3x3 * mdl_Ru6x6 * mdl_yu3x3 * mdl_conjg__Rd6x3; 
  mdl_I73x33 = mdl_CKM3x3 * mdl_Ru3x6 * mdl_yu3x3 * mdl_conjg__Rd3x6 *
      mdl_conjg__yd3x3;
  mdl_I73x36 = mdl_CKM3x3 * mdl_Ru6x6 * mdl_yu3x3 * mdl_conjg__Rd3x6 *
      mdl_conjg__yd3x3;
  mdl_I73x63 = mdl_CKM3x3 * mdl_Ru3x6 * mdl_yu3x3 * mdl_conjg__Rd6x6 *
      mdl_conjg__yd3x3;
  mdl_I73x66 = mdl_CKM3x3 * mdl_Ru6x6 * mdl_yu3x3 * mdl_conjg__Rd6x6 *
      mdl_conjg__yd3x3;
  mdl_I74x33 = mdl_Ru3x3 * mdl_conjg__Ru3x3; 
  mdl_I74x36 = mdl_Ru6x3 * mdl_conjg__Ru3x3; 
  mdl_I74x63 = mdl_Ru3x3 * mdl_conjg__Ru6x3; 
  mdl_I74x66 = mdl_Ru6x3 * mdl_conjg__Ru6x3; 
  mdl_I75x33 = mdl_Ru3x6 * mdl_conjg__Ru3x6; 
  mdl_I75x36 = mdl_Ru6x6 * mdl_conjg__Ru3x6; 
  mdl_I75x63 = mdl_Ru3x6 * mdl_conjg__Ru6x6; 
  mdl_I75x66 = mdl_Ru6x6 * mdl_conjg__Ru6x6; 
  mdl_I76x33 = mdl_Ru3x3 * mdl_conjg__Ru3x6 * mdl_conjg__yu3x3; 
  mdl_I76x36 = mdl_Ru6x3 * mdl_conjg__Ru3x6 * mdl_conjg__yu3x3; 
  mdl_I76x63 = mdl_Ru3x3 * mdl_conjg__Ru6x6 * mdl_conjg__yu3x3; 
  mdl_I76x66 = mdl_Ru6x3 * mdl_conjg__Ru6x6 * mdl_conjg__yu3x3; 
  mdl_I77x33 = mdl_Ru3x3 * mdl_conjg__Ru3x6 * mdl_conjg__tu3x3; 
  mdl_I77x36 = mdl_Ru6x3 * mdl_conjg__Ru3x6 * mdl_conjg__tu3x3; 
  mdl_I77x63 = mdl_Ru3x3 * mdl_conjg__Ru6x6 * mdl_conjg__tu3x3; 
  mdl_I77x66 = mdl_Ru6x3 * mdl_conjg__Ru6x6 * mdl_conjg__tu3x3; 
  mdl_I78x33 = mdl_Ru3x6 * mdl_tu3x3 * mdl_conjg__Ru3x3; 
  mdl_I78x36 = mdl_Ru6x6 * mdl_tu3x3 * mdl_conjg__Ru3x3; 
  mdl_I78x63 = mdl_Ru3x6 * mdl_tu3x3 * mdl_conjg__Ru6x3; 
  mdl_I78x66 = mdl_Ru6x6 * mdl_tu3x3 * mdl_conjg__Ru6x3; 
  mdl_I79x33 = mdl_Ru3x6 * mdl_yu3x3 * mdl_conjg__Ru3x3; 
  mdl_I79x36 = mdl_Ru6x6 * mdl_yu3x3 * mdl_conjg__Ru3x3; 
  mdl_I79x63 = mdl_Ru3x6 * mdl_yu3x3 * mdl_conjg__Ru6x3; 
  mdl_I79x66 = mdl_Ru6x6 * mdl_yu3x3 * mdl_conjg__Ru6x3; 
  mdl_I8x33 = mdl_Rd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__yu3x3; 
  mdl_I8x36 = mdl_Rd6x3 * mdl_conjg__CKM3x3 * mdl_conjg__yu3x3; 
  mdl_I80x33 = mdl_Ru3x3 * mdl_yu3x3 * mdl_conjg__Ru3x3 * mdl_conjg__yu3x3; 
  mdl_I80x36 = mdl_Ru6x3 * mdl_yu3x3 * mdl_conjg__Ru3x3 * mdl_conjg__yu3x3; 
  mdl_I80x63 = mdl_Ru3x3 * mdl_yu3x3 * mdl_conjg__Ru6x3 * mdl_conjg__yu3x3; 
  mdl_I80x66 = mdl_Ru6x3 * mdl_yu3x3 * mdl_conjg__Ru6x3 * mdl_conjg__yu3x3; 
  mdl_I81x33 = mdl_Ru3x6 * mdl_yu3x3 * mdl_conjg__Ru3x6 * mdl_conjg__yu3x3; 
  mdl_I81x36 = mdl_Ru6x6 * mdl_yu3x3 * mdl_conjg__Ru3x6 * mdl_conjg__yu3x3; 
  mdl_I81x63 = mdl_Ru3x6 * mdl_yu3x3 * mdl_conjg__Ru6x6 * mdl_conjg__yu3x3; 
  mdl_I81x66 = mdl_Ru6x6 * mdl_yu3x3 * mdl_conjg__Ru6x6 * mdl_conjg__yu3x3; 
  mdl_I82x33 = mdl_CKM3x3 * mdl_conjg__Rd3x3; 
  mdl_I82x36 = mdl_CKM3x3 * mdl_conjg__Rd6x3; 
  mdl_I83x33 = mdl_CKM3x3 * mdl_conjg__Rd3x6 * mdl_conjg__yd3x3; 
  mdl_I83x36 = mdl_CKM3x3 * mdl_conjg__Rd6x6 * mdl_conjg__yd3x3; 
  mdl_I84x33 = mdl_CKM3x3 * mdl_yu3x3 * mdl_conjg__Rd3x3; 
  mdl_I84x36 = mdl_CKM3x3 * mdl_yu3x3 * mdl_conjg__Rd6x3; 
  mdl_I85x33 = mdl_conjg__Rl3x3; 
  mdl_I85x36 = mdl_conjg__Rl6x3; 
  mdl_I86x33 = mdl_conjg__Rl3x6 * mdl_conjg__ye3x3; 
  mdl_I86x36 = mdl_conjg__Rl6x6 * mdl_conjg__ye3x3; 
  mdl_I88x33 = mdl_ye3x3 * mdl_conjg__Rn3x3; 
  mdl_I89x33 = mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3; 
  mdl_I89x36 = mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3; 
  mdl_I9x33 = mdl_Rd3x6 * mdl_yd3x3 * mdl_conjg__CKM3x3; 
  mdl_I9x36 = mdl_Rd6x6 * mdl_yd3x3 * mdl_conjg__CKM3x3; 
  mdl_I90x33 = mdl_conjg__CKM3x3 * mdl_conjg__Ru3x6 * mdl_conjg__yu3x3; 
  mdl_I90x36 = mdl_conjg__CKM3x3 * mdl_conjg__Ru6x6 * mdl_conjg__yu3x3; 
  mdl_I91x33 = mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3; 
  mdl_I91x36 = mdl_yd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3; 
  mdl_I92x33 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_conjg__Rd3x3; 
  mdl_I92x36 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_conjg__Rd3x3; 
  mdl_I92x63 = mdl_CKM3x3 * mdl_Ru3x3 * mdl_conjg__Rd6x3; 
  mdl_I92x66 = mdl_CKM3x3 * mdl_Ru6x3 * mdl_conjg__Rd6x3; 
  mdl_I93x33 = mdl_Rn3x3 * mdl_conjg__Rl3x3; 
  mdl_I93x36 = mdl_Rn3x3 * mdl_conjg__Rl6x3; 
  mdl_I94x33 = mdl_Rd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3; 
  mdl_I94x36 = mdl_Rd3x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3; 
  mdl_I94x63 = mdl_Rd6x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru3x3; 
  mdl_I94x66 = mdl_Rd6x3 * mdl_conjg__CKM3x3 * mdl_conjg__Ru6x3; 
  mdl_I95x33 = mdl_Rl3x3 * mdl_conjg__Rn3x3; 
  mdl_I95x36 = mdl_Rl6x3 * mdl_conjg__Rn3x3; 
  mdl_I96x33 = mdl_Rd3x3 * mdl_conjg__Rd3x3; 
  mdl_I96x36 = mdl_Rd6x3 * mdl_conjg__Rd3x3; 
  mdl_I96x63 = mdl_Rd3x3 * mdl_conjg__Rd6x3; 
  mdl_I96x66 = mdl_Rd6x3 * mdl_conjg__Rd6x3; 
  mdl_I97x33 = mdl_Rl3x3 * mdl_conjg__Rl3x3; 
  mdl_I97x36 = mdl_Rl6x3 * mdl_conjg__Rl3x3; 
  mdl_I97x63 = mdl_Rl3x3 * mdl_conjg__Rl6x3; 
  mdl_I97x66 = mdl_Rl6x3 * mdl_conjg__Rl6x3; 
  mdl_I98x33 = mdl_Ru3x3 * mdl_conjg__Ru3x3; 
  mdl_I98x36 = mdl_Ru6x3 * mdl_conjg__Ru3x3; 
  mdl_I98x63 = mdl_Ru3x3 * mdl_conjg__Ru6x3; 
  mdl_I98x66 = mdl_Ru6x3 * mdl_conjg__Ru6x3; 
  mdl_I99x33 = mdl_ye3x3; 
  mdl_complexi = std::complex<double> (0., 1.); 
  mdl_sqrt__2 = sqrt(2.); 
  mdl_sw__exp__2 = ((mdl_sw) * (mdl_sw)); 
  mdl_conjg__NN1x1 = conj(mdl_NN1x1); 
  mdl_conjg__NN1x2 = conj(mdl_NN1x2); 
  mdl_conjg__NN1x3 = conj(mdl_NN1x3); 
  mdl_conjg__NN1x4 = conj(mdl_NN1x4); 
  mdl_conjg__NN2x1 = conj(mdl_NN2x1); 
  mdl_conjg__NN2x2 = conj(mdl_NN2x2); 
  mdl_conjg__NN2x3 = conj(mdl_NN2x3); 
  mdl_conjg__NN2x4 = conj(mdl_NN2x4); 
  mdl_conjg__NN3x1 = conj(mdl_NN3x1); 
  mdl_conjg__NN3x2 = conj(mdl_NN3x2); 
  mdl_conjg__NN3x3 = conj(mdl_NN3x3); 
  mdl_conjg__NN3x4 = conj(mdl_NN3x4); 
  mdl_conjg__NN4x1 = conj(mdl_NN4x1); 
  mdl_conjg__NN4x2 = conj(mdl_NN4x2); 
  mdl_conjg__NN4x3 = conj(mdl_NN4x3); 
  mdl_conjg__NN4x4 = conj(mdl_NN4x4); 
  mdl_conjg__UU1x1 = conj(mdl_UU1x1); 
  mdl_conjg__UU1x2 = conj(mdl_UU1x2); 
  mdl_conjg__UU2x1 = conj(mdl_UU2x1); 
  mdl_conjg__UU2x2 = conj(mdl_UU2x2); 
  mdl_conjg__VV1x1 = conj(mdl_VV1x1); 
  mdl_conjg__VV1x2 = conj(mdl_VV1x2); 
  mdl_conjg__VV2x1 = conj(mdl_VV2x1); 
  mdl_conjg__VV2x2 = conj(mdl_VV2x2); 
  mdl_cos__alp = cos(mdl_alp); 
  mdl_sin__alp = sin(mdl_alp); 
  mdl_conjg__MUH = conj(mdl_MUH); 
  mdl_ee = 2. * sqrt(1./aEWM1) * sqrt(M_PI); 
  mdl_gp = mdl_ee/mdl_cw; 
  mdl_gw = mdl_ee/mdl_sw; 
  mdl_vev = (2. * mdl_cw * mdl_MZ * mdl_sw)/mdl_ee; 
  mdl_vd = mdl_vev * mdl_cos__beta; 
  mdl_vu = mdl_vev * mdl_sin__beta; 
  mdl_ee__exp__2 = ((mdl_ee) * (mdl_ee)); 
  if (mdl_Mneu2 < 0)
    mdl_Wneu2 = -abs(mdl_Wneu2); 
  if (mdl_Mneu3 < 0)
    mdl_Wneu3 = -abs(mdl_Wneu3); 
  if (mdl_Mneu4 < 0)
    mdl_Wneu4 = -abs(mdl_Wneu4); 
  if (mdl_Mgo < 0)
    mdl_Wgo = -abs(mdl_Wgo); 
}
void pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2::setIndependentCouplings()
{
  if (mdl_Mneu2 < 0)
    mdl_Wneu2 = -abs(mdl_Wneu2); 
  if (mdl_Mneu3 < 0)
    mdl_Wneu3 = -abs(mdl_Wneu3); 
  if (mdl_Mneu4 < 0)
    mdl_Wneu4 = -abs(mdl_Wneu4); 
  if (mdl_Mgo < 0)
    mdl_Wgo = -abs(mdl_Wgo); 
}
void pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2::setDependentParameters()
{
  mdl_sqrt__aS = sqrt(aS); 
  G = 2. * mdl_sqrt__aS * sqrt(M_PI); 
  mdl_G__exp__2 = ((G) * (G)); 
  if (mdl_Mneu2 < 0)
    mdl_Wneu2 = -abs(mdl_Wneu2); 
  if (mdl_Mneu3 < 0)
    mdl_Wneu3 = -abs(mdl_Wneu3); 
  if (mdl_Mneu4 < 0)
    mdl_Wneu4 = -abs(mdl_Wneu4); 
  if (mdl_Mgo < 0)
    mdl_Wgo = -abs(mdl_Wgo); 
}
void pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2::setDependentCouplings()
{
  GC_53 = -(mdl_complexi * G * mdl_I51x22); 
  GC_96 = mdl_complexi * mdl_G__exp__2 * mdl_I75x44; 
  GC_7 = mdl_complexi * G; 
  GC_6 = -G; 
  if (mdl_Mneu2 < 0)
    mdl_Wneu2 = -abs(mdl_Wneu2); 
  if (mdl_Mneu3 < 0)
    mdl_Wneu3 = -abs(mdl_Wneu3); 
  if (mdl_Mneu4 < 0)
    mdl_Wneu4 = -abs(mdl_Wneu4); 
  if (mdl_Mgo < 0)
    mdl_Wgo = -abs(mdl_Wgo); 
}

// Routines for printing out parameters
void pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2::printIndependentParameters()
{
  cout <<  "MSSM_SLHA2 model parameters independent of event kinematics:" <<
      endl;
  cout << setw(20) <<  "mdl_Wsl6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsl6 << endl;
  cout << setw(20) <<  "mdl_Wsl5 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsl5 << endl;
  cout << setw(20) <<  "mdl_Wsl4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsl4 << endl;
  cout << setw(20) <<  "mdl_Wsu6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsu6 << endl;
  cout << setw(20) <<  "mdl_Wsd6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsd6 << endl;
  cout << setw(20) <<  "mdl_Wsu5 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsu5 << endl;
  cout << setw(20) <<  "mdl_Wsd5 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsd5 << endl;
  cout << setw(20) <<  "mdl_Wsu4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsu4 << endl;
  cout << setw(20) <<  "mdl_Wsd4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsd4 << endl;
  cout << setw(20) <<  "mdl_Wch2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wch2 << endl;
  cout << setw(20) <<  "mdl_Wneu4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wneu4 << endl;
  cout << setw(20) <<  "mdl_Wneu3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wneu3 << endl;
  cout << setw(20) <<  "mdl_Wch1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wch1 << endl;
  cout << setw(20) <<  "mdl_Wneu2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wneu2 << endl;
  cout << setw(20) <<  "mdl_Wgo " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wgo << endl;
  cout << setw(20) <<  "mdl_Wsn3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsn3 << endl;
  cout << setw(20) <<  "mdl_Wsl3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsl3 << endl;
  cout << setw(20) <<  "mdl_Wsn2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsn2 << endl;
  cout << setw(20) <<  "mdl_Wsl2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsl2 << endl;
  cout << setw(20) <<  "mdl_Wsn1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsn1 << endl;
  cout << setw(20) <<  "mdl_Wsl1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsl1 << endl;
  cout << setw(20) <<  "mdl_Wsu3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsu3 << endl;
  cout << setw(20) <<  "mdl_Wsd3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsd3 << endl;
  cout << setw(20) <<  "mdl_Wsu2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsu2 << endl;
  cout << setw(20) <<  "mdl_Wsd2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsd2 << endl;
  cout << setw(20) <<  "mdl_Wsu1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsu1 << endl;
  cout << setw(20) <<  "mdl_Wsd1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Wsd1 << endl;
  cout << setw(20) <<  "mdl_WH " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_WH << endl;
  cout << setw(20) <<  "mdl_WA0 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_WA0 << endl;
  cout << setw(20) <<  "mdl_WH02 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_WH02 << endl;
  cout << setw(20) <<  "mdl_WH01 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_WH01 << endl;
  cout << setw(20) <<  "mdl_WW " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_WW << endl;
  cout << setw(20) <<  "mdl_WZ " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_WZ << endl;
  cout << setw(20) <<  "mdl_WT " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_WT << endl;
  cout << setw(20) <<  "mdl_Ryu3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_Ryu3x3 << endl;
  cout << setw(20) <<  "mdl_Rye3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_Rye3x3 << endl;
  cout << setw(20) <<  "mdl_Ryd3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_Ryd3x3 << endl;
  cout << setw(20) <<  "mdl_RVV2x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RVV2x2 << endl;
  cout << setw(20) <<  "mdl_RVV2x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RVV2x1 << endl;
  cout << setw(20) <<  "mdl_RVV1x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RVV1x2 << endl;
  cout << setw(20) <<  "mdl_RVV1x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RVV1x1 << endl;
  cout << setw(20) <<  "mdl_RCKM3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RCKM3x3 << endl;
  cout << setw(20) <<  "mdl_RCKM2x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RCKM2x2 << endl;
  cout << setw(20) <<  "mdl_RCKM1x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RCKM1x1 << endl;
  cout << setw(20) <<  "mdl_RRu6x6 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRu6x6 << endl;
  cout << setw(20) <<  "mdl_RRu6x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRu6x3 << endl;
  cout << setw(20) <<  "mdl_RRu5x5 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRu5x5 << endl;
  cout << setw(20) <<  "mdl_RRu4x4 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRu4x4 << endl;
  cout << setw(20) <<  "mdl_RRu3x6 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRu3x6 << endl;
  cout << setw(20) <<  "mdl_RRu3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRu3x3 << endl;
  cout << setw(20) <<  "mdl_RRu2x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRu2x2 << endl;
  cout << setw(20) <<  "mdl_RRu1x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRu1x1 << endl;
  cout << setw(20) <<  "mdl_RMNS3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RMNS3x3 << endl;
  cout << setw(20) <<  "mdl_RMNS2x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RMNS2x2 << endl;
  cout << setw(20) <<  "mdl_RMNS1x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RMNS1x1 << endl;
  cout << setw(20) <<  "mdl_RUU2x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RUU2x2 << endl;
  cout << setw(20) <<  "mdl_RUU2x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RUU2x1 << endl;
  cout << setw(20) <<  "mdl_RUU1x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RUU1x2 << endl;
  cout << setw(20) <<  "mdl_RUU1x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RUU1x1 << endl;
  cout << setw(20) <<  "mdl_Rtu3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_Rtu3x3 << endl;
  cout << setw(20) <<  "mdl_Rte3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_Rte3x3 << endl;
  cout << setw(20) <<  "mdl_Rtd3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_Rtd3x3 << endl;
  cout << setw(20) <<  "mdl_RRn3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRn3x3 << endl;
  cout << setw(20) <<  "mdl_RRn2x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRn2x2 << endl;
  cout << setw(20) <<  "mdl_RRn1x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRn1x1 << endl;
  cout << setw(20) <<  "aS " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << aS << endl;
  cout << setw(20) <<  "aEWM1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << aEWM1 << endl;
  cout << setw(20) <<  "mdl_RRl6x6 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRl6x6 << endl;
  cout << setw(20) <<  "mdl_RRl6x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRl6x3 << endl;
  cout << setw(20) <<  "mdl_RRl5x5 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRl5x5 << endl;
  cout << setw(20) <<  "mdl_RRl4x4 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRl4x4 << endl;
  cout << setw(20) <<  "mdl_RRl3x6 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRl3x6 << endl;
  cout << setw(20) <<  "mdl_RRl3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRl3x3 << endl;
  cout << setw(20) <<  "mdl_RRl2x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRl2x2 << endl;
  cout << setw(20) <<  "mdl_RRl1x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRl1x1 << endl;
  cout << setw(20) <<  "mdl_RNN4x4 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN4x4 << endl;
  cout << setw(20) <<  "mdl_RNN4x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN4x3 << endl;
  cout << setw(20) <<  "mdl_RNN4x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN4x2 << endl;
  cout << setw(20) <<  "mdl_RNN4x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN4x1 << endl;
  cout << setw(20) <<  "mdl_RNN3x4 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN3x4 << endl;
  cout << setw(20) <<  "mdl_RNN3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN3x3 << endl;
  cout << setw(20) <<  "mdl_RNN3x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN3x2 << endl;
  cout << setw(20) <<  "mdl_RNN3x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN3x1 << endl;
  cout << setw(20) <<  "mdl_RNN2x4 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN2x4 << endl;
  cout << setw(20) <<  "mdl_RNN2x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN2x3 << endl;
  cout << setw(20) <<  "mdl_RNN2x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN2x2 << endl;
  cout << setw(20) <<  "mdl_RNN2x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN2x1 << endl;
  cout << setw(20) <<  "mdl_RNN1x4 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN1x4 << endl;
  cout << setw(20) <<  "mdl_RNN1x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN1x3 << endl;
  cout << setw(20) <<  "mdl_RNN1x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN1x2 << endl;
  cout << setw(20) <<  "mdl_RNN1x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RNN1x1 << endl;
  cout << setw(20) <<  "mdl_RmU23x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmU23x3 << endl;
  cout << setw(20) <<  "mdl_RmU21x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmU21x1 << endl;
  cout << setw(20) <<  "mdl_RmQ23x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmQ23x3 << endl;
  cout << setw(20) <<  "mdl_RmQ21x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmQ21x1 << endl;
  cout << setw(20) <<  "mdl_mHu2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_mHu2 << endl;
  cout << setw(20) <<  "mdl_mHd2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_mHd2 << endl;
  cout << setw(20) <<  "mdl_RMx3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_RMx3 << endl;
  cout << setw(20) <<  "mdl_RMx2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_RMx2 << endl;
  cout << setw(20) <<  "mdl_RMx1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_RMx1 << endl;
  cout << setw(20) <<  "mdl_RmL23x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmL23x3 << endl;
  cout << setw(20) <<  "mdl_RmL21x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmL21x1 << endl;
  cout << setw(20) <<  "mdl_RmE23x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmE23x3 << endl;
  cout << setw(20) <<  "mdl_RmE21x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmE21x1 << endl;
  cout << setw(20) <<  "mdl_RmD23x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmD23x3 << endl;
  cout << setw(20) <<  "mdl_RmD21x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmD21x1 << endl;
  cout << setw(20) <<  "mdl_Msl6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msl6 << endl;
  cout << setw(20) <<  "mdl_Msl4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msl4 << endl;
  cout << setw(20) <<  "mdl_Msu6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msu6 << endl;
  cout << setw(20) <<  "mdl_Msd6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msd6 << endl;
  cout << setw(20) <<  "mdl_Msu4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msu4 << endl;
  cout << setw(20) <<  "mdl_Msd4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msd4 << endl;
  cout << setw(20) <<  "mdl_Mch2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Mch2 << endl;
  cout << setw(20) <<  "mdl_Mneu4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Mneu4 << endl;
  cout << setw(20) <<  "mdl_Mneu3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Mneu3 << endl;
  cout << setw(20) <<  "mdl_Mch1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Mch1 << endl;
  cout << setw(20) <<  "mdl_Mneu2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Mneu2 << endl;
  cout << setw(20) <<  "mdl_Mneu1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Mneu1 << endl;
  cout << setw(20) <<  "mdl_Mgo " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Mgo << endl;
  cout << setw(20) <<  "mdl_Msn3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msn3 << endl;
  cout << setw(20) <<  "mdl_Msl3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msl3 << endl;
  cout << setw(20) <<  "mdl_Msn1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msn1 << endl;
  cout << setw(20) <<  "mdl_Msl1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msl1 << endl;
  cout << setw(20) <<  "mdl_Msu3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msu3 << endl;
  cout << setw(20) <<  "mdl_Msd3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msd3 << endl;
  cout << setw(20) <<  "mdl_Msu1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msu1 << endl;
  cout << setw(20) <<  "mdl_Msd1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msd1 << endl;
  cout << setw(20) <<  "mdl_MH " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_MH << endl;
  cout << setw(20) <<  "mdl_MA0 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_MA0 << endl;
  cout << setw(20) <<  "mdl_MH02 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_MH02 << endl;
  cout << setw(20) <<  "mdl_MH01 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_MH01 << endl;
  cout << setw(20) <<  "mdl_MW " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_MW << endl;
  cout << setw(20) <<  "mdl_MZ " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_MZ << endl;
  cout << setw(20) <<  "mdl_Mta " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Mta << endl;
  cout << setw(20) <<  "mdl_MT " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_MT << endl;
  cout << setw(20) <<  "mdl_MB " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_MB << endl;
  cout << setw(20) <<  "mdl_MA2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_MA2 << endl;
  cout << setw(20) <<  "mdl_tb " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_tb << endl;
  cout << setw(20) <<  "mdl_RMUH " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_RMUH << endl;
  cout << setw(20) <<  "mdl_alp " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_alp << endl;
  cout << setw(20) <<  "mdl_RRd6x6 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRd6x6 << endl;
  cout << setw(20) <<  "mdl_RRd6x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRd6x3 << endl;
  cout << setw(20) <<  "mdl_RRd5x5 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRd5x5 << endl;
  cout << setw(20) <<  "mdl_RRd4x4 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRd4x4 << endl;
  cout << setw(20) <<  "mdl_RRd3x6 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRd3x6 << endl;
  cout << setw(20) <<  "mdl_RRd3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRd3x3 << endl;
  cout << setw(20) <<  "mdl_RRd2x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRd2x2 << endl;
  cout << setw(20) <<  "mdl_RRd1x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RRd1x1 << endl;
  cout << setw(20) <<  "mdl_Msu5 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msu5 << endl;
  cout << setw(20) <<  "mdl_Msu2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msu2 << endl;
  cout << setw(20) <<  "mdl_RmD22x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmD22x2 << endl;
  cout << setw(20) <<  "mdl_Msl2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msl2 << endl;
  cout << setw(20) <<  "mdl_Msl5 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msl5 << endl;
  cout << setw(20) <<  "mdl_RmL22x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmL22x2 << endl;
  cout << setw(20) <<  "mdl_Msd2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msd2 << endl;
  cout << setw(20) <<  "mdl_RmQ22x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmQ22x2 << endl;
  cout << setw(20) <<  "mdl_Msd5 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msd5 << endl;
  cout << setw(20) <<  "mdl_RmU22x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmU22x2 << endl;
  cout << setw(20) <<  "mdl_Msn2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Msn2 << endl;
  cout << setw(20) <<  "mdl_RmE22x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_RmE22x2 << endl;
  cout << setw(20) <<  "mdl_conjg__CKM3x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__CKM3x3 << endl;
  cout << setw(20) <<  "mdl_I93x22 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I93x22 << endl;
  cout << setw(20) <<  "mdl_I97x11 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I97x11 << endl;
  cout << setw(20) <<  "mdl_I43x22 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I43x22 << endl;
  cout << setw(20) <<  "mdl_I51x11 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I51x11 << endl;
  cout << setw(20) <<  "mdl_I75x55 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I75x55 << endl;
  cout << setw(20) <<  "mdl_I92x22 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I92x22 << endl;
  cout << setw(20) <<  "mdl_I96x11 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I96x11 << endl;
  cout << setw(20) <<  "mdl_I5x11 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I5x11 << endl;
  cout << setw(20) <<  "mdl_I100x44 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I100x44 << endl;
  cout << setw(20) <<  "mdl_conjg__Rn3x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rn3x3 << endl;
  cout << setw(20) <<  "mdl_I45x11 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I45x11 << endl;
  cout << setw(20) <<  "mdl_I75x44 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I75x44 << endl;
  cout << setw(20) <<  "mdl_I25x22 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I25x22 << endl;
  cout << setw(20) <<  "mdl_I52x44 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I52x44 << endl;
  cout << setw(20) <<  "mdl_conjg__Ru1x1 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Ru1x1 << endl;
  cout << setw(20) <<  "mdl_I32x44 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I32x44 << endl;
  cout << setw(20) <<  "mdl_conjg__Ru4x4 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Ru4x4 << endl;
  cout << setw(20) <<  "mdl_I6x44 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I6x44 << endl;
  cout << setw(20) <<  "mdl_I53x11 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I53x11 << endl;
  cout << setw(20) <<  "mdl_I74x11 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I74x11 << endl;
  cout << setw(20) <<  "mdl_conjg__Rd5x5 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rd5x5 << endl;
  cout << setw(20) <<  "mdl_conjg__Rl1x1 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rl1x1 << endl;
  cout << setw(20) <<  "mdl_I5x22 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I5x22 << endl;
  cout << setw(20) <<  "mdl_I101x55 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I101x55 << endl;
  cout << setw(20) <<  "mdl_I102x44 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I102x44 << endl;
  cout << setw(20) <<  "mdl_I52x55 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I52x55 << endl;
  cout << setw(20) <<  "mdl_I26x44 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I26x44 << endl;
  cout << setw(20) <<  "mdl_conjg__Ru2x2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Ru2x2 << endl;
  cout << setw(20) <<  "mdl_I26x55 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I26x55 << endl;
  cout << setw(20) <<  "mdl_I96x22 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I96x22 << endl;
  cout << setw(20) <<  "mdl_I13x44 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I13x44 << endl;
  cout << setw(20) <<  "mdl_I51x22 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I51x22 << endl;
  cout << setw(20) <<  "mdl_I89x11 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I89x11 << endl;
  cout << setw(20) <<  "mdl_I98x11 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I98x11 << endl;
  cout << setw(20) <<  "mdl_I100x55 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I100x55 << endl;
  cout << setw(20) <<  "mdl_Ru5x5 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Ru5x5 << endl;
  cout << setw(20) <<  "mdl_Rl1x1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Rl1x1 << endl;
  cout << setw(20) <<  "mdl_conjg__Rl4x4 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rl4x4 << endl;
  cout << setw(20) <<  "mdl_Ru2x2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Ru2x2 << endl;
  cout << setw(20) <<  "mdl_Ru4x4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Ru4x4 << endl;
  cout << setw(20) <<  "mdl_conjg__Rd4x4 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rd4x4 << endl;
  cout << setw(20) <<  "mdl_Rd1x1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Rd1x1 << endl;
  cout << setw(20) <<  "mdl_I7x22 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I7x22 << endl;
  cout << setw(20) <<  "mdl_I25x11 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I25x11 << endl;
  cout << setw(20) <<  "mdl_I6x55 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I6x55 << endl;
  cout << setw(20) <<  "mdl_CKM3x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_CKM3x3 << endl;
  cout << setw(20) <<  "mdl_Rn3x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Rn3x3 << endl;
  cout << setw(20) <<  "mdl_conjg__Rd1x1 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rd1x1 << endl;
  cout << setw(20) <<  "mdl_I85x22 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I85x22 << endl;
  cout << setw(20) <<  "mdl_conjg__Rn2x2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rn2x2 << endl;
  cout << setw(20) <<  "mdl_I39x11 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I39x11 << endl;
  cout << setw(20) <<  "mdl_I101x44 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I101x44 << endl;
  cout << setw(20) <<  "mdl_I102x55 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I102x55 << endl;
  cout << setw(20) <<  "mdl_atan__tb " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_atan__tb << endl;
  cout << setw(20) <<  "mdl_beta " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_beta << endl;
  cout << setw(20) <<  "mdl_cw " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_cw << endl;
  cout << setw(20) <<  "mdl_mD21x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mD21x1 << endl;
  cout << setw(20) <<  "mdl_mD22x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mD22x2 << endl;
  cout << setw(20) <<  "mdl_mD23x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mD23x3 << endl;
  cout << setw(20) <<  "mdl_mE21x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mE21x1 << endl;
  cout << setw(20) <<  "mdl_mE22x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mE22x2 << endl;
  cout << setw(20) <<  "mdl_mE23x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mE23x3 << endl;
  cout << setw(20) <<  "mdl_mL21x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mL21x1 << endl;
  cout << setw(20) <<  "mdl_mL22x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mL22x2 << endl;
  cout << setw(20) <<  "mdl_mL23x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mL23x3 << endl;
  cout << setw(20) <<  "mdl_mQ21x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mQ21x1 << endl;
  cout << setw(20) <<  "mdl_mQ22x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mQ22x2 << endl;
  cout << setw(20) <<  "mdl_mQ23x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mQ23x3 << endl;
  cout << setw(20) <<  "mdl_mU21x1 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mU21x1 << endl;
  cout << setw(20) <<  "mdl_mU22x2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mU22x2 << endl;
  cout << setw(20) <<  "mdl_mU23x3 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_mU23x3 << endl;
  cout << setw(20) <<  "mdl_MUH " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_MUH << endl;
  cout << setw(20) <<  "mdl_Mx1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Mx1 << endl;
  cout << setw(20) <<  "mdl_Mx2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Mx2 << endl;
  cout << setw(20) <<  "mdl_Mx3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Mx3 << endl;
  cout << setw(20) <<  "mdl_NN1x1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN1x1 << endl;
  cout << setw(20) <<  "mdl_NN1x2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN1x2 << endl;
  cout << setw(20) <<  "mdl_NN1x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN1x3 << endl;
  cout << setw(20) <<  "mdl_NN1x4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN1x4 << endl;
  cout << setw(20) <<  "mdl_NN2x1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN2x1 << endl;
  cout << setw(20) <<  "mdl_NN2x2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN2x2 << endl;
  cout << setw(20) <<  "mdl_NN2x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN2x3 << endl;
  cout << setw(20) <<  "mdl_NN2x4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN2x4 << endl;
  cout << setw(20) <<  "mdl_NN3x1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN3x1 << endl;
  cout << setw(20) <<  "mdl_NN3x2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN3x2 << endl;
  cout << setw(20) <<  "mdl_NN3x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN3x3 << endl;
  cout << setw(20) <<  "mdl_NN3x4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN3x4 << endl;
  cout << setw(20) <<  "mdl_NN4x1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN4x1 << endl;
  cout << setw(20) <<  "mdl_NN4x2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN4x2 << endl;
  cout << setw(20) <<  "mdl_NN4x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN4x3 << endl;
  cout << setw(20) <<  "mdl_NN4x4 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_NN4x4 << endl;
  cout << setw(20) <<  "mdl_Rd3x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Rd3x3 << endl;
  cout << setw(20) <<  "mdl_Rd3x6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Rd3x6 << endl;
  cout << setw(20) <<  "mdl_Rd6x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Rd6x3 << endl;
  cout << setw(20) <<  "mdl_Rd6x6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Rd6x6 << endl;
  cout << setw(20) <<  "mdl_Rl3x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Rl3x3 << endl;
  cout << setw(20) <<  "mdl_Rl3x6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Rl3x6 << endl;
  cout << setw(20) <<  "mdl_Rl6x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Rl6x3 << endl;
  cout << setw(20) <<  "mdl_Rl6x6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Rl6x6 << endl;
  cout << setw(20) <<  "mdl_Ru3x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Ru3x3 << endl;
  cout << setw(20) <<  "mdl_Ru3x6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Ru3x6 << endl;
  cout << setw(20) <<  "mdl_Ru6x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Ru6x3 << endl;
  cout << setw(20) <<  "mdl_Ru6x6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_Ru6x6 << endl;
  cout << setw(20) <<  "mdl_UU1x1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_UU1x1 << endl;
  cout << setw(20) <<  "mdl_UU1x2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_UU1x2 << endl;
  cout << setw(20) <<  "mdl_UU2x1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_UU2x1 << endl;
  cout << setw(20) <<  "mdl_UU2x2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_UU2x2 << endl;
  cout << setw(20) <<  "mdl_VV1x1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_VV1x1 << endl;
  cout << setw(20) <<  "mdl_VV1x2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_VV1x2 << endl;
  cout << setw(20) <<  "mdl_VV2x1 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_VV2x1 << endl;
  cout << setw(20) <<  "mdl_VV2x2 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_VV2x2 << endl;
  cout << setw(20) <<  "mdl_td3x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_td3x3 << endl;
  cout << setw(20) <<  "mdl_te3x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_te3x3 << endl;
  cout << setw(20) <<  "mdl_tu3x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_tu3x3 << endl;
  cout << setw(20) <<  "mdl_yd3x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_yd3x3 << endl;
  cout << setw(20) <<  "mdl_ye3x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_ye3x3 << endl;
  cout << setw(20) <<  "mdl_yu3x3 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_yu3x3 << endl;
  cout << setw(20) <<  "mdl_MZ__exp__2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_MZ__exp__2 << endl;
  cout << setw(20) <<  "mdl_bb " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_bb << endl;
  cout << setw(20) <<  "mdl_cw__exp__2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_cw__exp__2 << endl;
  cout << setw(20) <<  "mdl_sw " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_sw << endl;
  cout << setw(20) <<  "mdl_cos__beta " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_cos__beta << endl;
  cout << setw(20) <<  "mdl_sin__beta " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_sin__beta << endl;
  cout << setw(20) <<  "mdl_conjg__yu3x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__yu3x3 << endl;
  cout << setw(20) <<  "mdl_I1x33 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I1x33 << endl;
  cout << setw(20) <<  "mdl_conjg__yd3x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__yd3x3 << endl;
  cout << setw(20) <<  "mdl_I10x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I10x33 << endl;
  cout << setw(20) <<  "mdl_I10x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I10x36 << endl;
  cout << setw(20) <<  "mdl_conjg__Rd3x6 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rd3x6 << endl;
  cout << setw(20) <<  "mdl_I100x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I100x33 << endl;
  cout << setw(20) <<  "mdl_I100x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I100x36 << endl;
  cout << setw(20) <<  "mdl_conjg__Rd6x6 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rd6x6 << endl;
  cout << setw(20) <<  "mdl_I100x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I100x63 << endl;
  cout << setw(20) <<  "mdl_I100x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I100x66 << endl;
  cout << setw(20) <<  "mdl_conjg__Rl3x6 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rl3x6 << endl;
  cout << setw(20) <<  "mdl_I101x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I101x33 << endl;
  cout << setw(20) <<  "mdl_I101x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I101x36 << endl;
  cout << setw(20) <<  "mdl_conjg__Rl6x6 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rl6x6 << endl;
  cout << setw(20) <<  "mdl_I101x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I101x63 << endl;
  cout << setw(20) <<  "mdl_I101x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I101x66 << endl;
  cout << setw(20) <<  "mdl_conjg__Ru3x6 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Ru3x6 << endl;
  cout << setw(20) <<  "mdl_I102x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I102x33 << endl;
  cout << setw(20) <<  "mdl_I102x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I102x36 << endl;
  cout << setw(20) <<  "mdl_conjg__Ru6x6 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Ru6x6 << endl;
  cout << setw(20) <<  "mdl_I102x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I102x63 << endl;
  cout << setw(20) <<  "mdl_I102x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I102x66 << endl;
  cout << setw(20) <<  "mdl_I11x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I11x33 << endl;
  cout << setw(20) <<  "mdl_I11x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I11x36 << endl;
  cout << setw(20) <<  "mdl_conjg__Rd3x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rd3x3 << endl;
  cout << setw(20) <<  "mdl_I12x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I12x33 << endl;
  cout << setw(20) <<  "mdl_I12x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I12x36 << endl;
  cout << setw(20) <<  "mdl_conjg__Rd6x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rd6x3 << endl;
  cout << setw(20) <<  "mdl_I12x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I12x63 << endl;
  cout << setw(20) <<  "mdl_I12x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I12x66 << endl;
  cout << setw(20) <<  "mdl_I13x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I13x33 << endl;
  cout << setw(20) <<  "mdl_I13x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I13x36 << endl;
  cout << setw(20) <<  "mdl_I13x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I13x63 << endl;
  cout << setw(20) <<  "mdl_I13x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I13x66 << endl;
  cout << setw(20) <<  "mdl_conjg__td3x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__td3x3 << endl;
  cout << setw(20) <<  "mdl_I14x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I14x33 << endl;
  cout << setw(20) <<  "mdl_I14x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I14x36 << endl;
  cout << setw(20) <<  "mdl_I14x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I14x63 << endl;
  cout << setw(20) <<  "mdl_I14x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I14x66 << endl;
  cout << setw(20) <<  "mdl_I15x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I15x33 << endl;
  cout << setw(20) <<  "mdl_I15x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I15x36 << endl;
  cout << setw(20) <<  "mdl_I15x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I15x63 << endl;
  cout << setw(20) <<  "mdl_I15x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I15x66 << endl;
  cout << setw(20) <<  "mdl_I16x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I16x33 << endl;
  cout << setw(20) <<  "mdl_I16x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I16x36 << endl;
  cout << setw(20) <<  "mdl_I16x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I16x63 << endl;
  cout << setw(20) <<  "mdl_I16x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I16x66 << endl;
  cout << setw(20) <<  "mdl_I17x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I17x33 << endl;
  cout << setw(20) <<  "mdl_I17x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I17x36 << endl;
  cout << setw(20) <<  "mdl_I17x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I17x63 << endl;
  cout << setw(20) <<  "mdl_I17x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I17x66 << endl;
  cout << setw(20) <<  "mdl_I18x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I18x33 << endl;
  cout << setw(20) <<  "mdl_I18x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I18x36 << endl;
  cout << setw(20) <<  "mdl_I18x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I18x63 << endl;
  cout << setw(20) <<  "mdl_I18x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I18x66 << endl;
  cout << setw(20) <<  "mdl_I19x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I19x33 << endl;
  cout << setw(20) <<  "mdl_I19x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I19x36 << endl;
  cout << setw(20) <<  "mdl_I19x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I19x63 << endl;
  cout << setw(20) <<  "mdl_I19x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I19x66 << endl;
  cout << setw(20) <<  "mdl_I2x33 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I2x33 << endl;
  cout << setw(20) <<  "mdl_I20x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I20x33 << endl;
  cout << setw(20) <<  "mdl_I21x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I21x33 << endl;
  cout << setw(20) <<  "mdl_conjg__ye3x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__ye3x3 << endl;
  cout << setw(20) <<  "mdl_I22x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I22x33 << endl;
  cout << setw(20) <<  "mdl_I23x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I23x33 << endl;
  cout << setw(20) <<  "mdl_I23x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I23x36 << endl;
  cout << setw(20) <<  "mdl_conjg__Rl3x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rl3x3 << endl;
  cout << setw(20) <<  "mdl_I24x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I24x33 << endl;
  cout << setw(20) <<  "mdl_conjg__Rl6x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Rl6x3 << endl;
  cout << setw(20) <<  "mdl_I24x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I24x36 << endl;
  cout << setw(20) <<  "mdl_I25x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I25x33 << endl;
  cout << setw(20) <<  "mdl_I25x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I25x36 << endl;
  cout << setw(20) <<  "mdl_I25x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I25x63 << endl;
  cout << setw(20) <<  "mdl_I25x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I25x66 << endl;
  cout << setw(20) <<  "mdl_I26x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I26x33 << endl;
  cout << setw(20) <<  "mdl_I26x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I26x36 << endl;
  cout << setw(20) <<  "mdl_I26x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I26x63 << endl;
  cout << setw(20) <<  "mdl_I26x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I26x66 << endl;
  cout << setw(20) <<  "mdl_I27x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I27x33 << endl;
  cout << setw(20) <<  "mdl_I27x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I27x36 << endl;
  cout << setw(20) <<  "mdl_I28x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I28x33 << endl;
  cout << setw(20) <<  "mdl_I28x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I28x36 << endl;
  cout << setw(20) <<  "mdl_I29x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I29x33 << endl;
  cout << setw(20) <<  "mdl_I29x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I29x36 << endl;
  cout << setw(20) <<  "mdl_I3x33 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I3x33 << endl;
  cout << setw(20) <<  "mdl_I3x36 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I3x36 << endl;
  cout << setw(20) <<  "mdl_I30x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I30x33 << endl;
  cout << setw(20) <<  "mdl_I30x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I30x36 << endl;
  cout << setw(20) <<  "mdl_I31x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I31x33 << endl;
  cout << setw(20) <<  "mdl_I31x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I31x36 << endl;
  cout << setw(20) <<  "mdl_I31x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I31x63 << endl;
  cout << setw(20) <<  "mdl_I31x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I31x66 << endl;
  cout << setw(20) <<  "mdl_I32x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I32x33 << endl;
  cout << setw(20) <<  "mdl_I32x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I32x36 << endl;
  cout << setw(20) <<  "mdl_I32x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I32x63 << endl;
  cout << setw(20) <<  "mdl_I32x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I32x66 << endl;
  cout << setw(20) <<  "mdl_conjg__te3x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__te3x3 << endl;
  cout << setw(20) <<  "mdl_I33x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I33x33 << endl;
  cout << setw(20) <<  "mdl_I33x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I33x36 << endl;
  cout << setw(20) <<  "mdl_I33x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I33x63 << endl;
  cout << setw(20) <<  "mdl_I33x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I33x66 << endl;
  cout << setw(20) <<  "mdl_I34x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I34x33 << endl;
  cout << setw(20) <<  "mdl_I34x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I34x36 << endl;
  cout << setw(20) <<  "mdl_I34x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I34x63 << endl;
  cout << setw(20) <<  "mdl_I34x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I34x66 << endl;
  cout << setw(20) <<  "mdl_I35x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I35x33 << endl;
  cout << setw(20) <<  "mdl_I35x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I35x36 << endl;
  cout << setw(20) <<  "mdl_I35x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I35x63 << endl;
  cout << setw(20) <<  "mdl_I35x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I35x66 << endl;
  cout << setw(20) <<  "mdl_I36x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I36x33 << endl;
  cout << setw(20) <<  "mdl_I36x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I36x36 << endl;
  cout << setw(20) <<  "mdl_I36x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I36x63 << endl;
  cout << setw(20) <<  "mdl_I36x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I36x66 << endl;
  cout << setw(20) <<  "mdl_I37x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I37x33 << endl;
  cout << setw(20) <<  "mdl_I37x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I37x36 << endl;
  cout << setw(20) <<  "mdl_I37x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I37x63 << endl;
  cout << setw(20) <<  "mdl_I37x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I37x66 << endl;
  cout << setw(20) <<  "mdl_I38x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I38x33 << endl;
  cout << setw(20) <<  "mdl_I38x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I38x36 << endl;
  cout << setw(20) <<  "mdl_I38x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I38x63 << endl;
  cout << setw(20) <<  "mdl_I38x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I38x66 << endl;
  cout << setw(20) <<  "mdl_I39x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I39x33 << endl;
  cout << setw(20) <<  "mdl_I39x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I39x36 << endl;
  cout << setw(20) <<  "mdl_I4x33 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I4x33 << endl;
  cout << setw(20) <<  "mdl_I4x36 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I4x36 << endl;
  cout << setw(20) <<  "mdl_I40x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I40x33 << endl;
  cout << setw(20) <<  "mdl_I40x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I40x36 << endl;
  cout << setw(20) <<  "mdl_I41x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I41x33 << endl;
  cout << setw(20) <<  "mdl_I41x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I41x36 << endl;
  cout << setw(20) <<  "mdl_I42x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I42x33 << endl;
  cout << setw(20) <<  "mdl_I42x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I42x36 << endl;
  cout << setw(20) <<  "mdl_I44x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I44x33 << endl;
  cout << setw(20) <<  "mdl_I45x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I45x33 << endl;
  cout << setw(20) <<  "mdl_I45x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I45x36 << endl;
  cout << setw(20) <<  "mdl_I46x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I46x33 << endl;
  cout << setw(20) <<  "mdl_I46x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I46x36 << endl;
  cout << setw(20) <<  "mdl_I47x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I47x33 << endl;
  cout << setw(20) <<  "mdl_I47x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I47x36 << endl;
  cout << setw(20) <<  "mdl_I48x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I48x33 << endl;
  cout << setw(20) <<  "mdl_I48x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I48x36 << endl;
  cout << setw(20) <<  "mdl_I49x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I49x33 << endl;
  cout << setw(20) <<  "mdl_I49x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I49x36 << endl;
  cout << setw(20) <<  "mdl_I5x33 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I5x33 << endl;
  cout << setw(20) <<  "mdl_I5x36 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I5x36 << endl;
  cout << setw(20) <<  "mdl_I5x63 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I5x63 << endl;
  cout << setw(20) <<  "mdl_I5x66 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I5x66 << endl;
  cout << setw(20) <<  "mdl_conjg__Ru3x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Ru3x3 << endl;
  cout << setw(20) <<  "mdl_I50x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I50x33 << endl;
  cout << setw(20) <<  "mdl_conjg__Ru6x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__Ru6x3 << endl;
  cout << setw(20) <<  "mdl_I50x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I50x36 << endl;
  cout << setw(20) <<  "mdl_I51x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I51x33 << endl;
  cout << setw(20) <<  "mdl_I51x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I51x36 << endl;
  cout << setw(20) <<  "mdl_I51x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I51x63 << endl;
  cout << setw(20) <<  "mdl_I51x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I51x66 << endl;
  cout << setw(20) <<  "mdl_I52x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I52x33 << endl;
  cout << setw(20) <<  "mdl_I52x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I52x36 << endl;
  cout << setw(20) <<  "mdl_I52x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I52x63 << endl;
  cout << setw(20) <<  "mdl_I52x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I52x66 << endl;
  cout << setw(20) <<  "mdl_I53x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I53x33 << endl;
  cout << setw(20) <<  "mdl_I53x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I53x36 << endl;
  cout << setw(20) <<  "mdl_I53x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I53x63 << endl;
  cout << setw(20) <<  "mdl_I53x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I53x66 << endl;
  cout << setw(20) <<  "mdl_conjg__tu3x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__tu3x3 << endl;
  cout << setw(20) <<  "mdl_I54x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I54x33 << endl;
  cout << setw(20) <<  "mdl_I54x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I54x36 << endl;
  cout << setw(20) <<  "mdl_I54x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I54x63 << endl;
  cout << setw(20) <<  "mdl_I54x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I54x66 << endl;
  cout << setw(20) <<  "mdl_I55x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I55x33 << endl;
  cout << setw(20) <<  "mdl_I55x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I55x36 << endl;
  cout << setw(20) <<  "mdl_I55x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I55x63 << endl;
  cout << setw(20) <<  "mdl_I55x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I55x66 << endl;
  cout << setw(20) <<  "mdl_I56x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I56x33 << endl;
  cout << setw(20) <<  "mdl_I56x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I56x36 << endl;
  cout << setw(20) <<  "mdl_I56x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I56x63 << endl;
  cout << setw(20) <<  "mdl_I56x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I56x66 << endl;
  cout << setw(20) <<  "mdl_I57x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I57x33 << endl;
  cout << setw(20) <<  "mdl_I57x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I57x36 << endl;
  cout << setw(20) <<  "mdl_I57x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I57x63 << endl;
  cout << setw(20) <<  "mdl_I57x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I57x66 << endl;
  cout << setw(20) <<  "mdl_I58x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I58x33 << endl;
  cout << setw(20) <<  "mdl_I58x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I58x36 << endl;
  cout << setw(20) <<  "mdl_I58x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I58x63 << endl;
  cout << setw(20) <<  "mdl_I58x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I58x66 << endl;
  cout << setw(20) <<  "mdl_I59x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I59x33 << endl;
  cout << setw(20) <<  "mdl_I59x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I59x36 << endl;
  cout << setw(20) <<  "mdl_I59x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I59x63 << endl;
  cout << setw(20) <<  "mdl_I59x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I59x66 << endl;
  cout << setw(20) <<  "mdl_I6x33 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I6x33 << endl;
  cout << setw(20) <<  "mdl_I6x36 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I6x36 << endl;
  cout << setw(20) <<  "mdl_I6x63 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I6x63 << endl;
  cout << setw(20) <<  "mdl_I6x66 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I6x66 << endl;
  cout << setw(20) <<  "mdl_I60x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I60x33 << endl;
  cout << setw(20) <<  "mdl_I60x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I60x36 << endl;
  cout << setw(20) <<  "mdl_I60x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I60x63 << endl;
  cout << setw(20) <<  "mdl_I60x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I60x66 << endl;
  cout << setw(20) <<  "mdl_I61x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I61x33 << endl;
  cout << setw(20) <<  "mdl_I61x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I61x36 << endl;
  cout << setw(20) <<  "mdl_I62x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I62x33 << endl;
  cout << setw(20) <<  "mdl_I62x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I62x36 << endl;
  cout << setw(20) <<  "mdl_I63x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I63x33 << endl;
  cout << setw(20) <<  "mdl_I63x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I63x36 << endl;
  cout << setw(20) <<  "mdl_I64x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I64x33 << endl;
  cout << setw(20) <<  "mdl_I64x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I64x36 << endl;
  cout << setw(20) <<  "mdl_I65x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I65x33 << endl;
  cout << setw(20) <<  "mdl_I65x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I65x36 << endl;
  cout << setw(20) <<  "mdl_I66x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I66x33 << endl;
  cout << setw(20) <<  "mdl_I66x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I66x36 << endl;
  cout << setw(20) <<  "mdl_I66x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I66x63 << endl;
  cout << setw(20) <<  "mdl_I66x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I66x66 << endl;
  cout << setw(20) <<  "mdl_I67x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I67x33 << endl;
  cout << setw(20) <<  "mdl_I67x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I67x36 << endl;
  cout << setw(20) <<  "mdl_I67x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I67x63 << endl;
  cout << setw(20) <<  "mdl_I67x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I67x66 << endl;
  cout << setw(20) <<  "mdl_I68x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I68x33 << endl;
  cout << setw(20) <<  "mdl_I68x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I68x36 << endl;
  cout << setw(20) <<  "mdl_I68x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I68x63 << endl;
  cout << setw(20) <<  "mdl_I68x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I68x66 << endl;
  cout << setw(20) <<  "mdl_I69x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I69x33 << endl;
  cout << setw(20) <<  "mdl_I69x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I69x36 << endl;
  cout << setw(20) <<  "mdl_I69x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I69x63 << endl;
  cout << setw(20) <<  "mdl_I69x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I69x66 << endl;
  cout << setw(20) <<  "mdl_I7x33 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I7x33 << endl;
  cout << setw(20) <<  "mdl_I7x36 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I7x36 << endl;
  cout << setw(20) <<  "mdl_I70x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I70x33 << endl;
  cout << setw(20) <<  "mdl_I70x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I70x36 << endl;
  cout << setw(20) <<  "mdl_I70x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I70x63 << endl;
  cout << setw(20) <<  "mdl_I70x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I70x66 << endl;
  cout << setw(20) <<  "mdl_I71x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I71x33 << endl;
  cout << setw(20) <<  "mdl_I71x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I71x36 << endl;
  cout << setw(20) <<  "mdl_I71x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I71x63 << endl;
  cout << setw(20) <<  "mdl_I71x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I71x66 << endl;
  cout << setw(20) <<  "mdl_I72x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I72x33 << endl;
  cout << setw(20) <<  "mdl_I72x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I72x36 << endl;
  cout << setw(20) <<  "mdl_I72x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I72x63 << endl;
  cout << setw(20) <<  "mdl_I72x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I72x66 << endl;
  cout << setw(20) <<  "mdl_I73x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I73x33 << endl;
  cout << setw(20) <<  "mdl_I73x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I73x36 << endl;
  cout << setw(20) <<  "mdl_I73x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I73x63 << endl;
  cout << setw(20) <<  "mdl_I73x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I73x66 << endl;
  cout << setw(20) <<  "mdl_I74x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I74x33 << endl;
  cout << setw(20) <<  "mdl_I74x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I74x36 << endl;
  cout << setw(20) <<  "mdl_I74x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I74x63 << endl;
  cout << setw(20) <<  "mdl_I74x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I74x66 << endl;
  cout << setw(20) <<  "mdl_I75x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I75x33 << endl;
  cout << setw(20) <<  "mdl_I75x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I75x36 << endl;
  cout << setw(20) <<  "mdl_I75x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I75x63 << endl;
  cout << setw(20) <<  "mdl_I75x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I75x66 << endl;
  cout << setw(20) <<  "mdl_I76x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I76x33 << endl;
  cout << setw(20) <<  "mdl_I76x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I76x36 << endl;
  cout << setw(20) <<  "mdl_I76x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I76x63 << endl;
  cout << setw(20) <<  "mdl_I76x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I76x66 << endl;
  cout << setw(20) <<  "mdl_I77x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I77x33 << endl;
  cout << setw(20) <<  "mdl_I77x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I77x36 << endl;
  cout << setw(20) <<  "mdl_I77x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I77x63 << endl;
  cout << setw(20) <<  "mdl_I77x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I77x66 << endl;
  cout << setw(20) <<  "mdl_I78x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I78x33 << endl;
  cout << setw(20) <<  "mdl_I78x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I78x36 << endl;
  cout << setw(20) <<  "mdl_I78x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I78x63 << endl;
  cout << setw(20) <<  "mdl_I78x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I78x66 << endl;
  cout << setw(20) <<  "mdl_I79x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I79x33 << endl;
  cout << setw(20) <<  "mdl_I79x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I79x36 << endl;
  cout << setw(20) <<  "mdl_I79x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I79x63 << endl;
  cout << setw(20) <<  "mdl_I79x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I79x66 << endl;
  cout << setw(20) <<  "mdl_I8x33 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I8x33 << endl;
  cout << setw(20) <<  "mdl_I8x36 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I8x36 << endl;
  cout << setw(20) <<  "mdl_I80x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I80x33 << endl;
  cout << setw(20) <<  "mdl_I80x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I80x36 << endl;
  cout << setw(20) <<  "mdl_I80x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I80x63 << endl;
  cout << setw(20) <<  "mdl_I80x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I80x66 << endl;
  cout << setw(20) <<  "mdl_I81x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I81x33 << endl;
  cout << setw(20) <<  "mdl_I81x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I81x36 << endl;
  cout << setw(20) <<  "mdl_I81x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I81x63 << endl;
  cout << setw(20) <<  "mdl_I81x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I81x66 << endl;
  cout << setw(20) <<  "mdl_I82x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I82x33 << endl;
  cout << setw(20) <<  "mdl_I82x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I82x36 << endl;
  cout << setw(20) <<  "mdl_I83x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I83x33 << endl;
  cout << setw(20) <<  "mdl_I83x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I83x36 << endl;
  cout << setw(20) <<  "mdl_I84x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I84x33 << endl;
  cout << setw(20) <<  "mdl_I84x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I84x36 << endl;
  cout << setw(20) <<  "mdl_I85x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I85x33 << endl;
  cout << setw(20) <<  "mdl_I85x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I85x36 << endl;
  cout << setw(20) <<  "mdl_I86x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I86x33 << endl;
  cout << setw(20) <<  "mdl_I86x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I86x36 << endl;
  cout << setw(20) <<  "mdl_I88x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I88x33 << endl;
  cout << setw(20) <<  "mdl_I89x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I89x33 << endl;
  cout << setw(20) <<  "mdl_I89x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I89x36 << endl;
  cout << setw(20) <<  "mdl_I9x33 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I9x33 << endl;
  cout << setw(20) <<  "mdl_I9x36 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_I9x36 << endl;
  cout << setw(20) <<  "mdl_I90x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I90x33 << endl;
  cout << setw(20) <<  "mdl_I90x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I90x36 << endl;
  cout << setw(20) <<  "mdl_I91x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I91x33 << endl;
  cout << setw(20) <<  "mdl_I91x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I91x36 << endl;
  cout << setw(20) <<  "mdl_I92x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I92x33 << endl;
  cout << setw(20) <<  "mdl_I92x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I92x36 << endl;
  cout << setw(20) <<  "mdl_I92x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I92x63 << endl;
  cout << setw(20) <<  "mdl_I92x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I92x66 << endl;
  cout << setw(20) <<  "mdl_I93x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I93x33 << endl;
  cout << setw(20) <<  "mdl_I93x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I93x36 << endl;
  cout << setw(20) <<  "mdl_I94x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I94x33 << endl;
  cout << setw(20) <<  "mdl_I94x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I94x36 << endl;
  cout << setw(20) <<  "mdl_I94x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I94x63 << endl;
  cout << setw(20) <<  "mdl_I94x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I94x66 << endl;
  cout << setw(20) <<  "mdl_I95x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I95x33 << endl;
  cout << setw(20) <<  "mdl_I95x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I95x36 << endl;
  cout << setw(20) <<  "mdl_I96x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I96x33 << endl;
  cout << setw(20) <<  "mdl_I96x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I96x36 << endl;
  cout << setw(20) <<  "mdl_I96x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I96x63 << endl;
  cout << setw(20) <<  "mdl_I96x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I96x66 << endl;
  cout << setw(20) <<  "mdl_I97x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I97x33 << endl;
  cout << setw(20) <<  "mdl_I97x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I97x36 << endl;
  cout << setw(20) <<  "mdl_I97x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I97x63 << endl;
  cout << setw(20) <<  "mdl_I97x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I97x66 << endl;
  cout << setw(20) <<  "mdl_I98x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I98x33 << endl;
  cout << setw(20) <<  "mdl_I98x36 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I98x36 << endl;
  cout << setw(20) <<  "mdl_I98x63 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I98x63 << endl;
  cout << setw(20) <<  "mdl_I98x66 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I98x66 << endl;
  cout << setw(20) <<  "mdl_I99x33 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_I99x33 << endl;
  cout << setw(20) <<  "mdl_complexi " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_complexi << endl;
  cout << setw(20) <<  "mdl_sqrt__2 " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_sqrt__2 << endl;
  cout << setw(20) <<  "mdl_sw__exp__2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_sw__exp__2 << endl;
  cout << setw(20) <<  "mdl_conjg__NN1x1 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN1x1 << endl;
  cout << setw(20) <<  "mdl_conjg__NN1x2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN1x2 << endl;
  cout << setw(20) <<  "mdl_conjg__NN1x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN1x3 << endl;
  cout << setw(20) <<  "mdl_conjg__NN1x4 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN1x4 << endl;
  cout << setw(20) <<  "mdl_conjg__NN2x1 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN2x1 << endl;
  cout << setw(20) <<  "mdl_conjg__NN2x2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN2x2 << endl;
  cout << setw(20) <<  "mdl_conjg__NN2x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN2x3 << endl;
  cout << setw(20) <<  "mdl_conjg__NN2x4 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN2x4 << endl;
  cout << setw(20) <<  "mdl_conjg__NN3x1 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN3x1 << endl;
  cout << setw(20) <<  "mdl_conjg__NN3x2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN3x2 << endl;
  cout << setw(20) <<  "mdl_conjg__NN3x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN3x3 << endl;
  cout << setw(20) <<  "mdl_conjg__NN3x4 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN3x4 << endl;
  cout << setw(20) <<  "mdl_conjg__NN4x1 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN4x1 << endl;
  cout << setw(20) <<  "mdl_conjg__NN4x2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN4x2 << endl;
  cout << setw(20) <<  "mdl_conjg__NN4x3 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN4x3 << endl;
  cout << setw(20) <<  "mdl_conjg__NN4x4 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__NN4x4 << endl;
  cout << setw(20) <<  "mdl_conjg__UU1x1 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__UU1x1 << endl;
  cout << setw(20) <<  "mdl_conjg__UU1x2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__UU1x2 << endl;
  cout << setw(20) <<  "mdl_conjg__UU2x1 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__UU2x1 << endl;
  cout << setw(20) <<  "mdl_conjg__UU2x2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__UU2x2 << endl;
  cout << setw(20) <<  "mdl_conjg__VV1x1 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__VV1x1 << endl;
  cout << setw(20) <<  "mdl_conjg__VV1x2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__VV1x2 << endl;
  cout << setw(20) <<  "mdl_conjg__VV2x1 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__VV2x1 << endl;
  cout << setw(20) <<  "mdl_conjg__VV2x2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__VV2x2 << endl;
  cout << setw(20) <<  "mdl_cos__alp " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_cos__alp << endl;
  cout << setw(20) <<  "mdl_sin__alp " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_sin__alp << endl;
  cout << setw(20) <<  "mdl_conjg__MUH " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_conjg__MUH << endl;
  cout << setw(20) <<  "mdl_ee " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_ee << endl;
  cout << setw(20) <<  "mdl_gp " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_gp << endl;
  cout << setw(20) <<  "mdl_gw " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_gw << endl;
  cout << setw(20) <<  "mdl_vev " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_vev << endl;
  cout << setw(20) <<  "mdl_vd " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_vd << endl;
  cout << setw(20) <<  "mdl_vu " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << mdl_vu << endl;
  cout << setw(20) <<  "mdl_ee__exp__2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_ee__exp__2 << endl;
}
void pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2::printIndependentCouplings()
{
  cout <<  "MSSM_SLHA2 model couplings independent of event kinematics:" <<
      endl;

}
void pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2::printDependentParameters()
{
  cout <<  "MSSM_SLHA2 model parameters dependent on event kinematics:" <<
      endl;
  cout << setw(20) <<  "mdl_sqrt__aS " <<  "= " << setiosflags(ios::scientific)
      << setw(10) << mdl_sqrt__aS << endl;
  cout << setw(20) <<  "G " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << G << endl;
  cout << setw(20) <<  "mdl_G__exp__2 " <<  "= " <<
      setiosflags(ios::scientific) << setw(10) << mdl_G__exp__2 << endl;
}
void pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2::printDependentCouplings()
{
  cout <<  "MSSM_SLHA2 model couplings dependent on event kinematics:" << endl; 
  cout << setw(20) <<  "GC_53 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << GC_53 << endl;
  cout << setw(20) <<  "GC_96 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << GC_96 << endl;
  cout << setw(20) <<  "GC_7 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << GC_7 << endl;
  cout << setw(20) <<  "GC_6 " <<  "= " << setiosflags(ios::scientific) <<
      setw(10) << GC_6 << endl;
}


