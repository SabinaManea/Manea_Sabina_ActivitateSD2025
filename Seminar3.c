#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	//afiseaza toate atributele unei masini
	printf("ID: %d\n", masina.id);
	printf("Nr usi: %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("ID: %c\n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	//afiseaza toate elemente de tip masina din vector
	//prin apelarea functiei afisareMasina()
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	//adauga in vectorul primit o noua masina pe care o primim ca parametru
	//ATENTIE - se modifica numarul de masini din vector;
	Masina* vector = (Masina*)malloc(sizeof(Masina) * (*nrMasini) + 1);

	for (int i = 0; i < (*nrMasini); i++) {
		vector[i] = (*masini)[i];
	}

	//aici facem deep copy, se putea si deep

	vector[(*nrMasini)] = masinaNoua;
	vector[(*nrMasini)].model = malloc(sizeof(char) * (strlen(masinaNoua.model) + 1));
	strcpy_s(vector[(*nrMasini)].model, strlen(masinaNoua.model) + 1, masinaNoua.model);

	vector[(*nrMasini)].numeSofer = malloc(sizeof(char) * (strlen(masinaNoua.numeSofer) + 1));
	strcpy_s(vector[(*nrMasini)].numeSofer, strlen(masinaNoua.model) + 1, masinaNoua.numeSofer);
	(*nrMasini)++;
	free(*masini);
	(*masini) = vector;
}

Masina citireMasinaFisier(FILE* file) {
	//functia citeste o masina dintr-un strceam deja deschis
	//masina citita este returnata;
	char buffer[200];
	char separator[] = ",\n";
	Masina masinaCitita;

	fgets(buffer, sizeof(buffer), file);
	char* token = strtok(buffer, separator);
	masinaCitita.id = atoi(token);
	masinaCitita.nrUsi = atoi(strtok(NULL, separator));
	masinaCitita.pret = atof(strtok(NULL, separator));

	masinaCitita.model = (char*)malloc(sizeof(char) * (strlen(token) + 1));
	strcpy_s(masinaCitita.model, (strlen(token) + 1), token);

	masinaCitita.numeSofer = (char*)malloc(sizeof(char) * (strlen(token) + 1));
	strcpy_s(masinaCitita.numeSofer, (strlen(token) + 1), token);

	masinaCitita.serie = token[0];
	
	return masinaCitita;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	Masina* masini = NULL;

	while (!feof(f)) { //marcheaza ca a ajuns la final
		Masina m = citireMasinaFisier(f);
		adaugaMasinaInVector(&masini, nrMasiniCitite, m);
		free(m.model); // daca faceam shallow copy astea doua nu le mai scriam
		free(m.numeSofer);
	}
	fclose(f);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
	for (int i = 0; i < *nrMasini; i++) {
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	*vector = NULL;
	*nrMasini = 0;
}

int main() {
	Masina* masini = NULL;
	int nrMasini = 0;
	masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);
	dezalocareVectorMasini(&masini, &nrMasini);
	return 0;
}