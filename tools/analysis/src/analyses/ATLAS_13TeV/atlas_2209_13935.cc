#include "atlas_2209_13935.h"
#include "MVAUtils/BDT.h"
#include "TTree.h"
// AUTHOR: Inaki Lara
//  EMAIL: i.laraperez@uw.edu.pl
void Atlas_2209_13935::initialize() {
  setAnalysisName("atlas_2209_13935");          
  setInformation(""
    "# A search for the electroweak production of pairs of charged sleptons or charginos decaying intotwo-lepton final states with missing transverse momentum is presented. Two simplified modelsof 𝑅-parity-conserving supersymmetry are considered: direct pair-production of sleptons,with each decaying into a charged lepton and a neutralino, and direct pair-production ofthe lightest chargino, with each decaying into a 𝑊-boson and a neutralino. The lightest neutralino is assumed to be the lightest supersymmetric particle (LSP). The analysestarget the experimentally challenging mass regions where the mass differences between the selpton and chargino and the neutralino  are close to the 𝑊-boson mass (‘moderately compressed’ regions). The search uses 139 fb^−1 of CME= 13 TeV proton–proton collisions recorded by the ATLAS detector at the Large HadronCollider. No significant excesses over the expected background are observed. Exclusion limitson the simplified models under study are reported in the slepton-neutralino and chargino-neutralino mass planes at95% confidence level (CL). Sleptons with masses up to 150 GeV are excluded at 95% CL forthe case of a mass-splitting between sleptons and the LSP of 50 GeV. Chargino masses up to140 GeV are excluded at 95% CL for the case of a mass-splitting between the chargino and theLSP down to about 100 GeV.\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("SR-0J-inc-100;SR-0J-inc-110;SR-0J-inc-120;SR-0J-inc-130;SR-0J-inc-140;SR-0J-bin-100;SR-0J-bin-105;SR-0J-bin-110;SR-0J-bin-115;SR-0J-bin-120;SR-0J-bin-125;SR-0J-bin-130;SR-0J-bin-140;SR-1J-inc-100;SR-1J-inc-110;SR-1J-inc-120;SR-1J-inc-130;SR-1J-inc-140;SR-1J-bin-100;SR-1J-bin-105;SR-1J-bin-110;SR-1J-bin-115;SR-1J-bin-120;SR-1J-bin-125;SR-1J-bin-130;SR-1J-bin-140;SR-BDT-DF-inc-81;SR-BDT-DF-inc-82;SR-BDT-DF-inc-83;SR-BDT-DF-inc-84;SR-BDT-DF-inc-85;SR-BDT-SF-inc-77;SR-BDT-SF-inc-78;SR-BDT-SF-inc-79;SR-BDT-SF-inc-80;SR-BDT-DF-bin-8100;SR-BDT-DF-bin-8125;SR-BDT-DF-bin-8150;SR-BDT-DF-bin-8175;SR-BDT-DF-bin-8200;SR-BDT-DF-bin-8225;SR-BDT-DF-bin-8250;SR-BDT-DF-bin-8275;SR-BDT-DF-bin-8300;SR-BDT-DF-bin-8325;SR-BDT-DF-bin-8350;SR-BDT-DF-bin-8375;SR-BDT-DF-bin-8400;SR-BDT-DF-bin-8450;SR-BDT-DF-bin-8500;SR-BDT-DF-bin-8600;SR-BDT-SF-bin-7700;SR-BDT-SF-bin-7750;SR-BDT-SF-bin-7800;SR-BDT-SF-bin-7850;SR-BDT-SF-bin-7900;SR-BDT-SF-bin-7950;SR-BDT-SF-bin-8000;SR-BDT-SF-bin-8100");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.
  bookCutflowRegions("Cutflow-OJ-01-total;Cutflow-OJ-02-2l;Cutflow-OJ-03-ptl1>27;Cutflow-OJ-04-nj<2;Cutflow-OJ-05-ptl2>20;Cutflow-OJ-06-SF;Cutflow-OJ-07-OS;Cutflow-OJ-08-ml>11;Cutflow-OJ-09-ml<76>106;Cutflow-OJ-10-ETsig>7;Cutflow-OJ-11-nj0;Cutflow-OJ-12-ptllboost<5;Cutflow-OJ-13-costhetall<02;Cutflow-OJ-14-deltaphi_ptmissl1>22;Cutflow-OJ-15-deltaphill>22;Cutflow-OJ-16-ptl1>140;Cutflow-1J-01-total;Cutflow-1J-02-2l;Cutflow-1J-03-ptl1>27;Cutflow-1J-04-nj<2;Cutflow-1J-05-ptl2>20;Cutflow-1J-06-SF;Cutflow-1J-07-OS;Cutflow-1J-08-ml>11;Cutflow-1J-09-ml<76>106;Cutflow-1J-10-ETsig>7;Cutflow-1J-11-nj1;Cutflow-1J-12-nb0;Cutflow-1J-13-mll>60;Cutflow-1J-14-costhetall<01;Cutflow-1J-15-deltaphill>22;Cutflow-1J-16-ptl2>50;Cutflow-1J-17-ptl2>100;Cutflow-SF-01-total;Cutflow-SF-02-2l;Cutflow-SF-03-ptl1>27;Cutflow-SF-04-nj<2;Cutflow-SF-05-ml>11;Cutflow-SF-06-OS;Cutflow-SF-07-ETsig>8;Cutflow-SF-08-m0T2>50;Cutflow-SF-09-nj0;Cutflow-SF-10-SF;Cutflow-SF-11-ml<76>106;Cutflow-SF-12-BDTsignal;Cutflow-SF-13-BDTsignalother;Cutflow-DF-01-total;Cutflow-DF-02-2l;Cutflow-DF-03-ptl1>27;Cutflow-DF-04-nj<2;Cutflow-DF-05-ml>11;Cutflow-DF-06-OS;Cutflow-DF-07-ETsig>8;Cutflow-DF-08-m0T2>50;Cutflow-DF-09-nj0;Cutflow-DF-10-DF;Cutflow-DF-11-BDTsignal");
  // You should initialize any declared variables here
//  f1.open("2016_050.txt",std::ofstream::trunc);
//  f1.close();
  
  char *a = Global::get_maindir();
  std::string maindir(a, strlen(a));
  
  
  MVAUtilsReader("lgbm_DF",  maindir  + std::string("/data/atlas_2209_13935/ANA-SUSY-2019-02_DF0J_trained_odd.root"), maindir  + std::string("/data/atlas_2209_13935/ANA-SUSY-2019-02_DF0J_trained_even.root"), bdt_DF);
  MVAUtilsReader("lgbm_SF",  maindir  + std::string("/data/atlas_2209_13935/ANA-SUSY-2019-02_SF0J_trained_odd.root"), maindir  + std::string("/data/atlas_2209_13935/ANA-SUSY-2019-02_SF0J_trained_even.root"), bdt_SF);  
  
  eventNumber = 0;
}

void Atlas_2209_13935::analyze() {
  eventNumber++;
//f1.open("2016_050.txt",std::ofstream::app);
  signal_el.clear();
  signal_mu.clear();
  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out. Probably not since 3.4.2
//--------------------Lepton object definition--------------------
  electronsLoose=filterPhaseSpace( electronsLoose, 9., -2.47, 2.47);
  electronsTight=filterPhaseSpace( electronsTight, 9., -2.47, 2.47);
  muonsCombined =filterPhaseSpace(muonsCombined, 9., -2.6, 2.6);
//--------------------Calorimeter filtering----------------------------------------
  electronsLoose=filterIsolation(electronsLoose);
  electronsTight=filterIsolation(electronsTight);
  muonsCombined =filterIsolation(muonsCombined);
  signal_el = Isolate_leptons_with_inverse_track_isolation_cone(electronsTight, tracks, towers, 0.2, 10., 0.2, 0.15, 0.2, false);
  signal_mu = Isolate_leptons_with_inverse_track_isolation_cone(muonsCombined, tracks, towers, 0.3, 10., 0.2, 0.15, 1., false); 
  

  
//--------------------Jets------------------------------------------------------------
  jets = filterPhaseSpace(jets, 20., -2.4, 2.4);
  std::vector<Jet*> baseline_jets=filterPhaseSpace(jets, 20., -2.4, 2.4);
//--------------------Overlap cleaning----------------------------------------
  jets = overlapRemoval(jets, electronsLoose, 0.2);
  jets = overlapRemoval_muon_jet_tracks(jets, muonsCombined, 0.4, 3);
  signal_el = specialoverlap( signal_el, jets);
  signal_mu = specialoverlap( signal_mu, jets);

//--------------------identify b-jets------------------------------------------------------------
  std::vector<Jet*> bjets={};
  std::vector<Jet*> nonbjets={};
  for (int i = 0; i < jets.size(); i++) 
    if ( fabs(jets[i]->Eta) < 2.5 and checkBTag(jets[i],0) ) 
      bjets.push_back(jets[i]);
    else nonbjets.push_back(jets[i]);  
//--------------------signal leptons container----------------------------------------
  std::vector<FinalStateObject*> leptons;
  for ( int i = 0; i <  signal_el.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(signal_el[i]);
    leptons.push_back(lep);
  }
  for ( int i = 0; i < signal_mu.size(); i++ ) {
    FinalStateObject* lep = newFinalStateObject(signal_mu[i]);
    leptons.push_back(lep);
  }
  std::sort(leptons.begin(), leptons.end(), FinalStateObject::sortByPT);  
//--------------------ptmiss------------------------------------------------------------
  TLorentzVector pTmiss_soft = TLorentzVector(0., 0., 0., 0.);
  TLorentzVector pTmiss = TLorentzVector(0., 0., 0., 0.);
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
    bool used = false;
    for (int i = 0; i < electronsLoose.size(); i++) {
      if ( (*it)->Particle == electronsLoose[i]->Particle )
        used = true;
        break;      
    }
    for (int i = 0; i < muonsCombined.size() && !used; i++) {
      if ( (*it)->Particle == muonsCombined[i]->Particle )
        used = true;
        break;      
    }        
    for (int i = 0; i < baseline_jets.size() && !used; i++)
      for (int part = 0; part < baseline_jets[i]->Particles.GetEntries(); part++) 
        if ((*it)->PT > 0.4 && baseline_jets[i]->Particles.At(part) == (*it)->Particle) {
          used = true;
          break;
        }   
        
    for (int i = 0; i < photonsLoose.size() && !used; i++)
      for (int part = 0; part < photonsLoose[i]->Particles.GetEntries(); part++) 
        if ( photonsLoose[i]->Particles.At(part) == (*it)->Particle) {
          used = true;
          break;
        }       
    
    if (!used)  pTmiss_soft -= (*it)->P4();
  }
  //f1<<"s("<<pTmiss_soft.Px()<<','<<pTmiss_soft.Py()<<')'<<"\n";
  pTmiss = pTmiss_soft;
  //resolution = 0.;
  for (int i = 0; i <  jets.size(); i++) {
    pTmiss -=  jets[i]->P4();  
    //f1<<"j("<<-baseline_jets[i]->P4().Px()<<','<<-baseline_jets[i]->P4().Py()<<')'<<"\n"; 
      }
  for (int i = 0; i <  electronsTight.size(); i++){
    pTmiss -=  electronsTight[i]->P4();
    //f1<<"e("<<-electronsLoose[i]->P4().Px()<<','<<-electronsLoose[i]->P4().Py()<<')'<<"\n";
       }
  for (int i = 0; i < muonsCombined.size(); i++){
    pTmiss -= muonsCombined[i]->P4();  
   // f1<<"m("<<-muonsCombined[i]->P4().Px()<<','<<-muonsCombined[i]->P4().Py()<<')'<<"\n";
       }
  for (int i = 0; i < photonsLoose.size(); i++){
    pTmiss -= photonsLoose[i]->P4();  
    //f1<<"p("<<-photonsLoose[i]->P4().Px()<<','<<-photonsLoose[i]->P4().Py()<<')'<<"\n";
      }
  pTmiss.SetPz(0);
  pTmiss.SetE(pTmiss.Pt());
  //f1<<"T("<<pTmiss.Px()<<','<<pTmiss.Py()<<')'<<"\n";
//--------------------emiss significance----------------------------------------
  std::vector<FinalStateObject*> objects;
  for ( int i = 0; i <  signal_el.size(); i++ ) {
    FinalStateObject* obj = newFinalStateObject(signal_el[i]);
    objects.push_back(obj);
  }
  for ( int i = 0; i < signal_mu.size(); i++ ) {break;
    FinalStateObject* obj = newFinalStateObject(signal_mu[i]);
    objects.push_back(obj);
  }
  for ( int i = 0; i < jets.size(); i++ ) {break;
    FinalStateObject* obj = newFinalStateObject(jets[i]);
    objects.push_back(obj);
  }
  double ETmiss_S= std::abs(calcMETSignificance(objects,pTmiss.Pt()));
 //f1<<missingET->P4().Et()<<" "<<pTmiss.Et()<<" "<<pTmiss.Pt()<<"\n";

//-------------------preselection---------------------------------------------- 
countCutflowEvent("Cutflow-OJ-01-total");
countCutflowEvent("Cutflow-1J-01-total");
countCutflowEvent("Cutflow-SF-01-total");
countCutflowEvent("Cutflow-DF-01-total");
int nlep=leptons.size();



if (leptons.size()<2){return;}
else{
if (leptons.size()!=2){return;}
countCutflowEvent("Cutflow-OJ-02-2l");
countCutflowEvent("Cutflow-1J-02-2l");
countCutflowEvent("Cutflow-SF-02-2l");
countCutflowEvent("Cutflow-DF-02-2l");
if ((leptons[0]->PT)<=27){return;}
countCutflowEvent("Cutflow-OJ-03-ptl1>27");
countCutflowEvent("Cutflow-1J-03-ptl1>27");
countCutflowEvent("Cutflow-SF-03-ptl1>27");
countCutflowEvent("Cutflow-DF-03-ptl1>27");
if (jets.size()>=2){return;}
countCutflowEvent("Cutflow-OJ-04-nj<2");
countCutflowEvent("Cutflow-1J-04-nj<2");
countCutflowEvent("Cutflow-SF-04-nj<2");
countCutflowEvent("Cutflow-DF-04-nj<2");
//////          --------------  0J/1J ----------------------------------------------------
if (leptons[1]->PT>20){
	countCutflowEvent("Cutflow-OJ-05-ptl2>20");
	countCutflowEvent("Cutflow-1J-05-ptl2>20");
	if(leptons[0]->Type==leptons[1]->Type){	
		countCutflowEvent("Cutflow-OJ-06-SF");
		countCutflowEvent("Cutflow-1J-06-SF");
		if(leptons[0]->Charge*leptons[1]->Charge==-1){	
			countCutflowEvent("Cutflow-OJ-07-OS");
			countCutflowEvent("Cutflow-1J-07-OS");
			if((leptons[0]->P4()+leptons[1]->P4()).M()>11){	
				countCutflowEvent("Cutflow-OJ-08-ml>11");
				countCutflowEvent("Cutflow-1J-08-ml>11");
				if(((leptons[0]->P4()+leptons[1]->P4()).M()>106)||((leptons[0]->P4()+leptons[1]->P4()).M()<76)){	
					countCutflowEvent("Cutflow-OJ-09-ml<76>106");
					countCutflowEvent("Cutflow-1J-09-ml<76>106");
					if(ETmiss_S>7){	
						countCutflowEvent("Cutflow-OJ-10-ETsig>7");
						countCutflowEvent("Cutflow-1J-10-ETsig>7");
						if(jets.size()==0){countCutflowEvent("Cutflow-OJ-11-nj0");
							double ptllboost=(leptons[0]->P4()+leptons[1]->P4()+pTmiss).Pt();
							//cout<<ptllboost<<" "<<(leptons[0]->P4()+leptons[1]->P4()).Pt()<<" "<<pTmiss.Pt()<<" "<<missingET->P4().Et()<<"\n";
							if((leptons[0]->P4()+leptons[1]->P4()+pTmiss).Pt()<5){countCutflowEvent("Cutflow-OJ-12-ptllboost<5");
								//f1<<std::tanh(std::abs(leptons[0]->P4().PseudoRapidity()-leptons[1]->P4().PseudoRapidity())/2)<<"\n";
								if(std::tanh(std::abs(leptons[0]->P4().PseudoRapidity()-leptons[1]->P4().PseudoRapidity())/2)<0.2){countCutflowEvent("Cutflow-OJ-13-costhetall<02");
									if(std::abs(leptons[0]->P4().DeltaPhi(pTmiss))>2.2){countCutflowEvent("Cutflow-OJ-14-deltaphi_ptmissl1>22");
										//f1<<(leptons[0]->P4().DeltaPhi(leptons[1]->P4()))<<"\n";
										if(std::abs(leptons[0]->P4().DeltaPhi(leptons[1]->P4()))>2.2){countCutflowEvent("Cutflow-OJ-15-deltaphill>22");
											if(leptons[0]->PT>140){countCutflowEvent("Cutflow-OJ-16-ptl1>140");
												double mT100 = mT2(leptons[0]->P4(), leptons[1]->P4(), 100, pTmiss);
												//Inclusive
												if(mT100>=100){countSignalEvent("SR-0J-inc-100");}
												if(mT100>=110){countSignalEvent("SR-0J-inc-110");}
												if(mT100>=120){countSignalEvent("SR-0J-inc-120");}
												if(mT100>=130){countSignalEvent("SR-0J-inc-130");}
												if(mT100>=140){countSignalEvent("SR-0J-inc-140");}
												//Binned
												if(mT100>=100&&mT100<105){countSignalEvent("SR-0J-bin-100");}
												if(mT100>=105&&mT100<110){countSignalEvent("SR-0J-bin-105");}
												if(mT100>=110&&mT100<115){countSignalEvent("SR-0J-bin-110");}
												if(mT100>=115&&mT100<120){countSignalEvent("SR-0J-bin-115");}
												if(mT100>=120&&mT100<125){countSignalEvent("SR-0J-bin-120");}
												if(mT100>=125&&mT100<130){countSignalEvent("SR-0J-bin-125");}												
												if(mT100>=130&&mT100<140){countSignalEvent("SR-0J-bin-130");}
												if(mT100>=140){countSignalEvent("SR-0J-bin-140");}
											}}}}}}	
						if(jets.size()==1){countCutflowEvent("Cutflow-1J-11-nj1");
							if(bjets.size()==0){countCutflowEvent("Cutflow-1J-12-nb0");
								if((leptons[0]->P4()+leptons[1]->P4()).M()>60){countCutflowEvent("Cutflow-1J-13-mll>60");
									if(std::tanh(std::abs(leptons[0]->P4().PseudoRapidity()-leptons[1]->P4().PseudoRapidity())/2)<0.1){countCutflowEvent("Cutflow-1J-14-costhetall<01");
										if(std::abs(leptons[0]->P4().DeltaPhi(leptons[1]->P4()))>2.2){countCutflowEvent("Cutflow-1J-15-deltaphill>22");
											if(leptons[1]->PT>50){countCutflowEvent("Cutflow-1J-16-ptl2>50");
												if(leptons[0]->PT>100){countCutflowEvent("Cutflow-1J-17-ptl2>100");
													double mT100 = mT2(leptons[0]->P4(), leptons[1]->P4(), 100, pTmiss);
													//Inclusive
													if(mT100>=100){countSignalEvent("SR-1J-inc-100");}
													if(mT100>=110){countSignalEvent("SR-1J-inc-110");}
													if(mT100>=120){countSignalEvent("SR-1J-inc-120");}
													if(mT100>=130){countSignalEvent("SR-1J-inc-130");}
													if(mT100>=140){countSignalEvent("SR-1J-inc-140");}
													//Binned
													if(mT100>=100&&mT100<105){countSignalEvent("SR-1J-bin-100");}
													if(mT100>=105&&mT100<110){countSignalEvent("SR-1J-bin-105");}
													if(mT100>=110&&mT100<115){countSignalEvent("SR-1J-bin-110");}
													if(mT100>=115&&mT100<120){countSignalEvent("SR-1J-bin-115");}
													if(mT100>=120&&mT100<125){countSignalEvent("SR-1J-bin-120");}
													if(mT100>=125&&mT100<130){countSignalEvent("SR-1J-bin-125");}												
													if(mT100>=130&&mT100<140){countSignalEvent("SR-1J-bin-130");}
													if(mT100>=140){countSignalEvent("SR-1J-bin-140");}													
											}}}}}}}

}}}}}}
if (leptons[1]->PT>20){
	countCutflowEvent("Cutflow-SF-05-ml>11");
	countCutflowEvent("Cutflow-DF-05-ml>11");
	if(leptons[0]->Charge*leptons[1]->Charge==-1){
		countCutflowEvent("Cutflow-SF-06-OS");
		countCutflowEvent("Cutflow-DF-06-OS");
		if(ETmiss_S>8){
			countCutflowEvent("Cutflow-SF-07-ETsig>8");
			countCutflowEvent("Cutflow-DF-07-ETsig>8");
			double mT0 = mT2(leptons[0]->P4(), leptons[1]->P4(),0, pTmiss);
			if(mT0>50){			
				countCutflowEvent("Cutflow-SF-08-m0T2>50");
				countCutflowEvent("Cutflow-DF-08-m0T2>50");
				if(jets.size()==0){
					countCutflowEvent("Cutflow-SF-09-nj0");
					countCutflowEvent("Cutflow-DF-09-nj0");	
					if(leptons[0]->Type==leptons[1]->Type){	
						countCutflowEvent("Cutflow-SF-10-SF");
							if((((leptons[0]->P4()+leptons[1]->P4()).M()>106)||((leptons[0]->P4()+leptons[1]->P4()).M()<76))){
								countCutflowEvent("Cutflow-SF-11-ml<76>106");
                                double cosTstar = fabs(std::tanh(0.5*(leptons[0]->Eta - leptons[1]->Eta)));
                                double DPhib = fabs(missingET->P4().DeltaPhi(leptons[0]->P4() + leptons[1]->P4() + pTmiss));
                                double dphiMETl1 = fabs(missingET->P4().DeltaPhi(leptons[0]->P4()));
                                double dphiMETl2 = fabs(missingET->P4().DeltaPhi(leptons[1]->P4()));
                                std::vector<double> inSF{leptons[0]->PT, leptons[1]->PT, pTmiss.Perp(), mT0, (leptons[0]->P4()+leptons[1]->P4()).M(), DPhib, dphiMETl1, dphiMETl2, cosTstar, ETmiss_S};
                                auto BDTresult = evaluateBDT(inSF, 4, bdt_SF);                                
								if(BDTresult[1] > 0.77){
                                    countCutflowEvent("Cutflow-SF-12-BDTsignal");
									if(BDTresult[3] < 0.01){countCutflowEvent("Cutflow-SF-13-BDTsignalother");
									}}}}						
					if(leptons[0]->Type!=leptons[1]->Type){	
						countCutflowEvent("Cutflow-DF-10-DF");
                        double cosTstar = fabs(std::tanh(0.5*(leptons[0]->Eta - leptons[1]->Eta)));
                        double DPhib = fabs(missingET->P4().DeltaPhi(leptons[0]->P4() + leptons[1]->P4() + pTmiss));
                        double dphiMETl1 = fabs(missingET->P4().DeltaPhi(leptons[0]->P4()));
                        double dphiMETl2 = fabs(missingET->P4().DeltaPhi(leptons[1]->P4()));
                        std::vector<double> inDF{leptons[0]->PT, leptons[1]->PT, pTmiss.Perp(), mT0, (leptons[0]->P4()+leptons[1]->P4()).M(), DPhib, dphiMETl1, dphiMETl2, cosTstar, ETmiss_S};
                        auto BDTresult = evaluateBDT(inDF, 4, bdt_DF);
                        //cout << BDTresult[0] << "   " << BDTresult[1] << "   " << BDTresult[2] << "   " << BDTresult[3] << endl;
						if(BDTresult[1] > 0.81){countCutflowEvent("Cutflow-DF-11-BDTsignal");}}					
					}}}}}
}
//f1.close();
  
}
 
void Atlas_2209_13935::finalize() {
}       


bool Atlas_2209_13935::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

double  Atlas_2209_13935::check_track_pt(Jet* jet, std::vector<Track*> tracks) {
  
  double track_pt = 0.;
  
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
       if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) track_pt += (*it)->PT;
       
  return track_pt;
    
}
std::vector<Jet*> Atlas_2209_13935::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
  std::vector<Jet*> passed;
  for (std::vector<Jet*>::iterator jet = cand_jets.begin(); jet != cand_jets.end(); jet++) {
  
    if (check_nTrack_jet(*jet, tracks, nTracks)) {
      passed.push_back(*jet);
      continue;
    }
    double dR = deltaR;
    bool iso = true;
        
    for (std::vector<Muon*>::iterator mu=cand_muons.begin(); mu!=cand_muons.end(); mu++) 
      if ((*jet)->P4().DeltaR((*mu)->P4()) < dR) {
	iso = false;
	break;
      }
        		    
    if (iso) passed.push_back(*jet);
  }
  
  return passed;
}

void  Atlas_2209_13935::rotateXY(TMatrix &mat, TMatrix &mat_new, double phi) {
  double c = cos(phi);
  double s = sin(phi);
  double cc = c*c;
  double ss = s*s;
  double cs = c*s;

  mat_new(0,0) = mat(0,0)*cc + mat(1,1)*ss - cs*(mat(1,0) + mat(0,1));
  mat_new(0,1) = mat(0,1)*cc - mat(1,0)*ss + cs*(mat(0,0) - mat(1,1));
  mat_new(1,0) = mat(1,0)*cc - mat(0,1)*ss + cs*(mat(0,0) - mat(1,1));
  mat_new(1,1) = mat(0,0)*ss + mat(1,1)*cc + cs*(mat(1,0) + mat(0,1));
}

double Atlas_2209_13935::calcMETSignificance(std::vector<FinalStateObject*> objects,double met) {

  TMatrix cov_sum(2,2);

  TLorentzVector softVec = missingET->P4();
  
  TMatrix particle_u(2,2),particle_u_rot(2,2);
  for( int i = 0; i < objects.size(); i++ ) {
    softVec += objects[i]->P4();  // soft term is everything not included in hard objects
    //double pt_reso = 0.77*pow(objects[i]->PT, -0.39);
//    double pt_reso = objects[i]->PT < 200 ? 1.85*pow(objects[i]->PT, -0.71) : 0.05;  //cf. https://cds.cern.ch/record/2630948/files/ATLAS-CONF-2018-038.pdf Fig. 3
    double pt_reso = 1.05729 - 0.452141*log(objects[i]->PT) + 0.067873*pow(log(objects[i]->PT),2) - 0.00343522*pow(log(objects[i]->PT),3);
    double phi_reso = objects[i]->PT < 100 ? 1.23*pow(objects[i]->PT, -0.95) : 0.017;
    particle_u(0,0) = pow(pt_reso*objects[i]->PT, 2);
    particle_u(1,1) = pow(phi_reso*objects[i]->PT,2);
    rotateXY(particle_u, particle_u_rot, missingET->P4().DeltaPhi(objects[i]->P4()));
    cov_sum += particle_u_rot;
  }
  
 //add soft term resolution (fixed 10 GeV)
  particle_u(0,0) = 10*10;
  particle_u(1,1) = 10*10;
  rotateXY(particle_u, particle_u_rot, missingET->P4().DeltaPhi(softVec));
  cov_sum+=particle_u_rot;  

  //calculate significance
  double varL = cov_sum(0,0);
  double varT = cov_sum(1,1);
  double covLT = cov_sum(0,1);

  double significance = 0;
  double rho = 0;
  if( varL != 0 ){
    rho = covLT / sqrt( varL * varT ) ;
    if (fabs( rho ) >= 0.9 ) rho = 0; //too large - ignore it
    significance = met/sqrt((varL*(1-pow(rho,2))));
  }
  return significance;  
    
}


void Atlas_2209_13935::MVAUtilsReader(const std::string &name,
                               const std::string fname1,
                               const std::string fname2, MVAUtils::BDT *m_bdt[2]) {//1, MVAUtils::BDT *m_bdt2) {
  TFile *f1 = TFile::Open(fname1.c_str(), "READ");
  TTree *tree1 = nullptr;
  f1->GetObject(name.c_str(), tree1);
  if (tree1 == nullptr)
    throw std::runtime_error("Did not find MVA tree");
  m_bdt[0] = new MVAUtils::BDT(tree1);
  m_bdt[1] = nullptr;
  if (fname2 != "") {
    TFile *f2 = TFile::Open(fname2.c_str(), "READ");
    TTree *tree2 = nullptr;
    f2->GetObject(name.c_str(), tree2);
    if (tree2 == nullptr)
      throw std::runtime_error("Did not find MVA tree");
    m_bdt[1] = new MVAUtils::BDT(tree2);
  }
}


std::vector<double> Atlas_2209_13935::evaluateBDT(const std::vector<double> &values,
                              int numClasses, MVAUtils::BDT *m_bdt[2]) {
  MVAUtils::BDT *bdt = m_bdt[0];
  if (m_bdt[1] && ((eventNumber % 2) == 1))
    bdt = m_bdt[1];

  std::vector<float> floatValues(values.begin(), values.end());
  auto results = bdt->GetMultiResponse(floatValues, numClasses);

  std::vector<double> doubleResult(results.begin(), results.end());
  return doubleResult;
}

template <class X, class Y>
std::vector<X*> Atlas_2209_13935::specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours) {
      // If neighbours are empty, return candidates
      if(neighbours.size() == 0)
        return candidates;
      std::vector<X*> passed_candidates;
      // Loop over candidates
      for(int i = 0; i < candidates.size(); i++) {
        bool overlap = false;
        // If a neighbour is too close, declare overlap, break and don't save candidate
        for(int j = 0; j < neighbours.size(); j++) {
          if (candidates[i]->P4().DeltaR(neighbours[j]->P4()) > 0.2 and candidates[i]->P4().DeltaR(neighbours[j]->P4()) < std::min(0.4, 0.04 + 10./candidates[i]->PT) ) {
            overlap = true;
            break;
          }
        }
        if (!overlap)
          passed_candidates.push_back(candidates[i]);
      }
      return passed_candidates;
    }
