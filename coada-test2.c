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
//	Masina info;
//	Nod* next;
//	Nod* prev;
//};
//
//typedef struct LD LD;
//struct LD {
//	Nod* first;
//	Nod* last;
//};
//
//Masina citireMasinaDinFisier(FILE* file) {
//	Masina m;
//	const linie[100];
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
//void enqueue(LD* coada, Masina masina) {
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->info = masina;
//	nou->next = NULL;
//	nou->prev = coada->last;
//	if (coada->last != NULL) {
//		coada->last->next = nou;
//	}
//	else {
//		coada->first = nou;
//	}
//	coada->last = nou;
//}
//
//Masina dequeue(LD* coada) {
//	Masina m;
//	m.id = -1;
//	if (coada->first != NULL) {
//		m = coada->first->info;
//		Nod* aux = coada->first;
//		if (coada->first->next != NULL) {
//			coada->first = coada->first->next;
//			coada->first->prev = NULL;
//		}
//		else {
//			coada->last = NULL;
//			coada->first = NULL;
//		}
//		return m;
//	}
//}
//
//LD citireCoadaDeMasiniDinFisier(const char* numeFisier) {
//	FILE* file = fopen(numeFisier, "r");
//	LD coada;
//	coada.first = NULL;
//	coada.last = NULL;
//	while (!feof(file)) {
//		Masina m = citireMasinaDinFisier(file);
//		enqueue(&coada, m);
//	}
//	fclose(file);
//	return coada;
//}
//
//void dezalocare(LD* coada) {
//	Nod* aux = coada->last;
//	while (aux) {
//		Nod* temp = aux;
//		aux = aux->prev;
//		free(temp->info.numeSofer);
//		free(temp->info.model);
//		free(temp);
//	}
//	coada->last = NULL;
//	coada->first = NULL;
//}
//
//int main() {
//	LD coada = citireCoadaDeMasiniDinFisier("masini.txt");
//	Masina m = dequeue(&coada);
//	afisareMasina(m);
//
//	return 0;
//}