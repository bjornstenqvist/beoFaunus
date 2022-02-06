#include <faunus/faunus.h>



using namespace Faunus;
using namespace Faunus::Potential;

typedef Space<Geometry::Cuboid> Tspace;
//typedef CombinedPairPotential<HardSphere,SquaredWellMixed<>> Tpairpot;
//typedef CombinedPairPotential<Tpairpot0,SquaredWellMixed<>> Tpairpot;
typedef CombinedPairPotential<CoulombGalore,HardSphere> Tpairpot;
//typedef HardSphere Tpairpot;



int main() {

  cout << textio::splash();      // show faunus banner and credits
  Tmjson mcp = openjson("tubes.json");// read input file
  Tspace spc(mcp);
  
  // Energy functions and space
  auto pot = Energy::NonbondedCutg2g<Tspace,Tpairpot>(mcp);

  
  
  
  
 
  
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
