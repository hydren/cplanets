################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/widgets/abstract_layout.cpp \
../src/widgets/dialog_bgrwin.cpp \
../src/widgets/drop_menu.cpp \
../src/widgets/file_dialog.cpp \
../src/widgets/flow_layout.cpp \
../src/widgets/label_win.cpp \
../src/widgets/list_selection_model.cpp \
../src/widgets/list_win.cpp \
../src/widgets/scrollable_pane.cpp \
../src/widgets/spinner.cpp \
../src/widgets/tab_control.cpp \
../src/widgets/tab_set.cpp \
../src/widgets/toogle_button.cpp \
../src/widgets/value_shower.cpp \
../src/widgets/widgets_debug.cpp \
../src/widgets/widgets_util.cpp 

OBJS += \
./src/widgets/abstract_layout.o \
./src/widgets/dialog_bgrwin.o \
./src/widgets/drop_menu.o \
./src/widgets/file_dialog.o \
./src/widgets/flow_layout.o \
./src/widgets/label_win.o \
./src/widgets/list_selection_model.o \
./src/widgets/list_win.o \
./src/widgets/scrollable_pane.o \
./src/widgets/spinner.o \
./src/widgets/tab_control.o \
./src/widgets/tab_set.o \
./src/widgets/toogle_button.o \
./src/widgets/value_shower.o \
./src/widgets/widgets_debug.o \
./src/widgets/widgets_util.o 

CPP_DEPS += \
./src/widgets/abstract_layout.d \
./src/widgets/dialog_bgrwin.d \
./src/widgets/drop_menu.d \
./src/widgets/file_dialog.d \
./src/widgets/flow_layout.d \
./src/widgets/label_win.d \
./src/widgets/list_selection_model.d \
./src/widgets/list_win.d \
./src/widgets/scrollable_pane.d \
./src/widgets/spinner.d \
./src/widgets/tab_control.d \
./src/widgets/tab_set.d \
./src/widgets/toogle_button.d \
./src/widgets/value_shower.d \
./src/widgets/widgets_debug.d \
./src/widgets/widgets_util.d 


# Each subdirectory must supply rules for building sources it contributes
src/widgets/%.o: ../src/widgets/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	LC_ALL=C g++ -std=c++98 -I"/home/carlosfaruolo/Workspaces/cpp/cplanets/src_libs" -I"/home/carlosfaruolo/Workspaces/cpp/cplanets/src" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


