#include <faunus/faunus.h>
#include <faunus/ewald.h>

using namespace Faunus;
using namespace Faunus::Potential;

typedef Space<Geometry::Cuboid, PointParticle> Tspace;

int main()
{
    InputMap mcp("gctit.json");                   // open user input file
    Tspace spc(mcp);                              // simulation space
    auto pot =
        Energy::Nonbonded<Tspace, CoulombHS>(mcp)      // hamiltonian
            + Energy::EquilibriumEnergy<Tspace>(mcp);

    pot.setSpace(spc);                            // share space w. hamiltonian

    spc.load("state", Tspace::RESIZE);             // load old config. from disk (if any)

    Analysis::CombinedAnalysis analysis(mcp, pot, spc);
    Move::Propagator<Tspace> mv(mcp, pot, spc);

    cout << atom.info() + spc.info() + pot.info() + "\n";

    MCLoop loop(mcp);                             // class for handling mc loops
    while ( loop[0] )
    {
        while ( loop[1] )
        {
            mv.move();                           // move!
            analysis.sample();                   // sample
        }                                           // end of micro loop
        cout << loop.timing();
    }                                             // end of macro loop

    UnitTest test(mcp);                           // class for unit testing

    cout << loop.info() + mv.info() + test.info() + analysis.info();

    return test.numFailed();
}

/**
  @page example_grand Example: Grand Canonical Salt with proton titration

  This is an example of a grand canonical salt solution (NmuT)
  with the following MC moves:

  - salt translation
  - salt exchange with virtual bulk - i.e. constant chemical potential

  In addition, two different Widom methods are used to check the chemical
  potential. Note that this analysis is formally valid only in the canonical
  ensemble which may lead to deviations at small system sizes.
  It is, however, easy to turn off GC moves via the input file.

  Run this example from the terminal:

      $ cd src/examples
      $ python gctit.py
  
  Input
  =====

  In this example a python script is used to generate the input json file as
  well as run the executable.

  grand.json
  ----------
  @includelineno examples/grand.json  

  Output
  ======
  This is the output generated by `grand.cpp`. Note that the default is to
  use Unicode UTF-16 encoding to print mathematical symbols. If your terminal is unable to
  print this properly, Unicode output can be switched off during compilation.

  @include examples/grand.out

  grand.cpp
  =========
  @includelineno examples/grand.cpp
*/