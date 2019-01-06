#include "atlas_1712_02118_re.h"

#include <TRandom.h>
#include <TFile.h>
#include <TH2.h>
#include <TF1.h>
#include <TMath.h>

// AUTHOR: J.S.Kim
//  EMAIL: jsk@th.physik.uni-bonn.de



void Atlas_1712_02118_re::initialize() {
  setAnalysisName("atlas_1712_02118_re");          
  setInformation(""
		 "# Search for long-lived charginos based on a disappearing-track signature\n"
		 "");
  setLuminosity(36.0*units::INVFB);      
  bookSignalRegions("SR1");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  const std::string production = "electroweak";
  // _SHAREDIR should be defined via the preprocessor which should be initialised by the makefile... maybe
  const char *const acceffmapFilePath         = _SHAREEFFFILE;
  //const char *const acceffmapFilePath         = "/hdd/Tools/CheckMATE_Git/CheckMATECollab/current/tools/analysis/share/DisappearingTrack2016-TrackAcceptanceEfficiency.root";
  std::cout << "Trying to access efficiency from " << acceffmapFilePath << std::endl;
  const char *const acceffStrongHistName      = "StrongEfficiency";
  const char *const acceffElectroweakHistName = "ElectroweakEfficiency";
  

  // Read efficiency map
  if (!acceffmapFile) {
    acceffmapFile = new TFile (acceffmapFilePath);
    if (!acceffmapFile) {
      std::cerr << "Fail to open " << acceffmapFilePath << std::endl;
      return;
    }
  }
  if (!acceffmapHist) {
    std::string histname = production == "electroweak" ? acceffElectroweakHistName : acceffStrongHistName;
    acceffmapHist = dynamic_cast < TH2 * >(acceffmapFile->FindObjectAny(histname.c_str()));
    if (!acceffmapHist) {
      std::cerr << "Fail to find a histogram " << histname << std::endl;
      return;
    }
  }

}

void Atlas_1712_02118_re::analyze() {
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

  //  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  missingET->addMuons(muonsCombined);
  
  // Switch to simulated pT resolution of tracklets
  const bool doPtSmearing = true;
  //  const bool doPtSmearing = false;
    
  // Smearing function to simulation pT resolution
  const double smearPar0 = 1;                             // constant
  const double smearPar1 = -1.72142e+00;                  // mean  [/TeV]
  const double smearPar2 = 1.32009e+01 * (1 - 0.0843956); // sigma [/TeV]
  const double smearPar3 = 1.66707e+00 * (1 - 0.0498447); // slope
  
  std::vector<GenParticle*> charginos;
  std::vector<GenParticle*> charginostemp = true_llpmothers;
  std::cout << "Truemothes" << true_llpmothers.size() << std::endl;
  std::cout << "Truedecays" << true_llpdecays.size() << std::endl;
  std::cout << "Truevertices" << true_llpvertices.size() << std::endl;

  int iv = 0;
  for (auto v : true_llpvertices) {
      iv += 1;
      std::cout << " vertex " << iv << " comes from mothere with PID " << v->mother->PID << std::endl;
      std::cout << " it has " << v->tracks.size() << " tracks,   " << v->stableDaughters.size() << " stable daughters and   " << v->vertexDaughters.size() << " daughters associatd to the vertex itself " << std::endl;
      std::cout << " the associated daughters have PIDS " << std::endl;
      for (auto vd : v->stableDaughters) {
	  std::cout << "   " << vd->PID << std::endl;
      }
  }
  std::cout << std::endl;
  
  std::vector<GenParticle*> neutralinos;
  double r,y;
  

  

  std::cout << "-----------------NEU----------------------" << std::endl;
  /*
  for(int i=0;i<true_llpmothers.size();i++) {
      int d1 = true_llpmothers[i]->D1;
      int d2 = true_llpmothers[i]->D2;
      if( fabs(true_llpdecays[d1]->PID) == 1000022 && true_llpdecays[d1]->Status == 1)
	  neutralinos.push_back(true_llpdecays[d1]);
      if( fabs(true_llpdecays[d2]->PID) == 1000022 && true_llpdecays[d2]->Status == 1)
	  neutralinos.push_back(true_llpdecays[d2]);
  }
  std::cout << true_llpmothers.size() << " Mothers and " << neutralinos.size() << " Neutralinos" << std::endl;
  */
    // Simulate tracklet efficiency
  for (int i=0;i<charginostemp.size();i++) {
    // Simulate decay position
    if(charginostemp[i]->PT > 20. ){
	// need to access daughter for decay vertex
	GenParticle* dau1 = true_llpdecays[charginostemp[i]->D1];
      r=TMath::Sqrt(dau1->X*dau1->X+dau1->Y*dau1->Y);
      y=charginostemp[i]->Eta;
      std::cout << "i : " << i << std::endl;
      std::cout << "r : " << r << std::endl;
      std::cout << "eff: "<< acceffmapHist->GetBinContent(acceffmapHist->FindBin(y,r)) << std::endl;
      std::cout << "before " << charginostemp.size() <<std::endl;
      
      if (gRandom->Rndm() < acceffmapHist->GetBinContent(acceffmapHist->FindBin(y,r))) {
	charginos.push_back(charginostemp[i]);
	
      }
    }
  }
  
  std::cout << "after: " << charginos.size() <<std::endl;
  
  // Select up to one chargino in an event
  int charginoIndex = 0;
  switch (charginos.size()) {
  case 0:
    charginoIndex = -1;
    break;
  case 1:
    charginoIndex = 0;
    break;
  case 2:
    // select randomly (assuming up to two charginos)
    charginoIndex = gRandom->Rndm() > 0.5 ? 0 : 1;
    break;
  }

  // pT smearing
  auto fgFDQOverPt = new TF1("SmearingFunction",
			     [&](double *x, double *p) { return PixelTrackletSmearingFunction(x, p); },
			     -1000, 1000, 4);
  fgFDQOverPt->SetParameter(0, smearPar0);
  fgFDQOverPt->SetParameter(1, smearPar1);
  fgFDQOverPt->SetParameter(2, smearPar2);
  fgFDQOverPt->SetParameter(3, smearPar3);
  double ptSmeared = -1;
  if (doPtSmearing && charginoIndex > -1) {
    double qoverpt = charginos[charginoIndex]->Charge / charginos[charginoIndex]->PT * (1e+3); // GeV -> /TeV
    double qoverptSmeared = qoverpt + fgFDQOverPt->GetRandom();
    ptSmeared = fabs(1. / qoverptSmeared) * (1e+3); // /TeV -> GeV
  }
  
  std::cout << "charginoindex : " << charginoIndex << std::endl;

  
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);  
  electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47, true);
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 10., -2.7, 2.7);
  
  jets = overlapRemoval(jets, electronsTight, 0.2);
  electronsTight = overlapRemoval(electronsTight, jets,0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  std::vector<Jet*> jets50;			      
  jets50 = filterPhaseSpace(jets, 50., -2.8, 2.8);
  
  //double met = missingET->P4().Et();

  
  //  for (int i=0;i<charginos.size();i++) {
  //  met=met+charginos[i]->PT;
  //}

  TLorentzVector metVec = missingET->P4();

  double Px = 0;
  double Py = 0;
  for (int i=0;i<charginos.size();i++) {
    Px += charginos[i]->Px;
    Py += charginos[i]->Py;
  }

  // don't add charginos to MET, since neutrlalinos are already included.
  //  metVec.SetPxPyPzE(metVec.Px() + Px, metVec.Py() + Py, 0.0, 1.0);
  double met = metVec.Pt();

  

  //Trigger cut
  
  double randtoss = rand()/(RAND_MAX+1.);

  bool trigger=false;
  //  if( met > 110. && randtoss < 0.20 )
  if( met > 100. )
    trigger=true;

  //if( met > 70. && randtoss < 0.75 )
  //  trigger=true;

  
  //  trigger=true;

  if(!trigger)
    return;
  
  countCutflowEvent("0_all/0_veto");
  
  //Lepton Veto
  
  if( electronsTight.size() || muonsCombined.size() )
    return;

  countCutflowEvent("1_leptonveto");
  
  
  if(jets.size() == 0 )
    return;
  
  countCutflowEvent("2_at_least_one_jet");

  if(jets[0]->PT < 140.)
    return;
  
  countCutflowEvent("3_ptmin(j1)>140");
  

  if(met < 140.)
    return;
  
  countCutflowEvent("4_MET");
  

  for (int i = 0; i < jets50.size(); i++ ){
    if ( jets50.size() < 5 && fabs(jets50[i]->P4().DeltaPhi( missingET->P4() )) < 1.0 )
      return;
  }
  
  countCutflowEvent("5_DeltaPhiCut");

  if (charginoIndex >= 0) {
    double charginoPt = charginos[charginoIndex]->PT;
    countCutflowEvent("7_Chargino_candidate");
    if (doPtSmearing) {
      charginoPt = ptSmeared;
    }
    if (charginoPt > 100) {
      countCutflowEvent("8_Chargino_PT");
      // double deltaphi,deltaeta,delta;
      // double pi= 3.14159;
      // for(int i=0;i< jets50.size();i++){
      // 	deltaeta=fabs(jets[i]->Eta- charginos[charginoIndex]->Eta);
      // 	deltaphi=fabs(jets[i]->Phi- charginos[charginoIndex]->Phi);
      // 	if(deltaphi >= pi) delta -= 2.*pi;
      // 	if(deltaphi < -pi) delta += 2.*pi;
      // 	if(sqrt(deltaphi*deltaphi+deltaeta*deltaeta) < 0.4) return;
      // }

      // countCutflowEvent("9_Chargino_Isolation");
      
      // if (fabs(charginos[charginoIndex]->Eta) < 1.9 && fabs(charginos[charginoIndex]->Eta) > 0.1 ) {

	
      // 	countCutflowEvent("10_Chargino_Eta");
      countSignalEvent("SR1");
      
      //}
    }
  }
  

}

void Atlas_1712_02118_re::finalize() {
  // Whatever should be done after the run goes here
}       

double Atlas_1712_02118_re::PixelTrackletSmearingFunction(double *x, double *par) {
        double constant = par[0];
        double mean     = par[1];
        double sigma    = par[2];
        double alpha    = par[3];

        // evaluate the crystal ball function
        if (sigma < 0) {
            return 0;
        }
        if (alpha < 0) {
            return 0;
        }
        double z = (x[0] - mean) / sigma;
        alpha = fabs(alpha);
        double norm1 = sigma * sqrt(2 * M_PI) * erf(alpha / sqrt(2));
        double norm2 = sigma * exp(-alpha * alpha / 2) / alpha;
        double norm3 = norm2;
        constant /= (norm1 + norm2 + norm3);
        if (z < -alpha) {
            return constant * std::exp(+alpha * (z + 0.5 * alpha));
        } else if (z > +alpha) {
            return constant * std::exp(-alpha * (z - 0.5 * alpha));
        } else {
            return constant * std::exp(-0.5 * z * z);
        }
}




