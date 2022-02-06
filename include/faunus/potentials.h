#ifndef FAUNUS_POTENTIAL_H
#define FAUNUS_POTENTIAL_H

#ifndef SWIG
#include <faunus/common.h>
#include <faunus/point.h>
#include <faunus/textio.h>
#include <faunus/physconst.h>
#include <faunus/auxiliary.h>
#include <faunus/inputfile.h>
#include <faunus/species.h>
#include <faunus/average.h>
#endif

namespace Faunus {

    /**
     * @brief Namespace for pair potentials
     *
     * This namespace contains classes and templates that calculates the
     * pair interaction energy and force between particles.
     * All pair potentials are based
     * on `PairPotentialBase` and thus have common interfaces.
     * Several pair potentials can be combined into
     * a new pair potential and a number of
     * common combinations are pre-defined as typedefs.
     *
     * ~~~
     * CoulombHS primitiveModel();                // Note that constructor
     * auto nonbond = Coulomb() + LennardJones(); // arguments are here
     * auto bond    = Harmonic() - nonbond;       // omitted for clarity
     *
     * PointParticle a,b;                         // two particles
     * double r2 = 100;                           // a<->b squared distance
     * double u  = nonbond(a,b,r2);               // a<->b energy in kT
     * ~~~
     *
     * As shown in the last example, pair potentials can also be subtracted
     * which can be used to for example exclude non-bonded interactions
     * between bonded pairs.
     *
     * If the pair interaction depends on particle types, use
     * `PotentialMap` pair interaction.
     *
     * *Behind the scene:*
     *
     * The above combination of pair potentials is done at compile time
     * using templates. This means that there is a good chance that
     * the mixing overhead can be optimized out by the compiler.
     * For example, when adding two potentials
     * we construct a new `CombinedPairPotential`. Likewise when
     * subtracting, a new `Minus` template is created and then combined.
     *
     */
    namespace Potential {

	class DebyeHuckel;

	/**
	 * @brief Base class for pair potential classes
	 *
	 * This is a base class for all pair potentials. All derived classes
	 * are class functions, i.e. the function operator returns the
	 * energy. For example:
	 *
	 *     PointParticle a,b;
	 *     Coulomb pot(in);
	 *     pot(a,b,16.); // -> energy in kT
	 * 
	 */
	class PairPotentialBase {
	    private:
		virtual string _brief();
	    public:
		typedef PairMatrix<double> Tcutoff;
		Tcutoff rcut2;                        //!< Squared cut-off distance (angstrom^2)

		PairPotentialBase( );

		virtual ~PairPotentialBase();
		string name;       //!< Name of potential
		string brief();    //!< Brief, one-lined information string

		/**
		 * @brief Particle-particle force in units of `kT/AA`
		 * @param a First particle
		 * @param b Second particle
		 * @param r2 Squared distance between them (angstrom squared)
		 * @param p Vector from: p=b-a
		 */
		template<typename Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			std::cerr << "Using unimplemented force!" << std::endl;
			return Point(0,0,0);
		    }

		/** @brief Electric field at spatial position */
		template<typename Tparticle>
		    Point field(const Tparticle &a, const Point &r) const {
			return Point(0,0,0);
		    }

		template<typename Tpvec>
		    double internal(const Tpvec &p, const Group &g) const {
			return 0.0;
		    }

		/**
		 * @brief Set space dependent features such as density dependent potentials
		 *
		 * The base-class version does nothing.
		 */
		template<class Tspace>
		    void setSpace(Tspace&) {}

		virtual void test(UnitTest&);                    //!< Perform unit test

		virtual std::string info(char=20);
	};

	/**
	 * @brief Save pair potential and force table to disk
	 *
	 * This will save a pair potential to disk as three columns:
	 * `distance`, `energy`, and `force`. The distance interval
	 * is hard coded to `dmin=a.radius+b.radius` to `5*dmin`.
	 *
	 * Example:
	 * ~~~~
	 * using namespace Potential;
	 * CoulombLJ pot(...);
	 * save(pot, atom["Na"].id, atom["Cl"].id, "mytable.dat");
	 * ~~~~
	 */
	template<class Tparticle=PointParticle, class Tpairpot, class Tid>
	    bool save(Tpairpot pot, Tid ida, Tid idb, string file) {
		std::ofstream f(file.c_str());
		if (f) {
		    double min=atom[ida].radius+atom[idb].radius;
		    Tparticle a,b;
		    a = atom[ida];
		    b = atom[idb];
		    f << "# Pair potential: " << pot.brief() << endl
			<< "# Atoms: " << atom[ida].name << "<->" << atom[idb].name
			<< endl;
		    f.precision(12);
		    for (double r=min; r<=5*min; r+=0.01)
			f << std::left << std::setw(12) << r << " "
			    << pot(a,b,r*r) << " " << pot.force(a,b,r*r,Point(r,0,0)).x() << endl;
		    return true;
		}
		return false;
	    }

	/**
	 * @brief A potential that is read from an external file.
	 *
	 * Make sure that your data measures the energy in kT and the 
	 * distance in Angstrom. The potential is interpolated using @f$u(r) = \alpha r^2 + \beta r + \gamma @f$
	 * regression between 3 points to determine the value of the
	 * interaction for a given distance, r, and is
	 * zero if the r value is outside the min max values.
	 * 
	 * When constructing the potential the file where your potential
	 * is stored will be read and the \f$\alpha\f$, \f$\beta\f$, and \f$\gamma\f$
	 * coefficients will be calculated and tabulated 
	 * in coresponding vectors. 
	 *
	 * In the json file under nonbonded the following keywords are 
	 * required:
	 *
	 * Keyword     | Description
	 * :---------- | :------------------------------------------------------------------
	 * `datafile`  | filename for double column file with r and u(r) in angstrom and kT
	 */
	class Potfromfile : public PairPotentialBase {
	    private:
		std::vector <double> x, y, alfa, beta, gamma;
		double xmin,xmax;
		string _brief();
		string filename;
	    public:
		Potfromfile();

		Potfromfile(Tmjson &j){

		    name = "Potfromfile";
		    try{
			filename = j.at("datafile").get<string>(); //WILL CRASH THE RUN IF THERE IS NO FILE SPECIFIED IN THE JSON 
			std::ifstream fin(filename);

			if( !fin )
			    throw std::runtime_error("Could not open file"); //WILL CRASH THE RUN IF THE FILE CAN'T BE FOUND

			double tmpX, tmpY;
			while (fin >> tmpX >> tmpY) {
			    x.push_back(tmpX);
			    y.push_back(tmpY);
			};
			if (x.size()<3)
			    throw std::runtime_error("Table must have at least three points");

			fin.close();
		    }
		    catch (std::exception &e) {
			std::cerr << name << ": " << e.what() << endl;
			throw;
		    }
		    xmin = x[1];
		    xmax = x[x.size()-2];
		    int size = x.size()-2;

		    for(int i=1; i<=size; i++){
			double x2im1=x[i-1]*x[i-1];
			double x2i = x[i]*x[i];
			double x2ip1 = x[i+1]*x[i+1];
			double ximxim1 = x[i]-x[i-1];
			double xip1mxim1 = x[i+1]-x[i-1];
			double x2imx2im1 = x2i-x2im1;

			//Calculates and stores the alfa,beta,gamma coefficients.

			// WARNING due to the fact that you are skiping the first point in the 
			// data the indicies between the data and coefficients will be missmached by -1 

			alfa.push_back((((y[i+1]-y[i-1])*ximxim1) + (xip1mxim1*(-y[i]+y[i-1]))) / 
				(((x2im1 + x2ip1)*ximxim1)-(x2imx2im1*xip1mxim1)));

			beta.push_back((y[i]-alfa[i-1]*x2imx2im1-y[i-1])/ximxim1);

			gamma.push_back(y[i-1]- alfa[i-1]*x2im1-beta[i-1]*x[i-1]);

		    }
		}

		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) {

			double m=sqrt(r2);

			if(m >= xmin && m <= xmax){
			    auto it = std::lower_bound(x.begin(), x.end(), m);
			    int i = std::distance(x.begin(), it); // iterator --> index

			    // WARNING due to the fact that you are skiping the first point in the 
			    // data the indicies between the data and coefficients will be missmached by -1 

			    return alfa[i-1]*m*m + beta[i-1]*m + gamma[i-1];

			}
			return 0;  
		    }

		//This is needed if for some reason you want to run this potential using Energy::NonbonbedVector
		//Which is not recomended since it will slow down the calculations. 

		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			return operator()(a,b,r.squaredNorm());
		    }

		string info(char w);
	};


	/**
	 * @brief Harmonic pair potential
	 *
	 * The harmonic potential has the form
	 * \f$ \beta u_{ij} = k(r_{ij}-r_{eq})^2 \f$ where k is the force constant
	 * (kT/angstrom^2) and req is the equilibrium distance (angstrom).
	 *
	 * JSON keywords:
	 *
	 * Keyword   | Description
	 * :-------- | :-----------------------------------------------
	 * `req`     | Equilibrium distance (angstrom)
	 * `k`       | Force constant (kT/angstrom^2). See note below.
	 *
	 * @note We do not multiply with 1/2 which must be included in the
	 * supplied force constant, `k`.
	 */
	class Harmonic : public PairPotentialBase {
	    private:
		string _brief();

	    public:
		double k;   //!< Force constant (kT/A^2) - Remember to divide by two!
		double req; //!< Equilibrium distance (angstrom)

		Harmonic( double k=0, double req=0 );
		Harmonic( Tmjson &j );

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator()( const Tparticle &a, const Tparticle &b, double r2 ) const {
			double d=sqrt(r2)-req;
			return k*d*d;
		    }

		template<class Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			double r=sqrt(r2), d=r-req;
			return -2 * k * d / r * p;
		    }
	};

	/**
	 * @brief Cosine attraction
	 * @details This is an attractive potential used for coarse grained lipids
	 * and has the form:
	 * @f[
	 *     \beta u(r) = -\epsilon \cos^2 [ \pi(r-r_c)/2w_c ]
	 * @f]
	 * for \f$r_c\leq r \leq r_c+w_c\f$. For \f$r<r_c\f$, \f$\beta u=-\epsilon\f$,
	 * while zero for \f$r>r_c+w_c\f$.
	 *
	 * JSON keywords:
	 *
	 * Key     | Description
	 * :-------| :---------------------------
	 * `eps`   | Depth, \f$\epsilon\f$ [kT]
	 * `rc`    | Width, r_c [angstrom]
	 * `wc`    | Decay range, w_c [angstrom] 
	 *
	 */
	class CosAttract : public PairPotentialBase {
	    protected:
		double eps, wc, rc, rc2, c, rcwc2;
		string _brief();
	    public:

		CosAttract(Tmjson&);

		/**
		 * @todo
		 * The function `x(c,r2,rc)` defined herein could be approximated
		 * by a series expansion for `r2=rcwc2`. In this way one can
		 * avoid `cos()` and `sqrt()`. C code for this could be generated
		 * in Matlab:
		 *
		 * ~~~
		 * with(CodeGeneration)
		 * x := series(cos(c*(sqrt(r2)-rc)), r2 = rcwc2, 2)
		 * convert(x, polynom)
		 * C(%, resultname = "x")
		 * ~~~
		 */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
			if (r2<rc2)
			    return -eps;
			if (r2>rcwc2)
			    return 0;
#ifdef FAU_APPROXMATH
			double x=cosApprox( c*( sqrt(r2)-rc ) );
#else
			double x=cos( c*( sqrt(r2)-rc ) );
#endif
			return -eps*x*x;
		    }

		string info(char); // More verbose information

		template<class Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			if (r2<rc2 || r2>rcwc2)
			    return Point(0,0,0);
			double r=sqrt(r2);
#ifdef FAU_APPROXMATH
			double x1=cosApprox( c*( r-rc ) );
			double x2=sinApprox( c*( r-rc ) );
#else
			double x1=cos( c*( r-rc ) );
			double x2=sin( c*( r-rc ) );
#endif
			return -2*c*eps*x1*x2/r*p;
		    }
	};

	/**
	 * @brief Finite Extensible Nonlinear Elastic (FENE) potential
	 *
	 * This is an anharmonic bonding potential with the form:
	 * @f[
	 *     \beta u(r) = -\frac{k r_0^2}{2}\ln \left [ 1-(r/r_0)^2 \right ]
	 * @f]
	 * for \f$r<r_0\f$, otherwise infinity. JSON keywords:
	 *
	 * - `stiffness` Bond stiffness, `k` [kT]
	 * - `maxsep` Maximum separation, `r_0` [angstrom]
	 *
	 * More info: doi:10.1103/PhysRevE.59.4248
	 */
	class FENE : public PairPotentialBase {
	    private:
		double k, r02, r02inv;

		string _brief();

	    public:
		FENE(double k_kT, double rmax_A);

		FENE( Tmjson &j ) {
		    name="FENE";
		    k  = j.at("stiffness");
		    r02 = std::pow( double(j.at("maxsep")), 2);
		    r02inv = 1/r02;
		}

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    inline double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			return (r2>r02) ? pc::infty : -0.5*k*r02*std::log(1-r2*r02inv);
		    }

		template<class Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			return (r2>r02) ? -pc::infty*p : -k * r02 / (r02-r2) * p;
		    }
	};

	/**
	 * @brief Hertz pair potential
	 */
	class Hertz : public PairPotentialBase {
	    private:
		double E;
		string _brief();
	    public:
		Hertz(Tmjson&);

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator()(const Tparticle &a, const Tparticle &b, double r2) {
			double m = a.radius+b.radius;
			double diameter = 2.*a.radius;
			if(r2 <= m*m) {
			    return E*pow((1-(sqrt(r2)/diameter)),(5./2.));
			}
			return 0;
		    }

		template<class Tparticle>
		    double operator()(const Tparticle &a, const Tparticle &b,const Point &r2) {
			return operator()(a,b, r2.squaredNorm());
		    }

		string info(char);
	};

	/**
	 * @brief Hard sphere pair potential
	 */
	class HardSphere : public PairPotentialBase {
	    public:
		HardSphere();

		template<typename T>
		    HardSphere(const T&) { name="Hardsphere"; }

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			double m=a.radius+b.radius;
			return (r2<m*m) ? pc::infty : 0;
		    }

		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			return operator()(a,b,r.squaredNorm());
		    }

		string info(char w);
	};

	class NoSphere : public PairPotentialBase {
	    public:
		NoSphere();

		template<typename T>
		    NoSphere(const T&) { name="Nosphere"; }

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			return 0.0;
		    }

		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			return 0.0;
		    }

		string info(char w);
	};

	/**
	 * @brief Hard sphere pair potential
	 */
	class HardSphereKc : public PairPotentialBase {
	    private:
	      double length, rc;
	  
	    public:
		//HardSphereKc(Tmjson&);

		template<typename T>
		    HardSphereKc(const T &j) { 
		      name="Hardsphere"; 
		      length = j.at("length");
		      rc = j.at("radius");
		    }
		    
		template<class Tparticle>
		    bool isCornerWithinParticle(const Tparticle &a, Point corner) {
		      Point tmp = a + a.lv()*a.length()/2.0;
		      double dA = -tmp.dot(a.lv());
		      tmp = a-a.lv()*a.length()/2.0;
		      double dB = -tmp.dot(a.lv());
		      double dC = -corner.dot(a.lv());
		      //cout << "d: " << dA << ", " << dB << ", " << dC << endl;
		      bool lvb = false;
		      if( (dA <= dC) && (dC <= dB) )
			lvb = true;
		      if( (dA >= dC) && (dC >= dB) )
			lvb = true;
		      
		      tmp = a + a.wv()*a.width()/2.0;
		      dA = -tmp.dot(a.wv());
		      tmp = a-a.wv()*a.width()/2.0;
		      dB = -tmp.dot(a.wv());
		      dC = -corner.dot(a.wv());
		      //cout << "d: " << dA << ", " << dB << ", " << dC << endl;
		      bool wvb = false;
		      if( (dA <= dC) && (dC <= dB) )
			wvb = true;
		      if( (dA >= dC) && (dC >= dB) )
			wvb = true;
		      
		      tmp = a + a.dv()*a.depth()/2.0;
		      dA = -tmp.dot(a.dv());
		      tmp = a-a.dv()*a.depth()/2.0;
		      dB = -tmp.dot(a.dv());
		      dC = -corner.dot(a.dv());
		      //cout << "d: " << dA << ", " << dB << ", " << dC << endl;
		      bool dvb = false;
		      if( (dA <= dC) && (dC <= dB) )
			dvb = true;
		      if( (dA >= dC) && (dC >= dB) )
			dvb = true;
		      
		      if(lvb && wvb && dvb)
			return true;
		      
		      return false;
		    }
		   
		template<class Tparticle>
		    vector<Point> getCorners(const Tparticle &a) {
			vector<Point> corners;
			Point tmp = ( a.lv()*a.length() + a.wv()*a.width() + a.dv()*a.depth() )/2.0;
			corners.push_back(tmp);
			tmp = ( a.lv()*a.length() - a.wv()*a.width() + a.dv()*a.depth() )/2.0;
			corners.push_back(tmp);
			tmp = ( a.lv()*a.length() + a.wv()*a.width() - a.dv()*a.depth() )/2.0;
			corners.push_back(tmp);
			tmp = ( a.lv()*a.length() - a.wv()*a.width() - a.dv()*a.depth() )/2.0;
			corners.push_back(tmp);
			tmp = ( -a.lv()*a.length() + a.wv()*a.width() + a.dv()*a.depth() )/2.0;
			corners.push_back(tmp);
			tmp = ( -a.lv()*a.length() - a.wv()*a.width() + a.dv()*a.depth() )/2.0;
			corners.push_back(tmp);
			tmp = ( -a.lv()*a.length() + a.wv()*a.width() - a.dv()*a.depth() )/2.0;
			corners.push_back(tmp);
			tmp = ( -a.lv()*a.length() - a.wv()*a.width() - a.dv()*a.depth() )/2.0;
			corners.push_back(tmp);
			return corners;
		    }
		    
		    bool checkCylinder(vector<Point> corners) {
		      
		      for(unsigned int i = 0; i < corners.size(); i++) {
			Point xyz = corners.at(i);
			double r2 = xyz.x()*xyz.x() + xyz.y()*xyz.y();
			if(sqrt(r2) > rc)
			  return true;
			if(xyz.z() > length)
			  return true;
			if(xyz.z() < 0)
			  return true;
		      }
		      return false;
		    }

		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			vector<Point> cornersA = getCorners(a);
			vector<Point> cornersB = getCorners(b);
			
			if(checkCylinder(cornersA))
			  return pc::infty;
			if(checkCylinder(cornersB))
			  return pc::infty;
		      
			for(unsigned int i = 0; i < cornersA.size(); i++)
			  if(isCornerWithinParticle(b,a+cornersA.at(i)))
			    return pc::infty;
			return 0.0;
		    }

		string info(char w);
	};

	/**
	 * @brief Hard sphere pair potential
	 */
	class HardSphereTube : public PairPotentialBase {
	    public:
		HardSphereTube();

		template<typename T>
		    HardSphereTube(const T&) { name="HardSphereTube"; }
		    
		      /**
			* @brief Calculates the direction of a line and a point on the line of two intersecting planes
			* @param rA Point on plane 'A'
			* @param rB Point on plane 'B'
			* @param nA Normalized normal to plane 'A'
			* @param nB Normalized normal to plane 'B'
			* @param l0 Point on line
			* @param n Normalized direction of line
			*/
		      void getLineOfIntersectingPlanes(Point rA, Point rB, Point nA, Point nB, Point &l0, Point &n) {
			assert( std::fabs(nA.norm() - 1.0) < 1e-6 && "Vector is not normalized.!");
			assert( std::fabs(nB.norm() - 1.0) < 1e-6 && "Vector is not normalized.!");
			n = nA.cross(nB); // Direction of line
			n /= n.norm();
			double da = -n.dot(rA);
			double db = -n.dot(rB);
			double x = (rA.y()*db - rB.y()*da)/(rA.x()*rB.y() - rA.y()*rB.x());
			double y = -(rA.x()*db - rB.x()*da)/(rA.x()*rB.y() - rA.y()*rB.x());
			double z = 0.0;
			l0 = Point(x,y,z); // Point on line
		      }
		    
		      /**
			* @brief Calculates the distance between a point and a line, and the closest point on that line
			* @param P0 Point
			* @param lv Direction of line
			* @param l0 Point on line
			* @param d0 Closest point on line to 'P0'
			* @param d Closest distance between 'P0' and line
			*/
		      void getDistanceBetweenPointAndline(Point P0, Point lv, Point l0, Point &d0, double &d) {
			assert( std::fabs(lv.norm() - 1.0) < 1e-6 && "Line vector is not normalized.!");
			Point trial = P0 - l0;
			Point parallell = trial.dot(lv)*lv;
			Point perpendicular = trial - parallell;
			d = perpendicular.norm();
			d0 = l0 + parallell;
		      }
		      
		      /**
			* @brief Calculates the distance between a point and a line, and the closest point on that line
			* @param l0a Point on line 'A'
			* @param l0b Point on line 'A'
			* @param lva Normlalized direction of line 'A'
			* @param lvb Normlalized direction of line 'B'
			* @param Pao Closest point on line 'A' to line 'B'
			* @param Pbo Closest point on line 'B' to line 'A'
			* @param distance Minimum distance between lines 'A' and 'B'
			*/
		      void getClosestDistanceBetweenLines(Point l0a, Point l0b, Point lva, Point lvb, Point &Pao, Point &Pbo, double &distance) {
			assert( std::fabs(lva.norm() - 1.0) < 1e-6 && "Vector is not normalized.!");
			assert( std::fabs(lvb.norm() - 1.0) < 1e-6 && "Vector is not normalized.!");
			
			Point n = lva.cross(lvb);
			n /= n.norm();
			double da = -n.dot(l0a);
			double db = -n.dot(l0b);
			distance = std::fabs(db - da);
			
			Point n1 = lva.cross(lvb.cross(lva));
			Point n2 = lvb.cross(lva.cross(lvb));
			Pao = l0a + (l0b-l0a).dot(n2)*lva/(lva.dot(n2));
			Pbo = l0b + (l0a-l0b).dot(n1)*lvb/(lvb.dot(n1));
			
			Point vec = Pao - Pbo;
			if(std::fabs(vec.norm() - distance) > 1e-6)
			  std::runtime_error( name+": Something is wrong!.");
		      }
		      
		      bool getPointsOnCircleAndLine(Point c0, Point nc, double Rc, Point l0, Point lv, Point &a, Point &b) {
			Point d0(0,0,0);
			double d = 0.0;
			getDistanceBetweenPointAndline(c0,lv,l0,d0,d);
			if(d > Rc)
			  return false;
			
			double C = d0.dot(d0);
			double B = -2.0*lv.dot(d0);
			double A = lv.dot(lv);
			    
			double alpha1 = -(B - sqrt(-4.0*A*C+B*B))/(2.0*A);
			double alpha2 = -(B + sqrt(-4.0*A*C+B*B))/(2.0*A);
			    
			a = d0 + alpha1*lv; // First point on both line and circle
			b = d0 + alpha2*lv; // Second point on both line and circle
			
			return true;
		      }
		      
		      double checkIfCirclesAreLinked(const Point &ca, const Point &cb, const Point &dira, const Point &dirb, double rca, double rcb) {
			Point lv(0,0,0); // Direction of line
			Point l0(0,0,0); // Point on line
			getLineOfIntersectingPlanes(ca,cb,dira,dirb,l0,lv); // Calculate values
			
			Point a1(0,0,0);
			Point a2(0,0,0);
			bool crossA = getPointsOnCircleAndLine(ca,dira,rca,l0,lv,a1,a2);
			
			//cout << "checkIfCirclesAreLinked 1!" << endl;
			if(!crossA)
			  return 0.0;
			
			Point b1(0,0,0);
			Point b2(0,0,0);
			bool crossB = getPointsOnCircleAndLine(cb,dirb,rcb,l0,lv,b1,b2);
			
			//cout << "checkIfCirclesAreLinked 2!" << endl;
			if(!crossB)
			  return 0.0;
			
			//cout << "checkIfCirclesAreLinked 3!" << endl;
			if((a1-cb).norm() < rcb)
			  return pc::infty;
			if((a2-cb).norm() < rcb)
			  return pc::infty;
			if((b1-ca).norm() < rca)
			  return pc::infty;
			if((b2-ca).norm() < rca)
			  return pc::infty;
			
			//cout << "checkIfCirclesAreLinked 4!" << endl;
			return 0.0;
		      }
		      
		      Point getClosestPointOnCircleToLine(Point l0, Point lv, Point c0, Point dir, double Rc) {
			double d = -dir.dot(c0);
			double t = -(c0.dot(l0) + d )/c0.dot(lv);
			Point ld = l0 + t*lv;
			Point vec = ld - c0;
			Point P0 = c0 + vec/vec.norm()*Rc;
			return P0;
		      }
		      
		      double checkTubeCircleOverlap(Point l0, Point lv,double Rt, Point c0, Point dirc, double Rc) {
			Point P0 = getClosestPointOnCircleToLine(l0,lv,c0,dirc,Rc);
			Point Pl(0,0,0);
			double d = 0.0;
			getDistanceBetweenPointAndline(P0,lv,l0,Pl,d);
			if(d < Rt) {
			  //cout << "checkTubeCircleOverlap!" << endl;
			  return pc::infty;
			}
			return 0.0;
		      }
		      
		      
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
		      double r1 = r.norm();
		      if(a.is_sphere() && b.is_sphere()) {
			if(r1 < a.radius + b.radius)
			  return pc::infty;
			return 0.0;
		      }
		      if(!a.is_sphere() && !b.is_sphere()) {
			if(r1 > a.diagonal() + b.diagonal())
			  return 0.0;
			
			//cout << "Begin!" << endl;
			
			Point a_pos = r;
			Point b_pos = Point(0,0,0);
			
			// Get the closest points on the lines going through the tubes 
			Point Pao(0,0,0);
			Point Pbo(0,0,0);
			double distance = 0.0;
			getClosestDistanceBetweenLines(a_pos, b_pos,a.direction(),b.direction(),Pao,Pbo,distance);
			if(distance > a.radius_tube() + b.radius_tube())
			  return 0.0;
			
			//cout << "Past 1!" << endl;
			
			bool a_edge = false;
			bool b_edge = false;
			Point vecA = Pao - a_pos;
			if(vecA.norm() > a.height_tube()) {
			  Pao = a_pos + vecA/vecA.norm()*a.height_tube();
			  a_edge = true;
			}
			Point vecB = Pbo - b_pos;
			if(vecB.norm() > b.height_tube()) {
			  Pbo = b_pos + vecB/vecB.norm()*b.height_tube();
			  b_edge = true;
			}
			
			if(!a_edge && !b_edge) {
			  //cout << "Middle of tubes!" << endl;
			  return pc::infty;
			}
			
			//cout << "Past 2!" << endl;
			
			if(a_edge && b_edge) {
			  return pc::infty;
			  return checkIfCirclesAreLinked(Pao,Pbo,a.direction(),b.direction(),a.radius_tube(),b.radius_tube());
			}
			
			//cout << "Past 3!" << endl;
			
			if(a_edge)
			  return checkTubeCircleOverlap(b_pos,b.direction(),b.radius_tube(),Pao,a.direction(),a.radius_tube());
			return checkTubeCircleOverlap(a_pos,a.direction(),a.radius_tube(),Pbo,b.direction(),b.radius_tube());
			
			//std::runtime_error( name+": Can not describe tube-tube interactions at the moment.");
		      }
		      
		      // Tube-sphere interaction
		      Tparticle tube = a;
		      Tparticle sphere = b;
		      if(a.is_sphere()) {
			sphere = a;
			tube = b;
		      }
		      
		      double z = tube.direction().dot(r);
		      double theta = std::acos(z/r1);
		      Point rc_v = r - z*tube.direction();
		      double rc = rc_v.norm();
		      double rs = sphere.radius;
		      double rt = tube.radius_tube();
		      double h = tube.height_tube();
		      double d = tube.diagonal();
		      
		      cout << "Temp43" << endl;
		      
		      if(rs < rt)
			if(rc + rs < rt)
			  return 0.0;
		      
		      if(rc - rs > rt)
			return 0.0;
		      if(std::fabs(z) - rs > h)
			return 0.0;
		      if(std::fabs(z) < h) {
			cout << "Here7" << endl;
			return pc::infty;
		      }
		      double alpha = std::fabs(tube.angle() - theta);
		      double rmin = sqrt(r1*r1 + d*d - 2.0*r1*d*cos(alpha));
		      if(rmin < rs) {
			cout << "Here8" << endl;
			return pc::infty;
		      }
		      return 0.0;
		    }

		string info(char w) {
		    using namespace textio;
		    std::ostringstream o;
		    return o.str();
		}
	};

	/**
	 * @brief Hard overlap interaction between 'Capparticles'.
	 * @warning If both particles in a pair-interactions have non-zero cap-radius and one of them 
	 * is large enough to fully enclose the other, the potential may give inaccurate results. For 
	 * such cases only surface-overlaps is checked and not volume-overlaps.
	 * @warning Only works correctly if the cavities of the capparticles are not overlapping one another.
	 */
	class HardSphereCap2 : public PairPotentialBase {
	    private:
		string _brief() {
		    std::ostringstream o;
		    o << "HardSphereCap2";
		    return o.str();          
		}

	    public:
		template<typename T>
		    HardSphereCap2(const T&, const string &sec="") : PairPotentialBase() { 
			name="HardsphereCap2"; 
		    }

		/**
		 * @brief Checks if a segmant of a spherical surface intersects with a segmant of another spherical surface.
		 * @param n_i Normalized direction-vector of particle 'i's spherical surface segment
		 * @param n_j Normalized direction-vector of particle 'j's spherical surface segment
		 * @param theta_i The azimuth angle of particle 'i's spherical surface segment
		 * @param theta_j The azimuth angle of particle 'j's spherical surface segment
		 * @param R_i Radius of particle 'i's spherical surface segment
		 * @param R_j Radius of particle 'j's spherical surface segment
		 * @param r_ij The distance-vector between the spherical surface segment centers ( r_ij = r_j - r_i )
		 * @note Implemented from  <https://doi.org/10.1103/PhysRevE.82.031405>
		 * @warning Tested, but not fully.
		 */
		bool ArcArcOverlap(Point n_i, Point n_j, double theta_i, double theta_j, double R_i, double R_j, bool i_is_sphere, bool j_is_sphere, Point r_ij) const {
		    assert( ( ( fabs(n_i.norm()-1.0) < 1e-6 ) || ( fabs(n_j.norm()-1.0) < 1e-6 ) ) && "Center-of-cap point is not normalized!");
		    double r2 = r_ij.squaredNorm();
		    double r1 = sqrt(r2);
		    Point r_hat = -r_ij/r1; // Now 'r_hat' points from particle 'i' towards particle 'j'. Same as in article

		    if( ( R_i + R_j ) < r1 )
			return false; // No overlap at all

		    if( fabs( R_i - R_j ) > r1 )
			return false; // One of the spheres is encapsulating the other one

		    double cosPhi_ij = ( R_i*R_i - R_j*R_j + r1*r1 )/( 2.0*r1*R_i ); // Eq. A5
		    double cosPhi_ji = ( R_j*R_j - R_i*R_i + r1*r1 )/( 2.0*r1*R_j ); // Eq. A5
		    double phi_ij = acos( cosPhi_ij ); // Eq. A5, gives prinicpal value of phi_ij
		    double phi_ji = acos( cosPhi_ji ); // Eq. A5, gives prinicpal value of phi_ji
		    double cosW_ij = n_i.dot(-r_hat); // Eq. A6
		    double cosW_ji = n_j.dot(r_hat);   // Eq. A6
		    double w_ij = acos( cosW_ij );     // Eq. A6, gives prinicpal value of w_ij
		    double w_ji = acos( cosW_ji );     // Eq. A6, gives prinicpal value of w_ji

		    bool aijp = false;
		    bool aijm = false;
		    bool ajip = false;
		    bool ajim = false;
		    if( fabs(w_ij + phi_ij) < theta_i || theta_i >= pc::pi ) // Eq. A4 + extra condition
			aijp = true;
		    if( fabs(w_ij - phi_ij) < theta_i || theta_i >= pc::pi ) // Eq. A4 + extra condition
			aijm = true;
		    if( fabs(w_ji + phi_ji) < theta_j || theta_j >= pc::pi ) // Eq. A4 + extra condition
			ajip = true;
		    if( fabs(w_ji - phi_ji) < theta_j || theta_j >= pc::pi ) // Eq. A4 + extra condition
			ajim = true;

		    if(!aijp && !aijm && !ajip && !ajim)
			return false;
		    if(j_is_sphere && ( aijp || aijm ) )
			return true;
		    if(i_is_sphere && ( ajip || ajim ) )
			return true;
		    if(aijp && aijm && ( ajip || ajim ))
			return true;
		    if( ( aijp || aijm ) && ajip && ajim )
			return true;

		    double gamma_i = acos( ( cos(theta_i) - cosPhi_ij*cosW_ij )/( sqrt(1.0-cosPhi_ij*cosPhi_ij)*sqrt(1.0-cosW_ij*cosW_ij) ) ); // Eq. A9
		    double gamma_j = acos( ( cos(theta_j) - cosPhi_ji*cosW_ji )/( sqrt(1.0-cosPhi_ji*cosPhi_ji)*sqrt(1.0-cosW_ji*cosW_ji) ) ); // Eq. A9
		    Point np_i = n_i - cosW_ij*r_hat;
		    Point np_j = n_j - cosW_ji*(-r_hat);
		    double alpha_ij = acos( np_i.dot(np_j)/(np_i.norm()*np_j.norm()) ); // Eq. A8
		    if(fabs(alpha_ij) < fabs(gamma_i) + fabs(gamma_j))                  // Eq. A7
			return true;

		    return false;
		}

		/**
		 * @brief Checks is a particle with a cap collides with a true sphere.
		 * @param capsphere The cap-particle
		 * @param sphere The sphere particle
		 * @param r The distance-vector ( sphere.xyz - capsphere.xyz )
		 * @param r1 The length of parameter 'r'
		 * @note Important that the 'r'-parameter is in the correct direction!
		 */
		template<class Tparticle>
		    bool CapsphereSphereOverlap22(const Tparticle &capsphere, const Tparticle &sphere, const Point &r, double r1, int I) const {
		      bool arcoverlapFront = ArcArcOverlap(-capsphere.cap_center_point(I),Point(1,0,0), capsphere.angle_c(I), pc::pi, capsphere.cap_radius(I), sphere.radius, false,true, r - capsphere.cap_center_point(I)*capsphere.cap_center(I)); // front-back surface overlap
		      if(arcoverlapFront)
		        return true;
		      
		      if( r1 < capsphere.radius ) {
			Point cts =  ( r - capsphere.cap_center_point(I)*capsphere.cap_center(I) );    // Vector from origin of capsphere.cap pointing towards the center of 'sphere'
			double cts_norm = cts.norm();
			if(cts_norm < capsphere.cap_radius(I)) {
			  if(sphere.radius > capsphere.cap_radius(I))
			    return true;
			  if(capsphere.cap_radius(I) - cts_norm < sphere.radius)
			    return true;
			  return false;
			}
			return true;
		      }
		      
		      bool arcoverlapBack = ArcArcOverlap(-capsphere.cap_center_point(I),Point(1,0,0), capsphere.angle_p(I), pc::pi, capsphere.radius, sphere.radius, false, true, r); // back-back surface overlap
		      if(arcoverlapBack)
			return true;
		      return false;
		    }

		template<class Tparticle>
		    bool CapsphereSphereOverlap(const Tparticle &capsphere, const Tparticle &sphere, const Point &r, double r1, int I) const {
			bool arcoverlapFront = ArcArcOverlap(-capsphere.cap_center_point(I),Point(1,0,0), capsphere.angle_c(I), pc::pi, capsphere.cap_radius(I), sphere.radius, false,true, r - capsphere.cap_center_point(I)*capsphere.cap_center(I)); // front-back surface overlap
			if(arcoverlapFront)
			    return true;

			if( r1 < capsphere.radius ) {
			    Point cts =  ( r - capsphere.cap_center_point(I)*capsphere.cap_center(I) );    // Vector from origin of capsphere.cap pointing towards the center of 'sphere'
			    double cts_norm = cts.norm();
			    if(cts_norm < capsphere.cap_radius(I)) {
				if(sphere.radius > capsphere.cap_radius(I))
				    return true;
				if(capsphere.cap_radius(I) - cts_norm < sphere.radius)
				    return true;
				return false;
			    }
			    return true;
			}

			bool arcoverlapBack = ArcArcOverlap(-capsphere.cap_center_point(I),Point(1,0,0), capsphere.angle_p(I), pc::pi, capsphere.radius, sphere.radius, false, true, r); // back-back surface overlap
			if(arcoverlapBack)
			    return true;
			return false;
		    }

		bool sphereOverlap(Point a, Point b, double ra, double rb) {
		    if( (a - b).norm() > ra + rb)
			return false;
		    return true;
		}

		/**
		 * @brief Checks for overlap between two particles
		 * @param r The distance-vector between the spherical surface segment centers ( r = r_a - r_b )
		 * @note Should be correct
		 */
		template<class Tparticle>
		    bool capcapOverlap(const Tparticle &a, const Tparticle &b, const Point &r) {

			// Find cavities that are closest to each other
			double minV = pc::infty;
			int ia = -1;
			int ib = -1;
			for(int Ia = 0; Ia < a.nbrOfCav(); Ia++) {
			    for(int Ib = 0; Ib < b.nbrOfCav(); Ib++) {
				Point r_t = r + a.cap_center_point(Ia)*a.cap_center(Ia) - b.cap_center_point(Ib)*b.cap_center(Ib);
				double r_t2 = r_t.squaredNorm();

				if(r_t2 < minV) {
				    minV = r_t2;
				    ia = Ia;
				    ib = Ib;
				}
			    }
			}

			// Now we know both particles to be 'Capparticles'
			if( ArcArcOverlap(-a.cap_center_point(ia),-b.cap_center_point(ib), a.angle_p(ia), b.angle_p(ib), a.radius, b.radius, a.is_sphere(), b.is_sphere(), -r) ) // back-back surface overlap
			    return true;

			if( ArcArcOverlap(-a.cap_center_point(ia),-b.cap_center_point(ib), a.angle_p(ia), b.angle_c(ib), a.radius, b.cap_radius(ib), a.is_sphere(), b.is_sphere(), -r + b.cap_center_point(ib)*b.cap_center(ib)) ) // back-front surface overlap
			    return true;

			if( ArcArcOverlap(-a.cap_center_point(ia),-b.cap_center_point(ib), a.angle_c(ia), b.angle_p(ib), a.cap_radius(ia), b.radius, false,false, -r - a.cap_center_point(ia)*a.cap_center(ia)) ) // front-back surface overlap
			    return true;

			if( ArcArcOverlap(-a.cap_center_point(ia),-b.cap_center_point(ib), a.angle_c(ia), b.angle_c(ib), a.cap_radius(ia), b.cap_radius(ib), a.is_sphere(), b.is_sphere(), -r - a.cap_center_point(ia)*a.cap_center(ia) + b.cap_center_point(ib)*b.cap_center(ib)) ) // front-front surface overlap
			    return true;

			return false;
		    }

		/**
		 * @param  a Particle
		 * @param  b Particle
		 * @param  r The distance-vector ( r_a - r_b )
		 * @note Should be correct
		 * */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			double r1 = r.norm();
			if( r1 > a.radius+b.radius )
			    return 0.0;

			if(a.is_sphere() && b.is_sphere())
			    return pc::infty;

			// Now we know at least one particles to be a 'Capparticle'
			if(a.is_sphere()) {
			    // Find cavity that are closest to sphere
			    // If one cap_radius is larger than the radius of the particle, this procedure is be flawed
			    double minV = pc::infty;
			    int ib = -1;
			    for(int Ib = 0; Ib < b.nbrOfCav(); Ib++) {
			      //cout << "Nbr of cav b: " << b.nbrOfCav() << ", " << b.cap_center_point(Ib).transpose() << ", " << b.cap_center(Ib) << endl;
				Point r_t = r - b.cap_center_point(Ib)*b.cap_center(Ib);
				double r_t2 = r_t.squaredNorm();

				if(r_t2 < minV) {
				    minV = r_t2;
				    ib = Ib;
				}
			    }
			    //return double(ib+3);
			    //double Rt = b.cap_center(ib) - b.cap_radius(ib);
			    //if(Rt + a.radius< r1)
			    //  return pc::infty;
			    //return Rt;
			    //cout << "Checking overlap! " << CapsphereSphereOverlap(b, a,r, r1,ib) << endl;
			    bool booltmp = CapsphereSphereOverlap(b, a,r, r1,ib);
			    return booltmp ? pc::infty : 0.0;
			}

			if(b.is_sphere()) {
			    // Find cavity that are closest to sphere
			    // If one cap_radius is larger than the radius of the particle, this procedure is be flawed
			    double minV = pc::infty;
			    int ia = -1;
			    
			    for(int Ia = 0; Ia < a.nbrOfCav(); Ia++) {
				Point r_t = r + a.cap_center_point(Ia)*a.cap_center(Ia);
				//cout << "Nbr of cav a: " << a.nbrOfCav() << ", " << a.cap_center_point(Ia).transpose() << ", " << a.cap_center(Ia) << endl;
				double r_t2 = r_t.squaredNorm();

				if(r_t2 < minV) {
				    minV = r_t2;
				    ia = Ia;
				}
			    }
			    //return double(ia+3);
			    //double Rt = a.cap_center(ia) - a.cap_radius(ia);
			    //if(Rt + b.radius< r1)
			    //  return pc::infty;
			    //return Rt;
			    //cout << "Checking overlap! " << CapsphereSphereOverlap(a, b,-r, r1,ia) << endl;
			    bool booltmp = CapsphereSphereOverlap(a, b,-r, r1,ia);
			    return booltmp ? pc::infty : 0.0;
			}

			// Now we know both particles to be 'Capparticles'
			bool overlap = capcapOverlap(a,b,r);
			assert( overlap == capcapOverlap(b,a,-r) && "HardSphereCap2! Code is wrong!");
			return overlap ? pc::infty : 0.0;
		    }

		string info(char w) {
		    using namespace textio;
		    std::ostringstream o;
		    return o.str();
		}
	};


















	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
/*
	class SoftLockKey : public PairPotentialBase {
	    private:
		string _brief() {
		    std::ostringstream o;
		    o << "SoftLockKey";
		    return o.str();          
		}

	    public:
		template<typename T>
		    SoftLockKey(const T&, const string &sec="") : PairPotentialBase() { 
			name="SoftLockKey"; 
		    }

		bool ArcArcOverlap(Point n_i, Point n_j, double theta_i, double theta_j, double R_i, double R_j, bool i_is_sphere, bool j_is_sphere, Point r_ij) const {

		}

		template<class Tparticle>
		    bool CapsphereSphereOverlap22(const Tparticle &capsphere, const Tparticle &sphere, const Point &r, double r1, int I) const {
		    }

		template<class Tparticle>
		    bool CapsphereSphereOverlap(const Tparticle &capsphere, const Tparticle &sphere, const Point &r, double r1, int I) const {
		    }

		bool sphereOverlap(Point a, Point b, double ra, double rb) {
		}

		template<class Tparticle>
		    bool capcapOverlap(const Tparticle &a, const Tparticle &b, const Point &r) {
		    }
		    
		    // a.cap_center_point(ia) b.cap_center_point(ib)  a.angle_p(ia)  b.angle_p(ib)  a.radius  b.radius  a.is_sphere()  b.is_sphere()

		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			double r1 = r.norm();
			
			
			
			
			
			return overlap ? pc::infty : 0.0;
		    }

		string info(char w) {
		    using namespace textio;
		    std::ostringstream o;
		    return o.str();
		}
	};
	
	*/
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
























	/**
	 * @brief Hard overlap interaction between 'Capparticles'.
	 * @warning If both particles in a pair-interactions have non-zero cap-radius and one of them 
	 * is large enough to fully enclose the other, the potential may give inaccurate results. For 
	 * such cases only surface-overlaps is checked and not volume-overlaps.
	 */
	class HardSphereCap : public PairPotentialBase {
	    private:
		vector<bool> issphere;
		string _brief() {
		    std::ostringstream o;
		    o << "HardSphereCap";
		    return o.str();          
		}

	    public:
		template<typename T>
		    HardSphereCap(const T&, const string &sec="") : PairPotentialBase() { 
			name="HardsphereCap"; 
			issphere.resize(atom.size());

			int cnt = 0;
			for (auto &i : atom)
			    if( ( i.cap_radiusV.size() > 0 ) || ( i.cap_radius > 1e-6 ) ) {
				issphere.at(cnt++) = false;
			    } else {
				issphere.at(cnt++) = true;
			    }
		    }

		/**
		 * @brief Checks if a segmant of a spherical surface intersects with a segmant of another spherical surface.
		 * @param n_i Normalized direction-vector of particle 'i's spherical surface segment
		 * @param n_j Normalized direction-vector of particle 'j's spherical surface segment
		 * @param theta_i The azimuth angle of particle 'i's spherical surface segment
		 * @param theta_j The azimuth angle of particle 'j's spherical surface segment
		 * @param R_i Radius of particle 'i's spherical surface segment
		 * @param R_j Radius of particle 'j's spherical surface segment
		 * @param r_ij The distance-vector between the spherical surface segment centers ( r_ij = r_j - r_i )
		 * @note Implemented from  <https://doi.org/10.1103/PhysRevE.82.031405>
		 * @warning Tested, but not fully.
		 */
		bool SurfaceSurfaceOverlap(Point n_i, Point n_j, double theta_i, double theta_j, double R_i, double R_j, bool i_is_sphere, bool j_is_sphere, Point r_ij) const {
		    assert( ( ( fabs(n_i.norm()-1.0) < 1e-6 ) || ( fabs(n_j.norm()-1.0) < 1e-6 ) ) && "Center-of-cap point is not normalized!");
		    double r2 = r_ij.squaredNorm();
		    double r1 = sqrt(r2);
		    Point r_hat = -r_ij/r1; // Now 'r_hat' points from particle 'i' towards particle 'j'. Same as in article

		    if( ( R_i + R_j ) < r1 )
			return false; // No overlap at all

		    if( fabs( R_i - R_j ) > r1 )
			return false; // One of the spheres is encapsulating the other one

		    double cosPhi_ij = ( R_i*R_i - R_j*R_j + r1*r1 )/( 2.0*r1*R_i ); // Eq. A5
		    double cosPhi_ji = ( R_j*R_j - R_i*R_i + r1*r1 )/( 2.0*r1*R_j ); // Eq. A5
		    double phi_ij = acos( cosPhi_ij ); // Eq. A5, gives prinicpal value of phi_ij
		    double phi_ji = acos( cosPhi_ji ); // Eq. A5, gives prinicpal value of phi_ji
		    double cosW_ij = n_i.dot(-r_hat); // Eq. A6
		    double cosW_ji = n_j.dot(r_hat);   // Eq. A6
		    double w_ij = acos( cosW_ij );     // Eq. A6, gives prinicpal value of w_ij
		    double w_ji = acos( cosW_ji );     // Eq. A6, gives prinicpal value of w_ji

		    bool aijp = false;
		    bool aijm = false;
		    bool ajip = false;
		    bool ajim = false;
		    if( fabs(w_ij + phi_ij) < theta_i || theta_i >= pc::pi ) // Eq. A4 + extra condition
			aijp = true;
		    if( fabs(w_ij - phi_ij) < theta_i || theta_i >= pc::pi ) // Eq. A4 + extra condition
			aijm = true;
		    if( fabs(w_ji + phi_ji) < theta_j || theta_j >= pc::pi ) // Eq. A4 + extra condition
			ajip = true;
		    if( fabs(w_ji - phi_ji) < theta_j || theta_j >= pc::pi ) // Eq. A4 + extra condition
			ajim = true;

		    if(!aijp && !aijm && !ajip && !ajim)
			return false;
		    if(j_is_sphere && ( aijp || aijm ) )
			return true;
		    if(i_is_sphere && ( ajip || ajim ) )
			return true;
		    if(aijp && aijm && ( ajip || ajim ))
			return true;
		    if( ( aijp || aijm ) && ajip && ajim )
			return true;

		    double gamma_i = acos( ( cos(theta_i) - cosPhi_ij*cosW_ij )/( sqrt(1.0-cosPhi_ij*cosPhi_ij)*sqrt(1.0-cosW_ij*cosW_ij) ) ); // Eq. A9
		    double gamma_j = acos( ( cos(theta_j) - cosPhi_ji*cosW_ji )/( sqrt(1.0-cosPhi_ji*cosPhi_ji)*sqrt(1.0-cosW_ji*cosW_ji) ) ); // Eq. A9
		    Point np_i = n_i - cosW_ij*r_hat;
		    Point np_j = n_j - cosW_ji*(-r_hat);
		    double alpha_ij = acos( np_i.dot(np_j)/(np_i.norm()*np_j.norm()) ); // Eq. A8
		    if(fabs(alpha_ij) < fabs(gamma_i) + fabs(gamma_j))                  // Eq. A7
			return true;

		    return false;
		}

		/**
		 * @brief Checks is a particle with a cap collides with a true sphere.
		 * @param capsphere The cap-particle
		 * @param sphere The sphere particle
		 * @param r The distance-vector ( sphere.xyz - capsphere.xyz )
		 * @param r1 The length of parameter 'r'
		 * @note Important that the 'r'-parameter is in the correct direction!
		 */
		template<class Tparticle>
		    bool CapsphereSphereOverlap(const Tparticle &capsphere, const Tparticle &sphere, const Point &r, double r1) const {
			assert( fabs(capsphere.cap_center_point().norm()-1.0) < 1e-6 && "Center-of-cap point is not normalized!");
			double angle = acos(r.dot(capsphere.cap_center_point())/r1); // Angle between capsphere-center to sphere-center and capsphere-center to cap-center
			if( (angle > pc::pi - capsphere.angle_p()) && (r1 > capsphere.radius) ) // If the sphere is outside the prolonged cone formed by the origin of the cap-particle and the cap-'ring' then...
			    return true;

			Point cts =  ( r - capsphere.cap_center_point()*capsphere.cap_center() );    // Vector from origin of capsphere.cap pointing towards the center of 'sphere'
			double cts_norm = cts.norm();
			if( ( cts_norm > capsphere.cap_radius() - sphere.radius ) && (angle > pc::pi - capsphere.angle_p()) )
			    return true;

			Point cap_vector = capsphere.cap_center_point()*capsphere.cap_center() + cts/cts_norm*capsphere.cap_radius();
			Point closest_cap_point = capsphere + cap_vector; 			  // Closesed point from sphere.center to surface of capsphere.cap
			if( ( (capsphere - closest_cap_point).norm() <= capsphere.radius )) {   // If that point is closer than capsphere.radius then it truly is on the capsphere.cap
			    if( (-r  + cap_vector ).norm() < sphere.radius)          		  // If the distance between sphere.center to the closesed point on capsphere is shorter than the radius of sphere then...
				return true;                                                  	  // Return collision
			    //cout << "False 1!" << endl;
			    return false;                                                    	  // Return no collision
			} else {
			    // Now we are on the cap-surface that is not really a true surface on the particle
			    double closest_cap_distance_squared = capsphere.radius*capsphere.radius + r1*r1 - 2.0*capsphere.radius*r1*cos(pc::pi - capsphere.angle_p() - angle); // Closesed distance to the rim of the cap and sphere
			    if(closest_cap_distance_squared < sphere.radius*sphere.radius)
				return true;
			    //cout << "False 2! " << capsphere.transpose() << ", " << sphere.transpose() << ", " << closest_cap_point.transpose() << endl;
			    //cout << "False 2! " << capsphere.cap_center_point().transpose() << endl;
			    return false;
			}
			//cout << "False 3!" << endl;
			return false;
		    }

		bool sphereOverlap(Point a, Point b, double ra, double rb) {
		    if( (a - b).norm() > ra + rb)
			return false;
		    return true;
		}

		/**
		 * @param  r The distance-vector ( r_a - r_b )
		 * 
		 * */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			//cout << "HardSphereCap! " << a.cap_center_point().transpose() << ", " << a.cap_center_point().norm() << ", " << b.cap_center_point().transpose() << ", " << b.cap_center_point().norm() << endl;
			//cout << "Particles: a: " << a.is_sphere() << ", b: " << b.is_sphere() << endl;

			double r1 = r.norm();
			if( r1 > a.radius+b.radius )
			    return 0.0;

			if(a.is_sphere() && b.is_sphere())
			    return pc::infty;

			// Now we know at least one particles to be a 'Capparticle'
			if(a.is_sphere()) {
			    double Rt = b.cap_center() - b.cap_radius();
			    if(Rt + a.radius< r1)
			      return pc::infty;
			    return CapsphereSphereOverlap(b, a,r, r1) ? pc::infty : 0.0;
			}

			if(b.is_sphere()) {
			    double Rt = a.cap_center() - a.cap_radius();
			    if(Rt + b.radius< r1)
			      return pc::infty;
			    return CapsphereSphereOverlap(a, b,-r, r1) ? pc::infty : 0.0;
			}

			if(!sphereOverlap(r,b.cap_center_point()*b.cap_center(),a.radius,b.cap_radius())) // If cavity of 'b' does not cover 'a' at all, then we consider 'b' as a sphere
			    return CapsphereSphereOverlap(a, b,-r, r1) ? pc::infty : 0.0;

			if(!sphereOverlap(r+a.cap_center_point()*a.cap_center(),Point(0,0,0),a.cap_radius(),b.radius)) // If cavity of 'a' does not cover 'b' at all, then we consider 'a' as a sphere
			    return CapsphereSphereOverlap(b, a,r, r1) ? pc::infty : 0.0;

			// cap of 'a' overlap with 'b' AND cap of 'b' overlap with 'a'

			if(!sphereOverlap(r+a.cap_center_point()*a.cap_center(),b.cap_center_point()*b.cap_center(),a.cap_radius(),b.cap_radius())) {
			    // The caps do not overlap! (but all other spheres do)

			    if(CapsphereSphereOverlap(b, a,r, r1) || CapsphereSphereOverlap(a, b,-r, r1))
				return pc::infty;
			    return 0.0;
			}

			// All spheres intersect!


			// Now we know both particles to be 'Capparticles'
			if( SurfaceSurfaceOverlap(-a.cap_center_point(),-b.cap_center_point(), a.angle_p(), b.angle_p(), a.radius, b.radius, a.is_sphere(), b.is_sphere(), -r) ) // back-back surface overlap
			    return pc::infty;

			if( SurfaceSurfaceOverlap(-a.cap_center_point(),-b.cap_center_point(), a.angle_p(), b.angle_c(), a.radius, b.cap_radius(), a.is_sphere(), b.is_sphere(), -r + b.cap_center_point()*b.cap_center()) ) // back-front surface overlap
			    return pc::infty;

			if( SurfaceSurfaceOverlap(-a.cap_center_point(),-b.cap_center_point(), a.angle_c(), b.angle_p(), a.cap_radius(), b.radius, false,false, -r - a.cap_center_point()*a.cap_center()) ) // front-back surface overlap
			    return pc::infty;

			if( SurfaceSurfaceOverlap(-a.cap_center_point(),-b.cap_center_point(), a.angle_c(), b.angle_c(), a.cap_radius(), b.cap_radius(), a.is_sphere(), b.is_sphere(), -r - a.cap_center_point()*a.cap_center() + b.cap_center_point()*b.cap_center()) ) // front-front surface overlap
			    return pc::infty;

			return 0.0;
		    }

		string info(char w) {
		    using namespace textio;
		    std::ostringstream o;
		    return o.str();
		}
	};

	/**
	 * @brief Lennard-Jones (12-6) pair potential
	 *
	 * The Lennard-Jones potential has the form:
	 * @f$
	 * \beta u=4\epsilon_{lj}
	 * \left ((\sigma_{ij}/r_{ij})^{12}-(\sigma_{ij}/r_{ij})^6\right )
	 * @f$
	 * where
	 * \f$\sigma_{ij} = (\sigma_i+\sigma_j)/2\f$
	 * and \f$\epsilon_{lj}=\epsilon\f$
	 * is the same for all pairs in this class.
	 *
	 * JSON keywords:
	 *
	 * Keyword    |  Description
	 * :--------- |  :------------------------
	 * `eps`      |  Interaction strength
	 * `unit`     |  Unit of interaction strength - can be `kJ/mol` or `kT` (default)  
	 */
	class LennardJones : public PairPotentialBase {
	    private:
		string _brief();
	    protected:
		inline double r6(double sigma, double r2) const {
		    double x(sigma*sigma/r2);  // 2
		    return x*x*x;             // 6
		}
		double eps;
	    public:
		LennardJones();

		LennardJones(Tmjson &j);

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
			double x(r6(a.radius+b.radius,r2));
			return eps*(x*x - x);
		    }
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			return operator()(a,b,r.squaredNorm());
		    }
		template<typename Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			double s6=pow(a.radius+b.radius,6); //pow() is slow
			double r6=r2*r2*r2;
			double r14=r6*r6*r2;
			return 6.*eps*s6*(2*s6-r6)/r14*p;
		    }

		string info(char);
	};









	class WCAB : public PairPotentialBase {
	    private:
		string _brief();
	    protected:
		double eps;
		int N;
		double rho;
		vector<double> P;
	    public:
		WCAB();

		WCAB(Tmjson &j);

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
			return eps*pow(1.0/sqrt(r2),double(N));
		    }
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			return operator()(a,b,r.squaredNorm());
		    }
		template<typename Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			return double(N)*eps*pow(1.0/sqrt(r2),double(N-1))*p;
		    }

		string info(char);
	};


	class Buckingham : public PairPotentialBase {
	    private:
		string _brief();
	    protected:
		double rho;
		vector<double> P;
		inline double B(vector<double> Pin, double r1) const {
		    double Pout = 0.0;
		    for(unsigned int k = 0; k < Pin.size();k++)
			Pout += Pin.at(k)*pow(r1,double(k));
		    return Pout;
		}
		inline double dB(vector<double> Pin, double r1) const {
		    double dPout = 0.0;
		    for(unsigned int k = 0; k < Pin.size();k++)
			dPout += double(k)*Pin.at(k)*pow(r1,double(k)-1.0);
		    return dPout;
		}
	    public:
		Buckingham();

		Buckingham(Tmjson &j);

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
			double r1 = sqrt(r2);
			return (B(P,r1)*exp(-r1/rho));
		    }
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			return operator()(a,b,r.squaredNorm());
		    }
		template<typename Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			double r1 = sqrt(r2);
			return ( dB(P,r1)*rho - B(P,r1) )*exp(-r1/rho)/rho*p;
		    }

		string info(char);
	};

	class RX : public PairPotentialBase {
	    private:
		string _brief();
	    protected:
		double eps;
		int N;
		double rho;
		vector<double> P;
		double rswitch;

		inline double B(vector<double> Pin, double r1) const {
		    double Pout = 0.0;
		    for(unsigned int k = 0; k < Pin.size();k++)
			Pout += Pin.at(k)*pow(r1,double(k));
		    return Pout;
		}
		inline double dB(vector<double> Pin, double r1) const {
		    double dPout = 0.0;
		    for(unsigned int k = 0; k < Pin.size();k++)
			dPout += double(k)*Pin.at(k)*pow(r1,double(k)-1.0);
		    return dPout;
		}
		inline double functionZero(vector<double> Pin, double rho, int N, double rs) const {
		    return ((1.0/rho) - dB(Pin,rs)/B(Pin,rs) - double(N)/rs);
		}
		inline double NewtonRaphson(vector<double> Pin, double rho, int N, double rs, double h) const {
		    double f = functionZero(Pin,rho,N,rs);
		    double df = (functionZero(Pin,rho,N,rs+h) - functionZero(Pin,rho,N,rs-h))/(2*h);
		    return (rs - f/df);
		}
		inline double findRswitch(double rs=1.0, double maxerror=1e-6, int maxcnt=1e3, double h=1e-6) const {
		    rs = double(N)*rho;
		    //cout << "eps: " << P.at(0)*pow(double(N)*rho/exp(1.0),double(N)) << endl;
		    return rs;
		    double error = pc::infty;
		    int cnt = 0;
		    double rs_old = rs;
		    while( (fabs(error) > maxerror) && (cnt <= maxcnt) ) {
			rs = NewtonRaphson(P,rho,N,rs_old,h);
			error = rs - rs_old;
			cnt++;
		    }
		    return rs;
		}
	    public:
		RX();

		RX(Tmjson &j);

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
			double r1 = sqrt(r2);
			if(r1 > rswitch)
			    return (B(P,r1)*exp(-r1/rho));
			return eps*pow(1.0/r1,double(N));
		    }
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			return operator()(a,b,r.squaredNorm());
		    }
		template<typename Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			double r1 = sqrt(r2);
			if(r1 > rswitch)
			    return ( dB(P,r1)*rho - B(P,r1) )*exp(-r1/rho)/rho*p;
			return double(N)*eps*pow(1.0/r1,double(N-1))*p;
		    }

		string info(char);
	};













	/**
	 * @brief Cuts a pair-potential and shift to zero at cutoff
	 *
	 * This will cut any pair potential at `cutoff` and shift to
	 * zero at that distance.
	 * If `precalc=false` the evaluation
	 * potential is *twice* is expensive as the original since the
	 * energy needs to be evaluated at the cutoff for each pair
	 * of particles. By setting `precalc=true`, this is pre-calculated
	 * in the constructor but assumes that particle properties such
	 * as charge, size etc. do not change during simulation.
	 *
	 * In addition to the JSON keywords taken by the original pair
	 * potential, `cutoff` must be supplied.
	 *
	 * Example:
	 *
	 * ~~~~
	 * using namespace Faunus::Potential;
	 * typedef CutShift<LennardJones> LennardJonesCutShift;
	 * ~~~~
	 *
	 * @tparam Tpairpot Pair potential to cut and shift
	 * @tparam precalc Pre-calculate energy at cutoff for all atom types for speedup.
	 *                 This is enabled by default (`true`) but will generate problems
	 *                 if particle properties fluctuate during simulation (charge swap moves,
	 *                 for example)
	 *
	 * @note Twice as expensive as `Tpairpot` if `precalc=false`
	 * @todo Implement continuous force calculation
	 */
	template<class Tpairpot, bool precalc=true>
	    class CutShift : public Tpairpot {
		private:
		    double rc2; // squared cut-off distance
		    PairMatrix<double> ucut; // energy at cutoff
		public:
		    CutShift( Tmjson &j ) : Tpairpot(j) {
			rc2 = pow( double(j.at("cutoff")), 2 );
			Tpairpot::name+=" (rcut="
			    + std::to_string( sqrt(rc2) ) + textio::_angstrom + ")";

			if (precalc) { // calculate energy at cutoff between all atom types
			    size_t n = atom.size(); // number of atom types
			    ucut.resize(n);
			    for (auto &i : atom)
				for (auto &j : atom) {
				    PointParticle a, b;
				    a = atom[i.id];
				    b = atom[j.id];
				    ucut.set(i.id, j.id, Tpairpot::operator()(a,b,rc2));
				}
			}
		    }

		    /** @brief Energy in kT between two particles, r2 = squared distance */
		    template<class Tparticle>
			double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			    if (r2>rc2)
				return 0;
			    if (precalc) // "precalc" deduced at compile time --> no overhead
				return Tpairpot::operator()(a,b,r2) - ucut(a.id, b.id);
			    else return Tpairpot::operator()(a,b,r2) - Tpairpot::operator()(a,b,rc2);
			}
	    };


	/** @brief Lorentz-Berthelot Mixing Rule for sigma and epsilon */
	struct LorentzBerthelot {
	    const std::string name = "Lorentz-Berthelot Mixing";
	    inline double mixSigma(double sigma1, double sigma2) const { return 0.5 * ( sigma1 + sigma2 ); }
	    inline double mixEpsilon(double eps1, double eps2) const { return sqrt( eps1 * eps2 ); }
	    /**
	     * @brief Function operator
	     * @param sigma Pair of sigma values
	     * @param eps Pair of epsilon values
	     * @returns Pair of mixed sigma (first) and epsilon (second)
	     */
	    inline std::pair<double,double> operator()( const std::pair<double,double> &sigma, const std::pair<double,double> &eps) const {
		return { 0.5 * ( sigma.first+sigma.second ), std::sqrt( eps.first*eps.second ) };
	    }
	};




	template<class Tmixingrule = LorentzBerthelot>
	    class SquaredWellMixed : public PairPotentialBase {
		protected:
		    Tmixingrule mixer; // mixing rule class for sigma and epsilon
		    string _brief() { return name + " w. " + mixer.name; }
		    PairMatrix<double> s,s2,eps; // matrix of sigma_ij^2 and eps_ij
		    double threshold;
		    double maxrange, maxrange2;

		    inline void init() {
			size_t n=atom.size(); // number of atom types
			s.resize(n); // not required..
			s2.resize(n); // not required...
			eps.resize(n);// ...but possible reduced mem. fragmentation
			for (auto &i : atom)
			    for (auto &j : atom) {
				double sigma, epsilon; // mixed values
				std::tie( sigma, epsilon ) = mixer( {i.sigma, j.sigma}, {i.eps, j.eps} );
				s.set(  i.id, j.id, sigma );
				s2.set(  i.id, j.id, sigma*sigma );
				eps.set( i.id, j.id, 4*epsilon );
			    }
		    }

		public:
		    template<typename T>
			SquaredWellMixed(T &j) {
			    name="SquaredWellMixed";
			    init();
			    threshold = j["SquaredWellMixed"]["threshold"];
			    maxrange = j["SquaredWellMixed"]["maxrange"];
			    maxrange2 = maxrange*maxrange;
			    if (j.count("ljcustom")>0) {
				cout << name+": custom LJ parameters found...\n";
				customParameters( j["ljcustom"] );
			    }
			}

		    /** @brief Energy in kT between two particles, r2 = squared distance */
		    template<class Tparticle>
			double operator()(const Tparticle &a, const Tparticle &b, double r2) const {
			    double t2=s2(a.id,b.id) + 2.0*threshold*sqrt(s2(a.id,b.id)) + threshold*threshold;
			    if(a.id != b.id) {
				if( r2 < t2 )
				    return -eps(a.id,b.id);
				if( r2 < maxrange2 ) {
				    double r1 = sqrt(r2);
				    double k = eps(a.id,b.id)/(maxrange - s(a.id,b.id) - threshold);
				    double m = -eps(a.id,b.id)/maxrange/(maxrange - s(a.id,b.id) - threshold);
				    return (k*r1 + m);
				}
			    }
			    return 0.0;
			}

		    template<typename Tparticle>
			Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			    return 0.0 * p;
			}

		    template<class Tparticle>
			double operator()(const Tparticle &a, const Tparticle &b, const Point &r) const {
			    return operator()(a,b,r.squaredNorm());
			}

		    /**
		     * @brief This will set a custom epsilon for a pair of particles
		     * @param i Particle id of first particle
		     * @param j Particle id of second particle
		     * @param eps_kT epsilon in units of kT
		     */
		    void customEpsilon(int i, int j, double eps_kT) {
			eps.set(i,j,4*eps_kT);
		    }

		    void customSigma(int i, int j, double sigma) {
			s2.set(i,j,sigma*sigma);
			s.set(i,j,sigma);
		    }

		    /**
		     * @brief Read custom parameters from json section
		     *
		     * This will read a json section with custom epsilon and sigma
		     * values for pairs of atoms, bypassing the mixing rule.
		     * The following format is expected, with units of angstrom and
		     * kJ/mol, respectively:
		     *
		     * ~~~~
		     * {
		     *    "Na F" : { "sigma":2.0, "eps":0.7 },
		     *    "Na Cl" : { "sigma":2.2, "eps":0.5 }
		     * }
		     * ~~~~
		     */
		    void customParameters( const Tmjson &j ) {
			if (j.is_object()) {
			    for (auto it=j.begin(); it!=j.end(); ++it) { 
				auto v = textio::words2vec<string>( it.key() );
				if (v.size()==2) {
				    auto id1 = atom[ v[0] ].id;
				    auto id2 = atom[ v[1] ].id;
				    customEpsilon( id1, id2, it.value().at("eps").get<double>() * 1.0_kJmol );
				    customSigma( id1, id2, it.value().at("sigma").get<double>() );
				} else
				    std::runtime_error( name+": Exactly two atom types must be given for custom LJ parameters");
			    }
			}
		    }

		    string info(char w=0) {
			using namespace Faunus::textio;
			std::ostringstream o;
			o << indent(SUB) << name+" pair parameters:\n\n";
			o.precision(4);
			int n=(int)atom.size();
			for (int i=0; i<n; i++)
			    for (int j=0; j<n; j++)
				if (i>=j)
				    if (i!=0 && j!=0) // ignure first "UNK" particle type
					o << indent(SUBSUB) << setw(12) << atom[i].name+"<->"+atom[j].name
					    << indent(SUB) << sigma+" = " << setw(7) << sqrt( s2(i,j) ) << _angstrom
					    << indent(SUB) << epsilon+" = " << setw(7) << eps(i,j)/4 << kT+" = "
					    << eps(i,j) / 4.0_kJmol << " kJ/mol"
					    << endl;
			o << indent(SUB) << "Threshold" << setw(7) << threshold << endl;
			return o.str();
		    }
	    };

























	/**
	 * @brief Lennard-Jones with arbitrary mixing rule
	 *
	 * @details This is a template for Lennard-Jones pair interactions where the
	 * template parameter must be a class for the epsilon and sigma mixing rules.
	 * The atomic values for sigma and epsilon are taken from `AtomMap` via the
	 * global instance `atom`.
	 * Note that sigma for each atom is set to two times the radius found in
	 * `AtomMap`. Epsilon is stored internally in units of `4kT`.
	 *
	 * For example:
	 * 
	 *     Tmjson j = openjson( "config.json" );
	 *     LennardJonesMixed<LorentzBerthelot> lj( j );
	 */
	template<class Tmixingrule = LorentzBerthelot>
	    class LennardJonesMixed : public PairPotentialBase {
		protected:
		    Tmixingrule mixer; // mixing rule class for sigma and epsilon
		    string _brief() { return name + " w. " + mixer.name; }
		    PairMatrix<double> s2,eps; // matrix of sigma_ij^2 and eps_ij

		    inline void init() {
			size_t n=atom.size(); // number of atom types
			s2.resize(n); // not required...
			eps.resize(n);// ...but possible reduced mem. fragmentation
			for (auto &i : atom)
			    for (auto &j : atom) {
				double sigma, epsilon; // mixed values
				std::tie( sigma, epsilon ) = mixer( {i.sigma, j.sigma}, {i.eps, j.eps} );
				s2.set(  i.id, j.id, sigma*sigma );
				eps.set( i.id, j.id, 4*epsilon );
			    }
		    }

		public:
		    template<typename T>
			LennardJonesMixed(T &j) {
			    name="Lennard-Jones";
			    init();
			    if (j.count("ljcustom")>0) {
				cout << name+": custom LJ parameters found...\n";
				customParameters( j["ljcustom"] );
			    }
			}

		    /** @brief Energy in kT between two particles, r2 = squared distance */
		    template<class Tparticle>
			double operator()(const Tparticle &a, const Tparticle &b, double r2) const {
			    double x=s2(a.id,b.id)/r2; //s2/r2
			    x=x*x*x; // s6/r6
			    return eps(a.id,b.id) * (x*x - x);
			}

		    template<typename Tparticle>
			Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			    double s6=_powi<3>( s2(a.id,b.id) );
			    double r6=r2*r2*r2;
			    double r14=r6*r6*r2;
			    return 6.*eps(a.id,b.id) * s6 * (2*s6-r6) / r14 * p;
			}

		    template<class Tparticle>
			double operator()(const Tparticle &a, const Tparticle &b, const Point &r) const {
			    return operator()(a,b,r.squaredNorm());
			}

		    /**
		     * @brief This will set a custom epsilon for a pair of particles
		     * @param i Particle id of first particle
		     * @param j Particle id of second particle
		     * @param eps_kT epsilon in units of kT
		     */
		    void customEpsilon(int i, int j, double eps_kT) {
			eps.set(i,j,4*eps_kT);
		    }

		    void customSigma(int i, int j, double sigma) {
			s2.set(i,j,sigma*sigma);
		    }

		    /**
		     * @brief Read custom parameters from json section
		     *
		     * This will read a json section with custom epsilon and sigma
		     * values for pairs of atoms, bypassing the mixing rule.
		     * The following format is expected, with units of angstrom and
		     * kJ/mol, respectively:
		     *
		     * ~~~~
		     * {
		     *    "Na F" : { "sigma":2.0, "eps":0.7 },
		     *    "Na Cl" : { "sigma":2.2, "eps":0.5 }
		     * }
		     * ~~~~
		     */
		    void customParameters( const Tmjson &j ) {
			if (j.is_object()) {
			    for (auto it=j.begin(); it!=j.end(); ++it) { 
				auto v = textio::words2vec<string>( it.key() );
				if (v.size()==2) {
				    auto id1 = atom[ v[0] ].id;
				    auto id2 = atom[ v[1] ].id;
				    customEpsilon( id1, id2, it.value().at("eps").get<double>() * 1.0_kJmol );
				    customSigma( id1, id2, it.value().at("sigma").get<double>() );
				} else
				    std::runtime_error( name+": Exactly two atom types must be given for custom LJ parameters");
			    }
			}
		    }

		    string info(char w=0) {
			using namespace Faunus::textio;
			std::ostringstream o;
			o << indent(SUB) << name+" pair parameters:\n\n";
			o.precision(4);
			int n=(int)atom.size();
			for (int i=0; i<n; i++)
			    for (int j=0; j<n; j++)
				if (i>=j)
				    if (i!=0 && j!=0) // ignure first "UNK" particle type
					o << indent(SUBSUB) << setw(12) << atom[i].name+"<->"+atom[j].name
					    << indent(SUB) << sigma+" = " << setw(7) << sqrt( s2(i,j) ) << _angstrom
					    << indent(SUB) << epsilon+" = " << setw(7) << eps(i,j)/4 << kT+" = "
					    << eps(i,j) / 4.0_kJmol << " kJ/mol"
					    << endl;
			return o.str();
		    }
	    };

	template<class Tmixingrule=LorentzBerthelot>
	    class CosAttractMixed : public LennardJonesMixed<Tmixingrule> {
		protected:
		    typedef LennardJonesMixed<Tmixingrule> base;
		    PairMatrix<double> rc2,rc,c; // matrix of sigma_ij^2 and eps_ij
		public:
		    template<class T>
			CosAttractMixed(const T &dummy ) : base( dummy ) {
			    base::name="Cos" + textio::squared + " attraction (mixed)";
			    size_t n=atom.size(); // number of atom types
			    c.resize(n);
			    rc.resize(n);
			    rc2.resize(n);
			    for (size_t i=0; i<n; i++)
				for (size_t j=i; j<n; j++) {
				    rc.set(i,j,base::mixer.mixSigma( atom[i].pdis, atom[j].pdis));
				    base::rcut2.set(i,j,base::mixer.mixSigma( atom[i].pswitch, atom[j].pswitch));
				    c.set(i,j, 0.5*pc::pi/base::rcut2(i,j) );
				    base::rcut2.set(i,j, base::rcut2(i,j) + rc(i,j) );
				    base::rcut2.set(i,j, base::rcut2(i,j) * base::rcut2(i,j) );
				    rc2.set(i,j, rc(i,j)*rc(i,j) );
				}
			}

		    /** @brief Energy in kT between two particles, r2 = squared distance */
		    template<class Tparticle>
			double operator()(const Tparticle &a, const Tparticle &b, double r2) const {
			    if (r2<rc2(a.id,b.id)) {
				//epsilon is from LJmixed stored as multiplied by 4
				return -0.25*base::eps(a.id,b.id);
			    }
			    if (r2>base::rcut2(a.id,b.id))
				return 0;
			    double x=cos( c(a.id,b.id)*( sqrt(r2)-rc(a.id,b.id) ) );
			    //epsilon is from LJmixed stored as multiplied by 4
			    return -0.25*base::eps(a.id,b.id)*x*x;
			}
		    template<class Tparticle>
			double operator() (const Tparticle &a, const Tparticle &b, const Point &r) const {
			    return operator()(a,b,r.squaredNorm());
			}
	    };

	/**
	 * @brief Weeks-Chandler-Andersen pair potential
	 * @details This is a Lennard-Jones type potential, cut and shifted to zero
	 * at @f$r_c=2^{1/6}\sigma@f$. More info can be found in at
	 * <http://doi.org/ct4kh9> and the functional form is:
	 * @f[
	 * \beta u = 4 \beta \epsilon \left ( (b/r)^{12} - (b/r)^6 + \frac{1}{4} \right )
	 * @f]
	 * where sigma, epsilon per default are set
	 * using Lorentz-Berthelot mixing rules.
	 */
	class WeeksChandlerAndersen : public LennardJonesMixed<LorentzBerthelot> {
	    protected:
		double onefourth, twototwosixth;
	    public:
		typedef LennardJonesMixed<LorentzBerthelot> Tbase;

		template<class T>
		    inline WeeksChandlerAndersen(const T &dummy)
		    : Tbase( dummy ), onefourth(1/4.), twototwosixth(std::pow(2,2/6.))  {
			name="WeeksChandlerAnderson";
		    }

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    inline double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
			double x=s2(a.id,b.id); // s^2
			if (r2>x*twototwosixth)
			    return 0;
			x=x/r2;  // (s/r)^2
			x=x*x*x;// (s/r)^6
			return eps(a.id,b.id)*(x*x - x + onefourth);
		    }

		/** @brief Energy in kT between two particles, r2 = distance vector  */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) const {
			return operator()(a,b,r.squaredNorm());
		    }

		template<class Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			double x=s2(a.id,b.id); // s^2
			if (r2>x*twototwosixth)
			    return Point(0,0,0);
			x=x/r2;  // (s/r)^2
			x=x*x*x;// (s/r)^6
			return eps(a.id,b.id)*6*(2*x*x - x)/r2*p;
		    }
	};
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	class OnesidedAtt : public LennardJonesMixed<LorentzBerthelot> {
	    protected:
		double onefourth, twototwosixth;
	    public:
		typedef LennardJonesMixed<LorentzBerthelot> Tbase;

		template<class T>
		    inline OnesidedAtt(const T &dummy)
		    : Tbase( dummy ), onefourth(1/4.), twototwosixth(std::pow(2,2/6.))  {
			name="OnesidedAtt";
		    }

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    inline double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
			return -eps(a.id,b.id)/(r2*r2 + 1.0);
		    }

		/** @brief Energy in kT between two particles, r2 = distance vector  */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) const {
			return operator()(a,b,r.squaredNorm());
		    }

		template<class Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			return -4.0*eps(a.id,b.id)*r2*sqrt(r2)/(r2*r2 + 1.0)/(r2*r2 + 1.0)*p;
		    }
	};
	
	
	class OnesidedRep : public LennardJonesMixed<LorentzBerthelot> {
	    protected:
		double onefourth, twototwosixth;
	    public:
		typedef LennardJonesMixed<LorentzBerthelot> Tbase;

		template<class T>
		    inline OnesidedRep(const T &dummy)
		    : Tbase( dummy ), onefourth(1/4.), twototwosixth(std::pow(2,2/6.))  {
			name="OnesidedRep";
		    }

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    inline double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
			return eps(a.id,b.id)/(r2*r2 + 1.0);
		    }

		/** @brief Energy in kT between two particles, r2 = distance vector  */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) const {
			return operator()(a,b,r.squaredNorm());
		    }

		template<class Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			return 4.0*eps(a.id,b.id)*r2*sqrt(r2)/(r2*r2 + 1.0)/(r2*r2 + 1.0)*p;
		    }
	};
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	/**
	 * @brief Square well pair potential
	 */
	class SquareWell : public PairPotentialBase {
	    private:
		string _brief();
	    public:
		double threshold;                           //!< Threshold between particle *surface* [A]
		double depth;                               //!< Energy depth [kT] (positive number)
		SquareWell(Tmjson&); //!< Constructor

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    inline double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			if(a.id == b.id)
			  return 0.0;
		      
			double d=a.radius+b.radius+threshold;
			if ( r2 < d*d )
			    return -depth;
			return 0;
		    }
		string info(char);
	};

	/*!
	 * \brief Square well pair potential shifted
	 * \author Anil Kurut
	 */
	class SquareWellShifted : public SquareWell {
	    private:
		string _brief();
	    public:
		double threshold_lower;
		SquareWellShifted( Tmjson& ); //!< Constructor

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
			double d=a.radius+b.radius+threshold_lower;
			if ( r2 > d*d )
			    return SquareWell::operator()(a,b,r2);
			return 0;
		    }
		string info(char);
	};

	/**
	 * @brief Hydrophobic pair potential based on SASA and surface tension
	 * @todo Documentation is incorrect.
	 * @details The potential is not zero if the distance between hydrophobic particles
	 * is smaller than size of solvent molecule (2*Rs)  
	 * Potential has the form:
	 *
	 * \f$ u = Surface tension * (\Delta SASA_i + \Delta SASA_j) \f$
	 *
	 * Surface area which is not accesible for solvent
	 * \f$ \Delta SASA_i = (SASA_i(r_{ij})-SASA_i(\inf))
	 * \f$ is calculated based on surface of a sphere cap
	 *
	 * \f$ SA_{cap,i}=2\pi(R_i+R_s)h_i \f$
	 * where h is dependent on distance between the particles as 
	 *
	 * \f$ h_i=(R_i+R_s)*(\frac{(R_i+R_s)^2-(R_j+R_s)^2)+r_{ij}^2}{2r_{ij}(R_i+R_s)}\f$
	 *
	 */
	class SquareWellHydrophobic : public SquareWell {
	    public:
		SquareWellHydrophobic(Tmjson &j);

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			if (a.hydrophobic)
			    if (b.hydrophobic)
				return SquareWell::operator()(a,b,r2);
			return 0;
		    }
	};

	/*!
	 * \brief Soft repulsion of the form \f$ \beta u = \sigma^6 / (r_{ij}-r_i-r_j)^6 \f$
	 * \todo This applies sqrt() and thus may be slow. Also remove floating point comparison.
	 */
	class SoftRepulsion : public PairPotentialBase {
	    private:
		string _brief();
		double sigma6;
	    public:
		SoftRepulsion(Tmjson &j);

		string info(char w);

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			double d=a.radius+b.radius;
			if (r2<=d*d) //fp comparison!
			    return pc::infty;
			d = sqrt(r2)-d;
			d=d*d; //d^2
			return sigma6 / (d*d*d); // s^6/d^6
		    }
	};

	/*!
	 * \brief r12-Repulsion of the form
	 * \details \f$ \beta u = 4\epsilon_{lj} \left (  (\sigma_{ij}/r_{ij})^{12}  \right ) \f$
	 * where
	 * \f$\sigma_{ij} = (\sigma_i+\sigma_j)/2\f$ and \f$\epsilon_{lj}\f$
	 * is fixed for this class.
	 * \todo Same as LennardJonesR12. Remove?
	 */
	class R12Repulsion : public PairPotentialBase {
	    private:
		string _brief();
	    protected:
		double eps;
	    public:
		R12Repulsion(Tmjson &j);

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			double x=(a.radius+b.radius);
			x=x*x/r2; // r2
			x=x*x*x; // r6
			return eps*x*x;
		    }

		string info(char);
	};

	/**
	 * @brief Repulsive part of LennardJones
	 */
	class LennardJonesR12 : public LennardJones {
	    public:
		LennardJonesR12( Tmjson &j ) {
		    name+="R12";
		}

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			double x=r6(a.radius+b.radius,r2);
			return eps*x*x;
		    }
	};

	/**
	 * @brief Lennard-Jones truncated and shifted to sigma.
	 */
	class LennardJonesTrunkShift : public LennardJones {
	    public:
		LennardJonesTrunkShift( Tmjson& );

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			double sigma = a.radius+b.radius;
			if (r2 > sigma*sigma)
			    return 0;
			double x=r6(sigma,r2)*0.5;
			return eps*(x*x - x + 0.25);
		    }

		template<class Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			double sigma = a.radius+b.radius;
			if (r2 > sigma*sigma)
			    return Point(0,0,0);
			double x=r6(sigma,r2)*0.5;
			return eps*6*(2*x*x - x) / r2 * p;
		    }
	};


	/**
	 * @brief Coulomb pair potential between charges in a dielectric medium.
	 * @details The Coulomb potential has the form
	 * @f[
	 * \beta u_{ij} = \frac{e^2}{4\pi\epsilon_0\epsilon_rk_BT} \frac{z_i z_j}{r_{ij}}
	 *              = \lambda_B \frac{z_i z_j}{r_{ij}}
	 * @f]
	 * where \f$\lambda_B\f$ is the Bjerrum length and \c z are the valencies.
	 *
	 * JSON keywords:
	 *
	 * Keyword        |  Description
	 * :------------- | :---------------------
	 *  `epsr`        | Relative dielectric constant [default: 80]
	 *  `depsdt`      | \f$ \partial\epsilon_r/\partial T\f$ [default: -0.368]
	 */
	class Coulomb : public PairPotentialBase {
	    friend class Potential::DebyeHuckel;
	    private:
	    string _brief();

	    double epsilon_r;
	    protected:
	    double depsdt;      //!< \f$ T\partial \epsilon_r / \epsilon_r \partial T = -1.37 \f$
	    double lB;          //!< Bjerrum length (angstrom)

	    public:

	    Coulomb(Tmjson&); //!< Construct from json entry

	    double bjerrumLength() const;  //!< Returns Bjerrum length [AA]

	    /** @brief Energy in kT between two particles separated by squared distance r2 */
	    template<class Tparticle>
		double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
#ifdef FAU_APPROXMATH
		    return lB*a.charge*b.charge * invsqrtQuake(r2);
#else
		    return lB*a.charge*b.charge / sqrt(r2);
#endif
		}

	    template<class Tparticle>
		double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
		    return operator()(a,b,r.squaredNorm());
		}

	    template<class Tparticle>
		Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
#ifdef FAU_APPROXMATH
		    return lB*a.charge*b.charge * invsqrtQuake(r2) / r2 * p;
#else
		    return lB*a.charge*b.charge * p / (sqrt(r2)*r2);
#endif
		}

	    /** @brief Electric field at `r` due to charge `p`
	     * Gets returned in [e/Å] (\f$\beta eE \f$)
	     */
	    template<class Tparticle>
		Point field (const Tparticle &p, const Point &r) const {
		    double r2i = 1.0/r.squaredNorm();
		    return p.charge*r2i*r*sqrt(r2i)*lB;
		}

	    string info(char);
	    void test(UnitTest&); //!< Perform unit test
	};

	/**
	 * @brief Coulomb pair potential shifted according to Wolf/Yonezawa
	 * @details The potential has the form:
	 * @f[
	 * \beta u_{ij} = \frac{e^2}{4\pi\epsilon_0\epsilon_rk_BT}
	 * z_i z_j \left (
	 * \frac{1}{r} - \frac{1}{R_c} + \frac{r-R_c}{R_c^2}
	 * \right )
	 * @f]
	 *
	 * and is hence a particular simple form of the original Wolf
	 * formulation. This potential is expected to work reasonably well
	 * for dense liquids, see [here](http://dx.doi.org/10/j97).
	 *
	 * Upon construction the keywords from `Potential::Coulomb`
	 * are used in addition to `cutoff` to specifify the cut-off.
	 */
	class CoulombWolf : public Coulomb {

	    private:

		double Rc2, Rcinv;
		PairMatrix<double> lBxQQ; // matrix of lB * Qa * Qb

	    public:

		CoulombWolf( Tmjson& );

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			if (r2>Rc2)
			    return 0;
#ifdef FAU_APPROXMATH
			r2=invsqrtQuake(r2);  // 1/r
			return lBxQQ(a.id, b.id) * (r2 - Rcinv + (Rcinv/r2-1)*Rcinv );
#else
			r2=sqrt(r2); // r
			//return lBxQQ(a.id, b.id) * (1/r2 - Rcinv + (r2*Rcinv-1)*Rcinv ); // Fennell
			return lBxQQ(a.id, b.id) * (1/r2 - Rcinv + r2*Rcinv*Rcinv ); // Yonezawa
#endif
		    }

		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			return operator()(a,b,r.squaredNorm());
		    }

		template<typename Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			if (r2>Rc2) return Point(0,0,0);
			return lB*a.charge*b.charge*(1-Rcinv*Rcinv*r2)/(r2*sqrt(r2))*p;
		    }

		string info(char);
	};
















	/**
	 * @brief Coulomb potential for capparticles. The charge is uniformly distributed on the surface of the particle.
	 */
	class CoulombUniformCap2 : public Coulomb {
	    private:
		double rc2, epsr, kappa, lB;
		PairMatrix<double> lBxQQe, lBxQeQe; // matrix of real and effective charges
		vector<double> effective_charges, charges, charge_densities, area;

	    public:
		CoulombUniformCap2( Tmjson& );

		/**
		 * @param a Capparticle
		 * @param b Sphere
		 * @param r Direction \f$ r_a - r_b \f$
		 * @param r1 Length of 'r'
		 */
		template<class Tparticle>
		    double cap2sphere(const Tparticle &a, const Tparticle &b, const Point &r, double r1) {
			// Lock and key energy approximation in Faunus. Do not modify!
			// 'a' is cap, 'b' is sphere, 'r' = 'ra' - 'rb'

			int region = 0;
			double angleKLC = 0.0;
			double angleLCK = 0.0;
			Point CKv(0,0,0);
			double CK = 0.0;
			double d = 0.0;
			int I = 0;

			bool foundRegion = false;
			for(int k = 0; k < a.nbrOfCav(); k++) {
			  getRegion(a,b, region, angleKLC, angleLCK, CKv, CK, d, r,r1,k);
			  if(region != 4) {
			    I = k;
			    foundRegion = true;
			    break;
			  }
			}

			if(!foundRegion)
			  region = 4;

			if(region == 4) // Back segment
			    return getLineSegment(a,b,d,false,I);

			double E1 = getLineSegment(a,b,d,true,I);          

			if(region == 1) { // Inside inner cavity
			    //cout << "Region I" << endl;
			    double E2 = getLineSegment(a,b,d,false,I);
			    double k = (E2-E1)/(cos(a.angle_c(I))-1.0);
			    double m = E2 - k*cos(a.angle_c(I));
			    return ( k*cos(angleLCK) + m );
			}

			if(region == 2) { // Inside outer cavity
			    //cout << "Region II" << endl;
			    double E2 = getLineSegment(a,b,d,false,I);
			    double k = (E2-E1)/(-cos(a.angle_p(I))-1.0); // used that cos(pi-a) = -cos(a)
			    double m = E2 + k*cos(a.angle_p(I));            // used that cos(pi-a) = -cos(a)
			    return ( k*cos(angleKLC) + m );
			}

			if(region == 3) { // Inside big cone but not cavity
			    //cout << "Region III" << endl;
			    double rz = sqrt(a.radius*a.radius + r1*r1 + 2.0*a.radius*r1*cos(a.angle_p(I) + angleKLC)); // Closesed distance to the rim of the cap and sphere ( Used that cos(-a) = cos(a) )
			    double E2 = getLineSegment(a,b,rz-b.radius,false,I);
			    double k = (E2-E1)/(cos(a.angle_c(I))+1.0);
			    double m = E2 - k*cos(a.angle_c(I));
			    //cout << "E1: " << E1/lB << ", E2: " << E2/lB << ", angleLCK: " << angleLCK*180.0/pc::pi << ", a.angle_c(): " << a.angle_c()*180.0/pc::pi << endl;
			    return ( k*cos(angleLCK) + m );
			}
			return 0.0;
		    }

		template<class Tparticle>
		    double getLineSegment(const Tparticle &a, const Tparticle &b, double d, bool front, int &I) {
			// Get analytical interaction-energy between a lock and a key at a line
			double Rc = a.cap_radius(I);
			double Rl = a.radius;
			double Rk = b.radius;
			double c = a.cap_center(I);

			double p = -lBxQQe(a.id, b.id)*2*pc::pi/a.area();
			double z = 0;
			double cosP = (c*c+Rl*Rl-Rc*Rc)/(2*Rl*c);
			double cosC = (Rc*Rc+c*c-Rl*Rl)/(2*Rc*c);
			if(front) {
			    if(Rk < Rc) {
				z = d + c - Rc + Rk;
			    } else {
				z = d + Rl*cosP + sqrt(Rl*cosP*Rl*cosP + Rk*Rk - Rl*Rl);
			    }
			} else {
			    z = -(Rl+Rk+d);
			}

			// Get energy from back-segment
			double Ub = 0.0;
			if(abs(z/Rl) < 1e-6) {
			    Ub = Rl*(cosP+1)*exp(-Rl*kappa);
			} else {
			    if(abs(kappa) < 1e-9) {
				Ub = ( (-abs(Rl+z)) - (-sqrt(Rl*Rl-2*Rl*z*cosP+z*z)) )*Rl/z;
			    } else {
				Ub = ( exp(-abs(Rl+z)*kappa) - exp(-sqrt(Rl*Rl-2*Rl*z*cosP+z*z)*kappa) )*Rl/z/kappa;
			    }
			}

			// Get energy from front-segment
			double Uf = 0.0;
			if(abs((z -c)/Rl) < 1e-6) {
			    Uf = Rc*(cosC-1)*exp(-Rc*kappa);
			} else {
			    if(abs(kappa) < 1e-9) {
				Uf = -( (-abs(z-c+Rc)) - (-sqrt(Rc*Rc-2*Rc*(c-z)*cosC+(c-z)*(c-z))) )*Rc/(c-z);
			    } else {
				Uf = -( exp(-abs(z-c+Rc)*kappa) - exp(-sqrt(Rc*Rc-2*Rc*(c-z)*cosC+(c-z)*(c-z))*kappa) )*Rc/(c-z)/kappa;
			    }
			}
			double U = Uf + Ub;
			return U*p;
		    }

		template<class Tparticle>
		    void getRegion(const Tparticle &a, const Tparticle &b, int &region, double &angleKLC, double &angleLCK, Point &CKv, double &CK, double &d, const Point &r,double r1, int &I) {
			CKv = r + a.cap_center_point(I)*a.cap_center(I);
			CK = CKv.norm(); // 'a'-cap to 'b'
			bool Lb = r1 < a.radius; // Within sphere?

			if( ( CK > a.cap_radius(I)) && Lb) {
			  region = 4;
			  return;
			}

			angleKLC = acos(r.dot(-a.cap_center_point(I))/r1); // Angle between capsphere-center to sphere-center and capsphere-center to cap-center
			angleLCK = acos(CKv.dot(a.cap_center_point(I))/CK);
			if(( r1*cos(angleKLC) < -a.radius*cos(a.angle_p(I))) && Lb) { // Used that cos(pi - a) = -cos(a)
			    region = 1;
			    d = a.cap_radius(I) - CK - b.radius;
			    return;
			}

			if(angleKLC < pc::pi - a.angle_p(I)) { // Inside big cone
			    if(angleLCK < a.angle_c(I)) { // Inside cavity cone
				if(r1*cos(angleKLC) < -a.radius*cos(a.angle_p(I))) { // Inside inner cavity ( Used that cos(pi - a) = -cos(a) )
				    region = 1; // Inside inner cavity
				    d = a.cap_radius(I) - CK - b.radius;
				    return;
				} else {
				    region = 2; // Inside outer cavity
				    d = a.cap_radius(I) - CK - b.radius;
				    return;
				}
			    } else {
				region = 3; // Inside big cone but not cavity
				d = r1 -( a.cap_center(I) - a.cap_radius(I)) - b.radius;
				return;
			    }
			} else {
			    bool Cb = CK < a.cap_radius(I); // Within cavity?
			    if(Cb && Lb) {
				region = 1; // Inside inner cavity
				d = a.cap_radius(I) - CK - b.radius;
				return;
			    } else {
				region = 4; // Back segment
				d = r1 - a.radius - b.radius;
				return;
			    }
			}		
		    }

		/**
		 * @param r Direction \f$ r_a - r_b \f$
		 */
		template<class Tparticle>
		    double operator()(const Tparticle &a, const Tparticle &b, const Point &r) {
			//cout << "Uniform! " << a.cap_center_point().transpose() << ", " << a.cap_center_point().norm() << ", " << b.cap_center_point().transpose() << ", " << b.cap_center_point().norm() << endl;

		//cout << "Hej" << endl;

		double r2 = r.squaredNorm();
			if(r2 >= rc2)
			    return 0.0;
			double r1 = sqrt(r2);

			if(!a.is_sphere() && !b.is_sphere()) {
			  return ( 0.5*cap2sphere(a,b,r,r1) + 0.5*cap2sphere(b,a,-r,r1) );
			}

			if(!a.is_sphere() && b.is_sphere())
			    return cap2sphere(a,b,r,r1);

			if(a.is_sphere() && !b.is_sphere())
			    return cap2sphere(b,a,-r,r1);

			// Both particles are spheres
			return lBxQeQe(a.id, b.id)*exp(-kappa*r1)/r1;
		    }

		string info(char w) {
		    using namespace textio;
		    std::ostringstream o;
		    o << pad(SUB, w, "Temperature") << pc::T() << " K" << endl
			<< pad(SUB, w, "Dielectric constant") << epsr << endl
			<< pad(SUB, w + 6, "T" + partial + epsilon + "/" + epsilon + partial + "T") << depsdt << endl
			<< pad(SUB, w, "Bjerrum length") << lB << _angstrom << endl
			<< pad(SUB,w,"Cutoff") << sqrt(rc2) << _angstrom << endl
			<< pad(SUB,w, "Debye length") << 1.0/kappa << endl
			<< pad(SUB,w, "kappa") << kappa << endl;

		    o << pad(SUB, w, "Effective charges (charges)") << endl;
		    int cnt = 0;
		    for (auto &i : atom) {
			if(cnt == 0) {
			    cnt++;
			    continue;
			}
			o << pad(SUB, w, "  "+i.name) << effective_charges.at(cnt) << " ("  << charges.at(cnt) << ")" << endl;
			cnt++;
		    }

		    o << pad(SUB, w, "Charge densities (area)") << endl;
		    cnt = 0;
		    for (auto &i : atom) {
			if(cnt == 0) {
			    cnt++;
			    continue;
			}
			o << pad(SUB, w, "  "+i.name) << charge_densities.at(cnt) << " ("  << area.at(cnt) << ")" << endl;
			cnt++;
		    }

		    return o.str();
		}
	};

















	/**
	 * @brief Coulomb potential for capparticles. The charge is uniformly distributed on the surface of the particle.
	 */
	class CoulombUniformCap : public Coulomb {
	    private:
		double rc2, epsr, kappa, lB;
		PairMatrix<double> lBxQQe, lBxQeQe; // matrix of real and effective charges
		vector<double> effective_charges;

	    public:
		CoulombUniformCap( Tmjson& );

		/**
		 * @param a Capparticle
		 * @param b Sphere
		 * @param r Direction \f$ r_a - r_b \f$
		 * @param r1 Length of 'r'
		 */
		template<class Tparticle>
		    double cap2sphere(const Tparticle &a, const Tparticle &b, const Point &r, double r1) {
			// Lock and key energy approximation in Faunus. Do not modify!
			// 'a' is cap, 'b' is sphere, 'r' = 'ra' - 'rb'

			int region = 0;
			double angleKLC = 0.0;
			double angleLCK = 0.0;
			Point CKv(0,0,0);
			double CK = 0.0;
			double d = 0.0;
			getRegion(a,b, region, angleKLC, angleLCK, CKv, CK, d, r,r1);

			//return double(region);

			if(region == 4) // Back segment
			    return getLineSegment(a,b,d,false);

			double E1 = getLineSegment(a,b,d,true);          

			if(region == 1) { // Inside inner cavity
			    //cout << "Region I" << endl;
			    double E2 = getLineSegment(a,b,d,false);
			    double k = (E2-E1)/(cos(a.angle_c())-1.0);
			    double m = E2 - k*cos(a.angle_c());
			    return ( k*cos(angleLCK) + m );
			}

			if(region == 2) { // Inside outer cavity
			    //cout << "Region II" << endl;
			    double E2 = getLineSegment(a,b,d,false);
			    double k = (E2-E1)/(-cos(a.angle_p())-cos(0)); // used that cos(pi-a) = -cos(a)
			    double m = E2 + k*cos(a.angle_p());            // used that cos(pi-a) = -cos(a)
			    return ( k*cos(angleKLC) + m );
			}

			if(region == 3) { // Inside big cone but not cavity
			    //cout << "Region III" << endl;
			    double rz = sqrt(a.radius*a.radius + r1*r1 + 2.0*a.radius*r1*cos(a.angle_p() + angleKLC)); // Closesed distance to the rim of the cap and sphere ( Used that cos(-a) = cos(a) )
			    double E2 = getLineSegment(a,b,rz-b.radius,false);
			    double k = (E2-E1)/(cos(a.angle_c())+1.0);
			    double m = E2 - k*cos(a.angle_c());
			    //cout << "E1: " << E1/lB << ", E2: " << E2/lB << ", angleLCK: " << angleLCK*180.0/pc::pi << ", a.angle_c(): " << a.angle_c()*180.0/pc::pi << endl;
			    return ( k*cos(angleLCK) + m );
			}
			return 0.0;
		    }

		template<class Tparticle>
		    double getLineSegment(const Tparticle &a, const Tparticle &b, double d, bool front) {
			// Get analytical interaction-energy between a lock and a key at a line
			double Rc = a.cap_radius();
			double Rl = a.radius;
			double Rk = b.radius;
			double c = a.cap_center();

			double p = -lBxQQe(a.id, b.id)*2*pc::pi/a.area();
			double z = 0;
			double cosP = (c*c+Rl*Rl-Rc*Rc)/(2*Rl*c);
			double cosC = (Rc*Rc+c*c-Rl*Rl)/(2*Rc*c);
			if(front) {
			    if(Rk < Rc) {
				z = d + c - Rc + Rk;
			    } else {
				z = d + Rl*cosP + sqrt(Rl*cosP*Rl*cosP + Rk*Rk - Rl*Rl);
			    }
			} else {
			    z = -(Rl+Rk+d);
			}

			// Get energy from back-segment
			double Ub = 0.0;
			if(abs(z/Rl) < 1e-6) {
			    Ub = Rl*(cosP+1)*exp(-Rl*kappa);
			} else {
			    if(abs(kappa) < 1e-9) {
				Ub = ( (-abs(Rl+z)) - (-sqrt(Rl*Rl-2*Rl*z*cosP+z*z)) )*Rl/z;
			    } else {
				Ub = ( exp(-abs(Rl+z)*kappa) - exp(-sqrt(Rl*Rl-2*Rl*z*cosP+z*z)*kappa) )*Rl/z/kappa;
			    }
			}

			// Get energy from front-segment
			double Uf = 0.0;
			if(abs((z -c)/Rl) < 1e-6) {
			    Uf = Rc*(cosC-1)*exp(-Rc*kappa);
			} else {
			    if(abs(kappa) < 1e-9) {
				Uf = -( (-abs(z-c+Rc)) - (-sqrt(Rc*Rc-2*Rc*(c-z)*cosC+(c-z)*(c-z))) )*Rc/(c-z);
			    } else {
				Uf = -( exp(-abs(z-c+Rc)*kappa) - exp(-sqrt(Rc*Rc-2*Rc*(c-z)*cosC+(c-z)*(c-z))*kappa) )*Rc/(c-z)/kappa;
			    }
			}
			double U = Uf + Ub;
			return U*p;
		    }

		template<class Tparticle>
		    void getRegion(const Tparticle &a, const Tparticle &b, int &region, double &angleKLC, double &angleLCK, Point &CKv, double &CK, double &d, const Point &r,double r1) {
			CKv = r + a.cap_center_point()*a.cap_center();
			CK = CKv.norm(); // 'a'-cap to 'b'
			bool Lb = r1 < a.radius; // Within sphere?

			angleKLC = acos(r.dot(-a.cap_center_point())/r1); // Angle between capsphere-center to sphere-center and capsphere-center to cap-center
			angleLCK = acos(CKv.dot(a.cap_center_point())/CK);
			if(( r1*cos(angleKLC) < -a.radius*cos(a.angle_p())) && Lb) { // Used that cos(pi - a) = -cos(a)
			    region = 1;
			    d = a.cap_radius() - CK - b.radius;
			    return;
			}

			if(angleKLC < pc::pi - a.angle_p()) { // Inside big cone
			    if(angleLCK < a.angle_c()) { // Inside cavity cone
				if(r1*cos(angleKLC) < -a.radius*cos(a.angle_p())) { // Inside inner cavity ( Used that cos(pi - a) = -cos(a) )
				    region = 1; // Inside inner cavity
				    d = a.cap_radius() - CK - b.radius;
				    return;
				} else {
				    region = 2; // Inside outer cavity
				    d = a.cap_radius() - CK - b.radius;
				    return;
				}
			    } else {
				region = 3; // Inside big cone but not cavity
				d = r1 -( a.cap_center() - a.cap_radius()) - b.radius;
				return;
			    }
			} else {
			    bool Cb = CK < a.cap_radius(); // Within cavity?
			    if(Cb && Lb) {
				region = 1; // Inside inner cavity
				d = a.cap_radius() - CK - b.radius;
				return;
			    } else {
				region = 4; // Back segment
				d = r1 - a.radius - b.radius;
				return;
			    }
			}		
		    }

		/**
		 * @param r Direction \f$ r_a - r_b \f$
		 */
		template<class Tparticle>
		    double operator()(const Tparticle &a, const Tparticle &b, const Point &r) {
			//cout << "Uniform! " << a.cap_center_point().transpose() << ", " << a.cap_center_point().norm() << ", " << b.cap_center_point().transpose() << ", " << b.cap_center_point().norm() << endl;
			double r2 = r.squaredNorm();
			if(r2 >= rc2)
			    return 0.0;
			double r1 = sqrt(r2);

			if(!a.is_sphere() && !b.is_sphere()) {
			    return ( 0.5*cap2sphere(a,b,r,r1) + 0.5*cap2sphere(b,a,-r,r1) );
			}

			if(!a.is_sphere() && b.is_sphere())
			    return cap2sphere(a,b,r,r1);

			if(a.is_sphere() && !b.is_sphere())
			    return cap2sphere(b,a,-r,r1);

			// Both particles are spheres
			return lBxQeQe(a.id, b.id)*exp(-kappa*r1)/r1;
		    }

		string info(char w) {
		    using namespace textio;
		    std::ostringstream o;
		    o << pad(SUB, w, "Temperature") << pc::T() << " K" << endl
			<< pad(SUB, w, "Dielectric constant") << epsr << endl
			<< pad(SUB, w + 6, "T" + partial + epsilon + "/" + epsilon + partial + "T") << depsdt << endl
			<< pad(SUB, w, "Bjerrum length") << lB << _angstrom << endl
			<< pad(SUB,w,"Cutoff") << sqrt(rc2) << _angstrom << endl
			<< pad(SUB,w, "Debye length") << 1.0/kappa << endl
			<< pad(SUB,w, "kappa") << kappa << endl;

		    o << pad(SUB, w, "Effective charges:") << endl;
		    int cnt = 0;
		    for (auto &i : atom) {
			if(cnt == 0) {
			    cnt++;
			    continue;
			}
			o << pad(SUB, w, "  "+i.name) << effective_charges.at(cnt) << endl;
			cnt++;
		    }

		    return o.str();
		}
	};




























































	/**
	 * @brief Charge-nonpolar pair interaction
	 * @details This accounts for polarization of
	 * \f[
	 * \beta u_{ij} = -\frac{\lambda_B z_i^2 \delta a_j^3}{2r_{ij}^4}
	 * \f]
	 * where a is the radius of the nonpolar particle.
	 * Note that this version requires that one of the particles
	 * is charged, while the other is neutral.
	 * Delta is a unitless scaling parameter of the excess
	 * polarizability.
	 * For non-polar particles in a polar medium, this is a negative number.
	 * For more information, see Israelachvili, Chapter 5.
	 *
	 * The json object is scanned for
	 *
	 * - The parameters from `Potential::Coulomb`
	 * - `excess_polarization` for the delta value
	 *
	 */
	class ChargeNonpolar : public Coulomb {
	    private:
		double c;
	    public:
		ChargeNonpolar(Tmjson&);

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			double qq=a.charge * a.charge;
			if (qq>1e-6)
			    return -c*qq/(r2*r2)*b.alphax;
			qq=b.charge * b.charge;
			if (qq>1e-6)
			    return -c*qq/(r2*r2)*a.alphax;
			return 0;
		    }

		string info(char);

		template<class Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			double qq=a.charge * a.charge;
			if (qq>1e-6)
			    return -4*c*qq/(r2*r2)*b.alphax/r2*p;
			qq=b.charge * b.charge;
			if (qq>1e-6)
			    return -4*c*qq/(r2*r2)*a.alphax/r2*p;
			return Point(0,0,0);
		    }
	};

	class PolarPolar : public Coulomb {
	    public:
		PolarPolar(Tmjson&); //!< Construction from InputMap

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) {
			return -3*a.alphax*b.alphax/(r2*r2*r2);
		    }

		string info(char);

		template<class Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			return -18*a.alphax*b.alphax/(r2*r2*r2*r2)*p;
		    }
	};

	class YukawaGel : public Coulomb {
	    private:
		string _brief();
		double Z, nc,ns, v, k, Z2e2overER,d,kd,k2d2,ekd, braket7;

	    public:

		YukawaGel(Tmjson&);

		/** @brief Energy in kT between two particles, r2 = squared distance */
		template<class Tparticle>
		    double operator()(const Tparticle &a, const Tparticle &b, double r2) {
			double m = a.radius+b.radius;
			double r = sqrt(r2);
			double kr = k*r;


			double ekr=exp(-kr);

			if(r2 <= m*m) {
			    double roverd = r/d;

			    double ekdsinhkr = ekd*sinh(kr);

			    double A = (2./d)*Z2e2overER;

			    double braket = (6./5.)-(2.*pow(roverd,2))+((3./2.)*pow(roverd,3))-((1./5.)*pow(roverd,5));

			    double B = (72./((k2d2*k2d2)*r))*Z2e2overER;

			    double braket2 = (((1.-ekr+(0.5*kr*kr)+((1./24.)*pow(kr,4)))*(1.-(4./k2d2)))+((4.*ekdsinhkr)/kd));

			    double braket3 = ( ekdsinhkr + (k*k*d*r) + ( ((k*k*k*k)/6.) * ((d*d*d*r)+(r*r*r*d)) ) )* (1.+(4./k2d2));

			    double braket4 = ((4.*r)/d)*(1.+(0.5*k2d2)+((1./30.)*(k2d2*k2d2)));
			    double braket5 = ((8.*r*r*r)/(3.*d*d*d))*((k2d2/4.)+((k2d2*k2d2)/12.));
			    double braket6 = (((1./180.)*((k*k*k*k)/(d*d)))*(r*r*r*r*r*r));

			    double pot = (A*braket)-(B*(braket2+braket3-braket4-braket5-braket6));
			    return pot;
			}
			else{

			    double pot2 = ((144./(k2d2*k2d2))*(Z2e2overER)*(braket7*braket7)*(ekr/r));

			    return pot2;
			}
		    }
		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			return operator()(a,b,r.squaredNorm());
		    }
		string info(char);
	};

	/**
	 * @brief Debye-Huckel/Yukawa potential
	 *
	 * Coulomb with an exponential term to describe salt screening:
	 * \f[ \beta w_{ij} = \frac{e^2}{4\pi\epsilon_0\epsilon_rk_BT}
	 * \frac{z_i z_j}{r_{ij}} \exp(-\kappa r_{ij}) \f]
	 * where \f$\kappa=1/D\f$ is the inverse Debye screening length.
	 *
	 * JSON keywords:
	 *
	 * - `ionicstrength` [mol/l] 
	 * - `debyelength` [angstrom] (only if I=0, default)
	 */
	class DebyeHuckel : public Coulomb {
	    private:
		string _brief();
	    protected:
		double c,k,k2_count, z_count;
		Average<double> k2_count_avg;

	    public:

		DebyeHuckel( Tmjson &j );

		template<class Tparticle>
		    double operator()(const Tparticle &a, const Tparticle &b, double r2) const {
#ifdef FAU_APPROXMATH
			double rinv = invsqrtQuake(r2);
			return lB * a.charge * b.charge * rinv * exp_cawley(-k/rinv);
#else
			double r=sqrt(r2);
			return lB * a.charge * b.charge / r * exp(-k*r);
#endif
		    }

		double entropy(double, double) const;         //!< Returns the interaction entropy
		double ionicStrength() const;                 //!< Returns the ionic strength (mol/l)
		double debyeLength() const;                   //!< Returns the Debye screening length (angstrom)
		double excessChemPot(double, double=0) const; //!< Single ion excess chemical potential (kT)
		double activityCoeff(double, double=0) const; //!< Single ion activity coefficient (molar scale)
		string info(char);

		template<class Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
#ifdef FAU_APPROXMATH
			double rinv = invsqrtQuake(r2);
			return lB * a.charge * b.charge * rinv * exp_cawley(-k/rinv) * ( 1/r2 + k*rinv ) * p;
#else
			double r=sqrt(r2);
			return lB * a.charge * b.charge / (r*r2) * exp(-k*r) * ( 1 + k*r ) * p;
#endif
		    }

		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, const Point &r) {
			return operator()(a,b,r.squaredNorm());
		    }

		/**
		 * @brief Scaled charge according to
		 * @f$ z^{\prime} = z \sinh(\kappa a) / \kappa a @f$
		 */
		template<class Tparticle>
		    double scaledCharge(const Tparticle &p) const {
			double ka=p.radius/debyeLength();
			return std::sinh(ka)/ka*p.charge;
		    }

		/**
		 * @brief Adds counter ions to kappa
		 */
		template<class Tspace>
		    void setSpace(Tspace &s) {
			if (std::fabs(z_count)>1e-6) {
			    double N=netCharge(s.p.begin(), s.p.end()) / std::fabs(z_count);
			    double V=s.geo.getVolume();
			    double k2=k*k - k2_count; // salt contribution
			    k2_count = 4*pc::pi*lB*N/V*std::pow(z_count,2); // counter ion contrib
			    k=sqrt( k2+k2_count );    // total
			    k2_count_avg+=k2_count;   // sample average
			}
		    }
	};
	/**
	 * @brief Debye-Huckel potential
	 * @details Unlike in the Debye-Huckel/Yukawa potential,
	 * particle size is taken into account:
	 * \f[ \beta w_{ij} = \frac{e^2}{4\pi\epsilon_0\epsilon_rk_BT}
	 * \frac{z_i z_j}{r_{ij}(1+\kappa a)} \exp(-\kappa (r_{ij} - a)) \f]
	 * where \f$\kappa=1/D\f$ is the inverse Debye screening length
	 * and \f$a\f$ is the contact distance between two particles.
	 */
	class DebyeHuckelSD : public DebyeHuckel {
	    public:
		DebyeHuckelSD(Tmjson &j ) : DebyeHuckel(j) {}

		template<class Tparticle>
		    double operator()(const Tparticle &a, const Tparticle &b, double r2) {
			double contact=a.radius+b.radius;
#ifdef FAU_APPROXMATH
			double rinv = invsqrtQuake(r2);
			return (rinv>1/contact) ? pc::infty :
			    lB * a.charge * b.charge * rinv / (1 + k*contact) * exp_cawley(-k/rinv+k*contact);
#else
			double r=sqrt(r2);
			return (r<contact) ? pc::infty :
			    lB * a.charge * b.charge / r / (1 + k*contact) * exp(-k*(r-contact));
#endif
		    }

		template<class Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			double contact=a.radius+b.radius;
#ifdef FAU_APPROXMATH
			double rinv = invsqrtQuake(r2);
			return (rinv>1/contact) ? -pc::infty*p :
			    lB * a.charge * b.charge * rinv / (1 + k*contact) * exp_cawley(-k/rinv+k*contact) * ( 1/r2 + k*rinv ) * p;
#else
			double r=sqrt(r2);
			return (r<contact) ? -pc::infty*p :
			    lB * a.charge * b.charge / (r*r2) / (1 + k*contact) * exp(-k*(r-contact)) * ( 1 + k*r ) * p;
#endif
		    }
	};

	/**
	 * DebyeHuckel ala Chung and Denton, <http://dx.doi.org/10/nkc>
	 */
	class DebyeHuckelDenton : public DebyeHuckel {
	    private:
		double lB_org; // original Bjerrum length without prefactor

		void setBjerrum(double a_m, double a_n);

		double fmn(double m, double n);

		// Eq. 11
		template<class Tpvec>
		    double eta(Tpvec &p, double V) {
			double v=0;
			for (auto &i : p)
			    v += pow(i.radius,3);
			return 4*pc::pi/(3*V) * v;
		    }

		// Eq. 41 - volume energy contrib. to pressure (microions)
		// (currently salt free case, only!)
		template<class Tpvec>
		    double p0(Tpvec &p, double V) {
			double Z=0,sum=0;
			for (auto &i : p) {
			    Z+=i.charge; // total charge
			    sum += pow(i.charge / (1+k*i.radius),2) / V; // Eq.41 sum
			}
			double p_id = std::fabs(Z)/V; // assume microion valency is unity
			double p_ex = -k*lB_org/4/(1-eta(p,V))*sum; // Eq.41, complete
			cout << "eta = " << eta(p,V) << endl;
			cout << "id = " << p_id*1660*1e3 << " ex = " << p_ex*1660*1e3 << "\n";
			return p_id + p_ex;
		    }

	    public:
		DebyeHuckelDenton(Tmjson &in);

		// Effective macroion-macroion interaction (Eq. 37)
		template<class Tparticle>
		    double operator()(const Tparticle &m, const Tparticle &n, double r2) {
			setBjerrum(m.radius, n.radius);
			return DebyeHuckel::operator()(m,n,r2);
		    }

		template<class Tparticle>
		    Point force(const Tparticle &m, const Tparticle &n, double r2, const Point &p) {
			setBjerrum(m.radius, n.radius);
			return DebyeHuckel::force(m,n,r2,p);
		    }

		string info(char w);

		template<class Tpvec>
		    string info(Tpvec &p, double V) {
			cout << "p0 = " << p0(p,V)*1660*1e3 << " mM\n";
			return info(20);
		    }

		/**
		 * @brief Contribution to pressure due to (dU/dk)(dk/dV), Eq.A3 / Eq.42
		 */
		template<class Tpvec, class Tgeo>
		    double virial(Tpvec &p, Tgeo &geo) {
			int n=int(p.size());
			double P=0, V=geo.getVolume();
			for (int i=0; i<n-1; i++)
			    for (int j=i+1; j<n; j++) {
				double r2=geo.sqdist(p[i],p[j]);
				double vmn=operator()(p[i],p[j],r2);
				P += vmn * (fmn(p[i].radius, p[j].radius) - sqrt(r2)); // Eq. A5
			    }
			return P * -k/(2*V*( 1-eta(p,V) )); // Eq. A3 = A5*A4
		    }

		template<class Tspace>
		    void setSpace(Tspace &s) {}
	};

	/**
	 * @brief DebyeHuckel shifted to reach zero at given cut-off
	 *
	 * Shifted and truncated Yukawa potential of the form,
	 * @f[
	 * u(r) = u^{dh}(r) - u^{dh}(r_c)
	 *         - \frac{\partial u^{dh}(r_c)}{\partial r_c}
	 *         \left ( r-r_c \right )
	 * @f]
	 * where @f$r_c@f$ is a spherical cut-off beyond which the
	 * energy is zero.
	 * See more in for example <http://dx.doi.org/10/fm7qm5>.
	 *
	 * Additional JSON keyword: `cutoff`
	 */
	class DebyeHuckelShift : public DebyeHuckel {
	    private:
		double rc2,rc; // (squared) cutoff distance
		double u_rc,dudrc;
	    public:
		DebyeHuckelShift(Tmjson &j);

		template<class Tparticle>
		    inline double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
			if (r2>rc2)
			    return 0;
#ifdef FAU_APPROXMATH
			double r = 1./invsqrtQuake(r2);
			return lB * a.charge * b.charge
			    * ( exp_cawley(-k*r)/r - u_rc - (dudrc*(r-rc)) );
#else
			double r=sqrt(r2);
			return lB * a.charge * b.charge
			    * ( exp(-k*r)/r - u_rc - (dudrc*(r-rc))  );
#endif
		    }
		template<class Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			if (r2>rc2)
			    return Point(0,0,0);
#ifdef FAU_APPROXMATH
			double rinv = invsqrtQuake(r2);
			return lB * a.charge * b.charge * ( exp_cawley(-k/rinv) / r2 * (k+rinv) + dudrc*rinv) * p;
#else
			double r=sqrt(r2);
			return lB * a.charge * b.charge * ( exp(-k*r) / r2 * (k + 1/r) + dudrc / r) * p;
#endif
		    }
	};

	/**
	 * @brief Cardinaux pair potential:
	 *        @f$ \beta u_{ij}=4\beta\epsilon_{ij}
	 *        ( (\frac{\sigma_{ij}}{r_{ij}})^{2\alpha}
	 *        - (\frac{\sigma_{ij}}{r_{ij}})^{\alpha}  )@f$
	 *
	 * The interaction strength, @f$\epsilon@f$ is set by the
	 * quadratic mean of individual values from `AtomData`.
	 * By default @f$\alpha=90@f$ and may be changed via
	 * the `cardinaux/alpha` in the json object.
	 * More info at
	 * [http://dx.doi.org/doi:10.1209/0295-5075/77/48004]
	 *
	 * @todo Force calculation can be slightly optimized
	 */
	class Cardinaux : public Potential::PairPotentialBase {
	    private:
		string _brief();
		int alpha,alphahalf;
		PairMatrix<double> eps; // 4*beta*epsilon for all pairs

	    public:

		Cardinaux( Tmjson &j );

		template<class Tparticle>
		    double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
			double s=a.radius+b.radius;
			s=s*s/r2; // ^2
#if defined(__GNUG__)
			s=__builtin_powi(s,alphahalf); // = (s/r)^a
#else
			s=pow(s,alphahalf); // (s/r)^2
#endif
			return eps(a.id,b.id)*(s*s - s);
		    }

		template<typename Tparticle>
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			double s=a.radius+b.radius;
			s=s*s/r2; //^2
#if defined(__GNUG__)
			s=__builtin_powi(s,alphahalf); // = (s/r)^a
#else
			s=pow(s,alphahalf);
#endif
			return alpha*eps(a.id,b.id)*s*(2*s-1)/r2 * p; // extra division can be avoided
		    }

		string info(char w) { return "  "+_brief()+"\n"; }
	};

	/**
	 * @brief Load pair potential from disk.
	 *
	 * Example (not yet implemented):
	 * ~~~~
	 * "PotentialMap" : {
	 *    "Na Na"   : { "harmonic" : { "k":10, "req":0.01  }  },
	 *    "Na Cl"   : { "fromdisk" : "na-cl.dat" },
	 *    "default" : { "coulomb" : { "epsr":80.  } }
	 * }
	 * ~~~~
	 */
	template<class T=double>
	    class FromDisk : public Potential::PairPotentialBase {
		private:
		    InterpolTable<T> t;
		    string filename;

		    string _brief() override {
			std::ostringstream o;
			o << PairPotentialBase::name << ": " << filename << " ["
			    << t.xmin() << ":" << t.xmax() << "]";
			return o.str();
		    }

		public:

		    FromDisk( Tmjson &j ) {
			PairPotentialBase::name = "fromdisk";
			string filename = j.at( string("file") );
			if (!t.load(filename))
			    throw std::runtime_error("Couldn't load tabulated potential.");
		    }

		    template<class Tparticle>
			double operator() (const Tparticle &a, const Tparticle &b, double r2) const {
			    double r=sqrt(r2);
			    if (r<t.xmin() || r>t.xmax()) {
				std::cerr << r << " " << t.xmin() << " " << t.xmax() << endl;
				throw std::runtime_error("Distance outside tabulated area.");
			    }
			    return t(r);
			}

		    template<typename Tparticle>
			Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			    assert(2==1 && "not implemented");
			    return Point(0,0,0);
			}
	    };

	/**
	 * @brief Custom potentials between specific particle types
	 *
	 * If the pair is not recognized, i.e. not added with the
	 * `add()` function, the `Tdefault` pair potential is used.
	 *
	 * Example:
	 *
	 *     PotentialMap<CoulombLJ> pot(...);
	 *     pot.add( atom["Na"].id ,atom["CH4"].id, ChargeNonpolar(...) );
	 *     pot.add( atom["Cl"].id ,atom["CH4"].id, ChargeNonpolar(...) );
	 */
	template<typename Tdefault, typename Tparticle=PointParticle, typename Tdist=double>
	    class PotentialMap : public Tdefault {
		protected:
		    typedef opair<int> Tpair;
		    typedef std::function<double(const Tparticle&,const Tparticle&,Tdist)> Tfunc;
		    typedef std::function<Point(const Tparticle&,const Tparticle&,double,const Point&)> Tforce;
		    std::map<Tpair,Tfunc> m;
		    std::map<Tpair,Tforce> mforce;
		    std::string _info; // info for the added potentials (before turning into functors)

		    // Force function object wrapper class
		    template<class Tpairpot>
			struct ForceFunctionObject {
			    Tpairpot pot;
			    ForceFunctionObject(const Tpairpot &p) : pot(p) {}
			    Point operator()(const Tparticle &a, const Tparticle &b, double r2, const Point &r) {
				return pot.force(a,b,r2,r);
			    }
			};

		public:
		    PotentialMap(Tmjson &j) : Tdefault(j) {
			Tdefault::name += " (default)";
		    }

		    /**
		     * @warning Templating `Tpairpot` may cause code-bloat if used on
		     * many different potentials
		     */
		    template<class Tpairpot>
			void add(AtomData::Tid id1, AtomData::Tid id2, Tpairpot pot) {
			    pot.name=atom[id1].name + "<->" + atom[id2].name + ": " + pot.name;
			    _info+="\n  " + pot.name + ":\n" + pot.info(20);
			    m[Tpair(id1,id2)] = pot;
			    mforce[Tpair(id1,id2)] = ForceFunctionObject<decltype(pot)>(pot);
			}

		    double operator()(const Tparticle &a, const Tparticle &b, const Tdist &r2) {
			auto i=m.find( Tpair(a.id,b.id) );
			if (i!=m.end())
			    return i->second(a,b,r2);
			return Tdefault::operator()(a,b,r2);
		    }

		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			auto i=mforce.find( Tpair(a.id,b.id) );
			if (i!=mforce.end())
			    return i->second(a,b,r2,p);
			return Tdefault::force(a,b,r2,p);
		    }

		    std::string info(char w=20) {
			return Tdefault::info(w) + _info;
		    }
	    };

	/**
	 * @brief Combines two pair potentials
	 * @details This combines two PairPotentialBases. The combined potential
	 * can subsequently be used as a normal pair potential and even be
	 * combined with a third potential and so forth.
	 *
	 *     // mix two and three pair potentials
	 *     using namespace Potential;
	 *     typedef CombinedPairPotential< LennardJones, SquareWell > Tpairpot1;
	 *     typedef CombinedPairPotential< Tpairpot1, Coulomb > Tpairpot2;
	 *     Tpairpot2 mypairpot;
	 *     std::cout << mypairpot.info();
	 *
	 * @date Lund, 2012
	 */
	template<class T1, class T2>
	    class CombinedPairPotential : public PairPotentialBase {
		private:
		    string _brief() {
			return first.brief() + " " + second.brief();
		    }
		    void setCutoff() {
			for (size_t i=0; i<atom.size(); i++)
			    for (size_t j=0; j<atom.size(); j++) {
				if (first.rcut2(i,j) > second.rcut2(i,j))
				    PairPotentialBase::rcut2.set(i,j,first.rcut2(i,j));
				else
				    PairPotentialBase::rcut2.set(i,j,second.rcut2(i,j));
			    }
		    }

		public:
		    T1 first;  //!< First pair potential of type T1
		    T2 second; //!< Second pair potential of type T2

		    CombinedPairPotential(T1 a, T2 b) : first(a), second(b) {
			name=first.name+"+"+second.name;
			setCutoff();
		    }

		    CombinedPairPotential(Tmjson &j) :
			PairPotentialBase(), first(j), second(j) {
			    name=first.name+"+"+second.name;
			    setCutoff();
			}

		    CombinedPairPotential(Tmjson &j1, Tmjson &j2) :
			PairPotentialBase(), first(j1), second(j2) {
			    name=first.name+"+"+second.name;
			    setCutoff();
			}

		    template<class Tparticle, class Tdist>
			double operator()(const Tparticle &a, const Tparticle &b, const Tdist &r2) {
			    return first(a,b,r2) + second(a,b,r2);
			}

		    template<typename Tparticle>
			Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			    return first.force(a,b,r2,p) + second.force(a,b,r2,p);
			}

		    template<typename Tparticle>
			Point field(const Tparticle &a, const Point &r) {
			    return first.field(a,r) + second.field(a,r);
			}

		    template<typename Tpvec>
			double internal(const Tpvec &p, const Group &g) {
			    return first.internal(p,g) + second.internal(p,g);
			}

		    template<class Tspace>
			void setSpace(Tspace &s) {
			    first.setSpace(s);
			    second.setSpace(s);
			}

		    string info(char w=20) {
			return first.info(w) + second.info(w);
		    }

		    void test(UnitTest &t) {
			first.test(t);
			second.test(t);
		    }
	    };

	/**
	 * @brief Creates a new pair potential with opposite sign
	 */
	template<class T>
	    struct Minus : public T {
		Minus(const T &pot) : T(pot) { T::name = "- " + T::name; }
		template<class Tparticle> // isotropic energy
		    double operator()(const Tparticle &a, const Tparticle &b, double r2) {
			return -T::operator()(a,b,r2);
		    }
		template<class Tparticle> // anisotropic energy
		    double operator()(const Tparticle &a, const Tparticle &b, const Point &r2) {
			return -T::operator()(a,b,r2);
		    }
		template<class Tparticle> // force
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			return -T::force(a,b,r2,p);
		    }
	    };

	/**
	 * @brief Creates a new pair potential scaled by `s`
	 */
	template<class T>
	    struct Scale : public T {
		double s;
		Scale(const T &pot, double s) : T(pot), s(s) { T::name = std::to_string(s) + " x " + T::name; }
		template<class Tparticle> // isotropic energy
		    double operator()(const Tparticle &a, const Tparticle &b, double r2) {
			return s*T::operator()(a,b,r2);
		    }
		template<class Tparticle> // anisotropic energy
		    double operator()(const Tparticle &a, const Tparticle &b, const Point &r2) {
			return s*T::operator()(a,b,r2);
		    }
		template<class Tparticle> // force
		    Point force(const Tparticle &a, const Tparticle &b, double r2, const Point &p) {
			return s*T::force(a,b,r2,p);
		    }
	    };


	/**
	 * @brief Adds two pair potentials
	 *
	 * Example:
	 *
	 *     auto PrimitiveModel = Potential::Coulomb(...) + Potential::HardSphere(...);
	 */
	template<class T1, class T2,
	    class = typename std::enable_if<std::is_base_of<PairPotentialBase,T1>::value>::type,
	    class = typename std::enable_if<std::is_base_of<PairPotentialBase,T2>::value>::type>
		CombinedPairPotential<T1,T2>& operator+(const T1 &pot1, const T2 &pot2) {
		    return *(new CombinedPairPotential<T1,T2>(pot1,pot2));
		}

	/**
	 * @brief Subtracts two pair potentials
	 *
	 * This can be useful for excluding non-bonded interactions between bonded pairs.
	 * Beware, though, that in the case of strongly repulsive interactions
	 * for example due to particle overlap,
	 * first adding then subtracting
	 * may lead to numerical issues, often manifested in a system energy
	 * drift.
	 *
	 * Example:
	 *
	 *     auto mypot = Potential::Harmonic(...) - Potential::LennardJones(...);
	 */
	template<class T1, class T2,
	    class = typename std::enable_if<std::is_base_of<PairPotentialBase,T1>::value>::type,
	    class = typename std::enable_if<std::is_base_of<PairPotentialBase,T2>::value>::type>
		CombinedPairPotential<T1,Minus<T2>>& operator-(const T1 &pot1, const T2 &pot2) {
		    return *(new CombinedPairPotential<T1,Minus<T2>>(pot1,Minus<T2>(pot2)));
		}

	/** @brief Scale potential */
	template<class Tpairpot,
	    class = typename std::enable_if<std::is_base_of<PairPotentialBase,Tpairpot>::value>::type>
		Scale<Tpairpot>& operator*(double s, const Tpairpot &pot) {
		    return *( new Scale<Tpairpot>(pot,s) );
		}

	/**
	 * @brief Lennard-Jones potential with Lorentz-Berthelot mixing rule
	 */
	typedef LennardJonesMixed<LorentzBerthelot> LennardJonesLB;

	/**
	 * @brief Combined Coulomb / HardSphere potential
	 */
	typedef CombinedPairPotential<Coulomb, HardSphere> CoulombHS;

	/**
	 * @brief Combined Coulomb / LennardJones potential
	 */
	typedef CombinedPairPotential<Coulomb, LennardJones> CoulombLJ;

	/**
	 * @brief Combined Coulomb / WeeksChandlerAndersen potential
	 */
	typedef CombinedPairPotential<Coulomb, WeeksChandlerAndersen> CoulombWCA;

	/**
	 * @brief Combined Coulomb / LennardJonesTrunkShift potential
	 */
	typedef CombinedPairPotential<Coulomb, LennardJonesTrunkShift> CoulombLJTS;

	/**
	 * @brief Combined Coulomb / LennardJones potential
	 */
	typedef CombinedPairPotential<CoulombWolf, LennardJones> CoulombWolfLJ;

	/**
	 * @brief Combined DebyeHuckel / HardSphere potential
	 */
	typedef CombinedPairPotential<DebyeHuckel, HardSphere> DebyeHuckelHS;

	/**
	 * @brief Combined DebyeHuckel / LennardJones potential
	 */
	typedef CombinedPairPotential<DebyeHuckel, LennardJones> DebyeHuckelLJ;

	/**
	 * @brief Combined DebyeHuckel / R12Repulsion potential
	 */
	typedef CombinedPairPotential<DebyeHuckel, R12Repulsion> DebyeHuckelr12;

	typedef CombinedPairPotential<Hertz,YukawaGel> HertzYukawa;

    } //end of Potential namespace

} //end of Faunus namespace
#endif
