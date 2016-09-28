################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/planetarium/planetarium.cpp \
../src/planetarium/planetarium_pane.cpp 

OBJS += \
./src/planetarium/planetarium.o \
./src/planetarium/planetarium_pane.o 

CPP_DEPS += \
./src/planetarium/planetarium.d \
./src/planetarium/planetarium_pane.d 


# Each subdirectory must supply rules for building sources it contributes
src/planetarium/%.o: ../src/planetarium/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	LC_ALL=C g++ -std=c++98 -I"BUILD_PATH/src_libs" -I"BUILD_PATH/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


