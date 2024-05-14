//  ================================================================================
//  ||                                                                            ||
//  ||              transientDiffusion                                            ||
//  ||              ------------------------------------------------------        ||
//  ||              T R A N S I E N T D I F F U S I O N                           ||
//  ||                                                                            ||
//  ||              D E M O N S T R A T I O N   C O D E                           ||
//  ||              ------------------------------------------------------        ||
//  ||                                                                            ||
//  ||       Developed by: Scott R. Runnels, Ph.D.                                ||
//  ||                     University of Colorado Boulder                         ||
//  ||                                                                            ||
//  ||                For: CU Boulder CSCI 4576/5576 and associated labs          ||
//  ||                                                                            ||
//  ||           Copyright 2020 Scott Runnels                                     ||
//  ||                                                                            ||
//  ||                     Not for distribution or use outside of the             ||
//  ||                     this course.                                           ||
//  ||                                                                            ||
//  ================================================================================

#include<mpi.h>  
#include <cstdlib>

//  ================================================================================
//  ||                                                                            ||
//  ||  M P I I O   D u m p   W r i t e r                                         ||
//  ||                                                                            ||
//  ================================================================================

void write_mpiio_dump(mpiInfo &myMPI)
{

  printf("************* Writing dump file *****************\n");

  int nRealx  = myMPI.nRealx;    // Number of real points in the x- and y-directions
  int nRealy  = myMPI.nRealy;    //
  int myPE, numPE;    // This process' ID and the number of processes total
  int nPEx,  nPEy;    // Number of processes in the x- and y-directions, in the 2D grid of processes
  int iPE ,   jPE;    // This process' logical ID in the 2D array of processors

  nPEx = myMPI.nPEx;
  nPEy = myMPI.nPEy;
  myPE = myMPI.myPE;
  numPE = myMPI.numPE;
  jPE = int(myPE/nPEx);
  iPE = myPE - jPE*nPEx;
  
  // float *phiVal;
  // phiVal = new float [nField+1];
  // for (int i = 1; i <= nField; i++) 
  //     phiVal[i] = i + myPE/10.;

  MPI_Barrier(MPI_COMM_WORLD);
  int num_send = phi.size();
  int max_send;

  MPI_Allreduce(&num_send, &max_send, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
  
  int gather_size = max_send*myMPI.numPE;
  double* phi_array = new double[phi.size()];
  int* pe_gather = new int[phi.size()];

  for(int i = 0; i < phi.size(); ++i){
    phi_array[i] = phi[i];
    pe_gather[i] = myMPI.myPE;
  }
  
  double* phiTest = new double[gather_size];
  int* phiPE = new int[gather_size];
  
    for ( int i = 0 ; i < gather_size ; ++i ) {phiTest[i] = 0.; phiPE[i] = -1;}

  MPI_Allgather( pe_gather , max_send , MPI_INT   , phiPE , max_send , MPI_INT   ,  MPI_COMM_WORLD); 
  MPI_Allgather( phi_array , max_send , MPI_DOUBLE, phiTest , max_send , MPI_DOUBLE,  MPI_COMM_WORLD); 
  
  MPI_Barrier(MPI_COMM_WORLD);
  //if(myMPI.myPE==0){
  double* combined_phi = new double[gather_size - myMPI.numPE];
  int prev_pe = -1;
  int phiCount = 0;

  //Removes the padding 0 from the front and puts all actual values into a combined phi variable
  for(int i = 0; i < gather_size; ++i){
    if(prev_pe != phiPE[i]) prev_pe = phiPE[i];
    else {
      combined_phi[phiCount] = phiTest[i];
      phiCount++;
    }
  }

  int* place = new int[myMPI.numPE];
  for (int i = 0; i < myMPI.numPE; ++i){
    place[i] = 0 + (phi.size()-1)*i;
  }
  
  int big_mesh_x = myMPI.nRealx + nCellx*(myMPI.nPEx-1);
  int big_mesh_y = myMPI.nRealy + nCelly*(myMPI.nPEy-1);
  
  int current_pe = 0;
  int pe_change = 0;
  int current_position = 0;
  int num_changedy = 0;
  int num_changedx = 0;

  double* fixed_phi = new double[big_mesh_x*big_mesh_y];
  double* flipped_phi = new double[big_mesh_x*big_mesh_y];
  int i = 0;
  
  for (int y = 0; y < big_mesh_y; ++y){
    if(y%nCelly==0 && y!=0 && (num_changedy+1 < myMPI.nPEy)){
      pe_change += myMPI.nPEx;
      num_changedy++;
    }
    current_pe = current_pe + pe_change;

    for(int x = 0; x < big_mesh_x; x++){
      if(x%nCellx==0 && x!=0 && (num_changedx+1 < myMPI.nPEx)){
	place[current_pe]++;
	current_pe++;
	num_changedx++;
      }
      
      current_position = place[current_pe];
      fixed_phi[i] = combined_phi[current_position];
      i++;
      place[current_pe]++;
    }
    current_pe = 0;
    num_changedx = 0;
  }
  int c = 0;
  //Reverse the array so that it can transpose to the correct form
  double temp = 0.;
  int mesh_len = (big_mesh_x*big_mesh_y);
  for(int n = 0; n < mesh_len/2; n++) {
    temp = fixed_phi[mesh_len-n-1];
        fixed_phi[mesh_len-n-1] = fixed_phi[n];
        fixed_phi[n] = temp;
    }

  //Transpose the phi so that it can look correct
  //Source for transpose: https://stackoverflow.com/questions/16737298/what-is-the-fastest-way-to-transpose-a-matrix-in-c
  for(int n = 0; n < big_mesh_x*big_mesh_y; n++) {
        int i = n/big_mesh_y;
        int j = n%big_mesh_y;
        flipped_phi[n] = fixed_phi[big_mesh_x*j + i];
    }

  //print out phi
<<<<<<< HEAD
  //if (myMPI.myPE==0){
    //cout << "[" << endl;
  //for(int n = 0; n < big_mesh_x*big_mesh_y; n++) {
    //if(n%big_mesh_y==0) cout << endl;
    //cout << flipped_phi[n] << "  ";
    //}
  ///cout << "]" << endl;
  //
  
  //if(myMPI.myPE == 0){
  MPI_Datatype myRealNodes;

  MPI_Type_contiguous(nTotal, MPI_DOUBLE, &myRealNodes);
  
  MPI_Type_commit(&myRealNodes);
  //
  // ------------------------------------------------------------------------------------------------------

  MPI_File_delete( "phi_dump.bin", MPI_INFO_NULL );
  
  MPI_File fh;
  
  MPI_File_open(MPI_COMM_WORLD, "phi_dump.bin", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

  MPI_File_set_view (fh, 0, MPI_DOUBLE, myRealNodes, "native", MPI_INFO_NULL);
  //                           |                                     |
  //                           |                                     |
  //                       The data type of each element     Could contain optimization hints,
  //                                                         e.g., striping information
  // ------------------------------------------------------------------------------------------------------

  //  (8) Perform the collective write operation

  
  // ------------------------------------------------------------------------------------------------------
  //                            The sub_array describing this
  //        File pointer        PE's real-nodes part of A
  //                 |                   |
  //                 |                   |
  if(myMPI.myPE==0){
  MPI_File_write(fh, flipped_phi, 1, myRealNodes, MPI_STATUS_IGNORE);
  }
  //                       |
  //                       |
  //                 Memory location of original
  //                 data set for which the
  //                 sub arrays were created.
  // ------------------------------------------------------------------------------------------------------
  
  MPI_File_close(&fh);
  MPI_Type_free(&myRealNodes);
  //}
  free(pe_gather);
  free(phiPE);
  free(phi_array);
  free(phiTest);
  free(combined_phi);
  free(fixed_phi);
  free(flipped_phi);
  
  //}
  MPI_Barrier(MPI_COMM_WORLD);
}
