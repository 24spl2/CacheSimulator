#ifndef _CACHE_H_
#define _CACHE_H_

#include <stdlib.h>
#include "set.h"

/* 
 * Represents a memory system cache.  A memory system cache is an array
 * of sets, where each set is an array of cache blocks and each cache
 * block stores meta data about the part of memory it stores.
 */
struct cache{

  struct set **sets; //pointer to array of sets                                                                          
  int numSets;
  int setBits;
  int associativity;
  int offsetBits;
  int hits;
  int misses;
  int evictions;

};

/* 
 * Returns a pointer to a dynamically allocated cache with the specified
 * associativity and using the number of specified set index bits and block 
 * offset bits.
 */
struct cache *initializeCache(int setIndexBits, int associativity,
			      int blockOffsetBits);

/* 
 * Frees all memory dynamically allocated to store cache data structure 
 */
void freeCache(struct cache *cache);

/* 
 * Adds the access of specified address to the cache.  
 */
void addCacheAccess(struct cache *cache, unsigned long long int address);

#endif
