################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/widgets/extra/generic_list_win.cpp \
../src/widgets/extra/generic_spinner.cpp \
../src/widgets/extra/list_model.cpp 

OBJS += \
./src/widgets/extra/generic_list_win.o \
./src/widgets/extra/generic_spinner.o \
./src/widgets/extra/list_model.o 

CPP_DEPS += \
./src/widgets/extra/generic_list_win.d \
./src/widgets/extra/generic_spinner.d \
./src/widgets/extra/list_model.d 


# Each subdirectory must supply rules for building sources it contributes
src/widgets/extra/%.o: ../src/widgets/extra/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	LC_ALL=C g++ -std=c++98 -I"BUILD_PATH/src_libs" -I"BUILD_PATH/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


