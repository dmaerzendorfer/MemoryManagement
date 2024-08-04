#include <stdio.h>
#include <stdlib.h>
#include "mym.h"

#define SIZE 40

void testMyMemory(strategy s)
{
    mheap *h = myminit(40, s);
    void *gap1 = mymmalloc(h, 2);
    void *p104 = mymmalloc(h, 2);
    void *gap2 = mymmalloc(h, 1);
    void *p102 = mymmalloc(h, 3);
    void *p100 = mymmalloc(h, 1);
    void *gap3 = mymmalloc(h, 7);
    void *p106 = mymmalloc(h, 5);
    void *gap4 = mymmalloc(h, 2);
    void *p101 = mymmalloc(h, 1);
    void *gap5 = mymmalloc(h, 6);
    void *p103 = mymmalloc(h, 3);
    void *gap6 = mymmalloc(h, 3);
    void *p105 = mymmalloc(h, 2);

    mymfree(h, gap1);
    mymfree(h, gap2);
    mymfree(h, gap3);
    mymfree(h, gap4);
    mymfree(h, gap5);
    mymfree(h, gap6);

    mymfree(h, p100);
    void *p1 = mymmalloc(h, 4);
    void *p2 = mymmalloc(h, 2);
    void *p5 = p2;
    mymfree(h, p1);
    p1 = mymmalloc(h, 5);
    p2 = NULL;
    mymfree(h, p101);
    void *p3 = mymmalloc(h, 1);
    void *p4 = mymmalloc(h, 3);
    mymfree(h, p3);
    p3 = mymmalloc(h, 2);
    mymfree(h, p4);
    p4 = mymmalloc(h, 5);
    mymfree(h, p5);
    p5 = mymmalloc(h, 3);

    mymdump(h);
    mymterm(h);
}

mheap *init_starting_point(mheap *heap);

int main(int argc, char *argv[])
{
    //FirstFit
    printf("FirstFit:\n");
    testMyMemory(FirstFit);

    //NextFit
    printf("NextFit:\n");
    testMyMemory(NextFit);

    //BestFit
    printf("BestFit:\n");
    testMyMemory(BestFit);

    //user interface
    int input = 0;
    mheap *new_heap=NULL;
    int *mem_adresses[SIZE];
    int block_size = 0, adress_number, i = 0, strat;

    while (1)
    {
        printf("\nHeap-Simulation...\n\n\t (1) init\n\t (2) malloc\n\t (3) free\n\t (4) dump\n\t (5) load starting point\n\t (6) exit\n");
        printf("\nInput: ");
        fflush(stdin);
        scanf("%i", &input);
        system("cls");

        switch (input)
        {
        case 1:
            printf("\nStrategies...\n\n\t (0) FirstFit\n\t (1) NextFit\n\t (2) BestFit\n\t");
            printf("input: ");
            scanf("%i", &strat);
            new_heap = myminit(SIZE * sizeof(int), strat);
            break;
        case 2:
            printf("input: ");
            scanf("%i", &block_size);
            mem_adresses[i] = mymmalloc(new_heap, block_size);
            break;
        case 3:
            printf("input: ");
            scanf("%i", &adress_number);
            mymfree(new_heap, mem_adresses[adress_number]);
            break;
        case 4:
            mymdump(new_heap);
            break;
        case 5:
            new_heap = init_starting_point(new_heap);
        case 6:
            mymterm(new_heap);
            exit(0);
        default:
            printf("Invalid input!");
            break;
        }
        i++;
    }
    return 0;
}

mheap *init_starting_point(mheap * h){
    if(h==NULL)
        return NULL;
    //h = myminit(SIZE , BestFit);
    void *gap1 = mymmalloc(h, 2);
    mymmalloc(h, 2);
    void *gap2 = mymmalloc(h, 1);
    mymmalloc(h, 4);
    mymmalloc(h, 1);
    void *gap3 = mymmalloc(h, 7);
    mymmalloc(h, 5);
    void *gap4 = mymmalloc(h, 2);
    mymmalloc(h, 1);
    void *gap5 = mymmalloc(h, 6);
    mymmalloc(h, 3);
    void *gap6 = mymmalloc(h, 3);
    mymmalloc(h, 2);

    mymfree(h,gap1);
    mymfree(h,gap2);
    mymfree(h,gap3);
    mymfree(h,gap4);
    mymfree(h,gap5);
    mymfree(h,gap6);

    printf("Ausgangssituation: \n\n");
    mymdump(h);

    return h;
}

