import argparse
import ConfigParser
import os
import shutil

from global_functions import *
from organize_paths_and_files import *


""" This file is probably obsolete"""

def get_information_from_file(pfile):
    """File is parsed and parameters are extracted"""
    paths = get_standard_paths()
    files = get_standard_files()
    flags = dict()
    events = dict()
    analyses = list()
    output = AdvPrint()
    
    # Check if the given file does exist.
    if not os.path.isfile(pfile):
        output.cerr_exit("Parameter file '"+pfile+"' does not exist.")        
        
    # Read file.
    Config = ConfigParser.ConfigParser()
    Config.read(pfile)
    
    # Set standard values
    flags['md5'] = False
    flags['fullcl'] = False
    flags['likelihood'] = False
    flags['tempmode'] = False
    flags['quietmode'] = False
    flags['verbosemode'] = False
    flags['skipparamcheck'] = False
    flags['skipdelphes'] = False
    flags['skipevaluation'] = False
    flags['controlregions'] = False
    flags['outputexists'] = "ask"
    flags['randomseed'] = 0
    
    events['raw'] = []
    events['xsects'] = []
    events['xsecterrs'] = []
    events['processes'] = []
    

    # Insist on mandatory section and read in information
    sections = Config.sections()
    if "Mandatory Parameters" not in sections:
        output.cerr_exit("No mandatory section in parameter file!")
    mandatory_options = Config.options("Mandatory Parameters")
    if "name" not in mandatory_options:
        output.cerr_exit("No 'name' option in parameter file!")
    if "analyses" not in mandatory_options:
        output.cerr_exit("No 'analyses' option in parameter file!")
    
    # Read in analyses
    analyses = [a.lstrip().rstrip() for a in Config.get("Mandatory Parameters", "analyses").split(",")]
    sections.remove("Mandatory Parameters")

    # If there should be optional parameters, read them
    if "Optional Parameters" in sections:
        for optional_parameter in Config.options("Optional Parameters"):
            if optional_parameter == "tempmode":
                flags['tempmode'] = Config.getboolean("Optional Parameters", "tempmode")
            elif optional_parameter == "skipparamcheck":
                flags['skipparamcheck'] = Config.getboolean("Optional Parameters", "skipparamcheck")
            elif optional_parameter == "outputexists":
                flags['outputexists'] = Config.get("Optional Parameters", "outputexists")
            elif optional_parameter == "quietmode":
                flags['quietmode'] = Config.getboolean("Optional Parameters", "quietmode")
            elif optional_parameter == "verbosemode":
                flags['verbosemode'] = Config.getboolean("Optional Parameters", "verbosemode")
            elif optional_parameter == "skipdelphes":
                flags['skipdelphes'] = Config.getboolean("Optional Parameters", "skipdelphes")
            elif optional_parameter == "skipevaluation":
                flags['skipevaluation'] = Config.getboolean("Optional Parameters", "skipevaluation")
            elif optional_parameter == "outputdirectory":
                paths['results'] = Config.get("Optional Parameters", "outputdirectory")
            elif optional_parameter == "fullcl":
                flags['fullcl'] = Config.getboolean("Optional Parameters", "fullcl")
            elif optional_parameter == "likelihood":
                flags['likelihood'] = Config.getboolean("Optional Parameters", "likelihood")
            elif optional_parameter == "randomseed":
                flags['randomseed'] = Config.getint("Optional Parameters", "randomseed")
            elif optional_parameter == "controlregions":
                flags['controlregions'] = Config.getboolean("Optional Parameters", "controlregions")
            else:
                output.cerr_exit("Unknown optional parameter '"+optional_parameter+"'")
        sections.remove("Optional Parameters")

    if flags["controlregions"]:
      flags["skipevaluation"] = True
    # Update files and paths with given parameters
    files.update(get_analysis_files(analyses))

    paths.update(get_output_paths(paths['results'], Config.get("Mandatory Parameters", "name").replace(" ", "_"))) # spaces in "name" are replaced by underscores to prevent problems in the file handling
    files.update(get_output_files(paths['results'], Config.get("Mandatory Parameters", "name").replace(" ", "_"), analyses, flags))
    
    # Set up event information. Remaining sections consist of individual processes
    for process in sections:
        # Check and save event filea
        if "events" not in Config.options(process):
            output.cerr_exit("No event(s) given for process '"+process+"' in parameter file!")
        eventfiles = Config.get(process, "events").replace("\n", ",").replace(",,", ",").replace(" ", "").replace("\\", " ").split(",")
        for event in eventfiles:
            if not os.path.isfile(event):
                output.cerr_exit("Event file '"+event+"' does not exist.")
            events['raw'].append(os.path.abspath(event))
            events['processes'].append(process)
            # Read cross section in the form "Num Unit" or "Num*Unit"
            if "xsect" not in Config.options(process):
                output.cerr_exit("No 'XSect' line for process '"+process+"' in parameter file!")
            xsect = Config.get(process, "xsect")
            if "*" in xsect:
              xsect_split = xsect.split("*")
            else:
              xsect_split = xsect.split(" ")              
            if len(xsect_split) == 1:   # If "0", add random unit
              xsect_split.append("FB")
            xsect_split[1] = xsect_split[1].upper() # Make sure unit is in uppercase
            events['xsects'].append(xsect_split)
            
            # Same as above, just for the error
            xsecterr = Config.get(process, "xsecterr")  
            if "*" in xsecterr:
              xsecterr_split = xsecterr.split("*")
            else:
              xsecterr_split = xsecterr.split(" ")          
            # If relative error is given, transform into absolute error"
            if xsecterr_split[1] == "%":
              xsecterr_split[0] = str(float(xsect_split[0])*float(xsecterr_split[0].rstrip())/100.)
              xsecterr_split[1] = xsect_split[1]             
            if len(xsecterr_split) == 1:
              xsecterr_split.append("FB")
            xsecterr_split[1] = xsecterr_split[1].upper() # Make sure unit is in uppercase
            events['xsecterrs'].append(xsecterr_split)
                                    
    return (analyses, events, files, flags, output, paths)
    
    
def get_information_from_parameters():
    """Input parameters are parsed and organised into global objects for further processing"""
    paths = get_standard_paths()
    files = get_standard_files()
    flags = dict()
    events = dict()
    analyses = list()
    output = AdvPrint()
    # Define all allowed parser options.
    parser = argparse.ArgumentParser(description='CheckMATE takes an arbitrary set of eventfiles (in .hepmc, .hep or lhe format), processes them with Delphes and analyzes the output with a particular analysis to be chosen from a given subset. It is then tested how well the given event files is in agreement with the current results of the corresponding collider experiment in order to get an estimate, how strong this model is already excluded by experimental data..')
    parser.add_argument('-n', '--name', dest='name', required=True, help='name for the run')   
    parser.add_argument('events', metavar='eventfile', type=str, nargs='*', help='List of event files to be analysed (.hepmc, .hep, lhe)')
    parser.add_argument('-a', '--analysis', dest='analysis', required=True, help='Analysis to be applied on the processed event files')
    parser.add_argument('-p', '--processes', dest='processes', default="", type=str, help="Identifier of each event to which process it belongs. Events from equal/different processes are averaged/summed.")
    parser.add_argument('-xs', '--xsects', dest='xsects', required=True, type=str, help="Cross section, either one global for all events or for each event, separated by ;. Example format: 1.73 FB. ")
    parser.add_argument('-xse', '--xsecterrs', dest='xsecterrs', required=True, type=str, help="Cross section errors, either one global for all events or for each event, separated by ;. Example format: 0.01*FB or 10.4 %%. ")
    parser.add_argument('-cl', '--full-cl', dest='fullcl', action='store_true', help="Evaluate full CLs to the evaluated number of signal events (instead of just comparing to 95 percent limit).")
    parser.add_argument('-likeli', '--likelihood', dest='likelihood', action='store_true', help="Evaluate likelihood for each signal region (and sum all signal regions).")
    parser.add_argument('-od', '--outdir', dest='odir', default=paths['results'], help='Directory where the results should be saved (default: '+paths['results']+').')
    parser.add_argument('-oe', '--output-exists', dest='output_exists', default="ask", type=str, help="What to do if output already exists. overwrite will delete existing output and overwrite it with the new results. add will add the current results to the old ones. In any other case, a prompt will ask you.")   
    parser.add_argument('-q', '--quiet', dest='quiet', action='store_true', help='Suppresses all output (sets --force automatially).')  
    parser.add_argument('-v', '--verbose', dest='verbose', action='store_true', help='Shows delphes and analysis output on stdout.')  
    parser.add_argument('-sp', '--skip-paramcheck', dest='force', action='store_true', help="Skip startup parameter check.")               
    parser.add_argument('-sd', '--skip-delphes', dest='skipdelphes', action='store_true', help='Skips Delphes and runs analysis. Assumes that the ROOT files have been created in a previous run.')              
    parser.add_argument('-t', '--temporary', dest='temp', action='store_true', help='Temporary mode; Delphes output is deleted after analysis step.')
    parser.add_argument('-se', '--skip-evaluation', dest='skipevaluation', action='store_true', help='Skips evaluation step.')              
    parser.add_argument('-cr', '--control-regions', dest='controlregions', action='store_true', help='Analyses control regions instead of signal regions. Sets -se automatically.')
    parser.add_argument('-rs', '--random-seed', dest='randomseed', type=int, default=0, help='Chooses fixed seed for random number generator. 0 chooses a random seed automatically.')                            
    
    # Parse arguments and set return parameters
    args = parser.parse_args()
    
    flags['fullcl'] = False
    flags['likelihood'] = False
    flags['tempmode'] = args.temp
    flags['skipparamcheck'] = args.force
    flags['randomseed'] = args.randomseed
    flags['skipdelphes'] = False
    flags['skipevaluation'] = False
    flags["quietmode"] = False
    flags["verbosemode"] = False
    flags["controlregions"] = False
    if args.quiet:
        flags["quietmode"] = True
    if args.verbose:
        flags['verbosemode'] = True
    if args.skipdelphes:
      flags['skipdelphes'] = True
    if args.skipevaluation:
      flags['skipevaluation'] = True
    flags["outputexists"] = "ask"
    if args.output_exists == "overwrite":
      flags["outputexists"] = "overwrite"
    elif args.output_exists == "add":
      flags["outputexists"] = "add"
    if args.fullcl:
      flags["fullcl"] = True
    if args.likelihood:
      flags["likelihood"] = True
    if args.controlregions:
      flags["controlregions"] = True
      flags["skipevaluation"] = True
      
    
    # If analyses = "atlas", "cms" or "all", manually add all these to the list
    if args.analysis == "atlas":
      f = open(files['list_of_analyses'])
      for line in f:
        if line.startswith("atlas"):
          analyses.append(line.split()[0])
    elif args.analysis == "cms":
      f = open(files['list_of_analyses'])
      for line in f:
        if line.startswith("cms"):
          analyses.append(line.split()[0])          
    elif args.analysis == "all":
      f = open(files['list_of_analyses'])
      for line in f:
        if not line.startswith("#"):
          analyses.append(line.split()[0])
    else:
      analyses = [a.lstrip().rstrip() for a in args.analysis.split(",")]
      
    files.update(get_output_files(args.odir, args.name.replace(" ", "_"), analyses, flags))
    paths.update(get_output_paths(args.odir, args.name.replace(" ", "_")))
    
    files.update(get_analysis_files(analyses))
    
    events['raw'] = []
    events['xsects'] = []
    events['xsecterrs'] = []
    events['processes'] = []
    # Set up eventwise information.
    for i in range(len(args.events)):
        event = args.events[i]       
        # Abort if the event file does not exist.
        if not os.path.isfile(event):
            output.cerr_exit("Event file '"+event+"' does not exist.")
        events['raw'].append(os.path.abspath(event))
        
        # Cross sections are separated by semicolons.
        xsects = args.xsects.split(";")
        xsect = ""
        xsecterrs = args.xsecterrs.split(";")
        xsecterr = ""
        # If only one cross section is given, it is universal for all events,
        # otherwise one has to give as many cross sections as event files.
        if len(xsects) == len(args.events):
            xsect = xsects[i]
            if len(xsects) == len(xsecterrs):
              xsecterr = xsecterrs[i]
            else:
              output.cerr_exit("You have to give N cross section errors!")
        else:
            output.cerr_exit("You have to give N cross sections (N = number of event files)")
        
        if "*" in xsect:
          xsect_split = xsect.split("*")
        else:
          xsect_split = xsect.split(" ")             
        if len(xsect_split) == 1:   # If "0", add random unit
          xsect_split.append("FB")                        
        xsect_split[1] = xsect_split[1].upper() # Make sure unit is in uppercase
        events['xsects'].append(xsect_split)
        
        if "*" in xsecterr:
          xsecterr_split = xsecterr.split("*")
        else:
          xsecterr_split = xsecterr.split(" ")          
        # If relative error is given, transform into absolute error"
        if xsecterr_split[1] == "%":
          xsecterr_split[0] = str(float(xsect_split[0])*float(xsecterr_split[0].rstrip())/100.)
          xsecterr_split[1] = xsect_split[1]
        xsecterr_split[1] = xsecterr_split[1].upper() # Make sure unit is in uppercase
        events['xsecterrs'].append(xsecterr_split)
        
        # If process identifiers are given, they are separated by semicolons and
        # there need to be exactly as many as there are event files.
        processes = args.processes.split(";")
        process = ""
        if processes[0] == "":
            process = ""
        elif len(processes) == len(args.events):
            process = processes[i]
        else:
            output.cerr_exit("You have to give N process identifiers (N = number of event files)")
        events['processes'].append(process)
        check_agreement_of_identifiers_and_crosssections(events)
       
    return (analyses, events, files, flags, output, paths)


def prepare_run(analyses, events, files, flags, output, paths):
    """ Checks parameters and prepares output directories"""
    # Check environmental variables.
    #if not os.environ.get('ROOTSYS'):
    #    output.cerr_exit("$ROOTSYS does not exist. Please run 'thisroot.sh' or 'thisroot.csh' from your root installation.") 
    #if not "Delphes" in os.environ.get('LD_LIBRARY_PATH'):
    #    output.cerr_exit("$LD_LIBRARY_PATH does not include Delphes. Please run 'source delphesenv.sh' or 'source delphesenv.csh'.") 

    # Setup analysis configuration and check whether it is valid.
    
    analysis_info = dict()
    for a in analyses:
        analysis_info[a] = get_analysis_info(a)

    if flags["quietmode"]:
      flags["skipparamcheck"] = True
      output.mute()
    # Print parameters.
    output.cout("""
   ____ _               _    __  __    _  _____ _____ 
  / ___| |__   ___  ___| | _|  \/  |  / \|_   _| ____|
 | |   | '_ \ / _ \/ __| |/ / |\/| | / _ \ | | |  _|  
 | |___| | | |  __/ (__|   <| |  | |/ ___ \| | | |___ 
  \____|_| |_|\___|\___|_|\_\_|  |_/_/   \_\_| |_____|
  """)
    output.cout("The following settings are used:")
    output.cout("Eventfiles: "+str(len(events['raw'])))
    for i in range(len(events['raw'])):
        output.cout("\t"+events['raw'][i]+" (process: "+events['processes'][i]+", xsect: "+events['xsects'][i][0]+" "+events['xsects'][i][1]+" +- "+events['xsecterrs'][i][0]+" "+events['xsecterrs'][i][1]+")")
    output.cout("Analyses: ")
    for a in analyses:
        output.cout("\t"+a+" ("+analysis_info[a]+")")
    output.cout("Output Directories: ")
    output.cout("\t"+paths['output'])
    output.cout("Additional Settings: ")
    if flags['skipdelphes']:
        output.cout("\t - No Delphes step, existing Delphes files in output folder will be used for analysis")
    if flags['skipevaluation']:
        output.cout("\t - No evaluation step")
    if flags['fullcl']:
        output.cout("\t - Confidence of signal estimate will be explicitly calculated")
    if flags['likelihood']:
        output.cout("\t - Likelihood will be calculated for each signal region")
    if flags["outputexists"] == "overwrite":
        output.cout("\t - Old results will be deleted")
    if flags["outputexists"] == "add":
        output.cout("\t - New results will be added to old ones")
    if flags["controlregions"]:
        output.cout("\t - Analysing control regions")
    if flags["tempmode"]:
        output.cout("\t - Delphes files are deleted after the analysis step")
    if flags["randomseed"] != 0:
        output.cout("\t - Fixed random seed of "+str(flags["randomseed"]))
    # Let user check correctness of parameters, unless in force-mode.
    if not flags['skipparamcheck']:
        while True:
            c = raw_input("Is this correct? (y/n) ")
            if c == "y": 
                break
            elif c == "n": 
                exit(1)
                
        output.cout("")

    # If there are events to be analysed and results folder already exists, decide what to do.
    if len(events['raw']) != 0:
      if not flags["skipdelphes"]:
        if os.path.isdir(paths['output']):
            if flags["outputexists"] == "overwrite":                    
                shutil.rmtree(paths['output'])
                os.mkdir(paths['output'])
                os.mkdir(paths['output_delphes'])
                os.mkdir(paths['output_analysis'])
                os.mkdir(paths['output_evaluation'])
            elif flags["outputexists"] != "add":
                output.unmute()
                output.cout("Directory '%s' already exists." % paths['output'])
                if flags["quietmode"]:
                  output.mute()
                
                while True:
                    c = raw_input("Choose: (o)verwrite, (a)dd to existing results, (s)top\n")
                    if c == "o":
                        shutil.rmtree(paths['output'])
                        os.mkdir(paths['output'])
                        os.mkdir(paths['output_delphes'])
                        os.mkdir(paths['output_analysis'])
                        os.mkdir(paths['output_evaluation'])
                        break
                    elif c == "a":
                        shutil.rmtree(paths["output_evaluation"])
                        os.mkdir(paths['output_evaluation'])
                        break
                    elif c == "s":
                        exit(1)
        # Otherwise create all necessary directories.
        else:
            os.makedirs(paths['output'])
            open(files['output_progress'], 'a').close()
            os.makedirs(paths['output_analysis'])
            os.makedirs(paths['output_delphes'])  
            os.makedirs(paths['output_evaluation'])  
      else:
        # In skipdelphes mode, delete everything in the output except for the delphes folder            
        if not os.path.isdir(paths['output']):
          output.cerr_exit("You can only skip delphes if you give an output folder with already existing data from a previous run!")
        output.cout("You skipped Delphes. Old output (except for the Delphes ROOT files) will be reset.")
        shutil.rmtree(paths["output_analysis"])
        os.mkdir(paths["output_analysis"])
        shutil.rmtree(paths["output_evaluation"])
        os.mkdir(paths["output_evaluation"])
        for a in analyses:
            open(files["output_evaluation_event_numbers"][a], "w")            
            open(files["output_evaluation_r_limits"][a], "w")            
            open(files["output_evaluation_cl_limits"][a], "w")            
        open(files["output_result"], "w")
        open(files["output_progress"], "w")
            
    return
