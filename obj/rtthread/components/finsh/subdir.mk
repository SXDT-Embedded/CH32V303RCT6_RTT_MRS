################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rtthread/components/finsh/cmd.c \
../rtthread/components/finsh/msh.c \
../rtthread/components/finsh/msh_cmd.c \
../rtthread/components/finsh/msh_file.c \
../rtthread/components/finsh/shell.c \
../rtthread/components/finsh/symbol.c 

OBJS += \
./rtthread/components/finsh/cmd.o \
./rtthread/components/finsh/msh.o \
./rtthread/components/finsh/msh_cmd.o \
./rtthread/components/finsh/msh_file.o \
./rtthread/components/finsh/shell.o \
./rtthread/components/finsh/symbol.o 

C_DEPS += \
./rtthread/components/finsh/cmd.d \
./rtthread/components/finsh/msh.d \
./rtthread/components/finsh/msh_cmd.d \
./rtthread/components/finsh/msh_file.d \
./rtthread/components/finsh/shell.d \
./rtthread/components/finsh/symbol.d 


# Each subdirectory must supply rules for building sources it contributes
rtthread/components/finsh/%.o: ../rtthread/components/finsh/%.c
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\common\LwRB\inc" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\include\libc" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\common\FlexibleButton-master" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\common\agile_led" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\rt_vsnprintf_full-master" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\Debug" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\common\AT-Command-master\include" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\bsp\inc" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\utilities\ulog" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\utilities\ulog\backend" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\Core" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\User" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\Peripheral\include" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\drivers" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\drivers\include\drivers" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\include" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\libcpu\risc-v" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\libcpu\risc-v\common" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\src" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\drivers\include" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\drivers\misc" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\drivers\serial" -I"C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\rtthread\components\finsh" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

