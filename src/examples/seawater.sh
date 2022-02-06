#!/bin/bash

echo '{
  "atomlist" : {
    "Na"   : { "q": 1.0, "r":1.40, "dp":40 },
    "Cl"   : { "q":-1.0, "r":1.81, "dp":40 },
    "Mg"   : { "q": 2.0, "r":2.95, "dp":10 },
    "Ca"   : { "q": 2.0, "r":2.74, "dp":10 },
    "SO4"  : { "q":-2.0, "r":1.30, "dp":10 },
    "HCO3" : { "q":-1.0, "r":0.90, "dp":40 },
    "CO3"  : { "q":-2.0, "r":1.30, "dp":10 }
  },

  "moleculelist" : {
    "NaCl"  : { "atoms":"Na Cl", "Ninit":20, "atomic":true },
    "MgCl2" : { "atoms":"Mg Cl Cl", "Ninit":2, "atomic":true }
  },

  "energy" : {
    "nonbonded" : { "epsr":80 }
  },

  "moves" : {
    "atomtranslate" : {
      "NaCl" : { "peratom":true },
      "MgCl2" : { "peratom":true }
    }
  },

  "analysis" : {
    "widomscaled" : {"nstep":20, "ninsert":20, "lB":7}
  },

  "system" : {
    "temperature"  : 300,
    "geometry"     : { "length" : 100 },
    "mcloop"       : { "macro":10, "micro":1000 },
    "unittest"     : { "testfile":"seawater.test", "stable":false }
  }
}' > seawater.json

exe=./seawater
if [ -x $exe ]; then
  rm -fR state
  $exe
  rc=$?
  exit $rc
fi
exit 1

