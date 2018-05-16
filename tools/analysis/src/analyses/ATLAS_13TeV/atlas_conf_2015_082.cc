#include "atlas_conf_2015_082.h"
#include <stdlib.h>
#include <sstream>
// AUTHOR: Tim Keller
//  EMAIL: tim.keller@rwth-aachen.de


std::vector<Muon*> SpecialOverlapRemoval(std::vector<Muon*> candidates, std::vector<Jet*> neighbours, double dR) {
  if(neighbours.size() == 0)
    return candidates;
  std::vector<Muon*> passed_candidates;
  // Loop over candidates
  for(int i = 0; i < candidates.size(); i++) {
    bool overlap = false;
    // If a neighbour is too close, declare overlap, break and don't save candidate
    for(int j = 0; j < neighbours.size(); j++) {
      if (candidates[i]->P4().DeltaR(neighbours[j]->P4()) < dR+(10/(candidates[i]->PT))) {
        overlap = true;
        break;
      }
    }
    if (!overlap)
      passed_candidates.push_back(candidates[i]);
  }
  return passed_candidates;
  }
  

  
template<class T>
bool isolateLeptonT(T* lepton, std::vector<Track*>& tracks, double maxPT, double deltaR) {
    double pTsum = 0;
    for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
        if (lepton->Particle == (*it)->Particle) {
            continue;
        }
        if ((*it)->P4().DeltaR(lepton->P4()) > deltaR) {
            continue;
        }
        pTsum += (*it)->PT;
    }
    return pTsum < maxPT;
}

template<class T>
bool isolateLeptonC(T* lepton, std::vector<Tower*>& towers, double maxPT, double deltaR) {
    double pTsum = 0;
    for (std::vector<Tower*>::iterator it=towers.begin(); it!=towers.end(); it++) {
        if ((*it)->P4().DeltaR(lepton->P4()) > deltaR) {
            continue;
        }
        pTsum += (*it)->ET;
    }
    return pTsum - lepton->PT < maxPT;
}

template<class T>
std::vector<T*> isolateElectrons(std::vector<T*> leptons, std::vector<Track*>& tracks, std::vector<Tower*>& towers) {
    std::vector<T*> isolated;
    for (typename std::vector<T*>::iterator it=leptons.begin(); it!=leptons.end(); it++) {
        double dR = std::max(0.2, (double)(10/((*it)->PT)));
        if (isolateLeptonT(*it, tracks, 0.06*((*it)->PT), dR)  && isolateLeptonC(*it, towers, 0.06*((*it)->PT), 0.2)) {
            isolated.push_back(*it);
        }
    }
    return isolated;
}

template<class T>
std::vector<T*> isolateMuons(std::vector<T*> leptons, std::vector<Track*>& tracks) {
    std::vector<T*> isolated;
    for (typename std::vector<T*>::iterator it=leptons.begin(); it!=leptons.end(); it++) {
        double dR = std::max(0.3, (double)(10/((*it)->PT)));
        if (isolateLeptonT(*it, tracks, 0.06*((*it)->PT), dR)) {
            isolated.push_back(*it);
        }
    }
    return isolated;
}

void Atlas_conf_2015_082::initialize() {
  setAnalysisName("atlas_conf_2015_082");
  setInformation(""
    "# A search for supersymmetry in events containing a leptonically decaying Z boson, jets and missing transverse momentum in sqrt(s)=13TeV p p collisions with the ATLAS detector\n"
  "");
  setLuminosity(3.2*units::INVFB);
  bookSignalRegions("SRZ;");
  //bookCutFlowRegions("DilepttonCut;lepinvmass;dR;leadpt;subleadpt;oppositeflavour;samesign;dphi;dilepinvmass;ETMiss;HT;njets;");

  bookCutflowRegions("CR00_noCuts;CR01_leptonTriggerRequirement;\
CR02_rejectEventsWithBadJets;CR03_rejectEventsWithBadMuons;\
CR04_primaryVertexGE4tracks;CR05_vetoEventsWithCosmicMuons;\
CR06_GE2leptons;CR07_fakeMissET;\
\
CRee08_leptonIsolationTriggerMatching;CRee09_leptonFlavour;\
CRee10_promptLeptons;CRee11_oppositeChargeLeptons;\
CRee12_GT1jet;CRee13_electronCrackVeto;\
CRee14_mllGT15;CRee15_dPhi(leadingjet,missET)GT0.4;\
CRee16_dPhi(subleadingjet,missET)GT0.4;CRee17_mllIn(81,101);\
CRee18_HTGT600;CRee19_missETGT225;\
\
CRmm08_leptonIsolationTriggerMatching;CRmm09_leptonFlavour;\
CRmm10_promptLeptons;CRmm11_oppositeChargeLeptons;\
CRmm12_GT1jet;CRmm13_electronCrackVeto;\
CRmm14_mllGT15;CRmm15_dPhi(leadingjet,missET)GT0.4;\
CRmm16_dPhi(subleadingjet,missET)GT0.4;CRmm17_mllIn(81,101);\
CRmm18_HTGT600;CRmm19_missETGT225;Jet_hist_2;Jet_hist_3;Jet_hist_4;Jet_hist_5;Jet_hist_6;Jet_hist_7;Jet_hist_8;Jet_hist_9;Jet_hist_10;Jet_hist_11;Jet_hist_more");

  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}
  
  
void Atlas_conf_2015_082::analyze() {
  missingET->addMuons(muonsCombined);

  countCutflowEvent("CR00_noCuts");
  

  //Get b-Jets  
  std::vector<FinalStateObject*> bjets;
  for(int i=0;i<jets.size();i++){
    if(checkBTag(jets[i])){
      FinalStateObject* bj = newFinalStateObject(jets[i]);
      bjets.push_back(bj);
    }
  }

  
  //Filter Phase Space
  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47);
  electronsMedium = filterPhaseSpace(electronsMedium, 25., -2.47, 2.47);
  //electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47, true);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);

  //Overlap Removal
  electronsMedium = overlapRemoval(electronsMedium, bjets, 0.2);
  electronsLoose  = overlapRemoval(electronsLoose, bjets, 0.2);
  jets = overlapRemoval(jets, electronsLoose, 0.2);

  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  //electronsLoose  = overlapRemoval(electronsLoose, jets, 0.4);


  muonsCombined   = overlapRemoval(muonsCombined, bjets, 0.2);
  jets = overlapRemoval(jets, muonsCombined, 0.2);


  muonsCombined   = SpecialOverlapRemoval(muonsCombined, jets, 0.04);

  electronsMedium = overlapRemoval(electronsMedium, muonsCombined, 0.01);
  //electronsLoose  = overlapRemoval(electronsLoose, muonsCombined, 0.01);

  //for at least two baseline leptons
  if ( electronsMedium.size()+muonsCombined.size() < 2 ) return;
    countCutflowEvent("CR06_GE2leptons");
  
  std::vector<FinalStateObject*> signalLeptons;

  //My try to create a pt dependant Isolation(mentioned in the paper but not described). But It made everything worse
  //electronsMedium = isolateElectrons(electronsMedium,tracks,towers);
  
  electronsMedium = electronsMedium;
  for ( int i = 0; i < electronsMedium.size(); i++ ) {
    if ( electronsMedium[i]->P4().Et() >= 25. ) {
      FinalStateObject* lep = newFinalStateObject(electronsMedium[i]);
      signalLeptons.push_back(lep);
    }
  }

  //My try to create a pt dependant Isolation(mentioned in the paper but not described). But It made everything worse
  //std::vector<Muon*> muonsEtGE25 = isolateMuons(muonsCombined,tracks);

  std::vector<Muon*> muonsEtGE25 = muonsCombined;
  
  //std::vector<Muon*> muonsEtGE25 = filterIsolation(muonsCombined, 0);
  for ( int i = 0; i < muonsEtGE25.size(); i++ ) {
    if ( muonsEtGE25[i]->P4().Et() >= 25.) {
      FinalStateObject* lep = newFinalStateObject(muonsEtGE25[i]);
      signalLeptons.push_back(lep);
    }
  }

  std::vector<FinalStateObject*> jetsSignal;
  for ( int i = 0; i < jets.size(); i++ ) {
    if ( jets[i]->PT > 30. and abs(jets[i]->P4().Eta())<2.5 ){
      FinalStateObject* fso = newFinalStateObject(jets[i]);
      jetsSignal.push_back(fso);
    }
  }
  
  std::sort(signalLeptons.begin(), signalLeptons.end(), sortByPT);
  std::sort(jetsSignal.begin(), jetsSignal.end(), sortByPT);
  
  //for trigger and lepton flavour



  if( signalLeptons.size() < 2 || signalLeptons[0]->Type != signalLeptons[1]->Type ) return;
  
  bool ee = false; bool mm = false;
  std::string flavour;
  if ( signalLeptons[0]->Type == "electron") {
    ee = true;
    flavour = "ee";
  }
  else {
    mm = true;
    flavour = "mm";
  }
 
  //countCutflowEvent("CR"+flavour+"08_leptonIsolationTriggerMatching");
  countCutflowEvent("CR"+flavour+"09_leptonFlavour");

  //for to restrict two leading leptons PT
  if ( signalLeptons[0]->PT <= 50. || signalLeptons[1]->PT <= 25. ) return;
  countCutflowEvent("CR"+flavour+"10_promptLeptons");

  //for opposite charge
  if ( signalLeptons[0]->Charge*signalLeptons[1]->Charge > 0 ) return;
  countCutflowEvent("CR"+flavour+"11_oppositeChargeLeptons");

  //for amount of jetsSignal >= 2
  if ( jetsSignal.size() < 2 ) return;

  countCutflowEvent("CR"+flavour+"12_GT1jet");
  
  //Electron crack veto
  //countCutflowEvent("CR"+flavour+"13_electronCrackVeto");
  
  if( signalLeptons[0]->P4().DeltaR( signalLeptons[1]->P4() ) < 0.1 ) return;

  //for mll > 15
  //redundant
  double mll=(signalLeptons[0]->P4()+signalLeptons[1]->P4()).M();
  if ( mll <= 45. ) return;
  countCutflowEvent("CR"+flavour+"14_mllGT15");

  //for delta phi
  double dphi1 = 0.; double dphi2 = 0.;
  dphi1 = fabs( jetsSignal[0]->P4().DeltaPhi( missingET->P4() ) );
  dphi2 = fabs( jetsSignal[1]->P4().DeltaPhi( missingET->P4() ) );
  if ( dphi1 <= 0.4 ) return;
  countCutflowEvent("CR"+flavour+"15_dPhi(leadingjet,missET)GT0.4");
  if ( dphi2 <= 0.4 ) return;
  countCutflowEvent("CR"+flavour+"16_dPhi(subleadingjet,missET)GT0.4");

  //for mll in (81, 101)
  mll=(signalLeptons[0]->P4()+signalLeptons[1]->P4()).M();
  if ( !( mll > 81 && mll < 101 ) ) return;
  countCutflowEvent("CR"+flavour+"17_mllIn(81,101)");

  //for HT
  double HT = 0.;
  for ( int i = 0; i < jetsSignal.size(); i++ ) {
    HT += jetsSignal[i]->PT;
  }
  HT += signalLeptons[0]->PT + signalLeptons[1]->PT;
  if ( HT <= 600. ) return;
  countCutflowEvent("CR"+flavour+"18_HTGT600");

  //for missingET
  if ( missingET->P4().Et() <= 225.) return;
  countCutflowEvent("CR"+flavour+"19_missETGT225");

  //for counting in signal region


  if ( ee || mm ) countSignalEvent("SRZ");

}


void Atlas_conf_2015_082::finalize() {
  // Whatever should be done after the run goes here
}

