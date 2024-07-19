################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bsp/src/bsp_button.c \
../bsp/src/bsp_flash.c \
../bsp/src/bsp_hr.c \
../bsp/src/bsp_lcd_1621.c \
../bsp/src/bsp_led.c \
../bsp/src/bsp_rtc.c \
../bsp/src/bsp_vin_detection.c \
../bsp/src/bsp_wdg.c 

OBJS += \
./bsp/src/bsp_button.o \
./bsp/src/bsp_flash.o \
./bsp/src/bsp_hr.o \
./bsp/src/bsp_lcd_1621.o \
./bsp/src/bsp_led.o \
./bsp/src/bsp_rtc.o \
./bsp/src/bsp_vin_detection.o \
./bsp/src/bsp_wdg.o 

C_DEPS += \
./bsp/src/bsp_button.d \
./bsp/src/bsp_flash.d \
./bsp/src/bsp_hr.d \
./bsp/src/bsp_lcd_1621.d \
./bsp/src/bsp_led.d \
./bsp/src/bsp_rtc.d \
./bsp/src/bsp_vin_detection.d \
./bsp/src/bsp_wdg.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/src/%.o: ../bsp/src/%.c
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\LwRB" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\LwUtil" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\net\at" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\net\at\include" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\Peripheral\inc" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\include\libc" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\FlexibleButton" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\agile_led" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\rt_vsnprintf_full" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\Debug" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\AT Command" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\bsp\inc" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\utilities\ulog" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\utilities\ulog\backend" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\Core" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\User" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\drivers" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\drivers\include\drivers" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\include" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\libcpu\risc-v" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\libcpu\risc-v\common" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\src" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\drivers\include" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\drivers\misc" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\drivers\serial" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\finsh" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

