#######################################
# Order of execution of various modules
#######################################

set ExecutionPath {
  ParticlePropagator

  ChargedHadronTrackingEfficiency
  ElectronTrackingEfficiency
  MuonTrackingEfficiency

  ChargedHadronMomentumSmearing
  ElectronEnergySmearing
  MuonMomentumSmearing

  TrackMerger
  Calorimeter
  EFlowMerger
  
  MissingET

  FastJetFinder
  
  JetEnergyScale

  TagSkimmer

  TreeWriter
}

#################################
# Propagate particles in cylinder
#################################

module ParticlePropagator ParticlePropagator {
  set InputArray Delphes/stableParticles

  set OutputArray stableParticles
  set ChargedHadronOutputArray chargedHadrons
  set ElectronOutputArray electrons
  set MuonOutputArray muons

  # radius of the magnetic field coverage, in m
  set Radius 1.15
  # half-length of the magnetic field coverage, in m
  set HalfLength 3.51

  # magnetic field
  set Bz 2.0
}

####################################
# Charged hadron tracking efficiency
####################################

module Efficiency ChargedHadronTrackingEfficiency {
  set InputArray ParticlePropagator/chargedHadrons
  set OutputArray chargedHadrons

  # add EfficiencyFormula {efficiency formula as a function of eta and pt}

  # tracking efficiency formula for charged hadrons
  set EfficiencyFormula {                                                    (pt <= 0.1)   * (0.00) +
                                            (abs(eta) <= 1.5) * (pt > 0.1   && pt <= 1.0)   * (0.70) +
                                            (abs(eta) <= 1.5) * (pt > 1.0)                  * (0.95) +
                          (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1   && pt <= 1.0)   * (0.60) +
                          (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0)                  * (0.85) +
                          (abs(eta) > 2.5)                                                  * (0.00)}
}

##############################
# Electron tracking efficiency
##############################

module Efficiency ElectronTrackingEfficiency {
  set InputArray ParticlePropagator/electrons
  set OutputArray electrons

  # set EfficiencyFormula {efficiency formula as a function of eta and pt}

  # tracking efficiency formula for electrons
  set EfficiencyFormula {                                                    (pt <= 0.1)   * (0.00) +
                                            (abs(eta) <= 1.5) * (pt > 0.1   && pt <= 1.0)   * (0.73) +
                                            (abs(eta) <= 1.5) * (pt > 1.0   && pt <= 1.0e2) * (0.95) +
                                            (abs(eta) <= 1.5) * (pt > 1.0e2)                * (0.99) +
                          (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1   && pt <= 1.0)   * (0.50) +
                          (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0   && pt <= 1.0e2) * (0.83) +
                          (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0e2)                * (0.90) +
                          (abs(eta) > 2.5)                                                  * (0.00)}
}

##########################
# Muon tracking efficiency
##########################

module Efficiency MuonTrackingEfficiency {
  set InputArray ParticlePropagator/muons
  set OutputArray muons

  # set EfficiencyFormula {efficiency formula as a function of eta and pt}

  # tracking efficiency formula for muons
  set EfficiencyFormula {                                                    (pt <= 0.1)   * (0.00) +
                                            (abs(eta) <= 1.5) * (pt > 0.1   && pt <= 1.0)   * (0.75) +
                                            (abs(eta) <= 1.5) * (pt > 1.0)                  * (0.99) +
                          (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1   && pt <= 1.0)   * (0.70) +
                          (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0)                  * (0.98) +
                          (abs(eta) > 2.5)                                                  * (0.00)}
}

########################################
# Momentum resolution for charged tracks
########################################

module MomentumSmearing ChargedHadronMomentumSmearing {
  set InputArray ChargedHadronTrackingEfficiency/chargedHadrons
  set OutputArray chargedHadrons

  # set ResolutionFormula {resolution formula as a function of eta and pt}

  # resolution formula for charged hadrons
  set ResolutionFormula {                  (abs(eta) <= 1.5) * (pt > 0.1   && pt <= 1.0)   * (0.02) + \
                                           (abs(eta) <= 1.5) * (pt > 1.0   && pt <= 1.0e1) * (0.01) + \
                                           (abs(eta) <= 1.5) * (pt > 1.0e1 && pt <= 2.0e2) * (0.03) + \
                                           (abs(eta) <= 1.5) * (pt > 2.0e2)                * (0.05) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1   && pt <= 1.0)   * (0.03) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0   && pt <= 1.0e1) * (0.02) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0e1 && pt <= 2.0e2) * (0.04) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 2.0e2)                * (0.05)}
}

#################################
# Energy resolution for electrons
#################################

module EnergySmearing ElectronEnergySmearing {
  set InputArray ElectronTrackingEfficiency/electrons
  set OutputArray electrons

  set ResolutionFormula { \
     (abs(eta)< 0.1)*energy*sqrt(pow(0.09372/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 0.1)*(abs(eta)< 0.2) * energy*sqrt(pow(0.09555/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 0.2)*(abs(eta)< 0.3) * energy*sqrt(pow(0.09622/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 0.3)*(abs(eta)< 0.4) * energy*sqrt(pow(0.10007/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 0.4)*(abs(eta)< 0.5) * energy*sqrt(pow(0.10707/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 0.5)*(abs(eta)< 0.6) * energy*sqrt(pow(0.11536/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 0.6)*(abs(eta)< 0.7) * energy*sqrt(pow(0.12471/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 0.7)*(abs(eta)< 0.8) * energy*sqrt(pow(0.13641/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 0.8)*(abs(eta)< 0.9) * energy*sqrt(pow(0.15039/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 0.9)*(abs(eta)< 1.0) * energy*sqrt(pow(0.16067/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 1.0)*(abs(eta)< 1.1) * energy*sqrt(pow(0.17159/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 1.1)*(abs(eta)< 1.2) * energy*sqrt(pow(0.18019/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 1.2)*(abs(eta)< 1.3) * energy*sqrt(pow(0.19383/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 1.3)*(abs(eta)< 1.37) * energy*sqrt(pow(0.21058/sqrt(energy), 2) + pow(0.012, 2))+ \
     (abs(eta)>= 1.37)*(abs(eta)< 1.6) * energy*sqrt(pow(0.22071/sqrt(energy), 2) + pow(0.018, 2))+ \
     (abs(eta)>= 1.6)*(abs(eta)< 1.7) * energy*sqrt(pow(0.19193/sqrt(energy), 2) + pow(0.018, 2))+ \
     (abs(eta)>= 1.7)*(abs(eta)< 1.8) * energy*sqrt(pow(0.18400/sqrt(energy), 2) + pow(0.018, 2))+ \
     (abs(eta)>= 1.8)*(abs(eta)< 1.9) * energy*sqrt(pow(0.16564/sqrt(energy), 2) + pow(0.018, 2))+ \
     (abs(eta)>= 1.9)*(abs(eta)< 2.0) * energy*sqrt(pow(0.16168/sqrt(energy), 2) + pow(0.018, 2))+ \
     (abs(eta)>= 2.0)*(abs(eta)< 2.1) * energy*sqrt(pow(0.16049/sqrt(energy), 2) + pow(0.018, 2))+ \
     (abs(eta)>= 2.1)*(abs(eta)< 2.2) * energy*sqrt(pow(0.16759/sqrt(energy), 2) + pow(0.018, 2))+ \
     (abs(eta)>= 2.2)*(abs(eta)< 2.3) * energy*sqrt(pow(0.16665/sqrt(energy), 2) + pow(0.018, 2))+ \
     (abs(eta)>= 2.3)*(abs(eta)< 2.4) * energy*sqrt(pow(0.16665/sqrt(energy), 2) + pow(0.018, 2))+ \
     (abs(eta)>= 2.4)* energy*sqrt(pow(0.16665/sqrt(energy), 2) + pow(0.018, 2))}
}

###############################
# Momentum resolution for muons
###############################

module MomentumSmearing MuonMomentumSmearing {
  set InputArray MuonTrackingEfficiency/muons
  set OutputArray muons

  set ResolutionFormula { (\
  ((abs(eta) < 1.1)*(pt < 80) + (abs(eta) > 1.1)*(pt < 20))*( \
     (abs(eta) < 1.05)*sqrt(pow(0.25/pt, 2) + pow(0.0327, 2) + pow(0.168*pt/1000., 2)) + \
     (abs(eta) >= 1.05)*(abs(eta) < 1.7)*sqrt(pow(0.0649, 2) + pow(0.336*pt/1000., 2)) + \
     (abs(eta) >= 1.7)*(abs(eta) < 2.0)*sqrt(pow(0.0379, 2) + pow(0.196*pt/1000., 2)) + \
     (abs(eta) > 2.0)*sqrt(pow(0.15/pt, 2) + pow(0.0282, 2) + pow(0.469*pt/1000., 2))) + \
  (pt > 100)*( \
     (abs(eta) < 1.05)*sqrt(pow(0.0155, 2) + pow(0.417*pt/1000., 2))+ \
     (abs(eta) >= 1.05)*(abs(eta) < 1.7)*sqrt(pow(0.0255, 2) + pow(0.801*pt/1000., 2))+ \
     (abs(eta) >= 1.7)*(abs(eta) < 1.9)*sqrt(pow(0.0332, 2) + pow(0.985*pt/1000., 2))+ \
     (abs(eta) >= 1.9)*(abs(eta) < 2.0)*sqrt(pow(0.0332, 2) + pow(0.985*pt/1000.*sinh(eta), 2))+ \
     (abs(eta) >= 2.0)*sqrt(pow(0.0486, 2) + pow(0.069*pt/1000.*sinh(eta), 2))) + \
  ((abs(eta) < 1.1)*(pt>80)*(pt < 100) + (abs(eta) > 1.1)*(pt>20)*(pt < 100))*(1/2*sqrt( \
     pow( \
        (abs(eta) < 1.05)*sqrt(pow(0.25/pt, 2) + pow(0.0327, 2) + pow(0.168*pt/1000., 2)) + \
        (abs(eta) >= 1.05)*(abs(eta) < 1.7)*sqrt(pow(0.0649, 2) + pow(0.336*pt/1000., 2)) + \
        (abs(eta) >= 1.7)*(abs(eta) < 2.0)*sqrt(pow(0.0379, 2) + pow(0.196*pt/1000., 2)) + \
        (abs(eta) > 2.0)*sqrt(pow(0.15/pt, 2) + pow(0.0282, 2) + pow(0.469*pt/1000., 2)), 2) + \
     pow( \     
        (abs(eta) < 1.05)*sqrt(pow(0.0155, 2) + pow(0.417*pt/1000., 2))+ \
        (abs(eta) >= 1.05)*(abs(eta) < 1.7)*sqrt(pow(0.0255, 2) + pow(0.801*pt/1000., 2))+ \
        (abs(eta) >= 1.7)*(abs(eta) < 1.9)*sqrt(pow(0.0332, 2) + pow(0.985*pt/1000., 2))+ \
        (abs(eta) >= 1.9)*(abs(eta) < 2.0)*sqrt(pow(0.0332, 2) + pow(0.985*pt/1000.*sinh(eta), 2))+ \
        (abs(eta) >= 2.0)*sqrt(pow(0.0486, 2) + pow(0.069*pt/1000.*sinh(eta), 2)), 2)))) }
}

##############
# Track merger
##############

module Merger TrackMerger {
# add InputArray InputArray
  add InputArray ChargedHadronMomentumSmearing/chargedHadrons
  add InputArray ElectronEnergySmearing/electrons
#  add InputArray MuonMomentumSmearing/muons
  set OutputArray tracks
}

#############
# Calorimeter
#############

module OldCalorimeter Calorimeter {
  set ParticleInputArray ParticlePropagator/stableParticles
  set TrackInputArray TrackMerger/tracks

  set TowerOutputArray towers
  set PhotonOutputArray photons

  set EFlowTrackOutputArray eflowTracks
  set EFlowTowerOutputArray eflowTowers

  #set EFlowPhotonOutputArray eflowPhotons
  #set EFlowNeutralHadronOutputArray eflowNeutralHadrons


   set pi [expr {acos(-1)}]
  set PhiBins {}
  for {set i -18} {$i <= 18} {incr i} {
    add PhiBins [expr {$i * $pi/18.0}]
  }
  foreach eta {-3.2 -2.5 -2.4 -2.3 -2.2 -2.1 -2 -1.9 -1.8 -1.7 -1.6 -1.5 -1.4 -1.3 -1.2 -1.1 -1 -0.9 -0.8 -0.7 -0.6 -0.5 -0.4 -0.3 -0.2 -0.1 0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2 2.1 2.2 2.3 2.4 2.5 2.6 3.3} {
    add EtaPhiBins $eta $PhiBins
  }
  set PhiBins {}
  for {set i -9} {$i <= 9} {incr i} {
    add PhiBins [expr {$i * $pi/9.0}]
  }
  foreach eta {-4.9 -4.7 -4.5 -4.3 -4.1 -3.9 -3.7 -3.5 -3.3 -3 -2.8 -2.6 2.8 3 3.2 3.5 3.7 3.9 4.1 4.3 4.5 4.7 4.9} {
    add EtaPhiBins $eta $PhiBins
  }
  add EnergyFraction {0} {0.0 1.0}
  add EnergyFraction {11} {1.0 0.0}
  add EnergyFraction {22} {1.0 0.0}
  add EnergyFraction {111} {1.0 0.0}
  add EnergyFraction {12} {0.0 0.0}
  add EnergyFraction {13} {0.0 0.0}
  add EnergyFraction {14} {0.0 0.0}
  add EnergyFraction {16} {0.0 0.0}
  #@@newInvisibles@@

  add EnergyFraction {1000012} {0.0 0.0}
  add EnergyFraction {5000012} {0.0 0.0}
  add EnergyFraction {1000022} {0.0 0.0}
  add EnergyFraction {1000023} {0.0 0.0}
  add EnergyFraction {1000025} {0.0 0.0}
  add EnergyFraction {1000035} {0.0 0.0}
  add EnergyFraction {1000039} {0.0 0.0}
  add EnergyFraction {1000045} {0.0 0.0}
  add EnergyFraction {310} {0.3 0.7}
  add EnergyFraction {3122} {0.3 0.7}
  set ECalResolutionFormula {                  (abs(eta) <= 3.2) * sqrt(energy^2*0.0017^2 + energy*0.101^2) +
                              (abs(eta) > 3.2 && abs(eta) <= 4.9) * sqrt(energy^2*0.0350^2 + energy*0.285^2)}
  set HCalResolutionFormula {                  (abs(eta) <= 1.7) * sqrt(energy^2*0.0302^2 + energy*0.5205^2+1.59^2) +
                              (abs(eta) > 1.7 && abs(eta) <= 3.2) * sqrt(energy^2*0.0500^2 + energy*0.706^2) +
                              (abs(eta) > 3.2 && abs(eta) <= 4.9) * sqrt(energy^2*0.09420^2 + energy*0.075^2)}

}

####################
# Energy flow merger
####################

module Merger EFlowMerger {
# add InputArray InputArray
  add InputArray Calorimeter/eflowTracks
  add InputArray Calorimeter/eflowTowers
  set OutputArray eflow
}



###################
# Photon efficiency
###################


###################
# Missing ET merger
###################

module Merger MissingET {
# add InputArray InputArray
  add InputArray EFlowMerger/eflow
  set MomentumOutputArray momentum
}



############
# Jet finder
############


module FastJetFinder FastJetFinder {
  set InputArray EFlowMerger/eflow
  set OutputArray jets

  # algorithm: 1 CDFJetClu, 2 MidPoint, 3 SIScone, 4 kt, 5 Cambridge/Aachen, 6 antikt

  set JetAlgorithm 6
  set ParameterR 0.4
  set ConeRadius 0.5
  set SeedThreshold 1.0
  set ConeAreaFraction 1.0
  set AdjacencyCut 2
  set OverlapThreshold 0.75
  set MaxIterations 100
  set MaxPairSize 2
  set Iratch 1
  set JetPTMin 10.0
}


module TaggingParticlesSkimmer TagSkimmer {

  set ParticleInputArray Delphes/allParticles
  set PartonInputArray Delphes/partons
  
  set OutputArray particles

  set PTMin 1.0
  set EtaMax 5.0
}

##################
# Jet Energy Scale
##################

module EnergyScale JetEnergyScale {
  set InputArray FastJetFinder/jets
  set OutputArray jets

  # scale formula for jets
  # set ScaleFormula {  sqrt( (3.0 - 0.2*(abs(eta)))^2 / pt + 1.0 )  }
  set ScaleFormula { 1.08 }

}

module Merger FinalTrackMerger {
# add InputArray InputArray
  add InputArray TrackMerger/tracks
  add InputArray MuonMomentumSmearing/muons
  set OutputArray tracks
}

##################
# ROOT tree writer
##################

# tracks, towers and eflow objects are not stored by default in the output.
# if needed (for jet constituent or other studies), uncomment the relevant
# "add Branch ..." lines.

module TreeWriter TreeWriter {
# add Branch InputArray BranchName BranchClass
  add Branch TagSkimmer/particles Particle GenParticle

  add Branch TrackMerger/tracks Track Track
  add Branch EFlowMerger/eflow Tower Tower

  add Branch JetEnergyScale/jets Jet Jet
  add Branch ElectronEnergySmearing/electrons Electron Electron
  add Branch Calorimeter/photons Photon Photon
  add Branch MuonMomentumSmearing/muons Muon Muon
  add Branch MissingET/momentum MissingET MissingET
}

