#include "cms_1408_3583.h"
// AUTHOR: s.b.
//  EMAIL: swasti@th.physik.uni-bonn.de
void Cms_1408_3583::initialize() {
  setAnalysisName("cms_1408_3583");          
  setInformation(""
    "# CMS\n"
     "# CMS_1408_3583\n"
     "# monojet, etmiss\n"
     "# sqrt(s) = 8 TeV\n"
     "# int(L) = 19.7 fb^-1\n"
  "");
  setLuminosity(19.7*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("250;300;350;400;450;500;550;");
  bookCutflowRegions("0;1;2;3;4;5;6;7;8;9;9a;9b;9c");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.
  const Int_t nBins_Master = 30;
  const Float_t bins_Master [31] = {250.,275.,300.,325.,350.,375.,400.,425.,450.,475.,500.,525.,550.,575.,600.,625.,650.,675.,700.,725.,750.,775.,800.,825.,850.,875.,900.,925.,950.,975.,1000.};
  //hist_Met_Master = new TH1F("metMaster", "Missing ET distribution", nBins_Master, bins_Master);
  const Int_t nBins_MasterPT = 36;
  const Float_t bins_MasterPT [37] = {100.,125.,150.,175.,200.,225.,250.,275.,300.,325.,350.,375.,400.,425.,450.,475.,500.,525.,550.,575.,600.,625.,650.,675.,700.,725.,750.,775.,800.,825.,850.,875.,900.,925.,950.,975.,1000.};
  //hist_PT_Master = new TH1F("ptMaster", "PT_Master", nBins_MasterPT, bins_MasterPT);
  // You should initialize any declared variables here
}

void Cms_1408_3583::analyze() {
  electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.3, 2.3);
  electronsMedium = filterIsolation(electronsMedium);
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 10., -2.3, 2.3);
  muonsCombinedPlus = filterIsolation(muonsCombinedPlus);
  jets = filterPhaseSpace(jets, 10., -4.5, 4.5);
  jets = overlapRemoval(jets, electronsMedium, 0.4); 
  //------Cut0------------------
  // All events considered
  countCutflowEvent("0");
  //------Cut1------------------
  // Cut if too small missing energy
  if (missingET->P4().Et() < 200.0) 
    return;
  countCutflowEvent("1");
  //-------Cut2-------------------
  // Cut if no leading jet with PT > 110
    int i_central=1000;
    bool is_central_jets=false;
    for(int i=0;i<jets.size();i++){
        
        if (!is_central_jets && jets[i]->PT > 110 && fabs(jets[i]->Eta) < 2.4 ){
            i_central=i;
            is_central_jets=true;
        }
    }
    if(!is_central_jets)
        return;
        countCutflowEvent("2");
  //--------Cut3------------------ 
  // two jets with PT > 30 and |eta| = 4.5 allowed
    int counter=0;
    int i_secondjet=1000;
    for(int i=0;i<jets.size();i++){
         if(i != i_central && jets[i]->PT > 30. && fabs(jets[i]->Eta) < 4.5){
            i_secondjet=i;
            counter=counter+1;
        }
     }
    if( counter > 1)
        return;
   countCutflowEvent("3");
   //--------Cut4------------------ 
   //delatPhi between cenJet and second jet < 2.5
    if(counter == 1 && fabs(jets[i_secondjet]->P4().DeltaPhi(jets[i_central]->P4())) > 2.5 )
        return;
    countCutflowEvent("4");
  //----------Cut5-----------
  // Veto on isolated electrons and muons
  if (!electronsMedium.empty())
    return;    
  if (!muonsCombinedPlus.empty())
    return;    
  // Tau-tagging (Tau-lepton removal)
  std::vector<Jet*> tauJets;
  for(int j = 0; j < jets.size(); j++) {
    if( checkTauTag(jets[j], "medium")  and fabs(jets[j]->Charge) == 1) 
	  tauJets.push_back(jets[j]);      
  }
  tauJets = filterPhaseSpace(tauJets, 20., -2.3, 2.3);
  if (!tauJets.empty())
    return;    
  countCutflowEvent("5");

  //-----Fill-Hitograms-------------
  //hist_Met_Master->Fill(missingET->P4().Et(), weight);
  //hist_PT_Master->Fill(jets[i_central]->PT, weight);
  
  //-------------------------
  //------Signal Regions------
  //--------------------------  
  // Signal region 1
  if (missingET->P4().Et() < 250.) 
    return;
  countSignalEvent("250");
  countCutflowEvent("6");

  // Signal region 2
  if (missingET->P4().Et() < 300.)
    return;
  countSignalEvent("300");
  countCutflowEvent("7");

  // Signal region 3
  if (missingET->P4().Et() < 350.)
    return;
  countSignalEvent("350");
  countCutflowEvent("8");
  
  // Signal region 4
  if (missingET->P4().Et() < 400.)
    return;
  countSignalEvent("400");
  countCutflowEvent("9");
  
  // Signal region 5
  if (missingET->P4().Et() < 450.)
    return;
  countSignalEvent("450");
  countCutflowEvent("9a");

  // Signal region 6
  if (missingET->P4().Et() < 500.)
    return;
  countSignalEvent("500");
  countCutflowEvent("9b");
  
  // Signal region 7
  if (missingET->P4().Et() < 550.)
    return;
  countSignalEvent("550");
  countCutflowEvent("9c");
}

void Cms_1408_3583::finalize() {
  // Whatever should be done after the run goes here
  /* Daniel comments this out as it is most likely not required for the normal user
  TFile f1("/scratch/jsk-swasti/beta_private/results/8AV_1Glambda900G_NP4_c/8AV_1Glambda900G_NP4_c.root","recreate");
  Double_t norm = 0.2775*19.7*1000;
  hist_Met_Master->Write();
  Double_t scaleET = norm/(hist_Met_Master->Integral());
  hist_Met_Master->Scale(scaleET);
  hist_Met_Master->Draw();
  hist_PT_Master->Write();
  Double_t scalePT = norm/(hist_PT_Master->Integral());
  hist_PT_Master->Scale(scalePT);
  hist_PT_Master->Draw();
  f1.Close();
  outputHistFile(hist_Met_Master); 
  outputHistFile(hist_PT_Master); 
  */
}

void Cms_1408_3583::outputHistFile(TH1F* inHist) {
    // Write standard information    
    std::cout << "# " << inHist->GetTitle() << " \n";
    std::cout << "#  Bin    MC_Events    (Error)    Normalised_Events    (Error)    Histogram_Normalised_Events    (Error)    Bin_Width    \n";
    for ( int i=1; i<=inHist->GetNbinsX(); i++)
    std::cout << inHist->GetName() << "_" << i << "   " << inHist->GetBinCenter(i) << "    " << inHist->GetBinContent(i) << "    " << inHist->GetBinError(i) << "    " << normalize(inHist->GetBinContent(i)) << "    " << normalize(inHist->GetBinError(i)) << "    " << ((inHist->GetBinContent(i))*0.2775*19.7*1000)/(inHist->Integral()) << "    " << (inHist->GetBinError(i))/(inHist->Integral()) << "    " << inHist->GetBinWidth(i) << std::endl;
}
