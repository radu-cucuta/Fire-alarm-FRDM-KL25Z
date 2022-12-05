#include "adc.h"


#define CHANNELS_A_SHIFT 5
#define FLAME_PIN (1)

void adcInit(enum ADC_Low_Power lp,enum ADC_Size size,enum ADC_Sample_Time sampleTime,enum ADC_Avg avg){
    
		// Activare semnal de ceas pentru modulul adc0
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;

		// Low-Power Configuration , in cazul nostru e lasat default
    if(lp==ADC_Low_Power_ON)
        ADC0->CFG1 = ADC_CFG1_ADLPC_MASK;  
		
		
		// 
		
    if(sampleTime>0){
				//selectearea numarului de cicluri de ceas pentru acuratetea preluarii valorilor de la senzor (calibrare senzor)
        ADC0->CFG1 |= ADC_CFG1_ADLSMP_MASK;     
				//10 6 extra ADCK cycles; 10 ADCK cycles total sample time.
        ADC0->CFG2 = ADC_CFG2_ADLSTS(sampleTime&3);
    }

		
    ADC0->CFG1 |=ADC_CFG1_ADIV(1)    // Clock Divide Select: (Input Clock)/2 : 50MHz/2=25MHz
               | ADC_CFG1_MODE(size)               // (n)bits Resolution
               | ADC_CFG1_ADICLK(0);    // Input Clock: (Bus Clock=50MHz)

  /* ADC0_CFG2: ADACKEN=1,ADHSC=0,ADLSTS=0 for speed up */
  ADC0->CFG2 = (uint32_t)((ADC0->CFG2 & (uint32_t)~(uint32_t)(
               ADC_CFG2_ADHSC_MASK |
               ADC_CFG2_ADLSTS(0x03)
              )) | (uint32_t)(
               ADC_CFG2_ADACKEN_MASK
              ));

    if(avg>0)
    ADC0->SC3 = ADC_SC3_AVGE_MASK       // Hardware Average Enable
              | ADC_SC3_AVGS(avg&3);   // setarea valorii receptionate de catre modulul adc ca medie a 4 esantioane succesive
}


int adcSelect(){
    
		SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;// activare semnal de ceas pe portul B
		PORTB->PCR[FLAME_PIN]&= ~PORT_PCR_MUX_MASK;// analog Input
		return 9;    //alegerea canalului corespunzator pentru portul B,PIN 1
}


uint16_t adcRead(int channel) {
    // activarea pe AD9 channel si request catre senzor
    if(channel>=0){
			ADC0->SC1[0] = ADC_SC1_ADCH(channel & ~(1 << CHANNELS_A_SHIFT));

    // spinlock pana cand conversia este finalizata
			while ((ADC0->SC1[0] & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);

    // returnarea valorii
			return (uint16_t)ADC0->R[0];
    }
}