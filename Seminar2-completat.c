#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<string.h>

struct Masina {
    int id;
    char* model;
    float tonaj;
    char serie;
};

struct Masina initializare(int id, const char* model, float tonaj, char serie) {
    struct Masina m;
    m.id = id;
    m.model = (char*)malloc(strlen(model) + 1);
    strcpy(m.model, model);
    m.tonaj = tonaj;
    m.serie = serie;

    return m;
}

void afisare(struct Masina m) {
    printf("Id: %d \n", m.id);
    printf("Model: %s \n", m.model);
    printf("Tonaj: %f \n", m.tonaj);
    printf("Serie: %c \n", m.serie);
    printf("----------------------------\n");
}

void afisareVector(struct Masina* vector, int nrElemente) {
    for (int i = 0; i < nrElemente; i++) {
        afisare(vector[i]);
    }
}

struct Masina* copiazaPrimeleMasini(struct Masina* vector, int nrElemente, int nrElementeCopiate) {
    struct Masina* vectorNou = (struct Masina*)malloc(sizeof(struct Masina) * nrElementeCopiate);
    for (int i = 0; i < nrElementeCopiate; i++) {
        vectorNou[i] = vector[i];
        vectorNou[i].model = (char*)malloc(strlen(vector[i].model) + 1);
        strcpy(vectorNou[i].model, vector[i].model);
    }
    return vectorNou;
}

void dezalocare(struct Masina** vector, int* nrElemente) {
    for (int i = 0; i < *nrElemente; i++) {
        free((*vector)[i].model);
    }
    free(*vector);
    *vector = NULL;
    *nrElemente = 0;
}

void copiazaMasiniUsoare(struct Masina* vector, int nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
    *dimensiune = 0;
    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].tonaj < prag) {
            (*dimensiune)++;
        }
    }

    (*vectorNou) = (struct Masina*)malloc(sizeof(struct Masina) * (*dimensiune));
    int k = 0;
    for (int i = 0; i < nrElemente; i++) {
        if (vector[i].tonaj < prag) {
            (*vectorNou)[k] = vector[i];
            (*vectorNou)[k].model = (char*)malloc(strlen(vector[i].model) + 1);
            strcpy((*vectorNou)[k].model, vector[i].model);
            k++;
        }
    }
}

struct Masina getPrimulElementConditionat(struct Masina* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Masina m;
	m.id = -1;
    m.model = NULL;
    m.tonaj = 0;
    m.serie = '\0';

    for (int i = 0; i < nrElemente; i++) {
        if (strcmp(vector[i].model, conditie) == 0) {
            m.id = vector[i].id;
            m.serie = vector[i].serie;
            m.tonaj = vector[i].tonaj;

            m.model = (char*)malloc(strlen(vector[i].model) + 1);
            strcpy_s(m.model, strlen(vector[i].model) + 1, vector[i].model);
            return m;
        }
    }
	return m;
}

int main() {
    struct Masina m;
    m = initializare(1, "BMW", 2.3, 'C');
    afisare(m);

    struct Masina* vectorMasini;
    int nrMasini = 3;
    vectorMasini = (struct Masina*)malloc(sizeof(struct Masina) * nrMasini);
    vectorMasini[0] = initializare(2, "Mercedes", 2.2, 'S');
    vectorMasini[1] = initializare(3, "Audi", 4.7, 'P');
    vectorMasini[2] = initializare(4, "Audi", 2.3, 'A');
    afisareVector(vectorMasini, nrMasini);

    struct Masina* vectorMasiniCopiate;
    int nrElementeCopiate = 2;
    vectorMasiniCopiate = copiazaPrimeleMasini(vectorMasini, nrMasini, nrElementeCopiate);
    printf("Masini copiate: \n");
    afisareVector(vectorMasiniCopiate, nrElementeCopiate);

    printf("\nMasini usoare copiate: \n");
    copiazaMasiniUsoare(vectorMasini, nrMasini, 2.5, &vectorMasiniCopiate, &nrElementeCopiate);
    afisareVector(vectorMasiniCopiate, nrElementeCopiate);

    struct Masina masinaGasita = getPrimulElementConditionat(vectorMasini, nrMasini, "Audi");
    if (masinaGasita.id != -1) {
        printf("Masina gasita: \n");
        afisare(masinaGasita);
        free(masinaGasita.model);
    }
    else {
        printf("Nu s-a gasit nicio masina cu modelul dat.\n");
    }
   
    // Dezalocare memorie finală
    dezalocare(&vectorMasiniCopiate, &nrElementeCopiate);
    dezalocare(&vectorMasini, &nrMasini);

    return 0;
}