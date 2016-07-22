################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/physics/solvers/euler.cpp \
../src/physics/solvers/multistep.cpp \
../src/physics/solvers/order2.cpp \
../src/physics/solvers/rk2.cpp \
../src/physics/solvers/rk3.cpp \
../src/physics/solvers/rk4.cpp \
../src/physics/solvers/runge_kutta.cpp 

OBJS += \
./src/physics/solvers/euler.o \
./src/physics/solvers/multistep.o \
./src/physics/solvers/order2.o \
./src/physics/solvers/rk2.o \
./src/physics/solvers/rk3.o \
./src/physics/solvers/rk4.o \
./src/physics/solvers/runge_kutta.o 

CPP_DEPS += \
./src/physics/solvers/euler.d \
./src/physics/solvers/multistep.d \
./src/physics/solvers/order2.d \
./src/physics/solvers/rk2.d \
./src/physics/solvers/rk3.d \
./src/physics/solvers/rk4.d \
./src/physics/solvers/runge_kutta.d 


# Each subdirectory must supply rules for building sources it contributes
src/physics/solvers/%.o: ../src/physics/solvers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	LC_ALL=C g++ -std=c++98 -I"/home/carlosfaruolo/Workspaces/cpp/cplanets/src_libs" -I"/home/carlosfaruolo/Workspaces/cpp/cplanets/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


