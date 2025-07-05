import os
#os.environ["CUDA_VISIBLE_DEVICES"] = "-1"
#import jax
#jax.config.update('jax_default_device',jax.devices('cpu')[0])
import json, ROOT
from ROOT.Experimental import XRooFit as XRF
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

    with open(Info.paths['data']+ "/" + analysis + "/pyhf_conf.json") as f:
        hs3in = json.load(f)

    for i in len(h3in["analysis"]):
        if h3in["analysis"][i]["name"] == mbsr:
            bkg_only = h3in["analysis"][i]["bkgonly"]

    with open(Info.paths['data']+ "/" + analysis + "/"+ bkg_only) as serialized:
        conf = json.load(serialized)

    #print(s)
    for i in range(len(s)/2):  # signal regions
        conf['distrbutions'][2]['samples'][3]['data']['contents'][i] = s[i]
        conf['distrbutions'][2]['samples'][3]['data']['errors'][i] = ds[i]

    for i in range(len(s)/2,len(s)): # control regions
        conf['distrbutions'][0]['samples'][1]['data']['contents'][i] = s[i]
        conf['distrbutions'][0]['samples'][1]['data']['errors'][i] = ds[i]

    serialized.close()

    with open(Info.paths['data']+ "/" + analysis + "/workspace.json", 'w') as outfile:
        json.dump(conf, outffile)
    outfile.close()
    AdvPrint.cout("Created workspace for analysis: "+analysis+" , SR: "+mbsr)
    AdvPrint.cout("Signal events: "+str(s))

    #https://xroofit.readthedocs.io/en/latest/hypothesisTesting.html#xroofit-demo-computing-discovery-significance
    fileName  = Info.paths['data']+ "/" + analysis + "/workspace.json"           # path to the workspace
    pdfName   = "simPdf"                           # name of the top-level pdf in the workspace
    channels  = "*"                                # comma-separated list of channels to include (n.b. you should not include VRs)
    dsName    = "obsData"                          # name of the observed dataset, use "" to use an asimov dataset for the obsData
    poiName   = ""                                 # name of the parameter of interest - leave blank to auto-infer if possible
    asimovVal = 0                                  # POI-value to assume for asimov dataset (if dsName="")
    scanMin   = 0                                  # lower boundary poi value for limit scan (can be more restricted than fitting range)
    scanMax   = 10                                 # upper boundary poi value for limit scan (can be more restricted than fitting range)
    scanN     = 0                                  # number of points to scan, leave as 0 for an auto-scan
    scanType  = "cls visualize"                    # leave out the 'visualize' if you don't want to see progress during scan
    constPars = ""                                 # comma-separated list of nuisance parameters to hold const, e.g. do "*" for a stat-only limit
    tsType    = XRF.xRooFit.TestStatistic.qmutilde # choices: tmu, qmu, qmutilde, q0, u0
    nSigmas   = [0,1,2,-1,-2,float('nan')]         # list of nSigmas to compute limits at ... "NaN" is used by xRooFit to indicate you want obs limit
    outFile   = ""                                 # specify a path to save the post-scan workspace (with result) to

    ws = XRF.xRooNode(fileName)
    hs = w[pdfName].reduced(channels).nll(dsName).hypoSpace(poiName,tsType)
    #hs.scan(scanType,scanN,scanMin,scanMax,nSigmas)
    limits = hs.limits()
    obs_limit = limits['obs'].value()
    exp_limits = [limits['-2'].value(), limits['-1'].value(), limits['0'].value(), limits['1'].value(), limits['2'].value()]

    string = "================================\n Analysis: "+analysis+" , SR: "+mbsr+"\n"
    string += f"Limits with full likelihood (xRooFit):\n"
    if Info.flags["mbcls"]:
        AdvPrint.cout("Observed:")
        AdvPrint.cout("CL95: "+str(float(cls_obs)) )
        string += f"Observed CLs for mu = 1: {float(cls_obs)}"+'\n'
        if Info.flags["expected"]:
            string = string+f"Expected CLs band for mu = 1: {float(cls_exp[2])}"+'\n'
    if Info.flags["uplim"]:
        AdvPrint.cout("Observed:")	
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

    os.remove(Info.paths['data']+ "/" + analysis + "/workspace.json")    
    return float(obs_limit), float(exp_limits[2]), float(cls_obs), cls_exp        
