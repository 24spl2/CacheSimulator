#include "set.h"
#include <stdlib.h>
#include <stdio.h>
#include "block.h"

/* 
 * Returns a pointer to a dynamically allocated set with the specified
 * number of blocks allocated and initialized.
 */
struct set *initializeSet(int associativity){

  // allocates memory for set and blocks within
  struct set *ptrSet = (struct set*) malloc(sizeof(struct set));

  struct block *initialSetOfBlocks = (struct block*)
    malloc(sizeof(struct block) * associativity);
  
  ptrSet -> setOfBlocks = initialSetOfBlocks;
  ptrSet -> length = associativity;
  
  for(int i = 0; i < associativity; i++){

    ptrSet -> setOfBlocks[i].valid = 0;
    ptrSet -> setOfBlocks[i].LRU = -1;
    ptrSet -> setOfBlocks[i].tag = 0;
  }

  return ptrSet;
}

/* 
 * Frees all memory dynamically allocated to store set data structure 
 */
void freeSet(struct set* set){

  free(set -> setOfBlocks);
  free(set);
  
}


/* 
 * Adds the access of specified tag to the set.  Returns 1 if
 * was a hit.  Returns 0 if a miss with no eviction, -1 if a 
 * miss that caused an eviction.
 */
int addSetAccess(struct set*s, unsigned long long int tag){

  int largestLRU = 0;

  // check for hit
  for(int i = 0; i < s -> length; i++){
    //issue here
    
    if(s->setOfBlocks[i].LRU > s->setOfBlocks[largestLRU].LRU){
      largestLRU = i;
    }

    if(tag == s->setOfBlocks[i].tag){

      // hit case
      if(s->setOfBlocks[i].valid == 1){
	updateLRUsHit(s, s->setOfBlocks[i].LRU);
	// printf("HIT\n");
	return 1;
      }
    }
  }

  // if no hit and space in set, this is a cold miss
  // add data and return 0;
  // do a for loop to check if Valid bit is 0, if zero then add in the block.

  for(int j = 0; j < s -> length; j++){

    if(s->setOfBlocks[j].valid == 0){
      s->setOfBlocks[j].valid=1;
      
      s->setOfBlocks[j].tag = tag;
      updateLRUs(s, tag);
      // remove
      // printf("COLD MISS\n");
      // printf("valid: %d\n", s->setOfBlocks[j].valid);
      
      return 0;
    }
    
  }  
  // miss eviction case
  s -> setOfBlocks[largestLRU].tag = tag;
  // printf("eviction\n");
  updateLRUs(s, tag);
  // printf("miss eviction\n");
  return -1;
  
}

// iterates LRUs for all blocks other than newly updated block
void updateLRUs(struct set*s, unsigned long long int tag){

  for(int i = 0; i < s -> length; i++){
    if(tag != s->setOfBlocks[i].tag && s->setOfBlocks[i].valid == 1){
      s->setOfBlocks[i].LRU++;
    } else if(s->setOfBlocks[i].valid == 1){
      s->setOfBlocks[i].LRU = 0;
    }
  }
}

// iterates LRUs for all blocks with LRUs lower than the one given
void updateLRUsHit(struct set*s, int LRU){

  for(int i = 0; i < s->length; i++){

    if(s->setOfBlocks[i].LRU < LRU && s->setOfBlocks[i].valid == 1){
      s->setOfBlocks[i].LRU++;
    } else if(s->setOfBlocks[i].LRU == LRU){
      s->setOfBlocks[i].LRU = 0;
    }
  }
}
