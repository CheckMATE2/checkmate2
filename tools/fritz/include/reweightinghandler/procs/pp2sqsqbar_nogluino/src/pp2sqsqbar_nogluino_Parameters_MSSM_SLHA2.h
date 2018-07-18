//==========================================================================
// This file has been automatically generated for C++
// MadGraph5_aMC@NLO v. 2.5.1, 2016-11-04
// By the MadGraph5_aMC@NLO Development Team
// Visit launchpad.net/madgraph5 and amcatnlo.web.cern.ch
//==========================================================================

#ifndef PP2SQSQBAR_NOGLUINO_Parameters_MSSM_SLHA2_H
#define PP2SQSQBAR_NOGLUINO_Parameters_MSSM_SLHA2_H

#include <complex> 

#include "../../../read_slha.h"
using namespace std; 

class pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2
{
  public:

    static pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2 * getInstance(); 

    // Define "zero"
    double zero, ZERO; 
    // Model parameters independent of aS
    double mdl_Wsl6, mdl_Wsl5, mdl_Wsl4, mdl_Wsu6, mdl_Wsd6, mdl_Wsu5,
        mdl_Wsd5, mdl_Wsu4, mdl_Wsd4, mdl_Wch2, mdl_Wneu4, mdl_Wneu3, mdl_Wch1,
        mdl_Wneu2, mdl_Wgo, mdl_Wsn3, mdl_Wsl3, mdl_Wsn2, mdl_Wsl2, mdl_Wsn1,
        mdl_Wsl1, mdl_Wsu3, mdl_Wsd3, mdl_Wsu2, mdl_Wsd2, mdl_Wsu1, mdl_Wsd1,
        mdl_WH, mdl_WA0, mdl_WH02, mdl_WH01, mdl_WW, mdl_WZ, mdl_WT,
        mdl_Ryu3x3, mdl_Rye3x3, mdl_Ryd3x3, mdl_RVV2x2, mdl_RVV2x1, mdl_RVV1x2,
        mdl_RVV1x1, mdl_RCKM3x3, mdl_RCKM2x2, mdl_RCKM1x1, mdl_RRu6x6,
        mdl_RRu6x3, mdl_RRu5x5, mdl_RRu4x4, mdl_RRu3x6, mdl_RRu3x3, mdl_RRu2x2,
        mdl_RRu1x1, mdl_RMNS3x3, mdl_RMNS2x2, mdl_RMNS1x1, mdl_RUU2x2,
        mdl_RUU2x1, mdl_RUU1x2, mdl_RUU1x1, mdl_Rtu3x3, mdl_Rte3x3, mdl_Rtd3x3,
        mdl_RRn3x3, mdl_RRn2x2, mdl_RRn1x1, aS, aEWM1, mdl_RRl6x6, mdl_RRl6x3,
        mdl_RRl5x5, mdl_RRl4x4, mdl_RRl3x6, mdl_RRl3x3, mdl_RRl2x2, mdl_RRl1x1,
        mdl_RNN4x4, mdl_RNN4x3, mdl_RNN4x2, mdl_RNN4x1, mdl_RNN3x4, mdl_RNN3x3,
        mdl_RNN3x2, mdl_RNN3x1, mdl_RNN2x4, mdl_RNN2x3, mdl_RNN2x2, mdl_RNN2x1,
        mdl_RNN1x4, mdl_RNN1x3, mdl_RNN1x2, mdl_RNN1x1, mdl_RmU23x3,
        mdl_RmU21x1, mdl_RmQ23x3, mdl_RmQ21x1, mdl_mHu2, mdl_mHd2, mdl_RMx3,
        mdl_RMx2, mdl_RMx1, mdl_RmL23x3, mdl_RmL21x1, mdl_RmE23x3, mdl_RmE21x1,
        mdl_RmD23x3, mdl_RmD21x1, mdl_Msl6, mdl_Msl4, mdl_Msu6, mdl_Msd6,
        mdl_Msu4, mdl_Msd4, mdl_Mch2, mdl_Mneu4, mdl_Mneu3, mdl_Mch1,
        mdl_Mneu2, mdl_Mneu1, mdl_Mgo, mdl_Msn3, mdl_Msl3, mdl_Msn1, mdl_Msl1,
        mdl_Msu3, mdl_Msd3, mdl_Msu1, mdl_Msd1, mdl_MH, mdl_MA0, mdl_MH02,
        mdl_MH01, mdl_MW, mdl_MZ, mdl_Mta, mdl_MT, mdl_MB, mdl_MA2, mdl_tb,
        mdl_RMUH, mdl_alp, mdl_RRd6x6, mdl_RRd6x3, mdl_RRd5x5, mdl_RRd4x4,
        mdl_RRd3x6, mdl_RRd3x3, mdl_RRd2x2, mdl_RRd1x1, mdl_Msu5, mdl_Msu2,
        mdl_RmD22x2, mdl_Msl2, mdl_Msl5, mdl_RmL22x2, mdl_Msd2, mdl_RmQ22x2,
        mdl_Msd5, mdl_RmU22x2, mdl_Msn2, mdl_RmE22x2, mdl_conjg__CKM3x3,
        mdl_I93x22, mdl_I97x11, mdl_I43x22, mdl_I51x11, mdl_I75x55, mdl_I92x22,
        mdl_I96x11, mdl_I5x11, mdl_I100x44, mdl_conjg__Rn3x3, mdl_I45x11,
        mdl_I75x44, mdl_I25x22, mdl_I52x44, mdl_conjg__Ru1x1, mdl_I32x44,
        mdl_conjg__Ru4x4, mdl_I6x44, mdl_I53x11, mdl_I74x11, mdl_conjg__Rd5x5,
        mdl_conjg__Rl1x1, mdl_I5x22, mdl_I101x55, mdl_I102x44, mdl_I52x55,
        mdl_I26x44, mdl_conjg__Ru2x2, mdl_I26x55, mdl_I96x22, mdl_I13x44,
        mdl_I51x22, mdl_I89x11, mdl_I98x11, mdl_I100x55, mdl_Ru5x5, mdl_Rl1x1,
        mdl_conjg__Rl4x4, mdl_Ru2x2, mdl_Ru4x4, mdl_conjg__Rd4x4, mdl_Rd1x1,
        mdl_I7x22, mdl_I25x11, mdl_I6x55, mdl_CKM3x3, mdl_Rn3x3,
        mdl_conjg__Rd1x1, mdl_I85x22, mdl_conjg__Rn2x2, mdl_I39x11,
        mdl_I101x44, mdl_I102x55, mdl_atan__tb, mdl_beta, mdl_cw,
        mdl_MZ__exp__2, mdl_cw__exp__2, mdl_sw, mdl_cos__beta, mdl_sin__beta,
        mdl_sqrt__2, mdl_sw__exp__2, mdl_cos__alp, mdl_sin__alp, mdl_ee,
        mdl_gp, mdl_gw, mdl_vev, mdl_vd, mdl_vu, mdl_ee__exp__2;
    std::complex<double> mdl_mD21x1, mdl_mD22x2, mdl_mD23x3, mdl_mE21x1,
        mdl_mE22x2, mdl_mE23x3, mdl_mL21x1, mdl_mL22x2, mdl_mL23x3, mdl_mQ21x1,
        mdl_mQ22x2, mdl_mQ23x3, mdl_mU21x1, mdl_mU22x2, mdl_mU23x3, mdl_MUH,
        mdl_Mx1, mdl_Mx2, mdl_Mx3, mdl_NN1x1, mdl_NN1x2, mdl_NN1x3, mdl_NN1x4,
        mdl_NN2x1, mdl_NN2x2, mdl_NN2x3, mdl_NN2x4, mdl_NN3x1, mdl_NN3x2,
        mdl_NN3x3, mdl_NN3x4, mdl_NN4x1, mdl_NN4x2, mdl_NN4x3, mdl_NN4x4,
        mdl_Rd3x3, mdl_Rd3x6, mdl_Rd6x3, mdl_Rd6x6, mdl_Rl3x3, mdl_Rl3x6,
        mdl_Rl6x3, mdl_Rl6x6, mdl_Ru3x3, mdl_Ru3x6, mdl_Ru6x3, mdl_Ru6x6,
        mdl_UU1x1, mdl_UU1x2, mdl_UU2x1, mdl_UU2x2, mdl_VV1x1, mdl_VV1x2,
        mdl_VV2x1, mdl_VV2x2, mdl_td3x3, mdl_te3x3, mdl_tu3x3, mdl_yd3x3,
        mdl_ye3x3, mdl_yu3x3, mdl_bb, mdl_conjg__yu3x3, mdl_I1x33,
        mdl_conjg__yd3x3, mdl_I10x33, mdl_I10x36, mdl_conjg__Rd3x6,
        mdl_I100x33, mdl_I100x36, mdl_conjg__Rd6x6, mdl_I100x63, mdl_I100x66,
        mdl_conjg__Rl3x6, mdl_I101x33, mdl_I101x36, mdl_conjg__Rl6x6,
        mdl_I101x63, mdl_I101x66, mdl_conjg__Ru3x6, mdl_I102x33, mdl_I102x36,
        mdl_conjg__Ru6x6, mdl_I102x63, mdl_I102x66, mdl_I11x33, mdl_I11x36,
        mdl_conjg__Rd3x3, mdl_I12x33, mdl_I12x36, mdl_conjg__Rd6x3, mdl_I12x63,
        mdl_I12x66, mdl_I13x33, mdl_I13x36, mdl_I13x63, mdl_I13x66,
        mdl_conjg__td3x3, mdl_I14x33, mdl_I14x36, mdl_I14x63, mdl_I14x66,
        mdl_I15x33, mdl_I15x36, mdl_I15x63, mdl_I15x66, mdl_I16x33, mdl_I16x36,
        mdl_I16x63, mdl_I16x66, mdl_I17x33, mdl_I17x36, mdl_I17x63, mdl_I17x66,
        mdl_I18x33, mdl_I18x36, mdl_I18x63, mdl_I18x66, mdl_I19x33, mdl_I19x36,
        mdl_I19x63, mdl_I19x66, mdl_I2x33, mdl_I20x33, mdl_I21x33,
        mdl_conjg__ye3x3, mdl_I22x33, mdl_I23x33, mdl_I23x36, mdl_conjg__Rl3x3,
        mdl_I24x33, mdl_conjg__Rl6x3, mdl_I24x36, mdl_I25x33, mdl_I25x36,
        mdl_I25x63, mdl_I25x66, mdl_I26x33, mdl_I26x36, mdl_I26x63, mdl_I26x66,
        mdl_I27x33, mdl_I27x36, mdl_I28x33, mdl_I28x36, mdl_I29x33, mdl_I29x36,
        mdl_I3x33, mdl_I3x36, mdl_I30x33, mdl_I30x36, mdl_I31x33, mdl_I31x36,
        mdl_I31x63, mdl_I31x66, mdl_I32x33, mdl_I32x36, mdl_I32x63, mdl_I32x66,
        mdl_conjg__te3x3, mdl_I33x33, mdl_I33x36, mdl_I33x63, mdl_I33x66,
        mdl_I34x33, mdl_I34x36, mdl_I34x63, mdl_I34x66, mdl_I35x33, mdl_I35x36,
        mdl_I35x63, mdl_I35x66, mdl_I36x33, mdl_I36x36, mdl_I36x63, mdl_I36x66,
        mdl_I37x33, mdl_I37x36, mdl_I37x63, mdl_I37x66, mdl_I38x33, mdl_I38x36,
        mdl_I38x63, mdl_I38x66, mdl_I39x33, mdl_I39x36, mdl_I4x33, mdl_I4x36,
        mdl_I40x33, mdl_I40x36, mdl_I41x33, mdl_I41x36, mdl_I42x33, mdl_I42x36,
        mdl_I44x33, mdl_I45x33, mdl_I45x36, mdl_I46x33, mdl_I46x36, mdl_I47x33,
        mdl_I47x36, mdl_I48x33, mdl_I48x36, mdl_I49x33, mdl_I49x36, mdl_I5x33,
        mdl_I5x36, mdl_I5x63, mdl_I5x66, mdl_conjg__Ru3x3, mdl_I50x33,
        mdl_conjg__Ru6x3, mdl_I50x36, mdl_I51x33, mdl_I51x36, mdl_I51x63,
        mdl_I51x66, mdl_I52x33, mdl_I52x36, mdl_I52x63, mdl_I52x66, mdl_I53x33,
        mdl_I53x36, mdl_I53x63, mdl_I53x66, mdl_conjg__tu3x3, mdl_I54x33,
        mdl_I54x36, mdl_I54x63, mdl_I54x66, mdl_I55x33, mdl_I55x36, mdl_I55x63,
        mdl_I55x66, mdl_I56x33, mdl_I56x36, mdl_I56x63, mdl_I56x66, mdl_I57x33,
        mdl_I57x36, mdl_I57x63, mdl_I57x66, mdl_I58x33, mdl_I58x36, mdl_I58x63,
        mdl_I58x66, mdl_I59x33, mdl_I59x36, mdl_I59x63, mdl_I59x66, mdl_I6x33,
        mdl_I6x36, mdl_I6x63, mdl_I6x66, mdl_I60x33, mdl_I60x36, mdl_I60x63,
        mdl_I60x66, mdl_I61x33, mdl_I61x36, mdl_I62x33, mdl_I62x36, mdl_I63x33,
        mdl_I63x36, mdl_I64x33, mdl_I64x36, mdl_I65x33, mdl_I65x36, mdl_I66x33,
        mdl_I66x36, mdl_I66x63, mdl_I66x66, mdl_I67x33, mdl_I67x36, mdl_I67x63,
        mdl_I67x66, mdl_I68x33, mdl_I68x36, mdl_I68x63, mdl_I68x66, mdl_I69x33,
        mdl_I69x36, mdl_I69x63, mdl_I69x66, mdl_I7x33, mdl_I7x36, mdl_I70x33,
        mdl_I70x36, mdl_I70x63, mdl_I70x66, mdl_I71x33, mdl_I71x36, mdl_I71x63,
        mdl_I71x66, mdl_I72x33, mdl_I72x36, mdl_I72x63, mdl_I72x66, mdl_I73x33,
        mdl_I73x36, mdl_I73x63, mdl_I73x66, mdl_I74x33, mdl_I74x36, mdl_I74x63,
        mdl_I74x66, mdl_I75x33, mdl_I75x36, mdl_I75x63, mdl_I75x66, mdl_I76x33,
        mdl_I76x36, mdl_I76x63, mdl_I76x66, mdl_I77x33, mdl_I77x36, mdl_I77x63,
        mdl_I77x66, mdl_I78x33, mdl_I78x36, mdl_I78x63, mdl_I78x66, mdl_I79x33,
        mdl_I79x36, mdl_I79x63, mdl_I79x66, mdl_I8x33, mdl_I8x36, mdl_I80x33,
        mdl_I80x36, mdl_I80x63, mdl_I80x66, mdl_I81x33, mdl_I81x36, mdl_I81x63,
        mdl_I81x66, mdl_I82x33, mdl_I82x36, mdl_I83x33, mdl_I83x36, mdl_I84x33,
        mdl_I84x36, mdl_I85x33, mdl_I85x36, mdl_I86x33, mdl_I86x36, mdl_I88x33,
        mdl_I89x33, mdl_I89x36, mdl_I9x33, mdl_I9x36, mdl_I90x33, mdl_I90x36,
        mdl_I91x33, mdl_I91x36, mdl_I92x33, mdl_I92x36, mdl_I92x63, mdl_I92x66,
        mdl_I93x33, mdl_I93x36, mdl_I94x33, mdl_I94x36, mdl_I94x63, mdl_I94x66,
        mdl_I95x33, mdl_I95x36, mdl_I96x33, mdl_I96x36, mdl_I96x63, mdl_I96x66,
        mdl_I97x33, mdl_I97x36, mdl_I97x63, mdl_I97x66, mdl_I98x33, mdl_I98x36,
        mdl_I98x63, mdl_I98x66, mdl_I99x33, mdl_complexi, mdl_conjg__NN1x1,
        mdl_conjg__NN1x2, mdl_conjg__NN1x3, mdl_conjg__NN1x4, mdl_conjg__NN2x1,
        mdl_conjg__NN2x2, mdl_conjg__NN2x3, mdl_conjg__NN2x4, mdl_conjg__NN3x1,
        mdl_conjg__NN3x2, mdl_conjg__NN3x3, mdl_conjg__NN3x4, mdl_conjg__NN4x1,
        mdl_conjg__NN4x2, mdl_conjg__NN4x3, mdl_conjg__NN4x4, mdl_conjg__UU1x1,
        mdl_conjg__UU1x2, mdl_conjg__UU2x1, mdl_conjg__UU2x2, mdl_conjg__VV1x1,
        mdl_conjg__VV1x2, mdl_conjg__VV2x1, mdl_conjg__VV2x2, mdl_conjg__MUH;
    // Model parameters dependent on aS
    double mdl_sqrt__aS, G; 
    std::complex<double> mdl_G__exp__2; 
    // Model couplings independent of aS

    // Model couplings dependent on aS
    std::complex<double> GC_53, GC_96, GC_7, GC_6; 

    // Set parameters that are unchanged during the run
    void setIndependentParameters(SLHAReader& slha); 
    // Set couplings that are unchanged during the run
    void setIndependentCouplings(); 
    // Set parameters that are changed event by event
    void setDependentParameters(); 
    // Set couplings that are changed event by event
    void setDependentCouplings(); 

    // Print parameters that are unchanged during the run
    void printIndependentParameters(); 
    // Print couplings that are unchanged during the run
    void printIndependentCouplings(); 
    // Print parameters that are changed event by event
    void printDependentParameters(); 
    // Print couplings that are changed event by event
    void printDependentCouplings(); 


  private:
    static pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2 * instance; 
}; 

#endif  // PP2SQSQBAR_NOGLUINO_Parameters_MSSM_SLHA2_H

