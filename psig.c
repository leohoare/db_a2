// psig.c ... functions on page signatures (psig's)
// part of SIMC signature files
// Written by John Shepherd, September 2018

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "psig.h"
#include "hash.h"
#include "tsig.h"


Bits makePageSig(Reln r, Tuple t)
{
        // by Leo
        assert(r != NULL && t != NULL);
        // iterate through each tuple
        char tCopy[strlen(t)];
        strcpy(tCopy,t);
        char *rest = tCopy;
        char *tok;
	Bits pagesig = newBits(psigBits(r));
	while ((tok = strtok_r(rest, ",", &rest))){
                Bits cw = codeword(tok, psigBits(r), codeBits(r)); 
        	orBits(pagesig,cw);
        }    
        return pagesig;
}




void findPagesUsingPageSigs(Query q)
{
	assert(q != NULL);
	Bits querysig = newBits(psigBits(q->rel));
	char *tok, *rest = q->qstring;	
	while ((tok = strtok_r(rest, ",", &rest))) {
		if (tok[0]=='?' && strlen(tok) ==1){
			// dont set any bits
		}
		else {
			Bits cw = codeword(tok, psigBits(q->rel), codeBits(q->rel));
			orBits(querysig, cw);
		}
	}
	
	
	for (int pid = 0; pid<nPsigPages(q->rel); pid++){
		Page pp = getPage(psigFile(q->rel),pid);
		q->nsigpages++;
		for (int psid = 0; psid < pageNitems(pp); psid++){
			q->nsigs++;
			Bits psig = newBits(psigBits(q->rel));
			getBits(pp, psid, psig);
			if (isSubset(psig, querysig)){
				setBit(q->pages, psid + pid*maxPsigsPP(q->rel));
			}
		}
	}

	//TODO
	//setAllBits(q->pages); // remove this
}

