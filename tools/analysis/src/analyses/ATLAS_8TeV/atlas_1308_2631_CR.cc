#include "atlas_1308_2631.h"

void Atlas_1308_2631_CR::initialize() {
  setInformation(""
    "@#0 leptons + 2 b-jets + Etmiss [Sbottom/stop]\n"
  "");
  setLuminosity(20.1*units::INVFB);      
  setAnalysisName("atlas_1308_2631_CR");    
  ignore("towers");
  ignore("tracks");
  // You should initialize any declared variables here
}

void Atlas_1308_2631_CR::analyze() {
  // Your eventwise analysis code goes here
}

void Atlas_1308_2631_CR::finalize() {
  // Whatever should be done after the run goes here
}       
