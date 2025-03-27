#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
    int id;
    int nrUsi;
    float pret;
    char* model;
    char* numeSofer;
    unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
    Masina info;
    struct Nod* next;
    struct Nod* prev;
};
typedef struct Nod Nod;

struct listaDubla {
    Nod* head;
    Nod* tail;
    int nrNoduri;
};

typedef struct listaDubla listaDubla;

Masina citireMasinaDinFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    fgets(buffer, 100, file);
    char* aux;
    Masina m1;
    aux = strtok(buffer, sep);
    m1.id = atoi(aux);
    m1.nrUsi = atoi(strtok(NULL, sep));
    m1.pret = atof(strtok(NULL, sep));
    aux = strtok(NULL, sep);
    m1.model = malloc(strlen(aux) + 1);
    strcpy(m1.model, aux);  // Folosește strcpy în loc de strcpy_s

    aux = strtok(NULL, sep);
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);  // Folosește strcpy în loc de strcpy_s

    m1.serie = strtok(NULL, sep)[0];  // Corectare pentru a obține caracterul

    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi : %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasiniDeLaFinal(listaDubla listaDubla) {
    Nod* temp = listaDubla.tail;
    while (temp) {
        afisareMasina(temp->info);
        temp = temp->prev;
    }
}

void afisareListaMasiniDeLaInceput(listaDubla listaDubla) {
    Nod* temp = listaDubla.head;
    while (temp) {
        afisareMasina(temp->info);
        temp = temp->next;
    }
}

void adaugaMasinaInListaLaFinal(listaDubla* listaDubla, Masina masinaNoua) {
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->info = masinaNoua;
    nodNou->next = NULL;
    nodNou->prev = listaDubla->tail;

    if (listaDubla->head == NULL) {
        listaDubla->head = nodNou;
    }
    else {
        listaDubla->tail->next = nodNou;
    }

    listaDubla->nrNoduri++;
    listaDubla->tail = nodNou;
}

void adaugaLaInceputInLista(listaDubla* listaDubla, Masina masinaNoua) {
    Nod* nodNou = (Nod*)malloc(sizeof(Nod));
    nodNou->info = masinaNoua;
    nodNou->prev = NULL;
    nodNou->next = listaDubla->head;

    if (listaDubla->head == NULL) {
        listaDubla->tail = nodNou;
    }
    else {
        listaDubla->head->prev = nodNou;
    }

    listaDubla->head = nodNou;
    listaDubla->nrNoduri++;
}

listaDubla citireLDMasiniDinFisier(const char* numeFisier) {
    listaDubla lista;
    lista.head = NULL;
    lista.tail = NULL;
    lista.nrNoduri = 0;
    FILE* f = fopen(numeFisier, "r");

    if (f == NULL) {
        printf("Eroare la deschiderea fisierului %s\n", numeFisier);
        exit(1);
    }

    char buffer[100];
    while (fgets(buffer, sizeof(buffer), f)) {
        adaugaMasinaInListaLaFinal(&lista, citireMasinaDinFisier(f));
    }
    fclose(f);
    return lista;
}

void dezalocareLDMasini(listaDubla* listaDubla) {
    if (listaDubla->head) {
        Nod* temp = listaDubla->head;
        while (temp) {
            Nod* nextNode = temp->next;
            free(temp->info.numeSofer);
            free(temp->info.model);
            free(temp);
            temp = nextNode;
        }
        listaDubla->head = NULL;
        listaDubla->tail = NULL;
        listaDubla->nrNoduri = 0;
    }
}

float calculeazaPretMediu(listaDubla listaDubla) {
    Nod* aux = listaDubla.head;
    float suma = 0;
    while (aux) {
        suma += aux->info.pret;
        aux = aux->next;
    }
    return listaDubla.nrNoduri ? suma / listaDubla.nrNoduri : 0;
}

void stergeMasinaDupaID(listaDubla** lista, int id) {
    if (*lista == NULL || (*lista)->head == NULL) {
        printf("Lista este goala.\n");
        return;
    }

    Nod* temp = (*lista)->head;

    while (temp) {
        if (temp->info.id == id) {
            // Dacă nodul este la începutul listei
            if (temp == (*lista)->head) {
                (*lista)->head = temp->next;
                if ((*lista)->head != NULL) {
                    (*lista)->head->prev = NULL;
                }
            }
            // Dacă nodul este la sfârșitul listei
            else if (temp == (*lista)->tail) {
                (*lista)->tail = temp->prev;
                if ((*lista)->tail != NULL) {
                    (*lista)->tail->next = NULL;
                }
            }
            // Dacă nodul este în mijlocul listei
            else {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
            }

            free(temp->info.model);
            free(temp->info.numeSofer);
            free(temp);

            (*lista)->nrNoduri--; // Reducem numărul de noduri din listă
            return;
        }
        temp = temp->next;
    }

    // Dacă ajungem aici, înseamnă că nu am găsit niciun nod cu ID-ul respectiv
    printf("Masina cu ID-ul %d nu a fost gasita.\n", id);
}


char* getNumeSoferMasinaScumpa(listaDubla lista) {
    Nod* temp = lista.head;
    Nod* maxim = lista.head;
    while (temp) {
        if (temp->info.pret > maxim->info.pret) {
            maxim = temp;
        }
        temp = temp->next;
    }

    char* numeCopiat = (char*)malloc(strlen(maxim->info.numeSofer) + 1);
    strcpy(numeCopiat, maxim->info.numeSofer);  // Folosește strcpy în loc de strcpy_s
    return numeCopiat;
}

int main() {
    listaDubla lista;
    lista = citireLDMasiniDinFisier("masini.txt");
    afisareListaMasiniDeLaFinal(lista);
    printf("\n--------------------------------------\n");
    afisareListaMasiniDeLaInceput(lista);
    printf("\n--------------------------------------\n");
    float pretMediu = 0;
    pretMediu = calculeazaPretMediu(lista);
    printf("Pretul mediu este: %.2f", pretMediu);
    printf("\n--------------------------------------\n");
    char* numeReturnat = getNumeSoferMasinaScumpa(lista);
    printf("Nume returnat: %s", numeReturnat);

    int idDeSters = 4;
    stergeMasinaDupaID(&lista, idDeSters);
    afisareListaMasiniDeLaInceput(lista);

    dezalocareLDMasini(&lista);
    return 0;
}