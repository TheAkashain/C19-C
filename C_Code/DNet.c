#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "DNet.h"

int TI;
int STATE;

/*
  List of Disease Statuses:
  0 - Susceptible
  1 - Exposed
  2 - Pre-symptomatic
  3 - Symptomatic
  4 - Asymptomatic
  5 - Removed
  6 - Tested
  7 - Tested Removed
*/

void simulate(pPerson* orig, int size, int runs, String file, double data[]) {
  srand(time(NULL));
  /* DATA ORDER - HIGHLY IMPORTANT
     0 - Days
     1 - Beta (Infection Chance)
     2 - Alpha (Exposed to Pre-Symptomatic Chance)
     3 - Gamma (Recovery Chance)
     4 - Asymptomatic Chance
     5 - Asymptomatic Infection Rate
     6 - Symptomatic (Voluntary) Test Chance
     7 - Random Test Chance
     8 - Test Success Chance
     9 - Pre-symp Time (Days)
     10 - Border Crossings Per Day
     11 - Importation Infection Chance
     12 - Starting Phase (0 = yellow, 1 = orange, 2 = red)
     13 - Orange Case Threshold
     14 - Red Case Threshold 
     15 - Travel Isolation Time (0 = full house, 1 = just travel, 2 = non)
     16 - Average Travel Time
   */

  //Open file by provided name
  int compare = strcmp(file, "stdout");
  FILE* fPtr;
  if(compare == 0)
    fPtr = stdout;
  else {
    fPtr = fopen(file, "w");
    if(fPtr==NULL) {
      printf("Failed to open file.\n");
      return;
    }
  }

  //Set the total number of days per simulation
  int days = data[0];

  //Loop through the total number of executions
  for(int i = 0; i < runs; i++) {

    //Copy the network
    pPerson* testwork = createCopy(orig, size);

    //Print which run we are on, for keeping track of progress
    int frac = i / (double)runs * 10 + 1;
    printf("[");
    int count;
    for(count = 0; count < frac; count++) {
      printf("#");
    }
    for(; count < 10; count++) {
      printf("-");
    }
    
    printf("] \tRun %d of %d\r",i+1, runs);
    fflush(stdout);

    //Pick the index of a random person to be infected and set infected to 1
    int randval = rand() % 20000;
    testwork[randval]->status = 4;
    TI = 1;
    STATE = 0;

    //Loop through the days
    for(int j = 0; j < days; j++) {
      day(testwork, size, data);
      if(TI > 80) {
	STATE = 2;
      }
      else if(TI > 20) {
	STATE = 1;
      }
      else {
	STATE = 0;
      }
    }

    //Save and delete the network
    fPrintNetworkStatus(testwork, size, fPtr);
    freeNetwork(testwork, size);
  }

  //Close the file when done
  fclose(fPtr);
  printf("\n");
}

void day(pPerson* network, int size, double data[]) {
  double rands[size * 2];
  int rand_count = 0;
  for(int i = 0; i < size*2; i++) {
    rands[i] = rand() / ((double) RAND_MAX);
  }

  int final[size];
  for(int i = 0; i < size; i++) {
    final[i] = network[i]->status;
  }
	
  for(int i = 0; i < size; i++) {
    int state = network[i]->status;

    if(state != 0) {
      switch(state) {
      case 1: //Exposed
	if(rands[rand_count] < data[2]) {
	  //Pre-Symp
	  final[i] = 2;
	}
	rand_count += 1;
	break;
	
      case 2: //Pre-symptomatic
	if(network[i]->counter < data[9]) {
	  network[i]->counter += 1;
	}
	else {
	  if(rands[rand_count] < data[4]) {
	    //Asymp
	    final[i] = 4;
	    network[i]->counter = 0;
	  }
	  else{
	    //Symp
	    final[i] = 3;
	    network[i]->counter = 0;
	  }
	  rand_count += 1;
	}
	break;
	
      case 3: //Asymptomatic
	infect( network[i], data[1], data[5], final);

	if(rands[rand_count] < data[3]) {
	  //Removed
	  final[i] = 5;
	}
	else if(rands[rand_count+1] < data[6]) {
	  if(rands[rand_count+2] < data[8]) {
	    //Tested
	    final[i] = 6;
	    TI = TI + 1;
	  }
	}
	rand_count += 3;
	
	break;
	
      case 4: //Symptomatic
	infect(network[i], data[1], data[5], final);

	if(rands[rand_count] < data[3]) {
	  //Removed
	  final[i] = 5;
	}
	else if(rands[rand_count+1] < data[7]) {
	  if(rands[rand_count+2] < data[8]) {
	    //Tested
	    final[i] = 6;
	    TI = TI + 1;
	  }
	}
	rand_count += 3;
	break;
	
      case 6: //Tested	
	if(network[i]->counter == 2) {
	  test(network[i], data[8], final);
	}
	else {
	  network[i]->counter += 1;
	}

	if(rands[rand_count] < data[3]) {
	  //Tested Removed
	  final[i] = 7;
	  TI = TI - 1;
	  rand_count += 1;
	}
	
	break;
	
      case 7: //Tested Removed	
	if(network[i]->counter == 2) {
	  test(network[i], data[8], final);
	}
	else {
	  network[i]->counter += 1;
	}
	break;
      }
    }
  }
  for(int i = 0; i < size; i++) {
    network[i]->status = final[i];
  }
}

void infect(pPerson person, double beta, double rate, int* final) {  
  double lrate = 1;
  if(person->status == 3) {
    lrate = rate;
  }

  double randsL[person->cCount];
  for(int i = 0; i < person->cCount; i++) {
    randsL[i] = rand() / ((double) RAND_MAX);
  }

  //Infections for YELLOW
  if(STATE == 0) {
    //Loop through the connections
    for(int i = 0; i < person->cCount; i++) {
      //Conditions of if person is suscetible, if the random works, and if the connection type if valid (NA for Y)
      if((final[person->connections[i]] == 0) & (randsL[i] < (beta*lrate))) {
	final[person->connections[i]] = 1;
      }
    }
  }

  //Infections for ORANGE
  else if(STATE == 1) {
    for(int i = 0; i < person->cCount; i++) {
      if((final[person->connections[i]] == 0) & (randsL[i] < (beta*lrate)) & (person->con_type[i] != 2)) {
	final[person->connections[i]] = 1;
      }
    }
  }

  //Infections for RED
  else if(STATE == 2) {
    for(int i = 0; i < person->cCount; i++) {
      if((final[person->connections[i]] == 0) & (randsL[i] < (beta*lrate)) & (person->con_type[i] == 2)) {
	final[person->connections[i]] = 1;
      }
    }
  }
}

void test(pPerson person, double test, int* final) {
  for(int i = 0; i < person->cCount; i++) {
    double rands = rand() / ((double) RAND_MAX);
    if(rands < test) {
      if((final[person->connections[i]] == 3 ) | (final[person->connections[i] == 4]) | (final[person->connections[i] == 2])) {
	final[person->connections[i]] = 6;
	TI = TI+1;
      }
    }
  }
}

void fPrintNetworkStatus(pPerson* network, int size, FILE* fPtr) {
  int status_count[8] = {0};
  for(int i = 0; i < size; i++) {
    status_count[network[i]->status] += 1;
  }
  if(status_count[0] < size-1) {
    fprintf(fPtr, "%d, %d, %d, %d, %d, %d, %d, %d, %d\n", status_count[0], status_count[1],
	    status_count[2], status_count[3], status_count[4], status_count[5],
	    status_count[6], status_count[7], size - status_count[0]);
  }
}
