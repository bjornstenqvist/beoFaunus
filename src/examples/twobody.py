#!/usr/bin/env python
from __future__ import print_function
import json, sys, os
from subprocess import call, check_output
from shutil import copyfile

def mkinput():
  global Cs, pH, micro

  d = {
      "energy" : {
        "nonbonded" : {
          "eps":0.05, "epsr" : 78.7, "ionicstrength" : Cs
          },
        "cmconstrain" : {
          "protein protein" : { "mindist": 0, "maxdist": 1000 }
          }
        },

      "atomlist" : {
        "H3PO4":  { "q":0,  "r":2.0 },
        "H2PO4":  { "q":-1, "r":2.0 },
        "HPO4" :  { "q":-2, "r":2.0 },
        "PO4"  :  { "q":-3, "r":2.0 },
        "BPTI" :  { "q":7.3, "r":12.29 },
        "Na"   :  dict(q=1.0, r=1.9, mw=22.99, dp=100),
        "Cl"   :  { "q":-1, "r":1.7, "mw":35.45, "dp":100 },
        "I"    :  { "q":-1, "r":2.0, "mw":1 },
        "SCN"  :  { "q":-1, "r":2.0, "mw":1 },
        "ASP"  :  { "q":-1, "r":3.6, "mw":110 },
        "HASP" :  { "q":0,  "r":3.6, "mw":110 },
        "LASP" :  { "q":2,  "r":3.6, "mw":110 },
        "CTR"  :  { "q":-1, "r":2.0, "mw":16 },
        "HCTR" :  { "q":0,  "r":2.0, "mw":16 },
        "GLU"  :  { "q":-1, "r":3.8, "mw":122 },
        "HGLU" :  { "q":0,  "r":3.8, "mw":122 },
        "LGLU" :  { "q":2,  "r":3.8, "mw":122 },
        "HIS"  :  { "q":0,  "r":3.9, "mw":130 },
        "HHIS" :  { "q":1,  "r":3.9, "mw":130 },
        "NTR"  :  { "q":0,  "r":2.0, "mw":14 },
        "HNTR" :  { "q":1,  "r":2.0, "mw":14 },
        "TYR"  :  { "q":-1, "r":4.1, "mw":154 },
        "HTYR" :  { "q":0,  "r":4.1, "mw":154 },
        "LYS"  :  { "q":0,  "r":3.7, "mw":116 },
        "HLYS" :  { "q":1,  "r":3.7, "mw":116 },
        "CYb"  :  { "q":0,  "r":3.6, "mw":103 },
        "CYS"  :  { "q":-1, "r":3.6, "mw":103 },
        "HCYS" :  { "q":0,  "r":3.6, "mw":103 },
        "ARG"  :  { "q":0,  "r":4.0, "mw":144 },
        "HARG" :  { "q":1,  "r":4.0, "mw":144 },
        "ALA"  :  { "q":0,  "r":3.1, "mw":66 },
        "ILE"  :  { "q":0,  "r":3.6, "mw":102 },
        "LEU"  :  { "q":0,  "r":3.6, "mw":102 },
        "MET"  :  { "q":0,  "r":3.8, "mw":122 },
        "PHE"  :  { "q":0,  "r":3.9, "mw":138 },
        "PRO"  :  { "q":0,  "r":3.4, "mw":90 },
        "TRP"  :  { "q":0,  "r":4.3, "mw":176 },
        "VAL"  :  { "q":0,  "r":3.4, "mw":90 },
        "SER"  :  { "q":0,  "r":3.3, "mw":82 },
        "THR"  :  { "q":0,  "r":3.5, "mw":94 },
        "ASN"  :  { "q":0,  "r":3.6, "mw":108 },
        "GLN"  :  { "q":0,  "r":3.8, "mw":120 },
        "GLY"  :  { "q":0,  "r":2.9, "mw":54 }
        },

      "processes" : {
          "H-Asp" : { "bound":"HASP" , "free":"ASP" , "pKd":4.0  , "pX":pH },
          "H-Ctr" : { "bound":"HCTR" , "free":"CTR" , "pKd":2.6  , "pX":pH },
          "H-Glu" : { "bound":"HGLU" , "free":"GLU" , "pKd":4.4  , "pX":pH },
          "H-His" : { "bound":"HHIS" , "free":"HIS" , "pKd":6.3  , "pX":pH },
          "H-Arg" : { "bound":"HARG" , "free":"ARG" , "pKd":12.0 , "pX":pH },
          "H-Ntr" : { "bound":"HNTR" , "free":"NTR" , "pKd":7.5  , "pX":pH },
          "H-Cys" : { "bound":"HCYS" , "free":"CYS" , "pKd":10.8 , "pX":pH },
          "H-Tyr" : { "bound":"HTYR" , "free":"TYR" , "pKd":9.6  , "pX":pH },
          "H-Lys" : { "bound":"HLYS" , "free":"LYS" , "pKd":10.4 , "pX":pH },
          "K1"    : { "bound":"H3PO4", "free":"H2PO4","pKd":2.12,  "pX":pH },
          "K2"    : { "bound":"H2PO4", "free":"HPO4", "pKd":7.21,  "pX":pH },
          "K3"    : { "bound":"HPO4",  "free":"PO4",  "pKd":12.67, "pX":pH }
          },

      "moleculelist": {
          "protein1":  dict(structure='manybody.aam', Ninit=1, insdir='0 0 0', insoffset='0 0 -20'),
          "protein2":  dict(structure='manybody.aam', Ninit=1, insdir='0 0 0', insoffset='0 0 20'),
          "salt":      dict(atoms="Na Cl", Ninit=0, atomic=True)
          },

      "moves" : {
          "titrate" : { "prob":0.1,
             "processes" : {
                 "H-Asp" : { "bound":"HASP" , "free":"ASP" , "pKd":4.0  , "pX":pH },
                 "H-Ctr" : { "bound":"HCTR" , "free":"CTR" , "pKd":2.6  , "pX":pH },
                 "H-Glu" : { "bound":"HGLU" , "free":"GLU" , "pKd":4.4  , "pX":pH },
                 "H-His" : { "bound":"HHIS" , "free":"HIS" , "pKd":6.3  , "pX":pH },
                 "H-Arg" : { "bound":"HARG" , "free":"ARG" , "pKd":12.0 , "pX":pH },
                 "H-Ntr" : { "bound":"HNTR" , "free":"NTR" , "pKd":7.5  , "pX":pH },
                 "H-Cys" : { "bound":"HCYS" , "free":"CYS" , "pKd":10.8 , "pX":pH },
                 "H-Tyr" : { "bound":"HTYR" , "free":"TYR" , "pKd":9.6  , "pX":pH },
                 "H-Lys" : { "bound":"HLYS" , "free":"LYS" , "pKd":10.4 , "pX":pH },
                 "K1"    : { "bound":"H3PO4", "free":"H2PO4","pKd":2.12,  "pX":pH },
                 "K2"    : { "bound":"H2PO4", "free":"HPO4", "pKd":7.21,  "pX":pH },
                 "K3"    : { "bound":"HPO4",  "free":"PO4",  "pKd":12.67, "pX":pH }
                 }
              },
          "//atomtranslate" : {
            "salt" : { "peratom":True }
            },
          "random" : { "hardware": False},
          "//moltransrotcluster" : {
            "protein" : { "dp":0, "dprot":3, "prob":1.0, "permol":True, "dir":"0 0 1",
              "threshold":10, "clustergroup": "salt"} 
            }, 
          "moltransrot2body" : {
            "protein1" : dict(dp=20, dprot=3, dir="0 0 1"), 
            "protein2" : dict(dp=20, dprot=3, dir="0 0 1")
            }
          },

      "analysis" : {
          "cyldensity" : { "atomtype":"Na", "zmin":-125, "zmax":125  },
          "pqrfile" :   { "file": "confout.pqr" },
          "statefile" : { "file": "state" },
          "xtcfile" :   { "file": "traj.xtc", "nstep":1000 },
          "molrdf" : {
                "nstep":2, "pairs" : [
                       dict(name1="protein1", name2="protein2", dim=1, file="rdf.dat", dr=0.25)
                    ]
              }
          },

      "system" : {
          "temperature" : 298.15,
          "geometry" : { "length" : 200, "radius" : 40 },
          "mcloop"   : { "macro" : 10, "micro" : micro }
          }
      }
  with open('twobody.json', 'w+') as f:
      f.write(json.dumps(d, indent=4))

# Main execution starts here.
exe="./twobody"

runeq=True
runprod=True
copydata=False

for Cs in [0.05]:  # ionic strength (mol/l)
  for pH in [4.0]:

    prefix="Cs"+str(Cs)+"-pH"+str(pH)

    print(prefix)

    # Equilibration
    if (runeq==True):
      print("Equilibration run...(state file deleted)")
      try:
        os.remove('state')
      except: pass

      micro=5000
      mkinput()
      with open(prefix+'.eq', 'w+') as f:
          f.write( check_output( [exe] ).decode("utf-8")  )

      #print >> open(prefix+'.eq', 'w+'), check_output( [exe] )

    # Production run
    if (runprod==True):
      print("Production run...")
      micro=100000
      mkinput()
      with open(prefix+'.out', 'w+') as f:
          f.write( check_output( [exe] ).decode("utf-8")  )

    # Copy data
    if (copydata==True):
      copyfile( 'state', prefix+'.state' )
      copyfile( 'rdf.dat', prefix+'.rdf.dat' )
      copyfile( 'confout.pqr', prefix+'.pqr' )
      copyfile( 'twobody.json', prefix+'.json' )

