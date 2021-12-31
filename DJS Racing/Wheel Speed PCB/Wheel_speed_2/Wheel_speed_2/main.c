/*
 * Wheel_speed_2.c
 *
 * Created: 09-05-2020 22:08:43
 * Author : sarth
 */ 

#define F_CPU 1000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
double time1=0.0; double duty = 0.0;
void PWM_init()
{
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
	//TCCR0 = (1<<WGM00) | (1<<COM01) | (1<<CS00);
	DDRB|=(1<<PB3);
}
ISR(INT0_vect)
{
		TCCR1B&=~((1<<CS10)|(1<<CS12)|(1<<CS11));
		//time1 = TCNT1*(0.000001)*1024;
		time1 = TCNT1*(0.000001);
		double rpm = 60.0/time1;
		duty = (rpm*255)/2000;
		OCR0 = duty;
		TCNT1 = 0x00;
		TCCR1B = (1<<CS10);
		//TCCR1B|=(1<<CS10)|(1<<CS12);
		//serial_avr(duty);
}
int main(void)
{
	PWM_init();
	TCNT1 = 0x00;
	//TCCR1B = (1<<CS10)|(1<<CS12);
	TCCR1B = (1<<CS10);
    /* Replace with your application code */
	DDRD=0;
	GICR = 1<<INT0;		/* Enable INT0*/
	MCUCR = 1<<ISC01 | 1<<ISC00;  /* Trigger INT0 on rising edge */	
	sei();
    while (1) 
    {
		
    }
}

