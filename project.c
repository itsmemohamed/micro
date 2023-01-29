#include "MKL25Z4.h"
#include <stdio.h>

#define GREEN_BTN (1<<19) 
#define RED_BTN (1<<18) 

// Assign pins for 7-segment display
#define SEG_A (1<<0)
#define SEG_B (1<<1)
#define SEG_C (1<<2)
#define SEG_D (1<<3)
#define SEG_E (1<<4)
#define SEG_F (1<<5)
#define SEG_G (1<<6)

const uint8_t seg_lookup[10] = {
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,                 // 0
    SEG_B | SEG_C,                                                   // 1
    SEG_A | SEG_B | SEG_G | SEG_E | SEG_D,                         // 2
    SEG_A | SEG_B | SEG_G | SEG_C | SEG_D,                         // 3
    SEG_F | SEG_B | SEG_G | SEG_C,                                 // 4
    SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,                         // 5
    SEG_A | SEG_F | SEG_E | SEG_D | SEG_C | SEG_G,                 // 6
    SEG_A | SEG_B | SEG_C,                                          // 7
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,         // 8
    SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G                  // 9
};

volatile uint8_t count = 0; // Count for 7-segment display

void initGPIO()
{
    // Enable clock for PORTB and PORTC
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTC_MASK;

    // Configure green button as input
    PORTC->PCR[19] |= PORT_PCR_MUX(1);
    PTC->PDDR &= ~GREEN_BTN;

    // Configure red button as input
    PORTC->PCR[18] |= PORT_PCR_MUX(1);
    PTC->PDDR &= ~RED_BTN;

    // Configure 7-segment display as output
    PORTB->PCR[0] |= PORT_PCR_MUX(1);
    PORTB->PCR[1] |= PORT_PCR_MUX(1);
    PORTB->PCR[2] |= PORT_PCR_MUX(1);
    PORTB->PCR[3] |= PORT_PCR_MUX(1);
    PORTB->PCR[4] |= PORT_PCR_MUX(1);
    PORTB->PCR[5] |= PORT_PCR_MUX(1);
    PORTB->PCR[6] |= PORT_PCR_MUX(1);
	PTB->PDDR |= SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G;}

void displayCounter()
{
PTB->PDOR = seg_lookup[count];
}

int main()
{
initGPIO();
	while (1)
{
    // Check if green button is pressed
    if (!(PTC->PDIR & GREEN_BTN))
    {
        count++;
        if (count == 10)
        {
            count = 0;
        }
        displayCounter();
    }

    // Check if red button is pressed
    if (!(PTC->PDIR & RED_BTN))
    {
        count--;
        if (count < 0)
        {
            count = 9;
        }
        displayCounter();
    }
}
return 0;
