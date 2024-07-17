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
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\common\LwRB\inc" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\include\libc" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\common\FlexibleButton-master" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\common\agile_led" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\rt_vsnprintf_full-master" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\Debug" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\common\AT-Command-master\include" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\bsp\inc" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\utilities\ulog" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\utilities\ulog\backend" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\Core" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\User" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\Peripheral\include" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\drivers" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\drivers\include\drivers" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\include" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\libcpu\risc-v" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\libcpu\risc-v\common" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\src" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\drivers\include" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\drivers\misc" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\drivers\serial" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\finsh" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

