#include "cache.h"
#include "set.h"
#include <stdio.h>

/* 
 * Returns a pointer to a dynamically allocated cache with the specified
 * associativity and using the number of specified set index bits and block 
 * offset bits.
 */
struct cache *initializeCache(int setIndexBits, int associativity,
			      int blockOffsetBits){

  // allocate memory for cache and sets within
  struct cache *cash = (struct cache*) malloc(sizeof(struct cache));
  cash->setBits = setIndexBits;
  cash->numSets = 1<<setIndexBits;
  struct set** initialSet = (struct set**) malloc(sizeof(struct set*)
						  * (cash->numSets));

  cash->sets = initialSet;
  cash->associativity = associativity;
  cash->hits = 0;
  cash->misses = 0;
  cash->evictions = 0;
  cash->offsetBits = blockOffsetBits;

  // initialize sets within cache
  for(int i = 0; i < cash->numSets; i++) {

    cash->sets[i] = initializeSet(associativity);

  }

  return cash;
}

/* 
 * Frees all memory dynamically allocated to store cache data structure 
 */
void freeCache(struct cache *cache){

  for(int i = 0; i < cache->numSets; i++){
    freeSet(cache->sets[i]);
  }
  free(cache->sets);
  free(cache);
  
}

/* 
 * Adds the access of specified address to the cache.  
 */
void addCacheAccess(struct cache *cache, unsigned long long int address){

  // unsigned long long int offset = address & (1<<offsetBits - 1);
  unsigned long long int set = (address>>(cache->offsetBits)) &
    (cache->numSets-1);
  unsigned long long int tag = address>>(cache->offsetBits + cache->setBits);
  
  // printf("set: %llX tag: %llX\n", set, tag);

  int result = addSetAccess(cache->sets[set], tag);
  /* debugging print code
  for(int i = 0; i < cache->numSets; i++){
    printf("**** SET %d ****\n", i);
    for(int j = 0; j < cache->associativity; j++){
      printf("Element %d: tag %llX valid %d lru %d\n", j, cache->sets[i]->setOfBlocks[j].tag, cache->sets[i]->setOfBlocks[j].valid, cache->sets[i]->setOfBlocks[j].LRU); 
    }
  }
  printf("\n\n");
  */

  // iterate stats depending on hit, miss, or eviction
  if(result == 1){
    cache->hits++;
  } else if(result == 0){
    cache->misses++;
  } else{
    cache->misses++;
    cache->evictions++;
  }
  
}
