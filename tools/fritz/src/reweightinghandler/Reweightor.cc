#include <stack>

#include "Reweightor.h"
#include "reweightingaux.h"
#include "FritzConfig.h"
#include "ConfigParser.h"
#include "detector_simulation.h"
#include "HepMC/GenRanges.h"


Reweightor::Reweightor(const std::string & configPath){
	detectorSimulation = false;
	writingMode = false;
	matrixElementReweighting = false;
	pdfName = "NNPDF23_lo_as_0130_qed";
	procName = "";
	setup(configPath);
}

Reweightor::~Reweightor(){
	delete mesOld;
	std::map<std::string,Matrixelements*>::iterator it;
	for(it=mesNewMap.begin(); it!=mesNewMap.end(); it++) delete it->second;
	delete pdf;
}

static const std::string keyName = "name";
static const std::string keyPDFName = "pdf_name";
static const std::string keyProcName = "procname";
static const std::string keyWritingMode = "writing_mode";
static const std::string keyDetectorSimulation = "detector_simulation";
static const std::string keyMatrixElementReweighting = "matrix_element_reweighting";

static void unknownKeys(Properties props) {
	// only used for Section "global"
    std::vector<std::string> knownKeys;
    knownKeys.push_back(keyName);
    knownKeys.push_back(keyPDFName);
    knownKeys.push_back(keyProcName);
	knownKeys.push_back(keyWritingMode);
	knownKeys.push_back(keyDetectorSimulation);
	knownKeys.push_back(keyMatrixElementReweighting);
    warnUnknownKeys(
        props,
        knownKeys,
        props["name"],
        "Unknown key for a Reweightor in Section Global"
        );
}


void Reweightor::setup(const std::string & configPath){

	// read config file
	Global::print("Reweightor","Read configPath: "+configPath);
	Config conf = parseConfigFile(configPath);
	Global::print("Reweightor","done.");
	
	Sections sects = lookupRequired(
		conf,
		keySections,
		"Reweightor",
		"No Sections given in Reweighting settings file: "+configPath
		);

	for(std::map<std::string, Properties>::iterator it=sects.begin(); it!=sects.end(); it++){
		std::cout << it->first << std::endl;
	}

	Properties slhaFilesProps = lookupRequired(
		sects,
		keySLHAFiles,
		"Reweightor",
		"No SLHA files given for Reweighting in "+configPath
		);

	for(std::map<std::string, std::string>::iterator it=slhaFilesProps.begin(); it!=slhaFilesProps.end(); it++){
		if(it->first == "name") continue;
		SLHAReader slha(it->second);
		slhaConfig[it->first] = slha;
	}

	Properties globalProps;
	if(hasKey(sects,keyGlobal)){
		globalProps = sects[keyGlobal];
		unknownKeys(globalProps);
		pdfName = maybeLookup(globalProps,keyPDFName,pdfName).second;
		writingMode = maybeLookupBool(globalProps,keyWritingMode,writingMode).second;
		detectorSimulation = maybeLookupBool(globalProps,keyDetectorSimulation,detectorSimulation).second;
		matrixElementReweighting = maybeLookupBool(globalProps,keyMatrixElementReweighting,matrixElementReweighting).second;
		
	}else{
		Global::warn("Reweightor","No global settings for Reweighting in "+configPath+".\nUse defaults.");
	}

	pdf = LHAPDF::mkPDF(pdfName);

	// get matrix elements if matrixElementReweighting is true
	if(!matrixElementReweighting) return;

	procName = lookupRequired(
		globalProps,
		keyProcName,
		"Reweightor",
		"No procname given for Matrixelement Reweighting in "+configPath
		);

	Global::print("Reweightor","Init Matrixelements for reweighting.");
	mesOld = getMatrixElements();
	mesOld->init(slhaConfig["base"].file_name());

	for(std::map<std::string, SLHAReader>::iterator it=slhaConfig.begin(); it!=slhaConfig.end(); it++){
		if(it->first == "base") continue;
		Matrixelements * mesNew = getMatrixElements();
		mesNew->init(it->second.file_name());
		if(!mesNewMap.insert(std::make_pair(it->first, mesNew)).second){
			Global::abort("Reweightor","Found multiple Reweighting targets with equal name: "+it->first);
		};
	}
	
}


std::map<int,int> Reweightor::add_underlying_event(HepMC::GenEvent* new_evt, HepMC::GenEvent* old_evt, JetCandidateContainer & jet_candidates){

	// we want to return the interface between underlying event and hard event
	// which is represented by maps from particle barcodes to parent vertex barcodes.
	// We therefore return one such map for each particle in the initial state
	std::map<int,int> interface;

	HepMC::GenParticle* proton;
	HepMC::GenVertex* gv; // dummy pointer that we use to create all the new vertices
	HepMC::GenParticle *gp, *gp_copy; // dummy pointer that runs over all old particles and a copy that we put in the new event
	std::vector<HepMC::GenParticle*> queue;
	std::vector<HepMC::GenParticle*> visited;

	// start with the protons (barcode 1 and 2)
	for(int proton_bc=1;proton_bc<=2;proton_bc++){
		proton = old_evt->barcode_to_particle(proton_bc);
		queue = {proton};

		while(queue.size()>0){
			gp = queue[0];
			queue.erase(queue.begin());

			if(in_list(visited, gp)) continue;
			visited.push_back(gp);

			// not detector level and not hard event -> underlying event
			if(gp->end_vertex() && gp->end_vertex()->barcode() != -3){

				gp_copy = copy_genparticle(gp);

				if(gp->production_vertex() && gp->production_vertex()->barcode() != -3){
					int parent_vertex_barcode = gp->production_vertex()->barcode();
					new_evt->barcode_to_vertex(parent_vertex_barcode)->add_particle_out(gp_copy);
				}

				// check if end vertex is already part of the new event:
				if(new_evt->barcode_to_vertex(gp->end_vertex()->barcode())){
					new_evt->barcode_to_vertex(gp->end_vertex()->barcode())->add_particle_in(gp_copy);
				}else{
					gv = copy_genvertex(gp->end_vertex());
					gv->add_particle_in(gp_copy);
					new_evt->add_vertex(gv);
				}

				for(HepMC::GenParticle* q : HepMC::GenVertexParticleRange(*(gp->end_vertex()), HepMC::children)){
					queue.push_back(q);
				}

			// initial state:
			}else if(gp->end_vertex()){

				interface[gp->barcode()] = new_evt->barcode_to_vertex(gp->production_vertex()->barcode())->barcode();

			}else{
				// detector level (status=1) in underlying event

				// Same particle can be reached at least twice (once from every proton)
				// test if particle is already part of the event
				if(!new_evt->barcode_to_particle(gp->barcode())){
					gp_copy = copy_genparticle(gp);

					int parent_vertex_barcode = gp->production_vertex()->barcode();
					new_evt->barcode_to_vertex(parent_vertex_barcode)->add_particle_out(gp_copy);

					//fastjet::PseudoJet candidate = fv_to_pj(gp_copy->momentum());
					//candidate.set_user_index(gp->barcode());
					//jet_candidates.barcodes.push_back(gp->barcode());
					//jet_candidates.candidates.push_back(candidate);
				}
			}

		}	
	}

	return interface;
}


double Reweightor::pdfs(const std::vector<HepMC::GenParticle*> & initial_state, const double & scale){
	// We expect the PDFs to give us the leading behaviour of the angular distributions.

	double x1 = initial_state[0]->momentum().e()/6500.;
	double x2 = initial_state[1]->momentum().e()/6500.;
	double id1 = initial_state[0]->pdg_id();
	double id2 = initial_state[1]->pdg_id();

	return pdf->xfxQ(id1, x1, scale)/x1 * pdf->xfxQ(id2, x2, scale)/x2;
}



double Reweightor::phase_space(const std::vector<HepMC::GenParticle*> & final_state, const std::map<int,double> & masses){

	int pid3 = fabs(final_state[0]->pdg_id());
	int pid4 = fabs(final_state[1]->pdg_id());
	
	double m3;
	if(masses.find(pid3) != masses.end()){
		m3 = masses.at(pid3);
	}else{
		m3 = slhaConfig["base"].get_block_entry("mass",pid3);
	}

	double m4;
	if(masses.find(pid4) != masses.end()){
		m4 = masses.at(pid4);
	}else{
		m4 = slhaConfig["base"].get_block_entry("mass",pid4);
	}
	
	double shat = add(final_state).M2();

	double ret = kaellen(shat,m3*m3,m4*m4)/shat;
	if(ret != ret){
		cout << endl;
		cout << "m3 = " << m3 << endl;
		cout << "m4 = " << m4 << endl;
		cout << "shat = " << shat << endl;
		cout << "kaellen(shat,m3*m3,m4*m4) = " << kaellen(shat,m3*m3,m4*m4) << endl;
		return 1;
	}

	return ret;
}


double Reweightor::matrix_element(std::vector<HepMC::GenParticle*> & initial_state, std::vector<HepMC::GenParticle*> & final_state, Matrixelements * mes){

	// double me = pp2sqsqbar_nogluino_pdg(initial_state, final_state, (Matrixelements_pp2sqsqbar_nogluino*) mes);
	double me = mes->calc(initial_state, final_state);
	
	return me;

}




void Reweightor::smear_momenta(std::vector<HepMC::GenParticle*> particles, HepMC::GenEvent* new_evt){

	int pid = fabs(particles[0]->pdg_id());
	std::vector<HepMC::FourVector> old_momenta;
	old_momenta.resize(particles.size());
	transform(particles.begin(), particles.end(), old_momenta.begin(), [](HepMC::GenParticle* p){return p->momentum();});
	
	std::vector<HepMC::FourVector> new_momenta;
	switch(pid){
		case 11 : new_momenta = smear_electrons(old_momenta); break;
		case 13 : new_momenta = smear_muons(old_momenta); break;
		case 15 : new_momenta = smear_taus(old_momenta); break;
		case 747 : new_momenta = smear_jets(old_momenta); break;
	}

	if(new_momenta.size() == 0) return;

	HepMC::GenVertex * gv;
	for(unsigned i=0; i<particles.size(); i++){
		gv = new HepMC::GenVertex();
		gv->add_particle_in(particles[i]);
		gv->add_particle_out(
			new HepMC::GenParticle(
				new_momenta[i],
				particles[i]->pdg_id(),
				1
			)
		);
	}
	new_evt->add_vertex(gv);

}



std::vector<HepMC::GenParticle*> Reweightor::trans_initial_state(const std::vector<HepMC::GenParticle*> & state, const double & a){
	TLorentzVector mediator = fv_to_lv(state[0]->momentum())+fv_to_lv(state[1]->momentum());
	TLorentzVector new_mediator = TLorentzVector(0,0,a*mediator.Pz(),a*mediator.E());
	
	TVector3 bv = mediator.BoostVector();
	TVector3 new_bv = new_mediator.BoostVector();

	double m1_2 = state[0]->momentum().m2();
	double m2_2 = state[1]->momentum().m2();

	double new_r = kaellen(new_mediator.M2(), m1_2, m2_2)/(2*new_mediator.M());
	
	double new_e, new_px, new_py, new_pz;
	TLorentzVector new_lv;
	

	new_lv = fv_to_lv(state[0]->momentum());
	new_lv.Boost(neg(bv));

	new_e = sqrt(new_r*new_r + m1_2);
	new_px = new_r / new_lv.Vect().Mag() * new_lv.Px();
	new_py = new_r / new_lv.Vect().Mag() * new_lv.Py();
	new_pz = new_r / new_lv.Vect().Mag() * new_lv.Pz();

	new_lv.SetXYZT(new_px, new_py, new_pz, new_e);
	new_lv.Boost(new_bv);
	HepMC::GenParticle* new_p1 = new HepMC::GenParticle(lv_to_fv(new_lv), state[0]->pdg_id(), state[0]->status());
	


	new_lv = fv_to_lv(state[1]->momentum());
	new_lv.Boost(neg(bv));

	new_e = sqrt(new_r*new_r + m2_2);
	new_px = new_r / new_lv.Vect().Mag() * new_lv.Px();
	new_py = new_r / new_lv.Vect().Mag() * new_lv.Py();
	new_pz = new_r / new_lv.Vect().Mag() * new_lv.Pz();
	
	new_lv.SetXYZT(new_px, new_py, new_pz, new_e);
	new_lv.Boost(new_bv);
	HepMC::GenParticle* new_p2 = new HepMC::GenParticle(lv_to_fv(new_lv), state[1]->pdg_id(), state[1]->status());
	
		

	std::vector<HepMC::GenParticle*> ret = {new_p1, new_p2};
	return ret;

}



std::vector<HepMC::GenParticle*> Reweightor::trans(const std::vector<HepMC::GenParticle*> & state, 
						   const double & a,
				   	   	   const TVector3 & old_rest_frame,
				   	   	   const TVector3 & new_rest_frame,
				   	   	   SLHAReader & oldSLHA,
				   	   	   SLHAReader & newSLHA){

	std::vector<double> old_offshell_masses, new_offshell_masses;
	for(HepMC::GenParticle* p : state){
		double old_onshell_mass, new_onshell_mass;
		double old_offshell_mass, new_offshell_mass;
	
		int pid = fabs(p->pdg_id());

		old_onshell_mass = oldSLHA.get_block_entry("mass",pid);
		new_onshell_mass = newSLHA.get_block_entry("mass",pid);

		old_offshell_mass = p->momentum().m();
		if(old_offshell_mass < 0 || old_offshell_mass < 1)
			old_offshell_mass = 0;


		if( fabs(old_onshell_mass-old_offshell_mass) < 0.00001 ){
			new_offshell_mass = new_onshell_mass;
		}else{
			new_offshell_mass = new_onshell_mass + new_onshell_mass/old_onshell_mass * (old_offshell_mass - old_onshell_mass); // @todo: should be ratio of decay widths not onshell masses!
		}

		old_offshell_masses.push_back(old_offshell_mass);
		new_offshell_masses.push_back(new_offshell_mass);
	}

	TLorentzVector p1 = fv_to_lv(state[0]->momentum());
	p1.Boost(neg(old_rest_frame));
	TLorentzVector p2 = fv_to_lv(state[1]->momentum());
	p2.Boost(neg(old_rest_frame));
	std::vector<TLorentzVector> rf_state = {p1,p2};

	TLorentzVector q = p1+p2;
	double shat  = q.M2();
	double shat_prime = a * a * shat;

	double r_old = kaellen(shat, pow(old_offshell_masses[0],2), pow(old_offshell_masses[1],2))/(2*sqrt(shat));
	double r_new = kaellen(shat_prime, pow(new_offshell_masses[0],2), pow(new_offshell_masses[1],2))/(2*sqrt(shat_prime));
	double e1_new = sqrt(r_new*r_new + pow(new_offshell_masses[0],2));
	double e2_new = sqrt(r_new*r_new + pow(new_offshell_masses[1],2));

	TVector3 v1_new = r_new/r_old * p1.Vect();
	TVector3 v2_new = -v1_new;
	TLorentzVector p1_new = TLorentzVector(v1_new, e1_new);
	TLorentzVector p2_new = TLorentzVector(v2_new, e2_new);

	p1_new.Boost(new_rest_frame);
	p2_new.Boost(new_rest_frame);

	std::vector<HepMC::GenParticle*> new_state;


	HepMC::GenParticle* new_p;

	new_p = new HepMC::GenParticle(lv_to_fv(p1_new),state[0]->pdg_id(),state[0]->status());
	new_p->suggest_barcode(state[0]->barcode());
	new_state.push_back(new_p);

	new_p = new HepMC::GenParticle(lv_to_fv(p2_new),state[1]->pdg_id(),state[1]->status());
	new_p->suggest_barcode(state[1]->barcode());
	new_state.push_back(new_p);

	return new_state;


}



void Reweightor::trans_1_to_2(HepMC::GenParticle* old_in, HepMC::GenParticle* new_in, SLHAReader & new_slha, HepMC::GenEvent* new_evt){

	std::vector<HepMC::GenParticle*> old_out;
	for(auto it : HepMC::GenVertexParticleRange(*(old_in->end_vertex()), HepMC::children)){
		old_out.push_back(it);
	}
	std::vector<TLorentzVector> old_out_com = boost_to_restframe(old_in, old_out); // old event decay products in the center-of-mass (c.o.m.) frame 

	// decay: I -> J K
	double m_I = new_in->momentum().m();

	std::vector<double> new_out_masses;
	for(unsigned i=0;i<old_out.size();i++){
		int pid = fabs(old_out[i]->pdg_id());
		double old_onshell_mass = slhaConfig["base"].get_block_entry("mass",pid);
		double new_onshell_mass = new_slha.get_block_entry("mass",pid);
		if( old_onshell_mass-new_onshell_mass < 0.1 ){
			// no mass change intended
			double old_mass = old_out[i]->momentum().m();
			new_out_masses.push_back(old_mass);
		}else{
			// mass is changed
			if(!new_slha.has_block_entry("decay",pid)){
				// no width, i.e. no decay, i.e. on-shell
				new_out_masses.push_back(new_onshell_mass);
			}else{
				// keep off-shellness constant
				double a = new_slha.get_block_entry("decay",pid)/slhaConfig["base"].get_block_entry("decay",pid);
				double new_mass = new_onshell_mass + a * (old_out[i]->momentum().m() - old_onshell_mass);
				new_out_masses.push_back(new_mass);
			}

		}
	}
	double m_J = new_out_masses[0];
	double m_K = new_out_masses[1];


	double r_new = kaellen(pow(m_I,2), pow(m_J,2), pow(m_K,2)) / (2*m_I);
	double e_new, theta, phi;

	e_new = sqrt(m_J*m_J + r_new*r_new);
	theta = old_out_com[0].Theta();
	phi = old_out_com[0].Phi();
	TLorentzVector lv_new_J(r_new * sin(theta) * cos(phi),
							r_new * sin(theta) * sin(phi),
							r_new * cos(theta),
							e_new);
	lv_new_J.Boost(fv_to_lv(new_in->momentum()).BoostVector());
	HepMC::FourVector fv_new_J = lv_to_fv(lv_new_J);
	
	e_new = sqrt(m_K*m_K + r_new*r_new);
	theta = old_out_com[1].Theta();
	phi = old_out_com[1].Phi();
	TLorentzVector lv_new_K(r_new * sin(theta) * cos(phi),
						    r_new * sin(theta) * sin(phi),
						    r_new * cos(theta),
						    e_new);
	lv_new_K.Boost(fv_to_lv(new_in->momentum()).BoostVector());
	HepMC::FourVector fv_new_K = lv_to_fv(lv_new_K);

	std::vector<HepMC::GenParticle*> new_out = {
		new HepMC::GenParticle(fv_new_J, old_out[0]->pdg_id(), old_out[0]->status()),
		new HepMC::GenParticle(fv_new_K, old_out[1]->pdg_id(), old_out[1]->status())
	};
	new_out[0]->suggest_barcode(old_out[0]->barcode());
	new_out[1]->suggest_barcode(old_out[1]->barcode());

	HepMC::GenVertex* gv = copy_genvertex(old_in->end_vertex());
	gv->suggest_barcode(old_in->end_vertex()->barcode());
	new_evt->add_vertex(gv);

	gv->add_particle_in(new_in);
	for(HepMC::GenParticle* nout : new_out){
		gv->add_particle_out(nout);
	}
	
}


void Reweightor::trans_1_to_n(HepMC::GenParticle* old_in, HepMC::GenParticle* new_in, SLHAReader & new_slha, HepMC::GenEvent* new_evt){
	
	std::vector<HepMC::GenParticle*> old_out;
	for(auto it=old_in->end_vertex()->particles_out_const_begin(); it!=old_in->end_vertex()->particles_out_const_end(); it++){
		old_out.push_back(*it);
	}
	std::vector<TLorentzVector> old_out_com = boost_to_restframe(old_in, old_out); // old event decay products in the center-of-mass (c.o.m.) frame 

	std::vector<double> new_out_masses;
	for(unsigned i=0;i<old_out.size();i++){
		int pid = fabs(old_out[i]->pdg_id());
		double old_onshell_mass = slhaConfig["base"].get_block_entry("mass",pid);
		double new_onshell_mass = new_slha.get_block_entry("mass",pid);
		if( old_onshell_mass-new_onshell_mass < 0.1 ){
			// no mass change intended
			double old_mass = old_out[i]->momentum().m();
			new_out_masses.push_back(old_mass);
		}else{
			// mass is changed
			if(!new_slha.has_block_entry("decay",pid)){
				// no width, i.e. no decay, i.e. on-shell
				new_out_masses.push_back(new_onshell_mass);
			}else{
				// keep off-shellness constant
				double a = new_slha.get_block_entry("decay",pid)/slhaConfig["base"].get_block_entry("decay",pid);
				double new_mass = new_onshell_mass + a * (old_out[i]->momentum().m() - old_onshell_mass);
				new_out_masses.push_back(new_mass);
			}
		}
	}

	std::vector<double> momentum_fractions;

	if(old_out.size() > 4){
		std::vector<TVector3> u;
		std::vector<TVector3> orig;
		for(int i=0; i < old_out_com.size(); i++){
			u.push_back(1./old_out_com[i].Vect().Mag()*old_out_com[i].Vect());
			orig.push_back(old_out_com[i].Vect());
		}
		
		std::vector<std::vector<double>> eq_sys(3);
		for(int i = 1; i<u.size();i++){
			for(int j = 0; j<3;j++){
				eq_sys[j].push_back(u[i][j]);
			}
		}
		eq_sys[0].push_back(-u[0][0]);
		eq_sys[1].push_back(-u[0][1]);
		eq_sys[2].push_back(-u[0][2]);
		
		gauss_alg(eq_sys);
		
		std::vector<std::vector<double>> solution(3);
		for(int i = 3; i < orig.size();i++){
			solution[0].push_back(eq_sys[0][i]);
			solution[1].push_back(eq_sys[1][i]);
			solution[2].push_back(eq_sys[2][i]);
		}
		
		// minimize solution according to paper
		Mini min;
		momentum_fractions = min.minimize(orig,solution,new_in->momentum().m(),new_out_masses);
		
	}else{
		for(unsigned i=0; i<old_out.size(); i++){
			momentum_fractions.push_back(old_out_com[i].Vect().Mag()/old_out_com[0].Vect().Mag()); //calculate momentum fractions
		}
	}
	
		
	// calculate new momenta and energies based on the new invariant mass	
	func_rootfinder finder;
	finder.set(new_in->momentum().m(),new_out_masses,momentum_fractions);
	double abs_mom1 = finder.get_root();
	
	std::vector<HepMC::GenParticle*> new_out;

	for(unsigned i=0; i<old_out_com.size(); i++){

		double r_new = momentum_fractions[i]*abs_mom1;
		double e_new = sqrt(r_new*r_new + new_out_masses[i]*new_out_masses[i]); 
		TLorentzVector p_new(r_new * sin(old_out_com[i].Theta()) * cos(old_out_com[i].Phi()),
						  	 r_new * sin(old_out_com[i].Theta()) * sin(old_out_com[i].Phi()),
						  	 r_new * cos(old_out_com[i].Theta()),
						  	 e_new);
		p_new.Boost(fv_to_lv(new_in->momentum()).BoostVector());
		HepMC::FourVector fv_new = lv_to_fv(p_new);
		new_out.push_back(
			new HepMC::GenParticle(fv_new, old_out[i]->pdg_id(), old_out[i]->status())
		);
		new_out[new_out.size()-1]->suggest_barcode(old_out[i]->barcode());

	}


	HepMC::GenVertex* gv = new HepMC::GenVertex();
	gv->add_particle_in(new_in);
	for(HepMC::GenParticle* nout : new_out){
		gv->add_particle_out(nout);
	}
	new_evt->add_vertex(gv);
		
}



void Reweightor::trans_1_to_n_jets(HepMC::GenParticle* old_in, HepMC::GenParticle* new_in, std::vector<fastjet::PseudoJet> contributions, HepMC::GenEvent* new_evt){
	
	HepMC::GenVertex* gv = new HepMC::GenVertex();
	gv->add_particle_in(new_in);

	TLorentzVector old_in_momentum = fv_to_lv(old_in->momentum());
	TLorentzVector new_in_momentum = fv_to_lv(new_in->momentum());
	TLorentzVector diff = new_in_momentum - old_in_momentum;
	
	sort(contributions.begin(), contributions.end(), [](fastjet::PseudoJet pj1, fastjet::PseudoJet pj2){
		return pj1.e() > pj2.e();
	});

	HepMC::GenParticle* gp;
	for(unsigned i=0;i<contributions.size();i++){
		TLorentzVector cm = pj_to_lv(contributions[i]);
		if(i==0){
			cm += diff;
		}
		gp = new HepMC::GenParticle(lv_to_fv(cm), 746, 71);
		gv->add_particle_out(gp);
	}

	new_evt->add_vertex(gv);
}



void Reweightor::trans_jets(const std::vector<fastjet::PseudoJet> & old_jets, 
				const std::map<int,int> & jet_constituents_to_partons, 
				const std::vector<fastjet::PseudoJet> ISR_jet_candidates, 
				HepMC::GenEvent* old_evt, HepMC::GenEvent* new_evt)
{


	// do nothing -> no momentum conservation

	// std::map<HepMC::GenParticle*, std::vector<HepMC::GenParticle*>> old_ins;

	// for(fastjet::PseudoJet j : old_jets){
	// 	std::vector<fastjet::PseudoJet> constituents = j.constituents();
	// 	for(int i_constituent = 0; i_constituent < constituents.size(); i_constituent++){
	// 		fastjet::PseudoJet c = constituents[i_constituent];
	// 		if(jet_constituents_to_partons.find(c.user_index()) == jet_constituents_to_partons.end()) continue;
			
	// 		old_in = old_evt->barcode_to_particle(jet_constituents_to_partons.at(c.user_index()));
	// 		new_in = new_evt->barcode_to_particle(old_in->barcode());

	// 		if(new_in->end_vertex()==0){
	// 			HepMC::GenVertex* gv = copy_genvertex(old_in->end_vertex());
	// 			gv->add_particle_in(new_in);
	// 			new_evt->add_vertex(gv);
	// 		}
			
	// 		leaf = new HepMC::GenParticle(pj_to_fv(c), 747, 1);
	// 		new_in->end_vertex()->add_particle_out(leaf);
	// 		leaf->suggest_barcode(c.user_index());

	// 		old_ins[old_in].push_back(leaf);
			
	// 	}
	// }

	// cout << endl;
	// for(auto p : old_ins){
	// 	TLorentzVector total_momentum = fv_to_lv(p.first->momentum());
	// 	cout << p.first->barcode() << " -> [";
	// 	for(auto l : p.second){
	// 		total_momentum -= fv_to_lv(l->momentum());
	// 		cout << l->barcode() << ",";
	// 	}
	// 	cout << "]" << endl;
	// 	cout << "\t"; total_momentum.Print(); cout << endl;
	// }

	// return;

	// distribute additional momentum amongst the jets
	
	HepMC::GenParticle *old_in, *new_in;

	std::map<HepMC::GenParticle*, std::vector<fastjet::PseudoJet>> hard_parton_2_contributions;
	for(fastjet::PseudoJet j : old_jets){
		std::vector<fastjet::PseudoJet> constituents = j.constituents();
		for(int i_constituent = 0; i_constituent < constituents.size(); i_constituent++){
			fastjet::PseudoJet c = constituents[i_constituent];
			if(jet_constituents_to_partons.find(c.user_index()) == jet_constituents_to_partons.end()) 
				continue;
			
			old_in = old_evt->barcode_to_particle(jet_constituents_to_partons.at(c.user_index()));
			int n = hard_parton_2_contributions[old_in].size();

			if(i_constituent == 0 || n == 0){
				hard_parton_2_contributions[old_in].push_back(c); 
			}else{
				hard_parton_2_contributions[old_in][n-1] += c; 	
			}

		}
	}


	HepMC::GenVertex* gv = new HepMC::GenVertex();

	std::vector<fastjet::PseudoJet> new_jet_candidates;
	for(auto it : hard_parton_2_contributions){
		old_in = it.first;
		new_in = new_evt->barcode_to_particle(old_in->barcode());
		trans_1_to_n_jets(old_in, new_in, it.second, new_evt); 
		for(auto child : HepMC::GenVertexParticleRange(*(new_in->end_vertex()), HepMC::children)){
			new_jet_candidates.push_back(fv_to_pj(child->momentum()));
			gv->add_particle_in(child);
		}
	}
	new_jet_candidates.insert(new_jet_candidates.begin(), ISR_jet_candidates.begin(), ISR_jet_candidates.end());

	double R = 0.7;
	fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R);
	fastjet::ClusterSequence cs(new_jet_candidates, jet_def);
	std::vector<fastjet::PseudoJet> new_jets = sorted_by_pt(cs.inclusive_jets());

	// add new jets to new event
	HepMC::GenParticle* gp;
	std::vector<HepMC::GenParticle*> jet_genparticles; // needed for smearing
	for(fastjet::PseudoJet nj : new_jets){
		HepMC::FourVector fv(nj.px(), nj.py(), nj.pz(), nj.e());
		gp = new HepMC::GenParticle(fv, 747, 1);
		gv->add_particle_out(gp);
		jet_genparticles.push_back(gp);
	}
	new_evt->add_vertex(gv);

	// smear jets and add to new event
	if(detectorSimulation) smear_momenta(jet_genparticles, new_evt);

}



void Reweightor::trans_parton_shower(HepMC::GenParticle* old_in , HepMC::GenParticle* new_in, SLHAReader & new_slha, HepMC::GenEvent* new_evt){

	HepMC::GenVertex* gv = new HepMC::GenVertex();
	gv->add_particle_in(new_in);

	TLorentzVector diff = fv_to_lv(new_in->momentum()) - fv_to_lv(old_in->momentum());
	TVector3 b = boost_vector_between(fv_to_lv(old_in->momentum()), fv_to_lv(new_in->momentum()));

	std::vector<HepMC::GenParticle*> parton_shower_result;
	for(auto it : HepMC::GenVertexParticleRange(*(old_in->end_vertex()), HepMC::descendants)){
		if(it->end_vertex() != 0) continue;
		parton_shower_result.push_back(it);
	}

	sort(parton_shower_result.begin(), parton_shower_result.end(), [](HepMC::GenParticle* p1, HepMC::GenParticle* p2){
		return p1->momentum().e() > p2->momentum().e();
	});

	HepMC::GenParticle* gp;
	for(unsigned i=0; i<parton_shower_result.size(); i++){
		TLorentzVector o = fv_to_lv(parton_shower_result[i]->momentum());
		//if(i == 0) o += diff;
		o.Boost(b);
		HepMC::FourVector fv = lv_to_fv(o);
		gp = new HepMC::GenParticle(fv, parton_shower_result[i]->pdg_id(), 1);
		gv->add_particle_out(gp);
	}

	new_evt->add_vertex(gv);

}



void Reweightor::trans_decays(
	std::vector<HepMC::GenParticle*> boosted_final_state, 
	std::vector<HepMC::GenParticle*> new_boosted_final_state, 
	HepMC::GenEvent* new_evt, 
	SLHAReader & new_slha, 
	JetCandidateContainer & jet_candidates
){	


	stack<HepMC::GenVertex*> queue;
	std::map<int,int> jet_constituents_to_partons;
	fastjet::PseudoJet candidate;

	std::vector<fastjet::PseudoJet> ISR_jet_candidates = jet_candidates.candidates;

	for(unsigned i=0; i<boosted_final_state.size(); i++){
		if(boosted_final_state[i]->end_vertex() != 0) queue.push(boosted_final_state[i]->end_vertex());
	}

	HepMC::GenVertex *current_vertex, *gv;
	HepMC::GenParticle *old_in, *new_in, *gp;
	std::vector<HepMC::GenParticle*> new_out;
	
	while(!queue.empty()){
		current_vertex = queue.top();
		
		queue.pop();
		old_in = *(current_vertex->particles_begin(HepMC::parents));
		new_in = new_evt->barcode_to_particle(old_in->barcode());
		if(new_in==0) continue;

		/*
		if(old_in->pdg_id()==21 || is_quark(old_in->pdg_id())){
			for(auto child : HepMC::GenVertexParticleRange(*current_vertex, HepMC::descendants)){
				
				// status = 71 if hadronization is on else status = 1
				// @todo: should be recognized automatically or never allow hadronization ❤
				if(child->status() != 1) continue;

				jet_constituents_to_partons[child->barcode()] = old_in->barcode();
				if(in_list(jet_candidates.barcodes, child->barcode())) continue;

				candidate = fv_to_pj(child->momentum());
				candidate.set_user_index(child->barcode());
				jet_candidates.barcodes.push_back(child->barcode());
				jet_candidates.candidates.push_back(candidate);
			
			}
			continue;
		}
		*/

		if(current_vertex->particles_out_size() == 1){
			// 1 -> 1 vertex, i.e. only a boost of the particle
			// apply boost between old_in and old_out to new_in

			// debug("trans_1_to_1:");
			// debug(*old_in,"\t");

			TLorentzVector old_in_mom = fv_to_lv(old_in->momentum());
			HepMC::GenParticle* old_out = *(old_in->end_vertex()->particles_begin(HepMC::children));
			TLorentzVector old_out_mom = fv_to_lv(old_out->momentum());
			
			TLorentzVector new_out_mom = fv_to_lv(new_in->momentum());	

			TVector3 b = boost_vector_between(old_in_mom, old_out_mom);
			new_out_mom.Boost(b);

			gv = copy_genvertex(current_vertex);
			new_evt->add_vertex(gv);
			gv->add_particle_in(new_in);
			gp = new HepMC::GenParticle(lv_to_fv(new_out_mom), old_out->pdg_id(), old_out->status());
			gp->suggest_barcode(old_out->barcode());
			gv->add_particle_out(gp);

		
		}else if(current_vertex->particles_out_size() == 2 && old_in->momentum().m() > 0.001 && momentum_conserved(current_vertex)){
			// actual decay of massive particle

			// debug("trans_1_to_2:");
			// debug(*old_in,"\t");
			trans_1_to_2(old_in, new_in, new_slha, new_evt);

		}else if(current_vertex->particles_out_size() == 2){
			// parton shower branching

			// debug("trans_parton_shower");
			// debug(*old_in,"\t");
			trans_parton_shower(old_in, new_in, new_slha, new_evt);

		}else if(current_vertex->particles_out_size() > 2){
			
			// debug("trans_1_to_n");
			trans_1_to_n(old_in, new_in, new_slha, new_evt);
		
		}else{

			// debug("no condition");
			cout << current_vertex->particles_out_size() << ", " << momentum_conserved(current_vertex)
				 << ", " << old_in->momentum().m() << endl;
		}

		for(auto child : HepMC::GenVertexParticleRange(*current_vertex, HepMC::children)){
			if(child->end_vertex() != 0){
				queue.push(child->end_vertex());
			}else{
				if(detectorSimulation) smear_momenta({child}, new_evt);
			}
		}

	}

	/*
	// @todo: also transform if only ISR and no FSR
	if(jet_candidates.candidates.size() == ISR_jet_candidates.size()) return;

	double R = 0.7;
	JetDefinition jet_def(antikt_algorithm, R);
 
	// run the clustering, extract the jets
	ClusterSequence cs(jet_candidates.candidates, jet_def);
	std::vector<fastjet::PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());


	trans_jets(jets, jet_constituents_to_partons, ISR_jet_candidates, old_in->parent_event(), new_evt, detectorSimulation);
	*/

}





std::vector<std::pair<bool, std::pair<HepMC::GenEvent*,ReweightingProcInfo>>> Reweightor::reweight(HepMC::GenEvent* evt){
	
	JetCandidateContainer jet_candidates; // for later - filled in add_underlying_event and trans_decays; used in trans_decays and trans_jets
	std::map<HepMC::GenParticle*, std::vector<HepMC::GenParticle*>> merged_jets;


	// Step 1: Get Hard Event Data from Eventfile

	// define variables
	std::vector<HepMC::GenParticle*> initial_state, final_state; // hard initial and final state particles without parton shower effects
	std::vector<HepMC::GenParticle*> boosted_final_state; // hard initial and final state with parton shower effects
	HepMC::GenParticle* mediator; // sum of the initial (or final) state momenta without PS effects
	HepMC::GenParticle* boosted_mediator; // sum of the initial (of final) state momenta with PS effects
	TVector3 mediator_boostvector;
	TVector3 boosted_mediator_boostvector;
	HepMC::GenVertex* hard_vertex; // vertex where the collision happens (as produced by MadGraph)

	// get the vertex where the collision happens (always has barcode -3.. at least as long there is not ISR in the hard process definition..)
	hard_vertex = evt->barcode_to_vertex(-3);

	// get initial state from hepmc. boosted_initial_state would have to be created manually by boosting.
	for(auto it : HepMC::GenVertexParticleRange(*hard_vertex, HepMC::parents)){
		initial_state.push_back(it); // from hepmc
	}

 	if(initial_state[0]->pdg_id() != 21 && initial_state[0]->pdg_id() < 0){
 	  	initial_state = {initial_state[1], initial_state[0]};
 	}

	
	// get final states and mediators from hepmc.
	if(hard_vertex->particles_out_size() == 1){
		// s-channel mediator -> no LHE final state in hepmc file
		for(auto it : HepMC::GenVertexParticleRange(*hard_vertex, HepMC::children)){
			mediator = it;
		}
		boosted_mediator = last_status(mediator);
		for(auto it : HepMC::GenVertexParticleRange(*boosted_mediator->end_vertex(), HepMC::children)){
			boosted_final_state.push_back(last_status(it));
		}

	}else if(hard_vertex->particles_out_size() == 2){
		// no s-channel mediator -> LHE final state and PS final state in hepmc file
		for(auto it : HepMC::GenVertexParticleRange(*hard_vertex, HepMC::children)){
			final_state.push_back(it);
			boosted_final_state.push_back(last_status(it));
		}
		HepMC::FourVector v = lv_to_fv(add(final_state));
		mediator = new HepMC::GenParticle(v, 19, 22);
		v = lv_to_fv(add(boosted_final_state));
		boosted_mediator = new HepMC::GenParticle(v, 19, 62);
	
	}else{
	
		// There was some merging...
		for(auto it : HepMC::GenVertexParticleRange(*hard_vertex, HepMC::children)){
			if(is_quark(it->pdg_id()) || it->pdg_id() == 21){
				std::vector<HepMC::GenParticle*> leafs;
				for(auto it2 : HepMC::GenVertexParticleRange(*it->end_vertex(), HepMC::descendants)){
					if(it2->end_vertex() == 0) leafs.push_back(it2);
				}
				if(leafs.size()==0) leafs = {it};
				merged_jets[it] = leafs;
			}else{
				final_state.push_back(it);
				boosted_final_state.push_back(last_status(it));
			}
		}
		HepMC::FourVector v = lv_to_fv(add(final_state));
		mediator = new HepMC::GenParticle(v, 19, 22);
		v = lv_to_fv(add(boosted_final_state));
		boosted_mediator = new HepMC::GenParticle(v, 19, 62);		
	}


	if(final_state[0]->pdg_id() < 0){
		final_state = {final_state[1], final_state[0]};
	}
	 
	if(boosted_final_state[0]->pdg_id() < 0){
	 	boosted_final_state = {boosted_final_state[1], boosted_final_state[0]};
	}

	// create (pseudo-)mediator boost std::vectors
	mediator_boostvector = fv_to_lv(mediator->momentum()).BoostVector();
	boosted_mediator_boostvector = fv_to_lv(boosted_mediator->momentum()).BoostVector();


	// Step 2: Determine scaling factor and setup new GenEvent
	double total_final_state_mass = 0;
	
	for(HepMC::GenParticle* p : boosted_final_state){
		int pid = fabs(p->pdg_id());
		total_final_state_mass += slhaConfig["base"].get_block_entry("mass",pid);
	}
	

	int mediator_pid = mediator->pdg_id();
	int pid1 = fabs(boosted_final_state[0]->pdg_id());
	int pid2 = fabs(boosted_final_state[1]->pdg_id());
	double old_m1 = slhaConfig["base"].get_block_entry("mass",pid1);	
	double old_m2 = slhaConfig["base"].get_block_entry("mass",pid2);

	std::vector<std::pair<bool, std::pair<HepMC::GenEvent*,ReweightingProcInfo>>> ret;

	std::map<std::string,SLHAReader>::iterator slhaIterator;
	for(slhaIterator=slhaConfig.begin(); slhaIterator!=slhaConfig.end(); slhaIterator++){

		if(slhaIterator->first == "base") continue;
		SLHAReader& slha = slhaIterator->second;


		bool returnStatus = true;

		double new_m1 = slha.get_block_entry("mass",pid1);	
		double new_m2 = slha.get_block_entry("mass",pid2);

		double a;
		if(total_final_state_mass > 0){
			// at least one massive particle in a multi particle final state
			// use on-shell masses, so no jacobian necessary
			a = (new_m1+new_m2)/(old_m1+old_m2);
		}else if(slha.has_block_entry("masses",mediator_pid)){
			a = slha.get_block_entry("masses",mediator_pid)/slhaConfig["base"].get_block_entry("masses",mediator_pid);
		}else{
			// final state is completely massles and no s-channel mediator
			// TODO: One could take the invariant mass of the final state as scale
			std::stringstream msg;
			msg << "Final state is completely massless. This case is currently not supported by the Reweighting algorithm.";
			Global::warn("Reweightor",msg.str());
			returnStatus = false;
		}


		HepMC::GenEvent* newEvt = new HepMC::GenEvent(evt->signal_process_id(), evt->event_number()); // the new event that we want to return
		newEvt->use_units(HepMC::Units::GEV, HepMC::Units::MM);
		newEvt->set_event_scale(a*evt->event_scale()); // TODO: Check whether multiplying by a here is a good thing..
		newEvt->set_alphaQCD(evt->alphaQCD());
		newEvt->set_alphaQED(evt->alphaQED());

		std::map<int,int> interface;
		bool has_parton_shower = false;
		if(evt->valid_beam_particles()){
			has_parton_shower = true;
			interface = add_underlying_event(newEvt,evt,jet_candidates); // add everything that comes before the hard vertex. interface tells us where we have to add the hard event.
		}


		// Step 3: Transform the Hard Event
		
		double x1 = initial_state[0]->momentum().e() / 6500.;
		double x2 = initial_state[1]->momentum().e() / 6500.;
		

		double x1_new = a * x1;
		double x2_new = a * x2;
		double jacobianWeight = a*a;

		if(x1_new > 1 || x2_new>1){
			x1_new = 0.99;
			x2_new = 0.99;
			jacobianWeight = 0;	
		}


		double shat_old = x1*x2*pow(13000.,2);
		double shat_new = x1_new*x2_new*pow(13000.,2);


		// define variables
		std::vector<HepMC::GenParticle*> new_initial_state, new_final_state; // new hard initial and final state particles without parton shower effects
		std::vector<HepMC::GenParticle*> new_boosted_initial_state, new_boosted_final_state; // new hard initial and final state with parton shower effects
		HepMC::GenParticle* new_mediator; // new sum of the initial (or final) state momenta without PS effects
		HepMC::GenParticle* new_boosted_mediator; // new sum of the initial (of final) state momenta with PS effects
		TVector3 new_mediator_boostvector;
		TVector3 new_boosted_mediator_boostvector;


		// new initial state
		HepMC::GenParticle* new_isp;
		HepMC::FourVector v;
		
		new_isp = copy_genparticle(initial_state[0]);
		v = HepMC::FourVector(0, 0, x1_new/x1*initial_state[0]->momentum().pz(), x1_new/x1*initial_state[0]->momentum().e());
		new_isp->set_momentum(v);
		new_initial_state.push_back(new_isp);
		
		new_isp = copy_genparticle(initial_state[1]);
		v = HepMC::FourVector(0, 0, x2_new/x2*initial_state[1]->momentum().pz(), x2_new/x2*initial_state[1]->momentum().e());
		new_isp->set_momentum(v);
		new_initial_state.push_back(new_isp);


		// new (pseudo-)mediators and their boost std::vectors
		new_mediator = copy_genparticle(mediator);
		v = lv_to_fv(add(new_initial_state));
		new_mediator->set_momentum(v);

		new_boosted_mediator = copy_genparticle(boosted_mediator);
		TLorentzVector tlv; tlv.SetXYZM(0, // keep transverse ...
									 	0, // ... momentum constant (error source!)
									 	v.pz()+boosted_mediator->momentum().pz()-mediator->momentum().pz(), // add the initial state longitudinal momentum from the parton shower
									 	v.m());
		v = lv_to_fv(tlv);
		new_boosted_mediator->set_momentum(v);

		new_mediator_boostvector = fv_to_lv(new_mediator->momentum()).BoostVector();
		new_boosted_mediator_boostvector = fv_to_lv(new_boosted_mediator->momentum()).BoostVector();


		// new final state
		new_boosted_final_state = trans(boosted_final_state, a, boosted_mediator_boostvector, new_boosted_mediator_boostvector, slhaConfig["base"], slha);

		
		// add new particles to new event
		HepMC::GenVertex* new_hard_vertex = copy_genvertex(hard_vertex);
		newEvt->add_vertex(new_hard_vertex);

		for(HepMC::GenParticle* p : new_initial_state){
			HepMC::GenVertex* production_vertex;
			if(has_parton_shower){
				production_vertex = newEvt->barcode_to_vertex(interface[p->barcode()]);
			}else{
				production_vertex = new HepMC::GenVertex();
				newEvt->add_vertex(production_vertex);
			}
			production_vertex->add_particle_out(p);
			new_hard_vertex->add_particle_in(p);
		}


		for(auto it=merged_jets.begin(); it!=merged_jets.end(); it++){
			for(HepMC::GenParticle* gp : it->second){
				HepMC::GenParticle* new_gp = copy_genparticle(gp);
				new_hard_vertex->add_particle_out(new_gp);
			}
		}

		new_hard_vertex->add_particle_out(new_mediator);
		HepMC::GenVertex* gv;
		if(mediator->end_vertex()){
			gv = copy_genvertex(mediator->end_vertex());	
		}else{
			gv = new HepMC::GenVertex();
		}
		gv->add_particle_in(new_mediator);
		gv->add_particle_out(new_boosted_mediator);
		newEvt->add_vertex(gv);

		if(boosted_mediator->end_vertex()){
			gv = copy_genvertex(boosted_mediator->end_vertex());	
		}else{
			gv = new HepMC::GenVertex();
		}
		gv->add_particle_in(new_boosted_mediator);
		newEvt->add_vertex(gv);

		for(HepMC::GenParticle* p : new_boosted_final_state){
			gv->add_particle_out(p);
		}


		// Step 4: Decays and detector smearing
		trans_decays(boosted_final_state, new_boosted_final_state, newEvt, slha, jet_candidates);

		HepMC::GenParticle *old_n11, *new_n11, *old_n12, *new_n12;
		std::vector<fastjet::PseudoJet> old_jet_candidates, old_jets;
		std::vector<fastjet::PseudoJet> new_jet_candidates, new_jets;
		for(auto it : HepMC::GenVertexParticleRange(*boosted_final_state[0]->end_vertex(), HepMC::descendants)){
			if(it->pdg_id() == 1000022 && it->status() == 1){
				old_n11 = last_status(it);
			}
			if(it->status() == 1 && (it->pdg_id() == 21 || is_quark(it->pdg_id()))){
				old_jet_candidates.push_back(fv_to_pj(it->momentum()));
			}
		}
		for(auto it : HepMC::GenVertexParticleRange(*boosted_final_state[1]->end_vertex(), HepMC::descendants)){
			if(it->pdg_id() == 1000022 && it->status() == 1){
				old_n12 = last_status(it);
			}
			if(it->status() == 1 && (it->pdg_id() == 21 || is_quark(it->pdg_id()))){
				old_jet_candidates.push_back(fv_to_pj(it->momentum()));
			}
		}

		for(auto it=merged_jets.begin(); it!=merged_jets.end(); it++){
			for(HepMC::GenParticle* gp : it->second){
				old_jet_candidates.push_back(fv_to_pj(gp->momentum()));
			}
		}

		double R = 0.7;
		fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R);
		fastjet::ClusterSequence cs_old(old_jet_candidates, jet_def);
		old_jets = sorted_by_E(cs_old.inclusive_jets());

		for(auto it : HepMC::GenVertexParticleRange(*new_boosted_final_state[0]->end_vertex(), HepMC::descendants)){
			if(it->pdg_id() == 1000022 && it->status() == 1){
				new_n11 = it;
			}
			if(it->status() == 1 && (it->pdg_id() == 21 || is_quark(it->pdg_id()))){
				new_jet_candidates.push_back(fv_to_pj(it->momentum()));
			}
		}
		for(auto it : HepMC::GenVertexParticleRange(*new_boosted_final_state[1]->end_vertex(), HepMC::descendants)){
			if(it->pdg_id() == 1000022 && it->status() == 1){
				new_n12 = it;
			}
			if(it->status() == 1 && (it->pdg_id() == 21 || is_quark(it->pdg_id()))){
				new_jet_candidates.push_back(fv_to_pj(it->momentum()));
			}
		}
		
		for(auto it : HepMC::GenVertexParticleRange(*new_hard_vertex, HepMC::children)){
			if(it->pdg_id() == 21 || is_quark(it->pdg_id())){
				new_jet_candidates.push_back(fv_to_pj(it->momentum()));
			}
		}

		fastjet::ClusterSequence cs_new(new_jet_candidates, jet_def);
		new_jets = sorted_by_pt(cs_new.inclusive_jets());


		// Step 5: Reweighting
		double scale = slha.get_block_entry("mass",23);
		double pdfWeight = pdfs(new_initial_state,scale) / pdfs(initial_state,scale);
		double psWeight = 1.; // phase_space(new_boosted_final_state,new_masses) / phase_space(boosted_final_state,old_masses);
		double fluxWeight = mediator->momentum().m2()/new_mediator->momentum().m2();
		double meWeight = 1.;


		if(matrixElementReweighting){
			double meOld = mesOld->calc(initial_state, boosted_final_state);
			double meNew = mesNewMap[slhaIterator->first]->calc(new_initial_state, new_boosted_final_state);
			meWeight = meNew/meOld;
		}

		double newWeight = jacobianWeight * fluxWeight * pdfWeight * psWeight * meWeight;

		HepMC::WeightContainer & wc = newEvt->weights(); // get reference to our (empty) WeightContainer
		wc = evt->weights(); // copy all weights from the original events in out WeightContainer
		wc.push_back(newWeight); // add our new weight to the original weights

		// Finish up
		if(merged_jets.size() > 0){
			for(auto it=merged_jets.begin(); it!=merged_jets.end(); it++){
				HepMC::GenParticle* gp = copy_genparticle(it->first);
				new_hard_vertex->add_particle_out(gp);
				HepMC::GenVertex* gv = new HepMC::GenVertex();
				newEvt->add_vertex(gv);
				gv->add_particle_in(gp);
				for(HepMC::GenParticle* leaf : it->second){
					gp = copy_genparticle(leaf);
					gv->add_particle_out(gp);
				}
			}
		}


		if(has_parton_shower){
			HepMC::PdfInfo* old_pdfinfo = evt->pdf_info();
			HepMC::PdfInfo new_pdfinfo(old_pdfinfo->id1(),
								old_pdfinfo->id2(),
								a*old_pdfinfo->x1(),
								a*old_pdfinfo->x2(),
								a*old_pdfinfo->scalePDF(),
								0,0);
			newEvt->set_pdf_info(new_pdfinfo);
		}

		newEvt->set_signal_process_vertex(new_hard_vertex);

		ReweightingProcInfo newProcInfo;
		newProcInfo.id1 = initial_state[0]->pdg_id();
		newProcInfo.id2 = initial_state[1]->pdg_id();
		newProcInfo.x1 = initial_state[0]->momentum().e()/6500.;
		newProcInfo.x2 = initial_state[1]->momentum().e()/6500.;
		newProcInfo.scalePDF = scale;
		newProcInfo.pdf1 = pdf->xfxQ(newProcInfo.id1, newProcInfo.x1, scale)/newProcInfo.x1;
		newProcInfo.pdf2 = pdf->xfxQ(newProcInfo.id2, newProcInfo.x2, scale)/newProcInfo.x2;

		std::pair<HepMC::GenEvent*,ReweightingProcInfo> evenrPair = std::make_pair(newEvt,newProcInfo);
		std::pair<bool, std::pair<HepMC::GenEvent*,ReweightingProcInfo>> returnPair = std::make_pair(returnStatus,evenrPair);

		ret.push_back(returnPair);
	}

	return ret;

}

