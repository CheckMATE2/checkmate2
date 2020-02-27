#ifndef _UNITS
#define _UNITS

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <string>

//! Numerical implementation of cross section and luminosity units
namespace units {
    const double KB = 1.E18, B = 1.E15, MB = 1.E12, MUB = 1.E9, NB = 1.E6, PB = 1.E3, FB = 1.0, AB = 1.E-3, ZB = 1.E-6; 
    const double INVKB = 1./KB, INVB = 1./B, INVMB = 1./MB, INVMUB = 1./MUB, INVNB = 1./NB, INVPB = 1./PB, INVFB = 1./FB, INVAB = 1./AB, INVZB = 1./ZB;
    
    double strToUnit(std::string unit); //!< Returns the factor that corresponds to a given unit.
}

#endif
