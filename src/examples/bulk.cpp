#include <faunus/faunus.h>
using namespace Faunus;
using namespace Faunus::Potential;

typedef CombinedPairPotential<HardSphere,SquareWell> Tpairpot; // pair potential
typedef Geometry::Cuboid Tgeometry;   // geometry: cube w. periodic boundaries
typedef Space<Tgeometry,PointParticle> Tspace;

int main() {
  Tmjson mcp = openjson("lipids.json"); // open JSON input file
  MCLoop loop(mcp);                   // class for handling mc loops

  Tspace spc(mcp);                    // simulation space

  auto pot = Energy::Nonbonded<Tspace,Tpairpot>(mcp);

  spc.load("state");                  // load old config. from disk (if any)

  Analysis::CombinedAnalysis analyzer(mcp,pot,spc);
  Move::Propagator<Tspace> mv(mcp,pot,spc);

  cout << atom.info() + spc.info() + pot.info() + textio::header("MC Simulation Begins!");

  while ( loop[0] ) {  // Markov chain 
    while ( loop[1] ) {
      mv.move();
      analyzer.sample();
    } // end of micro loop

    cout << loop.timing();

  } // end of macro loop

  // print information
  cout << loop.info() + mv.info() + analyzer.info();

  return 0;
}
