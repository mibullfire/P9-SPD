/////////////////////////////////////////////////////////
//  Dpto ATC. 
//  version 2. Nov. 2012. Fernando Diaz del Rio/Pablo Iñigo Blasco /Manuel J Dominguez Morales
/////////////////////////////////////////////////////////
// this timing library is based on C++ OpenCV timing function getCPUTickCount(),
// this test uses a new version of QueryPerformanceTiming_rdtsc, to measure CPU cycles more precisely
// based on Windows x86 QueryPerformanceCounter library  .

// QueryPerformanceCounter  Granularity for x86 usually is 0.5 us approx.
//but here the CPU frequency must be defined in QueryPerformanceTiming_rdtsc.h

//	Note: getCPUTickCount() and rdtsc x86 instructions (time stamp counter) is finer but it depends strongly 
//  on current CPU frequency (that is variable in most current CPUs ), and measures
//  are not exact at all if CPU frequency does not match stock CPU (e.g if CPU load is low )
/////////////////////////////////////////////////////////

#pragma once
#include <stdio.h>
#include <stdint.h>

#define CPU_FREQ_HZ (3200000000)
//write here your CPU frequency 


 class QPTimer
{
	private:
		uint64_t startTime; //it contains the real timer (that was started when PC was switch on
		uint64_t result;    // a partial timing between Start() and Stop()
		uint64_t totalTime;  // the addition of several partial timing (result). It is the total time since timer was created or ResetAll
		uint64_t minimumTime;
		uint64_t overhead;
		int times;

		void ReadCycles (uint64_t *cycles);
		void PrintCycles (const char *pc, uint64_t *cycles);
		void PrintCyclesByIteration (const char *pc, uint64_t *cycles, int NumberOfIterations);
		double GetCyclesByIteration (uint64_t *cycles, int NumberOfIterations);
		void PrintTimeSeconds (const char *pc, uint64_t *cycles);
		void RestCycles (uint64_t *c_dif, uint64_t *c_final, uint64_t *c_init);
		void AddCycles (uint64_t *c_dif, uint64_t *c_final, uint64_t *c_init);
	public:
		uint64_t QPFrequencyHz;
		bool DisableOutput;
		QPTimer();
		int  NumberOfMeasures(void);
		void Start();
		void Stop();
		
		void Calibrate();
		void Reset();
		void ResetAll();
		void PrintTime(const char* message);
		void PrintCycles(const char* message);
		void PrintMeanTime(const char* message);
		void PrintMeanCycles(const char* message);
		void PrintMinimumTime(const char* message);
		void PrintMinimumCycles(const char* message);		

		void PrintMinimumCyclesByIteration (const char* message, int NumberOfIterations);
		double GetMinimumCyclesByIteration (int NumberOfIterations);
};
