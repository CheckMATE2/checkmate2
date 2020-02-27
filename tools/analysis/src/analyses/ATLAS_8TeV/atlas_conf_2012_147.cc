#include "atlas_conf_2012_147.h"

void Atlas_conf_2012_147::initialize() {
  setInformation(""
  "@# ATLAS\n"
  "@# ATLAS-CONF-2012-147\n"
  "@# Monojet + etmiss\n"
  "@# sqrt(s) = 8 TeV\n"
  "@# int(L) = 10 fb^-1\n"
  "");
  setLuminosity(10*units::INVFB);      
  setAnalysisName("atlas_conf_2012_147");          
  ignore("towers");
  ignore("tracks");
  ignore("electronsTight");
  ignore("muonsCombined");
  bookSignalRegions("1;2;3;4;");
  bookCutflowRegions("0;1;2;3;4;5;6;7");
  const Int_t nBins_1 = 14;
  const Float_t bins_1 [15] = {120.,140.,160.,180.,200.,220.,250.,300.,350.,400.,450.,500.,600.,800.,1200.};
  hist_Met_1 = new TH1F("met1", "ET_miss_1", nBins_1, bins_1);
  hist_PT_1 = new TH1F("pt1", "PT_1", nBins_1, bins_1);
  
  const Int_t nBins_2 = 9;
  const Float_t bins_2 [10] = {220.,250.,300.,350.,400.,450.,500.,600.,800.,1200.};
  hist_Met_2 = new TH1F("met2", "ET_miss_2", nBins_2, bins_2);
  hist_PT_2 = new TH1F("pt2", "PT_2", nBins_2, bins_2);
  
  const Int_t nBins_3 = 6;
  const Float_t bins_3 [7] = {350.,400.,450.,500.,600.,800.,1200.};
  hist_Met_3 = new TH1F("met3", "ET_miss_3", nBins_3, bins_3);
  hist_PT_3 = new TH1F("pt3", "PT_3", nBins_3, bins_3);
  
  const Int_t nBins_4 = 3;
  const Float_t bins_4 [4] = {500.,600.,800.,1200.};
  hist_Met_4 = new TH1F("met4", "ET_miss_4", nBins_4, bins_4);
  hist_PT_4 = new TH1F("pt4", "PT_4", nBins_4, bins_4);
  
}

void Atlas_conf_2012_147::analyze() {
  missingET->addMuons(muonsCombinedPlus);
  electronsMedium = filterPhaseSpace(electronsMedium, 20., -2.47, 2.47);
  electronsMedium = filterIsolation(electronsMedium);
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 7., -2.5, 2.5);
  muonsCombinedPlus = filterIsolation(muonsCombinedPlus);
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);
  photonsMedium = filterPhaseSpace(photonsMedium, 20., -4.5, 4.5);
  
  countCutflowEvent("0");
  
  // Remove jets that overlap with electrons
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  
  // Veto on isolated electrons and muons
  if (!(overlapRemoval(electronsMedium, jets, 0.4)).empty())
    return;    
  countCutflowEvent("1");
  
  if (!(overlapRemoval(muonsCombinedPlus, jets, 0.4)).empty())
    return;
  countCutflowEvent("2");    
  
  //------------------------
  // Cut if too small missing energy
  if (missingET->P4().Et() < 120.0) 
    return;
  countCutflowEvent("3");
  
  //------------------------
  // Cut if no hard central jet
  std::vector<Jet*> cenJets = filterPhaseSpace(jets, 120., -2., 2.);
  if (cenJets.size() < 1)
    return;
  countCutflowEvent("4");
  
  //------------------------
  // veto event if more than two jets > 30 Gev
  std::vector<Jet*> vetoJets = filterPhaseSpace(jets, 30., -4.5, 4.5);
  if (vetoJets.size() > 2)
    return;
  countCutflowEvent("5");    
  
  //------------------------
  // veto event if second jet lies collinear to missing energy
  if (jets.size() > 1) {
    if (fabs(jets[1]->P4().DeltaPhi(missingET->P4())) < 0.5) 
      return;
  }
  countCutflowEvent("6");
  
  //------------------------
  // veto event if hardest object a photon as unlikely to pass charged track requirement
  if (photonsMedium.size() > 0) {
    if (photonsMedium[0]->PT > jets[0]->PT) 
      return;
  }
  countCutflowEvent("7");
  
  //--------------------------
  //------Signal Regions------
  //--------------------------
  //---------------------------
  // Signal region 1
  if ((missingET->P4().Et() < 120.) || (jets[0]->PT < 120.))
    return;
  countSignalEvent("1");
  hist_Met_1->Fill(missingET->P4().Et(), weight);
  hist_PT_1->Fill(jets[0]->PT, weight);
  
  //---------------------------
  // Signal region 2
  if ((missingET->P4().Et() < 220) || (jets[0]->PT < 220.))
    return;
  countSignalEvent("2");  
  hist_Met_2->Fill(missingET->P4().Et(), weight);
  hist_PT_2->Fill(jets[0]->PT, weight);
  
  //---------------------------
  // Signal region 3
  if ((missingET->P4().Et() < 350) || (jets[0]->PT < 350.))
    return;
  countSignalEvent("3");
  hist_Met_3->Fill(missingET->P4().Et(), weight);
  hist_PT_3->Fill(jets[0]->PT, weight);
  
  //---------------------------
  // Signal region 4
  if ((missingET->P4().Et() < 500) || (jets[0]->PT < 500.))
    return;
  countSignalEvent("4");
  hist_Met_4->Fill(missingET->P4().Et(), weight);
  hist_PT_4->Fill(jets[0]->PT, weight);
}

void Atlas_conf_2012_147::finalize() {
  // Whatever should be done after the run goes here
}       
