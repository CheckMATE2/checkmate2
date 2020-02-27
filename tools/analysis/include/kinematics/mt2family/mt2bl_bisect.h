#ifndef mt2bl_BISECT_H
#define mt2bl_BISECT_H


/*The user can change the desired precision below, the larger one of the following two definitions is used. Relative precision less than 0.00001 is not guaranteed to be achievable--use with caution*/ 

#define RELATIVE_PRECISION 0.00001 //defined as precision = RELATIVE_PRECISION * scale, where scale = max{Ea, Eb}
#define ABSOLUTE_PRECISION 0.0     //absolute precision for mt2bl, unused by default

namespace mt2bl_bisect
{
class mt2bl
{  
   public:

      mt2bl();
      void   mt2bl_bisect();
      void   mt2bl_massless();
    void   set_momenta(double *pl0, double *pb10, double *pb20, double* pmiss0);  //b1 pairs with l
    void   set_momenta(double El,  double plx,  double ply,  double plz,
                       double Eb1, double pb1x, double pb1y, double pb1z,
                       double Eb2, double pb2x, double pb2y, double pb2z,
                       double pmissx, double pmissy);  // Same as above without pointers/arrays
//    void   set_mn(double mn);
      double get_mt2bl();
//      void   print();
//      int    nevt;
   private:  

      bool   solved;
      bool   momenta_set;
      double scan_step;
      double mt2bl_b;

      int    nsols(double Dsq);
      int    nsols_massless(double Dsq);
      inline int    signchange_n( long double t1, long double t2, long double t3, long double t4, long double t5);
      inline int    signchange_p( long double t1, long double t2, long double t3, long double t4, long double t5);
      int scan_high(double &Deltasq_high);
      int find_high(double &Deltasq_high);
      //data members
      double pax, pay, ma, Ea;
      double pmissx, pmissy;
      double pbx, pby, mb, Eb;
      double mn1, mn2;
      
     
      //auxiliary definitions
      double masq, Easq;
      double mbsq, Ebsq;
      double pmissxsq, pmissysq;
      double mn1sq, mn2sq;

      //auxiliary coefficients
      double a1, b1, c1, a2, b2, c2, d1, e1, f1, d2, e2, f2;
      double d11, e11, f12, f10, d21, d20, e21, e20, f22, f21, f20;

//      double scale;
      double precision;
};

}//end namespace mt2bl_bisect

#endif
