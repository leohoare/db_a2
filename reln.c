// reln.c ... functions on Relations
// part of SIMC signature files
// Written by John Shepherd, September 2018

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "defs.h"
#include "reln.h"
#include "page.h"
#include "tuple.h"
#include "tsig.h"
#include "bits.h"
#include "hash.h"
#include "psig.h"
// open a file with a specified suffix
// - always open for both reading and writing

File openFile(char *name, char *suffix)
{
	char fname[MAXFILENAME];
	sprintf(fname,"%s.%s",name,suffix);
	File f = open(fname,O_RDWR|O_CREAT,0644);
	assert(f >= 0);
	return f;
}

// create a new relation (five files)
// data file has one empty data page

Status newRelation(char *name, Count nattrs, float pF,
                   Count tk, Count tm, Count pm, Count bm)
{
	Reln r = malloc(sizeof(RelnRep));
	RelnParams *p = &(r->params);
	assert(r != NULL);
	p->nattrs = nattrs;
	p->pF = pF,
	p->tupsize = 28 + 7*(nattrs-2);
	Count available = (PAGESIZE-sizeof(Count));
	p->tupPP = available/p->tupsize;
	p->tk = tk; 
	if (tm%8 > 0) tm += 8-(tm%8); // round up to byte size
	p->tm = tm; p->tsigSize = tm/8; p->tsigPP = available/(tm/8);
	if (pm%8 > 0) pm += 8-(pm%8); // round up to byte size
	p->pm = pm; p->psigSize = pm/8; p->psigPP = available/(pm/8);
	if (p->psigPP < 2) { free(r); return -1; }
	if (bm%8 > 0) bm += 8-(bm%8); // round up to byte size
	p->bm = bm; p->bsigSize = bm/8; p->bsigPP = available/(bm/8);
	if (p->bsigPP < 2) { free(r); return -1; }
	r->infof = openFile(name,"info");
	r->dataf = openFile(name,"data");
	r->tsigf = openFile(name,"tsig");
	r->psigf = openFile(name,"psig");
	r->bsigf = openFile(name,"bsig");
	addPage(r->dataf); p->npages = 1; p->ntups = 0;
	addPage(r->tsigf); p->tsigNpages = 1; p->ntsigs = 0;
	addPage(r->psigf); p->psigNpages = 1; p->npsigs = 0;
	addPage(r->bsigf); p->bsigNpages = 1; p->nbsigs = 0; // replace this
	// function to add in all bsig files
	Page bp = getPage(r->bsigf,0);
	//int page_counter = 0;
	for (int bid=0; bid < p->pm; bid++){
	        if (pageNitems(bp) == p->bsigPP) {
	                addPage(r->bsigf);
        	        p->bsigNpages++;
                	free(bp);
                	bp = newPage();
                	if (bp == NULL) return NO_PAGE;
        	}
        	addOneItem(bp);
        	p->nbsigs++;
	}	
	closeRelation(r);
	return 0;
}

// check whether a relation already exists

Bool existsRelation(char *name)
{
	char fname[MAXFILENAME];
	sprintf(fname,"%s.info",name);
	File f = open(fname,O_RDONLY);
	if (f < 0)
		return FALSE;
	else {
		close(f);
		return TRUE;
	}
}

// set up a relation descriptor from relation name
// open files, reads information from rel.info

Reln openRelation(char *name)
{
	Reln r = malloc(sizeof(RelnRep));
	assert(r != NULL);
	r->infof = openFile(name,"info");
	r->dataf = openFile(name,"data");
	r->tsigf = openFile(name,"tsig");
	r->psigf = openFile(name,"psig");
	r->bsigf = openFile(name,"bsig");
	read(r->infof, &(r->params), sizeof(RelnParams));
	return r;
}

// release files and descriptor for an open relation
// copy latest information to .info file
// note: we don't write ChoiceVector since it doesn't change

void closeRelation(Reln r)
{
	// make sure updated global data is put in info file
	lseek(r->infof, 0, SEEK_SET);
	int n = write(r->infof, &(r->params), sizeof(RelnParams));
	assert(n == sizeof(RelnParams));
	close(r->infof); close(r->dataf);
	close(r->tsigf); close(r->psigf); close(r->bsigf);
	free(r);
}

// insert a new tuple into a relation
// returns page where inserted
// returns NO_PAGE if insert fails completely

PageID addToRelation(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL && strlen(t) == tupSize(r));
	Page p;  	PageID pid;
	Page tp; 	PageID tid;
	Page pp; 	PageID ppid;
	Page bp;
	RelnParams *rp = &(r->params);
	
	// add tuple to last page
	pid = rp->npages-1;
	p = getPage(r->dataf, pid);
	// check if room on last page; if not add new page
	if (pageNitems(p) == rp->tupPP) {
		addPage(r->dataf);
		rp->npages++;
		pid++;
		free(p);
		p = newPage();
		if (p == NULL) return NO_PAGE;
	}
	addTupleToPage(r, p, t);
	rp->ntups++;  //written to disk in closeRelation()
	int tupsInPage = pageNitems(p)-1;
	putPage(r->dataf, pid, p);

	// compute tuple signature and add to tsigf
	
	// by Leo
	tid = rp->tsigNpages-1;
	tp = getPage(r->tsigf,tid);	
	if (pageNitems(tp) == rp->tsigPP){
		addPage(r->tsigf);
		rp->tsigNpages++;
		tid++;
		free(tp);
		tp = newPage();
		if (tp == NULL) return NO_PAGE;
	}
	Bits tup_sig = makeTupleSig(r, t);	
	putBits(tp, pageNitems(tp), tup_sig);
	addOneItem(tp);
	rp->ntsigs++;
	putPage(r->tsigf, tid, tp);
	

	// compute page signature and add to psigf
	ppid = rp->psigNpages-1;
	pp = getPage(r->psigf,ppid);
	Bits add_sig = makePageSig(r, t);
	Bits old_sig = newBits(psigBits(r));
	// ADD NEW psig
	// deliberately refers to p instead of pp
	if (tupsInPage == rp->tupPP || tupsInPage == 0 ){
	        // ADD new page
		if (pageNitems(pp) == rp->psigPP){
            		
			addPage(r->psigf);
               		rp->psigNpages++;
               		ppid++;
                	free(pp);
                	pp = newPage();
                	if (pp == NULL) return NO_PAGE;
		// just add new sig (meant to be empty)
		}
		else {
	
		}
		
		rp->npsigs++;
		addOneItem(pp);
	}
	// merge signatures;
	else {
		getBits(pp, pageNitems(pp)-1, old_sig);
		orBits(add_sig,old_sig);
	}
	
	putBits(pp, pageNitems(pp)-1, add_sig);
	// extract page num for bsig	
	int pagenum = ppid * rp->psigPP + pageNitems(pp)-1;
	putPage(r->psigf, ppid, pp);


	// params we have
	// add_sig = page signiture
	// pagenum ppid*rp->psigPP+pageNitems(pp)-1     0,1,2....	

	bp = getPage(r->bsigf,0); // useless decloration but anyway
	for (int bid=0; bid < rp->pm; bid++) {
		if (bitIsSet(add_sig, bid) == 1) {
			bp = getPage(r->bsigf,bid/rp->bsigPP);
			Bits newBsig = newBits(rp->bm);
			getBits(bp, bid%rp->bsigPP, newBsig);
			setBit(newBsig, pagenum);
			putBits(bp, bid%rp->bsigPP, newBsig);
			putPage(r->bsigf, bid/rp->bsigPP, bp);
		}
		
	}

	return nPages(r)-1;
}

// displays info about open Reln (for debugging)

void relationStats(Reln r)
{
	RelnParams *p = &(r->params);
	printf("Global Info:\n");
	printf("Dynamic:\n");
    printf("  #items:  tuples: %d  tsigs: %d  psigs: %d  bsigs: %d\n",
			p->ntups, p->ntsigs, p->npsigs, p->nbsigs);
    printf("  #pages:  tuples: %d  tsigs: %d  psigs: %d  bsigs: %d\n",
			p->npages, p->tsigNpages, p->psigNpages, p->bsigNpages);
	printf("Static:\n");
    printf("  tups   #attrs: %d  size: %d bytes  max/page: %d\n",
			p->nattrs, p->tupsize, p->tupPP);
	printf("  sigs   bits/attr: %d\n", p->tk);
	printf("  tsigs  size: %d bits (%d bytes)  max/page: %d\n",
			p->tm, p->tsigSize, p->tsigPP);
	printf("  psigs  size: %d bits (%d bytes)  max/page: %d\n",
			p->pm, p->psigSize, p->psigPP);
	printf("  bsigs  size: %d bits (%d bytes)  max/page: %d\n",
			p->bm, p->bsigSize, p->bsigPP);
}
