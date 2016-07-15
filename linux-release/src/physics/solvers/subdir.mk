################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/physics/solvers/classic_rk4_solver.cpp \
../src/physics/solvers/euler_cromer_solver.cpp \
../src/physics/solvers/euler_solver.cpp \
../src/physics/solvers/heun_solver.cpp \
../src/physics/solvers/leapfrog_solver.cpp \
../src/physics/solvers/midpoint_solver.cpp \
../src/physics/solvers/semi_implicit_euler_solver.cpp \
../src/physics/solvers/stormer_verlet_solver.cpp \
../src/physics/solvers/velocity_verlet_solver.cpp 

OBJS += \
./src/physics/solvers/classic_rk4_solver.o \
./src/physics/solvers/euler_cromer_solver.o \
./src/physics/solvers/euler_solver.o \
./src/physics/solvers/heun_solver.o \
./src/physics/solvers/leapfrog_solver.o \
./src/physics/solvers/midpoint_solver.o \
./src/physics/solvers/semi_implicit_euler_solver.o \
./src/physics/solvers/stormer_verlet_solver.o \
./src/physics/solvers/velocity_verlet_solver.o 

CPP_DEPS += \
./src/physics/solvers/classic_rk4_solver.d \
./src/physics/solvers/euler_cromer_solver.d \
./src/physics/solvers/euler_solver.d \
./src/physics/solvers/heun_solver.d \
./src/physics/solvers/leapfrog_solver.d \
./src/physics/solvers/midpoint_solver.d \
./src/physics/solvers/semi_implicit_euler_solver.d \
./src/physics/solvers/stormer_verlet_solver.d \
./src/physics/solvers/velocity_verlet_solver.d 


# Each subdirectory must supply rules for building sources it contributes
src/physics/solvers/%.o: ../src/physics/solvers/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	LC_ALL=C g++ -std=c++98 -I"/home/carlosfaruolo/Workspaces/cpp/cplanets/src_libs" -I"/home/carlosfaruolo/Workspaces/cpp/cplanets/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


