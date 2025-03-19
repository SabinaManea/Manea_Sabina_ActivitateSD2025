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
	Masina masina;
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

void afisareListaMasini(Nod* head) {
	Nod* temp = head; // de aici incepe
	while (temp != NULL) {
		afisareMasina(temp->masina); // aici afiseaza pe rand fiecare nod
		temp = temp->next;
	}
}

void adaugaMasinaInLista(Nod** head, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod)); // pentru noua masina aloca memorie
	nou->masina = masinaNoua;
	nou->next = NULL;

	if (*head == NULL) { // daca nu sunt elemente in lista, noul nod devine primul
		*head = nou;
	} else { // daca sunt elemente in lista, noul nod seadauga la final
		Nod* temp = *head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void adaugaLaInceputInLista(Nod** head, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->masina = masinaNoua;
	nou->next = *head; // a doua pozitie devine capul de la lista initiala
	*head = nou; // head devine nodul pe care il introducem
}

void* citireListaMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (!file) {
		printf("Eroare la deschiderea fisierului!\n");
		return NULL;
	} 

	Nod* head = NULL;
	while (!feof(file)) {
		Masina masina = citireMasinaDinFisier(file);
		if (masina.model != NULL) {
			adaugaMasinaInLista(&head, masina);
		}
	}
	fclose(file);
	return head;
}

void dezalocareListaMasini(Nod** head) {
	Nod* temp;
	// Parcurge lista și eliberează memoria fiecărui nod și a câmpurilor char*
	while (*head != NULL) {
		temp = *head;
		*head = (*head)->next;
		free(temp->masina.model);
		free(temp->masina.numeSofer);
		free(temp);
	}
}

float calculeazaPretMediu(Nod* head) {
	int nr = 0;
	float suma = 0;
	Nod* temp = head;
	while (temp != NULL) {
		suma += temp->masina.pret;
		nr++;
		temp = temp->next;
	}
	return nr > 0 ? suma / nr : 0;
}

void stergeMasiniDinSeria(Nod** head, char serieCautata) {
	while (*head && (*head)->masina.serie == serieCautata) {
		Nod* temp = *head;
		*head = (*head)->next;
		free(temp->masina.model);
		free(temp->masina.numeSofer);
		free(temp);
	}
	Nod* curent = *head;
	while (curent && curent->next) {
		if (curent->next->masina.serie == serieCautata) {
			Nod* temp = curent->next;
			curent->next = curent->next->next;
			free(temp->masina.model);
			free(temp->masina.numeSofer);
			free(temp);
		} else {
			curent = curent->next;
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* head, const char* numeSofer) {
	float total = 0;
	Nod* temp = head;
	while (temp != NULL) {
		if (strcmp(temp->masina.numeSofer, numeSofer) == 0) {
			total += temp->masina.pret;
		}
		temp = temp->next;
	}
	return total;
}

int main() {
	const char* numeFisier = "masini.txt";

	// 1. Citim lista de mașini din fișier
	Nod* listaMasini = citireListaMasiniDinFisier(numeFisier);

	// 2. Afișăm lista inițială
	printf("Lista initiala de masini:\n");
	afisareListaMasini(listaMasini);

	// 3. Calculăm și afișăm prețul mediu al mașinilor
	float pretMediu = calculeazaPretMediu(listaMasini);
	printf("Pretul mediu al masinilor: %.2f\n", pretMediu);

	// 4. Ștergem mașinile cu seria 'A' și afișăm lista actualizată
	char serieDeSters = 'A';
	stergeMasiniDinSeria(&listaMasini, serieDeSters);

	printf("\nLista dupa stergerea masinilor cu seria '%c':\n", serieDeSters);
	afisareListaMasini(listaMasini);

	// 5. Testăm ștergerea unei serii inexistente
	char serieInexistenta = 'X';
	stergeMasiniDinSeria(&listaMasini, serieInexistenta);

	printf("\nLista dupa incercarea de a sterge masinile cu seria '%c' (care nu exista):\n", serieInexistenta);
	afisareListaMasini(listaMasini);

	// 6. Calculăm și afișăm prețul total al mașinilor unui anumit șofer
	const char* soferCautat = "Mihai Georgescu";  // Exemplu: căutăm mașinile acestui șofer
	float pretTotalSofer = calculeazaPretulMasinilorUnuiSofer(listaMasini, soferCautat);
	printf("\nPretul total al masinilor soferului %s: %.2f\n", soferCautat, pretTotalSofer);

	// 7. Dezalocăm memoria
	dezalocareListaMasini(&listaMasini);

	return 0;
}