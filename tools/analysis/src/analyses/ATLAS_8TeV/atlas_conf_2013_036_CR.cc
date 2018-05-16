#include "atlas_conf_2013_036.h"

void Atlas_conf_2013_036_CR::initialize() {
  setInformation(""
    "@#ATLAS\n"
     "@#ATLAS-CONF-2013-036\n"
     "@#4 leptons + etmiss\n"
     "@#sqrt(s) = 8 TeV\n"
     "@#int(L) = 20.7 fb^-1\n"
  "");
  setLuminosity(20.7*units::INVFB);      
  setAnalysisName("atlas_conf_2013_036_CR");    
  ignore("towers");
  ignore("tracks");
  // You should initialize any declared variables here
}

void Atlas_conf_2013_036_CR::analyze() {
  // Your eventwise analysis code goes here
}

void Atlas_conf_2013_036_CR::finalize() {
  // Whatever should be done after the run goes here
}       
