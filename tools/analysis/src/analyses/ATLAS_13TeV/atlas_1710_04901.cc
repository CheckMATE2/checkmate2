#include "atlas_1710_04901.h"
// AUTHOR: Boogeyman
//  EMAIL: boogeyman@horror.hell
void Atlas_1710_04901::initialize() {
  setAnalysisName("atlas_1710_04901");          
  setInformation(""
    "# atlas, 13TeV, tracks, displaced vertices and missing energy\n"
  "");
  setLuminosity(32.8*units::INVFB);      
  bookSignalRegions("SR1");
  // You can also book cutflow regions with bookCutflowRegions("CR1;CR2;..."). Note that the regions are
  //  always ordered alphabetically in the cutflow output files.

  // You should initialize any declared variables here
}

void Atlas_1710_04901::analyze() {
double acceffmap[12][8][8] = {{{0.2802,0.2851,0.2774,0.2655,0.1954,0.2125,0.1364,0.0},{0.3737,0.4521,0.4458,0.426,0.3844,0.2568,0.2308,0.2857},{0.4115,0.5969,0.6455,0.6645,0.6119,0.4888,0.4231,0.371},{0.4033,0.6468,0.7599,0.8011,0.7655,0.7114,0.6726,0.6282},{0.3543,0.6216,0.7561,0.8215,0.8217,0.8138,0.791,0.7518,},{0.2766,0.5018,0.678,0.809,0.845,0.8591,0.8581,0.8383},{0.0,0.0,0.5882,0.7438,0.87,0.8823,0.8855,0.8836},{0.0,0.0,0.0,1.0,0.84,0.89,0.8928,0.8934}},{{0.06298,0.07806,0.07449,0.04,0.01613,0.125,0.0,0.0},{0.1039,0.1169,0.1073,0.09719,0.07746,0.1111,0.0,0.0},{0.1128,0.1577,0.1887,0.1834,0.1533,0.1278,0.129,0.0},{0.1085,0.1812,0.2364,0.246,0.2322,0.2019,0.195,0.1299},{0.1463,0.1989,0.2282,0.2386,0.2558,0.2557,0.2399,0.205},{0.0,0.1053,0.1442,0.2381,0.2723,0.2785,0.2776,0.2335},{0.0,0.0,0.0,0.1111,0.2438,0.2757,0.2942,0.3022},{0.0,0.0,0.0,0.0,0.5,0.2692,0.3011,0.29}},{{0.08709,0.09924,0.07775,0.1101,0.1585,0.1111,0.0,0.0},{0.1407,0.1804,0.2052,0.1549,0.1504,0.125,0.25,0.0},{0.1602,0.2565,0.2773,0.3059,0.2537,0.2091,0.1711,0.0},{0.1699,0.2704,0.3847,0.4098,0.3943,0.3351,0.3103,0.1932},{0.08537,0.295,0.4156,0.4211,0.4452,0.4396,0.3898,0.3735},{0.0,0.125,0.3211,0.3761,0.4242,0.4816,0.477,0.4318},{0.0,0.0,0.0,0.3077,0.449,0.4882,0.5007,0.4932},{0.0,0.0,0.0,0.0,1.0,0.4673,0.5015,0.4849}},{{0.01048,0.006224,0.01079,0.01144,0.01399,0.0,0.0,0.0},{0.01149,0.01918,0.02122,0.01764,0.01205,0.0,0.0,0.0},{0.02023,0.02106,0.03253,0.03222,0.02877,0.01479,0.02752,0.0},{0.01583,0.03566,0.03645,0.04561,0.0487,0.0422,0.02811,0.04762},{0.0125,0.0142,0.03707,0.0464,0.05051,0.04665,0.04462,0.03311},{0.0,0.02857,0.02679,0.03477,0.04878,0.05055,0.05072,0.04425},{0.0,0.0,0.0,0.01449,0.05893,0.04793,0.05554,0.05854},{0.0,0.0,0.0,0.0,0.0,0.05164,0.05875,0.05403}},{{0.04526,0.05573,0.04129,0.02875,0.05882,0.0,0.0,0.0},{0.07339,0.06595,0.07368,0.07339,0.05952,0.07692,0.125,0.0},{0.08318,0.1195,0.1278,0.1332,0.1103,0.068,0.04598,0.0},{0.08327,0.1588,0.2098,0.2214,0.1871,0.1449,0.1491,0.1339},{0.09449,0.146,0.2238,0.2458,0.2425,0.2282,0.2031,0.1425},{0.0,0.04167,0.1302,0.199,0.2701,0.2834,0.2658,0.2325},{0.0,0.0,0.0,0.1481,0.2636,0.3228,0.3356,0.3272},{0.0,0.0,0.0,0.0,0.25,0.3054,0.3703,0.377}},{{0.04293,0.04807,0.04606,0.0498,0.02996,0.0,0.1429,0.0},{0.05983,0.07004,0.07386,0.0672,0.06068,0.01695,0.0,0.0},{0.07426,0.1059,0.1198,0.1189,0.09594,0.05743,0.05677,0.0},{0.087,0.1356,0.1665,0.182,0.161,0.131,0.1185,0.09375},{0.04104,0.1332,0.1943,0.221,0.1947,0.1795,0.1493,0.1384},{0.0,0.09459,0.1207,0.2087,0.2218,0.234,0.2208,0.1939},{0.0,0.0,0.0,0.1382,0.2279,0.2789,0.287,0.2653},{0.0,0.0,0.0,0.0,0.07692,0.2927,0.3211,0.3204}},{{0.08585,0.1027,0.1188,0.09314,0.1772,0.0,0.0,0.0},{0.141,0.1692,0.1656,0.1588,0.1719,0.125,0.0,0.0},{0.1774,0.225,0.2749,0.2751,0.2597,0.2229,0.08065,0.3},{0.1919,0.311,0.3974,0.433,0.3883,0.3116,0.2369,0.2},{0.1852,0.2565,0.4545,0.5376,0.4785,0.4227,0.3787,0.3006},{0.0,0.2353,0.305,0.4257,0.5127,0.5512,0.5131,0.4621},{0.0,0.0,0.0,0.2564,0.4933,0.6646,0.6902,0.634},{0.0,0.0,0.0,0.0,0.5,0.7143,0.8095,0.7987}},{{0.02209,0.0227,0.03417,0.02653,0.06122,0.0,0.0,0.0},{0.03759,0.03088,0.03767,0.02207,0.04331,0.0,0.0,0.0},{0.04217,0.06129,0.06257,0.06395,0.06502,0.01994,0.024,0.0},{0.03908,0.08071,0.1045,0.1235,0.1063,0.06798,0.05522,0.005263},{0.0303,0.04902,0.1097,0.1516,0.1374,0.1048,0.09298,0.05498},{0.0,0.02632,0.075,0.1139,0.1593,0.1582,0.1416,0.1159},{0.0,0.0,0.0,0.01515,0.1503,0.2051,0.2178,0.1802},{0.0,0.0,0.0,0.0,0.0,0.2258,0.2783,0.2701}},{{0.0399,0.02765,0.03723,0.05405,0.05882,0.1667,0.0,0.0},{0.05324,0.09059,0.06061,0.05291,0.06849,0.0,0.0,0.0},{0.0501,0.0783,0.09736,0.07677,0.08451,0.08491,0.07692,0.0},{0.04339,0.09733,0.1602,0.1664,0.1506,0.1033,0.05618,0.07843},{0.0,0.07273,0.1429,0.2091,0.2126,0.1778,0.1327,0.07729},{0.0,0.0,0.07895,0.1193,0.2051,0.2299,0.1977,0.1584},{0.0,0.0,0.0,0.07407,0.1886,0.3106,0.3348,0.2758},{0.0,0.0,0.0,0.0,0.5,0.2658,0.463,0.4624}},{{0.01618,0.01306,0.01833,0.01293,0.04878,0.0,0.0,0.0},{0.02299,0.0379,0.03851,0.03326,0.03311,0.0,0.0,0.0},{0.02984,0.04244,0.05501,0.04615,0.05382,0.02551,0.0,0.0},{0.02929,0.05095,0.07778,0.09255,0.07854,0.05007,0.03302,0.01869},{0.01527,0.02734,0.08413,0.1242,0.1095,0.08245,0.06969,0.05581},{0.0,0.0,0.04651,0.1159,0.1213,0.1332,0.107,0.06965},{0.0,0.0,0.0,0.06383,0.1259,0.179,0.1752,0.1454},{0.0,0.0,0.0,0.0,0.0,0.1827,0.2426,0.2236}},{{0.05729,0.08098,0.06491,0.08754,0.1,0.0,0.0,0.0},{0.0903,0.1148,0.09257,0.1162,0.15,0.1364,0.0,0.0},{0.09463,0.1384,0.1577,0.171,0.1402,0.1056,0.08,0.0},{0.06274,0.1812,0.2492,0.2839,0.2449,0.1481,0.0967,0.06316},{0.07752,0.1029,0.2402,0.3456,0.3193,0.2494,0.1976,0.1374},{0.0,0.03448,0.1397,0.2534,0.3387,0.3675,0.3175,0.254},{0.0,0.0,0.0,0.08772,0.2887,0.4824,0.5056,0.4423},{0.0,0.0,0.0,0.0,0.0,0.4823,0.6816,0.6378}},{{0.01546,0.01938,0.0171,0.02936,0.04375,0.0,0.0,0.0},{0.02097,0.03137,0.0263,0.04321,0.01538,0.0,0.0,0.0},{0.03,0.04387,0.05253,0.05951,0.05207,0.0122,0.0229,0.0},{0.02161,0.04796,0.08033,0.0959,0.08688,0.0532,0.04351,0.02956},{0.003922,0.02068,0.06417,0.114,0.1059,0.08409,0.06343,0.04305},{0.0,0.0,0.03704,0.06935,0.1007,0.1197,0.1048,0.07524},{0.0,0.0,0.0,0.009615,0.07536,0.1389,0.1545,0.1347},{0.0,0.0,0.0,0.0,0.0,0.1051,0.2146,0.2051}}};

  // Your eventwise analysis code goes here
  // The following objects are always defined unless they are 'ignored' above. They form std::vector objects of the respective Delphes class type (except for Etmiss which is a single object)
  // All std::vector members and etmiss have the common properties PT, Eta, Phi and P4() with the latter giving access to the full ROOT TLorentzVector.
  // Within a std::vector, all members are ordered with highest pt coming first.

  // electronsLoose, electronsMedium, electronsTight   are list of electrons that passed respective efficiency and reconstruction cuts
  // muonsCombinedPlus, muonsCombined                  as above for muons
  // photonsMedium                                     as above for photons
  // jets are all reconstructed jets                   as above for jets. Note that electrons are most certainly also reconstructed as a jet -> overlap removal do avoid double counting necessary!
  // tracks, towers                                    calorimeter and tracker information. Usually not needed.
  // missingET                                         rec missing ET EXCLUDING muons.

  
  // Here is a couple of useful functions and lines:  
  //------------Phase Space Cuts (defined for jets, electronsXYZ, muonsXYZ, photonsXYZ)
  // jets = filterPhaseSpace(jets, 20., -2.8, 2.8)  // The vector 'jets' only contains jets with pt >= 20 GeV and -2.8 < eta < 2.8. This function is applicable to other particles too (electronsMedium, ... ).
  // jets = overlapRemoval(jets, electronsLoose, 0.2) Removes all jets for which there exists any electron in 'electronsLoose' with deltaR < 0.2.
  // jets = overlapRemovel(jets, 0.2) If two jets overlap within deltaR < 0.2, only the harder jet is stored.
  
  //------------Isolation Checks (defined for electronsXYZ, muonsXYZ, photonsXYZ
  //------------        For each object, if the user entered N isolation conditions, they can be
  //------------        checked individually be the second argument (running from 0 to N-1).
  // electronsMedium = filterIsolation(electronsMedium, 0)            Removes electrons that do not pass the first isolation condition entered into the AnalysisManager by the user
  // std::vector<int> flags; flags.push_back(0); flags.push_back(2);
  // electronsMedium = filterIsolation(electronsMedium, flags)        Same as above, but both the first and the third condition have to be fulfilled
  // electronsMedium = filterIsolation(electronsMedium)               Same as above, but all conditions have to be fulfilled.
  
  //-----------Flavour Tag Checks (defined for jets only)
  //----------          Tau tags "loose", "medium" or "tight" can be individually checked if the user enabled tau tagging in the AM.
  //----------          For b-tags, if N working points have been defined, the ith condition can be tested by putting i-1 into the second argument (if there is only one, the argument can be omitted)
  // if checkTauTag(jets[0], "tight") leadingJetIsTagged = True;
  // if checkBTag(jets[0], 0) leadingJetIsBTagged = True;


  //-----------Auxiliary Information
  // - Always ensure that you don't access vectors out of bounds. E.g. 'if(jets[1]->PT > 150)' should rather be if (jets.size() > 1 && jets[1]->PT > 150). 
  // - Use rand()/(RAND_MAX+1.) for random numbers between 0 and 1. The random seed is determined from system time or by the RandomSeed parameter in CheckMATE.
  // - The 'return' statement will end this function for the current event and hence should be called whenever the current event is to be vetoed.
  // - Many advanced kinematical functions like mT2 are implemented. Check the manual for more information.
  // - If you need output to be stored in other files than the cutflow/signal files we provide, check the manual for how to do this conveniently.  

  missingET->addMuons(muonsCombined);  // Adds muons to missing ET. This should almost always be done which is why this line is not commented out.

  countCutflowEvent("1_initial");
  //       A: Missing ET cut
  TLorentzVector metVec = missingET->P4();
  float met = metVec.Pt();
  bool METcut = false;
  met=0.;
  metVec.SetPxPyPzE(0.,0.,0.,0.);
  vector <int> tooLLgluino;
//  for(int i=0;i<true_particles.size();++i){
//   auto part=true_particles[i];                           // open particle
//   if(abs(part->PID)==1000021){
//    float dist=TMath::Sqrt(part->X*part->X+part->Y*part->Y);
//    if(dist<3870.){
//     auto daughter=true_particles[part->D1];
//     dist=TMath::Sqrt(daughter->X*daughter->X+daughter->Y*daughter->Y);
//     if(dist>3870.){
//      met=met+part->PT;
//      tooLLgluino.push_back(i);                           // add the particle to the list
//     }
//    }
//   }
//  }
//  float rmax=0.;
  if(true_particles.size()<1)return;

  for(int i=0;i<true_particles.size();++i){               // loop over particles in the decay table
   auto part=true_particles[i];                           // open particle
   if(abs(part->PID)==1000022){
    if(part->Status<0)continue;                           // dismiss it if it is not stable
    bool motherintooLLg=false;
    if(tooLLgluino.size()>0){
     int decind=part->M1;
     while(decind>=tooLLgluino[0]){
      for(int j=0;j<tooLLgluino.size();++j){
       if(decind==tooLLgluino[j])motherintooLLg=true;
      }
      if(motherintooLLg)break;
      auto M=true_particles[decind];
      decind=M->M1;
     }
    }
    if(!motherintooLLg)metVec=metVec+part->P4();
   }
//   if(abs(part->PID)==1000021){
//    auto daughter=true_particles[part->D1];
//    float dist=TMath::Sqrt(daughter->X*daughter->X+daughter->Y*daughter->Y);
//    if(dist>rmax)rmax=dist;
//   }
  }
  met=metVec.Pt();
  if( met > 200. )METcut = true;                            // condition MET > 200GeV 
  if( !METcut )return;

//  countCutflowEvent("1_metcut");

  //       B: Define the displaced vertices

  //    1- Search the decay tables for displaced-produced stable charged particles

  vector <vector <float>> DVlist;
  vector <vector <int>> DVpart;
  vector <int> DVselprod;
  vector <vector <float>> DVmomentum;
  int counter=0;
  for(int i=0;i<true_particles.size();++i){               // loop over particles in the decay table
   auto part=true_particles[i];                           // open particle
   if(part->Status<0)continue;                            // dismiss it if it is not stable
   if(abs(part->Charge)==0)continue;                      // dismiss it if it is neutral
   if(part->X*part->X+part->Y*part->Y<1.)continue;        // dismiss it if it is not sufficiently displaced
    counter=counter+1;
    int DVpos=-1;                                         // initialize the vertex position in the list
   if(DVlist.size()>0){                                   // if some DVs are already known
    for(int j=0;j<DVlist.size();++j){                     // check over them
     double dist=(DVlist[j][0]-part->X)*(DVlist[j][0]-part->X)+(DVlist[j][1]-part->Y)*(DVlist[j][1]-part->Y)+(DVlist[j][2]-part->Z)*(DVlist[j][2]-part->Z);                                                      // the distance between the production vertex of the particle and the stored DVs
     if(dist<1.)DVpos=j;                                  // if dist is below threshold then store position of the corresponding DV
    }
   }
   if(DVpos<0){                                           // if the particle is produced at an as yet unknown DV
    vector <float> DVloc{part->X,part->Y,part->Z};        // define its location
    DVlist.push_back(DVloc);                              // and store it
    vector <int> partinDV{i};                             // define the particle address
    DVpart.push_back(partinDV);                           // and store it 
    DVmomentum.push_back((vector <float>){0.,0.,0.,0.});  // initialize the 4-momentum location
    DVselprod.push_back(0);                               // initialize the counter of selected decay products
    DVpos=DVlist.size()-1;                                // store the new-created address  
   } else {                                               // if the DV was already known
    DVpart[DVpos].push_back(i);                           // add the particle to the list
    DVlist[DVpos][0]=(DVlist[DVpos][0]*(float)(DVpart[DVpos].size()-1)+part->X)/(float)(DVpart[DVpos].size()); // re-center the DV
    DVlist[DVpos][1]=(DVlist[DVpos][1]*(float)(DVpart[DVpos].size()-1)+part->Y)/(float)(DVpart[DVpos].size());
    DVlist[DVpos][2]=(DVlist[DVpos][2]*(float)(DVpart[DVpos].size()-1)+part->Z)/(float)(DVpart[DVpos].size());
   }
   vector <float> momentuminDV{part->Px,part->Py,part->Pz,TMath::Sqrt(part->Px*part->Px+part->Py*part->Py+part->Pz*part->Pz+0.14*0.14)};                                          // define 4-momentum of the particle (assuming a pion mass)
   TLorentzVector MomProd=part->P4();
   TLorentzVector PosProd(part->X,part->Y,part->Z,0.);
   float imppar = part->D0;                               // define impact parameter of the particle
   float apimppar = TMath::Sqrt(part->X*part->X+part->Y*part->Y)*fabs(sin(MomProd.DeltaPhi(PosProd)));                               // define impact parameter of the particle
   imppar=apimppar;
   if(abs(part->PT) > 1. && fabs(imppar) > 2.){           // check if the particle qualifies as a selected decay product
    DVselprod[DVpos]=DVselprod[DVpos]+1;                  // adds the selected decay product to the counter
    for(int j=0;j<4;++j){                                 // and add the 4-momentum
     DVmomentum[DVpos][j]=DVmomentum[DVpos][j]+momentuminDV[j];
    }
   }
  }
//    cout << "identified clusters" << ": " << DVlist.size() << ", " << endl;
//    if(DVlist.size()>0){
//    for(int i=0;i<DVlist.size();i++){   
//    cout << "cluster " << i << ": " << DVlist[i][0] << ", " << DVlist[i][1] << ", " << DVlist[i][2] << ", " << endl;
//    cout << " properties:" << DVpart[i].size() << ", " << DVselprod[i] << ", " << TMath::Sqrt(DVmomentum[i][3]*DVmomentum[i][3]-DVmomentum[i][0]*DVmomentum[i][0]-DVmomentum[i][1]*DVmomentum[i][1]-DVmomentum[i][2]*DVmomentum[i][2]) << ", " << TMath::Sqrt(DVlist[i][0]*DVlist[i][0]+DVlist[i][1]*DVlist[i][1]) << endl;
//    }
//    }

  //    2- Check if there is still some overlap between the various clusters
  if(DVlist.size()<1)return;

  bool copiedcluster[DVlist.size()];                                    // flag: is the cluster copied in some other?
  copiedcluster[0]=false;
  for(int i=0;i<DVlist.size()-1;i++){                                   // explore DVlist[DVlist.size()-1-i]
   int index=DVlist.size()-1-i;
   copiedcluster[index]=false;                                          // initialize copiedcluster
   bool overlap=false;                                                  // initialize correlation
   for(int j=0;j<index;j++){                                            // consider the earlier clusters DVlist[j]
    if(overlap)continue;                                                // if a correlation with an earlier cluster has already been detected: skip
    for(int k=0;k<DVpart[index].size();k++){                            // otherwise, look at the particles of cluster DVlist[index]
     for(int l=0;l<DVpart[j].size();l++){                               // and those of cluster DVlist[j]
      auto parti=true_particles[DVpart[index][k]];
      auto partj=true_particles[DVpart[j][l]];
      double dist=(parti->X-partj->X)*(parti->X-partj->X)+(parti->Y-partj->Y)*(parti->Y-partj->Y)+(parti->Z-partj->Z)*(parti->Z-partj->Z);                                                                    // assess their distance^2
      if(dist<1.)overlap=true;                                          // if it is below threshold, there is an overlap between the clusters
     }
    }
    if(overlap){                                                        // if the comparison has detected an overlap
     DVlist[j][0]=(DVlist[j][0]*(float)(DVpart[j].size())+DVlist[index][0]*(float)(DVpart[index].size()))/(float)(DVpart[index].size()+DVpart[j].size());                                                     // re-center the DV DVlist[j]
     DVlist[j][1]=(DVlist[j][1]*(float)(DVpart[j].size())+DVlist[index][1]*(float)(DVpart[index].size()))/(float)(DVpart[index].size()+DVpart[j].size());
     DVlist[j][2]=(DVlist[j][2]*(float)(DVpart[j].size())+DVlist[index][2]*(float)(DVpart[index].size()))/(float)(DVpart[index].size()+DVpart[j].size());                                                     // then copy the particles of DVlist[index] to the j-list     
     DVpart[j].insert(DVpart[j].end(),DVpart[index].begin(),DVpart[index].end()); 
     DVselprod[j]=DVselprod[j]+DVselprod[index];                        // add the selected decay product to the j-list
     for(int k=0;k<4;++k){                                              // and add the 4-momentum
       DVmomentum[j][k]=DVmomentum[index][k]+DVmomentum[j][k];
     }
     copiedcluster[index]=true;                                         // finally mark the cluster DVlist[index] as copied
    }
   }
  }
//    cout << "identified clusters" << ": " << DVlist.size() << ", " << endl;
//    if(DVlist.size()>0){
//    for(int i=0;i<DVlist.size();i++){   
//    cout << "cluster " << i << ": " << DVlist[i][0] << ", " << DVlist[i][1] << ", " << DVlist[i][2] << ", " << endl;
//    cout << " properties:" << DVpart[i].size() << ", " << DVselprod[i] << ", " << TMath::Sqrt(DVmomentum[i][3]*DVmomentum[i][3]-DVmomentum[i][0]*DVmomentum[i][0]-DVmomentum[i][1]*DVmomentum[i][1]-DVmomentum[i][2]*DVmomentum[i][2]) << ", " << copiedcluster[i] << "." << endl;
//    }
//    }

  //       C: Jet cuts
  //    1- Define trackless jets
  vector<int> Tljets;
  // implement trackless condition
  bool trackless = true;
  for (int i=0;i<jets.size();i++) {
   trackless = true;
   float PTsum = 0.;
   for (int j=0;j<DVlist.size();j++) {
    if(!copiedcluster[j] && DVpart[j].size()>0){
     for (int k=0;k<DVpart[j].size();k++) {
      auto part=true_particles[DVpart[j][k]];
      TLorentzVector MomProd=part->P4();
      TLorentzVector PosProd(part->X,part->Y,part->Z,0.);
      float apimppar = TMath::Sqrt(part->X*part->X+part->Y*part->Y)*fabs(sin(MomProd.DeltaPhi(PosProd)));                               // define impact parameter of the particle
      if(apimppar<2. && part->P4().DeltaR(jets[i]->P4())<0.4 ) {
       PTsum=PTsum+part->PT;                                            // add the PT's of all the tracks that are collinear to the jet i
  // NB: only colinearity of the 4-momentum is checked. In the DV context, this might not be sufficient / relevant.
      }
     }
    }
   }
   if( PTsum>5. ) trackless=false;                         // trackless condition PT(tracks) < 5GeV
   if(trackless) Tljets.push_back(i);
  }
//  cout << "genjets " << genjets.size() << endl;
//  for(int i=0;i<genjets.size();i++){
//   cout << "trackless jets " << i << ", " << genjets[Tljets[i]]->PT << endl;
//  }

  //    2- Applying jet cuts on 75% of the events
  double rno = rand()/(RAND_MAX+1.);
  int jetselIndex = 0;
  jetselIndex = rno < 0.75 ? 0 : 1;
  if( jetselIndex == 0 ) {
   if( Tljets.size()==0 ){
    return;                                                // case with no trackless jets
   } else if( jets[Tljets[0]]->PT < 70. ) {
   if( Tljets.size() > 1 ){
    if( jets[Tljets[1]]->PT < 25. )return;
    }else return;                                          // at least one trackless jet with PT > 70GeV or two jets with PT > 25GeV
   }
  }
//  countCutflowEvent("2_jetcut");
//   for(int i=0;i<jets.size();i++){
//   cout << jets[i]->PT << endl;
//   }
//   cout << genjets.size() << endl;
  //       D: Displaced vertex condition
  //    1- Select LLP's in the fiducial region
  vector <int> obsLLP;
  float rmax=0.;
  // Checking fiducial cuts
  bool DVfidtest=false;
  bool DVfidcut = true;
  for (int i=0;i<DVlist.size();i++) {
   if(copiedcluster[i])continue;
   float r = TMath::Sqrt(DVlist[i][0]*DVlist[i][0]+DVlist[i][1]*DVlist[i][1]);
   float z = DVlist[i][2];
   DVfidcut = true;
   if(r<4. || r>300. || abs(z)>300.)DVfidcut = false;      // is DV i in fiducial volume?
   float MDV=TMath::Sqrt(DVmomentum[i][3]*DVmomentum[i][3]-DVmomentum[i][0]*DVmomentum[i][0]-DVmomentum[i][1]*DVmomentum[i][1]-DVmomentum[i][2]*DVmomentum[i][2]);                                                      // mass of the DV i
   if(DVfidcut && (DVselprod[i] < 5 || MDV < 10.))DVfidcut = false; // Constraints on the number of selected products and DV mass
   if(DVfidcut)DVfidtest=true;
   if(r > rmax && DVselprod[i] > 4 && MDV > 10.){
    rmax=r;                                                // find the position of the farthest DV (even if it is beyond the search region?)
   }
//   cout << "DVfidcut: " << DVfidcut << endl;

  //    2- Reconstruction efficiency of the DV as a function of its position, number of tracks and mass

   int NR;
   int NMDV;
   int Nselprod;
   if( DVfidcut ) {
    if(r>=4. && r<22.){ NR = 0;}
    else if(r>=22. && r<25.){NR = 1;}
    else if(r>=25. && r<29.){NR = 2;}
    else if(r>=29. && r<38.){NR = 3;}
    else if(r>=38. && r<46.){NR = 4;}
    else if(r>=46. && r<73.){NR = 5;}
    else if(r>=73. && r<84.){NR = 6;}
    else if(r>=84. && r<111.){NR = 7;}
    else if(r>=111. && r<120.){NR = 8;}
    else if(r>=120. && r<145.){NR = 9;}
    else if(r>=145. && r<180.){NR = 10;}
    else if(r>=180. && r<300.){NR = 11;}

    if(MDV >= 10. && MDV <15.){NMDV = 0;}
    if(MDV >= 15. && MDV <20.){NMDV = 1;}
    if(MDV >= 20. && MDV <30.){NMDV = 2;}
    if(MDV >= 30. && MDV <50.){NMDV = 3;}
    if(MDV >= 50. && MDV <100.){NMDV = 4;}
    if(MDV >= 100. && MDV <200.){NMDV = 5;}
    if(MDV >= 200. && MDV <500.){NMDV = 6;}
    if(MDV >= 500. && MDV <=5000.){NMDV = 7;}

    if(DVselprod[i] >= 5 && DVselprod[i] <6){Nselprod = 0;}
    if(DVselprod[i] >= 6 && DVselprod[i] <7){Nselprod = 1;}
    if(DVselprod[i] >= 7 && DVselprod[i] <10){Nselprod = 2;}
    if(DVselprod[i] >= 10 && DVselprod[i] <15){Nselprod = 3;}
    if(DVselprod[i] >= 15 && DVselprod[i] <20){Nselprod = 4;}
    if(DVselprod[i] >= 20 && DVselprod[i] <30){Nselprod = 5;}
    if(DVselprod[i] >= 30 && DVselprod[i] <50){Nselprod = 6;}
    if(DVselprod[i] >= 50 && DVselprod[i] <=200){Nselprod = 7;} 

    rno = rand()/(RAND_MAX+1.);
    if (rno < acceffmap[NR][Nselprod][NMDV]){
     obsLLP.push_back(i);
    }                                     // apply the efficiency map
   }
  }
//  cout << "observed DV: " << obsLLP.size() << endl;
//  if(obsLLP.size()<1)return;
//  countCutflowEvent("3_fidcut");

  //       D: MET efficiency depending on the position of the farthest (?) DV
  rno = rand()/(RAND_MAX+1.);
  if(rmax < 1150.){
   if(met >= 200. && met<250.){
    if(rno > 0.2211)return;
   } else if(met>=250. && met < 500.){
    if(rno > 0.8361)return;
   } else if(met>=500. && met < 2500.){
    if(rno > 0.9823)return;
   } else return;
  } else if(rmax >= 1150. && rmax < 3870){
   if(met >= 200. && met<300.){
    if(rno > 0.4701)return;
   } else if(met>=300. && met < 1000.){
    if(rno > 0.7423)return;
   } else if(met>=1000. && met < 2500.){
    if(rno > 0.9173)return;
   } else return;
  } else if(rmax >= 3870){
   if(met >= 200. && met<250.){
    if(rno > 0.7351)return;
   } else if(met>=250. && met < 350.){
    if(rno > 0.8349)return;
   } else if(met>=350. && met < 2500.){
    if(rno > 0.8762)return;
   } else return;
  } else return;
  // apply the efficiency map
  countCutflowEvent("2_effETMiss");

  if(!DVfidtest)return;
  countCutflowEvent("3_DVfid");

  if(obsLLP.size()<1)return;
  countCutflowEvent("4_effDV");

  countSignalEvent("SR1");
}

void Atlas_1710_04901::finalize() {
  // Whatever should be done after the run goes here
}       
