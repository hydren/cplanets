################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src_libs/futil/futil_exception.cpp \
../src_libs/futil/futil_general.cpp \
../src_libs/futil/futil_math.cpp \
../src_libs/futil/futil_string.cpp 

OBJS += \
./src_libs/futil/futil_exception.o \
./src_libs/futil/futil_general.o \
./src_libs/futil/futil_math.o \
./src_libs/futil/futil_string.o 

CPP_DEPS += \
./src_libs/futil/futil_exception.d \
./src_libs/futil/futil_general.d \
./src_libs/futil/futil_math.d \
./src_libs/futil/futil_string.d 


# Each subdirectory must supply rules for building sources it contributes
src_libs/futil/%.o: ../src_libs/futil/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	LC_ALL=C g++ -std=c++98 -I"BUILD_PATH/src_libs" -I"BUILD_PATH/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


