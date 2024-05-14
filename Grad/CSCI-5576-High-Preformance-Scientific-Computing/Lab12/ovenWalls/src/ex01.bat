#!/bin/bash

# -
# |
# | This is a batch script for running a MPI parallel job on Summit
# |
# | (o) To submit this job, enter:  sbatch --export=CODE='/home/scru5660/HPSC/codes/fd_mpi/src' ex_01.bat 
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
#SBATCH --ntasks=4
#SBATCH --time=00:10:00
#SBATCH --partition=atesting
#SBATCH --output=ex01.out

# -
# |
# | Part 2: Loading software
# |
# -

module purge
module load intel
module load impi 

# -
# |
# | Part 3: User scripting
# |
# -

echo "=="
echo "||"
echo "|| Begin Execution of fd in slurm batch script."
echo "||"
echo "=="

mpirun -n 1 ./ovenWalls -nCella_outer 10 -nCellc_outer 10  -nCella_inner 10 -nCellc_inner 10 -r_outer 10. -r_inner = 3. -lo = 10. -li = 3.

echo "=="
echo "||"
echo "|| Execution of solver in slurm batch script complete."
echo "||"
echo "=="








