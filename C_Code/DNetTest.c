#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "DNet.h"
#include <time.h>

int main() {
  printf("Starting Network Generation.\n");
  int size = 50000;
  //int connections = 12;
  String fName = "Net_Test.txt";

  clock_t start, end;
  double cpu_time_used;
  start = clock();
  pPerson* test = createNetwork(size);
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

  printNetwork(test, size, fName);
  
  printf("Network Generation Time: %f\n",cpu_time_used);

  //Disease Data
  double r0 = 3.1;
  double disease_length = 14;
  double people_total = 12 * disease_length;
  
  double infect_chance = r0 / people_total;
  double alpha = (1 - pow(0.5,(1/4.3)));
  double gamma = (1 - pow(0.5,(1/5.8)));
  double asymp_chance = 0.20;
  double asymp_rate = 0.33;
  double symp_test = 0.1;
  double rand_test = 0;
  double test_success = 1.0;
  double pre_time = 2.0;

  int days;
  printf("Enter Number of Days: ");
  scanf("%d", &days);
  int runs;
  printf("Enter Number of Runs: ");
  scanf("%d", &runs);

  //Simulation
  start = clock();
  double data[11] = {days, infect_chance, alpha, gamma, asymp_chance, asymp_rate, symp_test, rand_test, test_success, pre_time};
  printf("Starting Simulation.\n");
  simulate(test, size, runs, "Sim_Test.txt", data);
  end = clock();
  cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

  printf("Simulation Time: %f seconds\n",cpu_time_used);
  printf("Average of %f seconds per Simulation\n", cpu_time_used/runs);
  printf("Results saved to 'Sim_Test.txt'\n");
  
  freeNetwork(test, size);
  return(0);
}