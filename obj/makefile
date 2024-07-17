################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include rtthread/src/subdir.mk
-include rtthread/libcpu/risc-v/common/subdir.mk
-include rtthread/components/utilities/ulog/backend/subdir.mk
-include rtthread/components/utilities/ulog/subdir.mk
-include rtthread/components/rt_vsnprintf_full-master/subdir.mk
-include rtthread/components/finsh/subdir.mk
-include rtthread/components/drivers/serial/subdir.mk
-include rtthread/components/drivers/misc/subdir.mk
-include rtthread/subdir.mk
-include drivers/subdir.mk
-include common/agile_led/subdir.mk
-include common/LwRB/subdir.mk
-include common/FlexibleButton-master/subdir.mk
-include common/AT-Command-master/src/subdir.mk
-include bsp/src/subdir.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/source/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
MY_RTT_CH32V303_DEMO.hex \

SECONDARY_LIST += \
MY_RTT_CH32V303_DEMO.lst \

SECONDARY_SIZE += \
MY_RTT_CH32V303_DEMO.siz \


# 所有目标
all: MY_RTT_CH32V303_DEMO.elf secondary-outputs

# 工具调用
MY_RTT_CH32V303_DEMO.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "C:\Users\16005\Desktop\JT-DT-BX4C01_RTT-Nano-mbw_rtt\JT-DT-BX4C01_RTT-Nano-mbw_rtt\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Xlinker --print-memory-usage -Wl,-Map,"MY_RTT_CH32V303_DEMO.map" --specs=nano.specs -u _scanf_float --specs=nosys.specs  -o "MY_RTT_CH32V303_DEMO.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lprintfloat
	@	@
MY_RTT_CH32V303_DEMO.hex: MY_RTT_CH32V303_DEMO.elf
	@	riscv-none-embed-objcopy -O ihex "MY_RTT_CH32V303_DEMO.elf"  "MY_RTT_CH32V303_DEMO.hex"
	@	@
MY_RTT_CH32V303_DEMO.lst: MY_RTT_CH32V303_DEMO.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "MY_RTT_CH32V303_DEMO.elf" > "MY_RTT_CH32V303_DEMO.lst"
	@	@
MY_RTT_CH32V303_DEMO.siz: MY_RTT_CH32V303_DEMO.elf
	@	riscv-none-embed-size --format=berkeley "MY_RTT_CH32V303_DEMO.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) MY_RTT_CH32V303_DEMO.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
