#include "atlas_1605_04285.h"
#include<math.h>

#include<TMatrixD.h>
#include<TMatrixDEigen.h>
#include<TVectorD.h>



using namespace std;

// AUTHOR: Jan Schuteet-Engel
//  EMAIL: schuette@physik.rwth-aachen.de
void Atlas_1605_04285::initialize() {
  setAnalysisName("atlas_1605_04285");          
  setInformation(""
    "#  Search for gluinos in event with isolated lept \n"
  "");
  setLuminosity(3.2*units::INVFB);      
  bookSignalRegions("2jSoftLep;5jSoftLep;4jHighX;4jLowX;5j;6j;");//5jSoftLep_1;5jSoftLep_2;5jSoftLep_3;5jSoftLep_4;5jSoftLep_5;6j_1;6j_2;6j_3;6j_4;6j_5;6j_6;");
  //The signal regions which are commented out have been used to produce the histograms in the validation

  bookCutflowRegions("SR_hard_6j_1;SR_hard_6j_2;SR_hard_6j_3;SR_hard_6j_4;SR_hard_6j_5;SR_hard_6j_6;SR_hard_6j_7;SR_hard_6j_8;");
  bookCutflowRegions("SR_hard_5j_1;SR_hard_5j_2;SR_hard_5j_3;SR_hard_5j_4;SR_hard_5j_5;SR_hard_5j_6;SR_hard_5j_7;SR_hard_5j_8;");
  bookCutflowRegions("SR_hard_4j_low_x_00;SR_hard_4j_low_x_0;SR_hard_4j_low_x_1;SR_hard_4j_low_x_2;SR_hard_4j_low_x_3;SR_hard_4j_low_x_4;SR_hard_4j_low_x_5;SR_hard_4j_low_x_6;SR_hard_4j_low_x_7;");
  bookCutflowRegions("SR_hard_4j_high_x_1;SR_hard_4j_high_x_2;SR_hard_4j_high_x_3;SR_hard_4j_high_x_4;SR_hard_4j_high_x_5;SR_hard_4j_high_x_6;");
  bookCutflowRegions("SR_soft_2j_1;SR_soft_2j_2;SR_soft_2j_3;SR_soft_2j_4;SR_soft_2j_5;");
  bookCutflowRegions("SR_soft_5j_1;SR_soft_5j_2;SR_soft_5j_3;SR_soft_5j_4;SR_soft_5j_5;SR_soft_5j_6;SR_soft_5j_7;");
}

void Atlas_1605_04285::analyze() {

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

  overlap_muons_jets_with_track_conditions(preselected_muons_soft,preselected_jets_soft,tracks,0.4,3,500.0);
  overlap_muons_jets_with_track_conditions(preselected_muons_hard,preselected_jets_hard,tracks,0.4,3,500.0);

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

  //apply GradientLoose isolation requirements[64] (efficiencies) to medium muons.
  signal_muons_soft=filter_candidates_in_pT_range(signal_muons_soft,0,50,0.95);//because we started with preselected, which are based on medium we can directly apply the efficiencies!
  signal_muons_soft=filter_candidates_in_pT_range(signal_muons_soft,50,70,0.97);
  signal_muons_soft=filter_candidates_in_pT_range(signal_muons_soft,70,10000,0.99);

  signal_muons_hard=filter_candidates_in_pT_range(signal_muons_hard,0,50,0.95);
  signal_muons_hard=filter_candidates_in_pT_range(signal_muons_hard,50,70,0.97);
  signal_muons_hard=filter_candidates_in_pT_range(signal_muons_hard,70,10000,0.99);

  //now do the cuts on top of the preselection level!

  signal_jets_soft=filterPhaseSpace(signal_jets_soft,25,-2.8,2.8);
  signal_jets_hard=filterPhaseSpace(signal_jets_hard,25,-2.8,2.8);

  // Jets with pT<50, |eta|<2.4 and JVT less than 0.64 are rejected!
  // Comment: JVT is for pileup suppression. Therefore do not consider this!

  signal_electrons_soft=filterPhaseSpace(signal_electrons_soft,35,-100,100,false,true);
  signal_electrons_hard=filterPhaseSpace(signal_electrons_hard,35,-100,100,false,false);//setting the borders to 100 and -100 means that there is actual no eta cut!

  signal_muons_soft=filterPhaseSpace(signal_muons_soft,35,-100,100,false,true);
  signal_muons_hard=filterPhaseSpace(signal_muons_hard,35,-100,100,false,false);//actually the last 4 parameters here are not mandatory, because they are default in the function definition!


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

  checkSubset(signal_muons_soft,preselected_muons_soft);
  checkSubset(signal_muons_hard,preselected_muons_hard);

  checkSubset(signal_jets_soft,preselected_jets_soft);
  checkSubset(signal_jets_hard,preselected_jets_hard);


  //Event selection
  int N_signal_lep_soft=signal_electrons_soft.size()+signal_muons_soft.size();
  int N_signal_lep_hard=signal_electrons_hard.size()+signal_muons_hard.size();

  int N_preselected_lep_soft=preselected_electrons_soft.size()+preselected_muons_soft.size();
  int N_preselected_lep_hard=preselected_electrons_hard.size()+preselected_muons_hard.size();

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


    //2jSoftLep
    if(etmiss_soft>530){
      countCutflowEvent("SR_soft_2j_1");
      if(N_jets>=2){
        countCutflowEvent("SR_soft_2j_2");
        if(signal_jets_soft[0]->PT>180.0){
          countCutflowEvent("SR_soft_2j_3");
          if(signal_jets_soft[1]->PT>30.0){
            if(mT>100.0){
              countCutflowEvent("SR_soft_2j_4");
              if(etmiss_soft/m_eff_incl>0.38){
                countCutflowEvent("SR_soft_2j_5");
                countSignalEvent("2jSoftLep");
              }
            }
          }
        }
      }
    }
    
    //5jSoftLep
    if(etmiss_soft>375.0){
      countCutflowEvent("SR_soft_5j_1");
      if(N_jets>=5){
        countCutflowEvent("SR_soft_5j_2");
        if(signal_jets_soft[0]->PT>200.0){
          countCutflowEvent("SR_soft_5j_3");
          if(signal_jets_soft[1]->PT>200.0){
            countCutflowEvent("SR_soft_5j_4");
            if(signal_jets_soft[2]->PT>200.0){
              countCutflowEvent("SR_soft_5j_5");
              if(signal_jets_soft[3]->PT>30.0 && signal_jets_soft[4]->PT>30.0){
                //Aplanarity
                TMatrixD MomentumMatrix = Get_Momentum_Matrix(signal_jets_soft);
                TMatrixDEigen Eval(MomentumMatrix);
                TVectorD Eigenval = Eval.GetEigenValuesRe(); //Momentummatrix is symmetric and has therefore only real eigenvalues!
                float Aplanarity = 1.5*Eigenval.Min();
                if(Aplanarity>0.02){
                  countCutflowEvent("SR_soft_5j_6");
                  if(HT>1100.0){
                    countCutflowEvent("SR_soft_5j_7");
                    countSignalEvent("5jSoftLep");
                  }
                }
              }      
            }
          }
        }
      }
    }
    /*
    //SR's to reproduce figure  5f
    if(N_jets>=5){
      if(signal_jets_soft[0]->PT>200.0 && signal_jets_soft[1]->PT>200.0 && signal_jets_soft[2]->PT>200.0 && signal_jets_soft[3]->PT>30.0 && signal_jets_soft[4]->PT>30.0){
        if(HT>1100.0){
          if(Aplanarity>0.02){
            //reproduce figure 5f
            if(etmiss_soft>315.0 && etmiss_soft<=375.0){
              countSignalEvent("5jSoftLep_1");
            }
            if(etmiss_soft>375.0 && etmiss_soft<=435.0){
              countSignalEvent("5jSoftLep_2");
            }
            if(etmiss_soft>435.0 && etmiss_soft<495.0){
              countSignalEvent("5jSoftLep_3");
            }
            if(etmiss_soft>495.0 && etmiss_soft<=555.0){
              countSignalEvent("5jSoftLep_4");
            }
            if(etmiss_soft>555.0){// last bin includes the overflow
              countSignalEvent("5jSoftLep_5");
            }
          }
        }
      }
    }*/       
    
  }

  //
  //hard SR's
  //
  if(N_preselected_lep_hard==1){
    countCutflowEvent("SR_hard_4j_low_x_00");
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
    

      //4j_high_x
      if(etmiss_hard>200.0){
        countCutflowEvent("SR_hard_4j_high_x_1");
        if(N_jets>=4){
          countCutflowEvent("SR_hard_4j_high_x_2");
          if(signal_jets_hard[0]->PT>=325.0){
            countCutflowEvent("SR_hard_4j_high_x_3");
            if(signal_jets_hard[1]->PT>=30.0 && signal_jets_hard[2]->PT>=30.0 && signal_jets_hard[3]->PT>=30.0){
              countCutflowEvent("SR_hard_4j_high_x_4");
              if(mT>425.0){
                if(etmiss_hard/m_eff_incl>0.3){
                  countCutflowEvent("SR_hard_4j_high_x_5");
                  if(m_eff_incl>1800.0){
                    countCutflowEvent("SR_hard_4j_high_x_6");  
                    countSignalEvent("4jHighX");
                  }
                }
              }
            }
          }
        }
      }

      //4j_low_x
      if(N_jets>=3 && etmiss_hard>100.0){
        countCutflowEvent("SR_hard_4j_low_x_0");
        if(etmiss_hard>200.0){
          countCutflowEvent("SR_hard_4j_low_x_1");
          if(N_jets>=4){
            countCutflowEvent("SR_hard_4j_low_x_2");
            if(signal_jets_hard[0]->PT>=325.0){
              countCutflowEvent("SR_hard_4j_low_x_3");
              if(signal_jets_hard[1]->PT>=150.0 && signal_jets_hard[2]->PT>=150.0 && signal_jets_hard[3]->PT>=150.0){
                countCutflowEvent("SR_hard_4j_low_x_4");
                //aplanarity
                TMatrixD MomentumMatrix = Get_Momentum_Matrix(signal_jets_hard);
                TMatrixDEigen Eval(MomentumMatrix);
                TVectorD Eigenval = Eval.GetEigenValuesRe();
                float Aplanarity = 1.5 * Eigenval.Min();//this can lead to negative aplanarities, which is forbidden!
                if(Aplanarity>0.04){
                  countCutflowEvent("SR_hard_4j_low_x_5");
                  if(mT>125.0){
                    countCutflowEvent("SR_hard_4j_low_x_6");
                    if(m_eff_incl>2000.0){
                      countCutflowEvent("SR_hard_4j_low_x_7");
                      countSignalEvent("4jLowX");
                    }
                  }
                }
              }
            }
          }
        }
      }

      //5j
      if(etmiss_hard>250.0){
        countCutflowEvent("SR_hard_5j_1");
        if(N_jets>=5){
          countCutflowEvent("SR_hard_5j_2");
          if(signal_jets_hard[0]->PT>=225.0){
            countCutflowEvent("SR_hard_5j_3");
            if(signal_jets_hard[1]->PT>=50.0 && signal_jets_hard[2]->PT>=50.0 && signal_jets_hard[3]->PT>=50.0 && signal_jets_hard[4]->PT>=50.0){
              countCutflowEvent("SR_hard_5j_4");
              //aplanarity
              TMatrixD MomentumMatrix = Get_Momentum_Matrix(signal_jets_hard);
              TMatrixDEigen Eval(MomentumMatrix);
              TVectorD Eigenval = Eval.GetEigenValuesRe();
              float Aplanarity = 1.5 * Eigenval.Min();//this can lead to negative aplanarities, which is forbidden!
              if(Aplanarity>0.04){
                countCutflowEvent("SR_hard_5j_5");
                if(mT>275.0){
                  countCutflowEvent("SR_hard_5j_6");
                  if(etmiss_hard/m_eff_incl>0.1){
                    countCutflowEvent("SR_hard_5j_7");
                    if(m_eff_incl>1800.0){
                      countCutflowEvent("SR_hard_5j_8");
                      countSignalEvent("5j");
                    }
                  }
                }
              }
            }
          }
        }
      }

      //6j
      if(etmiss_hard>250.0){
        countCutflowEvent("SR_hard_6j_1");
        if(N_jets>=6){
          countCutflowEvent("SR_hard_6j_2");
          if(signal_jets_hard[0]->PT>=125.0){
            countCutflowEvent("SR_hard_6j_3");
            if(signal_jets_hard[1]->PT>=30.0 && signal_jets_hard[2]->PT>=30.0 && signal_jets_hard[3]->PT>=30.0 && signal_jets_hard[4]->PT>=30.0 && signal_jets_hard[5]->PT>=30.0){
              countCutflowEvent("SR_hard_6j_4");
              //aplanarity
              TMatrixD MomentumMatrix = Get_Momentum_Matrix(signal_jets_hard);
              TMatrixDEigen Eval(MomentumMatrix);
              TVectorD Eigenval = Eval.GetEigenValuesRe();
              float Aplanarity = 1.5 * Eigenval.Min();//this can lead to negative aplanarities, which is forbidden!
              if(Aplanarity>0.04){
                countCutflowEvent("SR_hard_6j_5");
                if(mT>225.0){
                  countCutflowEvent("SR_hard_6j_6");
                  if(etmiss_hard/m_eff_incl>0.2){
                    countCutflowEvent("SR_hard_6j_7");
                    if(m_eff_incl>1000){
                      countCutflowEvent("SR_hard_6j_8");
                      countSignalEvent("6j");
                    }
                  }
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


void overlap_muons_jets_with_track_conditions(vector<Muon*>& muons,vector<Jet*>& jets,vector<Track*>& tracks,double dR,int numberOfTracks,double pT_threshold){
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


TMatrixD Get_Momentum_Matrix(std::vector<Jet*> jets){
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


void Atlas_1605_04285::finalize() {
  // Whatever should be done after the run goes here
}       
