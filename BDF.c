/*HEADER
 * Houda Haidar
 * 11/03/2021
 * EX2: ODE 2
 * comp: gcc -lgsl BDF.c -o BDF -lm
 * ex: ./EX2_BDF
 */


#include<stdio.h>
#include<stdlib.h>
#include<gsl/gsl_errno.h>
#include<gsl/gsl_odeiv2.h>
#include<math.h>
#include<err.h>



int my_func(double t, const double* y, double* f, void* params)
{
	f[0] = -0.04*y[0] + 1.E4*y[1]*y[2];
	f[1] = 0.04*y[0] - 1.E4*y[1]*y[2] - 3.*1.E7*pow(y[1],2);
	f[2] = 3.E7*pow(y[1],2);
	return GSL_SUCCESS;
}

int func_jac(double t, const double* y, double* dfdy, double* dfdt, void *params)
{
	/*derivatives RH wrt to t*/
	dfdt[0] = 0.;
	dfdt[1] = 0.;
	dfdt[2] = 0.;

	/*jacobian of RHS function wrt to y*/
	dfdy[0*3 + 0 ] = -0.04  ;
	dfdy[0*3 + 1 ] =  1.E4*y[2] ;
	dfdy[0*3 + 2 ] =  1.E4*y[1];
	dfdy[1*3 + 0 ] =  0.04 ;
	dfdy[1*3 + 1 ] =  -1.E4*y[2] -3*1.E7*2*y[1];
	dfdy[1*3 + 2 ] =  -1.E4*y[1] ;
	dfdy[2*3 + 0 ] = 0.   ;
	dfdy[2*3 + 1 ] =   3.E7*2*y[1];
	dfdy[2*3 + 2 ] =  0. ;

	return GSL_SUCCESS;
}



int main(void)
{

	/*variables*/
	int i,status, ndim = 3;
	double h = 0.0001; /*intial stepsize*/
	double t = 0., t_fin = 0 /*t_fin = 1.E-2  OR 1.E10*/; /*intial and final times*/
	double abs_tol = 1.E-4, rel_tol = 1.E-4;
	double* y;
	/*file stuff*/
        FILE* result_c = NULL;
        char  file[50] = "results_c.dat";



	/*instructions*/

	/*t_final*/
	printf("please enter the value of t_final\n");
	scanf("%lf",&t_fin);

	/*set intial state of the solution*/
	y = malloc(3 * sizeof(double)); if (!y) {err(-1, "Malloc failed.\n");}
	y[0] = 1.;
	y[1] = 0.;
	y[2] = 0.;

	/*define sys, stepper, controler and evolution*/
	gsl_odeiv2_system sys = {&my_func,&func_jac,ndim,NULL};

	gsl_odeiv2_step* step = gsl_odeiv2_step_alloc(gsl_odeiv2_step_msbdf,ndim);
	gsl_odeiv2_control* control = gsl_odeiv2_control_y_new(abs_tol, rel_tol);
	gsl_odeiv2_evolve* evol = gsl_odeiv2_evolve_alloc(ndim);

	/*driver*/
	gsl_odeiv2_driver * driver = gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_msbdf,h,abs_tol,rel_tol);
	gsl_odeiv2_step_set_driver(step,driver);

	/*open file to write*/
	result_c= fopen(file, "w");
        /*check*/
        if ( result_c == NULL)
        {
                printf("Error opening %s \n", file);
                return 1;
        }


		i = 0.;
		while(t<t_fin && i < 100000)
		{	
			status = gsl_odeiv2_evolve_apply(evol,control,step,&sys,&t,t_fin,&h,y);
			if (status != GSL_SUCCESS )
			{
				err(-1, "The GSL solver failed while taking a step\n");
			}
			fprintf (result_c,"%.15E, %.15E,%.15E, %.15E\n", t,y[0],y[1], y[2]);
		}
		printf("final value of the sol: %.15E,%.15E, %.15E\n",y[0],y[1], y[2]);
	


	/*free memo*/
	free(y);
	y = NULL;
	gsl_odeiv2_step_free(step);
	gsl_odeiv2_control_free(control);
	gsl_odeiv2_evolve_free(evol);
	gsl_odeiv2_driver_free(driver);





	return EXIT_SUCCESS;

}
