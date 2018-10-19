// psig.c ... functions on page signatures (psig's)
// part of SIMC signature files
// Written by John Shepherd, September 2018

#include <unistd.h>
#include <string.h>
#include "defs.h"
#include "reln.h"
#include "query.h"
#include "psig.h"
#include "hash.h"
#include "bits.h"
#include "tsig.h"


int BitsPerTup (Reln r){
	return (psigBits(r)/maxTupsPP(r))- (psigBits(r)/maxTupsPP(r))%8;
}



/// Try use something like this? to me that seems most logical
// The psig assigns X amount of bits per page to each tuple
// Theoretical max tuples per page is maxTupPP
// assign PsigBits(r)/MaxTupPP to each tuple
// then when find pages
// make tuple signiture that is PsigBits(r)/MaxTupPP long (with same formula)
// Although needs to account for ? as similarly to tsig find (attribute sig for that is 00000...
// it will then find like so
//==============================================================
// 	|	|	|	|	|	|	|	| length per tup = PsigBits(r)/MaxTupPP(r)
//============================================================== codeBits(R) set per attr
// ^ 
//xxxxx	|
//	|xxxxxxx|
//		|xxxxxxx|
//			so on....
// Check for is subset at teach age of query signutres (whether psig is subset of qsig)
// iterate through each with Query sig length PsigBits(r)/MaxTupPP
// say the page isn't full
//==============================================================
//000101|1100101|0110110|0000000|0000000|0000000|0000000|0000000| length per tup = PsigBits(r)/MaxTupPP(r)
//============================================================== codeBits(R) set per attr
// make signuture	|1010101| 
// either and bits or just memcpy signiture into correct place to make
//
//==============================================================
//000101|1100101|0110110|1010101|0000000|0000000|0000000|0000000| length per tup = PsigBits(r)/MaxTupPP(r)
//============================================================== codeBits(R) set per attr


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

/*
QuerySig = makePageSig(Query)
Pages = AllZeroBits
foreach Psig in psigFile {
    if (Psig matches QuerySig) {
        PID = data page corresponding to Psig
        include PID in Pages
    }
}
*/

/*
// make a page signature (to use in addToRelation?)
Bits makePageSig(Reln r, Tuple t)
{
	// by Leo
	assert(r != NULL && t != NULL);
	// iterate through each tuple
	char tCopy[strlen(t)];
	strcpy(tCopy,t);
	char *rest = tCopy;
	char *tok;
	int tupbitlen = psigBits(r)/maxTupsPP(r); 
	Bits tsig = newBits(tupbitlen);
	int counter = tsigBits(r) - tupbitlen;
	while ((tok = strtok_r(rest, ",", &rest))){
                Bits cw = codeword(tok, tupbitlen/nAttrs(r), codeBits(r)); 
                for (int i=0; i<(tupbitlen/nAttrs(r)); i++){
                        if (bitIsSet(cw,i)){
                                setBit(tsig,i+counter);
                        }
                }
                counter -= tupbitlen;
        }    
        return tsig;
}

*/
Bits makePageSig(Reln r, Tuple t){
	char **tuple = tupleVals(r,t);
	for (int i; i < strlen(tuple); i++){
		
	}

	return NULL;
}


void findPagesUsingPageSigs(Query q)
{
	assert(q != NULL);
	
	
	

	//TODO
	setAllBits(q->pages); // remove this
}


