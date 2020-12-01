#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

	############################//############################

	Tommi Kunnari
	0543382
	1.12.2020
	Tietorakenteet ja algoritmit
	Harjoitustyö

	############################//############################

	Tämä ohjelma luo annetusta listasta numeroita
	itsensä tasapainoittavan binäärisen hakupuun ja
	tulostaa tämän. Lisäksi se esittää puuta rakentaessaan
	suoritettavat välivaiheet askel askeleelta.
	Käyttämäni tietorakenne on AVL-puu.

	Ohjelma osaa suorittaa tasapainoittamisen vaatimat operaatiot:
	rotaatiot, lapsisolmun vaihdon ja isommatkin puurakenteen
	muutokset. Ohjelma on tyypitetty vahvan rekursiivisesti.

	Työn toteuttamiseeen on käytetty Nano- tekstieditoria.

	Kurssimateriaalin lisäksi käytetyt lähteet:
		- https://www.cs.usfca.edu/~galles/visualization/AVLtree.html
		- https://www.tutorialspoint.com/data_structures_algorithms/avl_tree_algorithm.htm
		- https://www.youtube.com/watch?v=jDM6_TnYIqE&ab_channel=AbdulBari

	############################//############################

*/

/*
 	Puun solmurakenne, joka pitää sisällään solmun arvon,
	tilan puun tasapainoittamiseen, sekä osoitteet vasempaan sekä oikeaan alasolmuun.
*/
struct Node {
        int val;
        int state;
        struct Node * left;
        struct Node * right;
};

/*
	Alustetaan ohjelman käyttämät funktiot
*/

struct Node * rotateLeft(struct Node *, int *);
struct Node * rotateRight(struct Node *, int *);
void printTree(struct Node * current, int vali);

/*
 	Funktio, joka rekursiivisesti hakee solmun arvolle sopivan reitin puuhun,
	asettaa tämän tyhjälle paikalle, jonka jälkeen palaa rekursiivisesti puun
	juureen ja suorittaa tarvittavat rotaatiot solmujen tilojen mukaan.
*/

struct Node * addNode(struct Node * current, int value, int *etp) {
        struct Node * new_head = NULL;
	new_head = (struct Node*)malloc(sizeof(struct Node));

        if (current == NULL) {
		*etp = 1;
		new_head->val = value;
        	new_head->left = NULL;
		new_head->right = NULL;
		new_head->state = 0;
	        return new_head;
        }

        if (value >= current->val) {

                new_head = addNode(current->right, value, etp);
		current->right = new_head;

		if (*etp) {
			switch(current->state) {
				case 1:
					current->state = 0;
					*etp = 0;
					break;
				case 0:
					current->state = -1;
					break;
				case -1:
					current = rotateRight(current, etp);
			}
		}
        } else if (value <= current->val) {

		new_head = addNode(current->left, value, etp);
                current->left = new_head;

                if (*etp) {
                        switch(current->state) { 
                                case -1:
                                        current->state = 0;
                                        *etp = 0;
                                        break;
                                case 0:
                                        current->state = 1;
                                        break;
                                case 1:
                                        current = rotateLeft(current, etp);
			}
                }

	} else {
		*etp = 0;
		printf("Luku %d on jo puussa\n", value);
	}
	return current;
}

/*
	Funktio joka tulostaa nykyisen puun sen juuren perusteella. Tomii myös
	rotaatioiden esittämiseen, kun sille annetaan syötteeksi rotaatiohaaran
	juurisolmu.
*/

void printTree(struct Node * current, int vali) {
	int c = 10;
	if (current == NULL) {
		return; // Puun tyhjä oksa löydetty
	} else {
		vali += c;
		// Eka oikea
		printTree(current->right, vali);
		printf("\n");
		for (int i=c; i<vali; i++) {
			printf(" ");
		}
		printf("%d (%d)\n", current->val, current->state);
		// ... ja sit vasen
		printTree(current->left, vali);
	}
}

/*
	Seuraavana rotaatiofunktiot. rotateLeft ja rotateRight funktiot toimivat
	lähes samalla tavalla, mutta nimensä mukaisesti muuttavat puuta eri suuntiin.
	Solmun tilasta riippuen solmulle suoritetaan joko *L tai *R rotaatio. 
*/

struct Node * rotateLeft(struct Node * current, int *etp) {
	printf("%d", current->val);
	struct Node *child, *child1;
	child = current->left;

	if (child->state == 1) { // LL
		printf("\nLL\n");
		current->left = child->right;
		child->right = current;
		current->state = 0;
		current = child;

		printf("\nUudelleen järjestetty haara:\n");
		printTree(current, 0);

	} else { // LR
		printf("\nLR\n");
		child1 = child->right;
		child->right = child1->left;
		child1->left = child;
		current->left = child1->right;
		child1->right = current;
		
                printf("\nUudelleen järjestetty haara:\n");
                printTree(current, 0);


		switch(child1->state) {
			case 1:
				current->state = -1;
				child->state = 0;
				break;
			case 0:
				current->state = child->state = 0;
				break;
			case -1:
				current->state = 0;
				child->state = 1;
		}
		current = child1;

	}

	current->state = 0;
	*etp = 0;

	return current;
}


struct Node * rotateRight(struct Node * current, int *etp) {
	printf("%d", current->val);
        struct Node *child, *child1;
        child = current->right;

	printf("%d", current->state);

        if (child->state == -1) { // RR
                printf("\nRR\n");
		current->right = child->left;
                child->left = current;
                current->state = 0;
                current = child;

                printf("\nUudelleen järjestetty haara:\n");
                printTree(current, 0);

        } else { // RL
		printf("\nRL\n");
                child1 = child->left;
                child->left = child1->right;
                child1->right = child;
                current->right = child1->left;
                child1->left = current;

                printf("\nUudelleen järjestetty haara:\n");
                printTree(child1, 0);

                switch(child1->state) {
                        case -1:
                                current->state = -1;
                                child->state = 0;
                                break;
                        case 0:
                                current->state = child->state = 0;
                                break;
                        case 1:
                                current->state = 0;
                                child->state = 1;
                }
                current = child1;
        }

        current->state = 0;
        *etp = 0;

	return current;
}

/*
	Yksinkertainen rekursiivinen funktio, joka etsii solmun sen sisältämän
	arvon perusteella. Vaatii parametreiksi etsittävän arvon ja linkitetyn listan/
	puun juurisolmun osoitteen.
*/

struct Node * findNode(int search, struct Node * head) {
	if (head == NULL) {
                printf("\nAnnettua solmua ei löytynyt.\n");
                return NULL;
        }
	else if (search > head->val) {
		head = findNode(search, head->right);
	} else if (search < head->val) {
		head = findNode(search, head->left);
	} else if (search == head->val) {
		printf("\nSolmu löytyi!\n");
		return head;
	}
	return head;
}

void readFile(char* str) {
	FILE * file;
	if ((file = fopen("values.txt", "r")) == NULL) {
		printf("\nError reading file!\n");
		exit(1);
	}

	char data[5000];
	fscanf(file, "%[^\n]", data);

	strcpy(str, data);

	fclose(file);
}

/*
	Pääfunktio jossa määritellään rakennettavan binääripuun alkioiden arvot
	lohkomalla tiedostonlukufunktiosta saatu merkkijono int arrayksi ja
	kutsutaan jokaiselle näistä 'addNode()' funktiota.
*/

int main(int argc, char *argv[]) {

	int entries = 9;

	int etp = 0;

	struct Node * head = NULL;

	char data[5000];
	readFile(data); 
	int nodes[entries];

        char split[] = ",";
        char *ptr = strtok(data, split);

        int i=0;
        while (ptr != NULL) {
                nodes[i] = atoi(ptr);
                ptr = strtok(NULL, split);
                i++;
        }

	//int nodes[] = {2,4,6,8,10,12,14,30,28};

	printf("#######################################\nItsensä tasapainoittava AVL- binääripuu\n#######################################");

	for(int i=0; i < sizeof(nodes)/sizeof(*nodes); i++) {
		head = addNode(head, nodes[i], &etp);
		printf("\nAlkio lisätty\n");
		printTree(head, 0);
	}

	// Lisätään tehtävänannon mukaisesti luvut 26,24,22,20,18,16.
	printf("\nLisätään lisää lukuja\n");
	int new_vals[] = {26,24,22,20,18,16};
	for(int i=0; i < sizeof(new_vals)/sizeof(*new_vals); i++) {
                head = addNode(head, new_vals[i], &etp);
                printf("\nAlkio lisätty\n");
                printTree(head, 0);
        }

	printf("\nUudet alkiot lisätty.\n");
	printTree(head, 0);

	// Ensimmäinen parametri on etsittävän solmun arvo.
	struct Node * found;
	int searched_values[] = {6,1,10,16,26,32};

	for(int i=0; i < sizeof(searched_values)/sizeof(*searched_values); i++) {
		printf("####################\nEtsitään solmua %d...\n", searched_values[i]);
                found = findNode(searched_values[i], head);
        	if (found != NULL) {
                	printf("\nValue: %d State: %d\n", found->val, found->state);
        	}
	}
}
