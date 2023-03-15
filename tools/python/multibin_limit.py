import json
import numpy as np
import pyhf
import matplotlib.pyplot as plt
import os
import sys
import scipy

def data_from_CMresults(path):
    f_results=open(path+'/evaluation/total_results.txt','r')
    table = f_results.read()
    f_results.close()
    SRs = []
    for line in table.split('\n')[1:]:
        if line != '':
            SRs.append({'SR':line.split()[1],
                'o':float(line.split()[2]),
                'b':float(line.split()[3]),
                'db':float(line.split()[4]),
                's':float(line.split()[5]),
                'ds':float(line.split()[6])
                })
    return SRs

#Creates arrays with the data for the list of SRs passed. 
def select_MBsr(names, SRs):
    s=[]
    ds=[]
    b=[]
    db=[]
    o=[]
    for sr in SRs:
        if sr['SR'] in names:
            b.append(sr['b'])  
            db.append(sr['db'])  
            o.append(sr['o']) 
            s.append(max(sr['s'],0.01))  
            ds.append(max(min(sr['s'],sr['ds']),0.01)) 
    return o,b,db,s,ds

#Creates the workspace from the data and exports it to a new folder path/multibin_limits/ in json format.
def create_workspace(path, o, b, db, s, ds, systematics = 0, lumi = 0.017):
    os.system("mkdir -p "+path+'/multibin_limits')
    spec = {"channels":[{"name":"multibin-fit","samples":[{"name":"signal","data":s,"modifiers": [{"name": "mu","type": "normfactor","data": None},{"name":"staterror_signal","type":"staterror","data":ds},{ "name": "lumi", "type": "lumi", "data": None}, { "name": "sistematics", "type": "normsys", "data": {"hi": 1+systematics, "lo": 1-systematics}}]},{"name":"background","data":b,"modifiers":[{"name": "uncorr_bkguncrt","type": "histosys","data":{"hi_data":[x+y for x,y in zip(b,db)],"lo_data":[x-y for x,y in zip(b,db)]}}]}]}],"observations":[{"name":"multibin-fit","data":o}], "measurements":[ {"name": "Measurement","config": {"poi": "mu","parameters": [{'bounds': [[-100, 100]], 'fixed': False, 'inits': [1], 'name': 'mu'},
    {'bounds': [[-100, 100]], 'fixed': False, 'inits': [1], 'name': 'uncorr_bkguncrt'},
    {'auxdata': [1],
     'bounds': [[0.915, 1.085]],
     'inits': [1],
     'name': 'lumi',
     'sigmas': [lumi]}]}}],"version": "1.0.0"}
    #with open(path+'/multibin_limits/'+"workspace.json", "w") as write_file:
    #    json.dump(spec, write_file, indent=4)
    return pyhf.Workspace(spec)

#Performs the CLs hypotesis test. Returns CLs_obs, [CLs_exp -2sigma,CLs_exp -1sigma, CLs_exp -0, CLs_exp +1sigma, CLs_exp +2sigma]
def hypotest(workspace, ntoys=-1):
    test_poi = 1.0
    model = workspace.model()
    if ntoys<0:
    	result = pyhf.infer.hypotest(test_poi, workspace.data(model), model, test_stat = "q", return_expected_set = True)
    else:
    	result = pyhf.infer.hypotest(test_poi, workspace.data(model), model,
    		test_stat = "q",
    		return_expected_set = True,
    		calctype = "toybased",
    		ntoys = ntoys,
    		track_progress = True)   
    return result

#Calculates the upper limit overs signal strenght. The CLs value is calculated for a set of 50 values between 0.1 and 5 and the 95% limit is calculated form the interpolated line. Returns:
# poi_values = values of the signal strenght scanned (50 values between 0.1 and 5).
# obs_limit = observed upper limit .
# exp_limits = list of expected upper limits correspondign to [-2,-1,0,1,2]sigma.
# scan = iterable over poi_values.
# results = results of the hypotest for the poi_values.
# ntoys = if greater than 0 changes to toy based calculation instead of the asymptotic aproximation, with ntoys number of "toys".
def upperlim(workspace, ntoys = -1):
    #poi_values = np.linspace(0.1, 5, 50)
    #poi_values = np.logspace(np.log10(0.001),np.log10(10),100)
    poi_values = np.logspace(np.log10(0.001),np.log10(9),100)
    model = workspace.model()
    if ntoys <= 0:
    	obs_limit, exp_limits, (scan, results) = pyhf.infer.intervals.upperlimit(workspace.data(model), model, poi_values, level=0.05, return_results=True,test_stat="q")
    else:
    	obs_limit, exp_limits, (scan, results) = pyhf.infer.intervals.upperlimit(workspace.data(model), model, 
    		poi_values, 
    		level = 0.05, 
    		return_results = True,
    		calctype = "toybased",
    		ntoys = ntoys,
    		track_progress = True,
    		test_stat = "q")   
    return poi_values, obs_limit, exp_limits, (scan, results) 
##--------------------------------------------------------------------------------------


#Wraps around the fucntions to calculate the upper limit for the data in the CheckMATE results folder pointed through 'path' and the set of bins in 'names', then stores the results in  'path/multibin_limits/'.
#'names' must be a list of SRs names with the exact same notation as in 'path/evaluation/total_results.txt'.
# if ntoys greater than 0 changes to toy based calculation instead of the asymptotic aproximation, with ntoys number of "toys".
# 'systematics' is the relative systematic uncertanty of the signal (expressed as ratio ds/s, not as percentage). Default is 0.
# 'lumi is the uncertanty in the integrated luminosity, default value is 0.017 (1.7%).
def calc_point(path, names, analysis, mbsr, systematics = 0, lumi = 0.017, ntoys = 1000):
    string = "================================\n Analysis: "+analysis+" , SR: "+mbsr+"\n"
    SRs = data_from_CMresults(path)
    o, b, db, s, ds = select_MBsr(names, SRs)
    workspace = create_workspace(path, o, b, db, s, ds, systematics, lumi)
    result = hypotest(workspace, ntoys)
    poi_values, obs_limit, exp_limits, (scan, results) = upperlim(workspace, ntoys)	
    string += f"Observed CLs for mu = 1: {result[0]}"+'\n'
    string = string+f"Expected CLs band for mu = 1: {[exp.tolist() for exp in result[1]]}"+'\n'
    string = string+f"Upper limit (obs): mu = {obs_limit:.4f}"+'\n'
    #for i in range(5):
    #    string=string+f"Upper limit ({-2+i:2d} sigma) (exp): mu = {exp_limits[i]:.4f}"+'\n'
    string += f"Expected upper limit (+/-2 sigma) (exp): mu = ["
    for i in range(4):
        string += f"{exp_limits[i]:.4f}, "
    string += f"{exp_limits[4]:.4f}]"    
    string += "\n================================\n"
    with open(path+'/multibin_limits/'+"results.dat", "a") as write_file:
        write_file.write(string)
        #print(string,file=write_file)
    return obs_limit
