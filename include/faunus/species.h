#ifndef FAUNUS_SPECIES_H
#define FAUNUS_SPECIES_H

#ifndef SWIG
#include <faunus/common.h>
#include <faunus/json.h>
#include <faunus/json.hpp>
#include <faunus/physconst.h>
#include <faunus/textio.h>
#include <faunus/point.h>
#include <faunus/slump.h>
#include <faunus/average.h>

#endif

namespace Faunus
{

	/**
	 * @brief Base class for property data
	 *
	 * Used to collect properties for atoms, molecules etc.
	 */
	struct PropertyBase
	{
		typedef unsigned char Tid;
		std::string name;  //!< Unique, user-defined name
		Tid id;            //!< Unique id (automatically set by `PropertyVector`)
	};

	/**
	 * @brief Container for properties accessible either by index or string
	 *
	 * @details
	 * This is a specialization of `std::vector` where the elements
	 * must by derived from `PropertyBase` and can be inserted only
	 * using `push_back()`. This enforces that `id` of the data
	 * is always equal to the vector index. Elements can be accessed
	 * by either index of string matching.
	 *
	 * Example:
	 *
	 * ~~~~
	 * struct MyProp : public PropertyBase {
	 *   double mass;
	 * };
	 *
	 * PropertyVector<MyProp> v;
	 *
	 * MyProp d;
	 * d.name="oxygen";
	 * v.push_back(d);
	 * v["oxygen"].mass = 15.999;
	 *
	 * for (auto i : v)
	 *   std::cout << i.name << " " << i.mass; // --> oxygen 15.999
	 *
	 * std::cout << v[0].mass; // -> 15.999
	 * ~~~~
	 *
	 * Complexity: constant if direct access, linear if using string.
	 */
	template<class Tproperty, class base=std::vector<Tproperty> >
		class PropertyVector : private base
	{
		protected:
			string name;        // name of properties
		public:
			using typename base::value_type;
			using typename base::size_type;
			using typename base::iterator;
			using typename base::reference;
			using typename base::const_iterator;
			using typename base::const_reference;

			/** @brief Iterator to random element */
			const_iterator random() const { return slump.element(begin(), end()); }

			/** @brief Iterator to random element */
			iterator random() { return slump.element(begin(), end()); }

			/** @brief Add element at the end */
			void push_back( const value_type &d )
			{
				base::push_back(d);
				base::back().id = base::size() - 1;
			}

			iterator begin() { return base::begin(); } //!< Iterator
			iterator end() { return base::end(); }     //!< Iterator
			const_iterator begin() const { return base::begin(); } //!< Iterator
			const_iterator end() const { return base::end(); } //!< Iterator
			size_type size() const { return base::size(); } //!< Size
			bool empty() const { return base::empty(); } //!< Empty test

			/** @brief Find element by name */
			const_iterator find( const string &name ) const
			{
				return std::find_if(begin(), end(), [&name]( const value_type &i ) { return i.name == name; });
			}

			/** @brief Find element by name */
			iterator find( const string &name )
			{
				return std::find_if(begin(), end(), [&name]( const value_type &i ) { return i.name == name; });
			}

			/** @brief Access element by string */
			const_reference operator[]( const std::string &name ) const { return *find(name); }

			/** @brief Access element */
			const_reference operator[]( size_type i ) const
			{
				assert(i == base::operator[](i).id && "Property out of sync");
#ifdef NDEBUG
				return base::operator[](i);
#else
				return base::at(i);
#endif
			}

			/** @brief Access element */
			reference operator[]( size_type i )
			{
				assert(i == base::operator[](i).id && "Property out of sync");
				assert(i < base::size());
#ifdef NDEBUG
				return base::operator[](i);
#else
				return base::at(i);
#endif
			}

			/** Load data from json object */
			bool include( Tmjson &j )
			{
				base::reserve( j.size() );
				for ( auto it = j.begin(); it != j.end(); ++it )
				{
					try {
						push_back( value_type(it) );
					}
					catch(std::exception& e) {
						throw std::runtime_error(name + ", element " + it.key() + ": " + e.what());
					}
				}

				return !empty();
			}

			PropertyVector()
			{
				static_assert(std::is_base_of<PropertyBase, Tproperty>::value,
						"Elements must be derived from `PropertyBase`");
			}

			string info()
			{
				using namespace textio;
				char w = 25;
				std::ostringstream o;
				o << header(name)
					<< pad(SUB, w, "Number of entries:") << size() << endl;
				o << indent(SUB) << "Element info:";
				for ( auto &i : *this )
				{
					if ( i.id % 10 == 0 )
						o << endl << indent(SUBSUB);
					o << setw(SUBSUB + 2) << std::left << i.name;
				}
				o << endl;
				return o.str();
			}
	};

	/**
	 * @brief Storage for atomic properties
	 *
	 * This stores properties about single atoms and can be used together
	 * with `PropertyVector`. Values can be read from a JSON
	 * object with the following format:
	 *
	 * ~~~~
	 * "Na" : { "q": 1.0, "r":1.9, "mw":22.99 }, // sodium ion
	 * ~~~~
	 *
	 * or more advanced,
	 *
	 * ~~~~
	 * "pol" :
	 *    {
	 *      "r":2.0,
	 *      "mu":"1.3 0.1 0",
	 *      "alpha":"1.1   0   0  2.3   0    1",    // Matrix input in 
	 *      "theta":"2.4 0.3   0  1.8   0  3.2"     // (xx, xy, xz, yy, yz, zz) form
	 *    }
	 * ~~~~
	 *
	 * The key of type string is the `name` followed, in no particular order,
	 * by properties:
	 *
	 * Key           | Description
	 * :------------ | :-------------------------------------------------------
	 * `activity`    | Chemical activity for grand canonical MC [mol/l]
	 * `alpha`       | Polarizability in units of [\f$ 4\pi\epsilon_0 \f$]
	 * `dp`          | Translational displacement parameter [angstrom] 
	 * `dprot`       | Rotational displacement parameter [degrees] (will be converted to radians)
	 * `eps`         | Epsilon energy scaling commonly used for Lennard-Jones interactions etc. [kJ/mol] 
	 * `hydrophobic` | Is the particle hydrophobic? [true/false]
	 * `mu`          | Dipole moment vector [Debye]
	 * `Ninit`       | Initial number of atoms (used by `MoleculeData` to insert atoms
	 * `theta`       | Quadrupole moment tensor [Debye \f$ \unicode{x212B} \f$]
	 * `mw`          | Molecular weight [g/mol]
	 * `patchtype`   | Patchtype for sphero-cylinders
	 * `q`           | Valency / partial charge number [e]
	 * `r`           | Radius = `sigma/2` [angstrom]
	 * `sigma`       | `2*r` [angstrom] (overrides radius)
	 * `tfe`         | Transfer free energy [J/mol/angstrom^2/M] (default: 0.0)
	 * `alphax`      | Excess polarizability in units of [angstrom^3]
	 */

	class AtomData : public PropertyBase
	{
		public:
			double sigma,         //!< LJ diameter [angstrom]
			       eps,              //!< LJ epsilon [kJ/mol] (pair potentials should convert to kT)
			       radius,           //!< Radius [angstrom]
			       muscalar,         //!< Dipole momentscalar [e \f$ \unicode{x212B} \f$]
			       mw,               //!< Weight [g/mol]
			       charge,           //!< Charge/valency [e]
			       activity,         //!< Chemical activity [mol/l]
			       chemPot,          //!< Chemical potential, calculated from activity
			       dp,               //!< Translational displacement parameter [angstrom]
			       dprot,            //!< Rotational displacement parameter [degrees]
			       mean,             //!< Mean value... (charge, sasa, etc.)
			       variance,         //!< Spread around AtomData::mean
			       len,              //!< Spherocylinder length [angstrom]
			       half_len,         //!< Spherocylinder half length [angstrom]
			       pangl,            //!< Angle of attrative patch on PSC [degrees]
			       panglsw,          //!< Angle of angluar switch on sides of patch on PSC [degrees]
			       pdis,             //!< Dist. to which attraction is flat on attrative patch on PSC [angstrom]
			       pswitch,          //!< Distance on which attraction switches to zero on PSC [angstrom]
			       chiral_angle,     //!< Angle of chirality (rotation of patch) on PSC [degrees]
			       betaC,            //!< Value of the charge distribution (inverse) width [1/angstrom]
			       betaD,            //!< Value of the dipole distribution (inverse) width [1/angstrom]
			       betaQ,            //!< Value of the quadrupole distribution (inverse) width [1/angstrom]
			       tfe,              //!< Transfer free energy (J/mol/angstrom^2/M)
			       cap_radius,       //!< Radius of cap
			       cap_center,       //!< Distance to cap center from particle center
			       charge_position,  //!< Position of charge
			       charge_density,   //!< Charge density/valency [e / \f$  \AA^2 \f$ ]
			       area,             //!< Area of particle [angstrom^2]
			       alphax,           //!< Excess polarizability [angstrom^3]
			       radius_tube,      //!< Radius of tube
			       height_tube,      //!< Height of tube
			       length,      	 //!< Length
			       width,    	 //!< Width
			       depth; 	     	 //!< Depth
			Point mu;                //!< Dipolemoment vector
			Point lv;                //!< Kc unit vector
			Point wv;                //!< Kc unit vector
			Point dv;                //!< Kc unit vector
			Point off_center_dipole; //!< x-component of the position of the dipole
			int Ninit;               //!< Initial number of atoms to insert into simulation container
			short int patchtype;     //!< If patchy particle, which type of patch
			bool hydrophobic;        //!< Are we hydrophobic?
			bool charge_distribution;//!< Using a charge distribution
			vector<double> cap_radiusV, //!< Radius of cap
				cap_centerV,            //!< Distance to cap center from particle center
				charge_positionV;       //!< Position of charge
			vector<Point> cav_pos;
			Tensor<double>
				alpha,                 //!< Polarizability
				theta;                 //!< Quadrupole moment

			bool operator==( const AtomData &d ) const { return (*this == d); }

			/** @brief Constructor - by default data is initialized; mass set to unity */
			inline AtomData( Tmjson::iterator &atom )
			{
				auto _js = atom.value();
				if ( !atom.key().empty())
					name = atom.key();

				activity = _js.value("activity", 0.0);
				chemPot = log(activity * 1.0_molar);
				alpha << _js.value("alpha", string());
				alpha /= pc::lB(1.0);
				theta << _js.value("theta", string());
				theta *= 1.0_Debye;
				dp = _js.value("dp", 0.0);
				dprot = _js.value("dprot", 0.0) * 1.0_deg; // deg->rads
				eps = (_js["eps"] | 0.0) * 1.0_kJmol;
				hydrophobic = _js["hydrophobic"] | false;
				charge_distribution = _js["useRho"] | false;
				mu << (_js["mu"] | string("0 0 0"));
				muscalar = mu.len() * 1.0_Debye;
				if ( mu.len() > 1e-6 )
					mu = mu / mu.len();

				radius_tube   =  ( _js["radius_tube"] | 0.0 );
				height_tube   =  ( _js["height_tube"] | 0.0 );

				cap_radius   =  ( _js["cap_radius"] | 0.0 );
				cap_center   =  ( _js["cap_center"] | 0.0 );
				charge_position   =  ( _js["charge_position"] | 0.0 );
				off_center_dipole << (_js["off_center_dipole"] | string("0 0 0"));
				
				lv << (_js["lv"] | string("1 0 0"));
				wv << (_js["wv"] | string("0 1 0"));
				dv << (_js["dv"] | string("0 0 1"));
				length   =  ( _js["length"] | 0.0 );
				width   =  ( _js["width"] | 0.0 );
				depth   =  ( _js["depth"] | 0.0 );

				string tmp0 = _js.value("cap_radiusV", string());
				std::istringstream stm0(tmp0);
				string line0;
				while(std::getline(stm0, line0)) {
					std::string delimiter = " ";
					size_t pos = 0;
					std::string token;
					while ((pos = line0.find(delimiter)) != std::string::npos) {
						token = line0.substr(0, pos);
						cap_radiusV.push_back(std::stod(token));
						line0.erase(0, pos + delimiter.length());
					}
					cap_radiusV.push_back(std::stod(line0));
				}

				string tmp1 = _js.value("cap_centerV", string());
				std::istringstream stm1(tmp1);
				string line1;
				while(std::getline(stm1, line1)) {
					std::string delimiter = " ";
					size_t pos = 0;
					std::string token;
					while ((pos = line1.find(delimiter)) != std::string::npos) {
						token = line1.substr(0, pos);
						cap_centerV.push_back(std::stod(token));
						line1.erase(0, pos + delimiter.length());
					}
					cap_centerV.push_back(std::stod(line1));
				}

				string tmp2 = _js.value("cap_centerV", string());
				std::istringstream stm2(tmp2);
				string line2;
				while(std::getline(stm2, line2)) {
					std::string delimiter = " ";
					size_t pos = 0;
					std::string token;
					while ((pos = line2.find(delimiter)) != std::string::npos) {
						token = line2.substr(0, pos);
						charge_positionV.push_back(std::stod(token));
						line2.erase(0, pos + delimiter.length());
					}
					charge_positionV.push_back(std::stod(line2));
				}

				cav_pos.resize(cap_radiusV.size());
				for(unsigned int i = 0; i < cav_pos.size(); i++)
					cav_pos.at(i) = Point(0,0,0);

				string tmpX = _js.value("cav_posX", string());
				std::istringstream stmX(tmpX);
				int indexX = 0;
				string line;
				while(std::getline(stmX, line)) {
					std::string delimiter = " ";
					size_t pos = 0;
					std::string token;
					while ((pos = line.find(delimiter)) != std::string::npos) {
						token = line.substr(0, pos);
						cav_pos.at(indexX++).x() = std::stod(token);
						line.erase(0, pos + delimiter.length());
					}
					cav_pos.at(indexX++).x() = std::stod(line);
				}

				string tmpY = _js.value("cav_posY", string());
				std::istringstream stmY(tmpY);
				int indexY = 0;
				while(std::getline(stmY, line)) {
					std::string delimiter = " ";
					size_t pos = 0;
					std::string token;
					while ((pos = line.find(delimiter)) != std::string::npos) {
						token = line.substr(0, pos);
						cav_pos.at(indexY++).y() = std::stod(token);
						line.erase(0, pos + delimiter.length());
					}
					cav_pos.at(indexY++).y() = std::stod(line);
				}

				string tmpZ = _js.value("cav_posZ", string());
				std::istringstream stmZ(tmpZ);
				int indexZ = 0;
				while(std::getline(stmZ, line)) {
					std::string delimiter = " ";
					size_t pos = 0;
					std::string token;
					while ((pos = line.find(delimiter)) != std::string::npos) {
						token = line.substr(0, pos);
						cav_pos.at(indexZ++).z() = std::stod(token);
						line.erase(0, pos + delimiter.length());
					}
					cav_pos.at(indexZ++).z() = std::stod(line);
				}

				mw = _js["mw"] | 1.0;
				Ninit = _js["Ninit"] | 0.0;
				radius = (_js["r"] | 0.0) * 1.0_angstrom;
				sigma = (_js["sigma"] | 2 * radius) * 1.0_angstrom;
				radius = 0.5 * sigma;
				area = 4.0*acos(-1.0)*radius*radius;
				charge = _js["q"] | 0.0;

				if( ( cap_radiusV.size() > 0 ) || ( cap_radius > 1e-6 ) ) {
					area = 4.0*acos(-1.0)*radius*radius;
					for(unsigned int i = 0; i < cap_centerV.size(); i++)
						area = area - std::acos(-1.)/cap_centerV.at(i)*(cap_centerV.at(i)-radius-cap_radiusV.at(i))*(cap_centerV.at(i)-radius-cap_radiusV.at(i))*(cap_radiusV.at(i) - radius);
					if(cap_centerV.size() == 0) {
						area = 4.0*acos(-1.0)*radius*radius;
						area = area - std::acos(-1.)/cap_center*(cap_center-radius-cap_radius)*(cap_center-radius-cap_radius)*(cap_radius - radius);
					}
				}
				
				if(charge_distribution) {
				  charge_density = _js["q_rho"] | 0.0;
				  charge = charge_density*area;
				}

				tfe = _js["tfe"] | 0.0;
				alphax = (_js["alphax"] | 0.0) * std::pow(radius, 3);
				string unit = _js["alphax_unit"] | string("unitless");
				if ( unit == "angstrom^3" )
					alphax /= std::pow(radius, 3);

				// spherocylindrical properties
				half_len = 0.5 * (_js["len"] | 0.0);
				patchtype = _js["patchtype"] | 0.0;
				pswitch = _js["patchswitch"] | 0.0;
				pdis = _js["patchdistance"] | 0.0;
				pangl = (_js["patchangle"] | 0.0) * 1._deg;
				panglsw = (_js["patchangleswitch"] | 0.0) * 1._deg;
				chiral_angle = (_js["patchchiralangle"] | 0.0) * 1._deg;

				betaC = _js["betaC"] | pc::infty;
				betaD = _js["betaD"] | pc::infty;
				betaQ = _js["betaQ"] | pc::infty;
			}
	};

	/**
	 * @brief Class for loading and storing atomic properties
	 * 
	 * This will load atom properties from a JSON object and store them in a
	 * vector of `AtomData`.
	 * Due to compatibility, a default fallback property is added upon construction
	 * (index 0).
	 *
	 * For example:
	 *
	 *     {
	 *       "Na" : { "q": 1.0, "r":1.9, "mw":22.99 },
	 *       "Cl" : { "q":-1.0, "r":1.7, "mw":35.45 }
	 *     }
	 *
	 * Code example:
	 *
	 *     AtomMap a;
	 *     a.include("atoms.json");   // load parameters
	 *
	 *     double s=a["Na"].sigma;        // get LJ sigma for sodium
	 *
	 *     PointParticle p;               // Copy properties to particle
	 *     p = a["Cl"];
	 *     std::cout << a[p.id].activity; // Get property via particle id
	 *
	 *     for (auto &i : a)              // Loop over all atoms
	 *       std::cout << i.charge;
	 *
	 * Note that faunus currently has a global instance of `AtomMap`,
	 * simply named `atom`. This can be accessed from anywhere.
	 * 
	 */

	class AtomMap : public PropertyVector<AtomData>
	{
		public:
			typedef PropertyVector<AtomData> base;

			AtomMap()
			{
				base::name = "Atom Properties";

				// to be removed! This is here only to be compatible with test
				// data that expects an initial dummy atom with id 0.
				Tmjson j;
				j["unk"] = {{"dummy", 0}};
				auto it = j.begin();
				push_back(it); // add default property
			}

	};

	extern AtomMap atom; //!< Global instance of AtomMap - can be accessed from anywhere

}//namespace
#endif
