#ifndef FAUNUS_ENERGY_H
#define FAUNUS_ENERGY_H

#include <unordered_map>

#ifndef SWIG
#include <faunus/common.h>
#include <faunus/point.h>
#include <faunus/group.h>
#include <faunus/inputfile.h>
#include <faunus/space.h>
#include <faunus/textio.h>
#include <faunus/potentials.h>
#include <faunus/auxiliary.h>
#include <faunus/bonded.h>
#include <faunus/multipole.h>
#include <Eigen/Eigenvalues>

#endif
#ifdef ENABLE_MPI
#include <faunus/mpi.h>
#endif

#ifdef FAU_POWERSASA
#include <power_sasa.h>
#endif

namespace Faunus
{
/**
   * @brief Classes/templates that calculate the energy of particles, groups, system.
   *
   * This namespace containes classes and templates for calculating
   * energies of the system. This can be pair interactions, external
   * potentials, long range corrections, constraints etc.
   * All energy calculation classes inherit from the base class
   * `Energy::Energybase` and thus have a common interface.
   */
  namespace Energy
  {

/**
     *  @brief Base class for energy evaluation
     *
     *  This base class defines functions for evaluating interactions
     *  between particles, groups, external potentials etc.
     *  By default all energy functions return ZERO and derived classes are
     *  expected only to implement functions relevant for certain
     *  properties. I.e. a derived class for non-bonded interactions are
     *  not expected to implement `i_internal()`, for example.
     *
     *  @note All energy functions must return energies in units of kT.
     */
    template<class Tspace>
    class Energybase
    {
    public:
        typedef typename Tspace::ParticleType Tparticle;
        typedef typename Tspace::GeometryType Tgeometry;
        typedef Space<Tgeometry, Tparticle> SpaceType;
        typedef typename SpaceType::ParticleVector Tpvec;

    protected:
        string jsondir; // inputmap section
        char w; //!< Width of info output
        SpaceType *spc;

	    typename Tspace::GeometryType* geo;
        virtual std::string _info()=0;

        /** @brief Determines if given particle vector is the trial vector */
        template<class Tpvec>
        bool isTrial( const Tpvec &p ) const { return (&p == &spc->trial); }
        
        bool isGeometryTrial(typename Tspace::GeometryType &g) const { return (&g==&spc->geo_trial); }

    public:
        string name;  //!< Short informative name

        virtual ~Energybase() {}

        Energybase( const string &dir = "" ) : jsondir(dir), w(25), spc(nullptr), geo()
        {
            if ( jsondir.empty())
                jsondir = "energy";
        }

        virtual void setSpace( Tspace &s )
        {
            spc = &s;
            setGeometry(s.geo);
        }

        virtual Tspace &getSpace()
        {
            assert(spc != nullptr);
            return *spc;
        }
        
        virtual void setGeometry(typename Tspace::GeometryType &g)
        {
            geo=&g;
        }
	  
        virtual typename Tspace::GeometryType& getGeometry() 
	{
	  assert(geo!=nullptr);
          return *geo;
        }
        
        virtual double p2p( const Tparticle &, const Tparticle & ) // Particle-particle energy
        { return 0; }

        virtual Point f_p2p( const Tparticle &, const Tparticle & ) // Particle-particle force
        { return Point(0, 0, 0); }

        virtual double all2p( const Tpvec &, const Tparticle & )  // Particle-Particle vector energy
        { return 0; }

        virtual double i2i( const Tpvec &, int, int )           // i'th particle with j'th particle
        { return 0; }

        virtual double i2g( const Tpvec &, Group &, int )       // i'th particle with group
        { return 0; }

        virtual double i2all( Tpvec &, int )              // i'th particle with all other particles
        { return 0; }

        virtual double i_external( const Tpvec &, int )         // internal energy of i'th particle
        { return 0; }

        virtual double i_internal( const Tpvec &, int )         // External energy of i'th particle
        { return 0; }

        virtual double p_external( const Tparticle & )   // External energy of particle
        { return 0; }

        /* @brief Total energy of i'th = i2all+i_external+i_internal */
        double i_total( Tpvec &p, int i ) { return i2all(p, i) + i_external(p, i) + i_internal(p, i); }

        /* @brief Group to group */
        virtual double g2g( const Tpvec &, Group &, Group & )     // Group-Group energy
        { return 0; }

        /* @brief Group in vector 1 with group in vector 2 */
        virtual double g1g2( const Tpvec &, Group &, const Tpvec &, Group & ) { return 0; }

        virtual double g_external( const Tpvec &, Group & )      // External energy of group
        { return 0; }

        virtual double g_internal( const Tpvec &, Group & )      // Internal energy of group
        { return 0; }

        virtual double v2v( const Tpvec &, const Tpvec & )       // Particle vector-Particle vector energy
        { return 0; }

        virtual double external( const Tpvec & )                // External energy - pressure, for example.
        { return 0; }

        virtual double update( bool= true )                     // Bool is acceptance/rejection of previous move
        { return 0; }

        /** @brief Update energy function due to Change */
        virtual double updateChange( const typename Tspace::Change &c ) { return 0; }

        virtual void field( const Tpvec &, Eigen::MatrixXd & ) //!< Calculate electric field on all particles
        {}

        virtual double systemEnergy( const Tpvec &p )
        {
            double u_pair = 0.0;
            double u = external(p);
            for ( auto g : spc->groupList())
                if (!g->empty())
                    u += g_external(p, *g) + g_internal(p, *g);
            for ( auto i = spc->groupList().begin(); i != spc->groupList().end(); ++i )
                for ( auto j = i; ++j != spc->groupList().end(); )
                    u_pair += g2g(p, **i, **j);
            return u + u_pair;
        }

        virtual double g2All(const Tpvec & p, const std::map<int, vector<int>>& mg)
        {
            double du = 0;
            auto &g = spc->groupList();

            for ( auto &m : mg ) // loop over all moved groups
            {
                size_t i = size_t(m.first);                       // index of moved group

                // Calculate energy moved <-> static groups
                for ( size_t j = 0; j < g.size(); j++ ) {           // loop over through all groups
                    if ( mg.count(j) == 0 ) {                // If group j is not in mvGroup
                        du += g2g(p, *g[i], *g[j]);           // moved group<->static groups
                        if ( du == pc::infty )
                            return pc::infty;   // early rejection
                    }
                }
            }

            // Calculate energy moved <-> moved
            for ( auto i = mg.begin(); i != mg.end(); i++ )
            {
                for ( auto j = i; j != mg.end(); j++ ) // MIKAEL should it not be j = (++mg.begin()) instead of j = i, why include interaction with self
                {
                    auto _i = i->first;
                    auto _j = j->first;
                    du += g2g(p, *g[_i], *g[_j]);
                    if ( du == pc::infty )
                        return pc::infty;   // early rejection
                }
            }

            return du;
        }

        inline virtual std::string info()
        {
            assert(!name.empty() && "Energy name cannot be empty");
            if ( _info().empty())
                return string();
            return textio::header("Energy: " + name) + _info();
        }
    };

    /**
     *  \brief Energy matrix - allows acceleration of Metropolis algorithm up to 2x by storing pair-wise enegies
     *  \param EType - set type for energy storage in energy matrix - e.g. double, float, half_float
     *
     *  Usage:
     *
     * Energy::EnergyMatrix<double, Tspace, Energy::NonbondedCutg2g<Tspace,Tpairpot> > nonEM(mcp);
     *
     * NOTE: Currently only works for rigid many-particle species, i.e. All groups of GroupList needs to be molecular.
     *
     */
    template<typename EType, class Tspace, class Base>
      class EnergyMatrix : public Base {
    private:
        std::vector< std::vector<EType> > eMatrix;                 /// \brief Energy matrix, elements only valid for i > j indexes
        std::vector< std::vector<EType> > eMatrix2;                /// \brief Energy matrix duplicate, useful for certain move(isobaric for example)
        std::unordered_map<Group* , unsigned int> groupMap;   /// \brief Mapping unique_Group_ID==KEY to index in Energy matrix, groupMap[group.id] = index in eMatrix

        typedef Energy::Energybase<Tspace> SuperBase;
        typedef typename Tspace::ParticleType Tparticle;
        typedef typename Tspace::GeometryType Tgeometry;
        typedef typename Space<Tgeometry, Tparticle>::ParticleVector Tpvec;

        using SuperBase::spc;
        using SuperBase::isTrial;

    public:
        std::vector<double> changes;    /// \brief Temporarily stored changes to the Energy matrix generated in each trial configuration energy calculation

        bool all = false;       /// \brief true signifies that we need to swap EMs
        std::map<int, vector<int>> multi; /// \brief !empty() - signifies that we modified a series of particles and its interactions
        int single = -1;        /// \brief true signifies that we modified a single particle and its interactions

        std::vector< std::vector<EType> >* energyMatrix;       /// \brief We are using eMatrix via a pointer for easy swapping
        std::vector< std::vector<EType> >* energyMatrixTrial;

        ///  The simulation starts by systemEnergy() call to set up for drift calculation, this used for EM initiation
        ///
        ///
        ///  @Warning Mikael - This is potentialy problematic since the proper initiation of EM is depended on systemEnergy call outside of this class
        ///                  - Maybe modify ufunction and add another Init function call to it
        ///
        ///
        bool firstGlobal=true;  /// \brief Used to distinguish first systemEnergy method call for EM initiation

        /**
         * @brief EnergyMatrix - Constructor, allocates eMatrix and eMatrix2
         */
        EnergyMatrix(Tmjson &j) : Base(j)
        {
            unsigned int size = 0;
            energyMatrix = &eMatrix;
            energyMatrixTrial = &eMatrix2;

            try{
                changes.reserve(1024 + size);

                energyMatrix->resize( size ); // resize EMs so it has minimal space to properly store all i > j elements
                for(unsigned int i=0; i < size; i++) {
                    energyMatrix->operator [](i).resize( i );
                }

                energyMatrixTrial->resize( size );
                for(unsigned int i=0; i < size; i++) {
                    energyMatrixTrial->operator [](i).resize( i );
                }
            } catch(std::bad_alloc& bad) {
                fprintf(stderr, "\nTOPOLOGY ERROR: Could not allocate memory for Energy matrix");
                exit(1);
            }
        }

        double g2g( const Tpvec &p, Group &g1, Group &g2 )
        {
            if(isTrial(p)) {
                return Base::g2g(p, g1, g2);
            } else {
                if( groupMap[&g1] > groupMap[&g2] )
                    return (*energyMatrix)[groupMap[&g1]][groupMap[&g2]];
                else
                    return (*energyMatrix)[groupMap[&g2]][groupMap[&g1]];
            }
        }

        double g1g2( const Tpvec &p1, Group &g1, const Tpvec &p2, Group &g2 )
        {
            if(isTrial(p1) || isTrial(p2)) {
                return Base::g1g2(p1, g1, p2, g2);
            } else {
                if( groupMap[&g1] > groupMap[&g2] )
                    return (*energyMatrix)[groupMap[&g1]][groupMap[&g2]];
                else
                    return (*energyMatrix)[groupMap[&g2]][groupMap[&g1]];
            }
        }

        double init( const Tpvec &p )
        {
            cout << "\n...Initiating Energy Matrix...\n" << endl;
            double u = 0.0;

            resizeEMatrix(spc->molTrack.size());
            firstGlobal = false;

            // initialize energy matrix
            if(!energyMatrix->empty()) {
                auto map = spc->molTrack.getMap();

                initGroupMap();

                //
                // Equivalent of doing 2 for cycles:
                //
                //  for( i=1; i < size; ++i )
                //      for( j=0; j < i; ++j )
                //
                //  4 particles -> 0, 1, 2, 3
                //  Looping(i,j) : (1,0), (2,0), (2,1), (3,0), (3,1), (3,2)
                //
                unsigned int i=1, j=0;
                for(auto tid_i = map.begin(); tid_i != map.end(); ++tid_i) { // loop over molecule types -> elements of map <KEY = TID, vector<GROUP*>>
                    for(auto group_i = tid_i->second.begin(); group_i != tid_i->second.end(); ++group_i) { // loop over molecules of tid_i type -> elements of vector<GROUP*>
                        if(tid_i == map.begin() && group_i == tid_i->second.begin()) // discart first particle due to i > j ordering
                            ++group_i;

                        for(auto tid_j = map.begin(); tid_j != map.end(); ++tid_j) { // loop over molecule types -> elements of map <KEY = TID, vector<GROUP*>>
                            // loop over molecules of tid_j type -> elements of vector<GROUP*>, end when END of container or reached group_i
                            for(auto group_j = tid_j->second.begin(); ( (group_j != tid_j->second.end()) && (group_j != group_i) ); ++group_j) {
                                assert( i == groupMap[&(**group_i)] );
                                assert( j == groupMap[&(**group_j)] );
                                assert(i > j);
                                (*energyMatrix)[i][j] = Base::g2g(p, **group_i, **group_j );
                                u += (*energyMatrix)[i][j];
                                ++j;
                            }
                        }
                        j=0;
                        ++i;
                    }
                }
            }
            return u;
        }

        /**
         * @brief systemEnergy - Calculate system energy on the basis on Groups
         *
         * This implementation assumes the order of groups in EnergyMatrix is identical to the one in GroupList
         *
         * @param p
         * @return
         */
        double systemEnergy( const Tpvec &p ) override
        {
            double u_pair = 0.0;
            double u = Base::external(p);
            for ( auto g : spc->groupList())
                u += Base::g_external(p, *g) + Base::g_internal(p, *g);

            if(firstGlobal) {
                return u + init(p);
            }

            if(isTrial(p)) {
                all = true;
                if(!energyMatrix->empty()) {
                    auto map = spc->molTrack.getMap();

                    unsigned int i=1, j=0;
                    for(auto tid_i = map.begin(); tid_i != map.end(); ++tid_i) { // loop over molecule types -> elements of map <KEY = TID, vector<GROUP*>>
                        for(auto group_i = tid_i->second.begin(); group_i != tid_i->second.end(); ++group_i) { // loop over molecules of tid_i type -> elements of vector<GROUP*>
                            if(tid_i == map.begin() && group_i == tid_i->second.begin()) // discart first particle due to i > j ordering
                                ++group_i;

                            for(auto tid_j = map.begin(); tid_j != map.end(); ++tid_j) { // loop over molecule types -> elements of map <KEY = TID, vector<GROUP*>>
                                // loop over molecules of tid_j type -> elements of vector<GROUP*>, end when END of container or reached group_i
                                for(auto group_j = tid_j->second.begin(); ( (group_j != tid_j->second.end()) && (group_j != group_i) ); ++group_j) {
                                    assert( i == groupMap[&(**group_i)] );
                                    assert( j == groupMap[&(**group_j)] );
                                    assert(i > j);
                                    (*energyMatrix)[i][j] = Base::g2g(p, **group_i, **group_j );
                                    u_pair += (*energyMatrix)[i][j];
                                    ++j;
                                }
                            }
                            j=0;
                            ++i;
                        }
                    }
                }
            } else {
                // No need to translate through groupMap, sum all elements
                assert(energyMatrix->size() == spc->molTrack.size());
                if(!energyMatrix->empty()) {
                    for ( unsigned int i = 1; i < energyMatrix->size(); ++i ) {
                        for ( unsigned int j = 0; j < i; ++j ) {
                            u_pair += (*energyMatrix)[i][j];
                            assert( (*energyMatrix)[i][j] == g2g(p, *spc->groupList()[i], *spc->groupList()[j]) );
                        }
                    }
                }
            }

            assert(fabs( (u_pair+u) - Base::systemEnergy(p) ) < Base::systemEnergy(p)*1e-9 && "EM system energy significantly different from base");
            return u + u_pair;
        }

        inline double update(bool acc) override {
            if(acc) {
                modify();
            } else {
                all = false;
                single = -1;
                multi.clear();
            }
            return 0.0;
        }

        /**
         * @brief g2All Calculate group to group energies between groups of mg (moved groups) and rest of groups
         *
         * This implementation assumes the order of groups in EnergyMatrix is identical to the one in GroupList
         *
         * @param p
         * @param mg
         * @return
         */
        double g2All(const Tpvec & p, const std::map<int, vector<int>>& mg) override
        {
            double du = 0;
            auto &g = spc->groupList();
            int count = 0;

            if(isTrial(p)) {
                changes.resize(g.size() * mg.size());
                multi = std::map<int, vector<int>>(mg);

                for ( auto &m : mg ) // loop over all moved groups
                {
                    size_t i = size_t(m.first);                       // index of moved group
                    assert(g[i]->isMolecular());

                    // Calculate energy moved <-> static groups
                    for ( size_t j = 0; j < g.size(); j++ ) {           // loop over through all groups
                        if ( mg.count(j) == 0 ) {                // If group j is not in mvGroup
                            changes[count] = g2g(p, *g[i], *g[j]);
                            du += changes[count];           // moved group<->static groups
                            ++count;
                            if ( du == pc::infty )
                                return pc::infty;   // early rejection
                        }
                    }
                }

                // Calculate energy moved <-> moved
                for ( auto i = mg.begin(); i != mg.end(); i++ )
                {
                    for ( auto j = (++mg.begin()); j != mg.end(); j++ )
                    {
                        auto _i = i->first;
                        auto _j = j->first;
                        changes[count] = g2g(p, *g[_i], *g[_j]);
                        du += changes[count];
                        ++count;
                        if ( du == pc::infty )
                            return pc::infty;   // early rejection
                    }
                }
            } else {
                for ( auto &m : mg ) {
                    for ( auto &m : mg ) {
                        size_t i = size_t(m.first);
                        assert(g[i]->isMolecular());

                        for ( size_t j = 0; j < g.size(); j++ ) {
                            if ( mg.count(j) == 0 ) {
                                if(i > j) {
                                    du += (*energyMatrix)[i][j];
                                } else {
                                    du += (*energyMatrix)[j][i];
                                }
                            }
                        }
                    }
                    for ( auto i = mg.begin(); i != mg.end(); i++ )
                    {
                        for ( auto j = (++mg.begin()); j != mg.end(); j++ ) // Can't use energy matrix for calculation of internal energy (j = i as in the rest of energy implementation)
                        {
                            auto _i = i->first;
                            auto _j = j->first;
                            if(_i > _j) {
                                du += (*energyMatrix)[_i][_j];
                            }
                            else {
                                du += (*energyMatrix)[_j][_i];
                            }
                        }
                    }
                }
            }
            return du;
        }

        void resizeEMatrix(unsigned int size) {
            assert(size >= 0);

            energyMatrix->resize(size);
            for(unsigned int i=0; i < size; i++) {
                energyMatrix->at(i).resize( i );
            }

            energyMatrixTrial->resize(size);
            for(unsigned int i=0; i < size; i++) {
                energyMatrixTrial->at(i).resize( i );
            }
        }

        inline void modify() {
            if(!multi.empty()) {
                fixEMatrixMulti(multi);
                multi.clear();
                return;
            }
            if(single != -1) {
                fixEMatrixSingle(single);
                single = -1;
                return;
            }
            if(all) {
                std::swap(energyMatrix, energyMatrixTrial);
                all = false;
                return;
            }
            std::cout << "EnergyMatrix::Modify() Error, move not recorded" << std::endl;
        }

    private:
        /**
         * @brief initGroupMap - Initialize groupMap
         *
         * Take the adress of a Group (&Group) and map its position in GroupList
         *
         * SystemEnergy and g2All functions depends on the right ordering of Group in groupMap
         *
         * e.g. First group mapped to 0, second to 1, ...
         */
        void initGroupMap()
        {
            auto map = spc->molTrack.getMap();
            int i=0;
            for(auto g = map.begin(); g != map.end(); ++g) { // initiate mapping
                for(auto gg = g->second.begin(); gg != g->second.end(); ++gg) {
                    groupMap.insert( std::make_pair(&(**gg), i) );
                    ++i;
                }
            }
        }

        void show() {
            std::cout << "Energy matrix: " << std::endl;
            for(unsigned int i=1; i<energyMatrix->size(); ++i) {
                for(unsigned int j=0; j<i; ++j)
                    std::cout << std::setw(7) << std::setprecision(4) << (*energyMatrix)[i][j] << " ";
                std::cout << std::endl;
            }
        }

        /**
         * @brief fixEMatrixSingle - fix interaction energies when moving a single particle/molecule in Energy matrix
         * @param target - id of element whose inteactions changed
         */
        void fixEMatrixSingle(unsigned int target) {
            assert(changes.size() == energyMatrix->size());
            for(unsigned int i=0; i < target; i++) {
                energyMatrix->operator [](target)[i] = changes[i];
            }
            for(unsigned int i=target+1; i < energyMatrix->size(); i++) {
                energyMatrix->operator [](i)[target] = changes[i];
            }
        }

        /**
         * @brief fixEMatrixMultin - fix interaction energies when moving a multiple particles/groups in Energy matrix
         */
        void fixEMatrixMulti(std::map<int, vector<int>>& multi) {
            auto &g = spc->groupList();
            int count = 0;
            for ( auto &m : multi )
            {
                size_t i = size_t(m.first);
                for ( size_t j = 0; j < g.size(); j++ ) {
                    if ( multi.count(j) == 0 ) {
                        if(i > j)
                            (*energyMatrix)[i][j] = changes[count];
                        else
                            (*energyMatrix)[j][i] = changes[count];
                        ++count;
                    }
                }
            }

            for ( auto i = multi.begin(); i != multi.end(); i++ )
            {
                for ( auto j = (++multi.begin()); j != multi.end(); j++ )
                {
                    auto _i = i->first;
                    auto _j = j->first;
                    if(_i > _j)
                        (*energyMatrix)[_i][_j] = changes[count];
                    else
                        (*energyMatrix)[_j][_i] = changes[count];
                    ++count;
                }
            }
        }
    };

/**
     * @brief Add two energy classes together
     */
    template<class T1, class T2>
    class CombinedEnergy : public Energybase<typename T1::SpaceType>
    {
    private:
        string
        _info() override { return first.info() + second.info(); }

        typedef typename T1::SpaceType Tspace;
        typedef Energybase<Tspace> Tbase;
        typedef typename Tbase::Tparticle Tparticle;
        typedef typename Tbase::Tpvec Tpvec;

    public:
        T1 first;
        T2 second;

        auto tuple() -> decltype(std::tuple_cat(first.tuple(), second.tuple()))
        {
            return std::tuple_cat(first.tuple(), second.tuple());
        }

        CombinedEnergy( const T1 &a, const T2 &b ) : first(a), second(b) {}

        string info() override { return _info(); }

        void setSpace( typename T1::SpaceType &s ) override
        {
            first.setSpace(s);
            second.setSpace(s);
            Tbase::setSpace(s);
        }
        
        void setGeometry(typename T1::SpaceType::GeometryType &g) override {
          first.setGeometry(g);
          second.setGeometry(g);
          Tbase::setGeometry(g);
        } 

        double p2p( const Tparticle &a, const Tparticle &b ) override { return first.p2p(a, b) + second.p2p(a, b); }

        Point f_p2p( const Tparticle &a, const Tparticle &b ) override
        {
            return first.f_p2p(a, b) + second.f_p2p(a, b);
        }

        double all2p( const Tpvec &p, const Tparticle &a ) override { return first.all2p(p, a) + second.all2p(p, a); }

        double i2i( const Tpvec &p, int i, int j ) override { return first.i2i(p, i, j) + second.i2i(p, i, j); }

        double i2g( const Tpvec &p, Group &g, int i ) override { return first.i2g(p, g, i) + second.i2g(p, g, i); }

        double i2all( Tpvec &p, int i ) override { return first.i2all(p, i) + second.i2all(p, i); }

        double i_external( const Tpvec &p, int i ) override { return first.i_external(p, i) + second.i_external(p, i); }

        double i_internal( const Tpvec &p, int i ) override { return first.i_internal(p, i) + second.i_internal(p, i); }

        double g2g( const Tpvec &p, Group &g1, Group &g2 ) override
        {
            return first.g2g(p, g1, g2) + second.g2g(p, g1, g2);
        }

        double g1g2( const Tpvec &p1, Group &g1, const Tpvec &p2, Group &g2 ) override
        {
            return first.g1g2(p1, g1, p2, g2) + second.g1g2(p1, g1, p2, g2);
        }

        double g_external( const Tpvec &p, Group &g ) override
        {
            return first.g_external(p, g) + second.g_external(p, g);
        }

        double g_internal( const Tpvec &p, Group &g ) override
        {
            return first.g_internal(p, g) + second.g_internal(p, g);
        }

        double external( const Tpvec &p ) override { return first.external(p) + second.external(p); }

        double update( bool b ) override { return first.update(b) + second.update(b); }

        double updateChange( const typename Tspace::Change &c ) override
        {
            return first.updateChange(c) + second.updateChange(c);
        }

        double v2v( const Tpvec &p1, const Tpvec &p2 ) override { return first.v2v(p1, p2) + second.v2v(p1, p2); }

        void field( const Tpvec &p, Eigen::MatrixXd &E ) override
        {
            first.field(p, E);
            second.field(p, E);
        }
    };

/**
     * @brief Operator to conveniently add two energy classes together
     */
    template<class T1, class T2,
        class = typename
        std::enable_if<std::is_base_of<Energybase<typename T1::SpaceType>, T1>::value>::type,
        class = typename
        std::enable_if<std::is_base_of<Energybase<typename T1::SpaceType>, T2>::value>::type>
    CombinedEnergy<T1, T2> &operator+( const T1 &u1, const T2 &u2 ) { return *(new CombinedEnergy<T1, T2>(u1, u2)); }

    template<class Tgeometry> struct FunctorScalarDist
    {
        template<class Tparticle>
        inline double operator()( const Tgeometry &geo, const Tparticle &a, const Tparticle &b ) const
        {
            return geo.sqdist(a, b);
        }
    };

    template<class Tgeometry> struct FunctorVectorDist
    {
        template<class Tparticle>
        Point operator()( const Tgeometry &geo, const Tparticle &a, const Tparticle &b ) const
        {
            return geo.vdist(a, b);
        }
    };

/**
     * @brief Energy class for non-bonded interactions.
     *
     * `Tpairpot` is expected to be a pair potential with the following
     * properties:
     *
     * - `Tpairpot(const Tmjson&)`
     * - `double Tpairpot::operator()(const particle&, const particle&, double sqdist))`
     *
     * For a list of implemented potentials, see the `Faunus::Potential`
     * namespace.
     */
    template<class Tspace, class Tpairpot>
    class Nonbonded : public Energybase<Tspace>
    {
    protected:
        string
        _info() override { return pairpot.info(25); }

        typedef Energybase<Tspace> Tbase;
        typedef typename Tbase::Tparticle Tparticle;
        typedef typename Tbase::Tpvec Tpvec;

    public:
        typename Tspace::GeometryType geo;
        Tpairpot pairpot;

        Nonbonded(
            Tmjson &j,
            const string &sec = "nonbonded" ) : pairpot(j["energy"][sec])
        {

            assert(!j["energy"][sec].is_null());

            static_assert(
                std::is_base_of<Potential::PairPotentialBase, Tpairpot>::value,
                "Tpairpot must be a pair potential");
            Tbase::name = "Nonbonded N" + textio::squared + " - " + pairpot.name;
        }

        auto tuple() -> decltype(std::make_tuple(this))
        {
            return std::make_tuple(this);
        }

        void setSpace( Tspace &s ) override
        {
            geo = s.geo;
            Tbase::setSpace(s);
            pairpot.setSpace(s);
        }

        //!< Particle-particle energy (kT)
        inline double p2p( const Tparticle &a, const Tparticle &b ) override
        {
            return pairpot(a, b, geo.sqdist(a, b));
        }

        //!< Particle-particle force (kT/Angstrom)
        inline Point f_p2p( const Tparticle &a, const Tparticle &b ) override
        {
            auto r = geo.vdist(a, b);
            return pairpot.force(a, b, r.squaredNorm(), r);
        }

        double all2p( const Tpvec &p, const Tparticle &a ) override
        {
            double u = 0;
            for ( auto &b : p )
                u += pairpot(a, b, geo.sqdist(a, b));
            return u;
        }

        double i2i( const Tpvec &p, int i, int j ) override
        {
            return pairpot(p[i], p[j], geo.sqdist(p[i], p[j]));
        }

        double i2g( const Tpvec &p, Group &g, int j ) override
        {
            double u = 0;
            if ( !g.empty())
            {
                int len = g.back() + 1;
                if ( g.find(j))
                {   //j is inside g - avoid self interaction
                    for ( int i = g.front(); i < j; i++ )
                        u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j]));
                    for ( int i = j + 1; i < len; i++ )
                        u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j]));
                }
                else              //simple - j not in g
                    for ( int i = g.front(); i < len; i++ )
                        u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j]));
            }
            return u;
        }

        double i2all( Tpvec &p, int i ) override
        {
            assert(i >= 0 && i < int(p.size()) && "index i outside particle vector");
            double u = 0;
            int n = (int) p.size();
            for ( int j = 0; j != i; ++j )
                u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j]));
            for ( int j = i + 1; j < n; ++j )
                u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j]));
            return u;
        }

        double g2g( const Tpvec &p, Group &g1, Group &g2 ) override
        {
            double u = 0;
            if ( !g1.empty())
                if ( !g2.empty())
                {
                    // IN CASE ONE GROUP IS A SUBGROUP OF THE OTHER
                    if ( g1.find(g2.front()))
                        if ( g1.find(g2.back()))
                        {  // g2 is a subgroup of g1
                            assert(g1.size() >= g2.size());
                            for ( int i = g1.front(); i < g2.front(); i++ )
                                for ( auto j : g2 )
                                    u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j]));
                            for ( int i = g2.back() + 1; i <= g1.back(); i++ )
                                for ( auto j : g2 )
                                    u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j]));
                            return u;
                        }
                    if ( g2.find(g1.front()))
                        if ( g2.find(g1.back()))
                        {  // g1 is a subgroup of g2
                            assert(g2.size() >= g1.size());
                            for ( int i = g2.front(); i < g1.front(); i++ )
                                for ( auto j : g1 )
                                    u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j]));
                            for ( int i = g1.back() + 1; i <= g2.back(); i++ )
                                for ( auto j : g1 )
                                    u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j]));
                            return u;
                        }

                    // IN CASE BOTH GROUPS ARE INDEPENDENT (DEFAULT)
                    int ilen = g1.back() + 1, jlen = g2.back() + 1;
#pragma omp parallel for reduction (+:u)
                    for ( int i = g1.front(); i < ilen; ++i )
                        for ( int j = g2.front(); j < jlen; ++j )
                            u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j]));
                }
            return u;
        }

        double g1g2( const Tpvec &p1, Group &g1, const Tpvec &p2, Group &g2 ) override
        {
            double u = 0;
            for ( auto i : g1 )
                for ( auto j : g2 )
                    u += pairpot(p1[i], p2[j], geo.sqdist(p1[i], p2[j]));
            return u;
        }

        double g_internal( const Tpvec &p, Group &g ) override
        {
            assert(g.size() <= (int) p.size());
            double u = 0;
            int b = g.back(), f = g.front();
            if ( !g.empty())
                for ( int i = f; i < b; ++i )
                    for ( int j = i + 1; j <= b; ++j )
                        u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j]));
	    u += pairpot.internal(p,g);
            return u;
        }

        double v2v( const Tpvec &p1, const Tpvec &p2 ) override
        {
            double u = 0;
            for ( auto &i : p1 )
                for ( auto &j : p2 )
                    u += p2p(i, j);
            return u;
        }
    };

/**
     * @brief Energy class for non-bonded interactions that excludes bonded pairs.
     *
     * `Tpairpot` is expected to be a pair potential with the following
     * properties:
     *
     * - `Tpairpot(const Tmjson&)`
     * - `double Tpairpot::operator()(const particle&, const particle&, double sqdist))`
     *
     * For a list of implemented potentials, see the `Faunus::Potential`
     * namespace.
     */
    template<class Tspace, class Tpairpot, typename base=Nonbonded<Tspace, Tpairpot>>
    class NonbondedExcl : public base
    {
    protected:
        Table<int> excl;
        using typename base::Tpvec;
        using typename base::Tparticle;

    public:
        Tspace *spc;
        using base::geo;
        using base::pairpot;

        NonbondedExcl(
            Tmjson &j, Table<int> &t,
            const string &sec = "nonbonded" ) : base(j, sec)
        {
            assert(!j["energy"][sec].is_null());

            static_assert(
                std::is_base_of<Potential::PairPotentialBase, Tpairpot>::value,
                "Tpairpot must be a pair potential");
            Energybase<Tspace>::name = "Nonbonded N" + textio::squared + " - " + pairpot.name;
            excl = t;
            for ( int i = 0; i < excl.cols(); ++i )
            {
                for ( int j = 0; j < excl.rows(); ++j )
                {
                    if ( excl(i, j) != excl(j, i))
                    {
                        std::cerr << "Error: matrix has to be symmetric\n";
                        exit(1);
                    }
                }
            }
        }

        double i2i( const Tpvec &p, int i, int j )
        {
            return pairpot(p[i], p[j], geo.sqdist(p[i], p[j])) * excl(i, j);
        }

        double i2g( const Tpvec &p, Group &g, int j ) override
        {
            double u = 0;
            if ( !g.empty())
            {
                int len = g.back() + 1;
                if ( g.find(j))
                {   //j is inside g - avoid self interaction
                    for ( int i = g.front(); i < j; i++ )
                        u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j])) * excl(i, j);
                    for ( int i = j + 1; i < len; i++ )
                        u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j])) * excl(i, j);
                }
                else              //simple - j not in g
                    for ( int i = g.front(); i < len; i++ )
                        u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j])) * excl(i, j);
            }
            return u;
        }

        double i2all( Tpvec &p, int i ) override
        {
            assert(i >= 0 && i < int(p.size()) && "index i outside particle vector");
            double u = 0;
            int n = (int) p.size();
            for ( int j = 0; j != i; ++j )
                u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j])) * excl(i, j);
            for ( int j = i + 1; j < n; ++j )
                u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j])) * excl(i, j);
            return u;
        }

        double g_internal( const Tpvec &p, Group &g ) override
        {
            assert(g.size() <= (int) p.size());
            double u = 0;
            int b = g.back(), f = g.front();
            if ( !g.empty())
                for ( int i = f; i < b; ++i )
                    for ( int j = i + 1; j <= b; ++j )
                        u += pairpot(p[i], p[j], geo.sqdist(p[i], p[j])) * excl(i, j);
            return u;
        }
    };

/**
     * @brief Energy class for non-bonded interactions.
     *
     * `Tpairpot` is expected to be a pair potential with the following properties:
     *
     * - `Tpairpot(const Tmjson&)`
     * - `double Tpairpot::operator()(const particle&, const particle&, double sqdist))`
     *
     * For a list of implemented potentials, see the Faunus::Potential namespace.
     */
    template<class Tspace, class Tpairpot>
    class NonbondedVector : public Energybase<Tspace>
    {
    protected:
        string
        _info() override { return pairpot.info(25); }

        typedef Energybase<Tspace> Tbase;
        typedef typename Tbase::Tparticle Tparticle;
        typedef typename Tbase::Tpvec Tpvec;
        bool groupBasedField;

    public:
        typename Tspace::GeometryType geo;
        Tpairpot pairpot;

        NonbondedVector( Tmjson &in, const string &dir = "energy" )
            : Tbase(dir + "/nonbonded"), pairpot(in["energy"]["nonbonded"])
        {
            static_assert(
                std::is_base_of<Potential::PairPotentialBase, Tpairpot>::value,
                "Tpairpot must be a pair potential");
            Tbase::name = "Nonbonded N" + textio::squared + " - " + pairpot.name;
            groupBasedField = in.value("pol_g2g", false);
        }

        void setSpace( Tspace &s ) override
        {
            geo = s.geo;
            Tbase::setSpace(s);
        }

        auto tuple() -> decltype(std::make_tuple(this))
        {
            return std::make_tuple(this);
        }

        //!< Particle-particle energy (kT)
        inline double p2p( const Tparticle &a, const Tparticle &b ) override
        {
            return pairpot(a, b, geo.vdist(a, b));
        }

        //!< Particle-particle force (kT/Angstrom)
        inline Point f_p2p( const Tparticle &a, const Tparticle &b ) override
        {
            return pairpot.force(a, b, geo.sqdist(a, b), geo.vdist(a, b));
        }

        double all2p( const Tpvec &p, const Tparticle &a ) override
        {
            double u = 0;
            for ( auto &b : p )
                u += pairpot(a, b, geo.vdist(a, b));
            return u;
        }

        double i2i( const Tpvec &p, int i, int j ) override
        {
            return pairpot(p[i], p[j], geo.vdist(p[i], p[j]));
        }

        double i2g( const Tpvec &p, Group &g, int j ) override
        {
            double u = 0;
            if ( !g.empty())
            {
                int len = g.back() + 1;
                if ( g.find(j))
                {   //j is inside g - avoid self interaction
                    for ( int i = g.front(); i < j; i++ )
                        u += pairpot(p[i], p[j], geo.vdist(p[i], p[j]));
                    for ( int i = j + 1; i < len; i++ )
                        u += pairpot(p[i], p[j], geo.vdist(p[i], p[j]));
                }
                else              //simple - j not in g
                    for ( int i = g.front(); i < len; i++ )
                        u += pairpot(p[i], p[j], geo.vdist(p[i], p[j]));
            }
            return u;
        }

        double i2all( Tpvec &p, int i ) override
        {
            assert(i >= 0 && i < int(p.size()) && "index i outside particle vector");
            std::swap(p[0], p[i]);
            double u = 0;
#pragma omp parallel for reduction (+:u)
            for ( int j = 1; j < (int) p.size(); ++j )
                u += pairpot(p[0], p[j], geo.vdist(p[0], p[j]));
            std::swap(p[0], p[i]);
            return u;
        }

        double g2g( const Tpvec &p, Group &g1, Group &g2 ) override
        {
            double u = 0;
            if ( !g1.empty())
                if ( !g2.empty())
                {
                    // IN CASE ONE GROUP IS A SUBGROUP OF THE OTHER
                    if ( g1.find(g2.front()))
                        if ( g1.find(g2.back()))
                        {  // g2 is a subgroup of g1
                            assert(g1.size() >= g2.size());
                            for ( int i = g1.front(); i < g2.front(); i++ )
                                for ( auto j : g2 )
                                    u += pairpot(p[i], p[j], geo.vdist(p[i], p[j]));
                            for ( int i = g2.back() + 1; i <= g1.back(); i++ )
                                for ( auto j : g2 )
                                    u += pairpot(p[i], p[j], geo.vdist(p[i], p[j]));
                            return u;
                        }
                    if ( g2.find(g1.front()))
                        if ( g2.find(g1.back()))
                        {  // g1 is a subgroup of g2
                            assert(g2.size() >= g1.size());
                            for ( int i = g2.front(); i < g1.front(); i++ )
                                for ( auto j : g1 )
                                    u += pairpot(p[i], p[j], geo.vdist(p[i], p[j]));
                            for ( int i = g1.back() + 1; i <= g2.back(); i++ )
                                for ( auto j : g1 )
                                    u += pairpot(p[i], p[j], geo.vdist(p[i], p[j]));
                            return u;
                        }

                    // IN CASE BOTH GROUPS ARE INDEPENDENT (DEFAULT)
                    int ilen = g1.back() + 1, jlen = g2.back() + 1;
#pragma omp parallel for reduction (+:u) schedule (dynamic)
                    for ( int i = g1.front(); i < ilen; ++i )
                        for ( int j = g2.front(); j < jlen; ++j )
                            u += pairpot(p[i], p[j], geo.vdist(p[i], p[j]));
                }
            return u;
        }

        double g1g2( const Tpvec &p1, Group &g1, const Tpvec &p2, Group &g2 ) override
        {
            double u = 0;
            for ( auto i : g1 )
                for ( auto j : g2 )
                    u += pairpot(p1[i], p2[j], geo.vdist(p1[i], p2[j]));
            return u;
        }

        double g_internal( const Tpvec &p, Group &g ) override
        {
            double u = 0;
            const int b = g.back(), f = g.front();
            if ( !g.empty())
                for ( int i = f; i < b; ++i )
                    for ( int j = i + 1; j <= b; ++j )
                        u += pairpot(p[i], p[j], geo.vdist(p[i], p[j]));
            return u;
        }

        double v2v( const Tpvec &p1, const Tpvec &p2 ) override
        {
            double u = 0;
            for ( auto &i : p1 )
                for ( auto &j : p2 )
                    u += p2p(i, j);
            return u;
        }

        /**
               * Calculates the electric field on all particles
               * and stores (add) in the vector `E`.
               *
               * @param p Particle vector
               * @param E Holds field on each particle. Must have N columns.
               */
        void field( const Tpvec &p, Eigen::MatrixXd &E ) override
        {
            assert((int) p.size() == E.cols());

            // Include field only from external molecules
            if ( groupBasedField )
            {
                for ( auto gi : Tbase::spc->groupList()) // loop over all groups
                    for ( auto gj : Tbase::spc->groupList()) // loop over all group
                        if ( gi != gj ) // discard identical groups (addresss comparison)
                            for ( auto i : *gi )  // loop over particle index in 1st group
                                for ( auto j : *gj ) // loop ove
                                    E.col(i) = E.col(i) + pairpot.field(p[j], geo.vdist(p[i], p[j]));
            }
            else
            {
                size_t i = 0;
                for ( auto &pi : p )
                {
                    for ( auto &pj : p )
                        if ( &pi != &pj )
                            E.col(i) = E.col(i) + pairpot.field(pj, geo.vdist(pi, pj));
                    i++;
                }
            }
        }
    };

/**
     * @brief Nonbonded with early rejection for infinite energies
     *
     * Useful for potentials with a hard sphere part
     */
    template<class Tspace, class Tpairpot >
    class NonbondedEarlyReject : public Energy::Nonbonded<Tspace, Tpairpot>
    {
    private:
        typedef Energy::Nonbonded<Tspace, Tpairpot> base;
    public:
        NonbondedEarlyReject( Tmjson &in ) : base(in)
        {
            base::name += " (early reject)";
        }

        double g2g( const typename base::Tpvec &p, Group &g1, Group &g2 ) override
        {
            double u = 0;
            for ( auto i : g1 )
                for ( auto j : g2 )
                {
                    auto _u = base::pairpot(p[i], p[j], base::geo.sqdist(p[i], p[j]));
                    if ( std::isinf(_u))
                        return INFINITY;
                    u += _u;
                }
            return u;
        }

        double g_internal( const typename base::Tpvec &p, Group &g ) override
        {
            double u = 0;
            int b = g.back(), f = g.front();
            if ( !g.empty())
                for ( int i = f; i < b; ++i )
                    for ( int j = i + 1; j <= b; ++j )
                    {
                        auto _u = base::pairpot(p[i], p[j], base::geo.sqdist(p[i], p[j]));
                        if ( std::isinf(_u))
                            return INFINITY;
                        u += _u;
                    }
            return u;
        }

    };

/**
     * @brief Cuts group-to-group interactions at specified mass-center separation
     *
     * For two molecular groups (`Group::isMolecular()==true`) this will invoke
     * a mass center cut-off and return zero if beyond. This is best used
     * in connection with cut-off based pair potentials as a simple alternative
     * to cell lists. In this case the `g2g` cutoff should be set to the
     * particle-particle cutoff plus the maximum radii of the two groups,
     * @f$ r_{cut}^{g2g} = r_{cut}^{p2p} + a_{g1} + a_{g2} @f$.
     *
     * Upon construction the `Tmjson` is searched for the following in
     * section `energy/nonbonded/`:
     *
     * Keyword      |  Description
     * :----------- |  :------------------------------------
     * `cutoff_g2g` |  Cutoff (angstrom) [default: infinity]
     *
     */
    template<class Tspace, class Tpairpot>
    class NonbondedCutg2g : public Energy::Nonbonded<Tspace, Tpairpot>
    {
    private:
        typedef Energy::Nonbonded<Tspace, Tpairpot> base;
        using typename base::Tpvec;
        double rcut2;

        Point getMassCenter( const typename base::Tpvec &p, const Group &g )
        {
            assert(&p == &base::spc->p || &p == &base::spc->trial);
            return (&p == &base::spc->p) ? g.cm : g.cm_trial;
        }

        bool cut( const Tpvec &p, const Group &g1, const Group &g2 )
        {
            return cut(p, g1, p, g2);
        }

        bool cut( const Tpvec &p1, const Group &g1, const Tpvec &p2, const Group &g2 )
        {
            if ( g1.isMolecular())
                if ( g2.isMolecular())
                {
                    Point a = getMassCenter(p1, g1);
                    Point b = getMassCenter(p2, g2);
                    if ( base::geo.sqdist(a, b) > rcut2 )
                        return true;
                }
            return false;
        }

    public:
        bool noPairPotentialCutoff; //!< Set if range of pairpot is longer than rcut (default: false)

        NonbondedCutg2g( Tmjson &j, const string &sec = "nonbonded" ) : base(j, sec)
        {
            noPairPotentialCutoff = false;
            rcut2 = pow(j["energy"][sec]["cutoff_g2g"] | pc::infty, 2);
            base::name += " (g2g cut=" + std::to_string(sqrt(rcut2))
                + textio::_angstrom + ")";
        }

        double g2g( const Tpvec &p, Group &g1, Group &g2 ) override
        {
            return cut(p, g1, g2) ? 0 : base::g2g(p, g1, g2);
        }

        double g1g2( const Tpvec &p1, Group &g1, const Tpvec &p2, Group &g2 ) override
        {
            return cut(p1, g1, p2, g2) ? 0 : base::g1g2(p1, g1, p2, g2);
        }

        double i2g( const Tpvec &p, Group &g, int i ) override
        {
            auto gi = base::spc->findGroup(i);
            return cut(p, g, *gi) ? 0 : base::i2g(p, g, i);
        }

        /**
               * If no pair potential cutoff is applied, `i2all` may need
               * to re-calculate the full g2g interaction as the mass centers
               * may have moved. To activate this behavior set
               * `noPairPotentialCutoff=true`.
               */
        double i2all( typename base::Tpvec &p, int i ) override
        {
            if ( noPairPotentialCutoff )
            {
                auto gi = base::spc->findGroup(i);
                assert(gi != nullptr);
                double u = base::i2g(p, *gi, i); // i<->own group
                for ( auto gj : base::spc->groupList())
                    if ( gj != gi )
                        u += g2g(p, *gi, *gj);
                return u;
            }
            else
            {
                double u = 0;
                auto gi = base::spc->findGroup(i);
                for ( auto gj : base::spc->groupList())
                    if ( !cut(p, *gi, *gj))
                        u += base::i2g(p, *gj, i);
                return u;
            }
        }

        // unfinished
        void field( const typename base::Tpvec &p, Eigen::MatrixXd &E ) override
        {
            assert((int) p.size() == E.cols());
            assert(!"Validate this!");

            // double loop over all groups and test cutoff
            // todo: openmp pragma
            for ( auto gi : base::spc->groupList())
                for ( auto gj : base::spc->groupList())
                    if ( gi != gj )
                        if ( !cut(p, *gi, *gj))
                            for ( int i : *gi )
                                for ( int j : *gj )
                                    E.col(i) += base::pairpot.field(p[j], base::geo.vdist(p[i], p[j]));

            // now loop over all internal particles in groups
            for ( auto g : base::spc->groupList())
                for ( int i : *g )
                    for ( int j : *g )
                        if ( i != j )
                            E.col(i) += base::pairpot.field(p[j], base::geo.vdist(p[i], p[j]));
        }

        auto tuple() -> decltype(std::make_tuple(this))
        {
            return std::make_tuple(this);
        }
    };

/**
      @brief Treats groups as charged monopoles beyond cut-off
      */
    template<class Tspace, class Tpairpot, class Tmppot=Potential::DebyeHuckel>
    class NonbondedCutg2gMonopole : public NonbondedCutg2g<Tspace, Tpairpot>
    {
    private:
        double qscale, Q, R; // charge scaling
        Tmppot dh;
        typedef NonbondedCutg2g<Tspace, Tpairpot> base;

        string
        _info() override
        {
            using namespace Faunus::textio;
            std::ostringstream o;
            o << pad(SUB, 30, "Monopole radius") << R << _angstrom << endl
              << pad(SUB, 30, "Monopole charge") << Q / qscale << endl
              << pad(SUB, 30, "DH charge scaling") << qscale << endl;
            return o.str();
        }

    public:
        NonbondedCutg2gMonopole( Tmjson &in ) : base(in), dh(in)
        {
            base::name += "+MP";
            R = in.value("monopole_radius", 0.0);
            double k = 1 / dh.debyeLength();
            qscale = std::sinh(k * R) / (k * R);
            Q = qscale * in.value("monopole_charge", 0.0);
        }

        double g2g( const typename base::Tpvec &p, Group &g1, Group &g2 ) override
        {
            if ( g1.isMolecular())
                if ( g2.isMolecular())
                {
                    Point a = base::getMassCenter(p, g1);
                    Point b = base::getMassCenter(p, g2);
                    double r2 = base::geo.sqdist(a, b);
                    if ( r2 > base::rcut2 )
                    {
                        typename base::Tparticle p1, p2;
                        p1.charge = Q; //qscale*g1.charge(p);
                        p2.charge = Q; //qscale*g2.charge(p);
                        return dh(p1, p2, r2);
                    }
                }
            return base::base::g2g(p, g1, g2);
        }
    };

/**
     * @brief Class for handling bond pairs
     *
     * Takes care of bonded interactions and can handle mixed bond types. If you create bond BETWEEN
     * groups, make sure to set the `CrossGroupBonds` to `true`.
     *
     * Example:
     *
     *     vector<particle> p(...);            // particle vector
     *     int i=10, j=11;                     // particle index
     *     Energy::Bonded b;
     *     b.add(i, j, Potential::Harmonic(0.1,5.0) );
     *     std::cout << b.info();
     *     double rij2 = ... ;                 // squared distance between i and j
     *     double u = b(i,j)( p[i], p[j], rij2 ); // i j bond energy in kT
     *
     * @date Lund, 2011-2012
     */
    template<class Tspace>
    class Bonded : public Energybase<Tspace>,
                   protected pair_list<std::function<
                       double( const typename Tspace::ParticleType &,
                               const typename Tspace::ParticleType &, double )> >
    {
    private:
        typedef typename Energybase<Tspace>::Tparticle Tparticle;
        typedef typename Energybase<Tspace>::Tpvec Tpvec;

        typedef pair_list <std::function<
            double( const Tparticle &, const Tparticle &, double )>> Tbase;

        typedef std::function<
            Point( const Tparticle &, const Tparticle &, double, const Point & )> Tforce;

        using Energybase<Tspace>::spc;
        std::map<typename Tbase::Tpair, Tforce> force_list;

        string _infolist;

        string _info() override
        {
            using namespace Faunus::textio;
            std::ostringstream o;
            o << pad(SUB, 30, "Look for group-group bonds:")
              << std::boolalpha << CrossGroupBonds << endl
              << indent(SUBSUB) << std::left
              << setw(7) << "i" << setw(7) << "j" << endl;
            return o.str() + _infolist;
        }

        template<class Tpairpot>
        struct ForceFunctionObject
        {
            Tpairpot pot;

            ForceFunctionObject( const Tpairpot &p ) : pot(p) {}

            Point
            operator()( const Tparticle &a, const Tparticle &b,
                        double r2, const Point &r )
            {
                return pot.force(a, b, r2, r);
            }
        };

    public:
        bool CrossGroupBonds; //!< Set to true if bonds cross groups (slower!). Default: false

        Bonded()
        {
            this->name = "Bonded particles";
            CrossGroupBonds = false;
        }

        ~Bonded()
        {
            if ( !Tbase::list.empty())
                IO::writeFile("bondlist.tcl", VMDBonds(Tbase::list));
        }

        void setSpace( Tspace &s ) override
        {
            Energybase<Tspace>::setSpace(s);
            if ( Tbase::mlist.empty())
                add(spc->groupList()); // search for bonds
        }

        auto tuple() -> decltype(std::make_tuple(this))
        {
            return std::make_tuple(this);
        }

        /**
             * @brief Bond energy i with j
             * @todo Optimize by using iterator directly as returned by find.
             */
        double i2i( const Tpvec &p, int i, int j ) override
        {
            assert(i != j);
            auto f = this->list.find(opair<int>(i, j));
            if ( f != this->list.end())
                return f->second(p[i], p[j], spc->geo.sqdist(p[i], p[j]));
            return 0;
        }

        /**
             * @note This will work only for particles contained inside
             * Space main particle vector.
             */
        Point f_p2p( const Tparticle &a, const Tparticle &b ) override
        {
            int i = spc->findIndex(a);
            int j = spc->findIndex(b);
            assert(i >= 0 && j >= 0);
            assert(i < (int) spc->p.size() && j < (int) spc->p.size());
            auto f = force_list.find(opair<int>(i, j));
            if ( f != this->force_list.end())
            {
                auto r = spc->geo.vdist(a, b);
                return f->second(a, b, r.squaredNorm(), r);
            }
            return Point(0, 0, 0);
        }

        //!< All bonds w. i'th particle
        double i2all( Tpvec &p, int i ) override
        {
            assert(i >= 0 && i < (int) p.size()); //debug
            double u = 0;
            auto eqr = this->mlist.equal_range(i);
            for ( auto it = eqr.first; it != eqr.second; ++it )
            {
                int j = it->second; // partner index
                u += this->list[opair<int>(i, j)](
                    p[i], p[j], spc->geo.sqdist(p[i], p[j]));
            }
            return u;
        }

        double total( const Tpvec &p )
        {
            double u = 0;
            for ( auto &m : Tbase::list )
            {
                int i = m.first.first;
                int j = m.first.second;
                assert(i >= 0 && i < (int) p.size() && j >= 0 && j < (int) p.size()); //debug
                u += m.second(p[i], p[j], spc->geo.sqdist(p[i], p[j]));
            }
            return u;
        }

        /**
             * Group-to-group bonds are disabled by default as these are
             * rarely used and the current implementation is rather slow
             * for systems with *many* groups (but very general). To
             * activate `g2g()`, set `CrossGroupBonds=true`.
             *
             * @warning Untested!
             * @todo Possible optimization: swap groups so that `g1<g2`;
             */
        double g2g( const Tpvec &p, Group &g1, Group &g2 ) override
        {
            double u = 0;
            if ( CrossGroupBonds )
                for ( auto i : g1 )
                {
                    auto eqr = this->mlist.equal_range(i);
                    for ( auto it = eqr.first; it != eqr.second; ++it )
                    {
                        int j = it->second; // partner index
                        if ( g2.find(j))
                            u += this->list[opair<int>(i, j)](
                                p[i], p[j], spc->geo.sqdist(p[i], p[j]));
                    }
                }
            return u;
        }

        /**
             * @brief Internal bonds in Group, only
             *
             * Accounts for bonds between particles within a group.
             * Bonds with particles
             * outside the group are skipped and should be accounted for
             * by the g2g() energy function.
             */
        double g_internal( const Tpvec &p, Group &g ) override
        {
            double u = 0;
            if ( this->list.size() > pow(g.size(), 2))
            {
                // small group compared to bond list
                auto end = this->list.end();
                for ( auto i = g.front(); i < g.back(); i++ )
                    for ( auto j = i + 1; j <= g.back(); j++ )
                    {
                        auto it = this->list.find(opair<int>(i, j));
                        if ( it != end )
                            u += it->second(p[i], p[j], spc->geo.sqdist(p[i], p[j]));
                    }
            }
            else
            {
                // big group compared to bond list
                for ( auto &m : this->list )
                {
                    int i = m.first.first, j = m.first.second;
                    if ( g.find(i))
                        if ( g.find(j))
                            u += m.second(p[i], p[j], spc->geo.sqdist(p[i], p[j]));
                }
            }
            return u;
        }

        template<class Tpairpot>
        void add( int i, int j, Tpairpot pot )
        {
            std::ostringstream o;
            o << textio::indent(textio::SUBSUB) << std::left << setw(7) << i
              << setw(7) << j << pot.brief() + "\n";
            _infolist += o.str();
            pot.name.clear();   // potentially save a little bit of memory
            Tbase::add(i, j, pot);// create and add functor to pair list
            force_list[typename Tbase::Tpair(i, j)]
                = ForceFunctionObject<decltype(pot)>(pot);
        }

        /** @brief Add harmonic bond */
        void add( const Faunus::Bonded::BondData &hb )
        {
            if ( hb.type == Faunus::Bonded::BondData::Type::HARMONIC )
                add(hb.index.at(0), hb.index.at(1), Potential::Harmonic(hb.k, hb.req));
        }

        /** @brief Add all bonds found in a list of groups */
        void add( const std::vector<Group *> &groups )
        {
            for ( auto g : groups )
                if ( g->molId < spc->molecule.size())
                    for ( auto b : spc->molecule[g->molId].getBondList())
                    {
                        b.shift(g->front());
                        add(b);
                    }
        }

        /** @brief Get list of bonds */
        auto getBondList() -> decltype(this->list) { return Tbase::getBondList(); }

        /** @brief Reset and clear all bonds */
        void clear()
        {
            _infolist.clear();
            Tbase::clear();
        }

    };

/**
     * @brief Dummy energy class that sums missed energy changes to avoid energy drifts
     *
     * This energy function is designed to be used with Move::Movebase classes
     * that returns energy changes not detectable in the energy drift checkup
     * routines. The idea is simply to sum the energy change discrepancy
     * and treat
     * this as an external potential.
     */
    template<class Tspace>
    class EnergyRest : public Energybase<Tspace>
    {
    private:
        double usum;

        string
        _info() override { return ""; }

    public:
        EnergyRest()
        {
            usum = 0;
            this->name = "Dummy energy (drift calc. aid)";
        }

        //!< Add energy change disrepancy, dU = U(metropolis) - U(as in drift calculation)
        void add( double du ) { usum += du; }

        //!< Dumme rest treated as external potential to whole system
        double external( const typename Energybase<Tspace>::Tpvec &p ) override
        {
            return usum;
        }
    };

/**
     * @brief Energy from external pressure for use in the NPT-ensemble.
     *
     * This will count the number of particles in the system and
     * calculate the energy contribution to the pressure at a given
     * volume and external pressure.
     *
     * The system energy is
     * \f$\beta u = \beta pV - \ln V - N\ln V\f$
     * where the first two terms are returned by `external()` while the last
     * term is obtained by summing `g_external()` over molecular
     * and or atomic groups.
     * If applied on an atomic group, `N` will be set to the number of
     * atoms in the group, while for a molecular group `N=1`.
     *
     * To groups from being counted against `N`, insert them into
     * `ignored`.
     *
     * Upon construction, this will look for input section `moves/isobaric`
     * for the keyword `pressure` (units of mM). See also `Move::Isobaric`.
     *
     * @date Lund, 2011
     */
    template<class Tspace>
    class ExternalPressure : public Energybase<Tspace>
    {
    private:
        typedef typename Energybase<Tspace>::Tpvec Tpvec;
        double P; //!< Pressure, p/kT

        string _info() override
        {
            size_t N, Natom = 0, Nmol = 0;
            for ( auto g : this->getSpace().groupList())
            {
                if ( ignore.count(g) == 0 )
                {
                    if ( g->isAtomic())
                        Natom += g->size();
                    else
                        Nmol += g->numMolecules();
                }
            }
            N = Natom + Nmol;

            std::ostringstream o;
            o << textio::pad(textio::SUB, 15, "Pressure")
              << P / 1.0_mM << " mM = "
              << P / 1.0_Pa << " Pa = "
              << P / 1.0_atm << " atm\n"
              << textio::pad(textio::SUB, 25, "Number of molecules")
              << N << " (" << Nmol << " molecular + " << Natom << " atomic)\n";
            if ( !ignore.empty())
            {
                int cnt = 0;
                o << textio::pad(textio::SUB, 25, "Ignored groups:\n");
                for ( auto g : this->getSpace().groupList())
                    if ( ignore.count(g) != 0 )
                    {
                        o << "     " << cnt++ << " " << g->name << "\n";
                    }
            }
            return o.str();
        }

    public:
        std::set<Group *> ignore; //!< Ignore groups added here

        /**
               * @brief Set pressure
               * @param pressure Pressure in 1/A^3
               */
        void setPressure( double pressure ) { P = pressure; }

        ExternalPressure( Tmjson &j, string sec = "isobaric" )
        {
            this->name = "External Pressure";
            P = (j["moves"][sec].at("pressure").get<double>()) * 1.0_mM;
            if ( P < 0 )
                throw std::runtime_error("Negative pressure forbidden.");
        }

        auto tuple() -> decltype(std::make_tuple(this)) { return std::make_tuple(this); }

        /** @brief External energy working on system. pV/kT-lnV */
        double external( const Tpvec &p ) override
        {
            double V = this->getSpace().geo.getVolume();
            assert(V > 1e-9 && "Volume must be non-zero!");
            return P * V - log(V);
        }

        double g_external( const Tpvec &p, Group &g ) override
        {
            // should this group be ignored?
            if ( ignore.count(&g) != 0 )
                return 0;
            int N = g.numMolecules();
            double V = this->getSpace().geo.getVolume();
            return -N * log(V);
        }
    };
    
    template<class Tspace>
    class ExternalGravity : public Energybase<Tspace>
    {
    private:
        typedef typename Energybase<Tspace>::Tpvec Tpvec;
        double a; //!< Acceleration

        string _info() override
        {
            size_t N, Natom = 0, Nmol = 0;
            for ( auto g : this->getSpace().groupList())
            {
                if ( ignore.count(g) == 0 )
                {
                    if ( g->isAtomic())
                        Natom += g->size();
                    else
                        Nmol += g->numMolecules();
                }
            }
            N = Natom + Nmol;

            std::ostringstream o;
            o << textio::pad(textio::SUB, 15, "Acceleration")
              << a << " m/s^2"
              << textio::pad(textio::SUB, 25, "Number of molecules")
              << N << " (" << Nmol << " molecular + " << Natom << " atomic)\n";
            if ( !ignore.empty())
            {
                int cnt = 0;
                o << textio::pad(textio::SUB, 25, "Ignored groups:\n");
                for ( auto g : this->getSpace().groupList())
                    if ( ignore.count(g) != 0 )
                    {
                        o << "     " << cnt++ << " " << g->name << "\n";
                    }
            }
            return o.str();
        }

    public:
        std::set<Group *> ignore; //!< Ignore groups added here

        void setAcceleration( double acceleration ) { a = acceleration; }

        ExternalGravity( Tmjson &j, string sec = "gravity" )
        {
            this->name = "External Gravity";
	    cout << "Hej" << endl;
            a = (j["moves"][sec].at("acceleration").get<double>());
	    cout << "Hej da" << endl;
            if ( a < 0 )
                throw std::runtime_error("Negative acceleration forbidden.");
        }

        auto tuple() -> decltype(std::make_tuple(this)) { return std::make_tuple(this); }

        double external( const Tpvec &p ) override
        {
	    double u = 0.0;
	    for(unsigned int i = 0; i < p.size(); i++) {
	      u += p[i].mw*p[i].z()*a;
	    }
            return u;
        }

        double g_external( const Tpvec &p, Group &g ) override
        {
	    double u = 0.0;
	    for ( auto i : g )
	      u += this->getSpace().p[i].mw*this->getSpace().p[i].z()*a;
            return u;
        }
    };

    template<class Tspace>
    class ExternalField : public Energybase<Tspace>
    {
    private:
        typedef typename Energybase<Tspace>::Tpvec Tpvec;
	double lB, epsr;
	Point fieldE;

        string _info() override
        {
            std::ostringstream o;
	    o << pad(textio::SUB,15, "Field") << "( " << fieldE.x() << " , " << fieldE.y() << " , " << fieldE.z() << " )" << endl;
	    o << pad(textio::SUB,15, "Dielectric constant") << epsr << endl;
            return o.str();
        }

    public:
        ExternalField( Tmjson &j, string sec = "" )
        {
            this->name = "External Field";
	    auto _j = j["energy"]["nonbonded"];
	    epsr = _j.at("epsr");
	    lB = pc::lB(epsr);
	    fieldE = Point(_j.value("fieldE_x",0.0),_j.value("fieldE_y",0.0),_j.value("fieldE_z",0.0));
        }

        auto tuple() -> decltype(std::make_tuple(this)) { return std::make_tuple(this); }

        double external( const Tpvec &p ) override
        {
	  double Emu = 0;
	  for (unsigned int i = 0; i < p.size(); i++)
	    Emu += fieldE.dot( p[i].mu() ) * p[i].muscalar()*lB;
	  //cout << "Emu: " << Emu << endl;
          return Emu;
        }
    };
    
/**
     * @brief Sum additive external potential on particles.
     * @tparam Texpot External potential typically derived from
     *         `Potential::ExternalPotentialBase`
     */
    template<typename Tspace, typename Texpot>
    class ExternalPotential : public Energybase<Tspace>
    {
    private:
        typedef Energybase<Tspace> base;

        string
        _info() override { return expot.info(); }

    public:
        Texpot expot;

        ExternalPotential( Tmjson &in ) : expot(in)
        {
            base::name = "External Potential (" + expot.name + ")";
        }

        double p_external( const typename base::Tparticle &p ) override
        {
            return expot(p);
        }

        double i_external( const typename base::Tpvec &p, int i ) override
        {
            return p_external(p[i]);
        }

        double g_external( const typename base::Tpvec &p, Group &g ) override
        {
            double u = 0;
            for ( auto i : g )
                u += p_external(p[i]);
            return u;
        }

        /** @brief Field on all particles due to external potential */
        void field( const typename base::Tpvec &p, Eigen::MatrixXd &E ) override
        {
            assert((int) p.size() == E.cols());
            for ( size_t i = 0; i < p.size(); i++ )
                E.col(i) += expot.field(p[i]);
        }

        auto tuple() -> decltype(std::make_tuple(this))
        {
            return std::make_tuple(this);
        }
    };

/**
     * @brief Constrain two group mass centre separations to a distance interval [mindist:maxdist]
     *
     * This energy class will constrain the mass center separation between selected molecule
     * types to a certain interval. This can be useful to sample rare events and the constraint
     * is implemented as an external group energy that return infinity if the mass center
     * separation is outside the defined range.
     * An arbitrary number of pairs can be added
     * although one would rarely want to have more than one.
     * The input is read from json section `energy/cmconstrain` where the key
     * represents pairs of molecule names.
     *
     * ~~~~
     * {
     *   "energy" {
     *     "cmconstrain" : {
     *        "mymol1 mymol2" : { "mindist": 0, "maxdist": 60.0 }
     *     }
     *   }
     * }
     * ~~~~
     *
     * @date Lund, 2012
     * @todo Prettify output
     */
    template<typename Tspace>
    class MassCenterConstrain : public Energybase<Tspace>
    {
    private:
        using Energybase<Tspace>::spc;

        string _info() override
        {
            using namespace Faunus::textio;
            std::ostringstream o;
            o << indent(SUB) << "The following molecules have mass center constraints:\n";
            for ( auto &m : molIdMap )
                o << indent(SUBSUB)
                  << spc->molecule[m.first.first].name << " "
                  << spc->molecule[m.first.second].name << " "
                  << m.second.mindist << "-" << m.second.maxdist << _angstrom << endl;
            return o.str();
        }

        struct data
        {
            double mindist, maxdist;
        };

        std::map<opair < int>, data>
        molIdMap;

    public:

        MassCenterConstrain( Tmjson &j, Tspace &s )
        {
            this->setSpace(s);
            this->name = "Group Mass Center Distance Constraints";
            auto m = j["energy"]["cmconstrain"];
            for ( auto i = m.begin(); i != m.end(); ++i )
            {
                auto molname = textio::words2vec<string>(i.key());
                auto it1 = spc->molecule.find(molname[0]);
                auto it2 = spc->molecule.find(molname[1]);
                if ( molname.size() == 2 )
                    if ( it1 != spc->molecule.end())
                        if ( it2 != spc->molecule.end())
                        {
                            data d = {
                                i.value().at("mindist"),
                                i.value().at("maxdist")
                            };
                            molIdMap[opair<int>(it1->id, it2->id)] = d;
                        }
            }
        }

        auto tuple() -> decltype(std::make_tuple(this))
        {
            return std::make_tuple(this);
        }

        /** @brief Constrain treated as external potential */
        double g_external( const typename Tspace::ParticleVector &p, Group &g1 ) override
        {
            for ( auto &m : molIdMap )         // scan through pair map
                if ( m.first.find(g1.molId))
                {  // and look for molecule id
                    int id1, id2;
                    id1 = g1.molId;
                    if ( id1 == m.first.first )
                        id2 = m.first.second;
                    else
                        id2 = m.first.first;

                    Point cm1 = Geometry::massCenter(spc->geo, p, g1);

                    auto g2vec = spc->findMolecules(id2);
                    for ( auto g2 : g2vec )
                        if ( &g1 != g2 )
                        {
                            Point cm2 = Geometry::massCenter(spc->geo, p, *g2);
                            double r2 = spc->geo.sqdist(cm1, cm2);
                            double min = m.second.mindist;
                            double max = m.second.maxdist;
                            if ( r2 < min * min || r2 > max * max )
                                return pc::infty;
                        }
                }
            return 0;
        }
    };

/**
     * @brief Hydrophobic interactions using SASA
     *
     * This will add a square-well attraction between solvent
     * exposed, hydrophobic sites where the strength is given
     * by a surface tension argument,
     *
     * @f[ U = \sum_i^{N_{contact}} \gamma a_i @f]
     *
     * where @f$a@f$ is the SASA value for a residue and `i` runs
     * over all particles in contact with at least one other
     * hydrophobic and exposed site(s). Note that the value of the surface
     * tension, @f$\gamma@f$, is not comparable to a true, macroscopic
     * tension.
     *
     * Upon construction the following keywords are read from
     * section `energy/hydrophobicsasa`:
     *
     *  Keyword      | Comment
     *  :----------- | :--------------------------------------------------------
     *  `sasafile`   | SASA file - one column (angstrom^2)
     *  `duplicate`  | read SASA file n times (default: 1)
     *  `tension`    | surface tension (default: 0 dyne/cm)
     *  `threshold`  | surface distance threshold (default: 3.0 angstrom)
     *  `uofr`       | set to "yes" if U(r) should be sampled (default: "no")
     *  `dr`         | distance resolution of U(r) (default: 0.5 angstrom)
     *
     *  The `sasafile` should be a single column file with SASA values for each particle in
     *  the system (angstrom^2). Alternatively one can load the file several times using
     *  the `duplicate` option. To generate a SASA file of a protein, use the vmd-sasa.tcl
     *  VMD script, found in the `scripts` folder.
     *
     *  If `uofr` is specified, the hydrophobic interaction
     *  energy is averaged as a function of mass center separation between groups and
     *  saved to disk upon calling the destructor.
     *
     * @todo
     * Currently only `g2g` is implemented and it is assumed that a hydrophobic site on
     * one group is in contact with sites on *maximum one* other group. For large macro
     * molecules this is hardly a problem; if it is, an energy drift should show.
     *
     * @author Kurut / Lund
     * @date Lund, 2014
     * @note Experimental
     */
    template<class Tspace>
    class HydrophobicSASA : public Energybase<Tspace>
    {
    private:
        typedef Energybase<Tspace> base;
        typedef typename Tspace::p_vec Tpvec;
        typedef std::map<int, Average < double> >
        Tuofr;

        std::map<string, Tuofr> uofr; // sasa energy vs. group-2-group distance

        vector<bool> v;      // bool for all particles; true=active
        vector<double> sasa; // sasa for all particles
        double threshold;    // surface-surface distance threshold
        double tension;
        double tension_dyne;
        int duplicate;       // how many times to load sasa file
        string file;         // sasa file

        bool sample_uofr;    // set to true if we should sample U_sasa(r)
        double dr;           // U(r) resolution in r

        /** @brief Fraction of hydrophobic area */
        double fracHydrophobic() const
        {
            double h = 0, noh = 0;
            if ( sasa.size() == base::spc->p.size())
            {
                for ( size_t i = 0; i < sasa.size(); i++ )
                    if ( base::spc->p[i].hydrophobic )
                        h += sasa[i];
                    else
                        noh += sasa[i];
                return h / (h + noh);
            }
            return 0;
        }

        string _info() override
        {
            char w = 25;
            using namespace textio;
            std::ostringstream o;
            o << pad(SUB, w, "SASA file") << file << " (duplicated " << duplicate << " times)\n"
              << pad(SUB, w, "SASA vector size") << sasa.size()
              << " (particle vector = " << base::spc->p.size() << ")\n"
              << pad(SUB, w, "Hydrophobic SASA") << fracHydrophobic() * 100 << percent + "\n"
              << pad(SUB, w, "Threshold") << threshold << _angstrom + "\n"
              << pad(SUB, w, "Surface tension") << tension_dyne << " dyne/cm = " << tension
              << kT + "/" + angstrom + squared + "\n"
              << pad(SUB, w + 1, "Sample " + beta + "U(r)") << std::boolalpha << sample_uofr << "\n"
              << pad(SUB, w + 1, beta + "U(r) resolution") << dr << _angstrom + "\n";
            return o.str();
        }

        void loadSASA( const string &file, unsigned int n )
        {
            double s;
            std::vector<double> t;
            std::ifstream f(file);
            while ( f >> s )
                t.push_back(s);
            sasa.clear();
            while ( n-- > 0 )
                sasa.insert(sasa.end(), t.begin(), t.end());
            if ( sasa.empty() && !file.empty())
                throw std::runtime_error("No SASA data loaded from " + file);
        }

        /** @brief Save U(r) to disk if sampled */
        void save( const string &pfx = textio::prefix )
        {
            for ( auto &i : uofr )
            {
                std::ofstream f(pfx + "Usasa_" + i.first);
                if ( f )
                    for ( auto &j : i.second )
                        f << j.first * dr << " " << j.second << "\n";
            }
        }

    public:

        HydrophobicSASA( Tmjson &j, const string sec = "hydrophobicsasa" ) : base(sec)
        {
            base::name = "Hydrophobic SASA";
            auto m = j["energy"][sec];
            threshold = m.at("threshold");
            tension_dyne = m.at("tension");
            duplicate = m.value("duplicate", 0.0);
            sample_uofr = m.value("uofr", false);
            dr = m.at("dr");
            file = m.at("sasafile");

            // dyne/cm converted to kT/A^2; 1 dyne/cm = 0.001 J/m^2
            tension = tension_dyne * 1e-23 / (pc::kB * pc::T());
            loadSASA(file, duplicate);
        }

        ~HydrophobicSASA() { save(); }

        auto tuple() -> decltype(std::make_tuple(this)) { return std::make_tuple(this); }

        /** @brief Group-to-group energy */
        double g2g( const Tpvec &p, Group &g1, Group &g2 ) override
        {
            if ( fabs(tension) < 1e-6 )
                return 0;
            double dsasa = 0;
            if ( sasa.size() == p.size())
                if ( g1.isMolecular())
                    if ( g2.isMolecular())
                    {
                        v.resize(p.size());
                        std::fill(v.begin(), v.end(), true);
                        for ( auto i : g1 )
                            for ( auto j : g2 )
                                if ( p[i].hydrophobic )
                                    if ( p[j].hydrophobic )
                                        if ( sasa[i] > 1e-3 )
                                            if ( sasa[j] > 1e-3 )
                                                if ( v[i] || v[j] )
                                                {
                                                    double r2 = base::spc->geo.sqdist(p[i], p[j]);
                                                    if ( r2 < pow(threshold + p[i].radius + p[j].radius, 2))
                                                    {
                                                        if ( v[i] )
                                                            dsasa += sasa[i];
                                                        if ( v[j] )
                                                            dsasa += sasa[j];
                                                        v[i] = v[j] = false;
                                                    }
                                                }
                        // analyze
                        if ( sample_uofr && !base::isTrial(p))
                        {
                            double r = base::spc->geo.dist(g1.cm, g2.cm);
                            auto k = std::minmax(g1.name, g2.name);
                            uofr[k.first + "-" + k.second][to_bin(r, dr)] += -tension * dsasa;
                        }
                    }

            return -tension * dsasa; // in kT
        }
    };

/**
     * @brief Reaction Coordinate classes
     */
    namespace ReactionCoordinate
    {

/**
       * @brief Base class for reaction coordinates
       *
       * Derived classes must implement the function operator.
       *
       * Basic JSON keywords:
       *
       * Keyword  | Description
       * :------- | :----------------
       * `min`    | minumum coordinate values (array)
       * `max`    | maximum coordinate values (array)
       * `scale`  | penalty scaling (array)
       * `nstep`  | update frequency for penalty updates, sampling etc. (steps)
       *
       * @note Under construction
       */
      template<class Tspace>
      class ReactionCoordinateBase
      {
      public:
          typedef std::vector<double> Tvec;
      protected:
          Tvec _min, _max, _scale;
          unsigned int _nstep;     // update frequency [steps]
      public:
          ReactionCoordinateBase( Tmjson &j )
          {
              _min = j.at("min").get<Tvec>();
              _max = j.at("max").get<Tvec>();
              _scale = j.at("scale").get<Tvec>();
              _nstep = j.at("nstep");

              if ( (_min.size()!=_max.size() ) || _min.size()!=_scale.size() || _min.size()<1 )
                  throw std::runtime_error(
                      "Reaction coordinate error: min, max, scale must have same length >=1");
          }

          ReactionCoordinateBase() {}

          /** @brief Calculates reaction coordinate */
          virtual Tvec operator()( Tspace &s, typename Tspace::ParticleVector &p )=0;

          size_t dim() const { return _min.size(); }   //!< dimension of coordinate
          const Tvec &min() const { return _min; }     //!< min value(s) for penalty coordinates
          const Tvec &max() const { return _max; }     //!< max value(s) for penalty coordinates
          unsigned int nstep() const { return _nstep; }//!< update frequency (steps)
          const Tvec &scale() const { return _scale; } //!< scaling value value(s) for penalty coordinates

          /** @brief Determines if coordinate is within [min,max] */
          bool inrange( const Tvec &coord ) const
          {
              for ( size_t i = 0; i < coord.size(); ++i )
                  if ( coord[i] < min()[i] || coord[i] > max()[i] )
                      return false;
              return true;
          }
      };

/* @brief Helper class for combining two reaction coordinates */
      template<class Tspace, class base=ReactionCoordinateBase<Tspace> >
      class Combined : public base
      {
      private:
          vector <base> c;
          typedef typename base::Tvec Tvec;
      public:
          Combined( const base &c1, const base &c2 )
          {
              c.push_back(c1);
              c.push_back(c2);
              base::_nstep = std::min(c[0].nstep(), c[1].nstep());
              for ( auto &i : c )
              {
                  base::_min.insert(base::_min.end(), i.min().begin(), i.min().end());
                  base::_max.insert(base::_max.end(), i.max().begin(), i.max().end());
                  base::_scale.insert(base::_scale.end(), i.scale().begin(), i.scale().end());
              }
              assert(base::dim() == c[0].dim() + c[1].dim());
          }

          Tvec operator()( Tspace &s, typename Tspace::ParticleVector &p ) override
          {
              Tvec r;
              for ( auto &i : c )
                  r.insert(r.end(), i(s, p).begin(), i(s, p).end());
              assert(base::dim() == r.size());
              return r;
          }
      };

/* @brief Operator for combining two reaction coordinates */
      template<class Tspace>
      Combined<Tspace> operator+(
          const ReactionCoordinateBase<Tspace> &c1,
          const ReactionCoordinateBase<Tspace> &c2 )
      {
          return Combined<Tspace>(c1, c2);
      }

/**
       * @brief Reaction coordinate: molecule-molecule mass-center separation
       *
       * In addition to the keywords from `ReactionCoordinateBaseCandidate`:
       *
       * Keyword    | Description
       * :--------- | :--------------
       * `first`    | name of first molecule
       * `second`   | name of second molecule
       * `index`    | molecule index to select in case of multiple molecules (default "0 0").
       * `dir`      | which dimensions to use when calc. mass center
       *
       * Example:
       *
       *     {
       *        "first":"water", "second":"sodium", "index":"0 0", "dir":[1,1,1],
       *        "min":[0], "max":[50]
       *     }
       */
      template<class Tspace, class base=ReactionCoordinateBase<Tspace>>
      class MassCenterSeparation : public base
      {
      private:
          Point dir;
          Group *g1, *g2;
          typedef typename base::Tvec Tvec;
      public:

          MassCenterSeparation(
              Tspace &s, Tmjson &j ) : base(j)
          {

              dir << j.value("dir", Tvec({1,1,1}) );
              vector<int> i = j.value("index", vector<int>({0,0}) );
              if (i.size()!=2 || dir.size()!=3)
                  throw std::runtime_error("CM reaction coordinate error");

              string name1 = j.at("first");
              string name2 = j.at("second");
              g1 = s.findMolecules(name1, true).at(i[0]);
              g2 = s.findMolecules(name2, true).at(i[1]);

              assert(!g1->empty());
              assert(!g2->empty());
          }

          /** @brief Returns reaction coordinate */
          Tvec operator()( Tspace &s, typename Tspace::ParticleVector &p ) override
          {
              Point cm1 = massCenter(s.geo, p, *g1);
              Point cm2 = massCenter(s.geo, p, *g2);
              Point r = s.geo.vdist(cm1, cm2);
              return {r.cwiseProduct(dir).norm()};
          }
      };

    }//namespace

/**
     * @brief Base for calculating reaction coordinates
     *
     * This is a common interface for calculating reaction
     * coordinates used in i.e. penalty functions. The main
     * responsibility is to merely return the coordinate, given
     * a certain system configuration. The coordinate may be
     * many-dimensional and is returned in the form of an array.
     *
     * @todo Simplify by moving out save/load functions, penalty
     * function data member as well as MPI calls. This class
     * should only return the coordinate and keep info about
     * minimum and maximim allowed values. Loading, saving,
     * updating, energy calc. should be done by the energy
     * class. Suggested public, virtual functions:
     *
     * string name();             // arbitrary name. "CM-CM", "xyz" etc.
     * Tvec& max();               // upper limits
     * Tvec& min();               // lower limits
     * unsigned int dim();        // dimension, i.e. size of Tvec (1,2,..)
     * bool inrange( Tvec& );     // return true if inside min/max
     * unsigned int updateRate(); // how often to update
     * double scale();
     * template<...>
     *   Tvec operator( Space&, Tparticlevector& ); // calc. coordinate
     *
     * We could make a CombinedPenaltyFunction that can mergy
     * i.e. two 1D functions into one 2D function
     *
     * auto combined = cmcm(...) + gyration(...)
     */
    template<class Tspace>
    class ReactionCoordinateBase
    {
    private:
        Table<int> histo;
#ifdef ENABLE_MPI
        TimeRelativeOfTotal<std::chrono::microseconds> timer;
        Faunus::MPI::MPIController *mpiPtr;
        Faunus::MPI::FloatTransmitter ft;
#endif
    protected:
        typedef std::vector<double> Tvec;
        Tspace *spc;
        typedef typename Tspace::p_vec Tpvec;
        int _cnt, _update, _samplings;
        double _f, _scale, _du;
        Tvec _bw, _lo, _hi, _size;
        //typedef typename Tspace::GeometryType Tgeo;
    public:
        Table<double> penalty;
        std::pair<Tvec, Tvec> vpair;

        ReactionCoordinateBase( Tmjson &j, Tspace &s, const string &sec )
        {
            spc = &s;
            auto m = j["energy"]["penalty"][sec];
            _f = m["f0"] | 0.5;
            _scale = m["scale"] | 0.8;
            _update = m["update"] | 1e5;
            for ( int i = 0; i != 2; ++i )
            {
                string num = std::to_string(i + 1);
                _bw.push_back(m["bw" + num] | 1.);
                _lo.push_back(m["lo" + num] | 0.);
                _hi.push_back(m["hi" + num] | 0.);
                _size.push_back((_hi[i] - _lo[i]) / _bw[i] + 1.);
            }
            penalty.reInitializer(_bw, _lo, _hi);
            histo.reInitializer(_bw, _lo, _hi);
            _cnt = 0;
            _du = 0;
            _samplings = 1;
        }

#ifdef ENABLE_MPI
        ReactionCoordinateBase(Faunus::MPI::MPIController &mpi, Tmjson &j, Tspace &s, const string &sec)
            : mpiPtr(&mpi)
        {
            spc = &s;
            auto m = j["energy"]["penalty"][sec];
            _f = m["f0"] | 0.5;
            _scale = m["scale"] | 0.8;
            _update = m["update"] | 1e5;
            for (int i=0; i!=2; ++i) {
                string num = std::to_string(i+1);
                _bw.push_back(m["bw"+num] | 1.);
                _lo.push_back(m["lo"+num] | 0.);
                _hi.push_back(m["hi"+num] | 0.);
                _size.push_back((_hi[i]-_lo[i])/_bw[i]+1.);
            }
            penalty.reInitializer(_bw,_lo,_hi);
            histo.reInitializer(_bw,_lo,_hi);
            _cnt = 0;
            _du = 0;
            _samplings = 1;
        }
#endif

        vector<Group *> PenalizedGroup( Tmjson &j, const string &sec, const string &mol )
        {
            string molecule = j["energy"]["penalty"][sec][mol];
            auto m = spc->molList().find(molecule); // is molecule defined?
            if ( m == spc->molList().end())
            {
                std::cerr << "Error: molecule '" << molecule << "' not defined.\n";
                exit(1);
            }
            return spc->findMolecules(m->id);
        }

        double update( bool acceptance )
        {
            Tvec v;
            if ( acceptance )
                v = vpair.first;
            else
            {
                v = vpair.second;
                vpair.first = vpair.second;
            }
            ++_cnt;
            _du = _f;
            penalty[v] += _f;
            histo[v] += 1;
            for ( Tvec::size_type i = 0; i != v.size(); ++i )
            {
                if ( v[i] == 0 || v[i] == _size[i] - 1 )
                {
                    _du += _f;
                    penalty[v] += _f;
                    histo[v] += 1;
                }
            }
            if ( _cnt % _update == 0 )
            {
                bool b = histo.minCoeff() >= _samplings;
#ifdef ENABLE_MPI
                b = reduceDouble(*mpiPtr,histo.minCoeff())*mpiPtr->nproc() >= _samplings;
#endif
                if ( b )
                {
                    _du = penalty[v] - _du;
#ifdef ENABLE_MPI
                    timer.start();
                    int size = penalty.size();
                    Faunus::MPI::avgTables<Table<double>>(mpiPtr, ft, penalty, size);
                    timer.stop();
#endif
                    double max = penalty.maxCoeff();
                    double min = penalty.minCoeff();
                    penalty.translate(-min);
                    _du = penalty[v] - _du;
                    _f = _scale * _f;
                    _samplings = ceil(_samplings / _scale);
                    double dh = log(double(histo.maxCoeff())/histo.minCoeff());
                    cout << "Energy barrier: " << max - min << ", delta histo: " << dh << endl;
                    histo.clear();
                }
            }
            return _du;
        }

        /** @brief Save table to disk */
        void save( const string &filename, double s, const Tvec &limits )
        {
            double avg = -s * penalty.avg(limits);
            penalty.save(filename + "penalty", s, avg);
        }

        /** @brief Save one row of the matrix */
        void
        saveRow( const string &filename, const Tvec &row, double s, const Tvec &limits )
        {
            double avg = -s * penalty.avg(limits);
            penalty.saveRow(filename + "penalty", row, s, avg);
        }

        /** @brief Load table to disk */
        void load( const string &filename )
        {
            penalty.load(filename + "penalty");
        }

        void test( UnitTest &t )
        {
            double range = penalty.maxCoeff() - penalty.minCoeff();
            t("penalty_range", range, 0.1);
        }

        string info()
        {
            using namespace Faunus::textio;
            std::ostringstream o;
            if ( _f != 0 )
            {
                o << header("Penalty Function");
                char w = 25;
                o << textio::pad(SUB, w, "Final f") << _f << endl
                  << textio::pad(SUB, w, "z-range") <<
                  penalty.maxCoeff() - penalty.minCoeff() << endl;
#ifdef ENABLE_MPI
                o << pad(SUB,w,"Relative time consumption of MP ") << timer.result() << endl;
#endif
            }
            return o.str();
        }

        double getU( Tvec &v, bool trial )
        {
            if ( !penalty.isInRange(v))
            {
                //penalty.to_index(v);
                //vpair.first = v;
                return 1e20;
            }
            else
            {
                penalty.to_index(v);
                if ( trial )
                    vpair.first = v;
                else
                    vpair.second = v;
                return penalty[v];
            }
        }

        virtual double
        operator()( const Tpvec &, bool )=0;//const Tgeo&)=0;
    };

/* distance between the mass centra of two molecules or groups along line "dir" */
    template<class Tspace, typename base=ReactionCoordinateBase<Tspace>>
    class CmCm : public base
    {
    private:
        Point dir;
        vector<Group *> mol1, mol2;
    public:
        typedef vector<double> Tvec;

        CmCm( Tmjson &j, Tspace &s, const string &sec = "cm-cm" ) : base(j, s, sec)
        {
            mol1 = base::PenalizedGroup(j, sec, "first");
            mol2 = base::PenalizedGroup(j, sec, "second");
            dir << (j["energy"]["penalty"][sec]["dir"] | std::string("0 0 1"));
        }

#ifdef ENABLE_MPI
        CmCm(Faunus::MPI::MPIController &mpi, Tmjson &j, Tspace &s, const string &sec="cm-cm") : base(mpi,j,s,sec) {
            mol1 = base::PenalizedGroup(j,sec,"first");
            mol2 = base::PenalizedGroup(j,sec,"second");
            dir << ( j["energy"]["penalty"][sec]["dir"] | std::string("0 0 1") );
}
#endif

        double operator()( const typename base::Tpvec &p, bool trial )
        {
            Group g1(mol1.front()->front(), mol1.back()->back());
            Group g2(mol2.front()->front(), mol2.back()->back());
            auto cm1 = Geometry::massCenter(base::spc->geo, p, g1);
            auto cm2 = Geometry::massCenter(base::spc->geo, p, g2);
            double dist = abs(base::spc->geo.vdist(cm1, cm2).dot(dir));
            Tvec v;
            v.push_back(dist);
            return base::getU(v, trial);
        }
    };

/* position of a molecule or group */
    template<class Tspace, typename base=ReactionCoordinateBase<Tspace>>
    class XYZ : public base
    {
    private:
        vector<Group *> mol;
        Point dir;
    public:
        typedef vector<double> Tvec;

        XYZ( Tmjson &j, Tspace &s, const string &sec = "xyz" ) : base(j, s, sec)
        {
            mol = base::PenalizedGroup(j, sec, "first");
            dir << (j["energy"]["penalty"][sec]["dir"] | std::string("1 1 0"));
        }

#ifdef ENABLE_MPI
        XYZ(Faunus::MPI::MPIController &mpi, Tmjson &j, Tspace &s, const string &sec="xyz") : base(mpi,j,s,sec) {
            mol = base::PenalizedGroup(j,sec,"first");
            dir << ( j["energy"]["penalty"][sec]["dir"] | std::string("1 1 0") );
        }
#endif

        double operator()( const typename base::Tpvec &p, bool trial )
        {
            Group g(mol.front()->front(), mol.back()->back());
            auto cm = Geometry::massCenter(base::spc->geo, p, g);
            Tvec v;
            if ( dir.x() == 1 )
                v.push_back(cm.x());
            if ( dir.y() == 1 )
                v.push_back(cm.y());
            if ( dir.z() == 1 )
                v.push_back(cm.z());
            return base::getU(v, trial);
        }
    };

/* radius of gyration of a molecule or group */
    template<class Tspace, typename base=ReactionCoordinateBase<Tspace>>
    class Rg : public base
    {
    private:
        vector<Group *> mol;
    public:
        typedef vector<double> Tvec;

        Rg( Tmjson &j, Tspace &s, const string &sec = "R_g" ) : base(j, s, sec)
        {
            mol = base::PenalizedGroup(j, sec, "first");
        }

#ifdef ENABLE_MPI
        Rg(Faunus::MPI::MPIController &mpi, Tmjson &j, Tspace &s, const string &sec="R_g") : base(mpi,j,s,sec) {
            mol = base::PenalizedGroup(j,sec,"first");
        }
#endif

        double operator()( const typename base::Tpvec &p, bool trial )
        {
            Group g(mol.front()->front(), mol.back()->back());
            auto cm = Geometry::massCenter(base::spc->geo, p, g);
            auto S = Geometry::gyration(base::spc->geo, p, g, cm);
            Eigen::EigenSolver<Eigen::Matrix3d> es(S);
            double R = 0;
            for ( int i = 0; i < es.eigenvalues().size(); ++i )
                R += es.eigenvalues()[i].real();
            R = sqrt(R);
            Tvec v;
            v.push_back(R);
            return base::getU(v, trial);
        }
    };

/* cm-cm separation between two molecules/groups along line "dir" and
     * angle formed by principal axis of first molecule/group with line "dir"
     */
    template<class Tspace, typename base=ReactionCoordinateBase<Tspace>>
    class CmAngle : public base
    {
    private:
        Point dir;
        vector<Group *> mol1, mol2;
    public:
        typedef vector<double> Tvec;

        CmAngle( Tmjson &j, Tspace &s, const string &sec = "cm-angle" ) : base(j, s, sec)
        {
            mol1 = base::PenalizedGroup(j, sec, "first");
            mol2 = base::PenalizedGroup(j, sec, "second");
            dir << (j["energy"]["penalty"][sec]["dir"] | std::string("0 0 1"));
        }

#ifdef ENABLE_MPI
        CmAngle(Faunus::MPI::MPIController &mpi, Tmjson &j, Tspace &s, const string &sec="cm-angle") : base(mpi,j,s,sec) {
            mol1 = base::PenalizedGroup(j,sec,"first");
            mol2 = base::PenalizedGroup(j,sec,"second");
            dir << ( j["energy"]["penalty"][sec]["dir"] | std::string("0 0 1") );
        }
#endif

        double operator()( const typename base::Tpvec &p, bool trial )
        {
            Group g1(mol1.front()->front(), mol1.back()->back());
            Group g2(mol2.front()->front(), mol2.back()->back());
            auto cm1 = Geometry::massCenter(base::spc->geo, p, g1);
            auto cm2 = Geometry::massCenter(base::spc->geo, p, g2);
            double dist = abs(base::spc->geo.vdist(cm1, cm2).dot(dir));
            auto S = Geometry::gyration(base::spc->geo, p, g1, cm1);
            Eigen::EigenSolver<Eigen::Matrix3d> es(S);
            double max = std::numeric_limits<double>::min();
            int j = 0;
            for ( int i = 0; i < es.eigenvalues().size(); ++i )
            {
                double value = es.eigenvalues()[i].real();
                if ( value > max )
                {
                    max = value;
                    j = i;
                }
            }
            Point eig = es.eigenvectors().col(j).real();
            double cosine = eig.dot(dir);
            double angle = acos(cosine) * 180. / pc::pi;
            //if (angle > 90.) angle = 180. - angle;
            Tvec v;
            v.push_back(dist);
            v.push_back(angle);
            return base::getU(v, trial);
        }
    };

/**
     ** @brief General energy class for handling penalty function (PF) methods.
     **
     ** @details The reaction coordinate(s) is specified in the input file.
     ** The Tmjson class is scanned for the following keys:
     **
     ** Key              | Description
     ** :--------------- | :-----------------------------
     ** `first`  | 1st penalized molecule or group
     ** `second` | 2nd penalized molecule or group
     ** `update` | number of MC sweeps before scaling f0
     ** `f0`     | modification factor
     ** `scale`  | scaling factor to update f0
     ** `bw1`    | bin width of 1st coordinate
     ** `bw2`    | bin width of 2nd coordinate
     ** `lo1`    | lower limit of 1st coordinate
     ** `hi1`    | upper limit of 1st coordinate
     ** `lo2`    | lower limit of 2nd coordinate
     ** `hi2`    | upper limit of 2nd coordinate
     ** `dir`    | direction of the reaction coordinate
     **/
    template<class Tspace, typename base=Energybase<Tspace>>
    class PenaltyEnergy : public base
    {
    private:
        string
        _info() override { return "Energy from Penalty Function\n"; }

        typedef ReactionCoordinateBase<Tspace> Trc;
        typedef std::shared_ptr<Trc> Tptr;
        Tptr ptr;
        typedef typename Tspace::p_vec Tpvec;
        typedef vector<double> Tvec;
    public:
        PenaltyEnergy( Tmjson &j, Tspace &s )
        {
            auto m = j["energy"]["penalty"];
            if ( m.begin().key() == "cm-cm" )
                ptr = std::make_shared<CmCm<Tspace>>(j, s);
            if ( m.begin().key() == "xyz" )
                ptr = std::make_shared<XYZ<Tspace>>(j, s);
            if ( m.begin().key() == "R_g" )
                ptr = std::make_shared<Rg<Tspace>>(j, s);
            if ( m.begin().key() == "cm-angle" )
                ptr = std::make_shared<CmAngle<Tspace>>(j, s);
        }

#ifdef ENABLE_MPI
        PenaltyEnergy(Faunus::MPI::MPIController &mpi, Tmjson &j, Tspace &s) {
            auto m = j["energy"]["penalty"];
            if (m.begin().key()=="cm-cm")
                ptr = std::make_shared<CmCm<Tspace>>(mpi,j,s);
            if (m.begin().key()=="xyz")
                ptr = std::make_shared<XYZ<Tspace>>(mpi,j,s);
            if (m.begin().key()=="R_g")
                ptr = std::make_shared<Rg<Tspace>>(mpi,j,s);
            if (m.begin().key()=="cm-angle")
                ptr = std::make_shared<CmAngle<Tspace>>(mpi,j,s);
        }
#endif

        auto tuple() -> decltype(std::make_tuple(this))
        {
            return std::make_tuple(this);
        }

        string info() override { return ptr->info(); }

        void test( UnitTest &t ) { ptr->test(t); }

        void load( const string &filename = "" ) { ptr->load(filename); }

        double currentPenalty() { return ptr->penalty[ptr->vpair.first]; }

        void saveRow( const string &filename = "", const Tvec &row = {0, 0, 0},
                      double s = 1, const Tvec &limits = {0, 0, 0} )
        {
            ptr->saveRow(filename, row, s, limits);
        }

        void save( const string &filename = "", double s = 1, const Tvec &limits = {0} )
        {
            ptr->save(filename, s, limits);
        }

        double update( bool acceptance ) override
        {
            return ptr->update(acceptance);
        }

        double external( const Tpvec &p ) override
        {
            return ptr->operator()(p, base::isTrial(p));
        }
    };

/**
     * @brief Energy class for manybody interactions such as dihedrals and angular potentials
     *
     * This is a general class for interactions that can involve
     * any number of atoms.
     *
     * In the following example we add an angular potential between
     * particle index 3,4,5:
     *
     * ~~~~
     * Manybody<Tspace> pot;
     * pot.add( Potential::Angular( {3,4,5}, 70., 0.5 ) );
     * ~~~~
     *
     * @todo Currently implemented as `external()` which means
     * that all added potentials are evaluated for each move
     * eventhough only a subset of atoms are touched. Implement
     * dictionary to speed up.
     */
    template<class Tspace>
    class Manybody : public Energybase<Tspace>
    {
    protected:
        string _infosum;

        string _info() override
        {
            return _infosum;
        }

        typedef Energybase<Tspace> Tbase;
        typedef typename Tbase::Tpvec Tpvec;

        typedef std::function<double( typename Tbase::Tgeometry &, const Tpvec & )> EnergyFunct;
        vector <EnergyFunct> list;
        std::set<int> allindex; // index of all particles involved

    public:
        Manybody( Tspace &spc )
        {
            Tbase::name = "Manybody potential";
            Tbase::setSpace(spc);
        }

        /**
               * @brief Add a manybody potential
               */
        template<class Tmanybodypot>
        void add( const Tmanybodypot &f )
        {
            list.push_back(f);
            _infosum += "  " + f.brief() + "\n";
            for ( auto i : f.getIndex())
                allindex.insert(i);
        }

        double external( const Tpvec &p ) override
        {
            double u = 0;
            for ( auto &f : list )
                u += f(Tbase::spc->geo, p);
            return u;
        }
    };

#ifdef FAU_POWERSASA
/**
 * @brief SASA energy from transfer free energies
 *
 * This energy term calculates the change in solvent accessible
 * surface area (SASA) for each atom and use transfer free energies
 * to estimate the free energy change. This can be used to capture
 * salting-out effects caused by co-solutes.
 * Transfer free energies are defined on a per atom basis via the `tfe`
 * keyword in `AtomData`.
 *
 *  Keyword       | Description
 *  :------------ | :------------------------------------------------
 *  `proberadius` | Radius of probe (default: 1.4 angstrom)
 *  `molarity`    | Molar concentration of co-solute
 *
 * For more information see: http://dx.doi.org/10.1002/jcc.21844
 */
template<class Tspace>
class SASAEnergy : public Energybase<Tspace> {
    private:
        vector<double> tfe; // transfer free energies (1/angstrom^2)
        vector<double> sasa; // transfer free energies (1/angstrom^2)
        vector<Point> sasaCoords;
        vector<double> sasaWeights;
        double probe; // sasa probe radius (angstrom)
        double conc;  // co-solute concentration (mol/l)
        Average<double> avgArea; // average surface area

        typedef Energybase<Tspace> base;
        typedef typename base::Tpvec Tpvec;

        string _info() override {
            char w=20;
            std::ostringstream o;
            o << textio::pad(textio::SUB,w,"Probe radius")
                << probe << textio::_angstrom << "\n"
                << textio::pad(textio::SUB,w,"Co-solute conc.")
                << conc << " mol/l\n"
                << textio::pad(textio::SUB,w,"Average area")
                << avgArea.avg() << textio::_angstrom+textio::squared << "\n";
            return o.str();
        }

        template<class Tpvec>
            void updateSASA(const Tpvec &p) {
                size_t n=p.size(); // number of particles
                sasa.resize(n);
                sasaCoords.resize(n);
                sasaWeights.resize(n);

                for (size_t i=0; i<n; ++i) {
                    sasaCoords[i]  = p[i];
                    sasaWeights[i] = p[i].radius + probe;
                }

                // generate powersasa object and calc. sasa for all particles
                POWERSASA::PowerSasa<double,Point> ps(sasaCoords, sasaWeights, 1, 1, 1, 1);
                ps.calc_sasa_all();
                for (size_t i=0; i<n; ++i)
                    sasa[i] = ps.getSasa()[i];
            }

    public:
        SASAEnergy(Tmjson &j, const string &dir="sasaenergy") : base(dir) {
            base::name = "SASA Energy";
            auto _j = j["energy"][dir];
            probe = _j.value( "proberadius", 1.4 ); // angstrom
            conc = _j.at("molarity");         // co-solute concentratil (mol/l);
        }

        auto tuple() -> decltype(std::make_tuple(this)) {
            return std::make_tuple(this);
        }

        /**
         * @brief The SASA calculation is implemented
         * as an external potential, only
         */
        double external(const Tpvec &p) override {
            // if first run, resize and fill tfe vector
            if (tfe.size()!=p.size()) {
                tfe.resize(p.size());
                for (size_t i=0; i<p.size(); ++i)
                    tfe[i] = atom[ p[i].id ].tfe / (pc::kT() * pc::Nav); // -> kT
            }

            // calc. sasa and energy
            updateSASA(p);
            assert(sasa.size() == p.size());
            double u=0, A=0;
            for (size_t i=0; i<sasa.size(); ++i) {
                u += sasa[i] * tfe[i]; // a^2 * kT/a^2/M -> kT/M
                if (!this->isTrial(p))
                    A+=sasa[i];
            }
            if (!this->isTrial(p))
                avgArea+=A; // sample average area for accepted confs. only
            return u * conc; // -> kT
        }
};
#endif

    /**
     * @brief Additive Hamiltonian
     *
     * ~~~{.java}
     * "energy" : {
     *     "nonbonded" : { "type":"debyehuckel-lj", "epsr":80, "debyelength":0.01 },
     *     "isobaric" : { "pressure": 0.2 },
     *     "cmconstrain" : { ... }
     * }
     * ~~~
     *
     * @todo Unfinished and under construction
     */
    template<class Tspace, class Tbase=Energybase<Tspace>>
    class Hamiltonian : public Tbase
    {
    private:
        typedef Tbase *baseptr;
        typedef std::shared_ptr<Tbase> Tptr;
        std::vector<Tptr> baselist;
        typedef typename Tspace::ParticleType Tparticle;
        typedef typename Tspace::ParticleVector Tpvec;

    public:
        Hamiltonian( Tmjson &j, Tspace &spc )
        {
            Tbase::name = "Hamiltonian";
            setSpace(spc);

            auto &m = j.at("energy");
            for ( auto i = m.begin(); i != m.end(); ++i )
            {
                try {
                    auto &val = i.value();
                    size_t n = baselist.size();

                    using namespace Potential;

                    if (i.key()=="coulomb+lj")
                        baselist.push_back( Tptr( new Energy::Nonbonded<Tspace,
                                    CombinedPairPotential<CoulombGalore, LennardJonesLB>>(j) ) );

                    if (i.key()=="coulomb+hs")
                        baselist.push_back( Tptr( new Energy::Nonbonded<Tspace,
                                    CombinedPairPotential<CoulombGalore, HardSphere>>(j) ) );

                    if ( i.key() == "isobaric" )
                        baselist.push_back( Tptr( new Energy::ExternalPressure<Tspace>( j ) ) );

                    if ( i.key() == "cmconstrain" )
                        baselist.push_back( Tptr( new Energy::MassCenterConstrain<Tspace>(j, *Tbase::spc)) );

                    if ( i.key() == "penalty" )
                    {
                    }

                    if (n==baselist.size()) // nothing was added --> unknown type given --> error
                        throw std::runtime_error("unknown energy '" + i.key() + "'");

                }
                catch (std::exception &e) {
                    std::cerr << "Error in " << string(Tbase::name) << ". " << i.key() << ": " << e.what() << endl;
                    throw;
                }
            }
            if (baselist.empty())
                std::cerr << "Warning: Hamiltonian is empty\n";

            setSpace(spc);
        }

        auto tuple() -> decltype(std::make_tuple(this))
        {
            return std::make_tuple(this);
        }

        /** @brief Find pointer to given energy type; `nullptr` if not found. */
        template<class Tenergy>
        std::shared_ptr<Tenergy> get()
        {
            static_assert(std::is_base_of<Energybase<Tspace>, Tenergy>::value,
                          "Template parameter must be derived from `Energy::Energybase`");
            for (auto b : baselist)
            {
                auto ptr = std::dynamic_pointer_cast<Tenergy>( b );
                if ( ptr != nullptr )
                    return ptr;
            }
            return nullptr;
        }

        void setSpace( Tspace &s ) override
        {
            Tbase::setSpace(s);
            for ( auto b : baselist )
                b->setSpace(s);
        }

        double p2p( const Tparticle &p1, const Tparticle &p2 ) override
        {
            double u = 0;
            for ( auto b : baselist )
                u += b->p2p(p1, p2);
            return u;
        }

        Point f_p2p( const Tparticle &p1, const Tparticle &p2 ) override
        {
            Point p(0, 0, 0);
            for ( auto b : baselist )
                p += b->f_p2p(p1, p2);
            return p;
        }

        double all2p( const Tpvec &p, const Tparticle &a ) override
        {
            double u = 0;
            for ( auto b : baselist )
                u += b->all2p(p, a);
            return u;
        }

        // single particle interactions
        double i2i( const Tpvec &p, int i, int j ) override
        {
            double u = 0;
            for ( auto b : baselist )
                u += b->i2i(p, i, j);
            return u;
        }

        double i2g( const Tpvec &p, Group &g, int i ) override
        {
            double u = 0;
            for ( auto b : baselist )
                u += b->i2g(p, g, i);
            return u;
        }

        double i2all( Tpvec &p, int i ) override
        {
            double u = 0;
            for ( auto b : baselist )
                u += b->i2all(p, i);
            return u;
        }

        double i_external( const Tpvec &p, int i ) override
        {
            double u = 0;
            for ( auto b : baselist )
                u += b->i_external(p, i);
            return u;
        }

        double i_internal( const Tpvec &p, int i ) override
        {
            double u = 0;
            for ( auto b : baselist )
                u += b->i_internal(p, i);
            return u;
        }

        // Group interactions
        double g2g( const Tpvec &p, Group &g1, Group &g2 ) override
        {
            double u = 0;
            for ( auto b : baselist )
                u += b->g2g(p, g1, g2);
            return u;
        }

        /*!
               * For early rejection we do a reverse loop over energy
               * classes to test if the energy is infinity. Reverse
               * because constraining energy classes are often added
               * last.
               */
        double g_external( const Tpvec &p, Group &g ) override
        {
            double u = 0;
            for ( auto b = baselist.rbegin(); b != baselist.rend(); ++b )
            {
                u += (*b)->g_external(p, g);
                if ( u >= pc::infty )
                    break;
            }
            return u;
        }

        double g_internal( const Tpvec &p, Group &g ) override
        {
            double u = 0;
            for ( auto b : baselist )
                u += b->g_internal(p, g);
            return u;
        }

        double external( const Tpvec &p ) override
        {
            double u = 0;
            for ( auto b : baselist )
                u += b->external(p);
            return u;
        }

        double v2v( const Tpvec &v1, const Tpvec &v2 ) override
        {
            double u = 0;
            for ( auto b : baselist )
                u += b->v2v(v1, v2);
            return u;
        }

        double g1g2( const Tpvec &p1, Group &g1, const Tpvec &p2, Group &g2 ) override
        {
            double u = 0;
            for ( auto b : baselist )
                u += b->g1g2(p1, g2, p2, g2);
            return u;
        }

        string _info() override
        {
            using namespace textio;
            std::ostringstream o;
            o << indent(SUB) << "Energy Terms:" << endl;
            int i = 1;
            for ( auto e : baselist )
                o << indent(SUBSUB) << std::left << setw(4) << i++ << e->name << endl;
            for ( auto e : baselist )
                o << e->info();
            return o.str();
        }

        void field( const Tpvec &p, Eigen::MatrixXd &E ) override
        {
            assert((int) p.size() == E.size());
            for ( auto b : baselist )
                b->field(p, E);
        }

        /**
         * @brief systemEnergy Calculate System energy = external + internal + g2g() for all groups i,j from Space::Grouplist
         *        A convenience function intented for easy Energy matrix integration of configuration-wide moves - such as isobaric move
         * @param p
         * @return
         */
        virtual double systemEnergy( const Tpvec &p )
        {
            double u = external(p);
            for ( auto g : Energybase<Tspace>::spc->groupList())
                u += g_external(p, *g) + g_internal(p, *g);
            for ( unsigned int i = 0; i < Energybase<Tspace>::spc->groupList().size() - 1; i++ )
                for ( unsigned int j = i + 1; j < Energybase<Tspace>::spc->groupList().size(); j++ )
                    u += g2g(p, *Energybase<Tspace>::spc->groupList()[i], *Energybase<Tspace>::spc->groupList()[j]);
            return u;
        }

        /**
         * @brief g2All - Calculate energy between group g and Particle vector p based on Space::Grouplist using g2g() function
         *        A convenience function intended for easy Energy matrix intergration of group-based moves - such as TranslateRotate
         * @param p
         * @param g
         * @return
         */
        virtual double g2All(const Tpvec & p, Group &g)
        {
            double unew = 0.0;
            for ( auto o : Energybase<Tspace>::spc->groupList()) {
                if ( o != &g ) {
                    unew += g2g(p, *o, g);
                    if ( unew == pc::infty )
                        return pc::infty;   // early rejection
                }
            }
            return unew;
        }
    };

    /**
     * @brief Calculates the total system energy
     *
     * For a given particle vector, space, and energy class we try to
     * calculate the total energy taking into account inter- and
     * intra-molecular interactions as well as external potentials.
     * While this may not work for all systems it may be a useful
     * first guess.
     * This is the default energy routine for `Move::ParallelTempering`
     * and may also be used for checking energy drifts.
     */
    template<class Tspace, class Tenergy, class Tpvec>
    double systemEnergy( Tspace &spc, Tenergy &pot, const Tpvec &p )
    {
        pot.setSpace(spc); // ensure pot geometry is in sync with spc
        return pot.systemEnergy(p);
    }

    /**
     * @brief Calculates the total system energy
     *
     * For a given particle vector, space, and energy class we try to
     * calculate the total energy taking into account inter- and
     * intra-molecular interactions as well as external potentials.
     * While this may not work for all systems it may be a useful
     * first guess.
     * This is the default energy routine for `Move::ParallelTempering`
     * and may also be used for checking energy drifts.
     */
    template<class Tspace, class Tenergy, class Tpvec>
    double systemEnergy_old( Tspace &spc, Tenergy &pot, const Tpvec &p )
    {
        pot.setSpace(spc); // ensure pot geometry is in sync with spc
        double u = pot.external(p);
        for ( auto g : spc.groupList())
            u += pot.g_external(p, *g) + pot.g_internal(p, *g);
        for ( int i = 0; i < (int) spc.groupList().size() - 1; i++ )
            for ( int j = i + 1; j < (int) spc.groupList().size(); j++ )
                u += pot.g2g(p, *spc.groupList()[i], *spc.groupList()[j]);
        return u;
    }
    
      /**
       * @brief Help-function to 'energyChange'
       * @todo Fix such that it works for inserted and removed particles
       */
      template<class Tenergy, class Tpvec, class Tgeo, class Tparticle>
      double energyChangeConfiguration (
          Space<Tgeo,Tparticle> &spc,
          Tenergy &pot,
          const Tpvec &p,
          const typename Space<Tgeo,Tparticle>::Change &c )
      {
          double du = 0;
          auto &g = spc.groupList();
          du += pot.g2All(p, c.mvGroup);
          du += pot.external(p);

          for ( auto &m : c.mvGroup ) // loop over all moved groups
          {
              size_t i = size_t(m.first);                       // index of moved group

              // Calculate energy moved <-> static groups

              /*for ( size_t j = 0; j < g.size(); j++ ) // moved to g2all          // loop over through all groups
                  if ( c.mvGroup.count(j) == 0 )                // If group j is not in mvGroup
                      du += pot.g2g(p, *g[i], *g[j]);*/           // moved group<->static groups

              du += pot.g_external(p, *g[i]);                   // moved group <-> external

              if ( g[i]->isAtomic() )
              {                                                 // Check if moved group is atomic
                  for ( auto j : m.second )                     // loop over all moved groups
                      for ( auto k : *g[i] )                    // loop over all atoms in moved group
                          if ( std::find(m.second.begin(), m.second.end(), k) == m.second.end() || j
                              > k ) // check such that moved atoms does not interact OR moved atoms interact only once
                              du += pot.i2i(p, j, k);
              }
              if ( g[i]->isMolecular() )
              {
                  if (!m.second.empty()) // only recalculate internal energy if N>0
                     du += pot.g_internal(p, *g[i]);
              }
          }

          // Calculate energy moved <-> moved
          /*for ( auto i = c.mvGroup.begin(); i != c.mvGroup.end(); i++ ) // moved to g2all
          {
              for ( auto j = i; j != c.mvGroup.end(); j++ )
              {
                  auto _i = i->first;
                  auto _j = j->first;
                  du += pot.g2g(p, *g[_i], *g[_j]);
              }
          }*/

          return du;
      }

    /**
     * @brief Calculate energy change due to proposed modification defined by `Space::Change`
     */
      template<class Tenergy, class Tgeometry, class Tparticle>
      double energyChange( Space<Tgeometry, Tparticle> &s,
                           Tenergy &pot,
                           const typename Space<Tgeometry, Tparticle>::Change &c )
      {

          if (c.empty())
              return 0;

          auto backup = s.geo; // backup geometry;

          // make sure the new (trial) geometry is active
          if ( c.geometryChange )
          {
              s.geo = s.geo_trial;
              pot.setSpace(s);
          }

          // Check for container overlap

          for ( auto &m : c.mvGroup )  // loop over all moved groups
              if ( m.second.empty())  // if index vector is empty, assume that all particles have moved
              {
                  for ( auto j : *s.groupList()[m.first] )
                      if ( s.geo.collision(s.trial[j], s.trial[j].radius, Geometry::Geometrybase::BOUNDARY))
                          return pc::infty;
              }
              else
                  for ( auto j : m.second ) // if given, loop over specific particle index
                      if ( s.geo.collision(s.trial[j], s.trial[j].radius, Geometry::Geometrybase::BOUNDARY))
                          return pc::infty;

          double duNew = energyChangeConfiguration(s, pot, s.trial, c);

          // restore original geometry
          if ( c.geometryChange )
          {
              s.geo = backup;
              pot.setSpace(s);
          }

          double duOld = energyChangeConfiguration(s, pot, s.p, c);

          return (duNew - duOld);
      }



      /**
   * @brief EnergyTester class to conveniently compare two energy classes together
   *
   * Usage:     Energy::NonbondedCutg2g<Tspace,Tpairpot> non(mcp);
                Energy::EnergyMatrix<double, Tspace, Energy::NonbondedCutg2g<Tspace,Tpairpot> > nonEM(mcp);

                EnergyTester<Energy::NonbondedCutg2g<Tspace,Tpairpot>, Energy::EnergyMatrix<double, Tspace, Energy::NonbondedCutg2g<Tspace,Tpairpot> > > pot;

                or more conveniently -> auto pot = ((non) == (nonEM));
   */
      template<class T1, class T2>
      class EnergyTester : public Energybase<typename T1::SpaceType>
      {
      private:
          string
          _info() override { return first.info() + second.info(); }

          typedef typename T1::SpaceType Tspace;
          typedef Energybase<Tspace> Tbase;
          typedef typename Tbase::Tparticle Tparticle;
          typedef typename Tbase::Tpvec Tpvec;

      public:
          T1 first;
          T2 second;

          bool test(double a, double b) {
              return fabs(a-b) < 1e-9;
          }

          auto tuple() -> decltype(std::tuple_cat(first.tuple(), second.tuple()))
          {
              return std::tuple_cat(first.tuple(), second.tuple());
          }

          EnergyTester( const T1 &a, const T2 &b ) : first(a), second(b) {
              cout << "\n\n ENERGY TESTER RUNNING \n\n" << endl;
          }

          string info() override { return _info(); }

          void setSpace( typename T1::SpaceType &s ) override
          {
              first.setSpace(s);
              second.setSpace(s);
              Tbase::setSpace(s);
          }

          double p2p( const Tparticle &a, const Tparticle &b ) override { return first.p2p(a, b); }

          Point f_p2p( const Tparticle &a, const Tparticle &b ) override
          {
              return first.f_p2p(a, b);
          }

          double all2p( const Tpvec &p, const Tparticle &a ) override { return first.all2p(p, a); }

          double i2i( const Tpvec &p, int i, int j ) override { return first.i2i(p, i, j); }

          double i2g( const Tpvec &p, Group &g, int i ) override { return first.i2g(p, g, i); }

          double i2all( Tpvec &p, int i ) override {
              double a = first.i2all(p, i);
              double b = second.i2all(p, i);
              if(!test(a,b)) {
                  if(Tbase::isTrial(p))
                      std::cout << "Trial ";
                  else
                      std::cout << "pvec ";
                  std::cout << "Error i2all " << a << " " << b << endl;
              }
              return a;
          }

          double i_external( const Tpvec &p, int i ) override { return first.i_external(p, i); }

          double i_internal( const Tpvec &p, int i ) override { return first.i_internal(p, i); }

          double g2g( const Tpvec &p, Group &g1, Group &g2 ) override
          {
              double a = first.g2g(p, g1, g2);
              double b = second.g2g(p, g1, g2);
              if(!test(a,b))
                  std::cout << "Error g2g" << a << " " << b << endl;
              return a;
          }

          double g1g2( const Tpvec &p1, Group &g1, const Tpvec &p2, Group &g2 ) override
          {
              return first.g1g2(p1, g1, p2, g2);
          }

          double g_external( const Tpvec &p, Group &g ) override
          {
              return first.g_external(p, g);
          }

          double g_internal( const Tpvec &p, Group &g ) override
          {
              return first.g_internal(p, g);
          }

          double external( const Tpvec &p ) override { return first.external(p); }

          double update( bool b ) override { return first.update(b) + second.update(b); }

          double v2v( const Tpvec &p1, const Tpvec &p2 ) override { return first.v2v(p1, p2) + second.v2v(p1, p2); }

          void field( const Tpvec &p, Eigen::MatrixXd &E ) override
          {
              first.field(p, E);
              second.field(p, E);
          }

          double g2All(const Tpvec & p, const std::map<int, vector<int>>& mg) override
          {
              double a = first.g2All(p, mg);
              double b = second.g2All(p, mg);
              if(!test(a,b)) {
                  if(Tbase::isTrial(p))
                      std::cout << "Trial ";
                  else
                      std::cout << "pvec ";
                  std::cout << "Error g2All " << a << " " << b << " " << fabs(a-b)<< endl;
              }
              return a;
          }

          double systemEnergy(const Tpvec & p) override
          {
              double a = first.systemEnergy(p);
              double b = second.systemEnergy(p);
              if(!test(a,b)) {
                  if(Tbase::isTrial(p))
                      std::cout << "Trial ";
                  else
                      std::cout << "pvec ";
                  std::cout << "Error system E " << a << " " << b << " " << fabs(a-b) << "\n NOTE: Different order of sum - is the error numeric?" << endl;
              }
              return a;
          }
      };


      /**
   * @brief Operator to conveniently compare two energy classes together
   *
   * Usage:     Energy::NonbondedCutg2g<Tspace,Tpairpot> non(mcp);
                Energy::EnergyMatrix<double, Tspace, Energy::NonbondedCutg2g<Tspace,Tpairpot> > nonEM(mcp);

                auto pot = ((non) == (nonEM));
   */
  template<class T1, class T2,
      class = typename
      std::enable_if<std::is_base_of<Energybase<typename T1::SpaceType>, T1>::value>::type,
      class = typename
      std::enable_if<std::is_base_of<Energybase<typename T1::SpaceType>, T2>::value>::type >
  EnergyTester<T1, T2> &operator==( const T1 &u1, const T2 &u2 ) { return *(new EnergyTester<T1, T2>(u1, u2)); }


  }//Energy namespace
}//Faunus namespace
#endif
