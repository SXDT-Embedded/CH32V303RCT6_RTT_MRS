################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common/LwRB/lwrb.c \
../common/LwRB/lwutil.c 

OBJS += \
./common/LwRB/lwrb.o \
./common/LwRB/lwutil.o 

C_DEPS += \
./common/LwRB/lwrb.d \
./common/LwRB/lwutil.d 


# Each subdirectory must supply rules for building sources it contributes
common/LwRB/%.o: ../common/LwRB/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\LwRB\inc" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\include\libc" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\FlexibleButton-master" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\agile_led" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\rt_vsnprintf_full-master" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\Debug" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\AT-Command-master\include" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\bsp\inc" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\utilities\ulog" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\utilities\ulog\backend" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\Core" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\User" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\Peripheral\include" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\drivers" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\drivers\include\drivers" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\include" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\libcpu\risc-v" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\libcpu\risc-v\common" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\src" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\drivers\include" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\drivers\misc" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\drivers\serial" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\finsh" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

