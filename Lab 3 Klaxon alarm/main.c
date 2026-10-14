#include <msp430.h> 

// Frequencies calculated assuming a 1 MHz SMCLK clock
// Formula: Period = SMCLK_Freq / Tone_Freq
#define TONE_HIGH_PERIOD  1538          // ~650 Hz
#define TONE_LOW_PERIOD   2222          // ~450 Hz

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	P2DIR |= BIT1;                // Set P2.1 as Output
    P2SEL |= BIT0;                // Select Timer_A1 peripheral output function for P2.1

	TA0CTL = TASSEL_1 | MC_0 | ID_2; // SMCLK (1MHz), Up Mode, Divider /3
	
	while (1) {
        // Play High Tone
        TA0CCR0 = TONE_HIGH_PERIOD; 
        TA0CCR1 = TONE_HIGH_PERIOD / 2; // 50% Duty Cycle
        __delay_cycles(4000);                 // Tone duration (400 ms)
        // Play Low Tone
        TA0CCR0 = TONE_LOW_PERIOD; 
        TA0CCR1 = TONE_LOW_PERIOD / 2;  // 50% Duty Cycle
        __delay_cycles(4000);               // Tone duration (400 ms)
    }
	return 0;
}
