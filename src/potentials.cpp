#ifndef FAU_POT_BASE_H
#define FAU_POT_BASE_H

#include <faunus/common.h>
#include <faunus/point.h>
#include <faunus/inputfile.h>
#include <faunus/physconst.h>
#include <faunus/geometry.h>
#include <faunus/potentials.h>
#include <faunus/textio.h>

namespace Faunus
{

    namespace Potential
    {

	PairPotentialBase::PairPotentialBase()
	{
	    if ( atom.empty())
		std::cerr << "Warning: no atoms defined when initializing pair potential.\n";
	    rcut2.resize(atom.size());
	}

	PairPotentialBase::~PairPotentialBase()
	{
	}

	/*
	 * @param N Maximum number of atom types
	 * @param rc Default cutoff distance (angstrom)
	 *
	 void PairPotentialBase::initCutoff(size_t N, float rcut) {
	 rcut2.setConstant(N,N,rcut*rcut);
	 }*/

	/*
	 * @param i Particle type i
	 * @param j Particle type j
	 * @param rc Cutoff distance (angstrom)
	 void PairPotentialBase::setCutoff(size_t i, size_t j, float rcut) {
	 rcut2(i,j)=rcut2(j,i)=rcut*rcut;
	 }*/

	std::string PairPotentialBase::_brief()
	{
	    assert(!name.empty() && "Provide a descriptive name for the potential");
	    return name;
	}

	std::string PairPotentialBase::info( char w )
	{
	    return name + ": N/A";
	}

	string PairPotentialBase::brief()
	{
	    assert(!name.empty() && "Potential must have a name.");
	    return _brief();
	}

	void PairPotentialBase::test( UnitTest & ) {}


	string Potfromfile::_brief()
	{
	    std::ostringstream o;
	    o << name << " Filename = " << filename << std::endl;
	    return o.str();
	}

	string Potfromfile::info(char w)
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << pad(SUB, w, "File with the Potential") << filename << std::endl 
		<< pad(SUB, w, "The range of the tabulated potential")<< std::endl
		<< pad(SUB, w, "xmin: ") << xmin << _angstrom  << std::endl 
		<< pad(SUB, w, "xmax: ") << xmax << _angstrom <<  std::endl;

	    return o.str();
	}

	string Harmonic::_brief()
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << name + ": k=" << k << kT + "/" + angstrom + squared + " req=" << req << _angstrom;
	    return o.str();
	}

	Hertz::Hertz( Tmjson &j )
	{
	    name = "Hertz";
	    E = j.at("_E") = 0.0;
	}

	string Hertz::_brief()
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << name + " E:" << E;
	    return o.str();
	}

	string Hertz::info( char w )
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << name + " E: " << E;
	    return o.str();
	}

	YukawaGel::YukawaGel( Tmjson &j ) : Coulomb(j)
	{
	    name = "YukawaGel";

	    Z  = j["yukawagel_Z"] | 0.0;
	    nc = j["yukawagel_nc"] | 0.0;
	    ns = j["yukawagel_ns"] | 0.0;
	    v  = j["yukawagel_v"] | 1.0;
	    d  = j["yukawagel_d"] | 1.0;

	    k = sqrt(4. * pc::pi * (nc + 2. * ns) * v * v * bjerrumLength());
	    Z2e2overER = Z * Z * bjerrumLength();
	    kd = k * d;
	    k2d2 = k * k * d * d;
	    ekd = exp(-kd);
	    braket7 = (cosh(kd / 2.) - (2. * sinh(kd / 2.) / kd));
	    cout << Z << "   " << bjerrumLength() << "  " << k << endl;
	}

	string YukawaGel::_brief()
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << name;
	    return o.str();
	}

	string YukawaGel::info( char w )
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << name + " Z: " << Z << endl << " Counter ions:  " << nc << endl
		<< " Salt: " << ns << endl << " BjerrumL: " << bjerrumLength() << endl
		<< " kappa: " << k << endl;

	    return o.str();
	}

	CosAttract::CosAttract( Tmjson &j )
	{
	    name = "CosAttract";
	    eps = j.at("eps");
	    rc = j.at("rc");
	    wc = j.at("wc");
	    rc2 = rc * rc;
	    c = pc::pi / 2 / wc;
	    rcwc2 = pow((rc + wc), 2);
	}

	string CosAttract::_brief()
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << name + ": " + epsilon + "=" << eps << kT + " rc=" << rc << _angstrom
		<< " wc=" << wc << _angstrom;
	    return o.str();
	}

	string CosAttract::info( char w )
	{
	    using namespace textio;
	    std::ostringstream o;
	    o << pad(SUB, w, "Depth") << eps << kT << endl
		<< pad(SUB, w, "Decay length") << wc << _angstrom << endl
		<< pad(SUB, w, "Width") << rc << _angstrom << endl
		<< pad(SUB, w, "More info") << "doi:10/chqzjk" << endl;
	    return o.str();
	}

	HardSphere::HardSphere()
	{
	    name = "Hardsphere";
	}

	string HardSphere::info( char w )
	{
	    using namespace Faunus::textio;
	    return textio::indent(SUB) + name + "\n";
	}

	NoSphere::NoSphere()
	{
	    name = "Hardsphere";
	}

	string NoSphere::info( char w )
	{
	    using namespace Faunus::textio;
	    return textio::indent(SUB) + name + "\n";
	}

	string LennardJones::_brief()
	{
	    std::ostringstream o;
	    o << name << ": " << textio::epsilon + "(LJ)=" << eps / 4 << textio::kT;
	    return o.str();
	}

	string LennardJones::info( char w )
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << pad(SUB, w + 1, epsilon + "(LJ)") << eps / 4 << kT
		<< " = " << eps / 4.0_kJmol << " kJ/mol" << endl;
	    return o.str();
	}

	LennardJonesTrunkShift::LennardJonesTrunkShift( Tmjson &j ) : LennardJones(j)
	{
	    name += " Truncated and shifted to sigma";
	} 

	string WCAB::_brief()
	{
	    std::ostringstream o;
	    o << name << ": " << textio::epsilon + "=" << eps;
	    o << name << ": " << "N=" << N;
	    return o.str();
	}

	string WCAB::info( char w )
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << pad(SUB, w + 1, epsilon + "") << eps << endl;
	    o << pad(SUB, w + 1, "N") << N << endl;
	    return o.str();
	}

	string Buckingham::_brief()
	{
	    std::ostringstream o;
	    o << name << ": " << textio::rho << rho;
	    for(unsigned int k = 0; k < P.size(); k++)
	      o << name << ": " << "P"+k << P.at(k);
	    return o.str();
	}

	string Buckingham::info( char w )
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << pad(SUB, w + 1, textio::rho) << rho << endl;
	    for(unsigned int k = 0; k < P.size(); k++)
	      o << pad(SUB, w + 1, "P"+k) << P.at(k) << endl;
	    return o.str();
	}

	string RX::_brief()
	{
	    std::ostringstream o;
	    o << name << ": " << textio::epsilon + "=" << eps;
	    o << name << ": " << "N=" << N;
	    o << name << ": " << textio::rho << "=" << rho;
	    for(unsigned int k = 0; k < P.size(); k++)
	      o << name << ": " << "P"+k << "=" << P.at(k);
	    o << name << ": " << "rswitch=" << rswitch;
	    return o.str();
	}

	string RX::info( char w )
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << pad(SUB, w + 1, epsilon + " ") << eps << endl;
	    o << pad(SUB, w + 1, "N") << N << endl;
	    o << pad(SUB, w + 1, textio::rho) << rho << endl;
	    for(unsigned int k = 0; k < P.size(); k++)
	      o << pad(SUB, w + 1, "P"+k) << P.at(k) << endl;
	    o << pad(SUB, w + 1, "rswitch") << rswitch << endl;
	    return o.str();
	}

	/**
	 * @param j json obect is scanned for the keys `threshold` (angstrom) and `depth` (kT).
	 */
	SquareWell::SquareWell( Tmjson &j )
	{
	    name = "Square Well";
	    threshold = j.at("threshold");
	    depth = j.at("depth");
	}

	string SquareWell::_brief()
	{
	    std::ostringstream o;
	    o << name << ": u=" << depth << textio::kT + " r=" << threshold;
	    return o.str();
	}

	string SquareWell::info( char w )
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << pad(SUB, w, "Threshold") << threshold << " " + angstrom + " (surface-surface)" << endl;
	    o << pad(SUB, w, "Depth") << depth << kT << endl;
	    return o.str();
	}

	/**
	 * In addition to the keywords from `Potential::SquareWell` the json object is
	 * searched for:
	 * - `threshold_lower`
	 */
	SquareWellShifted::SquareWellShifted( Tmjson &j ) : SquareWell(j)
	{
	    name += " Shifted";
	    threshold_lower = j.at("threshold_lower");
	}

	string SquareWellShifted::_brief()
	{
	    std::ostringstream o;
	    o << name << ": u=" << depth << textio::kT
		<< " range=" << threshold_lower << "-" << threshold;
	    return o.str();
	}

	string SquareWellShifted::info( char w )
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << SquareWell::info(w)
		<< pad(SUB, w, "Threshold_lower") << threshold_lower << _angstrom
		<< " (surface-surface)" << endl;
	    return o.str();
	}

	string R12Repulsion::_brief()
	{
	    std::ostringstream o;
	    o << name << ": " << textio::epsilon + "(r12)=" << eps / 4 << textio::kT;
	    return o.str();
	}

	string R12Repulsion::info( char w )
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << pad(SUB, w + 1, epsilon + "(r12_rep)") << eps / 4 << kT << endl;
	    return o.str();
	}

	Coulomb::Coulomb( Tmjson &j )
	{
	    name = "Coulomb";
	    epsilon_r = j.value("epsr", 80.0);
	    depsdt = j.value("depsdt", -0.368*pc::T()/epsilon_r);
	    lB = pc::lB(epsilon_r);
	}

	string Coulomb::_brief()
	{
	    std::ostringstream o;
	    o << name << ": lB=" << lB << " eps_r=" << epsilon_r << " T=" << pc::T();
	    return o.str();
	}

	double Coulomb::bjerrumLength() const
	{
	    return lB;
	}

	string Coulomb::info( char w )
	{
	    using namespace textio;
	    std::ostringstream o;
	    o << pad(SUB, w, "Temperature") << pc::T() << " K" << endl
		<< pad(SUB, w, "Dielectric constant") << epsilon_r << endl
		<< pad(SUB, w + 6, "T" + partial + epsilon + "/" + epsilon + partial + "T") << depsdt << endl
		<< pad(SUB, w, "Bjerrum length") << lB << " " + angstrom << endl;

	    return o.str();
	}

	void Coulomb::test( UnitTest &t )
	{
	    t("bjerrum", bjerrumLength(), 1e-6);
	}
	
	CoulombUniformCap2::CoulombUniformCap2( Tmjson &j ) : Coulomb(j)
	{
	    rc2 = j.at("cutoff");
	    rc2 = rc2*rc2;
	    epsr = j.at("epsr");
	    double DebyeLengthAngstrom = j.at("debyelength").get<double>(); // Debye-length in Ångstrom
	    kappa = 1.0/DebyeLengthAngstrom; // Inverse Debye-length in Ångstrom^-1
	    lB = pc::lB( epsr );
	    name += "UniformCap2";
	    effective_charges.resize(atom.size());
	    charge_densities.resize(atom.size());
	    charges.resize(atom.size());
	    area.resize(atom.size());

	    int cnt = 0;
	    for (auto &i : atom) {
		double qis = i.charge; // Effective charge
		if( ( kappa > 1e-9 ) && ( i.radius > 1e-9 ) )
		    qis *= sinh(i.radius*kappa)/i.radius/kappa; // Effective charge of atom 'i' // 0.8825870836
		effective_charges.at(cnt) = qis;
		charge_densities.at(cnt) = i.charge_density;
		charges.at(cnt) = i.charge;
		area.at(cnt) = i.area;

		for (auto &j : atom) {
		    double qjs = j.charge; // Effective charge
		    if( ( kappa > 1e-9 ) && ( j.radius > 1e-9 ) )
			qjs *= sinh(j.radius*kappa)/j.radius/kappa; // Effective charge of atom 'j' // 0.8825870836


		    //cout << "kappa: " << kappa << ", qis: radius: " << i.radius << atom[i.id].name << " " << qis << ", qjs: radius: " << j.radius << ", " << atom[j.id].name << " " << qjs << endl;
		    lBxQQe.seta(i.id, j.id, lB * i.charge * qjs );     // Real charge of atom 'i' and effective charge of atom 'j'
		    lBxQeQe.seta(i.id, j.id, lB * qis * qjs );         // Effective charge of atom 'i' and Effective charge of atom 'j'
		}
		cnt++;
	    }
	}

	CoulombUniformCap::CoulombUniformCap( Tmjson &j ) : Coulomb(j)
	{
	    rc2 = j.at("cutoff");
	    rc2 = rc2*rc2;
	    epsr = j.at("epsr");
	    double DebyeLengthAngstrom = j.at("debyelength").get<double>(); // Debye-length in Ångstrom
	    kappa = 1.0/DebyeLengthAngstrom; // Inverse Debye-length in Ångstrom^-1
	    lB = pc::lB( epsr );
	    name += "UniformCap";
	    effective_charges.resize(atom.size());

	    int cnt = 0;
	    for (auto &i : atom) {
		double qis = i.charge; // Effective charge
		if( i.radius*kappa > 1e-9 )
		    qis *= sinh(i.radius*kappa)/i.radius/kappa; // Effective charge of atom 'i' // 0.8825870836
		effective_charges.at(cnt) = qis;

		for (auto &j : atom) {
		    double qjs = j.charge; // Effective charge
		    if( j.radius*kappa > 1e-9 )
			qjs *= sinh(j.radius*kappa)/j.radius/kappa; // Effective charge of atom 'j' // 0.8825870836


		    lBxQQe.seta(i.id, j.id, lB * i.charge * qjs );     // Real charge of atom 'i' and effective charge of atom 'j'
		    lBxQeQe.seta(i.id, j.id, lB * qis * qjs );         // Effective charge of atom 'i' and Effective charge of atom 'j'
		}
		cnt++;
	    }
	}

	CoulombWolf::CoulombWolf( Tmjson &j ) : Coulomb(j)
	{
	    double Rc = j.at("cutoff");
	    Rcinv = 1 / Rc;
	    Rc2 = Rc * Rc;
	    name += "Wolf/Yonezawa";

	    for (auto &i : atom)
		for (auto &j : atom)
		    lBxQQ.set(i.id, j.id, bjerrumLength() * i.charge * j.charge );
	}

	string CoulombWolf::info( char w )
	{
	    using namespace textio;
	    std::ostringstream o;
	    o << Coulomb::info(w)
		<< pad(SUB, w, "More info") << "doi:10/j97\n"
		<< pad(SUB, w, "Cut-off") << 1 / Rcinv << _angstrom + "\n";
	    return o.str();
	}

	ChargeNonpolar::ChargeNonpolar( Tmjson &j ) : Coulomb(j)
	{
	    name = "Charge-Nonpolar";
	    c = bjerrumLength() / 2.;
	}

	string ChargeNonpolar::info( char w )
	{
	    std::ostringstream o;
	    o << Coulomb::info(w)
		<< textio::pad(textio::SUB, w, "Excess polarization") << 2 * c * bjerrumLength() << endl;
	    return o.str();
	}

	PolarPolar::PolarPolar( Tmjson &j ) : Coulomb(j)
	{
	    name = "Polar-Polar";
	}

	string PolarPolar::info( char w )
	{
	    std::ostringstream o;
	    o << Coulomb::info(w) << endl;
	    return o.str();
	}

	string DebyeHuckel::_brief()
	{
	    std::ostringstream o;
	    o << Coulomb::_brief() << " I=" << ionicStrength();
	    return o.str();
	}

	double DebyeHuckel::ionicStrength() const
	{
	    return k * k / c;
	}

	double DebyeHuckel::debyeLength() const
	{
	    return 1 / k;
	}

	/**
	 * @details The Debye-Huckel potential is temperature dependent and contains entropy
	 * contributions from both solvent and salt degrees of freedom.
	 * This function return the entropy of interaction between a pair of
	 * particles interacting with an effective Debye-Huckel potential. This is done by
	 * taking the temperature derivate of w(R):
	 *
	 * @f[
	 * S(r_{ij})/k_B = -\frac{ \partial w(r_{ij},T) } {k_B \partial T}
	 *     = \beta w_{ij}\left [ \alpha - \frac{\kappa r_{ij}(\alpha+1)}{2}\right ]
	 * @f]
	 * where \f$ \alpha=T \partial \epsilon_r/\epsilon_r\partial T\f$
	 * is determined experimentally for pure water. To get the entropy from salt ions
	 * only, set \f$\alpha=0\f$ via the following keywords:
	 *
	 * @param  betaw    Inter particle free energy, \f$\beta w\f$, in units of kT.
	 * @param  r        Inter particle distance
	 * @return Interaction entropy \f$ S(r_{ij})/k_B = \beta TS(r_{ij})\f$
	 * @todo   Optimize
	 */
	double DebyeHuckel::entropy( double betaw, double r ) const
	{
	    return betaw * (depsdt - 0.5 * k * r * (depsdt + 1));
	}

	/**
	 * @return \f$\beta \mu_{\mbox{excess}} = -\frac{l_Bz^2\kappa}{2(1+\kappa a)}\f$
	 * @param z Charge number
	 * @param a Particle diameter (angstrom)
	 */
	double DebyeHuckel::excessChemPot( double z, double a ) const
	{
	    return -lB * z * z * k / (2 * (1 + k * a));
	}

	/**
	 * @return \f$\exp {(\beta\mu_{\mbox{excess}})}\f$
	 * @param z Charge number
	 * @param a Particle diameter (angstrom)
	 */
	double DebyeHuckel::activityCoeff( double z, double a ) const
	{
	    return exp(excessChemPot(z, a));
	}

	string DebyeHuckel::info( char w )
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << Coulomb::info(w)
		<< pad(SUB, w, "Ionic strength") << ionicStrength() << " mol/l" << endl
		<< pad(SUB, w + 1, "Debye length, 1/" + textio::kappa) << debyeLength() << " "
		<< 1 / k << " " + angstrom << endl;
	    if ( k2_count_avg.cnt > 0 )
	    {
		double k2_s = k * k - k2_count;
		o << pad(SUB, w + 1, "Debye length, 1/" + textio::kappa)
		    << 1 / sqrt(k2_count_avg.avg()) << " " + angstrom + " (counter ions)\n"
		    << pad(SUB, w + 1, "Debye length, 1/" + textio::kappa)
		    << 1 / sqrt(k2_s) << " " + angstrom + " (salt)" << endl;
	    }
	    return o.str();
	}

	Harmonic::Harmonic( double k, double req ) : k(k), req(req) { name = "Harmonic"; }

	FENE::FENE( double k_kT, double rmax_A ) : k(k_kT)
	{
	    name = "FENE";
	    r02 = rmax_A * rmax_A;
	    r02inv = 1 / r02;
	}

	LennardJones::LennardJones( Tmjson &j )
	{

	    name = "Lennard-Jones";
	    eps = 4.0 * j.at("eps").get<double>();
	    string unit = j.value("unit", string("kT"));
	    if ( unit == "kJ/mol" )
		eps *= 1.0_kJmol;
	}

	LennardJones::LennardJones() : eps(0) { name = "Lennard-Jones"; }

	WCAB::WCAB( Tmjson &j )
	{
	    name = "WCAB";
	    N = j.at("N").get<int>();
	    
	    rho = j.at("rho").get<double>();
	    
	    string str = j.at("P").get<string>();
	    std::istringstream is2(str), tmp(str);
	    int size = std::distance(std::istream_iterator<string>(tmp), std::istream_iterator<string>());
	    P.resize(size);
	    for (int i=0; i<size; i++)
		is2 >> P.at(i);
	    
	    eps = P.at(0)*pow(double(N)*rho/exp(1.0),double(N));
	    //eps = j.at("eps").get<double>();
	}

	WCAB::WCAB() : N(12),rho(1.0) { 
	  name = "WCAB";
	  P.resize(1);
	  P.at(0) = 1.0;
	  eps = P.at(0)*pow(double(N)*rho/exp(1.0),double(N));
	}

	Buckingham::Buckingham( Tmjson &j )
	{
	    name = "Buckingham";
	    rho = j.at("rho").get<double>();

	    string str = j.at("P").get<string>();
	    std::istringstream is2(str), tmp(str);
	    int size = std::distance(std::istream_iterator<string>(tmp), std::istream_iterator<string>());
	    P.resize(size);
	    for (int i=0; i<size; i++)
		is2 >> P.at(i);
	}

	Buckingham::Buckingham() : rho(1.0) { 
	    name = "Buckingham"; 
	    P.resize(1);
	    P.at(0) = 1.0;
	}

	RX::RX( Tmjson &j )
	{
	    name = "RX";
	    N = j.at("N").get<int>();
	    
	    rho = j.at("rho").get<double>();
	    
	    string str = j.at("P").get<string>();
	    std::istringstream is2(str), tmp(str);
	    int size = std::distance(std::istream_iterator<string>(tmp), std::istream_iterator<string>());
	    P.resize(size);
	    for (int i=0; i<size; i++)
		is2 >> P.at(i);
	    rswitch = findRswitch();
	    
	    eps = P.at(0)*pow(double(N)*rho/exp(1.0),double(N));
	    //eps = j.at("eps").get<double>();
	}

	RX::RX() : N(12),rho(1.0) { 
	  name = "RX"; 
	  P.resize(1);
	  P.at(0) = 1.0;
	  rswitch = findRswitch();
	  eps = P.at(0)*pow(double(N)*rho/exp(1.0),double(N));
	}

	SquareWellHydrophobic::SquareWellHydrophobic( Tmjson &j ) : SquareWell(j)
	{
	    name = "Hydrophobic " + name;
	}

	string SoftRepulsion::_brief()
	{
	    std::ostringstream o;
	    o << name << ": " << textio::sigma << pow(sigma6, 1 / 6.) << textio::_angstrom;
	    return o.str();
	}

	SoftRepulsion::SoftRepulsion( Tmjson &j )
	{
	    name = "Repulsive r6";
	    sigma6 = pow(j["sigma"] | 5.0, 6);
	}

	Harmonic::Harmonic( Tmjson &j )
	{
	    name = "Harmonic";
	    k = j.at("k");
	    req = j.at("req");
	}

	string FENE::_brief()
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << name + ": k=" << k << kT + "/" + angstrom + squared + " r0=" << sqrt(r02) << _angstrom;
	    return o.str();
	}

	Cardinaux::Cardinaux( Tmjson &j )
	{
	    name = "Cardinaux";
	    alpha = j.at("alpha");
	    alphahalf = 0.5 * alpha;
	    for ( auto &i : atom )
		for ( auto &j : atom )
		    eps.set(i.id, j.id, sqrt(i.eps * j.eps) * 4.0_kJmol);
	}

	string Cardinaux::_brief()
	{
	    return name + ": a=" + std::to_string(alpha);
	}

	DebyeHuckelShift::DebyeHuckelShift( Tmjson &j ) : DebyeHuckel(j)
	{
	    rc = j.at("cutoff");
	    rc2 = rc * rc;
#ifdef FAU_APPROXMATH
	    u_rc = exp_cawley(-k*rc)/rc; // use approx. func even here!
#else
	    u_rc = exp(-k * rc) / rc;
#endif
	    dudrc = -k * u_rc - u_rc / rc; // 1st derivative of u(r) at r_c
	    std::ostringstream o;
	    o << " (shifted, rcut=" << rc << textio::_angstrom << ")";
	    name += o.str();
	}

	string DebyeHuckelDenton::info( char w )
	{
	    lB = lB_org;
	    return DebyeHuckel::info(w);
	}

	DebyeHuckelDenton::DebyeHuckelDenton( Tmjson &in ) : DebyeHuckel(in)
	{
	    name += "-Denton";
	    lB_org = lB;
	}

	void DebyeHuckelDenton::setBjerrum( double a_m, double a_n )
	{
	    double ka_m = k * a_m, ka_n = k * a_n;
	    lB = lB_org * exp(ka_m + ka_n) / ((1 + ka_m) * (1 + ka_n));
	}

	double DebyeHuckelDenton::fmn( double m, double n )
	{
	    return k * (m * m + n * n + k * (m + n) * m * n) / ((1 + k * m) * (1 + k * n));
	}

	DebyeHuckel::DebyeHuckel( Tmjson &j ) : Coulomb(j)
	{
	    const double zero = 1e-10;
	    name = "Debye-Huckel";
	    c = 8 * lB * pc::pi * pc::Nav / 1e27;
	    double I = j.value("ionicstrength", 0.0);   // [mol/l]
	    z_count = j.value("countervalency", 0.0);  // [e]
	    k2_count = 0;
	    k = sqrt(I * c);
	    cout << "Ionic s = " << I << endl;
	    if ( k < zero )
		k = 1 / j.at("debyelength").get<double>(); // [A]
	}

	R12Repulsion::R12Repulsion( Tmjson &j )
	{
	    name = "r12-Repulsion";
	    eps = 4.0 * j.at("eps").get<double>();
	}

	string SoftRepulsion::info( char w )
	{
	    using namespace Faunus::textio;
	    std::ostringstream o;
	    o << textio::pad(SUB, w + 1, textio::sigma) << pow(sigma6, 1 / 6.) << _angstrom << endl;
	    return o.str();
	}
    } //Potential namespace

} //Faunus namespace
#endif

