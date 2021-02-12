#include "atlas_2101_01629.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_2101_01629::initialize() {
  setAnalysisName("atlas_2101_01629");          
  setInformation(""
    "# gluinos/squarks, 1 lepton, jets, MET\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("2J_bveto_700;2J_bveto_1300;2J_bveto_1900;2J_btag_700;2J_btag_1300;2J_btag_1900;4J_lx_bveto_1000;4J_lx_bveto_1600;4J_lx_bveto_2200;4J_lx_btag_1000;4J_lx_btag_1600;4J_lx_btag_2200;4J_hx_bveto_1000;4J_hx_bveto_1600;4J_hx_bveto_2200;4J_hx_btag_1000;4J_hx_btag_1600;4J_hx_btag_2200;0;6J_bveto_700;6J_bveto_1400;6J_bveto_2100;6J_bveto_2800;6J_btag_700;6J_btag_1400;6J_btag_2100;6J_btag_2800;2J_disc_gluino;2J_disc_squark;4J_disc_hx;4J_disc_lx;6J_disc_gluino;6J_disc_squark");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_2101_01629::analyze() {

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.
  
  std::vector<Jet*> jets_baseline = filterPhaseSpace(jets, 20., -4.5, 4.5);  
  std::vector<Electron*> electrons_baseline = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47, true);
  std::vector<Electron*> electrons_signal = filterPhaseSpace(electronsTight, 7., -2.47, 2.47, true);
  std::vector<Muon*> muons_baseline = filterPhaseSpace(muonsCombined, 6., -2.7, 2.7);
  
  jets_baseline = overlapRemoval(jets_baseline, electrons_baseline, 0.2, "y");   
  jets_baseline = overlapRemoval(jets_baseline, electrons_signal, 0.2, "y");  // just in case...   
  electrons_baseline = specialoverlap(electrons_baseline, jets_baseline);
  electrons_signal = specialoverlap(electrons_signal, jets_baseline);
  jets_baseline = overlapRemoval(jets_baseline, muons_baseline, 0.2, "y");     
  muons_baseline = specialoverlap(muons_baseline, jets_baseline);
  
  std::vector<Muon*>  muons_signal = filterPhaseSpace(muons_baseline, 6., -2.5, 2.5);
  jets_signal.clear();
  jets_signal = filterPhaseSpace(jets_baseline, 30., -2.8, 2.8);  
  
  bjets.clear();
  for (int i = 0; i < jets_signal.size(); i++) 
    if ( fabs(jets_signal[i]->Eta) < 2.5 && checkBTag(jets_signal[i]) ) bjets.push_back(jets_signal[i]);
    
  countCutflowEvent("00_all");  
    
  met = missingET->P4().Et();
  
  if ( (electrons_signal.size() + muons_signal.size()) != 1 ) return;
  lepton = (electrons_signal.size() == 1) ? electrons_signal[0]->P4() : muons_signal[0]->P4(); 
  mT = AnalysisBase::mT(lepton, missingET->P4());
  
  if (met < 180. or jets_signal.size()< 2  or mT < 50. ) return;
  
  countCutflowEvent("01_preselection");
  
  meff = met + lepton.Perp();
  for (int i = 0; i < jets_signal.size(); i++)
      meff += jets_signal[i]->PT;

  Passes_Cuts_2J( true, "2J");
  Passes_Cuts_4J( true, "4J");
  Passes_Cuts_6J( true, "6J");
  
  return;
  
}

void Atlas_2101_01629::finalize() {
  // Whatever should be done after the run goes here
}       


bool Atlas_2101_01629::Passes_Cuts_2J(bool cutflow, std::string sr) {
    
    if ( lepton.Perp() > 25. ) return false;
    if (cutflow) {countCutflowEvent(sr+"_btag_02_leptonPT");  countCutflowEvent(sr+"_bveto_02_leptonPT");}   
    
    if ( met < 400.) return false;
    if (cutflow) {countCutflowEvent(sr+"_btag_03_MET>400");  countCutflowEvent(sr+"_bveto_03_MET>400");}
    
    bool btag = false;
    if (bjets.size() > 0) btag = true;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_04_btag"); else  countCutflowEvent(sr+"_bveto_04_btag");}
    
    if (met/meff < 0.25) return false;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_05_MET/Meff"); else  countCutflowEvent(sr+"_bveto_05_MET/Meff");}
    
    if (jets_signal.size() *10 < lepton.Perp()) return false;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_06_NjLPT"); else  countCutflowEvent(sr+"_bveto_06_NjLPT");}
    
    if (meff < 700.) return false;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_07_Meff>700"); else  countCutflowEvent(sr+"_bveto_07_Meff>700");}
    
    if (mT < 100.) return false;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_08_MT>100"); else  countCutflowEvent(sr+"_bveto_08_MT>100");}
    
    if (meff < 1300) {
        if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_09_Meff700"); else  countCutflowEvent(sr+"_bveto_09_Meff700");}
        if (btag) countSignalEvent("2J_btag_700"); else  countSignalEvent("2J_bveto_700");  
        return true;
    }
    else if (meff < 1900) {
        if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_10_Meff1300"); else  countCutflowEvent(sr+"_bveto_10_Meff1300");}
        if (btag) countSignalEvent("2J_btag_1300"); else  {countSignalEvent("2J_bveto_1300"); countSignalEvent("2J_disc_squark");}        
    }   
    else
    {
        if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_11_Meff1900"); else  countCutflowEvent(sr+"_bveto_11_Meff1900");}
        if (btag) countSignalEvent("2J_btag_1900"); else  {countSignalEvent("2J_bveto_1900"); countSignalEvent("2J_disc_squark"); countSignalEvent("2J_disc_gluino");}        
    }    
    
    return true;
}


bool Atlas_2101_01629::Passes_Cuts_4J(bool cutflow, std::string sr) {
    
    if ( lepton.Perp()  < 25. ) return false;
    if (cutflow) {countCutflowEvent(sr+"_btag_02_leptonPT");  countCutflowEvent(sr+"_bveto_02_leptonPT");}   
    
    if ( jets_signal.size() < 4 or jets_signal.size() > 5) return false;   
    if (cutflow) {countCutflowEvent(sr+"_btag_03_Njet");  countCutflowEvent(sr+"_bveto_03_Njet");}   
    
    bool btag = false;
    if (bjets.size() > 0) btag = true;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_04_btag"); else  countCutflowEvent(sr+"_bveto_04_btag");}    
    
    if ( met < 300.) return false;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_05_MET>300"); else countCutflowEvent(sr+"_bveto_05_MET>300");}
        
    if (met/meff < 0.2) return false;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_06_MET/Meff"); else  countCutflowEvent(sr+"_bveto_06_MET/Meff");}
    
    if (Atlas_2101_01629::Aplanarity() < 0.01) return false; 
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_07_aplan"); else  countCutflowEvent(sr+"_bveto_07_aplan");}
    
    if (meff < 1000.) return false;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_08_Meff>1000"); else  countCutflowEvent(sr+"_bveto_08_Meff>1000");}
    
    if (mT < 150.) return false;
    if (mT < 520.) {
        if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_09_lowx"); else  countCutflowEvent(sr+"_bveto_09_lowx");}
        if (meff < 1300) {
            if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_10_lowxMeff1000"); else  countCutflowEvent(sr+"_bveto_10_lowxMeff1000");}
            if (btag) countSignalEvent("4J_lx_btag_1000"); else  countSignalEvent("4J_lx_bveto_1000");  
            return true;
        }
        else if (meff < 2200) {
            if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_11_lowxMeff1600"); else  countCutflowEvent(sr+"_bveto_11_lowxMeff1600");}
            if (btag) countSignalEvent("4J_lx_btag_1600"); else  countSignalEvent("4J_lx_bveto_1600");        
            return true;        
        }   
        else
        {
            if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_12_lowxMeff2200"); else  countCutflowEvent(sr+"_bveto_12_lowxMeff2200");}
            if (btag) countSignalEvent("4J_lx_btag_2200"); else  {countSignalEvent("4J_lx_bveto_2200"); countSignalEvent("4J_disc_lx"); }        
        }
        return true;
    }
        
    if (mT > 520. ) {
        if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_09_highx"); else  countCutflowEvent(sr+"_bveto_09_highx");}
        if (meff < 1300) {
            if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_10_highxMeff1000"); else  countCutflowEvent(sr+"_bveto_10_highxMeff1000");}
            if (btag) countSignalEvent("4J_hx_btag_1000"); else  countSignalEvent("4J_hx_bveto_1000");  
            return true;
        }
        else if (meff < 2200) {
            if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_11_highxMeff1600"); else  countCutflowEvent(sr+"_bveto_11_highxMeff1600");}
            if (btag) countSignalEvent("4J_hx_btag_1600"); else  countSignalEvent("4J_hx_bveto_1600");        
            return true;        
        }   
        else
        {
            if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_12_highxMeff2200"); else  countCutflowEvent(sr+"_bveto_12_highxMeff2200");}
            if (btag) countSignalEvent("4J_hx_btag_2200"); else  {countSignalEvent("4J_hx_bveto_2200"); countSignalEvent("4J_disc_hx"); }        
        }
        return true;
    }
    
    return true;
}


bool Atlas_2101_01629::Passes_Cuts_6J(bool cutflow, std::string sr) {
    
    if ( lepton.Perp()  < 25. ) return false;
    if (cutflow) {countCutflowEvent(sr+"_btag_02_leptonPT");  countCutflowEvent(sr+"_bveto_02_leptonPT");}   
    
    if ( jets_signal.size() < 6 ) return false;   
    if (cutflow) {countCutflowEvent(sr+"_btag_03_Njet");  countCutflowEvent(sr+"_bveto_03_Njet");}   
    
    bool btag = false;
    if (bjets.size() > 0) btag = true;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_04_btag"); else  countCutflowEvent(sr+"_bveto_04_btag");}    

    if (Atlas_2101_01629::Aplanarity() < 0.05) return false; 
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_05_aplan"); else  countCutflowEvent(sr+"_bveto_05_aplan");}    

    if (meff < 700.) return false;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_06_Meff>700"); else  countCutflowEvent(sr+"_bveto_06_Meff>700");}    
    
    if ( met < 300.) return false;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_07_MET>300"); else countCutflowEvent(sr+"_bveto_07_MET>300");}
        
    if (mT < 225.) return false;
    if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_08_mT>225"); else  countCutflowEvent(sr+"_bveto_08_mT>225");}
    
    if (meff < 1400) {
        if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_09_Meff700"); else  countCutflowEvent(sr+"_bveto_09_Meff700");}
        if (btag) countSignalEvent("6J_btag_700"); else  countSignalEvent("6J_bveto_700");  
        return true;
    }
    else if (meff < 2100) {
        if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_10_Meff1400"); else  countCutflowEvent(sr+"_bveto_10_Meff1400");}
        if (btag) countSignalEvent("6J_btag_1400"); else  countSignalEvent("6J_bveto_1400"); 
        return true;
    }
    else if (meff < 2800) {
        if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_11_Meff2100"); else  countCutflowEvent(sr+"_bveto_11_Meff2100");}
        if (btag) countSignalEvent("6J_btag_2100"); else  {countSignalEvent("6J_bveto_2100"); countSignalEvent("6J_disc_squark"); }
        return true;
    }  
    else {
        if (cutflow) {if (btag) countCutflowEvent(sr+"_btag_12_Meff2800"); else  countCutflowEvent(sr+"_bveto_12_Meff2800");}
        if (btag) countSignalEvent("6J_btag_2800"); else  {countSignalEvent("6J_bveto_2800"); countSignalEvent("6J_disc_squark"); countSignalEvent("6J_disc_gluino");}        
    }  
    
    return true;
}


double Atlas_2101_01629::Aplanarity() {
  
  double mag = pow(lepton.Rho(),2);;
  for (int k = 0; k < jets_signal.size(); k++)
    mag += pow(jets_signal[k]->P4().Rho(),2);
  
  TMatrixD st(TMatrixD::kZero, TMatrixD(3,3) );
  st(0,0) += lepton.X()*lepton.X()/mag;
  st(0,1) += lepton.X()*lepton.Y()/mag;
  st(0,2) += lepton.X()*lepton.Z()/mag;
  st(1,1) += lepton.Y()*lepton.Y()/mag;
  st(1,2) += lepton.Y()*lepton.Z()/mag;
  st(2,2) += lepton.Z()*lepton.Z()/mag;
  
  for (int k = 0; k < jets_signal.size(); k++) {
    st(0,0) += jets_signal[k]->P4().X()*jets_signal[k]->P4().X()/mag;
    st(0,1) += jets_signal[k]->P4().X()*jets_signal[k]->P4().Y()/mag;
    st(0,2) += jets_signal[k]->P4().X()*jets_signal[k]->P4().Z()/mag;
    st(1,1) += jets_signal[k]->P4().Y()*jets_signal[k]->P4().Y()/mag;
    st(1,2) += jets_signal[k]->P4().Y()*jets_signal[k]->P4().Z()/mag;
    st(2,2) += jets_signal[k]->P4().Z()*jets_signal[k]->P4().Z()/mag;
  }
  st(1,0) = st(0,1);
  st(2,0) = st(0,2);
  st(2,1) = st(1,2);
  
  TMatrixDEigen eigen(st);
  TMatrixD diag = eigen.GetEigenValues();
  
  std::vector<double> lambdas;
  lambdas.push_back( diag(0,0) );
  lambdas.push_back( diag(1,1) );
  lambdas.push_back( diag(2,2) );
  std::sort (lambdas.begin(), lambdas.end());
  
  return 1.5*lambdas[0];
}  

template <class X, class Y>
std::vector<X*> Atlas_2101_01629::specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours) {
      // If neighbours are empty, return candidates
      if(neighbours.size() == 0)
        return candidates;
      std::vector<X*> passed_candidates;
      // Loop over candidates
      for(int i = 0; i < candidates.size(); i++) {
        bool overlap = false;
        // If a neighbour is too close, declare overlap, break and don't save candidate
        for(int j = 0; j < neighbours.size(); j++) {
          if (candidates[i]->P4().DeltaR(neighbours[j]->P4()) < std::min(0.4, 0.04 + 10./candidates[i]->PT) ) {
            overlap = true;
            break;
          }
        }
        if (!overlap)
          passed_candidates.push_back(candidates[i]);
      }
      return passed_candidates;
    }
