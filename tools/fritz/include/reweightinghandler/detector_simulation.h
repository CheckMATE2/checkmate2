#include <cfloat>
#include <limits>
#include "BinnedFn.h"
#include <cmath>

using namespace std;


/* TAKEN FROM GAMBIT */


template <typename Momentum>
inline vector<Momentum> smear_electrons(const vector<Momentum> & electrons){
	random_device rd;
	mt19937 gen(rd());

	static HEPUtils::BinnedFn2D<double> coeffE2({{0, 2.5, 3., 5.}}, //< |eta|
	                                          {{0, 0.1, 25., DBL_MAX}}, //< pT
	                                          {{0.,          0.015*0.015, 0.005*0.005,
	                                            0.005*0.005, 0.005*0.005, 0.005*0.005,
	                                            0.107*0.107, 0.107*0.107, 0.107*0.107}});

	static HEPUtils::BinnedFn2D<double> coeffE({{0, 2.5, 3., 5.}}, //< |eta|
	                                         {{0, 0.1, 25., DBL_MAX}}, //< pT
	                                         {{0.,        0.,        0.05*0.05,
	                                           0.05*0.05, 0.05*0.05, 0.05*0.05,
	                                           2.08*2.08, 2.08*2.08, 2.08*2.08}});

	static HEPUtils::BinnedFn2D<double> coeffC({{0, 2.5, 3., 5.}}, //< |eta|
	                                         {{0, 0.1, 25., DBL_MAX}}, //< pT
	                                         {{0.,       0.,       0.25*0.25,
	                                           0.25*0.25,0.25*0.25,0.25*0.25,
	                                           0.,       0.,       0.}});

	vector<Momentum> new_electrons;

	// Now loop over the electrons and smear the 4-vectors
	for (Momentum e : electrons) {
		double abseta = fabs(e.eta());
		if (abseta > 5) continue;

		// Look up / calculate resolution
		const double c1 = coeffE2.get_at(abseta, e.perp());
		const double c2 = coeffE.get_at(abseta, e.perp());
		const double c3 = coeffC.get_at(abseta, e.perp());
		const double resolution = sqrt(c1*e.e()*e.e() + c2*e.e() + c3);

		// Smear by a Gaussian centered on the current energy, with width given by the resolution
		normal_distribution<> d(e.e(), resolution);
		double smeared_E = d(gen);
		if (smeared_E < 0) smeared_E = 0;
		// double smeared_pt = smeared_E/cosh(e.eta()); ///< @todo Should be cosh(|eta|)?
		// cout << "BEFORE eta " << electron.eta() << endl;
		Momentum new_momentum(e.px(), e.py(), e.pz(), smeared_E);
		new_electrons.push_back(new_momentum);
		// e.set_mom(HEPUtils::P4::mkEtaPhiME(e.eta(), e.phi(), e.mass(), smeared_E));
		// cout << "AFTER eta " << electron.eta() << endl;
	}

	return new_electrons;
}


/// Randomly smear the supplied muons' momenta by parameterised resolutions
template <typename Momentum>
inline vector<Momentum> smear_muons(const vector<Momentum> & muons){  // Function that mimics the DELPHES muon momentum resolution
	// We need to smear pT, then recalculate E, then reset 4 vector

	random_device rd;
	mt19937 gen(rd());

	static HEPUtils::BinnedFn2D<double> _muEff({{0,1.5,2.5}},
	                                         {{0,0.1,1.,10.,200.,DBL_MAX}},
	                                         {{0.,0.03,0.02,0.03,0.05,
	                                           0.,0.04,0.03,0.04,0.05}});

	vector<Momentum> new_muons;
	// Now loop over the muons and smear the 4-vectors
	for (Momentum mu : muons) {
		double abseta = fabs(mu.eta());
		if (abseta > 2.5) continue;

		// Look up resolution
		const double resolution = _muEff.get_at(abseta, mu.perp());

		// Smear by a Gaussian centered on the current energy, with width given by the resolution
		normal_distribution<> d(mu.perp(), resolution*mu.perp());
		double smeared_pt = d(gen);
		if (smeared_pt < 0) smeared_pt = 0;
		// const double smeared_E = smeared_pt*cosh(mu.eta()); ///< @todo Should be cosh(|eta|)?
		// cout << "Muon pt " << mu_pt << " smeared " << smeared_pt << endl;
		double theta = 2*atan(exp(-mu.eta()));
		double sin_theta = sin(theta);
		double cos_theta = cos(theta);
		double r = smeared_pt/sin_theta;
		Momentum new_momentum(
			r*cos(mu.phi())*sin_theta, 
			r*sin(mu.phi())*sin_theta, 
			r*cos_theta, 
			sqrt(r*r + mu.m()*mu.m())
		);
		//mu.set_mom(HEPUtils::P4::mkEtaPhiMPt(mu.eta(), mu.phi(), mu.mass(), smeared_pt));
		new_muons.push_back(new_momentum);
	}

	return new_muons;
}



/// Randomly smear the supplied taus' momenta by parameterised resolutions
template <typename Momentum>
inline vector<Momentum> smear_taus(const vector<Momentum> & taus) {
  // We need to smear pT, then recalculate E, then reset the 4-vector.
  // Same as for jets, but on a vector of particles. (?)

  // Const resolution for now
  const double resolution = 0.03;

  // Now loop over the jets and smear the 4-vectors
  random_device rd;
  mt19937 gen(rd());
  normal_distribution<> d(1., resolution);

  vector<Momentum> new_taus;
  for (Momentum tau : taus) {
    // Smear by a Gaussian centered on 1 with width given by the (fractional) resolution
    double smear_factor = d(gen);
    /// @todo Is this the best way to smear? Should we preserve the mean jet energy, or pT, or direction?
    double new_px = smear_factor*tau.px();
    double new_py = smear_factor*tau.py();
    double new_pz = smear_factor*tau.pz();
    double new_r2 = new_px*new_px + new_py*new_py + new_pz*new_pz;
    double new_energy = sqrt(tau.m2() + new_r2);
    Momentum new_momentum(new_px, new_py, new_pz, new_energy);
    new_taus.push_back(new_momentum);
    // p->set_mom(HEPUtils::P4::mkXYZM(p->mom().px()*smear_factor, p->mom().py()*smear_factor, p->mom().pz()*smear_factor, p->mass()));
  }

  return new_taus;
}


/// Randomly smear the supplied jets' momenta by parameterised resolutions
template <typename Momentum>
inline vector<Momentum> smear_jets(const vector<Momentum> & jets) {
  // Function that mimics the DELPHES jet momentum resolution.
  // We need to smear pT, then recalculate E, then reset the 4-vector.

  // Const resolution for now
  const double resolution = 0.03;

  // Now loop over the jets and smear the 4-vectors
  random_device rd;
  mt19937 gen(rd());
  normal_distribution<> d(1., resolution);

  vector<Momentum> new_jets;
  for (Momentum jet : jets) {
    // Smear by a Gaussian centered on 1 with width given by the (fractional) resolution
    double smear_factor = d(gen);
    /// @todo Is this the best way to smear? Should we preserve the mean jet energy, or pT, or direction?
    double new_px = smear_factor*jet.px();
    double new_py = smear_factor*jet.py();
    double new_pz = smear_factor*jet.pz();
    double new_r2 = new_px*new_px + new_py*new_py + new_pz*new_pz;
    double new_energy = sqrt(jet.m2() + new_r2);
    Momentum new_momentum(new_px, new_py, new_pz, new_energy);
    new_jets.push_back(new_momentum);
    // jet->set_mom(HEPUtils::P4::mkXYZM(jet->mom().px()*smear_factor, jet->mom().py()*smear_factor, jet->mom().pz()*smear_factor, jet->mass()));
  }

  return new_jets;
}




