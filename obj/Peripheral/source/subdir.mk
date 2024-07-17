################################################################################
# MRS Version: 1.9.1
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Peripheral/source/ch32v30x_adc.c \
../Peripheral/source/ch32v30x_bkp.c \
../Peripheral/source/ch32v30x_can.c \
../Peripheral/source/ch32v30x_crc.c \
../Peripheral/source/ch32v30x_dac.c \
../Peripheral/source/ch32v30x_dbgmcu.c \
../Peripheral/source/ch32v30x_dma.c \
../Peripheral/source/ch32v30x_dvp.c \
../Peripheral/source/ch32v30x_eth.c \
../Peripheral/source/ch32v30x_exti.c \
../Peripheral/source/ch32v30x_flash.c \
../Peripheral/source/ch32v30x_fsmc.c \
../Peripheral/source/ch32v30x_gpio.c \
../Peripheral/source/ch32v30x_i2c.c \
../Peripheral/source/ch32v30x_iwdg.c \
../Peripheral/source/ch32v30x_misc.c \
../Peripheral/source/ch32v30x_opa.c \
../Peripheral/source/ch32v30x_pwr.c \
../Peripheral/source/ch32v30x_rcc.c \
../Peripheral/source/ch32v30x_rng.c \
../Peripheral/source/ch32v30x_rtc.c \
../Peripheral/source/ch32v30x_sdio.c \
../Peripheral/source/ch32v30x_spi.c \
../Peripheral/source/ch32v30x_tim.c \
../Peripheral/source/ch32v30x_usart.c \
../Peripheral/source/ch32v30x_wwdg.c 

OBJS += \
./Peripheral/source/ch32v30x_adc.o \
./Peripheral/source/ch32v30x_bkp.o \
./Peripheral/source/ch32v30x_can.o \
./Peripheral/source/ch32v30x_crc.o \
./Peripheral/source/ch32v30x_dac.o \
./Peripheral/source/ch32v30x_dbgmcu.o \
./Peripheral/source/ch32v30x_dma.o \
./Peripheral/source/ch32v30x_dvp.o \
./Peripheral/source/ch32v30x_eth.o \
./Peripheral/source/ch32v30x_exti.o \
./Peripheral/source/ch32v30x_flash.o \
./Peripheral/source/ch32v30x_fsmc.o \
./Peripheral/source/ch32v30x_gpio.o \
./Peripheral/source/ch32v30x_i2c.o \
./Peripheral/source/ch32v30x_iwdg.o \
./Peripheral/source/ch32v30x_misc.o \
./Peripheral/source/ch32v30x_opa.o \
./Peripheral/source/ch32v30x_pwr.o \
./Peripheral/source/ch32v30x_rcc.o \
./Peripheral/source/ch32v30x_rng.o \
./Peripheral/source/ch32v30x_rtc.o \
./Peripheral/source/ch32v30x_sdio.o \
./Peripheral/source/ch32v30x_spi.o \
./Peripheral/source/ch32v30x_tim.o \
./Peripheral/source/ch32v30x_usart.o \
./Peripheral/source/ch32v30x_wwdg.o 

C_DEPS += \
./Peripheral/source/ch32v30x_adc.d \
./Peripheral/source/ch32v30x_bkp.d \
./Peripheral/source/ch32v30x_can.d \
./Peripheral/source/ch32v30x_crc.d \
./Peripheral/source/ch32v30x_dac.d \
./Peripheral/source/ch32v30x_dbgmcu.d \
./Peripheral/source/ch32v30x_dma.d \
./Peripheral/source/ch32v30x_dvp.d \
./Peripheral/source/ch32v30x_eth.d \
./Peripheral/source/ch32v30x_exti.d \
./Peripheral/source/ch32v30x_flash.d \
./Peripheral/source/ch32v30x_fsmc.d \
./Peripheral/source/ch32v30x_gpio.d \
./Peripheral/source/ch32v30x_i2c.d \
./Peripheral/source/ch32v30x_iwdg.d \
./Peripheral/source/ch32v30x_misc.d \
./Peripheral/source/ch32v30x_opa.d \
./Peripheral/source/ch32v30x_pwr.d \
./Peripheral/source/ch32v30x_rcc.d \
./Peripheral/source/ch32v30x_rng.d \
./Peripheral/source/ch32v30x_rtc.d \
./Peripheral/source/ch32v30x_sdio.d \
./Peripheral/source/ch32v30x_spi.d \
./Peripheral/source/ch32v30x_tim.d \
./Peripheral/source/ch32v30x_usart.d \
./Peripheral/source/ch32v30x_wwdg.d 


# Each subdirectory must supply rules for building sources it contributes
Peripheral/source/%.o: ../Peripheral/source/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\LwRB\inc" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\include\libc" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\FlexibleButton-master" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\agile_led" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\rt_vsnprintf_full-master" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\Debug" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\common\AT-Command-master\include" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\bsp\inc" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\utilities\ulog" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\utilities\ulog\backend" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\Core" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\User" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\Peripheral\include" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\drivers" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\drivers\include\drivers" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\include" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\libcpu\risc-v" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\libcpu\risc-v\common" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\src" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\drivers\include" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\drivers\misc" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\drivers\serial" -I"F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\rtthread\components\finsh" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

