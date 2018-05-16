#include "atlas_conf_2013_049.h"

void Atlas_conf_2013_049_CR::initialize() {
  setInformation(""
    "@#ATLAS\n"
     "@#ATLAS-CONF-2013-049\n"
     "@#2 leptons + etmiss\n"
     "@#sqrt(s) = 8 TeV\n"
     "@#int(L) = 20.3 fb^-1\n"
  "");
  setLuminosity(20.3*units::INVFB);      
  setAnalysisName("atlas_conf_2013_049_CR");    
  ignore("towers");
  ignore("tracks");
  // You should initialize any declared variables here
}

void Atlas_conf_2013_049_CR::analyze() {
  // Your eventwise analysis code goes here
}

void Atlas_conf_2013_049_CR::finalize() {
  // Whatever should be done after the run goes here
}       
