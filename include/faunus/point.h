#ifndef FAUNUS_POINT_H
#define FAUNUS_POINT_H

#ifndef SWIG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <Eigen/Core>
#include <Eigen/Geometry>

#pragma GCC diagnostic pop
#include <vector>

#endif

namespace Faunus
{

    class AtomData;

    /** @brief Convert eigen type to std::vector */
    template<typename OtherDerived>
	std::vector<double> Eigen2Vector( const Eigen::MatrixBase<OtherDerived> &v1 ) {
	    std::vector<double> v2;
	    v2.resize(v1.size());
	    OtherDerived::Map(&v2[0], v1.size()) = v1;
	    return v2;
	}

    /**
     * @brief Cartesian coordinates
     *
     * This is the base class for all particles and takes care
     * of positions, only. It is derived from `Eigen::Vector3d`
     * vector and all particles in faunus can hence be freely
     * mixed with Eigen objects.
     */
    struct PointBase : public Eigen::Vector3d
    {
	typedef double Tcoord;        //!< Floating point type for Point coordinates
	typedef Eigen::Vector3d Tvec; //!< 3D vector from Eigen

	/** @brief Default constructor. Data is *not* zeroed */
	inline PointBase() {}

	PointBase( Tcoord x, Tcoord y, Tcoord z ) : Tvec(x, y, z) {}

	template<typename OtherDerived>
	    PointBase( const Eigen::MatrixBase<OtherDerived> &other ) : Tvec(other) {}

	template<typename OtherDerived>
	    PointBase &operator=( const Eigen::MatrixBase<OtherDerived> &other )
	    {
		Tvec::operator=(other);
		return *this;
	    }

	/** @brief Zero data */
	void clear() { setZero(); }

	/**
	 * @brief Convert into random unit vector
	 *
	 * Based on the von Neumann method described in
	 * *Allen and Tildesley*, page 349, which ensures
	 * a uniform distribution on a unit sphere. More information
	 * about *Sphere Point Picking* can be found at
	 * [MathWorld](http://mathworld.wolfram.com/SpherePointPicking.html).
	 *
	 * @param ran Function that takes no arguments and returns a random
	 *            float uniformly distributed in the range `[0:1[`.
	 * @param dir Directions. Sphere=1,1,1 (default), XY circle=1,1,0 etc.
	 */
	template<class Trandombase>
	    PointBase &ranunit( Trandombase &ran, const PointBase& dir={1,1,1}  )
	    {
		assert( dir.size() == 3 );
		Tcoord r2;
		do
		{
		    for ( size_t i = 0; i < 3; ++i )
			operator[](i) = (2 * ran() - 1) * dir[i];
		    r2 = squaredNorm();
		}
		while ( r2 > 1 );
		*this = *this / std::sqrt(r2);
		assert(norm() - 1 < 1e-7); // is it really a unit vector?
		return *this;
	    }

	/**
	 * @brief Translate along a vector
	 * @param geo Geometry to use for boundary conditions (see Faunus::Geometry)
	 * @param a Vector to translate with
	 */
	template<typename Tgeometry>
	    void translate( const Tgeometry &geo, const PointBase &a )
	    {
		(*this) += a;
		geo.boundary(*this);
	    }

	/**
	 * @brief Coordinate scaling used for NPT and NVT ensemble
	 *
	 * This will perform a volume scaling of the Point by
	 * following the algorithm specified in the Geometry.
	 */
	template<typename Tgeometry>
	    void scale( const Tgeometry &geo, PointBase &s, double xyz = 1, double xy = 1 )
	    {
		geo.scale(*this, s, xyz, xy);
	    }


	/**
	 * @brief Convert cartesian- to spherical-coordinates
	 * @param origin The origin to be used (optional)
	 *
	 * @note Input (x,y,z), output \f$ (r,\theta,\phi) \f$  where \f$ r\in [0,\infty) \f$, \f$ \theta\in [-\pi,\pi) \f$, and \f$ \phi\in [0,\pi] \f$.
	 */
	PointBase xyz2rtp(PointBase origin=PointBase(0,0,0)) {
	    PointBase xyz_t = *this - origin;
	    double radius = xyz_t.norm();
	    PointBase rtp(radius,0.0,0.0);
	    rtp.y() = std::atan2(xyz_t.y(),xyz_t.x());
	    rtp.z() = std::acos(xyz_t.z()/radius);
	    return rtp;
	}

	/**
	 * @brief Convert spherical- to cartesian-coordinates
	 * @param origin The origin to be added (optional)
	 *
	 * @note Input \f$ (r,\theta,\phi) \f$  where \f$ r\in [0,\infty) \f$, \f$ \theta\in [0,2\pi) \f$, and \f$ \phi\in [0,\pi] \f$, and output (x,y,z).
	 */
	PointBase rtp2xyz(PointBase origin=PointBase(0,0,0)) {
	    PointBase rtp_t = *this;
	    PointBase xyz(0,0,0);
	    xyz.x() = rtp_t.x()*std::cos(rtp_t.y())*std::sin(rtp_t.z());
	    xyz.y() = rtp_t.x()*std::sin(rtp_t.y())*std::sin(rtp_t.z());
	    xyz.z() = rtp_t.x()*std::cos(rtp_t.z());
	    xyz += origin;
	    return xyz;
	}

	Tcoord len() const
	{
	    return norm();
	}

	/** @brief Read from stream */
	PointBase &operator<<( std::istream &in )
	{
	    for ( int i = 0; i < size(); ++i )
		in >> (*this)[i];
	    return *this;
	}

	/** @brief Read from string */
	PointBase &operator<<( const std::string &in )
	{
	    std::istringstream i(in);
	    return operator<<(i);
	}

	/*
	 * @brief Internal rotation. No effect on isotropic particle.
	 */
	template<typename Trotator>
	    void rotate( const Trotator &rot )
	    {
	    }
	    
	/*
	 * @brief Reflect vectors. No effect on isotropic particle.
	 */
	    void reflect( const PointBase &n )
	    {
	    }

	/** @brief Fill from std vector */
	template<class T>
	    PointBase &operator<<( const std::vector<T> &v )
	    {
		// see http://stackoverflow.com/questions/26094379/typecasting-eigenvectorxd-to-stdvector
		assert(v.size() == 3);
		for ( size_t i = 0; i < 3; ++i )
		    (*this)[i] = v.at(i);
		return *this;
	    }

	operator std::vector<double>() const
	{
	    return Eigen2Vector(*this);
	    //std::vector<double> v;
	    //v.resize( size() );
	    //Tvec::Map(&v[0], size()) = *this;
	    //return v;
	}
    };

    template<class T=double>
	struct Tensor : public Eigen::Matrix<T, 3, 3>
    {
	typedef Eigen::Matrix<T, 3, 3> Tmat; //!< Matrix from Eigen

	/** @brief Default constructor. Data is zeroed */
	Tensor()
	{
	    (*this).setZero();
	}

	Tensor( T xx, T xy, T xz, T yy, T yz, T zz )
	{
	    (*this) << xx, xy, xz, xy, yy, yz, xz, yz, zz;
	}

	template<typename OtherDerived>
	    Tensor( const Eigen::MatrixBase<OtherDerived> &other ) : Tmat(other) {}

	template<typename OtherDerived>
	    Tensor &operator=( const Eigen::MatrixBase<OtherDerived> &other )
	    {
		Tmat::operator=(other);
		return *this;
	    }

	/** @brief Read from stream */
	Tensor &operator<<( std::istream &in )
	{
	    (*this).setZero();
	    for ( int i = 0; i < (*this).rows(); i++ )
	    {
		for ( int j = i; j < (*this).cols(); j++ )
		{
		    in >> (*this)(i, j);
		    (*this)(j, i) = (*this)(i, j);
		}
	    }
	    return *this;
	}

	/** @brief Read from string */
	Tensor &operator<<( const std::string &in )
	{
	    std::istringstream i(in);
	    return operator<<(i);
	}

	/** @brief Write data members to stream */
	friend std::ostream &operator<<( std::ostream &o, const Tensor<T> &t )
	{
	    for ( int i = 0; i != t.rows(); ++i )
		for ( int j = i; j != t.cols(); ++j )
		    o << t(i, j) << " ";
	    return o;
	}

	template<typename Trotator>
	    void rotate( const Trotator &rot ) { *this = rot(*this); }

	void eye() { *this = Tmat::Identity(3, 3); }
    };

    /**
     * @brief Hypersphere particle
     * @date Lund, 2009-2013
     * @warning Unfinished - need to transfer from jurassic branch
     */
    class HyperPoint : public PointBase
    {
	private:
	    double w;

	public:
	    inline HyperPoint() {}

	    inline HyperPoint( double z1, double z2, double z3, double w ) : PointBase(z1, z2, z3)
	{
	    z4() = w;
	}

	    inline HyperPoint( double z1, double z2, double z3 ) : PointBase(z1, z2, z3)
	{
	    z4() = 1;
	}

	    template<typename OtherDerived>
		HyperPoint( const Eigen::MatrixBase<OtherDerived> &other ) : PointBase(other) {}

	    template<typename OtherDerived>
		HyperPoint &operator=( const Eigen::MatrixBase<OtherDerived> &other )
		{
		    PointBase::operator=(other);
		    return *this;
		}

	    inline const double &z1() const { return x(); }

	    inline const double &z2() const { return y(); }

	    inline const double &z3() const { return z(); }

	    inline const double &z4() const { return w; }

	    inline double &z1() { return x(); }

	    inline double &z2() { return y(); }

	    inline double &z3() { return z(); }

	    inline double &z4() { return w; }

	    /** @brief Read from stream */
	    HyperPoint &operator<<( std::istream &in )
	    {
		PointBase::operator<<(in);
		in >> z4();
		return *this;
	    }

	    /** @brief Write to stream */
	    friend std::ostream &operator<<( std::ostream &o, const HyperPoint &p )
	    {
		o << PointBase(p) << " " << p.z4();
		return o;
	    }

	    /**
	     * @brief Translate along a vector
	     * @param geo Geometry to use for boundary conditions (see Faunus::Geometry) 
	     * @param a Vector to translate with
	     */
	    template<typename Tgeometry>
		void translate( const Tgeometry &geo, const HyperPoint &a )
		{
		    (*this) += a;
		    geo.boundary(*this);
		}

	    /**
	     * @brief Coordinate scaling used for NPT ensemble
	     *
	     * This will perform a volume scaling of the Point by
	     * following the algorithm specified in the Geometry.
	     */
	    template<typename Tgeometry>
		void scale( const Tgeometry &geo, double newvol )
		{
		    geo.scale(*this, newvol);
		}

	    inline double sqdist( const HyperPoint &a ) const
	    {
		return dot(a) + z4() * a.z4();
	    }

	    /**
	     * @brief Geodesic distance between two hyperpoints
	     * @return @f[ r_g = \arccos{ (r^2) } @f]
	     */
	    inline double geodesic( const HyperPoint &a ) const
	    {
		return std::acos(sqdist(a));
	    }

	    void move( double du, double dv, double dw )
	    {
		double nz1, nz2, nz3, nz4,
		       tz1, tz2, tz3, tz4,
		       rho = du, omega = dv, fi = dw;
		nz1 = std::sqrt(1. - rho * rho);
		nz2 = nz1 * std::cos(fi);
		nz1 = nz1 * std::sin(fi);
		nz3 = rho * std::sin(omega);
		nz4 = rho * std::cos(omega);

		HyperPoint e1, e2, e3, te1, te2, te3;
		double fact1, fact2, fact3, nabla_nb, fi_nb;

		nabla_nb = 0;//slp.random_one()*2.*acos(-1.);
		fi_nb = 0;//std::acos(slp.random_one());

		e1.z1() = std::cos(nabla_nb);
		e1.z2() = std::sin(nabla_nb);
		e1.z3() = 0;
		e1.z4() = 0;
		e2.z1() = -std::cos(fi_nb) * std::sin(nabla_nb);
		e2.z2() = std::cos(fi_nb) * std::cos(nabla_nb);
		e2.z3() = std::sin(fi_nb);
		e2.z4() = 0;
		e3.z1() = std::sin(fi_nb) * std::sin(nabla_nb);
		e3.z2() = -std::sin(fi_nb) * std::cos(nabla_nb);
		e3.z3() = std::cos(fi_nb);
		e3.z4() = 0;

		// First create a random orthonormal basis set at North Pole
		fact1 = e1.z1() * z1() + e1.z2() * z2() + e1.z3() * z3();
		te1.z1() = e1.z1() - 1. / (1. + z4()) * fact1 * z1();
		te1.z2() = e1.z2() - 1. / (1. + z4()) * fact1 * z2();
		te1.z3() = e1.z3() - 1. / (1. + z4()) * fact1 * z3();
		te1.z4() = e1.z4() - 1. / (1. + z4()) * fact1 * (z4() + 1.);

		fact2 = e2.z1() * z1() + e2.z2() * z2() + e2.z3() * z3();
		te2.z1() = e2.z1() - 1. / (1. + z4()) * fact2 * z1();
		te2.z2() = e2.z2() - 1. / (1. + z4()) * fact2 * z2();
		te2.z3() = e2.z3() - 1. / (1. + z4()) * fact2 * z3();
		te2.z4() = e2.z4() - 1. / (1. + z4()) * fact2 * (z4() + 1.);

		fact3 = e3.z1() * z1() + e3.z2() * z2() + e3.z3() * z3();
		te3.z1() = e3.z1() - 1. / (1. + z4()) * fact3 * z1();
		te3.z2() = e3.z2() - 1. / (1. + z4()) * fact3 * z2();
		te3.z3() = e3.z3() - 1. / (1. + z4()) * fact3 * z3();
		te3.z4() = e3.z4() - 1. / (1. + z4()) * fact3 * (z4() + 1.);

		// Then move it to point of z1,z2,z3,z4
		tz1 = nz1 * te1.z1() + nz2 * te2.z1() + nz3 * te3.z1() + nz4 * z1();
		tz2 = nz1 * te1.z2() + nz2 * te2.z2() + nz3 * te3.z2() + nz4 * z2();
		tz3 = nz1 * te1.z3() + nz2 * te2.z3() + nz3 * te3.z3() + nz4 * z3();
		tz4 = nz1 * te1.z4() + nz2 * te2.z4() + nz3 * te3.z4() + nz4 * z4();

		// Update the point
		z1() = tz1;
		z2() = tz2;
		z3() = tz3;
		z4() = tz4;
	    }
    };

#ifdef FAU_HYPERSPHERE
    typedef HyperPoint Point;
#else
    typedef PointBase Point;  //!< 3D vector
#endif

    /**
     * @brief Class for isotropic particles
     *
     * Example:
     *
     * ~~~
     * std::vector<PointParticle> p(2);
     * p[0].radius = 2.0;
     * p[1].z() = 10;
     * std::cout << p[0];
     * ~~~
     *
     */
    struct PointParticle : public Point
    {
	typedef Point::Tcoord Tradius;
	typedef Point::Tcoord Tcharge;
	typedef Point::Tcoord Tmw;
	typedef Point::Tcoord Talphax;
	typedef unsigned char Tid;
	typedef bool Thydrophobic;
	Tid id;                                   //!< Particle identifier
	Tcharge charge;                           //!< Charge number
	Tradius radius;                           //!< Radius
	Talphax alphax;
	Tmw mw;                                   //!< Molecular weight
	Thydrophobic hydrophobic;                 //!< Hydrophobic flag

	Tensor<double> zeroT;
	Point zeroP;
	double zeroD;
	bool trueD;

	PointParticle() { clear(); }              //!< Constructor

	template<typename OtherDerived>
	    PointParticle( const Eigen::MatrixBase<OtherDerived> &other ) : Point(other) {}

	template<typename OtherDerived>
	    PointParticle &operator=( const Eigen::MatrixBase<OtherDerived> &other )
	    {
		Point::operator=(other);
		return *this;
	    }

	Tcharge &q() { return charge; }

	Tcharge q() const { return charge; }

	Point mu() const { return zeroP; }
	Point mup() const { return zeroP; }
	double muscalar() const { return zeroD; }
	Point& mu() { return zeroP; }
	Point& mup() { return zeroP; }
	double& muscalar() { return zeroD; }

	Point cap_center_point() const { return zeroP; } 
	Point charge_position() const { return zeroP; }
	double cap_radius() const { return zeroD; }
	double cap_center() const { return zeroD; }
	double angle_p() const { return zeroD; }
	double angle_c() const { return zeroD; }
	bool is_sphere() const { return trueD; }
	Point& cap_center_point() { return zeroP; } 
	Point& charge_position() { return zeroP; }
	double& cap_radius() { return zeroD; }
	double& cap_center() { return zeroD; }
	double& angle_p() { return zeroD; }
	double& angle_c() { return zeroD; }
	bool& is_sphere() { return trueD; }
	Tensor<double> alpha() const { return zeroT; }
	Tensor<double> theta() const { return zeroT; }
	
	Point lv() const { return zeroP; } 
	Point wv() const { return zeroP; } 
	Point dv() const { return zeroP; } 
	double length() const { return zeroD; }
	double width() const { return zeroD; }
	double depth() const { return zeroD; }
	Point& lv() { return zeroP; } 
	Point& wv() { return zeroP; }
	Point& dv() { return zeroP; }
	double& length() { return zeroD; }
	double& width() { return zeroD; }
	double& depth() { return zeroD; }

	template<class T,
	    class = typename std::enable_if<std::is_base_of<AtomData, T>::value>::type>
		PointParticle &operator=( const T &d )
		{
		    id = d.id;
		    charge = d.charge;
		    radius = d.radius;
		    alphax = d.alphax;
		    mw = d.mw;
		    hydrophobic = d.hydrophobic;
		    return *this;
		}

	/**
	 * @brief Copy from stream
	 *
	 * This will read all data from stream in the same order as written.
	 * Note that a short integer is expected for the particle id
	 * since chars (Tid=char) does not print well on screen.
	 * Derived classes should expand on this so
	 * that *all* data is read.
	 */
	PointParticle &operator<<( std::istream &in )
	{
	    short tmp; // avoid char output in readable text files
	    Point::operator<<(in);
	    in >> charge >> radius >> mw >> tmp >> hydrophobic;
	    id = (Tid) tmp;
	    return *this;
	}

	/**
	 * @brief Write to stream
	 *
	 * This will write all data to given stream. Note that the particle id is converted
	 * to a short integer since char output (Tid=char) does not print well on screen.
	 * Derived classes should expand on this so that *all* data is written.
	 */
	friend std::ostream &operator<<( std::ostream &o, const PointParticle &p )
	{
	    o << Point(p).transpose()
		<< " " << p.charge << " " << p.radius << " " << p.mw << " "
		<< (short) p.id << " " << p.hydrophobic;
	    return o;
	}

	/** @brief Volume of particle */
	double volume() const
	{
	    return 4 * std::acos(-1) * radius * radius * radius / 3;
	}

	/** @brief Zero data */
	void clear()
	{
	    Point::clear();
	    charge = mw = radius = alphax = 0;
	    hydrophobic = false;
	    id = 0;
	    zeroP = Point(0,0,0);
	    zeroD = 0.0;
	    trueD = true;
	}

    };

    /**
     * @brief Dipolar particle
     */
    struct DipoleParticle : public PointParticle
    {
	private:
	    Point _mu;        //!< Dipole moment unit vector (permanent+induced)
	    Point _mup;       //!< Permanent dipole moment vector
	    double _muscalar; //!< Dipole moment scalar (permanent+induced)
	    Tensor<double> _alpha;   //!< Polarization matrix
	    Tensor<double> _theta;   //!< Quadrupole matrix

	public:
	    Point mu() const { return _mu; } 
	    Point mup() const { return _mup; }
	    double muscalar() const { return _muscalar; }
	    Tensor<double> alpha() const { return _alpha; }
	    Tensor<double> theta() const { return _theta; }
	    Point& mu() { return _mu; } 
	    Point& mup() { return _mup; }
	    double& muscalar() { return _muscalar; }
	    Tensor<double>& alpha() { return _alpha; }
	    Tensor<double>& theta() { return _theta; }


	    inline DipoleParticle() : _mu(0, 0, 0), _muscalar(0), _mup(0, 0, 0) {};

	    /** @brief Copy constructor for Eigen derivatives */
	    template<typename OtherDerived>
		DipoleParticle( const Eigen::MatrixBase<OtherDerived> &other ) : PointParticle(other) {}

	    /** @brief Generic copy operator for Eigen derivatives */
	    template<typename OtherDerived>
		DipoleParticle &operator=( const Eigen::MatrixBase<OtherDerived> &other )
		{
		    PointParticle::operator=(other);
		    return *this;
		}

	    /** @brief Copy operator for base class (i.e no casting to Eigen derivatives) */
	    inline DipoleParticle &operator=( const PointParticle &p )
	    {
		PointParticle::operator=(p);
		return *this;
	    }

	    /** @brief Copy properties from AtomData object */
	    template<class T,
		class = typename std::enable_if<std::is_base_of<AtomData, T>::value>::type>
		    DipoleParticle &operator=( const T &d )
		    {
			PointParticle::operator=(d);
			_muscalar = d.muscalar;
			_mu = d.mu;
			_mup = _mu * _muscalar;
			_alpha = d.alpha;
			_theta = d.theta;
			return *this;
		    }

	    /* read in same order as written! */
	    inline DipoleParticle &operator<<( std::istream &in )
	    {
		PointParticle::operator<<(in);
		_mu.operator<<(in);
		in >> _muscalar;
		_mup.operator<<(in);
		_alpha.operator<<(in);
		_theta.operator<<(in);
		return *this;
	    }

	    /* write data members to stream */
	    friend std::ostream &operator<<( std::ostream &o, const DipoleParticle &p )
	    {
		o << PointParticle(p) << " " << p._mu.transpose() << " " << p._muscalar
		    << " " << p._mup << " " << p._alpha << " " << p._theta;
		return o;
	    }

	    /**
	     * @brief Internal rotation: dipole and polarizability
	     */
	    template<typename Trotator>
		void rotate( const Trotator &rot )
		{
		    assert(rot.getOrigin().squaredNorm() < 1e-6);
		    mu() = rot(mu());
		    mup() = rot(mup());
		    _alpha = rot(_alpha);
		    _theta = rot(_theta);
		}
    };

    /**
     * @brief Sphero-cylindrical particle
     * @date Brno, November 2012
     *
     * detailed information here...
     */
    class CigarParticle : public PointParticle
    {
	public:
	    Point dir; //!< Direction of sphero cylinder (unit vector)
	    Point patchdir;
	    Point patchsides[2];
	    Point chdir;
	    double patchangle;
	    double pcanglsw;
	    double pcangl;
	    double halfl;
	    double chiral_angle, panglsw, pangl;  // are these needed? From AtomData

	    inline CigarParticle() : halfl(0) {}

	    /** @brief Copy constructor for Eigen derivatives */
	    template<typename OtherDerived>
		CigarParticle( const Eigen::MatrixBase<OtherDerived> &other ) : PointParticle(other) {}

	    /**
	     * @brief Initialize patchy spherocylinder - run at start and after patch changes
	     *
	     * Calculates cosine of angles, patch direction including chirality
	     * and vector corresponding to sides of patch that are used in
	     * calculations of interactions. 
	     * This function must be called at the beginning of calculations and after changes
	     * of patch properties.
	     * It shall be also after a lot of move to remove accumulated comouptation errors
	     */
	    inline void init()
	    {
		const double zero = 1e-9;
		if ( halfl > zero )
		{
		    Point vec;
		    Eigen::Quaterniond Q;
		    pcangl = std::cos(0.5 * patchangle);
		    pcanglsw = std::cos(0.5 * patchangle + panglsw);

		    if ( dir.squaredNorm() < zero )
			dir = {1, 0, 0};

		    if ( patchdir.squaredNorm() < zero )
			patchdir = {0, 1, 0};

		    dir.normalize();

		    patchdir = patchdir - dir * patchdir.dot(dir); // perp. project
		    patchdir.normalize();

		    /* calculate patch sides */
		    if ( chiral_angle < zero )
			vec = dir;
		    else
		    {
			chdir = dir;
			Q = Eigen::AngleAxisd(0.5 * chiral_angle, patchdir);
			chdir = Q * chdir; // rotate
			vec = chdir;
		    }

		    /* create side vector by rotating patch vector by half size of patch*/
		    /* the first side */
		    patchsides[0] = patchdir;
		    Q = Eigen::AngleAxisd(0.5 * pangl + panglsw, vec);
		    patchsides[0] = Q * patchsides[0]; // rotate
		    patchsides[0].normalize();

		    /* the second side */
		    patchsides[1] = patchdir;
		    Q = Eigen::AngleAxisd(-0.5 * pangl - panglsw, vec);
		    patchsides[1] = Q * patchsides[1]; // rotate
		    patchsides[1].normalize();

		    if ( patchsides[0].squaredNorm() < zero )
			throw std::runtime_error("Patch side vector has zero size.");
		}
	    }

	    /** @brief Generic copy operator for Eigen derivatives */
	    template<typename OtherDerived>
		CigarParticle &operator=( const Eigen::MatrixBase<OtherDerived> &other )
		{
		    PointParticle::operator=(other);
		    return *this;
		}

	    /** @brief Copy operator for base class (i.e no casting to Eigen derivatives) */
	    inline CigarParticle &operator=( const PointParticle &p )
	    {
		PointParticle::operator=(p);
		return *this;
	    }

	    /** @brief Copy properties from AtomData object */
	    template<class T,
		class = typename std::enable_if<std::is_base_of<AtomData, T>::value>::type>
		    CigarParticle &operator=( const T &d )
		    {
			PointParticle::operator=(d);
			halfl = d.half_len;
			patchangle = d.pangl;
			pcanglsw = std::cos(0.5 * d.pangl + d.panglsw);
			pcangl = std::cos(0.5 * d.pangl);
			chiral_angle = d.chiral_angle;
			pangl = d.pangl;
			panglsw = d.panglsw;
			init();
			return *this;
		    }

	    template<typename Trotator>
		void rotate( const Trotator &rot )
		{
		    if ( halfl > 1e-6 )
		    {
			assert(rot.getOrigin().squaredNorm() < 1e-6);
			dir = rot(dir);
			patchdir = rot(patchdir);
			patchsides[0] = rot(patchsides[0]);
			patchsides[1] = rot(patchsides[1]);
			chdir = rot(chdir);
		    }
		}

	    /* read in same order as written! */
	    inline CigarParticle &operator<<( std::istream &in )
	    {
		PointParticle::operator<<(in);
		dir.operator<<(in);
		patchdir.operator<<(in);
		in >> halfl;
		return *this;
	    }

	    /* write data members to stream */
	    friend std::ostream &operator<<( std::ostream &o, const CigarParticle &p )
	    {
		o << PointParticle(p)
		    << " " << p.dir.transpose() << " " << p.patchdir.transpose()
		    << " " << p.halfl;
		return o;
	    }
    };

    /**
     * @brief Cap particle
     */
    struct CapParticle : public PointParticle {
	private:
	    Point _cap_center_point;               //!< Normalized center of cap
	    Point _charge_position;                //!< Position of charge
	    double _cap_radius;                    //!< Radius of cap
	    double _cap_center;                    //!< Distance between particle and cap center
	    double _angle_p;		           //!< angle_p is the azimuth angle of the spherical back segment of the particles surface
	    double _angle_c; 			   //!< angle_c is the azimuth angle of the spherical front segment of the particles surface
	    double _area;                          //!< Surface-area of the particle
	    double _mass_center;                   //!< Mass center in the direction of '_cap_center_point'
	    bool _is_sphere;

	public:
	    Point cap_center_point() const { return _cap_center_point; } 
	    Point charge_position() const { return _charge_position; }
	    double cap_radius() const { return _cap_radius; }
	    double cap_center() const { return _cap_center; }
	    double angle_p() const { return _angle_p; }
	    double angle_c() const { return _angle_c; }
	    double area() const { return _area; }
	    double mass_center() const { return _mass_center; }
	    bool is_sphere() const { return _is_sphere; }
	    Point& cap_center_point() { return _cap_center_point; } 
	    Point& charge_position() { return _charge_position; }
	    double& cap_radius() { return _cap_radius; }
	    double& cap_center() { return _cap_center; }
	    double& angle_p() { return _angle_p; }
	    double& angle_c() { return _angle_c; }
	    double& area() { return _area; }
	    bool& is_sphere() { return _is_sphere; }

	    inline CapParticle() : _cap_center_point(1,0,0),_charge_position(0,0,0),_cap_radius(0),_cap_center(0),_angle_p(0),_angle_c(0),_is_sphere(true) {};

	    /** @brief Copy constructor for Eigen derivatives */
	    template<typename OtherDerived>
		CapParticle(const Eigen::MatrixBase<OtherDerived>& other) : PointParticle(other) {}

	    /** @brief Generic copy operator for Eigen derivatives */
	    template<typename OtherDerived>
		CapParticle& operator=(const Eigen::MatrixBase<OtherDerived> &other) {
		    PointParticle::operator=(other);
		    return *this;
		}

	    /** @brief Copy operator for base class (i.e no casting to Eigen derivatives) */
	    inline CapParticle& operator=(const PointParticle &p) {
		PointParticle::operator=(p);
		return *this;
	    }

	    /** @brief Copy properties from AtomData object */
	    template<class T,
		class = typename std::enable_if<std::is_base_of<AtomData,T>::value>::type>
		    CapParticle& operator=(const T &d) {
			PointParticle::operator=(d);
			_cap_center=d.cap_center;
			_cap_center_point = Point(1,0,0);
			_charge_position = Point(d.charge_position,0,0);
			_cap_radius=d.cap_radius;
			if(_cap_radius > 1e-6)
			    _is_sphere = false;
			if( ( _cap_radius > 1e-6 ) && ( _cap_center > 1e-6) && ( radius > 1e-6 ) ) {
			    _angle_p = std::acos(-1.) - std::acos((radius*radius + _cap_center*_cap_center - _cap_radius*_cap_radius)/(2.0*radius*_cap_center));
			    _angle_c = std::acos((_cap_center*_cap_center + _cap_radius*_cap_radius - radius*radius)/(2.0*_cap_center*_cap_radius));
			}
			_area = std::acos(-1.)/_cap_center*(radius*radius*radius + (_cap_radius+2.0*_cap_center)*radius*radius - (_cap_radius*_cap_radius-_cap_center*_cap_center)*radius-(_cap_radius-_cap_center)*(_cap_radius-_cap_center)*_cap_radius);
			_mass_center = ((_cap_radius * _cap_radius + 4 * _cap_radius * _cap_center - radius * radius + _cap_center * _cap_center) * (-_cap_radius + _cap_center - radius) * (-_cap_radius + _cap_center - radius) / (_cap_center * _cap_center + (2 * _cap_radius - 2 * radius) * _cap_center - 3 * (radius + _cap_radius) * (radius + _cap_radius) ) / _cap_center) / 2.0;
			return *this;
		    }

	    /* read in same order as written! */
	    inline CapParticle& operator<<(std::istream &in) {
		PointParticle::operator<<(in);
		_cap_center_point.operator<<(in);
		_charge_position.operator<<(in);
		in >> _cap_radius;
		if(_cap_radius > 1e-6)
		    _is_sphere = false;
		in >> _cap_center;
		if( ( _cap_radius > 1e-6 ) && (_cap_center > 1e-6) && ( radius > 1e-6 ) ) {
		    _angle_p = std::acos(-1.) - std::acos((radius*radius + _cap_center*_cap_center - _cap_radius*_cap_radius)/(2.0*radius*_cap_center));
		    _angle_c = std::acos((_cap_center*_cap_center + _cap_radius*_cap_radius - radius*radius)/(2.0*_cap_center*_cap_radius));
		}
		_area = std::acos(-1.)/_cap_center*(radius*radius*radius + (_cap_radius+2.0*_cap_center)*radius*radius - (_cap_radius*_cap_radius-_cap_center*_cap_center)*radius-(_cap_radius-_cap_center)*(_cap_radius-_cap_center)*_cap_radius);
		_mass_center = ((_cap_radius * _cap_radius + 4 * _cap_radius * _cap_center - radius * radius + _cap_center * _cap_center) * (-_cap_radius + _cap_center - radius) * (-_cap_radius + _cap_center - radius) / (_cap_center * _cap_center + (2 * _cap_radius - 2 * radius) * _cap_center - 3 * (radius + _cap_radius) * (radius + _cap_radius) ) / _cap_center) / 2.0;
		return *this;
	    }

	    // remove later when not used!!!
	    void update() {
		if(!_is_sphere) {
		    if( ( _cap_radius > 1e-6 ) && ( _cap_center > 1e-6) && ( radius > 1e-6 ) ) {
			_angle_p = std::acos(-1.) - std::acos((radius*radius + _cap_center*_cap_center - _cap_radius*_cap_radius)/(2.0*radius*_cap_center));
			_angle_c = std::acos((_cap_center*_cap_center + _cap_radius*_cap_radius - radius*radius)/(2.0*_cap_center*_cap_radius));
		    } else {
			_angle_p = std::acos(-1.);
			_angle_c = 0.0;
		    }
		    _area = std::acos(-1.)/_cap_center*(radius*radius*radius + (_cap_radius+2.0*_cap_center)*radius*radius - (_cap_radius*_cap_radius-_cap_center*_cap_center)*radius-(_cap_radius-_cap_center)*(_cap_radius-_cap_center)*_cap_radius);
		    _mass_center = ((_cap_radius * _cap_radius + 4 * _cap_radius * _cap_center - radius * radius + _cap_center * _cap_center) * (-_cap_radius + _cap_center - radius) * (-_cap_radius + _cap_center - radius) / (_cap_center * _cap_center + (2 * _cap_radius - 2 * radius) * _cap_center - 3 * (radius + _cap_radius) * (radius + _cap_radius) ) / _cap_center) / 2.0;
		} else {
		    _angle_p = std::acos(-1.);
		    _angle_c = 0.0;
		    _area = 4.0*std::acos(-1.)*radius*radius;
		    _mass_center = 0.0;
		}
	    }

	    /* write data members to stream */
	    friend std::ostream &operator<<(std::ostream &o, const CapParticle &p)
	    {
		o << PointParticle(p) << " " << p._cap_center_point.transpose() << " " << p._charge_position.transpose() << " " << p._cap_radius << " " << p._cap_center;
		return o;
	    }

	    /**
	     * @brief Internal rotation: center-to-cap-vector
	     */
	    template<typename Trotator>
		void rotate(const Trotator &rot) {
		    assert(rot.getOrigin().squaredNorm()<1e-6);
		    assert(fabs(cap_center_point().norm()-1.0) < 1e-6);
		    cap_center_point() = rot(cap_center_point());
		    charge_position() = rot(charge_position());
		}
    };














    /**
     * @brief Cap particle
     */
    struct CapParticle2 : public PointParticle {
	private:
	    std::vector<Point> _cap_center_point;               //!< Normalized center of cap
	    std::vector<Point> _charge_position;                //!< Position of charge
	    std::vector<double> _cap_radius;                    //!< Radius of cap
	    std::vector<double> _cap_center;                    //!< Distance between particle and cap center
	    std::vector<double> _angle_p;		           //!< angle_p is the azimuth angle of the spherical back segment of the particles surface
	    std::vector<double> _angle_c; 			   //!< angle_c is the azimuth angle of the spherical front segment of the particles surface
	    double _area;                          //!< Surface-area of the particle
	    double _mass_center;                   //!< Mass center in the direction of '_cap_center_point'
	    bool _is_sphere;
	    int _nbrOfCav;                                 //!< Number of cavities

	public:
	    Point cap_center_point(int I=0) const { return _cap_center_point.at(I); } 
	    Point charge_position(int I=0) const { return _charge_position.at(I); }
	    double cap_radius(int I=0) const { return _cap_radius.at(I); }
	    double cap_center(int I=0) const { return _cap_center.at(I); }
	    double angle_p(int I=0) const { return _angle_p.at(I); }
	    double angle_c(int I=0) const { return _angle_c.at(I); }
	    double area() const { return _area; }
	    double mass_center() const { return _mass_center; }
	    bool is_sphere() const { return _is_sphere; }
	    int nbrOfCav() const { return _nbrOfCav; }
	    Point& cap_center_point(int I=0) { return _cap_center_point.at(I); } 
	    Point& charge_position(int I=0) { return _charge_position.at(I); }
	    double& cap_radius(int I=0) { return _cap_radius.at(I); }
	    double& cap_center(int I=0) { return _cap_center.at(I); }
	    double& angle_p(int I=0) { return _angle_p.at(I); }
	    double& angle_c(int I=0) { return _angle_c.at(I); }
	    double& area() { return _area; }
	    bool& is_sphere() { return _is_sphere; }
	    double charge_density;

	    inline CapParticle2() {
		_cap_center_point.resize(0);
		_charge_position.resize(0);
		_cap_radius.resize(0);
		_cap_center.resize(0);
		_angle_p.resize(0);
		_angle_c.resize(0);
		_nbrOfCav = 0;
		_is_sphere = true;
	    };

	    /** @brief Copy constructor for Eigen derivatives */
	    template<typename OtherDerived>
		CapParticle2(const Eigen::MatrixBase<OtherDerived>& other) : PointParticle(other) {}

	    /** @brief Generic copy operator for Eigen derivatives */
	    template<typename OtherDerived>
		CapParticle2& operator=(const Eigen::MatrixBase<OtherDerived> &other) {
		    PointParticle::operator=(other);
		    return *this;
		}

	    /** @brief Copy operator for base class (i.e no casting to Eigen derivatives) */
	    inline CapParticle2& operator=(const PointParticle &p) {
		PointParticle::operator=(p);
		return *this;
	    }

	    /** @brief Copy properties from AtomData object */
	    template<class T,
		class = typename std::enable_if<std::is_base_of<AtomData,T>::value>::type>
		    CapParticle2& operator=(const T &d) {
			PointParticle::operator=(d);


			_nbrOfCav = d.cap_radiusV.size();
			_cap_center_point.resize(_nbrOfCav);
			_charge_position.resize(_nbrOfCav);
			_cap_radius.resize(_nbrOfCav);
			_cap_center.resize(_nbrOfCav);
			_angle_p.resize(_nbrOfCav);
			_angle_c.resize(_nbrOfCav);
			if(_nbrOfCav > 0)
			    _is_sphere = false;
			_area = 4.0*acos(-1.0)*radius*radius;
			charge_density = d.charge_density;
			for(int I = 0; I < _nbrOfCav; I++) {
			    _cap_center.at(I)=d.cap_centerV.at(I);
			    _cap_center_point.at(I) = d.cav_pos.at(I);
			    _cap_center_point.at(I) = _cap_center_point.at(I)/_cap_center_point.at(I).norm();
			    _charge_position.at(I) = Point(d.charge_position,0,0);
			    _cap_radius.at(I)=d.cap_radiusV.at(I);
			    if(_cap_radius.at(I) > 1e-6)
				_is_sphere = false;
			    if( ( _cap_radius.at(I) > 1e-6 ) && ( _cap_center.at(I) > 1e-6) && ( radius > 1e-6 ) ) {
				_angle_p.at(I) = std::acos(-1.) - std::acos((radius*radius + _cap_center.at(I)*_cap_center.at(I) - _cap_radius.at(I)*_cap_radius.at(I))/(2.0*radius*_cap_center.at(I)));
				_angle_c.at(I) = std::acos((_cap_center.at(I)*_cap_center.at(I) + _cap_radius.at(I)*_cap_radius.at(I) - radius*radius)/(2.0*_cap_center.at(I)*_cap_radius.at(I)));
			    }
			    _area = _area + std::acos(-1.)/_cap_center.at(I)*(_cap_center.at(I)-radius-_cap_radius.at(I))*(_cap_center.at(I)-radius-_cap_radius.at(I))*(_cap_radius.at(I) - radius);
			    _mass_center = ((_cap_radius.at(I) * _cap_radius.at(I) + 4 * _cap_radius.at(I) * _cap_center.at(I) - radius * radius + _cap_center.at(I) * _cap_center.at(I)) * (-_cap_radius.at(I) + _cap_center.at(I) - radius) * (-_cap_radius.at(I) + _cap_center.at(I) - radius) / (_cap_center.at(I) * _cap_center.at(I) + (2 * _cap_radius.at(I) - 2 * radius) * _cap_center.at(I) - 3 * (radius + _cap_radius.at(I)) * (radius + _cap_radius.at(I)) ) / _cap_center.at(I)) / 2.0;
			}
			charge = charge_density*_area;
			return *this;
		    }

	    /* read in same order as written! */
	    inline CapParticle2& operator<<(std::istream &in) {
		PointParticle::operator<<(in);
		_area = 4.0*acos(-1.0)*radius*radius;
		for(int I = 0; I < _nbrOfCav; I++) {
		    _cap_center_point.at(I).operator<<(in);
		    _charge_position.at(I).operator<<(in);
		    in >> _cap_radius.at(I);
		    if(_cap_radius.at(I) > 1e-6)
			_is_sphere = false;
		    in >> _cap_center.at(I);
		    if( ( _cap_radius.at(I) > 1e-6 ) && (_cap_center.at(I) > 1e-6) && ( radius > 1e-6 ) ) {
			_angle_p.at(I) = std::acos(-1.) - std::acos((radius*radius + _cap_center.at(I)*_cap_center.at(I) - _cap_radius.at(I)*_cap_radius.at(I))/(2.0*radius*_cap_center.at(I)));
			_angle_c.at(I) = std::acos((_cap_center.at(I)*_cap_center.at(I) + _cap_radius.at(I)*_cap_radius.at(I) - radius*radius)/(2.0*_cap_center.at(I)*_cap_radius.at(I)));
		    }
		    _area = _area + std::acos(-1.)/_cap_center.at(I)*(_cap_center.at(I)-radius-_cap_radius.at(I))*(_cap_center.at(I)-radius-_cap_radius.at(I))*(_cap_radius.at(I) - radius);
		    _mass_center = ((_cap_radius.at(I) * _cap_radius.at(I) + 4 * _cap_radius.at(I) * _cap_center.at(I) - radius * radius + _cap_center.at(I) * _cap_center.at(I)) * (-_cap_radius.at(I) + _cap_center.at(I) - radius) * (-_cap_radius.at(I) + _cap_center.at(I) - radius) / (_cap_center.at(I) * _cap_center.at(I) + (2 * _cap_radius.at(I) - 2 * radius) * _cap_center.at(I) - 3 * (radius + _cap_radius.at(I)) * (radius + _cap_radius.at(I)) ) / _cap_center.at(I)) / 2.0;
		}
		in >> charge_density;
		charge = charge_density*_area;
		return *this;
	    }

	    // remove later when not used!!!
	    void update() {
		_area = 4.0*std::acos(-1.)*radius*radius;
		if(!_is_sphere) {
		    for(int I = 0; I < _nbrOfCav; I++) {
			if( ( _cap_radius.at(I) > 1e-6 ) && ( _cap_center.at(I) > 1e-6) && ( radius > 1e-6 ) ) {
			    _angle_p.at(I) = std::acos(-1.) - std::acos((radius*radius + _cap_center.at(I)*_cap_center.at(I) - _cap_radius.at(I)*_cap_radius.at(I))/(2.0*radius*_cap_center.at(I)));
			    _angle_c.at(I) = std::acos((_cap_center.at(I)*_cap_center.at(I) + _cap_radius.at(I)*_cap_radius.at(I) - radius*radius)/(2.0*_cap_center.at(I)*_cap_radius.at(I)));
			} else {
			    _angle_p.at(I) = std::acos(-1.);
			    _angle_c.at(I) = 0.0;
			}
			_area = _area + std::acos(-1.)/_cap_center.at(I)*(_cap_center.at(I)-radius-_cap_radius.at(I))*(_cap_center.at(I)-radius-_cap_radius.at(I))*(_cap_radius.at(I) - radius);
			_mass_center = ((_cap_radius.at(I) * _cap_radius.at(I) + 4 * _cap_radius.at(I) * _cap_center.at(I) - radius * radius + _cap_center.at(I) * _cap_center.at(I)) * (-_cap_radius.at(I) + _cap_center.at(I) - radius) * (-_cap_radius.at(I) + _cap_center.at(I) - radius) / (_cap_center.at(I) * _cap_center.at(I) + (2 * _cap_radius.at(I) - 2 * radius) * _cap_center.at(I) - 3 * (radius + _cap_radius.at(I)) * (radius + _cap_radius.at(I)) ) / _cap_center.at(I)) / 2.0;
		    }

		} else {
		    //_angle_p.at(0) = std::acos(-1.);
		    //_angle_c.at(0) = 0.0;
		    _mass_center = 0.0;
		}
		charge = charge_density*_area;
	    }

	    /* write data members to stream */
	    friend std::ostream &operator<<(std::ostream &o, const CapParticle2 &p)
	    {
		//o << "Out" << p.nbrOfCav() << std::endl;
		o << PointParticle(p);
		for(int I = 0; I < p.nbrOfCav(); I++)
		    o << " " << p._cap_center_point.at(I).transpose() << " " << p._charge_position.at(I).transpose() << " " << p._cap_radius.at(I) << " " << p._cap_center.at(I);
		o << " " << p.charge_density;
		return o;
	    }

	    /**
	     * @brief Internal rotation: center-to-cap-vector
	     */
	    template<typename Trotator>
		void rotate(const Trotator &rot) {
		    assert(rot.getOrigin().squaredNorm()<1e-6);
		    for(int I = 0; I < _nbrOfCav; I++) {
			assert(fabs(cap_center_point(I).norm()-1.0) < 1e-6);
			cap_center_point(I) = rot(cap_center_point(I));
			charge_position(I) = rot(charge_position(I));
		    }
		}
		
	    /**
	     * @brief Reflect all vectors in 'n'
	     */
		void reflect(const Point &n) {
		  assert(fabs(n.norm()-1.0) < 1e-6);
		    for(int I = 0; I < _nbrOfCav; I++) {
			assert(fabs(cap_center_point(I).norm()-1.0) < 1e-6);
			Point d = cap_center_point(I);
			cap_center_point(I) = ( d - 2.0*d.dot(n)*n );
			
			d = charge_position(I);
			charge_position(I) = ( d - 2.0*d.dot(n)*n );
		    }
		}
    };








    /**
     * @brief Cap particle
     */
    struct DipCapParticle : public PointParticle {
	private:
	    Point _cap_center_point;               //!< Normalized center of cap
	    Point _charge_position;                //!< Position of charge
	    Point _off_center_dipole;              //!< Off center dipole position
	    double _cap_radius;                    //!< Radius of cap
	    double _cap_center;                    //!< Distance between particle and cap center
	    double _angle_p;		           //!< angle_p is the azimuth angle of the spherical back segment of the particles surface
	    double _angle_c; 			   //!< angle_c is the azimuth angle of the spherical front segment of the particles surface
	    double _area;                          //!< Surface-area of the particle
	    double _mass_center;                   //!< Mass center in the direction of '_cap_center_point'
	    bool _is_sphere;
	    Point _mu;        //!< Dipole moment unit vector (permanent+induced)
	    Point _mup;       //!< Permanent dipole moment vector
	    double _muscalar; //!< Dipole moment scalar (permanent+induced)
	    Tensor<double> _alpha;   //!< Polarization matrix
	    Tensor<double> _theta;   //!< Quadrupole matrix

	public:
	    Point cap_center_point() const { return _cap_center_point; } 
	    Point charge_position() const { return _charge_position; }
	    Point off_center_dipole() const { return _off_center_dipole; }
	    double cap_radius() const { return _cap_radius; }
	    double cap_center() const { return _cap_center; }
	    double angle_p() const { return _angle_p; }
	    double angle_c() const { return _angle_c; }
	    double area() const { return _area; }
	    double mass_center() const { return _mass_center; }
	    bool is_sphere() const { return _is_sphere; }
	    Point& cap_center_point() { return _cap_center_point; } 
	    Point& charge_position() { return _charge_position; }
	    Point& off_center_dipole() { return _off_center_dipole; }
	    double& cap_radius() { return _cap_radius; }
	    double& cap_center() { return _cap_center; }
	    double& angle_p() { return _angle_p; }
	    double& angle_c() { return _angle_c; }
	    double& area() { return _area; }
	    bool& is_sphere() { return _is_sphere; }
	    Point mu() const { return _mu; } 
	    Point mup() const { return _mup; }
	    double muscalar() const { return _muscalar; }
	    Tensor<double> alpha() const { return _alpha; }
	    Tensor<double> theta() const { return _theta; }
	    Point& mu() { return _mu; } 
	    Point& mup() { return _mup; }
	    double& muscalar() { return _muscalar; }
	    Tensor<double>& alpha() { return _alpha; }
	    Tensor<double>& theta() { return _theta; }



	    inline DipCapParticle() : _cap_center_point(1,0,0),_charge_position(0,0,0),_off_center_dipole(0,0,0),_cap_radius(0),_cap_center(0),_angle_p(0),_angle_c(0),_is_sphere(true), _mu(0, 0, 0), _muscalar(0), _mup(0, 0, 0) {};

	    /** @brief Copy constructor for Eigen derivatives */
	    template<typename OtherDerived>
		DipCapParticle(const Eigen::MatrixBase<OtherDerived>& other) : PointParticle(other) {}

	    /** @brief Generic copy operator for Eigen derivatives */
	    template<typename OtherDerived>
		DipCapParticle& operator=(const Eigen::MatrixBase<OtherDerived> &other) {
		    PointParticle::operator=(other);
		    return *this;
		}

	    /** @brief Copy operator for base class (i.e no casting to Eigen derivatives) */
	    inline DipCapParticle& operator=(const PointParticle &p) {
		PointParticle::operator=(p);
		return *this;
	    }

	    /** @brief Copy properties from AtomData object */
	    template<class T,
		class = typename std::enable_if<std::is_base_of<AtomData,T>::value>::type>
		    DipCapParticle& operator=(const T &d) {
			PointParticle::operator=(d);
			_cap_center=d.cap_center;
			_cap_center_point = Point(1,0,0);
			_charge_position = Point(d.charge_position,0,0);
			_off_center_dipole = d.off_center_dipole;
			_cap_radius=d.cap_radius;
			if(_cap_radius > 1e-6)
			    _is_sphere = false;
			if( ( _cap_radius > 1e-6 ) && ( _cap_center > 1e-6) && ( radius > 1e-6 ) ) {
			    _angle_p = std::acos(-1.) - std::acos((radius*radius + _cap_center*_cap_center - _cap_radius*_cap_radius)/(2.0*radius*_cap_center));
			    _angle_c = std::acos((_cap_center*_cap_center + _cap_radius*_cap_radius - radius*radius)/(2.0*_cap_center*_cap_radius));
			}
			_area = std::acos(-1.)/_cap_center*(radius*radius*radius + (_cap_radius+2.0*_cap_center)*radius*radius - (_cap_radius*_cap_radius-_cap_center*_cap_center)*radius-(_cap_radius-_cap_center)*(_cap_radius-_cap_center)*_cap_radius);
			_mass_center = ((_cap_radius * _cap_radius + 4 * _cap_radius * _cap_center - radius * radius + _cap_center * _cap_center) * (-_cap_radius + _cap_center - radius) * (-_cap_radius + _cap_center - radius) / (_cap_center * _cap_center + (2 * _cap_radius - 2 * radius) * _cap_center - 3 * (radius + _cap_radius) * (radius + _cap_radius) ) / _cap_center) / 2.0;
			_muscalar = d.muscalar;
			_mu = d.mu;
			_mup = _mu * _muscalar;
			_alpha = d.alpha;
			_theta = d.theta;
			return *this;
		    }

	    /* read in same order as written! */
	    inline DipCapParticle& operator<<(std::istream &in) {
		PointParticle::operator<<(in);
		_cap_center_point.operator<<(in);
		_charge_position.operator<<(in);
		_off_center_dipole.operator<<(in);
		in >> _cap_radius;
		if(_cap_radius > 1e-6)
		    _is_sphere = false;
		in >> _cap_center;
		if( ( _cap_radius > 1e-6 ) && (_cap_center > 1e-6) && ( radius > 1e-6 ) ) {
		    _angle_p = std::acos(-1.) - std::acos((radius*radius + _cap_center*_cap_center - _cap_radius*_cap_radius)/(2.0*radius*_cap_center));
		    _angle_c = std::acos((_cap_center*_cap_center + _cap_radius*_cap_radius - radius*radius)/(2.0*_cap_center*_cap_radius));
		}
		_area = std::acos(-1.)/_cap_center*(radius*radius*radius + (_cap_radius+2.0*_cap_center)*radius*radius - (_cap_radius*_cap_radius-_cap_center*_cap_center)*radius-(_cap_radius-_cap_center)*(_cap_radius-_cap_center)*_cap_radius);
		_mass_center = ((_cap_radius * _cap_radius + 4 * _cap_radius * _cap_center - radius * radius + _cap_center * _cap_center) * (-_cap_radius + _cap_center - radius) * (-_cap_radius + _cap_center - radius) / (_cap_center * _cap_center + (2 * _cap_radius - 2 * radius) * _cap_center - 3 * (radius + _cap_radius) * (radius + _cap_radius) ) / _cap_center) / 2.0;
		_mu.operator<<(in);
		in >> _muscalar;
		_mup.operator<<(in);
		_alpha.operator<<(in);
		_theta.operator<<(in);
		return *this;
	    }

	    // remove later when not used!!!
	    void update() {
		if(!_is_sphere) {
		    if( ( _cap_radius > 1e-6 ) && ( _cap_center > 1e-6) && ( radius > 1e-6 ) ) {
			_angle_p = std::acos(-1.) - std::acos((radius*radius + _cap_center*_cap_center - _cap_radius*_cap_radius)/(2.0*radius*_cap_center));
			_angle_c = std::acos((_cap_center*_cap_center + _cap_radius*_cap_radius - radius*radius)/(2.0*_cap_center*_cap_radius));
		    } else {
			_angle_p = std::acos(-1.);
			_angle_c = 0.0;
		    }
		    _area = std::acos(-1.)/_cap_center*(radius*radius*radius + (_cap_radius+2.0*_cap_center)*radius*radius - (_cap_radius*_cap_radius-_cap_center*_cap_center)*radius-(_cap_radius-_cap_center)*(_cap_radius-_cap_center)*_cap_radius);
		    _mass_center = ((_cap_radius * _cap_radius + 4 * _cap_radius * _cap_center - radius * radius + _cap_center * _cap_center) * (-_cap_radius + _cap_center - radius) * (-_cap_radius + _cap_center - radius) / (_cap_center * _cap_center + (2 * _cap_radius - 2 * radius) * _cap_center - 3 * (radius + _cap_radius) * (radius + _cap_radius) ) / _cap_center) / 2.0;
		} else {
		    _angle_p = std::acos(-1.);
		    _angle_c = 0.0;
		    _area = 4.0*std::acos(-1.)*radius*radius;
		    _mass_center = 0.0;
		}
	    }

	    /* write data members to stream */
	    friend std::ostream &operator<<(std::ostream &o, const DipCapParticle &p)
	    {
		o << PointParticle(p) << " " << p._cap_center_point.transpose() << " " << p._charge_position.transpose() << " " << p._off_center_dipole.transpose() << " " << p._cap_radius << " " << p._cap_center << " " << p._mu.transpose() << " " << p._muscalar << " " << p._mup << " " << p._alpha << " " << p._theta;
		return o;
	    }

	    /**
	     * @brief Internal rotation: center-to-cap-vector
	     */
	    template<typename Trotator>
		void rotate(const Trotator &rot) {
		    assert(rot.getOrigin().squaredNorm()<1e-6);
		    assert(fabs(cap_center_point().norm()-1.0) < 1e-6);
		    cap_center_point() = rot(cap_center_point());
		    charge_position() = rot(charge_position());
		    off_center_dipole() = rot(off_center_dipole());
		    mu() = rot(mu());
		    mup() = rot(mup());
		    _alpha = rot(_alpha);
		    _theta = rot(_theta);
		}
    };

    /**
     * @brief Tube-particle
     */
    struct TubeParticle : public PointParticle {
	private:
	    Point _direction;
	    double _radius_tube, _height_tube, _angle, _diagonal;
	    bool _is_sphere;

	public:
	    Point direction() const { return _direction; } 
	    double radius_tube() const { return _radius_tube; }
	    double height_tube() const { return _height_tube; }
	    bool is_sphere() const { return _is_sphere; }
	    double angle() const { return _angle; }
	    double diagonal() const { return _diagonal; }
	    Point& direction() { return _direction; } 
	    double& radius_tube() { return _radius_tube; }
	    double& height_tube() { return _height_tube; }
	    bool& is_sphere() { return _is_sphere; }
	    double& angle() { return _angle; }
	    double& diagonal() { return _diagonal; }

	    inline TubeParticle() {
	      _direction = Point(1,0,0);
	    };

	    /** @brief Copy constructor for Eigen derivatives */
	    template<typename OtherDerived>
		TubeParticle(const Eigen::MatrixBase<OtherDerived>& other) : PointParticle(other) {}

	    /** @brief Generic copy operator for Eigen derivatives */
	    template<typename OtherDerived>
		TubeParticle& operator=(const Eigen::MatrixBase<OtherDerived> &other) {
		    PointParticle::operator=(other);
		    return *this;
		}

	    /** @brief Copy operator for base class (i.e no casting to Eigen derivatives) */
	    inline TubeParticle& operator=(const PointParticle &p) {
		PointParticle::operator=(p);
		return *this;
	    }

	    /** @brief Copy properties from AtomData object */
	    template<class T,
		class = typename std::enable_if<std::is_base_of<AtomData,T>::value>::type>
		    TubeParticle& operator=(const T &d) {
			PointParticle::operator=(d);
			_radius_tube = d.radius_tube;
			_height_tube = d.height_tube;
			_is_sphere = false;
			_angle = 0.0;
			if(std::fabs(_radius_tube) < 1e-6)
			  _is_sphere = true;
			if(std::fabs(_height_tube) < 1e-6)
			  _is_sphere = true;
			if(!_is_sphere) {
			  _angle = std::acos(-1.0)/2.0 - std::atan2(_height_tube,_radius_tube);
			  _diagonal = std::sqrt(_radius_tube*_radius_tube + _height_tube*_height_tube);
			}
			return *this;
		    }

	    /* read in same order as written! */
	    inline TubeParticle& operator<<(std::istream &in) {
		PointParticle::operator<<(in);
		_direction.operator<<(in);
		in >> _radius_tube;
		in >> _height_tube;
		_is_sphere = false;
		_angle = 0.0;
		if(std::fabs(_radius_tube) < 1e-6)
		  _is_sphere = true;
		if(std::fabs(_height_tube) < 1e-6)
		  _is_sphere = true;
		if(!_is_sphere) {
		  _angle = std::acos(-1.0)/2.0 - std::atan2(_height_tube,_radius_tube);
		  _diagonal = std::sqrt(_radius_tube*_radius_tube + _height_tube*_height_tube);
		}
		return *this;
	    }

	    // remove later when not used!!!
	    void update() {
	    }

	    /* write data members to stream */
	    friend std::ostream &operator<<(std::ostream &o, const TubeParticle &p)
	    {
		o << PointParticle(p);
		o << " " << p._direction.transpose() << " " << p.radius_tube() << " " << p.height_tube();
		return o;
	    }

	    /**
	     * @brief Internal rotation
	     */
	    template<typename Trotator>
		void rotate(const Trotator &rot) {
		    assert(rot.getOrigin().squaredNorm()<1e-6);
		    assert(fabs(_direction.norm()-1.0) < 1e-6);
		    _direction = rot(_direction);
		}
    };    
    
    /**
     * @brief Kc particle
     */
    struct KcParticle : public PointParticle
    {
	private:
	    Point _lv;        	//!< Unit vector
	    Point _wv;        	//!< Unit vector
	    Point _dv;        	//!< Unit vector
	    double _length;      //!< Length of box
	    double _width; 	//!< Width of box
	    double _depth;       //!< Depth of box

	public:
	    Point lv() const { return _lv; } 
	    Point wv() const { return _wv; } 
	    Point dv() const { return _dv; } 
	    double length() const { return _length; }
	    double width() const { return _width; }
	    double depth() const { return _depth; }
	    Point& lv() { return _lv; } 
	    Point& wv() { return _wv; } 
	    Point& dv() { return _dv; } 
	    double& length() { return _length; }
	    double& width() { return _width; }
	    double& depth() { return _depth; }
	    

	    inline KcParticle() : _lv(1.0, 0.0, 0.0), _wv(0.0, 1.0, 0.0), _dv(0.0, 0.0, 1.0), _length(0), _width(0), _depth(0)  {};

	    /** @brief Copy constructor for Eigen derivatives */
	    template<typename OtherDerived>
		KcParticle( const Eigen::MatrixBase<OtherDerived> &other ) : PointParticle(other) {}

	    /** @brief Generic copy operator for Eigen derivatives */
	    template<typename OtherDerived>
		KcParticle &operator=( const Eigen::MatrixBase<OtherDerived> &other )
		{
		    PointParticle::operator=(other);
		    return *this;
		}

	    /** @brief Copy operator for base class (i.e no casting to Eigen derivatives) */
	    inline KcParticle &operator=( const PointParticle &p )
	    {
		PointParticle::operator=(p);
		return *this;
	    }

	    /** @brief Copy properties from AtomData object */
	    template<class T,
		class = typename std::enable_if<std::is_base_of<AtomData, T>::value>::type>
		    KcParticle &operator=( const T &d )
		    {
			PointParticle::operator=(d);
			_length = d.length;
			_width = d.width;
			_depth = d.depth;
			return *this;
		    }

	    /* read in same order as written! */
	    inline KcParticle &operator<<( std::istream &in )
	    {
		PointParticle::operator<<(in);
		_lv.operator<<(in);
		_wv.operator<<(in);
		_dv.operator<<(in);
		in >> _length;
		in >> _width;
		in >> _depth;
		return *this;
	    }

	    /* write data members to stream */
	    friend std::ostream &operator<<( std::ostream &o, const KcParticle &p )
	    {
		o << PointParticle(p) << " " << p._lv.transpose() << " " << p._wv.transpose() << " " << p._dv.transpose() << " " << p._length
		    << " " << p._width << " " << p._depth;
		return o;
	    }

	    /**
	     * @brief Internal rotation: dipole and polarizability
	     */
	    template<typename Trotator>
		void rotate( const Trotator &rot )
		{
		    assert(rot.getOrigin().squaredNorm() < 1e-6);
		    lv() = rot(lv());
		    wv() = rot(wv());
		    dv() = rot(dv());
		}
    };

}//namespace
#endif
