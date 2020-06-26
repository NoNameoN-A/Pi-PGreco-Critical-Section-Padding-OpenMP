#include <omp.h>
#include <iostream>
using namespace std;
int main (){
	//VARIABLES
	unsigned NUM_THREADS = 8;
	unsigned ripetizioniTot = 10000000;

	double pGreco;
	double ripetizione = 1.0/(double) ripetizioniTot;
	omp_set_num_threads(NUM_THREADS);
	double x, aux, somma;
	double startTime, stopTime;

	//NUMERICAL INTEGRATION



	//CRITICAL SECTION

	startTime = omp_get_wtime();
	#pragma omp parallel
	{
		for(int i = 0 ; i < ripetizioniTot ; i++){
			x = (i+0.5)*ripetizione;
			aux = 4.0/(1.0 + x*x);
			#pragma omp critical
			somma = somma + aux;
		}
	}
	pGreco = ripetizione * somma;
	stopTime = omp_get_wtime();

	cout << "CRITICAL SECTION" << endl;
	cout << "Tempo impiegato: " << (stopTime - startTime) * 1000 << " millisecondi";
	cout << endl;

	//PADDING
	int nthreads, PAD = 8;
	double sommaV[NUM_THREADS][PAD];
	startTime = omp_get_wtime();
	#pragma omp parallel
	{
		int i, nthrds;
		int id = omp_get_thread_num();
		nthrds = omp_get_num_threads();
		if (id == 0) nthreads = nthrds;
		for(int i=id;i<ripetizioniTot;i+=nthrds){
			x = (i+0.5)*ripetizione;
			sommaV[id][0] += 4.0/(1.0+x*x);
		}
	}
		for(int i = 0, pGreco = 0.0; i < nthreads; i++){
			pGreco += sommaV[i][0]*ripetizione;
		}
	stopTime = omp_get_wtime();
	cout << "PADDING" << endl;
	cout << "Tempo impiegato: " << (stopTime - startTime) * 1000 << " millisecondi";
	cout << endl;
}
