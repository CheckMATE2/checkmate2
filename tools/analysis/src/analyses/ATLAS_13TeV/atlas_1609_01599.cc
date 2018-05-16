#include "atlas_1609_01599.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_1609_01599::initialize() {
  setAnalysisName("atlas_1609_01599");          
  setInformation(""
    "# ttV cross section measurement at 13 TeV\n"
    "# SS mu, 3 or 4 lepton channels\n"
    "# ttW and ttZ at 13 TeV 3.2/fb\n"
  "");
  setLuminosity(3.2*units::INVFB);      
  bookSignalRegions("2muSS;3lZ1b4j;3lZ2b3j;3lZ2b4j;3lnoZ2b;4lDF1b;4lDF2b;4lSF1b;4lSF2b");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.
  bookCutflowRegions("00_all;01_trigger;02_leptons>1;muSS_03_SSmuons;muSS_04_bjets>0;muSS_05_3lepveto;muSS_06_pT>25;muSS_07_met>40;muSS_08_ht>240;");
  bookCutflowRegions("3l_03_3leptons;3l_04_bj>1;3l_05_pt>20;3l_06_charge;3lZ_07_OSSF;3lZ_08_1b4j;3lZ_08_2b3j;3lZ_08_2b4j;3lnoZ_07_noZ;3lnoZ_08_jets;");
  bookCutflowRegions("4l_03_4leptons;4l_04_charge;4l_05_OSSFZ;4l_06_DF2b;4l_06_DF1b;4l_06a_SF2b;4l_06b_SF2b;4l_06b_SF1b;4l_06a_SF1b;");
  bookCutflowRegions("CR-3lWZ;CR-4lZZ;");
  
  
  // You should initialize any declared variables here
}

void Atlas_1609_01599::analyze() {
  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  electronsMedium = filterPhaseSpace(electronsMedium, 7., -2.47, 2.47, true);
  muonsCombined = filterPhaseSpace(muonsCombined, 7., -2.4, 2.4);
  jets = filterPhaseSpace(jets, 25., -2.5, 2.5);
  
  electronsMedium = filterIsolation(electronsMedium);
  electronsMedium = isolateLeptons(electronsMedium, 0.2);
  muonsCombined = isolateLeptons(muonsCombined, 0.3);
  
  jets = overlapRemoval(jets, electronsMedium, 0.2);
  electronsMedium = overlapRemoval(electronsMedium, jets, 0.4);
  muonsCombined = overlapRemoval(muonsCombined, jets, 0.4);
  
  int nbjets = 0; int nlight = 0;
  for (int i = 0; i < jets.size(); i++) 
    if ( checkBTag(jets[i]) ) 
      nbjets++;
    else
      nlight++;
    
  double met = missingET->P4().Et();  
    
  countCutflowEvent("00_all");
  
  if ( !( (electronsMedium.size() > 0 && electronsMedium[0]->PT > 25.) || (muonsCombined.size() > 0 && muonsCombined[0]->PT > 25.) ) ) return;
  countCutflowEvent("01_trigger");

  if ( electronsMedium.size() + muonsCombined.size() < 2 ) return;
  countCutflowEvent("02_leptons>1");
  
  //muSS
  if ( muonsCombined.size() > 1 && muonsCombined[0]->Charge*muonsCombined[1]->Charge > 0) {
    countCutflowEvent("muSS_03_SSmuons");
    
    bool muSS = true;
    
    if ( nbjets > 1 ) countCutflowEvent("muSS_04_bjets>0");  
    else muSS = false;
    
    if ( muSS && electronsMedium.size() == 0 && muonsCombined.size() == 2) countCutflowEvent("muSS_05_3lepveto");
    else muSS = false;
    
    if (muSS && muonsCombined[1]->PT > 25.) countCutflowEvent("muSS_06_pT>25");
    else muSS = false;
    
    if (muSS && met > 40.) countCutflowEvent("muSS_07_met>40");
    else muSS = false;
    
    double ht = muonsCombined[0]->PT + muonsCombined[1]->PT ;
    for (int i = 0; i < jets.size(); i++ )
      ht += jets[i]->PT;
    
    if (muSS && ht > 240. ) {
      countCutflowEvent("muSS_08_ht>240");
      countSignalEvent("2muSS");
    }
  }
  
  std::vector<FinalStateObject*> signalLeptons;
  for ( int i = 0; i < electronsMedium.size(); i++ ) {
    FinalStateObject* lep = new FinalStateObject(electronsMedium[i]);
    signalLeptons.push_back(lep);
  }
  for ( int i = 0; i < muonsCombined.size(); i++ ) {
    FinalStateObject* lep = new FinalStateObject(muonsCombined[i]);
    signalLeptons.push_back(lep);
  }  
  std::sort(signalLeptons.begin(), signalLeptons.end(), FinalStateObject::sortByPT);
  
  if ( signalLeptons.size() == 3 ) {
    countCutflowEvent("3l_03_3leptons");
    
    bool l3 = true;
    
    if (nbjets) countCutflowEvent("3l_04_bj>1");
    else l3 = false;
    
    if ( l3 && signalLeptons[2]->PT > 20. ) countCutflowEvent("3l_05_pt>20");
    else l3 = false;
    
    int charge = 0;
    for ( int i = 0; i < signalLeptons.size(); i++)  
      charge += signalLeptons[i]->Charge;
    
    if ( l3 && abs(charge) == 1 ) countCutflowEvent("3l_06_charge");
    else l3 = false;
    
    bool ossfZ = false;
    bool ossfnoZ = false;
    for (int i = 0; i < signalLeptons.size(); i++ )
      for (int j = i+1 ; j < signalLeptons.size(); j++ )
	if ( signalLeptons[i]->Type == signalLeptons[j]->Type &&  signalLeptons[i]->Charge * signalLeptons[j]->Charge < 0 ) {
	  double minv_temp = (signalLeptons[i]->P4() + signalLeptons[j]->P4() ).M();
	  if ( fabs(minv_temp - 91.2) < 10. ) ossfZ = true;
	  else ossfnoZ = true;
	}
    
    if ( l3 && ossfZ ) {
      countCutflowEvent("3lZ_07_OSSF");
      
      if (nbjets == 1 && jets.size() > 3 )  {
	countCutflowEvent("3lZ_08_1b4j");
	countSignalEvent("3lZ1b4j");
      }
      
      if (nbjets > 1 && jets.size() == 3 )  {
	countCutflowEvent("3lZ_08_2b3j");
	countSignalEvent("3lZ2b3j");
      } 
      
      if (nbjets > 1 && jets.size() > 3 )  {
	countCutflowEvent("3lZ_08_2b4j");
	countSignalEvent("3lZ2b4j");
      }                   
    }
    else if(l3 ) {
      countCutflowEvent("3lnoZ_07_noZ");
      
      if (nbjets > 1 && jets.size() < 5) {
	countCutflowEvent("3lnoZ_08_jets");
	countSignalEvent("3lnoZ2b");
      }
    }
    
    if ( nbjets == 0 && signalLeptons[2]->PT > 20. && jets.size() == 3 && ossfZ) countCutflowEvent("CR-3lWZ");
  }
  
  if ( signalLeptons.size() == 4 ) {
    countCutflowEvent("4l_03_4leptons");
    
    bool l4 = true;
    
    int charge = 0;
    for ( int i = 0; i < signalLeptons.size(); i++)  
      charge += signalLeptons[i]->Charge;
    
    if ( abs(charge) == 0 ) countCutflowEvent("4l_04_charge");
    else return;    
    
    double minv = 1000;
    int i_index = 10;
    int j_index = 10;
    bool ossfZ = false;    
    for (int i = 0; i < signalLeptons.size(); i++ )
      for (int j = i+1 ; j < signalLeptons.size(); j++ )
	if ( signalLeptons[i]->Type == signalLeptons[j]->Type &&  signalLeptons[i]->Charge * signalLeptons[j]->Charge < 0 ) {
	  double minv_temp = (signalLeptons[i]->P4() + signalLeptons[j]->P4() ).M();
	  if ( fabs(minv_temp - 91.2) < 10. ) ossfZ = true;
	  if ( fabs(minv_temp - 91.2) < fabs(minv - 91.2) ) {	    
	    minv = minv_temp;
	    i_index = i;
	    j_index = j;
	  }
	}
	
    if (!ossfZ) return;
    countCutflowEvent("4l_05_OSSFZ");
	
    std::vector<FinalStateObject*> remaining;
    for (int i = 0; i < signalLeptons.size(); i++) 
      if ( i != i_index && i != j_index) remaining.push_back(signalLeptons[i]);
    
    double pTZ2 = (remaining[0]->P4() + remaining[1]->P4()).Perp();   
      
    bool SF = false;  
    if ( remaining[0]->Type == remaining[1]->Type ) SF = true;
   
    double mZ2 = 0.;
    if (SF) mZ2 = (remaining[0]->P4() + remaining[1]->P4()).M();   
    
    if ( fabs( mZ2 - 91.2) < 10. && met < 50. ) countCutflowEvent("CR-4lZZ");
    
    if (nbjets >= 2 && !SF ) {
      countCutflowEvent("4l_06_DF2b");
      countSignalEvent("4lDF2b");
    }
    
    if (nbjets == 1 && !SF && pTZ2 > 35. ) {
      countCutflowEvent("4l_06_DF1b");
      countSignalEvent("4lDF1b");
    }   
    
    if (nbjets >= 2 && SF && fabs( mZ2 - 91.2) > 10. ) {
      countCutflowEvent("4l_06a_SF2b");
      countSignalEvent("4lSF2b");
    }

    if (nbjets >= 2 && SF && fabs( mZ2 - 91.2) < 10. && met > 40. ) {
      countCutflowEvent("4l_06b_SF2b");
      countSignalEvent("4lSF2b");
    }    
    
    if (nbjets == 1 && SF && pTZ2 > 25. && fabs( mZ2 - 91.2) < 10. && met > 80.) {
      countCutflowEvent("4l_06b_SF1b");
      countSignalEvent("4lSF1b");
    } 
    
    if (nbjets == 1 && SF && pTZ2 > 25. && fabs( mZ2 - 91.2) > 10. && met > 40.) {
      countCutflowEvent("4l_06a_SF1b");
      countSignalEvent("4lSF1b");
    }     
  }
  
  return;
}

void Atlas_1609_01599::finalize() {
  // Whatever should be done after the run goes here
}       

template<class T>
static bool isolateLepton(T* lepton, std::vector<Track*> tracks, double maxPT, double deltaR) {
    double pTsum = -lepton->PT;
    for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) {
        if ((*it)->P4().DeltaR(lepton->P4()) > deltaR) 
            continue;
        
        if ((*it)->P4().Perp() > 0.5)   // min track pT
	  pTsum += (*it)->PT;
    }
    return pTsum/lepton->PT < maxPT;
}

template<class T>
std::vector<T*> Atlas_1609_01599::isolateLeptons(std::vector<T*> leptons, double deltaR) {
    std::vector<T*> isolated;
    for (typename std::vector<T*>::iterator it=leptons.begin(); it!=leptons.end(); it++) {
        double dR = std::min(deltaR, (double)(10/((*it)->PT)));
        if (isolateLepton(*it, tracks, 0.06, dR)) {
            isolated.push_back(*it);
        }
    }
    return isolated;
}