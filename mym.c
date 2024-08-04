#include "mym.h"

//--------------------------
// memory implementation
//--------------------------
static int *seperateFreeBlock(int size, Node *curr, mheap *h)
{
    mblock *d = (mblock *)curr->Data;
    //create new block/node
    Node *n = (Node *)calloc(1, sizeof(Node));
    mblock *b = (mblock *)calloc(1, sizeof(mblock));
    if (!n || !b)
        return NULL;
    //b is the block with the remaining memory
    //we set b to be open withe the remaining size and the correct offset
    b->s = open;
    b->size = d->size - size;
    b->offset = d->offset + size;
    //n is the node in the block list of the heap
    n->Data = b;
    // d has just been allocated
    d->size = size;
    d->s = used;
    //now the have to rearange the block list of the heap
    //basically-->n is next of curr, if curr was last n is last now (also set prev etc)
    if (h->blocks->Last == curr)
    {
        curr->Next = n;
        n->Prev = curr;
        h->blocks->Last = n;
    }
    else
    {
        //in case curr was not last we also have to set the next of n to be currs old next ect
        n->Next = curr->Next;
        curr->Next->Prev = n;
        curr->Next = n;
        n->Prev = curr;
    }
    //finally we increase the list len by one
    h->blocks->Len++;

    //clean up the heap-->method runs through entire list
    //could also have just checked if the block after n is also empty -->if so add the size of n etc to it and not make new node and then merge(mblockcleanup) them
    mblockcleanup(h);

    //return the ptr
    return h->masterptr + d->offset;
}

mheap *myminit(int size, strategy s)
{
    mheap *h;
    //allocate memory for the heap struct
    h = (mheap *)calloc(1, sizeof(mheap));
    //check if it worked
    if (h == NULL)
        return NULL;
    //alloc memory for our master ptr
    h->masterptr = malloc(size);
    //check if worked
    if (h->masterptr == NULL)
        return NULL;
    //set the strat
    h->strat = s;
    //add one node into the list
    mblock *b = (mblock *)calloc(1, sizeof(mblock));
    if (b == NULL)
        return NULL;
    b->s = open;
    b->offset = 0;
    b->size = size;
    h->blocks = createList();
    insertFirst(h->blocks, b);
    return h;
}

void mymterm(mheap *h)
{
    //check if heap is not null
    if (h == NULL)
        return;
    //free the list of blocks
    rm_list(h->blocks);
    //free the master ptr
    free(h->masterptr);
    //free the heap struct
    free(h);
}
static void *nextFitFunc(mheap *h, int size)
{
    //run through list and find the right block
    //cnt is for remembering where we left off last time
    static int cnt = 1;
    //check if the cnt is over the list len-->due to freeing stuff etc
    if (cnt > h->blocks->Len)
    {
        cnt = h->blocks->Len;
    }
    //save to remember where we started
    int save = cnt;
    Node *curr = get_Node(h->blocks, cnt);
    mblock *d;
    //again bool for the extra lap when we get over the last
    int again = 1;
    if (curr == h->blocks->First)
        again = 0;
    while (curr != NULL)
    {
        d = (mblock *)curr->Data;
        if (size == d->size && d->s == open)
        {
            //found perfect block
            //cange to used and return the masterptr+the offset
            d->s = used;
            return h->masterptr + d->offset;
        }
        if (size < d->size && d->s == open)
        {
            //found a fitting block
            return seperateFreeBlock(size, curr, h);
        }

        curr = curr->Next;
        //if we got to the last element
        if (curr == NULL && again == 1)
        {
            again = 0;
            curr = h->blocks->First;
            cnt = 1;
        }
        else if (curr != NULL)
        {
            cnt++;
        }
        //we ran trough list and didnt find anything-->break and return NULL
        else if (save == cnt)
        {
            break;
        }
    }
    printf("No enough memory!\n");
    return NULL;
}
static void *firstFitFunc(mheap *h, int size)
{
    //run through list and find the right block
    Node *curr = h->blocks->First;
    mblock *d;
    while (curr != NULL)
    {
        d = (mblock *)curr->Data;
        if (size == d->size && d->s == open)
        {
            //found perfect block
            //cange to used and return the masterptr+the offset
            d->s = used;
            return h->masterptr + d->offset;
        }
        if (size < d->size && d->s == open)
        {
            //found a fitting block
            return seperateFreeBlock(size, curr, h);
        }

        curr = curr->Next;
    }
    printf("No enough memory!\n");
    return NULL;
}
void *bestFitFunc(mheap *h, int size)
{
    mblock *d;
    Node *best = NULL;
    for (Node *curr = h->blocks->First; curr != NULL; curr = curr->Next)
    {
        d = (mblock *)curr->Data;
        //if the current data is used we can skip it
        if (d->s == used)
        {
            continue;
        }
        //if the current size is less than we search for we skip it
        if (d->size < size)
        {
            continue;
        }
        //if perfect fit-->we are pretty much done
        if (d->size == size && d->s == open)
        {
            d->s = used;
            return h->masterptr + d->offset;
        }
        //for the first time we find a spot
        if (best == NULL && d->s == open && d->size > size)
        {
            best = curr;
        }
        //if we find a better one
        else if (((mblock *)best->Data)->size > d->size && d->s == open && d->size > size)
        {
            best = curr;
        }
    }
    //if we found a block we can alloc
    if (best != NULL)
    {
        return seperateFreeBlock(size, best, h);
    }
    printf("No enough memory!\n");
    return NULL;
}

void *mymmalloc(mheap *h, int size)
{
    //check if heap is not null and if size is proper
    if (h == NULL || size <= 0)
        return NULL;

    //depends on strat-->switch
    switch (h->strat)
    {
    case FirstFit:
        return firstFitFunc(h, size);
        break;
    case NextFit:
        return nextFitFunc(h, size);
        break;

    case BestFit:
        return bestFitFunc(h, size);
        break;

        // case WorstFit:
        //     return NULL;
        //     break;

    default:
        return NULL;
        break;
    }
}
void mymfree(mheap *h, void *ptr)
{
    //check if the pointers are not null
    if (h == NULL || ptr == NULL)
        return;
    //first find out to what address the ptr is pointing
    int target = ptr - h->masterptr;
    //target is now an offset of the masterptr

    Node *curr = h->blocks->First;
    mblock *d;
    while (curr != NULL)
    {
        d = (mblock *)curr->Data;

        if (d->offset == target)
        {
            //found it, now change it to open
            d->s = open;
            mblockcleanup(h);
            //!!!!!!!!
            //cant set the ptr to a null-ptr since i would need a double ptr, not sure if we want that!!!
            //!!!!!!!!
            return;
        }
        curr = curr->Next;
    }
}

void mblockcleanup(mheap *h)
{
    //this funciton merges any open neighbouring blocks in our list
    mblock *d;
    for (Node *curr = h->blocks->First; curr != NULL; curr = curr->Next)
    {
        d = (mblock *)curr->Data;
        if (d->s == open)
        {
            Node *n = curr->Next;
            Node *tmp;
            mblock *m;
            while (n != NULL)
            {
                m = (mblock *)n->Data;
                //check if open
                if (m->s == open)
                {
                    //if open add size to d then delete it
                    d->size += m->size;
                    tmp = n->Next;
                    //delete n-->deleteNode also sets the next/prev of the nodes
                    deleteNode(h->blocks, n);
                    n = tmp;
                }
                else
                {
                    //if not open exit while
                    break;
                }
            }
        }
    }
}

void mymdump(mheap *h)
{
    //check if heap is null
    if (h == NULL)
        return;
    //iterate list and print it in a nice way :)
    Node *curr = h->blocks->First;
    mblock *d;
    printf("--------------------\n");
    while (curr != NULL)
    {
        d = (mblock *)curr->Data;
        printf("offset: %d\nsize: %ld\nstate: %s\n", d->offset, d->size, d->s == 0 ? "used" : "open");
        printf("--------------------\n");
        curr = curr->Next;
    }
}

//-------------------
//List implementation
//-------------------

List *createList()
{
    List *l = calloc(1, sizeof(List));
    if (l != NULL)
        l->Len = 0;
    return l;
}

List *insertFirst(List *l, void *val)
{
    if (l == NULL)
        return l;
    Node *n;
    n = calloc(1, sizeof(Node));
    if (n == NULL)
        return l;
    n->Data = val;
    if (l->Len == 0)
    {
        l->Last = n;
        l->First = n;
        l->Len++;
        return l;
    }
    l->First->Prev = n;
    n->Next = l->First;
    l->First = n;
    l->Len++;
    return l;
}
List *insertLast(List *l, void *val)
{
    if (l == NULL)
        return l;

    Node *n;
    n = calloc(1, sizeof(Node));
    if (n == NULL)
        return l;
    n->Data = val;
    if (l->Len == 0)
    {
        l->Last = n;
        l->First = n;
        l->Len++;
        return l;
    }
    l->Last->Next = n;
    n->Prev = l->Last;
    l->Last = n;
    l->Len++;
    return l;
}
void *deleteFirst(List *l)
{
    if (l == NULL)
        return l;
    if (l->Len == 0)
        return NULL;

    void *val;
    val = l->First->Data;

    if (l->Len == 1)
    {
        free(l->First);
        l->First = NULL;
        l->Last = NULL;
        l->Len--;
    }
    else
    {
        l->First = l->First->Next;
        free(l->First->Prev);
        l->First->Prev = NULL;
        l->Len--;
    }
    return val;
}
void *deleteLast(List *l)
{
    if (l == NULL)
        return l;
    if (l->Len == 0)
        return l;

    void *val;
    val = l->Last->Data;

    if (l->Len == 1)
    {
        free(l->Last);
        l->First = NULL;
        l->Last = NULL;
        l->Len--;
    }
    else
    {
        l->Last = l->Last->Prev;
        free(l->Last->Next);
        l->Last->Next = NULL;
        l->Len--;
    }
    return val;
}

void pr_List(List *l, FILE *f)
{
    if (l == NULL || f == NULL)
        return;
    Node *n;
    n = l->First;
    while (n != NULL)
    {
        fprintf(f, "%s\n", (char *)n->Data);
        n = n->Next;
    }
}
void rm_list(List *l)
{
    if (l == NULL)
        return;
    Node *n;
    if (l->Len == 0)
    {
        free(l);
    }
    n = l->Last;
    while (n->Prev != NULL)
    {
        n = n->Prev;
        //do i need to free the data?
        free(n->Data);
        //or does free next suffice?
        free(n->Next);
    }
    free(n->Data);
    free(n);
    free(l);
}
void *get_entry(List *l, int i)
{
    if (l == NULL)
        return l;
    if (i <= 0)
        return NULL;

    Node *n = l->First;
    for (int j = 1; j < i; j++)
    {
        n = n->Next;
    }
    return n->Data;
}
void *get_Node(List *l, int i)
{
    if (l == NULL)
        return l;
    if (i <= 0)
        return NULL;

    Node *n = l->First;
    for (int j = 1; j < i; j++)
    {
        n = n->Next;
    }
    return n;
}

void deleteNode(List *l, Node *n)
{
    if (l->Len == 0 || l == NULL || n == NULL)
    {
        return;
    }
    if (l->Len == 1)
    {
        l->Len--;
        free(n);
        l->First = NULL;
        l->Last = NULL;
    }
    if (l->Last == n)
    {
        l->Last = n->Prev;
        l->Len--;
        free(n);
    }
    else if (l->First == n)
    {
        l->First = n->Next;
        l->Len--;

        free(n);
    }
    else
    {
        n->Prev->Next = n->Next;
        n->Next->Prev = n->Prev;
        l->Len--;

        free(n);
    }
}
