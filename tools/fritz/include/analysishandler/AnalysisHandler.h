#ifndef ANALYSISOBJECT_H
#define ANALYSISOBJECT_H
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <map>
#include <math.h>
#include <stdexcept>
#include <iostream>
#include <sstream>

#include <signal.h>

#include "TROOT.h"
#include "TApplication.h"

#include "TFile.h"
#include "TObjArray.h"
#include "TStopwatch.h"
#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include "TLorentzVector.h"

#include "modules/Delphes.h"
#include "classes/DelphesFactory.h"

#include "classes/DelphesClasses.h"
#include "CMExRootTreeBranch.h"
#include "CMExRootTreeWriter.h"

#include "DelphesHandler.h"
#include "AnalysisBase.h"

#include "Global.h"

class AnalysisHandler {
public:
    //! Used to map tag names to list of tag values
    typedef std::map<std::string, std::vector<int> > Tag_Map;
    //! Used to map parameter names to parameter values
    typedef std::map<std::string, std::string > Param_Map;


    //! Used to define tags in general
    struct tag_definition {
        std::string tag_name; //!< Unique name
    };

    //! Specific for isolation tags
    struct isolation_tag_definition : tag_definition {
        std::string particleType; //!< electron, muon or tau
        int counter; //!< which internal tag number
        std::string source; //!< 't' for tracks or 'c' for calorimeter
        double dR; //!< dR cone in which candidates should be considered
        double pTmin; //!< pTmin of the considered candidates
        double maxVal; //!< the maximum allowed value
        //! How should the maximum value be calculated
        /** If False: sum of nearby pT must be smaller than maxVal.
         *  If True: sum of nearby pT divided by candidate's pT must be smaller
         *           than maxVal
         */
        bool divideByPTCand;
    };

    //! Specific for jet flavour tags
    struct jet_tag_definition : tag_definition {
        int counter; //!< which internal tag number
        double eff; // efficiency working point, [0,1]
    };


    //! Standard Constructor
    AnalysisHandler();
    //! Virtual Destructor, defined by real daughter classes
    virtual ~AnalysisHandler();

    //! \brief Sets up analyses and links to Delphes or an event file
    //!         as necessary
    //!
    //! \param props properties of this analysis handler
    //! \param label the label of this analysis handler
    //! \conf the complete fritz configuration
    //! \eventFiles all available event files
    //! \delphesHandler all available delphes handler
    void setup(
            Properties props,
            std::string label,
            Config conf,
            std::map<std::string,EventFile> eventFiles,
            std::map<std::string,DelphesHandler*> delphesHandler,
            bool haveRandomSeed,
            int randomSeed
            );

    //! Processes event via all loaded analyses.
    /** \param iEvent the index of the event to be analysed, starting at 0.
     *  \return False if event could not be processed, else True.
     */
    bool processEvent(int iEvent);

    //! Sets cross section for analyses a posteriori
    /* Cross section can either be defined via eventParameters in the setup()
     * functions at the beginning or via this function at the end. This is
     * necessary if Pythia8 is used to calculate the cross section.
     * \input xsect cross section in fb
     * \input xsecterr absolute systematic cross section error in fb
     */
    void setCrossSection(double xsect,
                         double xsecterr = -1.0);

    //! Finalises analyses
    void finish();

    
    //! name which is printed in logfile
    std::string name;
    
protected:
    
    //! Performs experiment dependent initialisation
    virtual void initialize() {};

    //! Performs experiment dependent finalisation
    virtual void finalize() {};

    //! Books analysis, defined by daughter class of the right experiment
    virtual void bookAnalysis(std::string analysisName,
                              Tag_Map whichTags,
                              Param_Map eventParameters ) {};

    //! Performs experiment dependent particle procession
    /** Experiment dependent parts are identification/reconstruction
     *  efficiencies and jet tagging.
     */
    virtual void postProcessParticles();

    //! Links experiment dependent particle lists
    virtual void linkObjects();

    //! List of all booked analyses
    std::vector<AnalysisBase*> listOfAnalyses;

    //! List of all booked jet btags
    std::vector<jet_tag_definition*> listOfJetBTags;
    //! If true, do tau tagging
    bool doJetTauTags;
    //! List of all booked electron isolation tags
    std::vector<isolation_tag_definition*> listOfElectronTags;
    //! List of all booked muon isolation tags
    std::vector<isolation_tag_definition*> listOfMuonTags;
    //! List of all booked photon isolation tags
    std::vector<isolation_tag_definition*> listOfPhotonTags;

    /** @defgroup particlecontainers general eventwise particle lists
     *  These lists are set for each event and stores experiment independent
     *  information
     *  @{
     */
    std::vector<GenParticle*> true_c; //!< true c quarks to perform btagging
    std::vector<GenParticle*> true_b; //!< true b quarks to perform btagging
    std::vector<GenParticle*> true_tau; //!< true visible hadronic taus
    std::vector<Track*> tracks; //!< reconstructed tracks
    std::vector<Tower*> towers; //!< reconstructed calorimeter towers
    std::vector<Jet*> jets; //!< reconstructed jets
    std::vector<Electron*> electrons; //!< true smeared electrons
    std::vector<Muon*> muons; //!< true smeared muons
    std::vector<Photon*> photons; //!< true smeared photons after some pT cut
    ETMiss* missingET; //!< reconstruced missingET without muons
    /** @} */

    /** @defgroup tagcontainers general eventwise tag lists
     *  These lists are set for each event and stores the list of tags for
     *  each particle.
     *  @{
     */
    //! Electron isolation tags
    std::map<Electron*, std::vector<bool> > electronIsolationTags;
    //! Muon isolation tags
    std::map<Muon*, std::vector<bool> > muonIsolationTags;
    //! Photon isolation tags
    std::map<Photon*, std::vector<bool> > photonIsolationTags;
    //! Jet BTags
    std::map<Jet*, std::vector<bool> > jetBTags;
    //! Jet TauTags
    std::map<Jet*, std::vector<bool> > jetTauTags;
    /** @} */

    double eventWeight; //!< weight of the currently processed event

    //! EventFile object
    EventFile eventFile;

    //! DelphesHandler object, if used
    DelphesHandler* dHandler;

    //! ROOT TChain object to read .root file
    TChain* rootFileChain;
    //! object to read .root event trees
    ExRootTreeReader* treeReader;

private:
    //! \brief Sets up b tagging for the AnalysisHandler handlerLabel
    //!
    //! \param conf the complete fritz configuration
    //! \param handlerLabel the label of this AnalysisHandler
    //! \return a map from btag section label to btag id
    std::map<std::string,int> setupBTags(
            Config conf,
            std::string handlerLabel
            );

    //! \brief Sets up tau tagging for the AnalysisHandler handlerLabel
    //!
    //! \param conf the complete fritz configuration
    //! \param handlerLabel the label of this AnalysisHandler
    void setupTauTag(
            Config conf,
            std::string handlerLabel
            );

    //! \brief Sets up isolation criteria for the AnalysisHandler handlerLabel
    //!
    //! The type of isolation, i.e. electron, muon or photon is selected through
    //! keySection
    //!
    //! \param conf the complete fritz configuration
    //! \param handlerLabel the label of this AnalysisHandler
    //! \param keySection key of the type of isolation to set up
    //! \param particle name of the particle whose isolation is set up
    //! \param list the vector to store the isolation in
    //! \return a map from isolation section label to id
    std::map<std::string,int> setupIsolation(
            Config conf,
            std::string handlerLabel,
            std::string keySection,
            std::string particle,
            std::vector<isolation_tag_definition*>& list
            );

    //! \brief Setup all analyses belonging to this handler
    //!
    //! \param conf the complete fritz configuration
    //! \param handlerLabel the label of this AnalysisHandler
    //! \param analysisParameters the analysisParameters map
    //! \param bTagIds map from btag label to btag id
    //! \param electronIsoIds map from electron isolation label to id
    //! \param muonIsoIds map from muon isolation label to id
    //! \param photonIsoIds map from photon isolation label to id
    void setupAnalyses(
            Config conf,
            std::string handlerLabel,
            std::map<std::string,std::string> analysisParameters,
            std::map<std::string,int> bTagIds,
            std::map<std::string,int> electronIsoIds,
            std::map<std::string,int> muonIsoIds,
            std::map<std::string,int> photonIsoIds
            );

    //! \brief Create the analysisParameters map
    //!
    //! \param props Properties of the AnalysisHandler
    //! \return analysisParameters
    std::map<std::string,std::string> setupAnalysisParameters(
            Properties props,
            bool haveRandomSeed,
            int randomSeed
            );

    //! \brief Final setup of the AnalysisHandler object
    //!
    //! \param props Properties of the AnalysisHandler
    //! \param eventFiles all available event files
    //! \param delphesHandler all available delphes handler
    void setupAnalysisHandler(
            Properties props,
            std::map<std::string,EventFile> eventFiles,
            std::map<std::string,DelphesHandler*> delphesHandler
            );

    //! \brief Links analyses to a root file
    //!
    //! \param file EventFile that should be analysed
    void setup(EventFile file);

    //! \brief Link analyses to a delphes handler
    //!
    //! \param dHandler DelphesHandler to link against
    void setup(DelphesHandler* dHandler);

    //! Performes experiment independent analysis initialisation
    /* \sa setup function
     * \input rootInputFile Delphes .root file that should be analyses
     */
    void bookAnalysesViaInputFile(std::string configFile,
                                  std::string logFile,
                                  Param_Map eventParameters);

    //! Fills particle containers for given event
    bool readParticles(int iEvent);

    //! Interal subfunctions to isolate particles
    void isolateElectrons(); //!< isolates electrons
    void isolateMuons(); //!< isolates muons;
    void isolatePhotons(); //!< isolates photons;

    //! text file to store standard output and error of all analyses
    std::string analysisLogFile;

    // FixMe: store analysisParameters vector

    //! Internal ROOT objects which store the event wise information
    TClonesArray *branchGenParticle; //!< truth particles (b, c, tau)
    TClonesArray *branchEvent; //!< general event information
    TClonesArray *branchElectron; //!< truth smeared electrons
    TClonesArray *branchMuon; //!< truth smeared muons
    TClonesArray *branchJet; //!< reconstructed jets
    TClonesArray *branchPhoton; //!< truth smeared photons with ptmin cut
    TClonesArray *branchMissingET; //!< reconstructed ETMiss without muons
    TClonesArray *branchTrack; //!< reconstructed tracks
    TClonesArray *branchTower; //!< reconstruected calorimeter cells

    bool hasEvents;
};

#endif
