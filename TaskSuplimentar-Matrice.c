#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
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

// --------------------------------- FISIERE -------------------------------------
struct Masina citesteMasinaDinFisier(FILE* f) {
	struct Masina m;
	char buffer[100];

	fscanf(f, "%d", &m.id);
	fscanf(f, "%s", buffer);
	m.marca = (char*)malloc(strlen(buffer) + 1);
	strcpy_s(m.marca, strlen(buffer) + 1, buffer);

	fscanf(f, "%d", &m.nrCaiPutere);
	fscanf(f, "%d", &m.nrConsumuri);
	m.consumuri = (float*)malloc(m.nrConsumuri * sizeof(float));
	for (int i = 0; i < m.nrConsumuri; i++) {
		fscanf(f, "%f", &m.consumuri[i]);
	}

	return m;
}

struct Masina* citesteMasiniDinFisier(const char* numeFisier, int* nrMasini) {
	FILE* f = fopen(numeFisier, "r");
	if (!f) {
		printf("Eroare la deschiderea fisierului!\n");
		return NULL;
	}

	fscanf(f, "%d", nrMasini);
	struct Masina* masini = (struct Masina*)malloc((*nrMasini) * sizeof(struct Masina));

	for (int i = 0; i < (*nrMasini); i++) {
		masini[i] = citesteMasinaDinFisier(f);
	}

	fclose(f);
	return masini;
}

void salveazaMasinaInFisier(const char* numeFisier, struct Masina m) {
	FILE* f = fopen(numeFisier, "a");
	if (!f) {
		printf("Eroare la deschiderea fisierului!\n");
		return;
	}

	fprintf(f, "%d %s %d %d ", m.id, m.marca, m.nrCaiPutere, m.nrConsumuri);
	for (int i = 0; i < m.nrConsumuri; i++) {
		fprintf(f, "%.2f", m.consumuri[i]);
	}
	fprintf(f, "\n");
	fclose(f);
}

void salveazaVectorMasiniInFisier(const char* numeFisier, struct Masina* masini, int nrMasini) {
	FILE* f = fopen(numeFisier, "w");
	if (!f) {
		printf("Eroare la deschiderea fisierului!\n");
		return;
	}

	fprintf(f, "%d\n", nrMasini);
	for (int i = 0; i < nrMasini; i++) {
		fprintf(f, "%d %s %d %d ", masini[i].id, masini[i].marca, masini[i].nrCaiPutere, masini[i].nrConsumuri);
		for (int j = 0; j < masini[i].nrConsumuri; j++) {
			fprintf(f, "%.2f ", masini[i].consumuri[j]);
		}
		fprintf(f, "\n");
	}

	fclose(f);
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

// Functie pentru deep copy intr-o matrice in functie de caii putere
struct Masina** creeazaMatriceCluster(struct Masina* masini, int nrMasini, int* nrLinii, int** dimLinii) {
	*nrLinii = 3; // Clusterizare pe 3 niveluri de cai putere (<100, 100-200, >200)
	*dimLinii = (int*)calloc(*nrLinii, sizeof(int));
	struct Masina** matrice = (struct Masina**)malloc((*nrLinii) * sizeof(struct Masina*));

	for (int i = 0; i < nrMasini; i++) {
		if (masini[i].nrCaiPutere < 100) (*dimLinii)[0]++;
		else if (masini[i].nrCaiPutere <= 200) (*dimLinii)[1]++;
		else (*dimLinii)[2]++;
	}

	for (int i = 0; i < *nrLinii; i++) {
		matrice[i] = (struct Masina*)malloc((*dimLinii)[i] * sizeof(struct Masina));
	}

	int index[3] = { 0 };
	for (int i = 0; i < nrMasini; i++) {
		int linie;
		if (masini[i].nrCaiPutere < 100) linie = 0;
		else if (masini[i].nrCaiPutere <= 200) linie = 1;
		else linie = 2;

		matrice[linie][index[linie]].id = masini[i].id;
		matrice[linie][index[linie]].nrCaiPutere = masini[i].nrCaiPutere;
		matrice[linie][index[linie]].nrConsumuri = masini[i].nrConsumuri;

		matrice[linie][index[linie]].marca = (char*)malloc(strlen(masini[i].marca) + 1);
		strcpy(matrice[linie][index[linie]].marca, masini[i].marca);

		matrice[linie][index[linie]].consumuri = (float*)malloc(masini[i].nrConsumuri * sizeof(float));
		memcpy(matrice[linie][index[linie]].consumuri, masini[i].consumuri, masini[i].nrConsumuri * sizeof(float));

		index[linie]++;
	}
	return matrice;
}

void afiseazaMatrice(struct Masina** matrice, int nrLinii, int* dimLinii) {
	for (int i = 0; i < nrLinii; i++) {
		printf("Cluster %d:\n", i + 1);
		for (int j = 0; j < dimLinii[i]; j++) {
			printf("%d %s %d\n", matrice[i][j].id, matrice[i][j].marca, matrice[i][j].nrCaiPutere);
		}
		printf("---------------------\n");
	}
}

int main() {
	const char* fisier = "masini.txt";
	struct Masina masini[] = {
		{1, "BMW", 150, 3, (float[]) { 6.5, 7.2, 5.8 }},
		{2, "Audi", 140, 2, (float[]) { 7.5, 8.0 }},
		{3, "Ford", 90, 2, (float[]) { 6.0, 6.5 }},
		{4, "Mercedes", 220, 3, (float[]) { 8.0, 8.5, 9.0 }},
		{5, "Dacia", 75, 1, (float[]) { 5.5 }},
		{6, "Toyota", 120, 2, (float[]) { 6.5, 7.0 }},
		{7, "Honda", 95, 2, (float[]) { 6.2, 6.8 }},
		{8, "Mazda", 180, 3, (float[]) { 7.2, 7.6, 8.0 }},
		{9, "Volvo", 200, 2, (float[]) { 8.0, 8.2 }},
		{10, "Ferrari", 300, 2, (float[]) { 12.0, 13.0 }}
	};
	salveazaVectorMasiniInFisier(fisier, masini, 10);
	int nrMasini;
	struct Masina* masiniCitite = citesteMasiniDinFisier(fisier, &nrMasini);
	int nrLinii;
	int* dimLinii;
	struct Masina** matrice = creeazaMatriceCluster(masiniCitite, nrMasini, &nrLinii, &dimLinii);
	afiseazaMatrice(matrice, nrLinii, dimLinii);

	return 0;
}