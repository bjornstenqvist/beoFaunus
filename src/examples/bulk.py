#!/usr/bin/env python
from __future__ import print_function
import json, sys, os
from subprocess import call, check_output
from shutil import copyfile

pfx = os.path.join( os.path.dirname(__file__), "bulk")
try:
  copyfile( pfx+'.state', 'state' )
  copyfile( pfx+'.test', 'bulk.test' )
except: pass

d = {
    "atomlist" : {
      "Na" : { "q": 1.0, "sigma":3.33, "eps":0.01158968, "dp":1.0 },
      "Cl" : { "q":-1.0, "sigma":4.40, "eps":0.4184,     "dp":1.0 }
      },

    "moleculelist" : {
      "NaCl" : { "atoms":"Na Cl", "Ninit":1152, "atomic":True }
      },

    "energy" : {
        "nonbonded" : {'coulombtype':"yonezawa" ,'epsr': 1.0, 'cutoff': 14, 'order':2, 'alpha':0.0,
		       "ewald": { "cutoff": 14, "epsr":1, "eps_surf": 1e11, "alpha": 0.2, "cutoffK":0, "spherical_sum":True}
		       }
      },

    "analysis" : {
      "virial" : { "nstep":5 },
      "virtualvolume" : { "nstep":5, "dV":0.2 },
      "xtcfile" :   { "file": "bulk.xtc", "nstep":20 },
      "energyfile": { "file": "energy.dat", "nstep":20 },
      "pqrfile" :   { "file": "bulk.pqr" },
      "statefile" : { "file": "state" },
      "atomrdf" : { "nstep":10, "pairs" :
            [
               { "name1":"Na", "name2":"Cl", "dim":3, "dr":0.1, "file":"rdf_nacl.dat"},
               { "name1":"Na", "name2":"Na", "dim":3, "dr":0.1, "file":"rdf_nana.dat"}
            ]
          }
      },

    "moves" : {
      "isobaric" : { "dp":0.0, "pressure":11, "prob":0.0 },
      "atomtranslate" : { 
          "NaCl" : { "peratom":True, "prob": 1.0 }
        }
      },

    "system" : {
      "temperature"  : 1100,
      "geometry"     : { "length" : 42.5 },
      "mcloop"       : { "macro":10, "micro":50 },
      "unittest"     : { "testfile":"bulk.test", "stable":False },
      "atomlist"     : "bulk.json",
      "moleculelist" : "bulk.json"
      }
    }

# generate json file
with open('bulk.json', 'w+') as f:
    f.write(json.dumps(d, indent=4))

exe='./bulk'
rc=1
if ( os.access( exe, os.X_OK )):
  rc = call( [exe] )
sys.exit( rc )
