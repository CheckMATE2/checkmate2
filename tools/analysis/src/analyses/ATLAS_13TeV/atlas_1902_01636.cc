#include "atlas_1902_01636.h"
// AUTHOR: Roberto Ruiz
//  EMAIL: rruiz@ific.uv.es
void Atlas_1902_01636::initialize() {
  setAnalysisName("atlas_1902_01636");          
  setInformation(""
    "# ATLAS search of heavy long-lived particles with 13 TeV\n"
  "");
  setLuminosity(36.1*units::INVFB);      
  bookSignalRegions("SR-1Cand-FullDet-175;SR-1Cand-FullDet-375;SR-1Cand-FullDet-600;SR-1Cand-FullDet-825;SR-2Cand-FullDet-150;SR-2Cand-FullDet-350;SR-2Cand-FullDet-575;SR-2Cand-FullDet-800");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here

  SingleMuTurnOn_file = TFile::Open("../data/rootfiles/SingleMuTurnOn.root");
  EtmissTurnOn_file = TFile::Open("../data/rootfiles/EtmissTurnOn.root"); 
  LooseEff_file = TFile::Open("../data/rootfiles/LooseEff.root"); 
  TightPromotionEff_file = TFile::Open("../data/rootfiles/TightPromotionEff.root"); 
  MToFFullDet_file = TFile::Open("../data/rootfiles/MToFFullDet.root"); 

  TDirectory* dir_SingleMuTurnOn = SingleMuTurnOn_file->GetDirectory("Table 23");
  TDirectory* dir_EtmissTurnOn = EtmissTurnOn_file->GetDirectory("Table 22");
  TDirectory* dir_LooseEff = LooseEff_file->GetDirectory("Table 25"); 
  TDirectory* dir_TightPromotionEff = TightPromotionEff_file->GetDirectory("Table 26"); 
  TDirectory* dir_MToFFullDet = MToFFullDet_file->GetDirectory("Table 29"); 

  SingleMuTurnOn = (TH2F*)dir_SingleMuTurnOn->Get("Hist2D_y1");
  EtmissTurnOn = (TH1F*)dir_EtmissTurnOn->Get("Hist1D_y1");
  LooseEff = (TH2F*)dir_LooseEff->Get("Hist2D_y1"); 
  TightPromotionEff = (TH2F*)dir_TightPromotionEff->Get("Hist2D_y1");
  MToFFullDet_mean = (TH1F*)dir_MToFFullDet->Get("Hist1D_y1");
  MToFFullDet_std = (TH1F*)dir_MToFFullDet->Get("Hist1D_y1_e1");

}

void Atlas_1902_01636::analyze() {
  // Your eventwise analysis code goes here
  // The following objects are always defined unless they are 'ignored' above. They form std::vector objects of the respective Delphes class type (except for Etmiss which is a single object)
  // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
  // Within a std::vector, all members are ordered with highest pt coming first.

  // electronsLoose, electronsMedium, electronsTight   are list of electrons that passed respective efficiency and reconstruction cuts
  // muonsCombinedPlus, muonsCombined                  as above for muons
  // photonsMedium                                     as above for photons
  // jets are all reconstructed jets                   as above for jets. Note that electrons are most certainly also reconstructed as a jet -> overlap removal do avoid double counting necessary!
  // tracks, towers                                    calorimeter and tracker information. Usually not needed.
  // missingET                                         rec missing ET EXCLUDING muons.

  
  // Here is a couple of useful functions and lines:  
  //------------Phase Space Cuts (defined for jets, electronsXYZ, muonsXYZ, photonsXYZ)
  // jets = filterPhaseSpace(jets, 20., -2.8, 2.8)  // The vector 'jets' only contains jets with pt >= 20 GeV and -2.8 < eta < 2.8. This function is applicable to other particles too (electronsMedium, ... ).
  // jets = overlapRemoval(jets, electronsLoose, 0.2) Removes all jets for which there exists any electron in 'electronsLoose' with deltaR < 0.2.
  // jets = overlapRemovel(jets, 0.2) If two jets overlap within deltaR < 0.2, only the harder jet is stored.
  
  //------------Isolation Checks (defined for electronsXYZ, muonsXYZ, photonsXYZ
  //------------        For each object, if the user entered N isolation conditions, they can be
  //------------        checked individually be the second argument (running from 0 to N-1).
  // electronsMedium = filterIsolation(electronsMedium, 0)            Removes electrons that do not pass the first isolation condition entered into the AnalysisManager by the user
  // std::vector<int> flags; flags.push_back(0); flags.push_back(2);
  // electronsMedium = filterIsolation(electronsMedium, flags)        Same as above, but both the first and the third condition have to be fulfilled
  // electronsMedium = filterIsolation(electronsMedium)               Same as above, but all conditions have to be fulfilled.
  
  //-----------Flavour Tag Checks (defined for jets only)
  //----------          Tau tags "loose", "medium" or "tight" can be individually checked if the user enabled tau tagging in the AM.
  //----------          For b-tags, if N working points have been defined, the ith condition can be tested by putting i-1 into the second argument (if there is only one, the argument can be omitted)
  // if checkTauTag(jets[0], "tight") leadingJetIsTagged = True;
  // if checkBTag(jets[0], 0) leadingJetIsBTagged = True;


  //-----------Auxiliary Information
  // - Always ensure that you don't access vectors out of bounds. E.g. 'if(jets[1]->PT > 150)' should rather be if (jets.size() > 1 && jets[1]->PT > 150). 
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in CheckMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  double Etmiss = missingET->P4().Et(); 

  bool TriggerAccept = false;

  //Muon-trigger
  for (int i = 0; i < true_particles.size(); ++i) {

     auto part = true_particles[i];

     //The candidate has to be SUSY, with electical charge
     bool candidate = (isSUSY(part->PID) && part->Charge != 0 && part->Status == 23);
     
     if (candidate) {
      if (abs(true_particles[part->D1]->PID) != abs(part->PID)) {        
        auto partD = true_particles[true_particles[i]->D1];
        if (decayInsideAtlas(partD) && partD->PT < 26.0) continue;  
      }
     }
     else
      continue;
    
     double eta = part->Eta;
     double beta = part->P4().Beta();
     int bin_eta = SingleMuTurnOn->GetXaxis()->FindBin(fabs(eta));
     int bin_beta = SingleMuTurnOn->GetYaxis()->FindBin(beta);

     double effTrig = SingleMuTurnOn->GetBinContent(bin_eta, bin_beta);

     if (rand()/(RAND_MAX+1.) < effTrig) TriggerAccept = true;   
  }

  // Etmiss trigger accept
  if (Etmiss > 300.) TriggerAccept = true;
  int bin = EtmissTurnOn->GetXaxis()->FindBin(Etmiss);
  double eff_Met = EtmissTurnOn->GetBinContent(bin);
  if (rand()/(RAND_MAX+1.) < eff_Met) TriggerAccept = true;

  // If at least one trigger is expected to fire keep the event
  if (!TriggerAccept) return;

  countCutflowEvent("Trigger");

  int countLoose = 0;
  int countTight = 0;

  std::vector<double> Mass;
 
  for (int i = 0; i < true_particles.size(); ++i) {

    auto part = true_particles[i];

    //The candidate has to be SUSY, with electical charge 
    bool candidate = (isSUSY(part->PID) && part->Charge != 0 && part->Status == 23); 

    if (candidate) {
      if (abs(true_particles[part->D1]->PID) != abs(part->PID)) {        
        auto partD = true_particles[true_particles[i]->D1];
        
        if (decayInsideAtlas(partD) && partD->PT < 26.0) continue;  
      }
     }
     else
	continue;

    double eta = part->Eta;
    double beta = part->P4().Beta();
    double transmon = part->PT;

    Mass.push_back(part->Mass);

    TLorentzVector p4 = part->P4();

    double momentum = p4.Mag();
     
    // Estimate efficiencies
    int bin_eta = LooseEff->GetXaxis()->FindBin(fabs(eta));
    int bin_beta = LooseEff->GetYaxis()->FindBin(beta);
    double effLoose = LooseEff->GetBinContent(bin_eta, bin_beta);
    double effPromotion = TightPromotionEff->GetBinContent(bin_eta, bin_beta);
 
    // Momentum cut loose selection
    if (momentum <= 100.) continue;
    if (transmon <= 70.) continue;

    // Momen
    if (rand()/(RAND_MAX+1.) < effLoose) {
      ++countLoose;

      //
      if (momentum <= 200. || fabs(eta) >= 1.65) continue;

      if (rand()/(RAND_MAX+1.) < effPromotion) ++countTight;
    }
      }

  // Final definition of the different signal regions
  if (countLoose == 2) {
    //Events with exactly two loopse candidates
    countCutflowEvent("loose");

    // Sample the ToF mass fro two full-detector candidates
    int bin_massToF = MToFFullDet_mean->GetXaxis()->FindBin(Mass[0]);
    double massToF_mean = MToFFullDet_mean->GetBinContent(bin_massToF);
    double massToF_resol = MToFFullDet_std->GetBinError(bin_massToF);
    double massToF1 = myRandom.Gaus(massToF_mean, massToF_resol);

    bin_massToF = MToFFullDet_mean->GetXaxis()->FindBin(Mass[1]);
    massToF_mean = MToFFullDet_mean->GetBinContent(bin_massToF);
    massToF_resol = MToFFullDet_std->GetBinError(bin_massToF);
    double massToF2 = myRandom.Gaus(massToF_mean, massToF_resol);

    // Appply the lower mass cuts for the respective signal region
    if (std::min(massToF1, massToF2) > 150.) { 
      // Events in the signal region ST-FullDet-2Cand
      countSignalEvent("SR-2Cand-FullDet-150");
    }   
    // Appply the lower mass cuts for the respective signal region
    if (std::min(massToF1, massToF2) > 350.) { 
      // Events in the signal region ST-FullDet-2Cand
      countSignalEvent("SR-2Cand-FullDet-350");
    }   
    // Appply the lower mass cuts for the respective signal region
    if (std::min(massToF1, massToF2) > 575.) { 
      // Events in the signal region ST-FullDet-2Cand
      countSignalEvent("SR-2Cand-FullDet-575");
    }   
    // Appply the lower mass cuts for the respective signal region
    if (std::min(massToF1, massToF2) > 800.) { 
      // Events in the signal region ST-FullDet-2Cand
      countSignalEvent("SR-2Cand-FullDet-800");
    }   
  } else if (countTight == 1) {
    //Events with exactly two loopse candidates
    countCutflowEvent("tight");

    // Sample the ToF mass fro two full-detector candidates
    int bin_massToF = MToFFullDet_mean->GetXaxis()->FindBin(Mass[0]);
    double massToF_mean = MToFFullDet_mean->GetBinContent(bin_massToF);
    double massToF_resol = MToFFullDet_std->GetBinError(bin_massToF);
    double massToF = myRandom.Gaus(massToF_mean, massToF_resol);

    // Appply the lower mass cuts for the respective signal region
    if (massToF > 175.) {
      // Events in the signal region ST-FullDet-1Cand
      countSignalEvent("SR-1Cand-FullDet-175");
    }   
    if (massToF > 375.) {
      // Events in the signal region ST-FullDet-1Cand
      countSignalEvent("SR-1Cand-FullDet-375");
    }   
    if (massToF > 600.) {
      // Events in the signal region ST-FullDet-1Cand
      countSignalEvent("SR-1Cand-FullDet-600");
    }   
    if (massToF > 825.) {
      // Events in the signal region ST-FullDet-1Cand
      countSignalEvent("SR-1Cand-FullDet-825");
    }   
  }
    
}

bool Atlas_1902_01636::decayInsideAtlas(GenParticle* part) {
 
  double r = TMath::Sqrt(part->X*part->X + part->Y*part->Y);
  double z = part->Z;
  
  if (r < 12.0 && fabs(z) < 23.0 && fabs(part->Eta) < 2.0) return  true;
  else return false;

}

void Atlas_1902_01636::finalize() {
  // Whatever should be done after the run goes here
}       

