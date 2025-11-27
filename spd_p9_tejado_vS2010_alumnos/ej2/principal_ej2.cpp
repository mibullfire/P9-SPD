//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  SPD subject 3º GTI
//  - Understanding basic concepts for OpenMP
//-----------------------------------------------------

using namespace std;

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <omp.h>

void part_a_b_c (int n);

int main()
{
	//-----------------------------------
	part_a_b_c(24);
	//-----------------------------------
}

/*a
void part_a_b_c (int n)
{
	#pragma omp parallel
	{
		int this_thread = omp_get_thread_num();
		for (int i=0;i<n;i++)
		{
			#pragma omp critical
			{
			cout << "Iteration " << i << " is handled by thread " << this_thread << endl;
			}
		}
	}
}
*/

/*
void part_a_b_c(int n)
{
#pragma omp parallel
	{
		int tid = omp_get_thread_num();
		int nt = omp_get_num_threads();
		for (int i = tid*n/(nt); i < (tid+1) * n / nt; i++)
		{
			
			cout << "Iteration " << i << " is handled by thread " << tid << "\n" << endl;
			
		}
	}
}
*/



void part_a_b_c(int n)
{
	// El parallel for ya hace un fraccionamiento automatico de los parámetros del for
	// Por lo que se puede dejar exactamente igual que en el apartado a jajajajajajaja
#pragma omp parallel for

	for (int i = 0; i < n; i++)
	{
		
		cout << "Iteration " << i << " is handled by thread " << omp_get_thread_num() << "\n" << endl;
		
	}
	
}

