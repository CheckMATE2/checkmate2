#include "atlas_2004_10894.h"
// AUTHOR: JSK
//  EMAIL: jsk@th.ph
void Atlas_2004_10894::initialize() {
  setAnalysisName("atlas_2004_10894");          
  setInformation(""
    "# EWino search in Higgs (diphoton) and met\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("Cat1;Cat2;Cat3;Cat4;Cat5;Cat6;Cat7;Cat8;Cat9;Cat10;Cat11;Cat12");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_2004_10894::analyze() {
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

  photons = filterPhaseSpace(photons, 25., -2.37, 2.37, true);
  electronsMedium = filterPhaseSpace(electronsMedium, 20., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 20., -2.7, 2.7);
  jets = filterPhaseSpace(jets, 25., -4.4, 4.4);

  photons = overlapRemoval(photons, electronsMedium, 0.01);
  
  jets = overlapRemoval(jets, photons, 0.4);
  jets = overlapRemoval(jets, muonsCombined, 0.4);
  jets = overlapRemoval(jets, electronsMedium, 0.4); 

  electronsMedium = filterIsolation(electronsMedium);
  muonsCombined = filterIsolation(muonsCombined);

  //JJJ: define signal final states

  auto signal_photons = photons;
  auto signal_electrons = electronsMedium;
  auto signal_muons = muonsCombined;
  auto signal_jets = jets;

  double sumet = 0;

  for(int i=0; i < signal_photons.size(); ++i)
    {
      sumet += signal_photons[i]->PT;
    }
  
  for(int i=0; i < signal_muons.size(); ++i)
    {
      sumet += signal_muons[i]->PT;
    }
  for(int i=0; i < signal_electrons.size(); ++i)
    {
      sumet += signal_electrons[i]->PT;
    }
    for(int i=0; i < signal_jets.size(); ++i)
    {
      sumet += signal_jets[i]->PT;
    }


  
  
  if (signal_photons.size() < 2)
    return;

  double mgammagamma = (signal_photons[0]->P4()+signal_photons[1]->P4()).M();

  if(mgammagamma < 105. || mgammagamma > 160.)
    return;
  
  if (signal_photons[0]->PT < mgammagamma*0.35) return;
  if (signal_photons[1]->PT < mgammagamma*0.25) return;

  int n_leptons=signal_muons.size()+signal_electrons.size();
  int n_jets=signal_jets.size();
  bool gen_lepton_pass_ptcut=true;//we select 20GeV leptons already.
  double gen_mjj=0;
  if(signal_jets.size()>=2)gen_mjj=(signal_jets[0]->P4()+signal_jets[1]->P4()).M();
  //std::cout<<signal_jets.at(0).Pt()<<" "<<signal_jets.at(0).Eta()<<" "<<signal_jets.at(0).Phi()<<" "<<signal_jets.at(0).M()<<std::endl;
  //std::cout<<signal_jets.at(1).Pt()<<" "<<signal_jets.at(1).Eta()<<" "<<signal_jets.at(1).Phi()<<" "<<signal_jets.at(1).M()<<std::endl;
  //std::cout<<gen_mjj<<std::endl;
  bool gen_mjj_inrange = gen_mjj>40. and gen_mjj<120.;
  double gen_metsig=missingET->P4().Et()/sqrt(sumet);
  //double gen_metsig=met.Et()/19;
  //  countSignalEvent("Cat0");

  if (n_leptons >= 1 and gen_lepton_pass_ptcut and gen_metsig>0 and gen_metsig<=2) countSignalEvent("Cat1");
  if (n_leptons >= 1 and gen_lepton_pass_ptcut and gen_metsig>2 and gen_metsig<=4) countSignalEvent("Cat2");
  if (n_leptons >= 1 and gen_lepton_pass_ptcut and gen_metsig>4 and gen_metsig<=6) countSignalEvent("Cat3");
  if (n_leptons >= 1 and gen_lepton_pass_ptcut and gen_metsig>6) countSignalEvent("Cat4");
  if (n_leptons == 0 and n_jets >= 2 and gen_mjj_inrange and gen_metsig>5 and gen_metsig<=6) countSignalEvent("Cat5");
  if (n_leptons == 0 and n_jets >= 2 and gen_mjj_inrange and gen_metsig>6 and gen_metsig<=7) countSignalEvent("Cat6");
  if (n_leptons == 0 and n_jets >= 2 and gen_mjj_inrange and gen_metsig>7 and gen_metsig<=8) countSignalEvent("Cat7");
  if (n_leptons == 0 and n_jets >= 2 and gen_mjj_inrange and gen_metsig>8) countSignalEvent("Cat8");
  if (n_leptons == 0 and (n_jets<2 or (n_jets>=2 and not gen_mjj_inrange)) and gen_metsig>6 and gen_metsig<=7) countSignalEvent("Cat9");
  if (n_leptons == 0 and (n_jets<2 or (n_jets>=2 and not gen_mjj_inrange)) and gen_metsig>7 and gen_metsig<=8) countSignalEvent("Cat10");
  if (n_leptons == 0 and (n_jets<2 or (n_jets>=2 and not gen_mjj_inrange)) and gen_metsig>8 and gen_metsig<=9) countSignalEvent("Cat11");
  if (n_leptons == 0 and (n_jets<2 or (n_jets>=2 and not gen_mjj_inrange)) and gen_metsig>9) countSignalEvent("Cat12");

  
  
}

void Atlas_2004_10894::finalize() {
  // Whatever should be done after the run goes here
}       
