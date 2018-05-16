#include<iostream>
#include<string>
#include <stdlib.h> 
#include <cmath> 

#include "topness_struct.h" 

// code written by Michael L. Graesser 06/2015 

using namespace std;

const int DMAX=4; // # parameters to scan over, not the space-time dimension! Number of points in the simplex is DMAX+1. 

double my_dot(double p1[], double p2[], int d);

double my_dot(double p1[], double p2[], double a[], int d); 

double my_enorm(double p[], int d);

struct my_func; 

class my_simplex{ 
// co-ordinate space is d dimensional. analysis uses N=d+1 points                                                                                                                                     
 private:
  double alpha, beta, gamma;
  int d;

 public:
  my_func *f;
  my_simplex(int, double, double, double, my_func *);
  double xstart[DMAX*(DMAX+1)]; // length (d+1)*d. d elements are coordinates                                                                                                                         
  double x[DMAX*(DMAX+1)]; //    current polygon instance containing d+1 points of d dimension each, so size is d(d+1)                                                                                
  double xh[DMAX]; // coordinates of point with highest value                                                                                                                                         
  double xl[DMAX]; // coordinates of point with lowest value                                                                                                                                          
  double y[DMAX+1]; // function values at x                                                                                                                                                           
  double yl, ynh, yh; // lowest, next highest, and highest function values                                                                                                                            
  double xCentroid[DMAX]; // current centroid;                                                                                                                                                        
  double yReflect, yExpand, yContract; // function values at these points                                                                                                                             
  double xReflect[DMAX]; // P* reflection                                                                                                                                                             
  double xExpand[DMAX]; // P** expansion                                                                                                                                                               
  double xContract[DMAX]; // P*** contraction                                                                                                                                                         
  void find_min();
  void find_max();
  int imin,imax, inmax;
  void my_SetUp(double xin[]);
  void set_y();
  void my_Centroid(int);
  void my_Reflection();
  void my_Expansion();
  void my_Contraction();
  void replace_all();
  double get_yavg();
  double get_sigma();
  void print_Centroid();
  void print_Reflect();
  void print_Expand();
  void print_Contract();
  void print_max();
  void print_min();
  void print_all();
  void print_xy();
  void print_xyl();
  void print_xyh();
};

class my_Nelder_Mead {
 private:
  int d;
  int Ntry;      // N cycles of Nelder-Mead algorithm                                                                                                                                                 
  double Deltastep; // step to take when restarting after finding a new minimum                                                                                                                       
  int n;
  double eps;

 public:
  my_func *f;
  my_simplex simplex;
  bool convergeYes; // set to True when algorithm has converged                                                                                                                                       
  my_Nelder_Mead(int, double, double, double, int, double, double, int, my_func *);
  bool one_cycle(my_simplex *);
  bool find_global_min(double xin[DMAX*(DMAX+1)]);
  double yfinal;
  double xfinal[DMAX*(DMAX+1)];
};



