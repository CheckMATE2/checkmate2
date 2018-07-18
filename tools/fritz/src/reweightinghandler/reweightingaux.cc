#include "reweightingaux.h"
#include "Global.h"
#include "HepMC/GenRanges.h"

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "Math/RootFinder.h"
#include "TVector3.h"

#include <stack>
#include <utility>
#include <vector>
#include <cmath>



std::vector<int> squark_ids = {1000001,1000002,1000003,1000004,2000001,2000002,2000003,2000004};
std::vector<int> quark_ids = {1,2,3,4};

bool is_quark(const int & pdg_id){
	int pid = fabs(pdg_id);
	return in_list(quark_ids, pid);
}

bool is_squark(const int & pdg_id){
	int pid = fabs(pdg_id);
	return in_list(squark_ids, pid);
}

std::string particle_names(const int & pdg_id){
	if(pdg_id ==   6) return "top";
	if(pdg_id ==  -6) return "anti-top";
	if(pdg_id ==   5) return "bottom";
	if(pdg_id ==  -5) return "anti-bottom";
	if(pdg_id ==  24) return "W+";
	if(pdg_id == -24) return "W-";
	if(pdg_id ==  11) return "e+";
	if(pdg_id == -11) return "e-";
	if(pdg_id ==  13) return "mu+";
	if(pdg_id == -13) return "mu-";
	if(pdg_id ==  15) return "tau+";
	if(pdg_id == -15) return "tau-";
	if(pdg_id == 1000021) return "go";
	if(pdg_id == 1000022) return "n1";
	if(is_squark(pdg_id)) return "sq";

	return "";
}

int particle_ids(const std::string & name){
	if(name.c_str() == "q") return quark_ids[0];
	if(name.c_str() == "t") return 6;
	if(name.c_str() == "tbar") return -6;
	if(name.c_str() == "b") return 5;
	if(name.c_str() == "bbar") return -5;
	if(name.c_str() == "g") return 21;
	if(name.c_str() == "W+") return 24;
	if(name.c_str() == "W-") return -24;
	if(name.c_str() == "sq") return squark_ids[0];
	if(name.c_str() == "go") return 1000021;
	if(name.c_str() == "n1") return 1000022;

	return 0;
}


std::map<int,std::vector<HepMC::GenParticle*>> get_particles(HepMC::GenEvent* evt, std::vector<int> pdg_ids){
	std::map<int,std::vector<HepMC::GenParticle*>> ret;
	std::vector<fastjet::PseudoJet> jet_candidates;

	std::stack<HepMC::GenVertex*> queue;
	HepMC::GenVertex* current_vertex;
	HepMC::GenParticle* current_particle;

	// only return particles and jets that originate from the hard process
	HepMC::GenVertex* hard_vertex = evt->barcode_to_vertex(-3);
	queue.push(hard_vertex);

	std::vector<HepMC::GenParticle*> visited;


	while(!queue.empty()){
		
		current_vertex = queue.top();
		queue.pop();
		
		for(auto child : HepMC::GenVertexParticleRange(*current_vertex, HepMC::children)){

			current_particle = last_status(child);
			
			if(in_list(visited, current_particle)) continue; 
			visited.push_back(current_particle);

			int pid = fabs(current_particle->pdg_id());

			// catches all particles and all jets from new event
			if(in_list(pdg_ids, pid)){
				ret[pid].push_back(current_particle);	
			}

			if(current_particle->pdg_id() != 21 && !is_quark(current_particle->pdg_id())){
				if(current_particle->end_vertex() == 0) continue;
				queue.push(current_particle->end_vertex()); continue;
			}
			
			// found a quark or gluon -> handle attached jet if desired
			if(!in_list(pdg_ids, 747)) continue;

			if(current_particle->end_vertex() == 0){
				// no hadronization ( or even no parton shower ) -> take parton as jet (no jet alg)
				ret[747].push_back(current_particle);
				continue;
			}

			for(auto it2 : HepMC::GenVertexParticleRange(*(child->end_vertex()), HepMC::descendants)){
				if(it2->status() != 1) continue;
				if(in_list(visited, it2)) continue;
				visited.push_back(it2);
				
				if(it2->pdg_id() == 747){
					// new event (with hadronization) has jets with pdg id 747
					ret[747].push_back(it2);
				}else{
					// old event (with hadronization) does not. Run jet alg again
					fastjet::PseudoJet pj = fv_to_pj(it2->momentum());
					pj.set_user_index(it2->barcode());
					jet_candidates.push_back(pj);
				}
			}
			continue;
	
		}

	}

	if(!in_list(pdg_ids, 747) || ret[747].size() > 0){
		return ret;
	}


	// old event does not have jets with pdg_id 747 -> run jet algorithm and return result

	double R = 0.7;
	fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R);

	// run the clustering, extract the jets
	fastjet::ClusterSequence cs(jet_candidates, jet_def);
	std::vector<fastjet::PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());
	for(fastjet::PseudoJet j : jets){
		HepMC::FourVector fv = pj_to_fv(j);
		ret[747].push_back(new HepMC::GenParticle(fv, 747, 1));
	}

	return ret;
}


HepMC::GenParticle* last_status(HepMC::GenParticle* p){
	if(p->end_vertex()==0) return p;

	int pid = p->pdg_id();
	HepMC::GenVertex* end_vertex = p->end_vertex();
	
	HepMC::GenParticle* q = p;
	while(true){
		if(end_vertex->particles_out_size() > 1) break;
		if(end_vertex->particles_in_size() > 1) break;

		for(auto next : HepMC::GenVertexParticleRange(*end_vertex, HepMC::children)){
			if(next->pdg_id() == q->pdg_id()) q = next;
		}
		if(q->end_vertex()==0) break;
		if(end_vertex == q->end_vertex()) break;
		end_vertex = q->end_vertex();
	}
	return q;
}



TLorentzVector add(const std::vector<HepMC::GenParticle*> & particles){
	TLorentzVector v;
	double x,y,z,e;
	x=0.;y=0.;z=0.;e=0.;
	for(HepMC::GenParticle* p : particles){
		x += p->momentum().px();
		y += p->momentum().py();
		z += p->momentum().pz();
		e += p->momentum().e();
	}
	v.SetXYZT(x,y,z,e);
	return v;
}

TLorentzVector add(const std::vector<TLorentzVector> & momenta){
	TLorentzVector v;
	for(TLorentzVector tlv : momenta){
		v += tlv;
	}
	return v;	
}

TLorentzVector add(const std::vector<fastjet::PseudoJet> & momenta){
	TLorentzVector v;
	double x,y,z,e;
	for(fastjet::PseudoJet pj : momenta){
		x += pj.px();
		y += pj.py();
		z += pj.pz();
		e += pj.e();
	}
	v.SetXYZT(x,y,z,e);
	return v;	
}

double sp(const TLorentzVector & p1, const TLorentzVector & p2){
	double ret = p1.E()*p2.E() - (p1.Px()*p2.Px()+p1.Py()*p2.Py()+p1.Pz()*p2.Pz());
	return ret;
}

fastjet::PseudoJet fv_to_pj(const HepMC::FourVector & fv){
	fastjet::PseudoJet ret(fv.px(), fv.py(), fv.pz(), fv.e());
	return ret;
}

HepMC::FourVector pj_to_fv(const fastjet::PseudoJet & pj){
	HepMC::FourVector ret(pj.px(), pj.py(), pj.pz(), pj.e());
	return ret;
}

TLorentzVector pj_to_lv(const fastjet::PseudoJet & pj){
	TLorentzVector ret;
	ret.SetPx(pj.px());
	ret.SetPy(pj.py());
	ret.SetPz(pj.pz());
	ret.SetE(pj.e());
	return ret;
}


TLorentzVector fv_to_lv(const HepMC::FourVector & fv){
	TLorentzVector ret;
	ret.SetPx(fv.px());
	ret.SetPy(fv.py());
	ret.SetPz(fv.pz());
	ret.SetE(fv.e());
	return ret;
}

HepMC::FourVector lv_to_fv(const TLorentzVector & lv){
	HepMC::FourVector ret;
	ret.setPx(lv.Px());
	ret.setPy(lv.Py());
	ret.setPz(lv.Pz());
	ret.setE(lv.E());
	return ret;
}

TVector3 neg(const TVector3 & v){
	TVector3 ret;
	ret.SetX(-v.X());
	ret.SetY(-v.Y());
	ret.SetZ(-v.Z());
	return ret;
}

bool momentum_conserved(HepMC::GenVertex* v){
	std::vector<HepMC::GenParticle*> in,out;
	for(auto it : HepMC::GenVertexParticleRange(*v, HepMC::parents)){
		in.push_back(it);
	}
	TLorentzVector total_in = add(in);

	for(auto it : HepMC::GenVertexParticleRange(*v, HepMC::children)){
		out.push_back(it);
	}
	TLorentzVector total_out = add(out);

	double p = (total_out-total_in).P();
	double e = (total_out-total_in).E();
	if(sqrt(p*p+e*e)<0.00001) return true; // arbitrary choice.. 
	return false;
}

HepMC::GenParticle* copy_genparticle(HepMC::GenParticle* p){
	HepMC::FourVector momentum(p->momentum().px(),
						p->momentum().py(),
						p->momentum().pz(),
						p->momentum().e());
	HepMC::GenParticle* ret = new HepMC::GenParticle(momentum,p->pdg_id(),p->status());
	ret->suggest_barcode(p->barcode());
	
	return ret;
}

HepMC::GenVertex* copy_genvertex(HepMC::GenVertex* v){
	HepMC::FourVector position(v->position().x(),
						v->position().y(),
						v->position().z(),
						v->position().t());
	HepMC::GenVertex* ret = new HepMC::GenVertex(position,v->id(),v->weights());
	ret->suggest_barcode(v->barcode());
	
	return ret;
}



std::vector<TLorentzVector> boost_to_restframe(HepMC::GenParticle* p, const std::vector<HepMC::GenParticle*> & rest){
	std::vector<TLorentzVector> ret;

	TVector3 boostvector = fv_to_lv(p->momentum()).BoostVector();
	for(HepMC::GenParticle* gp : rest){
		TLorentzVector tlv = fv_to_lv(gp->momentum());
		tlv.Boost(neg(boostvector));
		ret.push_back(tlv);
	}

	return ret;
}


double kaellen(const double & a, const double & b, const double & c){
	double lambda2 = pow(a-b-c,2) - 4*b*c;
	if (lambda2 < 0) return 0.;
	double lambda = sqrt(lambda2);
	if (lambda < 1e-15) return 0.;
	return lambda;
}


void print_pseudojet(const fastjet::PseudoJet & pj){
	if(pj.user_index() != 0){
		Global::print("Reweightor", Global::intToStr(pj.user_index()));
	}
	std::stringstream msg;
	msg << "(" << pj.px() << ", " << pj.py() << ", " << pj.pz() << ", " << pj.e() << ")";
	Global::print("Reweightor",msg.str());
}


TVector3 boost_vector_between(const TLorentzVector & p, const TLorentzVector & q){
	TVector3 delta = (q-p).Vect();
	double n = delta.Mag();
	double beta = 2*n*(p.E()+q.E())/(n*n + pow(p.E()+q.E(),2));
	TVector3 ret(
		beta/n * delta.X(),
		beta/n * delta.Y(),
		beta/n * delta.Z()
	);
	return ret;
}



/* numerics */


double func_rootfinder::E = 0;
std::vector<double> func_rootfinder::mass;
std::vector<double> func_rootfinder::p_frac;

double func_rootfinder::func_for_root(double p1){
	double h = 0;
	for(int i = 0; i < mass.size(); i++){
		h +=sqrt(p_frac[i]*p_frac[i]*p1*p1+mass[i]*mass[i]);
	}
	return E-h;
}
	
void func_rootfinder::set(double E,const std::vector<double>& mass, const std::vector<double>& p_frac){
	this->E=E;
	this->mass=mass;
	this->p_frac=p_frac;
}

double func_rootfinder::get_root(){
	ROOT::Math::Functor1D f(&(this->func_for_root)); 
	ROOT::Math::RootFinder r_finder(ROOT::Math::RootFinder::kGSL_BRENT);
	r_finder.SetFunction(f, 0,E+10. ); //Set start intervall for the positive energy solution 
	r_finder.Solve(1000);
	return r_finder.Root();
	
}



std::vector<TVector3> Mini::orig;
std::vector<std::vector<double> > Mini::sol;
double Mini::new_inv_mass = 0;
std::vector<double> Mini::dec_par_vec_mass;

double Mini::foo(const double *xx){
	
	double ret = 0;
	double p_orig_tot = 0;
	double p_new_tot = 0;
	std::vector<double> frac;
	
	//calculate current momentum fractions
	frac.push_back(1.);
	for(int i = 0; i<3;i++){
		double f = 0;
		for(int l = 0; l<sol[0].size()-1; l++){
				f+=xx[l]*(-sol[i][l]);
		}
		f+=sol[i][sol[i].size()-1];
		frac.push_back(f);
	}
	for(int i = 0;i<sol[0].size()-1;i++){
		frac.push_back(xx[i]); 
	}
	
	
	
	func_rootfinder finder;
	finder.set(new_inv_mass,dec_par_vec_mass,frac);
	double abs_mom1 = finder.get_root();
	
	//calculate sum that we want to minimize
	for(int i = 0; i<orig.size();i++){
		p_orig_tot+=orig[i].Mag();
		p_new_tot +=abs_mom1*frac[i]; 
	}
	
	for(int i = 0; i<orig.size();i++){
		
		ret+=(abs_mom1*frac[i]/p_new_tot-orig[i].Mag()/p_orig_tot)*(abs_mom1*frac[i]/p_new_tot-orig[i].Mag()/p_orig_tot);
		
		}
	
	return ret;
	
}

std::vector<double> Mini::minimize(
	const std::vector<TVector3> & orig,
	const std::vector<std::vector<double> > & sol,
	const double & new_inv_mass,
	const std::vector<double> & dec_par_vec_mass
){
	
	this->orig = orig;
	this->sol = sol;
	this->new_inv_mass = new_inv_mass;
	this->dec_par_vec_mass = dec_par_vec_mass;

	ROOT::Math::Minimizer* min = ROOT::Math::Factory::CreateMinimizer("GSLMultiMin", "BFGS");
	min->SetMaxFunctionCalls(1000000);
    min->SetMaxIterations(100000);
	min->SetTolerance(0.005);
	
	ROOT::Math::Functor f(&(this->foo),sol[0].size()-1); 
	min->SetFunction(f);
	double* step = new double[sol[0].size()-1];
	double* variable = new double[sol[0].size()-1];
	for(int i = 0; i < sol[0].size()-1; i++){
		step[i] = 0.25;
		variable[i] = orig[i+4].Mag()/orig[0].Mag();
		min->SetLowerLimitedVariable(i,"x",variable[i], step[i],0.);//Momentum fraction is always postive
	}
	
	min->Minimize();
	
	std::vector<double> frac;
	frac.push_back(1.);
	for(int i = 0; i<3;i++){
		double f = 0;
		for(int l = 0; l<sol[0].size()-1; l++){
				f+=(min->X())[l]*(-sol[i][l]);
		}
		f+=sol[i][sol[i].size()-1];
		frac.push_back(f);	
	}
	for(int i = 0;i<sol[0].size()-1;i++){
		frac.push_back((min->X())[i]);
	}
	
	delete [] step;
	delete [] variable;
	return frac;
}



bool gauss_alg(std::vector<std::vector<double> > & matrix){
	
	//nxm Matrix
	int n = matrix.size();//rows
	int m = matrix[0].size();//columns
	
	if(m<n){return false;}
	
	for(int i = 0; i<n; i++){//rows
		int k = i; // columns
		
		//Search for biggest value in the column
		int biggest =i;
		for(int j = i; j < n; j++){
			if( matrix[j][k] > matrix[biggest][k]){
				biggest = j;
			}
		}
		
		if(abs(matrix[biggest][k]) < 0.000001) {return false;} //check whether the biggest element is zero
		
		//Swap to row with biggest element in the column
		matrix[i].swap(matrix[biggest]);
		
		//Divide row by the first element in the row
		double first_element = matrix[i][k];
		for(int l = k; l<m; l++){
			matrix[i][l] /= first_element;
			
		}
		
		//Substract row from other rows
		for(int j = i+1; j < n; j++){
			first_element = matrix[j][k];
			for(int l = k; l<m; l++){
				matrix[j][l] -= first_element*matrix[i][l];
			}
		}
	
	}
	
	for(int i = n-1; i>0;i--){
		for(int j = i-1; j>=0;j--){
			double first_element = matrix[j][i];
			for(int k = i; k <m;k++){
				matrix[j][k] -=first_element*matrix[i][k];
			}
		}
	}
	
	return true;
}