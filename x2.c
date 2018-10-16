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
		Page tp = getPage(tsigFile(r),0);
		for (int tid = 0; tid<pageNitems(p); tid++){
			Tuple t = getTupleFromPage(r, p, tid);
			Bits b = makeTupleSig(r, t);
			showBits(b);
			int n0, n1, tot; n0 = n1 = tot = 0;
			for (int i = 0; i < tsigBits(r); i++) {
				if (bitIsSet(b,i)) n1++; else n0++;
			tot++;
			}
			printf("%d total, %d set, %d zero\n", tot, n1, n0);
			putBits(tp, tid, b);
		}
		//putPage(tsigFile(r),0,tp);
	}
	
	Page testp = getPage(tsigFile(r),0);
	Bits test = newBits(64);
	getBits(testp, 0, test);
	showBits(test);
	

	//Byte *ptr = addrInPage(tp,0,sizeof(Byte));
		
	//Tuple t = readTuple(r, stdin);
	// match all tuples for now
	//Tuple t;
	//while ((t = readTuple(r,stdin)) != NULL) {
	//Tuple t = readTuple(r, stdin);
	//}
	//showBits(b); putchar('\n');
	return 0;
}
