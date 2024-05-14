#!/bin/bash

# -
# |
# | This is a batch script for running a MPI parallel job on Summit
# |
# | (o) To submit this job, enter:  sbatch --export=CODE='/home/trmc7708/Lab5/cs-roofline-toolkit/Empirical_Roofline_tool-1.1.0' ex01.bat 
# |
# | (o) To check the status of this job, enter: squeue -u <username>
# |
# -

# -
# |
# | Part 1: Directives
# |
# -

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=00:01:00
#SBATCH --partition=atesting
#SBATCH --output=ex01-%j.out

# -
# |
# | Part 2: Loading software
# |
# -

module purge
module load intel
module load impi
module use /projects/scru5660/public/software/module

# -
# |
# | Part 3: User scripting
# |
# -

echo "=="
echo "||"
echo "|| Begin Execution of Roofline Toolkit in slurm batch script."
echo "||"
echo "=="

lstopo --of ascii > topo
lscpu > cpuInfo
./ert Config/alpine  > tty.out

echo "=="
echo "||"
echo "|| Execution of Roofline Toolkit in slurm batch script complete."
echo "||"
echo "=="








