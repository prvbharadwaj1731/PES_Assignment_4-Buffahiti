################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/PES_Assignment_4.c \
../source/led_control.c \
../source/mtb.c \
../source/pwm.c \
../source/semihost_hardfault.c \
../source/timers.c \
../source/tsi.c 

OBJS += \
./source/PES_Assignment_4.o \
./source/led_control.o \
./source/mtb.o \
./source/pwm.o \
./source/semihost_hardfault.o \
./source/timers.o \
./source/tsi.o 

C_DEPS += \
./source/PES_Assignment_4.d \
./source/led_control.d \
./source/mtb.d \
./source/pwm.d \
./source/semihost_hardfault.d \
./source/timers.d \
./source/tsi.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\prvbh\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Assignment_4\board" -I"C:\Users\prvbh\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Assignment_4\source" -I"C:\Users\prvbh\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Assignment_4" -I"C:\Users\prvbh\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Assignment_4\drivers" -I"C:\Users\prvbh\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Assignment_4\CMSIS" -I"C:\Users\prvbh\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Assignment_4\utilities" -I"C:\Users\prvbh\Documents\MCUXpressoIDE_11.2.0_4120\workspace\PES_Assignment_4\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


