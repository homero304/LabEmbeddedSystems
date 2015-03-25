PROJECT=ucosii
UCOSII_SOURCE_SOFTWARE=/home/usuario/workspace_ucosii/Micrium/Software
LUMINARY_DRIVER_DIR=/home/usuario/workspace_ucosii/StellarisWare
CODESOURCERY_DIR=/home/usuario/tools/arm-2011.03
CC=$(CODESOURCERY_DIR)/bin/arm-none-eabi-gcc
ASM=$(CODESOURCERY_DIR)/bin/arm-none-eabi-as
OBJCOPY=$(CODESOURCERY_DIR)/bin/arm-none-eabi-objcopy
OBJDUMP=$(CODESOURCERY_DIR)/bin/arm-none-eabi-objdump
LDSCRIPT=$(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/OS/ucosii.ld

# should use --gc-sections but the debugger does not seem to be able to cope with the option.
#LINKER_FLAGS=-nostartfiles -nostdlib -lgcc -Xlinker -o$(PROJECT).elf -Xlinker -M -Xlinker -Map=$(PROJECT).map -Xlinker --no-gc-sections 
LINKER_FLAGS=-nostartfiles -Xlinker -o$(PROJECT).elf -Xlinker -M -Xlinker -Map=$(PROJECT).map -Xlinker --no-gc-sections 
#DEBUG= -g -gdwarf-2
DEBUG= -g
OPTIM= -O0
AFLAGS= -mcpu=cortex-m3 -mthumb-interwork -g


CFLAGS=$(DEBUG) -I . \
		-I $(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/BSP/uCOS-II\
		-I $(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/BSP \
		-I $(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/OS \
		-I $(UCOSII_SOURCE_SOFTWARE)/uC-CPU \
		-I $(UCOSII_SOURCE_SOFTWARE)/uC-CPU/ARM-Cortex-M3/GNU \
		-I $(UCOSII_SOURCE_SOFTWARE)/uC-LIB \
		-I $(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Ports/ARM-Cortex-M3/Generic/GNU \
		-I $(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Source \
		-I $(LUMINARY_DRIVER_DIR) \
		-I $(LUMINARY_DRIVER_DIR)/driverlib \
		-I $(LUMINARY_DRIVER_DIR)/utils \
		-I $(LUMINARY_DRIVER_DIR)/drivers \
		-I $(LUMINARY_DRIVER_DIR)/inc \
		-D inline= -mthumb -mcpu=cortex-m3 -mthumb-interwork $(OPTIM) \
		-D sprintf=usprintf -D snprintf=usnprintf -D printf=uipprintf -D vsnprintf=uvsnprintf \
		-T$(LDSCRIPT) \
		-D PACK_STRUCT_END=__attribute\(\(packed\)\) -D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) 
#		-I $(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/BSP/RIT/ \
#-D GCC_ARMCM3_LM3S102 -ffunction-sections -fdata-sections -fnon-call-exceptions -fno-unwind-tables\

CSOURCE=	$(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/OS/startup_gcc.c \
		$(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/OS/os_app_hooks.c \
		$(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/OS/app.c \
		$(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/BSP/uCOS-II/bsp_os.c\
		$(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/BSP/bsp.c \
		$(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/BSP/cpu_bsp.c \
		$(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/BSP/bsp_int.c \
		$(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/BSP/bsp_ser.c \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Ports/ARM-Cortex-M3/Generic/GNU/os_cpu_c.c \
		$(UCOSII_SOURCE_SOFTWARE)/uC-CPU/ARM-Cortex-M3/GNU/cpu_c.c \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Ports/ARM-Cortex-M3/Generic/GNU/os_dbg.c \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Source/os_core.c \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Source/os_flag.c \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Source/os_mbox.c \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Source/os_mem.c \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Source/os_mutex.c \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Source/os_q.c \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Source/os_sem.c \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Source/os_task.c \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Source/os_time.c \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Source/os_tmr.c \
		$(UCOSII_SOURCE_SOFTWARE)/uC-LIB/lib_ascii.c \
		$(UCOSII_SOURCE_SOFTWARE)/uC-LIB/lib_math.c \
		$(UCOSII_SOURCE_SOFTWARE)/uC-LIB/lib_str.c \
		$(UCOSII_SOURCE_SOFTWARE)/uC-LIB/lib_mem.c \
		$(UCOSII_SOURCE_SOFTWARE)/uC-CPU/cpu_core.c \
		$(LUMINARY_DRIVER_DIR)/drivers/rit128x96x4.c\
		$(LUMINARY_DRIVER_DIR)/driverlib/cpu.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/sysctl.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/gpio.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/watchdog.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/usb.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/udma.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/uart.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/timer.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/systick.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/ssi.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/qei.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/pwm.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/mpu.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/interrupt.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/i2c.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/hibernate.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/flash.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/ethernet.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/comp.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/can.c \
		$(LUMINARY_DRIVER_DIR)/utils/ustdlib.c \
		$(LUMINARY_DRIVER_DIR)/driverlib/adc.c 
 
#		$(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/OS/syscall.c \
#		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Source/ucos_ii.c \	
#		$(UCOSII_SOURCE_SOFTWARE)/uMon/monlib.c \



ASOURCE=	$(UCOSII_SOURCE_SOFTWARE)/uC-LIB/Ports/ARM-Cortex-M3/GNU/lib_mem_a.s \
		$(UCOSII_SOURCE_SOFTWARE)/uC-CPU/ARM-Cortex-M3/GNU/cpu_a.s \
		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Ports/ARM-Cortex-M3/Generic/GNU/os_cpu_a.s 
#		$(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/OS/startup.s 
#		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Ports/ARM/Generic/GNU/os_cpu_fpu_a.s \
		

#LIBS= $(LUMINARY_DRIVER_DIR)/driverlib/gcc/libdriver.a

OBJS = $(ASOURCE:.s=.o) $(CSOURCE:.c=.o) 

all: $(OBJS) $(PROJECT).bin $(PROJECT).elf $(PROJECT).lst

%o : %s Makefile
	$(ASM) $(DEBUG) $(AFLAGS) $< -o $@

%o : %c Makefile
	$(CC) -c $(CFLAGS) $< -o $@

%elf : $(OBJS) Makefile
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(LINKER_FLAGS)
	
%bin : %elf Makefile
	$(OBJCOPY) $< -O binary $@
	
%lst : %elf Makefile
	$(OBJDUMP) $< -S > $@

clean :
	rm -f $(OBJS)
	rm -f $(PROJECT).bin
	rm -f $(PROJECT).elf
	rm -f $(PROJECT).map
	rm -f $(PROJECT).lst

