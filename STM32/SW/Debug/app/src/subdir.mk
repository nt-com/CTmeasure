################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/src/app.c 

OBJS += \
./app/src/app.o 

C_DEPS += \
./app/src/app.d 


# Each subdirectory must supply rules for building sources it contributes
app/src/%.o: ../app/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F401RETx -DNUCLEO_F401RE -DDEBUG -I"/home/nmt/openSTM32_workspace/CoFeM_MK_1/drivers/inc" -I"/home/nmt/openSTM32_workspace/CoFeM_MK_1/app/inc" -I"/home/nmt/openSTM32_workspace/CoFeM_MK_1/app/src" -I"/home/nmt/openSTM32_workspace/CoFeM_MK_1/drivers/src" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


