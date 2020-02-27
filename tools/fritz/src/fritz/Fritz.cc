/*
 * Fritz.cc
 *
 *  Created on: Feb 26, 2015
 *      Author: daschm
 */

#include "Fritz.h"

#include "FritzConfig.h"
#include "ConfigParser.h"

bool Fritz::interupted = false;

Fritz::Fritz() {
    haveNEvents = false;
    nEvents = 0;
    haveRandomSeed = false;
    signal(SIGINT, signalHandler);
}

Fritz::~Fritz() {
    std::map<std::string,AnalysisHandler*>::iterator ita;
    for (ita=analysisHandler.begin(); ita!=analysisHandler.end(); ita++) {
        delete ita->second;
        ita->second = NULL;
    }

    std::map<std::string,DelphesHandler*>::iterator itd;
    for (itd=delphesHandler.begin(); itd!=delphesHandler.end(); itd++) {
        delete itd->second;
        itd->second = NULL;
    }
#ifdef HAVE_PYTHIA
    std::map<std::string,PythiaHandler*>::iterator itp;
    for (itp=pythiaHandler.begin(); itp!=pythiaHandler.end(); itp++) {
        delete itp->second;
        itp->second = NULL;
    }
#endif
}

void Fritz::signalHandler(int num) {
    interupted = true;
}

void Fritz::initialize(int argc, char* argv[]) {
    srand(time(0));
    // First, read settings from input arguments
    readInput(argc, argv);
    Global::print("Fritz",
                  " >> Successfully initialized and linked all handlers! <<");
}

void Fritz::processEventLoop() {
    int iEvent = 0; // index of currently processed event
    std::string strEvent = "0"; // string version of iEvent
    std::string message = ""; // progress message to be printed
    /* Loop until a handler returns an error, the interrupt signal is called
        or we have reached the desired event limit*/
    Global::print("Fritz", "Starting event loop!");
    while (!interupted && (!haveNEvents || iEvent < nEvents)) {
      if (!processEvent(iEvent)) break;
      iEvent++;
      strEvent = Global::intToStr(iEvent);
      message = "Progress: ";
      /* If there is a max nEvent, show 10-percentage progress.
       * Otherwise, print every 10000th event. For printouts, counting
       * starts with 1.*/
      if (nEvents != 0 && (nEvents < 100 || ((iEvent)%(nEvents/10) == 0)))
	message += Global::intToStr((iEvent)*100/nEvents) + " %";
      else if (nEvents == 0 && (iEvent) % 100 == 0)
	message += strEvent + " Events";
      if (message != "Progress: ")
	Global::print("Fritz", message);
    }
    Global::unredirect_cout();
    Global::print("Fritz", " >> Finalising after " + strEvent + " events. <<");
}

bool Fritz::processEvent(int iEvent) {
    // Any processEvent returns false if something went wrong
    bool running = false;
#ifdef HAVE_PYTHIA
    // Unless all handlers have stopped we are still running
    // If a single handler returns true, this gets set to true
    std::map<std::string,PythiaHandler*>::iterator itp;
    for (itp=pythiaHandler.begin(); itp!=pythiaHandler.end(); itp++) {
        running |= itp->second->processEvent(iEvent);
    }
#endif
    std::map<std::string,DelphesHandler*>::iterator itd;
    for (itd=delphesHandler.begin(); itd!=delphesHandler.end(); itd++) {
        running |= itd->second->processEvent(iEvent);
    }
    std::map<std::string,AnalysisHandler*>::iterator ita;
    for (ita=analysisHandler.begin(); ita!=analysisHandler.end(); ita++) {
        running |= ita->second->processEvent(iEvent);
    }
    return running;
}

void Fritz::finalize() {
    // Finalisation in opposite order of creation
    std::map<std::string,AnalysisHandler*>::iterator ita;
    for (ita=analysisHandler.begin(); ita!=analysisHandler.end(); ita++) {
        ita->second->finish();
    }

    std::map<std::string,DelphesHandler*>::iterator itd;
    for (itd=delphesHandler.begin(); itd!=delphesHandler.end(); itd++) {
        itd->second->finish();
    }
#ifdef HAVE_PYTHIA
    std::map<std::string,PythiaHandler*>::iterator itp;
    for (itp=pythiaHandler.begin(); itp!=pythiaHandler.end(); itp++) {
        itp->second->finish();
    }
#endif
    Global::print("Fritz", " >> Done <<");
}

// Warn the user if the input file contains sections that are not understood by fritz
static void unknownSections(Config conf) {
    std::vector<std::string> knownKeys;
    knownKeys.push_back(keyGlobalSection);
    knownKeys.push_back(keyBTagSection);
    knownKeys.push_back(keyTauTagSection);
    knownKeys.push_back(keyElectronIsoSection);
    knownKeys.push_back(keyMuonIsoSection);
    knownKeys.push_back(keyPhotonIsoSection);
    knownKeys.push_back(keyAnalysisSection);
    knownKeys.push_back(keyAnalysisHandlerSection);
    knownKeys.push_back(keyEventFileSection);
    knownKeys.push_back(keyPythiaHandlerSection);
    knownKeys.push_back(keyDelphesHandlerSection);
    knownKeys.push_back(keyAnalysisHandlerSection);
    warnUnknownKeys(conf, knownKeys, "Fritz", "Unknown section type in input file");
}

void Fritz::setupEventFiles(Config conf) {
    Sections sections = conf[keyEventFileSection];
    std::map<std::string,Properties>::iterator it;
    for (it=sections.begin(); it!=sections.end(); it++) {
        std::string label = it->first;
        Properties props = it->second;
        eventFiles[label] = setupEventFile(props);
    }
}

#ifdef HAVE_PYTHIA
void Fritz::setupPythiaHandler(Config conf) {
    Sections sections = conf[keyPythiaHandlerSection];
    std::map<std::string,Properties>::iterator it;
    for (it=sections.begin(); it!=sections.end(); it++) {
        std::string label = it->first;
        Properties props = it->second;
        PythiaHandler *pHandler = new PythiaHandler();
        pHandler->setup(props);
        pythiaHandler[label] = pHandler;
    }
}
#endif

void Fritz::setupDelphesHandler(Config conf) {
    Sections sections = conf[keyDelphesHandlerSection];
    std::map<std::string,Properties>::iterator it;
    for (it=sections.begin(); it!=sections.end(); it++) {
        std::string label = it->first;
        Properties props = it->second;
        DelphesHandler *dHandler = new DelphesHandler();
#ifdef HAVE_PYTHIA
        dHandler->setup(props,eventFiles,pythiaHandler);
#else
        dHandler->setup(props,eventFiles);
#endif
        delphesHandler[label] = dHandler;
    }
}

void Fritz::setupAnalysisHandler(Config conf) {
    Sections sections = conf[keyAnalysisHandlerSection];
    std::map<std::string,Properties>::iterator it;
    for (it=sections.begin(); it!=sections.end(); it++) {
        std::string label = it->first;
        Properties props = it->second;
        std::string type = lookupRequired(
                props,
                "analysistype",
                "Fritz",
                "analysistype is required"
                );
        AnalysisHandler *aHandler = NULL;
        if (type == "atlas") {
            aHandler = new AnalysisHandlerATLAS();
        } else if (type == "atlas7tev") {
            aHandler = new AnalysisHandlerATLAS_7TeV();
        } else if (type == "atlas8tev") {
            aHandler = new AnalysisHandlerATLAS_8TeV();
        } else if (type == "atlas13tev") {
            aHandler = new AnalysisHandlerATLAS_13TeV();
        } else if (type == "atlas14tev_projected") {
            aHandler = new AnalysisHandlerATLAS_14TeV_projected();
        } else if (type == "atlas14tev_hl_flatbtagger") {
            aHandler = new AnalysisHandlerATLAS_14TeV_HL_FlatBtagger();
        } else if (type == "cms") {
            aHandler = new AnalysisHandlerCMS();
        } else if (type == "cms7tev") {
            aHandler = new AnalysisHandlerCMS_7TeV();
        } else if (type == "cms8tev") {
            aHandler = new AnalysisHandlerCMS_8TeV();
        } else if (type == "cms13tev") {
            aHandler = new AnalysisHandlerCMS_13TeV();
        } else if (type == "cms14tev_projected") {
            aHandler = new AnalysisHandlerCMS_14TeV_projected();
        } else {
            Global::abort("Fritz", "Unknown analysis type "+type);
        }
        aHandler->setup(
                props,
                label,
                conf,
                eventFiles,
                delphesHandler,
                haveRandomSeed,
                randomSeed
                );
        analysisHandler[label] = aHandler;
    }
}

static const std::string keyGlobalNEvents = "nevents";
static const std::string keyGlobalRandomSeed = "randomseed";

static void unknownKeysGlobal(Properties props) {
    std::vector<std::string> knownKeys;
    knownKeys.push_back(keyGlobalNEvents);
    knownKeys.push_back(keyGlobalRandomSeed);
    warnUnknownKeys(
            props,
            knownKeys,
            "Fritz",
            "Unknown key in global section"
            );
}

void Fritz::setupGlobal(Config conf) {
    Sections sections = conf[keyGlobalSection];
    if (sections.size() > 1) {
        Global::abort("Fritz", "There can be only one global section");
    }
    if (sections.size() == 0) {
        return;
    }
    // There is a Global section. Global sections are unnamed, so it should have key "".
    Properties props = lookupRequired(
            sections,
            "",
            "Fritz",
            "The global section must not have a label"
            );
    std::pair<bool,int> pair;
    pair = maybeLookupInt(props, keyGlobalRandomSeed);
    haveRandomSeed = pair.first;
    randomSeed = pair.second;
    if (haveRandomSeed) {
        Global::print("Fritz", "Set random seed to " + Global::intToStr(randomSeed));
        srand(randomSeed);
        Global::randomSeed = randomSeed;
    }
    pair = maybeLookupInt(props, keyGlobalNEvents);
    haveNEvents = pair.first;
    nEvents = pair.second;
}

void Fritz::readInputFile(std::string filepath) {
    Global::print("Fritz", "Initialising handlers from file "+filepath);
    Config conf = parseConfigFile(filepath);
    unknownSections(conf);
    setupGlobal(conf);
    setupEventFiles(conf);
#ifdef HAVE_PYTHIA
    setupPythiaHandler(conf);
#else
    if (hasKey(conf, keyPythiaHandlerSection)) {
        Global::abort("Fritz", "To use pythia you need to compile fritz with pythia support.");
    }
#endif
    setupDelphesHandler(conf);
    setupAnalysisHandler(conf);
}

void printUsageMessage() {
    std::cout << "Usage: fritz configfile" << std::endl;
    std::cout << std::endl;
    std::cout << "Fritz consists of three main handlers: " << std::endl;
    std::cout << "  - event generation, " << std::endl;
    std::cout << "  - detector simulation and " << std::endl;
    std::cout << "  - analysis. " << std::endl;
    std::cout << "They can be activated individually and in any combination."                      << std::endl;
    std::cout << "A combined activation will lead to an internal link of the respective input "    << std::endl;
    std::cout << "output routines such that no intermediate file has to be stored. For example, "  << std::endl;
    std::cout << "loading  pythia and delphes simultaneously will process the generated events "  << std::endl;
    std::cout << "from pythia with delphes without a .hepmc file stored on the computer." << std::endl;
    std::cout << std::endl;
    std::cout << " Note that you will only have reduced functionality if you compiled " << std::endl;
    std::cout << " without Pythia or HePMC linking!" << std::endl;
    std::cout << std::endl;
}

void Fritz::readInput(int argc, char* argv[]) {
    if (argc==1 || argc>2) {
        printUsageMessage();
        exit(1);
    }
    if (argv[1] == "-h" || argv[1] == "--help") {
        printUsageMessage();
        exit(1);
    }
    readInputFile(std::string(argv[1]));
    Global::print("Fritz",
                  "Fritz successfully loaded command line parameters!");
}
