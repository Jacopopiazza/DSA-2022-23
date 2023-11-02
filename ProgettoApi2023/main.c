//
//  main.c
//  ProgettoApi2023
//
//  Created by Japo on 06/09/23.
//

#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#define RED   'R'
#define BLACK 'B'

typedef struct car{
    int km;
    struct car* next;
} Car;

typedef struct Station{
    int km;
    Car* cars;
} Station;

typedef struct Node{
    Station      station;
    char         color;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} RBT_Node;

typedef struct Tappa{
    struct Tappa *next;
    struct Tappa *prec;
    RBT_Node* node;
} Tappa_Percorso;

//LEAF OF RBT
RBT_Node T_Nil_Node;
RBT_Node* T_Nil = &T_Nil_Node;

//RBT ROOT
RBT_Node* ROOT = NULL;

//PERCORSO
Tappa_Percorso* PERCORSO = NULL;

//RBT FUNCTIONS
RBT_Node* newNode(int km);
int insert(RBT_Node** root, RBT_Node* z);
void insertFixup(RBT_Node** root, RBT_Node* z);
void leftRotate(RBT_Node** root, RBT_Node* x);
void rightRotate(RBT_Node** root, RBT_Node* x);
void rbTransplant(RBT_Node** root, RBT_Node *u, RBT_Node *v);
int delete(RBT_Node** root, RBT_Node* z);
void deleteFixup(RBT_Node** root, RBT_Node* x);
RBT_Node* treeMinimum(RBT_Node *x);
RBT_Node* treeMaximum(RBT_Node *x);
RBT_Node* treeSuccessor(RBT_Node *x);
RBT_Node* treePredecessor(RBT_Node *x);
RBT_Node* treeSearch(RBT_Node *x,int km);
void inorder_tree_walk(RBT_Node* root);

//Double-Linked List
Tappa_Percorso* inserisciTappa(Tappa_Percorso** inizio, RBT_Node* nodo);
void rimuoviUltimaTappa(Tappa_Percorso** inizio);
void cancellaPercorso(Tappa_Percorso** inizio);
void stampaPercorso(Tappa_Percorso *inizio);

//Project Functions
int autonomiaStazione(RBT_Node *node);
int aggiungiAuto(RBT_Node *node, int km);
int rimuoviAuto(RBT_Node *node, int km);
void stampaAuto(RBT_Node* node);
void distruggiStazione(RBT_Node *node);
int pianificaPercorso(RBT_Node *root, int kmInizio, int kmFine);
int pianificaPercorsoForward(RBT_Node *root, int kmInizio, int kmFine);
int pianificaPercorsoReverse(RBT_Node *root, int kmInizio, int kmFine);

int main(int argc, const char * argv[]) {

    ROOT = T_Nil;

    char input[25000];
    char *token;
    char *delimiter = " ";

    while(fgets(input, sizeof(input), stdin) != NULL){

        input[strcspn(input, "\n")] = '\0';

        char *strp = input;
        token = strsep(&strp, delimiter);

        if(strcmp(token, "aggiungi-stazione") == 0){
            token = strsep(&strp, delimiter);
            int kmStazione = atoi(token);

            RBT_Node* nuovoNodo = newNode(kmStazione);
            if(insert(&ROOT, nuovoNodo) < 0){
                printf("non aggiunta\n");
                continue;
            }

            token = strsep(&strp, delimiter);
            int nAuto = atoi(token);

            for(int i = 0;i<nAuto;i++){
                token = strsep(&strp, delimiter);
                int kmAuto = atoi(token);
                //Aggiungo auto
                aggiungiAuto(nuovoNodo, kmAuto);
            }
            printf("aggiunta\n");
            continue;
        }
        else if(strcmp(token, "demolisci-stazione") == 0){
            token = strsep(&strp, delimiter);
            int kmStazione = atoi(token);

            if(delete(&ROOT, treeSearch(ROOT, kmStazione)) > 0){
                printf("demolita\n");
                continue;
            }
            else{
                printf("non demolita\n");
                continue;
            }

        }
        else if(strcmp(token, "aggiungi-auto") == 0){
            token = strsep(&strp, delimiter);
            int kmStazione = atoi(token);

            RBT_Node *stazione = treeSearch(ROOT, kmStazione);
            if(stazione == T_Nil){
                printf("non aggiunta\n");
                continue;
            }

            token = strsep(&strp, delimiter);
            int kmAuto = atoi(token);
            aggiungiAuto(stazione, kmAuto);
            printf("aggiunta\n");
            continue;

        }
        else if(strcmp(token, "rottama-auto") == 0){
            token = strsep(&strp, delimiter);
            int kmStazione = atoi(token);

            RBT_Node *stazione = treeSearch(ROOT, kmStazione);
            if(stazione == T_Nil){
                printf("non rottamata\n");
                continue;
            }

            token = strsep(&strp, delimiter);
            int kmAuto = atoi(token);
            if(rimuoviAuto(stazione, kmAuto) > 0){
                printf("rottamata\n");
                continue;
            }
            else{
                printf("non rottamata\n");
                continue;
            }
        }
        else{
            //LESSSSS DOIT
            token = strsep(&strp, delimiter);
            int kmStazioneInizio = atoi(token);

            token = strsep(&strp, delimiter);
            int kmStazioneFine = atoi(token);

            int ris = pianificaPercorso(ROOT, kmStazioneInizio, kmStazioneFine);

            if(ris > 0){
                stampaPercorso(PERCORSO);
                cancellaPercorso(&PERCORSO);
                continue;
            }
            else if(ris < 0){
                cancellaPercorso(&PERCORSO);
                printf("nessun percorso\n");
                continue;
            }
            else{
                printf("%d-%d NOT IMPLEMENTED YET\n", kmStazioneInizio,kmStazioneFine);
                cancellaPercorso(&PERCORSO);
                continue;
            }

        }

    }



    return 0;
}

RBT_Node* newNode(int km)
{
    //Allocating space
    RBT_Node *temp   = (RBT_Node*) malloc(sizeof(RBT_Node));
    //Error in allocating space
    if(temp == NULL) return NULL;

    //Creating Station
    temp->station.cars = NULL;
    temp->station.km = km;

    //RBT operations
    temp->color  = RED;
    temp->left   = T_Nil;
    temp->right  = T_Nil;
    temp->parent = T_Nil;

    return temp;
}

int insert(RBT_Node** root, RBT_Node* z){

    RBT_Node *x = *root, *y = T_Nil;

    while( x != T_Nil){ //descend until reaching the sentinel
        y = x;
        if(z->station.km == x->station.km){
            return -1;
        }
        else if (z->station.km < x->station.km){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }
    z->parent = y; //found the location insert z with parent y
    if(y == T_Nil){
        *root = z;   //tree T was empty
    }
    else if (z->station.km < y->station.km){
        y->left = z;
    }
    else{
        y->right = z;
    }
    z->left = T_Nil; //both of z's children are the sentinel
    z->right = T_Nil;
    z->color = RED; //the new node is set red
    insertFixup(root, z); //correct any violations of red-black properties
    return 1;
}

void insertFixup(RBT_Node** root, RBT_Node* z){

    RBT_Node *y;

    while(z != NULL && z->parent != NULL && z->parent->color == RED){

        //Is z's parent a left child?
        if(z->parent->parent != NULL && z->parent == z->parent->parent->left){
            //y is z's uncle
            y = z->parent->parent->right;

            //are z's parent and uncle both red?
            if(y != NULL && y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else{
                //Z is a right child
                if (z == z->parent->right){
                    z = z->parent;
                    leftRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(root, z->parent->parent);
            }

        }
        else if(z->parent->parent != NULL && z->parent == z->parent->parent->right){

            y = z->parent->parent->left;

            if(y!=NULL && y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else{
                //Z is a left child
                if(z == z->parent->left){
                    z = z->parent;
                    rightRotate(root, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(root, z->parent->parent);
            }
        }
    }

    (*root)->color = BLACK;
}

void leftRotate(RBT_Node** root, RBT_Node* x){
    RBT_Node *y;

    y = x->right;
    x->right = y->left;
    if(y->left != T_Nil){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == T_Nil){
        *root = y;
    }
    else if(x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;

}

void rightRotate(RBT_Node** root, RBT_Node* x){
    RBT_Node *y;

    y = x->left;
    x->left = y->right;
    if(y->right != T_Nil){
        y->right->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == T_Nil){
        *root = y;
    }
    else if(x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }
    y->right = x;
    x->parent = y;

}

void rbTransplant(RBT_Node** root, RBT_Node *u, RBT_Node *v){
    if(u->parent == T_Nil){
        *root = v;
    }
    else if(u->parent != NULL && u == u->parent->left){
        u->parent->left = v;
    }
    else if(u->parent->right != NULL){
        u->parent->right = v;
    }
    v->parent = u->parent;
}

int delete(RBT_Node** root, RBT_Node* z){
    RBT_Node *x, *y;
    char y_original_color;

    if(z == T_Nil) return -1;
    if(*root == T_Nil) return -1;

    y = z;
    y_original_color = y->color;

    if(z->left ==T_Nil){
        x = z->right;
        rbTransplant(root, z, z->right);
    }
    else if(z->right == T_Nil){
        x = z->left;
        rbTransplant(root, z, z->left);
    }
    else{
        y = treeMinimum(z->right);
        y_original_color = y->color;
        x = y->right;

        if( y != z->right){
            rbTransplant(root, y, y->right);
            y->right = z->right;
            //TODO: Is a check needed on right child?
            y->right->parent = y;
        }
        else{
            x->parent = y;
        }
        rbTransplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if(y_original_color == BLACK){
        deleteFixup(root, x);
    }
    distruggiStazione(z);
    return 1;

}

RBT_Node* treeMinimum(RBT_Node *x){
    while(x->left != T_Nil){
        x = x->left;
    }
    return x;
}

RBT_Node* treeMaximum(RBT_Node *x){
    while(x->right != T_Nil){
        x = x->right;
    }
    return x;
}

RBT_Node* treeSuccessor(RBT_Node *x){
    RBT_Node *y;
    if(x->right != T_Nil){
        return treeMinimum(x->right);
    }
    else{
        y=x->parent;
        while (y != T_Nil && x == y->right) {
            x=y;
            y=y->parent;
        }
        return y;
    }
}

RBT_Node* treePredecessor(RBT_Node *x){
    RBT_Node *y;
    if(x->left != T_Nil){
        return treeMaximum(x->left);
    }
    else{
        y=x->parent;
        while (y != T_Nil && x == y->left) {
            x=y;
            y=y->parent;
        }
        return y;
    }
}

RBT_Node* treeSearch(RBT_Node *x,int km){
    if (x == T_Nil || x->station.km == km){
        return x;
    }
    if(km < x->station.km){
        return treeSearch(x->left, km);
    }
    return treeSearch(x->right, km);
}

void deleteFixup(RBT_Node** root, RBT_Node* x){

    RBT_Node *w;

    //TODO: CHECK WHATS HAPPENING HERE!!!
    if(x == T_Nil) return;

    while(x != *root && x != NULL && x->color == BLACK){

        if(x->parent != NULL && x == x->parent->left){
            w = x->parent->right;
            if(w != NULL && w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(root, x->parent);
                w = x->parent->right;
            }

            if(w != NULL && w->left != NULL && w->left->color == BLACK && w->right != NULL && w->right->color == BLACK){
                w->color = RED;
                x = x->parent;
            }
            else if(w != NULL && w->left != NULL && w->right != NULL){
                if(w->right != NULL && w->right->color == BLACK){
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(root, x->parent);
                x = *root;
            }
            else break;

        }
        else if(x->parent != NULL && x == x->parent->right){
            w = x->parent->left;
            if(w != NULL && w->color == RED){
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(root, x->parent);
                w = x->parent->left;
            }

            if(w != NULL && w->right != NULL && w->right->color == BLACK && w->left != NULL && w->left->color == BLACK){
                w->color = RED;
                x = x->parent;
            }
            else if(w!= NULL && w->right != NULL && w->left != NULL){
                if(w->left != NULL && w->left->color == BLACK){
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(root, x->parent);
                x = *root;
            }
            else break;

        }

    }
    x->color = BLACK;
}

void inorder_tree_walk(RBT_Node* root){
    if(root != T_Nil){
        inorder_tree_walk(root->left);
        printf("%d\n",root->station.km);
        inorder_tree_walk(root->right);
    }
}


int autonomiaStazione(RBT_Node *node){

    if(node == T_Nil) return 0;
    if(node->station.cars == NULL) return 0;
    return node->station.cars->km;

}

int aggiungiAuto(RBT_Node *node, int km){

    //No auto
    if(node->station.cars == NULL){
        node->station.cars = (Car*)malloc(sizeof(Car));
        node->station.cars->km = km;
        node->station.cars->next = NULL;
        return 1;
    }

    Car* temp = node->station.cars;

    if(temp->km < km){
        Car* temp2 = (Car*)malloc(sizeof(Car));
        temp2->km = km;
        temp2->next = temp;
        node->station.cars = temp2;
        return 1;
    }

    while(temp->next != NULL && temp->next->km > km){
        temp = temp->next;
    }

    if(temp->next == NULL && temp->km > km){
        temp->next = (Car*)malloc(sizeof(Car));
        temp->next->km = km;
        temp->next->next = NULL;
        return 1;
    }
    else if(temp->next == NULL && temp->km < km){
        Car* temp2 = (Car*)malloc(sizeof(Car));
        temp2->next = temp;
        temp2->km = km;
        node->station.cars = temp2;
        return 1;
    }
    else{
        Car* temp2 = (Car*)malloc(sizeof(Car));
        temp2->next = temp->next;
        temp->next = temp2;
        temp2->km = km;
        return 1;
    }

}

int rimuoviAuto(RBT_Node *node, int km){
    Car* car = node->station.cars;

    if(car == NULL) return -1;
    if(car->km < km) return -1;
    if(car->km == km){
        node->station.cars = node->station.cars->next;
        free(car);
        return 1;
    }

    while(car->next != NULL && car->next->km > km){
        car = car->next;
    }
    if(car->next == NULL){
        return -1;
    }
    if(car->next->km < km) return -1;
    if(car->next->km == km){
        Car *addr = car->next;
        car->next = car->next->next;
        free(addr);
        return 1;
    }

    return -2;
}

void stampaAuto(RBT_Node* node){
    Car* c = node->station.cars;

    while(c != NULL){
        printf("%d ,", c->km);
        c = c->next;
    }
    printf("\n");
}

void distruggiStazione(RBT_Node *node){

    Car* car = node->station.cars;


    while(car != NULL){
        Car* temp = car->next;
        free(car);
        car = temp;
    }

    free(node);
}

Tappa_Percorso* inserisciTappa(Tappa_Percorso** inizio, RBT_Node* nodo){

    if(*inizio == NULL){
        *inizio = (Tappa_Percorso*)malloc(sizeof(Tappa_Percorso));
        (*inizio)->next = NULL;
        (*inizio)->prec = NULL;
        (*inizio)->node = nodo;
        return *inizio;
    }
    Tappa_Percorso *temp = *inizio;
    while(temp->next != NULL){
        temp = temp->next;
    }

    temp->next = (Tappa_Percorso*)malloc(sizeof(Tappa_Percorso));
    temp->next->next = NULL;
    temp->next->prec = temp;
    temp->next->node = nodo;
    return temp->next;
}

void cancellaPercorso(Tappa_Percorso** inizio){
    Tappa_Percorso* tappa = *inizio;

    while(tappa != NULL){
        Tappa_Percorso* temp = tappa->next;
        free(tappa);
        tappa = temp;
    }

    *inizio = NULL;
}

void stampaPercorso(Tappa_Percorso *inizio){
    Tappa_Percorso* tappa = inizio;
    while(tappa != NULL){

        printf("%d", tappa->node->station.km);
        if(tappa->next != NULL) printf(" ");

        tappa = tappa->next;
    }
    printf("\n");
}

int pianificaPercorso(RBT_Node *root, int kmInizio, int kmFine){

    if(kmInizio == kmFine){
        inserisciTappa(&PERCORSO, treeSearch(ROOT, kmInizio));
        return 1;
    }
    else if(kmInizio < kmFine){
        return pianificaPercorsoForward(root, kmInizio, kmFine);
    }
    else{
        return pianificaPercorsoReverse(root, kmInizio, kmFine);
    }
}

int pianificaPercorsoForward(RBT_Node *root, int kmInizio, int kmFine){

    char end = 0;
    RBT_Node *nodoInizio = treeSearch(root, kmInizio);
    RBT_Node *nodoFine = treeSearch(root, kmFine);
    int limite = nodoInizio->station.km + autonomiaStazione(nodoInizio);

    //Dalla prima stazione arrivo direttamente a destinazione
    if(limite >= kmFine){
        inserisciTappa(&PERCORSO, nodoInizio);
        inserisciTappa(&PERCORSO, nodoFine);
        return 1;
    }

    //Preparo elementi
    int currMax = kmInizio;
    RBT_Node *currMaxNode = NULL;
    RBT_Node *currNode = treeSuccessor(nodoInizio);
    char foundNext = 0;

    //Aggiungo stazione di inizio nel percorso
    inserisciTappa(&PERCORSO, nodoInizio);

    //Cerco percorso con numero minimo di tappe
    while(end == 0 && currNode != T_Nil){
        foundNext = 0;
        while(currNode != T_Nil && currNode->station.km <= limite){
            //Se arriva più lontano cambio stazione per la tappa

            int maxDist = currNode->station.km + autonomiaStazione(currNode);

            if(maxDist > currMax || currMaxNode == NULL || (maxDist == currMax && currNode->station.km < currMaxNode->station.km)){
                foundNext = 1;
                currMax = maxDist;
                currMaxNode = currNode;

                //Perocrso trovato
                if(currMax >= kmFine){
                    end = 1;
                    break;
                }
            }

            currNode = treeSuccessor(currNode);
        }

        //Ho finito l'albero, il percorso non c'è...
        if(currNode == T_Nil) return -1;
        //Tutte le stazioni raggiungibili sono troppo lontane dalle altre
        if(foundNext==0 && currNode->station.km > limite) return -1;

        //Tappa trovata, resetto puntatori
        inserisciTappa(&PERCORSO, currMaxNode);
        currMax = currMaxNode->station.km;
        currNode = treeSuccessor(currMaxNode);
        limite = currMaxNode->station.km + autonomiaStazione(currMaxNode);
        currMaxNode = NULL;
    }

    //Nessuna altra stazione raggiungibile
    if(end == 0) return -1;

    Tappa_Percorso *last = inserisciTappa(&PERCORSO, nodoFine);
    Tappa_Percorso *toBeOptimized = last->prec;


    //Fino alla seconda tappa
    while(toBeOptimized != NULL && toBeOptimized->prec != NULL){

        int lowerBound = toBeOptimized->prec->node->station.km;
        int upperBound = toBeOptimized->node->station.km;

        int kmToReach = last->node->station.km;

        //RBT_Node* node = treeSearch(ROOT, toBeOptimized->prec->node->station.km);
        RBT_Node* node = toBeOptimized->prec->node;
        node = treeSuccessor(node);

        while(node->station.km > lowerBound && node->station.km < upperBound){
            //Ho trovato una stazione migliore
            if(node->station.km + autonomiaStazione(node) >= kmToReach && node->station.km < toBeOptimized->node->station.km){
                toBeOptimized->node = node;
            }
            //continuo a cercare
            node = treeSuccessor(node);
        }

        //Ottimizzo la tappa precedente
        last = last->prec;
        toBeOptimized = toBeOptimized->prec;

    }


    return 1;
}

void rimuoviUltimaTappa(Tappa_Percorso** inizio){
    Tappa_Percorso *point = *inizio;
    if(point == NULL) return;
    if(point->next == NULL){
        free(point);
        *inizio = NULL;
        return;
    }

    while(point->next->next != NULL){
        point = point->next;
    }

    free(point->next);
    point->next = NULL;

}


int pianificaPercorsoReverse(RBT_Node *root, int kmInizio, int kmFine){

    char end = 0;
    RBT_Node *nodoInizio = treeSearch(root, kmInizio);
    RBT_Node *nodoFine = treeSearch(root, kmFine);
    int limite = nodoInizio->station.km - autonomiaStazione(nodoInizio);

    //Dalla prima stazione arrivo direttamente a destinazione
    if(limite <= kmFine){
        inserisciTappa(&PERCORSO, nodoInizio);
        inserisciTappa(&PERCORSO, nodoFine);
        return 1;
    }

    //Preparo elementi
    int currMin = kmInizio;
    RBT_Node *currMinNode = NULL;
    RBT_Node *currNode = treePredecessor(nodoInizio);
    char foundNext = 0;

    //Aggiungo stazione di inizio nel percorso
    inserisciTappa(&PERCORSO, nodoInizio);

    //Cerco percorso con numero minimo di tappe
    while(end == 0 && currNode != T_Nil){
        foundNext = 0;
        while(currNode != T_Nil && currNode->station.km >= limite){
            //Se arriva più lontano cambio stazione per la tappa

            int maxDist = currNode->station.km - autonomiaStazione(currNode);

            if(maxDist < currMin || currMinNode == NULL || (maxDist == currMin && currNode->station.km < currMinNode->station.km)){
                foundNext = 1;
                currMin = maxDist;
                currMinNode = currNode;

                //Perocrso trovato
                if(currMin <= kmFine){
                    end = 1;
                    break;
                }
            }

            currNode = treePredecessor(currNode);
        }

        //Ho finito l'albero, il percorso non c'è...
        if(currNode == T_Nil) return -1;
        //Tutte le stazioni raggiungibili sono troppo lontane dalle altre
        if(foundNext==0 && currNode->station.km < limite) return -1;

        //Tappa trovata, resetto puntatori
        inserisciTappa(&PERCORSO, currMinNode);
        currMin = currMinNode->station.km;
        currNode = treePredecessor(currMinNode);
        limite = currMinNode->station.km - autonomiaStazione(currMinNode);
        currMinNode = NULL;
    }

    //Nessuna altra stazione raggiungibile
    if(end == 0) return -1;

    Tappa_Percorso *last = inserisciTappa(&PERCORSO, nodoFine);
    Tappa_Percorso *toBeOptimized = last->prec;

    //Fino alla seconda tappa
    while(toBeOptimized != NULL && toBeOptimized->prec != NULL){

        int lowerBound = toBeOptimized->prec->node->station.km - autonomiaStazione(toBeOptimized->prec->node);
        int upperBound = toBeOptimized->node->station.km;

        int kmToReach = last->node->station.km;

        //RBT_Node* node = treeSearch(ROOT, toBeOptimized->node->station.km);
        RBT_Node* node = toBeOptimized->node;
        node = treePredecessor(node);

        while(node->station.km >= lowerBound && node->station.km < upperBound){
            //Ho trovato una stazione migliore
            if(node->station.km - autonomiaStazione(node) <= kmToReach && node->station.km < toBeOptimized->node->station.km){
                toBeOptimized->node = node;
            }
            //continuo a cercare
            node = treePredecessor(node);
        }

        //Ottimizzo la tappa precedente
        last = last->prec;
        toBeOptimized = toBeOptimized->prec;

    }



    return 1;
}
