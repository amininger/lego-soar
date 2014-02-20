################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../LcmUtil.o \
../WMUtil.o 

CPP_SRCS += \
../LcmUtil.cpp \
../WMUtil.cpp \
../main.cpp 

OBJS += \
./LcmUtil.o \
./WMUtil.o \
./main.o 

CPP_DEPS += \
./LcmUtil.d \
./WMUtil.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DENABLE_SOAR -I/home/aaron/research/ev3/include -I/home/aaron/research/ev3/newsoarev3 -I/home/aaron/research/ev3/lcmlite -I/home/aaron/research/soar/out/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


