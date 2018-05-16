#include "atlas_conf_2016_032.h"
// AUTHOR: Anke Biekoetter
//  EMAIL: biekoetter@physik.rwth-aachen.de

template<class T> std::vector<T*> isolateLeptons(std::vector<T*> leptons, double deltaR);
template <class X, class Y> bool checkOverlap(std::vector<X*>& x,std::vector<Y*>& y);

void Atlas_conf_2016_032::initialize() {
  setAnalysisName("atlas_conf_2016_032");          
  setInformation(""
    "# Search for new phenomena using events with b-jets and a pair of \n"
    "# same-charge leptons in 3.2fb^-1 of pp collisions at sqrt(s) = 13 TeV\n"
    "# with the ATLAS detector\n"
  "");
  setLuminosity(3.2*units::INVFB);      
  bookSignalRegions("SR0;SR1;SR2;SR3;SR4;SR5;SR6;SR7");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
  hist_a_HT = new TH1F("hist_a_HT", "HT", 18, 0., 2700.);
  hist_b_MET = new TH1F("hist_b_MET", "MET", 15, 0., 600);  
  hist_c_nbjets = new TH1I("hist_c_nbjets", "nbjets", 6, -0.5, 5.5);
  hist_d_njets = new TH1I("hist_d_njets", "njets", 15, -0.5, 14.5);
  
  hist_d_njets1 = new TH1I("hist_d_njets1", "njets1", 15, -0.5, 14.5);
  hist_d_njets2 = new TH1I("hist_d_njets2", "njets2", 15, -0.5, 14.5);
  hist_d_njets3 = new TH1I("hist_d_njets3", "njets3", 15, -0.5, 14.5);
  hist_d_njets4 = new TH1I("hist_d_njets4", "njets4", 15, -0.5, 14.5);
  hist_d_njets5 = new TH1I("hist_d_njets5", "njets5", 15, -0.5, 14.5);
  hist_d_njets6 = new TH1I("hist_d_njets6", "njets6", 15, -0.5, 14.5);
}

void Atlas_conf_2016_032::analyze() {

	missingET->addMuons(muonsCombined);
	
	/////////////////////////
    //       trigger       //
    /////////////////////////
    
    if ( (muonsCombined.size() + electronsTight.size()) == 2) countCutflowEvent("a_01_beforeTrigger_2lep");
    if ( (muonsCombined.size() + electronsTight.size()) >= 3) countCutflowEvent("b_01_beforeTrigger_>=3lep");
    
    std::vector<FinalStateObject*> leptonsTriggered;
    FinalStateObject* lep;
    bool trigger = false;
    
    for( int i=0; i < electrons.size(); i++) {
    	if (electrons[i]->PT > 24){
    		lep = new FinalStateObject( electrons[i] );
        	leptonsTriggered.push_back(lep);
        	trigger = true;
        }
    }
    
    for( int i=0; i < muons.size(); i++) {
    	if (muons[i]->PT > 50){
    		lep = new FinalStateObject( muons[i] );
        	leptonsTriggered.push_back(lep);
        	trigger = true;
        }
    }
    
    for( int i=0; i < muonsCombined.size(); i++) {
    	if (muonsCombined[i]->PT < 50 && muonsCombined[i]->PT > 20) {
    		lep = new FinalStateObject( muonsCombined[i] );
        	leptonsTriggered.push_back(lep);
        	trigger = true;
        }
    }

	if (!trigger) return;
	
	if ( (muonsCombined.size() + electronsTight.size()) == 2) countCutflowEvent("a_02_afterTrigger_2lep");
    if ( (muonsCombined.size() + electronsTight.size()) >= 3) countCutflowEvent("b_02_afterTrigger_>=3lep");
    
    hist_d_njets2->Fill(jets.size(), weight);
	
    //////////////////////////////////////////////
    //                                          //
    //      PHYSICS OBJECTS RECONSTRUCTION      //
    //                                          //
    //////////////////////////////////////////////


    /////////////////////////
    //     candidates      //
    /////////////////////////
    
    std::vector<Electron*> elecSelected;
    std::vector<Muon*> muonSelected;

    // electrons
    elecSelected = filterPhaseSpace(electronsTight, 25, -2.47, 2.47, true);
    elecSelected = isolateElectrons(elecSelected,0.2);
    
    if ( (muonsCombined.size() + elecSelected.size()) == 2) countCutflowEvent("a_03_afterElecRemoval_2lep");
    if ( (muonsCombined.size() + elecSelected.size()) >= 3) countCutflowEvent("b_03_afterElecRemoval_>=3lep"); 
    
    
    // muons
    muonSelected = filterPhaseSpace(muonsCombined, 25, -2.5, 2.5);
    muonSelected = isolateMuons(muonSelected,0.3);
    muonSelected = overlapRemoval_muon_jet_tracks(muonSelected,jets,0.4,2);

	if ( (muonSelected.size() + elecSelected.size()) == 2) countCutflowEvent("a_03_afterMuonRemoval_2lep");
    if ( (muonSelected.size() + elecSelected.size()) >= 3) countCutflowEvent("b_03_afterMuonRemoval_>=3lep");  
    
    

    // jets
    jets = overlapRemoval(jets, elecSelected, 0.2, "y");
    hist_d_njets3->Fill(jets.size(), weight);
    elecSelected = overlapRemoval(elecSelected, jets, 0.4, "y");
    hist_d_njets4->Fill(jets.size(), weight);
    jets = overlapRemoval_jet_muon_tracks(jets,muonSelected,0.4,2);	
    hist_d_njets5->Fill(jets.size(), weight);
    jets = filterPhaseSpace(jets, 25, -2.5, 2.5);
    hist_d_njets6->Fill(jets.size(), weight);
    
    
    if ( (muonSelected.size() + elecSelected.size()) == 2) countCutflowEvent("a_04_afterJetRemoval_2lep");
    if ( (muonSelected.size() + elecSelected.size()) >= 3) countCutflowEvent("b_04_afterJetRemoval_>=3lep");
    
//    hist_d_njets3->Fill(jets.size(), weight);
    
    
    
    /////////////////////////
    // kinematic variables //
    /////////////////////////
    
    // number of (b-)jets
	double njets = jets.size();
	bjets = filterBJets(jets);
    double nbjets = bjets.size();
    hist_d_njets1->Fill(jets.size(), weight);

    // MET
    double MET = missingET->P4().Et();
    
    // fill muons and electrons into lepton vector and sort by pT
    std::vector<FinalStateObject*> leps;
    for( int i=0; i < elecSelected.size(); i++) {
        lep = new FinalStateObject( elecSelected[i] );
        leps.push_back(lep);
    }
    for( int i=0; i < muonSelected.size(); i++) {
        lep = new FinalStateObject( muonSelected[i] );
        leps.push_back(lep);
    }
    std::sort( leps.begin(), leps.end(), sortByPT );

    
    
    // At least one of the selected leptons must match a lepton identified in the trigger
    if (! checkOverlap(leps,leptonsTriggered)) return;
//    hist_d_njets4->Fill(jets.size(), weight);
    
    
    // HT variable
    double HT = 0;
    for(int i=0;i<jets.size();i++){
      HT+=jets[i]->PT;
    }
    double HTvar = HT;
    for (int i=0;i<leps.size();i++){
    	HTvar = HTvar + leps[i]->PT; 
    }
    
    
    if (leps.size() < 2) return;
    
//    hist_d_njets5->Fill(jets.size(), weight);
   
    
    if ( (leps.size()) == 2) countCutflowEvent("a_05_afterTriggerOverlap_2lep");
    if ( (leps.size()) >= 3) countCutflowEvent("b_05_afterTriggerOverlap_>=3lep");
    
    // either two same-sign leptons or 3 or more leptons   
    if (leps.size() < 3) {
    	if (leps[0]->Charge * leps[1]->Charge < 0) return;
    	
//    	if( leps[0]->Type == leps[1]->Type && leps[0]->Type == "electron" ){
//            double mee = ( leps[0]->P4() + leps[1]->P4() ).M();;
//            if (mee <= 15 || fabs(mee-91) <= 10 ) return;
//        }
    }
    if ( (leps.size()) == 2) countCutflowEvent("a_06_afterOSRemoval_2lep");
    if ( (leps.size()) >= 3) countCutflowEvent("b_06_afterOSRemoval_>=3lep");
    
//    hist_d_njets6->Fill(jets.size(), weight);
    
//    njets = jets.size();
//    nbjets = bjets.size();
    
    /////////////////////////
    //     histograms      //
    /////////////////////////
    hist_a_HT->Fill(HTvar, weight);
    hist_b_MET->Fill(MET, weight);
    hist_c_nbjets->Fill(nbjets, weight);
    hist_d_njets->Fill(njets, weight);
      
    // either two same-sign leptons or 3 or more leptons   
    if (leps.size() < 3) {
    	
    	if( leps[0]->Type == leps[1]->Type && leps[0]->Type == "electron" ){
            double mee = ( leps[0]->P4() + leps[1]->P4() ).M();;
            if (mee <= 15 || fabs(mee-91) <= 10 ) return;
        }
    }

	/////////////////////////
    //    signal regions   //
    /////////////////////////
    
    if (njets < 2) return;
    if (nbjets == 0) return;
    if (HTvar <= 400 ) return;
    if (MET <= 40) return;
    
    if (HTvar > 400 && HTvar < 700){

        if (nbjets == 1)
            countSignalEvent("SR0");
        else if (nbjets == 2)
            countSignalEvent("SR1");
        else 
            countSignalEvent("SR2");
        
    } else { // HTvar >= 700

        if (nbjets == 1){
            if (MET < 100) 
            	countSignalEvent("SR3");
            else 
            	countSignalEvent("SR4");
        }     
        else if (nbjets == 2) {
            if (MET < 100) 
            	countSignalEvent("SR5");
            else 
            	countSignalEvent("SR6");
        }
        else 
            countSignalEvent("SR7");
    }
  
}
///////////////////////////////////////////////////////////////////////////////////    
///////////////////////////////////////////////////////////////////////////////////
void Atlas_conf_2016_032::finalize() {
  outputHistFile(hist_a_HT);
  outputHistFile(hist_b_MET);
  outputHistFileI(hist_c_nbjets);
  outputHistFileI(hist_d_njets);
  
  outputHistFileI(hist_d_njets1);
  outputHistFileI(hist_d_njets2);
  outputHistFileI(hist_d_njets3);
  outputHistFileI(hist_d_njets4);
  outputHistFileI(hist_d_njets5);
  outputHistFileI(hist_d_njets6);
}   
///////////////////////////////////////////////////////////////////////////////////    
///////////////////////////////////////////////////////////////////////////////////

// filterBJets
std::vector<Jet*> Atlas_conf_2016_032::filterBJets(std::vector<Jet*> jets) {
    std::vector<Jet*> bJets;
    for (std::vector<Jet*>::iterator it=jets.begin(); it!=jets.end(); it++) {
        if (checkBTag(*it)) {
            bJets.push_back(*it);
        }
    }
    return bJets;
}

// check if there is an overlap between two particle vectors
template <class X, class Y>
bool checkOverlap(std::vector<X*>& x,std::vector<Y*>& y){
  bool overlap = false;
  for(int i=0;i<x.size();i++){
    for(int j=0;j<y.size();j++){
      if((*x[i])==(*y[j])) {
      	overlap=true;
      	break;
      }
    }
  }
  return overlap;
}

    //////////////////////////////////////////
    //              ISOLATION               //
    //////////////////////////////////////////
    
template<class T>
static bool isolateLepton(T* lepton, std::vector<Track*> tracks, double maxPT, double deltaR) {
    double pTsum = 0;
    for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
        if (lepton->Particle == (*it)->Particle) {
            continue;
        }
        if ((*it)->P4().DeltaR(lepton->P4()) > deltaR) {
            continue;
        }
        pTsum += (*it)->PT;
    }
    return pTsum < maxPT;
}


std::vector<Electron*> Atlas_conf_2016_032::isolateElectrons(std::vector<Electron*> electrons, double deltaR) {
    std::vector<Electron*> isolated;
    for (typename std::vector<Electron*>::iterator it=electrons.begin(); it!=electrons.end(); it++) {
        double dR = std::min(deltaR, (double)(10/((*it)->PT)));
        if (isolateLepton(*it, tracks, 0.06*((*it)->PT), dR)) {
            isolated.push_back(*it);
        }
    }
    return isolated;
}

std::vector<Muon*> Atlas_conf_2016_032::isolateMuons(std::vector<Muon*> muons, double deltaR) {
    std::vector<Muon*> isolated;
    for (typename std::vector<Muon*>::iterator it=muons.begin(); it!=muons.end(); it++) {
    	bool   iso = true;
        double dR  = std::min(deltaR, (double)(10/((*it)->PT)));
        
        for (std::vector<Track*>::iterator it2=tracks.begin(); it2!=tracks.end(); it2++) {
            if ( ((*it)->Particle != (*it2)->Particle) && ((*it2)->P4().DeltaR((*it)->P4()) < dR) ) { // any track in cone around muon
                iso = false;
            }
        }
        if (iso) isolated.push_back(*it);
    }
    return isolated;
}

// remove muon overlap with jets
static bool check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
	int nTracks = 0;
    for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
    	for(int part=0; part < jet->Particles.GetEntries(); part++){
		    if (jet->Particles.At(part) == (*it)->Particle) {
		        nTracks++;
		    }
		}
    }
    return nTracks > nTracksMin;
}


std::vector<Muon*> Atlas_conf_2016_032::overlapRemoval_muon_jet_tracks(std::vector<Muon*> muons,std::vector<Jet*> jets, double deltaR, int nTracks){
	std::vector<Muon*> isolated;
	for (typename std::vector<Muon*>::iterator it=muons.begin(); it!=muons.end(); it++) {
        double dR = std::min(deltaR, (double)(0.04 + 10/((*it)->PT)));
        bool iso = true;
        
        for (std::vector<Jet*>::iterator jet=jets.begin(); jet!=jets.end(); jet++) {
        	if (check_nTrack_jet(*jet, tracks, nTracks)) {
		    	if ((*jet)->P4().DeltaR((*it)->P4()) < dR)
		    		iso = false;
		    }
    	}
        		    
        if (iso) isolated.push_back(*it);
    }
    return isolated;
}

std::vector<Jet*> Atlas_conf_2016_032::overlapRemoval_jet_muon_tracks(std::vector<Jet*> jets,std::vector<Muon*> muons, double deltaR, int nTracks){
	std::vector<Jet*> isolated;
	bool iso = true;
	for (typename std::vector<Jet*>::iterator jet=jets.begin(); jet!=jets.end(); jet++) {
		for (std::vector<Muon*>::iterator it=muons.begin(); it!=muons.end(); it++) {
        	if (!check_nTrack_jet(*jet, tracks, nTracks)) {
        		double dy = fabs( ((*it)->P4()).Rapidity() - ((*jet)->P4()).Rapidity() );
        		double dPhi = (*jet)->P4().DeltaPhi((*it)->P4());
        		double dvarR = sqrt(dy*dy + dPhi*dPhi);
//        		std::cout << "Old def " << (*jet)->P4().DeltaR((*it)->P4()) << std::endl;
//        		std::cout << "New def " << dvarR << std::endl;
		    	if ( dvarR > deltaR)
		    		iso = false;
		    }
    	}
        		    
        if (iso) isolated.push_back(*jet);
    }
    return isolated;
}


	//////////////////////////////////////////
    //          OUTPUT HISTOGRAMS           //
    //////////////////////////////////////////

void Atlas_conf_2016_032::outputHistFile(TH1F* inHist) {
    // Assemble absolute filename
    std::string filename = "/media/anke/Daten/Master/Programme/CM2/branches/new_versions/CheckMATE-2.0.0/results/hist/"+std::string(inHist->GetTitle())+".dat";
    
    std::cout << "Writing file:" << std::endl;
    std::cout << filename << std::endl;
    // Open stream
    std::ofstream* file = new ofstream(filename.c_str());
    // Write standard information
    *file << "# " << inHist->GetTitle() << " \n";
    *file << "#  Bin    MC_Events    (Error)    Normalised_Events    (Error)    Histogram_Normalised_Events    (Error)    Bin_Width    \n";
    for ( int i=1; i<=inHist->GetNbinsX(); i++)
    	if (i < inHist->GetNbinsX())
      		*file << inHist->GetBinCenter(i) << "    " << inHist->GetBinContent(i) << "    " << inHist->GetBinError(i) << "    " 
      			<< normalize(inHist->GetBinContent(i)) << "    " << normalize(inHist->GetBinError(i)) << "    " 
      			<< (inHist->GetBinContent(i))/(inHist->Integral()) << "    " << (inHist->GetBinError(i))/(inHist->Integral()) << "    " 
      			<< inHist->GetBinWidth(i) << std::endl;
      	else if (i == inHist->GetNbinsX())
      		*file << inHist->GetBinCenter(i) << "    " << inHist->GetBinContent(i) + inHist->GetBinContent(i+1) << "    " 
      			<< inHist->GetBinError(i) << "    " << normalize(inHist->GetBinContent(i) + inHist->GetBinContent(i)) << "    " 
      			<< normalize(inHist->GetBinError(i)) << "    " << (inHist->GetBinContent(i) + inHist->GetBinContent(i))/(inHist->Integral()) 
      			<< "    " << (inHist->GetBinError(i))/(inHist->Integral()) << "    " << inHist->GetBinWidth(i) << std::endl;
    
    file->close();
}

void Atlas_conf_2016_032::outputHistFileI(TH1I* inHist) {
    // Assemble absolute filename
    std::string filename = "/media/anke/Daten/Master/Programme/CM2/branches/new_versions/CheckMATE-2.0.0/results/hist/"+std::string(inHist->GetTitle())+".dat";
    
    std::cout << "Writing file:" << std::endl;
    std::cout << filename << std::endl;
    // Open stream
    std::ofstream* file = new ofstream(filename.c_str());
    // Write standard information
    *file << "# " << inHist->GetTitle() << " \n";
    *file << "#  Bin    MC_Events    (Error)    Normalised_Events    (Error)    Histogram_Normalised_Events    (Error)    Bin_Width    \n";
    for ( int i=1; i<=inHist->GetNbinsX(); i++)
    	if (i < inHist->GetNbinsX())
      		*file << inHist->GetBinCenter(i) << "    " << inHist->GetBinContent(i) << "    " << inHist->GetBinError(i) << "    " 
      			<< normalize(inHist->GetBinContent(i)) << "    " << normalize(inHist->GetBinError(i)) << "    " 
      			<< (inHist->GetBinContent(i))/(inHist->Integral()) << "    " << (inHist->GetBinError(i))/(inHist->Integral()) << "    " 
      			<< inHist->GetBinWidth(i) << std::endl;
      	else if (i == inHist->GetNbinsX())
      		*file << inHist->GetBinCenter(i) << "    " << inHist->GetBinContent(i) + inHist->GetBinContent(i+1) << "    " 
      			<< inHist->GetBinError(i) << "    " << normalize(inHist->GetBinContent(i) + inHist->GetBinContent(i)) << "    " 
      			<< normalize(inHist->GetBinError(i)) << "    " << (inHist->GetBinContent(i) + inHist->GetBinContent(i))/(inHist->Integral()) 
      			<< "    " << (inHist->GetBinError(i))/(inHist->Integral()) << "    " << inHist->GetBinWidth(i) << std::endl;
    
    file->close();
}
