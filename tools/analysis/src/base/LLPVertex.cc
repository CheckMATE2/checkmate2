#include "LLPVertex.h"

LLPVertex::LLPVertex(GenParticle* m) {
    mother = m;
}

void LLPVertex::fillDaughters(std::vector<GenParticle*> LLPDecays) {
    assert(mother->D1 < LLPDecays.size());	
    GenParticle* d = LLPDecays[mother->D1];
    X = d->X;
    Y = d->Y;
    Z = d->Z;
    T = d->T;
       
    storeCandidate(mother, LLPDecays);
}

void LLPVertex::storeCandidate(GenParticle* candidate, std::vector<GenParticle*> llpdecays) {
    // All candidates which have the same vertex position as the 'mother' vertex ar stord as
    // vertexdaughters
    if (candidate->X == X && candidate->Y == Y && candidate->Z == Z)
	vertexDaughters.push_back(candidate);

    GenParticle* currPart1;
    GenParticle* currPart2;
    if (candidate->D1 >= 0) {
	currPart1 = llpdecays[candidate->D1];
	currPart2 = llpdecays[candidate->D2];
    }
    bool onePartDecay = candidate->D1 == candidate->D2;
    // dont store the mother candidat as "track" candidate!
    if (candidate != mother && candidate->Charge != 0)
	tracks.push_back(candidate);

    if (candidate->D1 >= 0)
	storeCandidate(currPart1, llpdecays);
    else // if there is no daughter, we have a stable particle
	stableDaughters.push_back(candidate);

    if (!onePartDecay && candidate->D2 >= 0)
	storeCandidate(currPart2, llpdecays);
}
