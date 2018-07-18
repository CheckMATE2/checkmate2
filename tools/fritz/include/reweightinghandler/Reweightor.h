#ifndef _REWEIGHTOR
#define _REWEIGHTOR

#include <map>
#include <vector>
#include <string>

#include "fastjet/ClusterSequence.hh"
#include "LHAPDF/LHAPDF.h"
#include "HepMC/GenEvent.h"
#include "LHAPDF/LHAPDF.h"

#include "read_slha.h"
#include "reweightingaux.h"
#include "Matrixelements.h"
#include "ReweightingProcInfo.h"

class Reweightor{

public:
	//! Standard Constructor
	Reweightor(const std::string & configPath);
	//! Standard Destructor
	~Reweightor();

	void setup(const std::string & configPath);	

	void initialize(const std::string & filepath);

	//! 
	//! \param evt GenEvent to be transformed and reweighted
	//! \return pair of a) boolean return code b) list of pairs of events and eventinformation. Each item in the list corresponds to one target parameter point.
	std::vector<std::pair<bool, std::pair<HepMC::GenEvent*,ReweightingProcInfo>>> reweight(HepMC::GenEvent* evt);


private:

	std::map<int,int> add_underlying_event(
		HepMC::GenEvent* new_evt, 
		HepMC::GenEvent* old_evt, 
		JetCandidateContainer & jet_candidates
		);

	double pdfs(
		const std::vector<HepMC::GenParticle*> & initial_state, 
		const double & scale
		);

	double phase_space(
		const std::vector<HepMC::GenParticle*> & final_state, 
		const std::map<int,double> & masses
		);

	double matrix_element(
		std::vector<HepMC::GenParticle*> & initial_state, 
		std::vector<HepMC::GenParticle*> & final_state, 
		Matrixelements * mes
		);

	void smear_momenta(
		std::vector<HepMC::GenParticle*> particles, 
		HepMC::GenEvent* new_evt
		);

	std::vector<HepMC::GenParticle*> trans_initial_state(
		const std::vector<HepMC::GenParticle*> & state, 
		const double & a
		);

	std::vector<HepMC::GenParticle*> trans(
		const std::vector<HepMC::GenParticle*> & state, 
		const double & a,
		const TVector3 & old_rest_frame,
		const TVector3 & new_rest_frame,
		SLHAReader & oldSLHA,
		SLHAReader & newSLHA
		);

	void trans_1_to_2(
		HepMC::GenParticle* old_in, 
		HepMC::GenParticle* new_in, 
		SLHAReader & new_slha,
		HepMC::GenEvent* new_evt
		);

	void trans_1_to_n(
		HepMC::GenParticle* old_in, 
		HepMC::GenParticle* new_in, 
		SLHAReader & new_slha, 
		HepMC::GenEvent* new_evt
		);

	void trans_1_to_n_jets(
		HepMC::GenParticle* old_in, 
		HepMC::GenParticle* new_in, 
		std::vector<fastjet::PseudoJet> contributions, 
		HepMC::GenEvent* new_evt
		);

	void trans_jets(
		const std::vector<fastjet::PseudoJet> & old_jets, 
		const std::map<int,int> & jet_constituents_to_partons, 
		const std::vector<fastjet::PseudoJet> ISR_jet_candidates, 
		HepMC::GenEvent* old_evt, HepMC::GenEvent* new_evt
		);

	void trans_parton_shower(
		HepMC::GenParticle* old_in , 
		HepMC::GenParticle* new_in, 
		SLHAReader & new_slha, 
		HepMC::GenEvent* new_evt
		);

	void trans_decays(
		std::vector<HepMC::GenParticle*> boosted_final_state, 
		std::vector<HepMC::GenParticle*> new_boosted_final_state, 
		HepMC::GenEvent* new_evt, 
		SLHAReader & new_slha,
		JetCandidateContainer & jet_candidates
		);
	
	Matrixelements * getMatrixElements();

	std::map<std::string,SLHAReader> slhaConfig;
	bool matrixElementReweighting;
	bool writingMode;
	bool detectorSimulation;
	std::string procName;
	
	Matrixelements * mesOld;
	std::map<std::string,Matrixelements*> mesNewMap;
	
	std::string pdfName; // name of the parton density function
	LHAPDF::PDF * pdf; // get parton density function from LHAPDF
	
	const std::string keySections = "section";
	const std::string keyGlobal = "Global";
	const std::string keySLHAFiles = "SLHAFiles";


};


#endif