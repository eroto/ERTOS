################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/main.c 

OBJS += \
./source/main.o 

C_DEPS += \
./source/main.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL25Z128VFM4" -I../ERTSch/ERTSch/InitApp -IC:/Users/uid87057/workspace.kds/ERTSch/startup -IC:/Users/uid87057/workspace.kds/ERTSch/board -IC:/Users/uid87057/workspace.kds/ERTSch/utilities -IC:/Users/uid87057/workspace.kds/ERTSch/CMSIS -IC:/Users/uid87057/workspace.kds/ERTSch/drivers -IC:/Users/uid87057/workspace.kds/ERTSch/Schr -IC:/Users/uid87057/workspace.kds/ERTSch/InitApp -IC:/Users/uid87057/workspace.kds/ERTSch/MyTypedef -IC:/Users/uid87057/workspace.kds/ERTSch/apptasks -I../ERTSch/Sys -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


