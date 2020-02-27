#ifndef _PYTHIAHANDLER
#define _PYTHIAHANDLER

#include "Pythia8/Pythia.h"

#ifdef HAVE_HEPMC
#include "Pythia8Plugins/HepMC2.h"
#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"
#endif

#include "FritzConfig.h"
#include "Global.h"

namespace Pythia8{
class LHAupMadgraph;
}

class PythiaHandler {
    friend class DelphesHandler;
    
public:
    //! Standard Constructor
    PythiaHandler();
    //! Standard Destructor
    ~PythiaHandler();

    //! \brief Initialise Pythia 8
    //!
    //! \param props map of parameters
    void setup(Properties props);

    //! Read initialisation info corresponding to subrun; for multiple
    //! LHE files or matching
    /** \param iSubRun the new Subrun to be initialised
     */
    bool initNextRun();
    
    //! Generates event via Pythia8 and stores it if needed.
    /** \param iEvent the index of the event to be analysed, starting at 0.
     *  \return False if event could not be generated or maxEvent was reached,
     *          else True.
     */
    bool processEvent(int iEvent);

    //! Returns total cross section of all events determined by Pythia8
    /** \return cross section and error in FB
     */
    double getCrossSection();
    double getCrossSectionErr();

    //! Return true if there is a next event
    bool hasNextEvent();
    
    //! Finalises Pythia8 run
    void finish();

    //! Name used for logfiles
    std::string name;
private:
    Pythia8::Pythia* mainPythia; //!< Pythia8 object
    std::string pythiaPath; //!< path to pythia directory in the results
    std::string pythiaLogFile; //!< path to file which stores output and error
    std::string pythiaConfigFile; //!< path to config file for multiple subruns
    int iSubRun; //!< counter for currently processed subrun
    int nSubRuns; //!< total number of subruns
    int iAbort; //!< counter for aborted events
    int nAborts;  //!< max allowed number of aborted events
    int nEvents; //!< max allowed number of events (user given)
    bool hasEvents; //!< indicator if there still are events available;

    // cross section calculation
    void setupXSect(Properties props);
    double kFactor;
    double xsect;
    double xsectErr;
    double xsectErrFactor;
    double xsectThresh;
    bool haveXSect;
    bool haveXSectErr;
    bool haveXSectErrFactor;
    bool haveXSectThresh;
#ifdef HAVE_HEPMC
    //! Internal object to create HepMC event
    HepMC::Pythia8ToHepMC* pythiaToHepMC;
    //! Internal object to store .hepmc file
    HepMC::IO_GenEvent* hepMCOutput;
#endif

    std::string pythiaRndmIn;
    
    //! Variables for merging
    bool useMerge;
    float xsecLHEF, xsecSum;
    int nAcceptRun, nJetEst, nJetMax, nRun;

    std::vector<double> xsecEst;
    std::vector<double> nTrialEst;
    std::vector<double> nAcceptEst;

    //! Variables for MG5
    bool useMG5;
    std::string mgProcCard; //!< path to file with generate commands
    std::string mgRunCard; //!< path to run card, not mandatory
    std::string mgConfigCard; //!< path to config card, not mandatory
    std::string mgParamCard; //!< path to param card 
    std::string mgRunPath; //!< path to param card 
    std::string mgSourcePath; //!< path to MadGraph source directory
    Pythia8::LHAupMadgraph* madgraph;

};



#endif
