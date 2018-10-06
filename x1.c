// test Bits dt

#include <stdio.h>
#include "defs.h"
#include "reln.h"
#include "tuple.h"
#include "bits.h"

int main(int argc, char **argv)
{
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
/*
	printf("t=0: "); showBits(b); printf("\n");
	setBit(b, 5);
	printf("t=1: "); showBits(b); printf("\n");
	setBit(b, 0);
	setBit(b, 50);
	setBit(b, 59);
	printf("t=2: "); showBits(b); printf("\n");
	if (bitIsSet(b,5)) printf("Bit 5 is set\n");
	if (bitIsSet(b,10)) printf("first Bit 10 is set\n");	
	
	if (bitIsSet(b,10)) printf("first Bit 10 is set\n");	
	setBit(b, 59);

	setBit(b, 9);
	
	if (bitIsSet(b,11)) printf("Bit 11 is set\n");	
	printf("t=2: "); showBits(b); printf("\n");
	setAllBits(b);
	printf("t=3: "); showBits(b); printf("\n");
	unsetBit(b, 40);
	printf("t=4: "); showBits(b); printf("\n");
	if (bitIsSet(b,20)) printf("Bit 20 is set\n");
	if (bitIsSet(b,40)) printf("Bit 40 is set\n");
	setBit(b, 59);

	setBit(b, 9);
	
	if (bitIsSet(b,10)) printf("Bit 10 is set\n");
	unsetAllBits(b);
	printf("t=5: "); showBits(b); printf("\n");
	Bits c = newBits(60);
	orBits(c, b);
	printf("t=5: "); showBits(c); printf("\n");
*/
return 0;
}
