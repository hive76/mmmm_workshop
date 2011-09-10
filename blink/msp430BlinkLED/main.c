//***************************************************************************************
// MSP430 Blink LED / Start Stop Blinking with Button Demo - Software Toggle P1.0 & P1.6
//
// Description; Toggle P1.0 and P1.6 by xor'ing them inside of a software loop. To
// start/stop blink, an interrupt in Port 1 will toggle the condition statement. 
// ACLK = n/a, MCLK = SMCLK = default DCO
//
// MSP430x2xx
// -----------------
// /|\| XIN|-
// | | |
// --|RST XOUT|-
// | P1.6|-->LED
// Button -->|P1.3 P1.0|-->LED
//
// Aldo Briano
// Texas Instruments, Inc
// June 2010
// Built with Code Composer Studio v4
//***************************************************************************************
#include <msp430g2231.h>


#define LED_0 BIT0 
#define LED_1 BIT6
#define LED_OUT P1OUT
#define LED_DIR P1DIR
#define BUTTON BIT3

unsigned int blink = 0;

void main(void)
{
WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
LED_DIR |= (LED_0 + LED_1); // Set P1.0 and P1.6 to output direction
LED_OUT &= ~(LED_0 + LED_1); // Set the LEDs off
P1IE |= BUTTON;

__enable_interrupt();

for (;;)
{

if(blink > 0)
{
P1OUT ^= (LED_0 + LED_1); // Toggle P1.0 and P1.6 using exclusive-OR

__delay_cycles(100000); // SW Delay of 10000 cycles at 1Mhz

}
}

} 

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
blink ^= 0x01;
P1IFG &= ~BUTTON; // P1.3 IFG cleared
LED_OUT &= ~(LED_0 + LED_1); // Clear the LEDs so they start in OFF state

}