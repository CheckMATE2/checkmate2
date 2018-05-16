#include<iostream>
#include<string>
#include<stdlib.h> 

#include "topness/simplex.h"

/* Wrapper for topness minimization using the Nelder-Mead algorithm 

   Parameters for the Nelder-Mead algorithm set here 

   DMAX set to 4 in simplex.h 

   top and W masses used in the topness algorithm set in topness_struc.h 

   Written by Michael L. Graesser, 06/2015    

*/

using namespace std; 

double topnesscompute(double pb1[4], double pb2[4], double pl[4], double MET[4], double sigmat, double sigmaW, double sigmas, double xbest[4]) 
{

  double alpha=1.0; // parameter for reflection 
  double beta=0.5;  // parameter for contraction 
  double gamma=2.0;  // parameter for expansion 

  const int d=4;    // number of parameters to scan over, not the space-time dimension! 
  const int DIMMAX=d*(d+1);  // dimension of simplex 
  double xin[d+1][d];    // starting point 
  double xstart[DIMMAX]; // algorithm stores d+1 points of simplex in a single array


  double eps=0.000002;   // tolerance
  double Deltastep=20.;  // initial spacing of points, in GeV
  int Ntry=100000;    // maximum number of Nelder-Mead cycles to perform for a given initial seed   
  int Nattempts=15; //   number of initial starts 

  // get random seed (once) 
  // default c++ "random number" generator good enough here      
  srand((unsigned)time(0));
  double rndm, start ;
  int n=3000;
  // get going                                                                     
  for (int i=0; i<100; i++)
    {
      rndm=(rand()%n)+1;
    }
  double edir[]={1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
  double ybest=1000000000.; // a big number 
  double ybest1=100000000.;                                           
  double xbest1[]={1000.,1000.,1000.,1000.};
  double ybest2=100000000.;  // a big number                                          
  double xbest2[]={1000.,1000.,1000.,1000.};
  int i,j,k;
  // int tid;                                                                                       
  bool converge1, converge2;         
  
  // initialize topness function  
  my_func topstat1(pb1,pb2,pl,MET,sigmat,sigmaW,sigmas,1.0);
  my_func topstat2(pb2,pb1,pl,MET,sigmat,sigmaW,sigmas,1.0); // other combination  

  // initialize topness computation 
  my_Nelder_Mead my_check1(d,alpha,beta,gamma, Ntry, eps, Deltastep, n, &topstat1);
  my_Nelder_Mead my_check2(d,alpha,beta,gamma, Ntry, eps, Deltastep, n, &topstat2);
  double yl;
  // begin loop over Nattempts
  for (k=0; k<Nattempts; k++)
    {
      // initialize starting point 
      for (i=0;i<d+1; i++)
        {
          for (j=0; j<d; j++)
            {
              if (i==0)
                {
                  rndm=(rand()%n)+1;
                  start= (rndm/n-0.5);
                  xin[0][j]=8000.0*start;
                }
              else
                {
                  xin[i][j]=xin[0][j]+Deltastep*edir[d*(i-1)+j];
                }
            }
          copy(xin[i],xin[i]+d, xstart+d*i);  // copy initial data into xstart          
        }
      // now first combination 
      converge1=my_check1.find_global_min(xstart);
      if (converge1==true)
        {

          yl=my_check1.yfinal;
          if (yl < ybest1)
            {
              ybest1=yl;
              copy(my_check1.xfinal,my_check1.xfinal+d,xbest1);
            }
        }
      else
        {
          cout << " Minimum not found...exiting " << endl;
        }
      // now do second combination                                                      
      converge2=my_check2.find_global_min(xstart);
      if (converge2==true)
        {
          yl=my_check2.yfinal;
          if (yl < ybest2)
            {
              ybest2=yl;
              copy(my_check2.xfinal,my_check2.xfinal+d,xbest2);
            }
        }
      else
	{
          cout << " Minimum not found...exiting " << endl;
        }

    }

  if (ybest1 < ybest2)
    {
    ybest=ybest1;
    copy(xbest1,xbest1+d,xbest);
    }
   else
   {
     ybest=ybest2;
     copy(xbest2,xbest2+d,xbest);
   }

  return ybest; 

}

