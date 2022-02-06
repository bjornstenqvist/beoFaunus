#!/bin/sh
#SBATCH -J SE
#SBATCH -n 1
#SBATCH -p snic      
#SBATCH -t 168:00:00
#SBATCH --mail-user=bjorn.stenqvist@teokem.lu.se
#SBATCH --mail-type=FAIL
#SBATCH -A snic2018-3-35

time ./run.py
