#include <faunus/faunus.h>
using namespace Faunus;
using namespace Faunus::Potential;

typedef CombinedPairPotential<HardSphereCap2,CoulombUniformCap2> Tpairpot; // pair potential
//typedef HardSphereCap2 Tpairpot; // pair potential
//typedef WeeksChandlerAndersen Tpairpot;
//typedef HardSphere Tpairpot; // pair potential

typedef Geometry::Cuboid Tgeometry;   // geometry: cube w. periodic boundaries
typedef Space<Tgeometry,CapParticle2> Tspace;

int main() {
  //Faunus::MPI::MPIController mpi;
  //mpi.cout << textio::splash();
  cout << textio::splash();           // show faunus banner and credits

  Tmjson mcp = openjson("capparticles.json"); // open JSON input file
  MCLoop loop(mcp);                   // class for handling mc loops
  Tspace spc(mcp);                    // simulation space
  auto pot = Energy::NonbondedVector<Tspace,Tpairpot>(mcp);// + Energy::ExternalPressure<Tspace>(mcp);
  spc.load("state");                  // load old config. from disk (if any)
  //int seed = mcp["seed"];
  //slump.seed(seed);
  //cout << "Seed: " << seed << endl;

  Analysis::CombinedAnalysis analyzer(mcp,pot,spc);
  Move::Propagator<Tspace> mv(mcp,pot,spc);
  //Move::ParallelTempering temper(mcp,pot,spc,mpi);
  
  cout << atom.info() + spc.info() + pot.info() + textio::header("MC Simulation Begins!");
  //mpi.cout << atom.info() << spc.info() << pot.info() << textio::header("MC Simulation Begins!");
  
  while ( loop[0] ) {  // Markov chain 
    while ( loop[1] ) {
      mv.move();
      analyzer.sample();
    } // end of micro loop

    cout << loop.timing();
    //mpi.cout << loop.timing();

  } // end of macro loop
  
  CapparticlePOVRay a;
  a.saveCapparticlePOVRay2(mcp["analysis"]["povray"]["file1"],spc); // In terminal: 'povray file.pov' gives output file. Add option 'Output_Alpha=on' to create transparent image.
  //a.saveCapparticlePOVRayDifference(mcp["analysis"]["povray"]["file_diff"],spc);

  // perform unit tests (irrelevant for the simulation)
  UnitTest test(mcp);                    // class for unit testing
  mv.test(test);
  analyzer.test(test);

  // print information
  cout << loop.info() + mv.info() + analyzer.info() + test.info();
  //mpi.cout << loop.info() + mv.info() + analyzer.info() + test.info();

  return test.numFailed();
}

/**
  @page example_capparticles Example: Capped particles

  In this example we simulate 'Capparticles'. We only use a hard interaction.
  
  Information about the input file can be found in `src/examples/capparticles.py`.

  capparticles.cpp
  ========
  @includelineno examples/capparticles.cpp
*/
