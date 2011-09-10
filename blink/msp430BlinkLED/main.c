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
	// The Tutorials show using |= and &= to set these values
	// but that isn't a good idea because they could have values
	// left over from the last time the hardware was ran.
	// We want to initialize these values exactly, so use the = operator.

	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
	LED_DIR = (LED_0 | LED_1); // Set P1.0 and P1.6 to output direction

	// The ~ operator takes the bit-wise complement of whatever we give it
	LED_OUT = ~(LED_0 | LED_1); // Set the LEDs off

	P1IE = BUTTON;

	__enable_interrupt();

	while(1) // infinite loop, we exit by cutting power	
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
	blink ^= 0x01; // the ^ is the XOR operator. Using it with the value of 1 toggles the first bit of the variable "blink".
	P1IFG &= ~BUTTON; // P1.3 IFG cleared
	LED_OUT &= ~(LED_0 + LED_1); // Clear the LEDs so they start in OFF state
}
