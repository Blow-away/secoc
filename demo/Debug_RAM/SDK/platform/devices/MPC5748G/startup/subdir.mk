################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/NXP/S32DS_Power_v2.1/S32DS/software/S32_SDK_S32PA_RTM_3.0.0/platform/devices/MPC5748G/startup/system_MPC5748G.c 

OBJS += \
./SDK/platform/devices/MPC5748G/startup/system_MPC5748G.o 

C_DEPS += \
./SDK/platform/devices/MPC5748G/startup/system_MPC5748G.d 


# Each subdirectory must supply rules for building sources it contributes
SDK/platform/devices/MPC5748G/startup/system_MPC5748G.o: C:/NXP/S32DS_Power_v2.1/S32DS/software/S32_SDK_S32PA_RTM_3.0.0/platform/devices/MPC5748G/startup/system_MPC5748G.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@SDK/platform/devices/MPC5748G/startup/system_MPC5748G.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


