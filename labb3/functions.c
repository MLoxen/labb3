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

/*
Button 1    P0.23
Button 2    P0.24
Button 3    P0.08
Button 4    P0.09
LED 1       P0.28
LED 2       P0.29
LED 3       P0.30
LED 4       P0.31
*/

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

//definierar vilka pinnar som ska användas för att skicka och ta emot data:
#define PIN_TXD 20
#define PIN_RXD 22

#define LED_ON_TIME 200
#define CLEAR_SCREEN "\033c"


//Skapar en driver instance för UARTE:
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

//skapar en buffer att ta emot tecken vi läser över UARTE



/*  Varialbes   */

char char_buffer[100];

/* End Varialbes */



void init(void){
    //Vi kör med default settings och anger våra TX- och RX-pinnar:
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);

    //Initierar UARTE:
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
    if (errr != 0){
        //Här kan vi implementera felhantering, men för stunden håller vi tummarna att allt funkar
    }
}

char uarte_buffer;


void uarte_write(char* data, int length){
    nrfx_uarte_tx(&instance, data, length, 0);
}

void read_string(void){
    int i = 0;
    char input;
    
    char res[100];

    while (i < 100-1){
        nrfx_uarte_rx(&instance, &input, 1);
        // while(nrfx_uarte_rx_check(&instance)){}

        if (input == '\r' || input == '\n'){
            res[i] = '\0';
            break;
        }
        res[i] = input;
        i++;
            // ECHO //
        //uarte_write(&input, 1);
    }
    uarte_write(res, strlen(res));
    return;
}

int read_int(void){
    int i = 0;
    char input;
    
    char res[100];

    while (i < 100-1){
        nrfx_uarte_rx(&instance, &input, 1);
        // while(nrfx_uarte_rx_check(&instance)){}

        if (input == '\r' || input == '\n'){
            res[i] = '\0';
            break;
        }
        res[i] = input;
        i++;
            // ECHO //
        // uarte_write(&input, 1);
    }

    // uarte_write(res, strlen(res));
    return(atoi(res));
}

void send_int(int integer){ /*Kanske funkar...*/
    sprintf(char_buffer, "%d", integer);
    uarte_write(char_buffer, strlen(char_buffer));
}

void uppgift2(int amount, int delay){
    /*Sätt dom som OUTPUT pins*/
    nrf_gpio_cfg_output(LED1);
    nrf_gpio_cfg_output(LED2);
    nrf_gpio_cfg_output(LED3);
    nrf_gpio_cfg_output(LED4);

    /*Vi kontrollerar inte LEDs direkt utan vi kontrollerar GPIO pinnen som styr LEDen, så logiken blir lite omvänd p.g.a. saker ni kommer lära i elektroniksystem. 1 blir OFF. */
    nrf_gpio_pin_write(LED1,1);
    nrf_gpio_pin_write(LED2,1);
    nrf_gpio_pin_write(LED3,1);
    nrf_gpio_pin_write(LED4,1);

    nrfx_systick_init();

    nrf_gpio_cfg_input(BUTTON_1, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON_2, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON_3, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON_4, NRF_GPIO_PIN_PULLUP);

    int i = 0;
    int led;
    while(i < amount){ // i < var
        led = uppgift4();
        
    
        nrfx_systick_delay_ms(100);
        
        if(led == 0){
            nrf_gpio_pin_toggle(LED1);
            nrfx_systick_delay_ms(delay);
            nrf_gpio_pin_toggle(LED1);
             // varr
        }
    
        if(led == 1){
            nrf_gpio_pin_toggle(LED2);
            nrfx_systick_delay_ms(delay);
            nrf_gpio_pin_toggle(LED2);
        }
    
        if(led == 2){
            nrf_gpio_pin_toggle(LED3);
            nrfx_systick_delay_ms(delay);
            nrf_gpio_pin_toggle(LED3);
        }
    
        if(led == 3){ // Randomizer upg4
            nrf_gpio_pin_toggle(LED4);
            nrfx_systick_delay_ms(delay);
            nrf_gpio_pin_toggle(LED4);
        }
        i++;
    }

}

void uppgift3(void){

    char msg1[] = "\n\rToo long of a delay!\n\r";
    char msg2[] = "\n\rWaiting!\n\r";
    char msg3[] = "\n\rWaited...\n\r";
    char msg4[] = "\n\rWAITER!\n\r";
    
    int num;

    nrfx_systick_init();

    uarte_write(msg4, strlen(msg4));
    num = read_int();

    if(num > 15){
        uarte_write(msg1, strlen(msg1));
        return;
    }

    uarte_write(msg2, strlen(msg2));

    nrfx_systick_delay_ms(num*1000);
    
    uarte_write(msg3, strlen(msg3));


}

int uppgift4(void){
     // Sätter igång klockan för att timern ska funka.
    NRF_CLOCK_S->TASKS_LFCLKSTART = 1;

    const nrfx_uarte_config_t uarte_config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);
    nrfx_uarte_init(&instance, &uarte_config, NULL);

    // Prescaler används för att den ska ändra siffra mycket snabbare, utan den så får man samma siffra om man trycker samma sekund.
    NRF_RTC0_S->PRESCALER = 0;
    // Startar timern
    NRF_RTC0_S->TASKS_START = 1;

    nrf_gpio_cfg_input(BUTTON_1, NRF_GPIO_PIN_PULLUP);

    // while (nrf_gpio_pin_read(BUTTON_1) != 0)
    // {
    // }

    int time = NRF_RTC0_S->COUNTER;
    srand(time);

    // int r = rand()%4;
    int r = rand()%100; 
    char buffer[100];
    
    // Förvandlar siffervärdet i time till text och sparar hela meningen i buffer.
    // sprintf(buffer, "RTC time: %d\r\n", time);
    // // Skickar texten till datorn så att den kan skrivas ut.
    // // Pekar ut adressen till bufferten och tolkar texten som unsigned råa 8-bitars bytes och strlen mäter exakt hur många bokstäver som ska skickas.
    // uarte_write(buffer, strlen(buffer)); 
    
    // // Sparar meningen i buffer. 
    // sprintf(buffer, "3 random tal:\r\n");
    // uarte_write(buffer, strlen(buffer));
    
    // for (int i = 0; i < 10; i++) {
    //     sprintf(buffer, "Tal %d: %d\r\n", i + 1, rand()%4);
    //     uarte_write(buffer, strlen(buffer));
    // }
    return r;
    
}