#ifndef _SET_H_
#define _SET_H_

#include <stdlib.h>
#include "block.h"

/* 
 * Represents a set in a memory system cache. A set is an array of
 * cache blocks where each cache block stores meta data about the part
 * of memory it stores.
 */
struct set{

  struct block *setOfBlocks;
  int length;
  //  int numOfBlocks;

};

/* 
 * Returns a pointer to a dynamically allocated set with the specified
 * number of blocks allocated and initialized.
 */
struct set *initializeSet(int associativity);

/* 
 * Frees all memory dynamically allocated to store set data structure 
 */
void freeSet(struct set* set);


/* 
 * Adds the access of specified tag to the set.  Returns 1 if
 * was a hit.  Returns 0 if a miss with no eviction, -1 if a 
 * miss that caused an eviction.
 */
int addSetAccess(struct set*s, unsigned long long int tag);

// iterates all LRU's but makes line with tag equal to 0
void updateLRUs(struct set*s, unsigned long long int tag);

void updateLRUsHit(struct set*s, int LRU);

#endif
