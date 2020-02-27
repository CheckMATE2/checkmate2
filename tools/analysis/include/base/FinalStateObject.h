#ifndef _FINALSTATEOBJECT
#define _FINALSTATEOBJECT

#include <stdio.h>
#include <TROOT.h>

#include "classes/DelphesClasses.h"

#include "ETMiss.h"
//! A class to combine objects of different types
/** This class is able to combine electrons, muons, jets and etmiss into one vector
 * in case one wants to perform analysis steps that in fact do not distinguish between
 * those. It saves PT, Eta, Phi, Charge (0 for etmiss), a function P4() that refers to a full TLorentzVector and
 * a string "Type" which is either "electron", "muon", "jet" or "etmiss".x
 */
class FinalStateObject {
    public:
        //! Constructor functions to cast object into general FinalStateObject.
        /** The constructor needs an object to be put in which is either of type Electron, Muon, Jet or ETMiss.
         *  The members are then taken from the repectie member of the class.
         */
        FinalStateObject(Electron* e) : PT(e->PT), Eta(e->Eta), Phi(e->Phi), Charge(e->Charge), Type("electron"), fourvector(e->P4()) { };
        FinalStateObject(Muon* m) : PT(m->PT), Eta(m->Eta), Phi(m->Phi), Charge(m->Charge), Type("muon"), fourvector(m->P4()) { };
        FinalStateObject(Jet* j) : PT(j->PT), Eta(j->Eta), Phi(j->Phi), Charge(j->Charge), Type("jet"), fourvector(j->P4()) { };
        FinalStateObject(ETMiss* etm) : PT(etm->PT), Eta(etm->Eta), Phi(etm->Phi), Charge(0), Type("etmiss"), fourvector(etm->P4()) { };        
        //! Standard Destructor.
       ~FinalStateObject() {};
        
        Float_t PT; //!< Transverse Momentum
        Float_t Eta; //!< Pseudorapidity
        Float_t Phi; //!< Polar angle
        Float_t Charge; //!< Charge (0 for EtMiss)
        std::string Type; //!< 'electron', 'muon', 'jet' or 'etmiss'
        inline TLorentzVector P4() { return fourvector;} //!< returns a TLorentzVector of the full 4 momentum.
    private:
        TLorentzVector fourvector;
        
    public:    
        static bool sortByPT(FinalStateObject *i, FinalStateObject *j);
        inline bool operator==( const FinalStateObject& rhs){
        	return this->PT == rhs.PT && this->Eta == rhs.Eta && this->Phi == rhs.Phi && this->Charge == rhs.Charge && this->Type == rhs.Type;
        }
};

#endif
