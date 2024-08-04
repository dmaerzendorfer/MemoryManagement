#include <stdio.h>
#include <stdlib.h>


//-----------------
//List-stuff
//-----------------
struct DLLNode
{
    void *Data;
    struct DLLNode *Next;
    struct DLLNode *Prev;
};
typedef struct DLLNode Node;

struct DLL_Header
{
    int Len;
    Node *First, *Last;
};
typedef struct DLL_Header List;

extern List *createList(void);
extern List *insertFirst(List *, void *);
extern List *insertLast(List *, void *);
extern void *deleteFirst(List *);
extern void *deleteLast(List *);
extern void pr_List(List*, FILE*);
extern void rm_list(List*);
extern void *get_entry(List*, int);
extern void *get_Node(List*, int);
extern void deleteNode(List* l, Node* n);

//-----------------
//my-memory stuff
//------------------
typedef enum state
{
    used,
    open
} state;
typedef enum strategy
{
    FirstFit,
    NextFit,
    BestFit/*,
    WorstFit*/
} strategy;

typedef struct mheap
{
    void *masterptr;
    List *blocks;
    strategy strat;

} mheap;
typedef struct mblock
{
    state s;
    int offset;
    long size;
} mblock;


mheap *myminit(int, strategy);
void mymterm(mheap*);
void *mymmalloc(mheap*,int);
void mymfree(mheap*,void*);
void mymdump(mheap*);
//merges neighboring blocks
void mblockcleanup(mheap*);
