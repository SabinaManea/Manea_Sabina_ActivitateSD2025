#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct listaDubla {
	Nod* head;
	Nod* tail;
	int nrNoduri;
};

typedef struct listaDubla listaDubla;

typedef struct NodLSI NodLSI;
struct NodLSI {
	Masina info;
	NodLSI* next;
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

//3. Realizati parcurgerile pentru lista dublu inlantuita, astfel incat sa afiseze elementele
//sortate crescator dar si sortate descrescator;
void afisareListaMasiniDeLaFinal(listaDubla listaDubla) {
	Nod* temp = listaDubla.tail;
	while (temp) {
		afisareMasina(temp->info);
		temp = temp->prev;
	}
}

void afisareListaMasiniDeLaInceput(listaDubla listaDubla) {
	Nod* temp = listaDubla.head;
	while (temp) {
		afisareMasina(temp->info);
		temp = temp->next;
	}
}

void adaugaMasinaInListaLaFinal(listaDubla* listaDubla, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	nodNou->prev = listaDubla->tail;

	if (listaDubla->head == NULL) {
		listaDubla->head = nodNou;
	}
	else {
		listaDubla->tail->next = nodNou;
	}

	listaDubla->nrNoduri++;
	listaDubla->tail = nodNou;
}

void adaugaLaInceputInLista(listaDubla* listaDubla, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->prev = NULL;
	nodNou->next = listaDubla->head;

	if (listaDubla->head == NULL) {
		listaDubla->tail = nodNou;
	}
	else {
		listaDubla->head->prev = nodNou;
	}

	listaDubla->head = nodNou;
	listaDubla->nrNoduri++;
}

listaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	listaDubla lista;
	lista.head = NULL;
	lista.tail = NULL;
	lista.nrNoduri = 0;
	FILE* f = fopen(numeFisier, "r");

	while (!feof(f)) {
		adaugaMasinaInListaLaFinal(&lista, citireMasinaDinFisier(f));
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(listaDubla* listaDubla) {
	if (listaDubla->head) {
		Nod* temp = listaDubla->head;
		while (temp->next != NULL) {
			free(temp->info.numeSofer);
			free(temp->info.model);
			temp = temp->next;
			free(temp->prev);
		}
		free(temp->info.numeSofer);
		free(temp->info.model);
		free(temp);
		listaDubla->head = NULL;
		listaDubla->tail = NULL;
		listaDubla->nrNoduri = 0;
	}
}

float calculeazaPretMediu(listaDubla listaDubla) {
	Nod* aux = listaDubla.head;
	float suma = 0;
	while (aux) {
		suma += aux->info.pret;
		aux = aux->next;
	}
	return listaDubla.nrNoduri ? suma / listaDubla.nrNoduri : 0;
}

char* getNumeSoferMasinaScumpa(listaDubla lista) { 
	Nod* temp = lista.head;
	Nod* maxim = lista.head;
	while (temp) {
		if (temp->info.pret > maxim->info.pret) {
			maxim = temp;
		}
		temp = temp->next;

	}

	char* numeCopiat = (char*)malloc(sizeof(maxim->info.numeSofer) + 1);
	strcpy_s(numeCopiat, strlen(maxim->info.numeSofer) + 1, maxim->info.numeSofer);
	return numeCopiat;
}

//1. Implementati o functie care sterge un nod de pe o pozitie data ca parametru.Daca lista
//are mai putine noduri decat index - ul dat, nu se realizeaza stergerea;
void stergeNodDePePozitia(listaDubla* lista, int index) {
	if (!lista->head || index < 0 || index >= lista->nrNoduri) 
		return;

	Nod* curent = lista->head;
	for (int i = 0; i < index; i++) {
		curent = curent->next;
	}

	if (curent->prev) {
		curent->prev->next = curent->next;
	}
	else {
		lista->head = curent->next;
	}

	if (curent->next) {
		curent->next->prev = curent->prev;
	}
	else {
		lista->tail = curent->prev;
	}

	free(curent->info.model);
	free(curent->info.numeSofer);
	free(curent);
	lista->nrNoduri--;
}

//2. Implementati o functie care sa insereze elementele in cadrul listei dublu inlantuite astfel
//incat acestea a fie sortate crescator dupa un camp la alegerea voastra;
void insereazaMasinaSortata(listaDubla* lista, Masina masinaNoua) {
	Nod* nodNou = malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = nodNou->prev = NULL;

	if (!lista->head) {
		lista->head = lista->tail = nodNou;
	}
	else {
		Nod* currentNode = lista->head;
		while (currentNode && currentNode->info.pret < masinaNoua.pret) {
			currentNode = currentNode->next;
		}
		if (!currentNode) { // la final
			lista->tail->next = nodNou;
			nodNou->prev = lista->tail;
			lista->tail = nodNou;
		}
		else if (!currentNode->prev) { // la inceput
			nodNou->next = currentNode;
			currentNode->prev = nodNou;
			lista->head = nodNou;
		}
		else { // la mijloc
			nodNou->next = currentNode;
			nodNou->prev = currentNode->prev;
			currentNode->prev->next = nodNou;
			currentNode->prev = nodNou;
		}
	}
	lista->nrNoduri++;
}

//4. Implementati o functie care salveaza intr - o lista simplu inlantuita toate obiectele care
//indeplinesc o conditie stabilita de voi.Realizati deep copy, astfel incat elementele din
//listasimplu inlantuita sa fie diferite de cele din lista dublu inlantuita.Observati diferenta
//dintre utilizarea de lista inlantuita si vector(task - ul precedent);
NodLSI* deepCopy(listaDubla lista) {
	float medie = calculeazaPretMediu(lista);
	NodLSI* cap = NULL;
	NodLSI* tailSI = NULL;
	Nod* currentNode = lista.head;
	while (currentNode) {
		if (currentNode->info.pret > medie) {
			Masina cop = currentNode->info;
			cop.model = malloc(strlen(currentNode->info.model) + 1);
			strcpy(cop.model, currentNode->info.model);
			cop.numeSofer = malloc(strlen(currentNode->info.numeSofer) + 1);
			strcpy(cop.numeSofer, currentNode->info.numeSofer);

			NodLSI* nodSI = malloc(sizeof(NodLSI));
			nodSI->info = cop;
			nodSI->next = NULL;
			if (!cap) cap = nodSI;
			else tailSI->next = nodSI;
			tailSI = nodSI;
		}
		currentNode = currentNode->next;
	}
	return cap;
}

void afisareListaSimpla(NodLSI* cap) {
	while (cap) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

//5. Implementati o functie care primeste lista dublu inlantuita si doua pozitii.In cadrul
//functiei trebuie sa interschimbati elementele de pe cele doua pozitii din lista primita.
//Interschimbati informatiile utile din cele doua noduri.
void schimbaPozitia(listaDubla* lista, int poz1, int poz2) {
	if (poz1 == poz2) {
		return;
	}
	if (poz1 < 0 || poz2 < 0 || poz1 >= lista->nrNoduri || poz2 >= lista->nrNoduri) {
		return;
	}

	if (poz1 > poz2) { 
		int t = poz1; poz1 = poz2; poz2 = t; 
	}

	Nod* n1 = lista->head;
	for (int i = 0; i < poz1; i++) {
		n1 = n1->next;
	}

	Nod* n2 = n1;
	for (int i = poz1; i < poz2; i++) {
		n2 = n2->next;
	}

	Masina tmp = n1->info;
	n1->info = n2->info;
	n2->info = tmp;
}

int main() {
	listaDubla lista;
	lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaFinal(lista);
	printf("\n--------------------------------------\n");
	afisareListaMasiniDeLaInceput(lista);

	printf("\n--------------------------------------\n");
	float pretMediu = 0;
	pretMediu = calculeazaPretMediu(lista);
	printf("Pretul mediu este: %.2f", pretMediu);

	printf("\n--------------------------------------\n");
	char* numeReturnat = getNumeSoferMasinaScumpa(lista);
	printf("Nume returnat: %s", numeReturnat);

	printf("\n--------------------------------------\n");
	stergeNodDePePozitia(&lista, 2);
	printf("\n\nDupa stergere poz 2:\n");
	afisareListaMasiniDeLaInceput(lista);

	Masina mNou;
	mNou.id = 11;
	mNou.nrUsi = 4;
	mNou.pret = 35000;
	mNou.model = (char*)malloc(strlen("Clio") + 1);
	strcpy_s(mNou.model, strlen("Clio") + 1, "Clio");
	mNou.numeSofer = (char*)malloc(strlen("Ion Popescu") + 1);
	strcpy_s(mNou.numeSofer, strlen("Ion Popescu") + 1, "Ion Popescu");
	mNou.serie = 'X';
	insereazaMasinaSortata(&lista, mNou);
	printf("\nDupa insertie sortata:\n");
	afisareListaMasiniDeLaInceput(lista);

	printf("\n--- Parcurgere crescator ---\n");
	afisareListaMasiniDeLaInceput(lista);

	printf("\n--- Parcurgere descrescator ---\n");
	afisareListaMasiniDeLaFinal(lista);

	NodLSI* simpla = deepCopy(lista);
	printf("\nLista simpla (pret > media):\n");
	afisareListaSimpla(simpla);
	while (simpla) {
		free(simpla->info.model);
		free(simpla->info.numeSofer);
		NodLSI* tmp = simpla;
		simpla = simpla->next;
		free(tmp);
	}

	schimbaPozitia(&lista, 0, 1);
	printf("\nDupa swap - 0 si 1:\n");
	afisareListaMasiniDeLaInceput(lista);

	dezalocareLDMasini(&lista);
	return 0;
}