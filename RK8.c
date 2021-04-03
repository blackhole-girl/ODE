/*HEADER
 * 11/03/2021
 * EX2: ODE 2
 * comp: gcc -lgsl RK8.c -o RK8 -lm
 * ex: ./RK8
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


int main(void)
{

	/*variables*/
	int i,status, ndim = 3, iter = 1000;
	double h = 0.01; /*intial stepsize*/
	double t = 0., t_fin = 0 /*t_fin = 1.E-2  OR 1.E10*/; /*intial and final times*/
	double abs_tol = 1.E-8, rel_tol = 1.E-8;
	double* y;
	/*file stuff*/
        FILE* result = NULL;
        char  file[50] = "results.dat";



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
	gsl_odeiv2_system sys = {&my_func,NULL,ndim,NULL};

	gsl_odeiv2_step* step = gsl_odeiv2_step_alloc(gsl_odeiv2_step_rk8pd,ndim);
	gsl_odeiv2_control* control = gsl_odeiv2_control_y_new(abs_tol, rel_tol);
	gsl_odeiv2_evolve* evol = gsl_odeiv2_evolve_alloc(ndim);

	/*open file to write*/
	result= fopen(file, "w");
        /*check*/
        if ( result == NULL)
        {
                printf("Error opening %s \n", file);
                return 1;
        }


	i = 0;
	while(t<t_fin && i < iter)
	{	
		status = gsl_odeiv2_evolve_apply(evol,control,step,&sys,&t,t_fin,&h,y);
		i ++;
		if (status != GSL_SUCCESS )
		{
			err(-1, "The GSL solver failed while taking a step\n");
		}

		fprintf (result,"%.15E, %.15E,%.15E, %.15E\n", t,y[0],y[1], y[2]);
	}
	printf("final value of the sol: %.15E,%.15E, %.15E\n",y[0],y[1], y[2]);
	

	/*close file*/


	/*free memo*/
	free(y);
	y = NULL;
	gsl_odeiv2_step_free(step);
	gsl_odeiv2_control_free(control);
	gsl_odeiv2_evolve_free(evol);





	return EXIT_SUCCESS;

}
