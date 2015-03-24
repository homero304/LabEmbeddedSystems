#include "inc/lm3s6965.h"
#include "inc/hw_memmap.h"
#include "inc/hw_ints.h"
#include "inc/hw_types.h"
#include "inc/hw_timer.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "drivers/rit128x96x4.h"
#include "driverlib/i2c.h"
#include "utils/ustdlib.c"


//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//****************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

 
const unsigned int DELAY=1;
char  data, chosen;
int pause = 0;
int delayflag = 0;
static unsigned int indexme = 0;
int i;
const unsigned long SLAVE_ADDRESS = 0x68;

char *names[] = {"seconds", "minutes", "hours", "weekDay", "day", "month", "year"};
char *weekdays[] = {"", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"};
static unsigned long RTCinfo[7];
char buff [20];
void setTime(){
//set Time
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, SLAVE_ADDRESS, false);
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}        

   	I2CMasterDataPut(I2C0_MASTER_BASE, 0x00);      //Pedir primer registro y mandar START
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START); 
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}
	for(i=0; i<7; i++){ //Ir escribiendo los datos del arreglo
		I2CMasterDataPut(I2C0_MASTER_BASE, RTCinfo[i]);      
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); 
		while(I2CMasterBusy(I2C0_MASTER_BASE)){}	
	}

	I2CMasterDataPut(I2C0_MASTER_BASE, 0x80);      
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);      //Mandar la senal de FINISH 
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}  

}

void readTime(){
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, SLAVE_ADDRESS, false);
	
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}     
	
	I2CMasterDataPut(I2C0_MASTER_BASE, 0x00);      //Pedir primer registro y mandar START
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_SEND); 
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}
		
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, SLAVE_ADDRESS, true);
	
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}     
	for(i=0; i<7; i++){       
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE); 
		while(I2CMasterBusy(I2C0_MASTER_BASE)){}
		RTCinfo[i] = I2CMasterDataGet(I2C0_MASTER_BASE);	
	}
}
int decimalToHex(int data){
		return (data/10 << 4) | (data%10);
}
int hexToDecimal(int data){
	return ((data & 0xF0)>>4)*10 + (data & 0x0F);
}
void updatePlus(){
	int data = 0;
	switch(indexme)
	{
		case 0:
		case 1:
			if(hexToDecimal(RTCinfo[indexme]) < 59){
				data = hexToDecimal(RTCinfo[indexme]); data++; 
				RTCinfo[indexme] = decimalToHex(data);
			}
			break;	
		case 2:
			if(hexToDecimal(RTCinfo[2]) < 23){
				data = hexToDecimal(RTCinfo[2]); data++; 
				RTCinfo[2] = decimalToHex(data);
			}
			break;	
		case 3: 
			if(RTCinfo[3] < 7) RTCinfo[3]++;
			break;	
		case 4:

			switch(RTCinfo[5]){
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 0x10:
				case 0x12:
					if(hexToDecimal(RTCinfo[4]) < 31){
						data = hexToDecimal(RTCinfo[4]); data++; 
						RTCinfo[4] = decimalToHex(data);
					}
					break;
				case 4:
				case 6:
				case 9:
				case 0x11:
					if(hexToDecimal(RTCinfo[4]) < 30){
						data = hexToDecimal(RTCinfo[4]); data++; 
						RTCinfo[4] = decimalToHex(data);
					}
					break;
				case 2:
					if(hexToDecimal(RTCinfo[6])%4==0)//anio bisiesto
					{
						if(hexToDecimal(RTCinfo[4]) < 29){
							data = hexToDecimal(RTCinfo[4]); data++; 
							RTCinfo[4] = decimalToHex(data);
						}
					}
					else
					{
						if(hexToDecimal(RTCinfo[4]) < 28){
							data = hexToDecimal(RTCinfo[4]); data++; 
							RTCinfo[4] = decimalToHex(data);
						}
					}
					break;
				default:
					break;
			}
			break;
		case 5:
			if(hexToDecimal(RTCinfo[5]) < 12){
				data = hexToDecimal(RTCinfo[5]); data++; 
				RTCinfo[5] = decimalToHex(data);
			}
			break;
		case 6:
			data = hexToDecimal(RTCinfo[6]); data++; 
			RTCinfo[6] = decimalToHex(data);
			break;
		default:
			break;
				
	}
}
void updateMinus(){
	int data = 0;
	switch(indexme)
	{
		case 0:
		case 1:
		case 2:
		case 6:
			if(hexToDecimal(RTCinfo[indexme]) > 0){
				data = hexToDecimal(RTCinfo[indexme]); data--;
				RTCinfo[indexme] = decimalToHex(data);
			}
			break;	
	
		case 3: 
			if(RTCinfo[3] > 1) RTCinfo[3]--;
			break;	
		case 4:
			if(hexToDecimal(RTCinfo[4]) > 1) 
			{
				data = hexToDecimal(RTCinfo[4]); data--;
				RTCinfo[4] = decimalToHex(data);
			}
			break;
		case 5:
			switch(RTCinfo[5]){
				case 1:
				case 2:
				case 3:
				case 5:
				case 7:
				case 8:
				case 4:
				case 6:
				case 9:
					if(RTCinfo[5] > 1) RTCinfo[5]--;
					break;
				case 0x10:
					RTCinfo[5] = 9;
					break;
				case 0x11:
					RTCinfo[5] = 0x10;
					break;
				case 0x12:
					RTCinfo[5] = 0x11;
					break;
				default:
					break;
			};
		default:
			break;	
	}
}

void stop(){
	//set Time
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, SLAVE_ADDRESS, false);
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}        

   	I2CMasterDataPut(I2C0_MASTER_BASE, 0x00);      //Pedir primer registro y mandar START
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START); 
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}
	
		I2CMasterDataPut(I2C0_MASTER_BASE, RTCinfo[0]|0x80);      
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); 
		while(I2CMasterBusy(I2C0_MASTER_BASE)){}	
	

	I2CMasterDataPut(I2C0_MASTER_BASE, 0x80);      
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);      //Mandar la senal de FINISH 
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}  
}
void start(){
	//set Time
	I2CMasterSlaveAddrSet(I2C0_MASTER_BASE, SLAVE_ADDRESS, false);
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}        

   	I2CMasterDataPut(I2C0_MASTER_BASE, 0x00);      //Pedir primer registro y mandar START
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_START); 
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}
	
		I2CMasterDataPut(I2C0_MASTER_BASE, RTCinfo[0]);      
		I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_CONT); 
		while(I2CMasterBusy(I2C0_MASTER_BASE)){}	
	

	I2CMasterDataPut(I2C0_MASTER_BASE, 0x80);      
	I2CMasterControl(I2C0_MASTER_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);      //Mandar la senal de FINISH 
	while(I2CMasterBusy(I2C0_MASTER_BASE)){}  
}

void delay(void)
{
	TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
	delayflag = 1;	
	data= ((GPIO_PORTF_DATA_R<<3)|GPIO_PORTE_DATA_R);
	chosen =  data & 0x1f;
	if(chosen != 0x1F){
		if(chosen == 0x1E){   	//boton UP
			if(pause){
				updatePlus();
			}		
		}
		else if(chosen == 0x17) //boton RIGHT
		{
			if(pause){	
				if(indexme > 0) indexme--;
			}		
		}
		else if(chosen == 0x1B)  //boton LEFT
		{			
			if(pause){
				if(indexme < 6)indexme++;
			}
				
		}
		else if(chosen == 0x1D)	//boton DOWN toggle del LED mediante 10segundos
		{
			if(pause){	
				updateMinus();
			}	
		
		}
		else if(chosen == 0x0F)
		{
			if(!pause){
				 stop();
				
			}
			else 
			{
				start();
				setTime();
				
			}
			pause = pause ^ 0x01;
		}
		
	}
	
}


int main(void)
{
	
	SYSCTL_RCGC2_R = (SYSCTL_RCGC2_GPIOF | SYSCTL_RCGC2_GPIOE);     // Activa puerto E y F
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);	//Timer1
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |SYSCTL_XTAL_8MHZ); //Configura el reloj a 8MHz
		
	TimerConfigure(TIMER1_BASE,TIMER_CFG_32_BIT_PER);
	TimerLoadSet(TIMER1_BASE, TIMER_A, (DELAY*SysCtlClockGet())>>2);  //delay solution for button rebound
		  
				//Habilitar los timer
	TimerEnable(TIMER1_BASE,TIMER_BOTH);
	IntMasterEnable();
	
	TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);

	TimerIntRegister(TIMER1_BASE,TIMER_BOTH, delay);
	GPIO_PORTE_DIR_R = 0x00;	// Declara PORT E entrada
	GPIO_PORTE_DEN_R = 0x0F;	// Entradas y salidas Digitales
	GPIO_PORTF_DIR_R = 0x00; 	// Declara PORT F entrada
	GPIO_PORTF_DEN_R = 0x02; 	// Entradas y salidas Digitales
	GPIO_PORTE_PUR_R |= 0x0F;   //Bitwise OR con Registro PUR
	GPIO_PORTF_PUR_R |= 0x02;


	

	RTCinfo[0] = 0; // seconds
	RTCinfo[1] = 0x37; // min
	RTCinfo[2] = 0x16; // hrs
	RTCinfo[3] = 1; //day of week
	RTCinfo[4] = 0x23; //day
	RTCinfo[5] = 0x03; //month
	RTCinfo[6] = 0x15; //year

	//init i2c
	SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);    
	SysCtlPeripheralReset(SYSCTL_PERIPH_I2C0);  
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);        
    	GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_2 | GPIO_PIN_3);
	I2CMasterInitExpClk(I2C0_MASTER_BASE, SysCtlClockGet(), false);//false for 100kHz mode
	start();
	RIT128x96x4Init(1000000);
	while(1)
	{
	
		if(!pause){

			readTime();
			usprintf (buff, "%02x/%02x/%02x %02x:%02x:%02x",RTCinfo[4],RTCinfo[5],RTCinfo[6],RTCinfo[2],RTCinfo[1],RTCinfo[0]);
			RIT128x96x4StringDraw(buff, 0, 14, 25);
			RIT128x96x4StringDraw(weekdays[RTCinfo[3]], 0, 25, 25);
		}
		else
		{
			
			usprintf (buff, "%02x/%02x/%02x %02x:%02x:%02x",RTCinfo[4],RTCinfo[5],RTCinfo[6],RTCinfo[2],RTCinfo[1],RTCinfo[0]);
			RIT128x96x4StringDraw(buff, 0, 14, 25);
			RIT128x96x4StringDraw(weekdays[RTCinfo[3]], 0, 25, 25);
			RIT128x96x4StringDraw("paused", 0, 35, 25);
			usprintf (buff, "modify %s", names[indexme]);
			RIT128x96x4StringDraw(buff, 0, 45, 25);
			if(delayflag){
				RIT128x96x4Clear();
				delayflag = 0;
			}
		}
				
	}
}


			


