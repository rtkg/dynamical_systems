#include <acado_toolkit.hpp>
#include <include/acado_gnuplot/gnuplot_window.hpp>

// if FLAG_0 is defined initial conditions for differential states are imposed as constraints
// if not provide a guess for the initial conditions for differential states
#define FLAG_0 

// NOTE 1: maybe we should define tau as a time varying parameter

// NOTE 2: I think that having a G.A.S. system is NOT useful in the context of this application (to discuss)

// QUESTION 1: What are the results with your parameter estimation algorithm (and what is it)? 

// QUESTION 2: Mixture model of dynamical systems?

// QUESTION 3: How about coupling (later on)?

int main(int argc, char **argv)
{

  USING_NAMESPACE_ACADO;

  double KKT_tol = 1e-3;
  
  // measurements contain: q, dq, ddq
  VariablesGrid measurements("../demonstrations/data.txt"); 

  // demonstration contain: q, dq
  VariablesGrid demonstration = measurements(0);  
  demonstration.appendValues(measurements(1)); 
  
  // define ACADO variables
  DifferentialState q, dq;
  Parameter         a, b, tau;
 
  Grid time_grid = demonstration.getTimePoints();

  // define other variables
  double T_start = time_grid.getFirstTime();  
  double T_end   = time_grid.getLastTime();  
  double g       = demonstration.getLastVector()(0);

#ifdef FLAG_0
  double  q0 = demonstration.getFirstVector()(0);
  double dq0 = demonstration.getFirstVector()(1);
#endif

  // ---------------------------------------------------
  // optimize
  // ---------------------------------------------------

  // define ODE
  DifferentialEquation f_opt;
  f_opt << dot(q)  == dq/tau;
  f_opt << dot(dq) == (a*(g-q)-b*dq)/tau;
  // using (a*(b*(g-q)-dq))/tau seems to be a bad idea (of course a & b have different meaning) - lets discuss tomorrow

  Function h;
  h << q;
  h << dq;

  Matrix Q(2,2);
  Q.setIdentity();
  //Q(0,0) = 5;
  
  /*
  MatrixVariablesGrid Q(2,2,time_grid);
  Matrix I(2,2); I.setIdentity();
  Q.setAllMatrices(I);
  
  Matrix Qi(2,2); Qi.setIdentity(); Qi(0,0) = 100; Qi(1,1) = 100;
  
  // the indexes depend on the discretization
  Q.setMatrix(9, Qi);
  Q.setMatrix(15, Qi);
  Q.setMatrix(25, Qi);
  */

  // define OCP
  OCP ocp(time_grid);
  //ocp.minimizeLSQ(h,demonstration);
  ocp.minimizeLSQ(Q,h,demonstration);
  
  // constraints
  ocp.subjectTo(f_opt);
  ocp.subjectTo( 1 <= tau <= T_end ); // <-- we should definitely impose constraints on tau (what bounds are reasonable?)
  //ocp.subjectTo(tau == 1);

#ifdef FLAG_0
  cout << endl << " AT_START:   q = q0, dq = dq0" << endl << endl;
  ocp.subjectTo( AT_START, q  ==  q0 );  
  ocp.subjectTo( AT_START, dq == dq0 );  
#endif

  // define optimization algorithm
  //ParameterEstimationAlgorithm algorithm(ocp); // quite a bit slower (but I don't know why)
  OptimizationAlgorithm algorithm(ocp);

  // define some useful options
  algorithm.set( INTEGRATOR_TYPE, INT_RK78 );
  algorithm.set( KKT_TOLERANCE, KKT_tol);  
  algorithm.set( PRINTLEVEL, HIGH );

  algorithm.set( MAX_NUM_ITERATIONS, 200 );
  algorithm.set (PRINT_COPYRIGHT, NO);
  
  algorithm.set( GLOBALIZATION_STRATEGY, GS_FULLSTEP ); // <-- important
  algorithm.set( LEVENBERG_MARQUARDT, 1e-6 );

#ifndef FLAG_0
  algorithm.initializeDifferentialStates(demonstration);
#endif

  // solve the problem
  algorithm.solve();

  // output solution
  VariablesGrid parameters, states_opt;
  algorithm.getParameters(parameters);
  algorithm.getDifferentialStates(states_opt);
  states_opt.printToFile( "../demonstrations/states_opt.txt","",PS_PLAIN );

  Vector p_sol = parameters.getLastVector();
  p_sol.print("parameters = \n");

  // ---------------------------------------------------
  // simulate
  // ---------------------------------------------------

  double a_sol = p_sol(0);
  double b_sol = p_sol(1);
  double tau_sol = p_sol(2);

  DifferentialEquation f_sim;
  f_sim << dot(q)  == dq/tau_sol;
  f_sim << dot(dq) == (a_sol*(g-q)-b_sol*dq)/tau_sol;
  
  IntegratorRK78 integrator( f_sim );  
  integrator.set( INTEGRATOR_PRINTLEVEL, NONE );
  integrator.set( INTEGRATOR_TOLERANCE, 1.0e-6 );

  // initial condition
  Vector x0(2);
  x0(0) = states_opt(0,0);
  x0(1) = states_opt(0,1);

  VariablesGrid states_sim;
  
  integrator.integrate( time_grid, x0 );
  integrator.getX( states_sim );
  
  states_sim.printToFile( "../demonstrations/states_sim.txt","",PS_PLAIN );

  // ----------------------------------------------------
  // simulate suing a ACADO::Process
  // here we can have control input
  // ----------------------------------------------------

  OutputFcn out;
  out << q;
  out << dq;
  
  DynamicSystem DynSys( f_opt, out );
  Process process;
  process.setDynamicSystem( DynSys, INT_RK78 );
  process.set( ABSOLUTE_TOLERANCE,1.0e-8 );
  process.initializeStartValues( x0 );
  process.set( PLOT_RESOLUTION,HIGH );

  process.run(T_start, T_end, emptyConstVector, p_sol);	 

  VariablesGrid y;
  process.getY( y );

  y.printToFile( "../demonstrations/states_sim1.txt","",PS_PLAIN );
  // ----------------------------------------------------
  

  return 0;
}
//EOF

