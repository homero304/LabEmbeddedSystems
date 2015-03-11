
#include "inc/hw_memmap.h" //Library for clock
#include "inc/hw_types.h" //Library for clock
#include "driverlib/sysctl.h" //Library for clock
#include "inc/lm3s6965.h"

#ifdef DEBUG

void
__error__(char *pcFilename, unsigned long ulLine){

}

#endif

void envio(unsigned char ucdata){
	if(!(UART0_FR_R & UART_FR_TXFF)){
		UART0_DR_R =ucdata;
	}
}

void send(const unsigned char *pucBuffer, unsigned long ulCount){
	while(ulCount--){	
		envio(*pucBuffer++);
	}
}

unsigned long ulDelta;
unsigned long baud;			
unsigned long UARTclk;
unsigned long ulDiv;
unsigned long data;

//Estructura con los nombres de los botones a precionar
const unsigned char *caracter[] = {	"RIGHT\r\n",
					"LEFT \r\n",
					"DOWN \r\n",
					"UP   \r\n"};
//apuntador que seleciona la pocicion dentro de una estructura
unsigned char secuencia[12] = {1,2,3,0,1,2,3,0,1,2,3,0};

//variable utilizadas
unsigned char size = 12;
unsigned char i = 0;
unsigned int j = 0;
unsigned char winer = 0;
unsigned char loser = 0;
unsigned char test;

int main(void){

	SysCtlClockSet(0x07803B80); // activate the clock to 8 MHZ
	baud=115200;
	UARTclk=SysCtlClockGet(); // Get the current speed clock
	ulDiv = (((UARTclk * 8) / baud) + 1) / 2;

	SYSCTL_RCGC2_R = (SYSCTL_RCGC2_GPIOF | SYSCTL_RCGC2_GPIOE | SYSCTL_RCGC2_GPIOA);// activate port E, F and A
	GPIO_PORTE_PUR_R |= 0x0F; 	// activate pull up recistance for port E
	GPIO_PORTE_DIR_R &= ~0x00;	//  set direction register as input
	GPIO_PORTE_AFSEL_R &= ~0x00; 	// regular port function
	GPIO_PORTE_DEN_R |= 0x0F; 	//  enable digital port
	GPIO_PORTF_DIR_R = 0x01; 	// make PORT F output 
	GPIO_PORTF_DEN_R = 0x01; 	// enable digital I/O on PF5

	//Configure the PORT A for the Serial Port
	GPIO_PORTA_DIR_R = 0x03;
	GPIO_PORTA_AFSEL_R = 0x23;
	GPIO_PORTA_DR2R_R = 0x0F;
	GPIO_PORTA_DR4R_R = 0x04;
	GPIO_PORTA_DR8R_R = 0x08;
	GPIO_PORTA_ODR_R = 0x0F;
	GPIO_PORTA_PUR_R = 0x13;
	GPIO_PORTA_PDR_R = 0x14;
	GPIO_PORTA_DEN_R = 0x1C;

	SYSCTL_RCGC1_R = (SYSCTL_RCGC1_TIMER0 | SYSCTL_RCGC1_UART0); //Enable TIMER0
	TIMER0_CTL_R = 0x00; // Cleared GPTM Control  ~ es de 8MHz
	TIMER0_CFG_R = 0x00; // Register Configures the GPTM module
	TIMER0_TAMR_R = 0x2; // GPTM TimerA Mode
	TIMER0_TAILR_R = 0x01E84800; // Value to load to TIMER0

	TIMER0_IMR_R = 0x01; // GPTM Interrupt Mask
	TIMER0_CTL_R = 0x01; // Start GPTM Control

	//configure the UART
	UART0_CTL_R = 0x0;
	UART0_CTL_R &= ~(0x00000020);
	UART0_IBRD_R = ulDiv / 64;
	UART0_FBRD_R = ulDiv % 64;
	UART0_LCRH_R = 0x60;	// 8 data bits
	UART0_FR_R &= 0x0;
	UART0_LCRH_R |= 0x00000010;
	UART0_CTL_R |= 0x00000301;

	test=0;

	//apuntador a la estructura de los botones
	//send((unsigned char *)caracter[secuencia[i]], 7);

	//NVIC_EN0_R = 1 << (21-16);
	//UART0_IM_R |= (0x010 | 0x040);

	GPIO_PORTF_DATA_R=0x01; //Data LED
	//siclo en el cual se iniciara la ejecucion
	

	send((unsigned char *)"HOLA\r\n",6);

	GPIO_PORTF_DATA_R=0x01; //Data LED
	
	
	while(1){
		data = GPIO_PORTE_DATA_R & 0x0F;
		GPIO_PORTF_DATA_R=0x01; //Data LED
	}
	
	
}



