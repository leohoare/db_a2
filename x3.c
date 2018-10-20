// test tsig/bits data types

#include <stdio.h>
#include "defs.h"
#include "reln.h"
#include "tuple.h"
#include "tsig.h"
#include "bits.h"

int main(int argc, char **argv)
{
	Reln r = openRelation("R");
	Page pp = getPage(psigFile(r),0);
	Page bp = getPage(bsigFile(r),0);
	printf("hi %i\n",bsigBits(r));	
	Bits psig = newBits(psigBits(r));
	Bits bsig = newBits(psigBits(r)/4);

	getBits(pp, 0, psig);
	getBits(bp, 0, bsig);
	
	showBits(psig);
	printf("\n\n");
	showBits(bsig);

	printf("\n\n");
	/*
	printf("Tuple: ");
	Tuple t = readTuple(r, stdin);
	Bits b = makeTupleSig(r, t);
	showBits(b); putchar('\n');
	printf("Query: ");
	Tuple q = readTuple(r, stdin);
	Bits bq = makeTupleSig(r, q);
	showBits(bq); putchar('\n');
	if (isSubset(bq,b))
		printf("Matches\n");
	else
		printf("No match\n");
	*/
	return 0;
}
