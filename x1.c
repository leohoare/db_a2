// test Bits dt

#include <stdio.h>
#include "defs.h"
#include "reln.h"
#include "tuple.h"
#include "bits.h"
//#include "page.h"

int main(int argc, char **argv)
{
	
	Reln r = openRelation("R");
	Count tsize = 28 + 7*(nAttrs(r)-2);
	printf("%i %i = %i\n",psigBits(r), tsigBits(r),
		tsigBits(r)*((PAGESIZE-sizeof(Count))/(28 + 7 *(nAttrs(r)-2))));	

	printf("%i %i %i \n", psigBits(r)/maxTupsPP(r), psigBits(r)/maxTupsPP(r)/nAttrs(r),codeBits(r));
	printf("%i %i %i\n",tupSize(r),maxTsigsPP(r)*tsigBits(r),tsigBits(r));
	printf("%i %i %i\n",psigBits(r)/8,psigBits(r)/tsize,maxTupsPP(r));
		
	//	File f = dataFile(r);
	/*
	Bits b = newBits(10);
	printf("%i\n",nPages(r));
	for (int pid=0; pid<nPages(r); pid++){
                printf("here: \n");
		Page p = getPage(tsigFile(r), pid);
		getBits(p,0,b);
	}
	*/
	/*

	Bits b = newBits(60);	
	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 0);
	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 1);
	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 2);
	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 3);
	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 4);
	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 5);
	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 10);

	printf("t=0: "); showBits(b); printf("\n");
	if (bitIsSet(b,9)) {printf("hi 9 \n");}
	if (bitIsSet(b,10)) {printf("hi 10\n");}
	if (bitIsSet(b,11)) {printf("hi 11\n");}

	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 5);
	printf("t=1: "); showBits(b); printf("\n");
	setBit(b, 0);
	setBit(b, 50);
	setBit(b, 59);
	printf("t=2: "); showBits(b); printf("\n");
	
	printf("t=5: "); showBits(b); printf("\n");
	Bits c = newBits(60);
	orBits(c, b);
	printf("t=6: "); showBits(c); printf("\n");
	unsetBit(b, 0);
	andBits(c,b);
	printf("t=6: "); showBits(c); printf("\n");
	*/	
return 0;
}
