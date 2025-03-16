#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

// Articol - Manea Sabina - Masina
struct Masina {
	int id;
	char* marca;
	int nrCaiPutere;
	int nrConsumuri;
	float* consumuri;
};

// Functie pentru citirea de la tastatura a unei masini
struct Masina citireMasina() {
	struct Masina m;
	printf("Introduceti Id-ul masinii: ");
	scanf("%d", &m.id);

	char buffer[100];
	printf("Introduceti marca masinii: ");
	scanf("%s", buffer);
	m.marca = (char*)malloc(strlen(buffer) + 1);
	strcpy_s(m.marca, strlen(buffer) + 1, buffer);

	printf("Introduceti nr. de cai putere: ");
	scanf("%d", &m.nrCaiPutere);

	printf("Introduceti nr. de consumuri: ");
	scanf("%d", &m.nrConsumuri);

	m.consumuri = (float*)malloc(m.nrConsumuri * sizeof(float));
	printf("Introduceti consumurile: ");
	for (int i = 0; i < m.nrConsumuri; i++) {
		scanf("%f", &m.consumuri[i]);
	}

	return m;
}

// O functie care calculeaza ceva - consumul mediu
float calculeazaConsumulMediu(struct Masina m) {
	if (m.nrConsumuri == 0) {
		return 0;
	}
	float suma = 0;
	for (int i = 0; i < m.nrConsumuri; i++) {
		suma = suma + m.consumuri[i];
	}
	return suma / m.nrConsumuri;
}

// O functie care modifica pentru un obiect primit ca parametru, un anumit camp. Noua valoare este primita ca parametru.
void modificaNrCaiPutere(struct Masina* m, int nouNrCaiPutere) {
	m->nrCaiPutere = nouNrCaiPutere;
}


// Functie de afisare 
void afisareMasina(struct Masina m) {
	printf("Id: %d\n", m.id);
	printf("Marca: %s\n", m.marca);
	printf("Nr. cai putere: %d\n", m.nrCaiPutere);
	printf("Nr. consumuri: %d\n", m.nrConsumuri);
	for (int i = 0; i < m.nrConsumuri; i++) {
		printf("Consum %d: %.2f\n", i + 1, m.consumuri[i]);
	}
	printf("------------------------------\n");
}

int main() {
	struct Masina m = citireMasina();
	printf("\n");
	afisareMasina(m);
	float consumMediu = calculeazaConsumulMediu(m);
	printf("Consum mediu: %.2f\n", consumMediu);
	modificaNrCaiPutere(&m, 10);
	printf("Masina modificata: \n");
	afisareMasina(m);
}