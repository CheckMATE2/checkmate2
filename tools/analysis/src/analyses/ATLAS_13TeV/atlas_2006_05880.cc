#include "atlas_2006_05880.h"
// AUTHOR: Inaki Lara Perez
//  EMAIL: Inaki.Lara@fuw.edu.pl
void Atlas_2006_05880::initialize() {
  setAnalysisName("atlas_2006_05880");          
  setInformation(""
    "# Search for top squarks in events whith a Higgs or a Z boson.\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions( "SR-Z1A;SR-Z1B;SR-Z2A;SR-Z2B;SR-h1A;SR-h1B;SR-Z-1A-MB200;SR-Z-1A-MB250;SR-Z-1A-MB300;SR-Z-1A-MB350;SR-Z-1B-MB150;SR-Z-1B-MB300;SR-Z-1B-MB450;SR-Z-1B-MB600;SR-Z-2A-MB;SR-Z-2B-MB300_50;SR-Z-2B-MB350_50;SR-Z-2B-MB300_150;SR-Z-2B-MB350_150;SR-h-1A-MB1-10;SR-h-1A-MB2-10;SR-h-1A-MB1-12;SR-h-1A-MB2-12;SR-h-1B-MB_5_1;SR-h-1B-MB_5_2;SR-h-1B-MB_6_1;SR-h-1B-MB_6_2;SR-h-1AB-MB");
  bookCutflowRegions("CZ0-Preselection"
  ";CZ1A1-3dlep>20;CZ1A2-minv;CZ1A3-nb1;CZ1A4-nj4;CZ1A5-MET250;CZ1A6-m3lT2"
  ";CZ1B1-3dlep>20;CZ1B2-minv;CZ1B3-nb1;CZ1B4-nj5;CZ1B5-MET150;CZ1B6-pTll>150;CZ1B7-pTb1>100"
  ";CZ2A1-3dlep<20;CZ2A2-minv;CZ2A3-pTj1>150;CZ2A4-MET200;CZ2A5-pTll<50"
  ";CZ2B1-3dlep<60;CZ2B2-minv;CZ2B3-nb1;CZ2B4-MET350;CZ2B5-pTll>150"
  ";Ch0-Preselection"
  ";Ch1A1-nb4;Ch1A2-nhcand1;Ch1A3-mT150;Ch1A4-nj4;Ch1A5-S12"
  ";Ch1B1-nb4;Ch1B2-nhcand1;Ch1B3-mT150;Ch1B4-nj6;Ch1B5-S7");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here

}

void Atlas_2006_05880::analyze() {
    
 vector<GenParticle*> stable;
 stable.clear();

for (int i = 0 ; i < true_particles.size(); ++i){
    auto part = true_particles[i];
    if (part->Status != 1) continue;        //Run over only stable particles
    stable.push_back(part);
}

  /////----------- Object identification ---------------------------------------------------------------------------------------------------------------------------------------// 
  
  std::vector<Electron*>  candidate_e=filterPhaseSpace(electronsLoose, 4.5, -2.47, 2.47);
  std::vector<Muon*>  candidate_mu=filterPhaseSpace(muonsCombined, 4., -2.4, 2.4);
  std::vector<Jet*> candidate_jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
  jets = overlapRemoval(candidate_jets, candidate_mu, 0.2);
  
  std::vector<Jet*> signal_jets;
  std::vector<Jet*> signal_bjets;

  for (int i = 0; i < jets.size(); i++) {
  	if(jets[i]->PT>200){
		signal_jets.push_back(jets[i]);
		if (checkBTag(jets[i])) signal_bjets.push_back(jets[i]);
		}
	else{
		bool dr= true;
		for (int j=0;j<candidate_e.size();j++){
			if (jets[i]->P4().DeltaR(candidate_e[j]->P4())<0.2) dr=false;
		}
		if (dr){
		       signal_jets.push_back(jets[i]);
		       if (checkBTag(jets[i])) signal_bjets.push_back(jets[i]);
		}
	}
  }
  
  std::vector<Electron*> signal_e=filterPhaseSpace(electronsMedium, 4.5, -2.47, 2.47);
  std::vector<Muon*> signal_mu=filterPhaseSpace(muonsCombined, 5., -2.4, 2.4);
  std::vector<Electron*> electrons;
  std::vector<Muon*>  muons;
  for (int j=0;j<signal_e.size();j++){
  	bool dr=true;
	bool e_isol=is_track_isolated(signal_e[j],stable, 0.06, 0.2);
	for (int i = 0; i < signal_jets.size(); i++) {
		if (signal_jets[i]->P4().DeltaR(signal_e[j]->P4())<0.4) dr=false;
  }       
       if(dr) electrons.push_back(signal_e[j]);

}

  for (int j=0;j<signal_mu.size();j++){
	          bool dr=true;
		  bool mu_isol=is_track_isolated(signal_mu[j],stable, 0.06, 0.3);
		  float dr_check=std::min(0.4,0.04+signal_mu[j]->PT/10);
		          for (int i = 0; i < signal_jets.size(); i++) {
				                  if (signal_jets[i]->P4().DeltaR(signal_mu[j]->P4())<dr_check) dr=false;
						    }
			         if(dr) muons.push_back(signal_mu[j]);

  }
 
  std::vector<FinalStateObject*> leptons;
  leptons.clear();
  for(int e = 0; e < electrons.size(); e++) {
    FinalStateObject* lep = newFinalStateObject(electrons[e]);
    leptons.push_back(lep);
  }  
  for(int m = 0; m < muons.size(); m++) {
    FinalStateObject* lep = newFinalStateObject(muons[m]);
    leptons.push_back(lep);
  }
  std::sort(leptons.begin(), leptons.end(), FinalStateObject::sortByPT);

//### New deffinition of MET
    TLorentzVector pTmiss = TLorentzVector(0., 0., 0., 0.);
   //TLorentzVector  resolution = 0.;
  for (int i = 0; i < signal_jets.size(); i++) 
    pTmiss -= signal_jets[i]->P4();     
  
  for (int i = 0; i < electrons.size(); i++) 
    pTmiss -= electrons[i]->P4();    

  for (int i = 0; i < muons.size(); i++) 
    pTmiss -= muons[i]->P4();    
 std::vector<Track*> softtracks;
  TLorentzVector pTmiss_soft = TLorentzVector(0., 0., 0., 0.);
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
    bool used = false;
    for (int i = 0; i < signal_jets.size() && !used; i++){
      for (int part = 0; part < signal_jets[i]->Particles.GetEntries(); part++){
        if ((*it)->PT > 0.4 && signal_jets[i]->Particles.At(part) == (*it)->Particle) {
	  used = true;
	  break;}}
	}
    for (int i = 0; i < electrons.size() && !used; i++){
        if ((*it)->PT > 0.4 && electrons[i]->Particle == (*it)->Particle) {
	  used = true;
	  break;}
	}
    for (int i = 0; i < muons.size() && !used; i++){
        if ((*it)->PT > 0.4 && muons[i]->Particle == (*it)->Particle) {
	  used = true;
	  break;}
	}
    if (!used)  pTmiss_soft -= (*it)->P4();
    softtracks.push_back(*it);
  }
  
  //pTmiss += pTmiss_soft; 
  // Old definition
  //pTmiss= missingET->P4();
       
  pTmiss += pTmiss_soft; 
 //###    
 Double_t pT_aux=pTmiss.Pt();
 Double_t phi_aux=pTmiss.Phi();
 pTmiss.SetPtEtaPhiE(pT_aux,0,phi_aux,pT_aux);
   
    
    countCutflowEvent("00-All");
/////-----------                       ---------------------------------------------------------------------------------------------------------------------------------------//
/////----------- Event selection       ---------------------------------------------------------------------------------------------------------------------------------------//
/////-----------                       ---------------------------------------------------------------------------------------------------------------------------------------//
if (electrons.size()+muons.size()>=3) {// *** 3l selection ***
// 3 leptons 1SFOS
bool SFOS=false;
std::vector<FinalStateObject*> SFOS_system;
float inv_mass=-1;
	for (int i=0;i<electrons.size();i++){
	        for (int j=i+1;j<electrons.size();j++){
        		if (electrons[i]->Charge*electrons[j]->Charge<0) {
			SFOS=true;
				if (std::abs((electrons[i]->P4()+electrons[j]->P4()).M()-91.2)<15) {
					if (std::abs(inv_mass-91.2)>std::abs((electrons[i]->P4()+electrons[j]->P4()).M()-91.2)){
						inv_mass=(electrons[i]->P4()+electrons[j]->P4()).M();
						SFOS_system.clear();
						FinalStateObject* lep1 = newFinalStateObject(electrons[i]);
						FinalStateObject* lep2 = newFinalStateObject(electrons[j]);
						SFOS_system.push_back(lep1);
						SFOS_system.push_back(lep2);						
						}
					}
}
        	}
	}
        for (int i=0;i<muons.size();i++){
                for (int j=i+1;j<muons.size();j++){
                        if (muons[i]->Charge*muons[j]->Charge<0) {
			SFOS=true;
                        if (std::abs((muons[i]->P4()+muons[j]->P4()).M()-91.2)<15) {
					if (std::abs(inv_mass-91.2)>std::abs((muons[i]->P4()+muons[j]->P4()).M()-91.2)){
						inv_mass=(muons[i]->P4()+muons[j]->P4()).M();
						SFOS_system.clear();
						FinalStateObject* lep1 = newFinalStateObject(muons[i]);
						FinalStateObject* lep2 = newFinalStateObject(muons[j]);
						SFOS_system.push_back(lep1);
						SFOS_system.push_back(lep2);			
						}
					}
}
                }
        }
int njets=0;
for (int i=0;i<signal_jets.size();i++){
	if (signal_jets[i]->PT>30) njets++;
	}
int nbjets=0;
for (int i=0;i<signal_bjets.size();i++){
        if (signal_bjets[i]->PT>30) nbjets++;
        }
//Preselection
bool SRZ1A=true;
bool SRZ1B=true;
bool SRZ2A=true;
bool SRZ2B=true;
bool pres=true;
if (!((leptons[0]->PT>40)&&(leptons[1]->PT>20))) {pres=false;}
if (njets<3) {pres=false;}
if ( std::abs(pTmiss.Et())<50) {pres=false;}
if (pres) countCutflowEvent("CZ0-Preselection");
//----------
if (!SFOS) {pres=false;}
if (inv_mass<0) {pres=false;}
float m3lT2=0;
float pTll=0;
if(pres){
FinalStateObject* third_lep;
for (int i=0;i<leptons.size();i++){
	if (!(leptons[i]==SFOS_system[0]||leptons[i]==SFOS_system[1])) third_lep=leptons[i];
}
m3lT2= mT2(SFOS_system[0]->P4()+SFOS_system[1]->P4(),third_lep->P4(), 0);
pTll=(SFOS_system[0]->P4()+SFOS_system[1]->P4()).Pt();
}
//SRZ1A
if (leptons[2]->PT<20) SRZ1A=false;
if (pres){
if (SRZ1A) countCutflowEvent("CZ1A1-3dlep>20");
if (std::abs(inv_mass-91.2)>15) SRZ1A=false;
if (SRZ1A) countCutflowEvent("CZ1A2-minv");
if (nbjets<1) SRZ1A=false;
if (SRZ1A) countCutflowEvent("CZ1A3-nb1");
if (njets<4) SRZ1A=false;
if (SRZ1A) countCutflowEvent("CZ1A4-nj4");
if (std::abs(pTmiss.Et())<250) SRZ1A=false;
if (SRZ1A) countCutflowEvent("CZ1A5-MET250");
if (m3lT2<100) SRZ1A=false;
if (SRZ1A) countCutflowEvent("CZ1A6-m3lT2");
if (SRZ1A) countSignalEvent("SR-Z1A");
//MB SRZ1A
if (!((leptons[2]->PT<20)||(std::abs(inv_mass-91.2)>15)||(nbjets<1)||(njets<4)||(m3lT2<100))){
if ((std::abs(pTmiss.Et())<250)&&(std::abs(pTmiss.Et())>200)) countSignalEvent("SR-Z-1A-MB200");  
else if ((std::abs(pTmiss.Et())<300)&&(std::abs(pTmiss.Et())>250)) countSignalEvent("SR-Z-1A-MB250");
else if ((std::abs(pTmiss.Et())<350)&&(std::abs(pTmiss.Et())>300)) countSignalEvent("SR-Z-1A-MB300");
else if ((std::abs(pTmiss.Et())>350)) countSignalEvent("SR-Z-1A-MB350");
}
}    
//SRZ1B
if (leptons[2]->PT<20) SRZ1B=false;
if (pres){
if (SRZ1B) countCutflowEvent("CZ1B1-3dlep>20");
if (std::abs(inv_mass-91.2)>15) SRZ1B=false;
if (SRZ1B) countCutflowEvent("CZ1B2-minv");
if (nbjets<1) {SRZ1B=false;}
if (SRZ1B) countCutflowEvent("CZ1B3-nb1");
if (njets<5) SRZ1B=false;
if (SRZ1B) countCutflowEvent("CZ1B4-nj5");
if (std::abs(pTmiss.Et())<150) SRZ1B=false;
if (SRZ1B) countCutflowEvent("CZ1B5-MET150");
if (pTll<150) SRZ1B=false;
if (SRZ1B) countCutflowEvent("CZ1B6-pTll>150");
if (nbjets<1) SRZ1B=false;
else{ if(signal_bjets[0]->PT<100)SRZ1B=false;}
if (SRZ1B) countCutflowEvent("CZ1B7-pTb1>100");
if (SRZ1B) countSignalEvent("SR-Z1B");
//MB SRZ1B
if (!((leptons[2]->PT<20)||(std::abs(inv_mass-91.2)>15)||(nbjets<1)||(njets<5)||(std::abs(pTmiss.Et())<150))){
    if(!(signal_bjets[0]->PT<100)){
        if ((pTll<300)&&(pTll>150)) countSignalEvent("SR-Z-1B-MB150");  
        else if ((pTll<450)&&(pTll>300)) countSignalEvent("SR-Z-1B-MB300");
        else if ((pTll<600)&&(pTll>450))  countSignalEvent("SR-Z-1B-MB450");
        else if ((pTll>600))  countSignalEvent("SR-Z-1B-MB600");
}  }  }
    
//SRZ2A
if (leptons[2]->PT>20) SRZ2A=false;
if (pres){
if (SRZ2A) countCutflowEvent("CZ2A1-3dlep<20");
if (std::abs(inv_mass-91.2)>15) SRZ1B=false;
if (SRZ2A) countCutflowEvent("CZ2A2-minv");
if (njets<3) SRZ2A=false;
else{ if(signal_jets[0]->PT<150)SRZ2A=false;}
if (SRZ2A) countCutflowEvent("CZ2A3-pTj1>150");
if (std::abs(pTmiss.Et())<200) SRZ2A=false;
if (SRZ2A) countCutflowEvent("CZ2A4-MET200");
if (pTll>50) SRZ2A=false;
if (SRZ2A) countCutflowEvent("CZ2A5-pTll<50");
if (SRZ2A) countSignalEvent("SR-Z2A");
if (SRZ2A) countSignalEvent("SR-Z-2A-MB");
}
//SRZ2B
if (leptons[2]->PT>60) SRZ2B=false;
if (pres){
if (SRZ2B) countCutflowEvent("CZ2B1-3dlep<60");
if (std::abs(inv_mass-91.2)>15) SRZ2B=false;
if (SRZ2B) countCutflowEvent("CZ2B2-minv");
if (nbjets<1) {SRZ2B=false;}
if (SRZ2B) countCutflowEvent("CZ2B3-nb1");
if (std::abs(pTmiss.Et())<350) SRZ2B=false;
if (SRZ2B) countCutflowEvent("CZ2B4-MET350");
if (pTll<150) SRZ2B=false;
if (SRZ2B) countCutflowEvent("CZ2B5-pTll>150");
if (SRZ2B) countSignalEvent("SR-Z2B");
//MB SRZ2B
if (!((leptons[2]->PT>60)||(std::abs(inv_mass-91.2)>15)||(nbjets<1))){
if ((std::abs(pTmiss.Et())<350)&&(std::abs(pTmiss.Et())>300)&&(pTll<150)&&(pTll>50)) countSignalEvent("SR-Z-2B-MB300_50");  
else if ((std::abs(pTmiss.Et())>350)&&(pTll<150)&&(pTll>50)) countSignalEvent("SR-Z-2B-MB350_50"); 
else if ((std::abs(pTmiss.Et())<350)&&(std::abs(pTmiss.Et())>300)&&(pTll>150)) countSignalEvent("SR-Z-2B-MB300_150");  
else if ((std::abs(pTmiss.Et())>350)&&(pTll>150)) countSignalEvent("SR-Z-2B-MB350_150");  
}
}
     
     
}


if (leptons.size()==1) {// *** 1l selection ***
//
//They are using a neural network to identify Higgs boson candidate. This is beyond of what the recasting can do, but. Instead we check if there is a pair of b-jets originated from the decay of higgs bosons (using generator information). 
//
int nhcand= calc_nhcand(stable);  
double rno = rand()/(RAND_MAX+1.); 
float met=std::abs(pTmiss.Et());
float S_score=0;
S_score= calc_S_score(signal_jets,leptons,pTmiss);
bool SRh1A=true;
bool SRh1B=true;
int nbjets=0;
for (int i=0;i<signal_bjets.size();i++){
        if (signal_bjets[i]->PT>30) nbjets++;
        }  
int njets=0;
for (int i=0;i<signal_jets.size();i++){
        if (signal_jets[i]->PT>30) njets++;
        }
float mT=sqrt(2*leptons[0]->PT*std::abs(pTmiss.Et())*(1-cos(leptons[0]->P4().DeltaPhi(pTmiss))));
//Preselection
if ((candidate_e.size()+candidate_mu.size())>1) {SRh1A=false;SRh1B=false;}
if (njets<4) {SRh1A=false;SRh1B=false;}
if (nbjets<3) {SRh1A=false;SRh1B=false;}
if (SRh1A) countCutflowEvent("Ch0-Preselection");
//-----
//SRh1A
if (nbjets<4) SRh1A=false;
if (SRh1A) countCutflowEvent("Ch1A1-nb4");
if ((nhcand<1)) SRh1A=false;
if (SRh1A) countCutflowEvent("Ch1A2-nhcand1");
if (mT<150) SRh1A=false;
if (SRh1A) countCutflowEvent("Ch1A3-mT150");
int nhjets=0;
for (int i=0;i<signal_jets.size();i++){
        if (signal_jets[i]->PT>60) nhjets++;
        }
if (nhjets<4) SRh1A=false;
if (SRh1A) countCutflowEvent("Ch1A4-nj4");
if (S_score<12) SRh1A=false;
if (SRh1A) countCutflowEvent("Ch1A5-S12");
if (SRh1A) countSignalEvent("SR-h1A");
//MB SRh1A   
if (!((nbjets<4)||(mT<150)||(nhjets<4))){
if ((nhcand==1)&&(S_score<12)&&(S_score>10)) countSignalEvent("SR-h-1A-MB1-10");  
else if ((nhcand>=2)&&(S_score<12)&&(S_score>10)) countSignalEvent("SR-h-1A-MB2-10"); 
else if ((nhcand==1)&&(S_score<14)&&(S_score>12)) countSignalEvent("SR-h-1A-MB1-10");
else if ((nhcand>=2)&&(S_score<14)&&(S_score>12)) countSignalEvent("SR-h-1A-MB2-12"); 
}
    
    
//SRh1B
if (nbjets<4) SRh1B=false;
if (SRh1B) countCutflowEvent("Ch1B1-nb4");
if ((nhcand<1)) SRh1B=false;
if (SRh1B) countCutflowEvent("Ch1B2-nhcand1");
if (mT<150) SRh1B=false; 
if (SRh1B) countCutflowEvent("Ch1B3-mT150");
nhjets=0;
for (int i=0;i<signal_jets.size();i++){
        if (signal_jets[i]->PT>60) nhjets++;
        }
if (nhjets<6) SRh1B=false;
if (SRh1B) countCutflowEvent("Ch1B4-nj6");
if (S_score<7) SRh1B=false;
if (SRh1B) countCutflowEvent("Ch1B5-S7");
if (SRh1A) countSignalEvent("SR-h1B");
//MB SRh1B   
if (!((nbjets<4)||(mT<150)||(S_score<7)||(S_score>14))){
if ((nhcand==1)&&(S_score<12)&&(nhjets==5)) countSignalEvent("SR-h-1B-MB_5_1");  
else if ((nhcand>=2)&&(S_score<12)&&(nhjets==5)) countSignalEvent("SR-h-1B-MB_5_2"); 
else if ((nhcand==1)&&(S_score<14)&&(nhjets>=6)) countSignalEvent("SR-h-1B-MB_6_1");
else if ((nhcand>=2)&&(S_score<14)&&(nhjets>=6)) countSignalEvent("SR-h-1B-MB_6_2"); 
}
    //MB SRh1AB  
if (!((nbjets<4)||(mT<150)||(nhjets<4))){
if ((nhcand>1)&&(S_score>14)&&(nhjets>4)) countSignalEvent("SR-h-1AB-MB");  
}

}
    return;
}


void Atlas_2006_05880::finalize() {
  // Whatever should be done after the run goes here
}


float Atlas_2006_05880::calc_S_score(std::vector<Jet*> jets,std::vector<FinalStateObject*> leptons,TLorentzVector pTmiss){
  TMatrix cov_sum(2,2);
  TLorentzVector softVec = pTmiss;
  float met=pTmiss.Et();
  TMatrix particle_u(2,2),particle_u_rot(2,2);
    
  std::vector<FinalStateObject*> hard_objects;
  for( int i = 0; i < leptons.size(); i++ ) {
      hard_objects.push_back(leptons[i]);
  }    
    
    
    
  for( int i = 0; i < hard_objects.size(); i++ ) {
    softVec += hard_objects[i]->P4();  // soft term is everything not included in hard objects
    //double pt_reso = 0.77*pow(jets[i]->PT, -0.39);
//    double pt_reso = jets[i]->PT < 200 ? 1.85*pow(jets[i]->PT, -0.71) : 0.05;  //cf. https://cds.cern.ch/record/2630948/files/ATLAS-CONF-2018-038.pdf Fig. 3
    double pt_reso = 1.05729 - 0.452141*log(hard_objects[i]->PT) + 0.067873*pow(log(hard_objects[i]->PT),2) - 0.00343522*pow(log(hard_objects[i]->PT),3);
    double phi_reso = hard_objects[i]->PT < 100 ? 1.23*pow(hard_objects[i]->PT, -0.95) : 0.017;
    particle_u(0,0) = pow(pt_reso*hard_objects[i]->PT, 2);
    particle_u(1,1) = pow(phi_reso*hard_objects[i]->PT,2);
    rotateXY(particle_u, particle_u_rot, missingET->P4().DeltaPhi(hard_objects[i]->P4()));
    cov_sum += particle_u_rot;
  }
    
  for( int i = 0; i < jets.size(); i++ ) {
    softVec += jets[i]->P4();  // soft term is everything not included in hard objects
    //double pt_reso = 0.77*pow(jets[i]->PT, -0.39);
//    double pt_reso = jets[i]->PT < 200 ? 1.85*pow(jets[i]->PT, -0.71) : 0.05;  //cf. https://cds.cern.ch/record/2630948/files/ATLAS-CONF-2018-038.pdf Fig. 3
    double pt_reso = 1.05729 - 0.452141*log(jets[i]->PT) + 0.067873*pow(log(jets[i]->PT),2) - 0.00343522*pow(log(jets[i]->PT),3);
    double phi_reso = jets[i]->PT < 100 ? 1.23*pow(jets[i]->PT, -0.95) : 0.017;
    particle_u(0,0) = pow(pt_reso*jets[i]->PT, 2);
    particle_u(1,1) = pow(phi_reso*jets[i]->PT,2);
    rotateXY(particle_u, particle_u_rot, missingET->P4().DeltaPhi(jets[i]->P4()));
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

void Atlas_2006_05880::rotateXY(TMatrix &mat, TMatrix &mat_new, double phi) {
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



template <class X>
bool Atlas_2006_05880::is_track_isolated(X* lepton, vector <GenParticle*> &stable, double epsilon, double dR_cone)
{
  FinalStateObject* lep = newFinalStateObject(lepton);
  bool isol = false;
  float dR_cone1=10/lep->PT;
  float dR_cone2=dR_cone;
  dR_cone=std::min(dR_cone1,dR_cone2);
  TLorentzVector p1, p2;
  p1.SetPtEtaPhiE(lep->PT,lep->Eta,lep->Phi,0);
  double pT_sum=0;

  //loop over all stable particles
  for (int i = 0; i < stable.size(); ++i){

    if (stable[i]->PT < 1) continue;
    if (abs(stable[i]->PID) == 12 || abs(stable[i]->PID) == 14 || abs(stable[i]->PID) == 16) continue;                  //minimum pt for particle to be considered for isolation

    p2.SetPtEtaPhiE(stable[i]->PT,stable[i]->Eta,stable[i]->Phi,stable[i]->E);
    double dR = p1.DeltaR(p2);

    if (dR >= dR_cone) continue;                          //skip particles outside cone
    if (dR == 0) continue;                            //skip same particle

    pT_sum+= stable[i]->PT;                           //scalar pT sum
  }

  if (pT_sum <= (lep->PT*epsilon)) isol = true;

  return isol;
}


template <class X>
bool Atlas_2006_05880::is_cal_isolated(X* lep, std::vector<Tower*> towers, double epsilon, double dR_cone)
{
  bool isol = false;
  double sumET=0;

    for (int t = 0; t < towers.size(); t++) {
    Tower* neighbour = towers[t];

    // check tower has 'some' momentum and check dR
    if (neighbour->ET < 0.00001 || neighbour->P4().DeltaR(lep->P4()) > dR_cone)
        continue;
                    // Ignore the lepton's tower
                 bool candidatesTower = false;//This testing is different from the testing in the tracks case, because to one track there corresponds one particle, but for one tower there is not only one particle.
                      for(int p = 0; p < neighbour ->Particles.GetEntries(); p++)
       { GenParticle* deposit_particle=(GenParticle*)neighbour->Particles.At(p);
           if ((abs(deposit_particle->PID) == abs(lep->PID))&(sqrt(pow(neighbour->Phi-lep->Phi,2)+pow(neighbour->Eta-lep->Eta,2))<0.2)) {

                 // break the loop and ignore the tower
                        candidatesTower = true;
                        break;
                      }
                    }
                    if (candidatesTower)
                      continue;
             sumET += neighbour->ET;
                  }



  if (sumET <= (lep->PT*epsilon)) isol = true;

  return isol;
}

int Atlas_2006_05880::calc_nhcand(std::vector<Jet*> bjets){
std::vector<int> hcands;
int n_hbjets=0;
for (int i = 0 ; i < bjets.size(); ++i){
auto parts = bjets[i]->Particles;
	for (int j = 0 ; j < parts.GetEntries(); ++j){
	        const TObject* point = parts.At(j);
		auto part = dynamic_cast<const GenParticle*>(point);
		if (part->Status != 1) continue;
		auto M_index = part->M1;
		auto M = part;
		while (M_index >= 0){              
      				M = (GenParticle*)true_particles[M_index];
      				if (abs(M->PID) == 25) break;        
      				else M_index = M->M1;
    		}
    		if (M_index < 0) continue;  
    		if (abs(M->PID) == 25) hcands.push_back(M_index);
	}
}
for (int i = 0 ; i < hcands.size(); ++i){
	bool equal=false;
	for (int j = i+1 ; j < hcands.size(); ++j){
		if (hcands[i]==hcands[j]) equal=true;
		}
		if (!equal) n_hbjets++;
}
return n_hbjets;
}
int Atlas_2006_05880::calc_nhcand(std::vector<GenParticle*> stable){
std::vector<int> hcands;
int n_hbjets=0;
	for (int j = 0 ; j < stable.size(); ++j){
	        bool bjet_f=false;
		GenParticle* part =stable[j];
		if (part->Status != 1) continue;
		auto M_index = part->M1;
		auto M = part;
		while (M_index >= 0){              
      				M = (GenParticle*)true_particles[M_index];
      				if (abs(M->PID)==5) bjet_f=true;
      				if (abs(M->PID) == 25) break;        
      				else M_index = M->M1;
    		}
    		if (M_index < 0) continue;  
    		if (abs(M->PID) == 25) hcands.push_back(M_index);
	}
for (int i = 0 ; i < hcands.size(); ++i){
	bool equal=false;
	for (int j = i+1 ; j < hcands.size(); ++j){
		if (hcands[i]==hcands[j]) equal=true;
		}
		if (!equal) n_hbjets++;
}
return n_hbjets;
}



