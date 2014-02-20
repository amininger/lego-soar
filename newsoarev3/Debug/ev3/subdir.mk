################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../ev3/Ev3Brick.o \
../ev3/Ev3Manager.o \
../ev3/InputDeviceManager.o \
../ev3/OutputDeviceManager.o 

CPP_SRCS += \
../ev3/Ev3Brick.cpp \
../ev3/Ev3Manager.cpp \
../ev3/InputDeviceManager.cpp \
../ev3/OutputDeviceManager.cpp 

OBJS += \
./ev3/Ev3Brick.o \
./ev3/Ev3Manager.o \
./ev3/InputDeviceManager.o \
./ev3/OutputDeviceManager.o 

CPP_DEPS += \
./ev3/Ev3Brick.d \
./ev3/Ev3Manager.d \
./ev3/InputDeviceManager.d \
./ev3/OutputDeviceManager.d 


# Each subdirectory must supply rules for building sources it contributes
ev3/%.o: ../ev3/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DENABLE_SOAR -I/home/aaron/research/ev3/include -I/home/aaron/research/ev3/newsoarev3 -I/home/aaron/research/ev3/lcmlite -I/home/aaron/research/soar/out/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


