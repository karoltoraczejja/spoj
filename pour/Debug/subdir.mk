################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../p1wrk.cpp \
../por.cpp \
../pour.cpp 

CPP_DEPS += \
./p1wrk.d \
./por.d \
./pour.d 

OBJS += \
./p1wrk.o \
./por.o \
./pour.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./p1wrk.d ./p1wrk.o ./por.d ./por.o ./pour.d ./pour.o

.PHONY: clean--2e-

