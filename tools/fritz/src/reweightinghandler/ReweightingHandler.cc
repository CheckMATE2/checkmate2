#include "ReweightingHandler.h"



ReweightingHandler::ReweightingHandler() {
	reweightor = NULL;
#ifdef HAVE_PYTHIA
	pHandler = NULL;
#endif
	lhefReader = NULL,
    pythiaToHepMC = NULL;
    hepmcInput = NULL;
    evt = NULL;
    reweightingLogFile = "reweighting.log";
    hasEvents = true;
    name = "ReweightingHandler";
}

ReweightingHandler::~ReweightingHandler() {
	delete reweightor;
    delete pythiaToHepMC;
    delete hepmcInput;
    delete evt;
    delete lhefReader;
}


// List of keys that are understood by reweighting handlers
static const std::string keyName = "name";
static const std::string keySettings = "settings";
static const std::string keyPythiaHandler = "pythiahandler";
static const std::string keyLogFile = "logfile";
static const std::string keyOutputDirectory = "outputdirectory";
static const std::string keyNTargets = "ntargets";
static const std::string keyEventFile = "eventfile";

// Warns the user if there are keys that are not supported in reweighting handler sections
static void unknownKeys(Properties props) {
    std::vector<std::string> knownKeys;
    knownKeys.push_back(keyName);
    knownKeys.push_back(keySettings);
    knownKeys.push_back(keyPythiaHandler);
    knownKeys.push_back(keyLogFile);
    knownKeys.push_back(keyOutputDirectory);
    knownKeys.push_back(keyNTargets);
    knownKeys.push_back(keyEventFile);
    warnUnknownKeys(props, knownKeys, props["name"], "Unknown key for PythiaHandler section");
}



#ifdef HAVE_PYTHIA
void ReweightingHandler::setup(
	Properties props,
	std::map<std::string,EventFile> eventFiles,
	std::map<std::string,PythiaHandler*> pythiaHandler
) {

	std::string settings = lookupRequired(
		props, 
		keySettings, 
		name, 
		"Path to reweighting config file is required."
	);

	int nTargets = lookupRequiredInt(
		props,
		keyNTargets,
		name,
		"Number of target parameter points is required"
	);
    nBranches = nTargets + 1;

	reweightingLogFile = lookupOrDefault(props, keyLogFile, "reweighting.log");
	std::string outputDirectory = lookupOrDefault(props, keyOutputDirectory, "");

	std::pair<bool,std::string> pair;
	// Check if Reweighting section in .ini file has a property with key keyEventFile
    // maybeLookup returns (bool existent, string name)
	pair = maybeLookup(props, keyEventFile);
	bool haveEvents = pair.first;
	std::string eventsLabel = pair.second;

	pair = maybeLookup(props, keyPythiaHandler);
	bool havePythia = pair.first;
	std::string pythiaLabel = pair.second;

	if (havePythia && haveEvents) {
        Global::abort(
                name,
                "PythiaHandler and EventFile can not be combined."
                );
    }
   
    if (!havePythia && !haveEvents) {
        Global::abort(
                name,
                "One of PythiaHandler or EventFile is required."
                );
    }

    if(havePythia){
    	pHandler = lookupRequired(
    		pythiaHandler,
    		pythiaLabel,
    		name,
    		"Can not find PythiaHandler with label "+pythiaLabel
    	);
    	 Global::print(
                name,
                "Initialising Reweighting Module via linking to "+pHandler->name
                );
        setup(props,pHandler);
    }else{
    	EventFile file = lookupRequired(
                eventFiles,
                eventsLabel,
                name,
                "Can not find EventFile with label "+eventsLabel
                );
        Global::print(
                name,
                "Initialising Reweighting Module via input event "+file.filepath
                );
        setup(props,file);
    }

}
#endif


void ReweightingHandler::setup(
	Properties props,
	std::map<std::string,EventFile> eventFiles
) {

	std::string settings = lookupRequired(
		props, 
		keySettings, 
		name, 
		"Path to reweighting config file is required."
	);

	int nTargetsStr = lookupRequiredInt(
		props,
		keyNTargets,
		name,
		"Number of target parameter points is required"
	);

	reweightingLogFile = lookupOrDefault(props, keyLogFile, "reweighting.log");
	std::string outputDirectory = lookupOrDefault(props, keyOutputDirectory, "");

	std::string eventsLabel = lookupRequired(
			props,
			keyEventFile,
			name,
			"Can not find EventFile with label "+eventsLabel
			);		

	EventFile file = lookupRequired(
            eventFiles,
            eventsLabel,
            name,
            "Can not find EventFile with label "+eventsLabel
            );
    Global::print(
            name,
            "Initialising Reweighting Module via input event "+file.filepath
            );
    setup(props,file);

}



void ReweightingHandler::setup(
	Properties props,
	EventFile eventFile
){

	this->eventFile = eventFile;
    std::string inputEventFileName = eventFile.filepath;

    // Figure out if the file is .lhe or .hepmc by looking at the file
    std::ifstream x(inputEventFileName.c_str());
    std::string firstLine;
    x >> firstLine;
    if(firstLine == "HepMC::Version") {
        Global::print(name, "Input File determined to be HepMC.");
        mode = HepMCMode;
        hepmcInput = new HepMC::IO_GenEvent(inputEventFileName.c_str(),std::ios::in);
    }
    else if(firstLine.find("LesHouchesEvents version") != std::string::npos){
        Global::print(name, "Input File determined to be LHEF.");
        mode = LHEFMode;
        lhefReader = new LHEF::Reader(inputEventFileName.c_str());
    }
    else {
        Global::abort(
        	name,
        	"Input File for reweighting must be either hepmc or lhe."
        	);        
    }

    std::string reweightingConfigPath = lookupRequired(
    	props, 
    	keySettings,
    	name,
    	"Need settings entry with path to config file"
    	);
    
    reweightor = new Reweightor(reweightingConfigPath);

}


#ifdef HAVE_PYTHIA
void ReweightingHandler::setup(
        Properties props,
        PythiaHandler *pHandler
    ) {
    mode = PythiaMode;
    this->pHandler = pHandler;
    mainPythia = pHandler->mainPythia;
    pythiaToHepMC = new HepMC::Pythia8ToHepMC();

}
#endif



bool ReweightingHandler::hasNextEvent(){
	return hasEvents;	
}


bool ReweightingHandler::processEvent(int iEvent){

	if (!hasEvents) {
        return false;
    }

    Global::redirect_cout(reweightingLogFile);

    // don't need to extra delete evt since it's the first entry of reweightedEvents. Just delete every GenEvent in reweightedEvents.
    std::vector<std::pair<HepMC::GenEvent*,ReweightingProcInfo> >::iterator reweightedEventsIterator;
    for(reweightedEventsIterator=reweightedEvents.begin(); reweightedEventsIterator!=reweightedEvents.end(); reweightedEventsIterator++){
        if(reweightedEventsIterator->first!=0){
            delete reweightedEventsIterator->first;        
            reweightedEventsIterator->first = NULL;
        }
    }
    reweightedEvents.clear();

 	// read event from the correct source
 	if (mode == PythiaMode){

#ifdef HAVE_PYTHIA
        if (!pHandler->hasNextEvent()) {
            hasEvents = false;
            return false;
        }
		pythiaToHepMC->fill_next_event(*mainPythia, evt);
        
#else
        Global::abort(
        	name,
        	"For reweighting to work with Pythia8, CheckMATE must be compiled with HepMC."
        	);
#endif

    }else if(mode == HepMCMode){

    	*hepmcInput >> evt;
    	if(!evt){
    		hasEvents = false;
    		return false;
    	}

    }else if(mode == LHEFMode){
    	
    	if(!readLHEFEvent(iEvent)){
    		hasEvents=false;
    		return false;
    	}

    }else{
    	Global::abort(
        	name,
        	"Could not recognize reweighting mode "+mode
        	);
    }
    
    std::cout << evt << std::endl;

    ReweightingProcInfo oldProcInfo = reweightor->oldProcInfo;
    std::pair<HepMC::GenEvent*,ReweightingProcInfo> eventPair = std::make_pair(evt,oldProcInfo);
    reweightedEvents.push_back(eventPair);

    std::vector<std::pair<bool, std::pair<HepMC::GenEvent*,ReweightingProcInfo>>> reweightingResult;
    reweightingResult = reweightor->reweight(evt);  

    bool success = true;
    std::vector<std::pair<bool, std::pair<HepMC::GenEvent*,ReweightingProcInfo>>>::iterator it;
    for(it = reweightingResult.begin(); it!=reweightingResult.end(); it++){
        std::cout << it->second.first << std::endl;
    	reweightedEvents.push_back(it->second);
    	success = success && it->first;
    }

    if(success){
    	fill_info();
    }else{
        Global::abort(
            name,
            "Could not successfully reweight the events."
        );
    }

    return true;

}


void ReweightingHandler::fill_info(){
	
	// reweightedEventWeights
	double totalWeight;

	std::vector<std::pair<HepMC::GenEvent*,ReweightingProcInfo>>::iterator it;
	for(it=reweightedEvents.begin(); it!=reweightedEvents.end(); it++){
        totalWeight = 1.;
		HepMC::WeightContainer eventWeights = it->first->weights();
		HepMC::WeightContainer::iterator it2;
		for(it2=eventWeights.begin(); it2!=eventWeights.end(); it2++){
			totalWeight *= *it2;
		}
		reweightedEventWeights.push_back(totalWeight);
	}


}



bool ReweightingHandler::readLHEFEvent(int iEvent){
	
	if(!lhefReader->readEvent()){
		return false;
	}


    evt->use_units(HepMC::Units::GEV, HepMC::Units::MM);
    const double weight = lhefReader->hepeup.weight();
    evt->weights().push_back(weight);


	// double accumulated_weight = 0.0;
	// double accumulated_weight_squared = 0.0;
	// int event_number = 0;

	// // Cross-section
	// HepMC::GenCrossSection xsec;
	// const int idwtup = lhefReader->heprup.IDWTUP;
	
	// double xsecval = -1.0;
	// double xsecerr = -1.0;
	// if (abs(idwtup) == 3) {
	//   xsecval = lhefReader->heprup.XSECUP[0];
	//   xsecerr = lhefReader->heprup.XSECUP[1];
	// } else if (abs(idwtup) == 4) {
	//   accumulated_weight += weight;
	//   accumulated_weight_squared += weight*weight;
	//   event_number += 1;
	//   xsecval = accumulated_weight/event_number;
	//   double xsecerr2 = (accumulated_weight_squared/event_number - xsecval*xsecval)/event_number;
	//   if (xsecerr2 < 0) {
	//     xsecerr2 = 0.0;
	//   }
	//   xsecerr = sqrt(xsecerr2);
	// } else  {
	//   Global::abort(
	//   	name,
	//   	"Could not convert lhef event to hepmc event.\n IDWTUP = " + idwtup + "value not handled yet."
	//   	);
	// }
	// xsec.set_cross_section(xsecval, xsecerr);
	// evt->set_cross_section(xsec);



 
    std::map<int,int> mother2endvertex;
    std::vector<int> added;
    HepMC::GenVertex *production_vertex,*end_vertex;
  
   	HepMC::GenVertex * hard_vertex = new HepMC::GenVertex();
    evt->add_vertex(hard_vertex);
    hard_vertex->suggest_barcode(-3);

    for(int i=0; i<lhefReader->hepeup.NUP; ++i){
      int mother1 = (lhefReader->hepeup.MOTHUP[i]).first;
      int mother2 = (lhefReader->hepeup.MOTHUP[i]).second;
      int pdg_id = lhefReader->hepeup.IDUP[i];
      int status = lhefReader->hepeup.ISTUP[i];
      double mass = lhefReader->hepeup.PUP[i][4];
      HepMC::FourVector v(lhefReader->hepeup.PUP[i][0], 
             lhefReader->hepeup.PUP[i][1],
             lhefReader->hepeup.PUP[i][2], 
             lhefReader->hepeup.PUP[i][3]);
      
      HepMC::GenParticle* gp = new HepMC::GenParticle(v, pdg_id, status);
      gp->set_generated_mass(mass); 
      gp->suggest_barcode(i+1);

      added.push_back(i+1);
     
      if(mother1==0 && mother2==0){
        production_vertex = new HepMC::GenVertex();
        evt->add_vertex(production_vertex);
        production_vertex->suggest_barcode(-(i+1));
        production_vertex->add_particle_out(gp);

        hard_vertex->add_particle_in(gp);

        mother2endvertex[i+1] = hard_vertex->barcode();
        continue;
      }

      production_vertex = evt->barcode_to_vertex(mother2endvertex[mother1]);
      production_vertex->add_particle_out(gp);

      if(status==1) continue;

      end_vertex = new HepMC::GenVertex();
      evt->add_vertex(end_vertex);
      end_vertex->add_particle_in(gp);
      
      mother2endvertex[i+1] = end_vertex->barcode();
    }

    return true;
}


ExRootTreeReader* ReweightingHandler::initTreeReader(){
    TChain* rootFileChain = new TChain("Delphes");
    rootFileChain->Add("Event");
    rootFileChain->Add("Particle");
    rootFileChain->Add("Jet");
    rootFileChain->Add("Track");
    rootFileChain->Add("Tower");
    rootFileChain->Add("Electron");
    rootFileChain->Add("Muon");
    rootFileChain->Add("Photon");
    rootFileChain->Add("MissingET");
    treeReader = new ExRootTreeReader(rootFileChain);

    return treeReader;
}

void ReweightingHandler::fillTreeReader(int iBranch){
    // TODO
}



void ReweightingHandler::finish(){
    Global::unredirect_cout();
    Global::print(name, "Reweighting successfully finished!");
}