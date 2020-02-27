#include "AnalysisBase.h"

AnalysisBase::AnalysisBase() {
    outputFolder = "";
    outputPrefix = "";
    analysis = "";
    information = "";
    nEvents = 0;
    sumOfWeights = 0;
    sumOfWeights2 = 0;
    xsect = 0;
    xsecterr = 0;
    luminosity = 0;
    weight = 0;
    missingET = NULL;
    result = NULL;
}

AnalysisBase::~AnalysisBase() {
    for (int i = 0; i < fStreams.size(); i++) {
        fStreams[i]->close();
        delete fStreams[i];
    }
}


void AnalysisBase::setup(std::map<std::string, std::vector<int> > whichTagsIn, std::map<std::string, std::string> eventParameters) {
    // parameters MUST include
    // - outdir, prefix
    std::map<std::string, std::string>::iterator searchIterator;

    searchIterator = eventParameters.find("outputFolder");
    if(searchIterator != eventParameters.end())
        outputFolder = searchIterator->second;
    else
        Global::abort("AnalysisHandler", "Tried to setup analysis without a proper output folder!");

    searchIterator = eventParameters.find("outputPrefix");
    if(searchIterator != eventParameters.end())
        outputPrefix = searchIterator->second;
    else
        Global::abort("AnalysisHandler", "Tried to setup analysis without a proper output prefix!");

    // CAN include xsect and xsecterr (can also be given at the end of run during finish()
    searchIterator = eventParameters.find("xsect");
    if(searchIterator != eventParameters.end())
        xsect = Global::strToDouble(searchIterator->second);

    searchIterator = eventParameters.find("xsectErr");
    if(searchIterator != eventParameters.end())
        xsecterr = Global::strToDouble(searchIterator->second);

    whichTags = whichTagsIn;
    initialize(); // specified by derived analysis classes
}

void AnalysisBase::processEvent(int iEvent) {
    sumOfWeights += weight;
    sumOfWeights2 += weight*weight;
    nEvents++;
    analyze(); // specified by derived analysis classes
    
    // deletes pointers created by final state objects
    for (int i = 0; i < finalStateObjects.size(); i++)
      delete finalStateObjects[i];
    finalStateObjects.clear(); 
}

void AnalysisBase::finish() {
    finalize(); // specified by derived analysis classes
    if(!cutflowRegions.empty()) {
      int cutflowOutput = bookFile(analysis+"_cutflow.dat");
      *fStreams[cutflowOutput] << "Cut  Sum_W  Sum_W2  Acc  N_Norm\n";
      for(std::map<std::string, double>::iterator cutflow_iter=cutflowRegions.begin(); cutflow_iter!=cutflowRegions.end(); ++cutflow_iter)
        *fStreams[cutflowOutput] << cutflow_iter->first << "  " << cutflow_iter->second << "  " << cutflowRegions2[cutflow_iter->first] << "  " << cutflow_iter->second/sumOfWeights << "  " << normalize(cutflow_iter->second) << "\n";
    }
    if(!signalRegions.empty()) {
      int signalOutput = bookFile(analysis+"_signal.dat");
      *fStreams[signalOutput] << "SR  Sum_W  Sum_W2  Acc  N_Norm\n";
      for(std::map<std::string, double>::iterator signal_iter=signalRegions.begin(); signal_iter!=signalRegions.end(); ++signal_iter)
        *fStreams[signalOutput] << signal_iter->first << "  " << signal_iter->second << "  " << signalRegions2[signal_iter->first] << "  " << signal_iter->second/sumOfWeights << "  " << normalize(signal_iter->second) << "\n";
    }
    if(!controlRegions.empty()) {
      int controlOutput = bookFile(analysis+"_control.dat");
      *fStreams[controlOutput] << "CR  Sum_W  Sum_W2  Acc  N_Norm\n";
      for(std::map<std::string, double>::iterator control_iter=controlRegions.begin(); control_iter!=controlRegions.end(); ++control_iter)
        *fStreams[controlOutput] << control_iter->first << "  " << control_iter->second << "  " << controlRegions2[control_iter->first] << "  " << control_iter->second/sumOfWeights << "  " << normalize(control_iter->second) << "\n";
    }

    for (int i = 0; i < fStreams.size(); i++)
        fStreams[i]->close();
}

void AnalysisBase::bookSignalRegions(std::string listOfRegions) {
  std::string currKey = "";
  // Sum letter by letter and define map-key as soon as ; is reached
  for(int i = 0; i < strlen(listOfRegions.c_str()); i++) {
    char c = listOfRegions[i];
    if (c == ';') {
      signalRegions[currKey] = signalRegions2[currKey] = 0.0;
      currKey = "";
    }
    else
      currKey += c;
  }
  // The last key might not be separated by ;
  if(currKey != "")
      signalRegions[currKey] = signalRegions2[currKey] = 0.0;
}

void AnalysisBase::bookControlRegions(std::string listOfRegions) {
  std::string currKey = "";
  // Sum letter by letter and define map-key as soon as ; is reached
  for(int i = 0; i < strlen(listOfRegions.c_str()); i++) {
    char c = listOfRegions[i];
    if (c == ';') {
      controlRegions[currKey] = controlRegions2[currKey] = 0.0;
      currKey = "";
    }
    else
      currKey += c;
  }
  // The last key might not be separated by ;
  if(currKey != "")
      controlRegions[currKey] = controlRegions2[currKey] = 0.0;
}

void AnalysisBase::bookCutflowRegions(std::string listOfRegions) {
  std::string currKey = "";
  // Sum letter by letter and define map-key as soon as ; is reached
  for(int i = 0; i < strlen(listOfRegions.c_str()); i++) {
    char c = listOfRegions[i];
    if (c == ';') {
      cutflowRegions[currKey] = cutflowRegions2[currKey] = 0.0;
      currKey = "";
    }
    else
      currKey += c;
  }
  // The last key might not be separated by ;
  if(currKey != "")
      cutflowRegions[currKey] = cutflowRegions2[currKey] = 0.0;
}
    
int AnalysisBase::bookFile(std::string name, bool noheader) {
    // Assemble absolute filename
    std::string filename = outputFolder+"/"+outputPrefix+"_"+name;
    // Open stream
    std::ofstream* file = new ofstream(filename.c_str());
    // Write standard information
    if (!noheader) {
      //FIXME Most of it is not known at the beginning!
      *file << information << "\n";
      *file << "@Inputfile:       " << "\n"; //FIXME what to put here?
      *file << "@XSect:           " << xsect << " fb\n";
      *file << "@ Error:          " << xsecterr << " fb\n";
      *file << "@MCEvents:        " << nEvents << "\n";
      *file << "@ SumOfWeights:   " << sumOfWeights << "\n";
      *file << "@ SumOfWeights2:  " << sumOfWeights2 << "\n";
      *file << "@ NormEvents:     " << normalize(sumOfWeights) << "\n\n";
    }
    fStreams.push_back(file);
    fNames.push_back(filename);
    // The returned number denotes the corresponding index for the fStreams vector
    return fStreams.size()-1;
}

std::vector<Photon*> AnalysisBase::filterIsolation(std::vector<Photon*> unfiltered, int relative_tag) {
    if(relative_tag < 0)
        Global::abort("AnalysisHandler", "You cannot ask for a photon isolation tag with index smaller than 0! ("+analysis+")");
    std::vector<int> analysisSpecificFlags = whichTags["PhotonIsolation"];
    if (relative_tag+2 > analysisSpecificFlags.size())
        Global::abort("AnalysisHandler", "You cannot ask for a photon isolation tag with index larger than "+Global::intToStr((int)analysisSpecificFlags.size()-2)+" in "+analysis);
    std::vector<Photon*> isolatedPhotons;
    for (int p = 0; p < unfiltered.size(); p++) {
        if (photonIsolationTags[unfiltered[p]][analysisSpecificFlags[relative_tag+1]]) // note that the 0th condition is reserved for the internal superloose condition
            isolatedPhotons.push_back(unfiltered[p]);
    }
    return isolatedPhotons;
}

std::vector<Photon*> AnalysisBase::filterIsolation(std::vector<Photon*> unfiltered, std::vector<int> relative_tags) {
    std::vector<Photon*> isolatedPhotons = unfiltered;
    // run filterIsolation on all reltags in the vector
    for(int t = 0; t < relative_tags.size(); t++)
        isolatedPhotons = filterIsolation(isolatedPhotons, relative_tags[t]);
    // If no flags are given then all should be checked
    if (relative_tags.size() == 0) {
        for(int reltag = 0; reltag+2 <= whichTags["PhotonIsolation"].size(); reltag++)
            isolatedPhotons = filterIsolation(isolatedPhotons, reltag);
    }
    return isolatedPhotons;
}

std::vector<Muon*> AnalysisBase::filterIsolation(std::vector<Muon*> unfiltered, int relative_tag) {
    if(relative_tag < 0)
        Global::abort("AnalysisHandler", "You cannot ask for a muon isolation tag with index than 0! ("+analysis+")");
    std::vector<int> analysisSpecificFlags = whichTags["MuonIsolation"];
    if (relative_tag+2 > analysisSpecificFlags.size())
        Global::abort("AnalysisHandler", "You cannot ask for a muon isolation tag with index larger than "+Global::intToStr((int)analysisSpecificFlags.size()-2)+" in "+analysis);
    std::vector<Muon*> isolatedMuons;
    for (int p = 0; p < unfiltered.size(); p++) {
        if (muonIsolationTags[unfiltered[p]][analysisSpecificFlags[relative_tag+1]]) // note that the 0th condition is reserved for the internal superloose condition
            isolatedMuons.push_back(unfiltered[p]);
    }
    return isolatedMuons;
}

std::vector<Muon*> AnalysisBase::filterIsolation(std::vector<Muon*> unfiltered, std::vector<int> relative_tags) {
    std::vector<Muon*> isolatedMuons = unfiltered;
    // run filterIsolation on all reltags in the vector
    for(int t = 0; t < relative_tags.size(); t++)
        isolatedMuons = filterIsolation(isolatedMuons, relative_tags[t]);
    // If no flags are given then all should be checked
    if (relative_tags.size() == 0) {
        for(int reltag = 0; reltag+2 <= whichTags["MuonIsolation"].size(); reltag++)
            isolatedMuons = filterIsolation(isolatedMuons, reltag);
    }
    return isolatedMuons;
}

std::vector<Electron*> AnalysisBase::filterIsolation(std::vector<Electron*> unfiltered, int relative_tag) {
    if(relative_tag < 0)
        Global::abort("AnalysisHandler", "You cannot ask for an electron isolation tag with index smaller than 0! ("+analysis+")");
    std::vector<int> analysisSpecificFlags = whichTags["ElectronIsolation"];
    if (relative_tag+2 > analysisSpecificFlags.size())
        Global::abort("AnalysisHandler", "You cannot ask for an electron isolation tag with index larger than "+Global::intToStr((int)analysisSpecificFlags.size()-2)+" in "+analysis);
    std::vector<Electron*> isolatedElectrons;
    for (int p = 0; p < unfiltered.size(); p++) {
        if (electronIsolationTags[unfiltered[p]][analysisSpecificFlags[relative_tag+1]]) // note that the 0th condition is reserved for the internal superloose condition
            isolatedElectrons.push_back(unfiltered[p]);
    }
    return isolatedElectrons;
}

std::vector<Electron*> AnalysisBase::filterIsolation(std::vector<Electron*> unfiltered, std::vector<int> relative_tags) {
    std::vector<Electron*> isolatedElectrons = unfiltered;
    // run filterIsolation on all reltags in the vector
    for(int t = 0; t < relative_tags.size(); t++)
        isolatedElectrons = filterIsolation(isolatedElectrons, relative_tags[t]);
    // If no flags are given then all should be checked
    if (relative_tags.size() == 0) {
        for(int reltag = 0; reltag+2 <= whichTags["ElectronIsolation"].size(); reltag++)
            isolatedElectrons = filterIsolation(isolatedElectrons, reltag);
    }
    return isolatedElectrons;
}

bool AnalysisBase::checkTauTag(Jet* candidate, std::string efficiency) {
    if (efficiency == "loose" && jetTauTags[candidate][0])
        return true;
    else if (efficiency == "medium" && jetTauTags[candidate][1])
        return true;
    else if (efficiency == "tight" && jetTauTags[candidate][2])
        return true;
    return false;
}

bool AnalysisBase::checkBTag(Jet* candidate, int relative_tag) {
    if(relative_tag < 0)
        Global::abort("AnalysisHandler", "You cannot ask for a btag with index smaller than 0! ("+analysis+")");
    std::vector<int> analysisSpecificFlags = whichTags["BJetTagging"];
    if (relative_tag+1 > analysisSpecificFlags.size())
        Global::abort("AnalysisHandler", "You cannot ask for a btag with index larger than "+Global::intToStr((int)analysisSpecificFlags.size()-1)+" in "+analysis);
    return (jetBTags.find(candidate)->second)[analysisSpecificFlags[relative_tag]];
}

double AnalysisBase::mT(const TLorentzVector & vis, const TLorentzVector & invis, const double m_invis) {
    // If no masses are given, assumed to be zero  
    return sqrt(m_invis*m_invis +  2.*vis.Pt()*invis.Et()*(1.-cos(fabs(vis.DeltaPhi(invis)))));
}

double AnalysisBase::mT2(const TLorentzVector & vis1, const TLorentzVector & vis2, double m_inv, const TLorentzVector & invis) {
    // Setup mt2 evaluation object.
    mt2_bisect::mt2 mt2_event;
    TLorentzVector zeroVector = TLorentzVector(0. ,0. ,0. ,0.);
    // If no invis is given, use missingET. Note that pmiss[0] is irrelvant, which is why we set it to -1.
    double pmiss[] = {-1, missingET->P4().Px(), missingET->P4().Py()};
    if (invis != zeroVector) {
        pmiss[0] = -1;
        pmiss[1] = invis.Px();
        pmiss[2] = invis.Py();
    }
 
    // Construct arrays that mt2_bisect needs as input and start evaluation
    double p1[3] = {vis1.M(), vis1.Px(), vis1.Py()};
    double p2[3] = {vis2.M(), vis2.Px(), vis2.Py()};
    mt2_event.set_momenta( p1, p2, pmiss );
    mt2_event.set_mn( m_inv );
    return mt2_event.get_mt2();  
}

double AnalysisBase::mCT(const TLorentzVector & v1,const TLorentzVector & v2)
{
    mctlib::mct mct_event;
    double v1t[4] = {v1.E(),v1.Px(),v1.Py(),v1.Pz()};
    double v2t[4] = {v2.E(),v2.Px(),v2.Py(),v2.Pz()};
    return mct_event.mctnorm(v1t,v2t);  //returns 'normal' mCT
}

double AnalysisBase::mCTcorr(const TLorentzVector & v1, const TLorentzVector & v2,const TLorentzVector & vds,const TLorentzVector & invis,const double ecm,const double mxlo)
{
    mctlib::mct mct_event;
    double v1t[4] = {v1.E(),v1.Px(),v1.Py(),v1.Pz()};
    double v2t[4] = {v2.E(),v2.Px(),v2.Py(),v2.Pz()};
    double vdst[4] = {vds.E(),vds.Px(),vds.Py(),vds.Pz()};
    double ptmt[2] = {invis.Px(),invis.Py()};
    return mct_event.mctcorr(v1t,v2t,vdst,ptmt,ecm,mxlo);
}

double AnalysisBase::mCTperp(const TLorentzVector & v1, const TLorentzVector & v2,const TLorentzVector & vds,const TLorentzVector & invis)
{
    mctlib::mct mct_event;
    double v1t[4] = {v1.E(),v1.Px(),v1.Py(),v1.Pz()};
    double v2t[4] = {v2.E(),v2.Px(),v2.Py(),v2.Pz()};
    double vdst[4] = {vds.E(),vds.Px(),vds.Py(),vds.Pz()};
    double ptmt[2] = {invis.Px(),invis.Py()};
    return mct_event.mcy(v1t,v2t,vdst,ptmt);
}

double AnalysisBase::mCTparallel(const TLorentzVector & v1, const TLorentzVector & v2,const TLorentzVector & vds,const TLorentzVector & invis)
{
    mctlib::mct mct_event;
    double v1t[4] = {v1.E(),v1.Px(),v1.Py(),v1.Pz()};
    double v2t[4] = {v2.E(),v2.Px(),v2.Py(),v2.Pz()};
    double vdst[4] = {vds.E(),vds.Px(),vds.Py(),vds.Pz()};
    double ptmt[2] = {invis.Px(),invis.Py()};
    return mct_event.mcx(v1t,v2t,vdst,ptmt);
}

double AnalysisBase::mT2_bl(const TLorentzVector & pl_in, const TLorentzVector & pb1_in, const TLorentzVector & pb2_in, const TLorentzVector & invis) {
    // Setup mt2_bl evaluation object.
    mt2bl_bisect::mt2bl mt2bl_event;
    TLorentzVector zeroVector = TLorentzVector(0. ,0. ,0. ,0.);
   
    double pl[4] = {pl_in.E(), pl_in.Px(), pl_in.Py(), pl_in.Pz()};      // El, plx, ply, plz,     (visible lepton)
    double pb1[4] = {pb1_in.E(), pb1_in.Px(), pb1_in.Py(), pb1_in.Pz()};  // Eb1, pb1x, pb1y, pb1z  (bottom on the same side as the visible lepton)
    double pb2[4] = {pb2_in.E(), pb2_in.Px(), pb2_in.Py(), pb2_in.Pz()};  // Eb2, pb2x, pb2y, pb2z  (other bottom, paired with the invisible W)
 
    // If no invis is given, use missingET. Note that pmiss[0] is irrelvant, which is why we set it to -1.
    double pmiss[] = {-1, missingET->P4().Px(), missingET->P4().Py()};
    if (invis != zeroVector) {
        pmiss[0] = -1;
        pmiss[1] = invis.Px();
        pmiss[2] = invis.Py();
    }
 
    // Construct arrays that mt2_bisect needs as input and start evaluation
    mt2bl_event.set_momenta(pl,pb1,pb2,pmiss);

    return mt2bl_event.get_mt2bl();  
}

double AnalysisBase::mT2_w(const TLorentzVector & pl_in, const TLorentzVector & pb1_in, const TLorentzVector & pb2_in, const TLorentzVector & invis, const double & upper_bound, const double & error_value, const double & scan_step) {
    // Setup mt2_bl evaluation object.
    //mt2w_bisect::mt2w mt2w_event(700.0, 699.0, 0.5);
    mt2w_bisect::mt2w mt2w_event(upper_bound, error_value, scan_step);   //set to default values, upper_bound = 2000.0, error_value = -1.0, scan_step = 0.5
    TLorentzVector zeroVector = TLorentzVector(0. ,0. ,0. ,0.);
   
    double pl[4] = {pl_in.E(), pl_in.Px(), pl_in.Py(), pl_in.Pz()};      // El, plx, ply, plz,     (visible lepton)
    double pb1[4] = {pb1_in.E(), pb1_in.Px(), pb1_in.Py(), pb1_in.Pz()};  // Eb1, pb1x, pb1y, pb1z  (bottom on the same side as the visible lepton)
    double pb2[4] = {pb2_in.E(), pb2_in.Px(), pb2_in.Py(), pb2_in.Pz()};  // Eb2, pb2x, pb2y, pb2z  (other bottom, paired with the invisible W)
 
    // If no invis is given, use missingET. Note that pmiss[0] is irrelvant, which is why we set it to -1.
    double pmiss[] = {-1, missingET->P4().Px(), missingET->P4().Py()};
    if (invis != zeroVector) {
        pmiss[0] = -1;
        pmiss[1] = invis.Px();
        pmiss[2] = invis.Py();
    }
 
    // Construct arrays that mt2_bisect needs as input and start evaluation
    mt2w_event.set_momenta(pl,pb1,pb2,pmiss);

    return mt2w_event.get_mt2w();  
}

double AnalysisBase::alphaT(const std::vector<Jet*> & jets, const double thresh_ET) {
    // alphaT code supplied by CMS
    // Need to pass jets and energy threshold of jets to be considered for variable (allows different ET to that of baseline jets)

    double HT = 0.;
    double mHTNorm = 0.;
    std::vector<Jet*> jetsThresh;
    TLorentzVector vecHT;
    for (int i = 0; i < jets.size(); i++) {
      double ET = jets[i]->P4().Et();
      if (ET > thresh_ET) {
        jetsThresh.push_back(jets[i]);
        HT += ET;
        vecHT += jets[i]->P4();
      }
    }
    double mHT = vecHT.Pt();
   
    std::vector<double> diff( 1<<(jetsThresh.size()-1) , 0. );
    for (unsigned i=0; i < diff.size(); i++) {
      for (unsigned j=0; j < jetsThresh.size(); j++) {
        diff[i] += jetsThresh[j]->P4().Et() * ( 1 - 2 * (int(i>>j)&1) ) ;
      }
    }
    double DHT = fabs( *min_element( diff.begin(), diff.end(), fabs_less() ) );
    double alphaT = 0.5 * ( HT - DHT ) / sqrt( HT*HT - mHT*mHT );
   
    return alphaT;
}

std::vector<double> AnalysisBase::razor(const std::vector<TLorentzVector> & finalStateObj, const TLorentzVector & invis) {
    // Razor code supplied by CMS (Maurizio Pierini)
    // Need to pass objects (4 vectors that could be jets or leptons) and missingET
    // Result is a 2 dimensional vector, [0] = MR, [1] = R

    TLorentzVector j1, j2;
    bool foundGood = false;
    int N_comb = 1;
    for(int i = 0; i < finalStateObj.size(); i++)
      N_comb *= 2;
 
    double M_min = 9999999999.0;
    int j_count;
    for(int i = 1; i < N_comb-1; i++) {
      TLorentzVector j_temp1, j_temp2;
      int itemp = i;
      j_count = N_comb/2;
      int count = 0;
      while(j_count > 0){
        if(itemp/j_count == 1)
          j_temp1 += finalStateObj[count];
        else
          j_temp2 += finalStateObj[count];
        itemp -= j_count*(itemp/j_count);
        j_count /= 2;
        count++;
      }
      double M_temp = j_temp1.M2()+j_temp2.M2();
      // smallest mass
      if (M_temp < M_min) {
        M_min = M_temp;
        j1 = j_temp1;
        j2 = j_temp2;
      }
    }  
    if (j2.Pt() > j1.Pt()) {
      TLorentzVector temp = j1;
      j1 = j2;
      j2 = temp;
    }  
   
    double A = j1.P();
    double B = j2.P();
    double az = j1.Pz();
    double bz = j2.Pz();
    TVector3 j1T, j2T;
    j1T.SetXYZ(j1.Px(),j1.Py(),0.0);
    j2T.SetXYZ(j2.Px(),j2.Py(),0.0);
    double ATBT = (j1T+j2T).Mag2();
    double MR = sqrt((A+B)*(A+B)-(az+bz)*(az+bz)-(j2T.Dot(j2T)-j1T.Dot(j1T))*(j2T.Dot(j2T)-j1T.Dot(j1T))/(j1T+j2T).Mag2());
    double mybeta = (j2T.Dot(j2T)-j1T.Dot(j1T))/sqrt(ATBT*((A+B)*(A+B)-(az+bz)*(az+bz)));
    double mygamma = 1./sqrt(1.-mybeta*mybeta);
    //gamma times MRstar  
    MR = MR*mygamma;
 
    double MRT = missingET->P4().Et()*(j1.Pt()+j2.Pt()) - missingET->P4().Vect().Dot(j1.Vect()+j2.Vect());
    MRT /= 2.;
    MRT = sqrt(MRT);
   
    double R = MRT/MR;
   
    std::vector<double> razor;
    razor.push_back(MR);
    razor.push_back(R);
   
    return razor;
}

bool AnalysisBase::sortByPT(const FinalStateObject *lhs, const FinalStateObject *rhs) {
     return lhs->PT > rhs->PT;
}

double AnalysisBase::topness(const TLorentzVector & pl_in, const TLorentzVector & pb1_in, const TLorentzVector & pb2_in, const TLorentzVector & invis, const double & sigmat, const double & sigmaW, const double & sigmas){
  
    double pl[4] = {pl_in.Px(), pl_in.Py(), pl_in.Pz(), pl_in.E()};      // plx, ply, plz, El     (visible lepton)
    double pb1[4] = {pb1_in.Px(), pb1_in.Py(), pb1_in.Pz(), pb1_in.E() };  // pb1x, pb1y, pb1z, Eb1  (bottom on the same side as the visible lepton)
    double pb2[4] = {pb2_in.Px(), pb2_in.Py(), pb2_in.Pz(), pb2_in.E()};
    double MET[4] = {invis.Px(), invis.Py(), 0.0, 0.0 };
  
    double xbest[4];
    double topness = log(topnesscompute(pb1, pb2, pl, MET, sigmat, sigmaW, sigmas, xbest));      
    
    return topness;
}

std::vector<double> AnalysisBase::superRazor(TLorentzVector l0, TLorentzVector l1, const TLorentzVector & met) {
  
    // ==========================================================================
    // Variable Name in ATLAS-CONF-2016-076    <=> Mapping to the variables below
    // ==========================================================================
    // R_{PT}                                  <=> pT_CM/(pT_CM+SHATR/4)
    // 1/gamma_{R+1}                           <=> 1/gamma_R 
    // M_{#Deta}^{R}                           <=> MDELTAR
    // #delta#phi_{#beta}^{R}                  <=> dphi_LL_vBETA_T
    //
    //      Authors:
    //            Alaettin Serhan Mete, alaettin.serhan.mete@cern.ch;
    //            Tommaso Lari, Tommaso.Lari@mi.infn.it; 
    //            Federico Meloni, federico.meloni@cern.ch; 
    //            Iacopo Vivarelli, iacopo.vivarelli@cern.ch; 
    //            Daniel Antrim, daniel.joseph.antrim@cern.ch;
    //
    //      Thanks to Matthew R. Buckley, mbuckley@physics.rutgers.edu
    //
    // Details given in paper (http://arxiv.org/abs/1310.4827) written by
    // Matthew R. Buckley, Joseph D. Lykken, Christopher Rogan, Maria Spiropulu

    //auxiliary variables
    TVector3 vBETA_z;
    TVector3 pT_CM;
    TVector3 vBETA_T_CMtoR;
    TVector3 vBETA_R;
    double SHATR;
    double dphi_LL_vBETA_T;
    double dphi_L1_L2;
    double gamma_R;
    double dphi_vBETA_R_vBETA_T;
    double MDELTAR;
    double costhetaRp1;

    //
    // Lab frame
    //
    //Longitudinal boost
    vBETA_z = (1. / (l0.E() + l1.E()))*(l0 + l1).Vect();
    vBETA_z.SetX(0.0);
    vBETA_z.SetY(0.0);

    l0.Boost(-vBETA_z);
    l1.Boost(-vBETA_z);

    //pT of CM frame
    pT_CM = (l0 + l1).Vect() + met.Vect();
    pT_CM.SetZ(0.0);

    TLorentzVector ll = l0 + l1;
    //invariant mass of the total event
    SHATR = sqrt(2.*(ll.E()*ll.E() - ll.Vect().Dot(pT_CM)
             + ll.E()*sqrt(ll.E()*ll.E() + pT_CM.Mag2() - 2.*ll.Vect().Dot(pT_CM))));

    vBETA_T_CMtoR = (1. / sqrt(pT_CM.Mag2() + SHATR*SHATR))*pT_CM;

    l0.Boost(-vBETA_T_CMtoR);
    l1.Boost(-vBETA_T_CMtoR);
    ll.Boost(-vBETA_T_CMtoR);

    //
    //R-frame
    //
    dphi_LL_vBETA_T = fabs((ll.Vect()).DeltaPhi(vBETA_T_CMtoR));

    dphi_L1_L2 = fabs(l0.Vect().DeltaPhi(l1.Vect()));

    vBETA_R = (1. / (l0.E() + l1.E()))*(l0.Vect() - l1.Vect());

    gamma_R = 1. / sqrt(1. - vBETA_R.Mag2());

    dphi_vBETA_R_vBETA_T = fabs(vBETA_R.DeltaPhi(vBETA_T_CMtoR));

    l0.Boost(-vBETA_R);
    l1.Boost(vBETA_R);

    //
    //R+1 frame
    //
    MDELTAR = 2.*l0.E();
    costhetaRp1 = l0.Vect().Dot(vBETA_R) / (l0.Vect().Mag()*vBETA_R.Mag());

    double R_p_T = pT_CM.Mag()/(pT_CM.Mag()+SHATR/4.);
    double inv_r = 1./gamma_R;
    double dphi = dphi_LL_vBETA_T;

    std::vector<double> srazor;
    srazor.push_back(R_p_T);
    srazor.push_back(inv_r);
    srazor.push_back(dphi);
    srazor.push_back(MDELTAR);

    return srazor;
}

double AnalysisBase::aplanarity(const std::vector<Jet*> input_jets) {
  
  TMatrixD MomentumMatrix(3,3);
  double PAbs=0.;
  for (int i = 0; i < jets.size(); i++) PAbs+=jets[i]->P4().Vect().Mag2();

  for (int i=0;i<3;i++) {
    for(int j=0;j<3;j++) {
      double PSum = 0.;
      for(int k=0;k<jets.size();k++) PSum+=jets[k]->P4().Vect()[i]*jets[k]->P4().Vect()[j]/PAbs;
      MomentumMatrix[i][j] = PSum;
    }
  }
  
  TMatrixDEigen Eval(MomentumMatrix);
  TVectorD Eigenval = Eval.GetEigenValuesRe();
  return 1.5*Eigenval.Min();
  
}

