This is an old assignment from my Bachelor studies. Its not that fancy but I enjoyed coding it so I made it public :)


Dokumentation für mym.(c|h)

    Diese Library stellt Code zur Verfügung welcher es erlaubt
    eine Speicherverwaltung in den Strategien FirstFit, BestFit und NextFit
    nachzustellen.

    Es werden dabei folgende Funktionen zur verfügung gestellt:
    >mheap *myminit(int size, strategy s)
        params:
            -size: gibt an wie groß der heap sein soll
            -s: ist ein strategy enum, gibt an welche strategy verwendet wird (BestFit, FirstFit, NextFit)
        myminit: initialisiert einene neuen Heap mit den Parametern und gibt einen pointer darauf zurück
        returns: mheap*

    >void mymterm(mheap *h)
        params:
            -h: ein pointer auf den heap der terminiert werden soll
        mymterm: bekommt einen heap mitgegeben und terminiert/löscht diesen
        returns: void

    >void *mymmalloc(mheap *h, int size)
        params:
            -h: ein heap pointer aus welchem eine size allokiert wird
            -size: ein int welcher angibt wie viel speicher allokiert werden soll
        mymalloc: allokiert die angegebene anzahl an speicher vom heap und liefert einen pointer zu dem speicher zurück
        returns: void* -->einen pointer auf den speicher (typenlos)
    >void mymfree(mheap *h, void *ptr)
        params:
            -h: der zugehörige heap speicher des pointers
            -ptr: ein pointer welcher im mheap* h liegt und freigegeben werden soll
        mymfree: gibt den speicher des ptr pointers im mheap* h frei
        returns: void

    >void mymdump(mheap *h)
        params:
            -h: ein pointer auf den heap der ausgeeben werden soll
        mymdump: gibt die blöcke eines heaps auf stdout aus
        returns: void
        

    >void mblockcleanup(mheap *h)
        params:
            -h: ein heap pointer dessen nebeneinander liegende lücken zusammengefasst werden sollen
        mblockcleanup: diese function fasst nebeneindander liegende lücken im speicher des mheap* h zu einer lücke zusammen
        returns: void

Demo-Beispiel:
--------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "mym.h"
int main(int argc, char *argv[])
{
    mheap* h=myminit(40*sizeof(int),FirstFit);
    int* p=(int*)mymalloc(h,sizeof(int));
    int* p2=(int*)mymalloc(h,sizeof(int));

    mymdump(h);
    mymfree(h,p2);
    mymdump(h);

    mymterm(h);

}
--------------------------------------

Einbindung in ein Project:
1) mym.c und mym.h in den project folder kopieren
2) #include "mym.h" zum gewünschten file hinzufügen

Kompilieren des Quelltextes:

zB: "gcc -Wall .\mymtest.c .\mym.h .\mym.c -g -o MyMemory"
