################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/futil/general/chrono.cpp \
../src_libs/futil/general/snprintf.cpp 

OBJS += \
./src_libs/futil/general/chrono.o \
./src_libs/futil/general/snprintf.o 

CPP_DEPS += \
./src_libs/futil/general/chrono.d \
./src_libs/futil/general/snprintf.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/futil/general/%.o: ../src_libs/futil/general/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	LC_ALL=C g++ -std=c++98 -I"BUILD_PATH/src_libs" -I"BUILD_PATH/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


