#!/usr/bin/env python
from __future__ import print_function
import json, sys, os
from subprocess import call
from shutil import copyfile

pfx = os.path.join( os.path.dirname(__file__), "grand")
try:
  copyfile( pfx+'.state', 'state' )
  copyfile( pfx+'.test', 'grand.test' )
except: pass

def mkinput():
  d = {
      "moleculelist": {
        "salt": { "Ninit": 20, "atomic": True, "atoms": "Na Cl" }
        }, 

      "energy": {
        "nonbonded": { "epsr": 80 }
        }, 

      "moves": {
        "atomtranslate": { "salt": { "permol": True, "prob": 0.01 } }, 
        "atomgc": { "molecule": "salt" },
        "random": { "hardware": True }
        }, 

      "system": {
        "mcloop"      : { "macro": 10, "micro": 100000 }, 
        "geometry"    : { "radius": 80 }, 
        "unittest"    : { "testfile": "grand.test", "stable": False }, 
        "coulomb"     : { "epsr": 80 },
        "temperature" : 298.15
        },

      "analysis" : {
        "widom" : { "nstep":20, "ninsert":15, "particles":["Na", "Cl"] },
        "widomscaled" : { "nstep":20, "ninsert":15, "lB":7.0 },
        "pqrfile" :   { "file": "grand.pqr" },
        "statefile" : { "file": "state" }
        },

      "atomlist": {
        "Na": { "q":  1.0, "r": 2.0, "dp": 50, "activity": 0.05 }, 
        "Cl": { "q": -1.0, "r": 2.0, "dp": 50, "activity": 0.05 }
        }
      }
  with open('grand.json', 'w+') as f:
      f.write(json.dumps(d, indent=4))

exe='./grand'
if ( os.access( exe, os.X_OK )):
  mkinput()
  rc = call( [exe] )
  sys.exit( rc )

