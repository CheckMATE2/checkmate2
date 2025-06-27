from __future__ import division

from builtins import str
from past.utils import old_div
import os, sys, shutil
from math import sqrt
from copy import deepcopy
from resultcollector import ResultCollector
from advprint import AdvPrint
from info import Info


class Events(dict):
    """ This class is supposed to work as an abstract base class! Don't use instances from it """
    name = '' # filename if real / process name if pythia/mg5
    identifier = '' # individual identifier    
    
    processed = False
    analysis_signal_files = dict() # for each analysis a, the respective _signal.dat file
    analysis_cutflow_files = dict() # for each analysis a, the respective _signal.dat file
    result_output_file = ""
    maxEvents = -1 # if positive, events procession stops at maxevents

    result_output_file = ""
    fritz_config_file = ""

    def __init__(self, name):
        # Note that the below condition should fail if this __ini__ condition is called from a derived class, as self.__class__.__name__ should be the daughter class name!
        if self.__class__.__name__ == "Events":
            AdvPrint.cerr_exit("Internal error: Instance of abstract base class 'Events' created!")
        self.name = name
        Info.book_events(self) # sets identifier
        
        self.processed = False
        self.analysis_result_files = dict()
        self.analysis_signal_files = dict()
        self.analysis_cutflow_files = dict()
        self.maxEvents = -1

        self.result_output_file = ""
        self.fritz_config_file = ""

    def printInfo(self):
        pass # purely virtual

    def check(self):
        pass
    
    def prepare(self):
        self.result_output_file = os.path.join(Info.paths["output_evaluation"], self.identifier+"_eventsResults.txt")
        self.fritz_config_file = os.path.join(Info.paths["output_fritz"], self.identifier+".ini")
     
    def get_resultCollectors(self):
        resultCollectors = dict() # list of all collectors of all analyses and all signal regions
        resultCollector = ResultCollector(self.identifier, "", "") # base collector object which we will just edit and copy
        
        for analysis in Info.analyses:
            # check if results file exists
            if not os.path.isfile(self.analysis_signal_files[analysis]):
                AdvPrint.cerr_exit("\t events::get_resultCollector() \n"
                                "\t Required analysis result file does not exist: \n "
                                "\t\t"+self.analysis_signal_files[analysis]+"\n"
                                "\t It is very likely that something went wrong in the delphes and/or the analysis step. \n"
                                "\t Please check \n "
                                "\t \t "+Info.files['delphes_log']+" \n   "
                                "\t \t "+Info.files['analysis_log']+"* \n  "
                                "\t for error messages and, should you not be able to fix them yourself, contact the authors under  \n"
                                "\t \t checkmate@projects.hepforge.org")
            
            # setup resultCollector object
            resultCollector.analysis = analysis
            resultCollectors[analysis] = dict()
            signal_regions = Info.get_analysis_parameters(analysis)["signal_regions"]

            # Read result file
            f = open(self.analysis_signal_files[analysis], "r")
            for line in f:
                # Ignore empty or commented lines
                line = line.rstrip()
                if line == "" or line[0] == "#":
                    continue
                
                # Read file:
                line = AdvPrint.remove_extra_spaces(line)
                tokens = [t for t in line.split("  ") if t != ""]
                # First, read information on total events number
                if tokens[0] == "MCEvents:":
                    resultCollector.total_mcevents = float(tokens[1])
                elif tokens[0] == " SumOfWeights:":
                    resultCollector.total_sumofweights = float(tokens[1])
                elif tokens[0] == " SumOfWeights2:":
                    resultCollector.total_sumofweights2 = float(tokens[1])
                elif tokens[0] == " NormEvents:":
                    resultCollector.total_normevents = float(tokens[1])
                elif tokens[0] == "XSect:":
                    xsect = float(tokens[1].split(" ")[0])
                elif tokens[0] == " Error:":      
                    xsecterr = float(tokens[1].split(" ")[0])
                else:
                    # SR  Sum_W  Sum_W2  Acc  N_Norm
                    for sr in signal_regions:                        
                        if tokens[0].startswith(sr):
                            resultCollector.sr = sr
                            # Read number of events
                            resultCollector.signal_sumofweights = float(tokens[1])
                            resultCollector.signal_sumofweights2 = float(tokens[2])
                            resultCollector.signal_normevents = float(tokens[4])
                            
                            # Calculate errors
                            if resultCollector.signal_sumofweights > 0:
                                resultCollector.signal_err_stat = old_div(resultCollector.signal_normevents*sqrt(resultCollector.signal_sumofweights2),resultCollector.signal_sumofweights)
                                resultCollector.signal_err_sys = old_div(resultCollector.signal_normevents*xsecterr,xsect)
                                resultCollector.signal_err_tot = sqrt(resultCollector.signal_err_stat**2+resultCollector.signal_err_sys**2)
                            else:
                                resultCollector.signal_err_stat = 0
                                resultCollector.signal_err_sys = 0
                                resultCollector.signal_err_tot = 0
                                
                                
                            # put copy of resultCollector in collector dict
                            resultCollectors[analysis][sr] = deepcopy(resultCollector)
            f.close()
            
        
        # Write events file, if wanted
        if Info.parameters["EventResultFileColumns"] != []: 
            AdvPrint.mute()        
            AdvPrint.set_cout_file(self.result_output_file, True)
            for col in Info.parameters["EventResultFileColumns"]:
                AdvPrint.cout(col+"  ", "nlb")
            AdvPrint.cout("")
            for a in sorted(resultCollectors.keys()):
                for sr in sorted(resultCollectors[a].keys()):
                    AdvPrint.cout(resultCollectors[a][sr].line_from_data(Info.parameters["EventResultFileColumns"]))
            AdvPrint.format_columnated_file(self.result_output_file)
            AdvPrint.set_cout_file("#None")
            AdvPrint.unmute()
            
        return resultCollectors

class PhysicalEvents(Events):
    """ This metaclass combines events samples which exist as a physical file, like .hep, .hepmc and .lhe """
    full_filename = ""
    def __init__(self, name, full_filename):
        if self.__class__.__name__ == "PhysicalEvents":
            AdvPrint.cerr_exit("Internal error: Instance of abstract base class 'PhysicalEvents' created!")
        Events.__init__(self, name)
        self.full_filename = full_filename.strip()

    def printInfo(self):
        pass # purely virtual
    
    def check(self):
        Events.check(self)
    
    def prepare(self):
        Events.prepare(self)
    
    def prepareFritzInputFile(self, fritzconfig):
        secname = "EventFile: "+self.identifier
        fritzconfig.add_section(secname)
        fritzconfig.set(secname, "file", self.full_filename)
        return fritzconfig, secname
        
class HEPEvents(PhysicalEvents):
    def printInfo(self):
        AdvPrint.cout("\t\t STDHEP events")
        AdvPrint.cout("\t\t\t - internal identifier:  '"+self.identifier+"'")
        AdvPrint.cout("\t\t\t - path to eventfile: "+self.full_filename)
        if self.maxEvents > 0:
            AdvPrint.cout("\t\t\t - at most "+str(self.maxEvents)+" events are analysed")
        if self.processed:
           AdvPrint.cout("\t\t\t   [Events already processed, results from earlier run are used!]")
            
class HepMCEvents(PhysicalEvents):
    def printInfo(self):
        AdvPrint.cout("\t\t HepMC events")
        AdvPrint.cout("\t\t\t - internal identifier:  '"+self.identifier+"'")
        AdvPrint.cout("\t\t\t - path to eventfile: "+self.full_filename)
        if self.maxEvents > 0:
            AdvPrint.cout("\t\t\t - at most "+str(self.maxEvents)+" events are analysed")
        if self.processed:
           AdvPrint.cout("\t\t\t   [Events already processed, results from earlier run are used!]")

class DelphesEvents(PhysicalEvents):
    def printInfo(self):
        AdvPrint.cout("\t\t ROOT events")
        AdvPrint.cout("\t\t\t - internal identifier: '"+self.identifier+"'")
        AdvPrint.cout("\t\t\t - path to eventfile: "+self.full_filename)
        if self.maxEvents > 0:
            AdvPrint.cout("\t\t\t - at most "+str(self.maxEvents)+" events are analysed")
        if self.processed:
           AdvPrint.cout("\t\t\t   [Events already processed, results from earlier run are used!]")

class Pythia8Events(Events):
    py8_infile = '' # full path to pythia .in file
    processString = '' #optional: might contain explicit "p p > SUSY SUSY" command
    
    def __init__(self, name):
        Events.__init__(self, name)
        self.py8_infile = ""
        self.processString = ""

    def set_inFile(self, inFile):
        self.py8_infile = Info.check_and_absolutize_file(inFile)
        
    def set_processString(self, processString):
        self.processString = processString

    def printInfo(self):
        AdvPrint.cout("\t\t Pythia8 Events")
        AdvPrint.cout("\t\t\t - internal identifier:  '"+self.identifier+"'")
        if self.py8_infile != "":
            AdvPrint.cout("\t\t\t - .in settings file: "+self.py8_infile)
        if self.processString != "":
            AdvPrint.cout("\t\t\t - simplified SUSY process: "+self.processString)
        if self.maxEvents > 0:
            AdvPrint.cout("\t\t\t - at most "+str(self.maxEvents)+" events are generated and analysed")
        if self.processed:
           AdvPrint.cout("\t\t\t   [Events already processed, results from earlier run are used!]")
    
    def check(self):
        if self.py8_infile == '' and self.processString == '':
            AdvPrint.cerr_exit("ERROR in Pythia8Events-object '"+self.name+"':\n\t Neither a .in file nor a process string was given!")
    
    def prepare(self):
        Events.prepare(self)
        if self.processString != '':
            self.setup_pythia8_from_process_string()
        
    def prepareFritzInputFile(self, fritzconfig):
        secname = "PythiaHandler: "+self.identifier
        fritzconfig.add_section(secname)
        logFile = os.path.join(Info.paths["output_pythia"], "pythia_"+self.identifier+".log")
        fritzconfig.set(secname, "pythiapath", Info.paths["output_pythia"])
        fritzconfig.set(secname, "logFile", logFile)
        fritzconfig.set(secname, "useMG5","false")
        fritzconfig.set(secname, "settings", self.py8_infile)
                 
        if Info.flags['write_pythia_events']:
            outputFile = os.path.join(
                    Info.paths["output_pythia"],
                    self.identifier+".hepmc"
                    )
            fritzconfig.set(secname, "outputFile", outputFile)
        return fritzconfig, secname
    
    def setup_pythia8_from_process_string(self):
        """ Generates Pythia8 input card """
        out_path = Info.paths['output_pythia']
        procnum = len(Info.files['pythia_cards'])
        filename = self.name+"card_"+str(procnum)+".in"
        fpath = os.path.join(out_path,filename)
        f = open(fpath,'w')
        
        # Part 2: Generate cards based on process string
        part = self.processString.split('>')[1].strip()
        proclist = list()
        if part == 'go go':
            proclist.append('SUSY:gg2gluinogluino  = on\n')
            proclist.append('SUSY:qqbar2gluinogluino  = on\n')
        elif part == 'go sq':
            proclist.append('SUSY:qg2squarkgluino = on\n')
            proclist.append('SUSY:idVecA = 1000001,1000002,1000003,1000004,2000001,2000002,2000003,2000004')
        elif part == 'sq sq~':
            proclist.append('SUSY:gg2squarkantisquark = on\n')
            proclist.append('SUSY:qqbar2squarkantisquark = on\n')
            proclist.append('SUSY:idVecA = 1000001,1000002,1000003,1000004,2000001,2000002,2000003,2000004')
        elif part == 't1 t1~':
            proclist.append('SUSY:gg2squarkantisquark = on\n')
            proclist.append('SUSY:qqbar2squarkantisquark = on\n')
            proclist.append('SUSY:idA = 1000006\n')
        elif part == '3gen':
            proclist.append('SUSY:gg2squarkantisquark = on\n')
            proclist.append('SUSY:qqbar2squarkantisquark = on\n')
            proclist.append('SUSY:qq2squarksquark = on\n')
            proclist.append('SUSY:idVecA = 1000005,100006,2000005,2000006\n')
        elif part == 'sq sq':
            proclist.append('SUSY:qq2squarksquark = on\n')
            proclist.append('SUSY:idVecA = 1000001,1000002,1000003,1000004,2000001,2000002,2000003,2000004\n')
        elif part.lower() == "ewsusy":
            proclist.append('SUSY:qqbar2chi0chi0  = on\n')
            proclist.append('SUSY:qqbar2chi+-chi0 = on\n')
            proclist.append('SUSY:qqbar2chi+chi-  = on\n')    
        elif part.lower() == "colsusy":
            proclist.append('SUSY:gg2gluinogluino  = on\n')
            proclist.append('SUSY:qqbar2gluinogluino  = on\n')
            proclist.append('SUSY:qg2squarkgluino = on\n')
            proclist.append('SUSY:gg2squarkantisquark = on\n')
            proclist.append('SUSY:qqbar2squarkantisquark = on\n')
            proclist.append('SUSY:qq2squarksquark = on\n')
            proclist.append('SUSY:idVecA = 1000001,1000002,1000003,1000004,1000005,100006,2000001,2000002,2000003,2000004,2000005,2000006\n')
        elif part.lower() == "allsusy":
            proclist.append('SUSY:all  = on\n')
        else:
            AdvPrint.cerr_exit("\t Process:genPy8card():: Cannot understand process " + part)

        # Write Pythia cards
        ecm_str = "Beams:eCM = 8000.\n"
        if( float(Info.parameters["ecm"]) == 7.0): 
            ecm_str = "Beams:eCM = 7000.\n"
        elif ( float(Info.parameters["ecm"]) == 13.0): 
            ecm_str = "Beams:eCM = 13000.\n"
        elif ( float(Info.parameters["ecm"]) == 14.0): 
            ecm_str = "Beams:eCM = 14000.\n"

        if len(proclist) == 0:
            AdvPrint.cerr_exit("No processes found")
        default = open(Info.files['pythia_settings_template'],'r')
        slhafile = Info.files['slha']

        for line in default:
            f.write(line)
        f.write(ecm_str + '\n' )
        if len(slhafile) > 0 :
            f.write('SLHA:file = ')
            f.write(slhafile + '\n')
        else:
            AdvPrint.cerr_exit("\t Process:genPy8card():: No SLHA file found")
        for item in proclist:            
            f.write(item)
        default.close()
        
        # if no maximal number set, set number of generated events to 5000
        if self.maxEvents == -1:
            AdvPrint.cout("\t "+self.name+":genPy8card(): Setting number of to-be-generated events to 5000. Use --maxevents parameter to change this behaviour.")
            self.maxEvents = 5000
        
        f.close()
        Info.files['pythia_cards'].append(fpath)
        self.py8_infile = fpath
        

class LHEEvents(Pythia8Events):
    # an lhe events object is a list of physical files which requires the functionalities of a pythia8events object
    full_filenames = list()
    py8aux = ""
    
    def __init__(self, name, full_filenames, py8aux = ""):
        Events.__init__(self, name)
        self.full_filenames = full_filenames
        self.py8aux = py8aux
        
    def check(self):
        pass # nothing to check
        
    def prepare(self):
        Pythia8Events.prepare(self)
        self.setup_pythia8_for_lhe_showering()
        
    def setup_pythia8_for_lhe_showering(self):        
        out_path = Info.paths['output_pythia']
        procnum = len(Info.files['pythia_cards'])
        filename = self.name+"card_"+str(procnum)+".in"
        fpath = os.path.join(out_path,filename)
        
        if self.py8aux != "":
            shutil.copy(Info.check_and_absolutize_file(self.py8aux), fpath)
        else:
            with open(fpath,'w') as f:            
                # generate LHE showering card with hepmc output
                with open(Info.files['pythia_lhe_template'],'r') as default:
                    for line in default:
                        f.write(line)
                    if len(Info.files["slha"]) > 0 :
                        f.write('SLHA:file = ')
                        f.write(Info.files["slha"] + '\n')

                    iRun = 0
                    run_str = list()
                    for lhe_filename in self.full_filenames:
                        # Check file exists, else skip file
                        if not os.path.isfile(lhe_filename):
                            AdvPrint.cerr("\t Process:genPy8card():: File not readable:"+lhe_filename)
                            continue
                        iRun+=1
                        run_str.append("Main:subrun = "+str(iRun)+"\n")
                        if iRun == 1:
                            run_str.append("Beams:frameType = 4\n")
                        else:
                            run_str.append("Beams:newLHEFsameInit = on\n")
                        run_str.append("Beams:LHEF = "+lhe_filename+"\n\n")

                    # Exit if no valid files
                    if iRun < 1:
                        AdvPrint.cerr_exit("\t Process:genPy8card():: No valid LHE files found")

                    # Write out all subruns
                    if iRun > 1:
                        subrun_str = "Main:numberOfSubruns = "+str(iRun)
                        f.write( subrun_str + '\n\n' )
                        for line in run_str:
                            f.write(line)
                    else:
                        f.write("Beams:frameType = 4\n")
                        f.write("Beams:LHEF = "+lhe_filename+"\n\n")
                    default.close()
            
        Info.files['pythia_cards'].append(fpath)
        self.py8_infile = fpath
    
    def printInfo(self):
        AdvPrint.cout("\t\tLHE Events")
        AdvPrint.cout("\t\t\t - internal identifier: '"+self.identifier+"'")
        counter = 1
        for filename in self.full_filenames:
            if len(self.full_filenames) == 1:
                AdvPrint.cout("\t\t\t - path to .lhe file: "+filename)
            else:
                AdvPrint.cout("\t\t\t - path to .lhe file #"+str(counter)+": "+filename)
            counter += 1        
        if self.py8_infile != "":
            AdvPrint.cout("\t\t\t - Pythia8 .in settings file for showering: "+self.py8_infile)
        if self.py8aux != "":
            AdvPrint.cout("\t\t\t - Pythia8 auxiliary settings file for showering: "+self.py8aux)            
        if self.maxEvents > 0:
            AdvPrint.cout("\t\t\t - at most "+str(self.maxEvents)+" events are generated/analysed")
        if self.processed:
           AdvPrint.cout("\t\t\t   [Events already processed, results from earlier run are used!]")
    
        
    def prepareFritzInputFile(self, config):    
        return Pythia8Events.prepareFritzInputFile(self, config)
    
    """ Daniel: This function was not needed in the end, but maybe the syntax proves useful at some point
    def get_xs(self):
        # important for LHE xs killer mode
        if len(self.full_filenames) == 0:
            AdvPrint.cerr_exit("Error in LHEEvents::get_xs - LHEEvents does not contain actual files!")
        if not os.path.isfile(self.full_filenames[0]):
            AdvPrint.cerr_exit("Error in LHEEvents::get_xs - file "+self.full_filenames[0]+" does not exist!")
        
        # the xs is hidden in the <init> block of an lhe file
        with open(self.full_filenames[0], "r") as f:
            init = False
            tokens = []
            for line in f:
                if init:
                    tokens = tokens + line.split()
                if "<init>" in line:
                    init = True
                if "</init>" in line:
                    init = False
                break
        if len(tokens) < 11:
            AdvPrint.cerr_exit("Error in LHEEvents::get_xs - init block in file "+self.full_filenames[0]+" does not exist or does not contain enough entries!")
        return float(tokens[10])
    """
            
            
class MG5Events(Pythia8Events):
    mg5_cards = {'run': "", 'param': "", 'proc': "", 'config': "", 'pythia8': ""}
    commandstring = "" # optional: can contain what is inside the proc card
    
    have_xsth = False # cross section provided by user?
    xsth = 0.0 # Cross section
    xsth_unit = 'pb' # Unit for cross section, pb or fb
    
    def __init__(self, name):
        Pythia8Events.__init__(self, name)
        self.mg5_cards = {'run': "", 'param': "", 'proc': "", 'config': "", 'pythia8': ""}
        self.commandstring = ""        
        self.have_xsth = False 
        self.xsth = 0.0 
        self.xsth_unit = 'pb' 
    
    def set_runcard(self, runcard):
        self.mg5_cards["run"] = runcard
        
    def set_paramcard(self, paramcard):
        self.mg5_cards["param"] = paramcard
        
    def set_proccard(self, proccard):
        self.mg5_cards["proc"] = proccard

    def set_configcard(self, configcard):
        self.mg5_cards["config"] = configcard
        
    def set_py8card(self, py8card):
        self.mg5_cards["pythia8"] = py8card        
        
    def set_commandstring(self, commandstring):
        self.commandstring = commandstring
    
    def set_xsthr(self, value, unit):
        self.have_xsth = True
        self.xsth = value
        self.xsth_unit = unit
        
        
    def check(self):
        if self.mg5_cards["proc"] == "" and self.commandstring == "":
            AdvPrint.cerr_exit("Error in MG5Events object "+self.name+":\n\t Neither a proc card nor a command string was provided!")
        
    def prepare(self):
        Pythia8Events.prepare(self)
        if self.commandstring != "":
            self.mg5_cards["proc"] = os.path.join(Info.paths["output_mg5"], self.identifier+"_proc_card.dat")
            with open(self.mg5_cards["proc"], "w") as f:
                f.write(self.commandstring)
        if self.mg5_cards["param"] == "" and Info.files["slha"] != "":
            self.mg5_cards["param"] = Info.files["slha"]
        if self.py8_infile == "":
            self.py8_infile = os.path.join(Info.paths["output_pythia"], self.identifier+"_showercard.in")
            if self.mg5_cards["pythia8"] != "":  # use user provided showering card
                shutil.copy(self.mg5_cards["pythia8"], self.py8_infile)
            else: # use standard showering card    
                with open(self.py8_infile, "w") as f:
                    with open(Info.files['pythia_mg5minimal_template'], "r") as g:
                        for line in g:
                            f.write(line)
                    f.write("SLHA:file = "+self.mg5_cards["param"]+"\n")
        if self.mg5_cards["run"] == "":
            # copy template and fill rlevant information
            self.mg5_cards["run"] = os.path.join(Info.paths["output_mg5"], self.identifier+"_run_card.dat")
            with open(self.mg5_cards["run"], "w") as f_out:
                with open(Info.files["mg5_run_template"], "r") as f_in:
                    for line in f_in:
                        ecmhalf = str(float(Info.parameters["ecm"])*1000./2.)
                        if self.maxEvents == -1:
                            AdvPrint.cout("\t "+self.name+":prepare(): Setting number of to-be-generated events to 5000. Use --maxevents parameter to change this behaviour.")
                            self.maxEvents = 5000
                        nevents = str(self.maxEvents)
                        seed = str(Info.parameters["randomseed"])
                        line = line.replace("@ecmhalf@", ecmhalf).replace("@nevents@", nevents).replace("@seed@", seed)
                        f_out.write(line)
        if self.mg5_cards["config"] == "":
            # copy template and fill rlevant information
            self.mg5_cards["config"] = os.path.join(Info.paths["output_mg5"], self.identifier+"_me5_configuration.txt")
            with open(self.mg5_cards["config"], "w") as f:
                with open(Info.files["me5_configuration_template"], "r") as g:
                    for line in g:
                        f.write(line)
        
    def printInfo(self):
        AdvPrint.cout("\t\tMG5_aMC@NLO Events")
        AdvPrint.cout("\t\t\t - internal identifier:  '"+self.identifier+"'")
    
        if self.mg5_cards["proc"] != "":        
            AdvPrint.cout("\t\t\t - proc_card: "+self.mg5_cards["proc"])
        
        if self.commandstring != "":
            AdvPrint.cout("\t\t\t - command: "+self.commandstring.replace("\n", "\n\t\t\t            "))
            
        if self.mg5_cards["run"] != "":
            AdvPrint.cout("\t\t\t - run_card: "+self.mg5_cards["run"])

        if self.mg5_cards["config"] != "":
            AdvPrint.cout("\t\t\t - config_card: "+self.mg5_cards["config"])
            
        if self.mg5_cards["param"] != "":
            AdvPrint.cout("\t\t\t - param_card: "+self.mg5_cards["param"])
        
        if self.maxEvents > 0:
            AdvPrint.cout("\t\t\t - at most "+str(self.maxEvents)+" events are generated/analysed")
            
        if self.py8_infile != "":
            AdvPrint.cout("\t\t\t - Pythia8 settings file used for showering: "+self.py8_infile)

        if self.have_xsth:
            AdvPrint.cout("\t\t\t - Pythia8 won't run if parton cross section falls below: "+str(self.xsth)+" "+str(self.xsth_unit))

        
        
    def prepareFritzInputFile(self, fritzconfig):
        fritzconfig, secname = Pythia8Events.prepareFritzInputFile(self, fritzconfig)
        if Info.files['py8rndm'] != '':
            fritzconfig.set(secname,"rndmIn",Info.files['py8rndm'])
        fritzconfig.set(secname, "useMG5","true")
        fritzconfig.set(secname, "mgProcCard",self.mg5_cards["proc"])
        if self.mg5_cards["run"] != "" :
            fritzconfig.set(secname, "mgRunCard",self.mg5_cards["run"])
        if self.mg5_cards["param"] != "" :
            fritzconfig.set(secname, "mgParamCard",self.mg5_cards["param"])
        if self.mg5_cards["config"] != "" :
            fritzconfig.set(secname, "mgConfigCard",self.mg5_cards["config"])
        fritzconfig.set(secname, "mgRunPath",os.path.join(Info.paths['output_mg5'], self.identifier))
        fritzconfig.set(secname, "mgSourcePath",Info.paths['mg5_source_path'])
        if self.have_xsth:
            fritzconfig.set(secname, "xsectthresh",old_div(self.xsth*Info.unit(self.xsth_unit),Info.unit('pb'))) # pythia calculates in pb
        return fritzconfig, secname
