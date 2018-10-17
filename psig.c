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

// make a page signature (to use in addToRelation?)
Bits makePageSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	char tCopy[strlen(t)];
	strcpy(tCopy,t);
	char *rest = tCopy;
	char *tok;
	Bits psig = newBits(psigBits(r));  //new bits with width of page sig(bits)
	int count = psigBits(r) - psigBits(r)/nTuples(r);  //might be wrong
	while (( tok = strtok_r(rest, ",", &rest) )){
		Bits cw = codeword(tok, psigBits(r)/nTuples(r), codeBits(r)); //
		for (int i = 0; i < (psigBits(r)/nTuples(r)); i++){
			if (bitIsSet(cw,i)){
				setBit(psig, i+count);
			}

		}
		count -= psigBits(r)/nTuples(r);

	}

	return psig; // remove this
}


// takes query and turns it into page sig
Bits pageSigForQuery (Query q){
	char qCopy[strlen(q->qstring)];
	strcpy(qCopy, q->qstring);
	char *rest = qCopy;
	char *tok;
	//Bits psig = newBits(psigBits(q->rel ->params));
	Bits psig = newBits(psigBits(q->rel));
	int count = psigBits(q->rel) - psigBits(q->rel)/nTuples(q->rel);  //might be wrong
	while (( tok = strtok_r(rest, ",", &rest) )){
		Bits cw = codeword(tok, psigBits(q->rel)/nTuples(q->rel), codeBits(q->rel)); //
		for (int i = 0; i < (psigBits(q->rel)/nTuples(q->rel)); i++){
			if (bitIsSet(cw,i)){
				setBit(psig, i+count);
			}

		}
		count -= psigBits(q->rel)/nTuples(q->rel);

	}

	return psig; // remove this

}



//check if numTuples per page * tsig bit size == psig size

// find "matching" pages using page signatures
void findPagesUsingPageSigs(Query q)
{
	assert(q != NULL);
	int page_num = 0;
	Bits qpsig = pageSigForQuery(q);
	for (int pid = 0; pid < nPsigPages(q->rel); pid++){
		Page p = getPage(psigFile(q->rel), pid);
		q->nsigpages++;        //
		//no. of items in page?
		for (int tid = 0; tid < pageNitems(p); tid++ ){
			q->nsigs++;
			Bits psig = newBits(psigBits(q->rel));
			getBits(p, tid, psig);
			if (isSubset(psig, qpsig)){
				setBit(q->pages, page_num/pageNitems(p));

			}
			page_num++;

		}     


	}

	
}

