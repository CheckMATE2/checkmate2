#include "atlas_1605_03814.h"
#include <math.h>
#include <stdio.h>
#include "TMath.h"
#include "TMatrixD.h"
#include "TMatrixDEigen.h"

// AUTHOR: Tim Keller
//  EMAIL: tim.keller@rwth-aachen.de
void Atlas_1605_03814::initialize() {
  setAnalysisName("atlas_1605_03814");
  setInformation(""
    "# Search for squarks and gluinos in final states with jets and missing transverse momentumat sqrt(s)=13TeV with the Atlas detector\n"
  "");
  setLuminosity(3.2*units::INVFB);
  bookSignalRegions("2jl;2jm;2jt;4jt;5j;6jm;6jt;");

  bookCutflowRegions("a_All;b_NoLep;c_ET_PT;d_Mult;e_dPhi;f_PT;g_ETHT;h_Aplanarity;i_MetMeff;j_Meff");
  // You can also book cutflow regions with bookCutFlowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1605_03814::analyze() {
  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  electronsLoose = filterPhaseSpace(electronsLoose, 10., -2.47, 2.47);
  electronsMedium = filterPhaseSpace(electronsMedium, 25., -2.47, 2.47); //maybe tight
  //electronsTight = filterPhaseSpace(electronsTight, 10., -2.47, 2.47, true);
  muonsCombined = filterPhaseSpace(muonsCombined, 10., -2.5, 2.5);
  jets = filterPhaseSpace(jets, 20., -2.8, 2.8);

  photons = filterPhaseSpace(photons, 130., -2.37, 2.37, true);
  photons = filterIsolation(photons);

  jets = overlapRemoval(jets, electronsLoose, 0.2);
  
  electronsLoose  = overlapRemoval(electronsLoose, jets, 0.4);
  muonsCombined   = overlapRemoval(muonsCombined, jets, 0.4);
  electronsLoose  = overlapRemoval(electronsLoose, 0.05);
  jets = overlapRemoval(jets, photons, 0.2);

  //electronsLoose = filterIsolation(electronsLoose, 0);
  //muonsCombined = filterIsolation(muonsCombined, 0);

  countCutflowEvent("a_All");

  if(electronsLoose.size()>0) return;

  if(muonsCombined.size()>0) return;

  countCutflowEvent("b_NoLep");

  jets = filterPhaseSpace(jets, 50., -2.8, 2.8);

  if ( Atlas_1605_03814::Passes_Cuts(jets, missingET, 200, 200, 200, 2, 0.8, 0, 15, 0, 0, 1200, false) ){
      countSignalEvent("2jl");
      /*
      int k=1;

      for(int i=850;i<2900;i=i+100){
	if( fabs(M_eff(jetsSignal, missingET, 0)-i) < 50 ) {
	  std::string String = static_cast<std::ostringstream*>( &(std::ostringstream() << k) )->str();
	  countCutflowEvent("hist_"+String);
	  }
	k++;
      }
      */

  }


  if ( Atlas_1605_03814::Passes_Cuts(jets, missingET, 200, 300, 50, 2, 0.4, 0, 15, 0, 0, 1600, false) ) countSignalEvent("2jm");

  if ( Atlas_1605_03814::Passes_Cuts(jets, missingET, 200, 200, 200, 2, 0.8, 0, 20, 0, 0, 2000, false) ) countSignalEvent("2jt");

  if ( Atlas_1605_03814::Passes_Cuts(jets, missingET, 200, 200, 100, 4, 0.4, 0.2, 0, 0.04, 0.2, 2200, false) ) countSignalEvent("4jt");

  if ( Atlas_1605_03814::Passes_Cuts(jets, missingET, 200, 200, 100, 5, 0.4, 0.2, 0, 0.04, 0.25, 1600, false) ) countSignalEvent("5j");

  if ( Atlas_1605_03814::Passes_Cuts(jets, missingET, 200, 200, 100, 6, 0.4, 0.2, 0, 0.04, 0.25, 1600, true) ) countSignalEvent("6jm");

  if ( Atlas_1605_03814::Passes_Cuts(jets, missingET, 200, 200, 100, 6, 0.4, 0.2, 0, 0.04, 0.2, 2000, false) ) countSignalEvent("6jt");


}

void Atlas_1605_03814::finalize() {
  // Whatever should be done after the run goes here
}

//Aplanarity Cut

static TMatrixD Get_Momentum_Matrix(std::vector<Jet*> jets){
  TMatrixD MomentumMatrix(3,3);
  float PAbs=0;
  for(int i=0;i<jets.size();i++) PAbs+=jets[i]->P4().Vect().Mag2();

  for(int i=0;i<3;i++){
    for(int j=0;j<3;j++){
      float PSum=0;
      for(int k=0;k<jets.size();k++){
	PSum+=jets[k]->P4().Vect()[i]*jets[k]->P4().Vect()[j]/PAbs;
      }
      MomentumMatrix[i][j]=PSum;
    }
  }
  return MomentumMatrix;
}

bool Atlas_1605_03814::Aplanarity_Cut( std::vector<Jet*> jets,float cut) {
  TMatrixD MomentumMatrix = Get_Momentum_Matrix(jets);
  TMatrixDEigen Eval(MomentumMatrix);
  TVectorD Eigenval = Eval.GetEigenValuesRe();
  float Aplanarity = (1.5) * Eigenval.Min();
  return Aplanarity > cut;
}

//M_eff Calculation

float Atlas_1605_03814::M_eff(std::vector<Jet*> jets, ETMiss* missingET, float j) {
  float Meff=0;
  int kmax=0;
  if(j==0){ kmax=jets.size(); }else{ kmax=j; }
  for(int i=0;i<kmax;i++) {
    if(jets[i]->PT > 50){
      Meff+=jets[i]->PT;
    }
  }
  Meff+=missingET->P4().Et();
  return Meff;
}

float Atlas_1605_03814::dPhi(std::vector<Jet*> jets, ETMiss* missingET, int NJet, int j){

  int kmax=0; int kmin=0;
  if( j==0 ){ kmax = std::min(int(jets.size()),3); kmin = 0; }else{ kmax = int(jets.size()); kmin = 3; }
  double dphimin = 0;

  for(int k=kmin;k<kmax;k++){
    if( k==kmin ){
      dphimin = fabs( jets[k]->P4().DeltaPhi( missingET->P4() ) );
    }else{
      dphimin = std::min(fabs( jets[k]->P4().DeltaPhi( missingET->P4() ) ),dphimin);
    }
  }
  return dphimin;
}

float Atlas_1605_03814::HT(std::vector<Jet*> jets){
  float PTSum=0;
  for(int i=0;i<jets.size();i++) PTSum+=jets[i]->PT;
  return PTSum;
}

// Test Signal Regions

bool Atlas_1605_03814::Passes_Cuts(std::vector<Jet*> jets, ETMiss* missingET, float METCut,float PT1Cut, float PT2Cut, float NJet, float dPhiCut1, float dPhiCut2, float METHTCut, float AplanarityCut, float METMeffCut, float MeffCut, bool cutflow){

  if( missingET->P4().Et() < METCut) return false;

  if( jets.size() < 2) return false;
  if( jets[0]->PT < PT1Cut ) return false;

  if (cutflow == true) countCutflowEvent("c_ET_PT");

  if( jets.size() < NJet) return false;

  if (cutflow == true) countCutflowEvent("d_Mult");

  if( dPhi(jets,missingET,NJet,0) < dPhiCut1 ) return false;

  if( NJet > 3 ){
  if( dPhi(jets,missingET,NJet,1) < dPhiCut2 ) return false;
  }

  if (cutflow == true) countCutflowEvent("e_dPhi");

  if( jets[1]->PT < PT2Cut ) return false;

  if( NJet > 2 ){
    for(int i=2;i<std::min(int(NJet),4);i++){
      if( jets[i]->PT < 100 ) return false;
    }
  }

  if (cutflow == true) countCutflowEvent("f_PT");

  if( missingET->P4().Et() / sqrt( HT(jets) ) < METHTCut ) return false;

  if (cutflow == true) countCutflowEvent("g_ETHT");

  if (NJet > 3){
  if( !Aplanarity_Cut(jets,AplanarityCut) ) return false;
  }


  if (cutflow == true) countCutflowEvent("h_Aplanarity");

  if( missingET->P4().Et() / M_eff(jets, missingET, NJet) < METMeffCut) return false;

  if (cutflow == true) countCutflowEvent("i_MetMeff");

  if( M_eff(jets, missingET, 0) < MeffCut) return false;

  if (cutflow == true) countCutflowEvent("j_Meff");

  return true;

}
