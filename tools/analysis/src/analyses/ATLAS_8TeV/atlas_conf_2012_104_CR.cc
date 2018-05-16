#include "atlas_conf_2012_104.h"

void Atlas_conf_2012_104_CR::initialize() {
  setInformation(""
    "@#ATLAS\n"
     "@#ATLAS-CONF-2012-104\n"
     "@#1 lepton + >= 4 jets + etmiss\n"
     "@#sqrt(s) = 8 TeV\n"
     "@#int(L) = 5.8 fb^-1\n"
  "");
  setLuminosity(5.8*units::INVFB);      
  setAnalysisName("atlas_conf_2012_104_CR");    
  ignore("towers");
  ignore("tracks");
  // You should initialize any declared variables here
}

void Atlas_conf_2012_104_CR::analyze() {
  // Your eventwise analysis code goes here
}

void Atlas_conf_2012_104_CR::finalize() {
  // Whatever should be done after the run goes here
}       
