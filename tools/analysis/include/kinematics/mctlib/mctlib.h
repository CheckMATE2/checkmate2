#ifndef MCTLIB_H
#define MCTLIB_H

#include <iostream>
#include <math.h>

namespace mctlib
{
class mct 
{

 public:

  mct();
  double mctcorr(const double v1[4],const double v2[4]
             ,const double vds[4],const double ptm[2]
             ,const double ecm=14000.0,const double mxlo=0.0);
  double mctnorm(const double v1[4],const double v2[4]);
  double mt2(const double v1[4],const double v2[4]
             ,const double vds[4],const double ptm[2]
             ,const double ecm=14000.0,const double mxlo=0.0);
  double mctminmt2(const double mctsqr,const double m1sqr,
               const double m2sqr,const double chisqr=0.0);
  double mt2neg(const double v1[4],const double v2[4]
            ,const double ptm[2],const double mxlo=0.0);
  double mcy(const double v1[4],const double v2[4]
             ,const double vds[4],const double ptm[2]);
  double mcx(const double v1[4],const double v2[4]
             ,const double vds[4],const double ptm[2]);

 private:

  double m_mctecm, m_mctehat, m_pb;

};

}//end namespace mctlib

#endif


  
