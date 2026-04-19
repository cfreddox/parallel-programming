#!/bin/bash
#SBATCH --job-name=lab3_intel
#SBATCH --time=00:15:00
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --mem=8gb
#SBATCH --output=slurm-%j.out
#SBATCH --partition=batch

export LD_LIBRARY_PATH=/soft/intel_ps/compilers_and_libraries_2018.2.199/linux/mpi/intel64/lib:$LD_LIBRARY_PATH

cd $SLURM_SUBMIT_DIR

echo "=== Start Job ==="
chmod +x lab3_intel
srun ./lab3_intel
echo "=== End Job ==="
