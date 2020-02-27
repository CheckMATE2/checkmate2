#include "atlas_phys_pub_2013_011.h"
// AUTHOR: Jong Soo Kim
//  EMAIL: jongsoo.kim@tu-dortmund.de
void Atlas_phys_pub_2013_011::initialize() {
  setAnalysisName("atlas_phys_pub_2013_011");          
  setInformation(""
    "# Search for top squark pair production in final states with 0 or 1 lepton\n"
  "");
  setLuminosity(3000.0*units::INVFB);
  ignore("towers"); // These won't read tower or track information from the
  ignore("tracks"); //  Delphes output branches to save computing time.
  //bookSignalRegions("1leptonmstop500;1leptonmstop600;1leptonmstop700;1leptonmstop800;1leptonmstop900;1leptonmstop1000;1leptonmstop1100;1leptonmstop1200;1leptonmstop1300;1leptonmstop1400;1leptonmstop1500;0leptonmstop500;0leptonmstop600;0leptonmstop700;0leptonmstop800;0leptonmstop900;0leptonmstop1000;0leptonmstop1100;0leptonmstop1200;0leptonmstop1300;0leptonmstop1400;0leptonmstop1500;");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
    bookSignalRegions("1leptonmstop500;1leptonmstop600;1leptonmstop700;1leptonmstop800;1leptonmstop900;1leptonmstop1000;1leptonmstop1100;1leptonmstop1200;0leptonmstop500;0leptonmstop600;0leptonmstop700;0leptonmstop800;0leptonmstop900;0leptonmstop1000;0leptonmstop1100;0leptonmstop1200;");
        bookCutflowRegions("0_0lepton_>leptonveto;1_0lepton_>6jets;2_0lepton_>pt(j2)>80GeV;3_0lepton_Deltaphi;4_0lepton_>=2bjets;5_0lepton_>hadronic_top1;6_0lepton_>hadronic_top2;0_1lepton_>#lepton==1;1_1lepton_>#jets>3_jetptcut;2_1lepton_>#bjets>0;3_1lepton_>deltaphicut;4_1lepton_>MET/sqrt(HT)<15;5_1lepton_>hadronic_top;");
}

void Atlas_phys_pub_2013_011::analyze() {
  // Your eventwise analysis code goes here
  // The following objects are always defined unless they are 'ignored' above. They form std::vector objects of the respective Delphes class type (except for Etmiss which is a single object)
  // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
  // Within a std::vector, all members are ordered with highest pt coming first.

  // electronsLoose, electronsMedium, electronsTight   are list of electrons that passed respective efficiency and reconstruction cuts
  // muonsCombinedPlus, muonsCombined                  as above for muons
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
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in ChekćkMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  
 // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
    
    
    electronsLoose = filterPhaseSpace(electronsLoose, 20., -2.5, 2.5);
    electronsTight = filterPhaseSpace(electronsTight, 20., -2.5, 2.5);
    electronsTight = filterIsolation(electronsTight);
//   electronsLoose = filterIsolation(electronsLoose);
    muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 10., -2.4, 2.4);
    muonsCombinedPlus = filterIsolation(muonsCombinedPlus);
    jets = filterPhaseSpace(jets, 25., -2.5, 2.5);
    
    //object removal
    
    
//    jets = overlapRemoval(jets, electronsLoose, 0.2);
    jets = overlapRemoval(jets, electronsLoose, 0.2);
    electronsLoose = overlapRemoval(electronsLoose, jets, 0.4);
    electronsTight = overlapRemoval(electronsTight, jets, 0.4);
    muonsCombinedPlus = overlapRemoval(muonsCombinedPlus, jets, 0.4);
    
    
    missingET->addMuons(muonsCombinedPlus);
    //one lepton signal regions
   // if(electronsLoose.size() + muonsCombinedPlus.size() == 1 && (electronsTight.size() == 1 || muonsCombinedPlus.size() == 1) ){
      if( electronsTight.size()+muonsCombinedPlus.size() == 1 ){

        
          
  
          
        countCutflowEvent("0_1lepton_>#lepton==1");
          
        if( !( jets.size() > 3 && jets[0]->PT > 80. && jets[1]->PT > 60. && jets[2]->PT > 40. && jets[3]->PT >  25. ) )
            return;
        
        countCutflowEvent("1_1lepton_>#jets>3_jetptcut");

        int bjet=0;
        for (int i=0;i<jets.size();i++){
            if(checkBTag(jets[i],0) == true) bjet=bjet+1;
        }

        

        if(bjet == 0 )
            return;

        countCutflowEvent("2_1lepton_>#bjets>0");
          
        if( fabs(jets[0]->P4().DeltaPhi(missingET->P4())) < 0.8)
            return;
        
        countCutflowEvent("3_1lepton_>deltaphicut");
        
        //etmss/h ratio
        


        // EtMiss/sqrt(HT) > 15
        // HT is sum over four jets
        double HT = 0.;
        for(int i = 0; i < jets.size(); i++){
            if( i < 4) HT += jets[i]->PT;
        }
        if( (missingET->P4().Et()/ sqrt(HT)) < 15.)
            return;
        
        countCutflowEvent("4_1lepton_>MET/sqrt(HT)<15");
          
        
        
        //m_had-top = 130-205
        /// Reconstruction was slightly too good so tighten window.
        // Try to reconstruct top mass -> use higher reco probability to match b-tagging weights procedure

        bool hadTop = false;
        
        
        TLorentzVector top_cand;
        std::vector<Jet*>  bTag_jets;
        std::vector<Jet*>  notbTag_jets;
        for(int i = 0; i < jets.size(); i++){
            if(checkBTag(jets[i],0) == true) bTag_jets.push_back(jets[i]);
            else notbTag_jets.push_back(jets[i]);
        }
        
        //need at at least two non bjets to reconstruct hadronic tops
        if(notbTag_jets.size() >1){
            for(int i = 0; i < bTag_jets.size(); i++){
                for(int j = 0; j < notbTag_jets.size(); j++){
                    for(int k = 0; k < notbTag_jets.size(); k++){
                        if (j !=k ) top_cand = bTag_jets[i]->P4() + notbTag_jets[j]->P4() + notbTag_jets[k]->P4();
                        if( (top_cand.M() >130.) && (top_cand.M() < 205.)){
                            hadTop = true;
                            break;
                        }
                    }
                }
            }
        }
        
        
        if(hadTop==1){
            
            countCutflowEvent("5_1lepton_>hadronic_top");
            
            TLorentzVector lepton;
            if(electronsTight.size() == 1) lepton = electronsTight[0]->P4();
            if(muonsCombinedPlus.size() == 1) lepton = muonsCombinedPlus[0]->P4();
            double mT = AnalysisBase::mT(lepton, missingET->P4());
            
            if(missingET->P4().Et() > 400. && mT > 250. ) countSignalEvent("1leptonmstop500");
            if(missingET->P4().Et() > 450. && mT > 300. ) countSignalEvent("1leptonmstop600");
            if(missingET->P4().Et() > 500. && mT > 350. ) countSignalEvent("1leptonmstop700");
            if(missingET->P4().Et() > 550. && mT > 350. ) countSignalEvent("1leptonmstop800");
            if(missingET->P4().Et() > 600. && mT > 400. ) countSignalEvent("1leptonmstop900");
            if(missingET->P4().Et() > 700. && mT > 500. ) countSignalEvent("1leptonmstop1000");
            if(missingET->P4().Et() > 750. && mT > 550. ) countSignalEvent("1leptonmstop1100");
            if(missingET->P4().Et() > 800. && mT > 550. ) countSignalEvent("1leptonmstop1200");
        }
        
        
        
    }
    
    //
    //zero lepton signal regions
    //
    if (electronsTight.size() + muonsCombinedPlus.size() == 0 ){
    
         countCutflowEvent("0_0lepton_>leptonveto");
        
        jets = filterPhaseSpace(jets, 35., -2.5, 2.5);
        if (jets.size() < 6)
            return;
        
        countCutflowEvent("1_0lepton_>6jets");
        
        if (jets[1]->PT < 80.0)
            return;
        
        countCutflowEvent("2_0lepton_>pt(j2)>80GeV");
        
        // Require that the three highest pt jets do not lie collinear with MET
        for (int i = 0; i < 3; i++) {
            if (fabs(jets[i]->P4().DeltaPhi(missingET->P4())) < (0.2*TMath::Pi()))
                return;
        }
        
        countCutflowEvent("3_0lepton_Deltaphi");
        
        // Find b-jets
        // Veto event if 2 bjets are not found
        std::vector<Jet*> bJets, notBJets;
        for (int i=0; i < jets.size(); i++) {
            if ( (fabs(jets[i]->Eta) < 2.5) && (checkBTag(jets[i],0)) )
                bJets.push_back(jets[i]);
            else
                notBJets.push_back(jets[i]);
        }
        if (bJets.size() < 2)
            return;

        countCutflowEvent("4_0lepton_>=2bjets");
        
        // mT calculated with b jet closest to met vector and be greater
        double bMetPhi = 10.;
        int bMetInt;
        for (int b = 0; b < bJets.size(); b++) {
            double bMetPhiNew = fabs(bJets[b]->P4().DeltaPhi(missingET->P4()));
            if ( bMetPhiNew < bMetPhi) {
                bMetPhi = bMetPhiNew;
                bMetInt = b;
            }
        }

        
        double mT=AnalysisBase::mT(bJets[bMetInt]->P4(), missingET->P4());
        //------------------------------
        // Need to find three closest jets in eta phi plane to form top system.
        // B-jet information is NOT used for top mass reconstruction
        double finDist = 10.;
        double ijDist, ikDist, jkDist;
        int iNum, jNum, kNum;
        for (int i = 0; i < jets.size(); i++) {
            for (int j = 0; j < jets.size(); j++) {
                for (int k = 0; k < jets.size(); k++) {
                    if ( (i != j) && (i != k) && (j != k)) {
                        ijDist = jets[i]->P4().DeltaR(jets[j]->P4());
                        ikDist = jets[i]->P4().DeltaR(jets[k]->P4());
                        jkDist = jets[j]->P4().DeltaR(jets[k]->P4());
                        if (ikDist > ijDist)
                            ijDist = ikDist;
                        if (jkDist > ijDist)
                            ijDist = jkDist;
                        if (ijDist < finDist) {
                            finDist = ijDist;
                            iNum = i;
                            jNum = j;
                            kNum = k;
                        }
                    }
                }
            }
        }
        TLorentzVector triJet = jets[iNum]->P4() + jets[jNum]->P4() + jets[kNum]->P4();
        double triMass = triJet.M();
        if ((triMass<80.) || (triMass>270.))
            return;

        countCutflowEvent("5_0lepton_>hadronic_top1");
        
        //------------------------------
        // Find the second three closest jets in eta phi plane to form 2nd top system.
        finDist = 10.;
        int iNum2, jNum2, kNum2;
        for (int i = 0; i < jets.size(); i++) {
            for (int j = 0; j < jets.size(); j++) {
                for (int k = 0; k < jets.size(); k++) {
                    if ( (i != j) && (i != k) && (j != k) && (i != iNum) && (i != jNum) && (i != kNum) && (j != iNum) && (j != jNum) && (j != kNum) && (k != iNum) && (k != jNum) && (k != kNum) ) {
                        ijDist = jets[i]->P4().DeltaR(jets[j]->P4());
                        ikDist = jets[i]->P4().DeltaR(jets[k]->P4());
                        jkDist = jets[j]->P4().DeltaR(jets[k]->P4());
                        if (ikDist > ijDist)
                            ijDist = ikDist;
                        if (jkDist > ijDist)
                            ijDist = jkDist;
                        if (ijDist < finDist) {
                            finDist = ijDist;
                            iNum2 = i;
                            jNum2 = j;
                            kNum2 =k;
                        }
                    }
                }
            }
        }
        triJet = jets[iNum2]->P4() + jets[jNum2]->P4() + jets[kNum2]->P4();
        triMass = triJet.M();
        if ((triMass<80.) || (triMass>270.))
            return; 

        countCutflowEvent("6_0lepton_>hadronic_top2");
        
        if(missingET->P4().Et() > 400. && mT > 250. ) countSignalEvent("0leptonmstop500");
        if(missingET->P4().Et() > 500. && mT > 250. ) countSignalEvent("0leptonmstop600");
        if(missingET->P4().Et() > 550. && mT > 250. ) countSignalEvent("0leptonmstop700");
        if(missingET->P4().Et() > 650. && mT > 250. ) countSignalEvent("0leptonmstop800");
        if(missingET->P4().Et() > 700. && mT > 250. ) countSignalEvent("0leptonmstop900");
        if(missingET->P4().Et() > 800. && mT > 400. ) countSignalEvent("0leptonmstop1100");
        if(missingET->P4().Et() > 900. && mT > 450. ) countSignalEvent("0leptonmstop1200");
        
    }
    else
        return;
    


  
}

void Atlas_phys_pub_2013_011::finalize() {
  // Whatever should be done after the run goes here
}       
