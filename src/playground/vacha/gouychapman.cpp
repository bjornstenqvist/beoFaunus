/*
 * This will simulate rigid molecules in a rectangular slit container:
 * container. Hamiltonian is:
 * - Debye-Huckel electrostatics, hard sphere particles
 * - Gouy-Chapman electrostatics
 */

#include <faunus/faunus.h>

using namespace Faunus;
using namespace Faunus::Potential;

typedef Space<Geometry::Cuboidslit> Tspace; 
//typedef Potential::DebyeHuckelr12 Tpairpot;
typedef CombinedPairPotential<DebyeHuckelr12,SquareWellHydrophobic> Tpairpot;

int main(int argc, char** argv) {

#ifdef TEMPER
  #define cout mpi.cout
  Faunus::MPI::MPIController mpi;
#endif

  cout << textio::splash();

  InputMap mcp(textio::prefix+"gouychapman.input");
  MCLoop loop(mcp);                    // class for handling mc loops
  FormatPQR pqr;                       // PQR structure file I/O
  FormatAAM aam;                       // AAM structure file I/O
  FormatXTC xtc(1000);                 // XTC gromacs trajectory format for only cuboid geomtries
  EnergyDrift sys;                     // class for tracking system energy drifts

  Tspace spc(mcp);

  auto pot = Energy::Nonbonded<Tspace, Tpairpot>(mcp) + 
    Energy::ExternalPotential<Tspace,Potential::GouyChapman<> >(mcp);
  pot.second.setSurfPositionZ( &spc.geo.len_half.z() );
  pot.setSpace(spc);

  // Add rigid molecules
  vector<GroupMolecular> pol( mcp.get("polymer_N",0));
  string polyfile = mcp.get<string>("polymer_file", "");
  for (auto &g : pol) {                    // load molecules
    aam.load(polyfile);
    Geometry::FindSpace f;
    f.find(*spc.geo, spc.p, aam.p);        // find empty spot in particle vector
    g = spc.insert( aam.p );               // insert into space
    g.name="Molecule";
    spc.enroll(g);
  }

  Move::TranslateRotate<Tspace> gmv(mcp,pot,spc);
#ifdef TEMPER
  Move::ParallelTempering<Tspace> temper(mcp,pot,spc,mpi);
#endif

  Analysis::RadialDistribution<float,int> rdf(0.25);
  Analysis::LineDistribution<float,int> surfdist(0.25);

  spc.load(textio::prefix+"state");
  pqr.save(textio::prefix+"initial.pqr", spc.p);

  sys.init( Energy::systemEnergy(spc,pot,spc.p) );

  cout << atom.info() << spc.info() << pot.info() << textio::header("MC Simulation Begins!");

  while ( loop.macroCnt() ) {                      // Markov chain 
    while ( loop.microCnt() ) {
      int k,i= slump.rand() % 1;
      switch (i) {
        case 0:                                    // translate and rotate molecules
          k=pol.size();
          while (k-->0) {
            gmv.setGroup( pol[ slump.rand() % pol.size() ] );
            sys+=gmv.move();
          }

          for (auto &g : pol)
            surfdist( gouy->dist2surf(g.cm) )++;   // molecule mass center to GC surface histogram

          for (auto i=pol.begin(); i!=pol.end()-1; i++)
            for (auto j=i+1; j!=pol.end(); j++)
              rdf( spc.geo->dist(i->cm,j->cm) )++; // molecule-molecule rdf

          break;
      }

      if ( slump.runtest(0.0001) ) {
        xtc.setbox( spc.geo.len );
        xtc.save(textio::prefix+"traj.xtc", spc);  // gromacs xtc file output
      }
    } // end of micro loop
#ifdef TEMPER
    temper.setCurrentEnergy( sys.current() );
    sys+=temper.move();
#endif

    sys.checkDrift( Energy::systemEnergy(spc,pot,spc.p) );

    cout << loop.timing();

    rdf.save(textio::prefix+"rdf_p2p.dat");
    surfdist.save(textio::prefix+"surfdist.dat");
    pqr.save(textio::prefix+"confout.pqr", spc.p);
    spc.save(textio::prefix+"state");

  } // end of macro loop

  cout << loop.info() << pot.first.info() << sys.info() << gmv.info();   
#ifdef TEMPER
  cout << temper.info();
#endif
}
