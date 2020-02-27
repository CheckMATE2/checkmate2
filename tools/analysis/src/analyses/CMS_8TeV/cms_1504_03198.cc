#include "cms_1504_03198.h"
// AUTHOR: Junjie Cao, Liangliang Shang, Peiwen Wu, Jin Min Yang, Yang Zhang
//  EMAIL: junjiec@itp.ac.cn, shlwell1988@gmail.com, peiwen.wu123@gmail.com, jmyang@itp.ac.cn, zhangyang@itp.ac.cn
void Cms_1504_03198::initialize() {
  setAnalysisName("cms_1504_03198");          
  setInformation(""
    "# cms\n"
     "# cms_1504_03198\n"
     "# 1 leptons, >=3 jets, >=1 b-jets, etmiss\n"
     "# sqrt(s) = 8 TeV\n"
     "# int(L) = 19.7 fb^-1\n"
  "");
  setLuminosity(19.7*units::INVFB);      
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  bookSignalRegions("SR1;");
  bookCutflowRegions("CR1_MET_320;CR2_1_lepton;CR3_3_jets;CR4_1_bjet;CR5_mTlepton_160;CR6_deltaj1j2_1dot2;CR7_mt2w_200;");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Cms_1504_03198::analyze() {
  
  // CMS ANALYSIS
  // IMPORTANT INFO: Only 'muons' and 'electrons' objects available for CMS (i.e not muonsCombined, electronsMedium, etc....)
  // These are Delphes defaults and have not been tuned by CheckMATE
  
  // Your eventwise analysis code goes here
  // The following objects are always defined unless they are 'ignored' above. They form std::vector objects of the respective Delphes class type (except for Etmiss which is a single object)
  // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
  // Within a std::vector, all members are ordered with highest pt coming first.

  // electrons                                         are list of electrons that passed respective efficiency and reconstruction cuts
  // muons                                             as above for muons
  // photons                                           as above for photons
  // jets are all reconstructed jets                   as above for jets. Note that electrons are most certainly also reconstructed as a jet -> overlap removal do avoid double counting necessary!
  // tracks, towers                                    calorimeter and tracker information. Usually not needed.
  // missingET                                         rec missing ET EXCLUDING muons.

  
  // Here is a couple of useful functions and lines:  
  //------------Phase Space Cuts (defined for jets, electronsXYZ, muonsXYZ, photonsXYZ)
  // jets = filterPhaseSpace(jets, 20., -2.8, 2.8)  // The vector 'jets' only contains jets with pt >= 20 GeV and -2.8 < eta < 2.8. This function is applicable to other particles too (electronsMedium, ... ).
  // jets = overlapRemoval(jets, electronsLoose, 0.2) Removes all jets for which there exists any electron in 'electronsLoose' with deltaR < 0.2.
  // jets = overlapRemoval(jets, 0.2) If two jets overlap within deltaR < 0.2, only the harder jet is stored.
  
  //------------Isolation Checks (defined for electronsXYZ, muonsXYZ, photonsXYZ
  //------------        For each object, if the user entered N isolation conditions, they can be
  //------------        checked individually be the second argument (running from 0 to N-1).
  // electrons = filterIsolation(electrons, 0)            Removes electrons that do not pass the first isolation condition entered into the AnalysisManager by the user
  // std::vector<int> flags; flags.push_back(0); flags.push_back(2);
  // electrons = filterIsolation(electrons, flags)        Same as above, but both the first and the third condition have to be fulfilled
  // electrons = filterIsolation(electrons)               Same as above, but all conditions have to be fulfilled.
  
  //-----------Flavour Tag Checks (defined for jets only)
  //----------          Tau tags "loose", "medium" or "tight" can be individually checked if the user enabled tau tagging in the AM.
  //----------          For b-tags, if N working points have been defined, the ith condition can be tested by putting i-1 into the second argument (if there is only one, the argument can be omitted)
  // if checkTauTag(jets[0], "tight") leadingJetIsTagged = True;
  // if checkBTag(jets[0], 0) leadingJetIsBTagged = True;


  //-----------Auxiliary Information
  // - Always ensure that you don't access vectors out of bounds. E.g. 'if(jets[1]->PT > 150)' should rather be if (jets.size() > 1 && jets[1]->PT > 150). 
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in ChekćkMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  

  missingET->addMuons(muons);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  

electrons = filterPhaseSpace(electrons, 30., -2.5, 2.5);
muons = filterPhaseSpace(muons, 30., -2.1, 2.1);
jets = filterPhaseSpace(jets, 30., -4.0, 4.0);

electrons = filterIsolation( electrons );
muons = filterIsolation( muons );

jets = overlapRemoval(jets, electrons, 0.2);
electrons = overlapRemoval(electrons, jets, 0.4);
muons = overlapRemoval(muons, jets, 0.4);

// MET > 320 GeV
if(missingET->PT < 320.0)
return;
countCutflowEvent("CR1_MET_320");

// 1 leptons
std::vector<Electron*> electronsEff;
  for(int i=0; i<electrons.size(); i++) {
    if( (fabs(electrons[i]->Eta) < 1.44) || (fabs(electrons[i]->Eta) > 1.57) ) 
	electronsEff.push_back(electrons[i]);
  }
  std::vector<FinalStateObject*> leptons;
  for(int i = 0; i < electronsEff.size(); i++) {
    FinalStateObject* lep = newFinalStateObject(electronsEff[i]);
    leptons.push_back(lep);
  }
  for(int i = 0; i < muons.size(); i++) {
    FinalStateObject* lep = newFinalStateObject(muons[i]);
    leptons.push_back(lep);
  }
  if(leptons.size() != 1)
    return;
countCutflowEvent("CR2_1_lepton");

// >=3 jets
if(jets.size() < 3)
return;
countCutflowEvent("CR3_3_jets");

// >=1 b-jets
  std::vector<Jet*> bjets;
  std::vector<Jet*> notbjets;
  for(int i=0; i<jets.size(); i++) {
    if(fabs(jets[i]->Eta)<2.4 && checkBTag(jets[i]))
      bjets.push_back(jets[i]);
    if( not checkBTag(jets[i]) )
      notbjets.push_back(jets[i]);
  }
  if(bjets.size()<1)
    return;
countCutflowEvent("CR4_1_bjet");

//mT(lepton,MET) > 160 GeV
if(AnalysisBase::mT(leptons[0]->P4(), missingET->P4()) < 160.)
return;
countCutflowEvent("CR5_mTlepton_160");

// min delta(j1j2,MET) > 1.2
if(std::min(  fabs( jets[0]->P4().DeltaPhi(missingET->P4()) ) , fabs( jets[1]->P4().DeltaPhi(missingET->P4()) )  ) < 1.2)
return;
countCutflowEvent("CR6_deltaj1j2_1dot2");

// mt2w > 200 GeV, do NOT forget the interchange of the two bjets, keep the smaller mt2w
double mt2w_value=1.E10;

// if only 1 bjets, then use the first 3 highest-pt notbjets as the other one bjet
if(bjets.size() == 1)
    for(int i=0 ; i <= std::min(int(notbjets.size())-1,2) ; i++)
mt2w_value = std::min(mt2w_value, std::min( AnalysisBase::mT2_w( leptons[0]->P4(), bjets[0]->P4(), notbjets[i]->P4(), missingET->P4()) , AnalysisBase::mT2_w( leptons[0]->P4(), notbjets[i]->P4(), bjets[0]->P4(), missingET->P4()) ) );

// if more than 1 bjets, then use all combinations of bjet pairs
if(bjets.size() > 1)
     for(int i = 0; i <= bjets.size()-2 ; i++)
	  for(int j = i + 1 ; j <= bjets.size()-1 ; j++)
mt2w_value = std::min(mt2w_value, std::min( AnalysisBase::mT2_w( leptons[0]->P4(), bjets[i]->P4(), bjets[j]->P4(), missingET->P4()) , AnalysisBase::mT2_w( leptons[0]->P4(), bjets[j]->P4(), bjets[i]->P4(), missingET->P4()) ));

if(mt2w_value < 200.0)
return;
countCutflowEvent("CR7_mt2w_200");

// collect events in SR
countSignalEvent("SR1");
}

void Cms_1504_03198::finalize() {
  // Whatever should be done after the run goes here
}       
