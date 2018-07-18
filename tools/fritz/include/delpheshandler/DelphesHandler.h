#ifndef _DELPHESHANDLER
#define _DELPHESHANDLER

#include <iostream>
#include <fstream>
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
#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"
#include "classes/DelphesHepMCReader.h"
#include "classes/DelphesSTDHEPReader.h"
#include "classes/DelphesLHEFReader.h"


#include "CMExRootTreeBranch.h"
#include "CMExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootTreeReader.h"

#ifdef HAVE_PYTHIA
#include "PythiaHandler.h"
#endif
#include "Global.h"
#include "EventFile.h"
#include "ReweightingHandler.h"
#include "FritzConfig.h"

class DelphesHandler {
    friend class AnalysisHandler;

public:
    //! Standard Constructor
    DelphesHandler();
    //! Standard Destructor
    ~DelphesHandler();
    
    //! \brief Setup Delphes from config file
    //!
    //! \param props map containing delphes settings
    //! \param eventFiles map of available event files
    //! \param pythiaHandler map of available pythia handlers
#ifdef HAVE_PYTHIA
    void setup(
            Properties props,
            std::map<std::string,EventFile> eventFiles,
            std::map<std::string,ReweightingHandler*> reweightingHandler,
            std::map<std::string,PythiaHandler*> pythiaHandler
            );
#endif

    //! \brief Setup Delphes from config file
    //!
    //! \param props map containing delphes settings
    //! \param eventFiles map of available event files
    void setup(
            Properties props,
            std::map<std::string,EventFile> eventFiles,
            std::map<std::string,ReweightingHandler*> reweightingHandler
            );


    //! Processes event via Delphes and stores it if needed.
    /** \param iEvent the index of the event to be analysed, starting at 0.
     *  \return False if event could not be processed, else True.
     */
    bool processEvent(int iEvent);

    //! Processes reweighted event via Delphes and stores it if needed.
    /** \param iEvent the index of the event to be analysed, starting at 0.
     *  \param iBranch the index of the reweighting branch
     *  \return False if event could not be processed, else True.
     */
    bool processEvent(int iEvent, int iBranch);


    //! Returns true if there are still events available
    bool hasNextEvent();

    //! Returns the cross section of the events processed by Delphes
    double getCrossSection();

    //! Returns the cross section error of the events processed by Delphes
    double getCrossSectionErr();

    //! Finalize Delphes
    void finish();

    //! Name to be printed in logfiles
    std::string name;

    // indicates whether reweighting mode is on
    bool reweightingOn;

    // number of reweighted Branches
    int nReweightingBranches;
    
private:
    enum DelphesMode {
        PythiaMode,
        ReweightingMode,
        STDHEPMode,
        HepMCMode,
        LHEFMode
    };

    int mode;


    //! Performs setup independent of event source
    void setupCommon(Properties props);
    //! Link Delphes to event file
    void setup(Properties props, EventFile eventFile);
    //! Link Delphes to event file
    void setup(Properties props, ReweightingHandler* rHandler);
    //! Set up Delphes object and potentially create output .root file
    void initialiseDelphes(std::string configFile,
                           std::string logFile,
                           std::string outputRootFileName);
    // in case of pHandler mode, translate Pythia event into Delphes event
    void readPythiaEvent(int iEvent);
    // in case of rHandler mode, translate Reweighting (HepMC) event into Delphes event
    void readReweightingEvent(int iEvent, int iBranch);
    
    // These are needed to read in events and process them further
    Delphes *mainDelphes;
    std::string delphesLogFile;
    DelphesFactory *factory;
    ExRootTreeWriter *treeWriter;
    ExRootConfReader *confReader;
    CMExRootTreeWriter *treeWriterCM;

    // this information is determined individually for a given event
    ExRootTreeBranch *branchEvent;
    TObjArray *stableParticleOutputArray;
    TObjArray *allParticleOutputArray;
    TObjArray *partonOutputArray;

    // only defined in pHandler read mode
#ifdef HAVE_PYTHIA
    PythiaHandler *pHandler;
    Pythia8::Pythia *mainPythia;
    //! Link Delphes to PythiaHandler
    void setup(Properties props, PythiaHandler* pHandler);
#endif

    //only defined in reweighting mode
    ReweightingHandler *rHandler;

    //only defined in hepmc or stdhep mode
    TStopwatch* readStopWatch;
    TStopwatch* procStopWatch;

    // only defined in hepmc read mode
    DelphesHepMCReader*  dHepmcReader;

    // only defined in stdhep read mode
    DelphesSTDHEPReader* dStdhepReader;

    // only defined in lhef read mode
    DelphesLHEFReader* dLhefReader;

    // indicates if there are still events available
    bool hasEvents;
    
    EventFile eventFile;

    // only defined in root write mode
    TFile* outputRootFile;
};


#endif
