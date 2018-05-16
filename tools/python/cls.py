#!/usr/bin/env python

"""Calculates CLs for simple one bin counting experiments.

CLs is calculated using the standard LHC techniques using a profile likelihood 
for the test statistic.
"""

__author__ = "Daniel Schmeier, Jamie Tattersall, Sebastian Belkner"
__copyright__ = "Copyright 2014, CheckMATE"
__credits__ = ["Daniel Schmeier"]
__license__ = "GPL"
__version__ = "1.0.2"
__maintainer__ = "Daniel Schmeier"
__email__ = "daschm@uni-bonn.de"
__status__ = "Prototype"


from math import *
import os, sys, scipy
import numpy as np
from math import log, exp, sqrt
import os, sys
import scipy.optimize as opt
# Note: Scipy packages are imported within the code, using a try-except setup
  


def nexp(mu, nu_b, nu_s):
  """
  Returns expected number of events given mu and nu
  expected = signal + background.
  Signal is multiplied with modifier mu.
  Signal is 'smeared' by corresponding nu_s.
  Background is 'smeared' by corresponding nu_b.
  The smearing is lognormal, which is performed by taking
    the exponential of a normal distributed random number (nu).
  Note that b0, s0, db and ds are global variables defined at input
  """
  return b0*exp(db/b0*nu_b) + mu*s0*exp(ds/s0*nu_s)

def eqForMaxL((nu_b, nu_s), n, nuTil_b, nuTil_s, mu):
  """
  Helper function to maximise likelihood 
  The nu_b, nu_s for which these functions are zero maximise the likelihood 
  """
  lam = nexp(mu, nu_b, nu_s)
  return ((n-lam)*db*exp(db/b0*nu_b)-lam*(nu_b-nuTil_b), (n-lam)*ds*mu*exp(ds/s0*nu_s)-lam*(nu_s-nuTil_s))
  #return ((n-lam)*db*exp(db/b0*nu_b)-lam*(nu_b-nuTil_b), db*exp(db/b0*nu_b-ds/s0*nu_s)*(nu_s-nuTil_s)-ds*mu*(nu_b-nuTil_b))

def nuWithMaxL(n, nuTil_b, nuTil_s, mu):
  """ Finds root of eqForMaxL to maximise Likelihood   """  
  global fsolve
  return fsolve(eqForMaxL, (nuTil_b, nuTil_s), (n, nuTil_b, nuTil_s, mu))

def reduced_logp(nuTil, nu):
  """ reduced_logp = log(p) without normalisation """
  return -(nuTil-nu)**2./2.

def qMu(n, nuTil_b, nuTil_s):  
  """ Test statisctic for signal+background hypothesis """

  # First, overall maximum is easy to calculate (b0, s0, db are defined during input)
  mu_hat = (n-b0*exp(db/b0*nuTil_b))/(s0*exp(ds/s0*nuTil_s))
  nu_b_hat = nuTil_b
  nu_s_hat = nuTil_s

  # mu_hat > 1 should result in zero to ensure one sided limit
  if mu_hat > 1:
    return 0
  # if mu_hat < 0, fix mu_hat = 0 and re-evaluate best nu_b
  elif mu_hat < 0:
    mu_hat = 0
    nu_b_hat, nu_s_hat = nuWithMaxL(n, nuTil_b, nuTil_s, 0.)    
    
  # Now, Maximise with mu(here: 1) fixed
  nu_b_mu_hat, nu_s_mu_hat = nuWithMaxL(n, nuTil_b, nuTil_s, 1.)
    
  # Having this, formulate the profile likelihood  
  # First, evaluate lambda for numerator and denominator
  lam_hat_hat = nexp(mu_hat, nu_b_hat, nu_s_hat)
  lam_hat = nexp(1., nu_b_mu_hat, nu_s_mu_hat)
  
  # If any lambda is almost zero, it should be set to zero
  
  if (lam_hat < 0) and (fabs(lam_hat) < 1E-5):
    lam_hat = 0
  if (lam_hat_hat < 0) and (fabs(lam_hat_hat) < 1E-5):
    lam_hat_hat = 0
    
  # If any lambda is really negative, there has been a problem somewhere
  if (lam_hat < 0) or (lam_hat_hat < 0):
    exit("ERROR: In S95 calculation, profile likelihood asked for nexp < 0!")
    
  result = 0
  # If any lambda is zero,...
  if lam_hat == 0 or lam_hat_hat == 0:
    # ... both lambdas must be zero (or so small that their product is 0)
    if lam_hat != 0 or lam_hat_hat != 0:
      exit("ERROR: In S95 calculation, profile likelihood asked for impossible maximisation parameters! ("+str((b0, s0, db, ds))+")")
    # In that case, the result is only the likelihood w.r.t the nuisance parameters
    result = -2.*(reduced_logp(nuTil_b, nu_b_mu_hat) + reduced_logp(nuTil_s, nu_s_mu_hat) - reduced_logp(nuTil_b, nu_b_hat) - reduced_logp(nuTil_s, nu_s_hat))
  else:
    # Otherwise, return the normal likelihood    
    result = -2.*(n*log(lam_hat/lam_hat_hat) - (lam_hat-lam_hat_hat) + reduced_logp(nuTil_b, nu_b_mu_hat) + reduced_logp(nuTil_s, nu_s_mu_hat) - reduced_logp(nuTil_b, nu_b_hat) - reduced_logp(nuTil_s, nu_s_hat))
  
  # We have to return a rounded result as E-15 differences in the floats can
  # have weird effects on the CLs+b and CLb results for special cases
  return round(result, 10)

def calc_CLs(n_obs, nuTil_b_obs, b0_in, db_in, nuTil_s_obs, s0_in, ds_in, nPseudo):
  """ Calcuates the CLs value given the measurement and signal/background estimates by running nPseudo experiments """

  global b0, s0, db, ds # Passes this to nexp function
  
  b0 = b0_in
  s0 = s0_in
  db = db_in
  ds = ds_in
  # Find observed test statistic
  qMu_obs = qMu(n_obs, nuTil_b_obs, nuTil_s_obs)
  
  # Find values for the nuisance parameters (in both hypotheses), which are most likely according to the observation
  nu_b_hat_obs_mu, nu_s_hat_obs_mu = nuWithMaxL(n_obs, nuTil_b_obs, nuTil_s_obs, 1.)
  nu_b_hat_obs_0, nu_s_hat_obs_0 = nuWithMaxL(n_obs, nuTil_b_obs, nuTil_s_obs, 0.)
  
  # Run pseudo experiments for the nuisance parameters. 
  qMu_pseu_sig = []
  qMu_pseu_bkg = []
  
  NSIG = nPseudo * 5 # You need more signal MC as only few will pass
  NBKG = nPseudo
  ###print "<<<<NSIG: "+str(NSIG)
  # - Retoss n_obs, assuming the most likely values for the nu  
  # - Retoss nu_tilde_observed assuming the most likely values for the nu
  nexp_sig = nexp(1., nu_b_hat_obs_mu, nu_s_hat_obs_mu)
  n_pseu_sig = np.random.poisson(nexp_sig, NSIG)
  nuTil_b_pseu_sig = np.random.normal(nu_b_hat_obs_mu, 1., NSIG)  
  nuTil_s_pseu_sig = np.random.normal(nu_s_hat_obs_mu, 1., NSIG)  
  
  nexp_bkg = nexp(0., nu_b_hat_obs_0, nu_s_hat_obs_0)
  n_pseu_bkg = np.random.poisson(nexp_bkg, NBKG)
  nuTil_b_pseu_bkg = np.random.normal(nu_b_hat_obs_0, 1., NBKG)
  nuTil_s_pseu_bkg = np.random.normal(nu_s_hat_obs_0, 1., NBKG)
  
    
  # - Determine how many pseudo experiments would have been as positive as the observed experiment.
  # - ps = (relative number of positive signal+background pseudoexperiments)
  # - 1-pb = (relative number of positive background pseudoexperiments)
  # - CLs = ps/(1-pb)
  # - Errors are determined as Poissonian MC errors
  positive_sig = 0
  positive_bkg = 0
  for i in range(NSIG):
    if qMu(n_pseu_sig[i], nuTil_b_pseu_sig[i], nuTil_s_pseu_sig[i]) >= qMu_obs:
      positive_sig += 1
  for i in range(NBKG):    
    if qMu(n_pseu_bkg[i], nuTil_b_pseu_bkg[i], nuTil_s_pseu_bkg[i]) >= qMu_obs:
      positive_bkg += 1
  
  #print str(NBKG)+"  "+str(positive_bkg)
  ps = positive_sig/float(NSIG)
  dps = sqrt(positive_sig)/float(NSIG)
  oneMinusPb = positive_bkg/float(NBKG)
  doneMinusPb = sqrt(positive_bkg)/float(NBKG)
  #print str(ps)+"     "+str(oneMinusPb)
  if oneMinusPb == 0:
    CLs = 1.0
    dCLs = 1.0
  else:
    CLs = ps/oneMinusPb
    dCLs = sqrt((dps/oneMinusPb)**2+(ps*doneMinusPb/oneMinusPb**2)**2)
    
  del n_pseu_sig
  del n_pseu_bkg
  del nuTil_b_pseu_sig
  del nuTil_b_pseu_bkg
  del nuTil_s_pseu_sig
  del nuTil_s_pseu_bkg
  
  return (round(CLs, 5), round(dCLs, 5))

def cls_obs(b, db, o, s, ds, randomseed): 
    # Attempt to import scipy with minimize. If this does not exist, CLs cannot be evaluated
    try:
        global minimize, brentq, fsolve
        from scipy.optimize import minimize, brentq, fsolve
    except ImportError:
        print "!!! IMPORTANT WARNING: CLS CANNOT BE EVALUATED SINCE SCIPY.MINIMIZE CANNOT BE LOADED! RETURNING -1"
        return (-1, -1)

    if randomseed != 0:
        np.random.seed = randomseed
    return calc_CLs(o, 0, b, db, 0, s, ds, 25000)

def cls_exp(b_in, db_in, o_in, s_in, ds_in, randomseed):
    # Attempt to import scipy with minimize. If this does not exist, CLs cannot be evaluated
    try:
        global minimize, brentq, fsolve
        from scipy.optimize import minimize, brentq, fsolve
    except ImportError:
        print "!!! IMPORTANT WARNING: CLS CANNOT BE EVALUATED SINCE SCIPY.MINIMIZE CANNOT BE LOADED! RETURNING -1"
        return (-1, -1)

    if randomseed != 0:
        np.random.seed = randomseed
        
    global b0, s0, db, ds # Passes this to nexp function
    b0 = b_in
    s0 = s_in
    db = db_in
    ds = ds_in
    # find best fit nuisance parameters and corresponding nexp
    
    nu_b_hat_obs_0, nu_s_hat_obs_0 = nuWithMaxL(o_in, 0., 0., 0.)
    nexp_bkg = nexp(0., nu_b_hat_obs_0, nu_s_hat_obs_0)
    # calculate CLs with these
    return calc_CLs(nexp_bkg, nu_b_hat_obs_0, b_in, db_in, nu_s_hat_obs_0, s_in, ds_in, 25000)
    
##Replaces the likelihood-function, which uses a MC-approach with an rootfind-approach ###
def Lambda(mu,  b,  s,  db,  ds,  nuisance = []):
    try:
        if(b==0.0 and s==0.0):
            return 1.0
        if(b==0.0):
            return mu*s*exp(ds/s*nuisance[0])
        if(s==0.0):
            return b*exp(db/b*nuisance[1])
        return mu*s*exp(ds/s*nuisance[0]) + b*exp(db/b*nuisance[1])
    except OverflowError as e:
        print "Error A",  e
        return 1.0
    except ZeroDivisionError as z:
        print "Error B",  z
        return 1.0

def derLambdasig(mu,  nu_sig,  s,  ds):
    try:
        return mu*ds*exp(ds/s*nu_sig)
    except OverflowError as e:
        print "Error E",  e
        return 1.0
    except ZeroDivisionError as z:
        print "Error F",  z
        return 1.0

def derLambdabackg(nu_backg, b,  db):
    try:
        return db*exp(db/b*nu_backg) 
    except OverflowError as e:
        print "Error G",  e
        return 1.0
    except ZeroDivisionError as z:
        print "Error H",  z
        return 1.0
    
def derLambdamu(nu_sig, s, ds):
    try:
        return s*exp(ds/s*nu_sig)
    except OverflowError as e:
        print "Error I",  e
        return 1.0
    except ZeroDivisionError as z:
        print "Error J",  z
        return 1.0

# Returns the np for the Constrained Maximum Likelihood    
def get_CMLparameter(mu,  N_obs, N_backg,  N_sig,  dN_backg,  dN_sig,  nuTilde =  []):
    NFunc = [0.0 for i in range(2)]
    guess = [0.0 for i in range(2)]
    def g(c):
	if(c>=250):
		c=np.random.uniform(low=0.8, high=1.0)*250.0
		#print "case c1", N_sig, N_backg
	try:
        	NFunc[0] = derLambdabackg(c,  N_backg,  dN_backg)*(-1.0+N_obs/Lambda(mu,  N_backg,  N_sig,  dN_backg,  dN_sig, [0.0,  c]))-(c-nuTilde[0])        
	except:
		NFunc[0] = 500.0
	return NFunc[0]
    def h(c):
	if(c[0]>=250):
		c[0]=np.random.uniform(low=0.8, high=1.0)*250.0
		#print "case c0, [N_sig, Nbackg] = [", N_sig, N_backg,"]"
	if(c[1]>=250):
		c[1]=np.random.uniform(low=0.8, high=1.0)*250.0
		#xprint "case c1, [N_sig, Nbackg] = [", N_sig, N_backg,"]"
	try:
        	NFunc[0] = derLambdasig(mu,   c[0],   N_sig, dN_sig)*(-1.0+N_obs/Lambda(mu,  N_backg,  N_sig,  dN_backg,  dN_sig, c))-(c[0]-nuTilde[0])
	except:
		NFunc[0] = 500.0
	try:
        	NFunc[1] = derLambdabackg(c[1],  N_backg, dN_backg)*(-1.0+N_obs/Lambda(mu, N_backg,  N_sig,  dN_backg,  dN_sig,  c))-(c[1]-nuTilde[1])
	except:
		NFunc[1] = 500.0
        return NFunc
    def i(c):
	if(c>=250):
		c=np.random.uniform(low=0.8, high=1.0)*250
		#print "case c0, [N_sig, Nbackg] = [", N_sig, N_backg,"]"
        NFunc[1] = derLambdasig(mu, c, N_sig, dN_sig)*(-1.0+N_obs/Lambda(mu,  N_backg,  N_sig,  dN_backg,  dN_sig, [c, 0.0]))-(c-nuTilde[1])
        return NFunc[1]
    
    #decision-tree
    opt_nuis_backg=1
    opt_nuis_sig=1
    if(N_sig==0.0):
        opt_nuis_sig=0
    if(N_backg==0.0):
        opt_nuis_backg=0
    if(mu==0.0):
        opt_nuis_sig=0
    if(dN_sig==0.0):
        opt_nuis_sig=0
    if(dN_backg==0.0):
        opt_nuis_backg=0
    
    counter=0
    succ=True
    
    if(opt_nuis_backg==0 and opt_nuis_sig==0):
        return [0.0,  0.0], succ
   
    if(opt_nuis_backg==0 and opt_nuis_sig==1):
	try:        
		guess[0] = N_sig/(2*dN_sig)*log(N_obs/(mu*N_sig))
	except:
		guess[0] = N_sig/(2*dN_sig)*N_obs/(mu*N_sig)
	solution = opt.root(i, guess[0])
	while(not solution.success and counter<=10):
		counter+=1
		succ=False
		try:
			guess[0] = N_sig/(2*dN_sig)*log(N_obs/(mu*N_sig))*np.random.uniform(low=0.0, high=2.0)
		except:
			guess[0] = N_sig/(2*dN_sig)*N_obs/(mu*N_sig)*np.random.uniform(low=0.0, high=2.0)
		solution = opt.root(i, guess[0])
        return [solution.x[0],  nuTilde[1]], succ 
    
    if(opt_nuis_backg==1 and opt_nuis_sig==0):
	try:
        	guess[1] = N_backg/(2*dN_backg)*log(N_obs/N_backg)
	except:
		guess[1] = N_backg/(2*dN_backg)*N_obs/N_backg
	solution = opt.root(g, guess[1])
	while(not solution.success and counter<=10):
		counter+=1
		succ=False
		try:
			guess[1] = N_backg/(2*dN_backg)*log(N_obs/N_backg)*np.random.uniform(low=0.0, high=2.0)
		except:
			guess[1] = N_backg/(2*dN_backg)*N_obs/N_backg*np.random.uniform(low=0.0, high=2.0)
		solution = opt.root(g, guess[1])
        return [nuTilde[0],  solution.x[0]], succ
    
    if(opt_nuis_backg==1 and opt_nuis_sig==1):
        try:
            guess[0] = log((N_obs-N_backg*exp(dN_backg/N_backg))/N_sig)*N_sig/(2*dN_sig)
            guess[1] = log((N_obs-N_sig*exp(dN_sig/N_sig))/N_backg)*N_backg/(dN_backg)
        except:
	    #print "Couldn't use a good guess, switching to approximation for tupel [N_obs, N_backg,  N_sig,  dN_backg,  dN_sig] = [",  N_obs, N_backg,  N_sig,  dN_backg,  dN_sig,"]"
            guess[0] = N_sig/(2*dN_sig)*log(N_obs/(mu*N_sig))
            guess[1] = N_backg/(2*dN_backg)*log(N_obs/N_backg)
        solution = opt.root(h, guess)
	while(not solution.success and counter<=10):
		counter+=1
		succ=False
		try:
        	    guess[0] = log((N_obs-N_backg*exp(dN_backg/N_backg))/N_sig)*N_sig/(2*dN_sig)*np.random.uniform(low=0.0, high=2.0)
          	    guess[1] = log((N_obs-N_sig*exp(dN_sig/N_sig))/N_backg)*N_backg/(dN_backg)*np.random.uniform(low=0.0, high=2.0)
       		except:
	  	    #print "Couldn't use a good guess, switching to approximation for tupel [N_obs, N_backg,  N_sig,  dN_backg,  dN_sig] = [",  N_obs, N_backg,  N_sig,  dN_backg,  dN_sig,"]"
            	    guess[0] = N_sig/(2*dN_sig)*log(N_obs/(mu*N_sig))*np.random.uniform(low=0.0, high=2.0)
            	    guess[1] = N_backg/(2*dN_backg)*log(N_obs/N_backg)*np.random.uniform(low=0.0, high=2.0)
		solution = opt.root(h, guess)
        return solution.x, succ

def Teststat(mu_tested, N_obs, N_backg,  N_sig,  dN_backg,  dN_sig,  nuisance_cml = [], nuisanceTilde = []):
    NFunc = 0.0
    try:
   	 NFunc += 2.*(Lambda(mu_tested, N_backg,  N_sig,  dN_backg,  dN_sig,  nuisance_cml)-N_obs)-2.*N_obs*log(Lambda(mu_tested, N_backg,  N_sig,  dN_backg,  dN_sig,  nuisance_cml)/N_obs) 
    except:
	NFunc += 500.0
    if(N_sig==0.0):
    	NFunc += (nuisance_cml[1]-nuisanceTilde[1])**2
    else:
        if(N_backg==0.0):
           NFunc += (nuisance_cml[0]-nuisanceTilde[0])**2
        else:
	   for i in range(2):   
                NFunc += (nuisance_cml[i]-nuisanceTilde[i])**2
    if NFunc<0.0:
        print "Likelihoodratio<0. Something went wrong. Setting Likelihoodratio to 500.0"
        return 500.0
    return NFunc
    
def findapproxlh(mu_tested, N_obs, N_backg,  N_sig,  dN_backg,  dN_sig,  nuTilde = []):
	print "Root-algorithm didn't converge. Approximating for tupel [N_obs, N_backg,  N_sig,  dN_backg,  dN_sig] = [",N_obs, N_backg,  N_sig,  dN_backg,  dN_sig,"]"
	runs=40
	Teststatb=[0.0 for i in range(runs)] 
	Teststatbsorted=[0.0 for i in range(runs)] 
	buffer = [[0.0 for i in range(2)] for j in range(runs)]
	sum=0.0
	lowervar=0.7 #Here I'm not sure whether or not there should also be some upward-fluctuation
	highervar=1.0
	lowervarerr=0.3
	highervarerr=0.7 #varying the error to lower values gives higher teststatistics and more robust results.
	for i in range(runs):   
		N_obsb=np.random.uniform(low=lowervar, high=highervar)*N_obs
		N_backgb=np.random.uniform(low=lowervar, high=highervar)*N_backg
		N_sigb=np.random.uniform(low=lowervar, high=highervar)*N_sig
		dN_backgb=np.random.uniform(low=lowervarerr, high=highervarerr)*dN_backg
		dN_sigb=np.random.uniform(low=lowervarerr, high=highervarerr)*dN_sig
		buffer[i] = get_CMLparameter(mu_tested, N_obsb,  N_backgb,  N_sigb,  dN_backgb,  dN_sigb, nuTilde)[0]
		Teststatb[i]=Teststat(mu_tested, N_obsb,   N_backgb,  N_sigb,  dN_backgb,  dN_sigb,  buffer[i], nuTilde)
	Teststatbsorted = sorted(Teststatb)
	#Only the highest teststatistic values are used for the approximation.
	for i in range(20):
		sum+=Teststatbsorted[i+20]
	return sum/float(20)
		
	

def likelihood(mu_tested,  N_obs, N_backg,  N_sig,  dN_backg,  dN_sig,  nuTilde_obs = []):
    if(int(scipy.__version__.split('.')[1]) <= 10):
        print "ERROR: Scipy  Version 0.11.0 needed. You have Version",  scipy.__version__,". Please upgrade your scipy."
        sys.exit()
    if(N_obs==0.0):
        N_obs=0.001
    if(N_backg<(N_obs/100.0)):
        N_backg=N_obs/100.0
        if(dN_backg > N_backg):
          dN_backg = N_backg 
    if(N_sig<(N_backg/100.0)):
        N_sig=(N_backg/100.0)
        if(dN_sig > N_sig):
          dN_sig = N_sig
    if(N_sig<(N_obs/100.0)):
        N_sig=(N_obs/100.0)
        if(dN_sig > N_sig):
          dN_sig = N_sig

    nuisance_cml_obs, success = get_CMLparameter(mu_tested, N_obs,  N_backg,  N_sig,  dN_backg,  dN_sig, nuTilde_obs)
    if(not success):
	return findapproxlh(mu_tested, N_obs, N_backg,  N_sig,  dN_backg,  dN_sig,  [0.0, 0.0])	    
    return Teststat(mu_tested, N_obs,   N_backg,  N_sig,  dN_backg,  dN_sig,  nuisance_cml_obs, nuTilde_obs) 
##/Replaces the likelihood-function, which uses a MC-approach with an rootfind-approach ###

if __name__ == '__main__':
  if len(sys.argv) == 6:
    result = cls_obs(float(sys.argv[1]), float(sys.argv[2]), float(sys.argv[3]), float(sys.argv[4]), float(sys.argv[5]), 25000)    
    print "CLs: "+str(result[0])+" +- "+str(result[1])    
  else :
    print "Syntax for CLs:   ./cls.py   nobs   bkg   bkgerr   sig   sigerr" 
  
  

