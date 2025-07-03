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
    print("Pyhf backend: "+pyhf.tensorlib.name)

    with open(Info.paths['data']+ "/" + analysis + "/workspace.json") as serialized:
        conf = json.load(serialized)

    for i in range(len(s)):
        conf['channels'][i*6]['samples'][0]['data'][0] = [s[i]]
        conf['channels'][i*6]['samples'][0]['modifiers'][1] = [ds[i]]
        
    