################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src_libs/futil/string/subdir.mk
-include src_libs/futil/math/subdir.mk
-include src_libs/futil/general/subdir.mk
-include src_libs/futil/subdir.mk
-include src_libs/SDL_widgets/subdir.mk
-include src/widgets/extra/subdir.mk
-include src/widgets/subdir.mk
-include src/planetarium/subdir.mk
-include src/physics/solvers/subdir.mk
-include src/physics/subdir.mk
-include src/geometry/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: cplanets

# Tool invocations
cplanets: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -o "cplanets" $(OBJS) $(USER_OBJS) $(LIBS) `sdl-config --cflags --libs`
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS)$(CXX_DEPS)$(OBJS)$(CPP_DEPS)$(C_DEPS) cplanets
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
