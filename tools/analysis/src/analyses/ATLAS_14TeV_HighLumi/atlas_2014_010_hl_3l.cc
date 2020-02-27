#include "atlas_2014_010_hl_3l.h"
// AUTHOR: Roberto Ruiz, Krzysztof Rolbiecki
//  EMAIL: rruiz@ific.uv.es
void Atlas_2014_010_hl_3l::initialize() {
    setAnalysisName("atlas_2014_010_hl_3l");          
    setInformation(""
		   "# ATLAS\n"
		   "# ATLAS-PUB-2014-010\n"
		   "# 3 leptons + etmiss (chargino+neutralino)\n"
		   "# sqrt(s) = 14 TeV\n"
		   "# int(L) = 3000 fb^-1\n"
		   "");
    setLuminosity(3000.0*units::INVFB);      
    bookSignalRegions("SRA;SRB;SRC;SRD;SRE;SRF;SRG;SRH;SR1l2tau;");
    // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
    //  always ordered alphabetically in the cutflow output files.

    // You should initialize any declared variables here
//    histo_Z = new TH1D("Z", "OSSF invariant mass", 100, 60 , 120);
//    histo_Z2 = new TH1D("Z2", "taus", 3, 0 , 3);
}

void Atlas_2014_010_hl_3l::analyze() {

    // Object selection
    missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

    electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47, false);
    muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.4, 2.4);
    jets = filterPhaseSpace(jets, 20., -2.5, 2.5);
  
    countCutflowEvent("00_all");

    // Overlap Removal
    jets = overlapRemoval(jets, electronsTight, 0.2);
    
//    if (electronsTight.size() + muonsCombined.size() != 3)
//	return;  
//    countCutflowEvent("01_3tightlep");    

    // Low resonance check
    std::vector<Electron*> noResonanceElecs;
    for (int t = 0; t < electronsTight.size(); t++) {
	bool valid = true;
	for (int m = 0; m < electronsTight.size(); m++) {
	    if (electronsTight[m]->Charge*electronsTight[t]->Charge > 0)
		continue;  // This also prevents an electron to be tested against itself
	    if ( (electronsTight[m]->P4() + electronsTight[t]->P4()).M() < 12.)
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
	    if ( (muonsCombined[m]->P4() + muonsCombined[t]->P4()).M() < 12.)
		valid = false;
	}
	if (valid)
	    noResonanceMuons.push_back(muonsCombined[t]);
    }
    electronsTight = noResonanceElecs;
    muonsCombined = noResonanceMuons;
    
//    if (electronsTight.size() + muonsCombined.size() != 3)
//	return;  
//    countCutflowEvent("02_resonance_veto");
    

    // Apply Isolaton
    electronsTight = filterIsolation(electronsTight);
    muonsCombined = filterIsolation(muonsCombined);

    //overlapRemoval
    electronsTight = overlapRemoval(electronsTight, jets, 0.4);
    muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
    
//    if (electronsTight.size() + muonsCombined.size() != 3)
//	return;  
//    countCutflowEvent("03_isolation");    
  
    // Nleptons
    // Evaluate universal bVeto condition for later
    bool bVeto = false;
    std::vector<Jet*> tauJets, noTauJets;
    for(int j = 0; j < jets.size(); j++) {
	if( checkBTag(jets[j]))
            bVeto = true;
        else if( checkTauTag(jets[j], "medium") )// and fabs(jets[j]->Charge) == 1)
            tauJets.push_back(jets[j]);
	else
	    noTauJets.push_back(jets[j]);
    }

    //histo_Z2->Fill(tauJets.size());
    //SR1l2tau
    tauJets = filterPhaseSpace(tauJets, 20., -2.47, 2.47, false); 
    tauJets = overlapRemoval(tauJets, electronsTight, 0.2);
    tauJets = overlapRemoval(tauJets, muonsCombined, 0.2);
    tauJets = overlapRemoval(tauJets, noTauJets, 0.2);

    if(bVeto)
	return;
    countCutflowEvent("04_bveto");

    // Combine leptons for SR that consider them equally 
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
    countCutflowEvent("05_3leptons");    

    // Check SR with taus
    switch (tauJets.size()) {
    case 0: { // ===============================================No Tau SR
	countCutflowEvent("WZ_06_tauveto");
	// In this case, sort leptons by pt
	std::sort(leptons.begin(), leptons.end(), FinalStateObject::sortByPT );
	// Test that leptons are separated by 0.1
	if (leptons[0]->P4().DeltaR(leptons[1]->P4()) < 0.1)
	    return;
	if (leptons[0]->P4().DeltaR(leptons[2]->P4()) < 0.1)
	    return;
	if (leptons[1]->P4().DeltaR(leptons[2]->P4()) < 0.1)
	    return;
	countCutflowEvent("WZ_07_lepton_separation");
   
	bool sfos = false;
	// Find the sfos pair with inv mass closest to the z-boson
	double msfos = 1E10;
	double mtThird = 0;
	if (leptons[0]->Charge * leptons[1]->Charge < 0 && leptons[0]->Type == leptons[1]->Type) {
	    sfos = true;
	    double minv = (leptons[0]->P4() + leptons[1]->P4()).M();
	    if (fabs(minv - 91.2) < fabs(msfos - 91.2)) {
		msfos = minv;
		mtThird = mT(leptons[2]->P4(), missingET->P4());
	    }
	}
	if (leptons[0]->Charge * leptons[2]->Charge < 0 && leptons[0]->Type == leptons[2]->Type) {
	    sfos = true;
	    double minv = (leptons[0]->P4() + leptons[2]->P4()).M();
	    if (fabs(minv - 91.2) < fabs(msfos - 91.2)) {
		msfos = minv;
		mtThird = mT(leptons[1]->P4(), missingET->P4());
	    }
	}
	if (leptons[2]->Charge * leptons[1]->Charge < 0 && leptons[2]->Type == leptons[1]->Type){
	    sfos = true;
	    double minv = (leptons[2]->P4() + leptons[1]->P4()).M();
	    if (fabs(minv - 91.2) < fabs(msfos - 91.2)) {
		msfos = minv;
		mtThird = mT(leptons[0]->P4(), missingET->P4());
	    }
	}

	bool msfos_cut = (81.2 < msfos && msfos < 101.2);
	
//	histo_Z->Fill(msfos);
//	if (msfos_cut) histo_Z2->Fill(msfos);
  
	//Signal Regions
	if(sfos) {
	  countCutflowEvent("WZ_08_OSSF");
	  if (msfos_cut) {
	    countCutflowEvent("WZ_09_Z");
	    if(leptons[0]->PT > 50. && leptons[1]->PT > 50. && leptons[2]->PT > 50.)
		{
		    countCutflowEvent("WZ_10_PT");
		    //SRA
		    if(missingET->P4().Et() > 250. && mtThird > 150.) {
			countCutflowEvent("WZ_11_SRA");
			countSignalEvent("SRA");
		    }
		    //SRB
		    if(missingET->P4().Et() > 300. && mtThird > 200.) {
			countCutflowEvent("WZ_11_SRB");
			countSignalEvent("SRB");
		    }
		    //SRC
		    if(missingET->P4().Et() > 400. && mtThird > 200.) {
			countCutflowEvent("WZ_11_SRC");
			countSignalEvent("SRC");
		    }
		    //SRD
		    if(missingET->P4().Et() > 500. && mtThird > 200.) {
			countCutflowEvent("WZ_11_SRD");
			countSignalEvent("SRD");
		    }
		}
	  }
	}

	double DRmin = 1E10;
	double minv = 0;
	if (leptons[0]->Charge * leptons[1]->Charge < 0) {
	    double DR = leptons[0]->P4().DeltaR(leptons[1]->P4());
	    if (DR < DRmin) {
		DRmin = DR;
		minv = (leptons[0]->P4() + leptons[1]->P4()).M();;
	    }
	}
	if (leptons[0]->Charge * leptons[2]->Charge < 0) {
	    double DR = leptons[0]->P4().DeltaR(leptons[2]->P4());
	    if (DR < DRmin) {
		DRmin = DR;
		minv = (leptons[0]->P4() + leptons[2]->P4()).M();;
	    }
	}
	if (leptons[2]->Charge * leptons[1]->Charge < 0){
	    double DR = leptons[2]->P4().DeltaR(leptons[1]->P4());
	    if (DR < DRmin) {
		DRmin = DR;
		minv = (leptons[1]->P4() + leptons[2]->P4()).M();;
	    }
	}

	double MT_1 = mT(leptons[0]->P4(), missingET->P4());
	double MT_2 = mT(leptons[1]->P4(), missingET->P4());
	double MT_3 = mT(leptons[2]->P4(), missingET->P4());
 
	if ( !sfos ) countCutflowEvent("Wh_06_CR_SFOSveto");
	
	if ( msfos_cut ) 
	    return;
	countCutflowEvent("Wh_06_Zveto");

	if(missingET->P4().Et() < 100.) return;
	countCutflowEvent("Wh_07_met>100");
	
	if (minv > 75.)  return;
	countCutflowEvent("Wh_08_mt<75");
	   
	if (MT_1 > 200. && MT_2 > 100. && MT_3 > 100.) {
	  countCutflowEvent("Wh_09_mT_SRE");
          countSignalEvent("SRE");
	}
	    //SRF
	if (MT_1 > 200. && MT_2 > 150. && MT_3 > 100.) {
           countCutflowEvent("Wh_09_mT_SRF");
	   countSignalEvent("SRF");
        }
	    //SRG
        if (MT_1 > 300. && MT_2 > 150. && MT_3 > 100.) {
          countCutflowEvent("Wh_09_mT_SRG");
          countSignalEvent("SRG");
	}
	    //SRH
	if (MT_1 > 400. && MT_2 > 150. && MT_3 > 100.) {
	   countCutflowEvent("Wh_09_mT_SRH");
           countSignalEvent("SRH");
	}
	break;
    }
    case 2: {  // ===============================================Two Tau SR
	countCutflowEvent("tau_06_2taus");
	// since we did not re-sort, lep[0] is the non-tau-lepton
	
	if(leptons[0]->PT < 25.) return;   
	countCutflowEvent("tau_07_pt_emu>25");

	if (leptons[1]->Charge*leptons[2]->Charge > 0) return;
	countCutflowEvent("tau_08_OS");

	if (missingET->P4().Et() < 250.) return;
	countCutflowEvent("tau_09_met>250");

	double mtautau = (leptons[1]->P4()+leptons[2]->P4()).M();
	if(mtautau < 80. or 130. < mtautau) return;
	countCutflowEvent("tau_10_mtautau");
	double sumpt = leptons[1]->PT + leptons[2]->PT;
	if(sumpt < 190.) return;
	countCutflowEvent("tau_11_pt_tautau");
	double mTl = mT(leptons[0]->P4(), missingET->P4());
	if(mTl < 130.) return;
	countSignalEvent("SR1l2tau");
	countCutflowEvent("tau_12_mT>130");
	break;
    }
    default: return;
    }
   return;

}

void Atlas_2014_010_hl_3l::finalize() {
    // Whatever should be done after the run goes here
//  TFile f1("3l_HL.root","recreate");
//  histo_Z->Write();
//  histo_Z2->Write();
//  f1.Close();  
}       

