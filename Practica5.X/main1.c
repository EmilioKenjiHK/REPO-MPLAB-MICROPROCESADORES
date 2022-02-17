#include <xc.h>
#include <stdint.h>

#define PIN_PULSADOR 5
#define PIN_LED3 3
#define T_PARPADEO 0 // en ms

#include "temp.h"

int main1 (void)
{
    uint32_t ticks_ant, ticks_act;
    int pulsador;
    
    ANSELB &= ~(1<<PIN_PULSADOR);
    ANSELC &= ~15;
    
    
    TRISC = ~9;                   // Poner como salida RC0 y RC3 
    LATC = 0xFFFF;                // Al principio ningún LED encendido
    TRISB = (1<<PIN_PULSADOR);    // Todos como salidas menos el pulsador
              
    InicializarTimer2();
    
    //Una vez inicializados los periféricos, activamos
    // las interrupciones
    INTCONbits.MVEC = 1; // Modo multivector
    asm(" ei");
    ticks_ant = TicksDesdeArr();
    
    while (1) {
        // Se lee el estado del pulsador
        pulsador = (PORTB >> PIN_PULSADOR) & 1;
        // pulsador = 0;
        
        // El pulsador es activo a nivel bajo
        // 0 -> pulsado
        // 1 -> no pulsado
        
        // Si el pulsador esta a 0 (pulsado) -> se enciende LED (0))
        // Si el pulsador esta a 1 (no pulsado) -> se apaga el LED (1)
        
        // Modifica el PIN RC0, a ese pin es donde debe conectarse el LED
        // Otra forma de realizar este ejercicio sería la siguiente
        
        // if (pulsador == 0) {
        //      LATC ^= 1;      Se invierte el bit (XOR)
        // }
        
        ticks_act = TicksDesdeArr();
        if((ticks_act-ticks_ant) > T_PARPADEO){
            ticks_ant = ticks_act;
            LATCINV = 1 << PIN_LED3;
        }
        
        if (pulsador == 0) {
            LATC &= ~1;
            } else {
            LATC |= 1;  
            }
    }
}
