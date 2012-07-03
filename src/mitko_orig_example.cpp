/* the following command line arguments should be provided
  
   argv[1] -  NumberOfImagePoints (int)
   argv[2] -  InitialGuess        (int)
   argv[3] -  OPTION              (int)
   argv[4] -  KKT_tol             (double)

   Example (assuming 30 points are available in "measurements.txt"):
   -----------------------------------------------------------------
   ./exercise2 30 0 1 1e-06
*/

#include <acado_toolkit.hpp>

// void OnePoint_ODE(double w[3], double v[3], double x[3], double dx[3])
// {
//   dx[0] =     0*x[0] - w[2]*x[1] + w[1]*x[2] + v[0];
//   dx[1] =  w[2]*x[0] +    0*x[1] - w[0]*x[2] + v[1];
//   dx[2] = -w[1]*x[0] + w[0]*x[1] +    0*x[2] + v[2];
// }

// void C_ODE( double *x_, double *f, void *user_data )
// {

//   double w[3];
//   double v[3]; 
//   double x[3]; 
//   double dx[3];
  
//   // angular velocity
//   w[0] = x_[0];
//   w[1] = x_[1];
//   w[2] = x_[2];

//   // linear velocity
//   v[0] = x_[3];
//   v[1] = x_[4];
//   v[2] = x_[5];

//   const int NumberOfImagePoints = ((double*) user_data)[0];

//   for (int i=0; i<NumberOfImagePoints; i++)
//     {
//       x[0] = x_[6+3*i+0];
//       x[1] = x_[6+3*i+1];
//       x[2] = x_[6+3*i+2];

//       OnePoint_ODE(w, v, x, dx);

//       f[3*i+0] = dx[0];
//       f[3*i+1] = dx[1];
//       f[3*i+2] = dx[2];
//     }
// }

int main(int argc, char **argv)
{

  USING_NAMESPACE_ACADO




  // const int NumberOfImagePoints = atoi(argv[1]);
  // const int InitialGuess = atoi(argv[2]);
  // const int OPTION = atoi(argv[3]);

  // const double KKT_tol = atof(argv[4]);

  // const int PointDimension = 3;
  
  // double t0 = 0.0;
  // double t1 = 1.0;
    
  // DifferentialState     s(PointDimension*NumberOfImagePoints);
  // Parameter             xi(6);

  // IntermediateState     x_(6+PointDimension*NumberOfImagePoints);

  // // parameters
  // for (int i=0; i<6; i++)
  //   x_(i) = xi(i);

  // // states
  // for (int i=0; i<PointDimension*NumberOfImagePoints; i++)
  //   x_(6+i) = s(i); 

  // // ==========================================  
  // CFunction ODE(PointDimension*NumberOfImagePoints, C_ODE );
  // DifferentialEquation 	f;

  // // you can use "user_data" to pass data that you don't want to declare as "IntermediateState"
  // double user_data[1];
  // user_data[0] = NumberOfImagePoints;
  
  // ODE.setUserData( &user_data );

  // f << ODE(x_); // define the ODE
  // // ==========================================

  // VariablesGrid measurements( "measurements.txt" );
  // VariablesGrid xi_GT( "xi.txt" );
  // //measurements.print();

  // OCP ocp(t0,t1,1); // use only one discrete interval!
  // ocp.subjectTo( f );

  // for (int i=0; i<PointDimension*NumberOfImagePoints; i++)
  //   ocp.subjectTo( AT_START, s(i) == measurements(0,i) );
  
  // if (OPTION == 0) // OPTION 0
  //   {  
  //     IntermediateState e = 0;
  //     for (int i=0; i<PointDimension*NumberOfImagePoints; i++)
  // 	e += pow(s(i) - measurements(1,i),2);  
      
  //     ocp.minimizeMayerTerm(e);
  //   }
  // else if (OPTION == 1) // OPTION 1
  //   {
  //     Function h;
  //     h << s;
      
  //     Vector r(PointDimension*NumberOfImagePoints);
  //     for (int i=0; i<PointDimension*NumberOfImagePoints; i++)
  // 	r(i) = measurements(1,i);  
      
  //     ocp.minimizeLSQEndTerm(h,r);
  //   }
  // // ==========================================

  // OptimizationAlgorithm algorithm(ocp);

  // algorithm.set( INTEGRATOR_TYPE, INT_RK45 ); 
  // algorithm.set( KKT_TOLERANCE, KKT_tol);
  // algorithm.set( ABSOLUTE_TOLERANCE, 1e-4);
  
  // algorithm.set( PRINTLEVEL, HIGH );
  // algorithm.set( MAX_NUM_ITERATIONS, 200 );
  // algorithm.set (PRINT_COPYRIGHT, NO);

  // if (OPTION == 1)
  //   {
  //     algorithm.set( HESSIAN_APPROXIMATION, EXACT_HESSIAN ); 
      
  //     algorithm.set(OBJECTIVE_SENSITIVITY , FORWARD_SENSITIVITY ); 
  //     algorithm.set(CONSTRAINT_SENSITIVITY, FORWARD_SENSITIVITY ); 
  //     algorithm.set(DYNAMIC_SENSITIVITY   , FORWARD_SENSITIVITY ); 
  //   } 

  // if (InitialGuess)
  //   algorithm.initializeParameters("InitialGuess.txt");

  // int status;
  // double clock1 = clock();
  // status = algorithm.solve();
  // double clock2 = clock();

  // VariablesGrid parameters;
  // algorithm.getParameters(parameters);
  // parameters.print();

  // // -----------------------------------------------
  // printf("\n computation time (ACADO) = %.16e \n", (clock2-clock1)/CLOCKS_PER_SEC);

  // cout << endl << " ERROR (xi-xi_GT): " ;
  // for (int i=0; i<6; i++)
  //   cout << parameters(0,i) - xi_GT(0,i) << " ";
  // cout << endl;
  // // -----------------------------------------------

  return 0;
}
//EOF

