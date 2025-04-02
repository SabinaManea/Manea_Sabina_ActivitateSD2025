#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// chaining, liniar probing, quadratic probing
// double hashing, rehashing
// rehashing - cand avem toate pozitiile ocupate si mai avem de adaugat, facem o noua dimensiune si rehashuim

typedef struct StructuraMasina Masina;
struct StructuraMasina {
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

typedef struct HashTable HashTable;
struct HashTable {
	int dim;
	Nod** vector; // vector de pointeri
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

void adaugaMasinaInLista(Nod* cap, Masina masinaNoua) { // daca lasam cu Nod* inseamna ca sigur exista lista, 
	// nu putem sa facem asta daca nu exista lista cand apelam
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	while (cap->next) {
		cap = cap->next;
	}
	cap->next = nou;
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* model, int dimensiune) {
	int suma = 0; // ar trb sa vedem daca dimensiune e diferit de 0 ca sa fie perfect
	for (int i = 0; i < strlen(model); i++) {
		suma += model[i];
	}
	return suma % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int pozitie = calculeazaHash(masina.model, hash.dim);
	if (hash.vector[pozitie] != NULL) {
		// AVEM COLIZIUNE !! 
		adaugaMasinaInLista(hash.vector[pozitie], masina);
	}
	else {
		// NU AVEM COLIZIUNE !!, daca functia adaugaMasinaInLista o facem si pt atunci cand dimensiunea e 0 atunci puteam sa punem direct functia aia
		hash.vector[pozitie] = (Nod*)malloc(sizeof(Nod));
		hash.vector[pozitie]->info = masina;
		hash.vector[pozitie]->next = NULL;
	}
}

HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {
	FILE* file = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(dimensiune);
	while (!feof(file)) {
		Masina masina = citireMasinaDinFisier(file);
		inserareMasinaInTabela(hash, masina);
	}
	fclose(file);
	return hash;
}

void afisareTabelaDeMasini(HashTable ht) {
	//sunt afisate toate masinile cu evidentierea clusterelor realizate - evidentiem toate coliziunile pe care le am avut
	for (int i = 0; i < ht.dim; i++) {
		printf("\nClusterul %d:\n", i);
		afisareListaMasini(ht.vector[i]);
	}
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
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
	HashTable hash = citireMasiniDinFisier("masini.txt", 10);
	afisareTabelaDeMasini(hash);

	return 0;
}