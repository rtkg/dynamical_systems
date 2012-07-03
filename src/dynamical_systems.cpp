#include <acado_toolkit.hpp>
#include <include/acado_gnuplot/gnuplot_window.hpp>

int main(int argc, char **argv)
{

  USING_NAMESPACE_ACADO

    //LOAD DEMONSTRATION
    //==================
  
   VariablesGrid demonstration("demonstrations/FFJ3_tripod_small_closed.txt"); //load a demonstration with columns for q, qd & qdd
   demonstration = demonstration(0); //take only the position values q

   demonstration.print();

   std::cout<<demonstration.getLastVector()(0)<<std::endl;

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
   f << dot(z) == a_z/Tau*(b_z*(g-y)-z);
   f << dot(y) == z/Tau;

  //DEFINE MEASUREMENT FUNCTION
  //===========================

   Function h;
   h << y;

  //DEFINE PARAMETER ESTIMATION PROBLEM
  //===================================

   OCP ocp(demonstration.getTimePoints());
   ocp.minimizeLSQ(h,demonstration);

   ocp.subjectTo(f);
   ocp.subjectTo( g == demonstration.getLastVector()(0));
   ocp.subjectTo( Tau == demonstration.getTimePoints().getLastTime());
   
  // //SETUP A PLOT WINDOW
  // //===================

 

   GnuplotWindow window(PLOT_NEVER);
   window.addSubplot(y, "y","time [s]", "angle [deg]");
   window.addSubplot(z, "z", "time [s]", "velocity");
   window.addSubplot(demonstration);
 
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

