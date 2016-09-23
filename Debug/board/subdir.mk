################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/pin_mux.c 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/pin_mux.o 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL25Z128VFM4" -I../ERTSch/ERTSch/InitApp -IC:/Users/uid87057/workspace.kds/ERTSch/startup -IC:/Users/uid87057/workspace.kds/ERTSch/board -IC:/Users/uid87057/workspace.kds/ERTSch/utilities -IC:/Users/uid87057/workspace.kds/ERTSch/CMSIS -IC:/Users/uid87057/workspace.kds/ERTSch/drivers -IC:/Users/uid87057/workspace.kds/ERTSch/Schr -IC:/Users/uid87057/workspace.kds/ERTSch/InitApp -IC:/Users/uid87057/workspace.kds/ERTSch/MyTypedef -IC:/Users/uid87057/workspace.kds/ERTSch/apptasks -I../ERTSch/Sys -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


