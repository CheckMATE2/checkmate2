#include "atlas_2103_11684.h"
// AUTHOR: J.S.Kim
//  EMAIL: jsk@th.physik.uni-bonn.de
void Atlas_2103_11684::initialize() {
  setAnalysisName("atlas_2103_11684");          
  setInformation(""
    "# Search for SUSY in events with four or more leptons\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("SR0-ZZloose;SR0-ZZtight");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_2103_11684::analyze() {
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

  electronsMedium = filterPhaseSpace(electronsMedium, 7., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 5., -2.7, 2.7);
  jets = filterPhaseSpace(jets, 20., -4.5, 4.5);

  std::vector<Jet*> tauJets;
  for(int i=0; i<jets.size(); i++) {
    if(fabs(jets[i]->Eta) < 2.5 and checkTauTag(jets[i], "medium") and fabs(jets[i]->Charge)==1) {
      tauJets.push_back(jets[i]);
    }
    
  }

  tauJets=filterPhaseSpace(tauJets, 20., -2.47, 2.47, true);//tau jet

  std::vector<Jet*> bjets;
  std::vector<Jet*> nonbjets;
  for (int i = 0; i < jets.size(); i++)
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i]) ) 
      bjets.push_back(jets[i]);
    else nonbjets.push_back(jets[i]);  

  std::vector<FinalStateObject*> leptons;
  for(int e = 0; e < electronsMedium.size(); e++) {
    FinalStateObject* lep = newFinalStateObject(electronsMedium[e]);
    leptons.push_back(lep);
  }
  for(int m = 0; m < muonsCombined.size(); m++) {
    FinalStateObject* lep = newFinalStateObject(muonsCombined[m]);
    leptons.push_back(lep);
  }

  
  //overlap removal
  //1.
  tauJets = overlapRemoval(tauJets, electronsMedium, 0.2);
  //1.
  tauJets = overlapRemoval(tauJets, muonsCombined, 0.2);
  //2.
  electronsMedium = overlapRemoval(electronsMedium, muonsCombined, 0.01);
  //3.
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  //4.
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.04);
  //5.
  jets = overlapRemoval(jets, muonsCombined, 0.2);
  //6.
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.04);
  //7.
  jets = overlapRemoval(jets, tauJets, 0.4);

  countCutflowEvent("01_object_removal");
 
  std::vector <std::pair<double, double>> zmassdiff;
  
  double mll=0;  
  
  if(electronsMedium.size() + muonsCombined.size() < 2)
    {
      return;
    }

  //SFOS removal:
  if(electronsMedium.size() > 1)
    {
      electronsMedium=removeLowMassResonances(electronsMedium, 8.4, 10.4);
      electronsMedium=removeLowMassResonances(electronsMedium, 0., 4.);
    }
  if(muonsCombined.size() > 1)
    {
      muonsCombined=removeLowMassResonances(muonsCombined, 8.4, 10.4);
      muonsCombined=removeLowMassResonances(muonsCombined, 0., 4.);
    }
  
  countCutflowEvent("02_2_leptons");

  if(0)//JJJ: implement trigger
    {
      return;
    }
  countCutflowEvent("03_trigger");
  
  if(electronsMedium.size() + muonsCombined.size() < 4)
    {
      return;
    }
      countCutflowEvent("04_4_leptons");

  std::cout << "0: " << electronsMedium.size() + muonsCombined.size()  << std::endl;

    if(electronsMedium.size() > 1)
      findZ(electronsMedium,zmassdiff);

    if(muonsCombined.size() > 1)
      findZ(muonsCombined,zmassdiff);

  //sort zmassdiff according to smallest mass difference between lepton pair and Z boson mass
  std::sort(zmassdiff.begin(),zmassdiff.end(), [](std::pair<double,double> a, std::pair<double, double> b){return a.first < b.first;});

  
  if(zmassdiff.size() < 2)
    {
      return;
    }
  countCutflowEvent("05_2_Z_candidates");  

  auto itz1 = zmassdiff.begin();
  auto itz2 = next(zmassdiff.begin(),1);

  if( ! ( (*itz1).second < 101.2 && (*itz1).second > 61.2 ) )
    {
      return;
    }
  std::cout << "1: " << (*itz1).second << std::endl;
      countCutflowEvent("06_ZZ_loose");  

  if( ! ( (*itz2).second < 101.2 && (*itz2).second > 81.2 ) )
    {
      return;
    }
  std::cout << "2: " << (*itz2).second << std::endl;
        countCutflowEvent("07_ZZ_tight");
  
  if( bjets.size() > 0 )
    {
      return;
    }

  if(tauJets.size() < 0)
    {
      return;
    }

  
  double met = missingET->P4().Et();
  if(met > 100.)
    {
      countSignalEvent("SR0-ZZloose");
    }
  if (met > 200.)
    {
      countSignalEvent("SR0-ZZtight");
    }
  
}

void Atlas_2103_11684::finalize() {
  // Whatever should be done after the run goes here
}       


template <class X>
std::vector <X*> Atlas_2103_11684::removeLowMassResonances(std::vector <X*> candidates, double minminv, double maxminv)
{
  int z_candidate=0;
  std::set <std::pair<double, double>> zmassdiff;
  double mll;
  
  //  for (int e = 0; e < candidates.size(); e++) {
  //  for (int e2 = e+1; e2 < candidates.size(); e2++) {

  auto it = candidates.begin();
  std::vector <int> removeCandidates;
  while(it != candidates.end())
    {
       removeCandidates.push_back(1);
      ++it;
    }
  
  
  for(auto cand1 = candidates.begin(); cand1 != candidates.end(); ++cand1){
    //    for(auto cand2 = std::next(cand1,1); cand2 != candidates.end();){
    for(auto cand2 = candidates.begin(); cand2 != candidates.end(); ++cand2){

      if(cand1 == cand2)
	continue;
      
      if ((*cand1)->Charge*(*cand2)->Charge > 0)
	continue;

      TLorentzVector pair = ( (*cand1)->P4() + (*cand2)->P4() );
      mll = pair.M();
      if (mll > minminv && mll < maxminv) 
	{
	  //	  std::distance(candidates.begin(), cand1);
	   removeCandidates[std::distance(candidates.begin(), cand1)]=0;
	   removeCandidates[std::distance(candidates.begin(), cand2)]=0;
	  //	  cand1 = candidates.erase(cand1);
	  //cand2 = candidates.erase(cand2);
	}
      //      else
      //	{
      //	  ++cand1;
      //	  ++cand2;
      //	}	
    }//for
  }//for


  it = candidates.begin();
  size_t index = 0;
  
  while( it != candidates.end())
    {
      if( removeCandidates[index] == 0){
	it = candidates.erase(it);
	++index;
      }
      else
	{
	  ++it;
	  ++index;
	}
    }
  

  return candidates;
}

    
template <class X>
void Atlas_2103_11684::findZ(std::vector<X*> candidates, std::vector <std::pair<double, double>> & zmassdiff)
{

  double mll=0.;
  
  for (int e = 0; e < candidates.size(); e++) {
    for (int e2 = e+1; e2 < candidates.size(); e2++) {      
      if (candidates[e]->Charge*candidates[e2]->Charge > 0) continue;

      TLorentzVector pair = ( candidates[e]->P4() + candidates[e2]->P4() );
      mll = pair.M();
      if ( fabs( mll-91.2) < 30.){
	zmassdiff.push_back(std::make_pair(fabs(mll-91.2),mll));
      }//if z boson
    }//for
  }//for
  return;
}
