#include<iostream>
#include<string>
#include <stdlib.h>
#include <cmath>

// code written by Michael L. Graesser 06/2015

using namespace std;

const double mt=172.;  // top quark mass
const double mW=80.4;  // W mass

double my_lp(double u[4], double v[4]); 

void my_add(double a[4], double b[4], double c[4]); 

struct my_func
{
  double pb1[4];
  double pb2[4];
  double pl[4];
  double pMET[4];
  double sigmat, sigmaW, sigmas, sign;

  my_func(double ppb1[4], double ppb2[4], double ppl[4], double ppMET[4], double sigmatt, double sigmaWW, double sigmass, double ssign) :
  sigmat (sigmatt), sigmaW(sigmaWW), sigmas(sigmass), sign(ssign) {
    pb1[0]=ppb1[0];
    pb1[1]=ppb1[1];
    pb1[2]=ppb1[2];
    pb1[3]=ppb1[3];
    pb2[0]=ppb2[0];
    pb2[1]=ppb2[1];
    pb2[2]=ppb2[2];
    pb2[3]=ppb2[3];
    pl[0]=ppl[0];
    pl[1]=ppl[1];
    pl[2]=ppl[2];
    pl[3]=ppl[3];
    pMET[0]=ppMET[0];
    pMET[1]=ppMET[1];
    pMET[2]=ppMET[2];
    pMET[3]=ppMET[3];
  }
  double operator()(double points[],int d) {  // points[0]=pv_x, points[1]=pv_y, points[2]=pv_z, points[3]=pW_z                                                                                         
    // d is size of points = 4                                                                                                                                                                        
    // pv_x, pv_y, pv_z                                                                                                                                                                               
    double pvx=points[0];
    double pvy=points[1];
    double pvz=points[2];
    // neutrino energy assuming mass-shell condition                                                                                                                                                  
    double Ev=sqrt(pow(pvx,2)+pow(pvy,2)+pow(pvz,2));

    double pv[]={pvx,pvy,pvz,Ev};

    // pW_z                                                                                                                                                                                           
    double pWz=points[3];
    // cout << "points=" << pvx << ", " << pvy << ", " << pvz << ", " << pWz << endl;                                                                                                                 
    // W momenta from neutrino and MET                                                                                                                                                                
    double pW[]={-pvx+pMET[0],-pvy+pMET[1],pWz,sqrt(pow(-pvx+pMET[0],2)+pow(-pvy+pMET[1],2)+pow(pWz,2) + pow(mW,2))};

    double pb1W[4];
    my_add(pb1,pW,pb1W);
    double plv[4];
    my_add(pl,pv,plv);
    double pb2lv[4];
    my_add(pb2,plv,pb2lv);
    double pb1b2lvW[4];
    my_add(pb2lv,pb1W,pb1b2lvW);
    // function to minimize                                                                                                                                                                           
    double fsum=0.;
    fsum= pow(my_lp(pb1W,pb1W)-pow(mt,2),2)/pow(sigmat,4)+pow(my_lp(pb2lv,pb2lv)-pow(mt,2),2)/pow(sigmat,4)+pow(my_lp(plv,plv)-pow(mW,2),2)/pow(sigmaW,4)+pow(my_lp(pb1b2lvW,pb1b2lvW)-4*pow(mt,2),2)\
      /pow(sigmas,4);
    //cout << "fsum = " << fsum << endl;                                                                                                                                                              
    return sign*fsum;
  }
};


