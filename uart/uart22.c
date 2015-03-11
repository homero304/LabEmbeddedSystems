//*****************************************************************************
//
// uart_echo.c - Example for reading data from and writing data to the UART in
//               an interrupt driven fashion.
//
// Copyright (c) 2005-2012 Texas Instruments Incorporated.  All rights reserved.
// Software License Agreement
// 
// Texas Instruments (TI) is supplying this software for use solely and
// exclusively on TI's microcontroller products. The software is owned by
// TI and/or its suppliers, and is protected under applicable copyright
// laws. You may not combine this software with "viral" open-source
// software in order to form a larger program.
// 
// THIS SOFTWARE IS PROVIDED "AS IS" AND WITH ALL FAULTS.
// NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT
// NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. TI SHALL NOT, UNDER ANY
// CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES, FOR ANY REASON WHATSOEVER.
// 
// This is part of revision 9453 of the EK-LM3S6965 Firmware Package.
//
//*****************************************************************************

#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "drivers/rit128x96x4.h"
static	unsigned char w[20]={0};
static	unsigned char v[20]={0};
static int ki=0,ij=0;	
unsigned char ad[4]={'1','1','0','1'};
//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>UART Echo (uart_echo)</h1>
//!
//! This example application utilizes the UART to echo text.  The first UART
//! (connected to the FTDI virtual serial port on the evaluation board) will be
//! configured in 115,200 baud, 8-n-1 mode.  All characters received on the
//! UART are transmitted back to the UART.
//
//*****************************************************************************

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif


//*****************************************************************************
//
// The UART interrupt handler.
//
//*****************************************************************************

void
UARTIntHandler(void)
{
	static	int i=0;
	long a;
    	unsigned long ulStatus;

    //
    // Get the interrrupt status.
    //
    ulStatus = UARTIntStatus(UART0_BASE, true);

    //
    // Clear the asserted interrupts.
    //
    UARTIntClear(UART0_BASE, ulStatus);

    //
    // Loop while there are characters in the receive FIFO.
	
    //
/*
    while(UARTCharsAvail(UART0_BASE))
    {
	
     
					i++;
        // Read the next character from the UART and write it back to the UART.
        //
				 a=(UARTCharGetNonBlocking(UART0_BASE));
    //    UARTCharPutNonBlocking(UART0_BASE, a);


		//	printf(a);
			RIT128x96x4StringDraw((const char *)&a ,     (i*5), 56, 15);
    }
*/
}



//*****************************************************************************
//
// Send a string to the UART.
//
//*****************************************************************************
void
UARTSend(const unsigned char *pucBuffer)
{
 char ii; 
	//
    // Loop while there are more characters to send.
    //
 //   while(u--)
do
    {
	ii++;
        //
        // Write the next character to the UART.
        //
       
	UARTCharPutNonBlocking(UART0_BASE, *pucBuffer++ );
       	if (ii==16) 
	{ 
	UARTDisable(UART0_BASE);UARTEnable(UART0_BASE);
	}
    }		while(*(pucBuffer) !=0x0);
}

//*****************************************************************************
//
//! Receives data over the UART port.
//!
//! \param pucData is the buffer to read data into from the UART port.
//! \param ulSize is the number of bytes provided in the \e pucData buffer that
//! should be written with data from the UART port.
//!
//! This function reads back \e ulSize bytes of data from the UART port, into
//! the buffer that is pointed to by \e pucData.  This function will not return
//! until \e ulSize number of bytes have been received.
//!
//! \return None.


//
//*****************************************************************************
char
UARTReceive(unsigned char *pucData, unsigned long ulSize)
{
  	char ii=0,ci=0;  
	//
	UARTDisable(UART0_BASE);
	UARTEnable(UART0_BASE);

    // Send out the number of bytes requested.
    //

  //  while(*(pucData-1)!=0xd) 
	while(ci<2) 
	{
      	  ii++;
        // Wait for the FIFO to not be empty.
        //
			
		*pucData++ =  UARTCharGet(UART0_BASE); 
		if (*(pucData-1)==0xd)
			 ci++; 
		else
      		RIT128x96x4StringDraw((const char *)(pucData-1), ki+(ii*7), ij, 15);

        // Receive a byte from the UART.

			
    	} 
		return(ii);
}

char
UARTReceive2(unsigned char *pucData, unsigned long ulSize)
{
  char ii=0,ci=0;  
	//
	UARTDisable(UART0_BASE);
	UARTEnable(UART0_BASE);

    // Send out the number of bytes requested.
    //

  //  while(*(pucData-1)!=0xd) 
	do// 	while(*(pucData-1)!=0) 
	{
        	ii++;
       
		*pucData++ =  UARTCharGet(UART0_BASE); 
		ci++;
      		RIT128x96x4StringDraw((const char *)(pucData-1),    ki+(ii*7), ij, 15);
		ulSize--;
     

			
	} 	while(ulSize!=0) ;

		return(ii);
}
UARTReceive3(unsigned char *pucData, unsigned long ulSize)
{
  char ii=0,ci=0,f=0,f2=0;  
	//
	

    // Send out the number of bytes requested.
    //

  //  while(*(pucData-1)!=0xd) 
	do// 	while(*(pucData-1)!=0) 
	{
      	 	ii++;
        // Wait for the FIFO to not be empty.
        //
		
		*pucData++ =  UARTCharGet(UART0_BASE); 
				 
      		RIT128x96x4StringDraw((const char *)(pucData-1),    ki+(ii*7), ij, 15);

        // Receive a byte from the UART.
		RIT128x96x4StringDraw((const char *)(pucData-1),    ki+(ii*7), 60, 15);

		if(ad[ci]==*(pucData-1)){
		RIT128x96x4StringDraw((const char *)(pucData-1),    ki+(ii*7), 79, 15);
		ci++;
		}
		else ci=0;
	
		if (ci==4) f=1;
			
		if ((f==1)&&(*(pucData-1)==0xD))f2=1;

	} while(f2==0) ;
		RIT128x96x4Clear();
		return(ii);
}

//*****************************************************************************
//
// This example demonstrates how to send a string of data to the UART.
//
//*****************************************************************************
void COMMAND(const unsigned char *pcStr)
{
//	char len;
	char a=0xd;
	ki=1;

	//char *p= &w[0];

	//do{
	ij=ij+8;
	RIT128x96x4StringDraw((const char *)pcStr ,     ki, ij, 15);
		
	UARTSend((const unsigned  char *)pcStr);
	
	a=0xD;
UARTSend((const unsigned char *)&a); 
		a=0xA; 		
	UARTSend((const unsigned char *)&a);
		ki=40;
	UARTReceive((unsigned char *)&w,2);


	
	
	
	//RIT128x96x4StringDraw((const char *)&w ,    40, 1+(ij*8), 15);

	ij++;
//}while (!((w[0] == 'O') && (w[1] =='K') ));

}
void COMMAND2(unsigned char *pcStr)
{

	static int ij=0;	
	char w[20];
	char a;
	RIT128x96x4StringDraw((const char *)pcStr ,     ki, ij, 15);

	do
	{
		ij++;
		UARTFIFODisable(UART0_BASE);
		UARTFIFOEnable(UART0_BASE);
	
		
		UARTSend((unsigned char *)pcStr);
		a=0x44;		
		UARTSend((unsigned char *)&a); 	
		a=0x38; 			
		UARTSend((unsigned char *)&a);
		/*	a=0x02; 				UARTSend((unsigned char *)&a);
			a=0x70; 				UARTSend((unsigned char *)&a);
		a=0x02; 				UARTSend((unsigned char *)&a);
			a=0x02; 				UARTSend((unsigned char *)&a);
		a=0x08; 				UARTSend((unsigned char *)&a);
			a=0x03; 				UARTSend((unsigned char *)&a);
		*/
		//UARTCharGetNonBlocking((unsigned long *) ulBase);
		UARTReceive2((unsigned char *)&w,1);
		RIT128x96x4StringDraw((const char *)&w ,    40, 1+(ij*8), 15);

		UARTDisable(UART0_BASE);
		UARTEnable(UART0_BASE);
	} while ((w[0] != 0x06));
	RIT128x96x4Clear();

}

int
main(void)
{
	
	char	c,u,d,a, HR,	SpO2,	LowBatt,SNSA;
	
	
    //
    // Set the clocking to run directly from the crystal.
    //
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_8MHZ);

    //
    // Initialize the OLED display and write status.
    //
    RIT128x96x4Init(1000000);
    RIT128x96x4StringDraw("UART Echo",            36,  0, 15);
    RIT128x96x4StringDraw("Port:   Uart 0",       12, 16, 15);
    RIT128x96x4StringDraw("Baud:   9600 bps",  12, 24, 15);
    RIT128x96x4StringDraw("Data:   8 Bit",        12, 32, 15);
    RIT128x96x4StringDraw("Parity: None",         12, 40, 15);
    RIT128x96x4StringDraw("Stop:   1 Bit",        12, 48, 15);

    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    //
    // Enable processor interrupts.
    //
    IntMasterEnable();

    //
    // Set GPIO A0 and A1 as UART pins.
    //
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //
    // Configure the UART for 115,200, 8-N-1 operation.
    //
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
                        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
                         UART_CONFIG_PAR_NONE));

 
UARTEnable(UART0_BASE);
    //
    // Prompt for text to be entered.
    //
//UARTSend("HOAL ",5);
/*RIT128x96x4Clear();
COMMAND( "ats0=-1");
COMMAND("ats512=4");
COMMAND("ate0");
COMMAND("ats538=1");*/
//COMMAND("at+btk=791545");
//COMMAND("at&w");
//UARTSend((const unsigned  char *)"atd00a0960e616b");
//UARTSend((const unsigned  char *)"atd001c0500e6ba");
//	a=0xD;
//UARTSend((const unsigned char *)&a); 	
//	a=0xA; 			
//UARTSend((const unsigned char *)&a);
//COMMAND("atd001c0500e6ba");
//COMMAND("at+btt?");
//UARTReceive((unsigned char *)&w,2);



//	RIT128x96x4Clear();
	UARTFIFODisable(UART0_BASE);
		UARTFIFOEnable(UART0_BASE);
//	ki=1;
//	ij=1;


//COMMAND2("d8");
RIT128x96x4Clear();

UARTSend("HOLA\0");
RIT128x96x4StringDraw("UART Echo",            36,  0, 15);


//  IntEnable(INT_UART0);
//	UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
    // Loop forever echoing data through the UART.
    //

		
}
