################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/futil/exception.cpp 

OBJS += \
./src_libs/futil/exception.o 

CPP_DEPS += \
./src_libs/futil/exception.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/futil/%.o: ../src_libs/futil/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	LC_ALL=C g++ -std=c++98 -I"BUILD_PATH/src_libs" -I"BUILD_PATH/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


