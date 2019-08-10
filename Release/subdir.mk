################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PointRewardSystem.cpp \
../PuyoChallengeSystem.cpp \
../SoundCues_win.cpp \
../VGPointReward.cpp 

OBJS += \
./PointRewardSystem.o \
./PuyoChallengeSystem.o \
./SoundCues_win.o \
./VGPointReward.o 

CPP_DEPS += \
./PointRewardSystem.d \
./PuyoChallengeSystem.d \
./SoundCues_win.d \
./VGPointReward.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


