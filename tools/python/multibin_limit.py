import json
import numpy as np
import pyhf
import os
import sys
import scipy
from info import Info

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
# Root based calculation of the upper limits
def upperlim_with_cov(_o,_b,_s,_ds,_cov,calculator):
    import os, sys
    import ROOT
    from array import array
    #Prepare variables fro using in ROOSTAT
    nbins=len(_o)
    s=array('d',_s)
    b=array('d',_b)
    ds=array('d',_ds)
    o=array('d',_o)
    cov=ROOT.TMatrixDSym(nbins,np.array(_cov))
    for i in range(nbins): 
        for j in range(nbins): 
            cov[i][j]=_cov[i,j]  
    pdf_string=''
    # Create workspace
    wspace= ROOT.RooWorkspace()
    wspace.factory("mu[1,0,5]") # create the variable mu to store the POI
    if calculator=='Asypmtotic':
        # Define, for each bin :
        # 1. The variable nexpi corresponding to the argument of the poisson distribution: sum of  mu*s_i + b0_i+V[j,i]*thb_i where s_i is the free-floating signal (constrained by constraints_i), b0_ is the median value (constant) of the background prediction , thb_i are the rotated nuisance parameters (constrained by constraintb) and V is the orthogonal matrix that diagonalizes inv(cov).
        # 2. A gaussian constraint for thb_i (nuissance parameters) centered around thb_0_i (constant global observables = 0) and with variance fixed by np.sqrt(1/v_[i,i]) (where v is the covariance matrix rotates to the diagonal basis).
        # 3. A poisson distribution with arguments nobsi (observables) and nexpi. 
        # 4. A gaussian constraint for s_i (nuissance parameters) centered around s0_i (constant global observables) and with variance fixed by ds (number).
        #  Define for all bins:
        # 2. The model p.d.f as the product of al poisson and constraint terms.
        from numpy import linalg 
        v,w= linalg.eig(linalg.inv(_cov))
        V=np.matrix(w)
        V=np.diag(np.ones(nbins))
        v_=np.matmul(V.transpose(),np.matmul(linalg.inv(_cov),V))
    # Define, for each bin :
        for i in range(nbins):         
                wspace.factory("sum::nexp"+str(i)+"(prod::muS"+str(i)+"(mu,s_"+str(i)+"["+str(s[i])+"]),b0_"+str(i)+"["+str(b[i])+"],sum::thb_sum"+str(i)+"("+(''.join([",prod::thb_prod"+str(i)+"_"+str(j)+"("+str(V[j,i])+",thb_"+str(j)+"["+str(0)+","+str(-1/min(v))+","+str(1/min(v))+"])" for j in range(nbins)]))[1:]+"))")
                wspace.factory("Gaussian::constraints"+str(i)+"(s_"+str(i)+",s0_"+str(i)+"["+str(s[i])+",0,1e+7],"+str(ds[i])+")")
                wspace.factory("RooPoisson::poiss"+str(i)+"(nobs"+str(i)+"["+str(o[i])+"],nexp"+str(i)+")")
                wspace.factory("Gaussian::constraintb"+str(i)+"(thb_0_"+str(i)+"[0],thb_"+str(i)+","+str(np.sqrt(1/v_[i,i]))+")")
                pdf_string=pdf_string+"poiss"+str(i)+","+",constraintb"+str(i)+","
        ##
        wspace.factory("PROD:model("+pdf_string[:-1]+")")
    else:
        # Define, for each bin :
        # 1. The variable nexpi corresponding to the argument of the poisson distribution: sum of  mu*s_i + b0_i+thb_i where s_i is the free-floating signal (constrained by constraints_i), b0_ is the median value (constant) of the background prediction and thb_i is the variation of bi around b0_i (constrained by constraintb).
        # 2. A gaussian constraint for s_i (nuissance parameters) centered around s0_i (constant global observables) and with variance fixed by ds (number).
        # 3. A poisson distribution with arguments nobsi (observables) and nexpi. 
        # 4. The variable zero_i to hold the target value of the nuissance parameters thb_i, treated as a global obsevable.
        #  Define for all bins:
        # 1. A multivariate gaussian constraint term for all of the thb_i with thb_i as random variable, zero_i as mean and covariance matrix defined by cov
        # 2. The model p.d.f as the product of al poisson and constraint terms.
        for i in range(nbins):           
                wspace.factory("sum::nexp"+str(i)+"(prod::muS"+str(i)+"(mu,s_"+str(i)+"["+str(s[i])+"0,1e+30]),b0_"+str(i)+"["+str(b[i])+"],thb_"+str(i)+"["+str(0)+","+str(-b[i])+","+str(b[i])+"])")
                wspace.factory("Gaussian::constraints"+str(i)+"(s_"+str(i)+",s0_"+str(i)+"["+str(s[i])+",0,1e+7],"+str(ds[i])+")")
                wspace.factory("RooPoisson::poiss"+str(i)+"(nobs"+str(i)+"["+str(o[i])+"],nexp"+str(i)+")")
                pdf_string=pdf_string+"poiss"+str(i)+",constraints"+str(i)+","
                wspace.factory("thb_0_"+str(i)+"[0]")#wspace.factory("zero_"+str(i)+"[0,-1e-10,1e-10]")
                wspace.var("thb_0_"+str(i)).setConstant()        
        constraintb=ROOT.RooMultiVarGaussian("constraintb","constraintb",ROOT.RooArgSet([wspace.var("thb_"+str(i)) for i in range(nbins)]),ROOT.RooArgSet([wspace.var("thb_0_"+str(i)) for i in range(nbins)]),cov)
        wspace.Import(constraintb)
        ##
        pdf_string=pdf_string+"constraintb"
        wspace.factory("PROD:model("+pdf_string+")")
    ## Set the observables and global obsevables as constants and fix their values, put reasonable limits on s to optimize calculation
    for i in range(nbins):
        wspace.var("b0_"+str(i)).setVal(b[i])
        wspace.var("b0_"+str(i)).setConstant(True)
        wspace.var("s0_"+str(i)).setVal(s[i])
        wspace.var("s0_"+str(i)).setConstant(True)
        wspace.var("thb_0_"+str(i)).setVal(0)
        wspace.var("thb_0_"+str(i)).setConstant(True)
        wspace.var("s_"+str(i)).setMax(s[i]+5*ds[i])
        wspace.var("s_"+str(i)).setMin(0)
        wspace.var("nobs"+str(i)).setVal(o[i])
        wspace.var("nobs"+str(i)).setConstant(True)
    ## Define sets of variables: data, constrained parameters and global observables
    wspace.defineSet("obs",ROOT.RooArgSet([wspace.var("nobs"+str(i)) for i in range(nbins) ]))
    data=ROOT.RooDataSet("data",'data',wspace.set("obs"))
    data.add(wspace.set("obs"))
    wspace.defineSet("const_p",ROOT.RooArgSet([wspace.var("thb_"+str(i)) for i in range(nbins) ]+[wspace.var("s_"+str(i)) for i in range(nbins) ]))
    constrainedParams=ROOT.RooDataSet("constrainedParams",'constrainedParams',wspace.set("const_p"))
    constrainedParams.add(wspace.set("const_p"))
    wspace.defineSet("glob_obs",ROOT.RooArgSet([wspace.var("s0_"+str(i)) for i in range(nbins) ]+[wspace.var("thb_0_"+str(i)) for i in range(nbins) ]))
    globalobservables=ROOT.RooDataSet("globalobservables",'globalobservables',wspace.set("glob_obs"))
    globalobservables.add(wspace.set("glob_obs"))
    ## Create a model for the alternate (mu=1) and null (mu=0) hypotesis.
    sbModel=ROOT.RooStats.ModelConfig("sbModel","",wspace)
    sbModel.SetPdf(wspace.pdf("model"))
    sbModel.SetParametersOfInterest(wspace.var("mu"))
    sbModel.SetNuisanceParameters(wspace.set("const_p"))
    sbModel.SetObservables(wspace.set("obs"))
    sbModel.SetGlobalObservables(wspace.set("glob_obs"))
    wspace.var("mu").setVal(1)
    sbModel.SetSnapshot(wspace.var("mu"))
    getattr(wspace,'import')(sbModel)
    getattr(wspace,'import')(data)
    bModel=ROOT.RooStats.ModelConfig("bModel","",wspace)
    bModel.SetPdf(wspace.pdf("model"))
    bModel.SetParametersOfInterest(wspace.var("mu"))
    bModel.SetNuisanceParameters(wspace.set("const_p"))
    bModel.SetObservables(wspace.set("obs"))
    bModel.SetGlobalObservables(wspace.set("glob_obs"))
    wspace.var("mu").setVal(0) 
    bModel.SetSnapshot(wspace.var("mu"))
    getattr(wspace,'import')(bModel)
    poi=sbModel.GetParametersOfInterest().first()


##  Choose the type of calculator:
#   1.Asymptotic (not valid with multivariate constraint terms in the likelihood)
#   2.Hybrid: A frequentist sampling of the test statistic distribution is obtained but with mariginalization of the nuisance parameters. The toys are generated by sampling the nuisance parameters according to their prior distribution. 
#   3.Frequentist: Hypothesis Test Calculator using a full frequentist procedure for sampling the test statistic distribution.The nuisance parameters are fixed to their MLEs
#
#  Hybrid and frequentist require to define the "toys" to use. The test statistic is set to profile likelihood ratio.
#  The number of toys for alternate, null hypotesis is set with hCalc.SetToys(ntoys_alternate,ntoys_null)
#  
#  Create a HypotestInverter object to  compute a confidence interval.
#
    if calculator=='Asypmtotic':
        asympCalc = ROOT.RooStats.AsymptoticCalculator(data,bModel,sbModel)
        asympCalc.SetOneSided(True)
        asympCalc.SetPrintLevel(-1000)
        calc=ROOT.RooStats.HypoTestInverter(asympCalc)
#
    elif calculator=='Hybrid':
        hCalc = ROOT.RooStats.HybridCalculator(data,bModel,sbModel)
        toymcs = hCalc.GetTestStatSampler()
        toymcs.SetNEventsPerToy(1)
        hCalc.SetToys(1000,1000)
        if ((sbModel.GetNuisanceParameters()) and (sbModel.GetNuisanceParameters().getSize() > 0)):
            nuisPdf = ROOT.RooStats.MakeNuisancePdf(sbModel,"nuisancePdf_sbmodel")
            hCalc.ForcePriorNuisanceAlt(nuisPdf)
            hCalc.ForcePriorNuisanceNull(nuisPdf)   
        profll=ROOT.RooStats.ProfileLikelihoodTestStat(sbModel.GetPdf())
        profll.SetOneSided(True)
        toymcs.SetTestStatistic(profll)
        calc=ROOT.RooStats.HypoTestInverter(hCalc)
#
    elif calculator=='Frequentist':
        fCalc = ROOT.RooStats.FrequentistCalculator(data,bModel,sbModel)
        toymcs = fCalc.GetTestStatSampler()
        toymcs.SetNEventsPerToy(1)
        fCalc.SetToys(1000,1000)
        profll=ROOT.RooStats.ProfileLikelihoodTestStat(sbModel.GetPdf())
        profll.SetOneSided(True)
        toymcs.SetTestStatistic(profll)
        calc=ROOT.RooStats.HypoTestInverter(fCalc)
#
    calc.SetVerbose(False)
    calc.SetConfidenceLevel(0.95)
    useCLs = True
    calc.UseCLs(useCLs)
    npoints = 50
    poimin = max(poi.getMin(),0.1)
    poimax = poi.getMax()
    calc.SetFixedScan(npoints, poimin, poimax)
    r = calc.GetInterval()
    upperLimit = r.UpperLimit()
    ulError = r.UpperLimitEstimatedError() 
    upperLimit_exp=[r.GetExpectedUpperLimit(-2),r.GetExpectedUpperLimit(-1),r.GetExpectedUpperLimit(0),r.GetExpectedUpperLimit(1),r.GetExpectedUpperLimit(2)]
    return upperLimit,upperLimit_exp



##--------------------------------------------------------------------------------------

#Wraps around the fucntions to calculate the upper limit for the data in the CheckMATE results folder pointed through 'path' and the set of bins in 'names', then stores the results in  'path/multibin_limits/'.
#'names' must be a list of SRs names with the exact same notation as in 'path/evaluation/total_results.txt'.
# if ntoys greater than 0 changes to toy based calculation instead of the asymptotic aproximation, with ntoys number of "toys".
# 'systematics' is the relative systematic uncertanty of the signal (expressed as ratio ds/s, not as percentage). Default is 0.
# 'lumi is the uncertanty in the integrated luminosity, default value is 0.017 (1.7%).
def calc_point(path, names, analysis, mbsr, systematics = 0, lumi = 0.017, ntoys = -1):
    
    global hepfiles_folder

    hepfiles_folder = Info.paths['data']+"/"     
    
    string = "================================\n Analysis: "+analysis+" , SR: "+mbsr+"\n"
    SRs = data_from_CMresults(path)
    o, b, db, s, ds = select_MBsr(names, SRs)
    
    with open(hepfiles_folder+analysis+"/cov.json") as serialized:
        cov = json.load(serialized)
    cov_mat=np.ones((int(np.sqrt(len(cov["values"]))),int(np.sqrt(len(cov["values"])))))
    for i in range(len(cov["values"])):
        cov_mat[int(float(cov["values"][i]["x"][0]["value"])-0.5),int(float(cov["values"][i]["x"][1]["value"])-0.5)]=float(cov["values"][i]["y"][0]["value"])
    '''
    with open(hepfiles_folder+analysis+"/corr.json") as serialized:
        corr = json.load(serialized)
    corr_mat=np.ones((int(np.sqrt(len(corr["values"]))),int(np.sqrt(len(corr["values"])))))
    for i in range(len(corr["values"])):
        corr_mat[int(float(corr["values"][i]["x"][0]["value"])-0.5),int(float(corr["values"][i]["x"][1]["value"])-0.5)]=float(corr["values"][i]["y"][0]["value"])
    cov_post_mat=np.ones((int(np.sqrt(len(corr["values"]))),int(np.sqrt(len(corr["values"])))))
    for i in range(corr_mat.shape[0]):
        for j in range(corr_mat.shape[0]):
            cov_post_mat[i,j]=corr_mat[i,j]*db[i]*db[j]
    '''
    # ----------------------------------    
    workspace = create_workspace(path, o, b, db, s, ds, systematics, lumi)
    result = hypotest(workspace, ntoys)
    poi_values, obs_limit, exp_limits, (scan, results) = upperlim(workspace, ntoys)
    string += f"Observed CLs for mu = 1: {result[0]}"+'\n'
    string = string+f"Expected CLs band for mu = 1: {[exp.tolist() for exp in result[1]]}"+'\n'
    string = string+f"Upper limit (obs): mu = {obs_limit:.4f}"+'\n'
    string += f"Expected upper limit (+/-2 sigma) (exp): mu = ["
    for i in range(4):
        string += f"{exp_limits[i]:.4f}, "
    string += f"{exp_limits[4]:.4f}]"    
    string += "\n================================\n"
    ##  ------------------   this part calculates the limits using the covariance matrix   ------------------ #
    #
    #calculator='Hybrid'
    calculator='Asypmtotic'
    #calculator='Frequentist'
    obs_limit_cov, exp_limits_cov = upperlim_with_cov(o,b,s,ds,cov_mat,calculator)   #<<<<<<<< This line returns the upper limit calculated with prefit covariance matrix 
    #obs_limit_cov_post, exp_limits_cov_post = upperlim_with_cov(o,b,s,ds,cov_post_mat,calculator)  #<<<<<<<< This line returns the upper limit calculated with prefit correlation matrix times postfit errors 
    
    string += f"Limits with  pre-fit covariance matrix:\n\n"
    string += f"Upper limit (obs): mu = {obs_limit_cov:.10f}"+'\n'
    string += f"Expected upper limit (+/-2 sigma) (exp): mu = ["   
    for i in range(4):
        #string += f"Upper limit ({2*(i-1):2d} σ) (exp): μ = {exp_limits_cov[i]:.4f}"+'\n'  
        string += f"{exp_limits_cov[i]:.4f}, "  
    string += f"{exp_limits_cov[4]:.4f}]"    
    string += "\n================================\n"
    #string += f"Limits with post-fit normalized covariance matrix:\n\n"
    #string += f"Upper limit (obs): mu = {obs_limit_cov_post:.10f}"+'\n'
    #string += f"Expected upper limit (+/-2 sigma) (exp): mu = ["   
    #for i in range(4):
        #string += f"Upper limit ({2*(i-1):2d} σ) (exp): μ = {exp_limits_cov[i]:.4f}"+'\n'  
    #    string += f"{exp_limits_cov_post[i]:.4f}, "  
    #string += f"{exp_limits_cov_post[4]:.4f}]"    
    #string += "\n================================\n"
    # ------------------------------------------------------------------#
    with open(path+'/multibin_limits/'+"results.dat", "a") as write_file:
        write_file.write(string)
        #print(string,file=write_file)
    return True
    #return obs_limit
