#include <acado_toolkit.hpp>
#include <include/acado_gnuplot/gnuplot_window.hpp>

int main(int argc, char **argv)
{

  USING_NAMESPACE_ACADO

  //LOAD DEMONSTRATION
  //==================
  
  VariablesGrid measurements("demonstrations/FFJ3_tripod_small_closed.txt"); //load a demonstration with columns for q, qd & qdd

  VariablesGrid demonstration = measurements(0);
  demonstration.appendValues(measurements(1)); //use q and qd

  //DEFINE VARIABLES
  //================

   DifferentialState z;
   DifferentialState y;
   
   Parameter         a_z;
   Parameter         b_z;
   Parameter         Tau;
   Parameter         g;

  //DEFINE DIFFERENTIAL EQUATION
  //============================

   DifferentialEquation f;
   f << dot(z) == a_z*(b_z*(g-y)-z);  
   f << dot(y) == z; 

//division by Tau blows optimization up
//  f << dot(z) == a_z/Tau*(b_z*(g-y)-z);
//  f << dot(y) == z/Tau 

  //DEFINE MEASUREMENT FUNCTION
  //===========================

   Function h;
   h << y;
   h << z;

  //DEFINE PARAMETER ESTIMATION PROBLEM
  //===================================

   OCP ocp(demonstration.getTimePoints());
   ocp.minimizeLSQ(h,demonstration);

   ocp.subjectTo(f);
   ocp.subjectTo( Tau == demonstration.getTimePoints().getLastTime());
   ocp.subjectTo( g == demonstration.getLastVector()(0));
   
   
  // //SETUP A PLOT WINDOW
  // //===================

 

   GnuplotWindow window(PLOT_NEVER);
   window.addSubplot(y, "y","time [s]", "angle [deg]");
   window.addSubplot(z, "z", "time [s]", "velocity");
   window.addSubplot(demonstration(0), "y demo");
    window.addSubplot(demonstration(1), "z demo");

  //DEFINE AN OPTIMIZATION ALGORITHM AND SOLVE
  //==========================================

   ParameterEstimationAlgorithm algorithm(ocp);
   algorithm << window;
   algorithm.initializeDifferentialStates(demonstration);
   double clock1 = clock();
   algorithm.solve();
   double clock2 = clock();
   printf("\n computation time (ACADO) = %.16e \n", (clock2-clock1)/CLOCKS_PER_SEC);

  //GET THE OPTIMAL PARAMETERS
  //==========================

   VariablesGrid parameters;
   algorithm.getParameters(parameters);

  //PLOT THE RESULTS
  //================

   std::cout<<parameters(0,0)<<" "<<parameters(0,1)<<" "<<parameters(0,2)<<" "<<parameters(0,3)<<std::endl;

   algorithm.getPlotWindow(window);
   window.plot();


  return 0;
}
//EOF

