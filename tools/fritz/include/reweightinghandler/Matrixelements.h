#ifndef _MATRIXELEMENTS
#define _MATRIXELEMENTS

#include <vector>
#include "HepMC/GenRanges.h"
#include "procs/MGProcs.h"

const double N = 3;
const double nf = 4;
const double CF = (N*N-1)/(2*N); // = 4/3
const double CO = N*(N*N-1); // = 24
const double CK = (N*N-1)/N; // = 8/3
const double pi = 3.141592653589793238462643383279502884197;

class Matrixelements{
public:
	std::string param_card;
	virtual void init(std::string pcard) = 0; // pure virtual function
	virtual double calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state) = 0; // pure virtual function
};


class Matrixelements_pp2z2mupmum : public Matrixelements{
public:
	PP2Z2MUPMUM_DDX_MUPMUM_CPPProcess ddx_mupmum_cppprocess;
	PP2Z2MUPMUM_UUX_MUPMUM_CPPProcess uux_mupmum_cppprocess;
	void init(std::string pcard){
		param_card = pcard;
		ddx_mupmum_cppprocess.initProc(param_card);
		uux_mupmum_cppprocess.initProc(param_card);
	}
	double calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state);
};


class Matrixelements_pp2zp2mupmum : public Matrixelements{
public:
	DDX_MUPMUM_CPPProcess ddx_mupmum_cppprocess;
	UUX_MUPMUM_CPPProcess uux_mupmum_cppprocess;
	SSX_MUPMUM_CPPProcess ssx_mupmum_cppprocess;
	CCX_MUPMUM_CPPProcess ccx_mupmum_cppprocess;
	UCX_MUPMUM_CPPProcess ucx_mupmum_cppprocess;
	CUX_MUPMUM_CPPProcess cux_mupmum_cppprocess;
	DSX_MUPMUM_CPPProcess dsx_mupmum_cppprocess;
	SDX_MUPMUM_CPPProcess sdx_mupmum_cppprocess;
	void init(std::string pcard){
		param_card = pcard;
		ddx_mupmum_cppprocess.initProc(param_card);
		uux_mupmum_cppprocess.initProc(param_card);
		ssx_mupmum_cppprocess.initProc(param_card);
		ccx_mupmum_cppprocess.initProc(param_card);
		ucx_mupmum_cppprocess.initProc(param_card);
		cux_mupmum_cppprocess.initProc(param_card);
		dsx_mupmum_cppprocess.initProc(param_card);
		sdx_mupmum_cppprocess.initProc(param_card);
	}
	double calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state);
};


class Matrixelements_pp2ttbar : public Matrixelements{
public:
	GG_TTX_CPPProcess gg_ttx_cppprocess;
	UUX_TTX_CPPProcess uux_ttx_cppprocess;
	void init(std::string pcard){
		param_card = pcard;
		gg_ttx_cppprocess.initProc(param_card);
		uux_ttx_cppprocess.initProc(param_card);
	}
	double calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state);
};


class Matrixelements_pp2ttbar2wbwb : public Matrixelements{
public:
	PP2TTBAR2WBWB_UUX_VEEPBEMVEXBX_CPPProcess uux_veepbemvexbx_cppprocess;
	PP2TTBAR2WBWB_GG_UDXBDUXBX_CPPProcess gg_udxbduxbx_cppprocess;
	PP2TTBAR2WBWB_UUX_VEEPBDUXBX_CPPProcess uux_veepbduxbx_cppprocess;
	PP2TTBAR2WBWB_UUX_UDXBDUXBX_CPPProcess uux_udxbduxbx_cppprocess;
	PP2TTBAR2WBWB_GG_VEEPBEMVEXBX_CPPProcess gg_veepbemvexbx_cppprocess;
	PP2TTBAR2WBWB_UUX_UDXBEMVEXBX_CPPProcess uux_udxbemvexbx_cppprocess;
	PP2TTBAR2WBWB_GG_UDXBEMVEXBX_CPPProcess gg_udxbemvexbx_cppprocess;
	PP2TTBAR2WBWB_GG_VEEPBDUXBX_CPPProcess gg_veepbduxbx_cppprocess;
	void init(std::string pcard){
		param_card = pcard;
		uux_veepbemvexbx_cppprocess.initProc(param_card);
		gg_udxbduxbx_cppprocess.initProc(param_card);
		uux_veepbduxbx_cppprocess.initProc(param_card);
		uux_udxbduxbx_cppprocess.initProc(param_card);
		gg_veepbemvexbx_cppprocess.initProc(param_card);
		uux_udxbemvexbx_cppprocess.initProc(param_card);
		gg_udxbemvexbx_cppprocess.initProc(param_card);
		gg_veepbduxbx_cppprocess.initProc(param_card);
	}
	double calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state);
};



class Matrixelements_pp2sqgo : public Matrixelements{
public:
	GC_CLGO_CPPProcess gc_clgo_cppprocess;
	GC_CRGO_CPPProcess gc_crgo_cppprocess;
	GCX_CLXGO_CPPProcess gcx_clxgo_cppprocess;
	GCX_CRXGO_CPPProcess gcx_crxgo_cppprocess;
	GD_DLGO_CPPProcess gd_dlgo_cppprocess;
	GD_DRGO_CPPProcess gd_drgo_cppprocess;
	GDX_DLXGO_CPPProcess gdx_dlxgo_cppprocess;
	GDX_DRXGO_CPPProcess gdx_drxgo_cppprocess;
	GS_SLGO_CPPProcess gs_slgo_cppprocess;
	GS_SRGO_CPPProcess gs_srgo_cppprocess;
	GSX_SLXGO_CPPProcess gsx_slxgo_cppprocess;
	GSX_SRXGO_CPPProcess gsx_srxgo_cppprocess;
	GU_ULGO_CPPProcess gu_ulgo_cppprocess;
	GU_URGO_CPPProcess gu_urgo_cppprocess;
	GUX_ULXGO_CPPProcess gux_ulxgo_cppprocess;
	GUX_URXGO_CPPProcess gux_urxgo_cppprocess;
	void init(std::string pcard){
		param_card = pcard;
		gc_clgo_cppprocess.initProc(param_card);
		gc_crgo_cppprocess.initProc(param_card);
		gcx_clxgo_cppprocess.initProc(param_card);
		gcx_crxgo_cppprocess.initProc(param_card);
		gd_dlgo_cppprocess.initProc(param_card);
		gd_drgo_cppprocess.initProc(param_card);
		gdx_dlxgo_cppprocess.initProc(param_card);
		gdx_drxgo_cppprocess.initProc(param_card);
		gs_slgo_cppprocess.initProc(param_card);
		gs_srgo_cppprocess.initProc(param_card);
		gsx_slxgo_cppprocess.initProc(param_card);
		gsx_srxgo_cppprocess.initProc(param_card);
		gu_ulgo_cppprocess.initProc(param_card);
		gu_urgo_cppprocess.initProc(param_card);
		gux_ulxgo_cppprocess.initProc(param_card);
		gux_urxgo_cppprocess.initProc(param_card);
	}
	double calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state);
};



class Matrixelements_pp2sqsqbar : public Matrixelements{
public:
	PP2SQSQBAR_DDX_DLDLX_CPPProcess ddx_dldlx_cppprocess;	
	PP2SQSQBAR_DDX_DRDRX_CPPProcess ddx_drdrx_cppprocess;	
	PP2SQSQBAR_DDX_DLDRX_CPPProcess ddx_dldrx_cppprocess;	
	PP2SQSQBAR_DDX_DRDLX_CPPProcess ddx_drdlx_cppprocess;	
	PP2SQSQBAR_GG_DLDLX_CPPProcess gg_dldlx_cppprocess;	
	PP2SQSQBAR_GG_DRDRX_CPPProcess gg_drdrx_cppprocess;	
	PP2SQSQBAR_UUX_DLDLX_CPPProcess uux_dldlx_cppprocess;	
	PP2SQSQBAR_UUX_DRDRX_CPPProcess uux_drdrx_cppprocess;	
	void init(std::string pcard){
		param_card = pcard;
		ddx_dldlx_cppprocess.initProc(param_card);
		ddx_drdrx_cppprocess.initProc(param_card);
		ddx_dldrx_cppprocess.initProc(param_card);
		ddx_drdlx_cppprocess.initProc(param_card);
		gg_dldlx_cppprocess.initProc(param_card);
		gg_drdrx_cppprocess.initProc(param_card);
		uux_dldlx_cppprocess.initProc(param_card);
		uux_drdrx_cppprocess.initProc(param_card);
	}
	
	double calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state);
};


class Matrixelements_pp2sqsqbar_nogluino : public Matrixelements{
public:
	PP2SQSQBAR_NOGLUINO_GG_DRDRX_CPPProcess gg_drdrx_cppprocess;
	PP2SQSQBAR_NOGLUINO_UUX_SLSLX_CPPProcess uux_slslx_cppprocess;
	PP2SQSQBAR_NOGLUINO_GG_SLSLX_CPPProcess gg_slslx_cppprocess;
	PP2SQSQBAR_NOGLUINO_GG_URURX_CPPProcess gg_ururx_cppprocess;
	PP2SQSQBAR_NOGLUINO_UUX_DRDRX_CPPProcess uux_drdrx_cppprocess;
	PP2SQSQBAR_NOGLUINO_UUX_URURX_CPPProcess uux_ururx_cppprocess;
	PP2SQSQBAR_NOGLUINO_GG_CLCLX_CPPProcess gg_clclx_cppprocess;
	PP2SQSQBAR_NOGLUINO_GG_ULULX_CPPProcess gg_ululx_cppprocess;
	PP2SQSQBAR_NOGLUINO_UUX_CLCLX_CPPProcess uux_clclx_cppprocess;
	PP2SQSQBAR_NOGLUINO_UUX_SRSRX_CPPProcess uux_srsrx_cppprocess;
	PP2SQSQBAR_NOGLUINO_GG_CRCRX_CPPProcess gg_crcrx_cppprocess;
	PP2SQSQBAR_NOGLUINO_UUX_CRCRX_CPPProcess uux_crcrx_cppprocess;
	PP2SQSQBAR_NOGLUINO_GG_DLDLX_CPPProcess gg_dldlx_cppprocess;
	PP2SQSQBAR_NOGLUINO_UUX_DLDLX_CPPProcess uux_dldlx_cppprocess;
	PP2SQSQBAR_NOGLUINO_GG_SRSRX_CPPProcess gg_srsrx_cppprocess;
	PP2SQSQBAR_NOGLUINO_UUX_ULULX_CPPProcess uux_ululx_cppprocess;
	void init(std::string pcard){
		param_card = pcard;
		gg_drdrx_cppprocess.initProc(param_card);
		uux_slslx_cppprocess.initProc(param_card);
		gg_slslx_cppprocess.initProc(param_card);
		gg_ururx_cppprocess.initProc(param_card);
		uux_drdrx_cppprocess.initProc(param_card);
		uux_ururx_cppprocess.initProc(param_card);
		gg_clclx_cppprocess.initProc(param_card);
		gg_ululx_cppprocess.initProc(param_card);
		uux_clclx_cppprocess.initProc(param_card);
		uux_srsrx_cppprocess.initProc(param_card);
		gg_crcrx_cppprocess.initProc(param_card);
		uux_crcrx_cppprocess.initProc(param_card);
		gg_dldlx_cppprocess.initProc(param_card);
		uux_dldlx_cppprocess.initProc(param_card);
		gg_srsrx_cppprocess.initProc(param_card);
		uux_ululx_cppprocess.initProc(param_card);
	}
	double calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state);
};





// double pp2z2mupmum(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, Matrixelements_pp2z2mupmum * mes);
// double pp2z2mupmum_pdg(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, Matrixelements_pp2z2mupmum * mes);
// double pp2zp2mupmum(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, Matrixelements_pp2zp2mupmum * mes);

// double pp2ttbar(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, const std::string & param_card);
// double pp2ttbar(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, Matrixelements_pp2ttbar * mes);
// double pp2ttbar_pdg(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, Matrixelements_pp2ttbar * mes);
// double pp2ttbar2wbwb(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, Matrixelements_pp2ttbar2wbwb * mes);
// double pp2ttbar2wbwb_analytical(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, Matrixelements_pp2ttbar2wbwb * mes);

// double pp2sqgo(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, Matrixelements_pp2sqgo * mes);
// double pp2sqgo_pdg(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, Matrixelements_pp2sqgo * mes);
// double pp2sqgo_analytical(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state,  Matrixelements_pp2sqgo * mes);

// double pp2sqsqbar(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, Matrixelements_pp2sqsqbar * mes);
// double pp2sqsqbar_nogluino(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, Matrixelements_pp2sqsqbar_nogluino * mes);
// double pp2sqsqbar_nogluino_pdg(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state, Matrixelements_pp2sqsqbar_nogluino * mes);


#endif