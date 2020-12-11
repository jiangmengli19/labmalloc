/**
 * UW CSE 351 Spring 2016 Lab 5
 *
 * Extra credit: implementing mm_realloc
 *
 * This requires mm_malloc and mm_free to be working correctly, so
 * don't start on this until you finish mm.c.
 */
#include "mm.c"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// Extra credit.
void* mm_realloc(void* ptr, size_t size) {

    // Write your code here ...
	//printf("pointer %p\n",ptr);
    if(ptr == NULL){
    	return mm_malloc(size);
    }
    if(size == 0){
    	mm_free(ptr);
    	return NULL;
    }


    BlockInfo *OldBlock = (BlockInfo *)UNSCALED_POINTER_SUB(ptr,WORD_SIZE);


    //checksize original size
    size_t checksize = SIZE(OldBlock->sizeAndTags);

    size_t asize;

   	size = size+WORD_SIZE;
    if(size<=MIN_BLOCK_SIZE){
    	asize = MIN_BLOCK_SIZE;
    }
    else{
    	asize = ALIGNMENT * ((size + ALIGNMENT - 1) / ALIGNMENT);
    }
    BlockInfo *followingBlock;
    followingBlock = (BlockInfo *)UNSCALED_POINTER_ADD(OldBlock,checksize);
    //printf("asize %zu\n",asize);
    size_t Preusedvalue = (OldBlock->sizeAndTags)&TAG_PRECEDING_USED;
    if(checksize >= asize){
        /*
    	if(checksize<asize+MIN_BLOCK_SIZE){
    	return ptr;
    	}

    	else{
    		size_t * prevBlock = (size_t *)(UNSCALED_POINTER_SUB(followingBlock,WORD_SIZE));
    		*prevBlock = (checksize-asize)|TAG_PRECEDING_USED;
    		BlockInfo * realnext = (BlockInfo *)UNSCALED_POINTER_ADD(OldBlock,asize);
    		realnext->sizeAndTags = (checksize-asize)|TAG_PRECEDING_USED;
    		insertFreeBlock(realnext);
    		coalesceFreeBlock(realnext);
            OldBlock->sizeAndTags = asize|TAG_USED;
            OldBlock->sizeAndTags = (OldBlock->sizeAndTags)|Preusedvalue;
    		return ptr;
    	}

    	/*
    	if(checksize==asize){
    		printf("when checksize==asize,we check the followingBlock Tag see what happen\n");
    		printf("following Tag %zu\n", followingBlock->sizeAndTags  );

    	}
    	*/
    	return ptr;


    }


    size_t nextAlloc = (followingBlock->sizeAndTags)&TAG_USED;
    //printf("nextalloc %zu\n",nextAlloc);
    size_t totalsize = checksize+SIZE(followingBlock->sizeAndTags);
    //size_t Preusedvalue = (OldBlock->sizeAndTags)&TAG_PRECEDING_USED;

    if((!nextAlloc)&&(totalsize>=asize)){
       //need to request some space first



    	OldBlock->sizeAndTags = totalsize|TAG_USED;
    	OldBlock->sizeAndTags = (OldBlock->sizeAndTags)|Preusedvalue;
        //printf("next size %zu\n",totalsize);
        removeFreeBlock(followingBlock);
    	*((size_t *)UNSCALED_POINTER_ADD(OldBlock,totalsize)) = *((size_t *)UNSCALED_POINTER_ADD(OldBlock,totalsize))|TAG_PRECEDING_USED;

        //printf("final tag %zu\n",*((size_t *)UNSCALED_POINTER_ADD(OldBlock,totalsize)));


    	//printf("ptr : %p\n",ptr);
    	return ptr;

    }
    /*


    if((((followingBlock->sizeAndTags)&TAG_USED)==0)&&(SIZE(followingBlock->sizeAndTags)+checksize>=asize)){

    	size_t totalsize = checksize+SIZE(followingBlock->sizeAndTags);
    	size_t Preusedvalue = (OldBlock->sizeAndTags)&TAG_PRECEDING_USED ;
    	if(totalsize-asize<=MIN_BLOCK_SIZE){

    		removeFreeBlock(followingBlock);
    		OldBlock->sizeAndTags =  totalsize|TAG_USED;
            OldBlock->sizeAndTags = (OldBlock->sizeAndTags)|Preusedvalue;

            *((size_t *)UNSCALED_POINTER_ADD(OldBlock,totalsize))=*((size_t *)UNSCALED_POINTER_ADD(OldBlock,totalsize))|TAG_PRECEDING_USED;

    	}
 /*
    	else{
    	OldBlock->sizeAndTags = asize|TAG_USED;
    	OldBlock->sizeAndTags = (OldBlock->sizeAndTags)|Preusedvalue;

    	removeFreeBlock(followingBlock);


    	//BlockInfo *currentNextFree = followingBlock->next;
    	//BlockInfo *currentPrevFree = followingBlock->prev;
    	BlockInfo *nextFree = (BlockInfo *)UNSCALED_POINTER_ADD(OldBlock,asize);
        /*
    	nextFree->sizeAndTags = (totalsize-asize)|TAG_PRECEDING_USED;

    	nextFree->next = currentNextFree;
    	nextFree->prev = currentPrevFree;
    	if(currentNextFree!=NULL){
    			  currentNextFree->prev = nextFree;
	    }
    		  //May have problem here
		if(currentPrevFree!=NULL){
			  currentPrevFree->next = nextFree;
		}
		else{
			  FREE_LIST_HEAD = nextFree;
		}




		*((size_t*)UNSCALED_POINTER_ADD(nextFree,totalsize-asize-WORD_SIZE)) = (totalsize-asize)|TAG_PRECEDING_USED;
		insertFreeBlock(nextFree);

    	}

    	return ptr;


    }
*/

    //printf("The address after followingblock %p\n",(size_t *)UNSCALED_POINTER_ADD(followingBlock,SIZE(followingBlock->sizeAndTags)));
    //printf("The address at the end %p\n",(size_t *)UNSCALED_POINTER_SUB(mem_heap_hi(),WORD_SIZE-1));
    if((size_t *)UNSCALED_POINTER_ADD(OldBlock,checksize)==(size_t *)UNSCALED_POINTER_SUB(mem_heap_hi(),WORD_SIZE-1)){
/*
    	size_t totalsize = SIZE(followingBlock->sizeAndTags)+checksize;
    	requestMoreSpace(asize-totalsize);
    	size_t *lastfoot = (size_t *)UNSCALED_POINTER_SUB(mem_heap_hi(),WORD_SIZE-1);
    	size_t newtotalsize = SIZE(*lastfoot)+checksize;
    	size_t Preusedvalue = (OldBlock->sizeAndTags)&TAG_PRECEDING_USED ;
    	if(newtotalsize-asize<MIN_BLOCK_SIZE){
    		removeFreeBlock(followingBlock);
    		OldBlock->sizeAndTags = newtotalsize|TAG_USED;
    		OldBlock->sizeAndTags = OldBlock->sizeAndTags|Preusedvalue;
    	}
    	else{
    		OldBlock->sizeAndTags = asize|TAG_USED;
			OldBlock->sizeAndTags = (OldBlock->sizeAndTags)|Preusedvalue;
    		//size_t newfreesize = newtotalsize-asize;
    		BlockInfo *nextFree = (BlockInfo *)UNSCALED_POINTER_ADD(OldBlock,asize);
    		BlockInfo *currentNextFree = followingBlock->next;
			BlockInfo *currentPrevFree = followingBlock->prev;
    		nextFree->sizeAndTags = (newtotalsize-asize)|TAG_PRECEDING_USED;
    		nextFree->next = currentNextFree;
    		nextFree->prev = currentPrevFree;
    		if(currentNextFree!=NULL){
    		   currentNextFree->prev = nextFree;
    	    }
    		if(currentPrevFree!=NULL){
    		   currentPrevFree->next = nextFree;
    		}
    		else{
    		    FREE_LIST_HEAD = nextFree;
    		}
    		*((size_t*)UNSCALED_POINTER_ADD(nextFree,newtotalsize-asize-WORD_SIZE)) = (newtotalsize-asize)|TAG_PRECEDING_USED;

    	}

    	return ptr;
    	*/
    	//printf("can optimize this\n");
    	//printf("nextalloc %zu/n",nextAlloc);
    	size_t requireSize = asize-checksize;
    	requestMoreSpace(requireSize);
    	size_t *preaddress = (size_t *)UNSCALED_POINTER_SUB(mem_heap_hi(),WORD_SIZE-1);
    	//can directly remove the followingBlock
    	removeFreeBlock(followingBlock);
    	size_t size_diff = (char *)preaddress- (char *)OldBlock;
    	OldBlock->sizeAndTags = size_diff|TAG_USED;
    	OldBlock->sizeAndTags = (OldBlock->sizeAndTags)|Preusedvalue;
    	*preaddress = *preaddress|TAG_PRECEDING_USED;
    	return ptr;
    }










    //printf("we use malloc here\n");
    void *returnptr = mm_malloc(size);

    size_t *movptr = (size_t *)returnptr;

    size_t payloadSize = checksize-WORD_SIZE;
    size_t* checkpointer;
    //may have problem in for loop

    for(checkpointer = (size_t*)ptr;checkpointer<(size_t *)UNSCALED_POINTER_ADD(ptr,payloadSize);checkpointer = (size_t*)UNSCALED_POINTER_ADD(checkpointer,WORD_SIZE)){

        //*((char *)movptr) =
        *movptr = *((size_t *)checkpointer);
    	movptr = (size_t *)UNSCALED_POINTER_ADD(movptr,WORD_SIZE);
    }


    //memcpy(returnptr,ptr,payloadSize);

    mm_free(ptr);
    //printf("The return ptr is %p\n",returnptr);
    return returnptr;

}
