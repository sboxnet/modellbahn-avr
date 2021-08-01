################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../bootloader-crt0.c \
../bootloader-fw.c \
../common.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
bootloader-crt0.o \
bootloader-fw.o \
common.o

OBJS_AS_ARGS +=  \
bootloader-crt0.o \
bootloader-fw.o \
common.o

C_DEPS +=  \
bootloader-crt0.d \
bootloader-fw.d \
common.d

C_DEPS_AS_ARGS +=  \
bootloader-crt0.d \
bootloader-fw.d \
common.d

OUTPUT_FILE_PATH +=sboxnet.elf

OUTPUT_FILE_PATH_AS_ARGS +=sboxnet.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler
./bootloader-crt0.o: .././bootloader-crt0.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\XMEGAA_DFP\1.2.141\include" -I"C:\Users\balagi\Documents\modellbahn\avr" -I"C:\Users\balagi\Documents\modellbahn\avr\sboxnet\sboxnet"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega128a4u -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\XMEGAA_DFP\1.2.141\gcc\dev\atxmega128a4u" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./bootloader-fw.o: .././bootloader-fw.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\XMEGAA_DFP\1.2.141\include" -I"C:\Users\balagi\Documents\modellbahn\avr" -I"C:\Users\balagi\Documents\modellbahn\avr\sboxnet\sboxnet"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega128a4u -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\XMEGAA_DFP\1.2.141\gcc\dev\atxmega128a4u" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./common.o: .././common.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\XMEGAA_DFP\1.2.141\include" -I"C:\Users\balagi\Documents\modellbahn\avr" -I"C:\Users\balagi\Documents\modellbahn\avr\sboxnet\sboxnet"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall -mmcu=atxmega128a4u -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\XMEGAA_DFP\1.2.141\gcc\dev\atxmega128a4u" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	




# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="sboxnet.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atxmega128a4u -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\XMEGAA_DFP\1.2.141\gcc\dev\atxmega128a4u" -Wl,--section-start=.appcrc=10000  -Wl,--section-start=.bldrapp=10100 -Wl,--section-start=.bldrinit=20000 -Wl,--section-start=.bldrvec=20200 -Wl,--section-start=.bootloader=20242 -Wl,--section-start=.bldrcrc=21dfe  -Wl,--section-start=.bldrstatic=21b00 -Wl,--section-start=.bldrstatvec=21ff6  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "sboxnet.elf" "sboxnet.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "sboxnet.elf" "sboxnet.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "sboxnet.elf" > "sboxnet.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "sboxnet.elf" "sboxnet.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "sboxnet.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "sboxnet.elf" "sboxnet.a" "sboxnet.hex" "sboxnet.lss" "sboxnet.eep" "sboxnet.map" "sboxnet.srec" "sboxnet.usersignatures"
	