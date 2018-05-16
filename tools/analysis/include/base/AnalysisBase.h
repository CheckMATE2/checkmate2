#ifndef _ANALYSISBASE
#define _ANALYSISBASE

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <map>
#include <math.h>
#include <typeinfo>

#include <TCanvas.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TH1.h>
#include <TObject.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TMath.h>
#include <TMatrixD.h>
#include <TMatrixDEigen.h>

#include "classes/DelphesClasses.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootResult.h"

#include "external/fastjet/JetDefinition.hh"
#include "external/fastjet/ClusterSequence.hh"

#include "ETMiss.h"
#include "FinalStateObject.h"
#include "Units.h"

#include "mt2_bisect.h"
#include "mctlib.h"
#include "mt2bl_bisect.h"
#include "mt2w_bisect.h"
#include "Wrappertopness.h"

#include "Global.h"

/*! \mainpage Introduction
 * This documentation describes all classes and functions that are used within the CheckMATE
 * analysis framework and how to use them in order to implement a given analysis. It also includes
 * the definitions of the most important Delphes classes CheckMATE uses. 
 * 
 * - "AnalysisBase Features" links to detailed documentation of
 *    all member functions and variables defined in AnalysisBase, grouped by common subjects.
 * - "Class Listing" shows all classes used within the CheckMATE analysis framework (including
 *   ones defined in Delphes) and their respective members.
 */
//! The base class which defines the structure and functionality of all CheckMATE analyses
/** This class is a base class all CheckMATE analyses inherit from. It takes care of passing
 *  and storing CheckMATE on-the-fly parameters, reading in the Delphes output ROOT files and providing
 *  convenient vectors of final state objects for each event. 
 */
class AnalysisBase {
    friend class AnalysisHandler; // Need access to members to set them
    friend class AnalysisHandlerATLAS; // Need access to members to set them
    friend class AnalysisHandlerATLAS_7TeV; // Need access to members to set them
    friend class AnalysisHandlerATLAS_8TeV; // Need access to members to set them
    friend class AnalysisHandlerATLAS_13TeV; // Need access to members to set them
    friend class AnalysisHandlerATLAS_14TeV_projected; // Need access to members to set them
    friend class AnalysisHandlerATLAS_14TeV_HL_FlatBtagger; // Need access to members to set them
    friend class AnalysisHandlerCMS; // Need access to members to set them
    friend class AnalysisHandlerCMS_7TeV; // Need access to members to set them
    friend class AnalysisHandlerCMS_8TeV; // Need access to members to set them
    friend class AnalysisHandlerCMS_13TeV; // Need access to members to set them
    friend class AnalysisHandlerCMS_14TeV_projected; // Need access to members to set them
 public:
    //! Constructor function to load the Delphes ROOT file.
    /** The constructor takes various input parameters automatically provided by the CheckMATE code, 
     *  which includes information about the analysed events, the structure of the Delphes output
     *  ROOT file and the directory the output files should be written into. It automatically loads
     *  the right branches of the Delphes ROOT file. */
    AnalysisBase();
    //! Destructor function to free pointers and close opened file streams.
    virtual ~AnalysisBase();
    //! Internal function that starts the event loop in main().
    /** This function is called by CheckMATE within the programs main() routine. It first runs the user 
     *  defined 'initialize()' function. It then starts the event loop, 
     *  during which for each event the final state objects are created and preisolation conditions
     *  are applied before the user defined 'analyze()' function is called. After the loop finished,
     *  the user defined 'finalize() function is called before all booked cutflow and signal tables
     *  are finalised and printed.
     */
    void setup(std::map<std::string, std::vector<int> > whichTagsIn, std::map<std::string, std::string> eventParameters);
    void processEvent(int iEvent);
    void finish();
//TODO Texts

 protected:
    //! Analysis functions the user has to define.
    /** @defgroup analysisbody Analysis Core
     *  These virtual function are only defined in classes that are derived from
     *  this AnalysisBase. They are to be defined by the user and contain the 
     *  actual analysis code.
     *  @{
     */
    //! Function to prepare the analysis.
    /** This function is called once at the beginning of an analysis. It is supposed to set general
     *  analysis information (using setInformation(), setAnalysisName() and setLuminosity() ), book 
     *  files (using bookSignalRegions(), bookCutflowRegions() and bookFile()) and initialise any
     *  global variables the user might need in the analyze() part.*/
    virtual void initialize() {};
    //! Function containing the event wise analysis code
    virtual void analyze() {}; 
    //! Function to finish the analyis.
    virtual void finalize() {}; 
     /** @} */
     

    //! Accessible reconstructed final state objects for each event.
    /** @defgroup containers Final State Objects
     *  Electrons, muons, jets, photons, tracks and towers are all stored using the classes defined within Delphes
     *  (see the <a href="https://cp3.irmp.ucl.ac.be/projects/delphes/wiki/WorkBook/RootTreeDescription">Delphes tree documentation</a>
     *  for more information). Objects of the same type are grouped using standard vector containers, which contains pointers to then
     *  respective objects. These containers are automatially filled for each event before the analyze() function is called. Using
     *  the ignore() function avoids the respective vector from being filled if the user does not need the content (which reduces
     *  the computational effort).
     * 
     *  missingET is defined using the ETMiss class. 
     * 
     * @{
     */
    std::vector<Electron*> electrons; //!< Container of all truth electrons after detector smearing in acceptance range.
    std::vector<Electron*> electronsLoose; //!< Container of 'electrons' objects that pass loose isolation condition.
    std::vector<Electron*> electronsMedium; //!< Container of 'electronsLoose'  objects that pass 'medium' efficiency cut.
    std::vector<Electron*> electronsTight; //!< Container of 'electronsLoose'  objects that pass 'medium' efficiency cut.
    std::vector<Muon*> muons; //!< Container of all truth muons after detector smearing in acceptance range.
    std::vector<Muon*> muonsLoose; //!< Container of all 'muons' that pass loose isolation condition.
    std::vector<Muon*> muonsCombinedPlus; //!< Container of  all 'muonsLoose' that pass 'CBplusST' efficiency.
    std::vector<Muon*> muonsCombined; //!< Container of 'muonsCombinedPlus'  objects that pass 'CB' efficiency.
    std::vector<Jet*> jets; //!< Container of all reconstructed jets.
    std::vector<Photon*> photons; //!< Container of all truth photons after detector smearing.
    std::vector<Photon*> photonsLoose; //!< Container of 'photons' that pass loose isolation condition.
    std::vector<Photon*> photonsMedium; //!< Container of 'photonsLoose'  that pass medium efficiency cut
    std::vector<Track*> tracks; //!< Container of all reconstructed tracks.
    std::vector<Tower*> towers; //!< Container of all calorimeter towers.    
    ETMiss* missingET; //!< Reconstructed missingET vector excluding muons. 
     /** @} */   
     
    
    //! Functions that remove particles checking different conditions
    /** @defgroup cut Check Object Conditions
     *  These functions all take an object or a list of objects and check
     *  conditions as they appear in many analyses. Note that for functions that return
     *  vectors of objects, these vectors have the following properties:
     *   - The order of objects is preserved (a priori: high pt first)
     *   - The output does not contain copies but identically the same objects that went in.
     *  @{
     */
    //! Require objects to have a certain ptmin and lie within a certain eta range
    /** A given set of objects (electrons, jets, ...) is filtered w.r.t minimum pt and a given eta range.
      * the overlap region 1.37 <= |eta| <= 1.52 is common for many objects and hence it has a seperate parameter to be checked.
      * \param unfiltered A vector containing objects of type Electron, Muon, Photon, Jet or FinalStateObject that should be filtered.
      * \param pTmin The required minimum transverse momentum of an object to pass the filter.
      * \param etamin The required minimum pseudorapidity of an object to pass the filter.
      * \param etamax The required maximum pseudorapidity of an object to pass the filter.
      * \param exclude_overlap If set to true, objects within 1.37 <= |pseudorapidity| <= 1.52 are always filtered.
      * \param exclude_higher_pTs if true exclude the candidate if pT>pTmin. If false, exclude the candidate if pT less than pTmin.
      * \param eta1 Allows a different overlap region (lower boundary) to be defined.
      * \param eta1 Allows a different overlap region (upper boundary) to be defined
      * \return A vector containing the objects that passed the filter.
      */ 
    template <class T>
    std::vector<T*> filterPhaseSpace(std::vector<T*> unfiltered, double pTmin = 0., double etamin = -100, double etamax = 100, bool exclude_overlap = false, bool exclude_higher_pTs = false, double eta1=1.37, double eta2=1.52) {
    std::vector<T*> filtered;
    if(!exclude_higher_pTs){
      for (int i = 0; i < unfiltered.size(); i++) {
          T* cand = unfiltered[i];
          if((cand->PT > pTmin) && (cand->Eta > etamin) && (cand->Eta < etamax)) {
                if(!exclude_overlap)
                  filtered.push_back(cand);
                else if( (fabs(cand->Eta) < eta1) || (fabs(cand->Eta) > eta2) )
                  filtered.push_back(cand);
          }
      }
    }
    else{
      for (int i = 0; i < unfiltered.size(); i++) {
          T* cand = unfiltered[i];
          if((cand->PT < pTmin) && (cand->Eta > etamin) && (cand->Eta < etamax)) {
                if(!exclude_overlap)
                  filtered.push_back(cand);
                else if( (fabs(cand->Eta) < eta1) || (fabs(cand->Eta) > eta2) )
                  filtered.push_back(cand);
          }
      }
    }
    return filtered;
    }       
    
    //! Remove objects if they are too close.
    /** A given set X of objects (electrons, jets, ...) is compared to another set Y of objects (not necessarily of the same type).
      * If any object in X is too close to any element in Y, it is *not* returned.
      * \param candidates A vector containing objects of type Electron, Muon, Photon, Jet or FinalStateObject that should be tested.
      * \param neighbours A vector containing objects of type Electron, Muon, Photon, Jet or FinalStateObject the candidates should 
      *                   be tested against. The two lists do not have to contain the same type of objects.
      * \param dR The minimum separation a candidate must have to all neighbours in order to pass.
      * \param pseudorapidity: If parameter is "eta" then dR is calculated as dR=sqrt((d_eta)^2+(d_phi)^2), if parameter is "y" then dR=((d_y)^2+(d_phi)^2) is calculated. The difference for high energy is expected to be very small, because eta=y for high energy.
      * \return A vector containing candidates which all are at least dR away from all neighbours.
      */ 
    template <class X, class Y>
    std::vector<X*> overlapRemoval(std::vector<X*> candidates, std::vector<Y*> neighbours, double dR,std::string pseudorapidity="eta") {
      // If neighbours are empty, return candidates
      if(neighbours.size() == 0)
        return candidates;
      std::vector<X*> passed_candidates;
      // Loop over candidates
      for(int i = 0; i < candidates.size(); i++) {
        bool overlap = false;
        // If a neighbour is too close, declare overlap, break and don't save candidate
        for(int j = 0; j < neighbours.size(); j++) {
          if(pseudorapidity=="eta"){
            if (candidates[i]->P4().DeltaR(neighbours[j]->P4()) < dR) {
              overlap = true;
              break;
            }
          }
          if(pseudorapidity=="y"){
            double y1   = 1./2.*log(  ((candidates[i]->P4()).E() + (candidates[i]->P4()).Pz())
            						/((candidates[i]->P4()).E() -(candidates[i]->P4()).Pz()) );
            double y2   = 1./2.*log(  ((neighbours[j]->P4()).E() + (neighbours[j]->P4()).Pz())
            						/((neighbours[j]->P4()).E() -(neighbours[j]->P4()).Pz()) );
        	double dy   = fabs(y1-y2);
        	double dPhi = candidates[i]->P4().DeltaPhi(neighbours[j]->P4());
        	double dR_y = sqrt(dy*dy + dPhi*dPhi);

            if (dR_y < dR) {
              overlap = true;
              break;
            }
          }
        }
        if (!overlap)
          passed_candidates.push_back(candidates[i]);
      }
      return passed_candidates;
    }
  
    
    //! Remove objects if they are to close, here with two dR1 ,dR2 boundaries.
    /** A given set X of objects (electrons, jets, ...) is compared to another set Y of objects (not necessarily of the same type).
      * If any object in X and Y have dR1 less than dR less tahn dR2 the element X is rejected.
      * \param candidates A vector containing objects of type Electron, Muon, Photon, Jet or FinalStateObject that should be tested.
      * \param neighbours A vector containing objects of type Electron, Muon, Photon, Jet or FinalStateObject the candidates should 
      *                   be tested against. The two lists do not have to contain the same type of objects.
      * \param dR1, dR2. If dR1 less than dR(X_i,Y_j) less than dR2 (for any j) the object X_i will not be in the set which will be returned 
      * \return A vector which contains objects X_i which did not fulfil the removal condition.
      */ 
    template <class X, class Y>
    std::vector<X*> overlapRemoval_2(std::vector<X*> candidates, std::vector<Y*> neighbours, double dR1, double dR2) {
       if(neighbours.size() == 0)
         return candidates;
       std::vector<X*> filtered_candidates;
       std::vector<bool> delete_candidates;
       for(int i=0;i<candidates.size();i++){
         delete_candidates.push_back(false);
       }
       for(int i=0;i<candidates.size();i++){
         for(int j=0;j<neighbours.size();j++){
           double dR=candidates[i]->P4().DeltaR(neighbours[j]->P4());
           if(dR1<dR && dR<dR2){
             delete_candidates[i]=true;//discard candidate
           }   
         }
       }
       for(int i=0;i<delete_candidates.size();i++){
         if(delete_candidates[i]==false){
           filtered_candidates.push_back(candidates[i]);
         }
       }
       return filtered_candidates;
    }

    //! Remove objects if they are to close to any other object in the same list
    /** Similar to overlapRemoval(std::vector<X*> candidates, std::vector<Y*> neighbours, double dR), 
      * but instead comparing the candidates to themselves. The differences are:
      *  - A candidate is never compared to itself
      *  - If a candidate overlaps with another candidate, only the one with lower pt is removed.
      * 
      *  \sa   overlapRemoval(std::vector<X*> candidates, std::vector<Y*> neighbours, double dR)
      */
    template <class X>
    std::vector<X*> overlapRemoval(std::vector<X*> candidates, double dR, bool removeBoth = false) {
      // Same as above for the special case that candidates = neighbours. In that case, the removal 
      // can be formulated more effectively as the sum only has to run half as many times
      if(candidates.size() == 0)
        return candidates;
      std::vector<X*> passed_candidates;
      std::vector<int> flags;
      // Loop over candidates
      for(int i = 0; i < candidates.size(); i++) {
        bool overlap = false;
        // If one of the other, still untested, candidates is too close: remove
        // Since the list is order w.r.t pt, this will always remove the softer object
        for(int j = 0; j < i; j++) {
          if (candidates[i]->P4().DeltaR(candidates[j]->P4()) < dR) {
            overlap = true;
            if (!removeBoth)
              break;
            else
              flags.push_back(j);
          }
        }
        if (!removeBoth && !overlap)
          passed_candidates.push_back(candidates[i]);
        if (removeBoth && overlap)
          flags.push_back(i);
     }
     if (!removeBoth)
       return passed_candidates;
     else {
       bool selected = true;
       for (int i=0; i<candidates.size(); i++) {
         for (int j=0; j<flags.size(); j++) {
           if(i==flags[j]) selected = false;
         }
         if (selected) passed_candidates.push_back(candidates[i]);
       }
       return passed_candidates;
     }
   }

    //! Checks if candidate is between eta_min and eta_max If this is the case the candidate remains with the probability (param_4)! 
    /**  Parameters
      *  - param_1: Candidates which should be checked
      *  - param_2, eta_min
      *  - param_3: eta_max 
      *  - param_4 :Candidates which lie in the eta range remain with param_4 probability (for example param_3=0.6, then 60 percent of the candidates in range |eta|<param_2 remain.).
      */

    template <class X>
    std::vector<X*> filter_candidates_in_eta_range(std::vector<X*> candidates,double eta_min,double eta_max,double percent){
      std::vector<X*> filtered_candidates;
      for(int i=0;i<candidates.size();i++){
        if(candidates[i]->Eta<eta_max && candidates[i]->Eta>eta_min){
          double zz=rand()/ double(RAND_MAX);//TODO: I think I just overtook this from another checkmate code, but I had to add the double converter for running. Maybe check weher in the checkmate code this appears without double converter? Another thing: Where is the random number initializator started?
          if (zz<percent){
            filtered_candidates.push_back(candidates[i]);
          }
        }  
        else{
          filtered_candidates.push_back(candidates[i]);
        }
      }
      return filtered_candidates;
    }


    //! Checks if candidate is between pT_min and pT_max If this is the case the candidate remains with the probability (param_4)! 
    /**  Parameters
      *  - param_1: Candidates which should be checked
      *  - param_2, pT_min
      *  - param_3: pT_max 
      *  - param_4 :Candidates which lie in the pT range remain with param_4 probability (for example param_3=0.6, then 60 percent of the candidates in range |eta|<param_2 remain.).
      */

    template <class X>
    std::vector<X*> filter_candidates_in_pT_range(std::vector<X*> candidates,double pT_min,double pT_max,double percent){
      std::vector<X*> filtered_candidates;
      for(int i=0;i<candidates.size();i++){
        if(candidates[i]->PT<pT_max && candidates[i]->PT>pT_min){
          double zz=rand()/ double(RAND_MAX);//TODO: I think I just overtook this from another checkmate code, but I had to add the double converter for running. Maybe check weher in the checkmate code this appears without double converter? Another thing: Where is the random number initializator started? 
          if (zz<percent){
            filtered_candidates.push_back(candidates[i]);
          }
        }  
        else{
          filtered_candidates.push_back(candidates[i]);
        }
      }
      return filtered_candidates;
    }


   //! Improved version of lepton isolation. Cone radius is antiproportional to lepton pT
    /** This is necessary, because some analyses use pT dependent cones in isolation studies
      * Parameters:
      *  - leptons: vector of lepton pointers which should be tested
      *  - tracks: All tracks
      *  - towers: All towers
      *  - For each lepton dR_track=pT_for_inverse_function_track/pT_Lepton is calculated. If dR_track>dR_track_max, then one sets dR_track=dR_track_max
      *  - For each lepton sumPT is the sum of PT's of tracks which lie in the cone of size dR_track. If PT_lepton*pT_amount_track<=sumPT the lepton is discarded
      *  -If checkTower=true the following steps are done
      *   For each lepton sumET is the sum of ET's of towers which lie in the cone of size dR_tower. If PT_lepton*pT_amount_tower<=sumET the lepton is discarded
      */
    
    template <class X>
    std::vector<X*> Isolate_leptons_with_inverse_track_isolation_cone(std::vector<X*> leptons,std::vector<Track*> tracks,std::vector<Tower*> towers,double dR_track_max,double pT_for_inverse_function_track,double dR_tower,double pT_amount_track,double pT_amount_tower,bool checkTower){
      std::vector<X*> filtered_leptons;
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
          if(neighbour->Particle == leptons[i]->Particle)
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
              if (neighbour->Particles.At(p) == leptons[i]->Particle) {
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


    //! Checks if element x_i is in set y. If x_i is not in set y it will be deleted.
    /** 
      * Parameters:
      *  - x: Set of points that should be tested
      *  - y: Set of points for testing
      */

  template <class X, class Y>
  void checkSubset(std::vector<X*>& x,std::vector<Y*>& y){
  std::vector<bool> del_x;
  for(int i=0;i<x.size();i++){
    del_x.push_back(false);
  }
  for(int i=0;i<x.size();i++){
    bool found=false;
    for(int j=0;j<y.size();j++){
      if(x[i]==y[j]){
        found=true;
      }
    }
    if(found==false){
      del_x[i]=true;
    }
  }
  std::vector<X*> filtered_x;
  for(int i=0;i<x.size();i++){
    if(del_x[i]==false){
      filtered_x.push_back(x[i]);
    }
  }
  x=filtered_x;
}




    //! Remove electrons that are not isolated.
    /** The input list of Electrons is checked against the electron isolation criteria defined by the user
     *  in the analysis manager and only those that pass all criteria are returned.
     * 
     *  **Important** If the user enters N conditions in the AnalysisManager, there will internally be 
     *  N+1 conditions (i.e. the user defined plus one CheckMATE internal). This function only has access
     *  to the N conditions defined by the user, labelled with indices 0 to N-1.
     * \param unfiltered Vector of electrons that should be tested
     * \param relative_flags Vector of integers of the flags to be tested. The first condition entered into 
     *                       the AnalysisManager corresponds to 0, the second (if it exists) to 1, etc.
     *                       If no second parameter is provided, all objects are compared to all existing
     *                       conditions. 
     */
    std::vector<Electron*> filterIsolation(std::vector<Electron*> unfiltered, std::vector<int> relative_flags = std::vector<int>());
    
    //! Remove electrons that are not isolated (simplified function for exactly one condition given as one integer). \sa filterIsolation(std::vector<Electron*> unfiltered, std::vector<int> relative_flags = std::vector<int>())
    std::vector<Electron*> filterIsolation(std::vector<Electron*> unfiltered, int relative_flag);
    
    //! Remove muons that are not isolated \sa filterIsolation(std::vector<Electron*> unfiltered, std::vector<int> relative_flags = std::vector<int>())
    std::vector<Muon*> filterIsolation(std::vector<Muon*> unfiltered, std::vector<int> relative_flags = std::vector<int>());
    
    //! Remove muons that are not isolated (simplified function for exactly one condition given as one integer) \sa filterIsolation(std::vector<Electron*> unfiltered, int relative_flag)
    std::vector<Muon*> filterIsolation(std::vector<Muon*> unfiltered, int relative_flag);
    
    //! Remove photons that are not isolated \sa filterIsolation(std::vector<Electron*> unfiltered, std::vector<int> relative_flags = std::vector<int>())
    std::vector<Photon*> filterIsolation(std::vector<Photon*> unfiltered, std::vector<int> relative_flags = std::vector<int>());
    
    //! Remove photons that are not isolated (simplified function for exactly one condition given as one integer) \sa filterIsolation(std::vector<Electron*> unfiltered, int relative_flag)
    std::vector<Photon*> filterIsolation(std::vector<Photon*> unfiltered, int relative_flag);
    
    //! Checks if candidate jet fulfills given tau identification cut 
    /** If tau tagging was activated in the AnalysisManager, a given jet candidate
     *  can be tested for tau tags according to the three main working points 'loose', 'medium' and 'tight'.
     *  Note that every 'tight' is always also a 'medium' tau jet and similarly every 'medium' is always also 'loose'.
     *  \param candidate The jet candidate to be tested.
     *  \param efficiency The to be tested efficiency 'loose', 'medium' or 'tight'.
     *  \return 'true' if the candidate was tagged as a tau jet, otherwise 'false.
     */
    bool checkTauTag(Jet* candidate, std::string efficiency);
    
    //! Checks if candidate jet fulfills given b-jet identification 
    /** If b tagging was activated in the AnalysisManager, a given jet candidate
     *  can be tested for b-jets giving the respective index of the defined
     *  working point, where the first b-efficiency defined in CheckMATE is tested with index 0,
     *  the second one with 1 etc. The tags are set such that each jet that passed the working point
     * with signal efficiency X will always also pass those with working points with effiencies larger
     * than X. (E.g. every 40% jet will always be a 60% jet too).
     * \param candidate The jet candidate to be tested.
     * \param relative_flag The condition to be tested with 0 being the first defined in the AnalysisManager.
     * \return 'true' if the candidate was tagged as a b jet, otherwise 'false.
     */
    bool checkBTag(Jet* candidate, int relative_tag = 0);
    /** @} */
    
    //! Sort final state object vectors by PT (largest first)
    static bool sortByPT(const FinalStateObject *lhs, const FinalStateObject *rhs);
    
    //! Advanced kinematical variables
    /** @defgroup kinematics Advanced Kinematics
     * These functions calculate advanced kinematical variables.
     * @{
     */
    //! Evaluates the transverse mass \f$m_T\f$. 
    /** The transverse mass is defined as     * 
     *  \f{align*}{ 
     *     m_T := \sqrt{2 |p_T^{\text{vis}}| |p_T^{\text{invis}}| \cos(1-\Delta \phi)}
     *  \f}     * 
     *  where \f$ |p_T^{\text{vis}}|\f$ is the magnitude of the visible particle's momentum in the transverse plane 
     *  and \f$ |p_T^{\text{invis}}|\f$ is the magnitude of the total missing momentum of the event. \f$\Delta \phi\f$
     *  denotes the relative angle between the two vectors in the transverse plane. 
     * 
     * Assuming a particle of mass \f$M\f$ decays into a visible particle and an invisible particle which solely contributes to
     * the missing momentum vector, then it can be shown that \f$m_T \leq M\f$
     */ 
    double mT(const TLorentzVector & vis, const TLorentzVector & invis, const double m_invis=0.);
    
    //! Evaluates \f$m_{T2}\f$.
    /** The definition is
     * 
     * For more information, see http://arxiv.org/abs/0810.5178.
     */
    double mT2(const TLorentzVector & vis1, const TLorentzVector & vis2, double m_inv, const TLorentzVector & invis = TLorentzVector(0., 0., 0., 0.));

    //! Evaluates normal \f$M_{CT}\f$.
    /** The definition is
     * 
     * For more information, see JHEP 0804:034,2008, http://arxiv.org/abs/0802.2879.
     */
    double mCT(const TLorentzVector & v1, const TLorentzVector & v2);
    
    //! Evaluates boost corrected \f$M_{CT}\f$.
    /** The definition is
     * 
     * For more information, see JHEP 0804:034,2008, http://arxiv.org/abs/0802.2879
     */
    double mCTcorr(const TLorentzVector & v1, const TLorentzVector & v2, const TLorentzVector & vds, const TLorentzVector & invis, const double ecm = 8000.0, const double mxlo = 0.0);
    
    //! Evaluates \f$M_{CT}\f$ transverse.
    /** The definition is
     * 
     * For more information, see http://arxiv.org/abs/0910.1584.
     */    
    double mCTperp(const TLorentzVector & v1, const TLorentzVector & v2, const TLorentzVector & vds, const TLorentzVector & invis);        
    
    //! Evaluates \f$M_{CT}\f$ parallel.
    /** The definition is
     * 
     * For more information, see http://arxiv.org/abs/0910.1584.
     */    
    double mCTparallel(const TLorentzVector & v1, const TLorentzVector & v2, const TLorentzVector & vds, const TLorentzVector & invis);        
          
    //! Evaluates \f$m_{T2}^{bl}\f$.
    /** The definition is
     * 
     * It is also known as asymmetric mT2 (e.g. in atlas_conf_2013_037).
     * For more information, see http://arxiv.org/abs/1203.4813
     */
    double mT2_bl(const TLorentzVector & pl_in, const TLorentzVector & pb1_in, const TLorentzVector & pb2_in, const TLorentzVector & invis = TLorentzVector(0., 0., 0., 0.));    

    //! Evaluates \f$m_{T2}^{w}\f$.
    /** The definition is
     * 
     * For more information, see http://arxiv.org/abs/1203.4813
     */
    double mT2_w(const TLorentzVector & pl_in, const TLorentzVector & pb1_in, const TLorentzVector & pb2_in, const TLorentzVector & invis = TLorentzVector(0., 0., 0., 0.), const double & upper_bound = 2000.0, const double & error_value = 1999.0, const double & scan_step = 0.5);    

    
    //! Evaluates \f$\alpha_T\f$.
    /** The definition is
     * 
     *  (code supplied by CMS)
     */
    double alphaT(const std::vector<Jet*> & jets,  const double thresh_ET = 0.);
    
    //! Evaluates razor.
    /** The definition is
     * 
     *  (code supplied by CMS)
     */
    std::vector<double> razor(const std::vector<TLorentzVector> & obj, const TLorentzVector & invis = TLorentzVector(0., 0., 0., 0.));

    //! Evaluates topness.
    /** The definition is
     * 
     *  Code supplied by Michael Graesser and Jessie Shelton
     *  For more information, see Phys.Rev.Lett. 111 (2013) no.12, 121802 
     */   
    double topness(const TLorentzVector&, const TLorentzVector&, const TLorentzVector&, const TLorentzVector&, const double & sigmat = 15., const double & sigmaW = 5., const double & sigmas = 1000.);        
  
    //! Evaluates super razor.
    /** The definition is
     * 
     * Details given in paper (http://arxiv.org/abs/1310.4827) by Matthew R. Buckley, Joseph D. Lykken, Christopher Rogan, Maria Spiropulu
     *
     * Code supplied by  Alaettin Serhan Mete, Tommaso Lari, Federico Meloni, Iacopo Vivarelli, Daniel Antrim
     * 
     */   
    std::vector<double> superRazor(TLorentzVector, TLorentzVector, const TLorentzVector & met);
    
    //! Evaluates aplanarity.
    /** The definition is in
     *  Phys.Rev.D85 (2012) 034007, arXiv:1112.2567
     */       
    double aplanarity(const std::vector<Jet*> input_jets);     
    
    /** @} */    
    
    
    //! Functions to declare and count signal, cutflow and control regions.
    /** @defgroup regions Signal/Cutflow/Control Region Management
     *  CheckMATE includes convenient means to count events with certain properties. One usually
     *  distinguishes between _signal regions_, which collect events with properties that characterise potential
     *  new physics, _cutflow regions_, which keep track of the individual conditions that lead to 
     *  signal regions to check the correctness of the individual steps, and _control regions_, which 
     *  in rare cases are used to check regions orthogonal to the signal regions for undesired contamination
     *  with signal events. Note that control regions should better be checked in the separate CR.cc analysis 
     *  file that is created if you answered the "Will you implement Control Regions?" question within the 
     *  AnalysisManger with 'yes'. Combining control regions and signal regions within one program is possible,
     *  but usually leads to hard to read code that can easily introduce bugs. 
     * 
     *  CheckMATE first needs to know which regions exist using the bookSignalRegions(), bookCutflowRegions()
     *  and bookControlRegions() functions. Then, a call of countCutflowEvent(), countSignalEvent() or countControlEvent()
     *   at the appropriate step within the analyze() function lets the program internally keep track of the 
     *  event weight that should be counted and - at the end of the run - lists all important information in
     *  cutflow.dat, signal.dat and/or control.dat output files within the CheckMATE results folder.
     * 
     *  **Note** Trying to run countXY on a region that has not been booked will cause the program to automatically
     *  book it as soon as a single event triggers the countXY function. *However* If no event passes the 
     *  unbooked region, it will not appear in any output file, whereas a properly booked region will always 
     *  be present in the output file.
     *  @{ */
    /** This function should be run within the initialize() part of a CheckMATE analysis and ensures that each of
     *  the listed regions is present in the signal.dat output files. In case of signal regions, this function is 
     *  automatically put with the right arguments given the information the user has entered into the AnalysisManager.
     *  To improve readability, the user is free to split a single bookRegion call into many function calls with
     *  shorter arguments. Regions are always listed alphabetically within the output file. Hence, if a certain 
     *  ordering is prefered, one should name the regions accordingly.
     * \param listOfRegions A string of the form "Region1;Region2;Region3;..." of all regions to be booked.
     */ 
    void bookSignalRegions(std::string listOfRegions);  //!< Function to book signal regions.
    void bookControlRegions(std::string listOfRegions); //!< Function to book control regions. \sa bookSignalRegions()
    void bookCutflowRegions(std::string listOfRegions); //!< Function to book cutflow regions. \sa bookSignalRegions()
    
    //! Function to count a given event for a signal region.
    /** Whenever an event within the analyze() function fulfills all properties to consider it for a 
     *  defined signal region, a call of this function will let the code add the weight of the event
     *  internally and consider it for the final output.
     *  \param region The name of the region the event shoud be counted for. This region should be booked using bookSignalRegions().
     */
    inline void countSignalEvent(std::string region) {      
      signalRegions[region] += weight;
      signalRegions2[region] += weight*weight;
    }
    //! Function to count a given event for a control region. \sa countSignalEvent
    inline void countControlEvent(std::string region) {      
      controlRegions[region] += weight;
      controlRegions2[region] += weight*weight;
    }
    //! Function to count a given event for a cutflow region. \sa countSignalEvent
    inline void countCutflowEvent(std::string region) {
      cutflowRegions[region] += weight;
      cutflowRegions2[region] += weight*weight;
    }    
    /** @} */
    
     
    //! Functions and objects to easily handle additional output files.
    /** @defgroup streams File Streams and IO
     *  Normally, a given analysis only has to create the respective cutflow.dat and signal.dat files,
     *  which is done automatically via the booking of signal and cutflow regions. However, for
     *  debugging purposes or further means of validation, it might be required to store intermediate
     *  results in separate files. The bookfile() routine should be used for this purpose for the 
     *  following reasons:
     *     - The booked file is stored in whatever output directory is used in the main CheckMATE runs
     *     - Each separate input event file is associated with a new file, identified by the XXX_ prefix
     *        known from the _signal.dat and _cutflow.dat files
     *     - Unless the second argument is specifically set to 'true', each file starts with the same 
     *        header information that is found in _cutflow.dat and _signal.dat
     * @{
     */
    // File streams for generalized output files
    std::vector<std::ofstream*> fStreams;  //!< Container of output file streams booked via bookFile()
    std::vector<std::string> fNames; //!< Container of output file names booked via bookFile()
    /** @} */
        
    //! Function to book file streams accessible via fStreams and fNames.
    /** @ingroup streams
     *  This function opens a file in the CheckMATE run-specific output directory
     *  for each input event file associating the same XYZ prefixes as for the cutflow and signal
     *  files. 
     *  \param name The file ist stored in \<CheckMATE output directory\>/analysis/\<prefix\>_name .
     *  \param noheader If not set to true, each file automatically starts with
     *  standard header information of the given analysis and the analysed event file. 
     *  \return An integer corresponding to the associated object within the fStreams and fNames vectors.
     */
    int bookFile(std::string name, bool noheader = false); 
        
    
    //! Other useful helper functions and variables
    /** @defgroup miscellaneous
     *  Functions here perform simple tasks and are most commonly run in
     *  initialize() or finalize(). Many of them are automatically set by the
     *  AnalysisManager.
     * @{
     */
    //! Sets the header that is printed on top of all output files.
    /** \param s The string that is to be printed as the header. Linebreaks have to be explicitly set using \\n. and lines should start with '# ' (including the space to 
        declare them as comments for the CheckMATE evaluation module. 
        */
    inline void setInformation(std::string s) {
    information = s;
    };
    
    //! Sets the luminosity
    /** \param l The luminosity of the analysis the input should be normalised to. The ::units objects should be used to give the unit.*/
    inline void setLuminosity(double l) {
      luminosity = l;
    };
    
    //! Sets the analysis name to denote the name of the output files.
    /** \param name The name of the analysis (should correspond to the name of the class) */
    inline void setAnalysisName(std::string name) {
      analysis = name;
    };
        
    //! Normalises number to luminosity and cross section.
    /** This function is useful if one stores data beyond the signal/cutflow/control region data which is normalised automatically.
     *  Example: One can define a histogram in initialize(), fill the bins with data in analyze() using the AnalysisBase::weight of the current event
     *  and normalize() the histogram bins in the finalize() step of the analysis to physical numbers.
     *  \param x The number to be normalised
     *  \returns x * luminosity * crossSectionOfEventFile / (sum of weights)
     */
    inline double normalize(double x) {
    return x*xsect*luminosity/sumOfWeights;
    };

    //! Returns a pointer to a new FinalStateObject that is automatically cleaned at the end of an event loop
    template <class X>
    FinalStateObject* newFinalStateObject(X* particle) {
    // allocate a final state particle pointer and store it before returning it
    FinalStateObject* fso = new FinalStateObject(particle);
    finalStateObjects.push_back(fso);
    return fso;
    }
    
    //! Object to ExRootAnalysis for internal studies
    ExRootResult *result;
    
    //! Does not read out unneeded ROOT information.
    /** If needed, this function should be called within initialize().
     *  \param ignore_what Which information should not be stored? (possible options:
     *   "electrons", "electrons_looseIsolation","electronsMedium","electronsTight","muons",
     *   "muons_looseIsolation","muonsCombinedPlus","muonsCombined","photons","photons_looseIsolation",
     *   "tracks","towers"
     */
    void ignore(std::string ignore_what) {}; // TODO Remove everywhere
    double weight; //!< Current event weight usable for e.g. histograms
    /** @} */

 private:
    // Information on files
    std::string outputFolder;
    std::string outputPrefix;

    // Information about the analysis to be printed at the start of each output file
    std::string analysis;
    std::string information;
    
    // Global parameters
    Long64_t nEvents;
    double sumOfWeights;
    double sumOfWeights2;
    double xsect;
    double xsecterr;
    double luminosity;
        
    // Sums up weights (and weights^2) that fall into control, signal or cutflow regions 
    std::map<std::string, double> controlRegions;
    std::map<std::string, double> signalRegions;    
    std::map<std::string, double> cutflowRegions;
    std::map<std::string, double> controlRegions2;
    std::map<std::string, double> signalRegions2;    
    std::map<std::string, double> cutflowRegions2;
    
    // There might be N tag conditions in total, but this analysis only
    // tests K out of them. This map tells for a given condition, which of the N entries
    // in the tag vectors correspond to the K conditions defined for the analysis.
    std::map<std::string, std::vector<int> > whichTags;
    // For each candidate there is a vector of booleans, each of which tells which of the N
    // overall conditions over all analyses returned true. The whichTags vector has to be used
    // to identify the right subset relevant for this analysis.
    std::map<Electron*, std::vector<bool> > electronIsolationTags;
    std::map<Muon*, std::vector<bool> > muonIsolationTags;
    std::map<Photon*, std::vector<bool> > photonIsolationTags;
    std::map<Jet*, std::vector<bool> > jetBTags;
    std::map<Jet*, std::vector<bool> > jetTauTags;
    
    // Used by alphaT code
    struct fabs_less { 
      bool operator()(const double x, const double y) const { 
        return fabs(x) < fabs(y); 
      } 
    };

    // keeps track of all loaded FinalStateParticles and properly frees them 
    std::vector<FinalStateObject*> finalStateObjects;

};


#endif
