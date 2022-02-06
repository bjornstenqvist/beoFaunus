#!/usr/bin/env python
from __future__ import print_function
import json, sys, os
from subprocess import call, check_output
from shutil import copyfile

pfx = os.path.join( os.path.dirname(__file__), "capparticles")
try:
  copyfile( pfx+'.state', 'state' )
  copyfile( pfx+'.test', 'capparticles.test' )
except: pass

d = {
    "atomlist" : {
     # "Cap" : { "q":-0.0, "sigma":1.00, "dp":0.5, "dprot":18.0, "cap_center":0.5, "charge_position":0.0, "cap_radius":0.5 },
     # "Sph" : { "q":0.0, "sigma":0.50, "dp":0.05, "dprot":0.0, "cap_center":0.0, "charge_position":0.0, "cap_radius":0.0 }
      'Cap' : { 'q': 9.285714285714290, 'q_rho':3.183098861837906, 'sigma':2*0.5, 'dp':0.3, 'dprot':2.5, 'cap_center':0.7, 'charge_position':0.0, 'cap_radius':0.7 },
      'Sph' : { 'q': -19.6, 'sigma':2*0.7, 'dp':0.01, 'dprot':2.5, 'cap_center':0.0, 'charge_position':0.0, 'cap_radius':0.0 }
      },

    'seed':1,
    "moleculelist" : {
      "Capsphere": {"structure": "cap.aam", "Ninit": 1, "insdir": "1 1 1"},
      "Sphere": {"structure": "sph.aam", "Ninit": 1, "insdir": "1 1 1"}
      },

    "energy" : {
        "nonbonded" : {'coulombtype':"none" ,'epsr': 1.0, 'cutoff': 5.0, 'order':2, 'alpha':0.0, 'debyelength':0.092129519109807 }
      },

    "analysis" : {
      "povray" :   { "file1": "capparticles1.pov", "camera1":"-10 0 0", "file2": "capparticles2.pov", "camera2":"0 -10 0", "file3": "capparticles3.pov", "camera3":"0 0 -10", "file_diff": "capparticles_diff.pov" },
      "_xtcfile" :   { "file": "capparticles.xtc", "nstep":100000 },
      "energyfile": { "file": "energy.dat", "nstep":20 },
      "pqrfile" :   { "file": "capparticles.pqr" },
      "statefile" : { "file": "state" },
      "atomrdf" : { "nstep":10, "pairs" :
            [
               #{ "name1":"Cap", "name2":"Cap", "dim":3, "dr":0.0001, "file":"rdf_capcap.dat"}
               { "name1":"Cap", "name2":"Sph", "dim":3, "dr":0.0001, "file":"rdf_capsph.dat"}
               #{ "name1":"Sph", "name2":"Sph", "dim":3, "dr":0.0001, "file":"rdf_sphsph.dat"}
            ]
          },
      "capanalysis" : { "nstep":10, "cutoff":10, "cutoffAnalysis":10, "cutoffBound":10, "pairs" :
            [
               { "name1":"Cap", "name2":"Sph", "dim":3, "dr":0.0001, "file":"corr_capsph.dat"}
            ]
          }
      },

  "moves" : {
    "moltransrot": { 'center_rotation':True,
        "Capsphere": {"dp": 1.0, "dprot": 1.0, "dir": "1 1 1", "permol": True, "prob": 1.0},
        "Sphere": {"dp": 1.0, "dprot": 0.0, "dir": "1 1 1", "permol": True, "prob": 1.0}
    },
    '_ClusterMove' : {  'center_rotation':True, 'cluster_probability':1.0,
	'Sphere' : {'spread':False, 'threshold':0.3, 'dp':1.5, 'dprot':0.6, 'prob':1.0, 'staticmol':[] } 
	}
  },

    "system" : {
      "temperature"  : 300,
      "geometry"     : { "length" : 5.925190059452778 },
      "mcloop"       : { "macro":10, "micro":1000 },
      "unittest"     : { "testfile":"capparticles.test", "stable":False },
      "atomlist"     : "capparticles.json",
      "moleculelist" : "capparticles.json"
      }
    

    }

# generate json file
with open('multicap.json', 'w+') as f:
    f.write(json.dumps(d, indent=4))
    
f = open('cap.aam', 'w')
f.write('''1
Cap    1    0.00    0.00    0.00  -0.000  1.000 0.700
''')
f.close()

f = open('sph.aam', 'w')
f.write('''1
Sph    1    0.00    0.00    0.00  0.000  1.000 0.500
''')
f.close()

exe='./capparticles'
rc=1
if ( os.access( exe, os.X_OK )):
  rc = call( [exe] )
sys.exit( rc )
