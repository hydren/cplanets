################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/physics/body2d.cpp \
../src/physics/physics2d.cpp \
../src/physics/physics2dsolver.cpp \
../src/physics/universe2d.cpp 

OBJS += \
./src/physics/body2d.o \
./src/physics/physics2d.o \
./src/physics/physics2dsolver.o \
./src/physics/universe2d.o 

CPP_DEPS += \
./src/physics/body2d.d \
./src/physics/physics2d.d \
./src/physics/physics2dsolver.d \
./src/physics/universe2d.d 


# Each subdirectory must supply rules for building sources it contributes
src/physics/%.o: ../src/physics/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	LC_ALL=C g++ -std=c++98 -I"BUILD_PATH/src_libs" -I"BUILD_PATH/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


