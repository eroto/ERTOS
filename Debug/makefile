################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include utilities/subdir.mk
-include startup/subdir.mk
-include source/subdir.mk
-include pm/subdir.mk
-include frdmkl25z/subdir.mk
-include drivers/subdir.mk
-include apptasks/subdir.mk
-include Timers/subdir.mk
-include SerialCom/subdir.mk
-include Schr/subdir.mk
-include Relay/subdir.mk
-include RTC/subdir.mk
-include LCD1602A/subdir.mk
-include InitApp/subdir.mk
-include IO/subdir.mk
-include CMSIS/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: ERTOS.axf

# Tool invocations
ERTOS.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="ERTOS.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0plus -mthumb -T ERTOS_Debug.ld -o "ERTOS.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) ERTOS.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "ERTOS.axf" ; checksum -p MKL25Z128xxx4 -d "ERTOS.hex" ; # arm-none-eabi-objcopy -v -O binary "ERTOS.axf" "ERTOS.bin" ; # arm-none-eabi-objcopy -O ihex "ERTOS.axf" "ERTOS.hex"
	-@echo ' '

.PHONY: all clean dependents post-build

-include ../makefile.targets
