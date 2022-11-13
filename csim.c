#include "cachelab.h"
#include "cache.h"
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_LENGTH 80

int main(int argc, char **argv)
{
  int numSets = 1;
  int associativity = 1;
  int numBlockBits = 1;
  char traceFile[MAX_LENGTH] = "";


  // store s, E, and b from arguments
  int c;
  while((c = getopt(argc, argv, "s:E:b:t:")) != -1){
    switch(c){
    case 's':
      numSets = atoi(optarg);
      break;
    case 'E':
      associativity = atoi(optarg);
      break;
    case 'b':
      numBlockBits = atoi(optarg);
      break;
    case 't':
      strcpy(traceFile, optarg);
      break;
    }
  }
  
  struct cache *simCache = initializeCache(numSets,
					   associativity, numBlockBits);

  FILE *fp;
  char str[60];
  
  fp = fopen(traceFile, "r");
  if(fp == NULL) {
    perror("Error opening file");
    return -1;
  }
  
  // get next line
  while(fgets(str, 60, fp) != NULL){

    char instruction[3];
    unsigned long long int address;
    int blockSize;

    // interprets elements in line as instruction type, address, and size
    sscanf(str, " %s %llx,%d", instruction, &address, &blockSize);  

    // skip if 'I' trace
    if(!strcmp(instruction,"I")){
      continue;
    }

    // check address in cache for L, S, or M
    addCacheAccess(simCache, address);

    // check again if it is a M instruction
    if(!strcmp(instruction, "M")){
      addCacheAccess(simCache, address);
    }
    
  }

  // prints out stats
  printSummary(simCache->hits, simCache->misses, simCache->evictions);
  
  freeCache(simCache);
  fclose(fp);
  return 0;
}
