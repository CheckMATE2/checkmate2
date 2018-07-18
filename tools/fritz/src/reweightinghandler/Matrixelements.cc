#include "HepMC/GenRanges.h"

#include "Matrixelements.h"
#include "reweightingaux.h"
#include "read_slha.h"


// double pp2sqgo_analytical(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state){

// 	SLHAReader slha(param_card);

// 	double alpha_s = slha.get_block_entry("sminputs", 3);
// 	double g = sqrt(4*pi*alpha_s);
// 	double m_go = slha.get_block_entry("mass",1000021);
// 	double m_sq;
// 	for(HepMC::GenParticle* gp : final_state){
// 		if(gp->pdg_id() != 1000021) m_sq = slha.get_block_entry("mass",fabs(gp->pdg_id()));
// 	}

// 	TLorentzVector k1 = fv_to_lv(initial_state[0]->momentum());
// 	TLorentzVector k2 = fv_to_lv(initial_state[1]->momentum());
// 	TLorentzVector p1 = fv_to_lv(final_state[0]->momentum());
// 	TLorentzVector p2 = fv_to_lv(final_state[1]->momentum());
// 	double s = (p1+p2).M2();
// 	double u1 = (k1-p2).M2() - m_sq*m_sq;
// 	double tg = (k2-p2).M2() - m_go*m_go;

// 	double a = CO * (1-2*s*u1/pow(tg,2)) - CK;
// 	double b = -tg/s + 2 * (m_go*m_go - m_sq*m_sq) * tg / (s*u1) * (1 + m_sq*m_sq/u1 + m_go*m_go/tg);

// 	return 2*pow(g,4) * a * b;

// }




// double pp2sqgo_pdg(
// 	const std::vector<HepMC::GenParticle*> & initial_state, 
// 	const std::vector<HepMC::GenParticle*> & final_state, 
// 	Matrixelements_pp2sqgo * mes
// ){
	
// 	SLHAReader slha(param_card);
	
// 	TLorentzVector pa, pb;
// 	for(HepMC::GenParticle* gp : initial_state){
// 		if(gp->pdg_id() == 21){
// 			pa = fv_to_lv(gp->momentum());		
// 		}else{
// 			pb = fv_to_lv(gp->momentum());
// 		}
// 	}
	
// 	TLorentzVector pc, pd;
// 	double m_go, m_sq;
// 	for(HepMC::GenParticle* gp : final_state){
// 		if(gp->pdg_id() == 1000021){
// 			m_go = slha.get_block_entry("mass",1000021);
// 			pc = fv_to_lv(gp->momentum());
// 		}else{
// 			m_sq = slha.get_block_entry("mass",fabs(gp->pdg_id()));
// 			pd = fv_to_lv(gp->momentum());
// 		}		  
// 	}

	
// 	double s = (pa+pb).M2();
// 	double t = (pc-pa).M2();
// 	double u = (pd-pa).M2();

// 	double tg = m_go*m_go-t;
// 	double u1 = m_sq*m_sq-u;
// 	double dm = m_sq*m_sq-m_go*m_go;

// 	double a = 1./(s*tg* u1*u1);
// 	double b = 16./3.*(s*s + u1*u1) + 4./3.*s*u1;
// 	double c = pow(m_go*m_go-u,2) + pow(dm,2) + 2*s*m_go*m_go*dm/tg;

// 	return a*b*c;

// }



double Matrixelements_pp2sqgo::calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state){

	double p1[4], p2[4], p3[4], p4[4];

	p1[0] = initial_state[0]->momentum().e();
	p1[1] = 0; // initial_state[0]->momentum().px();
	p1[2] = 0; // initial_state[0]->momentum().py();
	p1[3] = initial_state[0]->momentum().pz();

	p2[0] = initial_state[1]->momentum().e();
	p2[1] = 0; // initial_state[1]->momentum().px();
	p2[2] = 0; // initial_state[1]->momentum().py()
	p2[3] = initial_state[1]->momentum().pz();

	p3[0] = final_state[1]->momentum().e();
	p3[1] = final_state[1]->momentum().px();
	p3[2] = final_state[1]->momentum().py();
	p3[3] = final_state[1]->momentum().pz();

	p4[0] = final_state[0]->momentum().e();
	p4[1] = final_state[0]->momentum().px();
	p4[2] = final_state[0]->momentum().py();
	p4[3] = final_state[0]->momentum().pz();

	std::vector<double*> momenta = {p1,p2,p3,p4};

	int pid1 = initial_state[0]->pdg_id();
	int pid2 = initial_state[1]->pdg_id();
	int pid3 = final_state[1]->pdg_id();
	int pid4 = final_state[0]->pdg_id();


	double me = 0;

	if((pid1 == 21 && pid2 == 4 || pid1 == 4 && pid2 == 21) && fabs(pid3) == 1000004 && fabs(pid4) == 1000021){
		GC_CLGO_CPPProcess & process = gc_clgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
		
	}else if((pid1 == 21 && pid2 == 4 || pid1 == 4 && pid2 == 21) && fabs(pid3) == 2000004 && fabs(pid4) == 1000021){
		GC_CRGO_CPPProcess & process = gc_crgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
		
	}else if((pid1 == 21 && pid2 == -4 || pid1 == -4 && pid2 == 21) && (pid3 == -1000004 && pid4 == 1000021 || pid3 == 1000021 && pid4 == -1000004)){
		GCX_CLXGO_CPPProcess & process = gcx_clxgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
		
	}else if((pid1 == 21 && pid2 == -4 || pid1 == -4 && pid2 == 21) && (pid3 == -2000004 && pid4 == 1000021 || pid3 == 1000021 && pid4 == -2000004)){
		GCX_CRXGO_CPPProcess & process = gcx_crxgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
	
	}else if((pid1 == 21 && pid2 == 1 || pid1 == 1 && pid2 == 21) && fabs(pid3) == 1000001 && fabs(pid4) == 1000021){
		GD_DLGO_CPPProcess & process = gd_dlgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
		
	}else if((pid1 == 21 && pid2 == 1 || pid1 == 1 && pid2 == 21) && fabs(pid3) == 2000001 && fabs(pid4) == 1000021){
		GD_DRGO_CPPProcess & process = gd_drgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
		
	}else if((pid1 == 21 && pid2 == -1 || pid1 == -1 && pid2 == 21) && (pid3 == -1000001 && pid4 == 1000021 || pid3 == 1000021 && pid4 == -1000001)){
		GDX_DLXGO_CPPProcess & process = gdx_dlxgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
		
	}else if((pid1 == 21 && pid2 == -1 || pid1 == -1 && pid2 == 21) && (pid3 == -2000001 && pid4 == 1000021 || pid3 == 1000021 && pid4 == -2000001)){
		GDX_DRXGO_CPPProcess & process = gdx_drxgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
	
	}else if((pid1 == 21 && pid2 == 3 || pid1 == 3 && pid2 == 21) && fabs(pid3) == 1000003 && fabs(pid4) == 1000021){
		GS_SLGO_CPPProcess & process = gs_slgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
		
	}else if((pid1 == 21 && pid2 == 3 || pid1 == 3 && pid2 == 21) && fabs(pid3) == 2000003 && fabs(pid4) == 1000021){
		GS_SRGO_CPPProcess & process = gs_srgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
		
	}else if((pid1 == 21 && pid2 == -3 || pid1 == -3 && pid2 == 21) && (pid3 == -1000003 && pid4 == 1000021 || pid3 == 1000021 && pid4 == -1000003)){
		GSX_SLXGO_CPPProcess & process = gsx_slxgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
		
	}else if((pid1 == 21 && pid2 == -3 || pid1 == -3 && pid2 == 21) && (pid3 == -2000003 && pid4 == 1000021 || pid3 == 1000021 && pid4 == -2000003)){
		GSX_SRXGO_CPPProcess & process = gsx_srxgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];

	}else if((pid1 == 21 && pid2 == 2 || pid1 == 2 && pid2 == 21) && fabs(pid3) == 1000002 && fabs(pid4) == 1000021){
		GU_ULGO_CPPProcess & process = gu_ulgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
		
	}else if((pid1 == 21 && pid2 == 2 || pid1 == 2 && pid2 == 21) && fabs(pid3) == 2000002 && fabs(pid4) == 1000021){
		GU_URGO_CPPProcess & process = gu_urgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
		
	}else if((pid1 == 21 && pid2 == -2 || pid1 == -2 && pid2 == 21) && (pid3 == -1000002 && pid4 == 1000021 || pid3 == 1000021 && pid4 == -1000002)){
		GUX_ULXGO_CPPProcess & process = gux_ulxgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];
		
	}else if((pid1 == 21 && pid2 == -2 || pid1 == -2 && pid2 == 21) && (pid3 == -2000002 && pid4 == 1000021 || pid3 == 1000021 && pid4 == -2000002)){
		GUX_URXGO_CPPProcess & process = gux_urxgo_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		// const double * mes = process.getMatrixElements();
		// me = mes[0];

	}else{

		cerr << "no fitting subprocess found.. " << endl;
		cerr << pid1 << ", " << pid2 << " -> " << pid3 << ", " << pid4 << endl;		
	}
	

	return me;
	
}



double Matrixelements_pp2sqsqbar::calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state){

	// std::vector<std::vector<double>> p = sort_momenta_for_pair_production(initial_state, final_state);
	// std::vector<double*> momenta = {&p[0][0], &p[1][0], &p[2][0], &p[3][0]};

	double p1[4], p2[4], p3[4], p4[4];

	p1[0] = initial_state[0]->momentum().e();
	p1[1] = 0; // initial_state[0]->momentum().px();
	p1[2] = 0; // initial_state[0]->momentum().py();
	p1[3] = initial_state[0]->momentum().pz();

	p2[0] = initial_state[1]->momentum().e();
	p2[1] = 0; // initial_state[1]->momentum().px();
	p2[2] = 0; // initial_state[1]->momentum().py();
	p2[3] = initial_state[1]->momentum().pz();

	p3[0] = final_state[0]->momentum().e();
	p3[1] = final_state[0]->momentum().px();
	p3[2] = final_state[0]->momentum().py();
	p3[3] = final_state[0]->momentum().pz();

	p4[0] = final_state[1]->momentum().e();
	p4[1] = final_state[1]->momentum().px();
	p4[2] = final_state[1]->momentum().py();
	p4[3] = final_state[1]->momentum().pz();

	std::vector<double*> momenta = {p1,p2,p3,p4};

	int pid1 = initial_state[0]->pdg_id();
	int pid2 = initial_state[1]->pdg_id();
	int pid3 = final_state[0]->pdg_id();
	int pid4 = final_state[1]->pdg_id();


	double me = 0;


	if(fabs(pid1) == 1 && fabs(pid2) == 1 && fabs(pid3) == 1000001 && fabs(pid4) == 1000001){
		PP2SQSQBAR_DDX_DLDLX_CPPProcess & process = ddx_dldlx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		
	}else if(fabs(pid1) == 1 && fabs(pid2) == 1 && fabs(pid3) == 2000001 && fabs(pid4) == 2000001){
		PP2SQSQBAR_DDX_DRDRX_CPPProcess & process = ddx_drdrx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		
	}else if(fabs(pid1) == 1 && fabs(pid2) == 1 && (pid3 == 1000001 && pid4 == -2000001 || pid3 == -2000001 && pid4 == 1000001)){
		PP2SQSQBAR_DDX_DLDRX_CPPProcess & process = ddx_dldrx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		
	}else if(fabs(pid1) == 1 && fabs(pid2) == 1 && (pid3 == -1000001 && pid4 == 2000001 || pid3 == 2000001 && pid4 == -1000001)){
		PP2SQSQBAR_DDX_DRDLX_CPPProcess & process = ddx_drdlx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		
	}else if(pid1 == 21 && pid2 == 21 && fabs(pid3) == 1000001 && fabs(pid4) == 1000001){
		PP2SQSQBAR_GG_DLDLX_CPPProcess & process = gg_dldlx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		
	}else if(pid1 == 21 && pid2 == 21 && fabs(pid3) == 2000001 && fabs(pid4) == 2000001){
		PP2SQSQBAR_GG_DRDRX_CPPProcess & process = gg_drdrx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		
	}else if(fabs(pid1) > 1 && fabs(pid1) <= 4 && fabs(pid2) > 1 && fabs(pid2) <= 4 && fabs(pid3) == 1000001 && fabs(pid4) == 1000001){
		PP2SQSQBAR_UUX_DLDLX_CPPProcess & process = uux_dldlx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		
	}else if(fabs(pid1) > 1 && fabs(pid1) <= 4 && fabs(pid2) > 1 && fabs(pid2) <= 4 && fabs(pid3) == 2000001 && fabs(pid4) == 2000001){
		PP2SQSQBAR_UUX_DRDRX_CPPProcess & process = uux_drdrx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		
	}else{

		cerr << "no fitting subprocess found.. " << endl;
		cerr << pid1 << ", " << pid2 << " -> " << pid3 << ", " << pid4 << endl;		
	}
	

	return me;
	
}



double Matrixelements_pp2sqsqbar_nogluino::calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state){

	double p1[4], p2[4], p3[4], p4[4];

	p1[0] = initial_state[0]->momentum().e();
	p1[1] = 0; // initial_state[0]->momentum().px();
	p1[2] = 0; // initial_state[0]->momentum().py();
	p1[3] = initial_state[0]->momentum().pz();

	p2[0] = initial_state[1]->momentum().e();
	p2[1] = 0; // initial_state[1]->momentum().px();
	p2[2] = 0; // initial_state[1]->momentum().py();
	p2[3] = initial_state[1]->momentum().pz();

	p3[0] = final_state[0]->momentum().e();
	p3[1] = final_state[0]->momentum().px();
	p3[2] = final_state[0]->momentum().py();
	p3[3] = final_state[0]->momentum().pz();

	p4[0] = final_state[1]->momentum().e();
	p4[1] = final_state[1]->momentum().px();
	p4[2] = final_state[1]->momentum().py();
	p4[3] = final_state[1]->momentum().pz();

	std::vector<double*> momenta = {p1,p2,p3,p4};

	int pid1 = initial_state[0]->pdg_id();
	int pid2 = initial_state[1]->pdg_id();
	int pid3 = final_state[0]->pdg_id();
	int pid4 = final_state[1]->pdg_id();


	double me = 0;

	if(pid1 == 21 && pid2 == 21 && fabs(pid3) == 1000001 && fabs(pid4) == 1000001){	
		PP2SQSQBAR_NOGLUINO_GG_DLDLX_CPPProcess & process = gg_dldlx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 == 21 && pid2 == 21 && fabs(pid3) == 1000002 && fabs(pid4) == 1000002){	
		PP2SQSQBAR_NOGLUINO_GG_ULULX_CPPProcess & process = gg_ululx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 == 21 && pid2 == 21 && fabs(pid3) == 1000003 && fabs(pid4) == 1000003){	
		PP2SQSQBAR_NOGLUINO_GG_SLSLX_CPPProcess & process = gg_slslx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 == 21 && pid2 == 21 && fabs(pid3) == 1000004 && fabs(pid4) == 1000004){	
		PP2SQSQBAR_NOGLUINO_GG_CLCLX_CPPProcess & process = gg_clclx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 == 21 && pid2 == 21 && fabs(pid3) == 2000001 && fabs(pid4) == 2000001){
		PP2SQSQBAR_NOGLUINO_GG_DRDRX_CPPProcess & process = gg_drdrx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 == 21 && pid2 == 21 && fabs(pid3) == 2000002 && fabs(pid4) == 2000002){	
		PP2SQSQBAR_NOGLUINO_GG_URURX_CPPProcess & process = gg_ururx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 == 21 && pid2 == 21 && fabs(pid3) == 2000003 && fabs(pid4) == 2000003){	
		PP2SQSQBAR_NOGLUINO_GG_SRSRX_CPPProcess & process = gg_srsrx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 == 21 && pid2 == 21 && fabs(pid3) == 2000004 && fabs(pid4) == 2000004){	
		PP2SQSQBAR_NOGLUINO_GG_CRCRX_CPPProcess & process = gg_crcrx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 != 21 && pid2 != 21 && fabs(pid3) == 1000001 && fabs(pid4) == 1000001){	
		PP2SQSQBAR_NOGLUINO_UUX_DLDLX_CPPProcess & process = uux_dldlx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 != 21 && pid2 != 21 && fabs(pid3) == 1000002 && fabs(pid4) == 1000002){	
		PP2SQSQBAR_NOGLUINO_UUX_ULULX_CPPProcess & process = uux_ululx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 != 21 && pid2 != 21 && fabs(pid3) == 1000003 && fabs(pid4) == 1000003){	
		PP2SQSQBAR_NOGLUINO_UUX_SLSLX_CPPProcess & process = uux_slslx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 != 21 && pid2 != 21 && fabs(pid3) == 1000004 && fabs(pid4) == 1000004){	
		PP2SQSQBAR_NOGLUINO_UUX_CLCLX_CPPProcess & process = uux_clclx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 != 21 && pid2 != 21 && fabs(pid3) == 2000001 && fabs(pid4) == 2000001){	
		PP2SQSQBAR_NOGLUINO_UUX_DRDRX_CPPProcess & process = uux_drdrx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 != 21 && pid2 != 21 && fabs(pid3) == 2000002 && fabs(pid4) == 2000002){	
		PP2SQSQBAR_NOGLUINO_UUX_URURX_CPPProcess & process = uux_ururx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 != 21 && pid2 != 21 && fabs(pid3) == 2000003 && fabs(pid4) == 2000003){	
		PP2SQSQBAR_NOGLUINO_UUX_SRSRX_CPPProcess & process = uux_srsrx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else if(pid1 != 21 && pid2 != 21 && fabs(pid3) == 2000004 && fabs(pid4) == 2000004){	
		PP2SQSQBAR_NOGLUINO_UUX_CRCRX_CPPProcess & process = uux_crcrx_cppprocess;
		process.setInitial(pid1,pid2);
		process.setMomenta(momenta);
		process.sigmaKin();
		return process.sigmaHat();
	}else{

		cerr << "no fitting subprocess found.. " << endl;
		cerr << pid1 << ", " << pid2 << " -> " << pid3 << ", " << pid4 << endl;		
	}
	
	return me;
}



// double Matrixelements_pp2sqsqbar_nogluino_pdg::calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state){

// 	SLHAReader slha(param_card);
	
// 	std::vector<HepMC::GenParticle*> is, fs;
// 	if(initial_state[0]->pdg_id() < 0){
// 		is = {initial_state[1], initial_state[0]};
// 	}else{
// 		is = {initial_state[0], initial_state[1]}; 
// 	}

// 	if(final_state[0]->pdg_id() < 0){
// 		fs = {final_state[1], final_state[0]};
// 	}else{
// 		fs = {final_state[0], final_state[1]};
// 	}

// 	TLorentzVector pa, pb;
// 	pa = fv_to_lv(is[0]->momentum());	
// 	pb = fv_to_lv(is[1]->momentum());	

// 	TLorentzVector pc, pd;
// 	pc = fv_to_lv(fs[0]->momentum());	
// 	pd = fv_to_lv(fs[1]->momentum());	

	
// 	double m_sq = slha.get_block_entry("mass",1000001);;
// 	double m_sq2 = m_sq*m_sq;

// 	double s = (pa+pb).M2();
// 	double t = (pc-pa).M2();
// 	double u = (pd-pb).M2();

// 	double t1 = m_sq*m_sq-t;
// 	double u1 = m_sq*m_sq-u;
// 	double tg = m_sq*m_sq+t;
// 	double ug = m_sq*m_sq+u;


// 	if(is[0]->pdg_id() == 21){
// 		double a = 1./s;
// 		double b = 1./3. * pow(tg/t1,2) + 1./3. * pow(ug/u1,2) + 3./(32*s*s) * (8*s*(4*m_sq2-s) + 4*pow(u-t,2)) + 7./12. - 1./48. * pow(4*m_sq2-s,2)/(t1*u1);
// 		double c = 3./32. * ((t-u)*(4*m_sq2+4*t-s)-2*u1*(6*m_sq2+2*t-s))/(s*t1) + 3./32. * ((u-t)*(4*m_sq2+4*u-s)-2*t1*(6*m_sq2+2*u-s))/(s*u1);
// 		double d = 7./96. * (4*m_sq2+4*t-s)/t1 + 7./96. * (4*m_sq2+4*u-s)/u1;
// 		return a*(b+c+d);
// 	}else{
// 		double a = 1./s;
// 		double b = 2./(s*s);
// 		double c = -s*t-t1*t1;
// 		return a*b*c;
// 	}


// }


// double Matrixelements_pp2ttbar_pdg::calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state){

// 	SLHAReader slha(param_card);
	
// 	std::vector<HepMC::GenParticle*> is, fs;
// 	// if(initial_state[0]->pdg_id() < 0){
// 	// 	is = {initial_state[1], initial_state[0]};
// 	// }else{
// 	// 	is = {initial_state[0], initial_state[1]}; 
// 	// }

// 	// if(final_state[0]->pdg_id() < 0){
// 	// 	fs = {final_state[1], final_state[0]};
// 	// }else{
// 	// 	fs = {final_state[0], final_state[1]};
// 	// }

// 	is = initial_state;
// 	fs = final_state;

// 	TLorentzVector pa, pb;
// 	pa = fv_to_lv(is[0]->momentum());	
// 	pb = fv_to_lv(is[1]->momentum());	

// 	TLorentzVector pc, pd;
// 	pc = fv_to_lv(fs[0]->momentum());	
// 	pd = fv_to_lv(fs[1]->momentum());	

	
// 	double mt = slha.get_block_entry("mass",6);
// 	double mt2 = mt*mt;

// 	double s = (pa+pb).M2();
// 	double t = (pc-pa).M2();
// 	double u = (pd-pb).M2();

// 	double t1 = mt2-t;
// 	double u1 = mt2-u;

// 	if(is[0]->pdg_id() == 21){

// 		double a = 1./s;
// 		double b = sqrt(1-4*mt2/s);
// 		double c = 6*t1*u1/pow(s,2) - mt2*(s-4*mt2)/(3*t1*u1);
// 		double d = 4./3. * (t1*u1 - 2*mt2*(mt2+t))/pow(t1,2) + 4./3. * (t1*u1 - 2*mt2*(mt2+u))/pow(u1,2);
// 		double e = - 3 * (t1*u1 + mt2*(u-t))/(s*t1) - 3 * (t1*u1 + mt2*(t-u))/(s*u1);
// 		double me = c+d+e;
// 		return (c+d+e);

// 	}else{
		
// 		double a = 1./s;
// 		double b = sqrt(1-4*mt2/s);
// 		double c = 1./pow(s,2);
// 		double d = (t1*t1 + u1*u1 + 2*mt2*s);
// 		double me = c*d;
// 		return me;

// 	}

// }


double Matrixelements_pp2ttbar::calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state){

	double p1[4], p2[4], p3[4], p4[4];

	p1[0] = initial_state[0]->momentum().e();
	p1[1] = 0; // initial_state[0]->momentum().px();
	p1[2] = 0; // initial_state[0]->momentum().py();
	p1[3] = initial_state[0]->momentum().pz();

	p2[0] = initial_state[1]->momentum().e();
	p2[1] = 0; // initial_state[1]->momentum().px();
	p2[2] = 0; // initial_state[1]->momentum().py();
	p2[3] = initial_state[1]->momentum().pz();

	p3[0] = final_state[0]->momentum().e();
	p3[1] = final_state[0]->momentum().px();
	p3[2] = final_state[0]->momentum().py();
	p3[3] = final_state[0]->momentum().pz();

	p4[0] = final_state[1]->momentum().e();
	p4[1] = final_state[1]->momentum().px();
	p4[2] = final_state[1]->momentum().py();
	p4[3] = final_state[1]->momentum().pz();

	std::vector<double*> momenta = {p1,p2,p3,p4};

	int pid1 = initial_state[0]->pdg_id();
	int pid2 = initial_state[1]->pdg_id();
	int pid3 = final_state[0]->pdg_id();
	int pid4 = final_state[1]->pdg_id();

	if(fabs(pid1) == 21 && fabs(pid2) == 21 && fabs(pid3) == 6 && fabs(pid4) == 6){
		GG_TTX_CPPProcess process = gg_ttx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();

		const double * mes = process.getMatrixElements();
		return mes[0];
		
	}else if(is_quark(pid1) && is_quark(pid2) && fabs(pid3) == 6 && fabs(pid4) == 6){
		UUX_TTX_CPPProcess process = uux_ttx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();

		const double * mes = process.getMatrixElements();
		return mes[0];
		
	}
	

	cerr << "no fitting subprocess found.. " << endl;
	cerr << pid1 << ", " << pid2 << " -> " << pid3 << ", " << pid4 << endl;
	return 0;
	
}

// double Matrixelements_pp2ttbar2wbwb_analytical::calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & top_final_state){

// 	// SLHAReader slha(param_card);
// 	// double mt = slha.get_block_entry("mass",6);

// 	HepMC::GenParticle *top, *antitop;
// 	if(top_final_state[0]->pdg_id()>0){
// 		top = last_status(top_final_state[0]);
// 		antitop = last_status(top_final_state[1]);
// 	}else{
// 		top = last_status(top_final_state[1]);
// 		antitop = last_status(top_final_state[0]);
// 	}

// 	HepMC::GenParticle *wp, *b;
// 	for(auto it : HepMC::GenVertexParticleRange(*top->end_vertex(), HepMC::children)){
// 		if(it->pdg_id() > 10) wp = last_status(it);
// 		if(it->pdg_id() < 10) b = last_status(it);
// 	}

// 	HepMC::GenParticle *wm, *bx;
// 	for(auto it : HepMC::GenVertexParticleRange(*antitop->end_vertex(), HepMC::children)){
// 		if(it->pdg_id() < -10) wm = last_status(it);
// 		if(it->pdg_id() > -10) bx = last_status(it);
// 	}

// 	HepMC::GenParticle *lp, *np;
// 	for(auto it : HepMC::GenVertexParticleRange(*wp->end_vertex(), HepMC::children)){
// 		if(it->pdg_id() % 2 != 0) lp = last_status(it);
// 		if(it->pdg_id() % 2 == 0) np = last_status(it);
// 	}

// 	HepMC::GenParticle *lm, *nm;
// 	for(auto it : HepMC::GenVertexParticleRange(*wm->end_vertex(), HepMC::children)){
// 		if(it->pdg_id() % 2 != 0) lm = last_status(it);
// 		if(it->pdg_id() % 2 == 0) nm = last_status(it);
// 	}

// 	std::vector<HepMC::GenParticle*> top_branch = {top, wp, b, lp, np};
// 	std::vector<HepMC::GenParticle*> antitop_branch = {antitop, wm, bx, lm, nm};
// 	std::vector<std::vector<HepMC::GenParticle*>> branches = {top_branch, antitop_branch};

// 	double ret = 1;

// 	for(std::vector<HepMC::GenParticle*> branch : branches){

// 	// std::vector<HepMC::GenParticle*> branch = branches[0];

// 		TLorentzVector pt = fv_to_lv(branch[0]->momentum());
// 		TLorentzVector pb = fv_to_lv(branch[2]->momentum());
// 		TLorentzVector pl = fv_to_lv(branch[3]->momentum());
// 		TLorentzVector pn = fv_to_lv(branch[4]->momentum());
// 		TLorentzVector q = pl + pn;
// 		double mt = pt.M();
// 		double mq = q.M();

// 		// cout << "mt = " << pt.M() << endl;
// 		// cout << "mW = " << q.M() << endl;
// 		// cout << "mb = " << pb.M() << endl;
// 		// cout << "me = " << pl.M() << endl;
// 		// cout << "mn = " << pn.M() << endl;

// 		double mW = 80.42;
// 		double GammaW = 2.0476;

// 		double Vtb2 = 0.999146;
// 		double Gf = 1.16637e-05;
// 		double gw = mW*sqrt(8/sqrt(2)*Gf);

// 		double tmp = 0;
// 		tmp  = 128* sp(pb,pb) * sp(pb,pt) * sp(pb,pn) * sp(pb,pl) *pow(mW,-4);
// 		tmp -= 128* sp(pb,pb) * sp(pb,pt) * sp(pb,pn) * sp(pt,pl) *pow(mW,-4);
// 		tmp -= 128* sp(pb,pb) * sp(pb,pt) * sp(pb,pl) * sp(pt,pn) *pow(mW,-4);
// 		tmp -= 384* sp(pb,pb) * sp(pb,pt) * sp(pt,pt) * sp(pn,pl) *pow(mW,-4);
// 		tmp += 128* sp(pb,pb) * sp(pb,pt) * sp(pt,pn) * sp(pt,pl) *pow(mW,-4);
// 		tmp += 128* sp(pb,pb) * sp(pb,pt) * sp(pn,pl) *pow(mW,-2);
// 		tmp += 128* sp(pb,pb) * pow(sp(pb,pt),2) * sp(pn,pl) *pow(mW,-4);
// 		tmp -= 256* sp(pb,pb) * sp(pb,pn) * sp(pb,pl) * sp(pt,pt) *pow(mW,-4);
// 		tmp += 256* sp(pb,pb) * sp(pb,pn) * sp(pt,pt) * sp(pt,pl) *pow(mW,-4);
// 		tmp -= 128* sp(pb,pb) * sp(pb,pn) * sp(pt,pl) *pow(mW,-2);
// 		tmp += 256* sp(pb,pb) * sp(pb,pl) * sp(pt,pt) * sp(pt,pn) *pow(mW,-4);
// 		tmp -= 128* sp(pb,pb) * sp(pb,pl) * sp(pt,pn) *pow(mW,-2);
// 		tmp -= 256* sp(pb,pb) * sp(pt,pt) * sp(pt,pn) * sp(pt,pl) *pow(mW,-4);
// 		tmp -= 256* sp(pb,pb) * sp(pt,pt) * sp(pn,pl) *pow(mW,-2);
// 		tmp += 128* sp(pb,pb) * pow(sp(pt,pt),2) * sp(pn,pl) *pow(mW,-4);
// 		tmp += 256* sp(pb,pb) * sp(pt,pn) * sp(pt,pl) *pow(mW,-2);
// 		tmp -=  64* pow(sp(pb,pb),2) * sp(pb,pt) * sp(pn,pl) *pow(mW,-4);
// 		tmp += 128* pow(sp(pb,pb),2) * sp(pt,pt) * sp(pn,pl) *pow(mW,-4);
// 		tmp += 128* sp(pb,pt) * sp(pb,pn) * sp(pb,pl) * sp(pt,pt) *pow(mW,-4);
// 		tmp -= 128* sp(pb,pt) * sp(pb,pn) * sp(pt,pt) * sp(pt,pl) *pow(mW,-4);
// 		tmp -= 128* sp(pb,pt) * sp(pb,pl) * sp(pt,pt) * sp(pt,pn) *pow(mW,-4);
// 		tmp += 128* sp(pb,pt) * sp(pt,pt) * sp(pt,pn) * sp(pt,pl) *pow(mW,-4);
// 		tmp += 128* sp(pb,pt) * sp(pt,pt) * sp(pn,pl) *pow(mW,-2);
// 		tmp -=  64* sp(pb,pt) * pow(sp(pt,pt),2) * sp(pn,pl) *pow(mW,-4);
// 		tmp += 128* pow(sp(pb,pt),2) * sp(pt,pt) * sp(pn,pl) *pow(mW,-4);
// 		tmp += 256* sp(pb,pn) * sp(pb,pl) * sp(pt,pt) *pow(mW,-2);
// 		tmp -= 128* sp(pb,pn) * sp(pt,pt) * sp(pt,pl) *pow(mW,-2);
// 		tmp += 256* sp(pb,pn) * sp(pt,pl);
// 		tmp -= 128* sp(pb,pl) * sp(pt,pt) * sp(pt,pn) *pow(mW,-2);


// 		tmp *= (pow(gw,4) * Vtb2/128.) / (pow(mq*mq-mW*mW,2)+mW*mW*GammaW*GammaW);

// 		ret *= tmp;

// 	}

// 	return ret;
// }

double Matrixelements_pp2ttbar2wbwb::calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & top_final_state){

	double p1[4], p2[4], p3[4], p4[4], p5[4], p6[4], p7[4], p8[4];

	int ix1 = 0;
	int ix2 = 1;
	if(initial_state[0]->pdg_id() < 0){
		ix1 = 1;
		ix2 = 0;
	}

	p1[0] = initial_state[ix1]->momentum().e();
	p1[1] = 0; // initial_state[ix1]->momentum().px();
	p1[2] = 0; // initial_state[ix1]->momentum().py();
	p1[3] = initial_state[ix1]->momentum().pz();

	p2[0] = initial_state[ix2]->momentum().e();
	p2[1] = 0; // initial_state[ix2]->momentum().px();
	p2[2] = 0; // initial_state[ix2]->momentum().py();
	p2[3] = initial_state[ix2]->momentum().pz();


	HepMC::GenParticle *top, *antitop;
	if(top_final_state[0]->pdg_id()>0){
		top = last_status(top_final_state[0]);
		antitop = last_status(top_final_state[1]);
	}else{
		top = last_status(top_final_state[1]);
		antitop = last_status(top_final_state[0]);
	}

	HepMC::GenParticle *wp, *b;
	for(auto it : HepMC::GenVertexParticleRange(*top->end_vertex(), HepMC::children)){
		if(it->pdg_id() > 10) wp = last_status(it);
		if(it->pdg_id() < 10) b = last_status(it);
	}

	HepMC::GenParticle *wm, *bx;
	for(auto it : HepMC::GenVertexParticleRange(*antitop->end_vertex(), HepMC::children)){
		if(it->pdg_id() < -10) wm = last_status(it);
		if(it->pdg_id() > -10) bx = last_status(it);
	}

	HepMC::GenParticle *lp, *lpx;
	for(auto it : HepMC::GenVertexParticleRange(*wp->end_vertex(), HepMC::children)){
		if(it->pdg_id() > 0) lp = last_status(it);
		if(it->pdg_id() < 0) lpx = last_status(it);
	}

	HepMC::GenParticle *lm, *lmx;
	for(auto it : HepMC::GenVertexParticleRange(*wm->end_vertex(), HepMC::children)){
		if(it->pdg_id() > 0) lm = last_status(it);
		if(it->pdg_id() < 0) lmx = last_status(it);
	}


	p3[0] = lp->momentum().e();
	p3[1] = lp->momentum().px();
	p3[2] = lp->momentum().py();
	p3[3] = lp->momentum().pz();

	p4[0] = lpx->momentum().e();
	p4[1] = lpx->momentum().px();
	p4[2] = lpx->momentum().py();
	p4[3] = lpx->momentum().pz();

	p5[0] = b->momentum().e();
	p5[1] = b->momentum().px();
	p5[2] = b->momentum().py();
	p5[3] = b->momentum().pz();

	p6[0] = lm->momentum().e();
	p6[1] = lm->momentum().px();
	p6[2] = lm->momentum().py();
	p6[3] = lm->momentum().pz();

	p7[0] = lmx->momentum().e();
	p7[1] = lmx->momentum().px();
	p7[2] = lmx->momentum().py();
	p7[3] = lmx->momentum().pz();

	p8[0] = bx->momentum().e();
	p8[1] = bx->momentum().px();
	p8[2] = bx->momentum().py();
	p8[3] = bx->momentum().pz();

	std::vector<double*> momenta = {p1,p2,p3,p4,p5,p6,p7,p8};

	int pid1 = initial_state[ix1]->pdg_id();
	int pid2 = initial_state[ix2]->pdg_id();
	int pid3 = lp->pdg_id();
	int pid6 = lm->pdg_id();


	
	if(fabs(pid1) == 21 && fabs(pid2) == 21 && pid3 < 10 && pid6 < 10){
		PP2TTBAR2WBWB_GG_UDXBDUXBX_CPPProcess process = gg_udxbduxbx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
	}else if(fabs(pid1) == 21 && fabs(pid2) == 21 && pid3 > 10 && pid6 < 10){
		PP2TTBAR2WBWB_GG_VEEPBDUXBX_CPPProcess process = gg_veepbduxbx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
	}else if(fabs(pid1) == 21 && fabs(pid2) == 21 && pid3 < 10 && pid6 > 10){
		PP2TTBAR2WBWB_GG_UDXBEMVEXBX_CPPProcess process = gg_udxbemvexbx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
	}else if(fabs(pid1) == 21 && fabs(pid2) == 21 && pid3 > 10 && pid6 > 10){
		PP2TTBAR2WBWB_GG_VEEPBEMVEXBX_CPPProcess process = gg_veepbemvexbx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
		
	}else if(is_quark(pid1) && is_quark(pid2) && pid3 < 10 && pid6 < 10){
		PP2TTBAR2WBWB_UUX_UDXBDUXBX_CPPProcess process = uux_udxbduxbx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
	}else if(is_quark(pid1) && is_quark(pid2) && pid3 > 10 && pid6 < 10){
		PP2TTBAR2WBWB_UUX_VEEPBDUXBX_CPPProcess process = uux_veepbduxbx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
	}else if(is_quark(pid1) && is_quark(pid2) && pid3 < 10 && pid6 > 10){
		PP2TTBAR2WBWB_UUX_UDXBEMVEXBX_CPPProcess process = uux_udxbemvexbx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
	}else if(is_quark(pid1) && is_quark(pid2) && pid3 > 10 && pid6 > 10){
		PP2TTBAR2WBWB_UUX_VEEPBEMVEXBX_CPPProcess process = uux_veepbemvexbx_cppprocess;
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		return process.sigmaHat();
	}
	

	cerr << "no fitting subprocess found.. " << endl;
	cerr << pid1 << ", " << pid2 << " -> " << pid3 << ", " << pid6 << endl;
	return 0;
	
}



double Matrixelements_pp2zp2mupmum::calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state){

	double p1[4], p2[4], p3[4], p4[4];

	p1[0] = initial_state[0]->momentum().e();
	p1[1] = 0; // initial_state[0]->momentum().px();
	p1[2] = 0; // initial_state[0]->momentum().py();
	p1[3] = initial_state[0]->momentum().pz();

	p2[0] = initial_state[1]->momentum().e();
	p2[1] = 0; // initial_state[1]->momentum().px();
	p2[2] = 0; // initial_state[1]->momentum().py();
	p2[3] = initial_state[1]->momentum().pz();

	p3[0] = final_state[0]->momentum().e();
	p3[1] = final_state[0]->momentum().px();
	p3[2] = final_state[0]->momentum().py();
	p3[3] = final_state[0]->momentum().pz();

	p4[0] = final_state[1]->momentum().e();
	p4[1] = final_state[1]->momentum().px();
	p4[2] = final_state[1]->momentum().py();
	p4[3] = final_state[1]->momentum().pz();

	std::vector<double*> momenta = {p1,p2,p3,p4};

	int pid1 = initial_state[0]->pdg_id();
	int pid2 = initial_state[1]->pdg_id();
	int pid3 = final_state[0]->pdg_id();
	int pid4 = final_state[1]->pdg_id();


	if(fabs(pid1) == 1 && fabs(pid2) == 1 && fabs(pid3) == 13 && fabs(pid4) == 13){
		DDX_MUPMUM_CPPProcess process = ddx_mupmum_cppprocess;
		// process.initProc(param_card);
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		// const double * mes = process.getMatrixElements();
		// return mes[0];
		return process.sigmaHat();
	
	}else if(fabs(pid1) == 2 && fabs(pid2) == 2 && fabs(pid3) == 13 && fabs(pid4) == 13){
		UUX_MUPMUM_CPPProcess process = uux_mupmum_cppprocess;
		// process.initProc(param_card);
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		// const double * mes = process.getMatrixElements();
		// return mes[0];
		return process.sigmaHat();

	}else if(fabs(pid1) == 3 && fabs(pid2) == 3 && fabs(pid3) == 13 && fabs(pid4) == 13){
		SSX_MUPMUM_CPPProcess process = ssx_mupmum_cppprocess;
		// process.initProc(param_card);
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		// const double * mes = process.getMatrixElements();
		// return mes[0];
		return process.sigmaHat();

	}else if(fabs(pid1) == 4 && fabs(pid2) == 4 && fabs(pid3) == 13 && fabs(pid4) == 13){
		CCX_MUPMUM_CPPProcess process = ccx_mupmum_cppprocess;
		// process.initProc(param_card);
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		// const double * mes = process.getMatrixElements();
		// return mes[0];
		return process.sigmaHat();

	}else if(fabs(pid1) == 2 && fabs(pid2) == 4 && fabs(pid3) == 13 && fabs(pid4) == 13){
		UCX_MUPMUM_CPPProcess process = ucx_mupmum_cppprocess;
		// process.initProc(param_card);
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		// const double * mes = process.getMatrixElements();
		// return mes[0];
		return process.sigmaHat();

	}else if(fabs(pid1) == 4 && fabs(pid2) == 2 && fabs(pid3) == 13 && fabs(pid4) == 13){
		CUX_MUPMUM_CPPProcess process = cux_mupmum_cppprocess;
		// process.initProc(param_card);
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		// const double * mes = process.getMatrixElements();
		// return mes[0];
		return process.sigmaHat();

	}else if(fabs(pid1) == 1 && fabs(pid2) == 3 && fabs(pid3) == 13 && fabs(pid4) == 13){
		DSX_MUPMUM_CPPProcess process = dsx_mupmum_cppprocess;
		// process.initProc(param_card);
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		// const double * mes = process.getMatrixElements();
		// return mes[0];
		return process.sigmaHat();

	}else if(fabs(pid1) == 3 && fabs(pid2) == 1 && fabs(pid3) == 13 && fabs(pid4) == 13){
		SDX_MUPMUM_CPPProcess process = sdx_mupmum_cppprocess;
		// process.initProc(param_card);
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		// const double * mes = process.getMatrixElements();
		// return mes[0];
		return process.sigmaHat();

	}
	

	cerr << "no fitting subprocess found.. " << endl;
	cerr << pid1 << ", " << pid2 << " -> " << pid3 << ", " << pid4 << endl;
	return 0;
	
}




// double Matrixelements_pp2z2mupmum_pdg::calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state){

// 	double ncolors_i = 3;
// 	double ncolors_f = 1;
// 	double GF = 1.1663787e-5;

// 	SLHAReader slha(param_card);

// 	double mZ = slha.get_block_entry("mass",23);
// 	double gammaZ = slha.get_block_entry("decay",23);

// 	TLorentzVector mediator = add(final_state);
// 	double s = mediator.M2();

// 	double charge_f = 1;
// 	double weak_isospin_f = 0.5;

// 	double charge_i;
// 	double weak_isospin_i;
// 	if(in_list({2,4,6}, fabs(initial_state[0]->pdg_id()))){
// 		charge_i = 2./3.;
// 		weak_isospin_i = 0.5;
// 	}else{
// 		charge_i = -1./3.;
// 		weak_isospin_i = -0.5;
// 	}

// 	double sw2 = 0.23155;


// 	double l2_i = (8*GF*mZ*mZ/sqrt(2)) * pow(weak_isospin_i - sw2*charge_i,2);
// 	double r2_i = (8*GF*mZ*mZ/sqrt(2)) * pow(sw2*charge_i,2);

// 	double l2_f = (8*GF*mZ*mZ/sqrt(2)) * pow(weak_isospin_f - sw2*charge_f,2);
// 	double r2_f = (8*GF*mZ*mZ/sqrt(2)) * pow(sw2*charge_f,2);

// 	TLorentzVector muon = fv_to_lv(final_state[0]->momentum());
// 	muon.Boost(neg(mediator.BoostVector()));
// 	double cos_theta = muon.CosTheta();

// 	double a = ncolors_f/ncolors_i * s/(pow(s-mZ*mZ,2)+s*gammaZ*gammaZ);
// 	double b = (l2_i+r2_i)*(l2_f+r2_f)*(1+cos_theta*cos_theta) + (l2_i-r2_i)*(l2_f-r2_f)*2*cos_theta;

// 	return a*b;

// }





double Matrixelements_pp2z2mupmum::calc(const std::vector<HepMC::GenParticle*> & initial_state, const std::vector<HepMC::GenParticle*> & final_state){

	double p1[4], p2[4], p3[4], p4[4];

	p1[0] = initial_state[0]->momentum().e();
	p1[1] = 0; // initial_state[0]->momentum().px();
	p1[2] = 0; // initial_state[0]->momentum().py();
	p1[3] = initial_state[0]->momentum().pz();

	p2[0] = initial_state[1]->momentum().e();
	p2[1] = 0; // initial_state[1]->momentum().px();
	p2[2] = 0; // initial_state[1]->momentum().py();
	p2[3] = initial_state[1]->momentum().pz();

	p3[0] = final_state[0]->momentum().e();
	p3[1] = final_state[0]->momentum().px();
	p3[2] = final_state[0]->momentum().py();
	p3[3] = final_state[0]->momentum().pz();

	p4[0] = final_state[1]->momentum().e();
	p4[1] = final_state[1]->momentum().px();
	p4[2] = final_state[1]->momentum().py();
	p4[3] = final_state[1]->momentum().pz();

	std::vector<double*> momenta = {p1,p2,p3,p4};

	int pid1 = initial_state[0]->pdg_id();
	int pid2 = initial_state[1]->pdg_id();
	int pid3 = final_state[0]->pdg_id();
	int pid4 = final_state[1]->pdg_id();


	if(fabs(pid1) == 1 && fabs(pid2) == 1 && fabs(pid3) == 13 && fabs(pid4) == 13){
		PP2Z2MUPMUM_DDX_MUPMUM_CPPProcess process = ddx_mupmum_cppprocess;
		// process.initProc(param_card);
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		// const double * mes = process.getMatrixElements();
		// return mes[0];
		return process.sigmaHat();
	
	}else if(fabs(pid1) == 3 && fabs(pid2) == 3 && fabs(pid3) == 13 && fabs(pid4) == 13){
		PP2Z2MUPMUM_DDX_MUPMUM_CPPProcess process = ddx_mupmum_cppprocess;
		// process.initProc(param_card);
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		// const double * mes = process.getMatrixElements();
		// return mes[0];
		return process.sigmaHat();
	
	}else if(fabs(pid1) == 2 && fabs(pid2) == 2 && fabs(pid3) == 13 && fabs(pid4) == 13){
		PP2Z2MUPMUM_UUX_MUPMUM_CPPProcess process = uux_mupmum_cppprocess;
		// process.initProc(param_card);
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		// const double * mes = process.getMatrixElements();
		// return mes[0];
		return process.sigmaHat();

	}else if(fabs(pid1) == 4 && fabs(pid2) == 4 && fabs(pid3) == 13 && fabs(pid4) == 13){
		PP2Z2MUPMUM_UUX_MUPMUM_CPPProcess process = uux_mupmum_cppprocess;
		// process.initProc(param_card);
		process.setInitial(pid1, pid2);
		process.setMomenta(momenta);
		// Evaluate matrix element
		process.sigmaKin();
		// const double * mes = process.getMatrixElements();
		// return mes[0];
		return process.sigmaHat();
	}
	

	cerr << "no fitting subprocess found.. " << endl;
	cerr << pid1 << ", " << pid2 << " -> " << pid3 << ", " << pid4 << endl;
	return 0;
	
}
