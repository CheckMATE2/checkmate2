#include "AnalysisHandler.h"

AnalysisHandler::AnalysisHandler() {
    doJetTauTags = false;
    eventWeight = 0;
    branchGenParticle = NULL;
    branchEvent = NULL;
    branchElectron = NULL;
    branchMuon = NULL;
    branchJet = NULL;
    branchPhoton = NULL;
    branchMissingET = NULL;
    branchTrack = NULL;
    branchTower = NULL;
    dHandler = NULL;
    missingET = NULL;
    rootFileChain = NULL;
    treeReader = NULL;
    analysisLogFile = "analysis";
    hasEvents = true;
    name = "analysishandler";
}

AnalysisHandler::~AnalysisHandler() {
    delete rootFileChain;
    delete treeReader;
    for(int a = 0; a < listOfAnalyses.size(); a++)
        delete listOfAnalyses[a];
}

// all known keys
static const std::string keyName = "name";
static const std::string keyHandler = "analysishandler";
// btag
static const std::string keyBTagEfficiency = "eff";
// isolation
static const std::string keyIsoSource = "source";
static const std::string keyIsoDr = "dr";
static const std::string keyIsoPTmin = "ptmin";
static const std::string keyIsoMaxval = "maxval";
static const std::string keyIsoAbsorrel = "absorrel";
// analysis
static const std::string keyAnalysisElectronIso = "electron_isolation";
static const std::string keyAnalysisMuonIso = "muon_isolation";
static const std::string keyAnalysisPhotonIso = "photon_isolation";
static const std::string keyAnalysisBTags = "jet_btags";
// analysishandler
static const std::string keyAnalysisHandlerType = "analysistype";
static const std::string keyAnalysisHandlerOutputFolder = "outputdirectory";
static const std::string keyAnalysisHandlerOutputPrefix = "outputprefix";
static const std::string keyAnalysisHandlerDelphesHandler = "delpheshandler";
static const std::string keyAnalysisHandlerEventFile = "eventfile";
static const std::string keyAnalysisHandlerLogFile = "logfile";

static const void unknownKeysBTag(Properties props) {
    std::vector<std::string> knownKeys;
    knownKeys.push_back(keyName);
    knownKeys.push_back(keyHandler);
    knownKeys.push_back(keyBTagEfficiency);
    warnUnknownKeys(props,knownKeys,props["name"],"Unknown key in BTag section");
}

static const void unknownKeysTauTag(Properties props) {
    std::vector<std::string> knownKeys;
    knownKeys.push_back(keyName);
    knownKeys.push_back(keyHandler);
    warnUnknownKeys(props,knownKeys,props["name"], "Unknown key in tauTag section");
}

static const void unknownKeysIso(Properties props) {
    std::vector<std::string> knownKeys;
    knownKeys.push_back(keyName);
    knownKeys.push_back(keyHandler);
    knownKeys.push_back(keyIsoSource);
    knownKeys.push_back(keyIsoDr);
    knownKeys.push_back(keyIsoPTmin);
    knownKeys.push_back(keyIsoMaxval);
    knownKeys.push_back(keyIsoAbsorrel);
    warnUnknownKeys(props,knownKeys,props["name"],"Unknown key in Isolation section");
}

static const void unknownKeysAnalysis(Properties props) {
    std::vector<std::string> knownKeys;
    knownKeys.push_back(keyName);
    knownKeys.push_back(keyHandler);
    knownKeys.push_back(keyAnalysisElectronIso);
    knownKeys.push_back(keyAnalysisMuonIso);
    knownKeys.push_back(keyAnalysisPhotonIso);
    knownKeys.push_back(keyAnalysisBTags);
    warnUnknownKeys(props,knownKeys,props["name"],"Unknown key in Analysis section");
}

static const void unknownKeysAnalysisHandler(Properties props) {
    std::vector<std::string> knownKeys;
    knownKeys.push_back(keyName);
    knownKeys.push_back(keyAnalysisHandlerType);
    knownKeys.push_back(keyAnalysisHandlerOutputFolder);
    knownKeys.push_back(keyAnalysisHandlerOutputPrefix);
    knownKeys.push_back(keyAnalysisHandlerDelphesHandler);
    knownKeys.push_back(keyAnalysisHandlerEventFile);
    knownKeys.push_back(keyAnalysisHandlerLogFile);
    warnUnknownKeys(props,knownKeys,props["name"],"Unknown key in AnalysisHandler section");
}

std::map<std::string,int> AnalysisHandler::setupBTags(
        Config conf,
        std::string handlerLabel
        ) {
    std::map<std::string,int> ids;
    Sections sections = conf[keyBTagSection];
    std::map<std::string,Properties>::iterator it;
    int id = 0;
    for (it=sections.begin(); it!=sections.end(); it++) {
        std::string label = it->first;
        Properties props = it->second;
        std::string handler = lookupRequired(
                props,
                keyHandler,
                props["name"],
                "The analysis handler is required in btag sections"
                );
        if (handler!=handlerLabel) {
            continue;
        }
        unknownKeysBTag(props);
        double eff = lookupRequiredDouble(
                props,
                keyBTagEfficiency,
                props["name"],
                "The efficiency is required in b tag sections"
                );
        jet_tag_definition* tag = new jet_tag_definition;
        tag->tag_name = "btag";
        tag->counter = id;
        tag->eff = eff/100;
        listOfJetBTags.push_back(tag);
        ids[label] = id;
	id++;
    }
    return ids;
}

void AnalysisHandler::setupTauTag(
        Config conf,
        std::string handlerLabel
        ) {
    Sections sections = conf[keyTauTagSection];
    std::map<std::string,Properties>::iterator it;
    for (it=sections.begin(); it!=sections.end(); it++) {
        std::string label = it->first;
        Properties props = it->second;
        std::string handler = lookupRequired(
                props,
                keyHandler,
                props["name"],
                "The analysis handler is required in tautag sections"
                );
        if (handler!=handlerLabel) {
            continue;
        }
        unknownKeysTauTag(props);
        doJetTauTags = true;
    }
}

std::map<std::string,int> AnalysisHandler::setupIsolation(
        Config conf,
        std::string handlerLabel,
        std::string keySection,
        std::string particle,
        std::vector<isolation_tag_definition*>& list
        ) {
    Sections sections = conf[keySection];
    std::map<std::string,int> ids;
    std::map<std::string,Properties>::iterator it;
    int id = 0;
    for (it=sections.begin(); it!=sections.end(); it++) {
        std::string label = it->first;
        Properties props = it->second;
        std::string handler = lookupRequired(
                props,
                keyHandler,
                props["name"],
                "The analysis handler is required in btag sections"
                );
        if (handler!=handlerLabel) {
            continue;
        }
        unknownKeysIso(props);
        isolation_tag_definition* tag = new isolation_tag_definition;
        tag->tag_name = keySection;
        tag->particleType = particle;
        tag->counter = id;
        tag->source = lookupRequired(
                props,
                keyIsoSource,
                props["name"],
                "The isolation source is required in isolation sections."
                );
        tag->dR = lookupRequiredDouble(
                props,
                keyIsoDr,
                props["name"],
                "The dR is required in isolation sections"
                );
        tag->pTmin = lookupRequiredDouble(
                props,
                keyIsoPTmin,
                props["name"],
                "The ptmin is required in isolation sections"
                );
        tag->maxVal = lookupRequiredDouble(
                props,
                keyIsoMaxval,
                props["name"],
                "The maxval is required in isolation sections"
                );
        std::string absorrel = lookupRequired(
                props,
                keyIsoAbsorrel,
                props["name"],
                "The absorrel is required in isolation sections"
                );
        if (absorrel == "a") {
            tag->divideByPTCand = false;
        } else if (absorrel == "r") {
            tag->divideByPTCand = true;
        } else {
            Global::abort(name, "Unknown value for key 'absorrel' in isolation section");
        }
        list.push_back(tag);
        ids[label] = id;
	id++;
    }
    return ids;
}

//! \brief Link analysis to btag or isolation
//!
//! \param props The properties of the analysis
//! \param key The key to look for labels of objects to link against
//! \param dict Map from labels of objects to their ids
//! \param msg Name of the object to look for, for error messages
//! \return ids of objects this analysis uses
static std::vector<int> linkAnalysis(
        Properties props,
        std::string key,
        std::map<std::string,int> dict,
        std::string msg
        ) {
    std::vector<int> ids;
    std::vector<std::string> labels = lookupList(props, key);
    std::vector<std::string>::iterator it;
    for (it=labels.begin(); it!=labels.end(); it++) {
        std::string label = *it;
        int id = lookupRequired(
                dict,
                label,
                props["name"],
                "Can not find "+msg+" with label "+label
                );
        ids.push_back(id);

    }
    return ids;
}


void AnalysisHandler::setupAnalyses(
        Config conf,
        std::string handlerLabel,
        std::map<std::string,std::string> analysisParameters,
        std::map<std::string,int> bTagIds,
        std::map<std::string,int> electronIsoIds,
        std::map<std::string,int> muonIsoIds,
        std::map<std::string,int> photonIsoIds
        ) {
    Sections sections = conf[keyAnalysisSection];
    std::map<std::string,Properties>::iterator it;
    int id = 0;
    for (it=sections.begin(); it!=sections.end(); it++) {
        std::string label = it->first;
        Properties props = it->second;
        unknownKeysAnalysis(props);
        std::string handler = lookupRequired(
                props,
                keyHandler,
                props["name"],
                "The analysis handler is required in analysis sections"
                );
        if (handler!=handlerLabel) {
            continue;
        }
        std::map<std::string,std::vector<int> > whichTags;
        whichTags["ElectronIsolation"] = linkAnalysis(
                props,
                keyAnalysisElectronIso,
                electronIsoIds,
                "electron iso"
                );
        whichTags["MuonIsolation"] = linkAnalysis(
                props,
                keyAnalysisMuonIso,
                muonIsoIds,
                "muon iso"
                );
        whichTags["PhotonIsolation"] = linkAnalysis(
                props,
                keyAnalysisPhotonIso,
                photonIsoIds,
                "photon iso"
                );
        whichTags["BJetTagging"] = linkAnalysis(
                props,
                keyAnalysisBTags,
                bTagIds,
                "btag"
                );
        bookAnalysis(label, whichTags, analysisParameters);
    }
}

std::map<std::string,std::string>  AnalysisHandler::setupAnalysisParameters(
        Properties props,
        bool haveRandomSeed,
        int randomSeed
        ) {
    std::map<std::string,std::string> analysisParameters;
    analysisParameters["outputFolder"] = lookupRequired(
            props,
            keyAnalysisHandlerOutputFolder,
            name,
            "outputDirectory is required in an AnalysisHandler section"
            );
    analysisParameters["outputPrefix"] = lookupRequired(
            props,
            keyAnalysisHandlerOutputPrefix,
            name,
            "outputPrefix is required in an AnalysisHandler section"
            );
    if (haveRandomSeed) {
        analysisParameters["randomSeed"] = Global::intToStr(randomSeed);
    }
    return analysisParameters;
}

void AnalysisHandler::setupAnalysisHandler(
        Properties props,
        std::map<std::string,EventFile> eventFiles,
        std::map<std::string,DelphesHandler*> delphesHandler
        ) {
    std::pair<bool,std::string> pair;
    analysisLogFile = lookupOrDefault(props, keyAnalysisHandlerLogFile, "analysis");
    pair = maybeLookup(props, keyAnalysisHandlerEventFile);
    bool haveEventFile = pair.first;
    std::string eventFileLabel = pair.second;
    pair = maybeLookup(props, keyAnalysisHandlerDelphesHandler);
    bool haveDelphesHandler = pair.first;
    std::string delphesHandlerLabel = pair.second;
    if (haveEventFile && haveDelphesHandler) {
        Global::abort(
                name,
                "Only one of eventfile and delpheshandler can be used at the same time in"
                    " an analysishandler section"
                );
    }
    if (!haveEventFile && !haveDelphesHandler) {
        Global::abort(
                name,
                "One of eventfile and delpheshandler is required in an analysishandler section"
                );
    }
    if (haveEventFile) {
        EventFile eventFile = lookupRequired(
                eventFiles,
                eventFileLabel,
                name,
                "Can not find event file with label "+eventFileLabel
                );
        setup(eventFile);
    } else {
        DelphesHandler* dHandler = lookupRequired(
                delphesHandler,
                delphesHandlerLabel,
                name,
                "Can not find delphes handler with label "+delphesHandlerLabel
                );
        setup(dHandler);
    }
}

void AnalysisHandler::setup(
        Properties props,
        std::string label,
        Config conf,
        std::map<std::string,EventFile> eventFiles,
        std::map<std::string,DelphesHandler*> delphesHandler,
        bool haveRandomSeed,
        int randomSeed
        ) {
    name = props["name"];
    props.erase("name");
    Global::print(name, "Initialising AnalysisHandler");
    unknownKeysAnalysisHandler(props);
    std::map<std::string,int> bTagIds = setupBTags(conf, label);
    setupTauTag(conf, label);
    std::map<std::string,int> electronIsoIds = setupIsolation(
            conf,
            label,
            keyElectronIsoSection,
            "electron",
            listOfElectronTags
            );
    std::map<std::string,int> muonIsoIds = setupIsolation(
            conf,
            label,
            keyMuonIsoSection,
            "muon",
            listOfMuonTags
            );
    std::map<std::string,int> photonIsoIds = setupIsolation(
            conf,
            label,
            keyPhotonIsoSection,
            "photon",
            listOfPhotonTags
            );
    std::map<std::string,std::string> analysisParameters = setupAnalysisParameters(
            props,
            haveRandomSeed,
            randomSeed
            );
    setupAnalyses(
            conf,
            label,
            analysisParameters,
            bTagIds,
            electronIsoIds,
            muonIsoIds,
            photonIsoIds
            );
    setupAnalysisHandler(props,eventFiles,delphesHandler);
}

void AnalysisHandler::setup(EventFile file) {
    eventFile = file;
    std::string root_input_file = file.filepath;
    Global::print(name, "Reading ROOT file "+root_input_file);

    // Reading the Delphes Tree
    rootFileChain = new TChain("Delphes");
    rootFileChain->Add(root_input_file.c_str());
    treeReader = new ExRootTreeReader(rootFileChain);

    branchEvent = treeReader->UseBranch("Event");
    branchGenParticle = treeReader->UseBranch("Particle");
    branchJet = treeReader->UseBranch("Jet");
    branchTrack = treeReader->UseBranch("Track");
    branchTower = treeReader->UseBranch("Tower");
    branchElectron = treeReader->UseBranch("Electron");
    branchMuon = treeReader->UseBranch("Muon");
    branchPhoton = treeReader->UseBranch("Photon");
    branchMissingET = treeReader->UseBranch("MissingET");
    if(!branchGenParticle || !branchEvent || !branchJet || !branchTrack ||
       !branchTower || !branchElectron || !branchMuon || !branchPhoton ||
       !branchMissingET) {
        Global::abort(name,
                      "could not link all required branches to the "
                      +dHandler->name+" equivalents!");
    }
    Global::print(name,
                  "successfully loaded branches in ROOT file");
}


void AnalysisHandler::setup( DelphesHandler* dHandlerIn) {
    dHandler = dHandlerIn;

    Global::print(name, "Linking to "+dHandler->name+" tree");
    // link all required branch pointers to the respective pointers
    //  within Delphes in the delphes handlers
    std::set<CMExRootTreeBranch*> branches =
            dHandler->treeWriterCM->GetBranches();
    for (std::set<CMExRootTreeBranch*>::iterator it = branches.begin();
         it != branches.end();
         it++) {
        if ((std::string)(*it)->GetData()->GetName() == "Particle")
                branchGenParticle = (*it)->GetData();
        else if ((std::string)(*it)->GetData()->GetName() == "Event")
                branchEvent= (*it)->GetData();
        else if ((std::string)(*it)->GetData()->GetName() == "Jet")
                branchJet = (*it)->GetData();
        else if ((std::string)(*it)->GetData()->GetName() == "Track")
                branchTrack = (*it)->GetData();
        else if ((std::string)(*it)->GetData()->GetName() == "Tower")
                branchTower = (*it)->GetData();
        else if ((std::string)(*it)->GetData()->GetName() == "Electron")
                branchElectron = (*it)->GetData();
        else if ((std::string)(*it)->GetData()->GetName() == "Muon")
                branchMuon = (*it)->GetData();
        else if ((std::string)(*it)->GetData()->GetName() == "Photon")
                branchPhoton = (*it)->GetData();
        else if ((std::string)(*it)->GetData()->GetName() == "MissingET")
                branchMissingET = (*it)->GetData();
    }
    if(!branchGenParticle || !branchEvent || !branchJet || !branchTrack ||
       !branchTower || !branchElectron || !branchMuon || !branchPhoton ||
       !branchMissingET) {
        Global::abort(name,
                      "could not link all required branches to the"
                      +dHandler->name+" equivalents!");
    }
    Global::print(name,
                  "AnalysisHandler successfully linked to "+dHandler->name);
    initialize(); // virtual, defined by derived classes
}


bool AnalysisHandler::processEvent(int iEvent) {
    if(!hasEvents) {
        return false;
    }
    if(!readParticles(iEvent))
        return false;
    postProcessParticles();
    linkObjects();
    for(int a = 0; a < listOfAnalyses.size(); a++) {
        Global::unredirect_cout(); // This needs to stay, don't ask why - I don't know either.
        Global::redirect_cout(analysisLogFile+"_"+listOfAnalyses[a]->analysis+".log");
        listOfAnalyses[a]->processEvent(iEvent);
        //FIXME It must be possible to do this nicer...
        delete listOfAnalyses[a]->missingET;
    }
    Global::unredirect_cout();
    return true;
}

void AnalysisHandler::setCrossSection(double xsect,
                                      double xsecterr) {
    for(int a = 0; a < listOfAnalyses.size(); a++) {
            listOfAnalyses[a]->xsect = xsect;
            if (xsecterr != -1.0)
                listOfAnalyses[a]->xsecterr = xsecterr;
    }
    Global::print(name, "Analysis updated with sigma = "
                                     +Global::doubleToStr(xsect)+" fb");
    if (xsecterr != -1.0) {
        Global::print(name, "Analysis updated with dSigma = "
                                         +Global::doubleToStr(xsecterr)+" fb");
    }
}


void AnalysisHandler::finish() {
    double xsect;
    double xsectErr;
    if (dHandler!=NULL) {    
        Global::print(name, "Asking "+dHandler->name+" for cross section information");
        xsect = dHandler->getCrossSection();
        xsectErr = dHandler->getCrossSectionErr();
    } else {
        Global::print(name, "Asking "+eventFile.name+" for cross section information");
        xsect = eventFile.getCrossSection();
        xsectErr = eventFile.getCrossSectionErr();
    }
    
    Global::print(
        name,
        "Analyses updated with sigma = " +Global::doubleToStr(xsect)+" fb"
            " and dSigma = " +Global::doubleToStr(xsectErr)+" fb"
        );
    for(int a = 0; a < listOfAnalyses.size(); a++) {
        listOfAnalyses[a]->xsect = xsect;
        listOfAnalyses[a]->xsecterr = xsectErr;
        listOfAnalyses[a]->finish();
    }
    Global::unredirect_cout();
    finalize(); // virtual, defined by derived classes
    Global::print(name, "Analyses successfully finished!");
}

void AnalysisHandler::bookAnalysesViaInputFile(std::string configFile,
                                               std::string logFile,
                                               Param_Map eventParameters) {
    analysisLogFile = logFile;
    Global::print(name, "Reading analysis.ini file");
    std::string line;
    ifstream readFile(configFile.c_str());
    isolation_tag_definition* isotag = NULL;
    jet_tag_definition* jettag = NULL;

    // read file linewise
    while(getline(readFile,line)) {
        // In the main loop, we try to find headers of blocks
        if ((line.find("[ELECTRONISO:") == 0) ||
            (line.find("[MUONISO:") == 0) ||
            (line.find("[PHOTONISO:") == 0)) {
            // Load new isolation
            isotag = new isolation_tag_definition;
            if (line.find("[ELECTRONISO:") == 0) {
                isotag->tag_name = "ELECTRONISO";
                isotag->particleType = "electron";
            }
            else if (line.find("[MUONISO:") == 0) {
                isotag->tag_name = "MUONISO";
                isotag->particleType = "muon";
            }
            else if (line.find("[PHOTONISO:") == 0) {
                isotag->tag_name = "PHOTONISO";
                isotag->particleType = "photon";
            }

            // read counter (tedious in C++ *sigh* )
            int counter = 0;
            line.erase(line.begin(), line.begin()+line.find(" ")+1);
            line.erase(line.begin()+line.find("]"), line.end());
	    std::istringstream buffer(line);
            buffer >> counter;

            isotag->counter = counter;

            // Now, read the properties of the isolation
            while(getline(readFile,line)) {
                if (line == "") {
                    if (isotag) {
                        if (isotag->particleType == "electron")
                            listOfElectronTags.push_back(isotag);
                        else if (isotag->particleType == "muon")
                            listOfMuonTags.push_back(isotag);
                        else if (isotag->particleType == "photon")
                            listOfPhotonTags.push_back(isotag);
                        else
                            Global::abort(name,
                                          "Something went wrong while assigning"
                                          " the isolations!");
                        isotag = NULL;
                    }
                    break;
                }
                else if (line.find("source = ") == 0) {
                    line.erase(line.begin(), line.begin()+line.find(" = ")+3);
                    isotag->source = line;
                    continue;
                }
                else if (line.find("dr = ") == 0) {
                    line.erase(line.begin(), line.begin()+line.find(" = ")+3);
		    std::istringstream buffer(line);
                    buffer >> isotag->dR;
                    continue;
                }
                else if (line.find("ptmin = ") == 0) {
                    line.erase(line.begin(), line.begin()+line.find(" = ")+3);
                    std::istringstream buffer(line);
                    buffer >> isotag->pTmin;
                    continue;
                }
                else if (line.find("maxval = ") == 0) {
                    line.erase(line.begin(), line.begin()+line.find(" = ")+3);
                    std::istringstream buffer(line);
                    buffer >> isotag->maxVal;
                    continue;
                }
                else if (line.find("absorrel = ") == 0) {
                    line.erase(line.begin(), line.begin()+line.find(" = ")+3);
                    if(line == "a")
                        isotag->divideByPTCand = false;
                    else
                        isotag->divideByPTCand = true;
                    continue;
                }
            }
        }
        else if (line.find("[TAUTAG:") == 0){
            doJetTauTags = true;
        }
        else if (line.find("[BTAG:") == 0){
            jettag = new jet_tag_definition;

            int counter = 0;
            line.erase(line.begin(), line.begin()+line.find(" ")+1);
            line.erase(line.begin()+line.find("]"), line.end());
            std::istringstream buffer(line);
            buffer >> counter;

            jettag->tag_name = "btag";
            jettag->counter = counter;
            while(getline(readFile,line)) {
                if (line == "") {
                    if (jettag) {
                        listOfJetBTags.push_back(jettag);
                        jettag = NULL;
                    }
                    break;
                }
                else if (line.find("eff = ") == 0) {
                    line.erase(line.begin(), line.begin()+line.find(" = ")+3);
                    std::istringstream buffer(line);
                    buffer >> jettag->eff;
                    // AnalysisManager stores percentages, but we need fractions
                    jettag->eff = jettag->eff/100.;
                    continue;
                }
            }
        }
        else if (line.find("[ANALYSIS:") == 0){
            // Read in a new analysis
            // First find the name from the block header
            std::string analysisName = line;
            analysisName.erase(analysisName.begin(),
                               analysisName.begin()+analysisName.find(" ")+1);
            analysisName.erase(analysisName.begin()+analysisName.find("]"),
                               analysisName.end());
            std::map<std::string, std::string> branches;

            std::map<std::string, std::vector<int> > whichTags;

            while(getline(readFile,line)) {
                if (line.find("electron_isolation = ") == 0) {
                    int x;
                    line.erase(line.begin(), line.begin()+line.find("= ")+2);
                    std::stringstream buffer(line);
                    whichTags["ElectronIsolation"] = std::vector<int>();
                    while(buffer >> x)
                        whichTags["ElectronIsolation"].push_back(x);
                }
                else if (line.find("muon_isolation = ") == 0) {
                    int x;
                    line.erase(line.begin(), line.begin()+line.find("= ")+2);
                    std::stringstream buffer(line);
                    whichTags["MuonIsolation"] = std::vector<int>();
                    while(buffer >> x)
                        whichTags["MuonIsolation"].push_back(x);
                }
                else if (line.find("photon_isolation = ") == 0) {
                    int x;
                    line.erase(line.begin(), line.begin()+line.find("= ")+2);
                    std::stringstream buffer(line);
                    whichTags["PhotonIsolation"] = std::vector<int>();
                    while(buffer >> x)
                        whichTags["PhotonIsolation"].push_back(x);
                }
                else if (line.find("jet_btags = ") == 0) {
                    int x;
                    line.erase(line.begin(), line.begin()+line.find("= ")+2);
                    std::stringstream buffer(line);
                    whichTags["BJetTagging"] = std::vector<int>();
                     // Note that the AnalysisManager asks for percent but our
                    //   formulae use the fraction
                    while(buffer >> x)
                        whichTags["BJetTagging"].push_back(x);
                }
                else if (line == "") {
                    bookAnalysis(analysisName, whichTags, eventParameters);
                    break;
                }
            }
        }
    }
    if(listOfAnalyses.empty())
        Global::abort(name, "No analyses could be loaded!");
}



bool AnalysisHandler::readParticles(int iEvent) {
    // in ROOT file mode, we have to let the treeReader read the branches
    if(treeReader) {
        if(iEvent >= treeReader->GetEntries()) {
            hasEvents = false;
            return false; // abort the Fritz event loop
	}
        treeReader->ReadEntry(iEvent);
    } else {
        if (!dHandler->hasNextEvent()) {
            hasEvents = false;
            return false;
        }
    }
    // define particle vectors and fill them with content of the right branch
    true_b.clear();
    true_c.clear();
    true_tau.clear();
    if (!branchGenParticle) {
        Global::abort(name,
                      "GenParticleBranch not properly assigned!");
    }

    for(int i = 0; i < branchGenParticle->GetEntries(); i++) {
        if (abs( ((GenParticle*)branchGenParticle->At(i))->PID)  == 5)
            true_b.push_back((GenParticle*)branchGenParticle->At(i));
        else if (abs( ((GenParticle*)branchGenParticle->At(i))->PID)  == 4)
            true_c.push_back((GenParticle*)branchGenParticle->At(i));
        else if (abs( ((GenParticle*)branchGenParticle->At(i))->PID)  == 15) {
            true_tau.push_back((GenParticle*)branchGenParticle->At(i));
        }
    }
    branchGenParticle->Clear();

    tracks.clear();
    if (!branchTrack)
        Global::abort(name, "branchTrack not properly assigned!");
    for(int i = 0; i < branchTrack->GetEntries(); i++)
        tracks.push_back((Track*)branchTrack->At(i));
    branchTrack->Clear();

    towers.clear();
    if (!branchTower)
        Global::abort(name, "branchTower not properly assigned!");
    for(int i = 0; i < branchTower->GetEntries(); i++)
        towers.push_back((Tower*)branchTower->At(i));
    branchTower->Clear();

    jets.clear();
    if (!branchJet)
        Global::abort(name, "branchJet not properly assigned!");
    for(int i = 0; i < branchJet->GetEntries(); i++)
        jets.push_back((Jet*)branchJet->At(i));
    branchJet->Clear();

    electrons.clear();
    if (!branchElectron) {
        Global::abort(name,
                      "branchElectron not properly assigned!");
    }
    for(int i = 0; i < branchElectron->GetEntries(); i++)
        electrons.push_back((Electron*)branchElectron->At(i));
    branchElectron->Clear();

    muons.clear();
    if (!branchMuon)
        Global::abort(name, "branchMuon not properly assigned!");
    for(int i = 0; i < branchMuon->GetEntries(); i++)
        muons.push_back((Muon*)branchMuon->At(i));
    branchMuon->Clear();

    photons.clear();
    if (!branchPhoton)
        Global::abort(name, "branchPhoton not properly assigned!");
    for(int i = 0; i < branchPhoton->GetEntries(); i++)
        photons.push_back((Photon*)branchPhoton->At(i));
    branchPhoton->Clear();

    if (!branchMissingET || branchMissingET->GetEntries() == 0) {
        Global::abort(name,
                      "branchMissingET not properly assigned or empty!");
    }
    missingET = new ETMiss((MissingET*)branchMissingET->At(0));
    branchMissingET->Clear();

    if (!branchEvent || branchEvent->GetEntries() == 0) {
        Global::abort(name,
                      "branchEvent not properly assigned or empty!");
    }

    const char* eventClassName = branchEvent->At(0)->ClassName();
    if (!strcmp(eventClassName,"LHEFEvent")) {
        eventWeight = ((LHEFEvent*)branchEvent->At(0))->Weight;
    } else if (!strcmp(eventClassName,"HepMCEvent")) {
        eventWeight = ((HepMCEvent*)branchEvent->At(0))->Weight;
    } else {
        std::stringstream msg;
        msg << "Unsupported EventClass '" << eventClassName << "'" << std::endl;
        Global::abort(name, msg.str());
    }
    if (eventWeight != eventWeight) {// if eventWeight == NAN
        eventWeight= 1.;
    }

    branchEvent->Clear();
    return true;
}


void AnalysisHandler::postProcessParticles() {
    // The general AnalysisHandler only isolates;
    //  efficiency cuts are to be done by the daughter classes
    isolateElectrons();
    isolateMuons();
    isolatePhotons();
    // loop over electrons
}

void AnalysisHandler::isolateElectrons() {
    electronIsolationTags.clear();
    for (int e = 0; e < electrons.size(); e++) {
        Electron* cand = electrons[e];
        std::vector<bool> flags;

        // Check all isolation conditions
        for (int i = 0; i < listOfElectronTags.size(); i++) {
            isolation_tag_definition* iso = listOfElectronTags[i];
            double maxDR = iso->dR;
            double pTmin = iso->pTmin;

            double sumPT = 0;
            // loop over either the calos or the tracks
            if (iso->source == "t") {
                for (int t = 0; t < tracks.size(); t++) {
                    Track* neighbour = tracks[t];
                    // respect ptmin
                    if (neighbour->PT < pTmin)
                        continue;
                    // check dR
                    if (neighbour->P4().DeltaR(cand->P4()) > maxDR)
                        continue;
                    // Ignore the electron's track itself
                     if(neighbour->Particle == cand->Particle)
                         continue;
                    sumPT += neighbour->PT;
                }
            }
            else if (iso->source == "c") {
                for (int t = 0; t < towers.size(); t++) {
                    Tower* neighbour = towers[t];
                    // respect ptmin
                    if (neighbour->ET < pTmin)
                        continue;
                    // check dR
                    if (neighbour->P4().DeltaR(cand->P4()) > maxDR)
                        continue;
                    // Ignore the electron's tower
                    bool candidatesTower = false;
                    for(int p = 0; p < neighbour->Particles.GetEntries(); p++){
                        if (neighbour->Particles.At(p) == cand->Particle) {
                            // break the loop and ignore the tower
                            candidatesTower = true;
                            break;
                        }
                    }
                    if (candidatesTower)
                        continue;
                    sumPT += neighbour->ET;
                }
            }
            else
                Global::abort(name,
                              "Unknown isolation source "
                              +iso->source);

            // process sumPT depending on if we have an absolute or a
            //  relative limit
            if (iso->divideByPTCand)
                sumPT /= cand->PT;
            // now check against limit and if so, add the correct tag value
            if (sumPT < iso->maxVal)
                flags.push_back(true);
            else
                flags.push_back(false);
        }
        electronIsolationTags[cand] = flags;
    }
}

void AnalysisHandler::isolateMuons() {
    // Do the same as isolateElectrons()
    muonIsolationTags.clear();
    for (int m = 0; m < muons.size(); m++) {
        Muon* cand = muons[m];
        std::vector<bool> flags;
        // loop over isolation conditions
        for (int i = 0; i < listOfMuonTags.size(); i++) {
            isolation_tag_definition* iso = listOfMuonTags[i];
            double maxDR = iso->dR;
            double pTmin = iso->pTmin;

            double sumPT = 0;
            // loop over calos or tracks
            if (iso->source == "t") {
                for (int t = 0; t < tracks.size(); t++) {
                    Track* neighbour = tracks[t];
                    // respect ptmin
                    if (neighbour->PT < pTmin)
                        continue;
                    // check dR
                    if (neighbour->P4().DeltaR(cand->P4()) > maxDR)
                        continue;
                    // FIXME To be compatible with CheckMATE 1, muons do not
                    // appear in tracks, therefore no track=?=muon check needed
                    sumPT += neighbour->PT;
                }
            }
            else if (iso->source == "c") {
                for (int t = 0; t < towers.size(); t++) {
                    Tower* neighbour = towers[t];
                    // respect ptmin
                    if (neighbour->ET < pTmin)
                        continue;
                    // check dR
                    if (neighbour->P4().DeltaR(cand->P4()) > maxDR)
                        continue;
                    // Muons do not deposit into towers, so no tower=?=muon
                    sumPT += neighbour->ET;
                }
            }
            else
                Global::abort(name,
                              "Unknown isolation source "
                              +iso->source);
            // process sumPT depending on if we have an absolute or a
            //  relative limit
            if (iso->divideByPTCand)
                sumPT /= cand->PT;
            // now check against limit and if so, add the correct tag value
            if (sumPT < iso->maxVal)
                flags.push_back(true);
            else
                flags.push_back(false);
        }
        muonIsolationTags[cand] = flags;
    }
}

void AnalysisHandler::isolatePhotons() {
    photonIsolationTags.clear();
    for (int p = 0; p < photons.size(); p++) {
        Photon* cand = photons[p];
        std::vector<bool> flags;

        // loop over isolation conditions
        for (int i = 0; i < listOfPhotonTags.size(); i++) {
            // FIXME to be compatible with CM1 and to improve speed, avoid
            // test of any photon with PT < 10 GeV
            if(photons[p]->PT < 10.) {
                flags.push_back(false);
                continue;
            }

            isolation_tag_definition* iso = listOfPhotonTags[i];
            double maxDR = iso->dR;
            double pTmin = iso->pTmin;

            double sumPT = 0;
             // loop over calos or tracks
            if (iso->source == "t") {
                for (int t = 0; t < tracks.size(); t++) {
                    Track* neighbour = tracks[t];
                    // respect ptmin
                    if (neighbour->PT < pTmin)
                        continue;
                    // check dR
                    if (neighbour->P4().DeltaR(cand->P4()) > maxDR)
                        continue;
                    // photons do not appear in tracks
                    // TODO Check whether this is true in Delphes too
                    // as their overlap check could test
                    // mother(photon) =?= track
                    sumPT += neighbour->PT;
                }
            }
            else if (iso->source == "c") {
                for (int t = 0; t < towers.size(); t++) {
                    Tower* neighbour = towers[t];
                    // respect ptmin
                    if (neighbour->ET < pTmin)
                        continue;
                    // check dR
                    if (neighbour->P4().DeltaR(cand->P4()) > maxDR)
                        continue;
                    // Check for tower =?= photon
                    bool candidatesTower = false;
                    TRefArray nParticles = neighbour->Particles;
                    TRefArray cParticles = cand->Particles;
                    for(int np = 0; np < nParticles.GetEntries(); np++) {
                       for (int cp = 0; cp < cParticles.GetEntries(); cp++) {
                           if (nParticles.At(np) == cParticles.At(cp)) {
                                 candidatesTower = true;
                                 break;
                           }
                       }
                    }
                    if (candidatesTower)
                        continue;
                    sumPT += neighbour->ET;
                }
            }
            else
                Global::abort(name,
                              "Unknown isolation source "
                              +iso->source);
            // process sumPT depending on if we have an absolute or a
            //  relative limit
            if (iso->divideByPTCand)
                sumPT /= cand->PT;
            // now check against limit and if so, add the correct tag value
            if (sumPT < iso->maxVal)
                flags.push_back(true);
            else
                flags.push_back(false);
        }
        photonIsolationTags[photons[p]] = flags;
    }
}

void AnalysisHandler::linkObjects() {
    for(int a = 0; a < listOfAnalyses.size(); a++) {
        // important: as many analyses cut on the containers,
        //  every analysis must use its own container
        std::vector<Track*> tempTracks = tracks;
        listOfAnalyses[a]->tracks = tempTracks;
        std::vector<Tower*> tempTowers = towers;
        listOfAnalyses[a]->towers = tempTowers;
        std::vector<Jet*> tempJets = jets;
        listOfAnalyses[a]->jets = tempJets;
        std::vector<Electron*> tempElectrons = electrons;
        listOfAnalyses[a]->electrons = tempElectrons;
        std::vector<Muon*> tempMuons = muons;
        listOfAnalyses[a]->muons = tempMuons;
        std::vector<Photon*> tempPhotons = photons;
        listOfAnalyses[a]->photons = tempPhotons;
        ETMiss* tempMissingET =  new ETMiss(missingET);
        listOfAnalyses[a]->missingET = tempMissingET;
        listOfAnalyses[a]->weight = eventWeight;

        listOfAnalyses[a]->electronIsolationTags = electronIsolationTags;
        listOfAnalyses[a]->muonIsolationTags = muonIsolationTags;
        listOfAnalyses[a]->photonIsolationTags = photonIsolationTags;
    }
}
