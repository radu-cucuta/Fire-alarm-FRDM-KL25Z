//#include "gpio.h"
#include "adc.h"
#include "gpio.h"
#include "Uart.h"
int stop_data_flag = 0;

void UART0_IRQHandler(void) {

		if(UART0->S1 & UART0_S1_RDRF_MASK) {
			//char c=UART0->D;
			stop_data_flag = !stop_data_flag;
			GPIOC_PTOR |= (1<<BUZZER_PIN);
		}
}


void PIT_Init(void) {
	
	// Activarea semnalului de ceas pentru perifericul PIT
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	// Utilizarea semnalului de ceas pentru tabloul de timere
	PIT_MCR &= ~PIT_MCR_MDIS_MASK;
	// Oprirea decrementarii valorilor numaratoarelor in modul debug
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	
	// Setarea valoarea numaratorului de pe canalul 0 la o perioada de 0.2 secunda
	//bus clock 10485760
	//Load Value = Nr. sec. * BUS CLOCK Freq. - 1
	//2,097,151 = 0.2 * 10485760 - 1
	PIT->CHANNEL[0].LDVAL = 0x1FFFFF;
	
  // Activarea întreruperilor pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;
	// Activarea timerului de pe canalul 0
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
	
	
	
	// Activarea întreruperii mascabile si setarea prioritatiis
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn,5);
	NVIC_EnableIRQ(PIT_IRQn);
}

void printValueUart0(uint16_t data){
	//buzzer
	if(stop_data_flag == 0){
		if(data>=500){
			GPIOC_PSOR |= (1<<BUZZER_PIN);
		}else{
			GPIOC_PCOR |= (1<<BUZZER_PIN);
		}
	}
	else{
		GPIOC_PSOR |= (1<<BUZZER_PIN);
	}
	
	uint8_t arr[10];
	int counter=0;
	uint16_t second_data = data;
	
	if(second_data==0)
	{
		UART0_Transmit(0x30);
	}
	while(second_data!=0){
			arr[counter++]=second_data%10;
			second_data=second_data/10;
	}
	int j;
	for(j=counter-1;j>=0;j--){
		UART0_Transmit(arr[j]+0x30);
	}
	
}

void PIT_IRQHandler(void) {
	
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		printValueUart0(adcRead(adcSelect()));
		UART0_Transmit(0x0D);
		UART0_Transmit(0x0A);
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
	}
}



int main()
{
	
	//Initializare modul sunet
	Buzzer_Init();
	//Initializare timer folosit in transmitereeea datelor la un anumit interval de timp.
	PIT_Init();
	//initializare modul comunicatie seriala cu baud_rate de transmitere a datelor 115200.
	UART0_Init(115200);
	
	
	//initializare convertor adc
	
	//ADC_Low_Power_OFF = alimentare normala, performante ridicate
	//ADC_Size_10bits = gama de reprezentare a valorilor extrase de la senzorul analogic flame
	//ADC_Sample_Time_6_Extra_cycles = normal clock divider
	//ADC_Avg_4_Samples_Avg = setarea valorii receptionate de catre modulul adc ca medie a 4 esantioane succesive
	adcInit(ADC_Low_Power_OFF,ADC_Size_10bits,ADC_Sample_Time_6_Extra_cycles,ADC_Avg_4_Samples_Avg);
	while(1){}
}


