#!/bin/bash

# -
# |
# | This is a batch script for running a MPI parallel job on Summit
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
#SBATCH --output=ex01.out

# -
# |
# | Part 2: Loading software
# |
# -

module purge
module load intel
module load intel/2022.1.2
module load advisor/2022.0.0
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

srun -n 1 advixe-cl --collect=survey --project-dir=./ --search-dir src:r=./ -- ./solvers  -nPEx 1 -nPEy 1 -nCellx 10 -nCelly 10 -solver cg -nl nr

echo "=="
echo "||"
echo "|| Execution of solver in slurm batch script complete."
echo "||"
echo "=="








