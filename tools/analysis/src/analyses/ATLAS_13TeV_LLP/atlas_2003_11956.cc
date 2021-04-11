#include "atlas_2003_11956.h"
// AUTHOR: K. Rolbiecki
//  EMAIL: krolb@fuw.edu.pl
void Atlas_2003_11956::initialize() {
  setAnalysisName("atlas_2003_11956");          
  setInformation(""
    "# search for long-lived particles decaying into hadrons and at least one displaced muon, long lived top-squarks, R-parity violation\n"
  "");
  setLuminosity(136.0*units::INVFB);      
  bookSignalRegions("MET;Muon");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_2003_11956::analyze() {

  //missingET->addMuons(muonsCombined);  // skip muons
    
  countCutflowEvent("00_all");
  
  double MET = missingET->P4().Pt();
  muonsCombinedPlus = filterPhaseSpace(muonsCombinedPlus, 60., -1.05, 1.05);
  muonsCombined = filterPhaseSpace(muonsCombined, 25., -2.5, 2.5);
  
  bool met_trigger = false;
  bool muon_trigger = false;
  
  if (MET > 100.) {
      met_trigger = true;
      countCutflowEvent("MET_01_trigger");
  }
  
  if (muonsCombinedPlus.size() > 0) {
      muon_trigger = true;
      countCutflowEvent("Muon_01_trigger");
  }
  
  if (met_trigger and MET > 180.) Atlas_2003_11956::selection(muonsCombined.size(), 25., 2.5, "MET");
  if (muon_trigger and MET < 180.) Atlas_2003_11956::selection(muonsCombined.size(), 60., 1.05, "Muon");
  
  return;
  
}

void Atlas_2003_11956::finalize() {
  // Whatever should be done after the run goes here
}       

void Atlas_2003_11956::selection(int muons_count, double muon_ptmin, double muon_etamax, std::string sr) {
  countCutflowEvent(sr+"_02_MET");
  
  if (muons_count == 0 ) return;
  countCutflowEvent(sr+"_03_muon");
  
  if(true_particles.size() < 1) return;
//  countCutflowEvent(sr+"_04_trigger_match");

//  cout << "Event" << "\n";
//  cout << "PID         Px       X        Y      Z        D0       DZ" << "\n";
  std::vector<GenParticle*> selected_muons;
  for(int i = 0; i < true_particles.size(); ++i)  { //cout <<  true_particles[i]->PID << "   " << true_particles[i]->PT << "\n";;           
    if( abs(true_particles[i]->PID) == 13 and true_particles[i]->Status == 1 and true_particles[i]->PT > muon_ptmin and fabs(true_particles[i]->Eta) < muon_etamax  ) { selected_muons.push_back(true_particles[i]);
//    cout <<  true_particles[i]->PID << "   " << true_particles[i]->Px << "   " << true_particles[i]->X << "   " << true_particles[i]->Y << "   " << true_particles[i]->Z << "   " << true_particles[i]->D0 << "   " << true_particles[i]->DZ << "\n";  
    }
        
    }
  
  if (selected_muons.size() == 0 ) return; 
  countCutflowEvent(sr+"_04_trigger_match");
  
  std::vector<GenParticle*> displaced_muons;
  bool d0_check = false;
  bool z0_check = false;
  for (int i = 0; i < selected_muons.size(); ++i) {
    double d0 = -(selected_muons[i]->Px* selected_muons[i]->Y -  selected_muons[i]->Py* selected_muons[i]->X)/ selected_muons[i]->PT ; 
    double dZ = selected_muons[i]->Z - ((selected_muons[i]->Px* selected_muons[i]->Y -  selected_muons[i]->Py* selected_muons[i]->X)/ selected_muons[i]->PT) * selected_muons[i]->Pz/ selected_muons[i]->PT;
    if (fabs(d0) > 2. and fabs(d0) < 300.) {
          d0_check = true;
	  if (fabs(dZ) < 500.) {
              z0_check = true;
              displaced_muons.push_back(selected_muons[i]);
              //cout << "Muon " << i << "   " << selected_muons[i]->X << "    " << selected_muons[i]->Y << "    " << selected_muons[i]->Z << "\n";
          }
      }
      
  }
  //efficiency here?    
  if (!d0_check) return;
  countCutflowEvent(sr+"_05_D0");
  if (!z0_check) return;
  countCutflowEvent(sr+"_06_Z0");
  
  if (rand()/(RAND_MAX+1.) > 0.84) return;
  countCutflowEvent(sr+"_07_fake+cosmic");

  displaced_muons = Isolate_truemuons_with_inverse_track_isolation_cone(displaced_muons, tracks, towers, 0.3, 10., 0.2, 0.06, 0.06, true);
  if (displaced_muons.size() == 0) return;
  countCutflowEvent(sr+"_08_heavyfveto");    
  
  std::vector<DVertex> DVs = Atlas_2003_11956::DVfinder(2., 1., 1.);
  if (DVs.size() < 1) return;
  countCutflowEvent(sr+"_09_DVertex");  
  
  for (int i = 0; i < DVs.size(); i++) {
    if (DVs[i].D0() > 300. or  fabs(DVs[i].Z()) > 300.) {
         DVs.erase(DVs.begin() + i);
         i--;
     }
  }
  
  if (DVs.size() < 1) return;
  countCutflowEvent(sr+"_10_fiducial_far");
  
  for (int i = 0; i < DVs.size(); i++) {
     if ( pow(DVs[i].D0(),2) + pow(DVs[i].Z(), 2) < 16.) {
         DVs.erase(DVs.begin() + i);
         i--;
     }
  }
  
  if (DVs.size() < 1) return;
  countCutflowEvent(sr+"_11_fiducial_close");
  
  for (int i = 0; i < DVs.size(); i++) {
   // cout << i << "    " << DVs[i].X() << "   " << DVs[i].Y() << "   " << DVs[i].Z() << "\n";   
  }

  if (rand()/(RAND_MAX+1.) > 0.58) return; //very blunt method of the material veto
  countCutflowEvent(sr+"_12_material_veto");
  
  for (int i = 0; i < DVs.size(); i++) {
     if ( DVs[i].particles.size() < 3 ) {
         DVs.erase(DVs.begin() + i);
         i--;
     }
  }
  
  if (DVs.size() < 1) return;
  countCutflowEvent(sr+"_13_tracksatDV");
  
  for (int i = 0; i < DVs.size(); i++) 
     if ( DVs[i].P4().M() > 20.) {
         countCutflowEvent(sr+"_14_massatDV");
         countSignalEvent(sr);
         break;         
     }
     
  return;   
}


double DVertex::X(void) {
   return xpos;  
}
    
double DVertex::Y(void) {
   return ypos;  
}

double DVertex::Z(void) {
   return zpos;  
}

double DVertex::D0(void) {
   return impd;   
}

TLorentzVector DVertex::P4(void) {
   return momentum;   
}

DVertex::DVertex(GenParticle* track) {
    
   xpos = track->X;
   ypos = track->Y;
   zpos = track->Z;
   impd = sqrt( pow(xpos,2) + pow(ypos,2) );
   TLorentzVector v1(track->Px, track->Py, track->Pz, sqrt(pow(track->P, 2) + pow(0.14, 2)) );
   momentum = v1;
   particles.push_back(track);
   
}

void DVertex::addtrack(GenParticle* track) {
   
   //weighted
   double mtrack = 0.14; //(pion) or track->Mass;
   double mvertex = momentum.M();
   xpos = (track->X * mtrack + xpos*mvertex)/(mtrack + mvertex); 
   ypos = (track->Y * mtrack + ypos*mvertex)/(mtrack + mvertex); 
   zpos = (track->Z * mtrack + zpos*mvertex)/(mtrack + mvertex); 
   impd = sqrt( pow(xpos,2) + pow(ypos,2) );
   TLorentzVector v1(track->Px, track->Py, track->Pz, sqrt(pow(track->P, 2) + pow(0.14, 2)) );
   momentum = momentum + v1;
   particles.push_back(track);
    
}

void DVertex::merge(DVertex vertex) {
   
   for (int i = 0; i < vertex.particles.size(); i++) {
      bool match = false;
      for (int j = 0; j < particles.size(); j++)
         if ( vertex.particles[i] = particles[j] ) { match = true; break;}
      if ( !match ) this->addtrack(vertex.particles[i]); 
   }
    
}

std::vector< DVertex > Atlas_2003_11956::DVfinder(double d0min, double ptmin, double distmin) {
    
  vector<DVertex> DVlist;

  for(int i=0; i < true_particles.size(); i++) {                          // loop over reconstructed
      
    auto part = true_particles[i];     
    if ( part->Status != 1 ) continue;                           // dismiss it if it is not stable
    if ( abs(part->Charge) == 0 ) continue; 
    double d0 = -(part->Px* part->Y - part->Py* part->X)/ part->PT ;
    if ( d0 < d0min ) continue;                               // dismiss it if it is not sufficiently displaced
    if ( part->PT < ptmin ) continue;                               // dismiss if too soft 
    
    bool included = false;  
    for ( int j = 0; j < DVlist.size(); j++) {        
        double dist = pow(DVlist[j].X() - part->X, 2) + pow(DVlist[j].Y() - part->Y, 2) + pow(DVlist[j].Z() - part->Z, 2);                                                      // the distance between the production vertex of the particle and the stored DVs
        if( dist < distmin ) { 
            DVlist[j].addtrack(part); 
            included = true; 
            break;            
        }              
    }
    
    if (!included) {
       DVertex vert(part);
       DVlist.push_back(vert);
    }
  }
      
  //now merge vertices looking for the closest ones in each round
  double mindist = 1000.;
  int mini = -1;
  int minj = -1;
  while ( mindist < distmin and DVlist.size() > 1 )  {
    double mindist = 1000.;
    int mini = -1;
    int minj = -1;
    for ( int i = 0; i < DVlist.size() - 1; i++) 
      for ( int j = i + 1; j < DVlist.size(); j++) {
         double dist =  pow(DVlist[j].X() - DVlist[i].X(), 2) + pow(DVlist[j].Y() - DVlist[i].Y(), 2) + pow(DVlist[j].Z() - DVlist[i].Z(), 2);    
         if (dist < mindist) {
             mindist = dist;
             mini = i;
             minj = j;
         }
      }
    if (mindist < distmin ) {
      DVlist[mini].merge(DVlist[minj]);
      DVlist.erase(DVlist.begin() + minj);
    }
  }
      
   //finally remove vertices with one track only
   for (int i = 0; i < DVlist.size(); i++) 
      if (DVlist[i].particles.size() < 2) {
          DVlist.erase(DVlist.begin() + i);
          --i;
      }
      
      
   return DVlist;   
}

    std::vector<GenParticle*> Atlas_2003_11956::Isolate_truemuons_with_inverse_track_isolation_cone(std::vector<GenParticle*> leptons,std::vector<Track*> tracks,std::vector<Tower*> towers,double dR_track_max,double pT_for_inverse_function_track,double dR_tower,double pT_amount_track,double pT_amount_tower,bool checkTower){
      std::vector<GenParticle*> filtered_leptons;
      for(int i=0;i<leptons.size();i++){
        double dR_track=0;
        double sumPT=0;
        double sumET=0;
        dR_track=pT_for_inverse_function_track/leptons[i]->PT;
        if(dR_track >dR_track_max){
          dR_track=dR_track_max;
        }
        for (int t = 0; t < tracks.size(); t++) {
          Track* neighbour = tracks[t];

	  // Ignore the lepton's track itself
          if(neighbour->Particle == leptons[i])
            continue;
          if (neighbour->P4().DeltaR(leptons[i]->P4()) > dR_track)
            continue;
          sumPT += neighbour->PT;
        }
        if((leptons[i]->PT)*pT_amount_track<=sumPT){
          continue;
        }
        if(checkTower){
          for (int t = 0; t < towers.size(); t++) {
            Tower* neighbour = towers[t];
	    
            // check tower has 'some' momentum and check dR
            if (neighbour->ET < 0.00001 || neighbour->P4().DeltaR(leptons[i]->P4()) > dR_tower)
              continue;
            // Ignore the lepton's tower
            bool candidatesTower = false;//This testing is different from the testing in the tracks case, because to one track there corresponds one particle, but for one tower there is not only one particle.
            for(int p = 0; p < neighbour->Particles.GetEntries(); p++){
              if (neighbour->Particles.At(p) == leptons[i]) {
                // break the loop and ignore the tower
                candidatesTower = true;
                break;
              }
            }
            if (candidatesTower)
              continue;
            sumET += neighbour->ET;
          }
          if((leptons[i]->PT)*pT_amount_tower<=sumET){
            continue;
          }
        }
        filtered_leptons.push_back(leptons[i]);
      }
      return filtered_leptons;
    }
