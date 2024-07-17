################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/src/bsp_button.c \
../bsp/src/bsp_flash.c \
../bsp/src/bsp_hr.c \
../bsp/src/bsp_led.c \
../bsp/src/bsp_rtc.c \
../bsp/src/bsp_sensor.c \
../bsp/src/bsp_vin_detection.c \
../bsp/src/bsp_voice.c \
../bsp/src/bsp_wdg.c 

OBJS += \
./bsp/src/bsp_button.o \
./bsp/src/bsp_flash.o \
./bsp/src/bsp_hr.o \
./bsp/src/bsp_led.o \
./bsp/src/bsp_rtc.o \
./bsp/src/bsp_sensor.o \
./bsp/src/bsp_vin_detection.o \
./bsp/src/bsp_voice.o \
./bsp/src/bsp_wdg.o 

C_DEPS += \
./bsp/src/bsp_button.d \
./bsp/src/bsp_flash.d \
./bsp/src/bsp_hr.d \
./bsp/src/bsp_led.d \
./bsp/src/bsp_rtc.d \
./bsp/src/bsp_sensor.d \
./bsp/src/bsp_vin_detection.d \
./bsp/src/bsp_voice.d \
./bsp/src/bsp_wdg.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/src/%.o: ../bsp/src/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\common\LwRB\inc" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\include\libc" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\common\FlexibleButton-master" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\common\agile_led" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\rt_vsnprintf_full-master" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\Debug" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\common\AT-Command-master\include" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\bsp\inc" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\utilities\ulog" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\utilities\ulog\backend" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\Core" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\User" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\Peripheral\include" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\drivers" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\drivers\include\drivers" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\include" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\libcpu\risc-v" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\libcpu\risc-v\common" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\src" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\drivers\include" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\drivers\misc" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\drivers\serial" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\finsh" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

