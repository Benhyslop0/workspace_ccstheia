#include <msp430.h> 

volatile int count = 0;
volatile int waiting = 0;
volatile int pressed = 0;
/**
 * main.c
 */


#include <msp430.h> 

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    
    P1DIR &= ~BIT3; 
    P1REN |= BIT3;              // Enable pull-up/pull-down resistor on P1.3
    P1OUT |= BIT3;              // Set resistor as pull-up 

    
    P1DIR |= BIT6;              // Set P1.6 to output direction
    P2DIR |= BIT1 + BIT3;              // Set P2.1 to output direction

    P1OUT &= ~BIT6;             // Set P1.6 LOW this is the way to switch off (way to clear bit6)
    P2OUT &= ~BIT1 + BIT3;             // Set P2.1 LOW

    P1IES |= BIT3;              // Set interrupt on falling edge (high to low, button press (incase i couldnt be any clearer))
    P1IFG &= ~BIT3;             // Clear any existing P1.3 interrupt flag (so so fucking important)
    P1IE  |= BIT3;              // Enable local interrupt for P1.3 

    TA0CTL = TASSEL_1 | MC_0 | ID_2; // SMCLK (1MHz), Up Mode, Divider /3
    TA0CCR0 = 6158.39;                 // Set count limit for 50ms interval
    //TA0CCTL0 = CCIE;                 // Enable capture/compare interrupt for CCR0

     __enable_interrupt();
    return 0;
}

#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void) {
    if (P1IFG & BIT3) {
        if(P1IES & BIT3){
            TA0CTL |= MC_1; //MC_1 is state of on to start the timer
            TA0CCTL0 |= CCIE; //enabler
            pressed = 1;
        }
        else if (!(P1IES & BIT3)){
            //three variables to basically stop the timer interupt loop
            pressed = 0;
            count = 0;
            waiting = 0;
            TA0CTL &= ~MC_3;     // Stop the timer by clearing the Mode Control bits (sets mode to MC_0)
            TA0CCTL0 &= ~CCIE;   // Disable the Timer CCR0 interrupt
            //turn everything off
            P1OUT &= ~BIT6;
            P2OUT &= ~BIT1;
            P2OUT &= ~BIT3;
        }
        P1IES ^= BIT3;     // had to xor because turns out bit3 and P1ies stay at 1 leaving an or operation to be 1. but if xor then 1 and 1 = 0
        P1IFG &= ~BIT3;    //clearing because 
    }
    
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void) {
    //following the first button press flow and counting for 3 seconds
    if(pressed == 1){
        count++;
        P2OUT |= BIT1 + BIT3;
        if(count >= 3){
            pressed = 0;
            waiting = 1; 
            P2OUT &= ~(BIT1 +BIT3);
            P1OUT ^= BIT6;
        } 
    //needed to keep these if statements seperate 
    }
    if(waiting == 1){
        P1OUT ^= BIT6;          // Toggle P1.6
        P2OUT ^= BIT1;          // Toggle P2.1
    }
}

