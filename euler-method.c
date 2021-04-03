/*HEADER:
 * HOuda Haidar
 * 7/03/2021
 * ODE session: Euler's method
 * compile: gcc -lgsl euler-method.c -o euler-method -lm
 * ex: ./euler-method
 * /

#include<stdio.h>
#include<math.h>


double ODE(double a0, double y0);

int main(void)
{

	/*VARIABLES*/
	FILE* ode_results = NULL;
	char  file[50] = "ode_results.dat";

	double a0=0, y0=10, h=0, yn=0, t= 0, tf = 5.;
	/*int i=0;*/


	/*INSTRUCTIONS*/
	printf("Enter Initial Condition of a\n");
 	scanf("%lf", &a0);
 	printf("Enter the timestep h: ");
 	scanf("%lf", &h);


	/*opening file to write */
        /* opening new file to write in*/
        ode_results = fopen(file, "w");
        /*check*/

        if ( ode_results == NULL)
        {
                printf("Error opening %s \n", file);
                return 1;
        }

	/***********************************************
	 ******** EULER S METHOD FOR DIFF EQs**********
	 * ********************************************/
	while (t < tf){
        yn=y0+h*ODE(a0,y0);
        printf("%lf\t%lf\n",h*ODE(a0,y0),yn);
        y0=yn;
	t = t + h;
        fprintf(ode_results, "%lf  %.4lf\n", t, yn) ;
        }

        printf("%lf\t%lf\n",a0,y0);
        printf("The value of y is %lf\n\n",yn);




	return 0;
}

double ODE(double a0, double y0)
{
	return - y0*(0.2 + (12 / ( a0 + pow(y0 -3, 2))));
}
