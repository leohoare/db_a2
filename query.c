// query.c ... query scan functions
// part of SIMC signature files
// Manage creating and using Query objects
// Written by John Shepherd, September 2018

#include "defs.h"
#include "query.h"
#include "reln.h"
#include "tuple.h"
#include "bits.h"
#include "tsig.h"
#include "psig.h"
#include "bsig.h"
#include <math.h>

// check whether a query is valid for a relation
// e.g. same number of attributes

int checkQuery(Reln r, char *q)
{
	if (*q == '\0') return 0;
	char *c;
	int nattr = 1;
	for (c = q; *c != '\0'; c++)
		if (*c == ',') nattr++;
	return (nattr == nAttrs(r));
}

// take a query string (e.g. "1234,?,abc,?")
// set up a QueryRep object for the scan

Query startQuery(Reln r, char *q, char sigs)
{
	Query new = malloc(sizeof(QueryRep));
	assert(new != NULL);
	if (!checkQuery(r,q)) return NULL;
	new->rel = r;
	new->qstring = q;
	new->nsigs = new->nsigpages = 0;
	new->ntuples = new->ntuppages = new->nfalse = 0;
	new->pages = newBits(nPages(r));
	switch (sigs) {
	case 't': findPagesUsingTupSigs(new); break;
	case 'p': findPagesUsingPageSigs(new); break;
	case 'b': findPagesUsingBitSlices(new); break;
	default:  setAllBits(new->pages); break;
	}
	new->curpage = 0;
	return new;
}

// scan through selected pages (q->pages)
// search for matching tuples and show each
// accumulate query stats

void scanAndDisplayMatchingTuples(Query q, char* file_name)
{
	// written by Leo
	assert(q != NULL);
	for (int pid=0; pid<nPages(q->rel); pid++){
		if (bitIsSet(q->pages,pid)){
			Page p = getPage(dataFile(q->rel), pid);	
			for (int tid = 0; tid<pageNitems(p); tid++){
				// get tuple from page
				Tuple t = getTupleFromPage(q->rel, p, tid);
				// make copy of string
				char qstring_copy[strlen(q->qstring)+1];
				strcpy(qstring_copy,q->qstring);
				// iterate through each qstring && tuple token
				char *t_rest = t, *q_rest = qstring_copy;
				char *t_tok, *q_tok;
				while((t_tok = strtok_r(t_rest, ",", &t_rest)) &&
					 (q_tok = strtok_r(q_rest, ",", &q_rest))){
					// CHECK MATCH HERE
					// ADD IN STATISTICS!!! 
					printf("%s %s \n", t_tok, q_tok);
				}				
			}
		}
		// else ignore
	}	
}

// print statistics on query

void queryStats(Query q)
{
	printf("# signatures read:   %d\n", q->nsigs);
	printf("# sig pages read:    %d\n", q->nsigpages);
	printf("# tuples examined:   %d\n", q->ntuples);
	printf("# data pages read:   %d\n", q->ntuppages);
	printf("# false match pages: %d\n", q->nfalse);
}

// clean up a QueryRep object and associated data

void closeQuery(Query q)
{
	free(q->pages);
	free(q);
}

