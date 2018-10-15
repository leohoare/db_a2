// test tsig dt

#include <stdio.h>
#include "defs.h"
#include "tuple.h"
#include "tsig.h"
#include "bits.h"
#include "reln.h"

int main(int argc, char **argv)
{
	Reln r = openRelation("R");
	for (int pid=0; pid<nPages(r); pid++){
		Page p = getPage(dataFile(r), pid);
		for (int tid = 0; tid<pageNitems(p); tid++){
			Tuple t = getTupleFromPage(r, p, tid);
			Bits b = makeTupleSig(r, t);
			if (b) {printf("yes tuple\n");}
			else {printf("\n");}
		}
	}
	//Tuple t = readTuple(r, stdin);
	// match all tuples for now
	//Tuple t;
	//while ((t = readTuple(r,stdin)) != NULL) {
	//Tuple t = readTuple(r, stdin);
	//}
	//showBits(b); putchar('\n');
	//int n0, n1, tot; n0 = n1 = tot = 0;
	//for (int i = 0; i < tsigBits(r); i++) {
	//	if (bitIsSet(b,i)) n1++; else n0++;
	//	tot++;
	//}
	//printf("%d total, %d set, %d zero\n", tot, n1, n0);
	return 0;
}
