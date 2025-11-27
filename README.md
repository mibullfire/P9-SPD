# P9-SPD
Repositorio con el código de la Práctica 9 de SPD

## Tabla de Resultados

### Ejercicio 1

#### Código Apartado c)

```c++
int main() {
    omp_set_num_threads(4);
    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        cout << "Hello world!. Soy el hilo " << tid << " de " << nthreads << endl;
    }
    return 0;
}
```

#### Código Apartado d)

```c++
int main() {
    omp_set_num_threads(4);
    #pragma omp parallel
	  #pragma omp critical
    {
        int tid = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        cout << "Hello world!. Soy el hilo " << tid << " de " << nthreads << endl;
    }
    return 0;
}
```

### Ejercicio 2

#### Código Apartado b)

```c++
void part_b(int n)
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
```

#### Código Apartado c)

```c++
void part_c(int n)
{
#pragma omp parallel for
	for (int i = 0; i < n; i++)
	{
		cout << "Iteration " << i << " is handled by thread " << omp_get_thread_num() << "\n" << endl;	
	}
}
```

### Ejercicio 3

`Completar`!
