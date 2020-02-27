#include "atlas_conf_2016_078.h"
// AUTHOR: J. S. 
//  EMAIL: jongsoo.kim@tu-dortmund.de
void Atlas_conf_2016_078::initialize() {
  setAnalysisName("atlas_conf_2016_078");          
  setInformation(""
    "# Further searches for squarks and gluinos in final states with jets and missing transverse momentum at sqrt{s}=13 TeV with the ATLAS detector (Important: The Recursive Jigsaw Reconstruction technique is not implemented.\n"
  "");
  setLuminosity(13.3*units::INVFB);      

  //
  // IMPORTANT!!!
  //
  // This search is based on atlas_1605_03814. Fore more detailed comments, please refer to atlas_1605_03814
  //

 bookSignalRegions("Meff_2j_800;Meff_2j_1200;Meff_2j_1600;Meff_2j_2000;Meff_3j_1200;Meff_4j_1000;Meff_4j_1400;Meff_4j_1800;Meff_4j_2200;Meff_4j_2600;Meff_5j_1400;Meff_6j_1800;Meff_6j_2200");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_conf_2016_078::analyze() {
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
    
    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 200, 200, 0,0,0,0,2, 0.8,0.8, 0, 14, 0, 0, 1600, false) ) countSignalEvent("Meff_2j_800");
    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 250, 250, 0,0,0,0,2, 1.2,0.8, 0, 16, 0, 0, 1200, false) ) countSignalEvent("Meff_2j_1200");
    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 250, 250, 0,0,0,0,2, 1.2,0.8, 0, 18, 0, 0, 1600, false) ) countSignalEvent("Meff_2j_1600");
    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 250, 250, 0,0,0,0,2, 1.2,0.8, 0, 20, 0, 0, 2000, false) ) countSignalEvent("Meff_2j_2000");
    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 600, 50, 50,0,0,0,2, 20.,0.4, 0, 16, 0, 0, 1200, false) ) countSignalEvent("Meff_3j_1200");
    
    
    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 200, 100, 100,100,0,0,4, 1.2,0.4, 0.4, 0, 0.04, 0.25, 1000, false) ) countSignalEvent("Meff_4j_1000");
    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 200, 100, 100,100,0,0,4, 2.0,0.4, 0.4, 0, 0.04, 0.25, 1400, false) ) countSignalEvent("Meff_4j_1400");
    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 200, 100, 100,100,0,0,4, 2.0,0.4, 0.4, 0, 0.04, 0.2, 1800, false) ) countSignalEvent("Meff_4j_1800");
    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 200, 150, 150,150,0,0,4, 2.0,0.4, 0.4, 0, 0.04, 0.2, 2200, false) ) countSignalEvent("Meff_4j_2200");
    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 200, 150, 150,150,0,0,4, 20.0,0.4, 0.4, 0, 0.04, 0.2, 2600, false) ) countSignalEvent("Meff_4j_2600");

    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 500, 50, 50,50,50,0,5, 20.0,0.0, 0.2, 0, 0.0, 0.3, 2600, false) ) countSignalEvent("Meff_5j_1400");

    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 200, 50, 50,50,50,50,6, 2.0,0.4, 0.2, 0, 0.08, 0.2, 1800, false) ) countSignalEvent("Meff_6j_1800");
    if ( Atlas_conf_2016_078::Passes_Cuts(jets, missingET, 250, 200, 100, 100,100,100,100,6, 20.0,0.4, 0.2, 0, 0.08, 0.15, 2200, false) ) countSignalEvent("Meff_6j_2200");

    
    
  
}

void Atlas_conf_2016_078::finalize() {
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

bool Atlas_conf_2016_078::Aplanarity_Cut( std::vector<Jet*> jets,float cut) {
    TMatrixD MomentumMatrix = Get_Momentum_Matrix(jets);
    TMatrixDEigen Eval(MomentumMatrix);
    TVectorD Eigenval = Eval.GetEigenValuesRe();
    float Aplanarity = (1.5) * Eigenval.Min();
    return Aplanarity > cut;
}

//M_eff Calculation

float Atlas_conf_2016_078::M_eff(std::vector<Jet*> jets, ETMiss* missingET, float j) {
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

float Atlas_conf_2016_078::dPhi(std::vector<Jet*> jets, ETMiss* missingET, int NJet, int j){
    
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

float Atlas_conf_2016_078::HT(std::vector<Jet*> jets){
    float PTSum=0;
    for(int i=0;i<jets.size();i++) PTSum+=jets[i]->PT;
    return PTSum;
}

// Test Signal Regions

bool Atlas_conf_2016_078::Passes_Cuts(std::vector<Jet*> jets, ETMiss* missingET, float METCut,float PT1Cut, float PT2Cut, float PT3Cut, float PT4Cut, float PT5Cut, float PT6Cut, float NJet, float etaCut, float dPhiCut1, float dPhiCut2, float METHTCut, float AplanarityCut, float METMeffCut, float MeffCut, bool cutflow){
    
    if( missingET->P4().Et() < METCut) return false;
    
    if( jets.size() < 2) return false;
    if( jets[0]->PT < PT1Cut || fabs(jets[0]->Eta) > etaCut ) return false;
    
    if (cutflow == true) countCutflowEvent("c_ET_PT");
    
    if( jets.size() < NJet) return false;
    
    if (cutflow == true) countCutflowEvent("d_Mult");
    
    if( dPhi(jets,missingET,NJet,0) < dPhiCut1 ) return false;
    
    if( NJet > 3 ){
        if( dPhi(jets,missingET,NJet,1) < dPhiCut2 ) return false;
    }
    
    if (cutflow == true) countCutflowEvent("e_dPhi");
    
    if( jets[1]->PT < PT2Cut || fabs(jets[1]->Eta) > etaCut ) return false;
    
    if( NJet > 2 ){

        if( NJet >= 3 ){
            if( jets[2]->PT < PT3Cut || fabs(jets[2]->Eta) > etaCut ) return false;
        }
        
        if( NJet >= 4 ){
            if( jets[3]->PT < PT4Cut || fabs(jets[3]->Eta) > etaCut ) return false;
        }
        if( NJet == 5 ){
            if( jets[4]->PT < PT5Cut || fabs(jets[4]->Eta) > etaCut ) return false;
        }
        if( NJet >= 6 ){
            if( jets[5]->PT < PT6Cut || fabs(jets[5]->Eta) > etaCut ) return false;
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

