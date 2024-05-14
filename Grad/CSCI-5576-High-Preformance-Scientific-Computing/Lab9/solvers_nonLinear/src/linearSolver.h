//  ================================================================================
//  ||                                                                            ||
//  ||              solvers                                                       ||
//  ||              ------------------------------------------------------        ||
//  ||              S O L V E R S                                                 ||
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

//  ==
//  ||
//  ||  Jacobi
//  ||
//  ||  Performs Jacobi iterations on nField X nField linear
//  ||  system:  A*Solution = RHS
//  ||
//  ==

void Jacobi(VDD &Matrix , VD &RHS , VD &Solution , mpiInfo &myMPI )
  {
    int converged;
    int iter = 0;
    double newval;
    double cur_delta = 0.;
    double tol = 1.e-18;
    int max_iter = nField * 100;

    MPI_Status status;
    int        tag = 0;
    int        err;
    int        global_converged = 0;
    int        zero = 0;
    int        one  = 1;
    
    // (1) Initial guess

    //    rLOOP Solution[r] = 1.;
    
    // (2) Temporary storage for parallel communication
    
    VD Diag_sumPE  ; Diag_sumPE.resize  ( Solution.size() );  // Stores the diagonal for each node
    VD RowSum_sumPE; RowSum_sumPE.resize( Solution.size() );  // Stores RHS - Jacobisum for each node

    rLOOP Diag_sumPE[r] = Matrix[r][1];

    // (3) Sum diagonals on PE boundaries

    myMPI.PEsum( Diag_sumPE );
    
    // (4) Begin Iterations
    
    while ( global_converged == 0 && ++iter <= max_iter )
      {
	// (4.1) Update each row
	
	converged = 1;
	
	rLOOP
	  {
	    RowSum_sumPE[r] = RHS[r];
	    for ( int c = 2 ; c <= bandwidth ; ++c ) RowSum_sumPE[r] -=  Matrix[r][c] * Solution[Jcoef[r][c]];
	  }
	
	myMPI.PEsum( RowSum_sumPE );

	rLOOP
	  {
	    newval = RowSum_sumPE[r]/Diag_sumPE[r];
	    cur_delta  = fabs(Solution[r] - newval);
	    if ( cur_delta > tol ) converged = 0;
	    Solution[r]       = newval;
	  }

	// (4.2) Check convergence across PEs

	MPI_Allreduce(&converged, &global_converged, 1 , MPI_INT, MPI_MIN, MPI_COMM_WORLD);
      }
    
    // (5) Done - inform user

    if ( global_converged == 1 ) if ( myMPI.myPE == 0 ) cout << "  (o) Jacobi converged in " << iter << " iterations.\n" ;
    if ( global_converged == 0 ) if ( myMPI.myPE == 0 ) cout << "  (o) Jacobi failed to converge after " << iter << " iterations.\n" ;

  }





//  ==//  ||
//  || Utility routine: Dot
//  ||
//  ||
//  || Returns the dot product of vec1 and vec2, where vec1 and vec2 are complete
//  || on each PE.
//  ||
//  ==

double Dot(VD &vec1, VD &vec2 , mpiInfo &myMPI, int size)
{
  ANNOTATE_SITE_BEGIN(Dot);
  double  sum  = 0.;

  // peMulticitgy = 1 except on PE boundaries where it is equal to the
  // number of PEs containing the given node.
  ANNOTATE_TASK_BEGIN(Sum);
  sizeLOOP sum += vec1[row]*vec2[row] / myMPI.peMultiplicity[row];
  ANNOTATE_TASK_END();
  // Sum results across PEs and share that sum with all PEs

  double globalSum; 
  MPI_Allreduce(&sum, &globalSum, 1 , MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);   
  ANNOTATE_SITE_END();
  return globalSum;
}


//  ==
//  ||
//  || Utility routine: MatVecProd
//  ||
//  || Computes the matrix-vector product, prod = A*p where p is complete
//  || on each PE but A must be summed on PE boundaries.
//  ||
//  ==

void MatVecProd(VDD &Matrix , VD &p , VD &prod , mpiInfo &myMPI, int size, int Lower)
{

  // Serial computation on this PE
  ANNOTATE_SITE_BEGIN(MatVecProd);
  ANNOTATE_TASK_BEGIN(MainLoop);
  sizeLOOP
    {
      prod[row] = 0.;
      colLOOP
	{
	  
	  int Acol = Jcoef[row+Lower-1][col];
	  if ( Acol > 0 ) prod[row] += Matrix[row][col] * p[ Acol ];
	}
    }
  ANNOTATE_TASK_END();
  // Handle PE boundaries

  myMPI.PEsum(prod);
  ANNOTATE_SITE_END();
}

//  ==
//  ||
//  || Utility routine: Residual
//  ||
//  || Computes the residual, residual = b - A*Sol, where Sol is complete on
//  || each PE but b and A must be summed at PE boundaries.
//  ||
//  ==

void Residual(VDD &Matrix , VD &residual , VD &Sol , VD &RHS, mpiInfo &myMPI, int size, int Lower)
{
  
  ANNOTATE_SITE_BEGIN(Residual);
  MatVecProd(Matrix, Sol , residual , myMPI, size, Lower);
  
  ANNOTATE_TASK_BEGIN(rowLOOP);
  sizeLOOP residual[row] = RHS[row] - residual[row];
  ANNOTATE_TASK_END();
  ANNOTATE_SITE_END();
}



// ==
// ||
// || CG (Conjugate Gradient)
// ||
// || Solves the system using Conjugate-Gradient iteration.
// ||
// || See https://en.wikipedia.org/wiki/Conjugate_gradient_method
// ||
// ==

void CG(VDD &Matrix , VD &RHS , VD &Solution , mpiInfo & myMPI, int Lower, int Upper, int size)
{
  
  VD rnew; rnew.resize(nField + 1);
  VD r;       r.resize(nField + 1);
  VD p;       p.resize(nField + 1);
  VD Ap;     Ap.resize(nField + 1);
    
  double p_dot_Ap;         // Stores matrix-vector product
  double r_dot_r;          // Stores r dot r
  double rnew_dot_rnew;    // Stores rnew dot rnew
  double alpha;            // Alpha in the above-referenced algorithm
  double beta;             // Beta   "  "   "       "          "
  double cur_delta;
  double tol           = 1.e-15;
  int global_converged = 0;
  int iter             = 0;
  int max_iter         = nField * 0.5;
  int converged;

  // (1) Initial guess and other initializations

  sizeLOOP p[row] = r[row] = Ap[row] = rnew[row] =  0.;
  
  Solution     [0] = 0.;
  p       [0] = 0.;
  r       [0] = 0.;

  // (2) Prepare for parallel computations on RHS
  
  VD b_PEsum ;
  b_PEsum.resize(nField + 1 ) ;
  sizeLOOP b_PEsum[row] = RHS[row];
  myMPI.PEsum(b_PEsum);           
  
  // (3) Initialize residual, r, and r dot r for CG algorithm
  
  
  Residual(Matrix , r , Solution,b_PEsum,myMPI, size, Lower);
  sizeLOOP p[row] = r[row];

  r_dot_r  = Dot(r,r,myMPI, size);         
      
  // (4) CG Iterations

  ANNOTATE_SITE_BEGIN(CG_iterations);
  
  while ( global_converged == 0  && ++iter <= max_iter)
    {
      // (4.1) Compute alpha

      ANNOTATE_TASK_BEGIN(MatVecProd);
      MatVecProd(Matrix,p,Ap,myMPI, size, Lower);      // A*p (stored in Ap)
      ANNOTATE_TASK_END();

      ANNOTATE_TASK_BEGIN(Dot);
      p_dot_Ap = Dot(p,Ap,myMPI, size); // p*Ap
      ANNOTATE_TASK_END();
      
      alpha    = r_dot_r / p_dot_Ap;

      // (4.2) Update solution and residual
      ANNOTATE_TASK_BEGIN(R_Update);
      sizeLOOP {Solution [row] = Solution[row] + alpha *  p[row]; rnew    [row] = r[row]        - alpha * Ap[row];}
      
      // (4.3) Compute beta

      rnew_dot_rnew = Dot(rnew,rnew,myMPI, size);
      beta          = rnew_dot_rnew / r_dot_r;
      
      // (4.4) Update search direction
      sizeLOOP {p[row] = rnew[row] + beta*p[row]; r[row] = rnew[row];}
      
      // (4.5) r "new" will be r "old" for next iteration

      
      r_dot_r        = rnew_dot_rnew;
      ANNOTATE_TASK_END();
      

      // (4.6) Check convergence on this PE

      ANNOTATE_TASK_BEGIN(Check_Converge);

      if ( fabs(r_dot_r) < tol)
	converged = 1;
      else
	converged = 0;
      ANNOTATE_TASK_END();
      
      // (4.7) Check convergence across PEs, store result in "global_converged"

      
      MPI_Allreduce(&converged, &global_converged, 1 , MPI_INT, MPI_MIN, MPI_COMM_WORLD);   
    }
  //}
  ANNOTATE_SITE_END();
      
    // (5) Done - Inform user

  if ( global_converged == 1 ) if ( myMPI.myPE == 0 ) cout << "  (o) CG converged in " << iter << " iterations.\n" ;
  if ( global_converged == 0 ) if ( myMPI.myPE == 0 ) cout << "  (o) CG failed to converge after " << iter << " iterations.\n" ;

}
