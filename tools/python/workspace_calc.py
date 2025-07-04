import os
#os.environ["CUDA_VISIBLE_DEVICES"] = "-1"
import jax
#jax.config.update('jax_default_device',jax.devices('cpu')[0])
import json, pyhf
import numpy as np
from info import Info
from advprint import AdvPrint
import multibin_limit as mb
import multibin_limit_full as mbfull

def calc_workspace( path, analysis, mbsr ):
    inv_r = 10. 
    inv_r_exp = 10. 
    cls_obs = 1. 
    cls_exp = [1.,1.,1.,1.,1.]
    
    os.system("mkdir -p " + path + '/multibin_limits')

    mbfull.init(path, analysis, mbsr)
    names = mbfull.SR_dict.keys()
    SRs = mbfull.data_from_CMresults(path)
    o, b, db, s, ds = mbfull.select_MBsr(names, SRs)
    r = [x - 1.64*y for x, y in zip(s,ds)] #s - 1.64 ds
    if max(s) == 0. or max(r) <= 0.:
        AdvPrint.cout("No signal events in the selected SRs! Skipping")
        return 10., 10., 1., 1.
    if max(r) <= 0.:
        AdvPrint.cout("Signal events below MC uncertainty in the selected SRs! Skipping")
        return 10., 10., 1., 1.   
    
    if Info.parameters["backend"] == "":
        try:
            pyhf.set_backend("jax") # if backend unspecified try jax
        except ImportBackendError:
            pass
    elif Info.parameters["backend"] == "pytorch" and pyhf.tensorlib.name != "pytorch":
        pyhf.set_backend("pytorch")
    elif Info.parameters["backend"] == "jax" and pyhf.tensorlib.name != "jax":
        pyhf.set_backend("jax")    
    elif Info.parameters["backend"] == "tensorflow" and "tensorflow" in spey_pyhf.manager.available_backends and pyhf.tensorlib.name != "tensorflow":
        pyhf.set_backend("tensorflow")
        pyhf.set_backend("tensorflow")
    elif Info.parameters["backend"] == "numpy":
        pyhf.set_backend("numpy")
    pyhf.set_backend("jax")
    print("Pyhf backend: "+pyhf.tensorlib.name)

    with open(Info.paths['data']+ "/" + analysis + "/workspace.json") as serialized:
        conf = json.load(serialized)

    conf['measurements'][0]['config']['parameters'][4]['bounds'][0] = [0.0, 10.0] #signal strength
    for i in range(len(s)):
        sample = 0
        if i == 0:
            sample = 13
        conf['channels'][i*6]['samples'][sample]['data'][0] = s[i]
        conf['channels'][i*6]['samples'][sample]['modifiers'][1]['data'][0] = ds[i]

    workspace= pyhf.Workspace(conf)


    string = "================================\n Analysis: "+analysis+" , SR: "+mbsr+"\n"
    string += f"Limits with full likelihood (pyhf):\n"
    if Info.flags["mbcls"]:
        AdvPrint.cout("Observed:")
        cls_obs, cls_exp = hypotest(workspace)
        AdvPrint.cout("CL95: "+str(float(cls_obs)) )
        string += f"Observed CLs for mu = 1: {float(cls_obs)}"+'\n'
        if Info.flags["expected"]:
            string = string+f"Expected CLs band for mu = 1: {float(cls_exp[2])}"+'\n'
    if Info.flags["uplim"]:
        AdvPrint.cout("Observed:")
        poi_values, obs_limit, exp_limits, (scan, results) = upperlim(workspace)	
        AdvPrint.cout("Upper limit: "+str(obs_limit) )
        string = string+f"Observed upper limit: mu = {obs_limit:.4f}"+'\n'
        #for i in range(5):
        #    string=string+f"Upper limit ({-2+i:2d} sigma) (exp): mu = {exp_limits[i]:.4f}"+'\n'
        string += f"Expected upper limit (+/-2 sigma): mu = ["
        for i in range(4):
            string += f"{exp_limits[i]:.4f}, "
        string += f"{exp_limits[4]:.4f}]"    
    string += "\n================================\n"
    with open(path+'/multibin_limits/'+"results.dat", "a") as write_file:
        write_file.write(string)
        #print(string,file=write_file)
    return float(obs_limit), float(exp_limits[2]), float(cls_obs), cls_exp        
        
    

#Performs the CLs hypotesis test. Returns CLs_obs, [CLs_exp -2sigma,CLs_exp -1sigma, CLs_exp -0, CLs_exp +1sigma, CLs_exp +2sigma]
def hypotest(workspace, ntoys=-1):
    
    test_poi = 1.0
    model = workspace.model()
    data = workspace.data(model)
    init_pars = model.config.suggested_init()
    par_bounds = model.config.suggested_bounds()
    fixed_pars = [False] * model.config.npars
    cls_obs, cls_exp = pyhf.infer.hypotest(1.0, data, model, init_pars=init_pars, par_bounds=par_bounds, fixed_params=fixed_pars, return_expected_set = True, test_stat="qtilde")
    
    return cls_obs, cls_exp

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
    #poi_values = np.logspace(np.log10(0.001),np.log10(9),100)
    poi_values = np.linspace(0, 1, 10)
    model = workspace.model()
    data = workspace.data(model)
    init_pars = model.config.suggested_init()
    par_bounds = model.config.suggested_bounds()
    fixed_pars = [False] * model.config.npars
    obs_limit, exp_limits, (scan, results) = pyhf.infer.intervals.upper_limits.upper_limit(data, model, init_pars=init_pars, par_bounds=par_bounds, fixed_params=fixed_pars, scan = poi_values, level=0.05, return_results=True, test_stat="qtilde")
    print(results)

    return poi_values, obs_limit, exp_limits, (scan, results) 
##---------------------------------------------------------------------------------    