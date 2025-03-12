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
	printf("ID: %d \n", masina.id);
	printf("Numar de usi: %d \n", masina.nrUsi);
	printf("Pret: %.2f \n", masina.pret);
	printf("Model: %s \n", masina.model);
	printf("Nume sofer: %s \n", masina.numeSofer);
	printf("Serie: %c \n", masina.serie);
	printf("-----------------------------\n");
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
	Masina* copie = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1));
	copie[(*nrMasini)] = masinaNoua;
	for (int i = 0; i < (*nrMasini); i++) {
		copie[i] = (*masini)[i];
	}
		
	if (*masini) {
		free(*masini);
	}

	(*nrMasini)++;
	*masini = copie;
}

Masina citireMasinaFisier(FILE* file) {
	//functia citeste o masina dintr-un stream deja deschis
	//masina citita este returnata;

	char buffer[200];
	char delimitare[] = ",\n";
	Masina m;

	if (fgets(buffer, sizeof(buffer), file) != NULL) {
		char* element = strtok(buffer, delimitare);
		m.id = atoi(element);

		element = strtok(NULL, delimitare);
		m.nrUsi = atoi(element);

		element = strtok(NULL, delimitare);
		m.pret = atof(element);

		element = strtok(NULL, delimitare);
		m.model = (char*)malloc(strlen(element) + 1);
		strcpy(m.model, element);

		element = strtok(NULL, delimitare);
		m.numeSofer = (char*)malloc(strlen(element) + 1);
		strcpy(m.numeSofer, element);

		element = strtok(NULL, delimitare);
		m.serie = element[0];
	}
	else {
		m.id = -1; // NU S-A CITIT NICIO MASINA
	}

	return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* file = fopen(numeFisier, "r");
	char buffer[200];
	Masina* masini = NULL;
	*nrMasiniCitite = 0;
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		Masina m = citireMasinaFisier(buffer);
		if (m.id != -1) {
			adaugaMasinaInVector(&masini, nrMasiniCitite, m);
		}
	}
	fclose(file);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
	if (*vector) {
		for (int i = 0; i < *nrMasini; i++) {
			free((*vector)[i].model);
			free((*vector)[i].numeSofer);
		}
		free(*vector);
		*vector = NULL;
		*nrMasini = 0;
	}
}

int main() {
	Masina masina1;
	masina1.id = 1;
	masina1.nrUsi = 4;
	masina1.pret = 400;
	masina1.model = "BMW";
	masina1.numeSofer = "Marius";
	masina1.serie = 'A';
	
	afisareMasina(masina1);

	Masina vectorMasini[2];
	vectorMasini[0].id = 2;
	vectorMasini[0].nrUsi = 2;
	vectorMasini[0].pret = 200;
	vectorMasini[0].model = "Audi";
	vectorMasini[0].numeSofer = "Cris";
	vectorMasini[0].serie = 'B';

	vectorMasini[1].id = 3;
	vectorMasini[1].nrUsi = 3;
	vectorMasini[1].pret = 300;
	vectorMasini[1].model = "Mercedes";
	vectorMasini[1].numeSofer = "Iuliana";
	vectorMasini[1].serie = 'C';
	printf("Lista masini: \n");
	afisareVectorMasini(vectorMasini, 2);

	// TESTARE FUNCTIE CITIRE VECTOR MASINI, AFISARE VECTOR MASINI SI DEZALOCARE VECTOR MASINI
	int nrMasini = 0;
	Masina* masiniFisier = citireVectorMasiniFisier("masini.txt", &nrMasini);
	if (masiniFisier != NULL) {
		printf("Masini citite din fisier:\n");
		afisareVectorMasini(masiniFisier, nrMasini);

		dezalocareVectorMasini(&masiniFisier, &nrMasini);
	}

	return 0;
}