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
#SBATCH --time=01:00:00
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

#export I_MPI_PMI_LIBRARY=lib64/libpmi2.so

#srun -n 1 advixe-cl --collect=survey --project-dir=./ --search-dir src:r=./ -- ./solvers -nPEx 1 -nPEy 1 -nCellx 20 -nCelly 20 -solver cg -nl nr -r 0.3 -c0 0.5 -tau 0.5

#srun -n 1 advixe-cl --collect=suitability --project-dir=./ --search-dir src:r=./ -- ./solvers -nPEx 1 -nPEy 1 -nCellx 20 -nCelly 20 -solver cg -nl nr -r 0.3 -c0 0.5 -tau 0.5

#mpirun -n 4 -gtool "advisor -collect survey -module-filter-mode=exclude -mrte-mode=auto -interval=10 -data-limit=500 -stackwalk-mode=offline -stack-stitching -mkl-user-mode -no-profile-python -no-support-multi-isa-binaries -no-spill-analysis -no-static-instruction-mix -auto-finalize -no-profile-gpu -target-gpu=none -gpu-sampling-interval=1 -profile-intel-perf-libs --app-working-dir=/home/trmc7708/Lab9/solvers_nonLinear/src:0-3" /home/trmc7708/Lab9/solvers_nonLinear/src/solvers -nPEx 2 -nPEy 2 -nCellx 20 -nCelly 20 -solver cg -nl nr -relax 0.3 -c0 1 -tau 1

#srun -n 1 ./solvers  -nPEx 1 -nPEy 1 -nCellx 20 -nCelly 20 -solver cg -nl sa -relax 0.3 -c0 0.5 -tau 0.5 -th 4 > sacg201Thread.out

#srun -n 1 ./solvers  -nPEx 1 -nPEy 1 -nCellx 200 -nCelly 200 -solver cg -nl sa -relax 0.3 -c0 0.5 -tau 0.5 -th 4 > sacg2001-4Thread.out

#srun -n 1 ./solvers  -nPEx 1 -nPEy 1 -nCellx 200 -nCelly 200 -solver cg -nl sa -relax 0.3 -c0 0.5 -tau 0.5 -th 8 > sacg2001-8Thread.out

#srun -n 1 ./solvers  -nPEx 1 -nPEy 1 -nCellx 500 -nCelly 500 -solver cg -nl sa -relax 0.3 -c0 0.5 -tau 0.5 -th 4 > sacg5001Thread.out

#srun -n 1 ./solvers  -nPEx 1 -nPEy 1 -nCellx 200 -nCelly 200 -solver cg -nl sa -relax 0.3 -c0 0.5 -tau 0.5 > sacg2001Thread.out

#mpirun -n 2 ./solvers  -nPEx 2 -nPEy 1 -nCellx 200 -nCelly 200 -solver cg -nl sa -relax 0.3 -c0 0.5 -tau 0.5 -th 4 > sacg2002-4Thread.out

#mpirun -n 2 ./solvers  -nPEx 2 -nPEy 1 -nCellx 200 -nCelly 200 -solver cg -nl sa -relax 0.3 -c0 0.5 -tau 0.5 -th 8 > sacg2002-8Thread.out

#mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 200 -nCelly 200 -solver cg -nl sa -relax 0.3 -c0 0.5 -tau 0.5 -th 4 > sacg2004-4Thread.out

mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 200 -nCelly 200 -solver cg -nl sa -relax 0.3 -c0 0.5 -tau 0.5 -th 8 > sacg2004-8Thread.out

#mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 10 -nCelly 10 -solver cg -nl sa -relax 0.3 -c0 0.5 -tau 0.5 > sacgOPTIMAL.out

#mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 10 -nCelly 10 -solver cg -nl nr -relax 0.5 -c0 1 -tau 1 > nrcgrelaxHALF.out

#mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 10 -nCelly 10 -solver cg -nl sa -relax 0.5 -c0 1 -tau 1 > sacgrelaxHALF.out

#mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 10 -nCelly 10 -solver cg -nl nr -relax 1 -c0 1 -tau 1 > nrcgrelax1.out

#mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 10 -nCelly 10 -solver cg -nl sa -relax 1 -c0 1 -tau 1 > sacgrelax1.out

#srun -n 1 ./solvers  -nPEx 1 -nPEy 1 -nCellx 40 -nCelly 40 -solver cg -nl nr > nrcg0201.out

#mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 20 -nCelly 20 -solver cg -nl nr > nrcg204.out

#srun -n 1 ./solvers  -nPEx 1 -nPEy 1 -nCellx 20 -nCelly 20 -solver cg -nl sa > sacg0101.out



#srun -n 1 ./solvers  -nPEx 1 -nPEy 1 -nCellx 40 -nCelly 40 -solver cg -nl sa > sacg0201.out

#mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 20 -nCelly 20 -solver cg -nl sa > sacg204.out

#srun -n 1 ./solvers  -nPEx 1 -nPEy 1 -nCellx 20 -nCelly 20 -solver jacobi -nl nr > nrja0101.out

#mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 10 -nCelly 10 -solver jacobi -nl nr > nrja104.out

#srun -n 1 ./solvers  -nPEx 1 -nPEy 1 -nCellx 40 -nCelly 40 -solver jacobi -nl nr > nrja0201.out

#mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 20 -nCelly 20 -solver jacobi -nl nr > nrja204.out

#srun -n 1 ./solvers  -nPEx 1 -nPEy 1 -nCellx 20 -nCelly 20 -solver jacobi -nl sa > saja0101.out

#mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 10 -nCelly 10 -solver jacobi -nl sa > saja104.out

#srun -n 1 ./solvers  -nPEx 1 -nPEy 1 -nCellx 40 -nCelly 40 -solver jacobi -nl sa > saja0201.out

#mpirun -n 4 ./solvers  -nPEx 2 -nPEy 2 -nCellx 20 -nCelly 20 -solver jacobi -nl sa > saja204.out

echo "=="
echo "||"
echo "|| Execution of solver in slurm batch script complete."
echo "||"
echo "=="








