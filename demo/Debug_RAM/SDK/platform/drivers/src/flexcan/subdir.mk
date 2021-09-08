################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/NXP/S32DS_Power_v2.1/S32DS/software/S32_SDK_S32PA_RTM_3.0.0/platform/drivers/src/flexcan/flexcan_driver.c \
C:/NXP/S32DS_Power_v2.1/S32DS/software/S32_SDK_S32PA_RTM_3.0.0/platform/drivers/src/flexcan/flexcan_hw_access.c \
C:/NXP/S32DS_Power_v2.1/S32DS/software/S32_SDK_S32PA_RTM_3.0.0/platform/drivers/src/flexcan/flexcan_irq.c 

OBJS += \
./SDK/platform/drivers/src/flexcan/flexcan_driver.o \
./SDK/platform/drivers/src/flexcan/flexcan_hw_access.o \
./SDK/platform/drivers/src/flexcan/flexcan_irq.o 

C_DEPS += \
./SDK/platform/drivers/src/flexcan/flexcan_driver.d \
./SDK/platform/drivers/src/flexcan/flexcan_hw_access.d \
./SDK/platform/drivers/src/flexcan/flexcan_irq.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/drivers/src/flexcan/flexcan_driver.o: C:/NXP/S32DS_Power_v2.1/S32DS/software/S32_SDK_S32PA_RTM_3.0.0/platform/drivers/src/flexcan/flexcan_driver.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/flexcan/flexcan_driver.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/flexcan/flexcan_hw_access.o: C:/NXP/S32DS_Power_v2.1/S32DS/software/S32_SDK_S32PA_RTM_3.0.0/platform/drivers/src/flexcan/flexcan_hw_access.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/flexcan/flexcan_hw_access.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SDK/platform/drivers/src/flexcan/flexcan_irq.o: C:/NXP/S32DS_Power_v2.1/S32DS/software/S32_SDK_S32PA_RTM_3.0.0/platform/drivers/src/flexcan/flexcan_irq.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/drivers/src/flexcan/flexcan_irq.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


