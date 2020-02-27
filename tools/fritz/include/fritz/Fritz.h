/*
 * Fritz.h
 *
 *  Created on: Feb 26, 2015
 *      Author: daschm
 */
#ifndef FRITZ_H_
#define FRITZ_H_

#include <signal.h>

#include "Global.h"
#include "DelphesHandler.h"
#include "AnalysisHandlerATLAS.h"
#include "AnalysisHandlerATLAS_7TeV.h"
#include "AnalysisHandlerATLAS_8TeV.h"
#include "AnalysisHandlerATLAS_13TeV.h"
#include "AnalysisHandlerATLAS_14TeV_projected.h"
#include "AnalysisHandlerATLAS_14TeV_HL_FlatBtagger.h"
#include "AnalysisHandlerCMS.h"
#include "AnalysisHandlerCMS_7TeV.h"
#include "AnalysisHandlerCMS_8TeV.h"
#include "AnalysisHandlerCMS_13TeV.h"
#include "AnalysisHandlerCMS_14TeV_projected.h"
#include "EventFile.h"

#ifdef HAVE_PYTHIA
#include "PythiaHandler.h"
#endif 

/*! \mainpage Introduction
 * This documentation describes all classes and functions that are used by the
 * FRITZ tool within CheckMATE. You can find information on all handlers that
 * are accessible, how they set up the respective tools, how they can be linked
 * and which input/output combinations are possible.
 */
//! The main class that takes care of all the sub-handlers.
/** This class is the supervising object of the whole program. It takes care of
 *  the user input and loads all the handlers that are needed in such a way
 *  that they share IO information without storing intermediate files on hard
 *  disk unless explicitly enabled.
 */
class Fritz {
    public:
        //! Standard Constructor
        Fritz();
        //! Standard Destructor
        ~Fritz();

        //! Procedure that initializes all user-required handlers.
        /** This function is called by the program's main routine and is filled
         *  with the argc/argv information of the main program's parameters.
         *  The arguments are parsed and the respective handlers are loaded in
         *  a reasonable way, i.e. by linking them if possible.
         */
        void initialize(int argc,
                        char* argv[]);
        //! Procedure to run the general event loop
        /** Assuming that the handlers have been properly initialized, this
         *  procedure should be called within the program's main() to start the
         *  actual event generation, detector simulation and/or analysis chain
         *  by calling the processEvent(int iEvent) function.
         */
        void processEventLoop();
        //! Procedure to finalize all subhandlers
        /** Should be called in the program's main() in order to run the
         * finalization procedures of all loaded sub-handlers to end the
         * subprograms properly.
         */
        void finalize();

    private:
        //! Procedure to read command line parameters.
        void readInput(int argc,
                       char* argv[]);

        //! Function that parses the config file and then initializes
        //! Fritz and all Handlers specified in the file.
        void readInputFile(std::string filepath);

        //! Takes the config and extracts and initializes all event files
        void setupEventFiles(Config conf);

        //! Takes the config and initializes all delphes handlers
        void setupDelphesHandler(Config conf);

        //! Takes the config and initializes all analysis handlers
        void setupAnalysisHandler(Config conf);

        //! Read the contents of a global section
        void setupGlobal(Config conf);

        //! Runs processEvent of all loaded subhandlers.
        /** \param iEvent the index of the event to be analysed (starting at 0)
         *  \return False if event loop should be stopped (for example
         *   since the end of an input file has been reached), otherwise true.
         */
        bool processEvent(int iEvent);

        //! Static function to catch interrupt signals
        /* If an interrupt signal comes in, interrupted is set to true.
         * \param num parameter that depends on the type of signal (not used)
         */
        static void signalHandler(int num);

        //! Map from event file label to eventfile
        std::map<std::string,EventFile> eventFiles;
        //! Map from delphes handler label to delphes handler
        std::map<std::string,DelphesHandler*> delphesHandler;
        //! Map from analysis handler label to analysis handler
        std::map<std::string,AnalysisHandler*> analysisHandler;

#ifdef HAVE_PYTHIA
        //! Takes the config and initializes all pythia handlers
        void setupPythiaHandler(Config conf);
        //! Map from pythia handler label to pythia handler
        std::map<std::string,PythiaHandler*> pythiaHandler;
#endif

        bool haveNEvents; //!< Has nEvents been set
        int nEvents; //!< Total number of events to be processed
        bool haveRandomSeed; //!< Has randomSeed been set
        int randomSeed; //!< Random seed for this run
        static bool interupted; //!< set to true if interrupt signal is called
};

#endif /* FRITZ_H_ */
