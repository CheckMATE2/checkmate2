import json
import numpy as np
import pyhf
import matplotlib.pyplot as plt
import os
import sys
import scipy
from info import Info
from advprint import AdvPrint

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
    for sreg in names:
        for sr in SRs:
            if sr['SR'] == sreg:
                b.append(sr['b'])  
                db.append(sr['db'])  
                o.append(round(sr['o'])) 
                s.append(max(sr['s'],0.0))  
                ds.append(max(min(sr['s'],sr['ds']),0.0001)) 
                break
    '''    
    for sr in SRs:
        if sr['SR'] in names:
            b.append(sr['b'])  
            db.append(sr['db'])  
            o.append(round(sr['o'])) 
            s.append(max(sr['s'],0.0))  
            ds.append(max(min(sr['s'],sr['ds']),0.0001)) 
    '''        
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
    	#result = pyhf.infer.hypotest(test_poi, workspace.data(model), model, test_stat = "q", return_expected_set = True)
        if Info.flags["expected"]:
            cls_obs, cls_exp = pyhf.infer.hypotest(test_poi, workspace.data(model), model, return_expected_set = True, test_stat="q")
        else:    
            cls_obs = pyhf.infer.hypotest(test_poi, workspace.data(model), model, return_expected = False, test_stat="q")
            cls_exp = 1.
    else:
    	cls_obs, cls_exp = pyhf.infer.hypotest(test_poi, workspace.data(model), model,
    		test_stat = "q",
    		return_expected_set = True,
    		calctype = "toybased",
    		ntoys = ntoys,
    		track_progress = True)   
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
    poi_values = np.logspace(np.log10(0.001),np.log10(9),100)
    model = workspace.model()
    if ntoys <= 0:
    	#obs_limit, exp_limits, (scan, results) = pyhf.infer.intervals.upperlimit(workspace.data(model), model, poi_values, level=0.05, return_results=True,test_stat="q")
    	obs_limit, exp_limits, (scan, results) = pyhf.infer.intervals.upper_limits.upper_limit(workspace.data(model), model, poi_values, level=0.05, return_results=True, test_stat="q")
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

def upperlim_with_cov(_o, _b, _db, _s, _ds, _cov, calculator, sigconstraint):

    import ROOT
    from array import array
    #ROOT.Math.MinimizerOptions.SetDefaultMinimizer("Minuit2") #minuit 2 instead of minuit
    #Prepare variables for using in ROOSTAT
    nbins = len(_o)
    s = array('d',_s)
    b = array('d',_b)
    db = array('d',_db)
    ds = array('d',_ds)
    o = array('d',_o)
    cov = ROOT.TMatrixDSym(nbins,np.array(_cov))
    for i in range(nbins): 
        for j in range(nbins): 
            cov[i][j]=_cov[i,j]  
    pdf_string=''
    # Create workspace
    wspace= ROOT.RooWorkspace()
    wspace.factory("mu[1,-1,5]") # create the variable mu to store the POI

    # Define, for each bin :
    # 1. The variable nexpi corresponding to the argument of the poisson distribution: sum of  mu*s_i + b0_i+thb_i where s_i is the free-floating signal (constrained by constraints_i), b0_ is the median value (constant) of the background prediction and thb_i is the variation of bi around b0_i (constrained by constraintb).
    # 2. A gaussian constraint for s_i (nuissance parameters) centered around s0_i (constant global observables) and with variance fixed by ds (number).
    # 3. A poisson distribution with arguments nobsi (observables) and nexpi. 
    # 4. The variable zero_i to hold the target value of the nuissance parameters thb_i, treated as a global obsevable.
    #  Define for all bins:
    # 1. A multivariate gaussian constraint term for all of the thb_i with thb_i as random variable, zero_i as mean and covariance matrix defined by cov
    # 2. The model p.d.f as the product of al poisson and constraint terms.
    for i in range(nbins):
        if sigconstraint:
            wspace.factory("sum::nexp"+str(i)+"(0.0,prod::muS"+str(i)+"(mu,s_"+str(i)+"["+str(s[i])+",0,1e+30]),b0_"+str(i)+"["+str(b[i])+"],thb_"+str(i)+"["+str(o[i]-b[i]+0.1)+","+str(-b[i]+0.01)+","+str(5*db[i])+"])")
            wspace.factory("Gaussian::constraints"+str(i)+"(s_"+str(i)+",s0_"+str(i)+"["+str(s[i])+",0,1e+7],"+str(ds[i])+")")
        else:
            wspace.factory("sum::nexp"+str(i)+"(0.0,prod::muS"+str(i)+"(mu,"+str(s[i])+"),b0_"+str(i)+"["+str(b[i])+"],thb_"+str(i)+"["+str(o[i]-b[i]+0.1)+","+str(-b[i]+0.01)+","+str(5*db[i])+"])")
        wspace.factory("RooPoisson::poiss"+str(i)+"(nobs"+str(i)+"["+str(o[i])+"],nexp"+str(i)+")")
        pdf_string = pdf_string+"poiss"+str(i)+","
        if sigconstraint:
            pdf_string = pdf_string + "constraints"+str(i)+","
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
        wspace.var("thb_0_"+str(i)).setVal(0)
        wspace.var("thb_0_"+str(i)).setConstant(True)
        wspace.var("nobs"+str(i)).setVal(o[i])
        wspace.var("nobs"+str(i)).setConstant(True)
        if sigconstraint:
            wspace.var("s0_"+str(i)).setVal(s[i])
            wspace.var("s0_"+str(i)).setConstant(True)
            wspace.var("s_"+str(i)).setMax(s[i]+3*ds[i])
            wspace.var("s_"+str(i)).setMin(0)
    ## Define sets of variables: data, constrained parameters and global observables
    wspace.defineSet("obs",ROOT.RooArgSet([wspace.var("nobs"+str(i)) for i in range(nbins) ]))
    data = ROOT.RooDataSet("data",'data',wspace.set("obs"))
    data.add(wspace.set("obs"))
    if sigconstraint:
        wspace.defineSet("const_p",ROOT.RooArgSet([wspace.var("thb_"+str(i)) for i in range(nbins) ]+[wspace.var("s_"+str(i)) for i in range(nbins) ]))
    else:
        wspace.defineSet("const_p",ROOT.RooArgSet([wspace.var("thb_"+str(i)) for i in range(nbins) ]))
    constrainedParams=ROOT.RooDataSet("constrainedParams",'constrainedParams',wspace.set("const_p"))
    constrainedParams.add(wspace.set("const_p"))

    ## Create a model for the alternate (mu=1) and null (mu=0) hypotesis.
    sbModel = ROOT.RooStats.ModelConfig("sbModel","",wspace)
    sbModel.SetPdf(wspace.pdf("model"))
    sbModel.SetParametersOfInterest(wspace.var("mu"))
    sbModel.SetNuisanceParameters(wspace.set("const_p"))
    sbModel.SetObservables(wspace.set("obs"))
    wspace.var("mu").setVal(1.)
    sbModel.SetSnapshot(wspace.var("mu"))
    getattr(wspace,'import')(sbModel)
    getattr(wspace,'import')(data)
    bModel = ROOT.RooStats.ModelConfig("bModel","",wspace)
    bModel.SetPdf(wspace.pdf("model"))
    bModel.SetParametersOfInterest(wspace.var("mu"))
    bModel.SetNuisanceParameters(wspace.set("const_p"))
    bModel.SetObservables(wspace.set("obs"))
    wspace.var("mu").setVal(0) 
    bModel.SetSnapshot(wspace.var("mu"))
    getattr(wspace,'import')(bModel)
    poi=sbModel.GetParametersOfInterest().first()
    #wspace.Print()
    ROOT.Math.MinimizerOptions.SetDefaultPrintLevel(-1) #<< Change the messages level, smaller means less info.
    ROOT.RooMsgService.instance().setGlobalKillBelow(ROOT.RooFit.FATAL)

    if calculator == "PLLC":
        pl = ROOT.RooStats.ProfileLikelihoodCalculator(data,sbModel)
        #pl.SetAlternateParameters(sbModel.GetSnapshot())
        #pl.SetNullParameters(bModel.GetSnapshot())
        pl.SetConfidenceLevel(0.95)
        hypotest = pl.GetHypoTest()
        clb = hypotest.NullPValue()
        myPOI = sbModel.GetParametersOfInterest().first()
        myPOI.setConstant(False)
        interval = pl.GetInterval()
        print("Best fit: "+str(interval.GetBestFitParameters().getRealValue("mu"))+" +/- "+str(myPOI.getError()))
        upperLimit_exp = [5.,5.,5.,5.,5.]
        upperLimit = interval.UpperLimit(myPOI)
        print("Upper limit: " + str(upperLimit))
        '''
        # make plots for profile likelihood
        plot = ROOT.RooStats.LikelihoodIntervalPlot(interval)
        c = ROOT.TCanvas("Scan") 
        plot.SetNPoints(50)
        plot.SetRange(0., 2.)
        plot.SetMaximum(3.)
        plot.Draw()
        c.SaveAs("plot"+str(o[0])+"_2.pdf")
        '''
    elif calculator == "ASYMP":    
        asympCalc = ROOT.RooStats.AsymptoticCalculator(data,bModel,sbModel)
        asympCalc.SetOneSided(True)
        asympCalc.SetPrintLevel(0)
        hypo = asympCalc.GetHypoTest()
        print("CLs: "+str(1./hypo.CLs())) 
        #hypo.Print()
        calc = ROOT.RooStats.HypoTestInverter(asympCalc)
        calc.SetVerbose(True)
        calc.SetConfidenceLevel(0.95)
        useCLs = True
        calc.UseCLs(useCLs)
        npoints = 50
        poimin = max(poi.getMin(),0.1)
        poimax = poi.getMax()
        #calc.SetFixedScan(npoints, poimin, poimax)
        r = calc.GetInterval()
        upperLimit = r.UpperLimit()
        ulError = r.UpperLimitEstimatedError()
        print("Upper limit: " + str(upperLimit) + " +/- " + str(ulError))
        upperLimit_exp=[r.GetExpectedUpperLimit(-2),r.GetExpectedUpperLimit(-1),r.GetExpectedUpperLimit(0),r.GetExpectedUpperLimit(1),r.GetExpectedUpperLimit(2)]
        print(upperLimit_exp)
        ''' plot result of the scan CLs vs mu
        if False:   
            plot2 = ROOT.RooStats.HypoTestInverterPlot("HTI_Result_Plot", "CLs upper limit", r)
            c2 = ROOT.TCanvas("HypoTestInverter Scan") 
            c2.SetLogy(False)
            plot2.Draw("2CL")
            c2.SaveAs("SimpleCLsLimit.pdf") 
        '''        
    else:
        upperLimit = 5.
        upperLimit_exp = [5.,5.,5.,5.,5.]
        
    
    return upperLimit, upperLimit_exp

#------------------------------------------------------------------

def get_cov(analysis, db, corrmat = False):
    global hepfiles_folder
    hepfiles_folder = Info.paths['data']+"/"   #<----Set the path of the folder with the models here.
    
    if corrmat and os.path.isfile(hepfiles_folder+analysis+"/corr.json"):
        with open(hepfiles_folder+analysis+"/corr.json") as serialized:
            corr = json.load(serialized)
        corr_mat=np.identity((int(np.sqrt(len(corr["values"]))),int(np.sqrt(len(corr["values"])))))
        for i in range(len(corr["values"])):
            corr_mat[int(float(corr["values"][i]["x"][0]["value"])-0.5),int(float(corr["values"][i]["x"][1]["value"])-0.5)]=float(corr["values"][i]["y"][0]["value"])
            corr_mat[int(float(corr["values"][i]["x"][1]["value"])-0.5),int(float(corr["values"][i]["x"][0]["value"])-0.5)]=float(corr["values"][i]["y"][0]["value"])
        cov_mat=np.identity((int(np.sqrt(len(corr["values"]))),int(np.sqrt(len(corr["values"])))))
        for i in range(corr_mat.shape[0]):
            for j in range(corr_mat.shape[0]):
                    cov_mat[i,j]=corr_mat[i,j]*db[i]*db[j]
    elif os.path.isfile(hepfiles_folder+analysis+"/cov.json"):
        with open(hepfiles_folder+analysis+"/cov.json") as serialized:
            cov = json.load(serialized)
        if analysis == "cms_sus_19_005":
            size = 282
        else:
            size = int(np.sqrt(len(cov["values"])))
        cov_mat = np.identity(size)
        offset = float(cov["values"][0]["x"][0]["value"])
        #print(offset)
        #print(len(cov["values"]))
        for i in range(len(cov["values"])):
            cov_mat[int(float(cov["values"][i]["x"][0]["value"])-offset),int(float(cov["values"][i]["x"][1]["value"])-offset)]=float(cov["values"][i]["y"][0]["value"])
            cov_mat[int(float(cov["values"][i]["x"][1]["value"])-offset),int(float(cov["values"][i]["x"][0]["value"])-offset)]=float(cov["values"][i]["y"][0]["value"])
    else:
        AdvPrint.cerr_exit("Missing covariance matrix!")
        
    return cov_mat


#Wraps around the fucntions to calculate the upper limit for the data in the CheckMATE results folder pointed through 'path' and the set of bins in 'names', then stores the results in  'path/multibin_limits/'.
#'names' must be a list of SRs names with the exact same notation as in 'path/evaluation/total_results.txt'.
# if ntoys greater than 0 changes to toy based calculation instead of the asymptotic aproximation, with ntoys number of "toys".
# 'systematics' is the relative systematic uncertanty of the signal (expressed as ratio ds/s, not as percentage). Default is 0.
# 'lumi is the uncertanty in the integrated luminosity, default value is 0.017 (1.7%).
def calc_point(path, names, analysis, mbsr, systematics = 0, lumi = 0.017, ntoys = -1):
    obs_limit = 10.
    exp_limits = [10.,10.,10.,10.,10.]
    cls_obs = 1.
    cls_exp = [1.,1.,1.,1.,1.]
    string = "================================\n Analysis: "+analysis+" , SR: "+mbsr+"\n"
    string += f"Limits with simplified likelihood (pyhf):\n"
    SRs = data_from_CMresults(path)
    o, b, db, s, ds = select_MBsr(names, SRs)
    workspace = create_workspace(path, o, b, db, s, ds, systematics, lumi)
    if Info.flags["mbcls"]:
        AdvPrint.cout("Observed:")
        cls_obs, cls_exp = hypotest(workspace, ntoys)
        AdvPrint.cout("CL95: "+str(cls_obs) )
        string += f"Observed CLs for mu = 1: {cls_obs}"+'\n'
        if Info.flags["expected"]:
            string = string+f"Expected CLs band for mu = 1: {[exp.tolist() for exp in cls_exp]}"+'\n'
    if Info.flags["uplim"]:
        AdvPrint.cout("Observed:")
        poi_values, obs_limit, exp_limits, (scan, results) = upperlim(workspace, ntoys)	
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
    return obs_limit, exp_limits[2], cls_obs, cls_exp



def calc_cov(path, names, analysis, mbsr, calculator = "ASYMP", corrmat = False, sigconstraint = True, systematics = 0, lumi = 0.017):
    # calculator = "PLLC"
    # calculator = "ASYMP"
    # corrmat = False
    # corrmat = True
    # in 1908_04722 corr = True chooses correlation matrix and aggressive error treatment; otherwise does nothing
    # sigconstraint: whether to include Gaussian constraints on signal numbers
    
    os.system("mkdir -p "+path+'/multibin_limits')
    
    string = "================================\n Analysis: "+analysis+" , SR: "+mbsr+"\n"
    SRs = data_from_CMresults(path)
    o, b, db, s, ds = select_MBsr(names, SRs)
    

    cov_mat = get_cov(analysis, corrmat)
        
    obs_limit, exp_limits = upperlim_with_cov(o, b, db, s, ds, cov_mat, calculator, sigconstraint)
    #exp_limit, clb_exp = upperlim_with_cov(b, b, db, s, ds, cov_mat)
    obs_limit, exp_limits = upperlim_with_cov(o, b, db, s, ds, cov_mat, "PLLC", sigconstraint)
    
    string += f"Limits with covariance matrix (method = " + calculator + ", correlation matrix = "+ str(corrmat) + "):\n\n"
    if obs_limit < 5.:
        string += f"Upper limit (observed): mu = {obs_limit:.10f}"+'\n'
    else:
        string += f"Calculation of upper limit failed or the upper limit out of range\n"
    if calculator == "ASYMP" and exp_limits[2] < 5.:
        #string += f"Upper limit (expected): mu = {exp_limit[2]:.10f}"+'\n'   
        string += f"Expected upper limit (+/-2 sigma) (exp): mu = ["
        for i in range(4):
            string += f"{exp_limits[i]:.4f}, "
        string += f"{exp_limits[4]:.4f}]"
    elif calculator == "ASYMP" and exp_limits[2] >= 5.:    
        string += f"Calculation of upper limit failed or the upper limit out of range\n"
    string += "\n================================\n"
    with open(path+'/multibin_limits/'+"results.dat", "a") as write_file:
        write_file.write(string)
        
    return obs_limit
