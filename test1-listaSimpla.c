#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
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
	Masina info;
	Nod* next;
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

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(Nod* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInListaLaFinal(Nod** cap, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	if (*cap) {
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

void adaugaMasinaInListaLaInceput(Nod** cap, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = *cap;
	*cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	Nod* cap = NULL;
	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		adaugaMasinaInListaLaFinal(&cap, citireMasinaDinFisier(file));
	}
	fclose(file);
	return cap;
}

void dezalocareListaMasini(Nod** cap) {
	while (*cap) {
		Nod* p = *cap;
		(*cap) = p->next;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

float calculeazaPretMediu(Nod* cap) {
	float suma = 0;
	int contor = 0;
	while (cap) {
		suma += cap->info.pret;
		contor++;
		cap = cap->next;
	}
	return contor > 0 ? (suma / contor) : 0;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* cap, const char* numeSofer) {
	float suma = 0;
	while (cap) {
		if (strcmp(cap->info.numeSofer, numeSofer) == 0) {
			suma += cap->info.pret;
		}
		cap = cap->next;
	}
	return suma;
}

int main() {
	Nod* cap = citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(cap);
	printf("Pretul mediu este: %.2f\n", calculeazaPretMediu(cap));
	printf("Pretul masinilor soferului Ionescu este: %.2f\n", calculeazaPretulMasinilorUnuiSofer(cap, "Ionescu"));
	dezalocareListaMasini(&cap);
	return 0;
}