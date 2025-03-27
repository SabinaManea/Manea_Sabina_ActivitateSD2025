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

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;


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
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
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

void afisareListaMasini(Nod* lista) {
	//afiseaza toate elemente de tip masina din lista simplu inlantuita
	//prin apelarea functiei afisareMasina()
	while (lista) {
		afisareMasina(lista->info);
		lista = lista->next;
	}

}

void adaugaMasinaInLista(Nod** lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru

	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;


	if ((*lista) == NULL) {
		(*lista) = nou;
	}
	else {
		Nod* temp = (*lista);
		while (temp->next) {
			temp = temp->next;

		}
		temp->next = nou;
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) { //dublu pointer pt ca modificam lista mereu
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = *cap;
	*cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(f))
	{
		adaugaLaInceputInLista(&lista, citireMasinaDinFisier(f)); // asa o sa fie de la 10 la 1 afisate in consola
		// cu adaugaInLista o sa fie in ordine de la 1 la 10
	}
	fclose(f);
	return lista;
}

void dezalocareListaMasini(Nod** lista) {
	while (*lista) {
		Nod* p = (*lista);
		(*lista) = (*lista)->next;
		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}
}

float calculeazaPretMediu(Nod* lista) {
	float s = 0;
	int k = 0;
	while (lista) {
		s += lista->info.pret;
		k++;
		lista = lista->next;
	}
	return (k > 0) ? s / k : 0;
}

void stergeMasiniDinSeria(Nod** lista, char serieCautata) {
	while ((*lista) != NULL && (*lista)->info.serie == serieCautata) {
		Nod* temp = (*lista);
		(*lista) = (*lista)->next;
		free(temp->info.numeSofer);
		free(temp->info.model);
		free(temp);
	}
	Nod* p = (*lista);
	while (p != NULL) {
		while (p->next && p->next->info.serie != serieCautata) {
			p = p->next;
		}
		if (p->next) {
			Nod* temp = p->next;
			p->next = temp->next;
			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
		}
		else {
			p = p->next;
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
	float suma = 0;
	while (lista) {
		if (strcmp(lista->info.numeSofer, numeSofer) == 0) {
			suma += lista->info.pret;
		}
		lista = lista->next;
	}
	return suma;
}

//1. Implementati o functie care sterge un nod de pe o pozitie data ca parametru.Daca lista
//are mai putine noduri decat index - ul dat, nu se realizeaza stergerea.
void stergeNodDePePozitia(Nod** lista, int index) {
	// Daca lista e goala, nu se intampla nimic
	if ((*lista) == NULL) return; 

	// Daca index ul dat este chiar primul element, trebuie sa mutam capul listei la urmatorul element
	if (index == 0) {
		Nod* temp = (*lista);
		(*lista) = (*lista)->next; // Mutam capul listei la urmatorul element
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
		return;
	}

	Nod* curent = (*lista);
	int i = 0;

	// Parcurgem lista pana la pozitia dorita sau pana se termina lista
	while (curent->next != NULL && i < index - 1) {
		curent = curent->next;
		i++;
	}

	// Daca nodul urmator NU exista (index prea mare), iesim
	if (curent->next == NULL) return;

	// Stergem nodul de pe pozitia index
	Nod* temp = curent->next;
	curent->next = temp->next; // Sarim peste nodul care trebuie sters
	free(temp->info.model);
	free(temp->info.numeSofer);
	free(temp);
}

//2. Implementati o functie care sa insereze elementele in cadrul listei simplu inlantuite astfel
//incat acestea a fie sortate crescator dupa un camp la alegerea voastra.
void insereazaMasinaSortata(Nod** lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;

	// Daca lista e goala sau masina noua are pret mai mic decat primul element
	if ((*lista) == NULL || (*lista)->info.pret > masinaNoua.pret) {
		nou->next = (*lista);
		*lista = nou;
	}
	else {
		Nod* curent = (*lista);
		// Gasim pozitia corecta din lista
		while (curent->next != NULL && curent->next->info.pret < masinaNoua.pret) {
			curent = curent->next;
		}
		// Inseram masina dupa nodul curent
		nou->next = curent->next;
		curent->next = nou;
	}
}

//3. Implementati o functie care salveaza intr - un vector toate obiectele care indeplinesc o
//conditie stabilita de voi.Realizati deep copy, astfel incat elementele din vector sa fie
//diferentiate de cele din lista.Aveti grija la alocarea de spatiu, deoarece trebuie sa stabiliti
//pentru cate elemente veti aloca spatiu.
Masina* salveazaMasiniCareIndeplinescConditie(Nod* lista, int* nrMasini, float pretMinim) {
	// Determinam numarul de masini care indeplinesc conditia
	(*nrMasini) = 0;
	Nod* curent = lista;
	while (curent != NULL) {
		if (curent->info.pret >= pretMinim) {
			(*nrMasini)++;
		}
		curent = curent->next;
	}

	// Alocam spatiu pentru vectorul de masini
	Masina* vectorMasini = (Masina*)malloc((*nrMasini) * sizeof(Masina));

	//Parcurgem din nou lista si facem deep copy pentru masinile care indeplinesc conditia
	curent = lista;
	int i = 0;
	while (curent != NULL) {
		if (curent->info.pret >= pretMinim) {
			vectorMasini[i].model = (char*)malloc(strlen(curent->info.model) + 1);
			strcpy_s(vectorMasini[i].model, strlen(curent->info.model) + 1, curent->info.model);

			vectorMasini[i].numeSofer = (char*)malloc(strlen(curent->info.numeSofer) + 1);
			strcpy_s(vectorMasini[i].numeSofer, strlen(curent->info.numeSofer) + 1, curent->info.numeSofer);

			vectorMasini[i].id = curent->info.id;
			vectorMasini[i].nrUsi = curent->info.nrUsi;
			vectorMasini[i].pret = curent->info.pret;
			vectorMasini[i].serie = curent->info.serie;

			i++;
		}
		curent = curent->next;
	}
	return vectorMasini;
}

//4. Implementati o functie care primeste lista si doua pozitii.In cadrul functiei trebuie sa
//interschimbati elementele de pe cele doua pozitii din lista primita
void schimbaElementelePePozitii(Nod* lista, int poz1, int poz2) {
	if (poz1 == poz2) return;
	Nod* nod1 = NULL;
	Nod* nod2 = NULL;
	Nod* curent = lista;
	int i = 0;

	// Cautam nodurile de pe poz1 si poz2
	while (curent != NULL) {
		if (i == poz1) nod1 = curent;
		if (i == poz2) nod2 = curent;
		curent = curent->next;
		i++;
	}

	// Daca oricare dintre noduri este NULL, nu facem nimic
	if (nod1 == NULL || nod2 == NULL) return;

	// Interschimbam valorile dintre cele doua noduri
	Masina temp = nod1->info;
	nod1->info = nod2->info;
	nod2->info = temp;
}

void afisareVectorMasini(Masina* vector, int nrMasini) {
	for (int i = 0; i < nrMasini; i++) {
		afisareMasina(vector[i]);
	}
}

void dezalocareVectorMasini(Masina* vector, int nrMasini) {
	for (int i = 0; i < nrMasini; i++) {
		free(vector[i].model);
		free(vector[i].numeSofer);
	}
	free(vector);
}

int main() {
	Nod* masini = NULL;
	Masina m1 = { 1,4,5000.0,malloc(strlen("BMW") + 1),malloc(strlen("Ion Popescu") + 1) };
	strcpy_s(m1.model, strlen("BMW") + 1, "BMW");
	strcpy_s(m1.numeSofer, strlen("Ion Popescu") + 1, "Ion Popescu");

	Masina m2 = { 2,2,4000.0,malloc(strlen("Audi") + 1),malloc(strlen("Elena Ionescu") + 1),'B' };
	strcpy_s(m2.model, strlen("Audi") + 1, "Audi");
	strcpy_s(m2.numeSofer, strlen("Elena Ionescu") + 1, "Elena Ionescu");

	Masina m3 = { 3, 4, 1000.0, malloc(strlen("Mercedes") + 1), malloc(strlen("George Vasile") + 1), 'C' };
	strcpy_s(m3.model, strlen("Mercedes") + 1, "Mercedes");
	strcpy_s(m3.numeSofer, strlen("George Vasile") + 1, "George Vasile");

	/*adaugaMasinaInLista(&masini, m1);
	adaugaMasinaInLista(&masini, m2);
	adaugaMasinaInLista(&masini, m3);

	printf("Lista initiala: \n");
	afisareListaMasini(masini);

	printf("\nStergem masina de pe pozitia 1:\n");
	stergeNodDePePozitia(&masini, 1);
	afisareListaMasini(masini);

	printf("\nStergem masina de pe pozitia 0:\n");
	stergeNodDePePozitia(&masini, 0);
	afisareListaMasini(masini);

	printf("\nStergem masina de pe pozitia 100:\n");
	stergeNodDePePozitia(&masini, 100);
	afisareListaMasini(masini);*/

	insereazaMasinaSortata(&masini, m1);
	insereazaMasinaSortata(&masini, m2);
	insereazaMasinaSortata(&masini, m3);

	// Afisare lista de masini
	printf("Lista dupa inserare sortata:\n");
	afisareListaMasini(masini);

	printf("\n\nLista initiala:\n");
	afisareListaMasini(masini);
	printf("\nSchimbam elementele de pe pozitiile 1 si 2:\n");
	schimbaElementelePePozitii(masini, 1, 2);
	afisareListaMasini(masini);

	printf("\n\n----------------------------\n\n");
	int nrMasini = 0;
	Masina* vectorMasini = salveazaMasiniCareIndeplinescConditie(masini, &nrMasini, 4000);
	printf("Masini cu pret >= 4000: \n");
	afisareVectorMasini(vectorMasini, nrMasini);

	dezalocareListaMasini(&masini);
	dezalocareVectorMasini(vectorMasini, nrMasini);
	return 0;
}