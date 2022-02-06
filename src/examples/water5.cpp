#include <faunus/faunus.h>
#include <faunus/ewald.h>
//#define EWALD

using namespace Faunus;
using namespace Faunus::Potential;

typedef Space<Geometry::Cuboid> Tspace;
#ifdef EWALD
typedef LennardJonesLB Tpairpot;
#else
typedef CombinedPairPotential<CoulombGalore,HardSphere> Tpairpot0;
typedef CombinedPairPotential<Tpairpot0,SquaredWellMixed<>> Tpairpot;
#endif

int main() {

  cout << textio::splash();      // show faunus banner and credits
  Tmjson mcp = openjson("tubes.json");// read input file
  Tspace spc(mcp);
  
  // Energy functions and space
#ifdef EWALD
  auto pot = Energy::NonbondedEwald<Tspace,Tpairpot>(mcp);
#else
  auto pot = Energy::Nonbonded<Tspace,Tpairpot>(mcp);
#endif

  
  
  spc.load("state"); // load old config. from disk (if any)

  // Markov moves and analysis
  Analysis::CombinedAnalysis analyzer(mcp,pot,spc);
  Move::Propagator<Tspace> mv( mcp, pot, spc );

  cout << atom.info() + spc.info() + textio::header("MC Simulation Begins!");
  
  MCLoop loop(mcp);    // class for handling mc loops
  while ( loop[0] ) {          // Markov chain 
    while ( loop[1] ) {
      mv.move();
      analyzer.sample();
    } // end of micro loop

    cout << loop.timing();
  } // end of macro loop

  // perform unit 
  UnitTest test(mcp);
  mv.test(test);

  // print information
  cout << pot.info() + loop.info() + mv.info() + analyzer.info() + test.info();

  return test.numFailed();
}
