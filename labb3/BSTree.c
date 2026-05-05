#include "BSTree.h"
#include "functions.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <nrfx_systick.h>
#include <nrfx_rtc.h>
#include <nrf_gpio.h>


extern nrfx_uarte_t instance;
/*Det är helt tillåtet att lägga till egna hjälpfunktioner men de befintliga funktionerna ska implementeras enligt instruktionerna*/

/* Statiska hjälpfunktioner används av andra funktioner i trädet och ska inte ligga i interfacet (användaren behöver inte känna till dessa) */
/* Skapar en trädnod med det givna datat. Denna funktion bör vara den enda som använder malloc för att skapa en ny treeNode*/
static struct treeNode* create_tree_node(int data)
{
    
    struct treeNode *new_branch = (struct treeNode*)malloc(sizeof(struct treeNode));

    if(new_branch == NULL){
        return NULL;
    }

    new_branch->data = data;
    new_branch->left = NULL;
    new_branch->right = NULL;

    return new_branch;

}

/* Returnerar en dynamiskt allokerad array som innehåller trädets data sorterat */
int* write_sorted_to_array(const BSTree tree)
{
    /* Skapa en dynamisk array med rätt storlek
    
    Skriv datat från trädet sorterat till arrayen (minsta till största)
       - till detta kanske du behöver en hjälpfunktion */
    

}

char promptFinder[] = "\n\rHallo!";

/* Bygger upp ett sorterat, balanserat träd från en sorterad array */
void build_tree_sorted_from_array(BSTree* tree, const int arr[], int size)
{
    /* Bygg rekursivt från mitten.
    Mittenelementet i en delarray skapar rot i delträdet
    Vänster delarray bygger vänster delträd
    Höger delarray bygger höger delträd*/
    if(size<=0){ // <= 1 ??? 
        return;
    }
    
    int halfSize = (size-1)/2; // -1 för att justera till indexeringar
    int data = arr[halfSize];

    int left_arr[33];
    int right_arr[33];

    for(int i = 0; i<halfSize; i++){
        left_arr[i] = arr[i];
    }
    for(int i = size; i>halfSize; i--){
        left_arr[i] = arr[i];
    }

    BSTree new_branch = create_tree_node(data); //Root

    uarte_write(promptFinder, sizeof(promptFinder));
    
    if(new_branch!=NULL){
        build_tree_sorted_from_array(new_branch->left, left_arr, halfSize);
        uarte_write(promptFinder, sizeof(promptFinder));
        build_tree_sorted_from_array(new_branch->right, right_arr, halfSize);
    }
    else{
        return;
    }
    

}

/* Implementation av trädet, funktionerna i interfacet */

/* Skapar ett tomt träd - denna funktion ar färdig */
BSTree create_empty_tree(void)
{
	return NULL;
}

/* Returnerar 1 ifall trädet är tomt, 0 annars */
int tree_is_empty(const BSTree tree) //Finns exakt samma i förra Labben
{
    if (tree == NULL) {
            return 1; // Tom
            }
        else{
            return 0;
        }
}

/* Sätter in 'data' sorterat i *tree */
void insert_sorted(BSTree* tree, int data)
{
	/*Tänk på att trädet kan vara tomt vid insättning
    I vanliga fall kan man lösa dubletter på olika sätt. Eftersom vi vill att era lösningar
    fungerar likadant inför examinationen så bestämmer jag att dubletter ej ska tillåtas i trädet.
    Post-condition kan verifieras med hjälp av find(...)*/
}

/* Utskriftsfunktioner
Det räcker att ni implementerar LR-ordningarna*/
void print_preorder(const BSTree tree)
{

}

char testPrompt[] = "\n\rTesting: Inorder!\n\r";
void print_inorder(const BSTree tree)
{
//     uarte_write(promptFinder, sizeof(promptFinder));
//     if(tree == NULL){
//         return;
//     }
//     if(tree->left == NULL && tree->right == NULL){
//         uarte_write(&tree->data, sizeof(tree->data));
//         uarte_write(testPrompt, sizeof(testPrompt));
//         return;

//     }
//     else if(tree->left!=NULL){
//         print_inorder(tree->left);
//     }
//     else{
//         print_inorder(tree->right);
//     }
// 1. Basfall: Om vi nått en tom gren, vänd tillbaka
    // 1. Basfall: KOLLA DETTA FÖRST!
    if (tree == NULL) {
        return;
    }

    // 2. Nu är det säkert att förbereda buffer
    char buffer[12];
    int len = sprintf(buffer, "%d ", tree->data);

    // 3. Gå till vänster
    print_inorder(tree->left);

    // 4. Skriv ut
    uarte_write(buffer, len);

    // 5. Gå till höger
    print_inorder(tree->right);
}
void print_postorder(const BSTree tree)
{

}

/* Returnerar 1 om 'data' finns i tree, 0 annars */
int find(const BSTree tree, int data)
{
    // Tänk pa att trädet kan vara tomt

}

/* Tar bort 'data' från trädet om det finns */
void tree_remove_element(BSTree* tree, int data)
{
	/* Inget data ska/kan tas bort från ett tomt träd
    Tre fall: Ett löv (inga barn), ett barn (vänster eller höger), två barn
    
    Glöm inte att frigöra noden när den länkats ur trädet'*/
}

/* Returnerar hur många noder som totalt finns i trädet */
int tree_number_of_nodes(const BSTree tree)
{
 //Finns redan i labb2??
}

/* Returnerar hur djupt trädet är */
int depth(const BSTree tree)
{

}

/* Returnerar minimidjupet för trädet
Se math.h för användbara funktioner*/
int min_depth(const BSTree tree)
{

}

/* Balansera trädet så att depth(tree) == minDepth(tree) */
void balance_tree(BSTree* tree)
{
/* Förslag på algoritm:
- överfor trädet till en dynamiskt allokerad array med writeSortedToArray()
- töm trädet med freeTree()
- bygg upp trädet rekursivt från arrayen med buildTreeSortedFromArray()
- frigör minne för den dynamiskt allokerade arrayen


Post-conditions:
- tree har lika många noder som tidigare
  - djupet för trädet är samma som minimumdjupet för trädet */
}


/* Töm trädet och frigör minnet för de olika noderna */
void free_tree(BSTree* tree)
{
    free(tree); //???

}


