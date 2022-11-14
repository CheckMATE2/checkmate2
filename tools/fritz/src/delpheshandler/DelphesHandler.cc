#include "DelphesHandler.h"
#define _FILE_OFFSET_BITS_64

/* Most of the following lines of code are just reordered versions from the
 * Delphes-internal sources "DelphesHepMC.Cpp, DelphesSTDHep.cpp" and
 * "DelphesPythia8.cpp".
 */

DelphesHandler::DelphesHandler() {
    outputRootFile = NULL;
    treeWriterCM = NULL;
    treeWriter = NULL;
    branchEvent = NULL;
    confReader = NULL;
    mainDelphes = NULL;
    factory = NULL;
    stableParticleOutputArray = NULL;
    allParticleOutputArray = NULL;
    partonOutputArray = NULL;
    dHepmcReader = NULL;
    dStdhepReader = NULL;
    dLhefReader = NULL;
    readStopWatch = NULL;
    procStopWatch = NULL;
#ifdef HAVE_PYTHIA
    pHandler = NULL;
    mainPythia = NULL;
#endif
    delphesLogFile = "delphes.log";
    hasEvents = true;
    name = "delpheshandler";
}

DelphesHandler::~DelphesHandler() {
    delete dHepmcReader;
    delete dStdhepReader;
    delete readStopWatch;
    delete procStopWatch;
    delete mainDelphes;
    delete confReader;
    delete treeWriter;
    delete treeWriterCM;
    delete outputRootFile;
}

static const std::string keyName = "name";
static const std::string keySettings = "settings";
static const std::string keyPythiaHandler = "pythiahandler";
static const std::string keyEventFile = "eventfile";
static const std::string keyLogFile = "logfile";
static const std::string keyOutputFile = "outputfile";

static void unknownKeys(Properties props) {
    std::vector<std::string> knownKeys;
    knownKeys.push_back(keyName);
    knownKeys.push_back(keySettings);
    knownKeys.push_back(keyPythiaHandler);
    knownKeys.push_back(keyEventFile);
    knownKeys.push_back(keyLogFile);
    knownKeys.push_back(keyOutputFile);
    warnUnknownKeys(
        props,
        knownKeys,
        props["name"],
        "Unknown key for a DelphesHandler section"
        );
}

void DelphesHandler::setupCommon(Properties props) {
    std::string logFile = lookupOrDefault(props, keyLogFile, "delphes.log");
    std::string outputFile = lookupOrDefault(props, keyOutputFile, "");
    std::string settings = lookupRequired(
            props,
            keySettings,
            name,
            "settings is required."
            );
    initialiseDelphes(settings, logFile, outputFile);
}

#ifdef HAVE_PYTHIA
void DelphesHandler::setup(
        Properties props,
        std::map<std::string,EventFile> eventFiles,
        std::map<std::string,PythiaHandler*> pythiaHandler
        ) {
    name = props["name"];
    unknownKeys(props);
    std::pair<bool,std::string> pair;
    // Check if Delphes section in .ini file has a property with key keyPythiaHandler
    // maybeLookup returns (bool existent, string name)
    pair = maybeLookup(props, keyPythiaHandler);
    bool havePythia = pair.first;
    std::string pythiaLabel = pair.second;

    // Same as above for events
    pair = maybeLookup(props, keyEventFile);
    bool haveEvents = pair.first;
    std::string eventsLabel = pair.second;

    if (havePythia && haveEvents) {
        Global::abort(
                name,
                "PythiaHandler and EventFile can not be combined."
                );
    }
    if (!havePythia && !haveEvents) {
        Global::abort(
                name,
                "One of PythiaHandler and EventFile is required."
                );
    }
    if (havePythia) {
        pHandler = lookupRequired(
                pythiaHandler,
                pythiaLabel,
                name,
                "Can not find PythiaHandler with label "+pythiaLabel
                );
        Global::print(
                name,
                "Initialising Delphes via linking to "+pHandler->name
                );
        setup(props,pHandler);
    } else {
        EventFile file = lookupRequired(
                eventFiles,
                eventsLabel,
                name,
                "Can not find EventFile with label "+eventsLabel
                );
        Global::print(
                name,
                "Initialising Delphes via input event "+file.filepath
                );
        setup(props,file);
    }
}
#else
void DelphesHandler::setup(
        Properties props,
        std::map<std::string,EventFile> eventFiles
        ) {
    unknownKeys(props);
    if (hasKey(props, keyPythiaHandler)) {
        Global::abort(
                name,
                "To link delphes to pythia,"
                " fritz needs to be compiled with pythia support");
    }
    std::string eventsLabel = lookupRequired(
            props,
            keyEventFile,
            name,
            "EventFile is required."
            );

    EventFile file = lookupRequired(
            eventFiles,
            eventsLabel,
            name,
            "Can not find EventFile with label "+eventsLabel
            );
    Global::print(
            name,
            "Initialising Delphes via input event "+file.filepath
            );
    setup(props,file);
}
#endif

#ifdef HAVE_PYTHIA
void DelphesHandler::setup(
        Properties props,
        PythiaHandler *pHandler
    ) {
    mode = PythiaMode;
    setupCommon(props);
    this->pHandler = pHandler;
    mainPythia = pHandler->mainPythia;
    treeWriter->Clear();
    mainDelphes->Clear();
}
#endif
void DelphesHandler::setup(
        Properties props,
        EventFile eventFile
    ) {
    this->eventFile = eventFile;
    std::string inputEventFileName = eventFile.filepath;

    // Figure out if the file is .stdhep, .lhe or .hepmc by looking at the file
    std::ifstream x(inputEventFileName.c_str());
    std::string firstLine;
    x >> firstLine;
    if(firstLine == "HepMC::Version") {
        Global::print(name, "Input File determined to be HepMC.");
        dHepmcReader = new DelphesHepMC2Reader();
        mode = HepMCMode;
    }
    else if(firstLine.find("LesHouchesEvents version") != std::string::npos){
        Global::print(name, "Input File determined to be LHEF.");
        dLhefReader = new DelphesLHEFReader();
        mode = LHEFMode;
    }
    else {
        Global::print(name, "Input File determined to be STDHEP.");
        dStdhepReader = new DelphesSTDHEPReader();
        mode = STDHEPMode;
    }
    setupCommon(props); 
    FILE*inputFile = fopen(inputEventFileName.c_str(), "r");
    if(inputFile == NULL)
        Global::abort(name, "Cannot read "+inputEventFileName);
    fseek(inputFile, 0L, SEEK_END);
    Long64_t length = ftello(inputFile);
    fseek(inputFile, 0L, SEEK_SET);
    if(length <= 0) {
        fclose(inputFile);
        Global::abort(name, "Cannot read "+inputEventFileName);
    }

    Global::redirect_cout(delphesLogFile);
    treeWriter->Clear();
    mainDelphes->Clear();
    if(dHepmcReader) {
        dHepmcReader->SetInputFile(inputFile);
        dHepmcReader->Clear();
    }
    if(dStdhepReader) {
        dStdhepReader->SetInputFile(inputFile);
        dStdhepReader->Clear();
    }
    if(dLhefReader) {
        dLhefReader -> SetInputFile(inputFile);
        dLhefReader -> Clear();
    }
    Global::unredirect_cout();
    Global::print(name, "Input file successfully opened!");
}

bool DelphesHandler::processEvent(int iEvent) {
    /* We have to clear at the beginning of an event
     * to make sure that results are kept for later handlers
     * (like the analysis Handler)
     */
    if (!hasEvents) {
        return false;
    }

    treeWriter->Clear();
    mainDelphes->Clear();

    Global::redirect_cout(delphesLogFile);

    // read event from the correct sourcwe
#ifdef HAVE_PYTHIA
    if(mainPythia) {
        if (!pHandler->hasNextEvent()) {
            hasEvents = false;
            return false;
        }
        readPythiaEvent(iEvent);
        mainDelphes->ProcessTask();
    }
    else
#endif 
    if (dHepmcReader) {
        while(true) {
            if (!dHepmcReader->ReadBlock(factory,
                                         allParticleOutputArray,
                                         stableParticleOutputArray,
                                         partonOutputArray)) {
                hasEvents = false;
                return false; // this means we have reached the end of the file
            }
            if(dHepmcReader->EventReady())
                break;
        }
        mainDelphes->ProcessTask();
        dHepmcReader->AnalyzeEvent(branchEvent,
                                   iEvent,
                                   readStopWatch,
                                   procStopWatch);
    }
    else if (dStdhepReader) {
        while(true) {
            if(!dStdhepReader->ReadBlock(factory,
                                         allParticleOutputArray,
                                         stableParticleOutputArray,
                                         partonOutputArray)) {
                hasEvents = false;
                return false; // this means we have reached the end of the file
            }
            if(dStdhepReader->EventReady())
                break;
        }
        mainDelphes->ProcessTask();
        dStdhepReader->AnalyzeEvent(branchEvent,
                                    iEvent,
                                    readStopWatch,
                                    procStopWatch);
    }
    else if (dLhefReader) {
        while(true) {
            if(!dLhefReader->ReadBlock(factory,
                                         allParticleOutputArray,
                                         stableParticleOutputArray,
                                         partonOutputArray)) {
                hasEvents = false;
                return false; // this means we have reached the end of the file
            }
            if(dLhefReader->EventReady())
                break;
        }
        mainDelphes->ProcessTask();
        dLhefReader->AnalyzeEvent(branchEvent,
                                    iEvent,
                                    readStopWatch,
                                    procStopWatch);
    }
    else{
        Global::abort(name, "no valid input source");
    }

    treeWriter->Fill();
    if(dHepmcReader)
        dHepmcReader->Clear();
    if(dStdhepReader)
        dStdhepReader->Clear();
    Global::unredirect_cout();
    return true;
}

bool DelphesHandler::hasNextEvent() {
    return hasEvents;
}

double DelphesHandler::getCrossSection() {
#ifdef HAVE_PYTHIA
    if (pHandler!=NULL) {
        Global::print(name, "Asking "+pHandler->name+" for cross section information");
        return pHandler->getCrossSection();
    }
#endif
    Global::print(name, "Asking "+eventFile.name+" for cross section information");
    return eventFile.getCrossSection();
}
double DelphesHandler::getCrossSectionErr() {
#ifdef HAVE_PYTHIA
    if (pHandler!=NULL) {
        return pHandler->getCrossSectionErr();
    }
#endif
    return eventFile.getCrossSectionErr();
}

void DelphesHandler::finish() {
    Global::redirect_cout(delphesLogFile);
    mainDelphes->FinishTask();
    treeWriter->Write();
    Global::unredirect_cout();
    Global::print(name, "Delphes successfully finished!");
}

void DelphesHandler::initialiseDelphes(std::string configFile,
                                       std::string logFile,
                                       std::string outputRootFileName) {
    delphesLogFile = logFile;
    // First, set up general Delphes readers and writers
    Global::print(name, "Initialising settings from "+configFile);
    // If output file is required, set it up such that the treeWriter writes to it
    if(outputRootFileName != "") {
        Global::checkIfFileExistsAndRemoveAfterQuery(outputRootFileName);
        outputRootFile = TFile::Open(outputRootFileName.c_str(), "RECREATE");
        if(outputRootFile == NULL)
          Global::abort(name,
                        "can't create output file "+outputRootFileName);
        treeWriter = new ExRootTreeWriter(outputRootFile, "Delphes");
    }
    else
        treeWriter = new ExRootTreeWriter(NULL, "Delphes");

    Global::redirect_cout(delphesLogFile);
    readStopWatch = new TStopwatch();
    procStopWatch = new TStopwatch();

    if (mode == HepMCMode || mode == PythiaMode) {
        branchEvent = treeWriter->NewBranch("Event", HepMCEvent::Class());
    } else if (mode == STDHEPMode || mode == LHEFMode) {
        branchEvent = treeWriter->NewBranch("Event", LHEFEvent::Class());
    } else {
        Global::abort(name, "Unknown Delphes mode");
    }

    confReader = new ExRootConfReader;
    confReader->ReadFile(configFile.c_str());

    mainDelphes = new Delphes("Delphes");
    mainDelphes->SetConfReader(confReader);

    treeWriterCM = new CMExRootTreeWriter();
    treeWriterCM = (CMExRootTreeWriter*)treeWriter;
    mainDelphes->SetTreeWriter(treeWriter);

    factory = mainDelphes->GetFactory();
    allParticleOutputArray = mainDelphes->ExportArray("allParticles");
    stableParticleOutputArray = mainDelphes->ExportArray("stableParticles");
    partonOutputArray = mainDelphes->ExportArray("partons");

    mainDelphes->InitTask();
    Global::unredirect_cout();
    Global::print(name, "Delphes successfully initialised!");
}
#ifdef HAVE_PYTHIA
void DelphesHandler::readPythiaEvent(int iEvent) {
    // Translates Pythia HepMC event into Delphes event format
    HepMCEvent *element;
    Candidate *candidate;
    TDatabasePDG *pdg;
    TParticlePDG *pdgParticle;
    Int_t pdgCode;

    Int_t pid, status;
    Double_t px, py, pz, e, mass;
    Double_t x, y, z, t;

    // event information
    element = static_cast<HepMCEvent *>(branchEvent->NewEntry());

    element->Number = iEvent;
    element->ProcessID = mainPythia->info.code();
    element->MPI = 1;
    element->Weight = mainPythia->info.weight();
    element->Scale = mainPythia->info.QRen();
    element->AlphaQED = mainPythia->info.alphaEM();
    element->AlphaQCD = mainPythia->info.alphaS();

    element->ID1 = mainPythia->info.id1();
    element->ID2 = mainPythia->info.id2();
    element->X1 = mainPythia->info.x1();
    element->X2 = mainPythia->info.x2();
    element->ScalePDF = mainPythia->info.QFac();
    element->PDF1 = mainPythia->info.pdf1();
    element->PDF2 = mainPythia->info.pdf2();

    element->ReadTime = readStopWatch->RealTime();
    element->ProcTime = procStopWatch->RealTime();

    pdg = TDatabasePDG::Instance();
    for(int i = 1; i < mainPythia->event.size(); ++i) {
        Pythia8::Particle &particle = mainPythia->event[i];

        pid = particle.id();
        status = particle.statusHepMC();
        px = particle.px();
        py = particle.py();
        pz = particle.pz();
        e = particle.e();
        mass = particle.m();
        x = particle.xProd();
        y = particle.yProd();
        z = particle.zProd();
        t = particle.tProd();

        candidate = factory->NewCandidate();
        candidate->PID = pid;
        pdgCode = TMath::Abs(candidate->PID);
        candidate->Status = status;
        candidate->M1 = particle.mother1() - 1;
        candidate->M2 = particle.mother2() - 1;
        candidate->D1 = particle.daughter1() - 1;
        candidate->D2 = particle.daughter2() - 1;
        pdgParticle = pdg->GetParticle(pid);
        candidate->Charge = pdgParticle ? Int_t(pdgParticle->Charge()/3.):-999;
        candidate->Mass = mass;
        candidate->Momentum.SetPxPyPzE(px, py, pz, e);
        candidate->Position.SetXYZT(x, y, z, t);

        allParticleOutputArray->Add(candidate);

        if(!pdgParticle)
            continue;
        if(particle.isFinal())
            stableParticleOutputArray->Add(candidate);
        else if(pdgCode <= 5 || pdgCode == 21 || pdgCode == 15)
            partonOutputArray->Add(candidate);
    }
}
#endif


