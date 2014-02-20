################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../soar/Brick.o \
../soar/ColorSensor.o \
../soar/IRRemote.o \
../soar/Motor.o \
../soar/SoarManager.o \
../soar/TouchSensor.o 

CPP_SRCS += \
../soar/Brick.cpp \
../soar/ColorSensor.cpp \
../soar/IRRemote.cpp \
../soar/Motor.cpp \
../soar/SoarManager.cpp \
../soar/TouchSensor.cpp 

OBJS += \
./soar/Brick.o \
./soar/ColorSensor.o \
./soar/IRRemote.o \
./soar/Motor.o \
./soar/SoarManager.o \
./soar/TouchSensor.o 

CPP_DEPS += \
./soar/Brick.d \
./soar/ColorSensor.d \
./soar/IRRemote.d \
./soar/Motor.d \
./soar/SoarManager.d \
./soar/TouchSensor.d 


# Each subdirectory must supply rules for building sources it contributes
soar/%.o: ../soar/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DENABLE_SOAR -I/home/aaron/research/ev3/include -I/home/aaron/research/ev3/newsoarev3 -I/home/aaron/research/ev3/lcmlite -I/home/aaron/research/soar/out/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


