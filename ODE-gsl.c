/*HEADER:
 * Houda Haidar
 * 06/03/21
 * EX: solve ODE using GSL library
 * comp: gcc -lgsl ODE-gsl.c -o ODE-gsl-lm
 * ex: ./ODE-gsl
 */

#include<stdio.h>
#include<stdlib.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv2.h>
#include<math.h>



int my_func(double t, const double* y, double* f, void* params);


int main(void)
{
	/*variables*/
	/*file stuff*/
	FILE* ode_gsl = NULL;
        char  file[50] = "ode_gsl.dat";
	/*code stuff*/
	int ndim = 1; /*dimension of the ODE*/
	double abs_tol = 1.e-8;	/* absolute tolerance*/
    	double rel_tol = 1.e-10;	/* relative tolerance */
	double t = 0, t_fin= 5 , h = 0;
	double y= 10;
	int status = 0;
	/*instructions*/
        printf("Enter the timestep h:\n");
        scanf("%lf", &h);



	/* STEP1: sys definition*/
	gsl_odeiv2_system sys = {&my_func, NULL, ndim, NULL};
	
	
	/*STEP2: creating solver in 3 parts*/
	/*part 1: stepper */
	gsl_odeiv2_step* step = gsl_odeiv2_step_alloc(gsl_odeiv2_step_rk8pd,ndim);
	/*part 2: controler*/
	gsl_odeiv2_control* control = gsl_odeiv2_control_y_new(abs_tol, rel_tol);
	/*part 3: evolution function*/
	
	gsl_odeiv2_evolve* evol = gsl_odeiv2_evolve_alloc(ndim);



	 /*open file here to write*/
        ode_gsl= fopen(file, "w");
        /*check*/
        if ( ode_gsl == NULL)
        {
                printf("Error opening %s \n", file);
                return 1;
        }



	/* STEP3:evolve the solution one step ( step size adjusted automatically)*/	
	while ( t < t_fin)  /*evolution from t to t_fin*/
	{
		
		status = gsl_odeiv2_evolve_apply(evol,control,step,&sys,&t,t_fin,&h,&y);
		/*if (status != GSL_SUCCESS)
			printf ("error, return value=%d\n", status);
               		break;*/
 
           	
		printf ("%.5e %.5e \n", t, y);
		fprintf (ode_gsl,"%.5e %.5e \n", t, y); 

	}
	

	/*free*/
	gsl_odeiv2_evolve_free (evol);
	gsl_odeiv2_control_free (control);
	gsl_odeiv2_step_free (step);
	
	return 0;

}

int my_func(double t, const double* y, double* f, void* params)
{
        /* we compute the RHS of the ODE sys at current time t and current solution value y 
         * store it into array f*/
	double a = 0.05;
        f[0] = - y[0] * ( 0.2 + ( 12 / (a + pow(y[0] - 3, 2))));
        return GSL_SUCCESS;
}

