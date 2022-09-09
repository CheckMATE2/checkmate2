#include "atlas_conf_2019_040.h"
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"

using namespace TMVA;

// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_conf_2019_040::initialize() {
  setAnalysisName("atlas_conf_2019_040");          
  setInformation(""
    "# search for squarks and gluinos in MET_jet final states\n"
  "");
  setLuminosity(139.0*units::INVFB);      
  bookSignalRegions("MB-SSd-2-1000-10;MB-SSd-2-1000-16;MB-SSd-2-1000-22;MB-SSd-2-1600-10;MB-SSd-2-1600-16;MB-SSd-2-1600-22;MB-SSd-2-2200-16;MB-SSd-2-2200-22;MB-SSd-2-2800-16;MB-SSd-2-2800-22;MB-SSd-2-3400-22;MB-SSd-2-3400-28;MB-SSd-2-4000-22;MB-SSd-2-4000-28;MB-SSd-4-1000-10;MB-SSd-4-1000-16;MB-SSd-4-1000-22;MB-SSd-4-1600-10;MB-SSd-4-1600-16;MB-SSd-4-1600-22;MB-SSd-4-2200-16;MB-SSd-4-2200-22;MB-SSd-4-2800-16;MB-SSd-4-2800-22;MB-GGd-4-1000-10;MB-GGd-4-1000-16;MB-GGd-4-1000-22;MB-GGd-4-1600-10;MB-GGd-4-1600-16;MB-GGd-4-1600-22;MB-GGd-4-2200-10;MB-GGd-4-2200-16;MB-GGd-4-2200-22;MB-GGd-4-2800-10;MB-GGd-4-2800-16;MB-GGd-4-2800-22;MB-GGd-4-3400-10;MB-GGd-4-3400-16;MB-GGd-4-3400-22;MB-GGd-4-4000-10;MB-GGd-4-4000-16;MB-GGd-4-4000-22;MB-C-2-1600-16;MB-C-2-1600-22;MB-C-2-2200-16;MB-C-2-2200-22;MB-C-2-2800-16;MB-C-2-2800-22;MB-C-4-1600-16;MB-C-4-1600-22;MB-C-4-2200-16;MB-C-4-2200-22;MB-C-4-2800-16;MB-C-4-2800-22;MB-C-5-1600-16;MB-C-5-1600-22;MB-C-5-2200-16;MB-C-5-2200-22;MB-C-5-2800-16;MB-C-5-2800-22;SR-2j-1600;SR-2j-2200;SR-2j-2800;SR-4j-1000;SR-4j-2200;SR-4j-3400;SR-5j-1600;SR-6j-1000;SR-6j-2200;SR-6j-3400");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_conf_2019_040::analyze() {

  missingET->addMuons(muonsCombined);  
  
  electronsLoose = filterPhaseSpace(electronsLoose, 7., -2.47, 2.47);
  muonsCombined = filterPhaseSpace(muonsCombined, 6., -2.7, 2.7);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);
  
  jets = overlapRemoval(jets, electronsLoose, 0.2, "y");
  electronsLoose = specialoverlap(electronsLoose, jets);
  muonsCombined = specialoverlap(muonsCombined, jets);
  jets = overlapRemoval_muon_jet_tracks(jets, muonsCombined, 0.2, 2); 
  
  std::vector<Jet*> sigjets = filterPhaseSpace(jets, 50., -2.8, 2.8);
  
  countCutflowEvent("00_all");
  
  double met = missingET->P4().Et();
  
  if(electronsLoose.size() > 0) return;
  if(muonsCombined.size() > 0) return; 
  if (met < 300.) return;
  if ( sigjets.size() < 2 || sigjets[0]->PT < 200. ) return;
  if ( M_eff(sigjets, 0) < 800. ) return;

  
  countCutflowEvent("01_Preselection");
  
//  if( dPhi(sigjets, 0) < 0.4 ) return;
 
//                        PTj1  PTj2  Nj  Eta Phi1 Phi2 ET/HT Apl   Meff
  if (Passes_Cuts(sigjets, 250., 250., 2, 2.0, 0.8, 0.4, 16., 0.,   1600., false, "2j-1600") ) countSignalEvent("SR-2j-1600");
  if (Passes_Cuts(sigjets, 600., 50.,  2, 2.8, 0.4, 0.2, 16., 0.,   2200., false, "2j-2200") ) countSignalEvent("SR-2j-2200");  
  if (Passes_Cuts(sigjets, 250., 250., 2, 1.2, 0.8, 0.4, 16., 0.,   2800., false, "2j-2800") ) countSignalEvent("SR-2j-2800");    
  if (Passes_Cuts(sigjets, 200., 100., 4, 2.0, 0.4, 0.4, 16., 0.04, 1000., false, "4j-1000") ) countSignalEvent("SR-4j-1000");
  if (Passes_Cuts(sigjets, 200., 100., 4, 2.0, 0.4, 0.4, 16., 0.04, 2200., false, "4j-2200") ) countSignalEvent("SR-4j-2200");
  if (Passes_Cuts(sigjets, 200., 100., 4, 2.0, 0.4, 0.4, 10., 0.04, 3400., false, "4j-3400") ) countSignalEvent("SR-4j-3400");  
  if (Passes_Cuts(sigjets, 600., 50.,  5, 2.8, 0.4, 0.2, 16., 0.,   1600., false, "5j-1600") ) countSignalEvent("SR-5j-1600");  
  if (Passes_Cuts(sigjets, 200., 75.,  6, 2.0, 0.4, 0.2, 16., 0.08, 1000., false, "6j-1000") ) countSignalEvent("SR-6j-1000");  
  if (Passes_Cuts(sigjets, 200., 75.,  6, 2.0, 0.4, 0.2, 16., 0.08, 2200., false, "6j-2200") ) countSignalEvent("SR-6j-2200");
  if (Passes_Cuts(sigjets, 200., 75.,  6, 2.0, 0.4, 0.2, 10., 0.08, 3400., false, "6j-3400") ) countSignalEvent("SR-6j-3400");  
  
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., 16., 0.,   1000., 1600., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-1000-10");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 16., 22., 0.,   1000., 1600., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-1000-16");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 22., -1., 0.,   1000., 1600., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-1000-22");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 10., 16., 0.,   1600., -1.0 , false, "MB-SSd") ) countSignalEvent("MB-SSd-2-1600-10");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 16., 22., 0.,   1600., 2200., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-1600-16");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 22., -1., 0.,   1600., 2200., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-1600-22");  
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 16., 22., 0.,   2200., 2800., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-2200-16");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 22., -1., 0.,   2200., 2800., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-2200-22");    
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 16., 22., 0.,   2800., -1.0 , false, "MB-SSd") ) countSignalEvent("MB-SSd-2-2800-16");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 3, 2.0, 0.8, 0.4, 22., -1., 0.,   2800., 3400., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-2800-22");  
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 22., 28., 0.,   3400., 4000., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-3400-22");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 28., -1., 0.,   3400., 4000., false, "MB-SSd") ) countSignalEvent("MB-SSd-2-3400-28");    
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 22., 28., 0.,   4000., -1.0 , false, "MB-SSd") ) countSignalEvent("MB-SSd-2-4000-22");
  if (Passes_Cuts_MB(sigjets, 250., 250., 2, 0, 2.0, 0.8, 0.4, 28., -1., 0.,   4000., -1.0 , false, "MB-SSd") ) countSignalEvent("MB-SSd-2-4000-28"); 
  
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., 16., 0.,   1000., 1600., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-1000-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 16., 22., 0.,   1000., 1600., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-1000-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 22., -1., 0.,   1000., 1600., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-1000-22");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 10., 16., 0.,   1600., -1.0 , false, "MB-SSd") ) countSignalEvent("MB-SSd-4-1600-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 16., 22., 0.,   1600., 2200., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-1600-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 22., -1., 0.,   1600., 2200., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-1600-22");  
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 16., 22., 0.,   2200., 2800., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-2200-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 22., -1., 0.,   2200., 2800., false, "MB-SSd") ) countSignalEvent("MB-SSd-4-2200-22");   
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 16., 22., 0.,   2800., -1.0 , false, "MB-SSd") ) countSignalEvent("MB-SSd-4-2800-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.8, 0.4, 22., -1., 0.,   2800., 3400 , false, "MB-SSd") ) countSignalEvent("MB-SSd-4-2800-22");    
  
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 16., 0.04,   1000., 1600., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-1000-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 16., 22., 0.04,   1000., 1600., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-1000-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 22., -1., 0.04,   1000., 1600., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-1000-22");  
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 16., 0.04,   1600., 2200., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-1600-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 16., 22., 0.04,   1600., 2200., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-1600-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 22., -1., 0.04,   1600., 2200., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-1600-22");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 16., 0.04,   2200., 2800., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-2200-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 16., 22., 0.04,   2200., 2800., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-2200-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 22., -1., 0.04,   2200., 2800., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-2200-22");    
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 16., 0.04,   2800., 3400., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-2800-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 16., 22., 0.04,   2800., 3400., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-2800-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 22., -1., 0.04,   2800., 3400., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-2800-22");    
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 16., 0.04,   3400., 4000., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-3400-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 16., 22., 0.04,   3400., 4000., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-3400-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 22., -1., 0.04,   3400., 4000., false, "MB-GGd") ) countSignalEvent("MB-GGd-4-3400-22");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 10., 16., 0.04,   4000., -1.0 , false, "MB-GGd") ) countSignalEvent("MB-GGd-4-4000-10");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 16., 22., 0.04,   4000., -1.0 , false, "MB-GGd") ) countSignalEvent("MB-GGd-4-4000-16");
  if (Passes_Cuts_MB(sigjets, 200., 100., 4, 0, 2.0, 0.4, 0.2, 22., -1., 0.04,   4000., -1.0 , false, "MB-GGd") ) countSignalEvent("MB-GGd-4-4000-22");  

  if (Passes_Cuts_MB(sigjets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., 22., 0.0,   1600., 2200., false, "MB-C") ) countSignalEvent("MB-C-2-1600-16");
  if (Passes_Cuts_MB(sigjets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 22., -1., 0.0,   1600., 2200., false, "MB-C") ) countSignalEvent("MB-C-2-1600-22");
  if (Passes_Cuts_MB(sigjets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., 22., 0.0,   2200., 2800., false, "MB-C") ) countSignalEvent("MB-C-2-2200-16");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 22., -1., 0.0,   2200., 2800., false, "MB-C") ) countSignalEvent("MB-C-2-2200-22");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 16., 22., 0.0,   2800., -1.0 , false, "MB-C") ) countSignalEvent("MB-C-2-2800-16");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 2, 3, 2.8, 0.4, 0.2, 22., -1., 0.0,   2800., -1.0 , false, "MB-C") ) countSignalEvent("MB-C-2-2800-22");     
  
  if (Passes_Cuts_MB(sigjets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., 22., 0.0,   1600., 2200., false, "MB-C") ) countSignalEvent("MB-C-4-1600-16");
  if (Passes_Cuts_MB(sigjets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 22., -1., 0.0,   1600., 2200., false, "MB-C") ) countSignalEvent("MB-C-4-1600-22");
  if (Passes_Cuts_MB(sigjets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., 22., 0.0,   2200., 2800., false, "MB-C") ) countSignalEvent("MB-C-4-2200-16");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 22., -1., 0.0,   2200., 2800., false, "MB-C") ) countSignalEvent("MB-C-4-2200-22");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 16., 22., 0.0,   2800., -1.0 , false, "MB-C") ) countSignalEvent("MB-C-4-2800-16");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 4, 4, 2.8, 0.4, 0.2, 22., -1., 0.0,   2800., -1.0 , false, "MB-C") ) countSignalEvent("MB-C-4-2800-22");  
  
  if (Passes_Cuts_MB(sigjets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., 22., 0.0,   1600., 2200., false, "MB-C") ) countSignalEvent("MB-C-5-1600-16");
  if (Passes_Cuts_MB(sigjets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 22., -1., 0.0,   1600., 2200., false, "MB-C") ) countSignalEvent("MB-C-5-1600-22");
  if (Passes_Cuts_MB(sigjets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., 22., 0.0,   2200., 2800., false, "MB-C") ) countSignalEvent("MB-C-5-2200-16");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 22., -1., 0.0,   2200., 2800., false, "MB-C") ) countSignalEvent("MB-C-5-2200-22");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 16., 22., 0.0,   2800., -1.0 , false, "MB-C") ) countSignalEvent("MB-C-5-2800-16");    
  if (Passes_Cuts_MB(sigjets, 600.,  50., 5, 0, 2.8, 0.4, 0.2, 22., -1., 0.0,   2800., -1.0 , false, "MB-C") ) countSignalEvent("MB-C-5-2800-22");     
  
  //                         N_j , DPhi13, DPhi4, MET/Meff, Meff,  BDT
  if (Passes_Cuts_BDT(sigjets, 4,   0.4,    0.4,     0.2,   1400., 0.97, true, "BDT-GGd") ) countSignalEvent("BDT-GGd1");        
  
  return;  
  
}

void Atlas_conf_2019_040::finalize() {
  // Whatever should be done after the run goes here
}     


bool Atlas_conf_2019_040::sortByPT(Jet *i, Jet *j) { return (i->PT > j->PT); }

double Atlas_conf_2019_040::Aplanarity(std::vector<Jet*> input_jets) {
  
  double mag = 0.;
  for (int k = 0; k < input_jets.size(); k++)
    mag += pow(input_jets[k]->P4().Rho(),2);
  
  TMatrixD st(TMatrixD::kZero, TMatrixD(3,3) );
  for (int k = 0; k < input_jets.size(); k++) {
    st(0,0) += input_jets[k]->P4().X()*input_jets[k]->P4().X()/mag;
    st(0,1) += input_jets[k]->P4().X()*input_jets[k]->P4().Y()/mag;
    st(0,2) += input_jets[k]->P4().X()*input_jets[k]->P4().Z()/mag;
    st(1,1) += input_jets[k]->P4().Y()*input_jets[k]->P4().Y()/mag;
    st(1,2) += input_jets[k]->P4().Y()*input_jets[k]->P4().Z()/mag;
    st(2,2) += input_jets[k]->P4().Z()*input_jets[k]->P4().Z()/mag;
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

bool Atlas_conf_2019_040::check_nTrack_jet(Jet* jet, std::vector<Track*> tracks, int nTracksMin) {
  
  int nTracks = 0;
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
      if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) nTracks++;

    return nTracks > nTracksMin;
}

double Atlas_conf_2019_040::check_track_pt(Jet* jet, std::vector<Track*> tracks) {
  
  double track_pt = 0.;
  
  for (std::vector<Track*>::iterator it=tracks.begin(); it!=tracks.end(); it++) 
    for (int part = 0; part < jet->Particles.GetEntries(); part++)
       if (jet->Particles.At(part) == (*it)->Particle && (*it)->PT > 0.5) track_pt += (*it)->PT;
       
  return track_pt;
    
}

std::vector<Jet*> Atlas_conf_2019_040::overlapRemoval_muon_jet_tracks(std::vector<Jet*> cand_jets, std::vector<Muon*> cand_muons, double deltaR, int nTracks){
  
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

std::vector<Jet*> Atlas_conf_2019_040::overlapRemoval_muon_jet_tracks2(std::vector<Jet*> cand_jets,std::vector<Muon*> cand_muons, double deltaR){
  
  std::vector<Jet*> passed;
  for (std::vector<Jet*>::iterator jet = cand_jets.begin(); jet != cand_jets.end(); jet++) {
    
    double dR = deltaR;
    bool iso = true;
        
    for (std::vector<Muon*>::iterator mu=cand_muons.begin(); mu!=cand_muons.end(); mu++) 
       if ((*jet)->P4().DeltaR((*mu)->P4()) < dR && 0.7*check_track_pt(*jet, tracks) < (*mu)->PT && (*jet)->PT < 0.5*(*mu)->PT ) {
	 iso = false;
	 break;
       }
       
     if (iso) passed.push_back(*jet);
  }
  
  return passed;
}

template <class X, class Y>
std::vector<X*> Atlas_conf_2019_040::specialoverlap(std::vector<X*> candidates, std::vector<Y*> neighbours) {
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
    
double Atlas_conf_2019_040::dPhi(std::vector<Jet*> jets, int j) {

  int kmax = 0; int kmin = 0;
  if ( !j ) { 
    kmax = std::min(int(jets.size()), 3); 
    kmin = 0;     
  }
  else { 
    kmax = jets.size(); 
    kmin = 3;    
  }
  double dphimin = 10.;

  for (int k = kmin; k < kmax; k++)
      dphimin = std::min(fabs(jets[k]->P4().DeltaPhi( missingET->P4() )), dphimin);
     
  return dphimin;
}    
    
double Atlas_conf_2019_040::M_eff(std::vector<Jet*> jets, int j) {
  
  double Meff = missingET->P4().Et();
  int kmax = 0;
  if( !j ) kmax = jets.size(); else kmax=j; 
  
  for(int i = 0; i < kmax; i++) 
    if(jets[i]->PT > 50)  Meff+=jets[i]->PT;
  
  return Meff;
}    

double Atlas_conf_2019_040::HT(std::vector<Jet*> jets) {
  
  double  PTSum = 0.;
  for (int i = 0; i < jets.size(); i++) PTSum+=jets[i]->PT;
  return PTSum;
}
    
bool Atlas_conf_2019_040::Passes_Cuts(std::vector<Jet*> jets, double PT1Cut, double PT2Cut, int NJet, double EtaCut, double dPhiCut1, double dPhiCut2, double METHTCut, double AplanarityCut, double MeffCut, bool cutflow, std::string sr) {
  
  if ( jets.size() < NJet ) return false;
  if (cutflow) countCutflowEvent(sr+"_02_jetmulti");  
  
  if( dPhi(jets, 0) < dPhiCut1 ) return false;
  if (cutflow) countCutflowEvent(sr+"_03_dPhilow");  
  
  if( dPhi(jets, 1) < dPhiCut2 ) return false;
  if (cutflow) countCutflowEvent(sr+"_04_dPhihigh");    
  
  if ( jets[0]->PT < PT1Cut || jets[NJet-1]->PT < PT2Cut ) return false;
  
  if (cutflow) countCutflowEvent(sr+"_05_PTjets");
  
  for (int i = 0; i < NJet; i++)
    if ( fabs(jets[i]->Eta) > EtaCut ) return false;
    
  if (cutflow) countCutflowEvent(sr+"_06_Etajets");  
  
//  if ( !Aplanarity_Cut(jets, AplanarityCut) ) return false;
  if ( AplanarityCut > 0. and  aplanarity(jets) < AplanarityCut ) return false;
  if (cutflow) countCutflowEvent(sr+"_07_aplanarity");  
  
  if( missingET->P4().Et()/sqrt( HT(jets) ) < METHTCut ) return false;
  
  if (cutflow) countCutflowEvent(sr+"_08_METrelative");  
  
  if( M_eff(jets, 0) < MeffCut) return false;
  if (cutflow) countCutflowEvent(sr+"_09_Meff");  
  
  return true;
}


bool Atlas_conf_2019_040::Passes_Cuts_MB(std::vector<Jet*> jets, double PT1Cut, double PT2Cut, int NJetMin, int NJetMax, double EtaCut, double dPhiCut1, double dPhiCut2, double METHTMin, double METHTMax, double AplanarityCut, double MeffMin, double MeffMax, bool cutflow, std::string sr) {
  
  if ( jets.size() < NJetMin ) return false;
  if ( NJetMax and jets.size() > NJetMax ) return false;
  if (cutflow) countCutflowEvent(sr+"_02_jetmulti");  
  
  if( dPhi(jets, 0) < dPhiCut1 ) return false;
  if (cutflow) countCutflowEvent(sr+"_03_dPhilow");  
  
  if( dPhi(jets, 1) < dPhiCut2 ) return false;
  if (cutflow) countCutflowEvent(sr+"_04_dPhihigh");    
  
  if ( jets[0]->PT < PT1Cut || jets[NJetMin-1]->PT < PT2Cut ) return false;
  
  if (cutflow) countCutflowEvent(sr+"_05_PTjets");
  
  for (int i = 0; i < NJetMin; i++)
    if ( fabs(jets[i]->Eta) > EtaCut ) return false;
    
  if (cutflow) countCutflowEvent(sr+"_06_Etajets");  
  
//  if ( !Aplanarity_Cut(jets, AplanarityCut) ) return false;
  if ( AplanarityCut > 0. and  aplanarity(jets) < AplanarityCut ) return false;
  if (cutflow) countCutflowEvent(sr+"_07_aplanarity");  
  
  if( missingET->P4().Et()/sqrt( HT(jets) ) < METHTMin ) return false;
  if( METHTMax > 0. and missingET->P4().Et()/sqrt( HT(jets) ) > METHTMax ) return false;
  
  if (cutflow) countCutflowEvent(sr+"_08_METrelative");  
  
  if( M_eff(jets, 0) < MeffMin) return false;
  if( MeffMax > 0. and M_eff(jets, 0) > MeffMax) return false;
  if (cutflow) countCutflowEvent(sr+"_09_Meff");  
  
  return true;
}


bool Atlas_conf_2019_040::Passes_Cuts_BDT(std::vector<Jet*> jets, int NJetMin, double dPhiCut1, double dPhiCut2, double METMeff, double MeffMin, double BDTscore, bool cutflow, std::string sr) {
  
  Float_t met, meff, Ap, jetPt0, jetPt1, jetPt2, jetPt3, jetEta0, jetEta1, jetEta2, jetEta3;
  
  if ( jets.size() < NJetMin ) return false;  
  if (cutflow) countCutflowEvent(sr+"_02_jetmulti");  
  
  if( dPhi(jets, 0) < dPhiCut1 ) return false;
  if (cutflow) countCutflowEvent(sr+"_03_dPhilow");  
  
  if( dPhi(jets, 1) < dPhiCut2 ) return false;
  if (cutflow) countCutflowEvent(sr+"_04_dPhihigh");    
  
  met = missingET->P4().Et();
  meff = M_eff(jets, NJetMin);  
  
  if ( met/meff < METMeff ) return false;
  if (cutflow) countCutflowEvent(sr+"_05_METMeff");
  
  if( meff < MeffMin) return false;
  if (cutflow) countCutflowEvent(sr+"_06_Meff");    
  
  TMVA::Reader *reader = new TMVA::Reader( "!Color:!Silent" );

  reader->AddVariable( "met", &met );
  reader->AddVariable( "meff", &meff );
  reader->AddVariable( "Ap", &Ap );
  reader->AddVariable( "jetPt0", &jetPt0 );
  reader->AddVariable( "jetPt1", &jetPt1 );
  reader->AddVariable( "jetPt2", &jetPt2 );
  reader->AddVariable( "jetPt3", &jetPt3 );
  reader->AddVariable( "jetEta0", &jetEta0 );
  reader->AddVariable( "jetEta1", &jetEta1 );
  reader->AddVariable( "jetEta2", &jetEta2 );
  reader->AddVariable( "jetEta3", &jetEta3 );
  
  reader->BookMVA( "BDT", "/home/krolb/tools/CheckMATE/TMVA/data/ZeroLepton2018-SRBDT-GGd1_weight1.xml" );
  
  Ap = aplanarity(jets);
  jetPt0 = jets[0]->PT;
  jetPt1 = jets[1]->PT;
  jetPt2 = jets[2]->PT;
  jetPt3 = jets[3]->PT;
  jetEta0 = jets[0]->Eta;
  jetEta1 = jets[1]->Eta;
  jetEta2 = jets[2]->Eta;
  jetEta3 = jets[3]->Eta;
  
  double bdt = reader->EvaluateMVA( "BDT" );
  
  if (bdt < BDTscore) return false;
  if (cutflow) countCutflowEvent(sr+"_07_BDT");    
  
  delete reader;
  
  return true;
}
