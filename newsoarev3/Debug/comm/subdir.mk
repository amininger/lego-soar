################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../comm/CommStructs.o \
../comm/DirectCommunication.o \
../comm/Ev3Communication.o \
../comm/SoarCommunication.o 

CPP_SRCS += \
../comm/CommStructs.cpp \
../comm/DirectCommunication.cpp \
../comm/Ev3Communication.cpp \
../comm/SoarCommunication.cpp 

OBJS += \
./comm/CommStructs.o \
./comm/DirectCommunication.o \
./comm/Ev3Communication.o \
./comm/SoarCommunication.o 

CPP_DEPS += \
./comm/CommStructs.d \
./comm/DirectCommunication.d \
./comm/Ev3Communication.d \
./comm/SoarCommunication.d 


# Each subdirectory must supply rules for building sources it contributes
comm/%.o: ../comm/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DENABLE_SOAR -I/home/aaron/research/ev3/include -I/home/aaron/research/ev3/newsoarev3 -I/home/aaron/research/ev3/lcmlite -I/home/aaron/research/soar/out/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


