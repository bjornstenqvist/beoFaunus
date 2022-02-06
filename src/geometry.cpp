#include <faunus/geometry.h>
#include <faunus/inputfile.h>
#include <faunus/species.h>
#include <faunus/physconst.h>
#include <faunus/textio.h>
#include <faunus/group.h>
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace Faunus
{
    namespace Geometry
    {

	using namespace Faunus::textio;

	Geometrybase::Geometrybase() {}

	Geometrybase::Geometrybase( const string &name ) : name(name)
	{
	}

	Geometrybase::~Geometrybase() {}

	double Geometrybase::dist( const Point &p1, const Point &p2 ) const
	{
	    return sqrt(sqdist(p1, p2));
	}

	void Geometrybase::scale( Point &a, Point &s, const double xyz, const double xy ) const
	{
	    assert(!"Scaling function unimplemented for this geometry");
	}

	Cuboid Geometrybase::inscribe() const
	{
	    assert(!"Inscribe function not implemented for this geometry");
	    return Cuboid();
	}

	string Geometrybase::info( char w )
	{
	    using namespace textio;
	    std::ostringstream o;
	    o << pad(SUB, w, "Boundary") << name << endl
		<< pad(SUB, w, "Volume") << getVolume() << _angstrom << cubed
		<< " = " << getVolume() / 1e3 << " nm" << cubed
		<< " = " << getVolume() / 1e27 << " liters" << endl
		<< _info(w);
	    return o.str();
	}

	void Geometrybase::setVolume( double volume )
	{
	    _setVolume(volume);
	}

	double Geometrybase::getVolume(int dim) const
	{
	    assert(dim==1 || dim==2 || dim==3);
	    return _getVolume(dim);
	}

	double Geometrybase::_getRadius() const
	{
	    throw std::runtime_error(name + " error: radius undefined.");
	    return 0;
	}

	double Geometrybase::getRadius() const
	{
	    return _getRadius();
	}

	Sphere::Sphere( double radius ) : Geometrybase("Sphere")
	{
	    len = Point(r, diameter, 0);
	}

	/**
	 * Key        | Description
	 * :--------- | :-----------------------
	 * `radius`   | Sphere radius [angstrom]
	 */
	Sphere::Sphere( Tmjson &j ) : Geometrybase("Sphere")
	{
	    try {
		setRadius( j.at("radius") );
	    }
	    catch(std::exception& e) {
		throw std::runtime_error(name + ": " + e.what());
	    }
	}


	void Sphere::setRadius( double radius )
	{
	    assert(radius > 0 && "Radius must be larger than zero.");
	    r = radius;
	    r2 = r * r;
	    diameter = 2 * r;
	    len = Point(r, diameter, 0);
	}

	/**
	 * - 3D: \f$ 4\pi r^3/3 \f$ (default)
	 * - 2D: \f$ \pi r^2 \f$
	 * - 1D: \f$ 2r \f$
	 */
	double Sphere::_getVolume(int dim) const
	{
	    if (dim==1)
		return 2*r;
	    if (dim==2)
		return pc::pi * r * r;
	    return 4 / 3. * pc::pi * std::pow(r, 3);
	}

	void Sphere::_setVolume( double vol )
	{
	    setRadius(cbrt(3 * vol / (4 * pc::pi)));
	}

	void Sphere::setlen( const Point &l )
	{
	    Sphere::setRadius(l.x());
	    if ( getVolume() < 1e-9 )
		throw std::runtime_error("Sphere volume is zero.");
	}

	void Sphere::scale( Point &a, Point &s, const double xyz = 1, const double xy = 1 ) const
	{
	    assert(getVolume() > 0);
	    double newradius = cbrt(3 * xyz * xyz * xyz * getVolume() / (4 * pc::pi));
	    a = a * (newradius / r);
	}

	string Sphere::_info( char w )
	{
	    std::ostringstream o;
	    o << pad(SUB, w, "Radius") << r << textio::_angstrom << endl;
	    return o.str();
	}

	void Sphere::randompos( Point &a )
	{
	    do
	    {
		a.x() = (slump() - 0.5) * diameter;
		a.y() = (slump() - 0.5) * diameter;
		a.z() = (slump() - 0.5) * diameter;
	    }
	    while ( a.squaredNorm() > r2 );
	}

	bool Sphere::collision( const Point &a, double radius, collisiontype type ) const
	{
	    return (a.squaredNorm() > r2) ? true : false;
	}

	SphereSurface::SphereSurface( double radius ) : Geometrybase("SphereSurface")
	{
	    len = Point(r, diameter, 0);
	}

	/**
	 * Key        | Description
	 * :--------- | :-----------------------
	 * `radius`   | Sphere radius [angstrom]
	 */
	SphereSurface::SphereSurface( Tmjson &j ) : Geometrybase("SphereSurface")
	{
	    try {
		setRadius( j.at("radius") );
	    }
	    catch(std::exception& e) {
		throw std::runtime_error(name + ": " + e.what());
	    }
	}

	void SphereSurface::setRadius(double radius) {
	    assert(radius>0 && "Radius must be larger than zero.");
	    r = radius; 
	    r2 = r*r; 
	    diameter = 2.0*r; 
	    len = Point(r,diameter,0);
	}

	double SphereSurface::_getRadius() const {
	    return r;
	}

	double SphereSurface::_getVolume(int dim) const {
	    if (dim==1)
		return 2*pc::pi*r;      // circumference (surface area of a 2-sphere)
	    if (dim==2)
		return 4.0*pc::pi * r * r;   // surface area (of a 3-sphere)
	    return 2.0 * pc::pi*pc::pi * std::pow(r, 3); // surface area of a 4-sphere 
	}

	/**
	 * @param vol The surface area of the sphere
	 */
	void SphereSurface::_setVolume(double vol) {
	    setRadius( cbrt( vol/(2.0*pc::pi*pc::pi) ) );
	}

	void SphereSurface::setlen(const Point &l) {
	    SphereSurface::setRadius( l.x() );
	    if ( getVolume()<1e-9 )
		throw std::runtime_error( "Sphere volume is zero." );
	}

	void SphereSurface::scale(Point &a, Point &s, const double xyz=1, const double xy=1) const {
	    assert( getVolume()>0 );
	    double newradius = cbrt( 3*xyz*xyz*xyz*getVolume()/(4*pc::pi) );
	    a = a * (newradius/r);
	}

	string SphereSurface::_info(char w) {
	    std::ostringstream o;
	    o << pad(SUB, w, "Area") << getVolume(2) << _angstrom << squared
		<< " = " << getVolume(2) / 1e2 << " nm" << squared
		<< " = " << getVolume(2) / 1e18 << " dm" << squared << endl
		<< pad(SUB,w,"Radius") << r << textio::_angstrom << endl;
	    return o.str();
	}



	void SphereSurface::randompos(Point &a) {
	    do {
		a.x() = (slump()-0.5)*diameter;
		a.y() = (slump()-0.5)*diameter;
		a.z() = (slump()-0.5)*diameter;
	    } while ( a.squaredNorm()>r2 );
	    a = r*a/a.norm();
	}

	bool SphereSurface::collision(const Point &a, double radius, collisiontype type) const {
	    return (std::fabs( a.squaredNorm() - r2 ) > 1e-6) ? true:false;
	}

	Cuboid SphereSurface::inscribe() const
	{
	    Cuboid c;
	    c.setlen({diameter, diameter, diameter});
	    return c;
	}

	Cuboid Sphere::inscribe() const
	{
	    Cuboid c;
	    c.setlen({diameter, diameter, diameter});
	    return c;
	}

	Cuboid Cuboid::inscribe() const
	{
	    return *this;
	}

	Cuboid::Cuboid() {}

	void Cuboid::setlen( const Point &l )
	{
	    len = l;                    // Cuboid sidelength
	    len_half = l * 0.5;             // half Cuboid sidelength
	    len_inv.x() = 1 / len.x();      // inverse Cuboid side length
	    len_inv.y() = 1 / len.y();
	    len_inv.z() = 1 / len.z();
	    if ( getVolume() < 1e-9 )
		throw std::runtime_error("Cuboid volume is zero");
	}

	void Cuboid::_setVolume( double newV )
	{
	    double xyz = 1.0, xy = 1.0;
	    Point s = Point(1, 1, 1);
	    if ( scaledir == XYZ )
		xyz = cbrt(newV / getVolume());
	    if ( scaledir == XY )
		xy = sqrt(newV / getVolume());
	    scale(len, s, xyz, xy);
	    setlen(len);
	}

	/**
	 * - 3D: \f$ l_x l_y l_z \f$ (default)
	 * - 2D: \f$ l_x l_y \f$
	 * - 1D: \f$ l_z \f$
	 */
	double Cuboid::_getVolume(int dim) const
	{
	    if (dim==1)
		return len.z();
	    if (dim==2)
		return len.x() * len.y();
	    return len.x() * len.y() * len.z();
	}

	string Cuboid::_info( char w )
	{
	    std::ostringstream o;
	    o << pad(SUB, w, "Sidelengths")
		<< len.transpose() << " (" + textio::angstrom + ")" << endl
		<< pad(SUB, w, "Scale directions") << scaledirstr << endl;
	    return o.str();
	}

	Point Cuboid::randompos()
	{
	    Point m;
	    randompos(m);
	    return m;
	}

	void Cuboid::randompos( Point &m )
	{
	    m.x() = slump.half() * len.x();
	    m.y() = slump.half() * len.y();
	    m.z() = slump.half() * len.z();
	}

	void Cuboid::scale( Point &a, Point &s, const double xyz = 1, const double xy = 1 ) const
	{
	    assert(getVolume() > 0);
	    if ( scaledir == XYZ )
		a = Point(a.x() * s.x() * xyz, a.y() * s.y() * xyz, a.z() * s.z() * xyz);
	    if ( scaledir == XY )
		a = Point(a.x() * s.x() * xy, a.y() * s.y() * xy, a.z() * s.z());
	}

	/**
	 * The json object is scanned for the following parameters:
	 *
	 * Key           | Description
	 * :------------ | :-----------------------------------------------------------
	 * `length`      | Array of sidelengths _or_ single length for cube [angstrom]
	 * `scaledir`    | Isobaric scaling directions (`XYZ`=isotropic, `XY`=xy only).
	 */
	Cuboid::Cuboid( Tmjson &j ) : Geometrybase("Cuboid")
	{
	    try {
		if (j.is_object()) {
		    scaledirstr = j.value<string>("scaledir", "XYZ");
		    scaledir = (scaledirstr == "XY") ? XY : XYZ;
		    auto m = j.at("length");
		    if (m.is_number()) {
			double l = m.get<double>();
			setlen( {l,l,l} );
		    }
		    if (m.is_array()) {
			len << m.get<vector<double>>();
			setlen( len );
		    }
		    if (getVolume()<=0)
			throw std::runtime_error("volume is zero or less");
		}
	    }
	    catch(std::exception& e) {
		throw std::runtime_error(name + ": " + e.what());
	    }
	}

	Cuboidslit::Cuboidslit() { name += " (XY-periodicity)"; }

	Cuboidslit::Cuboidslit( Tmjson &j ) : Cuboid(j) { name += " (XY-periodicity)"; }

	/**
	 * @param length Length of the Cylinder (angstrom)
	 * @param radius Radius of the Cylinder (angstrom)
	 */
	Cylinder::Cylinder( double length, double radius ) : Geometrybase("Cylinder")
	{
	    init(length, radius);
	}

	/**
	 * The json object is scanned for the following parameters:
	 *
	 * Key      | Description
	 * :------- | :-------------------------
	 * `length` | Cylinder length [angstrom]
	 * `radius` | Cylinder radius [angstrom] 
	 */
	Cylinder::Cylinder( Tmjson &j ) : Geometrybase("Cylinder")
	{
	    try {
		init( j.at("length"),
			j.at("radius") );
	    }
	    catch(std::exception& e) {
		throw std::runtime_error(name + ": " + e.what());
	    }
	}

	void Cylinder::init( double length, double radius )
	{
	    name = "Cylinder (hard ends)";
	    _len = length;
	    setVolume(pc::pi * radius * radius * _len);
	}

	/**
	 * Dummy function not be used other than for compatibility
	 * reasons. Sets length to x component of vector.
	 */
	void Cylinder::setlen( const Point &l )
	{
	    init(l.x(), r);
	}

	void Cylinder::_setVolume( double newV )
	{
	    r2 = newV / (pc::pi * _len);
	    r = sqrt(r2);
	    diameter = 2 * r;
	    _halflen = _len / 2;
	}

	/**
	 * - 3D: \f$ l \pi r^2 \f$ (default)
	 * - 2D: \f$ \pi r^2 \f$
	 * - 1D: \f$ l \f$
	 */
	double Cylinder::_getVolume(int dim) const
	{
	    if (dim==1)
		return _len;
	    if (dim==2)
		return pc::pi * r2;
	    return r2 * pc::pi * _len;
	}

	void Cylinder::boundary( Point &p ) const {}

	void Cylinder::randompos( Point &m )
	{
	    double l = r2 + 1;
	    m.z() = slump.half() * _len;
	    while ( l > r2 )
	    {
		m.x() = slump.half() * diameter;
		m.y() = slump.half() * diameter;
		l = m.x() * m.x() + m.y() * m.y();
	    }
	}

	bool Cylinder::collision( const Point &a, double radius, collisiontype type ) const
	{
	    assert((_halflen - _len / 2) < 1e-9 && "Cylinder length initialization problems");
	    if ( a.z() < -_halflen )
		return true;
	    if ( a.z() > _halflen )
		return true;
	    if ( a.x() * a.x() + a.y() * a.y() > r2 )
		return true;
	    return false;
	}

	string Cylinder::_info( char w )
	{
	    std::ostringstream o;
	    o << pad(SUB, w, "Length") << _halflen * 2 << textio::_angstrom << endl
		<< pad(SUB, w, "Radius") << sqrt(r2) << textio::_angstrom << endl;
	    return o.str();
	}

	Cuboid Cylinder::inscribe() const
	{
	    Cuboid c;
	    c.setlen({diameter, diameter, _len});
	    return c;
	}



	PeriodicCylinder::PeriodicCylinder(
		double length, double radius ) : Cylinder(length, radius)
	{
	    name = "Periodic " + name;
	}

	PeriodicCylinder::PeriodicCylinder( Tmjson &j ) : Cylinder(j)
	{
	    name = "Periodic " + name;
	}

	void PeriodicCylinder::boundary( Point &a ) const
	{
	    if ( std::abs(a.z()) > _halflen )
		a.z() -= _len * anint(a.z() / _len);
	}

	Hexagon::Hexagon( double radius, double height ) : Geometrybase("Hexagon")
	{
	    len.x() = radius;
	    len.y() = len.x()*std::cos(pc::pi/6.0);
	    len.z() = height;
	    unitvX = Point(1.0,0.0,0.0);
	    unitvY = Point(0.5,sqrt(3.0)/2.0,0.0);
	    unitvZ = Point(-0.5,sqrt(3.0)/2.0,0.0);
	    //setRadius( radius );
	    //setHeight( height );
	}

	Hexagon::Hexagon( Tmjson &j ) : Geometrybase("Hexagon")
	{
	    try {
		scaledirstr = j.value<string>("scaledir", "XYZ");
		scaledir = (scaledirstr == "XY") ? XY : XYZ;
		setRadius( j.at("radius") );
		setHeight( j.at("height") );
		unitvX = Point(1.0,0.0,0.0);
		unitvY = Point(0.5,sqrt(3.0)/2.0,0.0);
		unitvZ = Point(-0.5,sqrt(3.0)/2.0,0.0);
	    }
	    catch(std::exception& e) {
		throw std::runtime_error(name + ": " + e.what());
	    }
	}

	// Keeps height constant
	void Hexagon::_setVolume( double vol )
	{
	    len.x() = sqrt(vol/len.z()*2.0/3.0/sqrt(3.0));
	    setRadius(len.x());
	}

	void Hexagon::setRadius(double radius) {
	    assert(radius>0 && "Radius must be larger than zero.");
	    len.x() = radius;
	    len.y() = radius*std::cos(pc::pi/6.0);
	}

	void Hexagon::setHeight(double height) {
	    assert(height>0 && "Height must be larger than zero.");
	    len.z() = height;
	}

	double Hexagon::_getRadius() const {
	    return len.x();
	}

	double Hexagon::_getVolume(int dim) const {
	    if (dim==1)
		return 6.0*len.x();      // Circumference
	    if (dim==2)
		return 3.0*sqrt(3.0)/2.0*len.x()*len.x(); // Area
	    return 3.0*sqrt(3.0)/2.0*len.x()*len.x()*len.z(); // Volume
	}

	void Hexagon::setlen(const Point &l) {
	    setRadius( l.x() );
	    setHeight( l.z() );
	    if ( getVolume()<1e-9 )
		throw std::runtime_error( "Volume is zero." );
	}

	// 's.y()' is irrelevante since scaling can only be in 'R'- and 'z'-axis
	void Hexagon::scale(Point &a, Point &s, const double xyz=1, const double xy=1) const {
	    assert(getVolume() > 0);
	    if ( scaledir == XYZ )
		a = Point(a.x()*s.x()*xyz,a.y()*s.x()*xyz,a.z()*s.z()*xyz);
	    if ( scaledir == XY )
		a = Point(a.x()*s.x()*xy,a.y()*s.x()*xy,a.z());
	}

	string Hexagon::_info(char w) {
	    std::ostringstream o;
	    o << pad(SUB, w, "Area") << getVolume(2) << _angstrom << squared
		<< " = " << getVolume(2) / 1e2 << " nm" << squared
		<< " = " << getVolume(2) / 1e18 << " dm" << squared << endl
		<< pad(SUB, w, "Volume") << getVolume(3) << _angstrom << cubed << endl
		<< pad(SUB,w,"Outer radius") << len.x() << textio::_angstrom << endl
		<< pad(SUB,w,"Inner radius") << len.y() << textio::_angstrom << endl
		<< pad(SUB,w,"Height") << len.z() << textio::_angstrom << endl;
	    return o.str();
	}

	// note: Different axis than in geometry setup
	Point Hexagon::getRandomPointdInTriangle() {
	    Point vec = len.x()*unitvX*slump()+len.x()*unitvY*slump();
	    Point meanV = (unitvX + unitvY);
	    meanV = meanV/meanV.norm();
	    double r = vec.dot(meanV);
	    if(r > len.y())
		vec = len.x()*(unitvX+unitvY) - vec;
	    return vec;
	}

	void Hexagon::randompos(Point &a)  {
	    Point vec = Hexagon::getRandomPointdInTriangle();
	    Point rtp = vec.xyz2rtp();
	    rtp.y() = rtp.y() + pc::pi/6.0 + 2.0*pc::pi; // Align with current coordinate-system
	    if(rtp.y() > 2.0*pc::pi)
	      rtp.y() -= 2.0*pc::pi;
	    int sector = std::floor(6.0*slump()); // Get random integer between 0-5
	    rtp.y() = rtp.y() + double(sector)*pc::pi/3.0;
	    a = rtp.rtp2xyz();
	    if ( scaledir != XY )
		a.z() = (slump()-0.5)*len.z();
	}

	Point Hexagon::randompos()
	{
	    Point m(0,0,0);
	    randompos(m);
	    return m;
	}

	Cuboid Hexagon::inscribe() const
	{
	    Cuboid c;
	    c.setlen({2.0*len.x(), 2.0*len.x(), len.z()});
	    return c;
	}

	Octahedron::Octahedron( double length ) : Geometrybase("Octahedron")
	{
	    setLength( length );

	    unitvX = Point(1.0,0.0,0.0);
	    unitvY = Point(0.0,1.0,0.0);
	    unitvZ = Point(0.0,0.0,1.0);
	    
	    unitvXYZ =  ( unitvX + unitvY + unitvZ);
	    unitvXiYZ = ( unitvX + unitvY - unitvZ);
	    unitvXYiZ = ( unitvX - unitvY - unitvZ);
	    unitvXYZi = ( unitvX - unitvY + unitvZ);
		
	    unitvXYZ = unitvXYZ/unitvXYZ.norm();
	    unitvXiYZ = unitvXiYZ/unitvXiYZ.norm();
	    unitvXYiZ = unitvXYiZ/unitvXYiZ.norm();
	    unitvXYZi = unitvXYZi/unitvXYZi.norm();
	    
	}

	Octahedron::Octahedron( Tmjson &j ) : Geometrybase("Octahedron")
	{
	    try {
		scaledirstr = j.value<string>("scaledir", "XYZ");
		scaledir = (scaledirstr == "XY") ? XY : XYZ;
		setLength( j.at("length") );
		unitvX = Point(1.0,0.0,0.0);
		unitvY = Point(0.0,1.0,0.0);
		unitvZ = Point(0.0,0.0,1.0);
		
		unitvXYZ =  ( unitvX + unitvY + unitvZ);
		unitvXiYZ = ( unitvX + unitvY - unitvZ);
		unitvXYiZ = ( unitvX - unitvY - unitvZ);
		unitvXYZi = ( unitvX - unitvY + unitvZ);
		
		unitvXYZ = unitvXYZ/unitvXYZ.norm();
		unitvXiYZ = unitvXiYZ/unitvXiYZ.norm();
		unitvXYiZ = unitvXYiZ/unitvXYiZ.norm();
		unitvXYZi = unitvXYZi/unitvXYZi.norm();
	    }
	    catch(std::exception& e) {
		throw std::runtime_error(name + ": " + e.what());
	    }
	}

	// Keeps height constant
	void Octahedron::_setVolume( double vol )
	{
	    double length = pow(vol/9.0/sqrt(2.0),1.0/3.0);
	    Octahedron::setLength(length);
	}

	// 'length' = a
	void Octahedron::setLength(double length) {
	    //assert(length>0 && "Length must be larger than zero.");
	    len.x() = length;
	    len.y() = length;
	    len.z() = length;
	    //radiusV.x() = 9.0*sqrt(10.0)/20.0*len.x();
	    //radiusV.y() = 1.5*len.x();
	    //radiusV.z() = sqrt(10.0)/2.0*len.x();
	    radiusV.y() = sqrt(2.0)*len.x();
	    radiusV.x() = sqrt(6.0)/2.0*len.x();
	    radiusV.z() = sqrt(10.0)/2.0*len.x();
	}

	double Octahedron::getLength() const {
	    return len.x();
	}

	double Octahedron::_getVolume(int dim) const {
	    if (dim==2)
		return (6.0 + 12.0*sqrt(3.0))*len.x()*len.x(); // Area
	    return 8.0*sqrt(2.0)*len.x()*len.x()*len.x(); // Volume
	}
	
	void Octahedron::setlen(const Point &length) {
	    setLength(length.x());
	    if ( getVolume()<1e-9 )
		throw std::runtime_error( "Volume is zero." );
	}

	void Octahedron::setlen(double length) {
	    setLength(length);
	    if ( getVolume()<1e-9 )
		throw std::runtime_error( "Volume is zero." );
	}

	// 's.y()' is irrelevante since scaling can only be in 'R'- and 'z'-axis
	void Octahedron::scale(Point &a, Point &s, const double xyz=1, const double xy=1) const {
	    assert(getVolume() > 0);
	    if ( scaledir == XYZ )
		a = Point(a.x()*s.x()*xyz,a.y()*s.y()*xyz,a.z()*s.z()*xyz);
	}

	string Octahedron::_info(char w) {
	    std::ostringstream o;
	    o << pad(SUB, w, "Area") << getVolume(2) << _angstrom << squared
		<< " = " << getVolume(2) / 1e2 << " nm" << squared
		<< " = " << getVolume(2) / 1e18 << " dm" << squared << endl
		<< pad(SUB, w, "Volume") << getVolume(3) << _angstrom << cubed << endl;
	    return o.str();
	}

	// Needs to be fixed
	void Octahedron::randompos(Point &a)  {
	    {
	      do
	      {
		  a.x() = (slump() - 0.5);
		  a.y() = (slump() - 0.5);
		  a.z() = (slump() - 0.5);
	      }
	      while ( a.squaredNorm() > 0.25 );
	      a = radiusV.x()*a;
	    }
	    while(Octahedron::collision(a,0.0));
	}

	Point Octahedron::randompos()
	{
	    Point m(0,0,0);
	    randompos(m);
	    return m;
	}

	Cuboid Octahedron::inscribe() const
	{
	    Cuboid c;
	    c.setlen({2.0*radiusV.z(), 2.0*radiusV.z(), 2.0*radiusV.z()});
	    return c;
	}

	void QuaternionRotate::setAxis( Geometrybase &g, const Point &beg, const Point &end, double angle )
	{
	    geoPtr = &g;
	    origin = beg;
	    angle_ = angle;
	    Point u(end - beg); //Point u(end-beg);
	    assert(u.squaredNorm() > 0 && "Rotation vector has zero length");
	    g.boundary(u);
	    u.normalize(); // make unit vector
	    q = Eigen::AngleAxisd(angle, u);

	    rot_mat << 0, -u.z(), u.y(), u.z(), 0, -u.x(), -u.y(), u.x(), 0;
	    rot_mat = Eigen::Matrix3d::Identity() + rot_mat * std::sin(angle) + rot_mat * rot_mat * (1.0 - std::cos(angle));
	}

	FindSpace::FindSpace()
	{
	    dir = Point(1, 1, 1);
	    allowContainerOverlap = false;
	    allowMatterOverlap = false;
	}

	CuboidNoPBC::CuboidNoPBC( ) { name += " (No PBC)"; }

	CuboidNoPBC::CuboidNoPBC( Tmjson &j ) : Cuboid(j) { name += " (No PBC)"; }
    }//namespace geometry
}//namespace
