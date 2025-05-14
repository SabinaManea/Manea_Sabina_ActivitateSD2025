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

//STACK - last in first out - o stiva de farfurii

//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector -- folosim lista simpla inlantuita 

typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* next;
};

void pushStack(Nod** stiva, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = (*stiva);
	(*stiva) = nou;
}

Masina popStack(Nod** stiva) {
	if ((*stiva) != NULL) {
		Masina nou = (*stiva)->info;
		Nod* prev = (*stiva);
		(*stiva) = (*stiva)->next;
		free(prev);
		return nou;
	}
	Masina m;
	m.id = -1;
	return m;
}

unsigned char emptyStack(Nod** stiva) {
	return stiva == NULL;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	Nod* stiva = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		pushStack(&stiva, m);
	}
	fclose(f);
	return stiva;
}

void dezalocareStivaDeMasini(Nod** stiva) {
	//sunt dezalocate toate masinile si stiva de elemente
	while ((*stiva) != NULL) {
		Masina m = popStack(stiva);
		free(m.numeSofer);
		free(m.model);
	}
}

int stackSize(Nod* stiva) {
	//returneaza numarul de elemente din stiva
	int nr = 0;
	while (stiva != NULL) {
		nr++;
		stiva = stiva->next;
	}
	return nr;
}

//QUEUE -- first in first out - o coada la magazin
typedef struct NodDublu NodDublu;
struct NodDublu {
	Masina info;
	NodDublu* next;
	NodDublu* prev;
};

typedef struct ListaDubla ListaDubla;
struct ListaDubla {
	NodDublu* prim;
	NodDublu* ultim;
};

//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector
void enqueue(ListaDubla* coada, Masina masina) {
	NodDublu* nou = malloc(sizeof(NodDublu));
	nou->info = masina;
	nou->next = NULL;
	nou->prev = coada->ultim;   

	if (coada->ultim != NULL) {
		coada->ultim->next = nou;
	}
	else {
		coada->prim = nou;        
	}

	coada->ultim = nou;           
}

Masina dequeue(ListaDubla* lista) {
	//extrage o masina din coada
	Masina m;
	m.id = -1;
	if (lista->prim != NULL) {
		m = lista->prim->info;
		NodDublu* aux = lista->prim;
		if (lista->prim->next != NULL) {
			lista->prim = lista->prim->next;
			lista->prim->prev = NULL;
		}
		else {
			lista->prim = NULL;
			lista->ultim = NULL;
		}
		free(aux);
	}
	return m;
}

ListaDubla citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		enqueue(&lista, m);
	}
	fclose(f);
	return lista;
}

void dezalocareCoadaDeMasini(ListaDubla* lista) {
	//sunt dezalocate toate masinile si coada de elemente
	NodDublu* aux = lista->ultim;
	while (aux) {
		NodDublu* temp = aux;
		aux = aux->prev;
		free(temp->info.numeSofer);
		free(temp->info.model);
		free(temp);
	}
	lista->prim = NULL;
	lista->ultim = NULL;
}

//metode de procesare
Masina getMasinaByID(/*stiva sau coada de masini*/int id) {

}

float calculeazaPretTotal(ListaDubla* coada) {
	float suma = 0;
	ListaDubla nou;
	nou.prim = NULL;
	nou.ultim = NULL;
	while ((*coada).prim) {
		Masina m = dequeue(coada);
		suma += m.pret; 
		enqueue(&nou, m);
	}
	coada->prim = nou.prim;
	coada->ultim = nou.ultim;
	return suma;
}

int main() {
	/*Nod* stiva = citireStackMasiniDinFisier("masini.txt");
	int nr = stackSize(stiva);
	printf("Nr de elemente in stiva: %d\n\n", nr);

	Masina m = popStack(&stiva);
	afisareMasina(m);
	free(m.numeSofer);
	free(m.model);

	m = popStack(&stiva);
	afisareMasina(m);*/

	// COADA
	ListaDubla coada = citireCoadaDeMasiniDinFisier("masini.txt");
	Masina m = dequeue(&coada);
	afisareMasina(m);
	printf("Suma preturi: %.2f", calculeazaPretTotal(&coada));
	return 0;
}