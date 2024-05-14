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

mpirun -n 1 $CODE/solvers -nPEx 1 -nPEy 1 -nCellx 18 -nCelly 18 -solver cg > tty1.out

mpirun -n 2 $CODE/solvers -nPEx 2 -nPEy 1 -nCellx 9 -nCelly 18 -solver cg > tty2.out

mpirun -n 3 $CODE/solvers -nPEx 3 -nPEy 1 -nCellx 6 -nCelly 18 -solver cg > tty3.out

mpirun -n 4 $CODE/solvers -nPEx 2 -nPEy 2 -nCellx 9 -nCelly 9 -solver cg > tty4.out

echo "=="
echo "||"
echo "|| Execution of solver in slurm batch script complete."
echo "||"
echo "=="








