#include <stdio.h>
#include <stdlib.h>

/*

	############################

	Tommi Kunnari
	0543382
	25.11.2020
	Tietorakenteet ja algoritmit
	Harjoitustyö

	############################

	Tämä ohjelma luo annetusta listasta numeroita
	itsensä tasapainoittavan binäärisen hakupuun ja
	tulostaa tämän. Lisäksi se esittää puuta rakentaessaan
	suoritettavat välivaiheet askel askeleelta.
	Käyttämäni tietorakenne on AVL-puu.

	Ohjelma osaa suorittaa tasapainoittamisen vaatimat operaatiot:
	rotaatiot, lapsisolmun vaihdon ja isommatkin puurakenteen
	muutokset. Ohjelma on tyypitetty vahvan rekursiivisesti.

	Työssä on käytetty Nano- tekstieditoria.

	############################

*/

struct Node {
        int val;
        int state;
        struct Node * left;
        struct Node * right;
};

struct Node * rotateLeft(struct Node *, int *);
struct Node * rotateRight(struct Node *, int *);
void printTree(struct Node * current, int vali);



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

//	printf("%d", current->val);

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
                                        current->state = -1;
                                        break;
                                case 1:
                                        current = rotateLeft(current, etp);
			}
                }

		/*
		new_head = addNode(current->left, value, etp);
		current->left = new_head;
		//printTree(current, 0);
		return current;
		*/
	} else {
		*etp = 0;
		printf("Luku %d on jo puussa\n", value);
	}
	return current;
}

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
		printf("%d\n", current->val);
		// ... ja sit vasen
		printTree(current->left, vali);
	}
}

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
                printTree(current, 0);

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


int main(int argc, char *argv[]) {

	int etp = 0;

	struct Node * head = NULL;

	// The list of numbers that the binary tree will be constructed from
	int nodes[] = {2,4,6,8,10,12,14,30,28};
	printf("Self-Balancing Binary Search Tree\n");

	for(int i=0; i < sizeof(nodes)/sizeof(*nodes); i++) {
		head = addNode(head, nodes[i], &etp);
		printf("\nAlkio lisätty\n");
		printTree(head, 0);
	}

}
