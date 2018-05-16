#include<iostream>
#include<string>
#include <stdlib.h>
#include <cmath> 
#include "topness/simplex.h"

/* does minimization algorithm from Nelder and Mead 

   Nelder, John A.; R. Mead (1965). "A simplex method for function minimization". Computer Journal 7: 308–313. doi:10.1093/comjnl/7.4.308.

   algorithm implemented here from their Fig. 1 

   code written by Michael L. Graesser, 06/2015

*/

using namespace std; 

//const int DMAX=4; // maximum size of arrays = number of parameters to scan over 

double my_dot(double p1[], double p2[], int d)   // d is size of arrays
{
// compute euclidean dot product between p1 and p2 
  double value=0.;  
  for (int i=0; i<d; i++)
    {
      //      cout << "p1: " << p1[i] <<", p2:" << p2[i] << endl;  
      value+=p1[i]*p2[i]; 
    }
  //cout << "dp value is " << value << endl;
  return value; 
}

double my_dot(double p1[], double p2[], double a[], int d)   // d is size of arrays                       
{
  // compute euclidean dot product between p1 and p2 with weights a                                
                  
  double value=0.;
  for (int i=0; i<d; i++)
    {
      value+=a[i]*p1[i]*p2[i];
    }
  return value;
}

double my_enorm(double p[], int d) 
{
  // compute euclidean norm of p 
  double value=0.;
  double norm=0.; 
  for (int i=0; i<d; i++)
    {
      value+=p[i]*p[i];
    }
  if (value>0.)
    {
      norm=sqrt(value); 
    }
  else 
    {
      norm=0.;
    }
  return norm;
}

my_simplex::my_simplex(int dd, double aalpha, double bbeta, double ggamma,  my_func (*ff)) : d(dd), alpha(aalpha), beta(bbeta), gamma(ggamma), f(ff){}
// check d and size of xin agree 

void my_simplex::my_SetUp(double xin[DMAX*(DMAX+1)]) 
{
  int D=d*(d+1);
  copy(xin,xin+D,xstart);
  for (int i=0; i<D; i++)
    {
      x[i]=xstart[i];
    }  

  for (int i=0; i<d+1; i++)
    {
      double xi[d];
      copy(x+d*i,x+d*i+d,xi);  // get coordinates of i'th point and copy into xi 
           y[i]=(*f)(xi,d);
    }
}

void my_simplex::set_y() 
{
  for (int i=0; i<d+1; i++)
    {
      double xi[d];
      copy(x+d*i,x+d*i+d,xi);  // get coordinates of i'th point and copy into xi                            
      y[i]=(*f)(xi,d); 
    }
}

void my_simplex::find_max()
{ 
  for (int i=0; i<d+1; i++) 
    { 
      if (y[i]> y[imax])   
	imax=i;
    }
  // now find second highest 
  if (imax==1) 
    inmax=0; 

  for (int i=0; i<d+1; i++) 
    {
      if (i==imax) continue;
      if (y[i] > y[inmax])
	{
	  inmax=i; 
	}
    }

  yh=y[imax];
  ynh=y[inmax];   
  // find coordinates with maximum value 
  for (int j=0; j<d; j++) 
    {
      xh[j]=x[d*imax+j]; 
    }
} 

void my_simplex::find_min()
{
  imin=0;   // first function value f(x_1)
  for (int i=0; i<d+1; i++)
    {
       if (y[i]< y[imin])
       {
        imin=i;
       }
    }
 
  yl=y[imin];  

  // find coordinates with minimum value                                                                                  
  for (int j=0; j<d; j++)
    {
      xl[j]=x[d*imin+j];
    }
 
}

void my_simplex::my_Centroid(int h) 
/* return P_bar */ 
{
  // compute centroid
  for (int j=0; j<d; j++)
    { 
      xCentroid[j]=0; 
      for (int i=0; i< d+1; i++)
	{
	  if (i==h) continue;
	  xCentroid[j]+=x[d*i+j];
	}
    }   
}

void my_simplex::my_Reflection() // h is highest point  
/* return P* 
   REFLECTION  P*=(1+alpha)P_bar - alpha*P_h   */ 
{
  for (int i=0; i<d;i++) 
  {
    xReflect[i] = xCentroid[i]*(1+alpha)/d- alpha*xh[i];  
  }
  yReflect=(*f)(xReflect,d); 
}

void my_simplex:: my_Expansion() 
/* return EXPANSION P** =(1+gamma)*P* -gamma* P_bar */ 
{
  for (int j=0; j<d; j++)
  {
    xExpand[j] =xCentroid[j]*(1-gamma)/d+ (gamma)*xReflect[j];  
  }
  yExpand=(*f)(xExpand,d);

}

void my_simplex:: my_Contraction()
/* return CONTRACTION P*** =beta* Ph +(1-beta)*P_bar */ 
{
  for (int j=0; j<d; j++)
  {
    xContract[j]=xCentroid[j]*(1-beta)/d+beta*xh[j];
  }
  yContract=(*f)(xContract,d);
}

void my_simplex::replace_all() 
{ 
  for (int i=0; i<d+1; i++)
    { 
      if (i==imin) continue;
      for (int j=0; j<d; j++) 
	{ 
	  x[d*i+j]=0.5*(x[d*i+j]+xl[j]); 
	}
    }
}

double my_simplex::get_yavg()
{
  double yavg=0.; 
  for (int i=0; i<d+1; i++) 
    { 
      yavg+=y[i]; 
    } 
  yavg=yavg/(d+1);
  return yavg; 
}

double my_simplex::get_sigma() 
{
  double yavg=0.;
  double sigma=0; 
  for (int i=0; i<d+1; i++)
    {
      yavg+=y[i];
    }
  yavg=yavg/(d+1); 
  for (int i=0; i<d+1; i++)
    { 
      sigma+=pow((y[i]-yavg),2); 
    }

  sigma=sigma/(d+1); 
  return sigma; 
}

void my_simplex::print_Centroid() 
{

  cout << "Current xCentroid is : " << endl;
  for (int k=0;k<d; k++)
    {
      cout << xCentroid[k];
      if (k==d-1)
	cout << endl;
      else cout<< ", ";

    }
}

void my_simplex::print_Reflect() 
{
  cout << "Current xReflect and y value are : " << endl;
  for (int k=0;k<d; k++)
    {
      cout << xReflect[k];
      if (k==d-1)
	cout <<", " << yReflect << endl;
      else cout<< ", ";

    }
}

void my_simplex::print_Expand() 
{

  cout << "Current xExpand and y value are : " << endl;
  for (int k=0;k<d; k++)
    {
      cout << xExpand[k];
      if (k==d-1)
	cout << ", " << yExpand << endl;
      else cout<< ", ";

    }
} 

void my_simplex::print_Contract() 
{
  cout << "Current xContract and y value are : " << endl; 
  for (int k=0; k<d; k++) 
    {
      cout << xContract[k]; 
      if (k==d-1) 
	cout << ", " << yContract << endl; 
      else cout << ", "; 

    }

}

void my_simplex::print_max()
{
  cout << "Printing imax and inmax and their values " << endl; 
  cout << "imax = " << imax << ", y[imax] = " << yh << endl; 
  cout << "inmax = " << inmax << ", y[inmax] = " << ynh << endl;
}

void my_simplex::print_min() 
{
  cout << "Printing imin and its value " << endl;
  cout << "imin = " << imin << ", y[imin] = " << yl << endl;
}

void my_simplex::print_xyh() 
{
  cout << "The highest value is " << endl;
  for (int i=0; i<d; i++)
    {
      cout << xh[i];
      if ((i+1)% d !=0)
        {
          cout << ", " ;
        }
      else
        {
          cout << ", " << yh << endl;
        }
    }
}

void my_simplex::print_xyl()
{
  cout << "The lowest value is " << endl;
  for (int i=0; i<d; i++)
    {
      cout << xl[i];
      if ((i+1)% d !=0)
        {
          cout << ", " ;
        }
      else
        {
          cout << ", " << yl << endl;
        }
    }

}	     

void my_simplex::print_xy()
{
  // print current x and y                                                                                                                           
  cout << "Current x and y values are: " << endl;
  for (int i=0; i< d*(d+1); i++)
    {
      cout << x[i];
      if ((i+1) % d !=0)
        {
          cout << ", ";
        }
      else
        {
          div_t ratio;
          ratio=div(i,d);
	  cout << ", " << y[ratio.quot] << endl;
        }
    }

}
void my_simplex::print_all() 
{ 
// print current x 
  cout << "Current x values are: " << endl; 
  for (int i=0; i< d*(d+1); i++) 
    { 
      cout << x[i]; 
      if ((i+1) % d !=0)
	{ 
	  cout << ", "; 
	} 
      else 
	{ 
	  div_t ratio; 
	  ratio=div(i,d);
	  cout << ", " << y[ratio.quot]<<endl; 
	}
    }
  cout << "Current centroid: " << endl; 
  for (int i=0; i< d; i++)
      {
	cout << xCentroid[i];
          if ((i+1) % d !=0)
          {
	      cout << ", ";
           }
          else
          {
	    cout << endl;
          }
     }
  cout << "Current Reflection: " << endl;
  for (int i=0; i< d; i++)
     {
         cout << xReflect[i];
	 if ((i+1) % d !=0)
	 {
	     cout << ", ";
	 }
	 else
	 {
	   cout << ", " << yReflect << endl;
	 }
    }
  cout << "Current Expansion: " << endl;
  for (int i=0; i< d; i++)
      {
	cout << xExpand[i];
	  if ((i+1) % d !=0)
	  {
	      cout << ", ";
	  }
	  else
	  {
	    cout << ", " << yExpand << endl;
	  }
      }
  cout << "Current Contraction: " << endl;
  for (int i=0; i< d; i++)
      {
	cout << xContract[i];
	  if ((i+1) % d !=0)
	  {
	      cout << ", ";
	  }
	  else
	  {
	    cout << ", " << yContract << endl;
	  }
     }
  cout << "The highest value is " << endl; 
  for (int i=0; i<d; i++) 
    { 
      cout << xh[i]; 
      if ((i+1)% d !=0) 
	{ 
	  cout << ", " ; 
	} 
      else 
	{ 
	  cout << ", " << yh << endl; 
	}
    }
  cout << "The lowest value is " << endl;
  for (int i=0; i<d; i++)
    {
      cout << xl[i];
      if ((i+1)% d !=0)
        {
          cout << ", " ;
        }
      else
        {
          cout << ", " << yl << endl;
        }
    }
    cout << "The lowest point is imin=" << imin << endl;

    cout << "The highest point is imax=" << imax << endl;

    cout << "The next highest point is inmax=" << inmax << endl;
}
 

my_Nelder_Mead::my_Nelder_Mead(int dd, double alpha, double beta, double gamma, int NNtry, double eeps, double DDeltastep, int nn, my_func *ff): d(dd), Ntry(NNtry), eps(eeps), Deltastep(DDeltastep), n(nn), f(ff), simplex(dd, alpha, beta, gamma, f){} 

bool my_Nelder_Mead::one_cycle(my_simplex *s)   
{
// execute one-iteration of Nelder-Mead method   
  (*s).my_Centroid((*s).imax); 
  //  (*s).print_Centroid(); 
  (*s).my_Reflection();
  // (*s).print_Reflect(); 
  if ((*s).yReflect <= (*s).yl) 
    {
      // do expansion
      (*s).my_Expansion();
      //      (*s).print_Expand(); 
      if ((*s).yExpand < (*s).yl) 
	{
	  // replace P_h with P_** 
	  copy((*s).xExpand,(*s).xExpand+d, (*s).x+d*((*s).imax));
	  (*s).set_y();
	  return false; 
	}
      else 
	{
	  // replace P_h with P_*                                                                               
	  copy((*s).xReflect,(*s).xReflect+d, (*s).x+d*((*s).imax));
	  (*s).set_y();
	  return true; 
	}
    }
  else if (((*s).yReflect) >= (*s).ynh ) // do contraction    
    {     
      if (((*s).yReflect) < (*s).yh) 
	{
	  copy((*s).xReflect,(*s).xReflect+d, (*s).x+d*((*s).imax));
	  (*s).set_y();
	  (*s).find_max();
	}
      (*s).my_Contraction();
      //    (*s).print_Contract(); 
      if ((*s).yContract < (*s).yh) 
	{ 
	  copy((*s).xContract,(*s).xContract+d, (*s).x+d*((*s).imax));
	  (*s).set_y();
	  return false;
      
	} 
      else 
	{ 
	  (*s).replace_all();
	  (*s).set_y();
	  return false; 
	}
     }
  else {
    // replace P_h with P_*                                                                                            
    copy((*s).xReflect,(*s).xReflect+d, (*s).x+d*((*s).imax));
    (*s).set_y();
    return true;
  }

  
}

bool my_Nelder_Mead::find_global_min(double xin[DMAX*(DMAX+1)]) 
/* try Nelder-Mead cycle Ntry times; if values converge then restart using point near new 
minimum  */ 
{    
  // initialize
  yfinal=10000000000000.; // a very large number
  simplex.imax=0; 
  simplex.inmax=1; 
  simplex.imin=2; 
  simplex.my_SetUp(xin);
  simplex.find_max();
  simplex.find_min();
  //  simplex.print_xy(); 
  //simplex.print_xyh(); 
  //simplex.print_xyl(); 
  //simplex.print_max(); 
  //simplex.print_min(); 
  convergeYes=false;
  bool reflectYes=false;
  //  double xnew[DMAX*(DMAX+1)];
  for (int i=0; i<Ntry; i++) 
    {
      reflectYes=one_cycle(&simplex);  
      //if (reflectYes==true) --i;
      simplex.find_max();
      simplex.find_min();
      double ynewmin=simplex.yl;
      double ynewmax=simplex.yh;
      //     cout << "i=" << i << endl; 
      //cout << endl << endl << endl;
      //simplex.print_xy();
      //simplex.print_xyh();
      //simplex.print_xyl();
      //simplex.print_max();
      //simplex.print_min();
      //cout << endl << endl << endl;
      // double yavg=simplex.get_yavg(); 
      //  double sigma=simplex.get_sigma(); 
      if (abs(ynewmax -ynewmin)/(abs(ynewmax)+abs(ynewmin)+eps) < eps)
      {
	    convergeYes=true;

	    // save old xi, generate new xstarti = xbest/rndm + rndm*step                             
	    if (ynewmin< yfinal)
	      {
		copy(simplex.x,simplex.x+d*(d+1),xfinal);
		yfinal=ynewmin;
		//		cout << "final i=" << i << ", ymin = " << ynewmin << ", ymax = " << ynewmax << endl; 
		break;
	      }
	    // create new starting point using best minimum
	    /*
	    for (int j=0; j<d*(d+1); j++) 
	      { 
		 double rndm=(rand()%n)+1;
		 double start= (rndm/n-0.5); // between -1/2 and 1/2
		 double rndm2=(rand()%n)+1; 
		 double rndmscale=(rndm2/n); // between 0 and 1
		 double scale; 
		 if (rndmscale!=0) 
		   {
		     scale=rndmscale;
		   }
		 else
		   {
		     scale=0.1;
		   }
		 xnew[j]=xfinal[j]/scale +start*Deltastep; 
     	      }
	    simplex.my_SetUp(xnew);
	    */
	}       	   
    }
  
  return convergeYes; 
}

