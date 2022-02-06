#include <faunus/faunus.h>
using namespace Faunus;
using namespace Faunus::Potential;

//typedef CombinedPairPotential<CoulombWolf,LennardJonesLB> Tpairpot; // pair potential
//typedef CombinedPairPotential<HardSphereCap,HardSphereCap2> Tpairpot; // pair potential
//typedef CoulombUniformCap2 Tpairpot; // pair potential
//typedef CombinedPairPotential<HardSphereCap2,CoulombUniformCap2> Tpairpot; // pair potential
typedef HardSphereCap2 Tpairpot; // pair potential

typedef Space<Geometry::Cuboid,CapParticle2> Tspace;

int main() 
{
  InputMap in("capparticles.json");          // open user input file
  Tspace spc(in);

  auto pot = Energy::NonbondedVector<Tspace,Tpairpot>(in);// + Energy::ExternalPressure<Tspace>(mcp);
  
  int moveParticle = in["xyz"]["moveParticle"];
  
  spc.p[0] = Point(0,0,0);
  spc.p[1] = Point(0,0,0);
  spc.trial = spc.p;
  
  double minY = in["xyz"]["minY"];
  double maxY = in["xyz"]["maxY"];
  double minX = in["xyz"]["minX"];
  double maxX = in["xyz"]["maxX"];
  double step = in["xyz"]["step"];
  
  //cout << "Values: " << minY << ", " << maxY << ", " << minX << ", " << maxX << ", " << step << endl;
  
  
  int cnt;
  for(double y = minY; y <= maxY; y+=step) {
    cnt = 0;
    for(double x = minX; x <= maxX; x+=step) {
      spc.p[moveParticle] = Point(x,y,in["xyz"]["z"]);
      spc.trial = spc.p;
      double energy = Energy::systemEnergy(spc,pot,spc.p);
      cnt++;
      cout << " " << energy;
    }
    cout << endl;
  }
  
    spc.p[0].update();
    spc.trial = spc.p;
    //FormatPQR::save("confout.pqr", spc.p, spc.geo.len); // final PQR snapshot for VMD etc.
    
    
    
    int Nbr = int((maxY-minY)/step);
    
    cout << in["xyz"]["minX"] << " " << in["xyz"]["maxX"] << " " << in["xyz"]["minY"] << " " << in["xyz"]["maxY"] << " " << in["xyz"]["step"];
    for(int i = 0; i < Nbr-5; i++)
        cout << " " << 0;
    cout << endl;
    
    cout << spc.p[0].nbrOfCav() << " " << spc.p[0].x() << " " << spc.p[0].y() << " " << spc.p[0].z() << " " << spc.p[0].radius << " " << spc.p[0].charge << " " << spc.p[0].is_sphere();
    for(int i = 0; i < Nbr-7; i++)
        cout << " " << 0;
    cout << endl;
    for(int k = 0; k < spc.p[0].nbrOfCav(); k++) {
      cout << spc.p[0].cap_center(k) << " " << spc.p[0].cap_radius(k) << " " << spc.p[0].angle_p(k) << " " << spc.p[0].angle_c(k);
      cout << " " << spc.p[0].cap_center_point(k).x() << " " << spc.p[0].cap_center_point(k).y() << " " << spc.p[0].cap_center_point(k).z();
      for(int i = 0; i < Nbr-7; i++)
        cout << " " << 0;
      cout << endl;
    }
    
    cout << spc.p[1].nbrOfCav() << " " << spc.p[1].x() << " " << spc.p[1].y() << " " << spc.p[1].z() << " " << spc.p[1].radius << " " << spc.p[1].charge << " " << spc.p[1].is_sphere();
    for(int i = 0; i < Nbr-7; i++)
        cout << " " << 0;
    cout << endl;
    for(int k = 0; k < spc.p[1].nbrOfCav(); k++) {
      cout << spc.p[1].cap_center(k) << " " << spc.p[1].cap_radius(k) << " " << spc.p[1].angle_p(k) << " " << spc.p[1].angle_c(k);
      cout << " " << spc.p[1].cap_center_point(k).x() << " " << spc.p[1].cap_center_point(k).y() << " " << spc.p[1].cap_center_point(k).z();
      for(int i = 0; i < Nbr-7; i++)
        cout << " " << 0;
      cout << endl;
    }
    
    cout << spc.p[0].nbrOfCav() << " " << spc.p[1].nbrOfCav();
    for(int i = 0; i < Nbr-2; i++)
        cout << " " << 0;
    cout << endl;
  
  return 0;

}