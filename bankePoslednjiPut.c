/*
U fajlu se nalaze podaci o uplatama novca.
ImeBanke, ImeOsobe PrezimeOsobe, novac, grad
Raiffeisen,Stefan Burgic,7000,Krusevac
Intesa,Stefan Burgic,7000,Beograd
CreditAgricole,Stefan Burgic,2000,Kragujevac
Raiffeisen,Stefan Burgic,5000,Krusevac
Intesa,Stefan Burgic,5000,Krusevac
obratite paznju da se u zadnjem redu nakon imena Krusevac nalazi razmak


1.Ucitati iz fajla u :
  a)Nesortiranu listu
  b)Sortiranu listu po imenu banke
2.Ispis liste
3.Ispis svih gradova u kome je ukupno uplaceno vise od 10 000 (duplikati !!! )

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Racun{
    char imeBanke[30];
    char punoImeOsobe[30];
    int novac;
    char imeGrada[20];
}Racun;

typedef struct Lista{
    Racun r;
    struct Lista *next;
}Lista;

int ispisiMeni(){
    char op[10];
    int opcija = 0;
    printf("MENI: \n");
    printf("1: Ucitaj iz fajla (sortirano)\n");
    printf("2: Ispisi listu\n");
    printf("3: Ispisi sve gradove koji zadovoljavaju uslov\n");
    printf("4: Izbrisi sve racune sa manje od 6000\n");
    printf("5: Izbrisi listu i prekini izvrsavanje programa\n");
    while(opcija < 1 || opcija > 5){
        printf("Izaberite opciju: ");
        fgets(op, 10, stdin);
        op[strlen(op) - 1] = 0;
        opcija = atoi(op);
    }
    return opcija;
}

Lista* dodajSortirano(Lista *lista, Lista *novi){
    if(!lista){
        return novi;
    }
    if(strcmp(lista->r.imeBanke, novi->r.imeBanke) > 0){
        novi->next = lista;
        return novi;
    }
    lista->next = dodajSortirano(lista->next, novi);
    return lista;
}

Lista* ucitajIzFajlaSortirano(){
    char naziv[128], string[90], *token;
    FILE *f;
    Lista *pocetak = NULL;
    printf("Unesite ime datoteke: ");
    fgets(naziv, 128, stdin);
    naziv[strlen(naziv) - 1] = 0;
    if(f = fopen(naziv, "r")){
        while(fgets(string, 89, f) == string){
            Lista *noviElement = (Lista*) malloc(sizeof(Lista));
            token = strtok(string, ",");
            strcpy(noviElement->r.imeBanke, token);
            token = strtok(NULL, ",");
            strcpy(noviElement->r.punoImeOsobe, token);
            token = strtok(NULL, ",");
            noviElement->r.novac = atoi(token);
            token = strtok(NULL, ", \n");
            strcpy(noviElement->r.imeGrada, token);
            noviElement->next = NULL;
            pocetak = dodajSortirano(pocetak, noviElement);
        }
        fclose(f);
        printf("Datoteka je uspesno ucitana\n");
    }else{
        printf("Datoteka ne postoji ili je nije moguce ucitati\n");
    }
    return pocetak;
}

void ispisiListu(Lista *lista){
    if(!lista){
        return;
    }else{
        printf("%31s|%31s|%10d|%21s\n", lista->r.imeBanke, lista->r.punoImeOsobe, lista->r.novac, lista->r.imeGrada);
        ispisiListu(lista->next);
    }
}

typedef struct LG{
    char imeGrada[20];
    int novac;
    struct LG *next;
}LG;

void ispisiSveGradoveKojiZadovoljavajuUslov(Lista *lista){
    LG *pocetak = NULL, *tekuci;
    Lista *pocListe = lista;
    while(pocListe){
        int f = 0;
        for(tekuci = pocetak; !f && tekuci; tekuci = tekuci->next){
            if(strcmp(pocListe->r.imeGrada, tekuci->imeGrada) == 0){
                f = 1;
            }
        }
        if(!f){
            LG *novi = (LG*) malloc(sizeof(LG));
            strcpy(novi->imeGrada, pocListe->r.imeGrada);
            novi->novac = 0;
            novi->next = pocetak;
            pocetak = novi;
        }
        pocListe = pocListe->next;
    }
    LG *t = pocetak;
    while(t){
        Lista *l = lista;
        while(l){
            if(strcmp(l->r.imeGrada, t->imeGrada) == 0){
                t->novac += l->r.novac;
            }
            l = l->next;
        }
        t = t->next;
    }
    while(pocetak){
        if(pocetak->novac > 10000){
            printf("%21s|%10d\n", pocetak->imeGrada, pocetak->novac);
        }
        LG *trenutni = pocetak->next;
        free(pocetak);
        pocetak = trenutni;
    }
}

Lista* brisiRacune(Lista *lista){
    char no[10];
    int novac;
    Lista *pocetak = lista;
    printf("Unesite zeljenu svotu novca: ");
    fgets(no, 10, stdin);
    no[strlen(no) - 1] = 0;
    novac = atoi(no);
    if(lista){
        while(pocetak && pocetak->r.novac < novac){
            lista = pocetak->next;
            free(pocetak);
            pocetak = lista;
        }
        while(lista && lista->next){
            if(lista->next->r.novac < novac){
                Lista *tek = lista->next->next;
                free(lista->next);
                lista->next = tek;
            }else{
                lista = lista->next;
            }
        }
    }
    return pocetak;
}

Lista* brisiListu(Lista *lista){
    if(!lista){
        return NULL;
    }else{
        Lista *t = lista->next;
        free(lista);
        brisiListu(t);
    }
}

int main(){
    Lista *racuni = NULL;
    int opcija;
    while(1){
        opcija = ispisiMeni();
        switch(opcija){
            case 1:{
                racuni = brisiListu(racuni);
                racuni = ucitajIzFajlaSortirano();
                break;
            }
            case 2:{
                ispisiListu(racuni);
                break;
            }
            case 3:{
                ispisiSveGradoveKojiZadovoljavajuUslov(racuni);
                break;
            }
            case 4:{
                racuni = brisiRacune(racuni);
                break;
            }
            case 5:{
                racuni = brisiListu(racuni);
                return 0;
            }
        }
    }
}
