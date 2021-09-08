################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/CanIf.c \
../src/Com.c \
../src/PduR_CanIf.c \
../src/PduR_Com.c \
../src/PduR_SecOC.c \
../src/SecOC.c \
../src/SecOC_Cfg.c 

OBJS += \
./src/CanIf.o \
./src/Com.o \
./src/PduR_CanIf.o \
./src/PduR_Com.o \
./src/PduR_SecOC.o \
./src/SecOC.o \
./src/SecOC_Cfg.o 

C_DEPS += \
./src/CanIf.d \
./src/Com.d \
./src/PduR_CanIf.d \
./src/PduR_Com.d \
./src/PduR_SecOC.d \
./src/SecOC.d \
./src/SecOC_Cfg.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	powerpc-eabivle-gcc "@src/CanIf.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


