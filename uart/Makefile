PROJECT=uart
LUMINARY_DRIVER_DIR=/home/usuario/workspace_ucosii/StellarisWare

CC=/home/usuario/tools/arm-2011.03/bin/arm-none-eabi-gcc
ASM=/home/usuario/tools/arm-2011.03/bin/arm-none-eabi-as
OBJCOPY=/home/usuario/tools/arm-2011.03/bin/arm-none-eabi-objcopy
LDSCRIPT=uart.ld

# should use --gc-sections but the debugger does not seem to be able to cope with the option.
LINKER_FLAGS=-nostartfiles -Xlinker -o$(PROJECT).elf -Xlinker -M -Xlinker -Map=$(PROJECT).map -Xlinker --no-gc-sections

DEBUG= -g -gdwarf-2
#DEBUG=
OPTIM= -O2
#AFLAGS= -mcpu=cortex-m3

CFLAGS=$(DEBUG) -I . \
		-I $(LUMINARY_DRIVER_DIR) \
		-D inline= -mthumb -mcpu=cortex-m3 $(OPTIM) \
		-T$(LDSCRIPT) 
#		-D PACK_STRUCT_END=__attribute\(\(packed\)\) -D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) \
#		-D sprintf=usprintf -D snprintf=usnprintf -D printf=uipprintf
#		-I $(UCOSII_SOURCE_SOFTWARE)/uMon/ \

CSOURCE= 	$(LUMINARY_DRIVER_DIR)/drivers/rit128x96x4.c \
		uart.c \
		startup_gcc.c

#ASOURCE=	$(UCOSII_SOURCE_SOFTWARE)/uC-LIB/Ports/ARM-Cortex-M3/GNU/lib_mem_a.s \
#		$(UCOSII_SOURCE_SOFTWARE)/uC-CPU/ARM-Cortex-M3/GNU/cpu_a.s \
#		$(UCOSII_SOURCE_SOFTWARE)/uCOS-II/Ports/ARM-Cortex-M3/Generic/GNU/os_cpu_a.s 
#		$(UCOSII_SOURCE_SOFTWARE)/EvalBoards/LuminaryMicro/EK-LM3S6965-RevC/GNU/OS/startup.s 

LIBS= $(LUMINARY_DRIVER_DIR)/driverlib/gcc/libdriver.a

#OBJS = $(ASOURCE:.s=.o) $(CSOURCE:.c=.o) 
OBJS = $(CSOURCE:.c=.o) 

all: $(OBJS) $(PROJECT).bin $(PROJECT).elf

#%o : %s Makefile
#	$(ASM) $(DEBUG) $(AFLAGS) $< -o $@

%o : %c Makefile
	$(CC) -c $(CFLAGS) $< -o $@

%elf : $(OBJS) Makefile
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) $(LINKER_FLAGS)

%bin : %elf Makefile
	$(OBJCOPY) $< -O binary $@
		
clean :
	rm -f $(PROJECT).elf
	rm -f $(PROJECT).map
	rm -f $(PROJECT).bin
	rm -f $(PROJECT).lst



