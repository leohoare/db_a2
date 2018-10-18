// tsig.c ... functions on Tuple Signatures (tsig's)
// part of SIMC signature files
// Written by John Shepherd, September 2018

#include <unistd.h>
#include <string.h>
#include "defs.h"
#include "tsig.h"
#include "reln.h"
#include "hash.h"
#include "bits.h"
#include "util.h"
#include "hash.h"

// by Leo adjusted slides by John Shepard
// assumes each attribute value bits is shorter than 32 bits?
Bits codeword(char *attr_value, int m, int k)
{
	int nbits = 0; // count of set bits
	Bits cword = newBits(m);
	srandom(hash_any(attr_value,strlen(attr_value)));
        while (nbits < k) {
                int i = random() % m;
		if ((1 << i) && !(bitIsSet(cword,i))) {
			setBit(cword,i);	
			nbits ++;
		}
        }
        return cword; // m-bits with k 1-bits and m-k 0-bits
}

// make a tuple signature
Bits makeTupleSig(Reln r, Tuple t)
{
	// by Leo
	assert(r != NULL && t != NULL);
	// iterate through each tuple
	char tCopy[strlen(t)];
	strcpy(tCopy,t);
	char *rest = tCopy;
	char *tok;
	Bits tsig = newBits(tsigBits(r));
	int counter = tsigBits(r) - tsigBits(r)/nAttrs(r);
	while ((tok = strtok_r(rest, ",", &rest))){
		Bits cw = codeword(tok, tsigBits(r)/nAttrs(r), codeBits(r)); 
		for (int i=0; i<(tsigBits(r)/nAttrs(r)); i++){
			if (bitIsSet(cw,i)){
				setBit(tsig,i+counter);
			}
		}
		counter -= tsigBits(r)/nAttrs(r);
	}		
	return tsig;
}

// takes query and turns it into tup sig
Bits tupSigForQuery (Query q){
	// by Leo
	char qCopy[strlen(q->qstring)];
	strcpy(qCopy,q->qstring);
	char *tok, *rest = qCopy;
	Bits tsig = newBits(tsigBits(q->rel));
	int counter = tsigBits(q->rel) - tsigBits(q->rel)/nAttrs(q->rel);
	while ((tok = strtok_r(rest, ",", &rest))) {
		if (tok[0] == '?' && strlen(tok) == 1) {
			//skip already set to 0's					
		}
		else {
			Bits cw = codeword(tok, tsigBits(q->rel)/nAttrs(q->rel), codeBits(q->rel)); 
			for (int i=0; i<(tsigBits(q->rel)/nAttrs(q->rel)); i++){
				if (bitIsSet(cw,i)){
					setBit(tsig,i+counter);
				}
			}
		}
		counter -= tsigBits(q->rel)/nAttrs(q->rel);
	}
	return tsig;
}


// find "matching" pages using tuple signatures

void findPagesUsingTupSigs(Query q)
{
	// by Leo
	assert(q != NULL);
	// tuple number used for marking page bit
	int tup_num = 0;
	// create bit string from Query
	Bits qtsig = tupSigForQuery(q);
	// iterate through tsig pages
	for (int pid=0; pid<nTsigPages(q->rel); pid++){
		Page tp = getPage(tsigFile(q->rel), pid);
		q->nsigpages++;
		// iterate through tsigs in page
		for (int tid = 0; tid<pageNitems(tp); tid++ ){
			q->nsigs++;
			Bits ttsig = newBits(tsigBits(q->rel));
			getBits(tp, tid, ttsig);
			// test if tsig matches
			if (isSubset(ttsig, qtsig)) {
				// set page accordingly
				setBit(q->pages, tup_num/pageNitems(tp));
			}
			tup_num++;
		}
	}	
	
	// Remove it before submitting this function
	printf("Matched Pages:"); showBits(q->pages); putchar('\n');
}
