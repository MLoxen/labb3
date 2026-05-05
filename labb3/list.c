#include "list.h"
#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <nrfx_systick.h>
#include <nrfx_rtc.h>
#include <nrf_gpio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

//Skapar en driver instance för UARTE:
// nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);


extern nrfx_uarte_t instance;

//Returnera en tom lista - funktionen är färdig
List create_empty_list(void)
{
    return NULL;
}

//Här skapas nya noder. Denna funktion är den enda som ska använda malloc.
//Denna funktion ska i sin tur bara kallas av add_first och add_last.
//Notera att den är static och inte finns med i interfacet
static Node * create_list_node(const Data data)
{
	//glöm inte att kolla så att malloc inte returnerade NULL

	Node *new_node = malloc(sizeof(Node));

	if (new_node == NULL){ //Felhantering
		return NULL;
	}

	
	//Fyll i nodens fält
	new_node->data = data;

	new_node->next = NULL;

	return new_node;
}

//Är listan tom?
//Returnerar 1 om listan är tom, annars 0
int is_empty(const List list)
{
    // const Node *current = list;
 	if (list == NULL) {
        return 1; // Tom
    	}
	else{
		return 0;
	}
}

//Lägg till en nod först i listan
void add_first(List *list, const Data data)
{
	Node *new_node = create_list_node(data);
    if (new_node != NULL)
    {
        new_node->next = *list;
        *list = new_node;
    }
}

//lägg till nod sist i listan
void add_last(List *list, const Data data)
{
	Node *new_node = create_list_node(data);
	
	if(*list == NULL){
		*list = new_node;
	}
	else{
		Node *current = *list;

		while (current->next != NULL){
			current = current->next;
		}

		current->next = new_node;
	}

}

//Ta bort första noden i listan
//precondition: listan är inte tom (testa med assert)
void remove_first(List *list)
{
	if(*list != NULL){ // Checka så att vi inte kollar i en tom lista
		Node *temp = *list; // Save pointer till första noden. 
		*list = (*list)->next; // Flyttar starten av listan till nästa nod.
		free(temp); // Tömer noden 
		//Assert?
	}
}

//ta bort sista noden i listan
//precondition: listan är inte tom (testa med assert)
void remove_last(List *list)
{
	assert(!is_empty(*list));

	if((*list)->next==NULL){
		// Node *current = *list;
		free(*list);
		*list = NULL;
	}
	else{
		Node *current = *list;

		while (current->next->next != NULL) //Om du bara vandrar tills current->next == NULL, så står du redan på den sista noden.
		{
			current = current->next;
		}
		free(current->next);
		// Sätt näst sista nodens next till NULL för att markera nya slutet
		current->next = NULL;
	}
	return;
}

//töm listan (ta bort alla noder ur listan)
//postcondition: Listan är tom, *list är NULL
void clear_list(List *list)
{
	//alla noder ska frigöras
	while(!is_empty(*list)){
		remove_first(list);
	}
	return;
}

//Skriv ut listan genom UART
void print_list(const List list)
{
	char buffer[32];
    const Node *current = list; // Använd const då vi bara läser

    // Skicka startindikator
    char start_msg[] = "\r\nList content: ";
    nrfx_uarte_tx(&instance, (uint8_t *)start_msg, strlen(start_msg), 0);
    while (nrfx_uarte_tx_in_progress(&instance)); // Vänta på att TX blir klar

    if (current == NULL) {
        char empty_msg[] = "EMPTY\r\n";
        nrfx_uarte_tx(&instance, (uint8_t *)empty_msg, strlen(empty_msg), 0);
        return;
    }

    while (current != NULL){
        // Format datan till sträng: t.ex. "[42] -> "
        int len = snprintf(buffer, sizeof(buffer), "[%d] -> ", current->data);
        
        // Skicka den formaterade strängen
        nrfx_uarte_tx(&instance, (uint8_t *)buffer, (size_t)len, 0);
        
        // Vänta tills tx är klar innan vi loopar igen
        while (nrfx_uarte_tx_in_progress(&instance));

        current = current->next;
    }
	
    // Avsluta raden
    char end_msg[] = "NULL\r\n";
    nrfx_uarte_tx(&instance, (uint8_t *)end_msg, strlen(end_msg), 0);
    while (nrfx_uarte_tx_in_progress(&instance));
	
}


// void print_list(const List list){

// 	char sprintfBuffer[100];
// 	if(list == NULL){
// 		return;
// 	}

// 	sprintf(sprintfBuffer, "\n\r[%d]", list->data);
// 	// uarte_write(sprintfBuffer, sizeof(sprintfBuffer));

// 	print_list(list->next);
// }
//returnera första datat i listan
//precondition: listan är inte tom (testa med assert)
Data get_first_element(const List list)
{
	// assert(!is_empty(list));
    const Node *current = list;
	
	return current->data;
}

//returnera sista datat i listan. 
//precondition: listan är inte tom (testa med assert)
Data get_last_element(const List list)
{
	// assert(!is_empty(list));
	
	const Node *current = list;
	while(current->next != NULL){
		current = current->next;
	}
	return current->data;
}

//Returnera hur många noder som finns i listan
int number_of_nodes(const List list)
{

	if(list == NULL) //om list är tom även om node är tom
		return 0;
	
	return 1 + number_of_nodes(list->next);

}

//Sök efter data i listan, returnera 1 om datat finns, annars 0.
int search(const List list, const Data data)
{
	
	if(list == NULL) 
		return 0;
	
	else if(list->data == data) 
		return 1;
	
	return search(list->next, data);

}

//Ta bort data ur listan (första förekomsten), returnera 0 om datat inte finns, annars 1
int remove_element(List *list, const Data data)
{
	//no assert?
	
	if(is_empty(*list)){
		return 0;
	}

	//Speciall Case, om första är det samma;
	if((*list)->data == data){
		remove_first(*list);
		return 1;
	}
	
	Node *current = *list;
	
	while (current->next != NULL)
	{
		if(current->next->data == data){
			Node *removal_node = current->next;

			current->next = current->next->next;

			free(removal_node);
			// current->next->data = 0; // Detta om vi bara vill "nollställa datan" och ej ta bort hela noden
			return 1;
		}
		else{
			current = current->next;
		}
	}
	return 0;
}

	
	
	
	
	
	
	
	
	
	
	
	
	

