// test tsig dt

#include <stdio.h>
#include "defs.h"
#include "tuple.h"
#include "tsig.h"
#include "bits.h"
#include "reln.h"
#include "psig.h"

int main(int argc, char **argv)
{
	Reln r = openRelation("R");
	
	for (int pid=0; pid<nPages(r); pid++){
		Page p = getPage(dataFile(r), pid);
		Page tp = getPage(psigFile(r),0);
		for (int tid = 0; tid<pageNitems(p); tid++){
			Tuple t = getTupleFromPage(r, p, tid);
			Bits b = makePageSig(r, t);
			
			printf("\n\n");
			showBits(b);
			printf("\n\n");
			/*	
			int n0, n1, tot; n0 = n1 = tot = 0;
			for (int i = 0; i < tsigBits(r); i++) {
				if (bitIsSet(b,i)) n1++; else n0++;
			tot++;
			}
			printf("%d total, %d set, %d zero\n", tot, n1, n0);
			putBits(tp, tid, b);
			*/
		}
		putPage(psigFile(r),0,tp);
	}
	/*
	Page testp = getPage(psigFile(r),0);
	Bits test = newBits(64);
	getBits(testp, 0, test);
	showBits(test);
	printf("\n");
	getBits(testp, 1, test);
	showBits(test);
	printf("\n");
	getBits(testp, 2, test);
	showBits(test);
	printf("\n");
	getBits(testp, 3, test);
	showBits(test);
	printf("\n");
	getBits(testp, 4, test);
	showBits(test);
	*/
	return 0;
}
