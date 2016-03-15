#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>


uint16_t  sine_wave[128] = {
	0, 7, 15, 23, 31, 38, 45, 52,
	60, 66, 73, 79, 87, 93, 99, 104,
	110, 115, 120, 124, 129, 133, 137, 140,
	143, 146, 149, 150, 153, 154, 154, 155,
	155, 155, 154, 154, 153, 150, 149, 146,
	143, 140, 137, 133, 129, 124, 120, 115,
	110, 104, 99, 93, 87, 79, 73, 66,
	60, 52, 45, 38, 31, 23, 15, 7,
	0, 7, 15, 23, 31, 38, 45, 52,
	60, 66, 73, 79, 87, 93, 99, 104,
	110, 115, 120, 124, 129, 133, 137, 140,
	143, 146, 149, 150, 153, 154, 154, 155,
	155, 155, 154, 154, 153, 150, 149, 146,
	143, 140, 137, 133, 129, 124, 120, 115,
	110, 104, 99, 93, 87, 79, 73, 66,
	60, 52, 45, 38, 31, 23, 15, 7
};


void osp_setup(void) {


	TCCR1B =  0;

	TCNT1 = 0x00;
	ICR1 = 0;
	TCCR1A = _BV(COM1A0) | _BV(COM1A1) | _BV(WGM11);
	TCCR1B = _BV(WGM12) | _BV(WGM13)| _BV(CS10);
}




// Test there is currently a pulse still in progress

#define OSP_INPROGRESS() (TCNT2>0)

#define OSP_SET_AND_FIRE_A(cycles) {uint16_t m=0xffff-(cycles-1); OCR1A=m;TCNT1 =m-1;}
#define OSP_SET_AND_FIRE_B(cycles) {uint16_t m=0xffff-(cycles-1); OCR1B=m;TCNT1 =m-1;}

uint8_t cycle = 15;

ISR (TIMER0_COMPA_vect)
{
	if(cycle > 126) {
		cycle = 0;
	} else {
		cycle++;
	}
	
	if(cycle & 0x40) {
		 TCCR1A = _BV(COM1A0) | _BV(COM1A1) | _BV(WGM11);
		  OSP_SET_AND_FIRE_A(sine_wave[cycle]);
		
	} else {
		 TCCR1A = _BV(COM1B0) | _BV(COM1B1) | _BV(WGM11);
		  OSP_SET_AND_FIRE_B(sine_wave[cycle]);
		
	}
}

int main(void) {
	
	
	
  	// Timer 0 konfigurieren
    TCCR0A = (1<<WGM01); // CTC Modus
    TCCR0B |= (1<<CS00); // Prescaler 8

    OCR0A = 156-1;
 
    // Compare Interrupt erlauben
    TIMSK0 |= (1<<OCIE0A);
 
    // Global Interrupts aktivieren
	osp_setup();
	DDRB |= (1<<PB1) | (1<<PB2);
		sei();
 
    while(1)
    {
		// overcurrent checken
		// voltage checken -> relais aktivieren
		


    }
}
