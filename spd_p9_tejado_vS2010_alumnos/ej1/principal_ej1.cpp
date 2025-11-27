//----------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Dpto ATC. www.atc.us.es 
//  SPD subject 3º GTI
//  - Understanding basic concepts for OpenMP
//-----------------------------------------------------

using namespace std;

#include <iostream>
#include <omp.h>

int main() {
    omp_set_num_threads(4);
    #pragma omp parallel
	// Para el apartado d agregar: #pragma omp critical
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        cout << "Hello world!. Soy el hilo " << tid << " de " << nthreads << endl;
    }
    return 0;
}


