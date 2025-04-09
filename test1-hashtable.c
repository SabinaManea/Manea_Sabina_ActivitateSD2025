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
	Masina info;
	Nod* next;
};

typedef struct Hashtable Hashtable;
struct Hashtable {
	Nod** tabela;
	int dim;
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

	token = strok(NULL, ",\n");
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
	printf("Pret: %d\n", m.pret);
	printf("Model: %d\n", m.model);
	printf("Nume sofer: %d\n", m.numeSofer);
	printf("Serie: %d\n", m.serie);
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

Hashtable initializareHashTable(int dimensiune) {
	Hashtable ht;
	ht.dim = dimensiune;
	ht.tabela = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.tabela[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* nume, int dimensiune) {
	int suma = 0;
	for (int i = 0; i < strlen(nume); i++) {
		suma += nume[i];
	}
	return suma % dimensiune;
}

inserareMasinaInTabela(Hashtable hash, Masina masina) {
	int pozitie = calculeazaHash(masina.numeSofer, hash.dim);
	adaugaMasinaInListaLaFinal(hash.tabela[pozitie], masina);
}

Hashtable citireMasinaDinFisier(const char* numeFisier, int dimensiune) {
	Hashtable ht = initializareHashTable(dimensiune);
	FILE* file = fopen(numeFisier, "r");
	while (!feof(file)) {
		Masina masina = citireMasinaDinFisier(file);
		inserareMasinaInTabela(ht, masina);
	}
	fclose(file);
	return ht;
}

void afisareTabelaDeMasini(Hashtable ht) {
	for (int i = 0; i < ht.dim; i++) {
		if (ht.tabela[i] != NULL) {
			printf("\nMasinile de pe pozitia %d sunt: \n", i);
			afisareListaMasini(ht.tabela[i]);
		}
		else {
			printf("\nPe pozitia %d nu sunt masini\n", i);
		}
	}
}

void dezalocareLs(Nod** cap) {
	while (*cap) {
		Nod* p = *cap;
		(*cap) = p->next;
		free(p->info.numeSofer);
		free(p->info.model);
		free(p);
	}
}

void dezalocareHt(Hashtable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		dezalocareLs(&(ht->tabela[i]));
	}
	free(ht->tabela);
	ht->tabela = NULL;
	ht->dim = 0;
}

int main() {

	return 0;
}