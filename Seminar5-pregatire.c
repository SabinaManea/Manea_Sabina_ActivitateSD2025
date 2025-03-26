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
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
    Masina info;
    Nod* next;
    Nod* prev;
};

struct ListaDubla {
    Nod* head;
    Nod* tail;
};

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
    strcpy(m1.model, aux);

    aux = strtok(NULL, sep);
    m1.numeSofer = malloc(strlen(aux) + 1);
    strcpy(m1.numeSofer, aux);

    m1.serie = *strtok(NULL, sep);
    return m1;
}

void afisareMasina(Masina masina) {
    printf("Id: %d\n", masina.id);
    printf("Nr. usi: %d\n", masina.nrUsi);
    printf("Pret: %.2f\n", masina.pret);
    printf("Model: %s\n", masina.model);
    printf("Nume sofer: %s\n", masina.numeSofer);
    printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(ListaDubla lista) {
    Nod* temp = lista.head;
    while (temp) {
        afisareMasina(temp->info);
        temp = temp->next;
    }
}

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->next = NULL;

    if (lista->tail == NULL) {
        nou->prev = NULL;
        lista->head = lista->tail = nou;
    }
    else {
        nou->prev = lista->tail;
        lista->tail->next = nou;
        lista->tail = nou;
    }
}

void adaugaLaInceputInLista(ListaDubla* lista, Masina masinaNoua) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = masinaNoua;
    nou->prev = NULL;

    if (lista->head == NULL) {
        nou->next = NULL;
        lista->head = lista->tail = nou;
    }
    else {
        nou->next = lista->head;
        lista->head->prev = nou;
        lista->head = nou;
    }
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
    ListaDubla lista = { NULL, NULL };
    FILE* file = fopen(numeFisier, "r");
    if (!file) return lista;

    while (!feof(file)) {
        Masina m = citireMasinaDinFisier(file);
        adaugaMasinaInLista(&lista, m);
    }
    fclose(file);
    return lista;
}

void dezalocareLDMasini(ListaDubla* lista) {
    Nod* temp;
    while (lista->head) {
        temp = lista->head;
        lista->head = lista->head->next;
        free(temp->info.model);
        free(temp->info.numeSofer);
        free(temp);
    }
    lista->tail = NULL;
}

float calculeazaPretMediu(ListaDubla lista) {
    float suma = 0;
    int count = 0;
    Nod* temp = lista.head;
    while (temp) {
        suma += temp->info.pret;
        count++;
        temp = temp->next;
    }
    return count ? suma / count : 0;
}

void stergeMasinaDupaID(ListaDubla* lista, int id) {
    Nod* temp = lista->head;
    while (temp) {
        if (temp->info.id == id) {
            if (temp->prev) temp->prev->next = temp->next;
            else lista->head = temp->next;

            if (temp->next) temp->next->prev = temp->prev;
            else lista->tail = temp->prev;

            free(temp->info.model);
            free(temp->info.numeSofer);
            free(temp);
            return;
        }
        temp = temp->next;
    }
}

char* getNumeSoferMasinaScumpa(ListaDubla lista) {
    if (!lista.head) return NULL;
    Nod* temp = lista.head;
    Nod* maxNode = lista.head;
    while (temp) {
        if (temp->info.pret > maxNode->info.pret) {
            maxNode = temp;
        }
        temp = temp->next;
    }
    return maxNode->info.numeSofer;
}

int main() {
    ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
    afisareListaMasini(lista);
    printf("Pret mediu: %.2f\n", calculeazaPretMediu(lista));

    stergeMasinaDupaID(&lista, 1);
    printf("Dupa stergere:\n");
    afisareListaMasini(lista);

    printf("Soferul celei mai scumpe masini: %s\n", getNumeSoferMasinaScumpa(lista));
    dezalocareLDMasini(&lista);
    return 0;
}