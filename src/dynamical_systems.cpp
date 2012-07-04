#include <acado_toolkit.hpp>
#include <include/acado_gnuplot/gnuplot_window.hpp>

// if FLAG_0 is defined initial conditions for differential states are imposed as constraints
// if not provide a guess for the initial conditions for differential states
#define FLAG_0 

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
  Parameter         a(3), b(3);
  TIME              t;
 
  Grid time_grid = demonstration.getTimePoints();

  // define other variables
  double g = demonstration.getLastVector()(0);

#ifdef FLAG_0
  double  q0 = demonstration.getFirstVector()(0);
  double dq0 = demonstration.getFirstVector()(1);
#endif

  // ---------------------------------------------------
  // optimize
  // ---------------------------------------------------

  // define ODE
  DifferentialEquation f_opt;
  f_opt << dot(q)  == dq;
  f_opt << dot(dq) == (a(0)*pow(t,2) + a(1)*t + a(2))*(g-q) - (b(0)*pow(t,2) + b(1)*t + b(2))*dq;
 
  Function h;
  h << q;
  h << dq;

  Matrix Q(2,2);
  Q.setIdentity();

  // define OCP
  OCP ocp(time_grid);
  ocp.minimizeLSQ(Q,h,demonstration);
  
  // constraints
  ocp.subjectTo(f_opt);

#ifdef FLAG_0
  cout << endl << " AT_START:   q = q0, dq = dq0" << endl << endl;
  ocp.subjectTo( AT_START, q  ==  q0 );  
  ocp.subjectTo( AT_START, dq == dq0 );  
#endif

  // define optimization algorithm
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

  double a_sol[3] = {p_sol(0),p_sol(1),p_sol(2)};
  double b_sol[3] = {p_sol(3),p_sol(4),p_sol(5)};

  DifferentialEquation f_sim;
  f_sim << dot(q)  == dq;
  f_sim << dot(dq) == (a_sol[0]*pow(t,2) + a_sol[1]*t + a_sol[2])*(g-q) - (b_sol[0]*pow(t,2) + b_sol[1]*t + b_sol[2])*dq;
 
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

  return 0;
}
//EOF

