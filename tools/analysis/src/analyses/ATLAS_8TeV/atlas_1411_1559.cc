#include "atlas_1411_1559.h"
// AUTHOR: Cristina Marcos
//  EMAIL: mcristina.marcosm@gmail.com
void Atlas_1411_1559::initialize() {
  setAnalysisName("atlas_1411_1559");          
  setInformation(""
    "@#Search for new phenomena in events with a photon and missing transverse momentum in pp collisions\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SRTotal;");
bookCutflowRegions("SRa; SRb; SRc; SRd; SRe; SRf; SRg;");
}

void Atlas_1411_1559::analyze() {
missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

 // Jets are required to have transverse momentum pT > 30.0 GeV, abs(eta) < 4.5 and a distance to the closest preselected electron or photon of 0.2

  electrons = filterPhaseSpace(electrons, 7., -2.47, 2.47, true);
  muons = filterPhaseSpace(muons, 6., -2.5, 2.5, true );

   jets = filterPhaseSpace(jets, 30., -4.5, 4.5, true);

  jets = overlapRemoval(jets, photons, 0.2);
   jets = overlapRemoval(jets, electrons, 0.2);

  photons = overlapRemoval(photons,jets, 0.4);
  electrons= overlapRemoval(electrons,jets, 0.4);


   // ----------------- Pre-selected -----------------
   // We apply the cuts for the pre-selected (PS) events

   // ----------------- First PS cut: Trigger -----------------
   // This cut means that all events must satisfy a missing transverse energy cut of at least 80 GeV

   if (missingET->P4().Et() < 80.0) return;


   countCutflowEvent("PS1");               // We keep the number of events that pass the first PS cut

   // ----------------- SR CUTS -----------------
   // We apply the cuts for the signal regions (SR) events

   // ----------------- First SR cut: SRa -----------------
   // Events in the SR are required to have etmiss < 150.0 GeV

   	//if ( rand()/( RAND_MAX+1.) > 0.96)
	 // return;

   if (missingET->P4().Et() < 150.) return;

   countCutflowEvent("SRa");               // We keep the number of events in the first SR cut

   // ----------------- Second SR cut: SRb -----------------
   // Events in the SR are required to have at least one loose photon with pT > 125.0 (abs(eta) < 2.37)

   photons = filterPhaseSpace(photons, 125.0, -2.37, 2.37, true);

   if (photons.size()!=1) {
       return;
   }

   countCutflowEvent("SRb");               // We keep the number of events in the second SR cut

	if ( rand()/( RAND_MAX+1.) > 0.9 )
	  return;

   // ----------------- Third SR cut: SRc -----------------
   // The leading photon is tight with abs(eta) < 1.37

  photons = filterPhaseSpace(photons, 125.0, -1.37, 1.37, true);
   // photons = filterPhaseSpace(photons, 125.0, -1.0, 1.0, true);
   if (photons.size() !=1) {
       return;
   }

   countCutflowEvent("SRc");               // We keep the number of events in the third SR cut

   // ----------------- Fourth SR cut: SRd -----------------
   // The leading photon is isolated


   // We required that the photons pass all the conditions entered into the AnalysisManager

   photons = filterIsolation(photons);

   if (photons.size()<1) {
       return;
   }
   countCutflowEvent("SRd");          // We keep the number of events in the fourth SR cut

   // ----------------- Fifth SR cut: SRe -----------------
   // The leading photon and etmiss are not overlapped in azimuth

       if (fabs(missingET->P4().DeltaPhi(photons[0]->P4()))<0.4){
           return;
       }

   countCutflowEvent("SRe");           // We keep the number of events in the fifth SR cut

   // ----------------- Sixth SR cut: SRf -----------------
   // Jet veto: events with more than one jet are rejected and events with one jet deltaPhi<0,4 between the jet and the etmiss are rejected

   int nJets = jets.size();
   // Events with more than one jet are rejected
   if (nJets>1) return;
   // Events with one jet deltaPhi<0.4 between the jet and the etmiss are rejected

	if (nJets == 1){
       if (fabs(missingET->P4().DeltaPhi(jets[0]->P4()))<0.4) return;
}

   countCutflowEvent("SRf");       // We keep the number of events in the sixth SR cut

   // ----------------- Seventh SR cut: SRg -----------------
   // Lepton veto: we count the number of electrons and muons for each event. If the event has at least one lepton, we don't accept the event

   int nElectrons = electrons.size();
   int nMuons = muonsCombined.size();

   int nLeptons = nElectrons + nMuons;

   if (nLeptons != 0) {
       return;
   }

   countCutflowEvent("SRg");      // We keep the number of events in the seventh SR cut.

   //
	countSignalEvent("SRTotal");
}

void Atlas_1411_1559::finalize() {
  // Whatever should be done after the run goes here
}       
