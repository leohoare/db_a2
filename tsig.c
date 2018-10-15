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
#include "hash.h"


// by Leo adjusted slides by John Shepard
// assumes each attribute value bits is shorter than 32 bits?
Bits codeword(char *attr_value, int m, int k)
{
	int nbits = 0; // count of set bits
	Bits cword = newBits(m);
	srandom(hash_any(attr_value,m));
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
	char *rest = t;
	char *tok;
	Bits tsig = newBits(tsigBits(r));
	int counter = tsigBits(r) - tsigBits(r)/nAttrs(r);
	while ((tok = strtok_r(rest, ",", &rest))){
		// WHAT VALUES SHOULD WE SET K? 
		// NEED TO BACK CALCULATE FROM FALSE MATCH PROBABILITY?????
		// SHOULD BE FORMULA FROM MY UNDERSTANDING
		Bits cw = codeword(tok, tsigBits(r)/nAttrs(r), 2); // HERE
		for (int i=0; i<(tsigBits(r)/nAttrs(r)); i++){
			if (bitIsSet(cw,i)){
				setBit(tsig,i+counter);
			}
		}
		counter -= tsigBits(r)/nAttrs(r);
		// showBits(cw); 
	}		
	return tsig;
}

// find "matching" pages using tuple signatures

void findPagesUsingTupSigs(Query q)
{
	assert(q != NULL);
	//TODO

	setAllBits(q->pages); // remove this

	// The printf below is primarily for debugging
	// Remove it before submitting this function
	printf("Matched Pages:"); showBits(q->pages); putchar('\n');
}
