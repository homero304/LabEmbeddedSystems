//*****************************************************************************
//
// startup_gcc.c - Startup code for use with GNU tools.
//
// Copyright (c) 2007-2010 Texas Instruments Incorporated.  All rights reserved.
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
// This is part of revision 6459 of the EK-LM3S6965 Firmware Package.
//
//*****************************************************************************
#include <includes.h>
#define STACK_SIZE 1024
//*****************************************************************************
//
// Forward declaration of the default fault handlers.
//
//*****************************************************************************
void ResetISR(void);
static void NmiSR(void);
static void FaultISR(void);
static void IntDefaultHandler(void);

//*****************************************************************************
//
// The entry point for the application.
//
//*****************************************************************************
extern int main(void);

//*****************************************************************************
//
// Reserve space for the system stack.
//
//*****************************************************************************

//static unsigned long pulStack[1024] __attribute__ ((section(".stack")));
__attribute__ ((section(".stack")))
static unsigned long pulStack[0x100];

//*****************************************************************************
//
// The vector table.  Note that the proper constructs must be placed on this to
// ensure that it ends up at physical address 0x0000.0000.
//
//*****************************************************************************
__attribute__ ((section(".isr_vector")))
void (* const g_pfnVectors[])(void) =
{
    (void (*)(void))((unsigned long)pulStack + sizeof(pulStack)),
                                            // The initial stack pointer
    ResetISR,                               // The reset handler
    NmiSR,                                  // The NMI handler
    FaultISR,                               // The hard fault handler
    IntDefaultHandler,                      // The MPU fault handler
    IntDefaultHandler,                      // The bus fault handler
    IntDefaultHandler,                      // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    IntDefaultHandler,                      // SVCall handler
    IntDefaultHandler,                      // Debug monitor handler
    0,                                      // Reserved
    OS_CPU_PendSVHandler,                   // The PendSV handler
    OS_CPU_SysTickHandler,                  // The SysTick handler
    BSP_IntHandlerGPIOA, 		// GPIO Port A
    BSP_IntHandlerGPIOB, 		// GPIO Port B
    BSP_IntHandlerGPIOC, 		// GPIO Port C	
    BSP_IntHandlerGPIOD, 		// GPIO Port D
    BSP_IntHandlerGPIOE, 		// GPIO Port E
    BSP_IntHandlerUART0, 		// UART0 Rx and Tx
    BSP_IntHandlerUART1, 		// UART1 Rx and Tx
    BSP_IntHandlerSSI0, 		// SSI Rx and Tx
    BSP_IntHandlerI2C0, 		// I2C Master and Slave
    BSP_IntHandlerPWM_FAULT,		// PWM Fault
    BSP_IntHandlerPWM_GEN0, 		// PWM Generator 0
    BSP_IntHandlerPWM_GEN1, 		// PWM Generator 1
    BSP_IntHandlerPWM_GEN2, 		// PWM Generator 2
    BSP_IntHandlerQEI0, 		// Quadrature Encoder
    BSP_IntHandlerADC0, 		// ADC Sequence 0
    BSP_IntHandlerADC1, 		// ADC Sequence 1
    BSP_IntHandlerADC2, 		// ADC Sequence 2
    BSP_IntHandlerADC3, 		// ADC Sequence 3
    BSP_IntHandlerWATCHDOG, 	// Watchdog timer
    BSP_IntHandlerTIMER0A, 	// Timer 0 subtimer A
    BSP_IntHandlerTIMER0B, 	// Timer 0 subtimer B
    BSP_IntHandlerTIMER1A, 	// Timer 1 subtimer A
    BSP_IntHandlerTIMER1B, 	// Timer 1 subtimer B
    BSP_IntHandlerTIMER2A, 	// Timer 2 subtimer A
    BSP_IntHandlerTIMER2B, 	// Timer 2 subtimer B
    BSP_IntHandlerCOMP0, 	// Analog Comparator 0
    BSP_IntHandlerCOMP1, 	// Analog Comparator 1
    IntDefaultHandler,		// Analog Comparator 2
    BSP_IntHandlerSYSCTL, 	// System Control (PLL, OSC, BO)
    BSP_IntHandlerFLASH, 	// FLASH Control
    BSP_IntHandlerGPIOF, 	// GPIO Port F
    BSP_IntHandlerGPIOG, 	// GPIO Port G
    IntDefaultHandler,  	// GPIO Port H
    BSP_IntHandlerUART2, 	// UART2 Rx and Tx
    IntDefaultHandler,  	// SSI1 Rx and Tx
    BSP_IntHandlerTIMER3A, 	// Timer 3 subtimer A
    BSP_IntHandlerTIMER3B, 	// Timer 3 subtimer B
    BSP_IntHandlerI2C1, 	// I2C1 Master and Slave
    BSP_IntHandlerQEI1, 	// Quadrature Encoder 1
    IntDefaultHandler, 		// CAN0
    IntDefaultHandler,  	// CAN1
    IntDefaultHandler,  	// Reserved
    BSP_IntHandlerETH, 		// Ethernet
    BSP_IntHandlerHIBERNATE 	// Hibernate
};

//*****************************************************************************
//
// The following are constructs created by the linker, indicating where the
// the "data" and "bss" segments reside in memory.  The initializers for the
// for the "data" segment resides immediately following the "text" segment.
//
//*****************************************************************************
extern unsigned long _flash_data;
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;
extern unsigned long _istk;
extern unsigned long _estk;

//*****************************************************************************
//
// This is the code that gets called when the processor first starts execution
// following a reset event.  Only the absolutely necessary set is performed,
// after which the application supplied entry() routine is called.  Any fancy
// actions (such as making decisions based on the reset cause register, and
// resetting the bits in that register) are left solely in the hands of the
// application.
//
//*****************************************************************************
void
ResetISR(void)
{
    unsigned long *pulSrc, *pulDest;

    //
    // Copy the data segment initializers from flash to SRAM.
    //
    pulSrc = &_etext;
    for(pulDest = &_data; pulDest < &_edata; )
    {
        *pulDest++ = *pulSrc++;
    }

    //
    // Zero fill the bss segment.
    //
    __asm("    ldr     r0, =_bss\n"
          "    ldr     r1, =_ebss\n"
          "    mov     r2, #0\n"
          "    .thumb_func\n"
          "zero_loop:\n"
          "        cmp     r0, r1\n"
          "        it      lt\n"
          "        strlt   r2, [r0], #4\n"
          "        blt     zero_loop");

    //
    // Call the application's entry point.
    //
    main();
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a NMI.  This
// simply enters an infinite loop, preserving the system state for examination
// by a debugger.
//
//*****************************************************************************
static void
NmiSR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives a fault
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
FaultISR(void)
{
    //
    // Enter an infinite loop.
    //
    while(1)
    {
    }
}

//*****************************************************************************
//
// This is the code that gets called when the processor receives an unexpected
// interrupt.  This simply enters an infinite loop, preserving the system state
// for examination by a debugger.
//
//*****************************************************************************
static void
IntDefaultHandler(void)
{
    //
    // Go into an infinite loop.
    //
    while(1)
    {
    }
}

int
uipprintf(const char *fmt, ...)
{
    return(0);
}

/*
int
vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    return(0);
}
*/
