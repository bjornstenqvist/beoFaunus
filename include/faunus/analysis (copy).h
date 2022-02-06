#ifndef FAUNUS_ANALYSIS_H
#define FAUNUS_ANALYSIS_H

#include <faunus/common.h>
#include <faunus/average.h>
#include <faunus/physconst.h>
#include <faunus/group.h>
#include <faunus/space.h>
#include <faunus/point.h>
#include <faunus/textio.h>
#include <faunus/energy.h>
#include <faunus/scatter.h>
#include <Eigen/Core>
#include <chrono>
#include <thread>
#include <functional>

namespace Faunus
{

    /**
     * @brief Namespace for analysis routines
     *
     * Most end-users will be interested in jumping
     * directly to `Analysis::CombinedAnalysis` which
     * aggregates most analysis methods below and lets
     * them be controlled by JSON input.
     */
    namespace Analysis
    {

	/**
	 * @brief Base class for analysis routines.
	 *
	 * This is the base class for analysis routines.
	 * Derived classes should implement:
	 *
	 * - a descriptive name
	 * - `_info()`
	 * - `_sample()`
	 *
	 * It is recommended that derived classes also implement:
	 *
	 * - the `cite` string to provide external information
	 * - `_test()` for unit testing
	 *
	 * The `sample()` wrapper function takes care of timing the analysis
	 * as well as sample the number of sample points at a given interval
	 * specified with the JSON keyword `nstep`.
	 *
	 * @todo Make `_sample()` pure virtual
	 */
	class AnalysisBase
	{
	    private:
		virtual string _info();  //!< info all classes must provide
		virtual Tmjson _json();   //!< result of analysis as json object
		virtual void _test( UnitTest & );

		int stepcnt;          //!< counter between sampling points

	    protected:
		TimeRelativeOfTotal <std::chrono::microseconds> timer;
		char w;               //!< width of info
		int steps;            //!< Sample interval (do not modify)
		unsigned long int cnt;//!< number of samples - increased for every run()==true.
		string name;          //!< descriptive name
		string cite;          //!< reference, url, doi etc. describing the analysis
		virtual void _sample();
	    public:
		AnalysisBase();
		AnalysisBase( Tmjson&, string=string() ); //!< Construct from JSON
		virtual ~AnalysisBase();
		string info();       //!< Print info and results
		void test( UnitTest & );//!< Perform unit test
		void sample();       //!< Sample event.
		Tmjson json();       //!< Get info and results as json object
	};

	/**
	 * @brief Pressure analysis using the virial theorem
	 *
	 * This calculates the excess pressure tensor defined as
	 * @f[
	 * \mathcal{P} = \frac{1}{3V}\left <
	 * \sum_{i}^{N-1} \sum_{j=i+1}^N \mathbf{r}_{ij} \otimes \mathbf{f}_{ij}
	 * \right >_{NVT}
	 * @f]
	 * where @f$r@f$ and @f$f@f$ are the distance and force, @f$V@f$ the system volume,
	 * and the excess pressure scalar is the trace of @f$\mathcal{P}@f$.
	 * The trivial kinetic contribution is currently not included.
	 *
	 * Upon construction the JSON entry is searched
	 * for the following keywords:
	 *
	 * Keyword   |  Description
	 * :-------- | :-------------------------------------------
	 * nstep     | Sample every steps time `sample()` is called
	 * dim       | Dimensions (default: 3)
	 * area      | Area if dim=2 (default: 0)
	 *
	 * References:
	 *
	 * - <http://dx.doi.org/10/ffwrhd>
	 * - <http://dx.doi.org/10/fspzcx>
	 *
	 * @todo At the moment this analysis is limited to "soft" systems only,
	 * i.e. for non-rigid systems with continuous potentials.
	 */
	template<typename SpaceType>
	    class VirialPressure : public AnalysisBase
	{
	    public:
		typedef Space<typename SpaceType::GeometryType, typename SpaceType::ParticleType> Tspace;
	    private:
		Tspace *spc;
		Energy::Energybase <Tspace> *pot;
		int dim;             // dimensions (default: 3)

		typedef Eigen::Matrix3d Ttensor;
		Ttensor Texcess;           // excess pressure tensor
		Average<double> Pid; // ideal pressure

		/** @brief Ignore internal pressure in molecular groups (default: false) */
		bool noMolecularPressure;

		inline string _info() override
		{
		    using namespace Faunus::textio;
		    std::ostringstream o;
		    if ( cnt > 0 )
		    {
			vector<double> P(3);
			vector<string> id = {"Ideal", "Excess", "Total"};

			P[0] = Pid.avg();       // ideal
			P[1] = (Texcess / cnt).trace(); // excess
			P[2] = P[0] + P[1];     // total

			char l = 15;
			o << "\n  " << std::right
			    << setw(l + l) << "kT/" + angstrom + cubed
			    << setw(l) << "mM" << setw(l) << "Pa" << setw(l) << "atm" << "\n";
			for ( int i = 0; i < 3; i++ )
			    o << std::left << setw(l) << "  " + id[i] << std::right
				<< setw(l) << P[i]
				<< setw(l) << P[i] / 1.0_mM
				<< setw(l) << P[i] / 1.0_Pa
				<< setw(l) << P[i] / 1.0_atm << "\n";
			o << "\n  Osmotic coefficient = " << 1 + P[1] / P[0] << "\n"
			    << "  Excess pressure tensor (mM):\n\n" << Texcess / cnt / 1.0_mM << endl;
		    }
		    return o.str();
		}

		inline void _test( UnitTest &test ) override
		{
		    test("virial_pressure_mM", (Texcess / cnt).trace() / 1.0_mM, 0.2);
		}

		template<class Tpvec, class Tgeo, class Tpot>
		    Ttensor g_internal( const Tpvec &p, Tgeo &geo, Tpot &pot, Group &g )
		    {
			Ttensor t;
			t.setZero();
			for ( auto i = g.front(); i < g.back(); i++ )
			    for ( auto j = i + 1; j <= g.back(); j++ )
			    {
				auto rij = geo.vdist(p[i], p[j]);
				auto fij = pot.f_p2p(p[i], p[j]);
				t += rij * fij.transpose();
			    }
			return t;
		    }

		template<class Tpvec, class Tgeo, class Tpot>
		    Ttensor g2g( const Tpvec &p, Tgeo &geo, Tpot &pot, Group &g1, Group &g2 )
		    {
			assert(&g1 != &g2);
			Ttensor t;
			t.setZero();
			for ( auto i : g1 )
			    for ( auto j : g2 )
			    {
				auto rij = geo.vdist(p[i], p[j]);
				auto fij = pot.f_p2p(p[i], p[j]);
				t += rij * fij.transpose();
			    }
			return t;
		    }

		inline void _sample() override
		{
		    Ttensor t;
		    t.setZero();

		    int N = spc->p.size();
		    double V = spc->geo.getVolume( dim );

		    // loop over groups internally
		    for ( auto g : spc->groupList())
		    {
			if ( noMolecularPressure )
			    if ( g->isMolecular())
			    {
				N = N - g->size() + 1;
				continue;
			    }
			t += g_internal(spc->p, spc->geo, *pot, *g);
		    }

		    // loop group-to-group
		    auto beg = spc->groupList().begin();
		    auto end = spc->groupList().end();
		    for ( auto gi = beg; gi != end; ++gi )
			for ( auto gj = gi; ++gj != end; )
			    t += g2g(spc->p, spc->geo, *pot, *(*gi), *(*gj));

		    // add to grand avarage
		    Texcess += t / (dim * V);
		    Pid += N / V;
		}

	    public:
		template<class Tpotential>
		    VirialPressure( Tmjson &j, Tpotential &pot, Tspace &spc ) : spc(&spc), pot(&pot), AnalysisBase(j)
		{
		    dim = j.value("dim", 3);
		    noMolecularPressure = j.value("noMolecularPressure", false);
		    name = "Virial Pressure";
		    Texcess.setZero();
		}
	};

	/**
	 * @brief Analysis of polymer shape - radius of gyration, shape factor etc.
	 * @date November, 2011
	 *
	 * This will analyse polymer groups and calculate Rg, Re and the shape factor. If
	 * sample() is called with different groups these will be distinguished by their
	 * *name* and sampled individually.
	 * Upon construction the following JSON keywords are read from section
	 * analysis/polymershape:
	 *
	 * Keyword   | Description
	 * :-------- | :------------------
	 * `nstep`   | Interval with which to sample (default: 1)
	 * `mollist` | List of molecule name to sample
	 */
	template<class SpaceType>
	    class PolymerShape : public AnalysisBase
	{
	    public:
		typedef Space<typename SpaceType::GeometryType, typename SpaceType::ParticleType> Tspace;
	    private:
		std::map<string, Average < double> > Rg2, Rg, Re2, Rs, Rs2, Rg2x, Rg2y,
		    Rg2z;
		Tspace *spc;

		vector<int> molid; // molecule id's to analyse

		string _info() override
		{
		    char w = 10;
		    using namespace textio;
		    std::ostringstream o;
		    if ( !Rg2.empty())
		    {
			o << endl << indent(SUBSUB) << std::left << setw(w) << "Group"
			    << setw(w + 5) << bracket("Rg" + squared)
			    << setw(w + 12) << bracket("Rg" + squared) + "-" + bracket("Rg") + squared
			    << setw(w + 5) << rootof + bracket("Rg" + squared)
			    << setw(w + 7) << rootof + bracket("Rgx" + squared)
			    << setw(w + 7) << rootof + bracket("Rgy" + squared)
			    << setw(w + 7) << rootof + bracket("Rgz" + squared)
			    << setw(w + 7) << rootof + bracket("Re" + squared)
			    << bracket("Re" + squared) + "/" + bracket("Rg" + squared) << endl;
			for ( auto &m : Rg2 )
			    o << indent(SUBSUB) << std::left << setw(w) << m.first
				<< std::setprecision(4)
				<< setw(w) << m.second.avg()
				<< setw(w + 2) << m.second.avg() - pow(Rg[m.first].avg(), 2)
				<< setw(w - 2) << sqrt(m.second.avg())
				<< setw(w) << sqrt(Rg2x[m.first].avg())
				<< setw(w) << sqrt(Rg2y[m.first].avg())
				<< setw(w) << sqrt(Rg2z[m.first].avg())
				<< setw(w) << sqrt(Re2[m.first].avg())
				<< Re2[m.first].avg() / Rg2[m.first].avg() << endl;
		    }
		    return o.str();
		}

		void _test( UnitTest &t ) override
		{
		    for ( auto &m : Rg2 )
			t("PolymerShape_Rg" + m.first, Rg[m.first].avg());
		}

		double gyrationRadiusSquared( const Group &pol, const Tspace &spc )
		{
		    assert(spc.geo.dist(pol.cm, pol.massCenter(spc)) < 1e-9
			    && "Mass center must be in sync.");
		    Point rg2 = vectorgyrationRadiusSquared(pol, spc);
		    return rg2.x() + rg2.y() + rg2.z();
		}

		Point vectorEnd2end( const Group &pol, const Tspace &spc )
		{
		    return spc.geo.vdist(spc.p[pol.front()], spc.p[pol.back()]);
		}

		void _sample() override
		{
		    if ( molid.empty()) // don't increase global counter if
			cnt = 0;           // there are no molecules
		    for ( auto id : molid )
			for ( auto pol : spc->findMolecules(id))
			{
			    Point r2 = vectorgyrationRadiusSquared(*pol, *spc);
			    double rg2 = r2.x() + r2.y() + r2.z();
			    double re2 = spc->geo.sqdist(spc->p[pol->front()], spc->p[pol->back()]);
			    Rg2[pol->name] += rg2;
			    Rg2x[pol->name] += r2.x();
			    Rg2y[pol->name] += r2.y();
			    Rg2z[pol->name] += r2.z();
			    Rg[pol->name] += sqrt(r2.x() + r2.y() + r2.z());
			    Re2[pol->name] += re2; //end-2-end squared
			    double rs = Re2[pol->name].avg() / Rg2[pol->name].avg(); // fluctuations in shape factor
			    Rs[pol->name] += rs;
			    Rs2[pol->name] += rs * rs;
			    //Point re = vectorEnd2end(pol,spc);
			    //Re2[pol.name] += pow(re.len(), 2);
			}
		}

	    public:

		PolymerShape() { name = "Polymer Shape"; }

		PolymerShape( Tmjson &j, Tspace &spc ) : AnalysisBase(j), spc(&spc)
	    {
		name = "Polymer Shape";
		auto m = j.at("mollist");
		for ( auto &i : m )
		{   // loop over molecule names
		    string molname = i.get<string>();
		    auto it = spc.molList().find(molname);
		    if ( it != spc.molList().end())
			molid.push_back(it->id);
		    else
			throw std::runtime_error(name + ": molecule not found");
		}
	    }

		Point vectorgyrationRadiusSquared( const Group &pol, const Tspace &spc ) const
		{
		    assert(spc.geo.dist(pol.cm, pol.massCenter(spc)) < 1e-9
			    && "Mass center must be in sync.");
		    double sum = 0;
		    Point t, r2(0, 0, 0);
		    for ( auto i : pol )
		    {
			t = spc.p[i] - pol.cm;                     // vector to center of mass
			spc.geo.boundary(t);                     // periodic boundary (if any)
			r2.x() += spc.p[i].mw * t.x() * t.x();
			r2.y() += spc.p[i].mw * t.y() * t.y();
			r2.z() += spc.p[i].mw * t.z() * t.z();
			sum += spc.p[i].mw;                      // total mass
		    }
		    assert(sum > 0 && "Zero molecular weight not allowed.");
		    return r2 * (1. / sum);
		}
	};

	/**
	 * @brief Analyse charge multipoles and their fluctuations of groups
	 *
	 * This analysis class will analyse selected groups and calculate
	 * their net-charge, dipole moment as well as their variances.
	 * It is possible to exclude certain atom types by added their
	 * names to an exclusionlist. Several groups may be analysed -
	 * the `sample()` function will automatically identify different
	 * groups via their names.
	 * The dipole moment is calculated with respect to the mass center.
	 *
	 * Parameters are specified in `["analysis"]["chargemultipole"]:
	 *
	 * Key        | Description
	 * :--------- | :--------------------------------
	 * `nstep`    | Sample at every n'th microstep
	 * `mollist`  | Array of molecule names to be analysed
	 *
	 * @author Anil Kurut
	 * @date Lund 2012
	 */
	template<typename Tspace>
	    class ChargeMultipole : public AnalysisBase
	{
	    private:
		Tspace *spc;
		vector<int> molid; // molid to analyse
		std::map<string, Average<double> > Z, Z2, mu, mu2, quad;

		/** @brief Determines particle should be excluded from analysis */
		template<class Tparticle>
		    bool excluded( const Tparticle &p )
		    {
			if ( exclusionlist.count(atom[p.id].name) == 0 )
			    return false;
			return true;
		    }

		/** @brief Sample properties of Group (identified by group name) */
		void __sample( const Group &g )
		{
		    assert(!g.name.empty() && "All Groups should have a name!");
		    if ( spc->molecule[g.molId].isMolecular())
		    {
			double z = netCharge(spc->p, g);
			Point dip = Geometry::dipoleMoment(*spc, g);
			auto Q = Geometry::quadrupoleMoment(spc->p, spc->geo, g);

			Z[g.name] += z;
			Z2[g.name] += z * z;
			mu[g.name] += dip.norm();
			mu2[g.name] += dip.norm() * dip.norm();
			quad[g.name] += Q.determinant();
		    }
		}

		/* @brief Sample properties of Group (identified by group name) */
		void _sample() override
		{
		    for ( auto id : molid ) // loop over molecule id's
			for ( auto g : spc->findMolecules(id)) // loop over molecules of that id
			    __sample(*g);
		}

		string _info() override
		{
		    using namespace textio;
		    char k = 13;
		    std::ostringstream o;
		    if ( !exclusionlist.empty())
		    {
			o << pad(SUB, w, "Exclusion list");
			for ( auto i : exclusionlist )
			    o << i << " ";
		    }
		    o << endl << indent(SUB) << std::left << setw(w) << "Macromolecule  "
			<< setw(k + 4) << bracket("Z")
			<< setw(k + 11) << bracket("Z" + squared) + "-" + bracket("Z") + squared
			<< setw(k + 5) << bracket(textio::mu)
			<< setw(k + 13) << bracket(textio::mu + squared) + "-" + bracket(textio::mu) + squared
			<< setw(k + 5) << bracket("|Quad|")
			<< endl;
		    for ( auto &m : Z )
			o << indent(SUB) << std::left << setw(w) << m.first << setw(k) << m.second.avg()
			    << setw(k + 1) << Z2[m.first].avg() - pow(m.second.avg(), 2)
			    << setw(k) << mu[m.first].avg()
			    << setw(k) << mu2[m.first].avg() - pow(mu[m.first].avg(), 2)
			    << setw(k) << quad[m.first].avg()
			    << endl;
		    return o.str();
		}

		Tmjson _json() override
		{
		    Tmjson j;
		    auto &_j = j[name]["mollist"];
		    for ( auto &m : Z )
		    {
			_j[m.first]["Z"] = m.second.avg();
			_j[m.first]["Z2"] = Z2[m.first].avg();
			_j[m.first]["mu"] = mu[m.first].avg();
			_j[m.first]["mu2"] = mu2[m.first].avg();
			_j[m.first]["quad"] = quad[m.first].avg();
		    }
		    return j;
		}

	    public:
		ChargeMultipole( Tmjson &j, Tspace &spc ) : AnalysisBase(j), spc(&spc)
	    {
		name = "Charge Multipole";
		auto m = j["mollist"];
		for ( auto &i : m )
		{   // loop over molecule names
		    string molname = i.get<string>();
		    auto it = spc.molList().find(molname);
		    if ( it != spc.molList().end())
			molid.push_back(it->id);
		    else
			throw std::runtime_error(name+": molecule not found");
		}
	    }

		std::set<string> exclusionlist; //!< Atom names listed here will be excluded from the analysis.
	};

	/**
	 * @brief Multipolar decomposition between groups as a function of separation
	 *
	 * This will analyse the electrostatic energy between two groups as
	 * a function of their mass center separation. Sampling consists of
	 * the following:
	 *
	 * 1. The exact electrostatic energy is calculated by explicitly summing
	 *    Coulomb interactions between charged particles
	 * 2. Each group -- assumed to be a molecule -- is translated into a
	 *    multipole (monopole, dipole, quadrupole)
	 * 3. Multipolar interaction energies are calculated, summed, and tabulated
	 *    together with the exact electrostatic interaction energy. Ideally
	 *    (infinite number of terms) the multipoles should capture full
	 *    electrostatics
	 *
	 * The points 1-3 above will be done as a function of group-to-group
	 * mass center separation @f$R@f$ and moments
	 * on molecule @f$a@f$ and @f$b@f$ with charges @f$q_i@f$ in position @f$\boldsymbol{r}_i@f$
	 * with respect to the mass center are calculated according to:
	 *
	 * \f{eqnarray*}{
	 *     q_{a/b}                & = & \sum_i q_i \\
	 *     \boldsymbol{\mu}_{a/b} & = & \sum_i q_i\mathbf{r_i} \\
	 *     \boldsymbol{Q}_{a/b}   & = & \frac{1}{2} \sum_i q_i\mathbf{r_i} \mathbf{r_i}^T
	 * \f}
	 *
	 * And, ignoring prefactors, the energy between molecule @f$a@f$ and @f$b@f$ at @f$R@f$ is:
	 *
	 * \f{eqnarray*}{
	 *    u_{ion-ion}  & = & \frac{q_aq_b}{R} \\
	 *    u_{ion-dip}  & = & \frac{q_a \boldsymbol{\mu}_b \boldsymbol{R}}{R^3} + ... \\
	 *    u_{dip-dip}  & = & \frac{\boldsymbol{\mu_a}\boldsymbol{\mu_b}   }{ R^3  }
	 *         - \frac{3 (\boldsymbol{\mu_a} \cdot \boldsymbol{R}) ( \boldsymbol{\mu_b}\cdot\boldsymbol{R})  }{R^5}   \\
	 *    u_{ion-quad} & = & \frac{ q_a \boldsymbol{R}^T \boldsymbol{Q}_b \boldsymbol{R} }{R^5}
	 *         - \frac{  q_a \mbox{tr}(\boldsymbol{Q}_b) }{R^3} + ... \\
	 *    u_{total}    & = & u_{ion-ion} + u_{ion-dip} + u_{dip-dip} + u_{ion-quad}\\
	 *    u_{exact}    & = & \sum_i^a\sum_j^b \frac{q_iq_j}{ | \boldsymbol{r_i} - \boldsymbol{r_j}  |    }
	 * \f}
	 *
	 * During simulation, these are thermally averaged over angles, co-solute degrees of
	 * freedom etc.
	 * Note also that the moments are defined with
	 * respect to the *mass* center, not *charge* center. While for most
	 * macromolecules there is only a minor difference between the two,
	 * the latter is more appropriate and is planned for a future update.
	 * A simply way to mimic this is to assign zero mass to all neutral
	 * atoms in the molecule.
	 *
	 * The constructor takes the following JSON keywords:
	 *
	 * Keyword   | Description
	 * :-------- | :---------------------------------------------
	 * `dr`      | Distance resolution in angstrom (default: 0.2)
	 * `groups`  | Exactly two group names (array)
	 * `file`    | Output filename (default: multipole.dat)
	 *
	 * @date Malmo 2014
	 * @todo Add option to use charge center instead of mass center
	 */
	template<class Tspace, class Tcoulomb=Potential::Coulomb>
	    class MultipoleDistribution : public AnalysisBase
	{
	    private:
		string filename;      // output file name
		int id1, id2;         // pair of molecular id's to analyse
		double dr;            // distance resolution

		Tcoulomb coulomb;     // coulomb potential
		Tspace &spc;

		string _info() override
		{
		    return string();
		}

		struct data
		{
		    double tot, ii, id, iq, dd, mucorr;
		    unsigned long int cnt;

		    data() : cnt(0) {}
		};

		std::map<int, data> m; // slow, but OK for infrequent sampling

		// convert group to multipolar particle
		template<class Tgroup>
		    DipoleParticle toMultipole( const Tspace &spc, const Tgroup &g ) const
		    {
			DipoleParticle m;
			m = g.cm;
			m.charge = netCharge(spc.p, g);            // monopole
			m.mu() = Geometry::dipoleMoment(spc, g); // dipole
			m.muscalar() = m.mu().norm();
			if ( m.muscalar() > 1e-8 )
			    m.mu() = m.mu() / m.muscalar();
			m.theta() = Geometry::quadrupoleMoment(spc.p, spc.geo, g); // quadrupole
			return m;
		    }

		double g2g( Tspace &spc, const Group &g1, const Group &g2 )
		{
		    double u = 0;
		    for ( auto i : g1 )
			for ( auto j : g2 ) {
			    double r2 = spc.geo.sqdist( spc.p[i], spc.p[j] );
			    u += coulomb(spc.p[i], spc.p[j], r2);
			}
		    return u;
		}

		/**
		 * @brief Sample multipole energy
		 *
		 * @param spc Simulation space
		 * @param g1  Group with molecule 1
		 * @param g2  Group with molecule 2
		 * @note Group mass-centers (`Group::cm`) must be up-to-date before
		 *       calling this function
		 */
		void sample( Tspace &spc, Group &g1, Group &g2 )
		{
		    // multipoles and cm-cm distance
		    auto a = toMultipole(spc, g1);
		    auto b = toMultipole(spc, g2);
		    auto r = spc.geo.vdist(g1.cm, g2.cm);
		    double r2inv = 1 / r.squaredNorm();
		    double rinv = sqrt(r2inv);

		    // multipolar energy
		    coulomb.setSpace(spc);
		    data d;
		    d.cnt++;
		    d.tot = g2g(spc, g1, g2); // exact el. energy
		    d.ii = a.charge * b.charge * rinv; // ion-ion, etc.
		    d.id = q2mu(a.charge*b.muscalar(),b.mu(),b.charge*a.muscalar(),a.mu(),r);
		    d.dd = mu2mu(a.mu(), b.mu(), a.muscalar() * b.muscalar(), r);
		    d.iq = q2quad(a.charge, b.theta(), b.charge, a.theta(), r);
		    d.mucorr = a.mu().dot(b.mu());

		    // add to grand average
		    int key = to_bin(1 / rinv, dr);
		    auto it = m.find(key);
		    if ( it == m.end())
			m[key] = d;
		    else
		    {
			it->second.cnt++;
			it->second.ii += d.ii;
			it->second.id += d.id;
			it->second.iq += d.iq;
			it->second.dd += d.dd;
			it->second.tot += d.tot;
			it->second.mucorr += d.mucorr;
		    }
		}

		/** @brief Save multipole distribution to disk */
		void save( const string &filename ) const
		{
		    if (cnt==0)
			return;
		    std::ofstream f(textio::prefix + filename.c_str());
		    if ( f )
		    {
			char w = 12;
			auto lB = coulomb.bjerrumLength();
			f.precision(4);
			f << "# Multipolar energy analysis (kT)\n"
			    << std::left << setw(w) << "# r/AA" << std::right << setw(w) << "exact"
			    << setw(w) << "total" << setw(w) << "ionion" << setw(w) << "iondip"
			    << setw(w) << "dipdip" << setw(w) << "ionquad" << setw(w) << "mucorr\n";
			for ( auto &i : m )
			    f << std::left
				<< setw(w) << i.first * dr                  // r
				<< std::right
				<< setw(w) << i.second.tot / i.second.cnt   // exact (already in kT)
				<< setw(w) << lB * (i.second.ii + i.second.id + i.second.dd + i.second.iq) / i.second.cnt // total
				<< setw(w) << lB * i.second.ii / i.second.cnt // individual poles...
				<< setw(w) << lB * i.second.id / i.second.cnt
				<< setw(w) << lB * i.second.dd / i.second.cnt
				<< setw(w) << lB * i.second.iq / i.second.cnt
				<< setw(w) << i.second.mucorr / i.second.cnt
				<< "\n";
		    }
		}
		void _sample() override
		{
		    auto v1 = spc.findMolecules(id1);
		    auto v2 = spc.findMolecules(id2);

		    for (auto gi : v1)
			for (auto gj : v2)
			    if (gi!=gj)
				sample( spc, *gi, *gj ); 
		}

		//@todo Add distributions
		Tmjson _json() override
		{
		    return {
			{ name,
			    {
				{ "groups", { spc.molecule[id1].name, spc.molecule[id2].name} },
				{ "file", filename },
				{ "dr", dr },
				{ "lB", coulomb.bjerrumLength() }
			    }
			}
		    };
		}

	    public:

		MultipoleDistribution( Tmjson &j, Tspace &s ) : AnalysisBase(j), coulomb(j), spc(s)
	    {
		name = "Multipole Distribution";
		dr = j.value("dr", 0.2);
		filename = j.value("file", string("multipole.dat"));
		if (j.count("groups")==1) {
		    vector<string> names = j["groups"];
		    if (names.size()==2) {
			auto f1 = s.molecule.find( names[0] );
			auto f2 = s.molecule.find( names[1] );
			if (f1 != s.molecule.end())
			    if (f2 != s.molecule.end()) {
				id1 = (*f1).id;
				id2 = (*f2).id;
				return;
			    }
		    }
		}
		throw std::runtime_error(name + ": specify exactly two existing group names");
	    }

		// @todo Use json output instead
		~MultipoleDistribution()
		{
		    save(filename);
		}

	};

	/**
	 * @brief Widom method for excess chemical potentials
	 *
	 * This class will use the ghost particle insertion technique
	 * to insert a collection of particles which, when summed, should
	 * have no net charge. This is used to calculate the mean excess
	 * chemical potential and activity coefficient.
	 *
	 *  Keyword    | Description
	 *  ---------- | ---------------------------------------
	 *  `ninsert`  | Number of insertions per sampling event (int)
	 *  `nstep`    | Sample every n'th step (int)
	 *  `particles`| Atom names to simultaneously insert (array)
	 */
	template<class Tspace>
	    class Widom : public AnalysisBase
	{
	    private:
		typedef typename Tspace::ParticleType Tparticle;

		Tspace &spc;
		Energy::Energybase<Tspace> &pot;

		Average<double> expsum; //!< Average of the excess chemical potential
		int ghostin;            //!< Number of insertions per sample event

		string _info() override
		{
		    using namespace Faunus::textio;
		    std::ostringstream o;
		    o << pad(SUB, w, "Number of insertions") << expsum.cnt << endl
			<< pad(SUB, w, "Excess chemical pot.") << muex() << kT << endl
			<< pad(SUB, w, "Mean activity coefficient") << gamma() << endl
			<< pad(SUB, w, "Ghost particles");
		    for ( auto &p : g )
			o << atom[p.id].name << " ";
		    return o.str() + "\n";
		}

		Tmjson _json() override
		{
		    if ( expsum.cnt>0 )
			return {
			    { name,
				{
				    { "number of insertions", expsum.cnt },
				    { "ninsert", ghostin },
				    { "muexcess", muex() },
				    { "activity coefficient", gamma() }
				}
			    }
			};
		    return Tmjson();
		}

		void _test( UnitTest &test ) override
		{
		    test("widom_muex", muex());
		}

		void _sample() override
		{
		    int n = g.size(), cnt=ghostin;
		    if ( n > 0 )
			while ( cnt-- > 0 )
			{
			    double du = 0;
			    for ( auto &i : g )
				spc.geo.randompos(i);     // random ghost positions
			    for ( auto &i : g )
				du += pot.all2p(spc.p, i);  // energy with all particles in space
			    for ( int i = 0; i < n - 1; i++ )
				for ( int j = i + 1; j < n; j++ )
				    du += pot.p2p(g[i], g[j]);// energy between ghost particles
			    expsum += exp(-du);
			}
		}

	    protected:
		std::vector<Tparticle> g; //!< Pool of ghost particles to insert (simultaneously)
	    public:
		Widom( Tmjson &j, Energy::Energybase<Tspace> &pot, Tspace &spc ) : spc(spc), pot(pot), AnalysisBase(j)
	    {
		name = "Multi Particle Widom Analysis";
		cite = "doi:10/dkv4s6";

		ghostin = j.value("ninsert", 10);

		if (j.count("particles")>0)
		    if (j["particles"].is_array()) {
			for (auto &name : j["particles"]) {
			    Tparticle a;
			    a = atom[ name.get<string>() ];
			    add(a);
			}
			return;
		    }
		throw std::runtime_error(name + ": invalid json input");
	    }

		void add( Tparticle &a ) { g.push_back(a); }

		/* @brief Add particle to insert - sum of added particle charges should be zero.*/
		void add( typename Tspace::ParticleVector &p )
		{
		    std::map<short, bool> map; // replace w. `std::set`
		    for ( auto i : p )
			map[i.id] = true;
		    for ( auto &m : map )
		    {
			Tparticle a;
			a = atom[m.first];
			add(a);
		    }
		}

		/** @brief Sampled mean activity coefficient */
		double gamma() { return exp(muex()); }

		/** @brief Sampled mean excess chemical potential */
		double muex() { return -log(expsum.avg()) / g.size(); }
	};

	/**
	 * @brief Single particle hard sphere Widom insertion with charge scaling
	 *
	 * This will calculate excess chemical potentials for single particles
	 * in the primitive model of electrolytes. Use the `add()` functions
	 * to add test or *ghost* particles and call `sample()` to perform single
	 * particle insertions.
	 * Inserted particles are *non-perturbing* and thus removed again after
	 * sampling. Electroneutrality for insertions of charged species is
	 * maintaing by charge re-scaling according to
	 *
	 * - [Svensson and Woodward, Mol. Phys. 1988, 64:247]
	 *   (http://doi.org/ft9bv9)
	 *
	 * Currently this works **only** for the primitive model of electrolytes, i.e.
	 * hard, charged spheres interacting with a Coulomb potential.
	 *
	 * JSON input:
	 *
	 *  Keyword    | Description
	 *  ---------- | ---------------------------------------
	 *  `lB`       | Bjerrum length (angstrom)
	 *  `ninsert`  | Number of intertions per sampling event
	 *  `nstep`    | Sample every n'th step
	 *
	 * @warning Works only for the primitive model
	 * @note This is a conversion of the Widom routine found in the `bulk.f`
	 *       fortran program by Bolhuis/Jonsson/Akesson at Lund University.
	 * @author Martin Trulsson and Mikael Lund
	 * @date Lund / Prague 2007-2008.
	 */
	template<class Tspace>
	    class WidomScaled : public AnalysisBase
	{

	    private:

		typedef std::vector<double> Tvec;
		typedef typename Tspace::ParticleType Tparticle;
		typename Tspace::ParticleVector g;//!< list of test particles

		Tspace &spc;

		Tvec chel;          //!< electrostatic
		Tvec chhc;          //!< hard collision
		Tvec chex;          //!< excess
		Tvec chexw;         //!< excess
		Tvec chtot;         //!< total
		vector <Tvec> ewden; //!< charging denominator
		vector <Tvec> ewnom; //!< charging nominator
		vector <Tvec> chint; //!< charging integrand
		Tvec chid;          //!< ideal term
		Tvec expuw;
		vector<int> ihc, irej;
		int ghostin;        //< ghost insertions
		double lB;          //!< Bjerrum length

		void init()
		{
		    int gspec = g.size();
		    chel.resize(gspec);
		    chhc.resize(gspec);
		    chex.resize(gspec);
		    chtot.resize(gspec);
		    ewden.resize(gspec);
		    ewnom.resize(gspec);
		    chint.resize(gspec);
		    expuw.resize(gspec);
		    chexw.resize(gspec);
		    ihc.resize(gspec);
		    irej.resize(gspec);

		    for ( int i = 0; i < gspec; i++ )
		    {
			chel[i] = 0;
			chhc[i] = 0;
			chex[i] = 0;
			chtot[i] = 0;
			ihc[i] = 0;
			ewden[i].resize(11);
			ewnom[i].resize(11);
			chint[i].resize(11);
			for ( int j = 0; j < 11; j++ )
			    ewden[i][j] = ewnom[i][j] = chint[i][j] = 0;
		    }
		}

		template<class Tgeo>
		    bool overlap( const Tparticle &a, const Tparticle &b, const Tgeo &geo )
		    {
			double s = a.radius + b.radius;
			return (geo.sqdist(a, b) < s * s) ? true : false;
		    }

		string _info() override
		{
		    using namespace textio;
		    std::ostringstream o;
		    double aint4, aint2, aint1;
		    for ( size_t i = 0; i < g.size(); i++ )
		    {
			for ( int cint = 0; cint < 11; cint++ )
			{
			    if ( ewden[i][cint] == 0 )
				std::cerr << "# WARNING: Widom denominator equals zero" << endl;
			    else
				chint[i][cint] = ewnom[i][cint] / ewden[i][cint];
			}
			aint4 = chint[i][1] + chint[i][3] + chint[i][5] + chint[i][7] + chint[i][9];
			aint2 = chint[i][2] + chint[i][4] + chint[i][6] + chint[i][8];
			aint1 = chint[i][0] + chint[i][10];
			chel[i] = 1. / 30. * (aint1 + 2 * aint2 + 4 * aint4);
		    }

		    int cnttot = cnt * ghostin;
		    o << pad(SUB, w, "Number of insertions") << cnttot << endl
			<< pad(SUB, w, "Excess chemical potentials (kT)") << endl
			<< "             total    elec.   hs             z        r" << endl;
		    char w = 10;
		    for ( size_t i = 0; i < g.size(); i++ )
		    {
			chhc[i] = -log(double(cnttot - ihc[i]) / cnttot);
			chexw[i] = -log(expuw[i]);
			chex[i] = chhc[i] + chel[i];
			o.unsetf(std::ios_base::floatfield);
			o << "    [" << i << "] "
			    << std::setprecision(4)
			    << std::setw(w) << chex[i]
			    << std::setw(w) << chel[i]
			    << std::setw(w) << chhc[i]
			    << std::setprecision(2) << std::fixed
			    << std::setw(w) << g[i].charge
			    << std::setw(w) << g[i].radius << endl;
		    }
		    return o.str();
		}

		Tmjson _json() override
		{
		    if ( cnt * ghostin>0 )
			return {
			    { name,
				{
				    { "number of insertions", cnt*ghostin },
				    { "ninsert", ghostin },
				    { "lB", lB }
				}
			    }
			};
		    return Tmjson();
		}


		void _sample() override
		{
		    auto &geo = spc.geo;
		    auto &p = spc.p;
		    if ( !g.empty())
			if ( !p.empty())
			{
			    Tparticle ghost;
			    double u, cu;
			    for ( int i = 0; i < ghostin; i++ )
			    {
				geo.randompos(ghost);
				int goverlap = 0;
				for ( size_t k = 0; k < g.size(); k++ )
				{
				    ghost.radius = g[k].radius;
				    irej[k] = 0;
				    int j = 0;
				    while ( !overlap(ghost, p[j], geo) && j < (int) p.size())
					j++;
				    if ( j != (int) p.size())
				    {
					ihc[k]++;
					irej[k] = 1;
					goverlap++;
				    }
				}

				if ( goverlap != (int) g.size())
				{
				    cu = 0;
				    u = 0;  //elelectric potential (Coulomb only!)
				    for ( auto &i : p )
				    {
					double invdi = 1 / geo.dist(ghost, i);
					cu += invdi;
					u += invdi * i.charge;
				    }
				    cu = cu * lB;
				    u = u * lB;
				    double ew, ewla, ewd;
				    for ( size_t k = 0; k < g.size(); k++ )
				    {
					if ( irej[k] == 0 )
					{
					    expuw[k] += exp(-u * g[k].charge);
					    for ( int cint = 0; cint < 11; cint++ )
					    {
						ew = g[k].charge *
						    (u - double(cint) * 0.1 * g[k].charge * cu / double(p.size()));
						ewla = ew * double(cint) * 0.1;
						ewd = exp(-ewla);
						ewden[k][cint] += ewd;
						ewnom[k][cint] += ew * ewd;
					    }
					}
				    }
				}
			    }
			}
		}

	    public:

		WidomScaled( Tmjson &j, Tspace &spc ) : spc(spc), AnalysisBase(j)
	    {
		lB = j.value("lB", 7.0);
		ghostin = j.value("ninsert", 10);
		name = "Single particle Widom insertion w. charge scaling";
		cite = "doi:10/ft9bv9 + doi:10/dkv4s6";

		add(spc.p);
	    }

		/**
		 * @brief Add ghost particle
		 *
		 * This will add particle `p` to the list of ghost particles
		 * to insert.
		 */
		void add( const Tparticle &p )
		{
		    g.push_back(p);
		    init();
		}

		/**
		 * @brief Add ghost particles
		 *
		 * This will scan the particle vector for particles and each unique type
		 * will be added to the list a ghost particles to insert.
		 */
		template<class Tpvec>
		    void add( const Tpvec &p )
		    {
			std::set<typename Tparticle::Tid> ids;
			for ( auto &i : p )
			    ids.insert(i.id);
			for ( auto i : ids )
			{
			    Tparticle a;
			    a = atom[i];
			    add(a);
			}
		    }

	}; // end of WidomScaled

	/**
	 * @brief Samples bilayer structure
	 *
	 * This was developed for coarse grained membrane models
	 * but should be general enough for other uses.
	 */
	class BilayerStructure : public AnalysisBase
	{

	    private:

		inline string _info()
		{
		    using namespace textio;
		    std::ostringstream o;
		    if ( cnt > 0 )
			o << pad(SUB, w, "Lipid order parameter") << S << endl
			    << pad(SUB, w, "Area per lipid") << A << " " + sigma + squared << endl;
		    return o.str();
		}

		Average<double> S, A;

		void _test( UnitTest &t );

	    public:

		inline BilayerStructure()
		{
		    name = "Bilayer structure";
		    cite = "doi:10/chqzjk";
		}

		template<class Tcuboid, class Tpvec, class Tgroup>
		    void sample( Tcuboid &geo, Tpvec &p, Tgroup &lipids )
		    {
			cnt++;
			S += orderParameter(geo, p, lipids);
			A += areaPerLipid(geo, p, lipids);
		    }

		/**
		 * @brief Sample lipid order parameter
		 *
		 * @f[
		 * S = \frac{1}{2} \left ( 3 (\mathbf{an})^2 -1 \right )
		 * @f]
		 *
		 * where `a` is the unit vector between the tail and the head group,
		 * `n` is the normal to the bilayer plane.
		 */
		template<class Tcuboid, class Tpvec, class Tgroup>
		    static double
		    orderParameter( Tcuboid &geo, Tpvec &p, Tgroup &lipids, Point n = Point(0, 0, 1))
		    {
			Average<double> S;
			for ( int i = 0; i < lipids.numMolecules(); i++ )
			{
			    Group g;
			    lipids.getMolecule(i, g); // i'th lipid
			    Point a = geo.vdist(p[g.front()], p[g.back()]).normalized();
			    S += 0.5 * (3 * pow(a.dot(n), 2) - 1);
			}
			return S.avg();
		    }

		/**
		 * @brief Sample area per lipid (normalized by sigma)
		 */
		template<class Tcuboid, class Tpvec>
		    static double
		    areaPerLipid( Tcuboid &geo, Tpvec &p, Group &lipids )
		    {
			return geo.len.x() * geo.len.y() / lipids.numMolecules() * 2
			    / pow(2 * p[lipids.front()].radius, 2);
		    }
	};

	/*
	 * Perhaps make this a template, taking T=double as parameter?
	 */
	template<class T=double>
	    class analyzeVector
	    {
		private:
		    std::vector<T> noise;
		    int N;
		    int lag;
		    double mu;
		    double sigma2;
		    double F_X_LB;
		    double F_X_BP;
		    double F_X_stud;
		    double sTd_alpha;
		public:
		    inline analyzeVector( std::vector<T> noise_in )
		    {
			noise = noise_in;
			N = noise.size();
			lag = std::round(std::round(std::log(N)));  // More general instead of the maybe more common lag=20'
			lag = N;                                  // Only meaningful for large N (???)
			initMu();
			initVariance();
			F_X_LB = 0;
			F_X_BP = 0;
			F_X_stud = 0;
			sTd_alpha = 0;
		    }

		    void setLag( int lag_in )
		    {
			lag = lag_in;
		    }

		    void initMu()
		    {
			mu = 0;
			for ( int k = 0; k < N; k++ )
			{
			    mu += noise[k];
			}
			mu /= N;
		    }

		    double getMu()
		    {
			return mu;
		    }

		    void initVariance()
		    {
			sigma2 = 0;
			for ( int k = 0; k < N; k++ )
			{
			    sigma2 += noise[k] * noise[k];
			}
			sigma2 -= mu * mu;
		    }

		    double getVariance()
		    {
			return sigma2;
		    }

		    // Approximate cumulative distribution function for value alpha in Ljung-Box method
		    double getF_X_LB()
		    {
			return F_X_LB;
		    }

		    // Approximate cumulative distribution function for value alpha in Box-Pierce method
		    double getF_X_BP()
		    {
			return F_X_BP;
		    }

		    // Approximate cumulative distribution function for value alpha in studentTdistribution method
		    double getF_X_stud()
		    {
			return F_X_stud;
		    }

		    // Value of alpha to approximately get F_X_stud in studentTdistribution method (Method fails if (x*x/dof > 1), see line further down)
		    double getStdAlpha()
		    {
			return sTd_alpha;
		    }

		    /**
		     * @brief Chi-squared distribution.
		     *        \f$ F(x;k) = \frac{\gamma\left(\frac{k}{2},\frac{x}{2}\right)}{\Gamma\left(\frac{k}{2}\right)} \f$ where \f$ s \f$ is half the lag.
		     *
		     * @param significance The significance to reach.
		     * @param chi2step Step size in the iteration.
		     */
		    double getChi2( double significance, double &F_X, double chi2_step = 0.01 )
		    {
			double x = -chi2_step;
			double level = 1 - significance;
			do
			{
			    x += chi2_step;
			    F_X = incompleteGamma(x / 2) / std::tgamma(double(lag) / 2);
			}
			while ( level > F_X );
			return x;
		    }

		    /**
		     * @brief Lower Incomplete Gamma Function.
		     *        \f$ \gamma(s,x) = x^se^{-x}\sum_{k=0}^{\infty}\frac{x^k}{s(s+1)...(s+k)} \f$ where \f$ s \f$ is half the lag.
		     *
		     * @param x Value to estimate for.
		     */
		    double incompleteGamma( double x )
		    {
			double s = double(lag) / 2;
			double sum = 0;
			double term = 1.0 / s;
			int n = 1;
			while ( term != 0 )
			{
			    sum += term;
			    term *= (x / (s + n++));
			}
			return pow(x, s) * exp(-x) * sum;
		    }

		    double sampleAutocorrelation( double k )
		    {
			double nom = 0;
			double den = 0;
			for ( int t = 0; t < N - k; t++ )
			{
			    nom += (noise[t] - mu) * (noise[t + k] - mu);
			    den += (noise[t] - mu) * (noise[t] - mu);
			}
			for ( int t = N - k; t < N; t++ )
			{
			    den += (noise[t] - mu) * (noise[t] - mu);
			}
			return (nom / den);
		    }

		    /**
		     * @brief The Ljung-Box test is a statistical test.
		     *
		     * This tests if a group of autocorrelations differ from zero,
		     * based on a number of lags. Initially it was developed for ARMA-processes.
		     * It is a portmanteau test. More info at DOI: 10.1093/biomet/65.2.297
		     *
		     * @param alpha The significance of the test. With a probability of \f$ (1-\alpha)*100 \f$ % the result is true.
		     */
		    bool LjungBox( double alpha )
		    {
			double Q = 0.0;
			F_X_LB = 0;
			for ( int k = 0; k < lag; k++ )
			    Q += sampleAutocorrelation(k) / (N - k);

			if ( Q * N * (N + 2) > getChi2(alpha, F_X_LB))
			    return false;
			return true;
		    }

		    /**
		     * @brief The Box-Pierce test is a statistical test.
		     *
		     * This tests if a group of autocorrelations differ from zero.
		     * It tests the wider randomness based on a number of lags.
		     * This is more simple, and not as accurate, as the Ljung_Box test.
		     *
		     * @param alpha The significance of the test. With a probability of
		     * \f$ (1-\alpha)*100 \f$ % the result is true.
		     */
		    bool BoxPierce( double alpha )
		    {
			double Q = 0.0;
			F_X_BP = 0;
			for ( int k = 0; k < lag; k++ )
			    Q += sampleAutocorrelation(k);

			if ( Q * N > getChi2(alpha, F_X_BP))
			    return false;
			return true;
		    }

		    /**
		     * @brief Hypergeometric function.
		     *
		     * This function uses \f$ x=(x)_1=(x)_2=... \f$ for x=a,x=b and x=c.
		     * \f$ F_1(a,b,c;z) = \sum_{n=0}^{\infty}\frac{(a)_n(b)_n}{(c)_n}\frac{z^n}{n!} \f$
		     *
		     * @param a Coefficient (usually a vector)
		     * @param b Coefficient (usually a vector)
		     * @param c Coefficient (usually a vector)
		     * @param z Value to estimate for. Only works for \f$ |z|<1 \f$.
		     */
		    double F2( double a, double b, double c, double z )
		    {
			assert(std::abs(z) < 1 && "|z| is not smaller than 1!");
			int n = 0;
			int nfac = 1;
			double term = 0;
			double sum = 0;
			double coeff = a * b / c;
			do
			{
			    term = coeff * (pow(z, n) / nfac);
			    sum += term;
			    nfac *= ++n;
			}
			while ( term > 1e-30 ); // Some cutoff
			return sum;
		    }

		    /**
		     * @brief Student's t-test distribution
		     *
		     * @param alpha The significance of the test.
		     *        With a probability of \f$ (1-\alpha)*100 \f$ % the result is true.
		     * @param dof Degrees of freedom.
		     */
		    double studentTdistribution( double alpha, int dof, double x_step = 0.01 )
		    {
			double level = 1 - alpha;
			F_X_stud = 0;
			double x = -x_step;
			do
			{
			    x += 0.01;
			    if ( x * x / dof > 1 )
			    {
				x -= x_step;
				sTd_alpha = 1 - F_X_stud;
				break;
			    }
			    F_X_stud = 0.5 + x * std::tgamma((dof + 1) / 2) * F2(0.5, (dof + 1) / 2, 1.5, -x * x / dof)
				/ (sqrt(pc::pi * dof) * std::tgamma(dof / 2));
			}
			while ( level > F_X_stud );
			return x;
		    }

		    /**
		     * @brief Testing the null hypothesis that the mean is equal to zero. The degrees of freedom used in this test is N − 1.
		     *
		     * @param alpha The significance of the test. With a probability of \f$ (1-\alpha)*100 \f$ % the result is true.
		     */
		    bool oneSampleT( double alpha )
		    {
			return (studentTdistribution(alpha, N - 1) > sqrt(N / sigma2) * mu);
		    }

		    /**
		     * @brief Returns the result of the LjungBox-, BoxPierce- and oneSampleT-tests.
		     *
		     * @param alpha The significance of the test. With a probability of \f$ (1-\alpha)*100 \f$ % the result is true.
		     */
		    std::vector<bool> checkAll( double alpha )
		    {
			std::vector<bool> tests(3);
			tests.at(0) = LjungBox(alpha);
			tests.at(1) = BoxPierce(alpha);
			tests.at(2) = oneSampleT(alpha);
			return tests;
		    }

		    string info( char w = 0 )
		    {
			using namespace Faunus::textio;
			std::ostringstream o;
			o << "Sample size: " << N << ", Mean value: " << mu << ", Variance: " << sigma2 << ", Lag: " << lag << endl;
			o << "F_X(Ljung-Box): " << F_X_LB << ", F_X(Box_Pierce): " << F_X_BP << ", Significance(oneSampleT): "
			    << sTd_alpha << endl;
			return o.str();
		    }
	    };

	template<typename Tspace>
	    class CylindricalDensity : public AnalysisBase
	{
	    private:

		int id;
		double zmin, zmax, dz, area;
		Tspace *spc;
		Table2D<double, Average<double> > data;

		inline string _info() override
		{
		    using namespace Faunus::textio;
		    std::ostringstream o;
		    if ( cnt > 0 )
		    {
		    }
		    return o.str();
		}

		inline void _sample() override
		{
		    for ( double z = zmin; z <= zmax; z += dz )
		    {
			unsigned int n = 0;
			for ( auto &i : spc->p )
			    if ( i.id == id )
				if ( i.z() >= z )
				    if ( i.z() < z + dz )
					n++;
			data(z) += n / (area * dz);
		    }
		}

	    public:
		CylindricalDensity( Tmjson &j, Tspace &spc ) : spc(&spc), data(0.2), AnalysisBase(j)
	    {
		name = "Cylindrical Density";
		zmin = j.at("zmin");
		zmax = j.at("zmax");
		dz = j.at("dz");

		string atomtype = j.at("atomtype");
		cout << "atomtype = " << atomtype << endl;
		id = atom[atomtype].id;

		auto ptr = dynamic_cast< Geometry::Cylinder * >( &(spc.geo));
		if ( ptr != nullptr )
		    area = std::acos(-1) * pow(ptr->getRadius(), 2);
		else
		    area = 0;
		cout << "area = " << area << endl;
	    }

		void save( const string &file )
		{
		    unsigned int n = 0;
		    for ( auto i : spc->p )
			if ( i.id == id )
			    n++;
		    data.save(file, spc->geo.getVolume() / n);
		}

		~CylindricalDensity()
		{
		    save("cyldensity.dat");
		}
	};

	/**
	 * @brief Analysis to be performed once at the end of the simulation
	 *
	 * This analysis takes a function object `void(string)` that will
	 * be called once at the very end of the simulation.
	 */
	class WriteOnceFileAnalysis : public AnalysisBase
	{
	    protected:
		string filename;

		string _info() override { return string(); }

		std::function<void(string)> f;

		void _sample() override
		{
		    f(filename);
		}

	    public:
		WriteOnceFileAnalysis( Tmjson &j, std::function<void(string)> writer ) : AnalysisBase(j)
	    {
		steps = j.value("nstep", int(-1));
		filename = j.at("file");
		name = filename; // better than nothing...
		f = writer;
	    }

		~WriteOnceFileAnalysis()
		{
		    if ( steps == -1 )
			_sample();
		}
	};

	/**
	 * @brief Write XTC trajectory file
	 *
	 * JSON keywords: `nstep`, `file`
	 */
	template<class Tspace>
	    class XTCtraj : public AnalysisBase
	{
	    private:

		FormatXTC xtc;
		Tspace *spc;
		string filename;

		void _sample() override
		{
		    xtc.setbox(spc->geo.inscribe().len);
		    xtc.save(filename, spc->p);
		}

		string _info() override
		{
		    using namespace Faunus::textio;
		    std::ostringstream o;
		    if ( cnt > 0 )
			o << pad(SUB, 30, "Filename") << filename + "\n";
		    return o.str();
		}

	    public:

		XTCtraj( Tmjson &j, Tspace &s ) : AnalysisBase(j), xtc(1e6), spc(&s)
	    {
		name = "XTC trajectory reporter";
		filename = j.at("file");
		cite = "http://manual.gromacs.org/online/xtc.html";
	    }
	};

	/**
	 * @brief Calculate excess pressure using virtual volume move
	 *
	 * This will perform a virtual volume move and calculate the
	 * excess pressure according to
	 *
	 * @f[
	 * \frac{p^{ex}}{k_BT} = -\frac{ \ln \langle
	 * e^{-\Delta U / k_BT} \rangle_{NVT} }{ \Delta V }
	 * @f]
	 *
	 * based on Widom perturbation. The energy as evaluated by
	 * calculating all group-to-group interactions (`Energy::g2g`)
	 * and, for atomic groups, the internal energy
	 * (`Energy::g_internal`). For using the full Hamiltonian,
	 * use the keyword `fullenergy` as shown below.
	 *
	 * JSON input:
	 *
	 * Keyword      | Description
	 * :----------  | :-------------------------------------------
	 * `nstep`      | Sample every n'th time `sample()` is called
	 * `dV`         | Volume perturbation (angstrom cubed)
	 * `fullenergy` | Use full Hamiltonian (default: False)
	 *
	 * More information:
	 * - doi:10.1063/1.472721 (basics)
	 * - doi:10.1063/1.4767375 (pressure tensor, surface tension etc.)
	 *
	 * @todo Untested for molecular systems
	 * @note Be aware that external energies are currently disabled. Issue w. volume?
	 */
	template<class Tspace>
	    class VirtualVolumeMove : public AnalysisBase
	    {
		private:
		    typedef Energy::Energybase <Tspace> Tenergy;
		    Tenergy *pot;
		    Tspace *spc;
		    Point dir;
		    double dV;
		    bool fullenergy;
		    Average<double> duexp; // < exp(-du/kT) >

		    void scale( double Vold, double Vnew )
		    {
			for ( auto g : spc->groupList())
			    g->setMassCenter(*spc); // make sure CM's are up-to-date

			double xyz = cbrt(Vnew / Vold);
			double xy = sqrt(Vnew / Vold);
			for ( auto g : spc->groupList())
			    g->scale(*spc, dir, xyz, xy); // scale trial coordinates to new volume

			spc->geo.setVolume(Vnew);
			pot->setSpace(*spc);
		    }

		    double energy( typename Tspace::ParticleVector &p )
		    {
			if ( fullenergy )
			    return Energy::systemEnergy(*spc, *pot, p);

			double u = 0;
			for ( auto g : spc->groupList())
			    if ( g->isAtomic())
				u += pot->g_internal(p, *g);
			for ( size_t i = 0; i < spc->groupList().size() - 1; i++ )
			    for ( size_t j = i + 1; j < spc->groupList().size(); j++ )
				u += pot->g2g(p, *spc->groupList().at(i), *spc->groupList().at(j));
			return u;
		    }

		    void _sample() override
		    {
			double Vold = spc->geo.getVolume();
			double Vnew = Vold + dV;

			double uold = energy(spc->p);
			scale(Vold, Vnew);
			double unew = energy(spc->trial);
			duexp += exp(-(uold - unew));

			// restore old configuration
			for ( auto g : spc->groupList())
			    g->undo(*spc);
			spc->geo.setVolume(Vold);
			pot->setSpace(*spc);

			assert(
				fabs(uold - energy(spc->p)) < 1e-7 && "System improperly restored!");
		    }

		    string _info() override
		    {
			char w = 30;
			double pex = -log(duexp.avg()) / dV;
			using namespace Faunus::textio;
			std::ostringstream o;
			if ( cnt > 0 )
			{
			    o << pad(SUB, w, "Volume perturbation (dV)") << dV << _angstrom + cubed + "\n"
				<< pad(SUB, w, "Perturbation directions") << dir.transpose() << "\n"
				<< pad(SUB, w, "Full Hamiltonian") << std::boolalpha << fullenergy << "\n"
				<< pad(SUB, w, "Excess pressure") << pex << kT + "/" + angstrom + cubed
				<< " = " << pex / 1.0_mM << " mM = " << pex / 1.0_Pa << " Pa\n";
			}
			return o.str();
		    }

		    void _test( UnitTest &test ) override
		    {
			double pex = -log(duexp.avg()) / dV;
			test("virtualvolume_pressure_mM", pex / 1.0_mM, 0.2);
		    }

		public:
		    VirtualVolumeMove( Tmjson &j, Tenergy &pot, Tspace &spc ) : AnalysisBase(j), spc(&spc), pot(&pot)
		{
		    dV = j.at("dV");
		    dir = {1, 1, 1};  // scale directions
		    fullenergy = j["fullenergy"] | false;
		    name = "Virtual Volume Move";
		    cite = "doi:10.1063/1.472721";
		}
	    };

	/**
	 * @brief Excess chemical potential of molecules
	 *
	 * This will insert a non-perturbing ghost molecule into
	 * the system and calculate a Widom average to measure
	 * the free energy of the insertion process. The position
	 * and molecular rotation is random.
	 * For use with rod-like particles on surfaces, the `absz`
	 * keyword may be used to ensure orientations on only one
	 * half-sphere.
	 *
	 * JSON input:
	 *
	 * Keyword       | Description
	 * :------------ | :-----------------------------------------
	 * `dir`         | Inserting direction array. Default `[1,1,1]`
	 * `molecule`    | Name of molecule to insert
	 * `ninsert`     | Number of insertions per sample event
	 * `absz`        | Apply `std::fabs` on all z-coordinates of inserted molecule (default: `false`)
	 */
	template<typename Tspace>
	    class WidomMolecule : public AnalysisBase
	{
	    private:
		typedef Energy::Energybase <Tspace> Tenergy;
		Tspace *spc;
		Energy::Energybase <Tspace> *pot;
		int ninsert;
		string molecule;
		Point dir;
		int molid;
		bool absolute_z=false;

	    public:
		Average<double> expu;
		Average<double> rho;

		void _sample() override
		{
		    typedef MoleculeData<typename Tspace::ParticleVector> TMoleculeData;
		    auto rins = RandomInserter<TMoleculeData>();
		    rho += spc->numMolecules(molid) / spc->geo.getVolume();
		    rins.dir = dir;
		    rins.checkOverlap = false;
		    for ( int i = 0; i < ninsert; ++i )
		    {
			auto pin = rins(spc->geo, spc->p, spc->molecule[molid]); // ('spc->molecule' is a vector of molecules
			assert( !pin.empty() );

			if (absolute_z)
			    for (auto &p : pin)
				p.z() = std::fabs(p.z());

			double u = pot->v2v(pin, spc->p); // energy between "ghost molecule" and system in kT
			Group g = Group(0, pin.size()-1 );
			u += pot->g_external(pin, g);
			expu += exp(-u); // widom average
		    }
		}

		inline string _info() override
		{
		    using namespace Faunus::textio;
		    std::ostringstream o;
		    char w = 30;
		    if ( cnt > 0 )
		    {
			double excess = -std::log(expu.avg());
			o << pad(SUB, w, "Insertion directions") << dir.transpose() << "\n"
			    << pad(SUB, w, "Insertion molecule") << molecule << "\n"
			    << pad(SUB, w, "Particle density") << WidomMolecule::rho.avg() << angstrom + superminus + cubed
			    << "\n"
			    << pad(SUB, w, "Excess chemical potential") << excess << kT << "\n";
		    }
		    return o.str();
		}

		inline Tmjson _json() override
		{
		    using namespace Faunus::textio;
		    double excess = -std::log(expu.avg());
		    double ideal = std::log(rho.avg()); // todo: think about units -> look in other Widom class
		    return {
			{ name,
			    {
				{ "dir", vector<double>( dir ) },
				{ "molecule", molecule },
				{ "insertions", expu.cnt },
				{ "rho", rho.avg() },
				{ "rho_unit", angstrom + superminus + cubed },
				{ "absz", absolute_z },
				{ "mu",
				    {
					{"excess", excess },
					{"ideal", ideal },
					{"total", ideal+excess },
					{"unit", trim(kT) }
				    }
				}
			    }
			}
		    };
		}

		WidomMolecule( Tmjson &j, Tenergy &pot, Tspace &spc ) : spc(&spc), pot(&pot), AnalysisBase(j)
	    {
		name = "Widom Molecule";
		ninsert = j.at("ninsert");
		dir << j.value("dir", vector<double>({1,1,1}) );
		molecule = j.at("molecule");
		absolute_z = j.value("absz", false);
		// look up the id of the molecule that we want to insert
		molid = -1;
		for ( unsigned long i = 0; i < spc.molecule.size(); ++i )
		{
		    if ( spc.molecule[i].name == molecule )
		    {
			molid = (int) i;
			break;
		    }
		}
		if ( molid == -1 )
		    throw std::runtime_error(name+": invalid molecule "+molecule);

	    }
	};

	/**
	 * @brief Mean force between two groups
	 *
	 * This will average the mean force along the mass center
	 * connection line between two groups. Currently, results
	 * are output only via the `json()` function.
	 *
	 * JSON keys | Description
	 * --------- | ---------------
	 *  `groups` | Exactly two group index (array)
	 *  `nsteps` | Sample interval
	 *
	 * To avoid calling virtual functions in the inner loop of
	 * the force calculation, we use `std::bind` to bind
	 * the force method which is created at compile time. Care should
	 * be taken when copying the class as described below.
	 *
	 * @warning When copying this class the bound function `func` will still refer
	 * to the initial class incl. mf1, mf2, pot etc. Fix this by implement copy
	 * operator. Not a problem when handled by `CombinedEnergy` where only a single
	 * copy is made.
	 */
	class MeanForce : public AnalysisBase {

	    void _sample() override;
	    Tmjson _json() override;

	    Average<double> mf1, mf2; // mean force on the two groups
	    int g1, g2; // group index in Space group list

	    std::function<void()> func;

	    template<class Tspace, class Tenergy>
		void evalforce(Tspace &spc, Tenergy &pot) {
		    Point f1 = {0,0,0}; // net force on group 1
		    Point f2 = {0,0,0}; // net force on group 2
		    auto &g = spc.groupList(); // list of groups
		    auto &p = spc.p;           // particle vector

		    // force all others, k <-> g1 and g2
		    for (int k=0; k!=(int)g.size(); k++)
			if (k!=g1)
			    if (k!=g2)
				for (auto i : *g[k]) {
				    for (auto j : *g[g1])
					f1 += pot.f_p2p( p[i], p[j] );
				    for (auto j : *g[g2])
					f2 += pot.f_p2p( p[i], p[j] );
				}
		    // force g1<->g2
		    for (auto i : *g[g2])
			for (auto j : *g[g1]) {
			    Point f = pot.f_p2p( p[i], p[j] );
			    f1 += f;
			    f2 -= f;
			}

		    // COM-COM unit vector and mean force
		    Point r = spc.geo.vdist( g[g1]->cm, g[g2]->cm );
		    mf1 += f1.dot( r/r.norm() );
		    mf2 += f2.dot( -r/r.norm() );
		    cout << mf1.cnt << " " << mf2.cnt << "\n";
		}

	    public:
	    template<class Tspace, class Tenergy>
		MeanForce( Tmjson j, Tenergy &pot, Tspace &spc ) : AnalysisBase(j)
	    {
		name = "Mean force";
		g1 = g2 = -1;
		if (j.at("groups").is_array()) {
		    vector<size_t> v = j["groups"];
		    if (v.size()==2) {
			g1 = v[0];
			g2 = v[1];
			if (g1>=0)
			    if (g2>=0)
				if (g1!=g2)
				    if (g1 < (int)spc.groupList().size() )
					if (g2 < (int)spc.groupList().size() ) {
					    func = std::bind( &MeanForce::evalforce<Tspace,Tenergy>,
						    this, std::ref(spc), std::ref(pot));
					    return;
					}
		    }
		}
		throw std::runtime_error(name + ": group array must contain \
			exactly two distinct and valid group index");
	    }
	};

	/** @brief Save system energy to disk. Keywords: `nstep`, `file` */
	class SystemEnergy : public AnalysisBase {

	    std::ofstream f;
	    std::function<double()> energy;

	    void _sample() override;

	    public:
	    template<class Tspace, class Tenergy>
		SystemEnergy( Tmjson j, Tenergy &pot, Tspace &spc ) : AnalysisBase(j)
	    {
		name = "System energy";
		string file = j.at("file");
		f.open( file );
		if (!f)
		    throw std::runtime_error(name + ": cannot open output file " + file);

		energy = [&spc, &pot]() { return Energy::systemEnergy(spc, pot, spc.p); };
	    }
	};

	/** @brief Save system energy to disk. Keywords: `nstep`, `file` */
	class PropertyTraj : public AnalysisBase {

	    std::ofstream f;
	    typedef std::function<double()> Tf;
	    std::vector<Tf> v;

	    void _sample() override;

	    public:
	    template<class Tspace, class Tenergy>
		PropertyTraj( Tmjson j, Tenergy &pot, Tspace &spc ) : AnalysisBase(j)
	    {
		name = "Property Trajectory";
		string file = j.at("file");
		f.open( file );
		if (!f)
		    throw std::runtime_error(name + ": cannot open output file " + file);

		if (j.value("energy", false)) {
		    f << "# U/kT ";
		    v.push_back( [&spc, &pot]() { return Energy::systemEnergy(spc, pot, spc.p); } );
		}
		auto mol = j.value("confindex", vector<string>() );
		if (!mol.empty())
		{
		    f << "confid ";
		    //for (auto &i : mol)
		    //{
		    //v.push_back( [&spc]() { return spc.molecule[i].getConformationIndex(); } );
		    //}
		}
	    }
	};


	/**
	 * @brief Base class for distribution functions etc.
	 */
	class PairFunctionBase : public AnalysisBase {

	    protected:
		struct data {
		    int dim;
		    double dr;
		    Table2D<double,double> hist;
		    Table2D<double,Average<double>> hist2;
		    string name1, name2, file, file2;
		    double Rhypersphere; // Radius of 2D hypersphere
		};
		std::vector<data> datavec;        // vector of data sets
		Average<double> V;                // average volume (angstrom^3)
		virtual void normalize(data &);
	    private:
		virtual void update(data &d)=0;   // called on each defined data set
		void _sample() override;
		Tmjson _json() override;

	    public:
		PairFunctionBase(Tmjson, string);
		virtual ~PairFunctionBase();
	};

	/**
	 * @brief Atomic radial distribution function, g(r)
	 *
	 * We sample the pair correlation function between atom id's _i_ and _j_,
	 * \f[
	 * g_{ij}(r) = \frac{ N_{ij}(r) }{ \sum_{r=0}^{\infty} N_{ij}(r) } \cdot \frac{ \langle V \rangle }{ V(r) }
	 * \f]
	 *
	 * where \f$ N_{ij}(r) \f$ is the number of observed pairs, accumulated over the
	 * entire ensemble,  in the separation
	 * interval \f$[r, r+dr] \f$ and \f$ V(r) \f$ is the corresponding volume element
	 * which depends on dimensionality:
	 *
	 * \f$ V(r) \f$               | Dimensions (`dim`)
	 * :------------------------- | :----------------------------------------
	 * \f$ 4\pi r^2 dr \f$        | 3 (for particles in free space, default)
	 * \f$ 2\pi r dr \f$          | 2 (for particles confined on a plane)
	 * \f$ 2\pi R sin(r/R) dr \f$ | 2 (for particles confined on a 2D hypersphere surface, also needs input `Rhypersphere`)
	 * \f$ dr \f$                 | 1 (for particles confined on a line)
	 *
	 * Example JSON input:
	 *
	 *     { "nstep":20, "pairs":
	 *        [
	 *          { "name1":"Na", "name2":"Cl", "dim":3, "dr":0.1, "file":"rdf-nacl.dat"},
	 *          { "name1":"Na", "name2":"Na", "dim":3, "dr":0.1, "file":"rdf-nana.dat"}
	 *        ]
	 *     }
	 */
	template<class Tspace>
	    class AtomRDF : public PairFunctionBase {
		Tspace &spc;
		void update(data &d) override
		{
		    V += spc.geo.getVolume( d.dim );
		    int N = spc.p.size();
		    int id1 = atom[ d.name1 ].id;
		    int id2 = atom[ d.name2 ].id;
		    for (int i=0; i<N-1; i++)
			for (int j=i+1; j<N; j++)
			    if (
				    ( spc.p[i].id==id1 && spc.p[j].id==id2 ) ||
				    ( spc.p[i].id==id2 && spc.p[j].id==id1 )
			       )
			    {
				double r = spc.geo.dist( spc.p[i], spc.p[j] );
				d.hist(r)++;
			    }
		}

		public:
		AtomRDF( Tmjson j, Tspace &spc ) : PairFunctionBase(j,
			"Atomic Pair Distribution Function"), spc(spc) {}

		~AtomRDF()
		{
		    for (auto &d : datavec) {
			normalize(d);
			d.hist.save( d.file );
		    }
		}
	    };

	/** @brief Same as `AtomRDF` but for molecules. Identical input. */
	template<class Tspace>
	    class MoleculeRDF : public PairFunctionBase {
		Tspace &spc;
		void update(data &d) override
		{
		    V += spc.geo.getVolume( d.dim );
		    auto g1 = spc.findMolecules( d.name1 );
		    auto g2 = spc.findMolecules( d.name2 );

		    if (d.name1!=d.name2)
			for (auto i : g1)
			    for (auto j : g2) {
				double r = spc.geo.dist( i->cm, j->cm );
				d.hist(r)++;
			    }
		    else {
			for (int i=0; i<(int)g1.size()-1; i++)
			    for (int j=i+1; j<(int)g1.size(); j++) {
				double r = spc.geo.dist( g1[i]->cm, g1[j]->cm );
				d.hist(r)++;
			    }
		    }
		}

		public:
		MoleculeRDF( Tmjson j, Tspace &spc ) : PairFunctionBase(j,
			"Molecular Pair Distribution Function"), spc(spc) {}

		~MoleculeRDF()
		{
		    for (auto &d : datavec) {
			normalize(d);
			d.hist.save( d.file );
		    }
		}
	    };

	template<class Tspace>
	    class MoleculeMumu : public PairFunctionBase {
		Tspace &spc;
		void update(data &d) override
		{
		    V += spc.geo.getVolume( d.dim );
		    auto g1 = spc.findMolecules( d.name1 );
		    auto g2 = spc.findMolecules( d.name2 );

		    if (d.name1!=d.name2)
			for (auto i : g1) {
			    Point mu1 = Geometry::dipoleMoment(spc,*i);
			    double mu1a = mu1.norm();
			    for (auto j : g2) {
				Point mu2 = Geometry::dipoleMoment(spc,*j);
				double mu2a = mu2.norm();
				double r = spc.geo.dist( i->cm, j->cm );
				d.hist(r)++;
				d.hist2(r) += mu1.dot(mu2)/mu1a/mu2a;
			    }
			}
		    else {
			for (int i=0; i<(int)g1.size()-1; i++) {
			    Point mu1 = Geometry::dipoleMoment(spc,*g1[i]);
			    double mu1a = mu1.norm();
			    for (int j=i+1; j<(int)g1.size(); j++) {
				Point mu2 = Geometry::dipoleMoment(spc,*g1[j]);
				double mu2a = mu2.norm();
				double r = spc.geo.dist( g1[i]->cm, g1[j]->cm );
				d.hist(r)++;
				d.hist2(r) += mu1.dot(mu2)/mu1a/mu2a;
			    }
			}
		    }
		}

		public:
		MoleculeMumu( Tmjson j, Tspace &spc ) : PairFunctionBase(j,
			"Molecular Pair Distribution Functions (g(r),mumu(r))"), spc(spc) {}

		~MoleculeMumu()
		{
		    for (auto &d : datavec) {
			normalize(d);
			d.hist.save( d.file );
			d.hist2.save( d.file2 );
		    }
		}
	    };

	/**
	 * @brief We sample the following pair functions between atom id's _i_ and _j_: the distant-dependent Kirkwood-factor
	 * 
	 * \f[
	 * ...
	 * \f]
	 * 
	 * along the dipole correlation (extention ".dipolecorr")
	 * 
	 * \f[
	 * \langle\hat{\mu}(0) \cdot \hat{\mu}(r)\rangle,
	 * \f]
	 * 
	 * the dipole correlation (extention ".dipoleint")
	 * 
	 * \f[
	 * \langle\frac{1}{2} ( 3 \hat{\mu}(0) \cdot \hat{\mu}(r) - 1 )\rangle,
	 * \f]
	 * 
	 * and also a dipole angle distribution (extention ".angledist")
	 * 
	 * \f[
	 * \hat{\mu}(0) \cdot \hat{\mu}(r).
	 * \f]
	 *
	 * Here \f$ \hat{\mu} \f$ is a unit dipole moment vector.
	 * 
	 * Input          | 
	 * :------------- | :----------------------------------------
	 * `name1`        | name of particle type 1
	 * `name2`        | name of particle type 2
	 * `dr`           | size of bins in analysis
	 * `file`         | filename where data is saved
	 *
	 * Example JSON input:
	 *
	 *     { "nstep":20, "pairs":
	 *        [
	 *          { "name1":"dip", "name2":"dip", "dr":0.1, "file":"rdf-dipdip.dat"}
	 *        ]
	 *     }
	 * 
	 */
	template<class Tspace>
	    class KirkwoodFactor : public PairFunctionBase {
		Tspace &spc;

		Table2D<double, double> mucorr_angle;
		Table2D<double, Average<double> > mucorr_dist;
		std::vector<data> datavec2;        // vector of data sets, for later use (mucorr_angle,mucorr_dist)

		public:
		void update( data &d ) override
		{
		    int N = spc.p.size() - 1;
		    int id1 = atom[ d.name1 ].id;
		    int id2 = atom[ d.name2 ].id;
		    for ( int i = 0; i < N; i++ )
		    {
			if ( spc.p[i].id==id1 || spc.p[i].id==id2 )
			    d.hist(0) += spc.p[i].mu().dot(spc.p[i].mu()) * spc.p[i].muscalar() * spc.p[i].muscalar();
			for ( int j = i + 1.; j < N + 1; j++ )
			{
			    if ( ( spc.p[i].id==id1 && spc.p[j].id==id2 ) || ( spc.p[i].id==id2 && spc.p[j].id==id1 ) )
			    {
				double r = spc.geo.dist(spc.p[i], spc.p[j]);
				double sca = spc.p[i].mu().dot(spc.p[j].mu());
				mucorr_angle(sca) += 1.;
				d.hist2(r) += sca;
				mucorr_dist(r) += 0.5 * (3 * sca * sca - 1.);
				d.hist(r) += 2 * sca * spc.p[i].muscalar() * spc.p[j].muscalar();
			    }
			}
		    }
		    if ( spc.p[N].id==id1 || spc.p[N].id==id2 )
			d.hist(0) += spc.p[N].mu().dot(spc.p[N].mu()) * spc.p[N].muscalar() * spc.p[N].muscalar();
		}

		void normalize(data &d) override
		{
		    double sum = 0;  
		    for (auto &i : d.hist.getMap()) {
			sum += i.second;
			i.second = sum;
		    }
		}

		KirkwoodFactor( Tmjson j, Tspace &spc ) : PairFunctionBase(j,"KirkwoodFactor"), spc(spc) {
		    mucorr_angle.setResolution(datavec.back().dr*0.1); // Interval goes only from -1 to 1, thus we generally must increase the resolution, hence the factor of 0.1
		    mucorr_dist.setResolution(datavec.back().dr);
		}

		~KirkwoodFactor()
		{
		    for (auto &d : datavec) {
			normalize(d);
			d.hist.save( d.file );
			d.hist2.save( d.file2 );
			mucorr_angle.save( d.file+".angledist" );
			mucorr_dist.save( d.file+".dipoleint" );
		    }
		}
	    };

	/**
	 * @brief We sample multipole aspects of the system. Firstly we sample
	 * 
	 * \f[
	 * \langle M_{\alpha}\rangle = \sum_{i=1}^N\left(r_{i,\alpha}q_i  \mu_{i,\alpha}\right)
	 * \f]
	 * 
	 * where \f[ \alpha \in \{x,y,z \} \f], \f[ N \f] is the sumber of particles in the system, \f[ r_{i,\alpha} \f] 
	 * is the \f[ \alpha \f]-component of particle \f[ i \f]'s position, and \f[ \mu_{i,\alpha} \f] is its dipole moment.
	 * Both a spherical volume around the origin and the total volume is sampled. We also sample
	 * 
	 * \f[
	 * \langle {\bf M}\cdot {\bf M}\rangle
	 * \f]
	 * 
	 * again for both a spherical volume around the origin and the total volume. Finally we also get the dielectric constant.
	 * 
	 * Input           | 
	 * :-------------- | :----------------------------------------
	 * `cutoff`        | Cutoff for spherical analysis
	 * `dielectric`    | Dielectric type 
	 * `kappa`         | Wolf-damping parameter. Only needed if dielectric type is `wolf`
	 * `eps_rf`        | Dielectric constant of the surrounding. If `eps_rf` < 0 then insulating boundary conditions will be used.
	 * 
	 * The dielectric type is either: `tinfoil` for conducting boundary conditions, `rf` for reaction-field 
	 * using insulating boundary conditions, or `wolf` for electrostatics using the Wolf-formalism. See DOI:http://doi.org/6v3
	 * for more details.
	 *
	 * Example JSON input:
	 *
	 *      "multipoleanalysis" : { "nstep":20, "cutoff":15.0, "dielectric":"tinfoil", 'pairs' :
	 *	  [
	 *	      { 'name1':'water', 'name2':'water', 'dim':3, 'file':'rdf_ww.dat', 'file2':'mucorr_ww.dat', 'dr':0.05  }
	 *	  ] 
	 *	}
	 * 
	 * @note The molecular rdf is retrieved for free and is thus unnecessary to get separately
	 */
	template<class Tspace>
	    class MultipoleAnalysis : public PairFunctionBase {
		Tspace &spc;
		Average<double> M2, M2_box, diel, V_t, groupDipole;
		vector<Average<double>> Q, Q_box, mu_abs, M, M_box;
		int atomsize;
		double rc, rc2, const_Diel, alpha, eps_rf;
		string type;
		std::function<double(double)> calcDielectric; // function for dielectric const. calc.

		void _sample() override
		{

		    for (auto &d : datavec)
			update(d);

		    V_t += spc.geo.getVolume();

		    // Updates from point multipoles
		    Point origin(0, 0, 0);
		    Point mu(0, 0, 0);        // In e\AA
		    Point mu_box(0, 0, 0);    // In e\AA
		    Tensor<double> quad;
		    Tensor<double> quad_box;

		    for ( auto &i : spc.p )
		    {
			if ( spc.geo.sqdist(i, origin) < rc2 )
			{
			    mu += i.mu() * i.muscalar();
			    quad += i*i.mu().transpose()*i.muscalar();
			    quad += i.theta();
			}
			else
			{
			    mu_box += i.mu() * i.muscalar();
			    quad_box += i*i.mu().transpose()*i.muscalar();
			    quad_box += i.theta();
			}
			for ( int j = 0; j < atomsize - 1; j++ )
			    if ( int(i.id) == j + 1 )
				mu_abs[j] += i.muscalar();
		    }
		    mu_box += mu;
		    quad_box += quad;

		    // Updates from point charges
		    Group all(0, spc.p.size() - 1);
		    all.setMassCenter(spc);

		    mu += Geometry::dipoleMoment(spc, all, rc);
		    mu_box += Geometry::dipoleMoment(spc, all);

		    quad += Geometry::quadrupoleMoment(spc, all, rc);
		    quad_box += Geometry::quadrupoleMoment(spc, all);

		    // Update system statistics
		    for(int i = 0; i < 3; i++) {
			M.at(i) += mu[i];
			M_box.at(i) += mu_box[i];
		    }

		    int cnt = 0;
		    for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
			    Q.at(cnt) += quad(i,j);
			    Q_box.at(cnt++) += quad_box(i,j);
			}
		    }

		    double sca = mu.dot(mu);
		    M2 += sca;
		    sca = mu_box.dot(mu_box);
		    M2_box += sca;
		    diel.add(getDielectricConstant());

		    // Update group statistics
		    double mus_group = 0.0;
		    for ( auto gi : spc.groupList())
		    {
			Point m = Geometry::dipoleMoment(spc, *gi);
			for ( auto i : *gi )
			    m += spc.p[i].muscalar() * spc.p[i].mu();
			mus_group += m.norm();
		    }
		    groupDipole += (mus_group / double(spc.groupList().size()));
		}

		void update(data &d) override
		{
		    V += spc.geo.getVolume( d.dim );
		    auto g1 = spc.findMolecules( d.name1 );

		    if (d.name1!=d.name2) {
			auto g2 = spc.findMolecules( d.name2 );
			for (auto i : g1) {
			    Point mi = Geometry::dipoleMoment(spc, *i);
			    for ( auto a : *i )
				mi += spc.p[a].muscalar() * spc.p[a].mu();
			    for (auto j : g2) {
				Point mj = Geometry::dipoleMoment(spc, *j);
				for ( auto b : *j )
				    mj += spc.p[b].muscalar() * spc.p[b].mu();
				double sca = mi.dot(mj);
				double r = spc.geo.dist( i->cm, j->cm );
				d.hist(r)++;
				d.hist2(r) += sca;
			    }
			}
		    } else {
			for (int i=0; i<(int)g1.size()-1; i++) {
			    Point mi = Geometry::dipoleMoment(spc, *g1[i]);
			    for ( auto a : *g1[i] )
				mi += spc.p[a].muscalar() * spc.p[a].mu();
			    for (int j=i+1; j<(int)g1.size(); j++) {
				Point mj = Geometry::dipoleMoment(spc, *g1[j]);
				for ( auto b : *g1[j] )
				    mj += spc.p[b].muscalar() * spc.p[b].mu();
				double sca = mi.dot(mj);
				double r = spc.geo.dist(g1[i]->cm, g1[j]->cm);
				d.hist(r)++;
				d.hist2(r) += sca;
			    }
			}
		    }
		}

		public:
		MultipoleAnalysis( Tmjson j, Tspace &spc ) : PairFunctionBase(j,"MultipoleAnalysis"), spc(spc) 
		{
		    alpha = j.value("alpha", 0.0);
		    rc = j.at("cutoff");
		    rc2 = rc*rc;
		    type = j.at("dielectric");
		    if ( type =="tinfoil") {
			calcDielectric = [&](double M2V) { return 1 + 3*M2V; };
		    } else if ( type =="reactionfield") {
			eps_rf = j.at("eps_rf");
			calcDielectric = [&](double M2V) { if(eps_rf < 0.0)
			    return ( 2.25*M2V + 0.25 + 0.75*sqrt(9.0*M2V*M2V + 2.0*M2V + 1.0) );
			    return (6*M2V*eps_rf + 2*eps_rf + 1.0)/(1.0 + 2*eps_rf - 3*M2V); };
		    } else if ( type =="wolf") {
			calcDielectric = [&](double M2V) { double T = erf(alpha*rc) - (2 / (3 * sqrt(pc::pi))) * exp(-alpha*alpha*rc*rc) * ( 2.0 * alpha*alpha*rc*rc + 3.0);
			    return (((T + 2.0) * M2V + 1.0)/ ((T - 1.0) * M2V + 1.0));};
		    } else if ( type =="fennel") {
			calcDielectric = [&](double M2V) { double T = erf(alpha*rc) - (2 / (3 * sqrt(pc::pi))) * exp(-alpha*alpha*rc*rc) * (alpha*alpha*rc*rc * alpha*alpha*rc*rc + 2.0 * alpha*alpha*rc*rc + 3.0);
			    return (((T + 2.0) * M2V + 1.0)/ ((T - 1.0) * M2V + 1.0)); };
		    } else if( type == "fanourgakis") {
			calcDielectric = [&](double M2V) { return 1 + 3*M2V; };
		    } else if( type == "yonezawa") {
			calcDielectric = [&](double M2V) { return 1 + 3*M2V; };
		    } else if( type == "plain") {
			calcDielectric = [&](double M2V) { return (2.0*M2V + 1.0)/(1.0 - M2V); };
		    } else {
			throw std::runtime_error(name + " error: invalid dielectric type");
		    }
		    const_Diel = pc::e * pc::e * 1e10 / (9.0 * pc::kT() * pc::e0);
		    atomsize = atom.size();
		    mu_abs.resize(atomsize-1);

		    M.resize(3);
		    M_box.resize(3);
		    Q.resize(9);
		    Q_box.resize(9);

		    for(int i = 0; i < 3; i++) {
			M.at(i).reset();
			M_box.at(i).reset();
		    }
		    for(int i = 0; i < 9; i++) {
			Q.at(i).reset();
			Q_box.at(i).reset();
		    }
		}  

		~MultipoleAnalysis()
		{
		    for (auto &d : datavec) {
			normalize(d);
			d.hist.save( d.file );
			d.hist2.save( d.file2 );
		    }
		}

		double getDielectricConstant() {
		    double M2V = M2_box.avg() * const_Diel / V_t.avg();
		    return calcDielectric(M2V);
		}

		Tmjson _json() override
		{
		    return {
			{ name,
			    {
				{ "<M_x>_SPHERE", M[0].avg() },
				{ "<M_y>_SPHERE", M[1].avg() },
				{ "<M_z>_SPHERE", M[2].avg() },
				{ "<M_x>_BOX", M_box[0].avg() },
				{ "<M_y>_BOX", M_box[1].avg() },
				{ "<M_z>_BOX", M_box[2].avg() },
				{ "<M^2>_SPHERE", M2.avg() },
				{ "<M^2>_BOX", M2_box.avg() },
				{ "<Q_xx>_SPHERE", Q.at(0).avg() },
				{ "<Q_xy>_SPHERE", Q.at(1).avg() },
				{ "<Q_xz>_SPHERE", Q.at(2).avg() },
				{ "<Q_yx>_SPHERE", Q.at(3).avg() },
				{ "<Q_yy>_SPHERE", Q.at(4).avg() },
				{ "<Q_yz>_SPHERE", Q.at(5).avg() },
				{ "<Q_zx>_SPHERE", Q.at(6).avg() },
				{ "<Q_zy>_SPHERE", Q.at(7).avg() },
				{ "<Q_zz>_SPHERE", Q.at(8).avg() },
				{ "<Q_xx>_BOX", Q_box.at(0).avg() },
				{ "<Q_xy>_BOX", Q_box.at(1).avg() },
				{ "<Q_xz>_BOX", Q_box.at(2).avg() },
				{ "<Q_yx>_BOX", Q_box.at(3).avg() },
				{ "<Q_yy>_BOX", Q_box.at(4).avg() },
				{ "<Q_yz>_BOX", Q_box.at(5).avg() },
				{ "<Q_zx>_BOX", Q_box.at(6).avg() },
				{ "<Q_zy>_BOX", Q_box.at(7).avg() },
				{ "<Q_zz>_BOX", Q_box.at(8).avg() },
				{ "dielectric constant("+type+")", diel.avg() },
				{ "dielectric constant std:", diel.stdev() },
				{ "<mu>", groupDipole.avg() }
			    }
			}
		    };
		}
	    };

	template<class Tspace>
	    class Capanalysis : public PairFunctionBase {
		Tspace &spc;

		Table2D<double, double> table_rv1_a, table_rv2_a, table_v1v2_a;
		Table2D<double, Average<double> > table_rv1_b, table_rv1_c, table_rv2_b, table_rv2_c, table_v1v2_b, table_v1v2_c;
		Table3D<double, double > hist_3d;
		Table2D<double, double > spec, unspec, unbound, filled, faceface;
		double rcA_s, rcB_s;

		void update( data &d ) override
		{
		    V += spc.geo.getVolume( d.dim );
		    double cntC = 0.0;
		    double cntS = 0.0;
		    double cntU = 0.0;
		    double cntUB = 0.0;
		    double cntFaceface = 0.0;
		    int N = spc.p.size();
		    int id1 = atom[ d.name1 ].id;
		    int id2 = atom[ d.name2 ].id;
		    double rcA = rcA_s*( atom[ d.name1 ].radius + atom[ d.name2 ].radius );
		    double rcB = rcB_s*( atom[ d.name1 ].radius + atom[ d.name2 ].radius );

		    int cnt = 0;
		    for ( int i = 0; i < N-1; i++ )
		    {
			//if ( spc.p[i].id==id1 || spc.p[i].id==id2 )
			//    d.hist(0) += spc.p[i].cap_center_point().dot(spc.p[i].cap_center_point()) * spc.p[i].cap_center() * spc.p[i].cap_center();
			for ( int j = i + 1.; j < N; j++ )
			{
			    if ( ( spc.p[i].id==id1 && spc.p[j].id==id2 ) || ( spc.p[i].id==id2 && spc.p[j].id==id1 ) )
			    {
				cnt++;
				Point r = spc.geo.vdist(spc.p[i], spc.p[j]);
				double r1 = r.norm();
				d.hist(r1)++;

				if(r1 > rcA)
				    continue;

				double rv1 = -r.dot(spc.p[i].cap_center_point())/r1;
				double rv2 = r.dot(spc.p[j].cap_center_point())/r1;
				double v1v2 = spc.p[i].cap_center_point().dot(spc.p[j].cap_center_point());

				table_rv1_a(rv1) += 1.;
				table_rv1_b(r1) += rv1;
				table_rv1_c(r1) += rv1 * rv1;

				table_rv2_a(rv2) += 1.;
				table_rv2_b(r1) += rv2;
				table_rv2_c(r1) += rv2 * rv2;

				table_v1v2_a(v1v2) += 1.;
				table_v1v2_b(r1) += v1v2;
				table_v1v2_c(r1) += v1v2 * v1v2;

				hist_3d(r1,v1v2) += 1.0;

				if(r1 > rcB) {
				    cntUB += 1.0;
				    continue;
				}
				if(r1 < spc.p[i].radius + spc.p[j].radius) {
				    cntC += 1.0; 
				}

				if( spc.p[i].is_sphere() ) {
				    if( !spc.p[j].is_sphere() ) {

					// i = sphere, j = cap
					if(pc::pi - spc.p[j].angle_p() > rv2) {
					    cntS += 1.0;
					} else {
					    cntU += 1.0;
					}

				    }
				} else {
				    if( spc.p[j].is_sphere() ) {

					// i = cap, j = sphere
					if(pc::pi - spc.p[i].angle_p() > rv1) {
					    cntS += 1.0;
					} else {
					    cntU += 1.0;
					}

				    } else {

					// i = cap, j = cap
					if(( pc::pi - spc.p[j].angle_p() > rv2) && ( pc::pi - spc.p[i].angle_p() > rv1 ) )
					    cntFaceface += 1.0;

					if(( pc::pi - spc.p[j].angle_p() > rv2) || ( pc::pi - spc.p[i].angle_p() > rv1 ) ) {
					    cntS += 1.0;
					} else {
					    cntU += 1.0;
					}

				    }
				}
			    }
			}
		    }
		    cntC /= double(cnt);
		    cntS /= double(cnt);
		    cntU /= double(cnt);
		    cntUB /= double(cnt);
		    cntFaceface /= double(cnt);

		    filled(cntC) += 1.0;
		    spec(cntS) += 1.0;
		    unspec(cntU) += 1.0;
		    unbound(cntUB) += 1.0;
		    faceface(cntFaceface) += 1.0;
		}

		/*
		   void normalize(data &d) override
		   {
		   double sum = 0;  
		   for (auto &i : d.hist.getMap()) {
		   sum += i.second;
		   i.second = sum;
		   }
		   }
		 */

		public:
		Capanalysis( Tmjson j, Tspace &spc ) : PairFunctionBase(j,"Capanalysis"), spc(spc) {
		    rcA_s = ( j.at("cutoffAnalysis") );
		    rcB_s = ( j.at("cutoffBound") );
		    spec.setResolution(datavec.back().dr);
		    unspec.setResolution(datavec.back().dr);
		    unbound.setResolution(datavec.back().dr);
		    filled.setResolution(datavec.back().dr);
		    faceface.setResolution(datavec.back().dr);

		    table_rv1_a.setResolution(datavec.back().dr*0.1); // Interval goes only from -1 to 1, thus we must increase the resolution, hence the factor of 0.1
		    table_rv1_b.setResolution(datavec.back().dr);
		    table_rv1_c.setResolution(datavec.back().dr);
		    table_rv2_a.setResolution(datavec.back().dr*0.1); // Interval goes only from -1 to 1, thus we must increase the resolution, hence the factor of 0.1
		    table_rv2_b.setResolution(datavec.back().dr);
		    table_rv2_c.setResolution(datavec.back().dr);
		    table_v1v2_a.setResolution(datavec.back().dr*0.1); // Interval goes only from -1 to 1, thus we must increase the resolution, hence the factor of 0.1
		    table_v1v2_b.setResolution(datavec.back().dr);
		    table_v1v2_c.setResolution(datavec.back().dr);
		    hist_3d.setResolution(datavec.back().dr,datavec.back().dr);
		}

		~Capanalysis()
		{
		    for (auto &d : datavec) {
			normalize(d);
			d.hist.save( d.file+".rdf" );
			table_rv1_a.save( d.file+".rv1a" );
			table_rv1_b.save( d.file+".rv1b" );
			table_rv1_c.save( d.file+".rv1c" );
			table_rv2_a.save( d.file+".rv2a" );
			table_rv2_b.save( d.file+".rv2b" );
			table_rv2_c.save( d.file+".rv2c" );
			table_v1v2_a.save( d.file+".v1v2a" );
			table_v1v2_b.save( d.file+".v1v2b" );
			table_v1v2_c.save( d.file+".v1v2c" );
			spec.normSave(d.file+".spec");
			unspec.normSave(d.file+".unspec");
			unbound.normSave(d.file+".unbound");
			filled.normSave(d.file+".filled");
			faceface.normSave(d.file+".faceface");

			hist_3d.save( d.file+".3d" );
		    }
		}
	    };

	/**
	 * @brief Sample scattering intensity
	 *
	 * A list of groups can be specified and the Debye formula is
	 * used to calculate the isotropic scattering. If `com` is true
	 * (default) only the mass center of molecular groups will be
	 * considered as a _single_ scattering site.
	 *
	 * Keyword   | Description
	 * :-------- | :------------------
	 * `nstep`   | Interval with which to sample (default: 1)
	 * `mollist` | List of molecule name to sample (array)
	 * `com`     | Use molecular mass center (bool, default: true)
	 * `qmin`    | Minimum q value (1/angstrom)
	 * `qmax`    | Maximum q value (1/angstrom)
	 * `dq`      | q spacing (1/angstrom)
	 */
	template<class Tspace, class Tformfactor=Scatter::FormFactorUnity<double>>
	    class ScatteringFunction : public AnalysisBase {
		private:

		    Tspace &spc;
		    vector<Point> p; // vector of scattering points
		    vector<string> mollist; // molecules to consider
		    bool usecom; // scatter from mass center, only?
		    string filename; // output file name of S(q) file

		    Scatter::DebyeFormula<Tformfactor> debye;

		    void _sample() override {
			p.clear();
			for (auto &name : mollist) { // loop over molecule names
			    auto groups = spc.findMolecules(name);
			    for (auto g : groups) // loop over groups
				if (usecom && g->isMolecular())
				    p.push_back( g->cm );
				else
				    for (auto i : *g) // loop over particle index in group
					p.push_back( spc.p[i] );
			}
			debye.sample( p, spc.geo.getVolume() );
		    }

		    Tmjson _json() override
		    {
			return {
			    { name,
				{
				    { "mollist", mollist },
				    { "com", usecom }
				}
			    }
			};
		    }

		public:

		    ScatteringFunction( Tmjson &j, Tspace &spc ) try :
			AnalysisBase(j, "Debye Formula Scattering"), spc(spc), debye(j) {
			    usecom = j.value("com", true);
			    mollist = j.at("mollist").get<decltype(mollist)>();
			    filename = j.at("file");
			}
		    catch( std::exception &e ) {
			std::cerr << "Debye Formula Scattering: ";
			throw;
		    }

		    ~ScatteringFunction() {
			debye.save( filename );
		    }
	    };

	/**
	 * @brief Class for accumulating analysis classes
	 *
	 * Upon construction the JSON section `analysis` is searched
	 * for the keywords below to activate analysis functions.
	 * All analysis classes take the JSON keyword `nstep` for
	 * specifying how many microloops to count between
	 * each analysis call.
	 * Aggregated results of all analysis function are saved
	 * to a JSON file upon descruction. Use `_jsonfile` to
	 * control the output file name.
	 *
	 * Keyword                 |  Description
	 * :---------------------  |  :----------------------------
	 * `atomrdf`               |  `Analysis::AtomRDF`
	 * `chargemultipole`       |  `Analysis::ChargeMultipole`
	 * `energyfile`            |  `Analysis::SystemEnergy`
	 * `kirkwoodfactor`        |  `Analysis::KirkwoodFactor`
	 * `capanalysis`           |  `Analysis::Capanalysis`
	 * `meanforce`             |  `Analysis::MeanForce`
	 * `molrdf`                |  `Analysis::MoleculeRDF`
	 * `multipoleanalysis`     |  `Analysis::MultipoleAnalysis`
	 * `multipoledistribution` |  `Analysis::MultipoleDistribution`
	 * `polymershape`          |  `Analysis::PolymerShape`
	 * `propertytraj`          |  `Analysis::PropertyTraj`
	 * `scatter`               |  `Analysis::ScatteringFunction`
	 * `virial`                |  `Analysis::VirialPressure`
	 * `virtualvolume`         |  `Analysis::VirtualVolumeMove`
	 * `widom`                 |  `Analysis::Widom`  
	 * `widommolecule`         |  `Analysis::WidomMolecule`
	 * `widomscaled`           |  `Analysis::WidomScaled`
	 * `xtctraj`               |  `Analysis::XTCtraj`
	 * `pqrfile`               |  Save PQR file at end of simulation, i.e. `"pqrfile" : {"file":"conf.pqr"}`
	 * `aamfile`               |  Save AAM file at end of simulation, i.e. `"aamfile" : {"file":"conf.aam"}`
	 * `statefile`             |  Save state file at end of simulation, i.e. `"statefile" : {"file":"state"}`
	 * `_jsonfile`             |  Ouput json file w. collected results (default: analysis_out.json)
	 */
	class CombinedAnalysis : public AnalysisBase
	{
	    private:
		typedef std::shared_ptr<AnalysisBase> Tptr;
		vector <Tptr> v;
		string _info() override;
		void _sample() override;
		string jsonfile;
	    public:

		template<class Tspace, class Tpotential>
		    CombinedAnalysis( Tmjson &j, Tpotential &pot, Tspace &spc )
		    {
			using std::ref;
			using std::placeholders::_1;

			auto &m = j.at("analysis");

			jsonfile = m.value("_jsonfile", "analysis_out.json");

			for ( auto i = m.begin(); i != m.end(); ++i )
			{
			    auto &val = i.value();

			    try {
				if ( i.key() == "xtcfile" )
				    v.push_back(Tptr(new XTCtraj<Tspace>(val, spc)));

				if ( i.key() == "pqrfile" )
				{
				    auto writer = std::bind(
					    []( string file, Tspace &s ) { FormatPQR::save(file, s.p, s.geo.inscribe().len); },
					    _1, ref(spc));
				    v.push_back(Tptr(new WriteOnceFileAnalysis(val, writer)));
				}

				if ( i.key() == "aamfile" )
				{
				    auto writer = std::bind(
					    []( string file, Tspace &s ) { FormatAAM::save(file, s.p); },
					    _1, ref(spc));
				    v.push_back(Tptr(new WriteOnceFileAnalysis(val, writer)));
				}

				if ( i.key() == "statefile" )
				{
				    auto writer = std::bind(
					    []( string file, Tspace &s ) { s.save(file); }, _1, ref(spc));
				    v.push_back(Tptr(new WriteOnceFileAnalysis(val, writer)));
				}

				if ( i.key() == "energyfile" )
				    v.push_back(Tptr(new SystemEnergy(val, pot, spc)));

				if ( i.key() == "virial" )
				    v.push_back(Tptr(new VirialPressure<Tspace>(val, pot, spc)));

				if ( i.key() == "virtualvolume" )
				    v.push_back(Tptr(new VirtualVolumeMove<Tspace>(val, pot, spc)));

				if ( i.key() == "polymershape" )
				    v.push_back(Tptr(new PolymerShape<Tspace>(val, spc)));

				if ( i.key() == "propertytraj" )
				    v.push_back(Tptr(new PropertyTraj(val, pot, spc)));

				if ( i.key() == "cyldensity" )
				    v.push_back(Tptr(new CylindricalDensity<Tspace>(val, spc)));

				if ( i.key() == "widom" )
				    v.push_back(Tptr(new Widom<Tspace>(val, pot, spc)));

				if ( i.key() == "widomscaled" )
				    v.push_back(Tptr(new WidomScaled<Tspace>(val, spc)));

				if ( i.key() == "widommolecule" )
				    v.push_back(Tptr(new WidomMolecule<Tspace>(val, pot, spc)));

				if ( i.key() == "chargemultipole" )
				    v.push_back(Tptr(new ChargeMultipole<Tspace>(val, spc)));

				if ( i.key() == "multipoledistribution" )
				    v.push_back(Tptr(new MultipoleDistribution<Tspace>(val, spc)));

				if ( i.key() == "kirkwoodfactor" )
				    v.push_back(Tptr(new KirkwoodFactor<Tspace>(val, spc)));

				if ( i.key() == "capanalysis" )
				    v.push_back(Tptr(new Capanalysis<Tspace>(val, spc)));

				if ( i.key() == "multipoleanalysis" )
				    v.push_back(Tptr(new MultipoleAnalysis<Tspace>(val, spc)));

				if ( i.key() == "meanforce" )
				    v.push_back(Tptr(new MeanForce(val, pot, spc)));

				if ( i.key() == "atomrdf" )
				    v.push_back(Tptr(new AtomRDF<Tspace>(val, spc)));

				if ( i.key() == "molrdf" )
				    v.push_back(Tptr(new MoleculeRDF<Tspace>(val, spc)));

				if ( i.key() == "molrdfmumu" )
				    v.push_back(Tptr(new MoleculeMumu<Tspace>(val, spc)));

				if ( i.key() == "scatter" )
				    v.push_back(Tptr(new ScatteringFunction<Tspace>(val, spc)));
			    }
			    catch(std::exception &e)
			    {
				std::cerr << "Analysis error: " << i.key() << endl;
				throw;
			    }
			}
		    }

		/**
		 * @brief Append analysis
		 * @tparam Tanalysis Type of the analysis. Must be derived from AnalysisBase.
		 * @param a Instance of the analysis
		 * @return Smart pointer to added analysis (a new instance will be created and maintained internally)
		 */
		template<class Tanalysis>
		    Tptr add( const Tanalysis &a )
		    {
			static_assert(std::is_base_of<AnalysisBase, Tanalysis>::value,
				"`Tanalysis` must be derived from `Analysis::Analysisbase`");
			auto ptr = Tptr(new Tanalysis(a));
			v.push_back(ptr);
			return v.back();
		    }

		/** @brief Find pointer to given analysis type; `nullptr` if not found. */
		template<class Tanalysis>
		    std::shared_ptr<Tanalysis> get()
		    {
			static_assert(std::is_base_of<AnalysisBase, Tanalysis>::value,
				"`Tanalysis` must be derived from `Analysis::Analysisbase`");
			for ( auto b : v )
			{
			    auto ptr = std::dynamic_pointer_cast<Tanalysis>( b );
			    if ( ptr != nullptr )
				return ptr;
			}
			return nullptr;
		    }

		void sample(); //!< Sample all enclosed analysis

		void test( UnitTest & );
		string info();
		Tmjson json();

		~CombinedAnalysis();
	};

    }//namespace
}//namespace
#endif
