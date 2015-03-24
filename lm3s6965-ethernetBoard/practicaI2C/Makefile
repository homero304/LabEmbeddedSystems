PROJECT=I2C
LUMINARY_DRIVER_DIR=/home/user/workspace_ucosii/StellarisWare

CC=/usr/bin/arm-none-eabi-gcc
ASM=/usr/bin/arm-none-eabi-as
OBJCOPY=/usr/bin/arm-none-eabi-objcopy
LDSCRIPT=I2C.ld

LINKER_FLAGS=-nostartfiles -Xlinker -o$(PROJECT).elf -Xlinker -M -Xlinker -Map=$(PROJECT).map -Xlinker --no-gc-sections

DEBUG= -g -gdwarf-2
OPTIM= -O2

CFLAGS=$(DEBUG) -I . \
		-I $(LUMINARY_DRIVER_DIR) \
		-D inline= -mthumb -mcpu=cortex-m3 $(OPTIM) \
		-T$(LDSCRIPT) 

CSOURCE= 	$(LUMINARY_DRIVER_DIR)/drivers/rit128x96x4.c \
		I2C.c \
		startup_gcc.c
 
LIBS= $(LUMINARY_DRIVER_DIR)/driverlib/gcc/libdriver.a
OBJS = $(CSOURCE:.c=.o) 

all: $(OBJS) $(PROJECT).bin $(PROJECT).elf
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





