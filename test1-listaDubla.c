//#define _CRT_SECURE_NO_WARNINGS
//#include <string.h>
//#include <stdlib.h>
//#include <stdio.h>
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
//typedef struct ListaDubla ListaDubla;
//struct ListaDubla {
//	Nod* first;
//	Nod* last;
//	int nrNoduri;
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
//	printf("Nr usi: %d\n", masina.nrUsi);
//	printf("Pret: %.2f\n", masina.pret);
//	printf("Model: %s\n", masina.model);
//	printf("Nume sofer: %s\n", masina.numeSofer);
//	printf("Serie: %c\n\n", masina.serie);
//}
//
//void afisareListaDeLaInceput(ListaDubla lista) {
//	Nod* cap = lista.first;
//	while (cap) {
//		afisareMasina(cap->info);
//		cap = cap->next;
//	}
//}
//
//void afisareListaDeLaFinal(ListaDubla lista) {
//	Nod* cap = lista.last;
//	while (cap) {
//		afisareMasina(cap->info);
//		cap = cap->prev;
//	}
//}
//
//void inserareMasinaInListaLaInceput(ListaDubla* lista, Masina masinaNoua) {
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->prev = NULL;
//	nou->next = lista->first;
//	nou->info = masinaNoua;
//	if (lista->first != NULL) {
//		lista->first->prev = nou;
//	}
//	else {
//		lista->last = nou;
//	}
//	lista->first = nou;
//	lista->nrNoduri++;
//}
//
//void inserareMasinaInListaLaFinal(ListaDubla* lista, Masina masinaNoua) {
//	Nod* nou = (Nod*)malloc(sizeof(Nod));
//	nou->info = masinaNoua;
//	nou->next = NULL;
//	nou->prev = lista->last;
//	if (lista->last != NULL) {
//		lista->last->next = nou;
//	}
//	else {
//		lista->first = nou;
//	}
//	lista->last = nou;
//	lista->nrNoduri++;
//}
//
//ListaDubla citireLdDinFisier(FILE* numeFisier) {
//	ListaDubla lista;
//	FILE* file = fopen(numeFisier, "r");
//	lista.first = NULL;
//	lista.last = NULL;
//	lista.nrNoduri = 0;
//	while (!feof(file)) {
//		inserareMasinaInListaLaFinal(&lista, citireMasinaDinFisier(file));
//	}
//	fclose(file);
//	return lista;
//}
//
//void dezalocareLd(ListaDubla* lista) {
//	Nod* p = lista->first;
//	while (p) {
//		Nod* aux = p;
//		p = p->next;
//		free(aux->info.model);
//		free(aux->info.numeSofer);
//		free(aux);
//	}
//	lista->first = NULL;
//	lista->last = NULL;
//	lista->nrNoduri = 0;
//}
//
//float calculeazaPretMediu(ListaDubla lista) {
//	if (lista.nrNoduri > 0) {
//		float suma = 0;
//		Nod* p = lista.first;
//		while (p) {
//			suma += p->info.pret;
//			p = p->next;
//		}
//		return suma / lista.nrNoduri;
//	}
//	return 0;
//}
//
//int main() {
//	ListaDubla lista = citireLdDinFisier("masini.txt");
//	afisareListaDeLaInceput(lista);
//	printf("Pretul mediu al masinilor este: %.2f", calculeazaPretMediu(lista));
//	dezalocareLd(&lista);
//	return 0;
//}