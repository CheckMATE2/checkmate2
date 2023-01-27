import json
import numpy as np
import pyhf, jsonpatch
import matplotlib.pyplot as plt
import os
import sys, subprocess
from info import Info

def init(path,analysis,MB_set):
    global analysis_name
    global SR_set
    global bkgonly
    global f_patchset
    global SR_dict  
    global histosize

    if not os.path.exists(path+'/multibin_limits'):
        os.mkdir(path+'/multibin_limits')
    analysis_name = analysis
    SR_set=MB_set
    with open(hepfiles_folder+analysis_name+"/pyhf_conf.json") as serialized:
       conf = json.load(serialized)
    ind=[x['name'] for x in conf["analysis"]].index(SR_set)
    bkgonly = conf["analysis"][ind]['bkgonly']
    f_patchset = conf["analysis"][ind]['patchset']
    SR_dict=conf["analysis"][ind]["SR_dict"]
    histosize=int(conf["analysis"][ind]["histosize"])

## Returns the list of regions defined inside the HEP_model file 'file_p'.
def lookup_SRs(file_p):
    with open(file_p) as serialized:
        spec = json.load(serialized)
    workspace = pyhf.Workspace(spec)
    print(workspace.channels)
      	

#Loads the data corresponding to observed, background and signal events from the CheckMATE results file in the results folder passed.
def data_from_CMresults(path):
    f_results=open(path+'/evaluation/total_results.txt','r')
    table=f_results.read()
    f_results.close()
    SRs=[]
    for line in table.split('\n')[1:]:
        if line!='':
            SRs.append({'SR':line.split()[1],
                'o':float(line.split()[2]),
                'b':float(line.split()[3]),
                'db':float(line.split()[4]),
                's':float(line.split()[5]),
                'ds':float(line.split()[6])
                })
    return SRs

#Creates arrays with the data for the list of SRs passed. 
def select_MBsr(names,SRs):
    s=[]
    ds=[]
    b=[]
    db=[]
    o=[]
    _names=[SR_dict[x] for x in SR_dict.keys()]
    for sr in SRs:
        if sr['SR'] in _names:
            b.append(sr['b'])  
            db.append(sr['db'])  
            o.append(sr['o']) 
            s.append(max(sr['s'],1e-3))  
            ds.append(max(min(sr['s'],sr['ds']),1e-4)) 
    return o,b,db,s,ds
    
#Creates a patch for the sample passed, based in the workspace in spec.
def patch(sample,spec,systematics=0):
    patch={"metadata": {
                "name": sample["name"],
                "values": sample["id"]       
            },
        "patch":[]}
    for i,channel in zip(range(len(spec['channels'])),[x["name"] for x in spec['channels']]):
        path="/channels/"+str(i)+"/samples/"+str(len(spec['channels'][i]['samples']))
        if channel in [ name for name in sample["SRs"]]:
            if histosize==1:
            	names=[name for name in sample["SRs"]]
            	s=[sample["s"][names.index(channel)]]
            	ds=[sample["ds"][names.index(channel)]]
            else:
            	names=[name+"["+str(i)+"]" for name,i in zip(sample["SRs"],range(histosize))]
            	s=sample["s"]
            	ds=sample["ds"]
        else:
            s=[0 for x in range(len(spec['channels'][i]['samples'][0]['data']))]
            ds=[0 for x in range(len(spec['channels'][i]['samples'][0]['data']))]
        
        patch["patch"].append(
                        {"op": "add","path": path,"value": {"data": s,"modifiers": [
                            {
                                "data": None,
                                "name": "lumi",
                                "type": "lumi"
                            },
                            {
                                "data": None,
                                "name": "mu_SIG",
                                "type": "normfactor"
                            },
                            {
                                "data": ds,
                                "name": "staterror_"+channel,
                                "type": "staterror"
                            },
                            {   "name": "sistematics", 
                                "type": "normsys", 
                                "data": {"hi": 1+systematics, "lo": 1-systematics}
                            }
            ],
            "name":sample["name"]
            }
        } 
        )
    return patch      
    
#Creates the patchset from the data and exports it to a new folder path/pyhf/ in json format.
def create_patchset(path,names,s,ds,systematics=0):
    import hashlib
    with open(hepfiles_folder+analysis_name+"/Likelihoods/"+bkgonly) as serialized:
        spec = json.load(serialized)
    with open(hepfiles_folder+analysis_name+"/Likelihoods/"+f_patchset) as serialized:
        spec_patchset = json.load(serialized)
    if not os.path.exists(path+'/pyhf_full'):
        os.mkdir(path+'/pyhf_full')
    if histosize==1:
        samples=[{"name":'Signal0',"id":len(spec_patchset["patches"][0]["metadata"]["values"])*[''],"SRs":names,"s":s,"ds":ds}]
    else:
        samples=[{"name":'Signal0',"id":len(spec_patchset["patches"][0]["metadata"]["values"])*[''],"SRs":[x[:-3] for x in names],"s":s,"ds":ds}]

    workspace_new={"metadata": {"analysis_id": spec_patchset["metadata"]["analysis_id"],"description": spec_patchset["metadata"]["description"],"digests": {"sha256": ""},"labels": spec_patchset["metadata"]["labels"],"references": {"hepdata": spec_patchset["metadata"]["references"]["hepdata"]}},"patches": [],"version": "1.0.0"}
    for sample in samples:
        workspace_new["patches"].append(patch(sample,spec,systematics))
    workspace_new["metadata"]["digests"]["sha256"]=hashlib.sha256(json.dumps(workspace_new).encode('utf8')).hexdigest()
    with open(path+'/pyhf_full/'+"patchset.json", "w") as write_file:
        json.dump(workspace_new, write_file, indent=4)


#Performs the CLs hypotesis test. Returns CLs_obs, [CLs_exp -2σ,CLs_exp -1, CLs_exp -0σ, CLs_exp +1σ, CLs_exp +2σ].
def hypotest(workspace,ntoys=-1):
    test_poi = 1.0
    model = workspace.model()
    if ntoys<0:
        result= pyhf.infer.hypotest(test_poi,workspace.data(model),model,test_stat="qtilde",return_expected_set=True)
    else:
    	result= pyhf.infer.hypotest(test_poi,workspace.data(model),model,
    		test_stat="qtilde",
    		return_expected_set=True,
    		calctype="toybased",
    		ntoys=ntoys,
    		track_progress=True)   
    return result

#Calculates the upper limit overs signal strenght. The CLs value is calculated for a set of 50 values between 0.1 and 5 and the 95% limit is calculated form the interpolated line. Returns:
# poi_values = values of the signal strenght scanned (50 values between 0.1 and 5).
# obs_limit = observed upper limit .
# exp_limits = list of expected upper limits correspondign to [-2σ,-1σ,0σ,1σ,2σ].
# scan = iterable over poi_values.
# results = results of the hypotest for the poi_values.
def upperlim(workspace,ntoys=-1):
    poi_values = np.logspace(np.log10(0.001),np.log10(10),100)
    model = workspace.model()
    if ntoys<=0:
    	obs_limit, exp_limits, (scan, results) = pyhf.infer.intervals.upperlimit(workspace.data(model), model, poi_values, level=0.05, return_results=True)
    else:
    	obs_limit, exp_limits, (scan, results) = pyhf.infer.intervals.upperlimit(workspace.data(model), model, 
    		poi_values, 
    		level=0.05, 
    		return_results=True,
    		calctype="toybased",
    		ntoys=ntoys,
    		track_progress=True)   
    return poi_values, obs_limit, exp_limits, (scan, results) 

def calc_point(path,analysis,MB_set,full,systematics=0,ntoys=-1):

    global hepfiles_folder
    
    hepfiles_folder = Info.paths['data']+"/"   #<----Set the path of the folder with the models here.
    #hepfiles_folder = "/home/krolb/tools/CheckMATE/multibin_1908_03122/data/"
    init(path,analysis,MB_set)
    names = SR_dict.keys()
    SRs = data_from_CMresults(path)
    o, b, db, s, ds = select_MBsr(names,SRs)
    patchset = create_patchset(path,names,s,ds,systematics)
    #string='jsonpatch '+hepfiles_folder+analysis_name+'/Likelihoods/'+bkgonly+' <(pyhf patchset extract '+path+'/pyhf_full/patchset.json --name \'Signal0\') > '+path+'/pyhf_full/'+SR_set+'_Signal0.json'
    #print(string)
    
    with open(path+'/pyhf_full/patchset.json') as serialized:
        signal = pyhf.PatchSet(json.load(serialized))
    signal0 = signal["Signal0"]    
    #os.system('pyhf patchset extract '+path+'/pyhf_full/patchset.json --name \'Signal0\' --output-file '+path+'/pyhf_full/'+SR_set+'_extract.json')
    #os.system('jsonpatch '+hepfiles_folder+analysis_name+'/Likelihoods/'+bkgonly+' '+ path+'/pyhf_full/'+SR_set+'_extract.json'+' > '+ path+'/pyhf_full/'+SR_set+'_Signal0.json')
    
    with open(hepfiles_folder+analysis_name+'/Likelihoods/'+bkgonly) as serialized:
        bckg_input = json.load(serialized)
    spec = jsonpatch.apply_patch(bckg_input, signal0) 
    #with open(path+"/pyhf_full/"+SR_set+"_Signal0.json") as serialized:
    #    spec = json.load(serialized)
    workspace = pyhf.Workspace(spec)
    result = hypotest(workspace,ntoys)
    
    if full:
        poi_values, obs_limit, exp_limits, (scan, results) = upperlim(workspace,ntoys)
    
    string = "================================\n Analysis: "+analysis+" , SR: "+MB_set+"\n"
    string += f"Observed CLs for μ = 1: {result[0]}"+'\n'
    string += f"Expected CLs band for μ = 1: {[exp.tolist() for exp in result[1]]}"+'\n'
    
    if full:
        string += f"Upper limit (obs): μ = {obs_limit:.4f}"+'\n'
        string += f"Expected upper limit (+/-2 sigma) (exp): mu = ["
        for i in range(4):
            string+=f"{exp_limits[i]:.4f}, "
        string+=f"{exp_limits[4]:.4f}]"                                                               
    
    #for i in range(5):
    #    string=string+f"Upper limit ({-2+i:2d} σ) (exp): μ = {exp_limits[i]:.4f}"+'\n'
    #[f"Upper limit ({-2+i:2d} σ) (exp): μ = {exp_limits[2]:.4f}" for i in range(4) ]
    
    string += "\n================================\n"
    with open(path+'/multibin_limits/'+"results.dat", "a") as write_file:
        print(string, file = write_file)
    if full:
        return obs_limit
    else:
        return result[0]
