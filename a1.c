/*
	Name:Nick Hodge(Hodgen1r)	Class: CPS470
	Section: 22377311		Assignment: 01
	Due: 1/30/20			Points: 10

	Description: Creates an amount (specified at execution) of random
	numbers. The random numbers are added to a list. If the same
	number is entered a second time the frequency of the number is
	incremented and that number is pushed forward in the list one
	position. Once all numbers are added to the list the list is
	printed to stdout and then the storage for the list is freed. 

 */

#include <stdio.h>
#include <stdlib.h>
#include<time.h>

#define POOLSZ 53
#define LINE_SIZE 72

typedef struct solnode{
	int eventnr;
	int eventfq;
	struct solnode *next;
} solnode;

solnode pool[POOLSZ];
solnode *avail;

int main(int argc, char **argv) {

	int size;
	int i = 0;
	int random;
	int check;
	solnode *list = NULL;
	solnode *current;
	solnode *previous;
	solnode *pprevious;

	void adjustlist(solnode **list, solnode *pprevious, solnode
	*previous, solnode *current);
	void displayevents(solnode *list);
	void getnode(solnode **prt);
	void freelist(solnode **list);
	void freenode(solnode **tofree);	
	void initpool(void);	
	int insertevent(solnode **list, int event);
	int nextevent(void);
	void srchevent(solnode *list, int xevent, solnode **current,
	solnode **previous, solnode **pprevious);
	void usage(char *progname);	

	if (argc < 2 || argc > 2) {
		usage("a1");
	}
	
	size = strtol(argv[1], NULL, 10);
	srand(time(NULL));
	initpool();

	for (i = 0; i < size; i++) {
		random = nextevent();
		srchevent(list, random, &current, &previous, &pprevious);
		if (current == NULL) {
			check = insertevent(&list, random);			
			if (check == 0) {
				fprintf(stderr, "Error on insert");
			}
		}
		else {
			current -> eventfq++;
			adjustlist(&list, pprevious, previous, current);
		}
	}
	
	displayevents(list);	
	freelist(&list);

	exit(0);	
}

/* Prints usage to stderr on command line errors and terminate */
void usage(char *progname) {

	fprintf(stderr, "usage: ./%s <event-count>\n", progname);
	exit(1);
}

/* Initializes a global pool of nodes with avail
	pointing to the first available node.
*/
void initpool(void) {
	
	int i;

	for(i = 0; i < POOLSZ-1; i++)
		pool[i].next = &pool[i+1];
	pool[i].next = NULL;
	avail = pool;
}

/* Gets a node from global pool, the pointer 
	to new node is NULL when no more nodes
	are available.
*/
void getnode(solnode **ptr) {

	*ptr = NULL;
	if(avail != NULL) {
		*ptr = avail;
		avail = avail -> next;
	}
}

/* Returns the node pointed by ptr to the available
	pool. Ptr is NULL after the operation.
*/
void freenode(solnode **ptr) {

	if(*ptr != NULL) {
		(*ptr)->next = avail;
		avail = *ptr;
		*ptr = NULL;
	}
}

/* Returns next event generated from rand(). 
	Random is seeded in main() with srand()
*/
int nextevent(void) {

	int random;
	random = (rand() % (53) + 1);
	return random;
}

/* Insert event at the tail of the list.
	Returns 0 on failure, 1 on success
*/
int insertevent(solnode **list, int event) {
	
	solnode *node;
	solnode *current = *list;
	getnode(&node);
	if (node == NULL) {
		return 0;
	}
	node -> eventnr = event;
	node -> eventfq = 1;
	node -> next = NULL;
	
	if(!current) {
		*list = node;
		return 1;
	}
	while(current -> next != NULL) {
		current = current -> next;
	}
	current -> next = node;

	return 1; 
}

/*
 Searches the list for xevent, current points to node containing
	xevent which is undefinded if the search failed.
	Previous always points to the predecessor of current.
	It returns current and previous to the caller.
*/
void srchevent(solnode *list, int xevent, solnode **current, solnode
**previous, solnode **pprevious) {

	*current = list;
	*previous = NULL;
	*pprevious = NULL;

	while (*current != NULL && (*current) -> eventnr != xevent) {
		*pprevious = *previous;
		*previous = *current;
		*current = (*current) -> next;
	}
}

/* Transposes current one position towards front of list.
	Only done if current is not already the front of list.
*/
void adjustlist(solnode **list, solnode *pprevious, solnode *previous,
solnode *current) {

	solnode *node;
	
	if (!previous) {
		*list = current;
		return;	
	}
	else {
		if (!pprevious) {
			node = current -> next;
			previous -> next = node;
			current -> next = previous;
			*list = current;
			return;		
		}
	}
	node = current -> next;
	previous -> next = node;
	current -> next = previous;
	pprevious -> next = current;
}

/* Prints the list contents to stdout as pair with event:frequency.
*/
void displayevents(solnode *list) {

	char line[LINE_SIZE + 1];
	char node[1024];
	int remaining = LINE_SIZE;
	int node_printed = 0;

	line[0] = '\0';
	while (list) {
		node_printed = snprintf(node, 1024, " %d : %d", list -> eventnr, list -> eventfq);
		if (node_printed > remaining + 1) {
			printf("\n");
			remaining = LINE_SIZE - sprintf(line, "%s", node);
		}
		else {
			if (remaining != LINE_SIZE) {
				remaining -= sprintf(line, ",%s", node);
			}
			remaining -= snprintf(line + LINE_SIZE - remaining, 1024, "%s", node);
		}
		list = list -> next;
		printf("%s", line);
	}
}

/* Returns the storage for list back to the system, one node at a time.
	Uses freenode(), list is empty after this call.
*/
void freelist(solnode **list) {
	
	freenode(list);
}
