################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Schr/Schr.c \
../Schr/schm_trm_isr.c 

OBJS += \
./Schr/Schr.o \
./Schr/schm_trm_isr.o 

C_DEPS += \
./Schr/Schr.d \
./Schr/schm_trm_isr.d 


# Each subdirectory must supply rules for building sources it contributes
Schr/%.o: ../Schr/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -D"CPU_MKL25Z128VFM4" -I../startup -I../board -I../utilities -I../CMSIS -I../drivers -I"C:\Users\uid87057\workspace.kds\ERTSch\InitApp" -I"C:\Users\uid87057\workspace.kds\ERTSch\MyTypedef" -I"C:\Users\uid87057\workspace.kds\ERTSch\Schr" -I"C:\Users\uid87057\workspace.kds\ERTSch\apptasks" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


