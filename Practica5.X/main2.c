#include <xc.h>
#include <stdint.h>

#define PIN_PULSADOR 5
#define PIN_LED2 2
#define PIN_LED3 3
#define T_PARPADEO 0 // en ms

#include "temp.h"
#include "temp2.h"

int main (void)
{
    uint32_t ticks_ant1, ticks_act1, ticks_ant3, ticks_act3;
    int pulsador;
    
    ANSELB &= ~(1<<PIN_PULSADOR);
    ANSELC &= ~15;
    
    
    TRISC = ~13;                   // Poner como salida RC0, RC2 y RC3 
    LATC = 0xFFFF;                // Al principio ningún LED encendido
    TRISB = (1<<PIN_PULSADOR);    // Todos como salidas menos el pulsador
              
    InicializarTimer2();
    InicializarTimer3();
    
    //Una vez inicializados los periféricos, activamos
    // las interrupciones
    INTCONbits.MVEC = 1; // Modo multivector
    asm(" ei");
    ticks_ant1 = TicksDesdeArr();
    ticks_ant3 = TicksDesdeArr3();
    
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
        
        ticks_act1 = TicksDesdeArr();
        if((ticks_act1-ticks_ant1) > T_PARPADEO){
            ticks_ant1 = ticks_act1;
            LATCINV = 1 << PIN_LED3;
        }
        
        ticks_act3 = TicksDesdeArr3();
        if((ticks_act3-ticks_ant3) > T_PARPADEO){
            ticks_ant3 = ticks_act3;
            LATCINV = 1 << PIN_LED2;
        }
        
        if (pulsador == 0) {
            LATC &= ~1;
            } else {
            LATC |= 1;  
            }
    }
}
