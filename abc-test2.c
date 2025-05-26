#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Masina Masina;
struct Masina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};

typedef struct Nod Nod;
struct Nod {
	Nod* st;
	Nod* dr;
	Masina info;
};

Masina citireMasinaDinFisier(FILE* file) {
	Masina m;
	char linie[100];
	fgets(linie, sizeof(linie), file);

	char* token = strtok(linie, ",\n");
	m.id = atoi(token);

	token = strtok(NULL, ",\n");
	m.nrUsi = atoi(token);

	token = strtok(NULL, ",\n");
	m.pret = atof(token);

	token = strtok(NULL, ",\n");
	m.model = (char*)malloc(strlen(token) + 1);
	strcpy_s(m.model, strlen(token) + 1, token);

	token = strtok(NULL, ",\n");
	m.numeSofer = (char*)malloc(strlen(token) + 1);
	strcpy_s(m.numeSofer, strlen(token) + 1, token);

	token = strtok(NULL, ",\n");
	m.serie = token[0];

	return m;
}

void afisareMasina(Masina m) {
	printf("Id: %d\n", m.id);
	printf("Nr usi: %d\n", m.nrUsi);
	printf("Pret: %.2f\n", m.pret);
	printf("Nume sofer: %s\n", m.numeSofer);
	printf("Model: %s\n", m.model);
	printf("Serie: %c\n\n", m.serie);
}

void adaugaMasinaInArbore(Nod** radacina, Masina masinaNoua) {
	if ((*radacina) != NULL) {
		if (masinaNoua.id < (*radacina)->info.id) {
			adaugaMasinaInArbore(&((*radacina)->st), masinaNoua);
		}
		else if (masinaNoua.id > (*radacina)->info.id) {
			adaugaMasinaInArbore(&((*radacina)->dr), masinaNoua);
		}
		else {
			free((*radacina)->info.model);
			free((*radacina)->info.numeSofer);
			(*radacina)->info = masinaNoua;
			(*radacina)->info.model = masinaNoua.model;
			strcpy_s((*radacina)->info.model, strlen(masinaNoua.model) + 1, masinaNoua.model);
			(*radacina)->info.numeSofer = (char*)malloc(sizeof(masinaNoua.numeSofer) + 1);
			strcpy_s((*radacina)->info.numeSofer, strlen(masinaNoua.numeSofer) + 1, masinaNoua.numeSofer);
		}
	}
	else {
		(*radacina) = (Nod*)malloc(sizeof(Nod));
		(*radacina)->info = masinaNoua;
		(*radacina)->info.model = (char*)malloc(strlen(masinaNoua.model) + 1);
		strcpy_s((*radacina)->info.model, strlen(masinaNoua.model) + 1, masinaNoua.model);
		(*radacina)->info.numeSofer = (char*)malloc(strlen(masinaNoua.numeSofer) + 1);
		strcpy_s((*radacina)->info.numeSofer, strlen(masinaNoua.numeSofer) + 1, masinaNoua.numeSofer);
		(*radacina)->dr = NULL;
		(*radacina)->st = NULL;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		adaugaMasinaInArbore(&radacina, m);
		free(m.model);
		free(m.numeSofer);
	}
	fclose(file);
	return radacina;
}

void afisareArboreInOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareArboreInOrdine(radacina->st);
		afisareMasina(radacina->info);
		afisareArboreInOrdine(radacina->dr);
	}
}

void afisareArborePreOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareMasina(radacina->info);
		afisareArborePreOrdine(radacina->st);
		afisareArborePreOrdine(radacina->dr);
	}
}

void afisareArborePostOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareArborePostOrdine(radacina->st);
		afisareArborePostOrdine(radacina->dr);
		afisareMasina(radacina->info);
	}
}

void dezalocareArboreDeMasini(Nod** radacina) {
	if ((*radacina) != NULL) {
		dezalocareArboreDeMasini(&((*radacina)->st));
		dezalocareArboreDeMasini(&((*radacina)->dr));
		free((*radacina)->info.numeSofer);
		free((*radacina)->info.model);
		free(*radacina);
		*radacina = NULL;
	}
}

int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");
	printf("--- AFISARE IN ORDINE ---\n");
	afisareArboreInOrdine(arbore);

	printf("--- AFISARE PRE ORDINE ---\n");
	afisareArborePreOrdine(arbore);

	printf("-- AFISARE POST ORDINE ---\n");
	afisareArborePostOrdine(arbore);

	dezalocareArboreDeMasini(&arbore);
	return 0;
}