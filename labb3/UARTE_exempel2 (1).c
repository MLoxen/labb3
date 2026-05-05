#include <nrfx.h>
#include <nrf5340_application.h>
#include <nrfx_config.h>
#include <nrf.h>
#include <nrfx_uarte.h>


//definierar vilka pinnar som ska användas för att skicka och ta emot data:
#define PIN_TXD 20
#define PIN_RXD 22

//Skapar en driver instance för UARTE:
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);

//skapar en buffer att ta emot tecken vi läser över UARTE
char uarte_buffer;

//en funktion för att skicka strängen data av storlek length, över UARTE
void uarte_write(char* data, int length)
{
    nrfx_uarte_tx(&instance, data, length, 0);
}

int main(void)
{ 

    //Vi kör med default settings och anger våra TX- och RX-pinnar:
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);

    //Initierar UARTE:
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
    if (errr != 0){
        //Här kan vi implementera felhantering, men för stunden håller vi tummarna att allt funkar
    }

    char msg1[] = " \n\r Prova skriva något så ska den eka tillbaka 2x det du trycker \n\r";
    nrfx_uarte_tx(&instance, msg1, sizeof(msg1), 0);

    while(1)
    {
        //Vi väntar på att läsa ett tecken över UARTE
        nrfx_uarte_rx (&instance, &uarte_buffer, sizeof(uarte_buffer));
        
        //Om vi lyckades läsa någonting så skickar vi tillbaka det som togs emot, två gånger
        uarte_write(&uarte_buffer, sizeof(uarte_buffer));
        uarte_write(&uarte_buffer, sizeof(uarte_buffer));
    }

} 


