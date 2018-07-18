#ifndef _REWEIGHTINGAUX
#define _REWEIGHTINGAUX

#include <vector>
#include <map>
#include <algorithm>

#include "TLorentzVector.h"
#include "HepMC/GenEvent.h"
#include "fastjet/ClusterSequence.hh"



struct JetCandidateContainer{
	std::vector<int> barcodes;
	std::vector<fastjet::PseudoJet> candidates;
};

/* debug */

template <typename T>
void debug(T thing, const char prefix[]=""){
	std::cout << "DEBUG: " << prefix << thing << std::endl;
}


/* implemented in reweightingaux.cpp */

bool is_quark(const int & pdg_id);

bool is_squark(const int & pdg_id);

std::string particle_names(const int & pdg_id);

int particle_ids(const std::string & name);

std::map<int,std::vector<HepMC::GenParticle*>> get_particles(HepMC::GenEvent* evt, std::vector<int> pdg_ids);

HepMC::GenParticle* last_status(HepMC::GenParticle* p);

TLorentzVector add(const std::vector<HepMC::GenParticle*> & particles);

TLorentzVector add(const std::vector<TLorentzVector> & momenta);

TLorentzVector add(const std::vector<fastjet::PseudoJet> & momenta);

double sp(const TLorentzVector & p1, const TLorentzVector & p2);

fastjet::PseudoJet fv_to_pj(const HepMC::FourVector & fv);

HepMC::FourVector pj_to_fv(const fastjet::PseudoJet & pj);

TLorentzVector pj_to_lv(const fastjet::PseudoJet & pj);

TLorentzVector fv_to_lv(const HepMC::FourVector & fv);

HepMC::FourVector lv_to_fv(const TLorentzVector & lv);

TVector3 neg(const TVector3 & v);

bool momentum_conserved(HepMC::GenVertex* v);

HepMC::GenParticle* copy_genparticle(HepMC::GenParticle* p);

HepMC::GenVertex* copy_genvertex(HepMC::GenVertex* v);

template <typename T>
inline bool in_list(const std::vector<T> & list, const T & thing){
	return find(list.begin(), list.end(), thing) != list.end();
}

template <typename T>
inline T sum(const std::vector<T> & list){
	T ret;
	for(T el : list){
		ret += el;
	}
	return ret;
}


std::vector<TLorentzVector> boost_to_restframe(HepMC::GenParticle* p, const std::vector<HepMC::GenParticle*> & rest);

double kaellen(const double & a, const double & b, const double & c);

void print_pseudojet(const fastjet::PseudoJet & pj);

TVector3 boost_vector_between(const TLorentzVector & p, const TLorentzVector & q);


bool gauss_alg(std::vector<std::vector<double> > & matrix);

class func_rootfinder{
	private:
		static double E;
		static std::vector<double> mass;
		static std::vector<double> p_frac;
	public:
		static double func_for_root(double p1);
		void set(double E,const std::vector<double>& mass, const std::vector<double>& p_frac); 
		double get_root();
	
	};

class Mini{
	private:
		static double foo(const double *xx);
		static std::vector<TVector3> orig;
		static std::vector<std::vector<double> > sol;
		static double new_inv_mass;
		static std::vector<double> dec_par_vec_mass;
	public:
		std::vector<double> minimize(const std::vector<TVector3> & orig,const std::vector<std::vector<double> > & sol,const double & new_inv_mass,const std::vector<double> & dec_par_vec_mass);
};


#endif