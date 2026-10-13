#include <msp430.h> 

volatile int counting = 1;
volatile int count = 0;
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1DIR &= ~BIT3; 
    P1REN |= BIT3;              // Enable pull-up/pull-down resistor on P1.3
    P1OUT |= BIT3;              // Set resistor as pull-up 

	P1DIR |= BIT6;              // Set P1.6 to output direction
	P1OUT &= ~BIT6;      

	P1IES |= BIT3;              // Set interrupt on falling edge (high to low, button press (incase i couldnt be any clearer))
    P1IFG &= ~BIT3;             // Clear any existing P1.3 interrupt flag (so so fucking important)
    P1IE  |= BIT3;              // Enable local interrupt for P1.3 

    TA0CTL = TASSEL_1 | MC_1 | ID_2; 
    TA0CCR0 = 5487;                 
	TA0CCTL0 = CCIE;               

     __enable_interrupt();
	return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {
	if (P1IFG & BIT3) {
        if(P1IES & BIT3){
            counting = 0;
			count = 0;
			P1OUT &= ~BIT6;
			P1IFG &= ~BIT3; 
            TA0R = 0;
            //need to reset timer
        }
	}
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void) {
	if(counting == 1){
		P1OUT ^= BIT6;
	}
	 else
    {
        count++;
        if(count >= 22)
        {
            counting = 1;
            count = 0;
        }
    }
	
}