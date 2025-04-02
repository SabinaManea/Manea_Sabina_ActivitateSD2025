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
struct Nod {
	Masina info;
	Nod* next;
};

struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

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
	m1.model = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
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

void afisareListaMasini(Nod* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	if ((*cap) == NULL) {
		(*cap) = nou;
	}
	else {
		Nod* temp = (*cap);
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable hash;
	hash.dim = dimensiune;
	hash.vector = (Nod**)malloc(dimensiune * sizeof(Nod*));
	for (int i = 0; i < dimensiune; i++) {
		hash.vector[i] = NULL;
	}
	return hash;
}

int calculeazaHash(char* model, int nrUsi, float pret, int dimensiune) {
	if (dimensiune > 0) {
		int rez = strlen(model) * nrUsi + (int)pret;
		return rez % dimensiune;
	}
	return -1;
}

void inserareMasinaInTabela(HashTable* hash, Masina masina) {
	int pozitie = calculeazaHash(masina.model, masina.nrUsi, masina.pret, hash->dim);
	if (hash->dim > 0) {
		if (hash->vector[pozitie] == NULL) {
			adaugaMasinaInLista(&(hash->vector[pozitie]), masina);
		}
		else {
			adaugaMasinaInLista(&(hash->vector[pozitie]), masina);
		}
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {
	FILE* file = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(dimensiune);

	while (!feof(file)) {
		inserareMasinaInTabela(&hash, citireMasinaDinFisier(file));
	}
	fclose(file);
	return hash;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("\nClusterul %d\n", i + 1);
		afisareListaMasini(ht.vector[i]);
	}
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	for (int i = 0; i < ht->dim; i++) {
		Nod* curent = ht->vector[i];
		while (curent) {
			free(curent->info.model);
			free(curent->info.numeSofer);
			Nod* temp = curent;
			curent = curent->next;
			free(temp);
		}
	}
	free(ht->vector);
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	return NULL;
}

Masina getMasinaDupaCheie(HashTable ht /*valoarea pentru masina cautata*/) {
	Masina m;
	//cauta masina dupa valoarea atributului cheie folosit in calcularea hash-ului
	//trebuie sa modificam numele functiei 
	return m;
}

int main() {
	HashTable ht = citireMasiniDinFisier("masini.txt", 5);
	afisareTabelaDeMasini(ht);

	return 0;
}