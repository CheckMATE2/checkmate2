#include<iostream>
#include<string>
#include<stdlib.h>
#include <cmath> 

#include "topness/topness_struct.h"

/* Written by Michael L. Graesser, 06/2015   

   evaluates topness function described in 
 
   Hunting Mixed Top Squark Decays 
   Michael L. Graesser, Jessie Shelton 
   Phys.Rev.Lett. 111 (2013) 12, 121802 
   DOI: 10.1103/PhysRevLett.111.121802 
   arXiv:1212.4495  

   top and W masses used in the topness algorithm set in the header file 

   please cite this article if using this code 

*/

using namespace std; 



// BEGIN TOPNESS 
//
// struct my_func returns topness statistic 
// initialized by b-jet, l, second jet  and MET momenta, and a given 
// pairing of the lepton with one of the b-jet and second jet.  
// Unknowns are 3-vec of neutrino pv and pZ component of the W. 
// Uses the measured transverse MET to solve for pWx, pWy. 
// On-shell mass-conditions for v and W assumed. 

// The function "my_func" below returns S, where topness = ln min S, where 
// the minimization is performed over all possible pv and pW inputs, as well 
// as the possible pairings of the lepton with the jets.
//
// Evaluation of "my_func"  on pvx, pvy, pvz and pWz overloaded 


// Lorentz inner product 
double my_lp(double u[4], double v[4]) 
{    // Lorentz product . assume x, y Lorentz vectors ordered as (px,py,pz,E) etc. Initialize with 
  // metric g==(-1,-1,-1,1)  
  
  double g[]={-1.,-1.,-1.,1.};
  double s=0;
  for (int j=0;j<4;j++)   
    {s+= u[j]*g[j]*v[j]; }        
  return s;
}

void my_add(double a[4], double b[4], double c[4])
{
  for (int i=0; i<4; i++) 
    { 
      c[i]=a[i]+b[i]; 
    }
}


