from math import sqrt
import operator

from advprint import AdvPrint
from resultcollector import ResultCollector
from cls import cls_obs, cls_exp, likelihood
from info import Info

class Evaluator:
    
    def __init__(self, resultCollector):
        self.warnings = list()
        self.resultCollector = resultCollector
    
        # expdata used
        self.obs = -1
        self.bkg = -1
        self.bkg_err = -1
    
        # efficiencies    
        self.signal_eff = -1
        self.signal_eff_error_stat = -1
        self.signal_eff_error_sys = -1
        self.signal_eff_error_tot = -1
    
        # r limits
        self.s95_obs = -1
        self.s95_exp = -1
        self.r_obs = -1
        self.r_obs_cons = -1
        self.r_obs_cons_sysonly = -1
        self.r_exp = -1
        self.r_exp_cons = -1
        self.r_exp_cons_sysonly = -1
        
        # cls limits
        self.cls_obs = -1
        self.cls_obs_err = -1
        self.cls_exp = -1
        self.cls_exp_err = -1
        
        # likelihood
        self.likelihood = -1

        # zsig
        self.expected_zsig = -1
        
        # load other information
        self.load_expdata()
        
    def printme(self):        
        print "results in resultCollector:"
        self.resultCollector.printme()
        
        print "signal_eff: "+str(self.signal_eff)
        print "signal_eff_error_stat: "+str(self.signal_eff_error_stat)
        print "signal_eff_error_sys: "+str(self.signal_eff_error_sys)
        print "signal_eff_error_tot: "+str(self.signal_eff_error_tot)
        
        # r limits
        print "s95_obs: "+str(self.s95_obs)
        print "s95_exp: "+str(self.s95_exp)
        print "r_obs: "+str(self.r_obs)
        print "r_obs_cons: "+str(self.r_obs_cons)
        print "r_obs_cons_sysonly: "+str(self.r_obs_cons_sysonly)
        print "r_exp: "+str(self.r_exp)
        print "r_exp_cons: "+str(self.r_exp_cons)
        print "r_exp_cons_sysonly: "+str(self.r_exp_cons_sysonly)
        
        # cls limits
        print "cls_obs: "+str(self.cls_obs)
        print "cls_obs_err: "+str(self.cls_obs_err)
        print "cls_exp: "+str(self.cls_exp)
        print "cls_exp_err: "+str(self.cls_exp_err)
        
        print "messages: "
        for m in self.warnings:
            print m
    
    def line_from_data(self, columns):
        """ Returns the information stated in the list 'columns' as a single line """
        line = ""
        for c in columns:   
            c = c.lower().strip()
            if c.lower() in ['name', 'id', 'identifier']:
                line +=  str(self.resultCollector.name)+"  "
            elif c.lower() in ['analysis', 'ana']:
                line +=  str(self.resultCollector.analysis)+"  "      
            elif c.lower() in ['sr', 'signalregion']:
                line +=  str(self.resultCollector.sr)+"  "
            elif c.lower() in ['totalmc', 'totalmcevents']:
                line +=  str(self.resultCollector.total_mcevents)+"  "    
            elif c.lower() in ['totalnorm', 'totalnormevents']:
                line +=  str(self.resultCollector.total_normevents)+"  "
            elif c.lower() in ['totalsw', 'totalsumofweights']:
                line +=  str(self.resultCollector.total_sumofweights)+"  "
            elif c.lower() in ['totalsw2', 'totalsumofweights2']:
                line +=  str(self.resultCollector.total_sumofweights2)+"  "
            elif c.lower() in ['signalsw', 'signalsumofweights']:
                line +=  str(self.resultCollector.signal_sumofweights)+"  "
            elif c.lower() in ['signalsw2', 'signalsumofweights2']:
                line +=  str(self.resultCollector.signal_sumofweights2)+"  "
            elif c.lower() in ['signalnorm', 'signalnormevents', 's']:
                line +=  str(self.resultCollector.signal_normevents)+"  "
            elif c.lower() in ['signalerrstat','signal_err_stat']:     
                line +=  str(self.resultCollector.signal_err_stat)+"  "   
            elif c.lower() in ['signalerrsys','signal_err_sys']:      
                line +=  str(self.resultCollector.signal_err_sys)+"  "  
            elif c.lower() in ['signalerrtot', 'ds','signal_err_tot']: 
                line +=  str(self.resultCollector.signal_err_tot)+"  "
            elif c.lower() in ['obs', 'o']:    
                line +=  str(self.obs)+"  "
            elif c.lower() in ['b', 'bkg']:   
                line +=  str(self.bkg)+"  "
            elif c.lower() in ['db', 'bkgerr']:   
                line +=  str(self.bkg_err)+"  "                
            elif c.lower() in ['signaleff', 'eff']:    
                line +=  str(self.signal_eff)+"  "
            elif c.lower() in ['signaleff_err_stat', 'eff_err_stat']:   
                line +=  str(self.signal_eff_error_stat)+"  "
            elif c.lower() in ['signaleff_err_sys', 'eff_err_sys']:   
                line +=  str(self.signal_eff_error_sys)+"  "
            elif c.lower() in ['signaleff_err_tot', 'eff_err_tot']:   
                line +=  str(self.signal_eff_error_tot)+"  " 
            elif c.lower() in ['s95obs']:   
                line +=  str(self.s95_obs)+"  "
            elif c.lower() in ['s95exp']:   
                line +=  str(self.s95_exp)+"  "
            elif c.lower() in ['robs','r_obs']:   
                line +=  str(self.r_obs)+"  "
            elif c.lower() in ['robscons','r_obs_cons']:   
                line +=  str(self.r_obs_cons)+"  "
            elif c.lower() in ['robsconssysonly','r_obs_cons_sysonly']:   
                line +=  str(self.r_obs_cons_sysonly)+"  "
            elif c.lower() in ['rexp','r_exp']:   
                line +=  str(self.r_exp)+"  "
            elif c.lower() in ['rexpcons','r_exp_cons']:   
                line +=  str(self.r_exp_cons)+"  "
            elif c.lower() in ['rexpconssysonly','r_exp_cons_sysonly']:   
                line +=  str(self.r_exp_cons_sysonly)+"  "
            elif c.lower() in ['clsobs','cls_obs']:   
                line +=  str(self.cls_obs)+"  "
            elif c.lower() in ['clsobs_err','cls_obs_err']:   
                line +=  str(self.cls_obs_err)+"  "
            elif c.lower() in ['clsexp','cls_exp']:     
                line +=  str(self.cls_exp)+"  "
            elif c.lower() in ['clsexp_err','cls_exp_err']:   
                line +=  str(self.cls_exp_err)+"  "
            elif c.lower() in ['likelihood']:   
                line +=  str(self.likelihood)+"  "  
                #todoelif c.lower() in ['likelihood']:   
               # line +=  str(self.likelihood)+"  "             
            else:
                AdvPrint.cerr_exit("evaluator::line_from_data - column "+c+" unknown!")
        return line
    
    
    def print_result(self):
        AdvPrint.set_cout_file(Info.files["output_result"], True)
        if self.cls_obs != -1:
            AdvPrint.cout("Test: Calculation of CLs(S, dS, B, dB, O) using profile likelihood")
        elif self.likelihood != -1:
            AdvPrint.cout("Test: Calculation of approximate (fast) likelihood given in results folder")
        elif self.r_obs != -1:
            AdvPrint.cout("Test: Calculation of r = signal/(95%CL limit on signal)")
        else:
            AdvPrint.cerr_exit("evaluator::printResult(): No result has been evaluated!")
            
        
        for w in self.warnings:
            AdvPrint.cout("\033[33mWarning: "+w+"\033[0m")
        result = "\033[31mExcluded\033[0m"
        if self.allowed():
            result = "\033[32mAllowed\033[0m"
        AdvPrint.cout("Result: "+result)
        if self.cls_obs != -1:
            AdvPrint.cout("Result for CLs: "+str(self.cls_obs))
        #if self.likelihood != -1:
        #    AdvPrint.cout("Result for likelihood: "+str(self.likelihood))
        elif self.r_obs != -1:
            AdvPrint.cout("Result for r: "+str(self.r_obs_cons))
        else:
            AdvPrint.cerr_exit("evaluator::printResult(): No result has been evaluated!")
        
        AdvPrint.cout("Analysis: "+self.resultCollector.analysis)
        AdvPrint.cout("SR: "+self.resultCollector.sr)
        AdvPrint.set_cout_file("#None")
    
    def load_expdata(self):
        analysis = self.resultCollector.analysis
        if analysis == "":
            return # for pseudo-evaluators
        
        sr = self.resultCollector.sr
        parameters = Info.get_analysis_parameters(analysis)
        
        # S95 comparison: r value compares lower s95 limit on s (which is almost 2 sigma) to the model independent limit
        if sr not in parameters["reference_data"]:
            AdvPrint.cerr_exit("evaluator::load_expdata unknown signal region "+analysis+" - "+sr)
        if "S95_obs" not in parameters["reference_data"][sr]:
            AdvPrint.cerr_exit("evaluator::load_expdata S95_obs value is unknown for "+analysis+" - "+sr)
        self.s95_obs = float(parameters["reference_data"][sr]["S95_obs"])
       
        # If no expected upper limit is given, observed limit is used
        if "S95_exp" not in parameters["reference_data"][sr]:
            self.s95_exp = self.s95_obs
            self.warnings.append("No expected limit could be found in reference data. Using expected = observed.")
        else:    
            self.s95_exp = float(parameters["reference_data"][sr]["S95_exp"])
  
        if "obs" not in parameters["reference_data"][sr]:
            AdvPrint.cerr_exit("evaluator::load_expdata 'obs' is unknown for "+analysis+" - "+sr)
        self.obs = float(parameters["reference_data"][sr]["obs"])
        
        if "bkg" not in parameters["reference_data"][sr]:
            AdvPrint.cerr_exit("evaluator::load_expdata 'bkg' is unknown for "+analysis+" - "+sr)
        self.bkg = float(parameters["reference_data"][sr]["bkg"])
        
        self.bkg_err = -1
        if "bkg_err" in parameters["reference_data"][sr]:
            self.bkg_err = float(parameters["reference_data"][sr]["bkg_err"])
        elif "bkg_errp" in parameters["reference_data"][sr]:
            # Asymmetric error: as a rough approximation, use the mean of the squares
            self.bkg_err = sqrt(float(parameters["reference_data"][sr]["bkg_errp"])**2 + float(parameters["reference_data"][sr]["bkg_errm"])**2)/sqrt(2.)
        elif "bkg_err_sys" in parameters["reference_data"][sr]:
            # Total error = independent quadratic sum of statistical and systematical component
            self.bkg_err = sqrt(float(parameters["reference_data"][sr]["bkg_err_stat"])**2 + float(parameters["reference_data"][sr]["bkg_err_sys"])**2 )
        elif "bkg_err_sysp" in parameters["reference_data"][sr]:
            self.bkg_err = sqrt(float(parameters["reference_data"][sr]["bkg_err_stat"])**2 + (float(parameters["reference_data"][sr]["bkg_err_sysp"])**2)/2. + (float(parameters["reference_data"][sr]["bkg_err_sysp"])**2)/2.)
        
        
    def calc_efficiencies(self, ):
        """ Calculates efficiencies """
        # efficiencies
        if self.resultCollector.total_normevents == 0:            
            self.signal_eff = 0
            self.signal_eff_error_stat =  0
            self.signal_eff_error_sys = 0
            self.signal_eff_error_tot = 0
        else:
            self.signal_eff = self.resultCollector.signal_normevents/self.resultCollector.total_normevents
            self.signal_eff_error_stat =  self.resultCollector.signal_err_stat/self.resultCollector.total_normevents
            self.signal_eff_error_sys = self.resultCollector.signal_err_sys/self.resultCollector.total_normevents
            self.signal_eff_error_tot = self.resultCollector.signal_err_tot/self.resultCollector.total_normevents
    
    def calc_r_values(self):
        """ Compares event numbers to pre-calculated S95 results """        
        self.r_obs = self.resultCollector.signal_normevents / self.s95_obs
        if self.r_obs < 0: # 95% lower limit on S cannot be negative
            self.r_obs = 0            
        self.r_obs_cons = (self.resultCollector.signal_normevents - 1.64*self.resultCollector.signal_err_tot) / self.s95_obs
        if self.r_obs_cons < 0:
            self.r_obs_cons = 0
        self.r_obs_cons_sysonly = (self.resultCollector.signal_normevents - 1.64*self.resultCollector.signal_err_sys) / self.s95_obs
        if self.r_obs_cons_sysonly < 0:
            self.r_obs_cons_sysonly = 0
        
        self.r_exp = self.resultCollector.signal_normevents / self.s95_exp
        if self.r_exp < 0: # 95% lower limit on S cannot be negative
            self.r_exp = 0            
        self.r_exp_cons = (self.resultCollector.signal_normevents - 1.64*self.resultCollector.signal_err_tot) / self.s95_exp
        if self.r_exp_cons < 0:
            self.r_exp_cons = 0
        self.r_exp_cons_sysonly = (self.resultCollector.signal_normevents - 1.64*self.resultCollector.signal_err_sys) / self.s95_exp
        if self.r_exp_cons_sysonly < 0:
            self.r_exp_cons_sysonly = 0
        
    def calc_cls_values(self):
        """ Calculates CLs explicitly """       
        
        # Determine the CLS parameters.
        sig = float(self.resultCollector.signal_normevents)
        sig_err = float(self.resultCollector.signal_err_tot)
        
        # Determine confidence limits. For conservative limits, use the 95% lower limit on S given 1 sigma delta S.
        if sig <= 0:
            self.cls_exp = 1
            self.cls_exp_err = 0
            self.cls_obs = 1
            self.cls_obs_err = 0
        else:
            (self.cls_exp, self.cls_exp_err) = cls_exp(self.bkg, self.bkg_err, self.obs, sig, sig_err, Info.parameters["randomseed"])
            (self.cls_obs, self.cls_obs_err)  = cls_obs(self.bkg, self.bkg_err, self.obs, sig, sig_err, Info.parameters["randomseed"]) 

    def calc_zsig(self):
        # Determine confidence limits. For conservative limits, use the 95% lower limit on S given 1 sigma delta S.
        signal_normevents = self.resultCollector.signal_normevents
        exit("ZSIG Calculation was disabled.")
        
        #if signal_normevents <= 0:
        #    self.expected_zsig = 0
        #else:
        #    self.expected_zsig = calculate_zsig.zsig_exp(float(self.bkg), self.bkg_err, signal_normevents)
 
    def calc_likelihood(self):       
        """ Calculates approximate likelihood using the root-find method """
        analysis = self.resultCollector.analysis
        sr = self.resultCollector.sr        
        parameters = Info.get_analysis_parameters(analysis)
        
        # Determine parameters 
        sig = float(self.resultCollector.signal_normevents)
        sig_err = float(self.resultCollector.signal_err_tot)
        obs = float(parameters["reference_data"][sr]["obs"])
        bkg = float(parameters["reference_data"][sr]["bkg"])
        bkg_err = 0
        if "bkg_err" in parameters["reference_data"][sr]:
            bkg_err = float(parameters["reference_data"][sr]["bkg_err"])
        elif "bkg_errp" in parameters["reference_data"][sr]:
            # Asymmetric error: as a rough approximation, use the mean of the squares
            bkg_err = sqrt(float(parameters["reference_data"][sr]["bkg_errp"])**2 + float(parameters["reference_data"][sr]["bkg_errm"])**2)/sqrt(2.)
        elif "bkg_err_sys" in parameters["reference_data"][sr]:
            # Total error = independent quadratic sum of statistical and systematical component
            bkg_err = sqrt(float(parameters["reference_data"][sr]["bkg_err_stat"])**2 + float(parameters["reference_data"][sr]["bkg_err_sys"])**2 )
        elif "bkg_err_sysp" in parameters["reference_data"][sr]:
            bkg_err = sqrt(float(parameters["reference_data"][sr]["bkg_err_stat"])**2 + (float(parameters["reference_data"][sr]["bkg_err_sysp"])**2)/2. + (float(parameters["reference_data"][sr]["bkg_err_sysp"])**2)/2.)

        self.likelihood  = likelihood(1.0, obs, bkg, sig, bkg_err, sig_err, [0.0,0.0])
    
    def allowed(self):
        """ Determines from the given result if the corresponding signal is excluded or not"""
        # If we have CLs, use CLs
        if self.cls_obs != -1:
            return self.cls_obs > 0.05
        
        # otherwise, use r-value
        elif self.r_obs_cons != -1:
            return self.r_obs_cons < 1
        
        else:
            AdvPrint.cerr_exit("evaluator::allowed() Cannot determine allowed/excluded without calculating cls or r-value!")
                
    def check_warnings(self):
        """ Checks situations of which the user should be aware of"""
        if self.resultCollector.signal_err_sys != 0 and self.resultCollector.signal_err_stat > self.resultCollector.signal_err_sys:
            self.warnings.append("Error is dominated by Monte Carlo Statistics!")
        if self.r_obs_cons != -1 and self.r_obs_cons < 1 and self.r_obs_cons_sysonly > 1:
            # guesstimate of delta_stat with which r would rise above 1
            stat_err_max2 = ((self.resultCollector.signal_normevents - self.s95_obs)/1.64)**2 - self.resultCollector.signal_err_sys**2
            if stat_err_max2 > 0:
                stat_err_max = sqrt(stat_err_max2)
                # find NMC which lets stat_err drop below stat_err_max
                r_nmc_min = (self.resultCollector.signal_err_stat/stat_err_max)**2
                self.warnings.append("There is a chance the signal could be excluded with about "+str(round(r_nmc_min, 1))+" times more MC events!")
        
def find_strongest_evaluators(list_of_evaluators, n_best):
    """ Finds the n_best strongest of all evaluators in given list or dict """
    def flat(d, out=[]):
        """ Transforms a dict of dicts of dicts ... into a single list """
        for val in d.values():
            if isinstance(val, dict):
                flat(val, out)
            else:
                out += [val,]
        return out
    list_of_evaluators = flat(list_of_evaluators)
    if n_best != 0 and len(list_of_evaluators) == 0:
        AdvPrint.cerr_exit("evaluator::find_strongest_evaluator \n Cannot find strongest out of 0 evaluators!")
    
    # Sort evaluators by cls_exp first
    best_evaluators = sorted(list_of_evaluators, key=operator.attrgetter('cls_exp'))
    # If the best evaluator has cls_exp which is not -1, it means that indeed 
    # all considered evaluators had a properly evaluated cls_exp
    if best_evaluators[0].cls_exp != -1:
        if n_best == 1:
            return best_evaluators[0]
        else:
            return best_evaluators[:n_best]
    # otherwise, sort again but now with respect to r_exp_cons
    best_evaluators = sorted(list_of_evaluators, key=operator.attrgetter('r_exp_cons'))
    best_evaluators.reverse() # largest r first
    if n_best == 1:
        return best_evaluators[0]
    else:
        return best_evaluators[:n_best]

def find_strongest_zsig(list_of_evaluators):
    """
    Find the strongest zsig overall and per analysis

    Arguments:
    list_of_evaluators: {analysis: {sr: evaluator} }

    Returns:
    [ {'analysis': analysis, 'sr': signal region, 'evaluator': evaluator} ]
    The list is sorted, with the strongest evaluator at the front
    """
    best = [_find_strongest_zsig_of_analysis(k, v) for k, v in list_of_evaluators.iteritems()]
    best.sort(key = lambda ev: ev['evaluator'].expected_zsig, reverse=True)
    return best

def _find_strongest_zsig_of_analysis(analysis, dict_of_evaluators):
    """
    Find the signal region with the highest expected zsig

    Arguments:
    dict_of_evaluators: dictionary of {name of signal region: evaluator}

    Returns:
    {'analysis': analysis, 'sr': sr, 'evaluator': evaluator}
    """
    key = max(dict_of_evaluators, key=lambda k: dict_of_evaluators[k].expected_zsig)
    return {'analysis': analysis, 'sr': key, 'evaluator': dict_of_evaluators[key]}
