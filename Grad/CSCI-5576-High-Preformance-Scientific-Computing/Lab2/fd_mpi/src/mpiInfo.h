#include "fd.h"

void Exit()
{
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  exit(0);
}


//  ==
//  ||
//  ||   C L A S S:    m p i I n f o
//  ||
//  ==

class mpiInfo
{
 public:

  int myPE;
  int numPE;
  int nRealx,  nRealy;
  int nPEx, nPEy;
  int iPE , jPE;
  int iMin, iMax, jMin, jMax ; // The global i-j numbers on this processor
  int nei_n, nei_s, nei_e, nei_w;
  int countx, county;

  double *phiL, *phiR;
  double *phiT, *phiB;
    
  double *phiSend_n,  *phiSend_s;
  double *phiSend_e,  *phiSend_w;
  double *phiRecv_n,  *phiRecv_s, *phiRecv_e,  *phiRecv_w;
  
  MPI_Status  status;
  int         err;
  int         tag;
  MPI_Request request;

  //  -
  //  |
  //  |   GridDecomposition: Set up PE numbering system in figure below and
  //  |                      establish communication arrays.
  //  |
  //  |                      nPEx -- number of PEs in the x-direction
  //  |                      nPEy -- number of PEs in the y-direction
  //  |                      numPE = total number of PEs
  //  |
  //  |                       +-------+-------+ . . .   +-------+
  //  |                       |       |       |         |       |
  //  |                       |       |       |         | numPE |
  //  |                       |       |       |         |       |
  //  |                       +-------+-------+ . . .   +-------+
  //  |                       .       .       .         .       .
  //  |                       .       .       .         .       .
  //  |                       .       .       .         .       .
  //  |                       +-------+-------+ . . .   +-------+
  //  |                       |       |       |         |       |
  //  |                       | nPEx  | nPEx+1|         |       |
  //  |                       |       |       |         |       |
  //  |                       +-------+-------+ . . .   +-------+
  //  |                       |       |       |         |       |
  //  |                       |   0   |   1   |         | nPEx-1|
  //  |                       |       |       |         |       |
  //  |                       +-------+-------+ . . .   +-------+
  //  |
  //  |
  //  -
  

  void GridDecomposition(int _nPEx, int _nPEy, int nCellx , int nCelly)
  {

    nRealx = nCellx + 1;
    nRealy = nCelly + 1;

    // Store and check incoming processor counts
    
    nPEx = _nPEx;
    nPEy = _nPEy;
    
    if (nPEx*nPEy != numPE)
      {
    	if ( myPE == 0 ) cout << "Fatal Error:  Number of PEs in x-y directions do not add up to numPE" << endl;
    	MPI_Barrier(MPI_COMM_WORLD);
    	MPI_Finalize();
    	exit(0);
      }
    
    // Get the i-j location of this processor, given its number.  See figure above:
    
    jPE = int(myPE/nPEx);
    iPE = myPE - jPE*nPEx;

    // Set neighbor values

    nei_n = nei_s = nei_e = nei_w = -1;

     if ( iPE > 0      )
      {
	nei_w = myPE - 1;
      }
    if ( jPE > 0      )
      {
	nei_s = myPE-2;
      }
    if ( iPE < nPEx-1 )
      {
	nei_e = myPE+1;
      }
    if ( jPE < nPEy-1 )
      {
	nei_n=myPE+2;
      }


    countx = nRealx + 2;
    county = nRealy + 2;
    
    phiL = new double [ county ];	
    phiR = new double [ county ];	
    phiT = new double [ countx ];	
    phiB = new double [ countx ];
    
    phiSend_n = new double [ countx ];
    phiSend_s = new double [ countx ];
    phiSend_e = new double [ county ];
    phiSend_w = new double [ county ];
    phiRecv_n = new double [ countx ];
    phiRecv_s = new double [ countx ];
    phiRecv_e = new double [ county ];
    phiRecv_w = new double [ county ];

    tag = 0;
  }

  void ExchangeBoundaryInfo(VD &Solution, VD &b)
  {
	sLOOP phiSend_n[s] = 0.;
	sLOOP phiSend_s[s] = 0.;
	tLOOP phiSend_e[t] = 0.;
	tLOOP phiSend_w[t] = 0.;
	
	// ----------------------------------------------
	// (1) Parallel communication on PE Boundaries:   ** See fd.h for tLOOP and sLOOP macros **
	// ----------------------------------------------

	// (1.1) Put them into communication arrays
	
	sLOOP phiSend_n[s] = Solution[pid(s, nRealy - 1)];
	sLOOP phiSend_s[s] = Solution[pid(s, 2)];
	tLOOP phiSend_e[t] = Solution[pid(nRealx - 1, t)];
	tLOOP phiSend_w[t] = Solution[pid(2, t)];

	// (1.2) Send them to neighboring PEs
	if ( nei_n >= 0 )  err = MPI_Isend(phiSend_n, nRealx + 2, MPI_DOUBLE, nei_n, 0, MPI_COMM_WORLD, &request); 
	if ( nei_s >= 0 )  err = MPI_Isend(phiSend_s, nRealx + 2, MPI_DOUBLE, nei_s, 0, MPI_COMM_WORLD, &request); 
	if ( nei_e >= 0 )  err = MPI_Isend(phiSend_e, nRealy + 2, MPI_DOUBLE, nei_e, 0, MPI_COMM_WORLD, &request); 
	if ( nei_w >= 0 )  err = MPI_Isend(phiSend_w, nRealy + 2, MPI_DOUBLE, nei_w, 0, MPI_COMM_WORLD, &request); 

	// (1.3) Receive values from neighobring PEs' physical boundaries.
	
	if ( nei_n >= 0 ) { err = MPI_Irecv(phiRecv_n, nRealx + 2, MPI_DOUBLE, nei_n, 0, MPI_COMM_WORLD, &request);   MPI_Wait(&request,&status); } 
	if ( nei_s >= 0 ) { err = MPI_Irecv(phiRecv_s, nRealx + 2, MPI_DOUBLE, nei_s, 0, MPI_COMM_WORLD, &request);   MPI_Wait(&request,&status); } 
	if ( nei_e >= 0 ) { err = MPI_Irecv(phiRecv_e, nRealy + 2, MPI_DOUBLE, nei_e, 0, MPI_COMM_WORLD, &request);   MPI_Wait(&request,&status); } 
	if ( nei_w >= 0 ) { err = MPI_Irecv(phiRecv_w, nRealy + 2, MPI_DOUBLE, nei_w, 0, MPI_COMM_WORLD, &request);   MPI_Wait(&request,&status); } 
	
	// (1.4) If new information was received, store it in the candy-coating values

	if ( nei_n >= 0 ) sLOOP Solution[pid(s, nRealy + 1)] = phiRecv_n[s] ;  
	if ( nei_s >= 0 ) sLOOP Solution[pid(s, 0)] = phiRecv_s[s] ;    
	if ( nei_e >= 0 ) tLOOP Solution[pid(nRealx + 1, t)] = phiRecv_e[t] ;
	if ( nei_w >= 0 ) tLOOP Solution[pid(0, t)] = phiRecv_w[t] ;
	
	// (1.5) Apply exchanged information as BCs
	
	if ( nei_n >= 0 ) sLOOP b[pid(s, nRealy + 1)] = phiRecv_n[s] ;
	if ( nei_s >= 0 ) sLOOP b[pid(s, 0)] = phiRecv_s[s] ; 
	if ( nei_e >= 0 ) tLOOP b[pid(nRealx + 1, t)] = phiRecv_e[t] ;
	if ( nei_w >= 0 ) tLOOP b[pid(0, t)] = phiRecv_w[t] ;
  };

  
  int pid(int i,int j) { return (i+1) + (j)*(nRealx+2); }  

};