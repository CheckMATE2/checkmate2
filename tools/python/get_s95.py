#!/usr/bin/env python

"""Calculates CLs and S95 limits for simple one bin counting experiments.

CLs is calculated using the standard LHC techniques using a profile likelihood 
for the test statistic. The observed S95  uses a simple regula falsi method to find
the value s with CLs = 0.05, using the Pegasus algorithm to improve the performance.
S95 expected calculates the best fit nuisance parameters given the observation and
uses these as the actual observation to find S95.
"""
from __future__ import print_function
from __future__ import division

from builtins import str
from builtins import range
from past.utils import old_div
__author__ = "Daniel Schmeier"
__copyright__ = "Copyright 2014, CheckMATE"
__credits__ = ["Daniel Schmeier"]
__license__ = "GPL"
__version__ = "1.0.0"
__maintainer__ = "Daniel Schmeier"
__email__ = "daschm@uni-bonn.de"
__status__ = "Prototype"


from math import *
import os, sys
import numpy as np
from scipy.optimize import minimize, brentq


def nexp(mu, nu_b):
  """
  Returns expected number of events given mu and nu
  expected = signal + background.
  Signal is multiplied with modifier mu.
  Background is 'smeared' by corresponding nu.
  The smearing is lognormal, which is performed by taking
    the exponential of a normal distributed random number (nu).
  Note that b0, s0 and db are global variables defined at input
  """
  return b0*exp(old_div(db,b0)*nu_b) + s0*mu

def eqForMaxL(nu_b, n, nuTil_b, mu,):
  """
  Helper function to maximise likelihood 
  The nu_b for which this function is zero maximises the likelihood 
  """
  lam = nexp(mu, nu_b)
  return (n-lam)*db*exp(old_div(db,b0)*nu_b)-lam*(nu_b-nuTil_b)

def nuWithMaxL(n, nuTil_b, mu):
  """ Finds root of eqForMaxL to maximise Likelihood   """
  return brentq(eqForMaxL, -20., 20., args=(n, nuTil_b, mu), full_output=False, disp=True)

def reduced_logp(nuTil, nu):
  """ reduced_logp = log(p) without normalisation """
  return -(nuTil-nu)**2./2.

def qMu(n, nuTil_b):  
  """ Test statisctic for signal+background hypothesis """

  # First, overall maximum is easy to calculate (b0, s0, db are defined during input)
  mu_hat = old_div((n-b0*exp(old_div(db,b0)*nuTil_b)),s0)
  nu_b_hat = nuTil_b

  # mu_hat > 1 should result in zero to ensure one sided limit
  if mu_hat > 1:
    return 0
  # if mu_hat < 0, fix mu_hat = 0 and re-evaluate best nu_b
  elif mu_hat < 0:
    mu_hat = 0
    nu_b_hat = nuWithMaxL(n, nuTil_b, 0.)    
    
  # Now, Maximise with mu=1 fixed
  nu_b_mu_hat = nuWithMaxL(n, nuTil_b, 1.)
    
  # Having this, formulate the profile likelihood  
  # First, evaluate lambda for numerator and denominator
  lam_hat_hat = nexp(mu_hat, nu_b_hat)
  lam_hat = nexp(1., nu_b_mu_hat)
  
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
  if lam_hat*lam_hat_hat == 0:
    # ... both lambdas must be zero
    if lam_hat != 0 or lam_hat_hat != 0:
      exit("ERROR: In S95 calculation, profile likelihood asked for impossible maximisation parameteres!")
    # In that case, the result is only the likelihood w.r.t the nuisance parameters
    result = -2.*(reduced_logp(nuTil_b, nu_b_mu_hat) - reduced_logp(nuTil_b, nu_b_hat))
  else:
    # Otherwise, return the normal likelihood    
    result = -2.*(n*log(old_div(lam_hat,lam_hat_hat)) - (lam_hat-lam_hat_hat) + reduced_logp(nuTil_b, nu_b_mu_hat) - reduced_logp(nuTil_b, nu_b_hat))
  
  return result

def calc_CLs(n_obs, nuTil_b_obs, b0_in, db_in, s0_in, nPseudo):
  """ Calcuates the CLs value given the measurement and signal/background estimates by running nPseudo experiments """
  global b0, s0, db # Passes this to nexp function
  b0 = b0_in
  s0 = s0_in
  db = db_in

  # Find observed test statistic
  qMu_obs = qMu(n_obs, nuTil_b_obs)
  
  # Find values for the nuisance parameters (in both hypotheses), which are most likely according to the observation
  nu_b_hat_obs_mu = nuWithMaxL(n_obs, nuTil_b_obs, 1.)
  nu_b_hat_obs_0 = nuWithMaxL(n_obs, nuTil_b_obs, 0.)
  
  # Run pseudo experiments for the nuisance parameters. 
  qMu_pseu_sig = []
  qMu_pseu_bkg = []
  
  NSIG = nPseudo * 5 # You need more signal MC as only few will pass
  NBKG = nPseudo
  ###print "<<<<NSIG: "+str(NSIG)
  # - Retoss n_obs, assuming the most likely values for the nu  
  # - Retoss nu_tilde_observed assuming the most likely values for the nu
  nexp_sig = nexp(1., nu_b_hat_obs_mu)
  n_pseu_sig = np.random.poisson(nexp_sig, NSIG)
  nuTil_b_pseu_sig = np.random.normal(nu_b_hat_obs_mu, 1., NSIG)  
  
  nexp_bkg = nexp(0., nu_b_hat_obs_0)
  n_pseu_bkg = np.random.poisson(nexp_bkg, NBKG)
  nuTil_b_pseu_bkg = np.random.normal(nu_b_hat_obs_0, 1., NBKG)
  
    
  # - Determine how many pseudo experiments would have been as positive as the observed experiment.
  # - ps = (relative number of positive signal+background pseudoexperiments)
  # - 1-pb = (relative number of positive background pseudoexperiments)
  # - CLs = ps/(1-pb)
  # - Errors are determined as Poissonian MC errors
  positive_sig = 0
  positive_bkg = 0
  for i in range(NSIG):
    if qMu(n_pseu_sig[i], nuTil_b_pseu_sig[i]) >= qMu_obs:
      positive_sig += 1
  for i in range(NBKG):    
    if qMu(n_pseu_bkg[i], nuTil_b_pseu_bkg[i]) >= qMu_obs:
      positive_bkg += 1
  
  ps = positive_sig/float(NSIG)
  dps = sqrt(positive_sig)/float(NSIG)
  oneMinusPb = positive_bkg/float(NBKG)
  doneMinusPb = sqrt(positive_bkg)/float(NBKG)
  #print str(ps)+"     "+str(oneMinusPb)
  if oneMinusPb == 0:
    CLs = 1.0
    dCLs = 1.0
  else:
    CLs = old_div(ps,oneMinusPb)
    if CLs == 0.0:
      dCLs = 1.0
    else:
      dCLs = sqrt((old_div(dps,oneMinusPb))**2+(old_div(ps*doneMinusPb,oneMinusPb**2))**2)
    
  del n_pseu_sig
  del n_pseu_bkg
  del nuTil_b_pseu_sig
  del nuTil_b_pseu_bkg
  
  return (round(CLs, 5), round(dCLs, 5))


    
def f(x, N, n_data, nuTil_b_data, b0, sigma_b):
  """ The root of this function is the point where CLs = 0.05 """
  res = calc_CLs(n_data, nuTil_b_data, b0, sigma_b, x, N)
  ###print res
  return res[0]-0.05 , res[1]

def new_x(x0, x1, y0, y1):
  """ Determines the new x in the regula falsi method """
  return x0 - old_div((x1-x0),(y1-y0))*y0


def find_s95_obs(n_data, nuTil_b_data, b0, sigma_b):  
  """ Uses the pegasus type regula falsi method to determine S for which CLs(n, b, db, s) = 0.05 """
  
  # First, get an estimate for the size of s
  db = sqrt(sigma_b*sigma_b + b0*exp(old_div(sigma_b,b0)*nuTil_b_data)) 
  if b0 < 1:
    db = sqrt(sigma_b*sigma_b + 1*exp(old_div(sigma_b,b0)*nuTil_b_data)) 
  if n_data > b0:
    db = sqrt(db*db+(n_data-b0)**2)
    
  l = db # Estimate for s0_min
  r = 3.*db # Estimate for s0_max
  nPseudo = 1000
  
  # Evaluate f(l) and f(r). Use error to shift the values up and down to make sure to not 
  #  get away from the root
  res = f(l, nPseudo, n_data, nuTil_b_data, b0, sigma_b)    
  yl = res[0] + 3.*res[1] # Use upper limit on f(l) for left y
  res = f(r, nPseudo, n_data, nuTil_b_data, b0, sigma_b)    
  yr = res[0] - 3.*res[1] # Use lower limit on f(r) for right y
  if (yl*yr > 0):
    print("Estimate does not work for "+str(b0)+"  "+str(sigma_b)+"  "+str(n_data)+"  "+str(nuTil_b_data)+"  ["+str(l)+", "+str(r)+"]")
    # Start with a definitly working but very extreme start
    l = 1
    res = f(l, nPseudo, n_data, nuTil_b_data, b0, sigma_b)    
    yl = res[0] + 3.*res[1]
    r = 10.*db
    res = f(r, nPseudo, n_data, nuTil_b_data, b0, sigma_b)    
    yr = res[0] - 3.*res[1]
    
  while(True):
    ###print str(l)+"  "+str(yl)
    ###print str(r)+"  "+str(yr)
    ###print ""
    
    m = new_x(l, r, yl, yr)    
    # If l and r are very close to each other, return the estimate
    if fabs(r-l) < sigma_b/10.:
      return m
    
    ym = f(m, nPseudo, n_data, nuTil_b_data, b0, sigma_b)    
    # If the new point is clearly on the wrong side (with CLs == 0 or 1), start next round straigt away:
    ###print "ym: "+str(ym)
    if ym[0] == 0.95:
      l, yl = m, ym[0]+3.*ym[1]
      continue
    elif ym[0] == -0.05:
      r, yr = m, ym[0]-3.*ym[1]
      continue
      
    # If, within the error, the new point varies around the zero, take further action
    if (ym[0]+3.*ym[1])*(ym[0]-3.*ym[1]) < 0:
      # if error is below 5 %, return
      if ym[1]/0.05 < 0.05:
        return m
      # else, demand more statistics to continue
      # the error goes with sqrt(nPseudo), so lets find enough statistic to go below 5 %      
      
      factor = (ym[1]/0.05)/0.05 * 1.2 # <- 1.2 just to be on the safe side      
      ###print " -> Need more statistics! Increase by factor of "+str(factor)
      nPseudo = int(nPseudo*factor**2)
      ###print ym
      ###print nPseudo
      continue
    
    # Put m on the right side of the root
    if ym[0] > 0:      
      l, yl = m, ym[0]+3.*ym[1]
    else:
      # If m is put on the right, then scale down yl to improve convergence (Pegasus algorithm)
      kappa = 0.5  
      yl = old_div(yr,(yr+ym[0]-3.*ym[1]))*yl 
      r, yr = m, ym[0]-3.*ym[1]

def find_s95_exp(n_data, b0, sigma_b):  
  # Find best fit values for observation  
  nu_b_hat_obs_0 = nuWithMaxL(n_data, 0., 0.)
  nexp_bkg = nexp(0., nu_b_hat_obs_0)
  # Evaluate s95 for these
  return find_s95_obs(nexp_bkg, nu_b_hat_obs_0, b0, sigma_b)
  
if __name__ == '__main__':
  if len(sys.argv) == 5:
    result = calc_CLs(float(sys.argv[1]), 0, float(sys.argv[2]), float(sys.argv[3]), float(sys.argv[4]), 5000)    
    print("CLs: "+str(result[0])+" +- "+str(result[1]))    
  elif len(sys.argv) == 4:
    result = find_s95_obs(float(sys.argv[1]), 0, float(sys.argv[2]), float(sys.argv[3]))
    print("S95_obs: "+str(result))
    result = find_s95_exp(float(sys.argv[1]), float(sys.argv[2]), float(sys.argv[3]))
    print("S95_exp: "+str(result))
  else :
    print("Syntax for S95:   ./get_s95.py   nobs   bkg   bkgerr")
    print("Syntax for CLs:   ./get_s95.py   nobs   bkg   bkgerr   s")
  
  

