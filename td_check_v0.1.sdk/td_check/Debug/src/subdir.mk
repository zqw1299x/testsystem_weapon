################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/common.c \
../src/fc1553.c \
../src/irqprocess.c \
../src/ls1553.c \
../src/main.c \
../src/platform.c \
../src/timerInit.c \
../src/uart.c 

OBJS += \
./src/common.o \
./src/fc1553.o \
./src/irqprocess.o \
./src/ls1553.o \
./src/main.o \
./src/platform.o \
./src/timerInit.o \
./src/uart.o 

C_DEPS += \
./src/common.d \
./src/fc1553.d \
./src/irqprocess.d \
./src/ls1553.d \
./src/main.d \
./src/platform.d \
./src/timerInit.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM v7 gcc compiler'
	arm-none-eabi-gcc -Wall -O0 -g3 -I../../td_check_bsp/ps7_cortexa9_0/include -I"D:\WorkFile\206Proj_20230105\1553_rt_v0331_0089_night_ok\prj\td_check_v0.1\td_check_v0.1.sdk\td_check\src" -I"D:\WorkFile\206Proj_20230105\1553_rt_v0331_0089_night_ok\prj\td_check_v0.1\td_check_v0.1.sdk\td_check\src\fc1553lib" -c -fmessage-length=0 -MT"$@" -mcpu=cortex-a9 -mfpu=vfpv3 -mfloat-abi=hard -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


