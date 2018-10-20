// bsig.c ... functions on Tuple Signatures (bsig's)
// part of SIMC signature files
// Written by John Shepherd, September 2018

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "bsig.h"
#include "tsig.h"
#include "psig.h"



// Searches through query signiture
// finds where bits are set
// where bit is set, it will open the bitslice associated with it
// it will then iterate through checker and increment a count for each page
// if the count for each page is equal to how many bits are set -> match
void findPagesUsingBitSlices(Query q)
{
	assert(q != NULL);
	// checker checks if for each page, all bits are subset of query
	int checker[nPages(q->rel)];
	for (int i=0; i<nPages(q->rel); i++){ checker[i] = 0; } 	
	int counter = 0;
	Page bp;
	Bits qsig = makePQuerySig(q);	
	// loop through page sig for query
	for (int psbit=0; psbit < psigBits(q->rel); psbit++){
		if (bitIsSet(qsig,psbit) == 1) {
			counter++;
			bp = getPage(bsigFile(q->rel),psbit/maxBsigsPP(q->rel));
			q->nsigs++;
			q->nsigpages++;
			Bits bitslice = newBits(bsigBits(q->rel));
			getBits(bp, psbit % maxBsigsPP(q->rel), bitslice);
			for (int bid = 0; bid < nPsigs(q->rel); bid++){
				if (bitIsSet(bitslice,bid) == 1){
					checker[bid]++;
				}
			}
			freeBits(bitslice);
		}
		
	}
	// check as described earlier, if page is subset of query 
	for (int i=0; i<nPages(q->rel); i++){
		if (checker[i] == counter){
			setBit(q->pages,i);
		}
	}	
}

