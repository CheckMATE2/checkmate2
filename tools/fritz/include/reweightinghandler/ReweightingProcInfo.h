#ifndef _REWEIGHTINGPROCINFO
#define _REWEIGHTINGPROCINFO

#include "TRef.h"

struct ReweightingProcInfo{
	Int_t id1;
	Int_t id2;
	Float_t x1;
	Float_t x2;
	Float_t scalePDF;
	Float_t pdf1;
	Float_t pdf2;
};

#endif