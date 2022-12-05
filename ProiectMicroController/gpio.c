
#include "gpio.h"
#include "Uart.h" 




void Buzzer_Init(void) {
	// Activarea semnalului de ceas pentru a putea folosi GPIO cu ajutorul pinului 1 de pe portul C
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTC_MASK;
	
	// Utilizarea GPIO impune selectarea variantei de multiplexare cu valorea 1
	PORTC->PCR[BUZZER_PIN] &= ~PORT_PCR_MUX_MASK;
	PORTC->PCR[BUZZER_PIN] |= PORT_PCR_MUX(1);
	
	//Setarea directiei pe output,default e input
	GPIOC_PDDR |= (1<<BUZZER_PIN);
	//Zeroizare, oprire buzzer
	GPIOC_PCOR |= (1<<BUZZER_PIN);
}
