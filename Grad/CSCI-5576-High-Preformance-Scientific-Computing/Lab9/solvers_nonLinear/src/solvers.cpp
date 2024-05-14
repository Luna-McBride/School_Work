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

#include "mpi.h"
#include "omp.h"
#include "solvers.h"
#include "mpiInfo.h"
#include "LaplacianOnGrid.h"


//  ==
//  ||
//  ||
//  ||  Main Program
//  ||
//  ||
//  ==

int main(int argc, char *argv[])
{
  
   mpiInfo myMPI;
   MPI_Init     (&argc         , &argv       );
   MPI_Comm_size(MPI_COMM_WORLD, &myMPI.numPE);
   MPI_Comm_rank(MPI_COMM_WORLD,&myMPI.myPE  );


   int nPEx, nPEy, nCellx, nCelly;
   string   solver;
   string nlsolver;
   double relax = 0.3;
   double c0    = 1.;
   double tau   = 1.;
   int bandwidth = 5;
   
   int numTH = 4;

   timingInfo myTime("main");
   myTime.Start(myMPI.myPE);
   
   if ( myMPI.myPE == 0 )
     {
       cout << "\n";
       cout << "------------------------------------------------------\n";
       cout << "\n";
       cout << " S O L V E R S                                        \n";
       cout << " D E M O   C O D E                                    \n";
       cout << "\n";
       cout << " Running on " << myMPI.numPE << " processors \n";
       cout << "\n";
       cout << "------------------------------------------------------\n";
       cout << "\n";
     }
    
   nPEx = nPEy = 1;  nCellx = nCelly = 10;

   solver   = "none";
   nlsolver = "none";
     
   for (int count =  0 ; count < argc; ++count)
     {
       if ( !strcmp(argv[count],"-nPEx"    ) ) nPEx   = atoi(argv[count+1]);
       if ( !strcmp(argv[count],"-nPEy"    ) ) nPEy   = atoi(argv[count+1]);
       if ( !strcmp(argv[count],"-nCellx"  ) ) nCellx = atoi(argv[count+1]);
       if ( !strcmp(argv[count],"-nCelly"  ) ) nCelly = atoi(argv[count+1]);
       if ( !strcmp(argv[count],"-solver"  ) ) solver =      argv[count+1] ;
       if ( !strcmp(argv[count],"-nl"      ) ) nlsolver =      argv[count+1] ;
       if ( !strcmp(argv[count],"-r"       ) ) relax    = atof(argv[count+1]) ;
       if ( !strcmp(argv[count],"-c0"      ) ) c0       = atof(argv[count+1]) ;
       if ( !strcmp(argv[count],"-tau"     ) ) tau      = atof(argv[count+1]) ;
       if ( !strcmp(argv[count],"-th"     ) ) numTH      = atoi(argv[count+1]) ;
     }
   int nField = (nCellx+1) * (nCelly+1);
   int numPerTh;
   int Lower;
   int Upper;
   int nField_TH;
   
   omp_set_num_threads(numTH);
   omp_set_nested(1);
   
   if ( myMPI.myPE == 0 )
     {
       cout << endl;
       cout << "Input Summary: " << endl;
       cout << "--------------------------- " << endl;
       cout << "No. PE   in  x-direction: " << nPEx   << endl;
       cout << "             y-direction: " << nPEy   << endl;
       cout << "No. Cells in x-direction: " << nCellx << endl;
       cout << "             y-direction: " << nCelly << endl;
       cout << "Linear solver           : " << solver << endl;
       cout << "Non-Linear solver       : " << nlsolver << endl;
       cout << "Threads                 : " << numTH << endl;
       cout << endl;
     }

   myMPI.GridDecomposition(nPEx,nPEy,nCellx,nCelly);

   // -
   // |
   // | Parallel Grid Generation and Laplace Solver
   // |
   // -
   
   double totalLength = 1.;
   double eachPElength_x = totalLength / nPEx;
   double eachPElength_y = totalLength / nPEy;

   double x0 = eachPElength_x * myMPI.iPE;   double x1 = x0 + eachPElength_x;
   double y0 = eachPElength_y * myMPI.jPE;   double y1 = y0 + eachPElength_y;
   
   LaplacianOnGrid MESH(x0,x1,y0,y1,nCellx,nCelly, myMPI );

   MESH.k0 = 1.;
   MESH.k1 = 1.;
   MESH.k2 = 0.;


   // ----------------------------------------------------------------------------
   // Linear Solve
   // ----------------------------------------------------------------------------
   
   if ( myMPI.myPE == 0 ) printf("LINEAR SOLVE for initial guess\n");
   timingInfo linTime("Linear Solver " + solver);
   linTime.Start(myMPI.myPE);
   //#pragma omp parallel
   //{
	 //int myTH = omp_get_thread_num();

       //numPerTh = nField / numTH;
       //Lower = (numPerTh * myTH) + 1;

       //if(myTH == numTH - 1) Upper = nField;
       //else Upper = Lower + numPerTh - 1;
   MESH.FormLS(myMPI,c0,tau, 1, nField, nlsolver);
   //#pragma omp barrier
   //}
       
   //MESH.cg_init();
   
   if      ( solver == "jacobi" ) MESH.Jacobi(MESH.Acoef,MESH.b,MESH.phi , myMPI );
   else if ( solver == "cg"     ) MESH.CG    (MESH.Acoef,MESH.b,MESH.phi , myMPI, 1, nField, nField );
   else                           FatalError("Solver " + solver + " not found.");
   
   MESH.plot( "phi"  , MESH.phi , 0 , myMPI      );
   linTime.Finish(myMPI.myPE);
   //return 0;
   // ----------------------------------------------------------------------------
   // Non-Linear Solve
   // ---------------------------------------------------------------------------- 
   if ( myMPI.myPE == 0 ) printf("NON-LINEAR SOLVE\n");

   double tol              = 1.e-08;
   int    iter             = 0;
   int    max_iter         = 1000;
   int    converged        = 0;
   int    global_converged = 0;

   
   
   // Initial guesses

   for ( int r = 1 ; r <= MESH.nField ; ++r )
     {
       MESH.phiNew[r] = MESH.phi[r];
       MESH.dPhi  [r] = 0.;
     }
   
   // Nonlinear iterations
   
  ANNOTATE_SITE_BEGIN(Nonlinear_iterations);
  timingInfo nlTime(nlsolver + " with " + solver);
   nlTime.Start(myMPI.myPE);

   
   while ( global_converged == 0 && ++iter < max_iter )
     {
       //       if ( myMPI.myPE == 0 ) { printf("\n\n");   printf("-- %s -- Iteration %d\n",nlsolver.c_str(),iter); }
       
       
       

       #pragma omp parallel
       {
       int myTH = omp_get_thread_num();
      
       numPerTh = nField / numTH;
       int Lower = (numPerTh * myTH) + 1;
       int Upper;
       
       if(myTH == numTH - 1) Upper = nField;
       else Upper = Lower + numPerTh - 1;
       
       int size = Upper - Lower;
       ANNOTATE_TASK_BEGIN(Form);
       MESH.FormLS(myMPI,c0,tau, Lower, Upper, nlsolver);
       ANNOTATE_TASK_END()
	 
       #pragma omp barrier
	 }
       if ( nlsolver == "nr" )  
	 {
	   
	   if      ( solver == "jacobi" ) {ANNOTATE_TASK_BEGIN(Jacobi); MESH.Jacobi(MESH.Jacobian, MESH.minusf, MESH.dPhi, myMPI  ); ANNOTATE_TASK_END();}
	   //else if ( solver == "cg"     ) {ANNOTATE_TASK_BEGIN(CG); MESH.CG    (MESH.Jacobian, MESH.minusf, MESH.dPhi, myMPI );ANNOTATE_TASK_END();}
	   else if ( solver == "cg"     ) {ANNOTATE_TASK_BEGIN(CG); MESH.CG    (MESH.Jacobian, MESH.minusf, MESH.dPhi, myMPI, 1, nField, nField);ANNOTATE_TASK_END();}
	   else                           FatalError("Solver " + solver + " not found.");

	   converged = MESH.NR_Phi_Update( tol , relax );
	 }
       
       else if ( nlsolver == "sa" ) 
	 {
	   //VDD aCo(MESH.Acoef.begin()+Lower, MESH.Acoef.begin()+Upper+1);
	   //VD bCo(MESH.b.begin()+Lower, MESH.b.begin()+Upper+1);
	   //VD ph(MESH.phiNew.begin()+Lower, MESH.phiNew.begin()+Upper+1);
	   
	   //#pragma omp barrier
	   
	   if      ( solver == "jacobi" ) {ANNOTATE_TASK_BEGIN(Jacobi); MESH.Jacobi(MESH.Acoef , MESH.b , MESH.phiNew , myMPI ); ANNOTATE_TASK_BEGIN(Jacobi);}
	   else if ( solver == "cg"     ) {ANNOTATE_TASK_BEGIN(CG); MESH.CG    (MESH.Acoef , MESH.b , MESH.phiNew , myMPI, 1, nField, nField ); ANNOTATE_TASK_END();}
	   // else if ( solver == "cg"     ) {ANNOTATE_TASK_BEGIN(CG); MESH.CG    (aCo, bCo, ph, myMPI, Lower, Upper, size ); ANNOTATE_TASK_END();}	   
	   else                           FatalError("Solver " + solver + " not found.");
	   // #pragma omp barrier
	   converged = MESH.SA_Phi_Update ( tol , relax );
	 }
       
       else
	 FatalError("Nonlinear Solver " + nlsolver + " not found.");
       
	   
             
       MPI_Barrier(MPI_COMM_WORLD);   MPI_Allreduce(&converged, &global_converged, 1 , MPI_INT, MPI_MIN, MPI_COMM_WORLD);
       //}
     
}
   ANNOTATE_SITE_END();

   if ( global_converged == 1 ) if ( myMPI.myPE == 0 ) cout << nlsolver << " converged in " << iter << " iterations.\n" ;
   if ( global_converged == 0 ) if ( myMPI.myPE == 0 ) cout << nlsolver << " failed to converge after " << iter << " iterations.\n" ;
    
   MESH.plot( "phi_"   + nlsolver , MESH.phi    , 0 , myMPI      );
   nlTime.Finish(myMPI.myPE);


   
   // -
   // |
   // | Wrap-Up
   // |
   // -
   
   myTime.Finish(myMPI.myPE);

   MPI_Finalize();
   return 0;

}
