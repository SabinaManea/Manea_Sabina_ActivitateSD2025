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

// Functia care afiseaza un vector de obiecte
void afisareVectorMasini(struct Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

// Functie care creeaza un nou vector cu obiectele care indeplinesc o anumita conditie - cai putere >= 100
struct Masina* filtrareMasiniCaiPutere(struct Masina* masini, int nrMasini, int* nrRezultat) {
	struct Masina* masiniFiltrate = (struct Masina*)malloc(nrMasini * sizeof(struct Masina));
	(*nrRezultat) = 0;

	for (int i = 0; i < nrMasini; i++) {
		if (masini[i].nrCaiPutere >= 100) {
			masiniFiltrate[*nrRezultat] = masini[i];
			(*nrRezultat)++;
		}
	}

	return masiniFiltrate;
}

// Functie care creeaza un nou vector cu obiecte care indeplinesc o anumita conditie - consum redus
struct Masina* filtrareMasiniConsumRedus(struct Masina* masini, int nrMasini, int* nrRezultat) {
	struct Masina* masiniFiltrate = (struct Masina*)malloc(nrMasini * sizeof(struct Masina));
	*nrRezultat = 0;

	for (int i = 0; i < nrMasini; i++) {
		if (calculeazaConsumulMediu(masini[i]) < 7) {
			masiniFiltrate[*nrRezultat] = masini[i];
			(*nrRezultat)++;
		}
	}

	// Realocăm memoria la dimensiunea corectă
	masiniFiltrate = (struct Masina*)realloc(masiniFiltrate, (*nrRezultat) * sizeof(struct Masina));

	return masiniFiltrate;
}

// Realizati o functie care concateneaza doi vectori.
struct Masina* concateneazaVectori(struct Masina* v1, int nrV1, struct Masina* v2, int nrV2, int* nrNou) {
	(*nrNou) = nrV1 + nrV2;
	struct Masina* rezultat = (struct Masina*)malloc(sizeof(struct Masina) * (*nrNou));
	for (int i = 0; i < nrV1; i++) {
		rezultat[i] = v1[i];
	}
	for (int i = 0; i < nrV2; i++) {
		rezultat[nrV1 + i] = v2[i];
	}

	return rezultat;
}

// Functia de dezalocare
void dezalocare(struct Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++) {
		free(masini[i].marca);
		free(masini[i].consumuri);
	}
	free(masini);
	masini = NULL;
	nrMasini = 0;
}

int main() {
	struct Masina m = citireMasina();
	printf("------------------------------------------ AFISARE MASINA ------------------------------------------\n");
	afisareMasina(m);
	float consumMediu = calculeazaConsumulMediu(m);
	printf("------------------------------------------ CONSUM MEDIU ------------------------------------------\n");
	printf("Consum mediu: %.2f\n", consumMediu);

	printf("------------------------------------------ MASINA MODIFICATA ------------------------------------------\n");
	modificaNrCaiPutere(&m, 10);
	printf("Masina modificata: \n");
	afisareMasina(m);

	// Crearea unui vector alocat dinamic cu S obiecte de tipul structurii
	printf("------------------------------------------ CITIRE MASINI ------------------------------------------\n");
	int s = 2;
	struct Masina* masini = (struct Masina*)malloc(sizeof(struct Masina) * s);
	for (int i = 0; i < s; i++) {
		printf("\nCitirea masinii %d:\n", i + 1);
		masini[i] = citireMasina();
	}

	printf("------------------------------------------ MASINI CAI PUTERE >= 100 ------------------------------------------\n");
	int nrFiltrat = 0;
	struct Masina* masiniCaiPutere = filtrareMasiniCaiPutere(masini, s, &nrFiltrat);
	printf("\nMasini cu cel putin 100 cai putere: \n");
	afisareVectorMasini(masiniCaiPutere, nrFiltrat);

	printf("------------------------------------------ MASINI CONSUM REDUS ------------------------------------------\n");
	int nrMasiniConsumRedus = 0;
	struct Masina* masiniConsumRedus = filtrareMasiniConsumRedus(masini, s, &nrMasiniConsumRedus);
	if (nrMasiniConsumRedus > 0) {
		afisareVectorMasini(masiniConsumRedus, nrMasiniConsumRedus);
	}
	else {
		printf("Nu exista masini cu consum redus.\n");
	}

	printf("------------------------------------------ CONCATENARE DOI VECTORI ------------------------------------------\n");
	int nrConcatenat = 0;
	struct Masina* masiniConcatenate = concateneazaVectori(masiniCaiPutere, nrFiltrat, masiniConsumRedus, nrMasiniConsumRedus, &nrConcatenat);
	printf("\nMasini concatenate (cele cu mai putin de 100 cai putere + cele cu consum redus): \n");
	afisareVectorMasini(masiniConcatenate, nrConcatenat);
}