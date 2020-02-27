#!/usr/bin/python
import sys,os
from math import sqrt
from copy import deepcopy
import json, pickle, cPickle
import os
import shutil

from advprint import AdvPrint
from info import Info
from process import Process
from detectorsettings import DetectorSettings
from evaluator import Evaluator, find_strongest_evaluators, find_strongest_zsig
from resultcollector import ResultCollector

class CheckMATE2:
    """ This is the main object whose instance corresponds to a full CheckMATE run """
    procList = list()

    def __init__(self):
        #global Info, AdvPrint
        """ Initialisation of a CheckMATE object leads to an entire run of the CheckMATE procedure"""
        # Initialisation steps
        
        #Info.init()
        Info.fill_standard_paths_and_files()
        if len(sys.argv) == 1:
            self.printUsage()
        self.printLogo()
        if len(sys.argv) == 2 and sys.argv[-1] != "-h":
            Info.fill_info_from_file(sys.argv[1])            
            self.procList = Info.fill_processes_from_file(sys.argv[1])
        else:
            Info.fill_info_from_parameters()
            self.procList = Info.fill_processes_from_parameters()        
        if Info.parameters["outputexists"] == "add":            
            self.load(Info.files['internal_processes'])         
        for p in self.procList:               
            p.checkInputConsistency()    
        self.user_param_check()
        self.prepare_run()       
        
        # Running the event-based part
        if self.procList == []:
            AdvPrint.cerr_exit("No processes are loaded!")
        for p in self.procList:
            p.prepare()
            p.run()
        AdvPrint.cout("\n")    
        # Evaluate
        if not Info.flags['skipevaluation']:
	    self.evaluate()

        # Store internal status
        Info.save(Info.files['internal_info'])        
        self.save(Info.files['internal_processes'])
        
    def save(self, filename):
        """ Stores the current status of this instance in a file """
        contents = dict()
        contents["procList"] = pickle.dumps(self.procList)
        with open(filename, "w") as f:
            json.dump(contents, f, indent=2)
        
    
    def load(self, filename):
        """ Loads contents for current instance from a valid file  """
        with open(filename, "r") as f:
            contents = json.load(f)            
        try:
            # processes = old processes plus added new ones of current run
            newProcList = self.procList
            self.procList = pickle.loads(contents["procList"])           
            
            for new_p in newProcList:
                combined = False
                for old_p in self.procList:
                    if old_p.name == new_p.name:
                        old_p.combine_processes(new_p)
                        combined = True
                        break
                if not combined:
                    self.procList.append(new_p)
                    
        except KeyError:
            AdvPrint.cerr_exit("Problem loading info file "+inputfile)        
        return self
    
    def printLogo(self):
        """ Obvious"""
        AdvPrint.cout( """
  ____ _               _    __  __    _  _____ _____ ____  
 / ___| |__   ___  ___| | _|  \/  |  / \|_   _| ____|___ \ 
| |   | '_ \ / _ \/ __| |/ / |\/| | / _ \ | | |  _|   __) |
| |___| | | |  __/ (__|   <| |  | |/ ___ \| | | |___ / __/ 
 \____|_| |_|\___|\___|_|\_\_|  |_/_/   \_\_| |_____|_____|
  """
        ) # TODO: add cite info?

    def printUsage(self):
        self.printLogo()
        AdvPrint.cout( """
                                                  ___   
                                      |__| _       | _  
                                      |  |(_)\)/   |(_) 
        """)
        AdvPrint.cout( "Method 1: Input Parameters")
        AdvPrint.cout( "\trun -n {name_for_this_run} -a {analysis} -p {process} -xs {crosssection} -xse {crosssection error} -ev {eventfile}")
        AdvPrint.cout( "Method 2: Input File")
        AdvPrint.cout( "\trun {inputfile}")
        AdvPrint.cout( "")
        AdvPrint.cout( "Examples:")
        AdvPrint.cout( "\t./CheckMATE -n testrun -a atlas_1405_7875 -p \"gg\" -xs \"1*FB\" -xse \"0.1 FB\" -ev /scratch/all/gluinopair.hepmc")
        AdvPrint.cout( "\t./CheckMATE testparam.dat")
        AdvPrint.cout( "")
        AdvPrint.cout( "Type './CheckMATE -h' for more information about available parameters or check")
        AdvPrint.cout( "the given 'testparam.dat' file for the desired structure of input files")
        exit(1)

    def user_param_check(self):
        """Prints settings on screen and awaits user confirmation"""
        analysis_info = dict()

        AdvPrint.cout("The following settings are used:")
        AdvPrint.cout("Analyses: ")
        for analysis in Info.analyses:
            parameters = Info.get_analysis_parameters(analysis)
            analysis_info = ""
            if parameters["expectation_known"] == "n":
                analysis_info += "[NO EXPECTATION KNOWN -> NO EXCLUSION TEST]   "
            analysis_info += parameters["short_info"]
            AdvPrint.cout("\t"+analysis+" ("+analysis_info+")")
            
        AdvPrint.cout("E_CM: "+str(Info.parameters["ecm"]))
        AdvPrint.cout("Processes: ")
        for process in self.procList:
            process.printInfo()
            AdvPrint.cout("")
            
        AdvPrint.cout("Output Directory: ")
        AdvPrint.cout("\t"+Info.paths['output'])
        AdvPrint.cout("Additional Settings: ")
        if Info.files['slha'] != "":
            AdvPrint.cout("\t - SLHA file "+Info.files['slha']+" will be used for event generation")
        if Info.parameters['invisiblePIDs'] != []:
            AdvPrint.cout("\t - The following PIDs will be considered as invisible for the detector: "+str(Info.parameters['invisiblePIDs']).translate(None, "[]'"))
        if Info.flags['skipanalysis']:
            AdvPrint.cout("\t - No analysis step")
        if Info.flags['skippythia']:
            AdvPrint.cout("\t - No pythia step")
        if Info.flags['skipevaluation']:
            AdvPrint.cout("\t - No evaluation step")
        if Info.flags['fullcls']:
            AdvPrint.cout("\t - CLs of all signal regions will be explicitly calculated")
        if Info.parameters['bestcls'] != 0:
            AdvPrint.cout("\t - CLs of "+str(Info.parameters['bestcls'])+" best signal region will be explicitly calculated")
        if Info.flags['likelihood']:
            AdvPrint.cout("\t - Likelihood will be calculated for each signal region")
        if Info.flags['no_mc_stat_err']:
            AdvPrint.cout("\t - No Monte Carlo statistical uncertainty will be included in the evaluation")    
        if Info.flags['eff_tab']:
            AdvPrint.cout("\t - Efficiency tables will be calculated for each signal region of every analysis run")        
        if Info.flags["controlregions"]:
            AdvPrint.cout("\t - Analysing control regions")
        if Info.parameters["outputexists"] == "overwrite":
            AdvPrint.cout("\t - Old results will be deleted")
        if Info.parameters["outputexists"] == "add":
            AdvPrint.cout("\t - New results will be added to old ones")
        if Info.parameters["randomseed"] != 0:
            AdvPrint.cout("\t - Fixed random seed of "+str(Info.parameters["randomseed"]))
        if Info.flags["write_delphes_events"]:
            AdvPrint.cout("\t - delphes .root files will be written!")
        if Info.flags["write_pythia_events"]:
            AdvPrint.cout("\t - pythia .hepmc files will be written!")
        if Info.parameters["EventResultFileColumns"] != ['analysis', 'sr', 'signal_normevents', 'signal_err_tot']:            
            AdvPrint.cout("\t - print columns "+str(Info.parameters['EventResultFileColumns']).translate(None, "[]'")+" in event result files!")
        if Info.parameters["ProcessResultFileColumns"] != ['analysis', 'sr', 'signal_normevents', 'signal_err_tot']:            
            AdvPrint.cout("\t - print columns "+str(Info.parameters['ProcessResultFileColumns']).translate(None, "[]'")+" in process result files!")
        if Info.parameters["TotalEvaluationFileColumns"] != ['analysis', 'sr', 'o', 'b', 'db', 's', 'ds', 's95obs', 's95exp', 'robscons', 'rexpcons']:
            AdvPrint.cout("\t - print columns "+str(Info.parameters['TotalEvaluationFileColumns']).translate(None, "[]'")+" in total evaluation files!")
        if Info.parameters["BestPerAnalysisEvaluationFileColumns"] != ['analysis', 'sr', 'o', 'b', 'db', 's', 'ds', 's95obs', 's95exp', 'robscons', 'rexpcons']:
            AdvPrint.cout("\t - print columns "+str(Info.parameters['BestPerAnalysisEvaluationFileColumns']).translate(None, "[]'")+" in bert-per-analysis evaluation files!")
            
        # Let user check correctness of parameters, unless in skipparamcheck.
        if not Info.flags['skipparamcheck']:
            while True:
                c = raw_input("Is this correct? (y/n) ")
                if c == "y": 
                    break
                elif c == "n": 
                    exit(1)
                    
            AdvPrint.cout("")



    def prepare_run(self):
        """Creates all output folders and files and generate cards that are needed"""        
        Info.prepare_config()
        # preparation only needed if new run is started with "overwrite"
        DetectorSettings.merge_settings()
        if Info.parameters["outputexists"] == "overwrite" or not os.path.isdir(Info.paths['output']):
            Info.prepare_output_directories()
            DetectorSettings.update_delphes_files()
        else:
            # If the original run used modified Delphes settings, we need them for the added run as well
            atlas_conf_new = os.path.join(Info.paths['output_delphes'], "modified_ATLAS_card.tcl")
            cms_conf_new = os.path.join(Info.paths['output_delphes'], "modified_CMS_card.tcl")
            if os.path.isfile(atlas_conf_new):
                Info.files['delphes_global_config_ATLAS'] = atlas_conf_new
            if os.path.isfile(cms_conf_new):
                Info.files['delphes_global_config_CMS'] = cms_conf_new
        

    def evaluate(self):
        """ Performs statistical evaluation of the result """
        AdvPrint.cout("Evaluating Results")
        resultCollectors = self.get_resultCollectors()
            
        # evaluate all results        
        evaluators = dict()
        for analysis in resultCollectors:
            evaluators[analysis] = dict()
                
        # only process those results and those signal regions that are given in the reference file
        for analysis in Info.analyses:
            signal_regions = Info.get_analysis_parameters(analysis)["signal_regions"]
            for sr in signal_regions:
                evaluator = Evaluator(resultCollectors[analysis][sr])
                # Calculate everything that should be calculated
                # TODO: Beware analyses with unknown background
                evaluator.calc_efficiencies()
                evaluator.calc_r_values()
                if Info.flags["likelihood"]:
                    evaluator.calc_likelihood()
                if Info.flags["fullcls"]:
                    evaluator.calc_cls_values()
                if Info.flags["zsig"]:
                    evaluator.calc_zsig()
                evaluators[analysis][sr] = evaluator
                
        if Info.parameters["bestcls"] != 0:
            AdvPrint.cout("Calculating CLs for the "+str(Info.parameters["bestcls"])+" most sensitive signal regions!")
            best_evaluators = find_strongest_evaluators(evaluators, Info.parameters["bestcls"])
            # if "bestcls" is 1, find_strongest_evaluators does not return a list but just the single best
            if Info.parameters["bestcls"] == 1:
                best_evaluators = [best_evaluators]
            for ev in best_evaluators:
                ev.calc_cls_values()
                     
        # find best result    
        best_evaluator_per_analysis = dict()
        for analysis in evaluators:
            # Find bes of all SRs in analysis
            best_evaluator_per_analysis[analysis] = find_strongest_evaluators(evaluators[analysis], 1)
        best_evaluator = find_strongest_evaluators(best_evaluator_per_analysis, 1)
            
        AdvPrint.set_cout_file(Info.files['output_totalresults'], True)
        AdvPrint.mute()        
        for col in Info.parameters["TotalEvaluationFileColumns"]:
            AdvPrint.cout(col+"  ", "nlb")
        AdvPrint.cout("")
        for a in sorted(evaluators.keys()):            
            for sr in sorted(evaluators[a].keys()):
                AdvPrint.cout(evaluators[a][sr].line_from_data(Info.parameters["TotalEvaluationFileColumns"]))
        AdvPrint.format_columnated_file(Info.files['output_totalresults'])
        
        AdvPrint.set_cout_file(Info.files['output_bestsignalregions'], True)
        AdvPrint.mute()        
        for col in Info.parameters["BestPerAnalysisEvaluationFileColumns"]:
            AdvPrint.cout(col+"  ", "nlb")
        AdvPrint.cout("")
        # print analyses in alphabetic order
        for a in sorted(best_evaluator_per_analysis.keys()):
            AdvPrint.cout(best_evaluator_per_analysis[a].line_from_data(Info.parameters["BestPerAnalysisEvaluationFileColumns"]))
        AdvPrint.format_columnated_file(Info.files['output_bestsignalregions'])
        AdvPrint.set_cout_file("#None")
        AdvPrint.unmute()
        best_evaluator.check_warnings()
        best_evaluator.print_result()

        if Info.flags['zsig']:
            _print_zsig(evaluators)
        if Info.flags['likelihood']:
            _print_likelihood(evaluators)  
          
    def get_resultCollectors(self):
        """ Finds the results of all events within all processes and sums and averages them """
        #setup resultCollector object
        resultCollectors_tot = dict()
        for analysis in Info.analyses:                
            resultCollectors_tot[analysis] = dict()
            signal_regions = Info.get_analysis_parameters(analysis)["signal_regions"]
            for sr in signal_regions:
                resultCollectors_tot[analysis][sr] = ResultCollector("total", analysis, sr)
                
        # loop over all associated processes
        for proc in self.procList:
            # process results are summed
            resultCollectors_proc = proc.get_resultCollectors()
            for analysis in resultCollectors_tot:
                for sr in resultCollectors_tot[analysis]:
                    resultCollectors_tot[analysis][sr].add_and_sum(resultCollectors_proc[analysis][sr])
                      
        return resultCollectors_tot

def _print_zsig(evaluators):
    for analysis, v in evaluators.iteritems():
        with open(Info.files['output_evaluation_zsig'][analysis], "w") as of:
            of.write("SR S B dB Z_exp\n")
            for sr, ev in v.iteritems():
                of.write(
                        sr+"  "
                        +str(ev.resultCollector.signal_normevents)+"  "
                        +str(float(ev.bkg))+"  "
                        +str(float(ev.bkg_err))+"  "
                        +str(ev.expected_zsig)+"\n"
                        )
    best = find_strongest_zsig(evaluators)
    with open(Info.files['output_bestsignificanceregions'], "w") as of:
        of.write("analysis  best  S  Zexp\n")
        for b in best:
            ev = b['evaluator']
            of.write(
                    b['analysis']+"  "
                    +b['sr']+"  "
                    +str(ev.resultCollector.signal_normevents)+"  "
                    +str(ev.expected_zsig)+"\n"
                    )
    with open(Info.files['output_result_zsig'], "w") as of:
        result_zexp = "Zexp_max = "+str(best[0]['evaluator'].expected_zsig)
        of.write("Result for Zexp: "+result_zexp+"\n")


def _print_likelihood(evaluators):
    tot_likeli = 0.
    dict_likeli = {}
    for analysis, v in evaluators.iteritems():
        ana_likeli =0.
        AdvPrint.set_cout_file(Info.files['output_evaluation_likelihood'][analysis], True)
        AdvPrint.mute()        
        AdvPrint.cout("SR  o  b  db  s  ds  likeli")
        for sr, ev in v.iteritems():
            AdvPrint.cout(sr+"  "
                        +str(float(ev.obs))+"  "
                        +str(float(ev.bkg))+"  "
                        +str(float(ev.bkg_err))+"  "
                        +str(ev.resultCollector.signal_normevents)+"  "
                        +str(ev.resultCollector.signal_err_tot)+"  "
                        +str(ev.likelihood))
            ana_likeli += ev.likelihood
        AdvPrint.format_columnated_file(Info.files['output_evaluation_likelihood'][analysis])
        AdvPrint.set_cout_file("#None")
        AdvPrint.unmute()
        dict_likeli[analysis] = ana_likeli
        tot_likeli += ana_likeli  
        
    AdvPrint.set_cout_file(Info.files['likelihood'], True)
    AdvPrint.mute()   
    AdvPrint.cout("Analysis  -2lnL")
    for a in dict_likeli:
        AdvPrint.cout(a+"  "+str(dict_likeli[a]))
    AdvPrint.cout("\nTotal:  "+str(tot_likeli))  
    AdvPrint.format_columnated_file(Info.files['likelihood'])
