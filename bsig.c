// bsig.c ... functions on Tuple Signatures (bsig's)
// part of SIMC signature files
// Written by John Shepherd, September 2018

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "bsig.h"
#include "tsig.h"
#include "psig.h"

void findPagesUsingBitSlices(Query q)
{
	assert(q != NULL);
	int checker[nPages(q->rel)];
	for (int i=0; i<nPages(q->rel); i++){ checker[i] = 0; } 	
	int counter = 0;
	Page bp;
	Bits qsig = makePQuerySig(q);	
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
	for (int i=0; i<nPages(q->rel); i++){
		if (checker[i] == counter){
			setBit(q->pages,i);
		}
	}	
	printf("Matched Pages:"); showBits(q->pages); putchar('\n');
}

