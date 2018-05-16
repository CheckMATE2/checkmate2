import os, shutil,  sys
import json, pickle
import argparse
import ConfigParser
from advprint import AdvPrint

class Info(dict):
    analyses = list()
    paths = dict()
    files = dict()
    flags = dict()
    parameters = dict()
    identifiers = dict() # Contains map from all used identifiers to all booked event files. Together with function "bookEvent" makes sure that no identifier is booked twice

    @classmethod
    def init(cls,config_paths):        
        cls.identifiers = dict()
        # Standard flag values
        paths = config_paths
        flags = dict()
        flags['fullcls'] = False
        flags['likelihood'] = False
        flags['likelihoodRootmethod'] = False
        flags['no_mc_stat_err'] = False
        flags['tempmode'] = False
        flags['quietmode'] = False
        flags['skipanalysis'] = False
        flags['skipparamcheck'] = False
        flags['skippythia'] = False
        flags['skipevaluation'] = False
        flags['controlregions'] = False
        flags['run_atlas_analyses'] = False
        flags['run_cms_analyses'] = False
        flags['write_delphes_events'] = False 
        flags['write_pythia_events'] = False 
        flags['eff_tab'] = False
        flags['zsig'] = False
        flags['mg5'] = False
        parameters = dict()
        parameters['outputexists'] = "ask"
        parameters['bestcls'] = 0
        parameters['randomseed'] = 0
        parameters['invisiblePIDs'] = []        
        parameters['EventResultFileColumns'] = ['analysis', 'sr', 'signal_normevents', 'signal_err_tot']
        parameters['ProcessResultFileColumns'] = ['analysis', 'sr', 'signal_normevents', 'signal_err_tot']
        parameters['TotalEvaluationFileColumns'] = ['analysis', 'sr', 'o', 'b', 'db', 's', 'ds', 's95obs', 's95exp', 'robscons', 'rexpcons']
        parameters['BestPerAnalysisEvaluationFileColumns'] = ['analysis', 'sr', 'o', 'b', 'db', 's', 'ds', 's95obs', 's95exp', 'robscons', 'rexpcons']
        
        cls.analysis_groups = {
                "ATLAS_7TeV",
                "ATLAS_8TeV",
                "ATLAS_13TeV",
                "ATLAS_14TeV_HighLumi",
                "CMS_7TeV",
                "CMS_8TeV",
                "CMS_13TeV",
                "CMS_14TeV_HighLumi"           
        }
        cls.experiments = {
                "atlas",
                "atlas7tev",
                "atlas8tev",
                "atlas13tev",
                "atlas14tev_projected",
                "atlas14tev_hl_flatbtagger",
                "cms",
                "cms7tev",
                "cms8tev",
                "cms13tev",
                "cms14tev_projected"
                }
        cls.detector_setups = {
                "ATLAS": {
                    7.0: {"default": ("atlas7tev", "ATLAS_7TeV")},
                    8.0: {"default": ("atlas8tev", "ATLAS_8TeV")},
                    13.0: {"default": ("atlas13tev", "ATLAS_13TeV")},
                    14.0: {
                        "projected": ("atlas14tev_projected", "ATLAS_14TeV_HighLumi"),
                        "hl_flatbtagger": ("atlas14tev_hl_flatbtagger", "ATLAS_14TeV_HighLumi")
                        },
                    },
                "CMS": {
                    7.0: {"default": ("cms7tev", "CMS_7TeV")},
                    8.0: {"default": ("cms8tev", "CMS_8TeV")},
                    13.0: {"default": ("cms13tev", "CMS_13TeV")},
                    14.0: {"projected": ("cms14tev_projected", "CMS_14TeV_HighLumi")},
                    }
                }
        cls.analysis_handlers = {
                "atlas": "AnalysisHandlerATLAS",
                "atlas7tev": "AnalysisHandlerATLAS_7TeV",
                "atlas8tev": "AnalysisHandlerATLAS_8TeV",
                "atlas13tev": "AnalysisHandlerATLAS_13TeV",
                "atlas14tev_projected": "AnalysisHandlerATLAS_14TeV_projected",
                "atlas14tev_hl_flatbtagger": "AnalysisHandlerATLAS_14TeV_HL_FlatBtagger",
                "cms": "AnalysisHandlerCMS",
                "cms7tev": "AnalysisHandlerCMS_7TeV",
                "cms8tev": "AnalysisHandlerCMS_8TeV",
                "cms13tev": "AnalysisHandlerCMS_13TeV",
                "cms14tev_projected": "AnalysisHandlerCMS_14TeV_projected",
                }
        cls.paths = paths
        cls.used_experiments = set()
        cls.config = ConfigParser.RawConfigParser()
        cls.flags = flags    
        cls.parameters = parameters     
    
    @classmethod
    def save(cls, filename):
        """ Stores the current status of this instance in a file """
        contents = dict()
        contents["analyses"] = Info.analyses
        contents["paths"] = Info.paths
        contents["files"] = Info.files
        contents["flags"] = Info.flags
        contents["parameters"] = Info.parameters
        contents["identifiers"] = pickle.dumps(Info.identifiers)
        with open(filename, "w") as f:
            json.dump(contents, f, indent=2)
    
    @classmethod
    def load(cls, filename):
        """ Loads contents for current instance from a valid file  """
        with open(filename, "r") as f:
            contents = json.load(f)            
        try:
           cls.analyses = contents["analyses"]
           cls.paths = contents["paths"]
           cls.files = contents["files"]
           cls.flags = contents["flags"]
           cls.parameters = contents["parameters"]
           cls.identifiers = pickle.loads(contents["identifiers"])
        except KeyError:
            AdvPrint.cerr_exit("Problem loading info.j file "+inputfile)        
        
    @classmethod            
    def parse_arguments(cls, emptyparser=False):       
        parser = argparse.ArgumentParser(description='CheckMATE takes an arbitrary set of eventfiles (in .hepmc, .hep or lhe format), processes them with Delphes and analyzes the output with a particular analysis to be chosen from a given subset. It is then tested how well the given event files is in agreement with the current results of the corresponding collider experiment in order to get an estimate, how strong this model is already excluded by experimental data..')
        parser.add_argument('-n', '--name', dest='name', default="CheckMATE_run", help='name for the run')   
        parser.add_argument('-a', '--analysis', dest='analysis', default="atlas8TeV,cms8TeV", help='Analysis/es to be applied on the processed event files')
        parser.add_argument('-p', '--process', dest='process', default="process", type=str, help="Process identifier. When combining, events from equal/different processes are averaged/summed.")
        parser.add_argument('-maxev', '--max-events', dest='maxevents', default=-1, type=int, help="Maximum number of events to be simulated and/or analysed.")
        
        parser.add_argument('-xs', '--xsect', dest='xsect', type=str, default="", help="Cross section for the given event. Example format: 1.73 FB. ")
        parser.add_argument('-xse', '--xsecterr', dest='xsecterr', type=str, default="", help="Cross section error for the given event;. Example format: 0.01*FB or 10.4 %%. ")        
        parser.add_argument('-xsth', '--xsectthreshold', dest='xsthresh', type=str, default="", help="Only in MG5+Pythia8: Set result to 0 if parton xs is smaller than this.")        
        parser.add_argument('-kf', '--kfactor', dest='kfactor', type=str, default="", help="K-factor for the given event.")
        parser.add_argument('-ev', '--events', dest='events', default="", type=str, help='List of event files to be analysed (.hepmc, .hep, .lhe), separated by semicolons')
        
        parser.add_argument('-slha', '--slha-file', dest='slhafile', default="", help='slha file (for pythia).')              
        parser.add_argument('-invpids', '--invisible-pids', dest='invpids', default="", help='PIDs for BSM particles that should be considered invisible for the detector (separate with ,)')              
        parser.add_argument('-mgcommand', '--madgraph-command', dest='mgcommand', default="", type=str, help='Process commands to be used by MG5_aMC@NLO (e.g. "import model mssm; generate p p > go go")')
        parser.add_argument('-mgproc', '--madgraph-process', dest='mgprocess', default="", type=str, help='Process card to be generated by MG5_aMC@NLO')
        parser.add_argument('-mgparam', '--madgraph-param', dest='mgparam', default="", type=str, help='Commands for MadGraph5 (param card)')
        parser.add_argument('-mgrun', '--madgraph-run', dest='mgrun', default="", type=str, help='Run card for MadGraph5')
        parser.add_argument('-mgconfig', '--madgraph-config', dest='mgconfig', default="", type=str, help='Config card for MadGraph5')
        parser.add_argument('-pyp', '--pythia-process', dest='pyprocess', default="", type=str, help='Process to be generated by Pythia8')
        parser.add_argument('-pyc', '--pythia-card', dest='pycard', default="", type=str, help='Input card used by Pythia8')
        parser.add_argument('-pyr', '--pythia-rndm', dest='pyrndm', default="", type=str, help='Binary generator random state for Pythia8')
        
        parser.add_argument('-wp8', '--write-pythia8', dest='writepythia8',  action='store_true', help='If set, pythia .hepmc files are stored.')
        parser.add_argument('-wd', '--write-delphes', dest='writedelphes',  action='store_true', help='If set, delphes .root files are stored.')      
        
        parser.add_argument('-cls', '--full-cls', dest='fullcls', action='store_true', help="Evaluate full CLs to the evaluated number of signal events (instead of just comparing to 95 percent limit).")
        parser.add_argument('-bcls', '--best-cls', dest='bestcls', type=int, help="Evaluates CLs of the best signal region determined by r-value test.")
        parser.add_argument('-likeli', '--likelihood', dest='likelihood', action='store_true', help="Evaluate likelihood for each signal region using the MC-approach (and sum all signal regions).")
        parser.add_argument('-likelirm', '--likelihoodRootmethod', dest='likelihoodRootmethod', action='store_true', help="Evaluate likelihood for each signal region using the root method approach (and sum all signal regions).")
        parser.add_argument('-mcstats_off', '--no_mc_stat_err', dest='no_mc_stat_err', action='store_true', help="Do not include Monte Carlo statistical error in total uncertainty")
        parser.add_argument('-eff_tab', '--eff_tab', dest='eff_tab', action='store_true', help="Creates efficiency tables for every signal region in each analysis run")
        parser.add_argument('-zsig', '--zsig', dest='zsig', action='store_true', help="Evaluate signal significance in a fully frequentist fashion.")
        
        parser.add_argument('-erfc', '--event-result-file-columns', dest='erfc', type=str, default="", help="Columns that should be stored in event-wise result files")
        parser.add_argument('-prfc', '--process-result-file-columns', dest='prfc', type=str, default="", help="Columns that should be stored in process-wise result files")
        parser.add_argument('-bpaefc', '--bestperanalysis-evaluation-file-columns', dest='bpaefc', type=str, default="", help="Columns that should be stored in best-per-analysis evaluation file")
        parser.add_argument('-tefc', '--total-evaluation-file-columns', dest='tefc', type=str, default="", help="Columns that should be stored in final evaluation file")
        
        
        parser.add_argument('-od', '--outdir', dest='odir', default=cls.paths['results'], help='Directory where the results should be saved (default: '+cls.paths['results']+').')
        parser.add_argument('-oe', '--output-exists', dest='output_exists', default="ask", type=str, help="What to do if output already exists. overwrite will delete existing output and overwrite it with the new results. add will add the current results to the old ones. In any other case, a prompt will ask you.")   
        parser.add_argument('-q', '--quiet', dest='quiet', action='store_true', help='Suppresses all output (sets automatially).')
        parser.add_argument('-sp', '--skip-paramcheck', dest='force', action='store_true', help="Skip startup parameter check.")               
        parser.add_argument('-sa', '--skip-analysis', dest='skipanalysis', action='store_true', help='Skips analysis step (e.g. to only produce a Delphes root file.')
        parser.add_argument('-spy', '--skip-pythia', dest='skippythia', action='store_true', help='Skpis Pythia step. Delphes will work with the provided LHE file.')
        parser.add_argument('-se', '--skip-evaluation', dest='skipevaluation', action='store_true', help='Skips evaluation step.')
        parser.add_argument('-cr', '--control-regions', dest='controlregions', action='store_true', help='Analyses control regions instead of signal regions. Sets -se automatically.')
        parser.add_argument('-rs', '--random-seed', dest='randomseed', type=int, default=0, help='Chooses fixed seed for random number generator. 0 chooses a random seed automatically.')     
        
        # Parse arguments and set return parameters
        if emptyparser:
            return parser.parse_args([]) # needed for fill_info_from_file
        else:
            return parser.parse_args()
 
    @classmethod
    def fill_info_from_args(cls, args):
        cls.files["slha"] = args.slhafile
        cls.files["py8rndm"] = args.pyrndm
        cls.paths["results"] = args.odir
        if args.invpids != "":
           try:
              cls.parameters['invisiblePIDs'] = [int(x) for x in args.invpids.split(",")]
           except ValueError:
              AdvPrint.cerr_exit("invisible PIDs are in wrong format. Must be integer numbers, split by ','")
        cls.parameters['randomseed'] = args.randomseed
        cls.parameters["outputexists"] = args.output_exists
        if args.force:
            cls.flags["skipparamcheck"] = True
        if args.quiet:
            cls.flags["quietmode"] = True
        if args.skipanalysis:
            cls.flags["skipanalysis"] = True
        if args.skippythia:
            cls.flags["skippythia"] = True
        if args.skipevaluation:
            cls.flags['skipevaluation'] = True            
        if args.fullcls:
            cls.flags["fullcls"] = True
        if args.bestcls:
            cls.flags["bestcls"] = int(args.bestcls)
        if args.likelihood:
            cls.flags["likelihood"] = True
        if args.likelihoodRootmethod:
            cls.flags["likelihoodRootmethod"] = True
        if args.no_mc_stat_err:
            cls.flags["no_mc_stat_err"] = True
        if args.eff_tab:
            cls.flags["eff_tab"] = True  
        if args.zsig:
            cls.flags['zsig'] = True
        if args.controlregions:
            cls.flags["controlregions"] = True
        if args.writepythia8:            
            cls.flags['write_pythia_events'] = True 
        if args.writedelphes:            
            cls.flags['write_delphes_events'] = True
        if args.tefc != "":
            cls.parameters["TotalEvaluationFileColumns"] = args.tefc.split(",")
        if args.prfc != "":
            cls.parameters["ProcessResultFileColumns"] = args.prfc.split(",")
        if args.erfc != "":
            cls.parameters["EventResultFileColumns"] = args.erfc.split(",")
        if args.bpaefc != "":
            cls.parameters["BestPerAnalysisEvaluationFileColumns"] = args.bpaefc.split(",") 
        cls.make_flags_consistent()
        
        cls.load_analyses(args.analysis)
        output_name =  args.name.replace(" ", "_")
        cls.fill_output_paths_and_files(cls.paths['results'], output_name) 
        cls.check_info()
 
    @classmethod
    def fill_info_from_parameters(cls):
        args = cls.parse_arguments()
        cls.fill_info_from_args(args)
        
    @classmethod
    def fill_info_from_file(cls, pfile):
        if not os.path.isfile(pfile):
            AdvPrint.cerr_exit("Parameter file '"+pfile+"' does not exist.")    
        
        Config = ConfigParser.ConfigParser()
        Config.read(pfile)
        sections = Config.sections()
        
        # read parameters, starting with the default from argparse
        args = cls.parse_arguments(True)
        
        # spaces in "name" are replaced by underscores to prevent problems in the file handling
        output_name = "CheckMATE_run"
        analyses_to_load = "atlas & 8TeV, cms & 8TeV"
        cls.files["slha"] = ""
        if "Parameters" in sections:
            for optional_parameter in Config.options("Parameters"):
                if optional_parameter == "skipparamcheck":
                    args.force = Config.getboolean("Parameters", "skipparamcheck")
                elif optional_parameter == "outputexists":
                    args.output_exists = Config.get("Parameters", "outputexists")
                elif optional_parameter == "quietmode":
                    args.quiet = Config.getboolean("Parameters", "quietmode")
                elif optional_parameter == "skipanalysis":
                    args.skipanalysis = Config.getboolean("Parameters", "skipanalysis")
                elif optional_parameter == "skippythia":
                    args.skippythia = Config.getboolean("Parameters","skippythia")
                elif optional_parameter == "skipevaluation":
                    args.skipevaluation = Config.getboolean("Parameters", "skipevaluation")
                elif optional_parameter == "controlregions":
                    args.controlregions = Config.getboolean("Parameters", "controlregions")
                elif optional_parameter == "efftab":
                    args.eff_tab = Config.get("Parameters", "efftab")
                elif optional_parameter == "fullcls":
                    args.fullcls = Config.getboolean("Parameters", "fullcls")
                elif optional_parameter == "bestcls":
                    args.bestcls = Config.getint("Parameters", "bestcls")
                elif optional_parameter == "likelihood":
                    args.likelihood = Config.getboolean("Parameters", "likelihood")
                elif optional_parameter == "likelihoodrootmethod":
                    args.likelihoodRootmethod = Config.getboolean("Parameters", "likelihoodRootmethod")
                elif optional_parameter == "no_mc_stat_err":
                    args.no_mc_stat_err = Config.getboolean("Parameters", "no_mc_stat_err")    
                elif optional_parameter == "zsig":
                    args.zsig = Config.getboolean("Parameters", "zsig")
                elif optional_parameter == "outputdirectory":
                    args.odir = Config.get("Parameters", "outputdirectory")
                elif optional_parameter == "invisiblepids":
                    args.invpids = Config.get("Parameters", "invisiblepids")
                elif optional_parameter == "slhafile":
                    args.slhafile = Config.get("Parameters", "slhafile")
                elif optional_parameter == "mgprocess":
                    args.mgprocess = Config.get("Parameters", "mgprocess")
                elif optional_parameter == "randomseed":
                    args.randomseed = Config.getint("Parameters", "randomseed")
                elif optional_parameter == "writepythiaevents":
                    args.writepythia8 = Config.getboolean("Parameters", "writepythiaevents")
                elif optional_parameter == "writedelphesevents":
                    args.writedelphes = Config.getboolean("Parameters", "writedelphesevents")
                elif optional_parameter == "processresultfilecolumns":
                    args.prfc = Config.get("Parameters", "processresultfilecolumns")
                elif optional_parameter == "eventresultfilecolumns":
                    args.erfc = Config.get("Parameters", "eventresultfilecolumns")
                elif optional_parameter == "bestperanalysisresultfilecolumns":
                    args.bpaefc = Config.get("Parameters", "bestperanalysisresultfilecolumns")
                elif optional_parameter == "totalresultfilecolumns":
                    args.tefc = Config.get("Parameters", "totalresultfilecolumns")
                elif optional_parameter == "name":
                    args.name = Config.get("Parameters", "name")
                elif optional_parameter == "analyses":
                    args.analysis = Config.get("Parameters", "analyses")
                else:
                    AdvPrint.cerr_exit("Unknown optional parameter '"+optional_parameter+"'")                    
        cls.fill_info_from_args(args)
       
    @classmethod
    def check_info(cls):
        """ Checks if info parameters are valid and updates if the output directory already exists """
        
        # check if the evaluation columns the user asked for actually exist
        # headers must be imported locally to avoid circular reference
        from evaluator import Evaluator
        from resultcollector import ResultCollector
        pseudoCollector = ResultCollector("", "", "")
        pseudoEvaluator = Evaluator(pseudoCollector)
        pseudoCollector.line_from_data(cls.parameters['EventResultFileColumns'])
        pseudoCollector.line_from_data(cls.parameters['ProcessResultFileColumns'])
        pseudoEvaluator.line_from_data(cls.parameters['TotalEvaluationFileColumns'])
        pseudoEvaluator.line_from_data(cls.parameters['BestPerAnalysisEvaluationFileColumns'])
        
        _check_outputexists()
        if os.path.isdir(Info.paths['output']) and os.path.isfile(Info.files['internal_info']):
            if Info.parameters["outputexists"] == "ask":
                while True:
                    AdvPrint.cout("Output directory with results already exists!")
                    c = raw_input("Choose: (o)verwrite, (a)dd to existing results, (s)top\n")
                    if c == "o":
                        Info.parameters["outputexists"] = "overwrite"
                        break
                    elif c == "s":
                        exit(1)
                    elif c == "a":
                        Info.parameters["outputexists"] = "add"
                        break
            if Info.parameters["outputexists"] == "add":
                AdvPrint.cout(" ('add' mode: settings of previous run are used and only new events/processes are added!)") 
                Info.load(Info.files['internal_info'])
                Info.parameters["outputexists"] = "add" # might have been overwritten during the loading process

        elif os.path.isdir(Info.paths['output']):
            if Info.parameters["outputexists"] == "ask":
                while True:
                    AdvPrint.cout("Output directory with incomplete results already exists!")
                    c = raw_input("Choose: (o)verwrite, (s)top\n")
                    if c == "o":
                        Info.parameters["outputexists"] = "overwrite"
                        break
                    elif c == "s":
                        exit(1)

    @classmethod
    def fill_process_from_args(cls, args):
        from process import Process
        from events import HEPEvents, HepMCEvents, DelphesEvents, Pythia8Events, MG5Events, LHEEvents
        process = Process(args.process.replace(" ", "_"))
        potentialPy8Events = None 
        potentialMG5Events = None 
        
        if args.events != "":
            lhefilevector = list() # need to be storesd separately as they go in one overall LHEEvents object
            for eventfile in args.events.replace(";", ",").split(","):
                eventfile = Info.check_and_absolutize_file(eventfile)
                ending = eventfile.split(".")[-1].lower()
                events = None
                if ending == "hep":
                    events = HEPEvents(args.process, eventfile)
                elif ending == "hepmc":
                    events = HepMCEvents(args.process, eventfile)
                elif ending == "root":
                    events = DelphesEvents(args.process, eventfile)
                elif ending == "lhe":
                   lhefilevector.append(eventfile)
                else:
                    AdvPrint.cerr_exit("File ending "+ending+" not known!")                                
                if events != None:
                    process.eventsList.append(events)
                    
            if lhefilevector != list():
                process.eventsList.append(LHEEvents(args.process, lhefilevector))
        
        if args.xsect != "":
            xsect_str = args.xsect
            if "*" in xsect_str:
                xsect_split = xsect_str.split("*")
            else:
                xsect_split = xsect_str.split(" ")             
            if len(xsect_split) != 2:
                AdvPrint.cerr_exit("Cross section in invalid format ("+str(xsect_str)+")")
            process.have_xsect = True
            process.xsec = float(xsect_split[0])
            process.xsec_unit = xsect_split[1]
   
        if args.xsecterr != "":
            xsecterr_str = args.xsecterr  
            if "*" in xsecterr_str:
                xsecterr_split = xsecterr_str.split("*")
            else:
                xsecterr_split = xsecterr_str.split(" ")            
            # Note that no unit has to be given if the error is exactly 0
            if len(xsecterr_split) != 2 and float(xsecterr_split[0]) != 0.0:                        
                AdvPrint.cerr_exit("Cross section error in invalid format ("+str(xsecterr_str)+")")
            process.have_xerr = True
            process.xerr = float(xsecterr_split[0])
            process.xerr_unit = xsecterr_split[1]
            
        if args.kfactor != "":
            process.have_kfac = True
            process.kfac = float(args.kfactor)
 
            
        
        # first, global check for py8 parameters which require py8 linking and which load the Py8 event
        if args.pyprocess != "" or args.pycard != "":
            if 'pythia8_lib_path' not in Info.paths or Info.paths['pythia8_lib_path'] == "":
                AdvPrint.cerr_exit("You cannot generate/shower events with Pythia8 without properly linking CheckMATE to this tool. Please restart the CheckMATE-installation routine and use the --with-pythia parameter during the ./configure step!")
            potentialPy8Events = Pythia8Events(args.process)
                
        if args.pyprocess != "":            
            potentialPy8Events.set_processString(args.pyprocess)
            
        if args.pycard != "":
            potentialPy8Events.set_inFile(args.pycard)

        # same for MG5Events
        if args.mgcommand != "" or args.mgprocess != "" or args.mgparam != "" or args.mgrun != "" or args.mgconfig != "" or args.mgcommand != "" or args.xsthresh != "":
            if 'mg5_source_path' not in Info.paths or Info.paths['mg5_source_path'] == "":
                    AdvPrint.cerr_exit("You cannot generate events using MG5_aMC@NLO without properly linking CheckMATE to this tool. Please restart the CheckMATE-installation routine and use the --with-madgraph parameter during the ./configure step!")
                    
            if 'pythia8_lib_path' not in Info.paths or Info.paths['pythia8_lib_path'] == "":
                    AdvPrint.cerr_exit("You cannot generate using MG5_aMC@NLO without properly linking CheckMATE to both MG5_aMC@NLO and Pythia8!. Please restart the CheckMATE-installation routine and use the --with-pythia parameter during the ./configure step!")
            potentialMG5Events = MG5Events(args.process)
            
        if args.mgcommand != "":
            potentialMG5Events.set_commandstring(args.mgcommand)
            
        if args.mgprocess != "":
            potentialMG5Events.set_proccard(Info.check_and_absolutize_file(args.mgprocess))
            
        if args.mgparam != "":
            potentialMG5Events.set_paramcard(Info.check_and_absolutize_file(args.mgparam))
            
        if args.mgrun != "":
            potentialMG5Events.set_runcard(Info.check_and_absolutize_file(args.mgrun))
        
        if args.mgconfig != "":
            potentialMG5Events.set_configcard(Info.check_and_absolutize_file(args.mgconfig))
	  
        if args.xsthresh != "":            
            xsectth_str = args.xsthresh
            if "*" in xsectth_str:
                xsectth_split = xsectth_str.split("*")
            else:
                xsectth_split = xsectth_str.split(" ")             
            if len(xsectth_split) != 2:
                AdvPrint.cerr_exit("Cross section Threshold in invalid format ("+str(xsectth_str)+")")
            potentialMG5Events.set_xsthr(float(xsectth_split[0]), xsectth_split[1])
            
        if potentialPy8Events != None:
            process.eventsList.append(potentialPy8Events)
        if potentialMG5Events != None:
            process.eventsList.append(potentialMG5Events)
            
        if args.maxevents != "":
            for events in process.eventsList:                
                events.maxEvents = int(args.maxevents)
                
        process.ecm = cls.parameters["ecm"]
        return process
        
    @classmethod
    def fill_processes_from_parameters(cls):
        args = cls.parse_arguments()
        # command line input only allows for 1 process at a time
        return [cls.fill_process_from_args(args)]
    
    @classmethod
    def fill_processes_from_file(cls, pfile):
        if not os.path.isfile(pfile):
            AdvPrint.cerr_exit("Parameter file '"+pfile+"' does not exist.")  
            
        Config = ConfigParser.ConfigParser()
        Config.read(pfile)
        sections = Config.sections()
        
        # Remove non-process blocks 
        if "Parameters" in sections:
            sections.remove("Parameters")
            
        # Set up event information. Remaining sections consist of individual processes
        procList = []
        for process_block in sections:
            # translate Config object in args
            
            # first, get template args
            args = cls.parse_arguments(True)       

            args.process = process_block
            # Check and save event file
            if "events" in Config.options(process_block):
                args.events = Config.get(process_block, "events").replace("\n", ",").replace(",,", ",").replace(" ", "").replace("\\", " ")

            # Read cross section in the form "Num Unit" or "Num*Unit"
            if "xsect" in Config.options(process_block):
                args.xsect = Config.get(process_block, "xsect")
            if "xsecterr" in Config.options(process_block):
                args.xsecterr = Config.get(process_block, "xsecterr")
            if "xsectthreshold" in Config.options(process_block):
                args.xsthresh = Config.get(process_block, "xsectthreshold")
    
            if "kfactor" in Config.options(process_block):
                args.kfactor = Config.get(process_block, "kfactor")
            
            if "pythia8process" in Config.options(process_block):
                args.pyprocess = Config.get(process_block, "pythia8process")
            if "pythia8rndm" in Config.options(process_block):
                args.pyrndm = Config.get(process_block, "pythia8rndm")
                
            if "pythia8card" in Config.options(process_block):
                args.pycard = Config.get(process_block, "pythia8card")

            if "mgprocess" in Config.options(process_block):
                args.mgprocess = Config.get(process_block,"mgprocess")
            if "mgcommand" in Config.options(process_block):
                args.mgcommand = Config.get(process_block,"mgcommand")
            if "mgparam" in Config.options(process_block):
                args.mgparam = Config.get(process_block,"mgparam")
            if "mgrun" in Config.options(process_block):
                args.mgrun = Config.get(process_block,"mgrun")
            if "mgconfig" in Config.options(process_block):
                args.mgconfig = Config.get(process_block,"mgconfig")
            if "maxevents" in Config.options(process_block):
                args.maxevents = Config.get(process_block, "maxevents")
            procList.append(cls.fill_process_from_args(args))
            
        return procList    

    @classmethod
    def load_analyses(cls, analysis_input_string):
        """ Uses the input string from the user to load one or more analyses """    
        analysis_input_string = analysis_input_string.lower() # to avoid capitalisation errors
        tokens = analysis_input_string.split(",")
        for token in tokens:
            any_passed = False
            token = token.strip().lower()
         
            for a in cls.analysis_list:
                parameters = cls.get_analysis_parameters(a)
                # to avoid case issues, transform parameters to lower()

                passed = True

                if token not in parameters["experiment"] and token != a:
                    passed = False
 
                if passed:
                    any_passed = True
                    if "ecm" in cls.parameters and cls.parameters["ecm"] != 0.0 and ( abs(float(cls.parameters["ecm"]) - float(parameters["ecm"]))>0.01):
                        AdvPrint.cerr_exit("    You must not load analyses with different center of mass energies!\nIf you desire to do this, please run CheckMATE separately for each center of mass energy!")
                    cls.parameters["ecm"] = parameters["ecm"]
                    cls.analyses.append(a)
                    experiment = parameters["experiment"]
                    if not experiment in cls.experiments:
                        AdvPrint.cerr_exit("Unknown experiment: "+experiment+" used by analysis"+a)
                    cls.used_experiments.add(experiment)
            if not any_passed:
                AdvPrint.cerr_exit("Couldn't find any analyses for '"+token+"'.")
             
            
    @classmethod
    def book_events(cls, events):
        # Registers a process and gives a unique identifier
        from events import Events
        if not isinstance(events, Events):
            AdvPrint.cerr_exit("Internal Error while trying to book an event! (event does not seem to be an 'Event')")
        same_ident = [ident for ident in cls.identifiers if (events.name == ident or events.name+"_" in ident)]
        if len(same_ident) == 0: # No same id: Just register
            events.identifier = events.name
        elif len(same_ident) == 1: # One same id already: rename old 'name_1' and register 'name_2'            
            move_events = cls.identifiers[events.name]
            cls.identifiers.pop(events.name)
            # rename old identifier to "identifier_1" only if it hasn't been processed yet (to not screp up match to existing filenames)
            if not move_events.processed:
                move_events.identifier = move_events.identifier+"_event1"   
            cls.identifiers[move_events.identifier] = move_events
            events.identifier = events.name+"_event2"
        else: # More: find max of the _X and add _X+1
            maxnum = max([int(ident.split("_event")[-1]) for ident in same_ident if "_event" in ident])
            events.identifier = events.name+"_event"+str(maxnum+1)        
        cls.identifiers[events.identifier] = events
        return
    
    @classmethod
    def unbook_event(cls, events):
        """Sometimes initially booked events turn out to false (e.g. double booking in "add" mode or when trying to merge .lhe files which count as 1 event"""
        if events.__class__.__name__ != "Event":
            AdvPrint.cerr_exit("Internal error while trying to book an event!")
        same_ident = [ident for ident in cls.identifiers if events.name in ident]
        if len(same_ident) == 0: # error: the event can only be unbooked if it exists
            AdvPrint.cerr_exit("Internal error: trying to unbook an unbooked event!")
        elif len(same_ident) == 1: # Simple: Just remove event from booking list
            cls.identifiers.pop(events.name)
        elif len(same_ident) == 2: # Remove event and the other event does not need an "_X" index
            cls.identifiers.pop(events.name)
            if not cls.identifiers[0].processed: # to avoid changing things which 
                cls.identifiers[0].identifier = cls.identifiers[0].identifier[:cls.identifiers[0].identifier.rfind("_")]
        else: # remove this event and shift index of all upcoming ones down by 1
            # suffix of to-be-removed event
            remove_index = int(cls.identifiers[0].identifier[:cls.identifiers[0].identifier.rfind("_event")+6:])
            #remove the event
            cls.identifiers.pop(events.name)
            # shift all events which have a higher index down by one (if they haven't been processed yet, that is)
            for ident in same_ident:
                curr_event = cls.identifiers[ident].identifier
                if not curr_events.processed:
                    old_index = int(curr_events.identifier[curr_events.rfind("_event")+6:])
                    if old_index > remove_index:
                        new_index = old_index-1
                        cls.identifiers[ident].identifier =  curr_events.identifier[:curr_events.identifier.rfind("_event")]+"_event"+str(new_index)
        return

    @classmethod
    def add_analysis(cls, analysis_name, analysis_group):
        """
        Inserts an analysis in the info class
        """
        if analysis_name in cls.analysis_list:
            # The analysis is already there. Don't add it twice.
            return
        cls.analysis_list.append(analysis_name)
        cls.analysis_group_map[analysis_name] = analysis_group
        cls.files["analysis_settings"][analysis_name] = os.path.join(cls.paths['analysis_info'], analysis_group, "{}_var.j".format(analysis_name))
        cls.files["evaluation_reference"][analysis_name] = os.path.join(cls.paths['analysis_info'], analysis_group, "{}_ref.dat".format(analysis_name))
        cls.files["analysis_source"][analysis_name] = os.path.join(cls.paths['analysis'], 'src', 'analyses', analysis_group, analysis_name+'.cc')
        cls.files["analysis_CR_source"][analysis_name] = os.path.join(cls.paths['analysis'], 'src', 'analyses', analysis_group, analysis_name+'_CR.cc')
        cls.files["analysis_header"][analysis_name] = os.path.join(cls.paths['analysis'], 'include', 'analyses', analysis_group, analysis_name+'.h')

    @classmethod
    def fill_standard_paths_and_files(cls, config_paths=dict()):
        """Fills cls.paths with standard paths of the CheckMATE installation"""

        if config_paths != dict() and config_paths['mg5_source_path']:
            cls.paths['mg5_source_path'] = config_paths['mg5_source_path']
        if config_paths != dict() and config_paths['pythia8_lib_path']:
            cls.paths['pythia8_lib_path'] = config_paths['pythia8_lib_path']

        cls.paths['checkmate'] = os.path.split(os.path.split(os.path.split(os.path.realpath(__file__))[0])[0])[0]
        
        cls.paths['results'] = os.path.join(cls.paths['checkmate'], 'results')
        cls.paths['tools'] = os.path.join(cls.paths['checkmate'], 'tools')
        cls.paths['data'] = os.path.join(cls.paths['checkmate'], 'data')
        
        cls.paths['analysis'] = os.path.join(cls.paths['tools'], 'analysis')
        cls.paths['fritz'] = os.path.join(cls.paths['tools'], 'fritz')
        
        cls.paths['analysis_info'] = os.path.join(cls.paths['data'], 'analysis_info')
        cls.paths['cards'] = os.path.join(cls.paths['data'], 'cards')        
        cls.files['list_of_analyses'] = dict()
        for group in cls.analysis_groups:
            cls.files["list_of_analyses"][group] = os.path.join(cls.paths['analysis_info'], '{}_analyses.txt'.format(group))
                
        cls.files['pythia_settings_template'] = os.path.join(cls.paths['cards'], 'pythia_default_card.in')
        cls.files['pythia_mg5minimal_template'] = os.path.join(cls.paths['cards'], 'pythia_mg5minimal_default_card.in')
        cls.files['pythia_lhe_template'] = os.path.join(cls.paths['cards'], 'pythia_default_LHE_card.in')                
        cls.files['mg5_run_template'] = os.path.join(cls.paths['cards'], 'mg5_default_run_card.dat')                
        cls.files['me5_configuration_template'] = os.path.join(cls.paths['cards'], 'mg5_default_me5_configuration.txt')

        delphes_global_config = dict()
        delphes_global_config["atlas"] = os.path.join(cls.paths['cards'], 'delphes_skimmed_ATLAS.tcl')
        delphes_global_config["atlas7tev"] = os.path.join(cls.paths['cards'], 'delphes_skimmed_ATLAS.tcl')
        delphes_global_config["atlas8tev"] = os.path.join(cls.paths['cards'], 'delphes_skimmed_ATLAS.tcl')
        delphes_global_config["atlas13tev"] = os.path.join(cls.paths['cards'], 'delphes_skimmed_ATLAS_13TeV.tcl')
        delphes_global_config["atlas14tev_projected"] = os.path.join(cls.paths['cards'], 'delphes_skimmed_ATLAS_14TeV.tcl')
        delphes_global_config["atlas14tev_hl_flatbtagger"] = os.path.join(cls.paths['cards'], 'delphes_skimmed_ATLAS_14TeV.tcl')
        delphes_global_config["cms"] = os.path.join(cls.paths['cards'], 'delphes_skimmed_CMS.tcl')
        delphes_global_config["cms7tev"] = os.path.join(cls.paths['cards'], 'delphes_skimmed_CMS.tcl')
        delphes_global_config["cms8tev"] = os.path.join(cls.paths['cards'], 'delphes_skimmed_CMS.tcl')
        delphes_global_config["cms13tev"] = os.path.join(cls.paths['cards'], 'delphes_skimmed_CMS_13TeV.tcl')
        delphes_global_config["cms14tev_projected"] = os.path.join(cls.paths['cards'], 'delphes_skimmed_CMS_14TeV.tcl')
        cls.files['delphes_global_config'] = delphes_global_config
        
        cls.files['fritz_bin'] = os.path.join(cls.paths['fritz'], 'bin', "fritz")
        
        cls.files['analysis_makefile'] = os.path.join(cls.paths['analysis'], "Makefile.am")
        cls.files['analysis_template_source'] = os.path.join(cls.paths['analysis'], 'src', 'base', 'template.cc.raw')
        cls.files['analysis_template_CR_source'] = os.path.join(cls.paths['analysis'], 'src', 'base', 'template_CR.cc.raw')
        cls.files['analysis_template_header'] = os.path.join(cls.paths['analysis'], 'include', 'base', 'template.h.raw')
        cls.files['analysis_template_header_cr'] = os.path.join(cls.paths['analysis'], 'include', 'base', 'template_cr.h.raw')
        
        # analysis information
        cls.files['analysis_settings'] = dict()
        cls.files["analysis_source"] = dict()
        cls.files["analysis_CR_source"] = dict()
        cls.files["analysis_header"] = dict()
        cls.files['delphes_config'] = dict()
        cls.files['evaluation_reference'] = dict()
        
        # analysis handler sources
        src_dir = os.path.join(cls.paths["fritz"], "src", "analysishandler")
        inc_dir = os.path.join(cls.paths["fritz"], "include", "analysishandler")
        for analysis_handler in cls.analysis_handlers.values():
            cls.files[analysis_handler] = dict()
            cls.files[analysis_handler]["src"] = os.path.join(src_dir, "{}.cc".format(analysis_handler))
            cls.files[analysis_handler]["include"] = os.path.join(inc_dir, "{}.h".format(analysis_handler))
        
        analyses, groups = cls.get_all_checkmate_analyses()
        cls.analysis_list = analyses
        cls.analysis_group_map = groups
        for a in cls.analysis_list:
            group = cls.analysis_group_map[a]
            cls.files["analysis_settings"][a] = os.path.join(cls.paths['analysis_info'], group, "{}_var.j".format(a))
            cls.files["evaluation_reference"][a] = os.path.join(cls.paths['analysis_info'], group, "{}_ref.dat".format(a))
            cls.files["analysis_source"][a] = os.path.join(cls.paths['analysis'], 'src', 'analyses', group, a+'.cc')
            cls.files["analysis_CR_source"][a] = os.path.join(cls.paths['analysis'], 'src', 'analyses', group, a+'_CR.cc')
            cls.files["analysis_header"][a] = os.path.join(cls.paths['analysis'], 'include', 'analyses', group, a+'.h')



    @classmethod
    def fill_output_paths_and_files(cls, odir, oname):
        """Fills cls.paths with paths given a particular output directory"""
        cls.paths['output'] = os.path.join(odir, oname)
        cls.paths['output_delphes'] = os.path.join(cls.paths['output'], "delphes")
        cls.paths['output_pythia'] = os.path.join(cls.paths['output'], "pythia")
        cls.paths['output_mg5'] = os.path.join(cls.paths['output'], "mg5amcatnlo")
        cls.paths['output_internal'] = os.path.join(cls.paths['output'], "internal")
        cls.paths['mg5_procs'] = list() # List of generated process directories; will be filled dynamically by each process
        cls.files['mg5_proc_cards'] = list() # List of generated process directories; will be filled dynamically by each process
        cls.paths['output_fritz'] = os.path.join(cls.paths['output'], "fritz")
        cls.paths['output_analysis'] = os.path.join(cls.paths['output'], "analysis")
        cls.paths['output_evaluation'] = os.path.join(cls.paths['output'], "evaluation")
        #cls.files['output_progress'] = os.path.join(cls.paths['output'], "progress.txt") TODO: Outdated. USed in CM1, but in CM2 there are the internal/var.j files
        
        cls.files['pythia_cards'] = list()
        cls.files['pythia_events'] = list()
        cls.files['delphes_events'] = list()
        
        cls.files['internal_info'] = os.path.join(cls.paths['output_internal'], "info.j")
        cls.files['internal_processes'] = os.path.join(cls.paths['output_internal'], "processes.j")
        
        cls.files['delphes_log'] = os.path.join(cls.paths['output_delphes'], "delphes_output.log")
        cls.files['pythia_log'] = os.path.join(cls.paths['output_pythia'], "pythia_output.log")
        cls.files['analysis_log'] = os.path.join(cls.paths['output_analysis'], "analysisstdout")
        cls.files['fritz_log'] = os.path.join(cls.paths['output_fritz'], "fritz_error.log")
        cls.files['mg5_log'] = os.path.join(cls.paths['output_mg5'], "mg5amcatnlo_output.log")
        
        if cls.flags["likelihood"]:
            cls.files['likelihood'] = os.path.join(cls.paths['output'], "likelihood.txt")
        if cls.flags["likelihoodRootmethod"]:
            cls.files['likelihoodRootmethod'] = os.path.join(cls.paths['output'], "likelihoodrm.txt")
        cls.files['output_totalresults'] = os.path.join(cls.paths['output'], "evaluation", "total_results.txt")        
        cls.files['output_bestsignalregions'] = os.path.join(cls.paths['output'], "evaluation", "best_signal_regions.txt")
        cls.files['output_result'] = os.path.join(cls.paths['output'], "result.txt")
        
        if cls.flags["zsig"]:
            cls.files['output_result_zsig'] = os.path.join(cls.paths['output'], "result_Zexp.txt")
            cls.files['output_bestsignificanceregions'] = os.path.join(cls.paths['output'], "evaluation", "best_significance_regions.txt")     
        
        cls.files['output_evaluation_event_numbers'] = dict()
        cls.files['output_evaluation_r_limits'] = dict()
        if cls.flags["fullcls"]:
            cls.files['output_evaluation_cl_limits'] = dict()
        cls.files['output_evaluation_likelihood'] = dict()        
        cls.files['output_evaluation_likelihoodRootmethod'] = dict()        
        cls.files['eff_tab'] = dict()
        if cls.flags['zsig']:
            cls.files['output_evaluation_zsig'] = dict()
        if cls.flags["likelihood"]:
            cls.files['output_evaluation_likelihood'] = dict()
        if cls.flags["likelihoodRootmethod"]:
            cls.files['output_evaluation_likelihoodRootmethod'] = dict()
        
        for a in cls.analyses:
            cls.files['output_evaluation_event_numbers'][a] = os.path.join(cls.paths['output'], "evaluation", a+"_event_numbers.txt")
            cls.files['output_evaluation_r_limits'][a] = os.path.join(cls.paths['output'], "evaluation", a+"_r_limits.txt")
            if cls.flags["fullcls"]:
                cls.files['output_evaluation_cl_limits'][a] = os.path.join(cls.paths['output'], "evaluation", a+"_cl_limits.txt")
            cls.files['output_evaluation_likelihood'][a] = os.path.join(cls.paths['output'], "evaluation", a+"_likelihood.txt")
            cls.files['output_evaluation_likelihoodRootmethod'][a] = os.path.join(cls.paths['output'], "evaluation", a+"_likelihoodrm.txt")
            cls.files['eff_tab'][a] = os.path.join(cls.paths['output'], "evaluation", a+"_eff_tab.txt")
            if cls.flags['zsig']:
                cls.files['output_evaluation_zsig'][a] = os.path.join(cls.paths['output'], "evaluation", a+"_zsig.txt")

    @classmethod
    def fill_result_files(cls, analysis):
        """Fills cls.files with files that belong to a certain analysis in a given result directory"""
        
        # Get all files in output folder
        if 'output_analysis' not in cls.paths:
            exit("Error in Info.fill_result_files()")
        rdir = cls.paths['output_analysis']
        signals = [os.path.join(rdir, f) for f in os.listdir(rdir) if "signal" in f and analysis in f]
        #cutflows = [os.path.join(rdir, f) for f in os.listdir(rdir) if "cutflow" in f and analysis in f] Not yet further evaluated
        
        cls.files['results_signal'] = dict()
        # Read the prefix of every file and use it as key for the files dictionaries
        for i in range(len(signals)):
            prefix = os.path.split(signals[i])[1][0:3]
            cls.files['results_signal'][prefix] = signals[i]

    @classmethod
    def prepare_output_directories(cls):
        if os.path.isdir(cls.paths['output']):            
            shutil.rmtree(cls.paths['output'])
                
        os.makedirs(cls.paths['output'])
        os.mkdir(cls.paths['output_mg5'])
        os.mkdir(cls.paths['output_pythia'])
        os.mkdir(cls.paths['output_fritz'])
        os.mkdir(cls.paths['output_delphes'])
        os.mkdir(cls.paths['output_analysis'])
        os.mkdir(cls.paths['output_evaluation'])            
        os.mkdir(cls.paths['output_internal'])            
        #open(cls.files['output_progress'], 'a').close() TODO: Outdated. Not needed any more
        
        
    @classmethod
    def prepare_config(cls):
        randomseed = cls.parameters['randomseed']
        if randomseed!=0:
            name = "Global"
            if not cls.config.has_section(name):
               cls.config.add_section(name)
            cls.config.set(name, 'randomseed', randomseed)
        
    @classmethod
    def make_flags_consistent(cls):
        """ In case there are flags which are dependent on one another """
        if cls.flags["controlregions"]:
            cls.flags["skipevaluation"] = True
           
        if cls.flags["quietmode"]:
            cls.flags["skipparamcheck"] = True
            AdvPrint.mute()

        if cls.flags["skippythia"]:
            cls.flags["write_pythia_events"] = False
            

    @classmethod
    def get_all_checkmate_analyses(cls):
        if 'list_of_analyses' not in cls.files:
            AdvPrint.cerr_exit("Cannot get list of analyses!")
        analysis_list = list()
        groups = dict()
        for group in cls.analysis_groups:
            path = cls.files["list_of_analyses"][group]
            with open(path, "r") as f:
                for line in f:
                    if line.startswith("#") or line.strip() == "":
                        continue
                    analysis = line.split()[0]
                    analysis_list.append(analysis)
                    groups[analysis] = group
        return (analysis_list, groups)
        
    @classmethod
    def get_analysis_parameters(cls, analysis):    
        """ Reads in the _var.j file of a given analysis and returns the parameters
            as a dictionary """
        if 'analysis_settings' not in cls.files or analysis not in cls.files['analysis_settings']:
            print cls.files['analysis_settings']
            AdvPrint.cerr_exit("Cannot find files for reading parameters of analysis "+analysis)
        jfile = open(cls.files['analysis_settings'][analysis], "rb")
        parameters = json.loads(jfile.read())
        jfile.close()
        return parameters

    @classmethod
    def unit(cls, in_unit):
        """ Define everything in fb"""        
        if in_unit.lower() == "fb":
            return 1.
        elif in_unit.lower() == "pb":
            return 1.E3
        elif in_unit.lower() == "nb":
            return 1.E6
        elif in_unit.lower() == "mub":
            return 1.E9
        elif in_unit.lower() == "mb":
            return 1.E12
        elif in_unit.lower() == "b":
            return 1.E15
        else:
            AdvPrint.cerr_exit("Unit "+str(in_unit)+" unknown!")

    @classmethod
    def check_and_absolutize_file(cls, filename, abortOnFail=True):
       """ If the user provides relative files, pythia etc cannot handle them. So try to find the file with the given #filename at the following places
       - #filename
       - (if param.dat provided): #folder_of_given_param.dat/#filename
       - $PWD/#filename
       """
       absolutePath = True
       guesses = list()
       if os.path.isfile(filename):
           return os.path.abspath(filename)
       guesses.append(filename)
       if (len(sys.argv) == 2):
           paramFileDir = os.path.dirname(os.path.abspath(sys.argv[1]))
           guess = os.path.join(paramFileDir, filename)
           if os.path.isfile(guess):
               return guess
           guesses.append(guess)
       cwd = os.getcwd()
       guess = os.path.join(cwd, filename)
       if os.path.isfile(guess):
           return guess3
       guesses.append(guess)
       guesses = list(set(guesses))
       error_output = "Couldn't find file "+filename+". Tried the following possibilities:"
       for guess in guesses:
           error_output += "\n  - "+guess
       if abortOnFail:
           AdvPrint.cerr_exit(error_output)
       else:
           return None
       


def _check_outputexists():
    """
    Test if the user entered something useful for 'outputexists'.

    The allowed values of the outputexists field are:
    * add
    * ask
    * overwrite

    The value of the parameter is treated as case insensitive and is normalized
    to lower case.
    """
    string = Info.parameters['outputexists'].lower()
    Info.parameters['outputexists'] = string
    if string != "add" and string != "overwrite" and string != "ask":
        AdvPrint.cerr_exit(
                "Invalid argument for 'OutputExists': "
                +string
                +"\nAllowed values are: 'add', 'ask' and 'overwrite'")

