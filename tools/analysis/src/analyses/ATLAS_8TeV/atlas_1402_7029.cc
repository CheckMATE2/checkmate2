#include "atlas_1402_7029.h"
// AUTHOR: Daniel Schmeier
//  EMAIL: daschm@uni-bonn.de

void Atlas_1402_7029::initialize() {
    setAnalysisName("atlas_1402_7029");          
    setInformation(""
           "@#ATLAS\n"
           "@#arXiv:1402.7029\n"
           "@#3 leptons and etmiss\n"
           "@#sqrt(s) = 8 Tev\n"
           "@#int(L) = 20.3 fb^-1\n"
           "");
    setLuminosity(20.3*units::INVFB);      
    ignore("towers"); // These won't read tower or track information from the
    ignore("tracks"); //  Delphes output branches to save computing time.
    bookSignalRegions("SR0taua01;SR0taua02;SR0taua03;SR0taua04;SR0taua05;SR0taua06;SR0taua07;SR0taua08;SR0taua09;SR0taua10;SR0taua11;SR0taua12;SR0taua13;SR0taua14;SR0taua15;SR0taua16;SR0taua17;SR0taua18;SR0taua19;SR0taua20;SR0taub;SR1tau;SR2taua;SR2taub;");
    // Cutflow regions are not booked as there are too many... note that an unbooked CR will appear as long as there is at least one event for it
  
    cutSingleTriggerElectronPT = 25.;
    cutSymTriggerElectronPT = 14.;
    cutAsymTriggerElectronPT1 = 25.;
    cutAsymTriggerElectronPT2 = 10.;
    cutMixedTrigger1ElectronPT = 14.;
    cutMixedTrigger2ElectronPT = 10.;  
    cutSingleTriggerMuonPT = 25.;
    cutSymTriggerMuonPT = 14.0;
    cutAsymTriggerMuonPT1 = 18.0;
    cutAsymTriggerMuonPT2 = 10.;
    cutMixedTrigger1MuonPT = 10.;
    cutMixedTrigger2MuonPT = 18.;
}

void Atlas_1402_7029::analyze() {
    // Object selection
    missingET->addMuons(muonsCombined);  
  
    electronsMedium = filterPhaseSpace(electronsMedium, 10., -2.47, 2.47, false);
    electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47, false);
    muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
    jets = filterPhaseSpace(jets, 20., -2.5, 2.5);
  
    countCutflowEvent("CR0_0All");
  
    // Overlap Removal
    electronsMedium = overlapRemoval(electronsMedium, 0.1);
    electronsTight = overlapRemoval(electronsTight, 0.1);
    jets = overlapRemoval(jets, electronsMedium, 0.2);
    electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
    electronsTight = overlapRemoval(electronsTight, jets, 0.4);
    muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);

    // Low resonance check
    std::vector<Electron*> noResonanceElecs;
    for (int t = 0; t < electronsTight.size(); t++) {
    bool valid = true;
    for (int m = 0; m < electronsMedium.size(); m++) {
        if (electronsMedium[m]->Charge*electronsTight[t]->Charge > 0)
        continue;  // This also prevents an electron to be tested against itself
        if ( (electronsMedium[m]->P4() + electronsTight[t]->P4()).M() < 12)
        valid = false;
    }
    if (valid)
        noResonanceElecs.push_back(electronsTight[t]);
    }
    std::vector<Muon*> noResonanceMuons;
    for (int t = 0; t < muonsCombined.size(); t++) {
    bool valid = true;
    for (int m = 0; m < muonsCombined.size(); m++) {
        if (muonsCombined[m]->Charge*muonsCombined[t]->Charge > 0)
        continue;  // This also prevents a muon to be tested against itself
        if ( (muonsCombined[m]->P4() + muonsCombined[t]->P4()).M() < 12)
        valid = false;
    }
    if (valid)
        noResonanceMuons.push_back(muonsCombined[t]);
    }
    electronsTight = noResonanceElecs;
    muonsCombined = noResonanceMuons;

    // Apply Isolaton
    electronsTight=filterIsolation(electronsTight);
    muonsCombined=filterIsolation(muonsCombined);
  
    //    Trigger Cuts
    bool trigger = false;
    // single electron trigger
    if( electronsTight.size() > 0 && electronsTight[0]->PT > cutSingleTriggerElectronPT )
    trigger = true;
    // single muon trigger
    else if( muonsCombined.size() > 0 && muonsCombined[0]->PT > cutSingleTriggerMuonPT  ) 
    trigger = true;
    //symmetric dielectron trigger
    else if( electronsTight.size() > 1 && electronsTight[0]->PT > cutSymTriggerElectronPT && electronsTight[1]->PT > cutSymTriggerElectronPT  )
    trigger = true;
    //asymmetric dielectron trigger
    else if( electronsTight.size() > 1 && electronsTight[0]->PT > cutAsymTriggerElectronPT1 && electronsTight[1]->PT > cutAsymTriggerElectronPT2 )
    trigger = true;
    //symmetric dimuon trigger
    else if( muonsCombined.size() > 1 && muonsCombined[0]->PT > cutSymTriggerMuonPT && muonsCombined[1]->PT > cutSymTriggerMuonPT )
    trigger = true;
  
    //asymmetric dimuon trigger
    else if( muonsCombined.size() > 1 && muonsCombined[0]->PT > cutAsymTriggerMuonPT1 && muonsCombined[1]->PT > cutAsymTriggerMuonPT2 )
    trigger=true;
    //mixed electron-muon trigger
    else if( electronsTight.size() > 0 && muonsCombined.size() > 0 && electronsTight[0]->PT > cutMixedTrigger1ElectronPT && muonsCombined[0]->PT > cutMixedTrigger1MuonPT  )
    trigger = true;
    //mixed electron-muon trigger
    else if( electronsTight.size() > 0 && muonsCombined.size() > 0 && electronsTight[0]->PT > cutMixedTrigger2ElectronPT && muonsCombined[0]->PT > cutMixedTrigger2MuonPT)
    trigger = true;

    if( !trigger )
      return;
    countCutflowEvent("CR0_1Trigger");

    //object removal is applied
    //--------------------------
    //------Signal Regions------
    //--------------------------

    // Nleptons
    // Evaluate universal bVeto condition for later
    bool bVeto = false;
    std::vector<Jet*> tauJets;
    for(int j = 0; j < jets.size(); j++) {
       if( checkBTag(jets[j]))
            bVeto = true;
        else if( checkTauTag(jets[j], "medium")  and fabs(jets[j]->Charge) == 1)
            tauJets.push_back(jets[j]);
     }


    // Combine leptons for SR that consider them equally (note that in this definition, tauJets always come last and there is no pt ordering!)
    std::vector<FinalStateObject*> leptons;
    for(int e = 0; e < electronsTight.size(); e++) {
    FinalStateObject* lep = newFinalStateObject(electronsTight[e]);
    leptons.push_back(lep);
    }  
    for(int m = 0; m < muonsCombined.size(); m++) {
    FinalStateObject* lep = newFinalStateObject(muonsCombined[m]);
    leptons.push_back(lep);
    }
    for(int t = 0; t < tauJets.size(); t++) {
    FinalStateObject* lep = newFinalStateObject(tauJets[t]);
    leptons.push_back(lep);
    }

    if (leptons.size()  != 3)
    return;  
    countCutflowEvent("CR0_2ThreeLeptons");

    // Test that leptons are separated by 0.3
    if (leptons[0]->P4().DeltaR(leptons[1]->P4()) < 0.3)
    return;
    if (leptons[0]->P4().DeltaR(leptons[2]->P4()) < 0.3)
    return;
    if (leptons[1]->P4().DeltaR(leptons[2]->P4()) < 0.3)
    return;
    countCutflowEvent("CR0_3Separated");

    if(  electronsTight.size() + muonsCombined.size() == 0 )
    return;  
    countCutflowEvent("CR0_4EorMu");

    // Check SR with taus
    switch (tauJets.size()) {
    case 2: {  // ===============================================Two Tau SR
    countCutflowEvent("CR_2_0TwoTaus");
    do { // Check taua
        if(bVeto)
        break; // NO RETURN as this region is not orthogonal to taub
        countCutflowEvent("CR_2a_1BVeto");
        if (missingET->P4().Et() < 50.)
        break;
        countCutflowEvent("CR_2a_2Etmiss");   
        // check mT2 values
        double mT2_1 = mT2(leptons[0]->P4(), leptons[1]->P4(), 0.);
        double mT2_2 = mT2(leptons[0]->P4(), leptons[2]->P4(), 0.);
        double mT2_3 = mT2(leptons[1]->P4(), leptons[2]->P4(), 0.);
        // Probe the largest of the three
        double max_mT2 = std::max(std::max(mT2_1, mT2_2), mT2_3); // > mT2_2 ? (mT2_1 > mT2_3 ? mT2_1 : mT2_3) : (mT2_2 > mT2_3 ? mT2_2 : mT2_3));

        if (max_mT2 < 100.)
        break;
        countCutflowEvent("CR_2a_3mT2"); 
        countSignalEvent("SR2taua");
        break;
    }
    while (true); // End pseudloop

    do { // Check taub
        if (tauJets[0]->Charge*tauJets[1]->Charge > 0)
        break;
        countCutflowEvent("CR_2b_1OSTaus");
        if (bVeto)
        break;
        countCutflowEvent("CR_2b_2BVeto");
        if (missingET->P4().Et() < 60.)
        break;
        countCutflowEvent("CR_2b_3Etmiss");      
        double mtautau = (tauJets[0]->P4()+tauJets[1]->P4()).M();
        if(  mtautau < 70. or 120. < mtautau)
        break;
        countCutflowEvent("CR_2b_4mtautau");
        double sumpt = tauJets[0]->PT + tauJets[1]->PT;
        if(sumpt < 110.)
        break;
        countCutflowEvent("CR_2b_5TauPT");
        countSignalEvent("SR2taub");
        break;
    }
    while (true); // End pseudoloop
    break;
    }
    case 1: { // ===============================================One Tau SR
    countCutflowEvent("CR_1_0OneTau");
    double mTL_1 = (leptons[0]->P4() + leptons[2]->P4()).M(); // leptons[2] is the tau!
    double mTL_2 = (leptons[1]->P4() + leptons[2]->P4()).M();
    double mTL = fabs(mTL_1 - 125.) < fabs(mTL_2 - 125.) ? mTL_1 : mTL_2;
    double mLL   = (leptons[0]->P4() + leptons[1]->P4()).M();
    bool samesignl = (leptons[0]->Charge*leptons[1]->Charge > 0);
        bool oppsignlt = (leptons[0]->Charge*leptons[2]->Charge < 0);
    bool zElectronVeto = (leptons[0]->Type == "electron" && leptons[1]->Type == "electron" && mLL > 81.2 && mLL < 101.2);
    double sumpt = leptons[0]->PT + leptons[1]->PT;

    // Check Sign combination
    if (!samesignl)
        return;
    countCutflowEvent("CR_1_1SSLeptons");
      
    if (!oppsignlt)
        return;
    countCutflowEvent("CR_1_2OSTau");

    // Check z Veto
    if (zElectronVeto)
        return;
    countCutflowEvent("CR_1_3Zeeveto"); 

    // Veto bs
    if (bVeto)
        return;
    countCutflowEvent("CR_1_4BVeto");

    // ETmiss
    if (missingET->P4().Et() < 50.)
        return;
    countCutflowEvent("CR_1_5Etmiss");      

    // sumpt
    if (sumpt < 70.)
        return;
    countCutflowEvent("CR_1_6SumLeptonPT");      

    if (leptons[0]->PT < 30. || leptons[1]->PT < 30.) // note that there is no pt ordering in leptons so both 0 and 1 have to be checked
        return;
    countCutflowEvent("CR_1_7SecondLeptonPT");      

    if(mTL > 120.)
        return;
    countCutflowEvent("CR_1_8LeptonMT");     
    countSignalEvent("SR1tau");     
    break;
    }
    case 0: { // ===============================================No Tau SR
    countCutflowEvent("CR_0_1NoTau");
    bool sfos = false;
    if (leptons[0]->Charge * leptons[1]->Charge < 0 && leptons[0]->Type == leptons[1]->Type)
        sfos = true;
    else if (leptons[0]->Charge * leptons[2]->Charge < 0 && leptons[0]->Type == leptons[2]->Type)
        sfos = true;
    else if (leptons[2]->Charge * leptons[1]->Charge < 0 && leptons[2]->Type == leptons[1]->Type)
        sfos = true;

    // First, let's check the SFOS-veto signal regions, because these have a shorter code
    if (!sfos) {
        countCutflowEvent("CR_0b_1nosfos");
        // We explicitly have to require that there are not three leptons of one kind
        if (leptons[0]->Type == leptons[2]->Type)
        return;
        countCutflowEvent("CR_0b_2OF");
        // We explicitly have to require that there are is no SSS combination
        if (leptons[0]->Charge == leptons[1]->Charge && leptons[1]->Charge == leptons[2]->Charge)
        return;
        countCutflowEvent("CR_0b_3noSSS");
        if (bVeto) // No bs, please
        return;
        countCutflowEvent("CR_0b_4BVeto");
        if (missingET->P4().Et() < 50.)
        return;
        countCutflowEvent("CR_0b_5Etmiss");

        double deltaPhi1 = fabs(leptons[0]->P4().DeltaPhi(leptons[2]->P4()));
        double deltaPhi2 = 0;
        if (leptons[0]->Type != leptons[1]->Type)
        deltaPhi2 = fabs(leptons[0]->P4().DeltaPhi(leptons[1]->P4()));
        else             
        deltaPhi2 = fabs(leptons[1]->P4().DeltaPhi(leptons[2]->P4()));
        double mindeltaPhi = std::min(deltaPhi1, deltaPhi2);
        if(leptons[0]->PT < 20. || leptons[1]->PT < 20. || leptons[2]->PT < 20.)
        return;
        countCutflowEvent("CR_0b_6pt3");
        if( mindeltaPhi >= 1.0)
        return;
        countCutflowEvent("CR_0b_7dphi");
        countSignalEvent("SR0taub"); 
    }
    else {
        countCutflowEvent("CR_0a_0sfos");
        // Find the sfos pair with inv mass closest to the z-boson
        double msfos = -1E10;
        double mtThird = 0;
        if (leptons[0]->Charge * leptons[1]->Charge < 0 && leptons[0]->Type == leptons[1]->Type) {
        double minv = (leptons[0]->P4() + leptons[1]->P4()).M();
        if (fabs(minv - 92.) < fabs(msfos - 92.)) {
            msfos = minv;
            mtThird = mT(leptons[2]->P4(), missingET->P4());
        }
        }
        if (leptons[0]->Charge * leptons[2]->Charge < 0 && leptons[0]->Type == leptons[2]->Type) {
        double minv = (leptons[0]->P4() + leptons[2]->P4()).M();
        if (fabs(minv - 92.) < fabs(msfos - 92.)) {
            msfos = minv;
            mtThird = mT(leptons[1]->P4(), missingET->P4());
        }
        }
        else if (leptons[2]->Charge * leptons[1]->Charge < 0 && leptons[2]->Type == leptons[1]->Type){
        double minv = (leptons[2]->P4() + leptons[1]->P4()).M();
        if (fabs(minv - 92.) < fabs(msfos - 92.)) {
            msfos = minv;
            mtThird = mT(leptons[0]->P4(), missingET->P4());
        }
        }

        // inv mass of the three lepton system
        double m3l = (leptons[0]->P4() + leptons[1]->P4() + leptons[2]->P4()).M();

        bool msfos12_40 = (12. < msfos && msfos < 40);
        bool msfos40_60 = (40. < msfos && msfos < 60);
        bool msfos60_81 = (60. < msfos && msfos < 81.2);
        bool msfos81_101 = (81.2 < msfos && msfos < 101.2);
        bool msfos101_ = (101.2 < msfos);

        bool etmiss50_90 = (50. <= missingET->PT && missingET->PT < 90.);
        bool etmiss90_ = (90. <= missingET->PT);
        bool etmiss75_ = (75. <= missingET->PT);
        bool etmiss50_75 = (50. <= missingET->PT && missingET->PT < 75.);
        bool etmiss50_135 = (50. <= missingET->PT && missingET->PT < 135.);
        bool etmiss135_   = (135. <= missingET->PT);
        bool etmiss50_210 = (50. <= missingET->PT && missingET->PT < 210.);
        bool etmiss210_  = (210. <= missingET->PT);

        bool mt0_80 = (0. <= mtThird && mtThird < 80);
        bool mt80_ = (80. <= mtThird );
        bool mt0_110 =  (0. <= mtThird && mtThird <= 110.) ;
        bool mt110_ = (110. <= mtThird);
        bool mt0_180 =  (0. <= mtThird && mtThird < 180.);
        bool mt180_ = (180. <= mtThird);
        bool mt0_120 =  (0. <= mtThird && mtThird < 120.);
        bool mt120_ = (120. <= mtThird);

        // region checks
        if (msfos12_40) {
        countCutflowEvent("CR_0a_01_1msfos");
        countCutflowEvent("CR_0a_02_1msfos");
        countCutflowEvent("CR_0a_03_1msfos");
        countCutflowEvent("CR_0a_04_1msfos");
        if(!bVeto) {
            countCutflowEvent("CR_0a_01_2bveto");
            countCutflowEvent("CR_0a_02_2bveto");
            countCutflowEvent("CR_0a_03_2bveto");
            countCutflowEvent("CR_0a_04_2bveto");
            if(etmiss50_90) {
            countCutflowEvent("CR_0a_01_3etmiss");
            if(mt0_80) {
                countCutflowEvent("CR_0a_01_4mt3");
                countSignalEvent("SR0taua01");
            }
            }
            if(etmiss90_) {
            countCutflowEvent("CR_0a_02_3etmiss");
            if(mt0_80) {
                countCutflowEvent("CR_0a_02_4mt3");
                countSignalEvent("SR0taua02");
            }
            }
            if(etmiss50_75) {
            countCutflowEvent("CR_0a_03_3etmiss");
            if(mt80_) {
                countCutflowEvent("CR_0a_03_4mt3");
                countSignalEvent("SR0taua03");
            }
            }
            if(etmiss75_) {
            countCutflowEvent("CR_0a_04_3etmiss");
            if(mt80_) {
                countCutflowEvent("CR_0a_04_4mt3");
                countSignalEvent("SR0taua04");
            }
            }
        }
        }
        if (msfos40_60) {
        countCutflowEvent("CR_0a_05_1msfos");
        countCutflowEvent("CR_0a_06_1msfos");
        countCutflowEvent("CR_0a_07_1msfos");
        countCutflowEvent("CR_0a_08_1msfos");
        if(!bVeto) {
            countCutflowEvent("CR_0a_05_2bveto");
            countCutflowEvent("CR_0a_06_2bveto");
            countCutflowEvent("CR_0a_07_2bveto");
            countCutflowEvent("CR_0a_08_2bveto");
            if(etmiss50_75) {
            countCutflowEvent("CR_0a_05_3etmiss");
            if(mt0_80) {
                countCutflowEvent("CR_0a_05_4mt3");
                if(m3l < 81.2 || m3l > 101.2) {
                countCutflowEvent("CR_0a_05_5zveto");
                countSignalEvent("SR0taua05");
                }
            }
            }
            if(etmiss75_) {
            countCutflowEvent("CR_0a_06_3etmiss");
            if(mt0_80) {
                countCutflowEvent("CR_0a_06_4mt3");
                countSignalEvent("SR0taua06");
            }
            }
            if(etmiss50_135) {
            countCutflowEvent("CR_0a_07_3etmiss");
            if(mt80_) {
                countCutflowEvent("CR_0a_07_4mt3");
                countSignalEvent("SR0taua07");
            }
            }
            if(etmiss135_) {
            countCutflowEvent("CR_0a_08_3etmiss");
            if(mt80_) {
                countCutflowEvent("CR_0a_08_4mt3");
                countSignalEvent("SR0taua08");
            }
            }
        }
        }
        if (msfos60_81) {
        countCutflowEvent("CR_0a_09_1msfos");
        countCutflowEvent("CR_0a_10_1msfos");
        countCutflowEvent("CR_0a_11_1msfos");
        countCutflowEvent("CR_0a_12_1msfos");
        if(!bVeto) {
            countCutflowEvent("CR_0a_09_2bveto");
            countCutflowEvent("CR_0a_10_2bveto");
            countCutflowEvent("CR_0a_11_2bveto");
            countCutflowEvent("CR_0a_12_2bveto");
            if(etmiss50_75) {
            countCutflowEvent("CR_0a_09_3etmiss");
            if(mt0_80) {
                countCutflowEvent("CR_0a_09_4mt3");
                if(m3l < 81.2 || m3l > 101.2) {
                countCutflowEvent("CR_0a_09_5zveto");
                countSignalEvent("SR0taua09");
                }
            }
            }
            if(etmiss50_75) {
            countCutflowEvent("CR_0a_10_3etmiss");
            if(mt80_) {
                countCutflowEvent("CR_0a_10_4mt3");
                countSignalEvent("SR0taua10");
            }
            }
            if(etmiss75_) {
            countCutflowEvent("CR_0a_11_3etmiss");
            if(mt0_110) {
                countCutflowEvent("CR_0a_11_4mt3");
                countSignalEvent("SR0taua11");
            }
            }
            if(etmiss75_) {
            countCutflowEvent("CR_0a_12_3etmiss");
            if(mt110_) {
                countCutflowEvent("CR_0a_12_4mt3");
                countSignalEvent("SR0taua12");
            }
            }
        }
        }
        if (msfos81_101) {
        countCutflowEvent("CR_0a_13_1msfos");
        countCutflowEvent("CR_0a_14_1msfos");
        countCutflowEvent("CR_0a_15_1msfos");
        countCutflowEvent("CR_0a_16_1msfos");
        if(!bVeto) {
            countCutflowEvent("CR_0a_13_2bveto");
            countCutflowEvent("CR_0a_14_2bveto");
            countCutflowEvent("CR_0a_15_2bveto");
            countCutflowEvent("CR_0a_16_2bveto");
            if(etmiss50_90) {
            countCutflowEvent("CR_0a_13_3etmiss");
            if(mt0_110) {
                countCutflowEvent("CR_0a_13_4mt3");
                if(m3l < 81.2 || m3l > 101.2) {
                countCutflowEvent("CR_0a_13_5zveto");
                countSignalEvent("SR0taua13");
                }
            }
            }
            if(etmiss90_) {
            countCutflowEvent("CR_0a_14_3etmiss");
            if(mt0_110) {
                countCutflowEvent("CR_0a_14_4mt3");
                countSignalEvent("SR0taua14");
            }
            }
            if(etmiss50_135) {
            countCutflowEvent("CR_0a_15_3etmiss");
            if(mt110_) {
                countCutflowEvent("CR_0a_15_4mt3");
                countSignalEvent("SR0taua15");
            }
            }
            if(etmiss135_) {
            countCutflowEvent("CR_0a_16_3etmiss");
            if(mt110_) {
                countCutflowEvent("CR_0a_16_4mt3");
                countSignalEvent("SR0taua16");
                if(m3l < 81.2 || m3l > 101.2)
                countCutflowEvent("CR_0a_16_5zveto");
            }
            }
        }
        }
        if (msfos101_) {
        countCutflowEvent("CR_0a_17_1msfos");
        countCutflowEvent("CR_0a_18_1msfos");
        countCutflowEvent("CR_0a_19_1msfos");
        countCutflowEvent("CR_0a_20_1msfos");
        if(!bVeto) {
            countCutflowEvent("CR_0a_17_2bveto");
            countCutflowEvent("CR_0a_18_2bveto");
            countCutflowEvent("CR_0a_19_2bveto");
            countCutflowEvent("CR_0a_20_2bveto");
            if(etmiss50_210) {
            countCutflowEvent("CR_0a_17_3etmiss");
            if(mt0_180) {
                countCutflowEvent("CR_0a_17_4mt3");
                countSignalEvent("SR0taua17");
            }
            }
            if(etmiss50_210) {
            countCutflowEvent("CR_0a_18_3etmiss");
            if(mt180_) {
                countCutflowEvent("CR_0a_18_4mt3");
                countSignalEvent("SR0taua18");
            }
            }
            if(etmiss210_) {
            countCutflowEvent("CR_0a_19_3etmiss");
            if(mt0_120) {
                countCutflowEvent("CR_0a_19_4mt3");
                countSignalEvent("SR0taua19");
            }
            }
            if(etmiss210_) {
            countCutflowEvent("CR_0a_20_3etmiss");
            if(mt120_) {
                countCutflowEvent("CR_0a_20_4mt3");
                countSignalEvent("SR0taua20");
            }
            }
        }
        }
    }
    }
    }
}

void Atlas_1402_7029::finalize() {
    // Whatever should be done after the run goes here
}       
