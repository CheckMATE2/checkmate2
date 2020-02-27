 #include "atlas_conf_2016_096.h"
// AUTHOR: Ferdinand Eiteneuer
//  EMAIL: ferdinand.eiteneuer@rwth-aachen.de
void Atlas_conf_2016_096::initialize() {
  setAnalysisName("atlas_conf_2016_096");          
  setInformation(""
    "# ATLAS\n"
    "# ATLAS-CONF-2016-096\n"
    "# 2 or 3 leptons and ptmiss\n"
    "# sqrt(s) = 13 TeV\n"
    "# int(L) = 13.3 fb^-1\n"
  "");
  setLuminosity(13.3*units::INVFB);      
  bookSignalRegions("2LASF;2LADF;2LBSF;2LBDF;2LCSF;2LCDF;3LI;3LH");
  bookCutflowRegions("CR_Neither2or3Lep;CR2Lep;CR3Lep;CR2Lep_OppositeCharge;CR2Lep_LeptonsLargePT;CR2Lep_SameFlavour;CR2Lep_DifferentFlavour;CR2Lep_Z_Veto;CR2Lep_jetVeto;"
    "CR3Lep_passed_btagveto;CR3Lep_noSFOS_samecharge;CR3Lep_noSFOS_sameflavour;"
    "CR3Lep_mmm;CR3Lep_mme;CR3Lep_mee;CR3Lep_eee;CR3Lep_mmm_i;CR3Lep_mme_i;CR3Lep_mee_i;CR3Lep_eee_i;"
    "3e_loose_i;3e_medium_i;3e_tight_i;3m_i");
  ignore("towers"); // These won't read tower or track information from the
//ignore("tracks"); //  Delphes output branches to save computing time.
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here


}

double Z_mass = 91.1876;
//function to calculate the sum of PT for all tracks for each jet 
std::vector<double> sumPTjetTracks(std::vector<Track*> tracks, std::vector<Jet*> jets){
    
    vector<double> PT_jet_tracks(jets.size(), 0);
    vector<Track*>::size_type t;
    vector<Jet*>::size_type j;
    
    for (t=0; t!=tracks.size(); ++t) {
        for (j=0; j!=jets.size(); ++j) {
            if (jets[j]->P4().DeltaR(tracks[t]->P4()) < 0.4){
                PT_jet_tracks[j] += jets[j]->PT;
            }
        }
    }
    return PT_jet_tracks;
}

void Atlas_conf_2016_096::analyze() {
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


    //Checking how many electrons and muons there are at the beginning of analysis
    int N_mu = muonsCombined.size();
    int N_e_l = electronsLoose.size();
    int N_e_m = electronsMedium.size();
    int N_e_t = electronsTight.size();
    
    if (N_mu == 3)
        countCutflowEvent("3m_i");
    if (N_e_l == 3)
        countCutflowEvent("3e_loose_i");
    if (N_e_m == 3)
        countCutflowEvent("3e_medium_i");
    if (N_e_t == 3)
        countCutflowEvent("3e_tight_i");
    
    bool mmmflag = false;
    if ((N_e_l == 0) && (N_mu == 3)){
        countCutflowEvent("CR3Lep_mmm_i");
	mmmflag = true;
    }
    if ((N_e_l == 1) && (N_mu == 2))
        countCutflowEvent("CR3Lep_mme_i");
    if ((N_e_l == 2) && (N_mu == 1))
        countCutflowEvent("CR3Lep_mee_i");
    if ((N_e_l == 3) && (N_mu == 0))
        countCutflowEvent("CR3Lep_eee_i");
    
    
    #include <iostream>
    if (mmmflag == true)
        std::cout << 1 << " ";
    else
        std::cout << 0 << " ";
    std::cout << muonsCombined.size() << " " << electronsLoose.size() << " " << electronsMedium.size() << " " << electronsTight.size();
    
    missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

    
    electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47, false);
    electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47, false);
    electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47, false); //p.5 Electron candidates are reconstructed from an isolated electromagnetic calorimeter energy deposit matched to an ID track and are required to have abs(eta) < 2.47, a traverse momentum pt > 10 GeV

    muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5, false); 
//p5. Muon candidates are reconstructed in the region abs(eta) < 2.5 from muon spectrometer tracks matching ID tracks. All muons must have pt > 10 GeV and must pass the medium identification requirements. Must pass medium identification requirement as defined in Ref[61]??TODO??

    jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
    
    // Overlap Removal (p.6)
    //1. Any electron sharing an ID track with a muon is removed.
    electronsLoose = overlapRemoval(electronsLoose, muonsCombined, 0.05);
    electronsMedium = overlapRemoval(electronsMedium, muonsCombined, 0.05);
    electronsTight = overlapRemoval(electronsTight, muonsCombined, 0.05);

    //2 Jets within dR = 0.2 of an electron candidate are discarded
    jets = overlapRemoval(jets, electronsLoose, 0.2);
    jets = overlapRemoval(jets, electronsMedium, 0.2);
    jets = overlapRemoval(jets, electronsTight, 0.2);

    //3 Electrons within dR = 0.4 of a jet candidate are discarded
    electronsLoose = overlapRemoval(electronsLoose, jets, 0.4);
    electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
    electronsTight = overlapRemoval(electronsTight, jets, 0.4);
    
    //4
    std::vector<double> sum_PTjetTracks = sumPTjetTracks(tracks,jets);

    //Checks Condition 4
    for(vector<Jet*>::size_type j=0; j<jets.size() ; j++){
        double PTjet = jets[j]->PT;
        for(vector<Muon*>::size_type m=0 ; m < muonsCombined.size() ; m++){
            double PTmuon = muonsCombined[m]->PT;
            if (muonsCombined[m]->P4().DeltaR(jets[j]->P4()) < 0.2){
                if ((PTmuon > 0.5 * PTjet) && (PTmuon > 0.7 * sum_PTjetTracks[j])){ //TODO this gets calculated beforehand, for all cases, which lowers speed
                    jets.erase(jets.begin() + j);
                    j -= 1;
                    break;   
                }
            }
        }
    }
    
    
    // 5. Muons within deltaR = 0.4 of a jet candidate are discarded to suppress muons from semileptonic decays of c- and b-hadrons
    muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);



    electronsTight=filterIsolation(electronsTight);
    electronsMedium=filterIsolation(electronsMedium);
    muonsCombined=filterIsolation(muonsCombined);
    
    int e_count = 0;
    int mu_count = 0;
    std::vector<FinalStateObject*> leptons;
    for(int e = 0; e < electronsMedium.size(); e++) {
        FinalStateObject* lep = newFinalStateObject(electronsMedium[e]);
        leptons.push_back(lep);
        e_count += 1;
    }
    for(int m = 0; m < muonsCombined.size(); m++) {
        FinalStateObject* lep = newFinalStateObject(muonsCombined[m]);
        leptons.push_back(lep);
        mu_count += 1;
    }
    std::cout << " " << muonsCombined.size() << " " << electronsMedium.size() << endl;
    //checking for right number of leptons
    if ((leptons.size() >= 4) || (leptons.size() <= 1)){
        countCutflowEvent("CR_Neither2or3Lep");
        return;
    }
    /////////////////////////////
    //(p.7) TWO LEPTON ANALYSIS//
    /////////////////////////////

    //only 2 leptons with Opposite Charge and PT > 25,20 + jetvetos + mass constraints on mt2, m_LepLep_invariantmass)
    if (leptons.size() == 2){
        countCutflowEvent("CR2Lep");
        
        //Do they have opposite Charge?
        bool CR2Lep_OppositeCharge = false;
        if (leptons[0]->Charge * leptons[1]->Charge == -1){
            countCutflowEvent("CR2Lep_OppositeCharge");
            CR2Lep_OppositeCharge = true;
        }
        
        //Large PT's ? Leading (Subleading) Lepton > 25 (20)
        bool CR2Lep_LeptonsLargePT = false;
        double PT0=leptons[0]->PT;
        double PT1=leptons[1]->PT;
        if( (max(PT0,PT1) > 25) && (min(PT0,PT1) > 20) ){
            CR2Lep_LeptonsLargePT = true;
            countCutflowEvent("CR2Lep_LeptonsLargePT");
        }
        
        // Same Flavour or Different Flavour?
        bool SF = ( (electronsMedium.size()==2 ) || ( muonsCombined.size()==2 ));
        bool DF = !SF; 
        if (SF)
            countCutflowEvent("CR2Lep_SameFlavour");
        else
            countCutflowEvent("CR2Lep_DifferentFlavour");

        
        //jet tagging
        bool CR2Lep_jetVeto = false;
        for (int j = 0 ; j < jets.size(); j++){
            
            double jetPT = jets[j]->PT;
            bool absEta = fabs(jets[j]->Eta);
            
            //1. central light jets
            if((absEta < 2.4) && !checkBTag(jets[j])){
                if( (SF && (jetPT > 20)) || (DF & (jetPT > 30)) ){
                    CR2Lep_jetVeto = true;
                    countCutflowEvent("CR2Lep_jetVeto");
                    break;
                }
            }
            
            //2. central b-tagged jets
            if( (jetPT > 20) && (absEta < 2.4) && (checkBTag(jets[j])) ){
                CR2Lep_jetVeto = true;
                countCutflowEvent("CR2Lep_jetVeto");
                break;
            }
            
            //3. forward jets
            if( (jetPT > 30) && (2.4 < absEta) && (absEta < 4.5)){
                CR2Lep_jetVeto = true;
                countCutflowEvent("CR2Lep_jetVeto");
                break;
            }
        }
        
        //Z-veto Low resonance check
        bool CR2Lep_Z_Veto = false;
        if (fabs((leptons[0]->P4() + leptons[1]->P4()).M() - Z_mass) < 10){
            CR2Lep_Z_Veto = true;
            countCutflowEvent("CR2Lep_Z_Veto");
        }
                
        //transverse mass mt2
        double mt_2 = mT2(leptons[0]->P4(), leptons[1]->P4(), 0);
        
        if (CR2Lep_OppositeCharge && CR2Lep_LeptonsLargePT && !CR2Lep_jetVeto){
            if((SF) && (!CR2Lep_Z_Veto)){
                if ((mt_2 > 90) && (mt_2 <= 120)){
                    countSignalEvent("2LASF");
                }
                else if ((mt_2 > 120) && (mt_2 <=150)){
                    countSignalEvent("2LASF");
                    countSignalEvent("2LBSF");
                }
                else if (mt_2 > 150){
                    countSignalEvent("2LASF");
                    countSignalEvent("2LBSF");
                    countSignalEvent("2LCSF");
                }
            }
            if(DF){
                if ((mt_2 > 90) && (mt_2 <= 120)){
                        countSignalEvent("2LADF");
                }
                else if ((mt_2 > 120) && (mt_2 <=150)){
                    countSignalEvent("2LADF");
                    countSignalEvent("2LBDF");
                }
                else if (mt_2 > 150){
                    countSignalEvent("2LADF");
                    countSignalEvent("2LBDF");
                    countSignalEvent("2LCDF");
                }
            }
        }
            

    }
    

    ///////////////////////////////
    //(p.8) THREE LEPTON ANALYSIS//
    ///////////////////////////////

    if (leptons.size() == 3){
        
        countCutflowEvent("CR3Lep");
        
        //Check how many e, mu there are after cuts
        int N_e = electronsMedium.size();
        int N_mu = muonsCombined.size();
        if ((N_e == 0) && (N_mu == 3))
            countCutflowEvent("CR3Lep_mmm");
        if ((N_e == 1) && (N_mu == 2))
            countCutflowEvent("CR3Lep_mme");
        if ((N_e == 2) && (N_mu == 1))
            countCutflowEvent("CR3Lep_mee");
        if ((N_e == 3) && (N_mu == 0))
            countCutflowEvent("CR3Lep_eee");
            
        //// Check if SFOS pair present (from atlas_1402_7029) ////
        bool sfos = false;
        if (leptons[0]->Charge * leptons[1]->Charge < 0 && leptons[0]->Type == leptons[1]->Type)
            sfos = true;
        else if (leptons[0]->Charge * leptons[2]->Charge < 0 && leptons[0]->Type == leptons[2]->Type)
            sfos = true;
        else if (leptons[2]->Charge * leptons[1]->Charge < 0 && leptons[2]->Type == leptons[1]->Type)
            sfos = true;
        if (sfos)
            countCutflowEvent("CR3Lep_SFOS");
        
        if(!sfos){
            //do all leptons have the same flavour & no opposite sign)?
            if ((leptons[0]->Type == leptons[2]->Type) && (leptons[1]->Type == leptons[2]->Type)){
                countCutflowEvent("CR3Lep_noSFOS_sameflavour");
                return;
            }
            //do all leptons have the same charge & no opposite sign?
            if (leptons[0]->Charge == leptons[1]->Charge && leptons[1]->Charge == leptons[2]->Charge){
                countCutflowEvent("CR3Lep_noSFOS_samecharge");
                return;
            } //TODO redundancy

        } else {
            // Find the sfos pair with inv mass closest to the z-boson (from atlas_1402_7029)
            // if leptons = e+ e+ e+ or mu- mu- mu- etc we have quit already.
            // therefore, the variables msfos and mtThird will make sense.
            double msfos = 1E20;
            double mtThird = 0;
            double ptThird = 0;
            if (leptons[0]->Charge * leptons[1]->Charge < 0 && leptons[0]->Type == leptons[1]->Type) {
            double minv = (leptons[0]->P4() + leptons[1]->P4()).M();
            if (fabs(minv - Z_mass) < fabs(msfos - Z_mass)) {
                msfos = minv;
                mtThird = mT(leptons[2]->P4(), missingET->P4());
                ptThird = leptons[2]->PT;
            }
            }
            if (leptons[0]->Charge * leptons[2]->Charge < 0 && leptons[0]->Type == leptons[2]->Type) {
            double minv = (leptons[0]->P4() + leptons[2]->P4()).M();
            if (fabs(minv - Z_mass) < fabs(msfos - Z_mass)) {
                msfos = minv;
                mtThird = mT(leptons[1]->P4(), missingET->P4());
                ptThird = leptons[1]->PT;
            }
            }
            if (leptons[2]->Charge * leptons[1]->Charge < 0 && leptons[2]->Type == leptons[1]->Type){
            double minv = (leptons[2]->P4() + leptons[1]->P4()).M();
            if (fabs(minv - Z_mass) < fabs(msfos - Z_mass)) {
                msfos = minv;
                mtThird = mT(leptons[0]->P4(), missingET->P4());
                ptThird = leptons[0]->PT;                
            }
            }
        
            
            ////Check for b-tagged jets////
            bool btag_veto = false;
            for (int j = 0 ; j < jets.size(); j++){
                if( (jets[j]->PT > 20) && (fabs(jets[j]->Eta) < 2.4) && (checkBTag(jets[j])) ){
                    btag_veto = true;
                    break;
                }
            }
            if (btag_veto){
                countCutflowEvent("CR3Lep_failed_btag");
                return;        
            }
            
            ////Check if mtThird > 110////
            if (mtThird < 110){
                countCutflowEvent("CR3Lep_failed_mtThird");
                return;
            }
            
            //count Signal Events
            if ((msfos > 101.2) && (ptThird > 80) && (missingET->PT > 60)){
                countSignalEvent("3LH");
                return;
            }
            if ((msfos > 81.2) && (msfos < 101.2) && (ptThird > 30) && (missingET->PT > 120)){
                countSignalEvent("3LI");
                return;
            }
            ////Check Mass of SF-OS pair////
        }
    //end 3 Lep analysis
    }

//the end.
}   



void Atlas_conf_2016_096::finalize() {
  // Whatever should be done after the run goes here
}
