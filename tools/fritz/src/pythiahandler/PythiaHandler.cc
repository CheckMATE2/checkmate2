#include "PythiaHandler.h"
#include "MG5toPy8.h"

#include <iostream>
#include <fstream>
#include <string>

// List of keys that are understood by pythia handlers
static const std::string keyName = "name";
static const std::string keySettings = "settings";
static const std::string keyPythiaPath = "pythiapath";
static const std::string keyLogFile = "logfile";
static const std::string keyOutputFile = "outputfile";
static const std::string keyKFactor = "kfactor";
static const std::string keyXSect = "xsect";
static const std::string keyXSectErr = "xsecterr";
static const std::string keyXSectErrFactor = "xsecterrfactor";
static const std::string keyXSectThresh = "xsectthresh";
static const std::string keyRndmIn = "rndmin";
static const std::string keyUseMG5 = "usemg5";
static const std::string keyMGproc = "mgproccard";
static const std::string keyMGparam = "mgparamcard";
static const std::string keyMGrun = "mgruncard";
static const std::string keyMGconfig = "mgconfigcard";
static const std::string keyMGrunpath = "mgrunpath";
static const std::string keyMGsourcepath = "mgsourcepath";

PythiaHandler::PythiaHandler() {
    mainPythia = NULL;
#ifdef HAVE_HEPMC
    pythiaToHepMC = NULL;
    hepMCOutput = NULL;
#endif
    pythiaLogFile = "pythia.log";
    hasEvents = true;
    iSubRun = 0;
    nSubRuns = 0;
    iAbort = 0;
    nAborts = 0;
    name = "pythiahandler";
    madgraph = NULL;
}

PythiaHandler::~PythiaHandler() {
    delete mainPythia;
#ifdef HAVE_HEPMC
    delete pythiaToHepMC;
    delete hepMCOutput;
#endif
}

// Warns the user if there are keys that are not supported in pythia handler sections
static void unknownKeys(Properties props) {
    std::vector<std::string> knownKeys;
    knownKeys.push_back(keyName);
    knownKeys.push_back(keySettings);
    knownKeys.push_back(keyPythiaPath);
    knownKeys.push_back(keyLogFile);
    knownKeys.push_back(keyOutputFile);
    knownKeys.push_back(keyKFactor);
    knownKeys.push_back(keyXSect);
    knownKeys.push_back(keyXSectErr);
    knownKeys.push_back(keyXSectThresh);
    knownKeys.push_back(keyXSectErrFactor);
    knownKeys.push_back(keyRndmIn);
    knownKeys.push_back(keyUseMG5);
    knownKeys.push_back(keyMGproc);
    knownKeys.push_back(keyMGparam);
    knownKeys.push_back(keyMGrun);
    knownKeys.push_back(keyMGconfig);
    knownKeys.push_back(keyMGrunpath);
    knownKeys.push_back(keyMGsourcepath);
    warnUnknownKeys(props, knownKeys, props["name"], "Unknown key for PythiaHandler section");
}

void PythiaHandler::setupXSect(Properties props) {
    std::pair<bool,double> pair;
    // k factor
    pair = maybeLookupDouble(props, keyKFactor, 1.0);
    bool haveKFactor = pair.first;
    kFactor = pair.second;
    // xsect
    pair = maybeLookupDouble(props, keyXSect);
    haveXSect = pair.first;
    xsect = pair.second;
    if (haveXSect && haveKFactor) {
        Global::abort(name, keyKFactor+" and "+keyXSect+" can not be combined.");    
    }
    // xsectErr
    pair = maybeLookupDouble(props, keyXSectErr);
    haveXSectErr = pair.first;
    xsectErr = pair.second;
    // xsectErrFactor
    pair = maybeLookupDouble(props, keyXSectErrFactor);
    haveXSectErrFactor = pair.first;
    xsectErrFactor = pair.second;
    // xsectThresh
    pair = maybeLookupDouble(props, keyXSectThresh);
    haveXSectThresh = pair.first;
    xsectThresh = pair.second; // CheckMATE will provide this cross section in pb!
    
    if (haveXSect && !haveXSectErr && !haveXSectErrFactor) {
        Global::abort(name, "If an xsect is given either xsectErr or xsectErrFactor is required.");    
    }
    if (haveXSectErr && haveXSectErrFactor) {
        Global::abort(name, "xsectErr and xsectErrFactor can not be combined.");
    }
    if (haveXSectThresh)
	Global::print(name, "Set cross section threshold to "+Global::doubleToStr(xsectThresh)+" pb");

}

void PythiaHandler::setup(Properties props) {
    name = props["name"];
    unknownKeys(props);

    pythiaPath = lookupRequired(props,
				keyPythiaPath,
				name,
				keyPythiaPath+" is required in pythia handler sections."
				);

    
    pythiaLogFile = lookupOrDefault(props, keyLogFile, "pythia.log");
    std::string outputFile = lookupOrDefault(props, keyOutputFile, "");
#ifndef HAVE_HEPMC
    if (outputFile != "") {
        Global::warn(
            name,
            "Writing pythia events in an output file is only supported if Fritz was compiled with HepMC."
        );
    }
#endif

 useMG5 = (lookupRequired(
			  props,
			  keyUseMG5,
			  name,
			  keyUseMG5+" is required in pythia handler sections."
			  ) == "true");
  if(!useMG5){
    pythiaConfigFile = lookupRequired(
				      props,
				      keySettings,
				      name,
				      keySettings+" is required in pythia handler sections."
				      );

  }
  else {
    pythiaConfigFile = lookupOrDefault(
				  props,
				  keySettings,
				  ""
				  );
  }
  setupXSect(props);

  pythiaRndmIn = lookupOrDefault(
				 props,
				 keyRndmIn,
				 ""
				 );
    
  if (useMG5) {
    mgRunPath = lookupOrDefault(
				  props,
				  keyMGrunpath,
				  "madgraphrun"
				  );

    mgSourcePath = lookupRequired(
				props,
				keyMGsourcepath,
				name,
				keyMGsourcepath+" is required to use MadGraph sections."
				);


    mgProcCard = lookupRequired(
				props,
				keyMGproc,
				name,
				keyMGproc+" is required in pythia handler sections."
				);

    mgParamCard = lookupOrDefault(
				  props,
				  keyMGparam,
				  ""
				  );
    
    mgRunCard =  lookupOrDefault(
				 props,
				 keyMGrun,
				 ""
				 );

    mgConfigCard =  lookupOrDefault(
				 props,
				 keyMGconfig,
				 ""
				 );
	
    Global::print(name, "read mgProcCard ="+ mgProcCard);
    Global::print(name, "read mgParamCard ="+ mgParamCard);
    Global::print(name, "read mgRunCard ="+ mgRunCard);
    Global::print(name, "read mgConfigCard ="+ mgConfigCard);
    
  }  
    
  Global::print(name, "Pythia8 output redirected to " + pythiaLogFile);  
  Global::redirect_cout(pythiaLogFile);  
  mainPythia = new Pythia8::Pythia("", true);     
  if(useMG5) {
    // Alias does not work for Jamie so testing with explicit path
    madgraph = new Pythia8::LHAupMadgraph(mainPythia, true, mgRunPath, mgSourcePath+"/bin/mg5_aMC");
    
    // Read MG5 command line
    ifstream mg5File;
    mg5File.open(mgProcCard.c_str(),ios::in);
    std::string line;
    while(std::getline(mg5File, line)){	
      Global::unredirect_cout();  
      Global::print(name, "reading MG5_aMC@NLO command line'" + line + "'");
      Global::redirect_cout(pythiaLogFile);  
      madgraph->readString(line);
    }
    Global::unredirect_cout();  
    Global::print(name, "setting MG5_aMC@NLO seed to "+Global::intToStr(Global::randomSeed));
    Global::redirect_cout(pythiaLogFile);  
    madgraph->setSeed(Global::randomSeed);
    if(mgParamCard != "") madgraph->addCard(mgParamCard,"param_card.dat");
    if(mgRunCard != "") madgraph->addCard(mgRunCard,"run_card.dat");
    if(mgConfigCard != "") madgraph->addCard(mgConfigCard,"me5_configuration.txt");
    mainPythia->setLHAupPtr(madgraph);
    
    // Set LHE file by hand
    //if(!madgraph->setInit())
    //  Global::abort("PythiaHandler", "could not generate events in MG5.");
    // string LHEfile = mgRunPath + "/Events/run/unweighted_events.lhe";
    // mainPythia->readString("Beams:frameType = 4");
    // mainPythia->readString("Beams:LHEF = "+LHEfile);
  }
  Global::unredirect_cout();

  // Try to initialize Pythia8 object
  mainPythia->readString("Main:numberOfEvents = 100000001");
  if(pythiaConfigFile != "") {
    Global::unredirect_cout();  
    Global::print(name, "Initializing Pythia8 with " + pythiaConfigFile);
    // Initialise Pythia based on subrun info
    Global::redirect_cout(pythiaLogFile);

    if (!mainPythia->readFile(pythiaConfigFile)) {
      Global::unredirect_cout();
      Global::abort(name, "could not read " + pythiaConfigFile);
    }
  }
  Global::redirect_cout(pythiaLogFile);

  // Use last random generator state
  if(pythiaRndmIn !="")
    mainPythia->rndm.readState(pythiaRndmIn);
  
  if (!mainPythia->init()){      
    Global::unredirect_cout();  
    if(useMG5)
      Global::abort(name, "could not initialise Pythia for MG5. Is MadGraph correctly installed at "+mgSourcePath+" ?");
    else
      Global::abort(name, "could not initialise Pythia.");
  }
  else {
    mainPythia->rndm.dumpState(pythiaPath+"/rndm-init.dat");
    Global::unredirect_cout();  
    if(useMG5)
      Global::print(name, "Pythia8 successfully initialized using MG5_aMC@NLO!");
    else
      Global::print(name, "Pythia8 successfully initialized!");
  }

  nAborts = mainPythia->mode("Main:timesAllowErrors");
  iAbort = 0;
  nSubRuns = mainPythia->mode("Main:numberOfSubruns");
  
  if( nSubRuns > 1 ) {
    Global::redirect_cout(pythiaLogFile);  
    // First subrun initialisation
    if(!mainPythia->readFile(pythiaConfigFile, 1) ||
       !mainPythia->init()) {
      Global::unredirect_cout();  
      Global::abort(name, "could not initialise Pythia for subruns");
    }
  }

  Global::unredirect_cout();
  Global::print(name, "Pythia8 successfully initialised!");
  nEvents = mainPythia->mode("Main:numberOfEvents");

  std::stringstream ss;
  ss << nEvents;
  if (nEvents != 100000001)
    Global::print(name, "Pythia8 will generate "+ ss.str() + " events");
  if (nEvents > 99999999 && mainPythia->mode("Beams:frameType") < 4)
    nEvents = 10000;

  // Killer mode: If threshold is set and LHE mode is used, set nEvents to 0 if threshold is not passed
  if (haveXSectThresh && madgraph != NULL && (madgraph->getXS() < xsectThresh)) {
      Global::print(name, "MG5 returned partonic process with cross section "+Global::doubleToStr(madgraph->getXS())+" pb which is smaller than the given threshold.");
      Global::print(name, "Setting nEvents to 0!");
      nEvents = 0;
      mainPythia->readString("Main:numberOfEvents = 0");
  }
      
  
  
#ifdef HAVE_HEPMC
  // If an outputfile is given, load the toHepMCconverter accordingly
  if (outputFile != "") {
    Global::checkIfFileExistsAndRemoveAfterQuery(outputFile);
    pythiaToHepMC = new HepMC::Pythia8ToHepMC();
    hepMCOutput = new HepMC::IO_GenEvent(outputFile, std::ios::out);
    Global::print(name,
		  "Pythia8 will write events to " + outputFile);
  }
#endif
}

bool PythiaHandler::initNextRun() {
  
    if (useMG5) return false;
  
    iSubRun++;
    std::stringstream ss;
    ss << iSubRun;

    // Try to initialise the next subrun, if it exists
    if (iSubRun > nSubRuns)
        return false;
    else {
        if (!mainPythia->readFile(pythiaConfigFile,iSubRun)) {
            Global::abort(name, "could not read " + pythiaConfigFile
                                           + "for subRun " + ss.str());
        }
    }
    if (!mainPythia->init()) {
        Global::abort(name, "could not initialise Pythia8 for subrun"
                                       + ss.str());
    }
    return true;
}


bool PythiaHandler::processEvent(int iEvent) {

    if (!hasEvents) {
        return false;
    }

    // Abort if maximum number of events is reached
    if (iEvent >= nEvents) { // Daniel: This has been " > " for a long time, but I think it is wrong. If I for example set nEvents to 0, I would still get 1 tested event!
      hasEvents = false;
      return false;
    }

    Global::redirect_cout(pythiaLogFile);

    if (!mainPythia)
        Global::abort(name,
                      "PythiaHandler object corrupted!");

    // Note that LHE files cause->next() to fail at the end, but that is fine
    // as long as the atEndOfFile() tells us that we indeed are at the end

    while (!mainPythia->next()) {
      Global::unredirect_cout();

      if(mainPythia->info.atEndOfFile() && !initNextRun()) {
          hasEvents = false;
          return false;
      } else {
        iAbort++;
        if(iAbort >= nAborts && nAborts > 0) {
	  std::stringstream ss;
	  ss << iAbort;
	  Global::print(name, "Aborting after " + ss.str() + "errors");
          Global::print(name,
                "Pythia8 stopped unexpectedly. Generated events so "
                "far are still processed but you should find out "
                "the reason for the abort!");
          hasEvents = false;
	  return false;
        }
      }
    }

    // If hepmc output is required, the event has to be converted and written
#ifdef HAVE_HEPMC
    if (pythiaToHepMC != NULL && hepMCOutput != NULL) {
        HepMC::GenEvent* hepMCEvent = new HepMC::GenEvent();
        pythiaToHepMC->fill_next_event(*mainPythia, hepMCEvent);
        *hepMCOutput << hepMCEvent;
        delete hepMCEvent;
    }
#endif
    Global::unredirect_cout();

    return true;
}

bool PythiaHandler::hasNextEvent() {
    return hasEvents;
}

double PythiaHandler::getCrossSection() {
    if (!mainPythia)
        Global::abort(name, "Pythia8 object not avaliable!");
    if (!haveXSect) {
        xsect = mainPythia->info.sigmaGen() * 1.E12; // We use fb
        Global::print(name, "Pythia8 returned cross section of "
                                       + Global::doubleToStr(xsect)+ " fb");
        xsect*=kFactor;
    }
    return xsect;
}

double PythiaHandler::getCrossSectionErr() {
    if (!mainPythia) {
        Global::abort(name, "Pythia8 object not avaliable!");
    }
    if (haveXSectErr) {
	// Nothing
    } else if (haveXSectErrFactor) {
        if (!haveXSect) {
            xsect = mainPythia->info.sigmaGen() * 1.E12; // We use fb
            xsect *= kFactor;
        }
        return xsectErr = xsect*xsectErrFactor;
    } else {
        //xsectErr = mainPythia->info.sigmaErr() * 1.E12; // We use fb
        //xsectErr*=kFactor;
        xsectErr = 0;
    }
    Global::print(name, "Pythia8 returned cross section error of "
                                       + Global::doubleToStr(xsectErr)+ " fb");
    return xsectErr;
}

void PythiaHandler::finish() {
    
#ifdef HAVE_HEPMC
    delete pythiaToHepMC;
    pythiaToHepMC = NULL;
    delete hepMCOutput;
    hepMCOutput = NULL;
#endif

    Global::print(name, "Pythia8 successfully finished!");
    Global::redirect_cout(pythiaLogFile);
    mainPythia->stat();
    Global::unredirect_cout();

    // Write the last random number generator state
    mainPythia->rndm.dumpState(pythiaPath+"/rndm-end.dat");    
    
    delete mainPythia;
    mainPythia = NULL;   
    
    if (useMG5){
      delete madgraph;
    }


    
}

