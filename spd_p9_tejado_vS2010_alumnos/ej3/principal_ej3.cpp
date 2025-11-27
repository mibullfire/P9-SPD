//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  SPD subject 3º GTI
//  - Understanding Roofline model with OpenMP and SSE2 (or AVX)
//-----------------------------------------------------

using namespace std;

#include <iostream>
#include <iomanip>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "querypmrdtsc.h"

#define CPU_FREQ_GHZ (2.66)

#define NUMREP 25
#define MINSIZE 64 //small size so it enters in L1 cache 
//#define MAXSIZE (128)
#define MAXSIZE (8*1024*1024)  // big size to be sure L3 cache is exceeded

typedef float TypeVector;

__declspec(align(32)) TypeVector w1[MAXSIZE], w2[MAXSIZE], w3[MAXSIZE], w4[MAXSIZE], w5[MAXSIZE], w6[MAXSIZE];

__declspec(align(32)) int  z[MAXSIZE];

void init_vectors(int);

void low_DA(int n);
void high_DA_omp_asm_only_float (int n);
void high_DA(int n);

TypeVector summation(int n);
int countzeros (int n);
int countzeros_condit_processing(int n);


////////////////////////////////////////////////////////////////////////////////////
int main()
{
	int repe, n;
	int flop_iter_C = 0;
	QPTimer c1;
	TypeVector res = 0;
	unsigned nz = 0, nz_condit = 0;
	string function_name;
		
	c1.Calibrate();
	for (n = MINSIZE; n<=MAXSIZE; n=n*2)
	{
		//		double initial_time, elapsed_time, minimum_time = 1e+307;
		for (repe = 0; repe<NUMREP; repe++)
		{           
			init_vectors(n);
			//	initial_time = omp_get_wtime(); //another way to measure times (see comment below)

			c1.Start();
			// Uncomment the call to the function under test.
			//------------------------------------------------

			low_DA(n); flop_iter_C = 2; function_name="LOW_DA";

			//high_DA(n);  flop_iter_C = 4; function_name = "HIGH_DA";

			// high_DA_omp_asm_only_float (n); flop_iter_C = 0;  function_name="HIGH_DA_omp_asm_only_float "; 
			 // flop_iter_C must be the same as in high_DA(n); 
			
			//res = summation(n); flop_iter_C = 2;  function_name="SUMMATION"; 
			
			//nz = countzeros(n); flop_iter_C = 0; function_name="COUNT_ZEROS"; 
			
			 // nz_condit = countzeros_condit_processing(n); flop_iter_C = 0; function_name="COUNTzeros_condit_processing"; 
			 // flop_iter_C must be the same as in countzeros(n);  

			//------------------------------------------------
			c1.Stop();
			c1.Reset();

		//	elapsed_time = omp_get_wtime() - initial_time;
		//  if ((elapsed_time ) < minimum_time)
			   // minimum_time = elapsed_time ;
			// omp_get_wtime returns a value in seconds of the time elapsed from some point.
			// the problem is that the frequency is usually variable in current CPUs
			 }        
		cout << "Size: " << setw(9) << n
			<< ". Minimum cycles per iter C: " << setiosflags(ios::fixed) << setw(8) << setprecision(4) << c1.GetMinimumCyclesByIteration(n)
			<< ". Maximum GFLOPs: " << setiosflags(ios::fixed) << setw(8) << setprecision(4) << CPU_FREQ_GHZ / c1.GetMinimumCyclesByIteration(n) * flop_iter_C
			<< endl;
		c1.ResetAll();
	}
	cout << endl << " *** " << function_name.data();
	cout << endl << "Ignore this... " << nz << " " << nz_condit << " " << res << endl;
}
/////////////////////////////////////////////////////////////////////////////
void init_vectors (int n)
{
	int i;
	srand(1);
	for (i=0;i<n;i++)
	{
		TypeVector x = (TypeVector)i / n;
		w1[i] = 1 - x;
		w2[i] = 2 - x;
		w3[i] = 3 - x;
		w4[i] = 4 - x;
		w5[i] = 5 - x;
		w6[i] = 1 - x;
		z[i] =  ((rand()>(RAND_MAX / 2)) ? 1 : 0);
	}
}

// Low D_A
void low_DA(int n)
{
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
		w1[i] = w2[i] * w3[i];
		w4[i] = w5[i] + w6[i];
	}
}

TypeVector b0 = 2, b1 = 3;
// high  D_A
void high_DA(int n)
{
	for (int i = 0; i < n; i++) {
		w2[i] = b0 * w2[i] * w2[i] * w2[i] + b1; 
	}
}

// High D_A asm 
void high_DA_omp_asm_only_float (int n)
{
	void high_DA_omp_asm_f_asm_float_AVX(int, int, int, int);
	void high_DA_omp_asm_f_asm_float_SSE2(int, int, int, int);

	{
		int n_threads = omp_get_num_threads();
		int id_thread = omp_get_thread_num();
		int first_iter = id_thread * n / n_threads, last_iter = (id_thread + 1) * n / n_threads;
		high_DA_omp_asm_f_asm_float_SSE2(first_iter * sizeof(TypeVector), last_iter * sizeof(TypeVector), last_iter - first_iter, 16 / sizeof(TypeVector));

		// for those machines with AVX multimedia extensions:
		//high_DA_omp_asm_f_asm_float_AVX(first_iter * sizeof(TypeVector), last_iter * sizeof(TypeVector), last_iter - first_iter, 32 / sizeof(TypeVector));
	}
}

// Computes and return the summation
TypeVector summation (int n)
{
	TypeVector sum=0, aux;
	for (int i = 0; i < n; i++) {
		aux = w6[i] * w5[i];
		sum += aux;
	}
	return sum; // just a filler
}

// Counts and returns the number of zero-elements in Z
int countzeros (int n)
{
	int count1=0;

	for (int i = 0; i < n; i++) {
		if (z[i] <= 0)
			count1++;
		}
	return count1; // just a filler
}

// Counts and returns the number of zero-elements in Z
int countzeros_condit_processing(int n)
{
	int  count2 = 0;
	for (int i = 0; i < n; i++) {
		count2 += (z[i] <= 0);
	}
	return count2; // just a filler
}
///////////////////////////////

// SSE2 versions

void high_DA_omp_asm_f_asm_float_SSE2 (int first_byte, int last_byte, int n_iter, int n_elem_iter) {
	__asm
	{
			push        ebx
			push        esi
			push        edi
			push		ecx

			mov ecx, n_elem_iter
			lea esi, b0
			movss  xmm0, [esi]
			shufps  xmm0, xmm0, 0
			
			lea esi, b1
			movss  xmm1, [esi]
			shufps  xmm1, xmm1, 0

			lea esi, w2
			add esi, first_byte
			xor ebx, ebx
			mov edi, n_iter
			/*	for (int i = 0; i < n; i++) {
			w2[i] = b0 * w2[i] * w2[i] * w2[i] + b1;
			}
			*/
		loop_asm :
			movaps    xmm2, xmmword ptr[esi]
			movaps    xmm6, xmm2
			mulps      xmm2, xmm2
			mulps      xmm2, xmm6
			mulps      xmm2, xmm0
			addps      xmm2, xmm1
			movaps     xmmword ptr[esi], xmm2
			add         esi, 16
			add         ebx, ecx
			cmp         ebx, edi
			jl          loop_asm

			pop         ecx
			pop         edi
			pop         esi
			pop         ebx
	}
}


////////////////////////////

// AVX versions

void high_DA_omp_asm_f_asm_double_AVX (int first_byte, int last_byte, int n_iter, int n_elem_iter) {
	__asm
	{
			push        ebx
			push        esi
			push        edi
			push		ecx

			mov ecx, n_elem_iter
			lea esi, b0
			VBROADCASTSD  ymm0, ymmword ptr[esi]
			lea esi, b1
			VBROADCASTSD  ymm1, ymmword ptr[esi]

			lea esi, w2
			add esi, first_byte
			xor ebx, ebx
			mov edi, n_iter
	/*	for (int i = 0; i < n; i++) {
			w2[i] = b0 * w2[i] * w2[i] * w2[i] + b1; 
			}
*/
		loop_asm :
			vmovapd     ymm2, ymmword ptr[esi]
			vmulpd      ymm7, ymm2, ymm2
			vmulpd      ymm7, ymm2, ymm7
			vmulpd      ymm7, ymm0, ymm7
			vaddpd      ymm7, ymm1, ymm7
			vmovapd     ymmword ptr[esi], ymm7
			add         esi, 32
			add         ebx, ecx
			cmp         ebx, edi
			jl          loop_asm

			pop         ecx
			pop         edi
			pop         esi
			pop         ebx
	}
}

void high_DA_omp_asm_f_asm_float_AVX(int first_byte, int last_byte, int n_iter, int n_elem_iter) {
	__asm
	{
			push        ebx
			push        esi
			push        edi
			push		ecx

			mov ecx, n_elem_iter
			lea esi, b0
			VBROADCASTSS  ymm0, ymmword ptr[esi]
			lea esi, b1
			VBROADCASTSS  ymm1, ymmword ptr[esi]

			lea esi, w2
			add esi, first_byte
			xor ebx, ebx
			mov edi, n_iter
			/*	for (int i = 0; i < n; i++) {
					w2[i] = b0 * w2[i] * w2[i] * w2[i] + b1;
					}
			*/
		loop_asm :
			vmovaps    ymm2, ymmword ptr[esi]
			vmulps      ymm7, ymm2, ymm2
			vmulps      ymm7, ymm2, ymm7
			vmulps      ymm7, ymm0, ymm7
			vaddps      ymm7, ymm1, ymm7
			vmovaps     ymmword ptr[esi], ymm7
			add         esi, 32
			add         ebx, ecx
			cmp         ebx, edi
			jl          loop_asm

			pop         ecx
			pop         edi
			pop         esi
			pop         ebx
	}
}
