#ifndef _LLPVERTEX
#define _LLPVERTEX

#include <stdio.h>
#include <assert.h> 
#include <TROOT.h>

#include "classes/DelphesClasses.h"

//! A class to describe LLP Vertices
/** This class stores information for each true gen particle with a long lifetime and stores its lifetime/position and links to its mother and its daughters.
 */
class LLPVertex {
 public:
    //! Constructor functions to cast object into general FinalStateObject.
    /** The constructor needs an object to be put in which is either of type Electron, Muon, Jet or ETMiss.
     *  The members are then taken from the repectie member of the class.
     */
    LLPVertex(GenParticle* m);
        
    //! Standard Destructor.
    ~LLPVertex() {};

    GenParticle* mother;
    std::vector<GenParticle*> tracks; //!< All charged particles in the decay tree
    std::vector<GenParticle*> vertexDaughters; //!< All daughters associated to the decay vertex
    std::vector<GenParticle*> stableDaughters; //!< All stable daughters in the decay tree

    float X;
    float Y;
    float Z;
    float T;
        
 public:    
    void fillDaughters(std::vector<GenParticle*> LLPDecays); //!< fills tracks and daughters with full delphes info
    inline bool operator==( const LLPVertex& rhs){
	return this->mother == rhs.mother;
    }

 private:
    void storeCandidate(GenParticle* candidate, std::vector<GenParticle*> LLPDecays); 
	
};

#endif
