################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Sys/Events.c \
../Sys/INT_PIT.c \
../Sys/PE_LDD.c \
../Sys/cpu.c \
../Sys/vector_tbl.c 

OBJS += \
./Sys/Events.o \
./Sys/INT_PIT.o \
./Sys/PE_LDD.o \
./Sys/cpu.o \
./Sys/vector_tbl.o 

C_DEPS += \
./Sys/Events.d \
./Sys/INT_PIT.d \
./Sys/PE_LDD.d \
./Sys/cpu.d \
./Sys/vector_tbl.d 


# Each subdirectory must supply rules for building sources it contributes
Sys/%.o: ../Sys/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL25Z128VFM4" -IC:/Users/uid87057/workspace.kds/ERTSch/startup -IC:/Users/uid87057/workspace.kds/ERTSch/board -IC:/Users/uid87057/workspace.kds/ERTSch/utilities -IC:/Users/uid87057/workspace.kds/ERTSch/CMSIS -IC:/Users/uid87057/workspace.kds/ERTSch/drivers -IC:/Users/uid87057/workspace.kds/ERTSch/Schr -IC:/Users/uid87057/workspace.kds/ERTSch/InitApp -IC:/Users/uid87057/workspace.kds/ERTSch/MyTypedef -IC:/Users/uid87057/workspace.kds/ERTSch/apptasks -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


