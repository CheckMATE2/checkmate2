#include "atlas_higg_2013_03.h"
// AUTHOR: Arran Freegard
//  EMAIL: acf1g14@soton.ac.uk
void Atlas_higg_2013_03::initialize() {
  setAnalysisName("atlas_higg_2013_03");          
  setInformation(""
    "# Z plus Higgs to invisible production\n"
    "# two leptons and missing transverse momentum\n"
    "# 8 TeV, 20.3 fb^-1\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  bookSignalRegions("sig");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}
 // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
void Atlas_higg_2013_03::analyze() {
//Signal electrons fit the 'tight' criteria (defined in paper)
//Electrons with pT>7 GeV, |eta|<2.47 (defined in paper)
  electronsTight = filterPhaseSpace(electronsTight, 7., -2.47, 2.47);

//Muons with pT>7 GeV, |eta|<2.5 (defined in paper)
  muonsCombined = filterPhaseSpace(muonsCombined, 7., -2.4, 2.4);  

//Jets with pT>25 GeV, |eta|<2.5 (defined in paper)
  jets = filterPhaseSpace(jets, 25., -2.5, 2.5); 
 

  tracks = filterPhaseSpace(tracks, 0.5, -2.5, 2.5);


  //1) Removes all electrons for which there exists any muon with deltaR < 0.2 (defined in paper, pg2)
electronsTight = overlapRemoval(electronsTight, muonsCombined, 0.2, "y"); 

  // 2) Removes all jets for which there exists any electron with deltaR < 0.2. (defined in paper, pg2)
jets = overlapRemoval(jets, electronsTight, 0.2, "y");

  //3) Removes all electrons for which there exists any jet with deltaR < 0.4 (defined in paper, pg2)
electronsTight = overlapRemoval(electronsTight, jets, 0.4, "y");

  //4) Removes all muons for which there exists any jet with deltaR < 0.4 (defined in paper, pg2)
muonsCombined = overlapRemoval(muonsCombined, jets, 0.4, "y");   

//============================================================================================================================================================
//============================================================================================================================================================

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  std::vector<Electron*> electrons_signal = electronsTight;
  std::vector<Muon*> muons_signal = muonsCombined;
    
  double met = missingET->P4().Et();

  countCutflowEvent("01_Initial"); 
//if the total number of signal electrons and muons is not 2, remove    
  if ( (muons_signal.size() + electrons_signal.size()) != 2 ) return;  
 
  bool ee = false; bool mumu = false; bool emu = false;
  std::string flavour;
  std::vector<TLorentzVector> leptons;
  if (electrons_signal.size() + muons_signal.size() != 2) return;
  if (electrons_signal.size() == 2) {
    ee = true;
    flavour = "ee";
    leptons.push_back( electrons_signal[0]->P4() );
    leptons.push_back( electrons_signal[1]->P4() );
  }
  else if (muons_signal.size() == 2) {
    mumu = true;
    flavour = "mumu";
    leptons.push_back( muons_signal[0]->P4() );
    leptons.push_back( muons_signal[1]->P4() );
  }
  else {
    emu = true;
    flavour = "emu";
    if ( electrons_signal[0]->PT > muons_signal[0]->PT ) {
      leptons.push_back( electrons_signal[0]->P4() );
      leptons.push_back( muons_signal[0]->P4() );
    }
    else {
      leptons.push_back( muons_signal[0]->P4() );
      leptons.push_back( electrons_signal[0]->P4() );
    }
  }
  
//If lepton pT is less than 20 GeV, remove (pg2)
  if ( leptons[0].Perp() < 20. or leptons[1].Perp() < 20.) return;  

  countCutflowEvent("02_OS_leptons"); 

////////////////////////////////////////////////////////////////////////

  double mll = (leptons[0] + leptons[1]).M();

//"Z window");
//need 76 GeV < mll < 106 GeV
if (mll < 76. or mll > 106) return;  
   countCutflowEvent("03_Zwindow");

//"MET > 90 GeV");
if ( met < 90) return;
  countCutflowEvent("04_MET>90 GeV");

////////////////////////////////////////////////////////////////////
    // basic variables for the two leptons
    TLorentzVector p_dilepton = (leptons[0] + leptons[1]);

    // MET and pTmiss
  TLorentzVector track_MET;
  for (int i = 0; i < tracks.size(); i++) 
     if ( tracks[i]->PT > 0.5 and fabs(tracks[i]->Eta) < 2.5 ) track_MET -= tracks[i]->P4();   
     //another calculation of MET 
  TLorentzVector pTmiss = TLorentzVector(0., 0., 0., 0.);
  for (int i = 0; i < jets.size(); i++)
    pTmiss -= jets[i]->P4();
  
  TLorentzVector pTmiss_soft = TLorentzVector(0., 0., 0., 0.);
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
    bool used = false;
    for (int i = 0; i < jets.size() && !used; i++)
      for (int part = 0; part < jets[i]->Particles.GetEntries(); part++)
        if ((*it)->PT > 0.4 && jets[i]->Particles.At(part) == (*it)->Particle) {
	  used = true;
	  break;
	}
    if (!used)  pTmiss_soft -= (*it)->P4();
  }
  
  pTmiss += pTmiss_soft;
  
///////////////////////////////////////////////////////////////////

//"dilepton-MET separation");
    // azimuthal separation between...
	
    //if (dphi_ll_met < 2.6) return;
    if (fabs((leptons[0] + leptons[1]).DeltaPhi(missingET->P4())) < 2.6) return;
 //   if (fabs((leptons[0] + leptons[1]).DeltaPhi(met)) < 2.6) return;
 countCutflowEvent("05_dilepton-MET_separation");

//"lepton-lepton separation");
    // the azimuthal opening angle of the two leptons is required to be less
    // than 1.7

    //if (dphi_l_l > 1.7)  return;
    if (fabs(leptons[0].DeltaPhi(leptons[1])) > 1.7)  return;
 countCutflowEvent("06_lepton-lepton_separation");

//"pTmiss-MET separation");
    // cut on the azimuthal angle between MET and pTmiss to reject MET from
    // misreconstructed energy in the calorimeter (dphi < 0.2)
	if (fabs(track_MET.DeltaPhi(pTmiss)) > 0.2) return;
 countCutflowEvent("07_pTmiss-MET_separation");


//"pTll-MET similarity");
    // pT of the dilepton pair and MET should be similar
    if (fabs(  (met - p_dilepton.Perp()) / p_dilepton.Perp() ) > 0.2) return;
 countCutflowEvent("08_pTll-MET_similarity");


//"jet veto");
//Remove if the jet size is not zero
  if (jets.size() != 0 ) return;
	  countCutflowEvent("09_jetveto");

  		countSignalEvent("sig");
/////////////////////////////////////////////////////

    }

void Atlas_higg_2013_03::finalize() {
  // Whatever should be done after the run goes here
}     
