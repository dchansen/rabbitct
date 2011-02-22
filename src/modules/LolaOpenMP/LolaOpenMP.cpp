/** RabbitCT - Version 1.0

	RabbitCT enables easy benchmarking of backprojection algorithms.
*/


// include the required header files
#include <iostream>
#include <omp.h>
#include "rabbitct.h"


RabbitCtGlobalData * s_rcgd = NULL;


// LolaOpenMP includes


/** \brief Initialization routine.

	This method is required for initializing the data required for
	backprojection. It is called right before the first iteration.
	Here any time intensive preliminary computations and 
	initializations should be performed.
*/
FNCSIGN bool RCTLoadAlgorithm(RabbitCtGlobalData * rcgd)
{
	return true;
}


/** \brief Finish routine.

	This method is called after the last projection image. Here
	it should be made sure the the rcgd->out_volume pointer
	is set correctly.
*/
FNCSIGN bool RCTFinishAlgorithm(RabbitCtGlobalData * rcgd)
{
	// Lola has nothing to do
	return true;
}



/** \brief Cleanup routine.

	This method can be used to clean up the allocated 
	data required for backprojection. It is called just before
	the benchmark finishes.
*/
FNCSIGN bool RCTUnloadAlgorithm(RabbitCtGlobalData * rcgd)
{
	return true;
}


inline double p_n(int i, int j)
{
	if (i>=0 && i<(int)s_rcgd->S_x && j>=0 && j<(int)s_rcgd->S_y)
		return s_rcgd->I_n[j*s_rcgd->S_x + i];

	return 0.0;
}

inline double p_hat_n(double x, double y)
{
	int i = (int)x;
	int j = (int)y;
	double alpha = x-(int)x;
	double beta  = y-(int)y;

	return   (1.0-alpha)*(1.0-beta)*p_n(i,j)
		   + alpha*(1.0-beta)*p_n(i+1,j)
		   + (1.0-alpha)*beta*p_n(i,j+1)
		   + alpha*beta*p_n(i+1,j+1);
}


/** \brief Backprojection iteration.
	
	This function should perform the backprojection of the
	current projection image. The time used to execute
	this method is measured for comparison. Thus it should
	be as fast as possible :-).
*/
FNCSIGN bool RCTAlgorithmBackprojection(RabbitCtGlobalData * r)
{
	unsigned int L = r->L;
	float O_L = r->O_L;
	float R_L = r->R_L;
	double * A_n = r->A_n;
	float * I_n = r->I_n;
	float * f_L = r->f_L;

	s_rcgd = r;

#pragma omp parallel for
	for (int k=0; k<(int)L; ++k) // OpenMP requires k to be signed; cast L to avoid warning
	{
//if (omp_get_thread_num() == 0)
//printf("There are %d threads\n", omp_get_num_threads());
		double z = O_L + (double)k*R_L;
		for (unsigned int j=0; j<L; j++)
		{
			double y = O_L + (double)j*R_L;
			for (unsigned int i=0; i<L; i++)
			{
				double x = O_L + (double)i*R_L;

				double w_n = A_n[2]*x + A_n[5]*y + A_n[8]*z + A_n[11];
				double u_n = (A_n[0]*x + A_n[3]*y + A_n[6]*z + A_n[9])  / w_n;
				double v_n = (A_n[1]*x + A_n[4]*y + A_n[7]*z + A_n[10]) / w_n;

				f_L[k*L*L + j*L + i] += (float)(1.0/(w_n*w_n) * p_hat_n(u_n,v_n)); 
			}
		}
	}

	return true;
}
