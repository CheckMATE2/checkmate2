//==========================================================================
// This file has been automatically generated for C++ Standalone by
// MadGraph5_aMC@NLO v. 2.5.1, 2016-11-04
// By the MadGraph5_aMC@NLO Development Team
// Visit launchpad.net/madgraph5 and amcatnlo.web.cern.ch
//==========================================================================

#ifndef PP2SQSQBAR_NOGLUINO_MG5_Sigma_MSSM_SLHA2_UUX_SLSLX_H
#define PP2SQSQBAR_NOGLUINO_MG5_Sigma_MSSM_SLHA2_UUX_SLSLX_H


#include "../../src/pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2.h"

using namespace std; 

//==========================================================================
// A class for calculating the matrix elements for
// Process: u u~ > sl sl~ WEIGHTED<=2 / go @1
// Process: c c~ > sl sl~ WEIGHTED<=2 / go @1
// Process: d d~ > sl sl~ WEIGHTED<=2 / go @1
// Process: s s~ > sl sl~ WEIGHTED<=2 / go @1
//--------------------------------------------------------------------------

class PP2SQSQBAR_NOGLUINO_UUX_SLSLX_CPPProcess
{
  public:

    // Constructor.
    PP2SQSQBAR_NOGLUINO_UUX_SLSLX_CPPProcess() {}

    // Initialize process.
    virtual void initProc(string param_card_name); 

    // Calculate flavour-independent parts of cross section.
    virtual void sigmaKin(); 

    // Evaluate sigmaHat(sHat).
    virtual double sigmaHat(); 

    // Info on the subprocess.
    virtual string name() const {return "u u~ > sl sl~ (MSSM_SLHA2)";}

    virtual int code() const {return 1;}

    const vector<double> & getMasses() const {return mME;}

    // Get and set momenta for matrix element evaluation
    vector < double * > getMomenta(){return p;}
    void setMomenta(vector < double * > & momenta){p = momenta;}
    void setInitial(int inid1, int inid2){id1 = inid1; id2 = inid2;}

    // Get matrix element vector
    const double * getMatrixElements() const {return matrix_element;}

    // Constants for array limits
    static const int ninitial = 2; 
    static const int nexternal = 4; 
    static const int nprocesses = 2; 

  private:

    // Private functions to calculate the matrix element for all subprocesses
    // Calculate wavefunctions
    void calculate_wavefunctions(const int perm[], const int hel[]); 
    static const int nwavefuncs = 5; 
    std::complex<double> w[nwavefuncs][18]; 
    static const int namplitudes = 1; 
    std::complex<double> amp[namplitudes]; 
    double matrix_1_uux_slslx_no_go(); 

    // Store the matrix element value from sigmaKin
    double matrix_element[nprocesses]; 

    // Color flows, used when selecting color
    double * jamp2[nprocesses]; 

    // Pointer to the model parameters
    pp2sqsqbar_nogluino_Parameters_MSSM_SLHA2 * pars; 

    // vector with external particle masses
    vector<double> mME; 

    // vector with momenta (to be changed each event)
    vector < double * > p; 
    // Initial particle ids
    int id1, id2; 

}; 


#endif  // MG5_Sigma_MSSM_SLHA2_uux_slslx_H
