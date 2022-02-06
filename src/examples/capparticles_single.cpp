#include <faunus/faunus.h>
#include <faunus/ewald.h>
using namespace Faunus;
using namespace Faunus::Potential;

typedef CombinedPairPotential<HardSphereCap,CoulombUniformCap> Tpairpot; // pair potential
//typedef HardSphereCap Tpairpot; // pair potential

typedef Geometry::Cuboid Tgeometry;   // geometry: cube w. periodic boundaries
typedef Space<Tgeometry,CapParticle> Tspace;

int main() {
  cout << textio::splash();           // show faunus banner and credits

  Tmjson mcp = openjson("capparticles.json"); // open JSON input file
  MCLoop loop(mcp);                   // class for handling mc loops

  Tspace spc(mcp);                    // simulation space

  auto pot = Energy::NonbondedVector<Tspace,Tpairpot>(mcp);

  spc.load("state");                  // load old config. from disk (if any)
  spc.p[0] = Point(0,0,0);
  spc.p[1] = Point(0,0,0);
  spc.trial = spc.p;

  Analysis::CombinedAnalysis analyzer(mcp,pot,spc);
  Move::Propagator<Tspace> mv(mcp,pot,spc);
  
  cout << atom.info() + spc.info() + pot.info() + textio::header("MC Simulation Begins!");
  
  while ( loop[0] ) {  // Markov chain 
    while ( loop[1] ) {
      mv.move();
      analyzer.sample();
    } // end of micro loop

    spc.p[1] = spc.p[1] + Point(0.01,0,0);
    spc.trial = spc.p;

    cout << loop.timing();

  } // end of macro loop
  
  Point camera1, camera2, camera3;
  camera1 << mcp["analysis"]["povray"]["camera1"];
  camera2 << mcp["analysis"]["povray"]["camera2"];
  camera3 << mcp["analysis"]["povray"]["camera3"];
  
  CapparticlePOVRay a;
  a.saveCapparticlePOVRay(mcp["analysis"]["povray"]["file1"],spc,camera1); // In terminal: 'povray file.pov' gives output file. Add option 'Output_Alpha=on' to create transparent image.
  a.saveCapparticlePOVRay(mcp["analysis"]["povray"]["file2"],spc,camera2); // In terminal: 'povray file.pov' gives output file. Add option 'Output_Alpha=on' to create transparent image.
  a.saveCapparticlePOVRay(mcp["analysis"]["povray"]["file3"],spc,camera3); // In terminal: 'povray file.pov' gives output file. Add option 'Output_Alpha=on' to create transparent image.
  a.saveCapparticlePOVRayDifference(mcp["analysis"]["povray"]["file_diff"],spc);

  // perform unit tests (irrelevant for the simulation)
  UnitTest test(mcp);                    // class for unit testing
  mv.test(test);
  analyzer.test(test);

  // print information
  cout << loop.info() + mv.info() + analyzer.info() + test.info();

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
