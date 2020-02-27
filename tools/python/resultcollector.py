from math import sqrt
import os, sys

from advprint import AdvPrint
from info import Info

class ResultCollector:
    identifier = ""
    analysis = ""
    sr = ""
    
    total_mcevents = 0
    total_normevents = 0
    total_sumofweights = 0
    total_sumofweights2 = 0
    
    signal_sumofweights = 0
    signal_sumofweights2 = 0
    signal_normevents = 0
    signal_err_stat = 0
    signal_err_sys = 0
    signal_err_tot = 0
    
    def __init__(self, name, analysis, sr):
        self.name = name
        self.analysis = analysis
        self.sr = sr
        
        self.total_mcevents = 0
        self.total_normevents = 0
        self.total_sumofweights = 0
        self.total_sumofweights2 = 0
        
        self.signal_sumofweights = 0
        self.signal_sumofweights2 = 0
        self.signal_normevents = 0
        self.signal_err_stat = 0
        self.signal_err_sys = 0
        self.signal_err_tot = 0
    
    def printme(self):
        print "Name: "+self.name
        print "Analysis - SR: "+str(self.analysis)+" "+str(self.sr)
        print "\t total mcevents: "+str(self.total_mcevents)
        print "\t total_normevents: "+str(self.total_normevents)
        print "\t total_sumofweights: "+str(self.total_sumofweights)
        print "\t total_sumofweights2: "+str(self.total_sumofweights2)
    
        print "\t signal_sumofweights: "+str(self.signal_sumofweights)
        print "\t signal_sumofweights2: "+str(self.signal_sumofweights2)
        print "\t signal_normevents: "+str(self.signal_normevents)
        print "\t signal_err_stat: "+str(self.signal_err_stat)
        print "\t signal_err_sys: "+str(self.signal_err_sys)
        print "\t signal_err_tot: "+str(self.signal_err_tot)
        
        
    def line_from_data(self, columns):
        """ Returns the information stated in the list 'columns' as a single line """
        line = ""
        for c in columns:   
            c = c.lower().strip()
            if c.lower() in ['name', 'id', 'identifier']:
                line +=  str(self.name)+"  "
            elif c.lower() in ['analysis', 'ana']:
                line +=  str(self.analysis)+"  "      
            elif c.lower() in ['sr', 'signalregion']:
                line +=  str(self.sr)+"  "
            elif c.lower() in ['totalmc', 'totalmcevents','total_mcevents']:
                line +=  str(self.total_mcevents)+"  "    
            elif c.lower() in ['totalnorm', 'totalnormevents','total_normevents']:
                line +=  str(self.total_normevents)+"  "
            elif c.lower() in ['totalsw', 'totalsumofweights','total_sumofweights']:
                line +=  str(self.total_sumofweights)+"  "
            elif c.lower() in ['totalsw2', 'totalsumofweights2','total_sumofweights2']:
                line +=  str(self.total_sumofweights2)+"  "
            elif c.lower() in ['signalsw', 'signalsumofweights','signal_sumofweights']:
                line +=  str(self.signal_sumofweights)+"  "
            elif c.lower() in ['signalsw2', 'signalsumofweights2','signal_sumofweights2']:
                line +=  str(self.signal_sumofweights2)+"  "
            elif c.lower() in ['signalnorm', 'signalnormevents', "signal_normevents", "s"]:
                line +=  str(self.signal_normevents)+"  "
            elif c.lower() in ['signalerrstat', 'signal_err_stat', 'signalerrstat', 'sig_err_stat']:     
                line +=  str(self.signal_err_stat)+"  "   
            elif c.lower() in ['signalerrsys', 'signal_err_sys', 'signalerrsys', 'sig_err_sys']:      
                line +=  str(self.signal_err_sys)+"  "  
            elif c.lower() in ['signalerrtot', 'signal_err_tot', "ds", 'signalerrtot', 'sig_err_tot']: 
                line +=  str(self.signal_err_tot)+"  "
            else:
                AdvPrint.cerr_exit("resultcollector::line_from_data - column "+c+" unknown!")
        return line
    
     
    def add_and_sum(self, other):
        """ Used to add process by process: event numbers are added and errors are added independently """
        # only compatible results should be added
        if self.analysis != other.analysis or self.sr != other.sr:
            AdvPrint.cerr_exit("\t resultCollector::add_and_sum() \n \t Only results of the same analysis and same signal region can be added!")
        
        # When summing the weights, each subsample has to be properly reweighted: The overall contribtuion should only be proportional to the cross section i.e. totalnormevents. 
        # It should not depend on the MC size of the sample which hence should be divided out.
        
        # If 'self' is 0, then the weights are trivially just 'other':
        if self.total_normevents == 0:
            weight_self = 0
            weight_other = 1
        # If 'other' is 0, then the weights are trivially just 'self':
        elif other.total_normevents == 0:
            weight_self = 1
            weight_other = 0
        # otherwise, evaluate weights properly
        else: 
            weight_self = self.total_normevents / (other.total_normevents + self.total_normevents) * (self.total_sumofweights + other.total_sumofweights)/self.total_sumofweights
            weight_other = other.total_normevents / (other.total_normevents + self.total_normevents) * (self.total_sumofweights + other.total_sumofweights)/other.total_sumofweights
        
        self.total_mcevents = self.total_mcevents + other.total_mcevents
        self.total_normevents = self.total_normevents + other.total_normevents
        
        # Note thatw weight_self and weight_other are normalised such that self.total_sow * weight_self + other.total_sow * weight_other = self.total_sow + other.total_sow
        self.total_sumofweights = self.total_sumofweights + other.total_sumofweights         
        self.total_sumofweights2 = self.total_sumofweights2 * weight_self**2 + other.total_sumofweights2 * weight_other**2
            
        self.signal_sumofweights = self.signal_sumofweights * weight_self + other.signal_sumofweights * weight_other
        self.signal_sumofweights2 = self.signal_sumofweights2 * weight_self**2 + other.signal_sumofweights2 * weight_other**2
        self.signal_normevents = self.signal_normevents + other.signal_normevents # note that doing signal_sumofweights/total_sumofweights * total_normevents yields the same!
        
        if Info.flags['no_mc_stat_err']:
            self.signal_err_stat = 0.0 
        else:
            self.signal_err_stat = sqrt(self.signal_err_stat**2 + other.signal_err_stat**2) # Added in quadrature. Using signal_sumogweights2 yields almost same result (except for the N=0 staterror which comes for each process)
        
        self.signal_err_sys = sqrt(self.signal_err_sys**2 + other.signal_err_sys**2) # Added in quadrature
        self.signal_err_tot = sqrt(self.signal_err_stat**2+self.signal_err_sys**2)
    
    def add_and_average(self, other):
        """ Used to add event samples within the same process: weights are added and event numbers are redetermined """
        
        if self.analysis != other.analysis or self.sr != other.sr:
            AdvPrint.cerr_exit("\t resultCollector::add_and_average() \n \t Only results of the same analysis and same signal region can be added!")
        
        # This number must be equal for all averaged resultCollector (or 0, if this 'other' is the first item that is added)
        if self.total_normevents == 0:
            self.total_normevents = other.total_normevents
        else:
            weight1 = self.total_mcevents / (self.total_mcevents + other.total_mcevents)
            weight2 = other.total_mcevents / (self.total_mcevents + other.total_mcevents)
            self.total_normevents = (self.total_normevents * weight1 + other.total_normevents * weight2)/(weight1 + weight2)
        """    
          the below test is not correct anymore as combined runs of the same process with event gneration via Pythia8 or MadGraph can lead to different
          values for the respective cross sections. In that case, total_normevents should be given as the weighted mean of the respective total_normevents-numbers, using
          total_mcevents as the weight
        #elif (self.total_normevents - other.total_normevents)/float(self.total_normevents + other.total_normevents) > 0.001: # This number must be universal for averaging, can slightly differ due to rounding!
        #    AdvPrint.cerr_exit("\t resultCollector::add_and_average() \n \t Event files that should be averaged ("+self.name+" "+other.name+") have different total normevents in "+self.analysis+" - "+self.sr+". Something must have gone wrong!")
        """
        self.total_mcevents = self.total_mcevents + other.total_mcevents # add mc events
        self.total_sumofweights = self.total_sumofweights + other.total_sumofweights # add sum of weights
        self.total_sumofweights2 = self.total_sumofweights2 + other.total_sumofweights2 # add sum of weights^2
        
        
        # store old relative syserror (beware the case N = 0 and that self and other should be consistent!)
        rel_sys_error = 0
        if self.signal_err_sys != 0:
            rel_sys_error = self.signal_err_sys/self.signal_normevents
        if other.signal_err_sys != 0:
            rel_sys_error = other.signal_err_sys/other.signal_normevents
        # ds1/s1 must equal ds2/s2, or in other words ds1*s2 = ds2*s1, which avoids rounding problems
        if self.signal_err_sys != 0 and other.signal_err_sys != 0 and abs(self.signal_err_sys*other.signal_normevents - other.signal_err_sys*self.signal_normevents)>1E-5:
            print self.signal_err_sys # debug
            print self.signal_normevents # debug
            print other.signal_err_sys # debug
            print other.signal_normevents # debug
            print self.signal_err_sys*other.signal_normevents # debug 
            print other.signal_err_sys*self.signal_normevents #debug
            print self.signal_err_sys*other.signal_normevents - other.signal_err_sys*self.signal_normevents #debug
            AdvPrint.cerr_exit("\t resultCollector::add_and_average() \n \t Event files that should be averaged have different syserrors. Something must have gone wrong!")
        
        # weights can be added
        self.signal_sumofweights = self.signal_sumofweights + other.signal_sumofweights
        self.signal_sumofweights2 = self.signal_sumofweights2 + other.signal_sumofweights2
        
        # redetermine event numbers and stat error
        if self.total_sumofweights != 0:
            self.signal_normevents = self.total_normevents * self.signal_sumofweights / self.total_sumofweights
            
            if self.signal_sumofweights <= 0: # If there are no events in the signal region, ...
                if Info.flags['no_mc_stat_err']:
                    self.signal_err_stat = 0.0 
                else:  
                    self.signal_err_stat = 1.*self.total_normevents/self.total_sumofweights # ... set staterror to error on 1 Monte Carlo Event under the assumption that all events have equal weight
                self.signal_err_sys = 0 # ... set syserror to 0
            else:
                if Info.flags['no_mc_stat_err']:
                    self.signal_err_stat = 0.0 
                else:
                    self.signal_err_stat = self.total_normevents*sqrt(self.signal_sumofweights2)/(self.total_sumofweights)
                self.signal_err_sys = self.signal_normevents * rel_sys_error # multiply new normevents by old rel_sys_error
        else: # if there are no events, everything is 0
            self.signal_normevents = 0
            self.signal_err_stat = 0
            self.signal_err_sys = 0
                    
        self.signal_err_tot = sqrt(self.signal_err_stat**2+self.signal_err_sys**2)
        
