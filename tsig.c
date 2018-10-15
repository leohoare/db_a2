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


// djb2 hash function
/*
unsigned long hash(const char *word) {
	unsigned long  hashAddress = 5381;
	for (int counter = 0; word[counter]!='\0'; counter++){
    		hashAddress = ((hashAddress << 5) + hashAddress) + word[counter];
	}
	return hashAddress;
}
*/

// by Leo written by John Shepard (in slides)
// assumes each attribute value bits is shorter than 32 bits?
int codeword(char *attr_value, int m, int k)
{
        int nbits = 0; // count of set bits
        int cword = 0;
	//Bits cword = newBits(m); // assuming m <= 32 bits
        srandom(hash_any(attr_value,256));
        while (nbits < k) {
                int i = random() % m;
                if (((1 << i) & cword) == 0) {
                        cword |= (1 << i);
                        nbits++;
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
	while ((tok = strtok_r(rest, ",", &rest))){
		// WHAT VALUES SHOULD WE SET FOR M and K
		// SHOULD BE FORMULA FROM MY UNDERSTANDING
		int cw = codeword(tok,12,2);
		printf("%i\n",cw);	
	}		
	//TODO
	return NULL;
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
