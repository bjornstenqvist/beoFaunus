#!/usr/bin/env python

from __future__ import print_function
import json, sys, os
from subprocess import call, check_output
from shutil import copyfile

import argparse
parser = argparse.ArgumentParser()
parser.add_argument('--norun', action='store_true')
args = parser.parse_args()

# 0.092129519109807         0.029133912012300

global Rl, Rc, c, Rk
Rl = 0.5
Rc = Rl
c = Rl
Rk = Rl

name = 'capparticles'

d = {
    'system': {
	"temperature"  : 293.15,
        'geometry': {'length': 10},
        'mcloop': {'macro': 10, 'micro': 200}
    },
    
    "xyz" : {
	"z":0.0, "minX":-2*Rl, "maxX":2*Rl, "minY":-2*Rl, "maxY":2*Rl, "step":0.01, "parA_is_sphere":0, "parB_is_sphere":0, 
	"moveParticle":1, "parA":0, "parB":1, "dirX0":-1, "dirY0":-1, "dirZ0":0.0, "dirX":1, "dirY":1, "dirZ":0
    },
    
  "energy" : {
    "nonbonded" : {
	"epsr" : 72.0, "cutoff":100.0, "eps_rf":72.0, "debyelength":0.288885900972721, "scale":10000.0
    }
  },
    
    'atomlist': {
      "Cap" : { "q":10.0, "q_rho":3.183098861837905, "sigma":2*Rl, "dp":0.5, "dprot":18.0, "cap_centerV":"0.5 0.5", "charge_positionV":"0 0", "cap_radiusV":"0.5 0.5", "cav_posX":"1 -1", "cav_posY":"0 0", "cav_posZ":"0 0" },
      "Sph" : { "q":1.0, "q_rho":3.183098861837905, "sigma":2*Rk, "dp":0.05, "dprot":0.0 }
    },
    'moleculelist': {
	"solA" : { "atoms":"Cap", "Ninit":3, "atomic":True, "rotate":False, 'insdir': '1 1 0' },
	"solB" : { "atoms":"Sph", "Ninit":2, "atomic":True, "rotate":False, 'insdir': '1 1 0' }
    },
  "moves" : {
    "atomtranslate" : { "solA" : { "peratom":True, "prob":1.0, "dir":"1 1 1" } }
  },
    'analysis' : {
      'povray' :   { 'file1': 'capparticles1.pov', 'camera1':'-10 0 0', 'file2': 'multicap2.pov', 'camera2':'0 -10 0', 'file3': 'multicap3.pov', 'camera3':'0 0 -10', 'file_diff': 'capparticles_diff.pov' },
        'pqrfile' :   { 'file': 'capparticles.pqr' }
        }
}

# generate json file
with open(name + '.json', 'w+') as f:
    f.write(json.dumps(d, indent=4))

if args.norun == False:
    exe = './' + name
    rc = 1
    if (os.access(exe, os.X_OK)):
        rc = call([exe])
    sys.exit(rc)
