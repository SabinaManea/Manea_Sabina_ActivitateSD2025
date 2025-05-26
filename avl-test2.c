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
	printf("Model: %.2f\n", m.pret);
	printf("Model: %s\n", m.model);
	printf("Nume sofer: %s\n", m.numeSofer);
	printf("Serie: %c\n", m.serie);
	printf("\n");
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina != NULL) {
		int inaltimeSt = calculeazaInaltimeArbore(radacina->st);
		int inaltimeDr = calculeazaInaltimeArbore(radacina->dr);
		return 1 + (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr);
	}
	else {
		return 0;
	}
}

void rotireDreapta(Nod** radacina) {
	Nod* aux = (*radacina)->st;
	(*radacina)->st = aux->dr;
	aux->dr = (*radacina);
	(*radacina) = aux;
}

void rotireStanga(Nod** radacina) {
	Nod* aux = (*radacina)->dr;
	(*radacina)->dr = aux->st;
	aux->st = (*radacina);
	(*radacina) = aux;
}

int calculeazaGradEchilibru(Nod* radacina) {
	if (radacina != NULL) {
		return calculeazaInaltimeArbore(radacina->st) - calculeazaInaltimeArbore(radacina->dr);
	}
	else {
		return 0;
	}
}

void adaugaMasinaInArboreEchilibrat(Nod** radacina, Masina masinaNoua) {
	if ((*radacina) != NULL) {
		if ((*radacina)->info.id > masinaNoua.id) {
			adaugaMasinaInArboreEchilibrat(&((*radacina)->st), masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&((*radacina)->dr), masinaNoua);
		}

		int grad = calculeazaGradEchilibru(*radacina);
		if (grad == +2) {
			if (calculeazaGradEchilibru((*radacina)->st) >= 0) {
				rotireDreapta(radacina);            // LL
			}
			else {
				rotireStanga(&(*radacina)->st);     // LR
				rotireDreapta(radacina);
			}
		}
		else if (grad == -2) {
			if (calculeazaGradEchilibru((*radacina)->dr) <= 0) {
				rotireStanga(radacina);             // RR
			}
			else {
				rotireDreapta(&(*radacina)->dr);    // RL
				rotireStanga(radacina);
			}
		}

	}
	else {
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = masinaNoua;
		nod->st = NULL;
		nod->dr = NULL;
		(*radacina) = nod;
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(file)) {
		Masina m = citireMasinaDinFisier(file);
		adaugaMasinaInArboreEchilibrat(&radacina, m);
	}
	fclose(file);
	return radacina;
}

void afisareArborePreOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareMasina(radacina->info);
		afisareArborePreOrdine(radacina->st);
		afisareArborePreOrdine(radacina->dr);
	}
}

void afisareArboreInOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareArboreInOrdine(radacina->st);
		afisareMasina(radacina->info);
		afisareArboreInOrdine(radacina->dr);
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
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);
		*radacina = NULL;
	}
}

int main() {
	Nod* arboreAvl = citireArboreDeMasiniDinFisier("masini.txt");
	afisareArborePreOrdine(arboreAvl);

	dezalocareArboreDeMasini(&arboreAvl);

	return 0;
}