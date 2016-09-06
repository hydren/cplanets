################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SDL_util.cpp \
../src/main.cpp \
../src/main_window.cpp \
../src/planetarium.cpp \
../src/planetarium_pane.cpp \
../src/program_io.cpp 

OBJS += \
./src/SDL_util.o \
./src/main.o \
./src/main_window.o \
./src/planetarium.o \
./src/planetarium_pane.o \
./src/program_io.o 

CPP_DEPS += \
./src/SDL_util.d \
./src/main.d \
./src/main_window.d \
./src/planetarium.d \
./src/planetarium_pane.d \
./src/program_io.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	LC_ALL=C g++ -std=c++98 -I"BUILD_PATH/src_libs" -I"BUILD_PATH/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


