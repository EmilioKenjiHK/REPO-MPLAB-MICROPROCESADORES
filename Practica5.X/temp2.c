#include <xc.h>
#include <stdint.h>
#include "temp2.h"

void InicializarTimer3(void)
{
    // Configuración del Timer
    TMR3 = 0;
    T3CON = 0;
    IFS0bits.T3IF = 0;
    PR3 = 39061;
    // Se configura la prioridad de la interrupción
    IPC3bits.T3IP = 4;
    //Se configura la subprioridad de la interrupción
    IPC3bits.T3IS = 0;
    // Se borra el flag de interrupción por si estaba pendiente
    IFS0bits.T3IF = 0;
    // y por último se habilita su interrupción
    IEC0bits.T3IE = 1;
    
    T3CON = 0x8060; //Arranca el Timer con prescalado 64
}

static uint32_t ticks = 0; // Per. reloj desde arranque

__attribute__((vector(_TIMER_3_VECTOR),interrupt(IPL4SOFT),nomips16))
void InterrupcionTimer3(void)
{
    // Se borra el flag de interrupción para no volver a
    // entrar en esta rutina hasta que se produzca una nueva
    // interrupción.
    IFS0bits.T3IF = 0;
    ticks ++;
}

uint32_t TicksDesdeArr3(void)
{
    uint32_t c_ticks;
    
    asm(" di"); // Evitamos Ley de Murphy
    c_ticks = ticks;
    asm(" ei");
    
    return c_ticks;
}
