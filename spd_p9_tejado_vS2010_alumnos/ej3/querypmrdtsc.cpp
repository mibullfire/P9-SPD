/////////////////////////////////////////////////////////
//  Dpto ATC. 
//  version 2. Nov. 2012. Fernando Diaz del Rio/Pablo Iñigo Blasco /Manuel J Dominguez Morales
//  Some magic preprocessing by Miguel Angel Rodriguez-Jodar to succesfully compile under Linux
/////////////////////////////////////////////////////////
// this timing library is based on C++ OpenCV timing function getCPUTickCount(), that are
// based on Windows x86 QueryPerformanceCounter library  .

// QueryPerformanceCounter  Granularity for x86 usually is 0.5 us approx.
//but here the CPU frequency must be defined in QueryPerformanceTiming_rdtsc.h

//	Note: getCPUTickCount() and rdtsc x86 instructions (time stamp counter) is finer but it depends strongly 
//  on current CPU frequency (that is variable in most current CPUs ), and measures
//  are not exact at all if CPU frequency does not match stock CPU (e.g if CPU load is low )
/////////////////////////////////////////////////////////


#include <stdio.h>
#include <iostream>
#include <stdint.h>

#ifdef WIN32
#include <intrin.h>
#endif

using namespace std;

#include "querypmrdtsc.h"


#define MAX_DOUBLE_VALUE (1e+307)
/////////////////////////////////////////////////////////
//public methods 
/////////////////////////////////////////////////////////
QPTimer::QPTimer()
{
    minimumTime= UINT64_MAX;
	times=0;
	overhead= 0;
	totalTime=0;
	Reset();
	QPFrequencyHz =  CPU_FREQ_HZ ;
	//QPFrequencyHz = getTickFrequency();
	DisableOutput=false;
}

void QPTimer::Start()
{
	ReadCycles(&startTime);
}

void QPTimer::Stop()
{
	uint64_t now;
	ReadCycles(&now);

	uint64_t r;
	RestCycles(&r,&now,&startTime);
	RestCycles(&r,&r,&overhead);

	AddCycles(&result,&result,&r);

	AddCycles(&totalTime,&totalTime,&r);

	if (minimumTime > result)
			minimumTime = result;

	times++;
}
///////////////////////////////////////////////////
void QPTimer::Calibrate()
{
	int test_times=50;
    overhead= UINT64_MAX;

	for (int i=0; i<test_times; i++)
	{
		uint64_t r;
		uint64_t now;

		ReadCycles(&startTime);
		ReadCycles(&now);

		RestCycles(&r,&now,&startTime);
		
		if (overhead > r)
			overhead = r;
	}
	ResetAll();
}


void QPTimer::Reset()
{
	//startTime=0.0;
	result=0;
}

void QPTimer::ResetAll()
{
	//startTime=0.0;
	result=0;
    minimumTime=UINT64_MAX;
	totalTime=0;

	times=0;
}

void QPTimer:: PrintTime(const char* message)
{
	if(!DisableOutput)
		PrintTimeSeconds(message,&result);
}

void QPTimer:: PrintCycles(const char* message)
{
	if(!DisableOutput)
		PrintCycles(message,&result);
}

void QPTimer::PrintMinimumTime(const char* message)
{
	if(!DisableOutput)
		PrintTimeSeconds(message,&minimumTime);
}

void QPTimer::PrintMinimumCycles(const char* message)
{
	if(!DisableOutput)
		PrintCycles(message,&minimumTime);
}

void QPTimer::PrintMinimumCyclesByIteration (const char* message, int num)
{
	if(!DisableOutput)
		PrintCyclesByIteration(message,&minimumTime,num);
}

double QPTimer::GetMinimumCyclesByIteration (int num)
{
		return GetCyclesByIteration(&minimumTime,num);
}

void QPTimer::PrintMeanTime (const char* message)
{
	if(!DisableOutput)
	{
		uint64_t MeanTime ;
		MeanTime = (totalTime/times);

		PrintTimeSeconds(message,&MeanTime);
		cout << "   (in " << times << " measures)" <<endl;
	}
}

void QPTimer::PrintMeanCycles (const char* message)
{
	if(!DisableOutput)
	{
		uint64_t MeanCycles ;
		MeanCycles = (totalTime/times);

		PrintCycles(message,&MeanCycles );
		cout << "(in " << times << " measures)" <<endl;
	}
}
int QPTimer::NumberOfMeasures (void) 
{
	return times;
}

/////////////////////////////////////////////////////////
//private methods 
/////////////////////////////////////////////////////////

#ifdef __GNUG__
#if defined(__i386__)

static __inline__ unsigned long long __rdtsc(void)
{
  unsigned long long int x;
     __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
     return x;
}
#else
static __inline__ unsigned long long __rdtsc(void)
{
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}
#endif
#endif

void QPTimer::ReadCycles (uint64_t *cycles)
{
	uint64_t time;

	time = __rdtsc();
    *cycles = time;
}

void QPTimer::PrintCycles (const char *pc, uint64_t *cycles)
{
	cout << pc << *cycles ;
}	


void QPTimer::PrintCyclesByIteration (const char *pc, uint64_t *cycles, int NumberOfIterations)
{
	cout << pc << *cycles/(double)NumberOfIterations ; // << endl;
}	


double QPTimer::GetCyclesByIteration (uint64_t *cycles, int NumberOfIterations)
{
	return (*cycles/(double)NumberOfIterations) ;
}	


void QPTimer::PrintTimeSeconds (const char *pc, uint64_t *cycles)
{   
	cout << pc << *cycles/(double)QPFrequencyHz << endl;
/*	if(pc!=NULL && strlen(pc)>0)
		printf("%s",pc);		*/
}
void QPTimer::RestCycles (uint64_t *c_dif, uint64_t *c_final, uint64_t *c_init)
{
	*c_dif=*c_final-*c_init;

}

void QPTimer::AddCycles (uint64_t *c_add, uint64_t *c_final, uint64_t *c_init)
{
	*c_add=*c_final+*c_init;
}
/////////////////////////////////////////////////////////
