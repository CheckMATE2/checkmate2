#ifndef _ETMISS
#define _ETMISS

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <TObject.h>
#include <TROOT.h>

#include "classes/DelphesClasses.h"

//! A class to parametrise missingET.
/** This class serves as an intrinsic parametrisation of the missingET
  * vector, which is defined such that it has the same properties as the
  * other final state objects like electrons and jets. Furthermore,
  * the user can decide which class of muons he wants to take into account
  * for the total missing energy vector.
  */
class ETMiss {
 public:
   //! Constructor
   /** The constructor takes a missing ET object as it is defined
    *  within Delphes. The energy is then automatically smeared 
    *  by adding a vector with random magnitude Gauss(m=20 GeV, s = 1 GeV)
    *  [chosen empirically] and uniformly random direction.
    */
    ETMiss(ETMiss* x) {
        PT = x->PT;
        Eta = x->Eta;
        Phi = x->Phi;
        content = x->content;
    }
    ETMiss(MissingET* met) {
        double missingET_ET = met->MET;
        double missingET_Phi = met->Phi;
        double missingET_Ex = missingET_ET*cos(missingET_Phi);
        double missingET_Ey = missingET_ET*sin(missingET_Phi);
        
        // Smearing of ET due to pile up set at 20GeV
        double deltaPT = 20.0;
        
        double x = rand() /(RAND_MAX+1.);
        double y = rand() /(RAND_MAX+1.);
        double uni_gauss = sqrt(-2.*log(x))*cos(2.*3.1415*y);
        double uni_normal = rand() / (RAND_MAX+1.);

        double smear_x = deltaPT*uni_gauss*cos(2.*3.1415*uni_normal);
        double smear_y = deltaPT*uni_gauss*sin(2.*3.1415*uni_normal);

        double new_ET = sqrt(pow(missingET_Ex+smear_x, 2)+pow(missingET_Ey+smear_y, 2));
        content.SetPxPyPzE(missingET_Ex+smear_x, missingET_Ey+smear_y, 0, new_ET);
        PT = content.Pt();
        Eta = content.Eta();
        Phi = content.Phi();    
    }
    
    //! Add muons to missingET calculation.
    /** Since there is no unique way to define how muons are taken into account
     *  in the missingET calculation, thy have to be added manually by the user. 
     *  With this general function, one just has to declare which set of muons
     * one wants to take into account.
     *  \param muons A std::vector of Muon objects that should be added.
     */
    void addMuons(std::vector<Muon*> muons) {
        TLorentzVector sum = TLorentzVector(0,0,0,0);
        for(int i = 0; i < muons.size(); i++)
            sum += muons[i]->P4();
      
        double new_ET = sqrt(pow(content.Px()-sum.Px(), 2)+pow(content.Py()-sum.Py(), 2));
        content.SetPxPyPzE(content.Px()-sum.Px(), content.Py()-sum.Py(), 0, new_ET);
        PT = content.Pt();
        Eta = content.Eta();
        Phi = content.Phi();
    }
    
    Float_t PT; //!< Transverse momentum
    Float_t Eta; //!< Pseudorapidity
    Float_t Phi; //!< Polar angle
    TLorentzVector P4() { return content; } //!< Returns a TLorentzVector object to the full 4 momentum.

 private:
    TLorentzVector content;
};

#endif
