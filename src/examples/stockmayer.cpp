#include <faunus/faunus.h>
#include <faunus/multipole.h>
#include <faunus/ewald.h>
#define EWALD

using namespace Faunus;
using namespace Faunus::Move;
using namespace Faunus::Potential;

typedef Space<Geometry::Cuboid, DipoleParticle> Tspace;
#ifdef EWALD
typedef LennardJonesLB Tpair;
#else
typedef CombinedPairPotential<LennardJonesLB,DipoleDipoleGalore> Tpair;
#endif

int main()
{
    InputMap in("stockmayer.json");                // open parameter file for user input
    Tspace spc(in);                                // sim.space, particles etc.

#ifdef EWALD
    auto pot = Energy::NonbondedEwald<Tspace,Tpair,false,false,true>(in);
#else
    auto pot = Energy::NonbondedVector<Tspace,Tpair>(in);
#endif

    spc.load("state");

    int seed = in["seed"];
    //slump.seed(seed);
    cout << "Seed: " << seed << endl;

#ifdef __POLARIZE
    Move::Propagator<Tspace,true> mv(in,pot,spc);
#else
    Move::Propagator<Tspace, false> mv(in, pot, spc);
#endif

    Analysis::CombinedAnalysis analyzer(in,pot,spc);

    //Table2D<double,double> theta, phi;
    //theta.setResolution(0.02);
    //phi.setResolution(0.02);
    //int N = spc.p.size();
    //spc.p[N-1] = Point(0,0,0);
    //spc.trial = spc.p;

    MCLoop loop(in);                               // class for mc loop counting
    while ( loop[0] )
    {                            // Markov chain
        while ( loop[1] )
        {
            mv.move();
            analyzer.sample();

	    //cout << "0: " << spc.p[0].radius << endl;
	    //cout << "N-1: " << spc.p[N-1].radius << endl;
	    //Point rtp = spc.p[N-1].mu().xyz2rtp();
	    //theta(rtp.y()) += 1.0;
	    //phi(rtp.z()) += 1.0;
        }
        cout << loop.timing() << std::flush;
    }

    UnitTest test(in);
    mv.test(test);
    analyzer.test(test);
    DipoleWRL sdp;
    sdp.saveDipoleWRL("stockmayer.wrl", spc, Group(0, spc.p.size() - 1));

    //theta.save("theta.dat");
    //phi.save("phi.dat");

    std::cout << spc.info() + pot.info() + mv.info() + test.info() + analyzer.info();

    return test.numFailed();
}
/**  @page example_stockmayer Example: Stockmayer potential

  This will simulate a Stockmayer potential in a cubic box.

  Run this example from the `examples` directory:

  ~~~~~~~~~~~~~~~~~~~
  $ make
  $ cd src/examples
  $ ./stockmayer.py
  ~~~~~~~~~~~~~~~~~~~

  stockmayer.cpp
  ==============

  @includelineno examples/stockmayer.cpp

 */
