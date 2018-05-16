#include "atlas_conf_2016_054.h"

#include<math.h>

#include<TMatrixD.h>
#include<TMatrixDEigen.h>
#include<TVectorD.h>

using namespace std;
// AUTHOR: J.S. Kim
//  EMAIL: jongsoo.kim@tu-dortmund.de
void Atlas_conf_2016_054::initialize() {
  setAnalysisName("atlas_conf_2016_054");          
  setInformation(""
    "# search for squarks and gluinos in events with an isolated lepton, jets and missing transverse momentum at sqrt{s}=13 TeV with the ATLAS detector\n"
  "");
  setLuminosity(14.8*units::INVFB);      
  bookSignalRegions("GG2J;GG6J_bulk;GG6J_high_mass;GG4J_low_x;GG4J_low_x_b_veto;GG4J_high_x;SS4J;SS5J;SS4J_lox_x;SS5J_high_x");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_conf_2016_054::analyze() {
    // I have updated the study atlas_1605_04285 from Jan Schuteet-Engel with slightly different signal regions and the higher total integrated luminosity of 14.8 fb^-1

    
    //The reconstructed primary vertex of an event is required to be consistent with the beamspot envelope and
    //to have at least five associated tracks with p T > 400 MeV. When more than one such vertex is found, the
    //vertex with the largest p T 2 of the associated tracks is chosen.
    //Comment: We do not check this, because this step is to remove noise which is not simulated by delphes.
    
    //Jets are reconstructed from topological clusters in the calorimeters using the anti-k t algorithm with a
    //radius parameter R = 0.4 [64, 65]. Prior to jet reconstruction, clusters are calibrated to the electromagnetic
    //scale response. Additional correction factors derived from simulation and data are applied to the measured
    //jet energy to calibrate it to the particle level [65].
    //Comment: Checkmate uses th anti k_T algorithm with R=0.4
    //TODO: Question: Do I need to apply these correction factors?
    
    std::vector<Jet*> preselected_jets_soft=filterPhaseSpace(jets, 20.0, -4.5, 4.5,false);
    std::vector<Jet*> preselected_jets_hard=filterPhaseSpace(jets, 20.0, -4.5, 4.5,false);
    
    //TODO:
    //Question: Make sure that the electronLoos container in Checkmate is filled according to [63]
    std::vector<Electron*> electron_candidates=electronsLoose;
    
    std::vector<Electron*> preselected_electrons_soft=filterPhaseSpace(electron_candidates, 7.0, -2.47, 2.47,false);
    std::vector<Electron*> preselected_electrons_hard=filterPhaseSpace(electron_candidates, 10.0, -2.47, 2.47,false);
    
    //In the function overlapRemoval the delta_R with eta is used, but we need the deltaR with y.
    // I checked the difference between |p| and E for electrons and jets, it is not huge and therefore we have y=eta!
    
    preselected_electrons_soft=overlapRemoval_2(preselected_electrons_soft,preselected_jets_soft, 0.2, 0.4);
    preselected_electrons_hard=overlapRemoval_2(preselected_electrons_hard,preselected_jets_hard, 0.2, 0.4);
    
    preselected_jets_soft=overlapRemoval(preselected_jets_soft,preselected_electrons_soft, 0.2);
    preselected_jets_hard=overlapRemoval(preselected_jets_hard,preselected_electrons_hard, 0.2);
    
    
    std::vector<Muon*> muon_candidates=muonsLoose;
    
    //TODO the medium muons could also be implemented as a standart default Checkmate container
    muon_candidates=filter_candidates_in_eta_range(muon_candidates,-0.1,0.1,0.7);//implementation of medium muon identification efficiency
    
    std::vector<Muon*> preselected_muons_soft=filterPhaseSpace(muon_candidates,6.0,-2.4,2.4,false);
    std::vector<Muon*> preselected_muons_hard=filterPhaseSpace(muon_candidates,10.0,-2.4,2.4,false);
    
    overlap_muons_jets_with_track_conditions_2(preselected_muons_soft,preselected_jets_soft,tracks,0.4,3,500.0);
    overlap_muons_jets_with_track_conditions_2(preselected_muons_hard,preselected_jets_hard,tracks,0.4,3,500.0);
    
    preselected_electrons_soft=overlapRemoval(preselected_electrons_soft,preselected_muons_soft,0.01);
    preselected_electrons_hard=overlapRemoval(preselected_electrons_hard,preselected_muons_hard,0.01);
    
    
    //CM internal calculation of etmiss
    missingET->addMuons(muonsCombined);
    double etmiss_soft = missingET->P4().Et();
    double etmiss_hard = etmiss_soft;
    
    std::cout<<"etmiss from checkmate internally"<<std::endl;
    //std::cout<<etmiss_soft<<std::endl;
    //std::cout<<etmiss_hard<<std::endl;
    
    
    //------------------------------------//
    //                                    //
    //                SIGNAL              //
    //                                    //
    //------------------------------------//
    
    vector<Jet*> signal_jets_soft=preselected_jets_soft;
    vector<Jet*> signal_jets_hard=preselected_jets_hard;
    
    vector<Electron*> signal_electrons_soft=preselected_electrons_soft;//preselected are based on Loose
    vector<Electron*> signal_electrons_hard=preselected_electrons_hard;
    vector<Electron*> signal_electrons_hard2=preselected_electrons_hard; //there are two definitions of hard leptons; Here, hard2 is denoted to the eletrons in SR GG4j_low_x_(b_veto)
    
    // make electrons tight_electrons!
    checkSubset(signal_electrons_soft,electronsTight);
    checkSubset(signal_electrons_hard,electronsTight);
    
    //apply GradientLoose isolation requirements[64] (efficiencies) to electrons.
    //Question: I took the efficiencies from [64], but this is not strrictly correct, because these were efficiencies for medium muons, here I apply these efficiencies also to the electrons.
    signal_electrons_soft=filter_candidates_in_pT_range(signal_electrons_soft,0,50,0.95);//Gradient Loose efficiencies
    signal_electrons_soft=filter_candidates_in_pT_range(signal_electrons_soft,50,70,0.97);
    signal_electrons_soft=filter_candidates_in_pT_range(signal_electrons_soft,70,10000,0.99);
    
    signal_electrons_hard=filter_candidates_in_pT_range(signal_electrons_hard,0,50,0.95);
    signal_electrons_hard=filter_candidates_in_pT_range(signal_electrons_hard,50,70,0.97);
    signal_electrons_hard=filter_candidates_in_pT_range(signal_electrons_hard,70,10000,0.99);
    
    
    vector<Muon*> signal_muons_soft=preselected_muons_soft;//preselected are based on medium
    vector<Muon*> signal_muons_hard=preselected_muons_hard;
    vector<Muon*> signal_muons_hard2=preselected_muons_hard;//there are two definitions of hard leptons; Here, hard2 is denoted to the muons in SR GG4j_low_x_(b_veto)
    
    //apply GradientLoose isolation requirements[64] (efficiencies) to medium muons.
    signal_muons_soft=filter_candidates_in_pT_range(signal_muons_soft,0,50,0.95);//because we started with preselected, which are based on medium we can directly apply the efficiencies!
    signal_muons_soft=filter_candidates_in_pT_range(signal_muons_soft,50,70,0.97);
    signal_muons_soft=filter_candidates_in_pT_range(signal_muons_soft,70,10000,0.99);
    
    signal_muons_hard=filter_candidates_in_pT_range(signal_muons_hard,0,50,0.95);
    signal_muons_hard=filter_candidates_in_pT_range(signal_muons_hard,50,70,0.97);
    signal_muons_hard=filter_candidates_in_pT_range(signal_muons_hard,70,10000,0.99);
    
    //now do the cuts on top of the preselection level!
    
    signal_jets_soft=filterPhaseSpace(signal_jets_soft,30,-2.8,2.8);
    signal_jets_hard=filterPhaseSpace(signal_jets_hard,30,-2.8,2.8);
    
    // Jets with pT<50, |eta|<2.4 and JVT less than 0.64 are rejected!
    // Comment: JVT is for pileup suppression. Therefore do not consider this!
    
    signal_electrons_soft=filterPhaseSpace(signal_electrons_soft,35,-100,100,false,true);
    signal_electrons_hard=filterPhaseSpace(signal_electrons_hard,35,-100,100,false,false);//setting the borders to 100 and -100 means that there is actual no eta cut!
   
    signal_electrons_hard2=filterPhaseSpace(signal_electrons_hard,7,-100,100,false,false);
    
    signal_muons_soft=filterPhaseSpace(signal_muons_soft,35,-100,100,false,true);
    signal_muons_hard=filterPhaseSpace(signal_muons_hard,35,-100,100,false,false);//actually the last 4 parameters here are not mandatory, because they are default in the function definition!
    
    signal_muons_hard2=filterPhaseSpace(signal_muons_hard,6,-100,100,false,false);
    
    //To enforce compatibility with the primary vertex, the distance |z_0 ·sin(θ)| is required to be less than 0.5 mm
    //for signal lepton tracks, where z 0 is the longitudinal impact parameter with respect to the primary-vertex position.
    //TODO: Hier nicht ganz klar, ob ich das richtig mache. Weil das auch nicht wichtig sein wird, kommentiere ich das aus.
    /*for(int j=0;j<signal_electrons_hard.size();j++){
     for(int i=0;i<tracks.size();i++){
     if(tracks[i]->Particle==signal_electrons_hard[j]){
     double theta=2*atan(exp(-tracks[i]->Eta));
     if(abs(tracks[i]->Z*sin(theta))<0.5){//assume 1) tracks[i]->Z gives track vertex z position and primary- vertex is at 0 of the coordinate frame! 2) tracks[i]->Z gives something in mm!?
     //delete electron and corresponding track
     }
     }
     }
     }*/
    
    //Moreover, in the transverse plane the distance of closest approach of the lepton track to the
    //proton beam line, divided by the corresponding uncertainty, must be less than three for muons and less
    //than five for electrons.
    //Comment: This cannot be implemented, because we have not the uncertainty!
    
    //make sure tha signal_electrons is subset of preselected_electrons, signal_muons is subset of preselected_muons and signal_jets is subset of preselected_jets,
    //Comment: I think such cases can occur, because for preselected procedure we use at the beginning LooseElectrons, MediumMuons and in the signal procedure we use TightElectrons and MediumMuons.
    //         Nevertheless I think cases where preselected container do not contain all signal objects are very rare.
    checkSubset(signal_electrons_soft,preselected_electrons_soft);
    checkSubset(signal_electrons_hard,preselected_electrons_hard);
    checkSubset(signal_electrons_hard2,preselected_electrons_hard);
    
    checkSubset(signal_muons_soft,preselected_muons_soft);
    checkSubset(signal_muons_hard,preselected_muons_hard);
    checkSubset(signal_muons_hard2,preselected_muons_hard);
    
    checkSubset(signal_jets_soft,preselected_jets_soft);
    checkSubset(signal_jets_hard,preselected_jets_hard);
    
    
    vector<Jet*> bjets_77;
    for( int i=0;i<signal_jets_soft.size();i++){
        if(checkBTag(signal_jets_soft[i],0)){//do the 77% efficiency b jet check!
            bjets_77.push_back(signal_jets_soft[i]);
        }
    }
    
    
    //Event selection
    int N_signal_lep_soft=signal_electrons_soft.size()+signal_muons_soft.size();
    int N_signal_lep_hard=signal_electrons_hard.size()+signal_muons_hard.size();
    int N_signal_lep_hard2=signal_electrons_hard2.size()+signal_muons_hard2.size();
    
    int N_preselected_lep_soft=preselected_electrons_soft.size()+preselected_muons_soft.size();
    int N_preselected_lep_hard=preselected_electrons_hard.size()+preselected_muons_hard.size();
    int N_preselected_lep_hard2=preselected_electrons_hard.size()+preselected_muons_hard.size();
    
    //
    //soft SR's
    //
    if(N_signal_lep_soft==1 && N_preselected_lep_soft==1){
        int N_jets=signal_jets_soft.size();
        //The jets are pT ordered, starting with largest pT value in the vector
        
        //calculate mT,m_eff_incl, HT before
        double mT=0;
        double m_eff_incl=0;
        double HT=0;
        if(signal_electrons_soft.size()==1){
            double dphi=signal_electrons_soft[0]->P4().DeltaPhi(missingET->P4());
            mT=sqrt(2*etmiss_soft*signal_electrons_soft[0]->PT*(1-cos(dphi)));
            HT=signal_electrons_soft[0]->PT;
            for(int i=0;i<signal_jets_soft.size();i++){
                HT+=signal_jets_soft[i]->PT;
            }
            m_eff_incl=signal_electrons_soft[0]->PT+etmiss_soft;
            for(int i=0;i<signal_jets_soft.size();i++){
                if(signal_jets_soft[i]->PT>30){
                    m_eff_incl+=signal_jets_soft[i]->PT;
                }
            }
        }
        else if(signal_muons_soft.size()==1){
            double dphi=signal_muons_soft[0]->P4().DeltaPhi(missingET->P4());
            mT=sqrt(2*etmiss_soft*signal_muons_soft[0]->PT*(1-cos(dphi)));
            HT=signal_muons_soft[0]->PT;
            for(int i=0;i<signal_jets_soft.size();i++){
                HT+=signal_jets_soft[i]->PT;
            }
            m_eff_incl=signal_muons_soft[0]->PT+etmiss_soft;
            for(int i=0;i<signal_jets_soft.size();i++){
                if(signal_jets_soft[i]->PT>30.0){
                    m_eff_incl+=signal_jets_soft[i]->PT;
                }
            }
        }
        
        
        //GG2J
        if(etmiss_soft>460){

            if(N_jets>=2){

                if(signal_jets_soft[0]->PT>200.0){

                    if(signal_jets_soft[1]->PT>30.0){
                        if(mT>100.0){

                            if(etmiss_soft/m_eff_incl>0.35){

                                countSignalEvent("GG2J");
                            }
                        }
                    }
                }
            }
        }
        
        
        
        
        
    }
    
    
    //hard SR with soft leptons
    if(N_preselected_lep_hard2==1){
        
        if(N_signal_lep_hard2==1){
            int N_jets=signal_jets_hard.size();
            //The jets are pT ordered, starting with largest pT value in the vector
            
            //calculate mT,m_eff_incl before
            double mT=0;
            double m_eff_incl=0;
            if(signal_electrons_hard2.size()==1){
                double dphi=signal_electrons_hard2[0]->P4().DeltaPhi(missingET->P4());
                mT=sqrt(2*etmiss_hard*signal_electrons_hard2[0]->PT*(1-cos(dphi)));
                m_eff_incl=signal_electrons_hard2[0]->PT+etmiss_soft;
                for(int i=0;i<signal_jets_hard.size();i++){
                    if(signal_jets_hard[i]->PT>=30.0){
                        m_eff_incl+=signal_jets_hard[i]->PT;
                    }
                }
            }
            else if(signal_muons_hard2.size()==1){
                double dphi=signal_muons_hard2[0]->P4().DeltaPhi(missingET->P4());
                mT=sqrt(2*etmiss_hard*signal_muons_hard2[0]->PT*(1-cos(dphi)));
                m_eff_incl=signal_muons_hard2[0]->PT+etmiss_hard;
                for(int i=0;i<signal_jets_hard.size();i++){
                    if(signal_jets_hard[i]->PT>=30.0){
                        m_eff_incl+=signal_jets_hard[i]->PT;
                    }
                }
            
            }
            
            //GG4J_low_x
            if(N_jets>=4 && etmiss_hard>250.0){
                
                if(etmiss_hard>250.0){
                    
                    if(N_jets>=4){
                        
                        if(signal_jets_hard[0]->PT>=100.0){
                            
                            if(signal_jets_hard[1]->PT>=100.0 && signal_jets_hard[2]->PT>=100.0 && signal_jets_hard[3]->PT>=100.0){
                                
                                //aplanarity
                                TMatrixD MomentumMatrix = Get_Momentum_Matrix_2(signal_jets_hard);
                                TMatrixDEigen Eval(MomentumMatrix);
                                TVectorD Eigenval = Eval.GetEigenValuesRe();
                                float Aplanarity = 1.5 * Eigenval.Min();//this can lead to negative aplanarities, which is forbidden!
                                if(Aplanarity>0.06){
                                    
                                    if(mT>125.0){
                                        
                                        if(m_eff_incl>2000.0){
                                            
                                            countSignalEvent("GG4J_low_x");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            
            //GG4J_low_x_b_veto
            if(N_jets>=4 && etmiss_hard>250.0){
                
                if(etmiss_hard>250.0){
                    
                    if(N_jets>=4){
                        
                        if(signal_jets_hard[0]->PT>=100.0){
                            
                            if(signal_jets_hard[1]->PT>=100.0 && signal_jets_hard[2]->PT>=100.0 && signal_jets_hard[3]->PT>=100.0){
                                
                                //aplanarity
                                TMatrixD MomentumMatrix = Get_Momentum_Matrix_2(signal_jets_hard);
                                TMatrixDEigen Eval(MomentumMatrix);
                                TVectorD Eigenval = Eval.GetEigenValuesRe();
                                float Aplanarity = 1.5 * Eigenval.Min();//this can lead to negative aplanarities, which is forbidden!
                                if(Aplanarity>0.06){
                                    
                                    if(mT>125.0){
                                        
                                        if(m_eff_incl>2000.0){
                                            if (bjets_77.size()==0)
                                                countSignalEvent("GG4J_low_x_b_veto");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

        }
    }

    
    
    //
    //hard SR's
    //
    if(N_preselected_lep_hard==1){
        
        if(N_signal_lep_hard==1){
            int N_jets=signal_jets_hard.size();
            //The jets are pT ordered, starting with largest pT value in the vector
            
            //calculate mT,m_eff_incl before
            double mT=0;
            double m_eff_incl=0;
            if(signal_electrons_hard.size()==1){
                double dphi=signal_electrons_hard[0]->P4().DeltaPhi(missingET->P4());
                mT=sqrt(2*etmiss_hard*signal_electrons_hard[0]->PT*(1-cos(dphi)));
                m_eff_incl=signal_electrons_hard[0]->PT+etmiss_hard;
                for(int i=0;i<signal_jets_hard.size();i++){
                    if(signal_jets_hard[i]->PT>=30.0){
                        m_eff_incl+=signal_jets_hard[i]->PT;
                    }
                }
            }
            else if(signal_muons_hard.size()==1){
                double dphi=signal_muons_hard[0]->P4().DeltaPhi(missingET->P4());
                mT=sqrt(2*etmiss_hard*signal_muons_hard[0]->PT*(1-cos(dphi)));
                m_eff_incl=signal_muons_hard[0]->PT+etmiss_hard;
                for(int i=0;i<signal_jets_hard.size();i++){
                    if(signal_jets_hard[i]->PT>=30.0){
                        m_eff_incl+=signal_jets_hard[i]->PT;
                    }
                }
            }
            
            
            //GG4J_high_x
            if(etmiss_hard>250.0){
                
                if(N_jets>=4){
                
                    if(signal_jets_hard[0]->PT>=400.0){
                
                        if(signal_jets_hard[1]->PT>=30.0 && signal_jets_hard[2]->PT>=30.0 && signal_jets_hard[3]->PT>=30.0 && signal_jets_hard[3]->PT<=100.0){
                
                            if(mT>475.0){
                                if(etmiss_hard/m_eff_incl>0.3){
                
                                    if(m_eff_incl>1600.0){
                
                                        countSignalEvent("GG4J_high_x");
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            
            //GG6J_bulk
            if(etmiss_hard>250.0){
                
                if(N_jets>=6){
                
                    if(signal_jets_hard[0]->PT>=125.0){
                
                        if(signal_jets_hard[1]->PT>=30.0 && signal_jets_hard[2]->PT>=30.0 && signal_jets_hard[3]->PT>=30.0 && signal_jets_hard[4]->PT>=30.0 && signal_jets_hard[5]->PT>=30.0){
                
                            //aplanarity
                            TMatrixD MomentumMatrix = Get_Momentum_Matrix_2(signal_jets_hard);
                            TMatrixDEigen Eval(MomentumMatrix);
                            TVectorD Eigenval = Eval.GetEigenValuesRe();
                            float Aplanarity = 1.5 * Eigenval.Min();//this can lead to negative aplanarities, which is forbidden!
                            if(Aplanarity>0.04){
                
                                if(mT>225.0){
                
                                    if(etmiss_hard/m_eff_incl>0.2){
                
                                        if(m_eff_incl>1000){
                
                                            countSignalEvent("GG6J_bulk");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            //GG6J_high_mass
            if(etmiss_hard>250.0){
                
                if(N_jets>=6){
                    
                    if(signal_jets_hard[0]->PT>=125.0){
                        
                        if(signal_jets_hard[1]->PT>=30.0 && signal_jets_hard[2]->PT>=30.0 && signal_jets_hard[3]->PT>=30.0 && signal_jets_hard[4]->PT>=30.0 && signal_jets_hard[5]->PT>=30.0){
                            
                            //aplanarity
                            TMatrixD MomentumMatrix = Get_Momentum_Matrix_2(signal_jets_hard);
                            TMatrixDEigen Eval(MomentumMatrix);
                            TVectorD Eigenval = Eval.GetEigenValuesRe();
                            float Aplanarity = 1.5 * Eigenval.Min();//this can lead to negative aplanarities, which is forbidden!
                            if(Aplanarity>0.04){
                                
                                if(mT>225.0){
                                    
                                    if(etmiss_hard/m_eff_incl>0.1){
                                        
                                        if(m_eff_incl>2000){
                                            
                                            countSignalEvent("GG6J_high_mass");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            
            
            //SS4J
            if(etmiss_hard>300.0){
                
                if(N_jets>=4){
                    
                    if(signal_jets_hard[0]->PT>=50.0){
                        
                        if(signal_jets_hard[1]->PT>=50.0 && signal_jets_hard[2]->PT>=50.0 && signal_jets_hard[3]->PT>=50.0 ){
                            
                            //aplanarity
                            TMatrixD MomentumMatrix = Get_Momentum_Matrix_2(signal_jets_hard);
                            TMatrixDEigen Eval(MomentumMatrix);
                            TVectorD Eigenval = Eval.GetEigenValuesRe();
                            float Aplanarity = 1.5 * Eigenval.Min();//this can lead to negative aplanarities, which is forbidden!
                            if(Aplanarity>0.08){
                                
                                if(mT>175.0){
                                    
                                    if (bjets_77.size()==0){
                                        if(m_eff_incl>1200.){
                                            countSignalEvent("SS4J");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            

            //SS4J_lox_x
            if(etmiss_hard>250.0){
                
                if(N_jets>=4){
                    
                    if(signal_jets_hard[0]->PT>=250.0){
                        
                        if(signal_jets_hard[1]->PT>=250.0 && signal_jets_hard[2]->PT>=30.0 && signal_jets_hard[3]->PT>=30.0 ){
                            
                            //aplanarity
                            TMatrixD MomentumMatrix = Get_Momentum_Matrix_2(signal_jets_hard);
                            TMatrixDEigen Eval(MomentumMatrix);
                            TVectorD Eigenval = Eval.GetEigenValuesRe();
                            float Aplanarity = 1.5 * Eigenval.Min();//this can lead to negative aplanarities, which is forbidden!
                            if(Aplanarity>0.03){
                                
                                if(mT>150.0 && mT < 400.){
                                    
                                    if (bjets_77.size()==0){
                                        
                                        countSignalEvent("SS4J_lox_x");
                                    
                                    }
                                }
                            }
                        }
                    }
                }
            }
            

            
            //SS5J
            if(etmiss_hard>300.0){
                
                if(N_jets>=5){
                
                    if(signal_jets_hard[0]->PT>=50.0){
                
                        if(signal_jets_hard[1]->PT>=50.0 && signal_jets_hard[2]->PT>=50.0 && signal_jets_hard[3]->PT>=50.0 && signal_jets_hard[4]->PT>=50.0){
                
                            
                            if(mT>175.0){
                                
                                if (bjets_77.size()==0){
                                    if(etmiss_hard/m_eff_incl>0.2){
                                        countSignalEvent("SS5J");
                                    }
                                }
                            }
                            
                        }
                    }
                }
            }
            
            
            //SS5J_high_x
            if(etmiss_hard>400.0){
                
                if(N_jets>=5){
                    
                    if(signal_jets_hard[0]->PT>=30.0){
                        
                        if(signal_jets_hard[1]->PT>=30.0 && signal_jets_hard[2]->PT>=30.0 && signal_jets_hard[3]->PT>=30.0 && signal_jets_hard[4]->PT>=30.0){
                            
                            //aplanarity
                            TMatrixD MomentumMatrix = Get_Momentum_Matrix_2(signal_jets_hard);
                            TMatrixDEigen Eval(MomentumMatrix);
                            TVectorD Eigenval = Eval.GetEigenValuesRe();
                            float Aplanarity = 1.5 * Eigenval.Min();//this can lead to negative aplanarities, which is forbidden!
                            if(Aplanarity>0.03){
                                
                                if(mT>400.0){
                                    
                                    if (bjets_77.size()==0)
                                        countSignalEvent("SS5J_high_x");
                                }
                            }
                        }
                    }
                }
            }
            
            
            /*
             //The signal regions which are defined here are just test signal regions to produce the histograms which were used to validate everthing!
             //repuduce figure 5d
             if(N_jets>=6){//6j
             if(signal_jets_hard[0]->PT>=125.0 && signal_jets_hard[1]->PT>=30.0 && signal_jets_hard[2]->PT>=30.0 && signal_jets_hard[3]->PT>=30.0 && signal_jets_hard[4]->PT>=30.0 && signal_jets_hard[5]->PT>=30.0){
             if(etmiss_hard>250.0){
             if(etmiss_hard/m_eff_incl>0.2){
             if(m_eff_incl>1000.0){
             if(Aplanarity>0.04){
             if(mT>=65 && mT<145){
             countSignalEvent("6j_1");
             }
             if(mT>=145 && mT<225){
             countSignalEvent("6j_2");
             }
             if(mT>=225 && mT<305){
             countSignalEvent("6j_3");
             }
             if(mT>=305 && mT<385){
             countSignalEvent("6j_4");
             }
             if(mT>=385 && mT<465){
             countSignalEvent("6j_5");
             }
             if(mT>=465){//last bin includes overflow!
             countSignalEvent("6j_6");
             }
             }
             }
             }
             }
             }
             }*/
        }
    }
    
    
}


void overlap_muons_jets_with_track_conditions_2(vector<Muon*>& muons,vector<Jet*>& jets,vector<Track*>& tracks,double dR,int numberOfTracks,double pT_threshold){
    std::vector<bool> delete_jets;
    std::vector<bool> delete_muons;
    for(int i=0;i<jets.size();i++){
        delete_jets.push_back(false);
    }
    for(int i=0;i<muons.size();i++){
        delete_muons.push_back(false);
    }
    for(int i=0;i<muons.size();i++){
        for(int j=0;j<jets.size();j++){
            if(muons[i]->P4().DeltaR(jets[j]->P4()) <dR){
                int N_jets_pT_lt_500=0;
                for(int k=0;k<tracks.size();k++){
                    for(int l=0;l<jets[j]->Particles.GetEntries();l++){
                        if(tracks[k]->Particle==jets[j]->Particles.At(l) && tracks[k]->PT >pT_threshold*0.001){
                            N_jets_pT_lt_500+=1;
                        }
                    }
                }
                
                if(N_jets_pT_lt_500<numberOfTracks){
                    delete_jets[j]=true;
                }
                else{
                    delete_muons[i]=true;
                }
            }
        }
    }
    
    std::vector<Muon*> filtered_muons;
    std::vector<Jet*> filtered_jets;
    
    for(int i=0;i<jets.size();i++){
        if(delete_jets[i]==false){
            filtered_jets.push_back(jets[i]);
        }
    }
    for(int i=0;i<muons.size();i++){
        if(delete_muons[i]==false){
            filtered_muons.push_back(muons[i]);
        }
    }
    jets=filtered_jets;//produced a copy!
    muons=filtered_muons;
    
}


TMatrixD Get_Momentum_Matrix_2(std::vector<Jet*> jets){
    TMatrixD MomentumMatrix(3,3);
    
    //Comment: Atlas leute sagen, dass die jets, die in die MomentumMatrix eingehen nicht (!) geboosted sind. Also im Lab frame.
    //Im folgenden habe ich deshalb die Variante, die die jets boosted auskommentiert!
    /*
     //calculate total jet 4 momentum which defined the jets rest Frame
     TLorentzVector total_p4(0,0,0,0);
     
     for(int i=0;i<jets.size();i++){
     total_p4+=jets[i]->P4();
     }
     std::vector<TLorentzVector> boosted_jet_momenta;
     
     for(int i=0;i<jets.size();i++){
     boosted_jet_momenta.push_back(jets[i]->P4());
     boosted_jet_momenta[i].Boost(-total_p4.BoostVector());
     std::cout<<"boosted_jet_momenta_E["<<i<<"] = "<<boosted_jet_momenta[i].E()<<std::endl;
     std::cout<<"boosted_jet_momenta_x["<<i<<"] = "<<boosted_jet_momenta[i].Vect().X()<<std::endl;
     std::cout<<"boosted_jet_momenta_y["<<i<<"] = "<<boosted_jet_momenta[i].Vect().Y()<<std::endl;
     std::cout<<"boosted_jet_momenta_z["<<i<<"] = "<<boosted_jet_momenta[i].Vect().Z()<<std::endl;
     }
     
     //check that everything is going right
     TLorentzVector sum_jets_in_jet_rest_frame;
     for(int i=0;i<jets.size();i++){
     sum_jets_in_jet_rest_frame+=boosted_jet_momenta[i];
     }
     std::cout<<"sum boosted_jet_momenta x"<<sum_jets_in_jet_rest_frame.X()<<std::endl;
     std::cout<<"sum boosted_jet_momenta y"<<sum_jets_in_jet_rest_frame.Y()<<std::endl;
     std::cout<<"sum boosted_jet_momenta z"<<sum_jets_in_jet_rest_frame.Z()<<std::endl;
     std::cout<<"-------------------------------"<<std::endl;
     
     
     float PAbs=0;
     for(int i=0;i<jets.size();i++) PAbs+=boosted_jet_momenta[i].Vect().Mag2();
     std::cout<<"Momentum matrix"<<std::endl;
     for(int i=0;i<3;i++){
     for(int j=0;j<3;j++){
     float PSum=0;
     for(int k=0;k<jets.size();k++){
     PSum+=boosted_jet_momenta[k].Vect()[i]*boosted_jet_momenta[k].Vect()[j]/PAbs;
     }
     MomentumMatrix[i][j]=PSum;
     std::cout<<MomentumMatrix[i][j];
     }
     std::cout<<" "<<std::endl;
     }
     */
    
    //methode, die die jets nicht boosted!
    float PAbs=0;
    for(int i=0;i<jets.size();i++) PAbs+=jets[i]->P4().Vect().Mag2();
    
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            float PSum=0;
            for(int k=0;k<jets.size();k++){
                PSum+=jets[k]->P4().Vect()[i]*jets[k]->P4().Vect()[j]/PAbs;
            }
            MomentumMatrix[i][j]=PSum;
            //std::cout<<MomentumMatrix[i][j];
        }
        //std::cout<<" "<<std::endl;
    }
    return MomentumMatrix;

  
}

void Atlas_conf_2016_054::finalize() {
  // Whatever should be done after the run goes here
}       
