#!/bin/bash

faunus=../../../
exe=$faunus/src/playground/mlund/mlund-manybody
base="manybody"

# ----------------------------------
#   GENERATE TITRATION INPUT
# ----------------------------------
function mktit() {
echo "
Process  H3PO4 H2PO4   2.12    $pH
Process  H2PO4 HPO4    7.21    $pH
Process  HPO4  PO4     12.67   $pH
Process  HASP  ASP     4.0     $pH
Process  HCTR  CTR     2.6     $pH
Process  HGLU  GLU     4.4     $pH
Process  HHIS  HIS     6.3     $pH
Process  HNTR  NTR     7.5     $pH
Process  HTYR  TYR     9.6     $pH
Process  HLYS  LYS     10.4    $pH
Process  HCYS  CYS     10.8    $pH
Process  HARG  ARG     12.0    $pH
" > $base.titration
}

# ----------------------------------
#   GENERATE ATOM PARAMETERS
# ----------------------------------
function mkatoms() {
echo "
Atom  Na      +1     2.0    0.1    1       no
Atom  Cl      -1     2.0    0.1    1       no
Atom  I       -1     2.0    0.1    1       no
Atom  SCN     -1     2.0    0.1    1       no
Atom  MM      +1     3.0    0.1    1       no

Atom  ASP     -1     3.6    0.1    110     no
Atom  HASP     0     3.6    0.1    110     no
Atom  CTR     -1     2.0    0.1    16      no
Atom  HCTR     0     2.0    0.1    16      no
Atom  GLU     -1     3.8    0.1    122     no
Atom  HGLU     0     3.8    0.1    122     no
Atom  HIS      0     3.9    0.1    130     no
Atom  HHIS     1     3.9    0.1    130     no
Atom  NTR      0     2.0    0.1    14      no
Atom  HNTR     1     2.0    0.1    14      no
Atom  TYR     -1     4.1    0.1    154     no
Atom  HTYR     0     4.1    0.1    154     no
Atom  LYS      0     3.7    0.1    116     no
Atom  HLYS     1     3.7    0.1    116     no
Atom  CYS     -1     3.6    0.1    103     no 
Atom  HCYS     0     3.6    0.1    103     no 
Atom  ARG      0     4.0    0.1    144     no
Atom  HARG     1     4.0    0.1    144     no

Atom  ALA      0     3.1    0.1    66      yes
Atom  ILE      0     3.6    0.1    102     yes
Atom  LEU      0     3.6    0.1    102     yes
Atom  MET      0     3.8    0.1    122     yes
Atom  PHE      0     3.9    0.1    138     yes
Atom  PRO      0     3.4    0.1    90      yes
Atom  TRP      0     4.3    0.1    176     yes
Atom  VAL      0     3.4    0.1    90      yes
Atom  SER      0     3.3    0.1    82      no
Atom  THR      0     3.5    0.1    94      no
Atom  ASN      0     3.6    0.1    108     no
Atom  GLN      0     3.8    0.1    120     no
Atom  GLY      0     2.9    0.1    54      no
" > $base.atoms
}

# ----------------------------------
#   GENERATE MAIN INPUT PARAMETERS
# ----------------------------------
function mkinput() {
echo "
atomlist               $base.atoms
eq_processfile         $base.titration

loop_macrosteps        10
loop_microsteps        $micro

temperature            298     # Kelvin
epsilon_r              780.7    # Water dielectric const
dh_ionicstrength       0.010   # mol/l
lj_eps                 0.05    # kT
pairpot_cutoff         30
squarewell_depth       0.0     # kT
squarewell_threshold   1.5     # angstrom

cuboid_len             $boxlen # Box side length Angstrom
npt_P                  113.2   # mM
npt_dV                 0       # log(dV)
transrot_transdp       50 #180
transrot_rotdp         2 #6

molecule_N1            100
molecule_file1         $base.aam
molecule_N2            100
molecule_file2         $base.aam

test_stable            yes
test_file              $base.test
" > $base.input
}

# ----------------------------------
#   GENERATE INPUT MOLECULE
# ----------------------------------
function mkstruct() {
echo "3
 ASP  0   0.00   0.00   0.00    -0   1  3.0
 UNK  0   3.00   0.00   0.00    -0   1  3.0
 HIS  0   6.00   0.00   0.00    -0   1  3.0
" > ${base}.aam
}

mkatoms
mkstruct
boxlen=100

for pH in 4 
do
  # equilibration
  rm -fR $base.state
  micro=100
  mktit
  mkinput
  $exe -i $base.input -c $base.state -o $base.state #> eq
  exit

  # production
  micro=1000
  mktit
  mkinput
  $exe -i $base.input -c $base.state -o $base.state > out
  z=`tail out | grep $base.aam | gawk '{print $2}'`
  echo $pH $z
done

