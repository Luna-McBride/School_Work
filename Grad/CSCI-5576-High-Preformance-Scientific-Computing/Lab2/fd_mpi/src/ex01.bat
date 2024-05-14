#!/bin/bash

# -
# |
# | This is a batch script for running a MPI parallel job on Summit
# |
# | (o) To submit this job, enter:  sbatch --export=CODE='/home/trmc7708/Lab2/fd_mpi/src' ex01.bat 
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
module update gcc
module load gcc
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

export OMPI_MCA_opal_common_ucx_opal_mem_hooks=1

srun -n 4 ./fd -nPEx 2 -nPEy 2 -nCellx 5 -nCelly 5  > tty.out

echo "=="
echo "||"
echo "|| Execution of fd in slurm batch script complete."
echo "||"
echo "=="







