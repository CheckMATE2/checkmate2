#ifndef _REWEIGHTINGHANDLER
#define _REWEIGHTINGHANDLER

#include <string>
#include <vector>

#include "Pythia8Plugins/HepMC2.h"
#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"

#ifdef HAVE_PYTHIA
#include "PythiaHandler.h"
#endif

#include "Reweightor.h"
#include "ReweightingProcInfo.h"
#include "LHEF.h"
#include "EventFile.h"
#include "Global.h"
#include "FritzConfig.h"




class ReweightingHandler {
	friend class DelphesHandler;

public:
	//! Standard Constructor
	ReweightingHandler();
	//! Standard Destructor
	~ReweightingHandler();

	//! \brief Initialise Reweighting Module
    //!
    //! \param props map of parameters
    //! \param eventFiles map of available event files
    //! \param pythiaHandler map of available pythia handlers
#ifdef HAVE_PYTHIA
	void setup(
		Properties props,
 		std::map<std::string,EventFile> eventFiles,
        std::map<std::string,PythiaHandler*> pythiaHandler
    );
#endif

    //! \brief Initialise Reweighting Module
    //!
    //! \param props map of parameters
    //! \param eventFiles map of available event files
	void setup(
		Properties props,
 		std::map<std::string,EventFile> eventFiles
    );

     //! Link Reweighting Module to event file
    void setup(Properties props, EventFile eventFile);
#ifdef HAVE_PYTHIA    
    //! Link Reweighting Module to pythia handler
    void setup(Properties props, PythiaHandler* pHandler);
#endif

	

    //! Reads event from either Pythia8 object or Eventfile (lhe or hepmc) and stores it if needed.
    /** \param iEvent the index of the event to be analysed, starting at 0.
     *  \return False if event could not be reweighted or maxEvent was reached,
     *          else True.
     */
    bool processEvent(int iEvent);

    //! Return true if there is a next event
    bool hasNextEvent();

    //! Finalises Reweighting run
    void finish();

    // initialize TTree Reader for direct input to AnalysisHandler
    ExRootTreeReader* initTreeReader();

    // initialize TTree Reader for direct input to AnalysisHandler
    void fillTreeReader(int iBranch);


    // Name used for logfiles
    std::string name;

    // list of reweighted Events
    std::vector<std::pair<HepMC::GenEvent*,ReweightingProcInfo> > reweightedEvents;

    // list of weightd of reweightted Events
    std::vector<double> reweightedEventWeights;

    // Info object containing information about the process
    std::vector<ReweightingProcInfo> info;


    // number of target parameter points
    int nBranches;

private:
	enum ReweightingMode {
        PythiaMode,
        HepMCMode,
        LHEFMode
    };

    int mode;

    // convert lhef event to HepMC::GenEvent
    bool readLHEFEvent(int iEvent);

    //! extract and return weights of reweighted events
    void fill_info();

    // Only in lhefMode
    LHEF::Reader* lhefReader;

	// object from reweighting module
	Reweightor* reweightor;

	EventFile eventFile;

	std::string reweightingLogFile; //!< path to file which stores output and error
	bool hasEvents; //!< indicator if there still are events available;


#ifdef HAVE_PYTHIA
	PythiaHandler* pHandler;
	Pythia8::Pythia *mainPythia;
    //! Internal object to create HepMC event from Pythia8 if needed
    HepMC::Pythia8ToHepMC* pythiaToHepMC;
#endif

    //! Internal object to store .hepmc file if needed
    HepMC::IO_GenEvent* hepmcInput;
    //! Original event from either Pythia8 or hepmc file
    HepMC::GenEvent* evt;


    //! TTree Reader for direct input to AnalysisHandler
    ExRootTreeReader* treeReader;

};



#endif