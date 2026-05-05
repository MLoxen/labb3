#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <nrfx_systick.h>
#include <nrfx_rtc.h>
#include <nrf_gpio.h>
#include "functions.h"
#include "list.h"
#include "BSTree.h"


//definierar vilka pinnar som ska användas för att skicka och ta emot data:
#define PIN_TXD 20
#define PIN_RXD 22

#define LED1 28
#define LED2 29
#define LED3 30
#define LED4 31

#define LED_OFF 1
#define LED_ON 0


#define BUTTON_1 23
#define BUTTON_2 24
#define BUTTON_3 8
#define BUTTON_4 9

#define LED_ON_TIME 200
#define CLEAR_SCREEN "\033c"


/* Variables */
int resultat;

char str_res[100];

char Spacing[] = "\r\n\n";
char peek[] = "\r\n\nPeek: ";
char front[] = "\r\n\nFront: ";
char push[] = "\r\nPushing: \n\r";
char enqueue[] = "\r\n\nEnqueueing: \n\r";
char poping[] = "\r\n\nPoping: \n\r";
char dequeue[] = "\r\n\nDequeueing: \n\r";


int arr[10] = {1, 2, 5, 6, 8, 9, 12, 15, 27, 67};

extern nrfx_uarte_t instance;
/* End Variables */

// Pekar ALLTID, på första elementet i lista.
int main(void){ 
    init(); /* Initialize board*/
    uarte_write(peek, sizeof(peek));
    // List list = create_empty_list();
    BSTree tree = create_empty_tree();
    
    build_tree_sorted_from_array(tree, arr, 10);
    uarte_write(peek, sizeof(peek));
    print_inorder(&tree);
    uarte_write(peek, sizeof(peek));
} 