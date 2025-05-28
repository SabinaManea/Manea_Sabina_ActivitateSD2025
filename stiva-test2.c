//#define _CRT_SECURE_NO_WARNINGS
//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//
//typedef struct Masina Masina;
//struct Masina {
//	int id;
//	int nrUsi;
//	float pret;
//	char* model;
//	char* numeSofer;
//	unsigned char serie;
//};
//
//typedef struct Nod Nod;
//struct Nod {
//	Nod* next;
//	Masina info;
//};
//
//Masina citireMasinaDinFisier(FILE* file) {
//	Masina m;
//	char linie[100];
//	fgets(linie, sizeof(linie), file);
//
//	char* token = strtok(linie, ",\n");
//	m.id = atoi(token);
//
//	token = strtok(NULL, ",\n");
//	m.nrUsi = atoi(token);
//
//	token = strtok(NULL, ",\n");
//	m.pret = atof(token);
//
//	token = strtok(NULL, ",\n");
//	m.model = (char*)malloc(strlen(token) + 1);
//	strcpy_s(m.model, strlen(token) + 1, token);
//
//	token = strtok(NULL, ",\n");
//	m.numeSofer = (char*)malloc(strlen(token) + 1);
//	strcpy_s(m.numeSofer, strlen(token) + 1, token);
//
//	token = strtok(NULL, ",\n");
//	m.serie = token[0];
//
//	return m;
//}
//
//void afisareMasina(Masina masina) {
//	printf("Id: %d\n", masina.id);
//	printf("Nr. usi : %d\n", masina.nrUsi);
//	printf("Pret: %.2f\n", masina.pret);
//	printf("Model: %s\n", masina.model);
//	printf("Nume sofer: %s\n", masina.numeSofer);
//	printf("Serie: %c\n\n", masina.serie);
//}
//
//void pushStack(Nod** stiva, Masina masina) {
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->info = masina;
//	nou->next = (*stiva);
//	(*stiva) = nou;
//}
//
//Masina popStack(Nod** stiva) {
//	if ((*stiva) != NULL) {
//		Masina rezultat = (*stiva)->info;
//		Nod* varfVechi = (*stiva);
//		(*stiva) = (*stiva)->next;
//		free(varfVechi);
//		return rezultat;
//	}
//	Masina m;
//	m.id = -1;
//	return m;
//}
//
//unsigned char emptyStack(Nod* stiva) {
//	return stiva == NULL;
//}
//
//Nod* citireStackMasiniDinFisier(const char* numeFisier) {
//	FILE* file = fopen(numeFisier, "r");
//	Nod* stiva = NULL;
//	while (!feof(file)) {
//		Masina m = citireMasinaDinFisier(file);
//		pushStack(&stiva, m);
//	}
//	fclose(file);
//	return stiva;
//}
//
//int stackSize(Nod* stiva) {
//	int nr = 0;
//	while (stiva != NULL) {
//		nr++;
//		stiva = stiva->next;
//	}
//	return nr;
//}
//
//void dezalocareStiva(Nod** stiva) {
//	while ((*stiva) != NULL) {
//		Masina m = popStack(stiva);
//		free(m.numeSofer);
//		free(m.model);
//	}
//}
//
//int main() {
//	Nod* stiva = citireStackMasiniDinFisier("masini.txt");
//	int nr = stackSize(stiva);
//	printf("Nr de elemente in stiva: %d\n\n", nr);
//
//	Masina m = popStack(&stiva);
//	afisareMasina(m);
//	free(m.numeSofer);
//	free(m.model);
//
//	return 0;
//}