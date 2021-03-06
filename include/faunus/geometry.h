#ifndef FAU_GEOMETRY_H
#define FAU_GEOMETRY_H

#ifndef SWIG
#include <faunus/point.h>
#include <faunus/slump.h>
#include <faunus/textio.h>
#include <faunus/physconst.h>
#include <faunus/species.h>
#include <faunus/inputfile.h>
#include <Eigen/Geometry>

#endif

namespace Faunus
{

    /**
     * @brief Namespace for geometric operations.
     *
     * This namespace contains classes for handling various simulation geometries
     * such as cubes, spheres, cylinder, slits etc. The geometry of a simulation
     * is handled by the base class Geometry::Geometrybase that gives all geometries
     * a common interfaces to handle distance calculations, boundary conditions,
     * volume calculation and so forth. In this way, the geometry of a simulation may be
     * changed without any code changes in for example energy calculations.
     */
    namespace Geometry
    {

	class Cuboid;

	/**
	 * @brief Polymorphic class for simulation geometries
	 *
	 * This is a polymorph class that handles simulation geometries.
	 * It contains distance calculation functions, boundary conditions, volume
	 * etc. A number of functions are defined as pure virtual (=0) meaning
	 * that these MUST be defined in derived classes.
	 *
	 * @note This class uses dynamic polymorphism, i.e. virtual functions, which
	 *       may have negative impact on performance as function inlining may not be
	 *       possible. This is usually a problem only for inner loop distance calculations.
	 *       To get optimum performance in inner loops use a derived class directly and do
	 *       static, compile-time polymorphism (templates).
	 */
	class Geometrybase
	{
	    private:
		virtual string _info( char )=0;
		virtual void _setVolume( double )=0;
		virtual double _getVolume(int) const =0;
		virtual double _getRadius() const;
	    protected:
		string name;                                        //!< Name of the geometry
		inline int anint( double x ) const
		{
		    return int(x > 0. ? x + .5 : x - .5);
		}

	    public:
		enum collisiontype { BOUNDARY, ZONE };                 //!< Types for collision() function
		double getVolume(int=3) const;                      //!< Get volume of container (A^3, A^2, A)
		double getRadius() const;                           //!< Get radius of appropriate geometries (A)
		void setVolume( double );                              //!< Specify new volume (A^3)
		double dist( const Point &, const Point & ) const;     //!< Distance between two points (A)
		std::string info( char= 20 );                          //!< Return info string

		virtual bool collision( const Point &,
			double,
			collisiontype= BOUNDARY ) const =0;//!< Check for collision with boundaries, forbidden zones, matter,..
		virtual void randompos( Point & )=0;              //!< Random point within container
		virtual void boundary( Point & ) const =0;             //!< Apply boundary conditions to a point
		virtual void scale( Point &, Point &, const double, const double ) const;  //!< Scale point
		virtual double sqdist( const Point &a, const Point &b ) const =0; //!< Squared distance between two points
		virtual Point vdist( const Point &, const Point & ) const =0;  //!< Distance in vector form

		virtual Cuboid inscribe() const; //!< Inscribe geometry in a cuboid

		Geometrybase();

		/**
		 * @brief Constructor
		 * @param name Name of geometry
		 */
		Geometrybase( const string& );

		virtual ~Geometrybase();
	};

	/**
	 * @brief Quaternion rotation routine using the Eigen library
	 * @note Boundary condition are respected.
	 */
	class QuaternionRotate
	{
	    private:
		double angle_;
		Eigen::Vector3d origin;
		Eigen::Quaterniond q;
		Eigen::Matrix3d rot_mat; // rotation matrix
		Geometrybase *geoPtr;

	    public:
		//!< Get rotation origin
		Eigen::Vector3d &getOrigin() { return origin; }

		//!< Get rotation origin
		Eigen::Vector3d getOrigin() const { return origin; }

		//!< Get set rotation angle
		double getAngle() const { return angle_; }

		bool ignoreBoundaries;

		QuaternionRotate()
		{
		    ignoreBoundaries = false;
		}

		/**
		 * @brief Set rotation axis and angle
		 * @param g Geometry to use for periodic boundaries (if any)
		 * @param beg Starting point for vector to rotate around
		 * @param end Ending point for vector to rotate around
		 * @param angle Radians to rotate
		 */
		void setAxis( Geometrybase &g, const Point &beg, const Point &end, double angle );

		/** @brief Rotate point - respect boundaries */
		inline Point operator()( Point a ) const
		{
		    if ( ignoreBoundaries )
			return q * a;
		    a = a - origin;
		    geoPtr->boundary(a);
		    a = q * a + origin;
		    geoPtr->boundary(a);
		    return a;
		}

		inline Eigen::Matrix3d operator()( Eigen::Matrix3d a ) const
		{
		    a = rot_mat * a * rot_mat.transpose();
		    return a;
		}
	};

	/**
	 * @brief Spherical-surface geometry
	 *
	 * This is a spherical-surface simulation container.
	 */
	class SphereSurface : public Geometrybase 
	{
	    private:
		double r,r2,diameter;
		void _setVolume(double) override;
		double _getVolume(int) const override;
		double _getRadius() const override;                     //!< Get radius (angstrom)
		string _info(char) override;
	    public:
		Point len;
		void setlen(const Point&);              //!< Reset radius (angstrom)
		void setRadius(double);                 //!< Set radius (angstrom)

		SphereSurface(double=0);                         //!< Construct from radius (angstrom)

		/** @brief Construct from json object */
		SphereSurface( Tmjson & );

		void randompos(Point &) override;
		void boundary(Point &p) const override {};
		bool collision(const Point&, double, collisiontype=BOUNDARY) const override;

		inline double sqdist(const Point &a, const Point &b) const override {
		    double r1 = r*std::acos(a.dot(b)/r2);
		    return r1*r1;
		}

		/**
		 * @warning Not true!
		 */
		inline Point vdist(const Point &a, const Point &b) const override { return a-b; }

		void scale(Point&, Point &, const double, const double) const override; //!< Linear scaling along radius (NPT ensemble)

		Cuboid inscribe() const override;

	};

	/**
	 * @brief Spherical geometry
	 *
	 * This is a spherical simulation container, surrounded by a hard wall.
	 */
	class Sphere : public Geometrybase
	{
	    private:
		double r, r2, diameter;
		void _setVolume( double ) override;
		double _getVolume(int) const override;
		string _info( char ) override;
	    public:
		Point len;
		void setlen( const Point & );              //!< Reset radius (angstrom)
		void setRadius( double );                 //!< Set radius (angstrom)
		Sphere( double=0 );                       //!< Construct from radius (angstrom)

		/** @brief Construct from json object */
		Sphere( Tmjson & );

		void randompos( Point & ) override;

		void boundary( Point &p ) const override {};
		bool collision( const Point &, double, collisiontype= BOUNDARY ) const override;

		inline double sqdist( const Point &a, const Point &b ) const override
		{
		    return (a - b).squaredNorm();
		}

		inline Point vdist( const Point &a, const Point &b ) const override { return a - b; }

		void scale( Point &,
			Point &,
			const double,
			const double ) const override; //!< Linear scaling along radius (NPT ensemble)

		Cuboid inscribe() const override;

	};

	/**
	 * @brief Hexagonal prism geometry
	 *
	 * This is a Hexagonal prism simulation container.
	 */
	class Hexagon : public Geometrybase 
	{
	    private:
		void _setVolume( double ) override;
		double r,r2,diameter;
		double _getVolume(int) const override;
		double _getRadius() const override;                     //!< Get radius (angstrom)
		string _info(char) override;
		enum scaletype { XYZ, XY };
		scaletype scaledir;                      //!< Scale directions for pressure scaling
		string scaledirstr;
		Point unitvX;
		Point unitvY;
		Point unitvZ;
	    public:
		Point len;
		void setlen(const Point&);              //!< Reset radius (angstrom)
		void setRadius(double);                 //!< Set radius (angstrom)
		void setHeight(double);                 //!< Set height (angstrom)
		Point getRandomPointdInTriangle();

		Hexagon(double=0,double=0);                         //!< Construct from radius (angstrom)

		/** @brief Construct from json object */
		Hexagon( Tmjson & );

		Point randompos();
		void randompos(Point &) override;

		void scale(Point&, Point &, const double, const double) const override; //!< Linear scaling along radius (NPT ensemble)

		inline bool collision(const Point &a, double radius, collisiontype type = BOUNDARY) const override {

		    if(std::fabs(a.dot(unitvX)) > len.y())
			return true;
		    if(std::fabs(a.dot(unitvY)) > len.y())
			return true;
		    if(std::fabs(a.dot(unitvZ)) > len.y())
			return true;

		    if(std::fabs(a.z()) > len.z()/2.0)
			return true;

		    return false;
		}

		inline double sqdist(const Point &a, const Point &b) const override {
		    Point r = a - b;
		    Hexagon::boundary(r);
		    return r.squaredNorm();
		}

		inline Point vdist(const Point &a, const Point &b) const override { 
		    Point r = a - b;
		    Hexagon::boundary(r);
		    return r;
		}

		inline void boundary( Point &a ) const override	{
		    if(a.dot(unitvX) > len.y())
			a = a - 2.0*len.y()*unitvX;
		    if(a.dot(unitvX) < -len.y())
			a = a + 2.0*len.y()*unitvX;
		    if(a.dot(unitvY) > len.y()) {
			a = a - 2.0*len.y()*unitvY;
			if(a.dot(unitvX) < -len.y()) // Check that point did not get past x-limit
			    a = a + 2.0*len.y()*unitvX;
		    }
		    if(a.dot(unitvY) < -len.y()) {
			a = a + 2.0*len.y()*unitvY;
			if(a.dot(unitvX) > len.y()) // Check that point did not get past x-limit
			    a = a - 2.0*len.y()*unitvX;
		    }
		    if(a.dot(unitvZ) > len.y())
			a = a - 2.0*len.y()*unitvZ;
		    if(a.dot(unitvZ) < -len.y())
			a = a + 2.0*len.y()*unitvZ;

		    if(a.z() > len.z()/2.0)
			a.z() = a.z() - len.z();
		    if(a.z() < -len.z()/2.0)
			a.z() = a.z() + len.z();
		}

		Cuboid inscribe() const override;
	};

	/**
	 * @brief Truncated octahedron geometry
	 *
	 * This is a Truncated octahedron simulation container.
	 */
	class Octahedron : public Geometrybase 
	{
	    private:
		void _setVolume( double ) override;
		double r,r2,diameter;
		double _getVolume(int) const override;
		double getLength() const;                     //!< Get length (angstrom)
		string _info(char) override;
		enum scaletype { XYZ, XY };
		scaletype scaledir;                      //!< Scale directions for pressure scaling
		string scaledirstr;
		Point unitvX, unitvY, unitvZ;
		Point unitvXYZ, unitvXiYZ, unitvXYiZ, unitvXYZi;
	    public:
		Point len;
		Point radiusV;                      //!< inradius, midradius, circumradius
		void setlen(double);              //!< Reset length (angstrom)
		void setlen(const Point&);              //!< Reset length (angstrom)
		void setLength(double);                 //!< Set radius (angstrom)
		Point getRandomPointdInTriangle();

		Octahedron(double=0);                         //!< Construct from length (angstrom)

		/** @brief Construct from json object */
		Octahedron( Tmjson & );

		Point randompos();
		void randompos(Point &) override;

		void scale(Point&, Point &, const double, const double) const override; //!< Linear scaling along radius (NPT ensemble)

		inline bool collision(const Point &a, double radius, collisiontype type = BOUNDARY) const override {

		    if(std::fabs(a.dot(unitvX)) > radiusV.y()) // sqrt(2) square
			return true;
		    if(std::fabs(a.dot(unitvY)) > radiusV.y())
			return true;
		    if(std::fabs(a.dot(unitvZ)) > radiusV.y())
			return true;
		    if(std::fabs(a.dot(unitvXYZ)) > radiusV.x()) // sqrt(6)/2 hexagon
			return true;
		    if(std::fabs(a.dot(unitvXiYZ)) > radiusV.x())
			return true;
		    if(std::fabs(a.dot(unitvXYiZ)) > radiusV.x())
			return true;
		    if(std::fabs(a.dot(unitvXYZi)) > radiusV.x())
			return true;

		    return false;
		}

		inline double sqdist(const Point &a, const Point &b) const override {
		    Point r = a - b;
		    Octahedron::boundary(r);
		    return r.squaredNorm();
		}

		inline Point vdist(const Point &a, const Point &b) const override { 
		    Point r = a - b;
		    Octahedron::boundary(r);
		    return r;
		}

		inline void boundary( Point &a ) const override	{
		  bool tmp = false;
		  
		  do {
		    tmp = false;

		    
		    if(a.dot(unitvXYZ) > radiusV.x()) {
		      a = a - 2.0*radiusV.x()*unitvXYZ;
		      tmp = true;
		    }
		    if(a.dot(unitvXYZ) < -radiusV.x()) {
		      a = a + 2.0*radiusV.x()*unitvXYZ;
		      tmp = true;
		    }
		    if(a.dot(unitvXiYZ) > radiusV.x()) {
		      a = a - 2.0*radiusV.x()*unitvXiYZ;
		      tmp = true;
		    }
		    if(a.dot(unitvXiYZ) < -radiusV.x()) {
		      a = a + 2.0*radiusV.x()*unitvXiYZ;
		      tmp = true;
		    }
		    
		    if(a.dot(unitvXYiZ) > radiusV.x()) {
		      a = a - 2.0*radiusV.x()*unitvXYiZ;
		      tmp = true;
		    }
		    if(a.dot(unitvXYiZ) < -radiusV.x()) {
		      a = a + 2.0*radiusV.x()*unitvXYiZ;
		      tmp = true;
		    }
		    if(a.dot(unitvXYZi) > radiusV.x()) {
		      a = a - 2.0*radiusV.x()*unitvXYZi;
		      tmp = true;
		    }
		    if(a.dot(unitvXYZi) < -radiusV.x()) {
		      a = a + 2.0*radiusV.x()*unitvXYZi;
		      tmp = true;
		    }
		    
		  } while(tmp);
		  
		    if(a.dot(unitvX) > radiusV.y()) {
		      a = a - 2.0*radiusV.y()*unitvX;
		    }
		    if(a.dot(unitvX) < -radiusV.y()) {
		      a = a + 2.0*radiusV.y()*unitvX;
		    }
		    if(a.dot(unitvY) > radiusV.y()) {
		      a = a - 2.0*radiusV.y()*unitvY;
		    }
		    if(a.dot(unitvY) < -radiusV.y()) {
		      a = a + 2.0*radiusV.y()*unitvY;
		    }
		    if(a.dot(unitvZ) > radiusV.y()) {
		      a = a - 2.0*radiusV.y()*unitvZ;
		    }
		    if(a.dot(unitvZ) < -radiusV.y()) {
		      a = a + 2.0*radiusV.y()*unitvZ;
		    }
		}

		Cuboid inscribe() const override;
	};

	/** @brief Cuboid geometry with periodic boundaries
	 *
	 *  The Cuboid simulation container has right angles, rectangular faces 
	 *  and periodic boundaries.
	 *
	 *  @author Chris Evers
	 *  @date Lund, nov 2010
	 */
	class Cuboid : public Geometrybase
	{
	    private:
		string _info( char ) override;             //!< Return info string
		void _setVolume( double ) override;
		double _getVolume(int) const override;
		enum scaletype { XYZ, XY };
		scaletype scaledir;                      //!< Scale directions for pressure scaling
		string scaledirstr;
	    protected:
		Point len_inv;                           //!< Inverse sidelengths

	    public:

		Cuboid();

		Cuboid( Tmjson & ); //!< Construct from JSON input

		void setlen( const Point & );               //!< Reset Cuboid sidelengths
		Point len;                               //!< Sidelengths
		Point len_half;                          //!< Half sidelength
		Point randompos();
		void randompos( Point & ) override;

		inline bool collision( const Point &a, double radius, collisiontype type = BOUNDARY ) const override
		{
		    if ( std::abs(a.x()) > len_half.x())
			return true;
		    if ( std::abs(a.y()) > len_half.y())
			return true;
		    if ( std::abs(a.z()) > len_half.z())
			return true;
		    return false;
		}

		/**
		 * For reviews of minimum image algorithms,
		 * see doi:10/ck2nrd and doi:10/kvs
		 */
		inline double sqdist( const Point &a, const Point &b ) const override
		{
		    Point d = (a - b).cwiseAbs();
		    for ( int i = 0; i < 3; ++i )
			if ( d[i] > len_half[i] )
			    d[i] -= 2 * len_half[i];
		    return d.squaredNorm();

		    // Alternative algorithm:
		    // Eigen::Vector3d d = a-b;
		    // Eigen::Vector3i k = (2*d.cwiseProduct(len_inv)).cast<int>();
		    // return (d-k.cast<double>().cwiseProduct(len)).squaredNorm();
		}

		inline Point vdist( const Point &a, const Point &b ) const override
		{
		    Point r = a - b;
		    if ( r.x() > len_half.x())
			r.x() -= len.x();
		    else if ( r.x() < -len_half.x())
			r.x() += len.x();
		    if ( r.y() > len_half.y())
			r.y() -= len.y();
		    else if ( r.y() < -len_half.y())
			r.y() += len.y();
		    if ( r.z() > len_half.z())
			r.z() -= len.z();
		    else if ( r.z() < -len_half.z())
			r.z() += len.z();
		    return r;
		}

		inline void boundary( Point &a ) const override
		{
		    if ( std::abs(a.x()) > len_half.x())
			a.x() -= len.x() * anint(a.x() * len_inv.x());
		    if ( std::abs(a.y()) > len_half.y())
			a.y() -= len.y() * anint(a.y() * len_inv.y());
		    if ( std::abs(a.z()) > len_half.z())
			a.z() -= len.z() * anint(a.z() * len_inv.z());
		}

		void scale( Point &, Point &, const double, const double ) const override;

		Cuboid inscribe() const override;
	};

	/**
	 * @brief Cuboid with no periodic boundaries in z direction
	 *
	 * Same input as for `Cuboid`.
	 *
	 * @author Chris Evers
	 * @date Lund, nov 2010
	 */
	class Cuboidslit : public Cuboid
	{
	    public:
		Cuboidslit();
		Cuboidslit( Tmjson & );

		inline double sqdist( const Point &a, const Point &b ) const override
		{
		    double dx = std::abs(a.x() - b.x());
		    double dy = std::abs(a.y() - b.y());
		    double dz = a.z() - b.z();
		    if ( dx > len_half.x())
			dx -= len.x();
		    if ( dy > len_half.y())
			dy -= len.y();
		    return dx * dx + dy * dy + dz * dz;
		}

		inline Point vdist( const Point &a, const Point &b ) const override
		{
		    Point r(a - b);
		    if ( r.x() > len_half.x())
			r.x() -= len.x();
		    else if ( r.x() < -len_half.x())
			r.x() += len.x();
		    if ( r.y() > len_half.y())
			r.y() -= len.y();
		    else if ( r.y() < -len_half.y())
			r.y() += len.y();
		    return r;
		}

		inline void boundary( Point &a ) const override
		{
		    if ( std::abs(a.x()) > len_half.x())
			a.x() -= len.x() * anint(a.x() * len_inv.x());
		    if ( std::abs(a.y()) > len_half.y())
			a.y() -= len.y() * anint(a.y() * len_inv.y());
		}
	};

	/** @brief Cuboid with no periodic boundaries (hard box) */
	struct CuboidNoPBC : public Cuboid
	{
	    CuboidNoPBC();
	    CuboidNoPBC( Tmjson & );

	    inline Point vdist( const Point &a, const Point &b ) const override { return a - b; }

	    inline double sqdist( const Point &a, const Point &b ) const override { return (a - b).squaredNorm(); }

	    inline void boundary( Point &a ) const override {}
	};

	/**
	 * @brief Cylindrical simulation container
	 *
	 * This is a cylindrical simulation container where all walls
	 * are HARD. The origin is in the middle of the cylinder.
	 *
	 * @todo Fix cylinder class so that it will work with
	 *       isobaric volume moves. Currently, the variable
	 *       `Point len` is merely a dummy.
	 */
	class Cylinder : public Geometrybase
	{
	    private:
		string _info( char ) override; //!< Cylinder info
		void _setVolume( double ) override;
		double _getVolume(int) const override;
		void init( double, double );
		double r2;    //!< Cylinder radius squared
		double r;     //!< Cylinder radius
		double diameter;
	    protected:
		double _len;   //!< Cylinder length
		double _halflen; //!< Cylinder half length
	    public:
		Point len;                        //!< Dummy
		Cylinder( Tmjson & );             //!< Constructor
		Cylinder( double=0, double=0);    //!< Construct from length and radius
		void setlen( const Point & );     //!< Set length via vector (dummy function)
		void randompos( Point & ) override;
		void boundary( Point & ) const override;
		bool collision( const Point &, double, collisiontype= BOUNDARY ) const override;

		inline double sqdist( const Point &a, const Point &b ) const override
		{
		    return (a - b).squaredNorm();
		}

		inline Point vdist( const Point &a, const Point &b ) const override
		{
		    return a - b;
		}
		
		inline double getLength() const { return _len; }

		inline double getRadius() const { return r; }
		
		inline double getDiameter() const { return diameter; }

		Cuboid inscribe() const override;
	};

	/**
	 * @brief Cylinder with periodic boundaries in the z direction
	 */
	class PeriodicCylinder : public Cylinder
	{

	    public:

		PeriodicCylinder( Tmjson & );
		PeriodicCylinder( double=0, double=0 );

		void boundary( Point & ) const override;

		inline double sqdist( const Point &a, const Point &b ) const override
		{
		    double dx = a.x() - b.x();
		    double dy = a.y() - b.y();
		    double dz = std::abs(a.z() - b.z());
		    if ( dz > _halflen )
			dz -= _len;
		    return dx * dx + dy * dy + dz * dz;
		}

		inline Point vdist( const Point &a, const Point &b ) const override
		{
		    Point r = a - b;
		    if ( r.z() > _halflen )
			r.z() -= _len;
		    else if ( r.z() < -_halflen )
			r.z() += _len;
		    return r;
		}
	};

	/**
	 * @brief Calculate center of cluster of particles
	 * @param geo Geometry
	 * @param p Particle vector
	 * @param g Range of particle index (`Group`, `vector<int>`, ...)
	 * @param weight Weight function
	 *
	 * The weight function is typically a lambda function that takes a particle
	 * as an argument and returns the weight, for example Mw, charge, or unity
	 * for mass center, charge center, or geometric center. Functions for
	 * these three examples are predefined.
	 */
	template<class Tgeo, class Tpvec, class TGroup, class Tweightf>
	    Point anyCenter( const Tgeo &geo, const Tpvec &p, const TGroup &g, Tweightf weight )
	    {
		assert(g.back() < (int) p.size());
		Point cm(0, 0, 0);
		if ( !g.empty())
		{
		    Point o = p[g.front() + (g.back() - g.front()) * 0.5];  // set origo to middle particle
		    //cout << "center of group: " << o.transpose() << endl;
		    double sum = 0;
		    for ( auto i : g )
		    {
			Point t = p[i] - o;       // translate to origo
			geo.boundary(t);        // periodic boundary (if any)
			//cout << "Dist: " << t.norm() << ", " << t.transpose() << ", " << p[i].transpose() << ", " << o.transpose() << endl;

			cm += t * weight(p[i]);
			sum += weight(p[i]);
		    }
		    if ( fabs(sum) < 1e-6 )
			sum = 1;
		    cm = cm / sum + o;
		    geo.boundary(cm);
		}
		return cm;
	    }

	/**
	 * @brief Calculate geometrical center of cluster of groups
	 * @param spc Space
	 * @param group Vector of pointers to the groups in the cluster
	 */
	template<class Tgeo, class Tparticles, class GroupIndex>
	    Point trigoComCluster( const Tgeo &geo, const Tparticles &p, const GroupIndex &groups, const std::vector<int> &dir = {0, 1, 2} )
	    {
		assert(!dir.empty() && dir.size() <= 3);
		Point xhi(0, 0, 0), zeta(0, 0, 0), theta(0, 0, 0), com(0, 0, 0);
		for ( auto k : dir ) {
		    double q = 2 * pc::pi / geo.len[k];
		    size_t N=0;
		    for (auto i : groups)
			for (auto particle : *i) {
			    Point pos = p[particle];
			    theta[k] = pos[k] * q;
			    zeta[k] += std::sin(theta[k]);
			    xhi[k] += std::cos(theta[k]);
			    N++;
			}
		    theta[k] = std::atan2(-zeta[k] / N, -xhi[k] / N) + pc::pi;
		    com[k] = geo.len[k] * theta[k] / (2 * pc::pi);
		}
		geo.boundary(com); // is this really needed?
		return com;
	    }


	/**
	 * @brief Calculate center of cluster of groups
	 * @param geo Geometry
	 * @param p Particle vector
	 * @param g Vector of pointers to range of particle index (`Group`, `vector<int>`, ...)
	 * @param weight Weight function
	 *
	 * The weight function is typically a lambda function that takes a particle
	 * as an argument and returns the weight, for example Mw, charge, or unity
	 * for mass center, charge center, or geometric center. Functions for
	 * these three examples are predefined.
	 */
	template<class Tgeo, class Tpvec, class TGroup, class Tweightf>
	    Point clusterCenter( const Tgeo &geo, const Tpvec &p, const vector<TGroup *> g, Tweightf weight )
	    {
		assert(g.size() <= (int) p.size());
		Point cm(0, 0, 0);
		if ( !g.empty())
		{
		    Point o = p[g.at(0)->front() + (g.at(0)->back() - g.at(0)->front()) * 0.5];  // set origo to middle particle
		    double sum = 0;
		    for(auto k : g)  {
			for ( auto i : *k )
			{
			    Point t = p[i] - o;       // translate to origo
			    geo.boundary(t);        // periodic boundary (if any)
			    cm += t * weight(p[i]);
			    sum += weight(p[i]);
			}
		    }
		    if ( fabs(sum) < 1e-6 )
			sum = 1;
		    cm = cm / sum + o;
		    geo.boundary(cm);
		}
		return cm;
	    }

	/** @brief Calculate mass center of cluster of particles */
	template<class Tgeo, class Tpvec, class TGroup>
	    Point clusterMassCenter( const Tgeo &geo, const Tpvec &p, const vector<TGroup *> g )
	    {
		return clusterCenter(geo, p, g,
			[]( const typename Tpvec::value_type &x ) { return x.mw; });
	    }


	/** @brief Calculate mass center of cluster of particles */
	template<class Tgeo, class Tpvec, class TGroup>
	    Point massCenter( const Tgeo &geo, const Tpvec &p, const TGroup &g )
	    {
		return anyCenter(geo, p, g,
			[]( const typename Tpvec::value_type &x ) { return x.mw; });
	    }

	/** @brief Calculate mass center of a particle vector */
	template<typename Tgeometry, typename Tp_vec>
	    Point massCenter( const Tgeometry &geo, const Tp_vec &p )
	    {
		if ( p.empty())
		    return Point(0, 0, 0);
		return massCenter(geo, p, Group(0, p.size() - 1));
	    }

	/** @brief Calculate charge center of cluster of particles */
	template<class Tgeo, class Tpvec, class TGroup>
	    Point chargeCenter( const Tgeo &geo, const Tpvec &p, const TGroup &g )
	    {
		return anyCenter(geo, p, g,
			[]( const typename Tpvec::value_type &x ) { return std::fabs(x.charge); });
	    }

	/** @brief Calculate charge center of a particle vector */
	template<typename Tgeometry, typename Tp_vec>
	    Point chargeCenter( const Tgeometry &geo, const Tp_vec &p )
	    {
		if ( p.empty())
		    return Point(0, 0, 0);
		return chargeCenter(geo, p, Group(0, p.size() - 1));
	    }

	/** @brief Calculate geometric center of cluster of particles */
	template<class Tgeo, class Tpvec, class TGroup>
	    Point geometricCenter( const Tgeo &geo, const Tpvec &p, const TGroup &g )
	    {
		return anyCenter(geo, p, g,
			[]( const typename Tpvec::value_type &x ) { return 1.0; });
	    }

	/**
	 * @brief Electric dipole moment
	 * @param s Space
	 * @param g Group or other contained with atom index
	 * @param cutoff Spherical cutoff (default: 1e9 angstrom)
	 * @param mu Initial dipole moment (default: [0,0,0])
	 */
	template<class Tspace, class Tgroup>
	    Point dipoleMoment( const Tspace &s, const Tgroup &g, double cutoff = 1e9, Point mu = Point(0, 0, 0))
	    {
		assert(g.size() <= (int) s.p.size());
		for ( auto i : g )
		{
		    Point t = s.p[i] - g.cm;
		    s.geo.boundary(t);
		    if ( t.squaredNorm() < cutoff * cutoff )
			mu += t * s.p[i].charge;
		}
		return mu;
	    }

	/**
	 * @brief Electric quadrupole moment
	 * @param s Space
	 * @param g Group or other contained with atom index
	 * @param cutoff Spherical cutoff (default: 1e9 angstrom)
	 * @param quad Initial quadrupole moment (default: [0 0 0,0 0 0,0 0 0])
	 */
	template<class Tspace, class Tgroup, class T=double>
	    Tensor<T> quadrupoleMoment(const Tspace &s, const Tgroup &g, double cutoff=1e9, Tensor<T> quad=Tensor<T>()) {
		assert(g.size()<=(int)s.p.size());
		for (auto i : g) {
		    Point t=s.p[i] - g.cm;
		    s.geo.boundary(t);
		    if(t.squaredNorm() < cutoff*cutoff)  {
			Tensor<T> temp = t*t.transpose();
			quad += temp*s.p[i].charge;
		    }
		}
		return quad;
	    }

	/**
	 * @brief Calculates quadrupole moment tensor (with trace)
	 */
	template<class Tpvec, class Tgroup, class Tgeo>
	    Tensor<double> quadrupoleMoment( const Tpvec &p, const Tgeo &geo, const Tgroup &g )
	    {
		Tensor<double> theta;
		theta.setZero();
		assert(g.size() <= (int) p.size());
		for ( auto i : g )
		{
		    Point t = p[i] - g.cm;
		    geo.boundary(t);
		    theta = theta + t * t.transpose() * p[i].charge;
		}
		return 0.5 * theta;
	    }

	/** @brief Translate a particle vector by a vector */
	template<class Tgeo, class Tpvec>
	    void translate( const Tgeo &geo, Tpvec &p, const Point &d )
	    {
		for ( auto &pi : p )
		{
		    pi += d;
		    geo.boundary(pi);
		}
	    }

	/** @brief Translate a particle vector so mass center is in (0,0,0) */
	template<class Tgeo, class Tpvec>
	    void cm2origo( const Tgeo &geo, Tpvec &p )
	    {
		translate(geo, p, -massCenter(geo, p));
	    }

	/** @brief Translate a particle vector so charge center is in (0,0,0) */
	template<class Tgeo, class Tpvec>
	    void cc2origo( const Tgeo &geo, Tpvec &p )
	    {
		translate(geo, p, -chargeCenter(geo, p));
	    }



	/**
	 * @brief Find an empty space for a particle vector in a space of other particles
	 * @author Mikael Lund
	 * @date Asljunga 2011
	 * @todo Implement random rotation in addition to current translation scheme.
	 *       Reimplement as derivative of `MoleculeInserterBase`
	 */
	class FindSpace
	{
	    private:
		template<class Tgeometry, class Tpvec>
		    bool matterOverlap( const Tgeometry &geo, const Tpvec &p1, const Tpvec &p2 ) const
		    {
			if ( allowMatterOverlap == false )
			    for ( auto &i : p1 )
				for ( auto &j : p2 )
				{
				    double max = i.radius + j.radius;
				    if ( geo.sqdist(i, j) < max * max )
					return true;
				}
			return false;
		    }

		template<class Tgeometry, class Tpvec>
		    bool containerOverlap( const Tgeometry &geo, const Tpvec &p ) const
		    {
			if ( allowContainerOverlap == false )
			    for ( auto &i : p )
				if ( geo.collision(i, i.radius))
				    return true;
			return false;
		    }

	    public:
		Point dir;                  //!< default = [1,1,1]
		bool allowContainerOverlap; //!< default = false;
		bool allowMatterOverlap;    //!< default = false;

		FindSpace();

		/**
		 * @param geo Geometry to use
		 * @param dst Destination particle vector (will not be touched!)
		 * @param p Particle vector to find space for. Coordinates will be changed.
		 * @param maxtrials Number of times to try before timeout.
		 */
		template<class Tgeometry, class Tpvec>
		    bool find( Tgeometry &geo, const Tpvec &dst, Tpvec &p, int maxtrials = 1e3 ) const
		    {
			using namespace textio;
			cout << "Trying to insert " << p.size() << " particle(s)";
			Point v;
			do
			{
			    cout << ".";
			    maxtrials--;
			    Point cm = massCenter(geo, p);
			    geo.randompos(v);
			    v = v.cwiseProduct(dir);
			    translate(geo, p, -cm + v);
			}
			while ( maxtrials > 0 && (containerOverlap(geo, p) || matterOverlap(geo, p, dst)));
			if ( maxtrials > 0 )
			{
			    cout << " OK!\n";
			    return true;
			}
			cout << " timeout!\n";
			assert(!"Timeout - found no space for particle(s).");
			return false;
		    }

		template<class Tgeometry, class Tpvec>
		    bool find( const Tpvec &dst, Tpvec &p, Tgeometry &geo ) const
		    {

			Point v;
			Point cm = massCenter(geo, p);
			geo.randompos(v);
			v = v.cwiseProduct(dir);
			translate(geo, p, -cm + v);

			if ( !containerOverlap(geo, p) && !matterOverlap(geo, p, dst))
			    return true;

			return false;
		    }
	};

	/**
	 * @brief Calculates the volume of a collection of particles
	 *
	 * This will use a brute force, stochastic hit and miss algorithm to
	 * calculate the net volume of a collection of overlapping
	 * particles.
	 *
	 * @param p Particle vector (structure must be whole)
	 * @param n Number of iterations (default: 1e7)
	 * @param pradius Probe radius (default: 0)
	 *
	 * @todo More efficient sampling may be achieved by adjusting the
	 *       box vs. molecular volume (i.e. 1:1)
	 */
	template<typename Tpvec>
	    double calcVolume( const Tpvec &p, unsigned int n = 1e7, double pradius = 0 )
	    {
		double L = 0;      // size of test box
		Point gc(0, 0, 0); // geometric center of molecule
		for ( auto &i : p )
		    gc += i / p.size();
		for ( auto &i : p )
		    L = std::max(L, 2 * ((i - gc).norm() + i.radius + pradius));

		// Start shooting!
		Point r;
		unsigned int hit = 0, cnt = 0;
		while ( ++cnt < n )
		{
		    r.x() = slump.half();
		    r.y() = slump.half();
		    r.z() = slump.half();
		    r = r * L + gc;
		    for ( auto &i : p )
			if ((i - r).squaredNorm() < pow(i.radius + pradius, 2))
			{
			    hit++;
			    break;
			}
		}
		return hit / double(cnt) * pow(L, 3);
	    }

	/** @brief Check for hard-core overlap between two particles (true if overlap) */
	template<typename Tgeometry, typename Tparticle>
	    bool overlap( const Tgeometry &geo, const Tparticle &a, const Tparticle &b )
	    {
		auto dmin = a.radius + b.radius;
		return (geo.sqdist(a, b) < dmin * dmin) ? true : false;
	    }

	/** @brief Check for hard-core overlap between a vector of particles and a particle (true if overlap) */
	template<typename Tgeometry, typename Tparticle, typename Talloc>
	    bool overlap( const Tgeometry &geo, const std::vector<Tparticle, Talloc> &p, const Tparticle &j )
	    {
		for ( auto &i : p )
		    if ( overlap(geo, i, j))
			return true;
		return false;
	    }

	/**
	 * @brief Calculates the gyration tensor of a molecular group
	 *
	 * @param geo Geometry to use for periodic boundaries (if any)
	 * @param p Particle vector
	 * @param g Molecular group
	 * @param cm Center-of-mass of the molecular group
	 *
	 * The gyration tensor is computed from the dyadic product of the position
	 * vectors in the c-o-m reference system, \f$ t_{i} = r_{i} - cm \f$:
	 * \f$ S = \sum_{i=0}^{N} t_{i} t_{i}^{T} \f$
	 *
	 */
	template<typename Tgeometry, typename Tpvec, typename Tgroup>
	    Tensor<double> gyration( Tgeometry &geo, Tpvec &p, Tgroup &g, const Point &cm )
	    {
		Tensor<double> S;
		for ( auto i : g )
		{
		    Point t = p[i] - cm;
		    geo.boundary(t);
		    S += t * t.transpose();
		}
		return S * (1. / g.size());
	    }

	/* 
	 * @brief Calculate mass center of cluster of particles in unbounded environment 
	 * @warning Untested
	 * 
	 * @param geo Geometry to use
	 * @param p Particle vector
	 * @param g Molecular group
	 * @param dir Directions to loop over: x=0, y=1, z=2. Default: xyz={0,1,2}
	 *
	 * [More info](http://dx.doi.org/10.1080/2151237X.2008.10129266)
	 *
	 * Example:
	 *
	 *     auto com = trigoCom( spc.geo, spc.p, g, {0,1,2} ); // xyz
	 *     auto com = trigoCom( spc.geo, spc.p, g, {2} );     // z only
	 *
	 * @todo Rename?
	 */
	template<class Tgeo, class Tpvec, class TGroup>
	    Point trigoCom( const Tgeo &geo, const Tpvec &p, const TGroup &g, const vector<int> &dir = {0, 1, 2} )
	    {
		assert(!dir.empty() && dir.size() <= 3);
		double N = g.size();
		Point xhi(0, 0, 0), zeta(0, 0, 0), theta(0, 0, 0), com(0, 0, 0);
		for ( auto k : dir )
		{
		    double q = 2 * pc::pi / geo.len[k];
		    for ( auto i : g )
		    {
			theta[k] = p[i][k] * q;
			zeta[k] += std::sin(theta[k]);
			xhi[k] += std::cos(theta[k]);
		    }
		    theta[k] = std::atan2(-zeta[k] / N, -xhi[k] / N) + pc::pi;
		    com[k] = geo.len[k] * theta[k] / (2 * pc::pi);
		}
		geo.boundary(com);
		return com;
	    }
    }//namespace Geometry
}//namespace Faunus
#endif
