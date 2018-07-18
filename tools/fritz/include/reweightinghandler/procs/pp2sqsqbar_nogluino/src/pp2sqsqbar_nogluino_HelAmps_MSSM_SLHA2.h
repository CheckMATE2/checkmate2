//==========================================================================
// This file has been automatically generated for C++ Standalone
// MadGraph5_aMC@NLO v. 2.5.1, 2016-11-04
// By the MadGraph5_aMC@NLO Development Team
// Visit launchpad.net/madgraph5 and amcatnlo.web.cern.ch
//==========================================================================

#ifndef PP2SQSQBAR_NOGLUINO_HelAmps_MSSM_SLHA2_H
#define PP2SQSQBAR_NOGLUINO_HelAmps_MSSM_SLHA2_H

#include <cmath> 
#include <complex> 

using namespace std; 

namespace PP2SQSQBAR_NOGLUINO_MG5_MSSM_SLHA2 
{
void ixxxxx(double p[4], double fmass, int nhel, int nsf, std::complex<double>
    fi[6]);

void txxxxx(double p[4], double tmass, int nhel, int nst, std::complex<double>
    fi[18]);

void vxxxxx(double p[4], double vmass, int nhel, int nsv, std::complex<double>
    v[6]);

void oxxxxx(double p[4], double fmass, int nhel, int nsf, std::complex<double>
    fo[6]);

void sxxxxx(double p[4], int nss, std::complex<double> sc[3]); 

double Sgn(double e, double f); 

void FFV1P0_3(std::complex<double> F1[], std::complex<double> F2[],
    std::complex<double> COUP, double M3, double W3, std::complex<double> V3[]);

void VVSS1_0(std::complex<double> V1[], std::complex<double> V2[],
    std::complex<double> S3[], std::complex<double> S4[], std::complex<double>
    COUP, std::complex<double> & vertex);

void VSS1_0(std::complex<double> V1[], std::complex<double> S2[],
    std::complex<double> S3[], std::complex<double> COUP, std::complex<double>
    & vertex);

void VSS1_2(std::complex<double> V1[], std::complex<double> S3[],
    std::complex<double> COUP, double M2, double W2, std::complex<double> S2[]);

void VVV1P0_1(std::complex<double> V2[], std::complex<double> V3[],
    std::complex<double> COUP, double M1, double W1, std::complex<double> V1[]);

void VSS1_3(std::complex<double> V1[], std::complex<double> S2[],
    std::complex<double> COUP, double M3, double W3, std::complex<double> S3[]);

}  // end namespace PP2SQSQBAR_NOGLUINO_MG5_MSSM_SLHA2

#endif  // PP2SQSQBAR_NOGLUINO_HelAmps_MSSM_SLHA2_H
